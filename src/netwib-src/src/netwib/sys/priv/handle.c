/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_handle_type(NETWIBHANDLE h,
                                   netwib_handletype *phandletype)
{
  DWORD ftype;
  netwib_handletype htype;

  htype = NETWIB_HANDLETYPE_UNKNOWN;
  ftype = GetFileType(h);
  if (ftype == FILE_TYPE_CHAR) {
    htype = NETWIB_HANDLETYPE_CONSOLE;
  } else if (ftype == FILE_TYPE_DISK) {
    htype = NETWIB_HANDLETYPE_FILE;
  } else if (ftype == FILE_TYPE_PIPE) {
    htype = NETWIB_HANDLETYPE_PIPE;
  }

  if (phandletype != NULL) *phandletype = htype;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_handle_write(NETWIBHANDLE h,
                                    netwib_constbuf *pbuf)
{
  netwib_data data;
  netwib_uint32 datasize;
  DWORD writtensize;
  BOOL err;

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);

  /*nothing to write */
  if (datasize == 0) {
    return(NETWIB_ERR_OK);
  }

  /*write a block */
  err = WriteFile(h, data, datasize, &writtensize, NULL);
  if (!err) {
    return(NETWIB_ERR_FUWRITEFILE);
  }

  if (writtensize != datasize) {
    return(NETWIB_ERR_FUWRITEFILE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_handle_read(NETWIBHANDLE h,
                                   netwib_buf *pbuf)
{
  netwib_data data;
  netwib_uint32 maxmsgsize;
  ULONG readsize;
  BOOL err;

  netwib_er(netwib_buf_wishspace(pbuf, NETWIB_PRIV_MINMSGSIZE_READ,
                               &data, &maxmsgsize));
  if (maxmsgsize == 0) {
    return(NETWIB_ERR_DATANOSPACE);
  }

  err = ReadFile(h, data, maxmsgsize, &readsize, NULL);
  if (!err) {
    return(NETWIB_ERR_DATAEND);
  }
  if (readsize == 0) {
    return(NETWIB_ERR_DATAEND);
  }

  /* store read data */
  pbuf->endoffset += readsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_handle_read_byte(NETWIBHANDLE h,
                                        netwib_byte *pb)
{
  netwib_byte data[1];
  ULONG readsize;
  BOOL err;

  err = ReadFile(h, data, 1, &readsize, NULL);
  if (!err) {
    return(NETWIB_ERR_DATAEND);
  }
  if (readsize == 0) {
    return(NETWIB_ERR_DATAEND);
  }

  /* store read data */
  if (pb != NULL) *pb = data[0];

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_handle_wait(NETWIBHANDLE h,
                                   netwib_handletype handletype,
                                   netwib_io_waytype way,
                                   netwib_consttime *pabstime,
                                   netwib_bool *pevent)
{
  DWORD err, msec;
  DWORD nbevent;
  DWORD n, i;
  DWORD readbytes, bytesavail;
  INPUT_RECORD ir[50];
  netwib_bool event, elapsed;

  if (pabstime != NETWIB_TIME_ZERO && pabstime != NETWIB_TIME_INFINITE) {
    netwib_er(netwib_time_iselapsed(pabstime, &elapsed));
    if (elapsed) {
      if (pevent != NULL) *pevent = NETWIB_FALSE;
      return(NETWIB_ERR_OK);
    }
  }

  event = NETWIB_FALSE;
  switch (handletype) {
    case NETWIB_HANDLETYPE_FILE:
      /* it's a filehandle for a disk file, for which there is
         only data, or endoffile. So there is no need to wait. */
      event = NETWIB_TRUE;
      break;

    case NETWIB_HANDLETYPE_CONSOLE:
      switch(way) {
        case NETWIB_IO_WAYTYPE_READ:
        case NETWIB_IO_WAYTYPE_RDWR:
        case NETWIB_IO_WAYTYPE_SUPPORTED:
          break;
        case NETWIB_IO_WAYTYPE_WRITE:
        case NETWIB_IO_WAYTYPE_NONE:
          return(NETWIB_ERR_PAINVALIDTYPE);
          break;
      }
      /* wait during the specified time */
      netwib_er(netwib_priv_time_timeout_winwait(pabstime, &msec));
      err = WaitForSingleObject(h, msec);
      if (err == WAIT_FAILED)
        return(NETWIB_ERR_FUWAITFORSINGLEOBJECT);
      /* no data to read */
      if (err == WAIT_TIMEOUT)
        event = NETWIB_FALSE;
      else if (err == WAIT_OBJECT_0)
        event = NETWIB_TRUE;
      else
        return(NETWIB_ERR_FUWAITFORSINGLEOBJECT);
      if (event) {
        /* check there is really data */
        err = GetNumberOfConsoleInputEvents(h, &nbevent);
        if (err) {
          err = PeekConsoleInput(h, ir, 50, &n);
          if (!err)
            return(NETWIB_ERR_FUPEEKCONSOLEINPUT);
          event = NETWIB_FALSE;
          for (i = 0; i < n; i++) {
            if (ir[i].EventType == KEY_EVENT &&
                ir[i].Event.KeyEvent.bKeyDown) {
              event = NETWIB_TRUE;
              break;
            }
          }
        }
      }
      break;

    case NETWIB_HANDLETYPE_PIPE:
      err = PeekNamedPipe(h, NULL, 0xFFFF, &readbytes, &bytesavail, NULL);
      if (!err) {
        /* end event */
        event = NETWIB_TRUE;
      } else {
        if (readbytes || bytesavail)
          event = NETWIB_TRUE;
      }
      if (!event) return(NETWIB_ERR_PLEASELOOPTIME);
      break;

    case NETWIB_HANDLETYPE_UNKNOWN:
      /* wait during the specified time */
      netwib_er(netwib_priv_time_timeout_winwait(pabstime, &msec));
      err = WaitForSingleObjectEx(h, msec, TRUE);
      if (err == WAIT_FAILED)
        return(NETWIB_ERR_FUWAITFORSINGLEOBJECT);
      /* no data to read */
      if (err == WAIT_TIMEOUT)
        event = NETWIB_FALSE;
      else if (err == WAIT_OBJECT_0)
        event = NETWIB_TRUE;
      else if (err == WAIT_ABANDONED)
        event = NETWIB_TRUE;
      else if (err == WAIT_IO_COMPLETION)
        event = NETWIB_TRUE;
      else
        return(NETWIB_ERR_FUWAITFORSINGLEOBJECT);
      break;

    default:
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  if (pevent != NULL) *pevent = event;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_handle_waitsimple(NETWIBHANDLE h,
                                         netwib_consttime *pabstime,
                                         netwib_bool *pevent)
{
  DWORD err, msec;
  netwib_bool event;

  event = NETWIB_FALSE;

  /* wait during the specified time */
  netwib_er(netwib_priv_time_timeout_winwait(pabstime, &msec));
  err = WaitForSingleObjectEx(h, msec, TRUE);
  if (err == WAIT_FAILED)
    return(NETWIB_ERR_FUWAITFORSINGLEOBJECT);
  /* no data to read */
  if (err == WAIT_TIMEOUT)
    event = NETWIB_FALSE;
  else if (err == WAIT_OBJECT_0)
    event = NETWIB_TRUE;
  else if (err == WAIT_ABANDONED)
    event = NETWIB_TRUE;
  else if (err == WAIT_IO_COMPLETION)
    event = NETWIB_TRUE;
  else
    return(NETWIB_ERR_FUWAITFORSINGLEOBJECT);

  if (pevent != NULL) *pevent = event;
  return(NETWIB_ERR_OK);
}
