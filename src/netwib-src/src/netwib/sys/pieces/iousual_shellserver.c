/* This file is part of Netwib. Read and honor its license.
*/

#if NETWIBDEF_IOSHELL_SUPPORTED == 1

/*-------------------------------------------------------------*/
/* those defines are here because I have a script checking if
 * netwib contains MT-unsafe functions. Like this, they are ignored. */
#define netwib_getpwuid getpwuid
#define netwib_ttyname ttyname

/*-------------------------------------------------------------*/
typedef struct {
  netwib_priv_io_execcommon common;
  netwib_buf bufwrite;
  int ptymaster;
} netwib_priv_io_shellserver;

/*-------------------------------------------------------------*/
#define NETWIB_PRIV_IO_SHELL_MSG_SEPARATOR 0xFF
#define NETWIB_PRIV_IO_SHELL_MSG_WINDOWSIZE 0xFE
#define NETWIB_PRIV_IO_SHELL_MSG_KEEPALIVE 0xFD

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_shellserver_msg_analyze(netwib_priv_io_shellserver *ptr,
                                                         netwib_bool *pneedmoredata)
{
  struct winsize ws;
  netwib_buf buf;
  netwib_data data;
  netwib_uint32 datasize, msg;
  int reti;

  *pneedmoredata = NETWIB_FALSE;

  /* first check */
  datasize = netwib__buf_ref_data_size(&ptr->bufwrite);
  if (datasize == 0) {
    return(NETWIB_ERR_LOINTERNALERROR);
  }
  data = netwib__buf_ref_data_ptr(&ptr->bufwrite);
  if (data[0] != NETWIB_PRIV_IO_SHELL_MSG_SEPARATOR) {
    return(NETWIB_ERR_LOINTERNALERROR);
  }

  /* message type */
  if (datasize < 2) {
    *pneedmoredata = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }
  msg = data[1];

  /* decode message */
  switch(msg) {
  case NETWIB_PRIV_IO_SHELL_MSG_SEPARATOR :
    netwib_er(netwib_buf_init_ext_arrayfilled(data, 1, &buf));
    netwib_er(netwib_priv_io_execcommon_write(&ptr->common, &buf));
    ptr->bufwrite.beginoffset += 2;
    break;
  case NETWIB_PRIV_IO_SHELL_MSG_WINDOWSIZE :
    if (datasize < 2+16) {
      *pneedmoredata = NETWIB_TRUE;
      return(NETWIB_ERR_OK);
    }
    data += 2;
    netwib__data_decode_uint32(data, ws.ws_row);
    netwib__data_decode_uint32(data, ws.ws_col);
    netwib__data_decode_uint32(data, ws.ws_xpixel);
    netwib__data_decode_uint32(data, ws.ws_ypixel);
    reti = ioctl(ptr->ptymaster, TIOCSWINSZ, &ws);
    if (reti < 0) {
      return(NETWIB_ERR_FUIOCTL);
    }
    ptr->bufwrite.beginoffset += 2+16;
    break;
  case NETWIB_PRIV_IO_SHELL_MSG_KEEPALIVE :
    ptr->bufwrite.beginoffset += 2;
    break;
  default :
    return(NETWIB_ERR_LOINTERNALERROR);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_shellserver_init_child(netwib_uint32 uid,
                                                        netwib_constbuf *pbufterm,
                                                        int ptymaster,
                                                        int ptyslave,
                                                        int checkexec[2])
{
  netwib_buf buf;
  netwib_string argv[10], envp[10]; /* 10 is sufficient for now */
  netwib_string filename;
  struct passwd *ppasswd;
  netwib_uint32 uid2;
  int reti, argc, envc;

  /* close unneeded file descriptor */
  reti = close(ptymaster);
  if (reti == -1) return(NETWIB_ERR_FUCLOSE);

  /* child becomes session leader : like this, a kill will kill
     every sub-child.
  */
  reti = setsid();
  if (reti == -1) return(NETWIB_ERR_FUSETSID);

  /* obtain information about user (no need to be reentrant because we are
     in child) */
  uid2 = uid;
  if (uid == NETWIB_IO_INIT_SHELLSERVER_UID_NONE) {
    uid2 = getuid();
  }
  ppasswd = netwib_getpwuid(uid2);
  if (ppasswd == NULL) {
    return(NETWIB_ERR_FUGETPWUID);
  }

  /* create the variables */
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_string(ppasswd->pw_shell, &buf));
  netwib_er(netwib_buf_ref_string(&buf, &filename));
  /**/
  argc = 0;
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_string("shellserver", &buf));
  netwib_er(netwib_buf_ref_string(&buf, &argv[argc++]));
  argv[argc] = NULL;
  /**/
  envc = 0;
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_string("SHELL=", &buf));
  netwib_er(netwib_buf_append_string(ppasswd->pw_shell, &buf));
  netwib_er(netwib_buf_ref_string(&buf, &envp[envc++]));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_string("HOME=", &buf));
  netwib_er(netwib_buf_append_string(ppasswd->pw_dir, &buf));
  netwib_er(netwib_buf_ref_string(&buf, &envp[envc++]));
  if (pbufterm != NETWIB_IO_INIT_SHELLSERVER_TERM_NONE) {
    netwib_er(netwib_buf_init_mallocdefault(&buf));
    netwib_er(netwib_buf_append_string("TERM=", &buf));
    netwib_er(netwib_buf_append_buf(pbufterm, &buf));
    netwib_er(netwib_buf_ref_string(&buf, &envp[envc++]));
  }
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_string("PATH=", &buf));
  if (uid2 == 0) {
    netwib_er(netwib_buf_append_string("/sbin:/bin:/usr/sbin:/usr/bin:/usr/local/sbin:/usr/local/bin", &buf));
  } else {
    netwib_er(netwib_buf_append_string("/bin:/usr/bin:/usr/local/bin", &buf));
  }
  netwib_er(netwib_buf_ref_string(&buf, &envp[envc++]));
  envp[envc] = NULL;

  /* associate file descriptors */
  reti = dup2(ptyslave, STDIN_FILENO);
  if (reti == -1) return(NETWIB_ERR_FUDUP2);
  reti = dup2(ptyslave, STDOUT_FILENO);
  if (reti == -1) return(NETWIB_ERR_FUDUP2);
  reti = dup2(ptyslave, STDERR_FILENO);
  if (reti == -1) return(NETWIB_ERR_FUDUP2);

  /* change user */
  if (uid != NETWIB_IO_INIT_SHELLSERVER_UID_NONE) {
    reti = setgid(ppasswd->pw_gid);
    if (reti == -1) return(NETWIB_ERR_FUSETGID);
    reti = setgroups(0, NULL);
    if (reti == -1) return(NETWIB_ERR_FUSETGROUPS);
    reti = setuid(ppasswd->pw_uid);
    if (reti == -1) return(NETWIB_ERR_FUSETUID);
  }
  reti = chdir(ppasswd->pw_dir);
  if (reti == -1) return(NETWIB_ERR_FUCHDIR);

  /* exec */
  netwib_er(netwib_priv_io_execcommon_launch_child2(filename, argv, envp, checkexec));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_shellserver_init3(netwib_uint32 uid,
                                                   netwib_constbuf *pbufterm,
                                                   int ptymaster,
                                                   int ptyslave,
                                                   int *ppid)
{
  netwib_err ret;
  int checkexec[2];

  netwib_er(netwib_priv_io_execcommon_launch_father1(&checkexec, ppid));

  if (*ppid == 0) {
    /* child */
    ret = netwib_priv_io_shellserver_init_child(uid, pbufterm,
                                                ptymaster, ptyslave,
                                                checkexec);
    netwib_er(netwib_priv_io_execcommon_launch_child1(checkexec, ret));
  }

  netwib_er(netwib_priv_io_execcommon_launch_father2(checkexec));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_shellserver_init2(netwib_uint32 uid,
                                                   netwib_constbuf *pbufterm,
                                                   int *ppid,
                                                   int *preadfd,
                                                   int *pwritefd)
{
  int ptymaster, ptyslave;
  int reti;
  netwib_err ret;

  /* open pty */
  reti = openpty(&ptymaster, &ptyslave, NULL, NULL, NULL);
  if (reti == -1) return(NETWIB_ERR_FUOPENPTY);
  /* On some old systems, chown/chmod are not done by openpty. So we do
     them in case. However, these calls only work if we are root, or already
     the right user. So, on secure systems, there is no problem, but on
     insecure systems, users won't be able to call these functions : this
     generates an error (so this IO don't work on insecure systems). */
  reti = chown(netwib_ttyname(ptyslave), uid, -1);
  if (reti == -1) return(NETWIB_ERR_FUCHOWN);
  reti = chmod(netwib_ttyname(ptyslave), 0600);
  if (reti == -1) return(NETWIB_ERR_FUCHMOD);

  /* try to launch child */
  ret = netwib_priv_io_shellserver_init3(uid, pbufterm, ptymaster, ptyslave,
                                         ppid);
  if (ret != NETWIB_ERR_OK) {
    close(ptymaster);
    close(ptyslave);
    return(ret);
  }

  /* close unneeded file descriptors */
  reti = close(ptyslave);
  if (reti == -1) {
    close(ptymaster);
    return(NETWIB_ERR_FUCLOSE);
  }

  /* store information */
  *preadfd = ptymaster;
  *pwritefd = ptymaster;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_shellserver_init(netwib_uint32 uid,
                                                  netwib_constbuf *pbufterm,
                                                  netwib_bool killonclose,
                                                  netwib_bool *pexitednormally,
                                                  netwib_uint32 *preturnedvalue,
                                                  netwib_bool *preadinitialized,
                                                  netwib_bool *pwriteinitialized,
                                                  netwib_priv_io_shellserver *ptr)
{
  netwib_err ret=NETWIB_ERR_OK;
  int pid=0, readfd=0, writefd=0;

  /* try to launch child */
  ret = netwib_priv_io_shellserver_init2(uid, pbufterm,
                                         &pid, &readfd, &writefd);
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }
  ptr->ptymaster = readfd;

  /* store information */
  netwib_er(netwib_buf_init_mallocdefault(&ptr->bufwrite));
  ptr->bufwrite.flags = NETWIB_BUF_FLAGS_CANSLIDE;
  netwib_er(netwib_priv_io_execcommon_init(readfd, writefd, pid, killonclose, pexitednormally, preturnedvalue, preadinitialized, pwriteinitialized, &ptr->common));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_shellserver_close(netwib_priv_io_shellserver *ptr)
{
  netwib_er(netwib_buf_close(&ptr->bufwrite));
  ptr->common.writefd = -1; /* already closed */
  netwib_er(netwib_priv_io_execcommon_close(&ptr->common));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_shellserver_read(netwib_io *pio,
                                                  netwib_buf *pbuf)
{
  netwib_priv_io_shellserver *ptr = (netwib_priv_io_shellserver *)pio->pcommon;
  netwib_er(netwib_priv_io_execcommon_read(&ptr->common, pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_shellserver_write(netwib_io *pio,
                                                   netwib_constbuf *pbuf)
{
  netwib_priv_io_shellserver *ptr = (netwib_priv_io_shellserver *)pio->pcommon;
  netwib_buf buf;
  netwib_data data, pc;
  netwib_uint32 datasize, chunksize;
  netwib_bool needmoredata;

  /* simple cases */
  if (netwib__buf_ref_data_size(pbuf) == 0) {
    return(NETWIB_ERR_OK);
  }
  if (netwib__buf_ref_data_size(&ptr->bufwrite) == 0) {
    data = netwib__buf_ref_data_ptr(pbuf);
    datasize = netwib__buf_ref_data_size(pbuf);
    pc = netwib_c_memchr(data, NETWIB_PRIV_IO_SHELL_MSG_SEPARATOR, datasize);
    if (pc == NULL) {
      netwib_er(netwib_priv_io_execcommon_write(&ptr->common, pbuf));
      return(NETWIB_ERR_OK);
    }
  }

  /* full case */
  netwib_er(netwib_buf_append_buf(pbuf, &ptr->bufwrite));
  while (NETWIB_TRUE) {
    datasize = netwib__buf_ref_data_size(&ptr->bufwrite);
    if (datasize == 0) {
      break;
    }
    data = netwib__buf_ref_data_ptr(&ptr->bufwrite);
    pc = netwib_c_memchr(data, NETWIB_PRIV_IO_SHELL_MSG_SEPARATOR, datasize);
    if (pc == NULL) {
      chunksize = datasize;
    } else {
      chunksize = pc - data;
    }
    if (chunksize) {
      netwib_er(netwib_buf_init_ext_arrayfilled(data, chunksize, &buf));
      netwib_er(netwib_priv_io_execcommon_write(&ptr->common, &buf));
      ptr->bufwrite.beginoffset += chunksize;
    }
    if (netwib__buf_ref_data_size(&ptr->bufwrite) == 0) {
      break;
    }
    netwib_er(netwib_priv_io_shellserver_msg_analyze(ptr, &needmoredata));
    if (needmoredata) {
      break;
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_shellserver_wait(netwib_io *pio,
                                                  netwib_io_waytype way,
                                                  netwib_consttime *pabstime,
                                                  netwib_bool *pevent)
{
  netwib_priv_io_shellserver *ptr = (netwib_priv_io_shellserver *)pio->pcommon;

  netwib_er(netwib_priv_io_execcommon_wait(&ptr->common, way, pabstime, pevent));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_shellserver_ctl_set(netwib_io *pio,
                                                     netwib_io_waytype way,
                                                     netwib_io_ctltype type,
                                                     netwib_ptr p,
                                                     netwib_uint32 ui)
{
  netwib_priv_io_shellserver *ptr = (netwib_priv_io_shellserver *)pio->pcommon;
  netwib_er(netwib_priv_io_execcommon_ctl_set(&ptr->common, way, type, p, ui));
  if (type == NETWIB_IO_CTLTYPE_END) {
    pio->wr.supported = NETWIB_FALSE;
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_shellserver_ctl_get(netwib_io *pio,
                                                     netwib_io_waytype way,
                                                     netwib_io_ctltype type,
                                                     netwib_ptr p,
                                                     netwib_uint32 *pui)
{
  netwib_priv_io_shellserver *ptr = (netwib_priv_io_shellserver *)pio->pcommon;

  switch(type) {
  case NETWIB_IO_CTLTYPE_RES:
    if (pui != NULL) *pui = NETWIB_IO_RESTYPE_SHELLSERVER;
    return(NETWIB_ERR_OK);
  default:
    return(netwib_priv_io_execcommon_ctl_get(&ptr->common, way, type, p, pui));
  }

  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_shellserver_fclose(netwib_io *pio)
{
  netwib_priv_io_shellserver *ptr = (netwib_priv_io_shellserver *)pio->pcommon;

  netwib_er(netwib_priv_io_shellserver_close(ptr));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

#endif

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_shellserver(netwib_uint32 uid,
                                      netwib_constbuf *pbufterm,
                                      netwib_bool killonclose,
                                      netwib_bool *pexitednormally,
                                      netwib_uint32 *preturnedvalue,
                                      netwib_io **ppio)
{
#if NETWIBDEF_IOSHELL_SUPPORTED == 1
  netwib_bool rdsup, wrsup;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_shellserver), &pcommon));
  ret = netwib_priv_io_shellserver_init(uid, pbufterm,
                                        killonclose, pexitednormally,
                                        preturnedvalue, &rdsup, &wrsup,
                                        (netwib_priv_io_shellserver *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdsup, wrsup,
                           pcommon,
                           &netwib_priv_io_shellserver_read,
                           &netwib_priv_io_shellserver_write,
                           &netwib_priv_io_shellserver_wait,
                           NULL,
                           &netwib_priv_io_shellserver_ctl_set,
                           &netwib_priv_io_shellserver_ctl_get,
                           &netwib_priv_io_shellserver_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
#else
  uid = uid; /* for compiler warning */
  pbufterm = pbufterm; /* for compiler warning */
  killonclose = killonclose; /* for compiler warning */
  pexitednormally = pexitednormally; /* for compiler warning */
  preturnedvalue = preturnedvalue; /* for compiler warning */
  ppio = ppio; /* for compiler warning */
  return(NETWIB_ERR_LONOTIMPLEMENTED);
#endif
}
