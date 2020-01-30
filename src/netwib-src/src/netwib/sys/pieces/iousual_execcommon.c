/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
typedef struct {
  int readfd;
  int writefd;
  int pid;
  netwib_bool killonclose;
  netwib_bool childisdead;
  netwib_bool *pexitednormally;
  netwib_uint32 *preturnedvalue;
} netwib_priv_io_execcommon;

/*-------------------------------------------------------------*/
#if NETWIBDEF_LIBPTHREADFROMMIT == 0
static netwib_err netwib_priv_io_execcommon_close_descriptors(netwib_uint32 excepted)
{
  netwib_uint32 themax, i;

 #if NETWIBDEF_HAVEFUNC_GETRLIMIT==1
  {
    struct rlimit lim;
    int reti;
    reti = getrlimit(RLIMIT_NOFILE, &lim);
    if (reti == -1) {
      return(NETWIB_ERR_FUGETRLIMIT);
    }
    themax = lim.rlim_cur;
  }
 #elif NETWIBDEF_HAVEFUNC_SYSCONF==1
  themax = sysconf(_SC_OPEN_MAX);
 #else
  themax = OPEN_MAX;
 #endif

  /* we could use "fcntl(fd, F_SETFD, FD_CLOEXEC);", but we have to check
     if fd is open. So, I choose to close everything. */
  for (i = 3 ; i < excepted; i++) {
    /*ignore*/close(i);
  }
  for (i = excepted+1 ; i < themax; i++) {
    /*ignore*/close(i);
  }

  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_execcommon_launch_child2(netwib_conststring filename,
                                                          netwib_string const argv[],
                                                          netwib_string const envp[],
                                                          int checkexec[2])
{
  int reti;

  /* prepare our trick to check if exec succeeded */
  reti = close(checkexec[0]);
  if (reti == -1) return(NETWIB_ERR_FUCLOSE);
  reti = fcntl(checkexec[1], F_SETFD, FD_CLOEXEC);
  if (reti == -1) return(NETWIB_ERR_FUFCNTL);

  /* close unneeded file descriptors */
#if NETWIBDEF_LIBPTHREADFROMMIT == 0
  /* except for MIT POSIX threads, because a private fd is used, and we
     don't know which one it is */
  netwib_er(netwib_priv_io_execcommon_close_descriptors(checkexec[1]));
#endif

  /* exec */
  reti = execve(filename, argv, envp);
  if (reti < 0) return(NETWIB_ERR_LOCANTEXEC);

  /* should not be reached */
  return(NETWIB_ERR_LOINTERNALERROR);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_execcommon_launch_father1(int (*pcheckexec)[2],
                                                           int *ppid)
{
  int pid;
  int reti;

  /* this pipe is used to check if exec worked */
  reti = pipe(*pcheckexec);
  if (reti == -1) return(NETWIB_ERR_FUPIPE);

  /* try to launch child */
  pid = fork();
  if (pid == -1) {
    close((*pcheckexec)[0]);
    close((*pcheckexec)[1]);
    return(NETWIB_ERR_FUFORK);
  }

  *ppid = pid;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_execcommon_launch_child1(int checkexec[2],
                                                          netwib_err retchild)
{
  /* error number is written to checkexec pipe */
  /*ignore*/netwib_priv_fdpipe_write_uint32(checkexec[1], retchild);
  _exit(NETWIB_ERR_DATAEND);
  return(NETWIB_ERR_LOINTERNALERROR);
  /* never reached */
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_execcommon_launch_father2(int checkexec[2])
{
  netwib_err ret, retchild;
  int reti;

  /* wait to ensure exec is ok */
  reti = close(checkexec[1]);
  if (reti == -1) return(NETWIB_ERR_FUCLOSE);
#if NETWIBDEF_LIBPTHREADFROMMIT == 1
  /* read implementation in MIT POSIX threads does not detect the close.
     So, we first wait for one second */
 {
   netwib_time t;
   netwib_bool event;
   netwib_er(netwib_time_init_now(&t));
   netwib_er(netwib_time_plus_sec(&t, 1));
   netwib_er(netwib_priv_fd_wait(checkexec[0], NETWIB_IO_WAYTYPE_READ,
                                 &t, &event));
   if (event) {
     ret = netwib_priv_fd_read_uint32(checkexec[0], (netwib_uint32*)&retchild);
   } else {
     ret = NETWIB_ERR_DATAEND;
   }
 }
#else
  ret = netwib_priv_fd_read_uint32(checkexec[0], (netwib_uint32*)&retchild);
#endif
  close(checkexec[0]);
  if (ret == NETWIB_ERR_OK) {
    if (retchild == NETWIB_ERR_OK) {
      return(NETWIB_ERR_LOERROROKKO);
    } else {
      return(retchild);
    }
  } else if (ret != NETWIB_ERR_DATAEND) {
    return(ret);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_execcommon_init(int readfd,
                                                 int writefd,
                                                 int pid,
                                                 netwib_bool killonclose,
                                                 netwib_bool *pexitednormally,
                                                 netwib_uint32 *preturnedvalue,
                                                 netwib_bool *preadinitialized,
                                                 netwib_bool *pwriteinitialized,
                                                 netwib_priv_io_execcommon *ptr)
{
  ptr->readfd = readfd;
  ptr->writefd = writefd;
  ptr->pid = pid;
  ptr->killonclose = killonclose;
  ptr->childisdead = NETWIB_FALSE;
  ptr->pexitednormally = pexitednormally;
  ptr->preturnedvalue = preturnedvalue;
  *preadinitialized = (readfd==-1)?NETWIB_FALSE:NETWIB_TRUE;
  *pwriteinitialized = (writefd==-1)?NETWIB_FALSE:NETWIB_TRUE;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_execcommon_waitpid(netwib_priv_io_execcommon *ptr,
                                                    netwib_bool blocking)
{
  int reti, status;

  if (ptr->childisdead) {
    /* all information is already set */
    return(NETWIB_ERR_OK);
  }

  if (blocking) {
    if (ptr->killonclose) {
      /* smooth then violent kill of the child */
#if NETWIBDEF_LIBPTHREADFROMMIT == 1
      /* waitpid implementation in MIT POSIX threads does not support
         WNOHANG */
      /* note we send signal to the group */
      /* ignore */ kill( - ptr->pid, SIGTERM);
      netwib_er(netwib_priv_pause());
      /* ignore */ kill( - ptr->pid, SIGKILL);
      reti = waitpid(ptr->pid, &status, 0);
#else
      reti = waitpid(ptr->pid, &status, WNOHANG);
      if (reti == 0) {
        /* note we send signal to the group */
        /* ignore */ kill( - ptr->pid, SIGTERM);
        netwib_er(netwib_priv_pause());
        reti = waitpid(ptr->pid, &status, WNOHANG);
        if (reti == 0) {
          /* ignore */ kill( - ptr->pid, SIGKILL);
          reti = waitpid(ptr->pid, &status, 0);
        }
      }
#endif
    } else {
      reti = waitpid(ptr->pid, &status, 0);
    }
    if (reti != ptr->pid) {
      return(NETWIB_ERR_FUWAITPID);
    }
  } else {
    reti = waitpid(ptr->pid, &status, WNOHANG);
    if (reti == 0) {
      return(NETWIB_ERR_DATANOTAVAIL);
    }
    ptr->childisdead = NETWIB_TRUE;
  }

  if (WIFEXITED(status)) {
    if (ptr->pexitednormally != NULL) {
      *(ptr->pexitednormally) = NETWIB_TRUE;
    }
    if (ptr->preturnedvalue != NULL) {
      *(ptr->preturnedvalue) = WEXITSTATUS(status);
    }
  } else {
    if (ptr->pexitednormally != NULL) {
      *(ptr->pexitednormally) = NETWIB_FALSE;
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_execcommon_close(netwib_priv_io_execcommon *ptr)
{
  int reti;

  netwib_er(netwib_priv_io_execcommon_waitpid(ptr, NETWIB_TRUE));

  /* close descriptors */
  if (ptr->readfd != -1) {
    reti = close(ptr->readfd);
    if (reti == -1) return(NETWIB_ERR_FUCLOSE);
  }
  if (ptr->writefd != -1) {
    reti = close(ptr->writefd);
    if (reti == -1) return(NETWIB_ERR_FUCLOSE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_execcommon_read(netwib_priv_io_execcommon *ptr,
                                                 netwib_buf *pbuf)
{
  netwib_err ret;

  if (ptr->childisdead) {
    return(NETWIB_ERR_DATAEND);
  }

  ret = netwib_priv_fd_read(ptr->readfd, pbuf);
  if (ret == NETWIB_ERR_FUREAD) {
    ret = NETWIB_ERR_DATAEND;
  }
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_execcommon_write(netwib_priv_io_execcommon *ptr,
                                                  netwib_constbuf *pbuf)
{
  return(netwib_priv_fdpipe_write(ptr->writefd, pbuf));
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_execcommon_wait(netwib_priv_io_execcommon *ptr,
                                                 netwib_io_waytype way,
                                                 netwib_consttime *pabstime,
                                                 netwib_bool *pevent)
{
  netwib_err ret;

  switch (way) {
  case NETWIB_IO_WAYTYPE_READ :
    netwib_er(netwib_priv_fd_wait(ptr->readfd, way, pabstime, pevent));
    if (! *pevent) {
      /* With shellserver, there is a pty plugged on the bash. When bash
         is dead, sometimes the other side of the pty is not informed,
         so it can't detect end. We have to check if bash is dead. */
      ret = netwib_priv_io_execcommon_waitpid(ptr, NETWIB_FALSE);
      if (ret == NETWIB_ERR_OK) {
        *pevent = NETWIB_TRUE;
      }
    }
    break;
  case NETWIB_IO_WAYTYPE_WRITE :
    netwib_er(netwib_priv_fd_wait(ptr->writefd, way, pabstime, pevent));
    break;
  case NETWIB_IO_WAYTYPE_RDWR :
  case NETWIB_IO_WAYTYPE_SUPPORTED :
    return(NETWIB_ERR_PLEASECONSTRUCT);
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_execcommon_ctl_set(netwib_priv_io_execcommon *ptr,
                                                    netwib_io_waytype way,
                                                    netwib_io_ctltype type,
                                                    netwib_ptr p,
                                                    netwib_uint32 ui)
{
  int reti;

  switch(type) {
    case NETWIB_IO_CTLTYPE_RES:
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    case NETWIB_IO_CTLTYPE_END:
      if (way != NETWIB_IO_WAYTYPE_WRITE) return(NETWIB_ERR_PAINVALIDTYPE);
      if (ptr->writefd != -1) {
        reti = close(ptr->writefd);
        if (reti == -1) return(NETWIB_ERR_FUCLOSE);
        ptr->writefd = -1;
      }
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
static netwib_err netwib_priv_io_execcommon_ctl_get(netwib_priv_io_execcommon *ptr,
                                                    netwib_io_waytype way,
                                                    netwib_io_ctltype type,
                                                    netwib_ptr p,
                                                    netwib_uint32 *pui)
{

  switch(type) {
    case NETWIB_IO_CTLTYPE_END:
      return(NETWIB_ERR_PAINVALIDTYPE);
    default:
      return(NETWIB_ERR_PLEASETRYNEXT);
  }

  ptr = ptr; /* for compiler warning */
  way = way; /* for compiler warning */
  p = p; /* for compiler warning */
  pui = pui; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}
