/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
netwib_err netwib_priv_fd_block_set(int fd,
                                  netwib_bool beblocking)
{
  int old_fc, new_fc, reti;

  old_fc = fcntl(fd, F_GETFL, 0);
  if (old_fc < 0) {
    if (errno == EBADF) {
      errno = 0;
      return(NETWIB_ERR_DATAEND);
    }
    return(NETWIB_ERR_FUFCNTL);
  }
  if (beblocking)
    new_fc = old_fc & (~O_NONBLOCK);
  else
    new_fc = old_fc | O_NONBLOCK;
  reti = fcntl(fd, F_SETFL, new_fc);
  if (reti < 0)
    return(NETWIB_ERR_FUFCNTL);
  return(NETWIB_ERR_OK);
}
#elif defined NETWIBDEF_SYSNAME_Windows
  /* this function is not needed */
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
netwib_err netwib_priv_fd_block_get(int fd,
                                    netwib_bool *pbeblocking)
{
  int fc;
  netwib_bool beblocking;

  fc = fcntl(fd, F_GETFL, 0);
  if (fc < 0)
    return(NETWIB_ERR_FUFCNTL);
  if (fc & O_NONBLOCK) {
    beblocking = NETWIB_FALSE;
  } else {
    beblocking = NETWIB_TRUE;
  }

  if (pbeblocking != NULL)
    *pbeblocking = beblocking;
  return(NETWIB_ERR_OK);
}
#elif defined NETWIBDEF_SYSNAME_Windows
  /* this function is not needed */
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_fd_write(int fd,
                                netwib_constbuf *pbuf)
{
  netwib_data data;
  netwib_uint32 datasize;
  int reti;

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);

  reti = netwib_c_write(fd, data, datasize);
  if (reti == -1) {
    if (errno == EBADF) {
      errno = 0;
      return(NETWIB_ERR_LOOBJWRITENOTSUPPORTED);
    }
    return(NETWIB_ERR_FUWRITE);
  }

  if ((netwib_uint32)reti != datasize) {
    /* should we use a loop and try to write missing ? */
    return(NETWIB_ERR_FUWRITE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
netwib_err netwib_priv_fdpipe_write(int fd,
                                    netwib_constbuf *pbuf)
{
  netwib_c_sighandlert oldsig;
  netwib_err ret;

  /*ignore SIGPIPE to have errno==EPIPE (man 2 write) */
  oldsig = signal(SIGPIPE, SIG_IGN);
  if (oldsig == SIG_ERR) return(NETWIB_ERR_FUSIGNAL);
  ret = netwib_priv_fd_write(fd, pbuf);
  oldsig = signal(SIGPIPE, oldsig);
  if (oldsig == SIG_ERR) return(NETWIB_ERR_FUSIGNAL);

  return(ret);
}
#elif defined NETWIBDEF_SYSNAME_Windows
  /* this function is not needed */
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_fd_write_uint32(int fd,
                                       netwib_uint32 ui)
{
  netwib_byte array[4];
  netwib_uint32 sizetowrite, offset;
  int reti;

  array[0] = netwib_c2_uint32_0(ui);
  array[1] = netwib_c2_uint32_1(ui);
  array[2] = netwib_c2_uint32_2(ui);
  array[3] = netwib_c2_uint32_3(ui);

  offset = 0;
  sizetowrite = 4;
  while(NETWIB_TRUE) {
    reti = netwib_c_write(fd, array+offset, sizetowrite);
    if (reti == -1) {
      if (errno == EBADF) {
        errno = 0;
        return(NETWIB_ERR_LOOBJWRITENOTSUPPORTED);
      }
      return(NETWIB_ERR_FUWRITE);
    }
    if ((netwib_uint32)reti == sizetowrite) {
      break;
    }
    offset += reti;
    sizetowrite -= reti;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
netwib_err netwib_priv_fdpipe_write_uint32(int fd,
                                           netwib_uint32 ui)
{
  netwib_c_sighandlert oldsig;
  netwib_err ret;

  /*ignore SIGPIPE to have errno==EPIPE (man 2 write) */
  oldsig = signal(SIGPIPE, SIG_IGN);
  if (oldsig == SIG_ERR) return(NETWIB_ERR_FUSIGNAL);
  ret = netwib_priv_fd_write_uint32(fd, ui);
  oldsig = signal(SIGPIPE, oldsig);
  if (oldsig == SIG_ERR) return(NETWIB_ERR_FUSIGNAL);

  return(ret);
}
#elif defined NETWIBDEF_SYSNAME_Windows
  /* this function is not needed */
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
#if NETWIBDEF_HAVEFUNC_POLL==1
static netwib_err netwib_priv_fd_wait_poll(int fd,
                                           netwib_io_waytype way,
                                           netwib_consttime *pabstime,
                                           netwib_bool *pevent)
{
  struct pollfd pollarray[1];
  int reti, mask, timeoutint;

  /* set wanted mask */
#if ! defined POLLRDNORM
 #define POLLRDNORM 0
#endif
#if ! defined POLLRDBAND
 #define POLLRDBAND 0
#endif
#if ! defined POLLWRNORM
 #define POLLWRNORM 0
#endif
#if ! defined POLLWRBAND
 #define POLLWRBAND 0
#endif
  mask = POLLHUP;
  switch(way) {
    case NETWIB_IO_WAYTYPE_READ :
      mask |= POLLIN | POLLRDNORM | POLLRDBAND | POLLPRI;
      break;
    case NETWIB_IO_WAYTYPE_WRITE :
      mask |= POLLOUT | POLLWRNORM | POLLWRBAND;
      break;
    case NETWIB_IO_WAYTYPE_RDWR :
      mask |= POLLIN | POLLRDNORM | POLLRDBAND | POLLPRI;
      mask |= POLLOUT | POLLWRNORM | POLLWRBAND;
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
  }

  /* set fd */
  pollarray[0].fd = fd;
  pollarray[0].events = mask;
  pollarray[0].revents = 0;

  /* wait */
  netwib_er(netwib_priv_time_timeout_poll(pabstime, &timeoutint));
  if (timeoutint == 0 && pabstime != NETWIB_TIME_ZERO) {
    if (pevent != NULL) *pevent = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  }
  reti = poll(pollarray, 1, timeoutint);
  if (reti < 0) {
    if (errno == EINTR) {
      if (pevent != NULL) *pevent = NETWIB_FALSE;
      return(NETWIB_ERR_OK);
    }
    return(NETWIB_ERR_FUPOLL);
  }

  /* set error values */
  if (reti == 0) {
    if (pevent != NULL) *pevent = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  }

  /* set normal values */
  if (pevent != NULL) {
    if (pollarray[0].revents & mask) {
      *pevent = NETWIB_TRUE;
    } else {
      *pevent = NETWIB_FALSE;
    }
  }
  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
#if NETWIBDEF_HAVEFUNC_POLL==0
static netwib_err netwib_priv_fd_wait_select(int fd,
                                             netwib_io_waytype way,
                                             netwib_consttime *pabstime,
                                             netwib_bool *pevent)
{
  netwib_bool event;
  fd_set readfdset, writefdset, exceptfdset;
  struct timeval to, *pto;
  int reti;

  if (fd < 0) {
    return(NETWIB_ERR_PATOOLOW);
  }
#if defined FD_SETSIZE
 #if ! defined NETWIBDEF_SYSNAME_Windows
  /* sous Windows, FD_SETSIZE vaut 64, mais fd peut le dépasser sans souci */
  if (fd >= FD_SETSIZE) {
    return(NETWIB_ERR_PATOOHIGH);
  }
 #endif
#endif

  /* initialize fd */
  FD_ZERO(&readfdset);
  FD_ZERO(&writefdset);
  FD_ZERO(&exceptfdset);
  FD_SET(fd, &exceptfdset);
  switch(way) {
    case NETWIB_IO_WAYTYPE_READ :
      FD_SET(fd, &readfdset);
      break;
    case NETWIB_IO_WAYTYPE_WRITE :
      FD_SET(fd, &writefdset);
      break;
    case NETWIB_IO_WAYTYPE_RDWR :
      FD_SET(fd, &readfdset);
      FD_SET(fd, &writefdset);
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
  }

  /* select */
  netwib_er(netwib_priv_time_timeout_select(pabstime, &to, &pto));
  if (pto != NULL) {
    if (pto->tv_sec == 0 && pto->tv_usec == 0 &&
        pabstime != NETWIB_TIME_ZERO) {
      if (pevent != NULL) *pevent = NETWIB_FALSE;
      return(NETWIB_ERR_OK);
    }
  }
  reti = select(fd + 1, &readfdset, &writefdset, &exceptfdset, pto);
  if (reti < 0) {
    return(NETWIB_ERR_FUSELECT);
  }

  /* check output */
  event = NETWIB_FALSE;
  if (FD_ISSET(fd, &readfdset) ||
      FD_ISSET(fd, &writefdset) ||
      FD_ISSET(fd, &exceptfdset)) {
    event = NETWIB_TRUE;
  }

  if (pevent != NULL) *pevent = event;
  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_fd_wait(int fd,
                               netwib_io_waytype way,
                               netwib_consttime *pabstime,
                               netwib_bool *pevent)
{

#if NETWIBDEF_HAVEFUNC_POLL==1
  netwib_er(netwib_priv_fd_wait_poll(fd, way, pabstime, pevent));
#else
  netwib_er(netwib_priv_fd_wait_select(fd, way, pabstime, pevent));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_fd_read(int fd,
                               netwib_buf *pbuf)
{
  netwib_data data;
  netwib_uint32 maxmsgsize;
  int readsize;

  netwib_er(netwib_buf_wishspace(pbuf, NETWIB_PRIV_MINMSGSIZE_READ,
                                 &data, &maxmsgsize));
  if (maxmsgsize == 0) {
    return(NETWIB_ERR_DATANOSPACE);
  }

  /* read */
  readsize = netwib_c_read(fd, data, maxmsgsize);
  /* an error occurred */
  if (readsize == -1) {
    if (errno == EAGAIN) {
      /* nothing was available */
      errno = 0;
      return(NETWIB_ERR_DATANOTAVAIL);
    } else if (errno == EBADF) {
      /* reached end */
      errno = 0;
      return(NETWIB_ERR_DATAEND);
    } else {
      /* fatal error */
      return(NETWIB_ERR_FUREAD);
    }
  }

  /* if we read 0, we are at end of file */
  if (readsize == 0) {
    return(NETWIB_ERR_DATAEND);
  }

  /* store read data */
  pbuf->endoffset += readsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_fd_read_byte(int fd,
                                    netwib_byte *pb)
{
  netwib_byte array[1];
  int readsize;

  readsize = netwib_c_read(fd, array, 1);
  if (readsize == -1) {
    if (errno == EAGAIN) {
      /* nothing was available */
      errno = 0;
      return(NETWIB_ERR_DATANOTAVAIL);
    } else if (errno == EBADF) {
      /* reached end */
      errno = 0;
      return(NETWIB_ERR_DATAEND);
    } else {
      /* fatal error */
      return(NETWIB_ERR_FUREAD);
    }
  }
  if (readsize == 0) {
    return(NETWIB_ERR_DATAEND);
  }

  if (pb != NULL) {
    *pb = array[0];
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_fd_read_uint32(int fd,
                                      netwib_uint32 *pui)
{
  netwib_byte array[4];
  netwib_uint32 sizetoread, offset;
  int readsize;

  offset = 0;
  sizetoread = 4;
  while(NETWIB_TRUE) {
    readsize = netwib_c_read(fd, array+offset, sizetoread);
    if (readsize == -1) {
      if (errno == EAGAIN) {
        /* nothing was available */
        errno = 0;
        return(NETWIB_ERR_DATANOTAVAIL);
      } else if (errno == EBADF) {
        /* reached end */
        errno = 0;
        return(NETWIB_ERR_DATAEND);
      } else {
        /* fatal error */
        return(NETWIB_ERR_FUREAD);
      }
    }
    if (readsize == 0) {
      return(NETWIB_ERR_DATAEND);
    }
    if ((netwib_uint32)readsize == sizetoread) {
      break;
    }
    offset += readsize;
    sizetoread -= readsize;
  }

  if (pui != NULL) {
    *pui = netwib_c2_uint32_4(array[0], array[1], array[2], array[3]);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_fd_flush(int fd)
{
  int reti;

#if defined NETWIBDEF_SYSNAME_Unix
  reti = fsync(fd);
#elif defined NETWIBDEF_SYSNAME_Windows
  reti = _commit(fd);
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif
  if (reti < 0) {
    return(NETWIB_ERR_FUFSYNC);
  }

  return(NETWIB_ERR_OK);
}
