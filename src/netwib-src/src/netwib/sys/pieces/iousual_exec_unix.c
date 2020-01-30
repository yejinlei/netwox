/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
#define NETWIBDEF_IOSHELL_SUPPORTED 0
#if defined NETWIBDEF_SYSNAME_Linux
 #if NETWIBDEF_HAVEINCL_PTY == 1
  #if NETWIBDEF_LIBUTILINSTALLED == 1
   #undef NETWIBDEF_IOSHELL_SUPPORTED
   #define NETWIBDEF_IOSHELL_SUPPORTED 1
  #endif
 #endif
#endif

/*-------------------------------------------------------------*/
typedef struct {
  netwib_priv_io_execcommon common;
} netwib_priv_io_exec;

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_exec_init_child(netwib_conststring filename,
                                                 netwib_string const argv[],
                                                 int childtofather[2],
                                                 int fathertochild[2],
                                                 int checkexec[2])
{
  int reti, devnull;

  /* close unneeded file descriptors */
  if (childtofather != NULL) {
    reti = close(childtofather[0]);
    if (reti == -1) return(NETWIB_ERR_FUCLOSE);
  }
  if (fathertochild != NULL) {
    reti = close(fathertochild[1]);
    if (reti == -1) return(NETWIB_ERR_FUCLOSE);
  }

  /* child becomes session leader : like this, a kill will kill
     every sub-child.
  */
  reti = setsid();
  if (reti == -1) return(NETWIB_ERR_FUSETSID);

  /* open /dev/null */
  devnull = open("/dev/null", O_RDWR);
  if (devnull < 0) return(NETWIB_ERR_FUOPEN);

  /* associate file descriptors */
  if (fathertochild != NULL) {
    reti = dup2(fathertochild[0], STDIN_FILENO);
    if (reti == -1) return(NETWIB_ERR_FUDUP2);
    reti = close(fathertochild[0]);
    if (reti == -1) return(NETWIB_ERR_FUCLOSE);
  } else {
    reti = dup2(devnull, STDIN_FILENO);
    if (reti == -1) return(NETWIB_ERR_FUDUP2);
  }

  if (childtofather != NULL) {
    reti = dup2(childtofather[1], STDOUT_FILENO);
    if (reti == -1) return(NETWIB_ERR_FUDUP2);
    reti = close(childtofather[1]);
    if (reti == -1) return(NETWIB_ERR_FUCLOSE);
  } else {
    reti = dup2(devnull, STDOUT_FILENO);
    if (reti == -1) return(NETWIB_ERR_FUDUP2);
  }

  reti = dup2(devnull, STDERR_FILENO);
  if (reti == -1) return(NETWIB_ERR_FUDUP2);

  /* exec */
  netwib_er(netwib_priv_io_execcommon_launch_child2(filename, argv, NULL, checkexec));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_exec_init3(netwib_string filename,
                                            netwib_string argv[],
                                            int childtofather[2],
                                            int fathertochild[2],
                                            int *ppid)
{
  netwib_err ret;
  int checkexec[2];

  netwib_er(netwib_priv_io_execcommon_launch_father1(&checkexec, ppid));

  if (*ppid == 0) {
    /* child */
    ret = netwib_priv_io_exec_init_child(filename, argv,
                                         childtofather, fathertochild,
                                         checkexec);
    netwib_er(netwib_priv_io_execcommon_launch_child1(checkexec, ret));
  }

  netwib_er(netwib_priv_io_execcommon_launch_father2(checkexec));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_exec_init2(netwib_string filename,
                                            netwib_string argv[],
                                            netwib_io_waytype providedway,
                                            int *ppid,
                                            int *preadfd,
                                            int *pwritefd)
{
  netwib_bool provideread, providewrite;
  netwib_err ret;
  int fathertochild[2];
  int childtofather[2];
  int reti;

  switch(providedway) {
    case NETWIB_IO_WAYTYPE_READ :
      provideread = NETWIB_TRUE;
      providewrite = NETWIB_FALSE;
      break;
    case NETWIB_IO_WAYTYPE_WRITE :
      provideread = NETWIB_FALSE;
      providewrite = NETWIB_TRUE;
      break;
    case NETWIB_IO_WAYTYPE_RDWR :
      provideread = NETWIB_TRUE;
      providewrite = NETWIB_TRUE;
      break;
    case NETWIB_IO_WAYTYPE_NONE :
      provideread = NETWIB_FALSE;
      providewrite = NETWIB_FALSE;
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  /* we use pipes to communicate */
  if (provideread) {
    reti = pipe(childtofather);
    if (reti == -1) return(NETWIB_ERR_FUPIPE);
  }
  if (providewrite) {
    reti = pipe(fathertochild);
    if (reti == -1) {
      if (provideread) {
        close(childtofather[0]);
        close(childtofather[1]);
      }
      return(NETWIB_ERR_FUPIPE);
    }
  }

  /* try to launch child */
  ret = netwib_priv_io_exec_init3(filename, argv,
                                  provideread?childtofather:NULL,
                                  providewrite?fathertochild:NULL,
                                  ppid);
  if (ret != NETWIB_ERR_OK) {
    if (provideread) {
      close(childtofather[0]);
      close(childtofather[1]);
    }
    if (providewrite) {
      close(fathertochild[0]);
      close(fathertochild[1]);
    }
    return(ret);
  }

  /* close unneeded file descriptors */
  if (provideread) {
    reti = close(childtofather[1]);
    if (reti == -1) {
      close(childtofather[0]);
      if (providewrite) {
        close(fathertochild[0]);
        close(fathertochild[1]);
      }
      return(NETWIB_ERR_FUCLOSE);
    }
  }
  if (providewrite) {
    reti = close(fathertochild[0]);
    if (reti == -1) {
      close(fathertochild[1]);
      if (provideread) {
        close(childtofather[0]);
        close(childtofather[1]);
      }
      return(NETWIB_ERR_FUCLOSE);
    }
  }

  /* store information */
  *preadfd = provideread?childtofather[0]:-1;
  *pwritefd = providewrite?fathertochild[1]:-1;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_exec_init(netwib_constbuf *pbufcommand,
                                           netwib_io_waytype providedway,
                                           netwib_bool killonclose,
                                           netwib_bool *pexitednormally,
                                           netwib_uint32 *preturnedvalue,
                                           netwib_bool *preadinitialized,
                                           netwib_bool *pwriteinitialized,
                                           netwib_priv_io_exec *ptr)
{
  netwib_string filename, *argv;
  netwib_err ret=NETWIB_ERR_OK;
  int pid=0, readfd=0, writefd=0;

  /* try to launch child */
  netwib_er(netwib_priv_cmdline_init(pbufcommand, &filename, NULL, &argv));
  ret = netwib_priv_io_exec_init2(filename, argv, providedway,
                                  &pid, &readfd, &writefd);
  netwib_er(netwib_priv_cmdline_close(&filename, &argv));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  /* store information */
  netwib_er(netwib_priv_io_execcommon_init(readfd, writefd, pid, killonclose, pexitednormally, preturnedvalue, preadinitialized, pwriteinitialized, &ptr->common));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_exec_close(netwib_priv_io_exec *ptr)
{
  netwib_er(netwib_priv_io_execcommon_close(&ptr->common));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_exec_read(netwib_io *pio,
                                           netwib_buf *pbuf)
{
  netwib_priv_io_exec *ptr = (netwib_priv_io_exec *)pio->pcommon;
  netwib_er(netwib_priv_io_execcommon_read(&ptr->common, pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_exec_write(netwib_io *pio,
                                            netwib_constbuf *pbuf)
{
  netwib_priv_io_exec *ptr = (netwib_priv_io_exec *)pio->pcommon;
  netwib_er(netwib_priv_io_execcommon_write(&ptr->common, pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_exec_wait(netwib_io *pio,
                                           netwib_io_waytype way,
                                           netwib_consttime *pabstime,
                                           netwib_bool *pevent)
{
  netwib_priv_io_exec *ptr = (netwib_priv_io_exec *)pio->pcommon;
  netwib_er(netwib_priv_io_execcommon_wait(&ptr->common, way, pabstime, pevent));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_exec_ctl_set(netwib_io *pio,
                                              netwib_io_waytype way,
                                              netwib_io_ctltype type,
                                              netwib_ptr p,
                                              netwib_uint32 ui)
{
  netwib_priv_io_exec *ptr = (netwib_priv_io_exec *)pio->pcommon;
  netwib_er(netwib_priv_io_execcommon_ctl_set(&ptr->common, way, type, p, ui));
  if (type == NETWIB_IO_CTLTYPE_END) {
    pio->wr.supported = NETWIB_FALSE;
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_exec_ctl_get(netwib_io *pio,
                                              netwib_io_waytype way,
                                              netwib_io_ctltype type,
                                              netwib_ptr p,
                                              netwib_uint32 *pui)
{
  netwib_priv_io_exec *ptr = (netwib_priv_io_exec *)pio->pcommon;

  switch(type) {
  case NETWIB_IO_CTLTYPE_RES:
    if (pui != NULL) *pui = NETWIB_IO_RESTYPE_EXEC;
    return(NETWIB_ERR_OK);
  default:
    return(netwib_priv_io_execcommon_ctl_get(&ptr->common, way, type, p, pui));
  }

  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_exec_fclose(netwib_io *pio)
{
  netwib_priv_io_exec *ptr = (netwib_priv_io_exec *)pio->pcommon;

  netwib_er(netwib_priv_io_exec_close(ptr));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_exec(netwib_constbuf *pbufcommand,
                               netwib_io_waytype providedway,
                               netwib_bool killonclose,
                               netwib_bool *pexitednormally,
                               netwib_uint32 *preturnedvalue,
                               netwib_io **ppio)
{
  netwib_bool rdsup, wrsup;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_exec), &pcommon));
  ret = netwib_priv_io_exec_init(pbufcommand, providedway,
                                 killonclose, pexitednormally,
                                 preturnedvalue, &rdsup, &wrsup,
                                 (netwib_priv_io_exec *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdsup, wrsup,
                           pcommon,
                           &netwib_priv_io_exec_read,
                           &netwib_priv_io_exec_write,
                           &netwib_priv_io_exec_wait,
                           NULL,
                           &netwib_priv_io_exec_ctl_set,
                           &netwib_priv_io_exec_ctl_get,
                           &netwib_priv_io_exec_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}
