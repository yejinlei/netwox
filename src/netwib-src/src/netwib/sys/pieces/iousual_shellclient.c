/* This file is part of Netwib. Read and honor its license.
*/

#if NETWIBDEF_IOSHELL_SUPPORTED == 1

/*-------------------------------------------------------------*/
typedef struct {
  struct termios tio;
  int readfd;
  int writefd;
  netwib_time timenextkeepalive;
} netwib_priv_io_shellclient;

/*-----------------------------------------------------------------------*/
/* set to true to send on session initialization */
static netwib_bool netwib_priv_io_shellclient_windowsize_recv = NETWIB_TRUE;
static void netwib_priv_io_shellclient_windowsize_handler(int sig)
{
  netwib_priv_io_shellclient_windowsize_recv = NETWIB_TRUE;
  sig = sig;
}
static netwib_err netwib_priv_io_shellclient_windowsize(netwib_buf *pbuf)
{
  netwib_byte array[2+16];
  netwib_data data;
  struct winsize ws;
  int reti;

  /* obtains information */
  reti = ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);
  if (reti < 0) {
    return(NETWIB_ERR_FUIOCTL);
  }

  /* construct message */
  data = array;
  netwib__data_append_uint8(data, NETWIB_PRIV_IO_SHELL_MSG_SEPARATOR);
  netwib__data_append_uint8(data, NETWIB_PRIV_IO_SHELL_MSG_WINDOWSIZE);
  netwib__data_append_uint32(data, ws.ws_row);
  netwib__data_append_uint32(data, ws.ws_col);
  netwib__data_append_uint32(data, ws.ws_xpixel);
  netwib__data_append_uint32(data, ws.ws_ypixel);
  netwib_er(netwib_buf_append_data(array, sizeof(array), pbuf));

  return(NETWIB_ERR_OK);
}

