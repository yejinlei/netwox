/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
typedef struct {
  HANDLE readhandle;
  HANDLE writehandle;
  HANDLE processhandle;
  netwib_bool killonclose;
  netwib_bool *pexitednormally;
  netwib_uint32 *preturnedvalue;
} netwib_priv_io_exec;

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_exec_init3(netwib_string cmd,
                                            HANDLE stdinrd,
                                            HANDLE stdoutwr,
                                            HANDLE *pprocess)
{
  PROCESS_INFORMATION pi;
  STARTUPINFO si;
  HANDLE hnull;

  hnull = CreateFile("NUL", GENERIC_READ|GENERIC_WRITE,
                     FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
                     OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
                     NULL);
  if (hnull == INVALID_HANDLE_VALUE) return(NETWIB_ERR_FUCREATEFILE);

  ZeroMemory(&si, sizeof(STARTUPINFO));
  si.cb = sizeof(STARTUPINFO);
  si.dwFlags = STARTF_USESTDHANDLES;
  si.hStdInput  = (stdinrd==INVALID_HANDLE_VALUE)?hnull:stdinrd;
  si.hStdOutput = (stdoutwr==INVALID_HANDLE_VALUE)?hnull:stdoutwr;
  si.hStdError  = hnull;
  if (!CreateProcess(NULL, cmd, NULL, NULL, TRUE,
                     0, NULL, NULL, &si, &pi))
    return(NETWIB_ERR_LOCANTEXEC);

  if (!CloseHandle(hnull)) return(NETWIB_ERR_FUCLOSEHANDLE);

  *pprocess = pi.hProcess;
  /* we do not need the handle to the first thread in the process */
  if (!CloseHandle(pi.hThread)) return(NETWIB_ERR_FUCLOSEHANDLE);
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_exec_init2(netwib_string cmd,
                                            HANDLE *phrd,
                                            HANDLE *phwr,
                                            HANDLE *pprocess)
{
  HANDLE stdoutrdtmp, stdoutrd, stdoutwr;
  HANDLE stdinwrtmp, stdinrd, stdinwr;
  SECURITY_ATTRIBUTES sa;
  netwib_err ret;

  sa.nLength = sizeof(SECURITY_ATTRIBUTES);
  sa.lpSecurityDescriptor = NULL;
  sa.bInheritHandle = TRUE;

  /* create pipes, then create new handles which are uninheritable */
  stdoutrd = INVALID_HANDLE_VALUE;
  stdoutwr = INVALID_HANDLE_VALUE;
  if (phrd != NULL) {
    if (!CreatePipe(&stdoutrdtmp, &stdoutwr, &sa, 0))
      return(NETWIB_ERR_FUCREATEPIPE);
    if (!DuplicateHandle(GetCurrentProcess(), stdoutrdtmp,
                         GetCurrentProcess(), &stdoutrd,
                         0, FALSE, DUPLICATE_SAME_ACCESS))
      return(NETWIB_ERR_FUDUPLICATEHANDLE);
    if (!CloseHandle(stdoutrdtmp)) return(NETWIB_ERR_FUCLOSEHANDLE);
  }
  stdinrd = INVALID_HANDLE_VALUE;
  stdinwr = INVALID_HANDLE_VALUE;
  if (phwr != NULL) {
    if (!CreatePipe(&stdinrd, &stdinwrtmp, &sa, 0))
      return(NETWIB_ERR_FUCREATEPIPE);
    if (!DuplicateHandle(GetCurrentProcess(), stdinwrtmp,
                         GetCurrentProcess(), &stdinwr,
                         0, FALSE, DUPLICATE_SAME_ACCESS))
      return(NETWIB_ERR_FUDUPLICATEHANDLE);
    if (!CloseHandle(stdinwrtmp)) return(NETWIB_ERR_FUCLOSEHANDLE);
  }

  ret = netwib_priv_io_exec_init3(cmd, stdinrd, stdoutwr, pprocess);

  if (phrd != NULL) {
    if (!CloseHandle(stdoutwr)) return(NETWIB_ERR_FUCLOSEHANDLE);
    if (ret == NETWIB_ERR_OK) {
      *phrd = stdoutrd;
    } else {
      CloseHandle(stdoutrd);
    }
  }
  if (phwr != NULL) {
    if (!CloseHandle(stdinrd)) return(NETWIB_ERR_FUCLOSEHANDLE);
    if (ret == NETWIB_ERR_OK) {
      *phwr = stdinwr;
    } else {
      CloseHandle(stdinwr);
    }
  }

  return(ret);
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
  netwib_bool provideread, providewrite;
  netwib_buf bufcmd;
  netwib_string strcmd;
  HANDLE readhandle;
  HANDLE writehandle;
  HANDLE processhandle;
  netwib_err ret;

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

  /* try to launch child */
  netwib_er(netwib_buf_init_mallocdefault(&bufcmd));
  netwib_er(netwib_priv_cmdline_win(pbufcommand, &bufcmd));
  netwib_er(netwib_buf_ref_string(&bufcmd, &strcmd));
  ret = netwib_priv_io_exec_init2(strcmd,
      provideread?&readhandle:NULL,
      providewrite?&writehandle:NULL,
      &processhandle);
  netwib_er(netwib_buf_close(&bufcmd));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  /* store information */
  ptr->readhandle = provideread?readhandle:INVALID_HANDLE_VALUE;
  ptr->writehandle = providewrite?writehandle:INVALID_HANDLE_VALUE;
  ptr->processhandle = processhandle;
  ptr->killonclose = killonclose;
  ptr->pexitednormally = pexitednormally;
  ptr->preturnedvalue = preturnedvalue;
  *preadinitialized = provideread;
  *pwriteinitialized = providewrite;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_exec_close(netwib_priv_io_exec *ptr)
{
  netwib_bool waskilled;
  DWORD exitcode, err;

  /* obtain exit code */
  if (!GetExitCodeProcess(ptr->processhandle, &exitcode))
    return(NETWIB_ERR_FUGETEXITCODEPROCESS);

  /* eventually kill */
  waskilled = NETWIB_FALSE;
  if (exitcode == STILL_ACTIVE) {
    if (ptr->killonclose) {
      if (!TerminateProcess(ptr->processhandle, NETWIB_ERR_DATAEND)) {
        /* ignore error when process has already terminated */
        if (GetLastError() != ERROR_INVALID_PARAMETER) {
          return(NETWIB_ERR_FUTERMINATEPROCESS);
        }
      }
      waskilled = NETWIB_TRUE;
    }
    err = WaitForSingleObject(ptr->processhandle, INFINITE);
    if (err == WAIT_FAILED)
      return(NETWIB_ERR_FUWAITFORSINGLEOBJECT);
    if (!GetExitCodeProcess(ptr->processhandle, &exitcode))
      return(NETWIB_ERR_FUGETEXITCODEPROCESS);
  }

  /* set values */
  if (waskilled) {
    if (ptr->pexitednormally != NULL) {
      *(ptr->pexitednormally) = NETWIB_FALSE;
    }
  } else {
    if (ptr->pexitednormally != NULL) {
      *(ptr->pexitednormally) = NETWIB_TRUE;
    }
    if (ptr->preturnedvalue != NULL) {
      *(ptr->preturnedvalue) = exitcode;
    }
  }

  /* close descriptors */
  if (ptr->readhandle != INVALID_HANDLE_VALUE) {
    if (!CloseHandle(ptr->readhandle)) return(NETWIB_ERR_FUCLOSEHANDLE);
  }
  if (ptr->writehandle != INVALID_HANDLE_VALUE) {
    if (!CloseHandle(ptr->writehandle)) return(NETWIB_ERR_FUCLOSEHANDLE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_exec_read(netwib_io *pio,
                                           netwib_buf *pbuf)
{
  netwib_priv_io_exec *ptr = (netwib_priv_io_exec *)pio->pcommon;
  return(netwib_priv_handle_read(ptr->readhandle, pbuf));
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_exec_write(netwib_io *pio,
                                            netwib_constbuf *pbuf)
{
  netwib_priv_io_exec *ptr = (netwib_priv_io_exec *)pio->pcommon;
  return(netwib_priv_handle_write(ptr->writehandle, pbuf));
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_exec_wait(netwib_io *pio,
                                           netwib_io_waytype way,
                                           netwib_consttime *pabstime,
                                           netwib_bool *pevent)
{
  netwib_priv_io_exec *ptr = (netwib_priv_io_exec *)pio->pcommon;

  switch (way) {
    case NETWIB_IO_WAYTYPE_READ :
      netwib_er(netwib_priv_handle_wait(ptr->readhandle,
                                        NETWIB_HANDLETYPE_PIPE,
                                        way, pabstime, pevent));
      break;
    case NETWIB_IO_WAYTYPE_WRITE :
      netwib_er(netwib_priv_handle_wait(ptr->writehandle,
                                        NETWIB_HANDLETYPE_PIPE,
                                        way, pabstime, pevent));
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
static netwib_err netwib_priv_io_exec_ctl_set(netwib_io *pio,
                                              netwib_io_waytype way,
                                              netwib_io_ctltype type,
                                              netwib_ptr p,
                                              netwib_uint32 ui)
{
  netwib_priv_io_exec *ptr = (netwib_priv_io_exec *)pio->pcommon;
  netwib_buf buf;

  switch(type) {
    case NETWIB_IO_CTLTYPE_RES:
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    case NETWIB_IO_CTLTYPE_END:
      if (way != NETWIB_IO_WAYTYPE_WRITE) return(NETWIB_ERR_PAINVALIDTYPE);
      if (ptr->writehandle != INVALID_HANDLE_VALUE) {
                    /* try to send Control-Z : this might not work */
        netwib_er(netwib_buf_init_malloc(10, &buf));
        netwib_er(netwib_buf_append_byte(0x1A, &buf));
        netwib_er(netwib_priv_handle_write(ptr->writehandle, &buf));
        netwib_er(netwib_buf_close(&buf));
      }
      netwib_er(netwib_io_ctl_set_support(pio, way, NETWIB_FALSE));
      return(NETWIB_ERR_OK);
      break;
    default:
      return(NETWIB_ERR_PLEASETRYNEXT);
  }

  way = way; /* for compiler warning */
  p = p; /* for compiler warning */
  ui = ui; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_exec_ctl_get(netwib_io *pio,
                                              netwib_io_waytype way,
                                              netwib_io_ctltype type,
                                              netwib_ptr p,
                                              netwib_uint32 *pui)
{

  switch(type) {
    case NETWIB_IO_CTLTYPE_RES:
      if (pui != NULL) *pui = NETWIB_IO_RESTYPE_EXEC;
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
  ret = netwib_priv_io_exec_init(pbufcommand, providedway, killonclose,
                                 pexitednormally, preturnedvalue,
                                 &rdsup, &wrsup,
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