/*-----------------------------------------------------------------------*/
#define NETWIB_PRIV_IO_SHELLCLIENT_KEEPALIVE_INTERVAL 300
static netwib_err netwib_priv_io_shellclient_keepalive_updatetime(netwib_priv_io_shellclient *ptr)
{
  netwib_er(netwib_time_init_now(&ptr->timenextkeepalive));
  netwib_er(netwib_time_plus_sec(&ptr->timenextkeepalive, NETWIB_PRIV_IO_SHELLCLIENT_KEEPALIVE_INTERVAL));
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_io_shellclient_keepalive(netwib_priv_io_shellclient *ptr,
                                                       netwib_buf *pbuf)
{
  netwib_byte array[2];
  netwib_data data;

  /* construct message */
  data = array;
  netwib__data_append_uint8(data, NETWIB_PRIV_IO_SHELL_MSG_SEPARATOR);
  netwib__data_append_uint8(data, NETWIB_PRIV_IO_SHELL_MSG_KEEPALIVE);
  netwib_er(netwib_buf_append_data(array, sizeof(array), pbuf));

  netwib_er(netwib_priv_io_shellclient_keepalive_updatetime(ptr));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_shellclient_init(netwib_priv_io_shellclient *ptr)
{
  struct termios tio;
  netwib_ptr signalret;
  int reti;

  ptr->readfd = STDIN_FILENO;
  ptr->writefd = STDOUT_FILENO;

  /* change configuration */
  reti = tcgetattr(ptr->readfd, &tio);
  if (reti == -1) return(NETWIB_ERR_FUTCGETATTR);
  ptr->tio = tio;
  /* tcsetattr manpage is not very helpful, so I'm not really sure for these*/
  tio.c_iflag &= ~IGNBRK;
  tio.c_iflag &= ~BRKINT;
  tio.c_iflag &= ~IGNPAR;
  tio.c_iflag &= ~INPCK;
  tio.c_iflag &= ~ISTRIP;
  tio.c_iflag &= ~INLCR;
  tio.c_iflag &= ~IGNCR;
  tio.c_iflag &= ~ICRNL;
  tio.c_iflag &= ~IUCLC;
  tio.c_iflag &= ~IXON;
  tio.c_iflag &= ~IXANY;
  tio.c_iflag &= ~IXOFF;
  tio.c_iflag &= ~IMAXBEL;
  tio.c_oflag &= ~OPOST;
  tio.c_oflag &= ~OLCUC;
  tio.c_oflag &= ~ONLCR;
  tio.c_oflag &= ~OCRNL;
  tio.c_oflag &= ~ONOCR;
  tio.c_oflag &= ~ONLRET;
  tio.c_oflag &= ~OFILL;
  tio.c_lflag &= ~ISIG;
  tio.c_lflag &= ~ICANON;
  tio.c_lflag &= ~ECHO;
  tio.c_lflag &= ~IEXTEN;
  tio.c_cc[VMIN] = 1;
  tio.c_cc[VTIME] = 0;
  reti = tcsetattr(ptr->readfd, TCSADRAIN, &tio);
  if (reti == -1) return(NETWIB_ERR_FUTCSETATTR);
  /* there is no need to also set ptr->writefd because they are linked */

  /* prepare for window size handling */
  signalret = signal(SIGWINCH, &netwib_priv_io_shellclient_windowsize_handler);
  if (signalret == SIG_ERR) return(NETWIB_ERR_FUSIGNAL);

  /* prepare for keep alive messages */
  netwib_er(netwib_priv_io_shellclient_keepalive_updatetime(ptr));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_shellclient_close(netwib_priv_io_shellclient *ptr)
{
  netwib_ptr signalret;
  int reti;

  /* restore configuration */
  reti = tcsetattr(ptr->readfd, TCSADRAIN, &ptr->tio);
  if (reti == -1) return(NETWIB_ERR_FUTCSETATTR);

  signalret = signal(SIGWINCH, SIG_DFL);
  if (signalret == SIG_ERR) return(NETWIB_ERR_FUSIGNAL);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_shellclient_read(netwib_io *pio,
                                                  netwib_buf *pbuf)
{
  netwib_priv_io_shellclient *ptr = (netwib_priv_io_shellclient *)pio->pcommon;
  netwib_buf buf;
  netwib_data data, pc;
  netwib_time timenow;
  netwib_uint32 datasize, chunksize;
  netwib_cmp cmp;
  netwib_err ret;

  /* give a windowsize message */
  if (netwib_priv_io_shellclient_windowsize_recv) {
    netwib_er(netwib_priv_io_shellclient_windowsize(pbuf));
    netwib_priv_io_shellclient_windowsize_recv = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  }

  /* check if it's keep alive time */
  netwib_er(netwib_time_init_now(&timenow));
  netwib_er(netwib_time_cmp(&ptr->timenextkeepalive, &timenow, &cmp));
  if (cmp == NETWIB_CMP_LT) {
    netwib_er(netwib_priv_io_shellclient_keepalive(ptr, pbuf));
    return(NETWIB_ERR_OK);
  }
  /* now, as the read is blocking, we might not send a keep alive
     if user does not press any key. For it to work, we have to use the
     io_wait (netwib_priv_io_shellclient_wait).
  */

  /* normal data, but repeating the separator */
  netwib_er(netwib_buf_init_mallocdefault(&buf))
  ret = netwib_priv_fd_read(ptr->readfd, &buf);
  if (ret == NETWIB_ERR_OK) {
    data = netwib__buf_ref_data_ptr(&buf);
    datasize = netwib__buf_ref_data_size(&buf);
    if (datasize) {
      netwib_er(netwib_priv_io_shellclient_keepalive_updatetime(ptr));
    }
    while (datasize) {
      pc = netwib_c_memchr(data, NETWIB_PRIV_IO_SHELL_MSG_SEPARATOR, datasize);
      if (pc == NULL) {
        netwib_er(netwib_buf_append_data(data, datasize, pbuf));
        break;
      } else {
        chunksize = pc - data + 1;
        netwib_er(netwib_buf_append_data(data, chunksize, pbuf));
        netwib_er(netwib_buf_append_byte(NETWIB_PRIV_IO_SHELL_MSG_SEPARATOR,
                                         pbuf));
        data += chunksize;
        datasize -= chunksize;
      }
    }
  }
  netwib_er(netwib_buf_close(&buf))

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_shellclient_write(netwib_io *pio,
                                                   netwib_constbuf *pbuf)
{
  netwib_priv_io_shellclient *ptr = (netwib_priv_io_shellclient *)pio->pcommon;

  netwib_er(netwib_priv_fd_write(ptr->writefd, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_shellclient_wait(netwib_io *pio,
                                                  netwib_io_waytype way,
                                                  netwib_consttime *pabstime,
                                                  netwib_bool *pevent)
{
  netwib_priv_io_shellclient *ptr = (netwib_priv_io_shellclient *)pio->pcommon;
  netwib_time timenow;
  netwib_cmp cmp;

  if (way == NETWIB_IO_WAYTYPE_SUPPORTED) {
    return(NETWIB_ERR_PLEASECONSTRUCT);
  } else if (way == NETWIB_IO_WAYTYPE_READ) {
    if (netwib_priv_io_shellclient_windowsize_recv) {
      if (pevent != NULL) *pevent = NETWIB_TRUE;
    } else {
      netwib_er(netwib_time_cmp(&ptr->timenextkeepalive, pabstime, &cmp));
      if (cmp == NETWIB_CMP_LT) {
        netwib_er(netwib_priv_fd_wait(ptr->readfd, way, &ptr->timenextkeepalive, NULL));
        if (pevent != NULL) *pevent = NETWIB_TRUE;
      } else {
        netwib_er(netwib_time_init_now(&timenow));
        netwib_er(netwib_time_cmp(&ptr->timenextkeepalive, &timenow, &cmp));
        if (cmp == NETWIB_CMP_LT) {
          if (pevent != NULL) *pevent = NETWIB_TRUE;
        } else {
          netwib_er(netwib_priv_fd_wait(ptr->readfd, way, pabstime, pevent));
        }
      }
    }
  } else if (way == NETWIB_IO_WAYTYPE_WRITE) {
      netwib_er(netwib_priv_fd_wait(ptr->writefd, way, pabstime, pevent));
  } else {
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_shellclient_ctl_set(netwib_io *pio,
                                                     netwib_io_waytype way,
                                                     netwib_io_ctltype type,
                                                     netwib_ptr p,
                                                     netwib_uint32 ui)
{
  switch(type) {
  case NETWIB_IO_CTLTYPE_RES:
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  case NETWIB_IO_CTLTYPE_END:
    netwib_er(netwib_io_ctl_set_support(pio, way, NETWIB_FALSE));
    return(NETWIB_ERR_OK);
    break;
  default:
    return(NETWIB_ERR_PLEASETRYNEXT);
  }

  p = p; /* for compiler warning */
  ui = ui; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_shellclient_ctl_get(netwib_io *pio,
                                                     netwib_io_waytype way,
                                                     netwib_io_ctltype type,
                                                     netwib_ptr p,
                                                     netwib_uint32 *pui)
{

  switch(type) {
  case NETWIB_IO_CTLTYPE_RES:
    if (pui != NULL) *pui = NETWIB_IO_RESTYPE_SHELLCLIENT;
    return(NETWIB_ERR_OK);
  case NETWIB_IO_CTLTYPE_END:
    return(NETWIB_ERR_PAINVALIDTYPE);
  default:
    return(NETWIB_ERR_PLEASETRYNEXT);
  }

  pio = pio; /* for compiler warning */
  way = way; /* for compiler warning */
  p = p; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_shellclient_fclose(netwib_io *pio)
{
  netwib_priv_io_shellclient *ptr = (netwib_priv_io_shellclient *)pio->pcommon;

  netwib_er(netwib_priv_io_shellclient_close(ptr));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

#endif

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_shellclient(netwib_io **ppio)
{
#if NETWIBDEF_IOSHELL_SUPPORTED == 1
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_shellclient), &pcommon));
  ret = netwib_priv_io_shellclient_init((netwib_priv_io_shellclient *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(NETWIB_TRUE, NETWIB_TRUE,
                           pcommon,
                           &netwib_priv_io_shellclient_read,
                           &netwib_priv_io_shellclient_write,
                           &netwib_priv_io_shellclient_wait,
                           NULL,
                           &netwib_priv_io_shellclient_ctl_set,
                           &netwib_priv_io_shellclient_ctl_get,
                           &netwib_priv_io_shellclient_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
#else
  ppio = ppio; /* for compiler warning */
  return(NETWIB_ERR_LONOTIMPLEMENTED);
#endif
}

/*-------------------------------------------------------------*/
netwib_err netwib_shellclient_term(netwib_bufext *pbufterm)
{
  netwib_string term;

  term = getenv("TERM");
  if (term != NULL) {
    netwib_er(netwib_buf_init_ext_string(term, pbufterm));
  } else {
    netwib_er(netwib_buf_init_ext_empty(pbufterm));
  }

  return(NETWIB_ERR_OK);
}
