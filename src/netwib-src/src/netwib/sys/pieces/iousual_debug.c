/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
typedef struct {
  netwib_io *pnormalio;
  netwib_io *pdebugio;
  netwib_bool closeiosatend;
  netwib_bool havetodecread;
  netwib_bool havetodecwrite;
} netwib_priv_io_debug;

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_debug_init(netwib_io *pnormalio,
                                            netwib_io *pdebugio,
                                            netwib_bool closeiosatend,
                                            netwib_bool *preadinitialized,
                                            netwib_bool *pwriteinitialized,
                                            netwib_priv_io_debug *ptr)
{

  ptr->closeiosatend = closeiosatend;

  ptr->pdebugio = pdebugio;
  pdebugio->wr.numusers++;

  ptr->pnormalio = pnormalio;
  if (pnormalio->rd.supported) {
    *preadinitialized = NETWIB_TRUE;
    pnormalio->rd.numusers++;
    ptr->havetodecread = NETWIB_TRUE;
  } else {
    *preadinitialized = NETWIB_TRUE;
    ptr->havetodecread = NETWIB_FALSE;
  }
  if (pnormalio->wr.supported) {
    *pwriteinitialized = NETWIB_TRUE;
    pnormalio->wr.numusers++;
    ptr->havetodecwrite = NETWIB_TRUE;
  } else {
    *pwriteinitialized = NETWIB_TRUE;
    ptr->havetodecwrite = NETWIB_FALSE;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_debug_close(netwib_priv_io_debug *ptr)
{

  /* we do not use them */
  ptr->pdebugio->wr.numusers--;
  /* we have to use this saved value (because pnormalio might not
     support read now) */
  if (ptr->havetodecread) {
    ptr->pnormalio->rd.numusers--;
  }
  if (ptr->havetodecwrite) {
    ptr->pnormalio->wr.numusers--;
  }

  /* close io */
  if (ptr->closeiosatend) {
    netwib_er(netwib_io_close(&(ptr->pdebugio)));
    netwib_er(netwib_io_close(&(ptr->pnormalio)));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_debug_read(netwib_io *pio,
                                            netwib_buf *pbuf)
{
  netwib_priv_io_debug *ptr = (netwib_priv_io_debug *)pio->pcommon;
  netwib_byte bufarray[64];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_arraysizeofempty(bufarray, &buf));
  netwib_er(netwib_buf_append_fmt(&buf, "READ(%p,%p)\n", ptr->pnormalio,
                                  pbuf));
  netwib_er(netwib_io_write(ptr->pdebugio, &buf));

  ret = netwib_io_read(ptr->pnormalio, pbuf);

  netwib_er(netwib_buf_init_ext_arraysizeofempty(bufarray, &buf));
  netwib_er(netwib_buf_append_fmt(&buf, "READ:%p ret=%{uint32}\n",
                                  ptr->pnormalio, ret));
  netwib_er(netwib_io_write(ptr->pdebugio, &buf));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_debug_write(netwib_io *pio,
                                             netwib_constbuf *pbuf)
{
  netwib_priv_io_debug *ptr = (netwib_priv_io_debug *)pio->pcommon;
  netwib_uint32 datasize;
  netwib_byte bufarray[64];
  netwib_buf buf;
  netwib_err ret;

  datasize = netwib__buf_ref_data_size(pbuf);
  netwib_er(netwib_buf_init_ext_arraysizeofempty(bufarray, &buf));
  netwib_er(netwib_buf_append_fmt(&buf, "WRITE(%p,%p) [%{uint32} bytes]\n",
                                  ptr->pnormalio, pbuf, datasize));
  netwib_er(netwib_io_write(ptr->pdebugio, &buf));

  ret = netwib_io_write(ptr->pnormalio, pbuf);

  netwib_er(netwib_buf_init_ext_arraysizeofempty(bufarray, &buf));
  netwib_er(netwib_buf_append_fmt(&buf, "WRITE:%p ret=%{uint32}\n",
                                  ptr->pnormalio, ret));
  netwib_er(netwib_io_write(ptr->pdebugio, &buf));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_debug_wait(netwib_io *pio,
                                            netwib_io_waytype way,
                                            netwib_consttime *pabstime,
                                            netwib_bool *pevent)
{
  netwib_priv_io_debug *ptr = (netwib_priv_io_debug *)pio->pcommon;
  netwib_byte bufarray[128];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_arraysizeofempty(bufarray, &buf));
  netwib_er(netwib_buf_append_fmt(&buf, "WAIT(%p,%{uint32},",
                                  ptr->pnormalio, way));
  netwib_er(netwib_buf_append_time(pabstime, NETWIB_TIME_ENCODETYPE_BEST,
                                   &buf));
  netwib_er(netwib_buf_append_fmt(&buf, ",%p)\n", pevent));
  netwib_er(netwib_io_write(ptr->pdebugio, &buf));

  ret = netwib_io_wait(ptr->pnormalio, way, pabstime, pevent);

  netwib_er(netwib_buf_init_ext_arraysizeofempty(bufarray, &buf));
  netwib_er(netwib_buf_append_fmt(&buf, "WAIT:%p ret=%{uint32}\n",
                                  ptr->pnormalio, ret));
  netwib_er(netwib_io_write(ptr->pdebugio, &buf));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_debug_unread(netwib_io *pio,
                                              netwib_constbuf *pbuf)
{
  netwib_priv_io_debug *ptr = (netwib_priv_io_debug *)pio->pcommon;
  netwib_uint32 datasize;
  netwib_byte bufarray[64];
  netwib_buf buf;
  netwib_err ret;

  datasize = netwib__buf_ref_data_size(pbuf);
  netwib_er(netwib_buf_init_ext_arraysizeofempty(bufarray, &buf));
  netwib_er(netwib_buf_append_fmt(&buf, "UNREAD(%p,%p) [%{uint32} bytes]\n",
                                  ptr->pnormalio, pbuf, datasize));
  netwib_er(netwib_io_write(ptr->pdebugio, &buf));

  ret = netwib_io_unread(ptr->pnormalio, pbuf);

  netwib_er(netwib_buf_init_ext_arraysizeofempty(bufarray, &buf));
  netwib_er(netwib_buf_append_fmt(&buf, "UNREAD:%p ret=%{uint32}\n",
                                  ptr->pnormalio, ret));
  netwib_er(netwib_io_write(ptr->pdebugio, &buf));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_debug_ctl_set(netwib_io *pio,
                                               netwib_io_waytype way,
                                               netwib_io_ctltype type,
                                               netwib_ptr p,
                                               netwib_uint32 ui)
{
  netwib_priv_io_debug *ptr = (netwib_priv_io_debug *)pio->pcommon;
  netwib_byte bufarray[128];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_arraysizeofempty(bufarray, &buf));
  netwib_er(netwib_buf_append_fmt(&buf, "CTLTYPE_SET(%p,%{uint32},%{uint32}",
                                  ptr->pnormalio, way, type));
  netwib_er(netwib_buf_append_fmt(&buf, ",%p,%{uint32})\n", p, ui));
  netwib_er(netwib_io_write(ptr->pdebugio, &buf));

  ret = netwib_io_ctl_set(ptr->pnormalio, way, type, p, ui);

  netwib_er(netwib_buf_init_ext_arraysizeofempty(bufarray, &buf));
  netwib_er(netwib_buf_append_fmt(&buf, "CTLTYPE_SET:%p ret=%{uint32}\n",
                                  ptr->pnormalio, ret));
  netwib_er(netwib_io_write(ptr->pdebugio, &buf));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_debug_ctl_get(netwib_io *pio,
                                               netwib_io_waytype way,
                                               netwib_io_ctltype type,
                                               netwib_ptr p,
                                               netwib_uint32 *pui)
{
  netwib_priv_io_debug *ptr = (netwib_priv_io_debug *)pio->pcommon;
  netwib_byte bufarray[128];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_arraysizeofempty(bufarray, &buf));
  netwib_er(netwib_buf_append_fmt(&buf, "CTLTYPE_GET(%p,%{uint32},%{uint32}",
                                  ptr->pnormalio, way, type));
  netwib_er(netwib_buf_append_fmt(&buf, ",%p,%p)\n", p, pui));
  netwib_er(netwib_io_write(ptr->pdebugio, &buf));

  ret = netwib_io_ctl_get(ptr->pnormalio, way, type, p, pui);

  netwib_er(netwib_buf_init_ext_arraysizeofempty(bufarray, &buf));
  netwib_er(netwib_buf_append_fmt(&buf, "CTLTYPE_GET:%p ret=%{uint32}\n",
                                  ptr->pnormalio, ret));
  netwib_er(netwib_io_write(ptr->pdebugio, &buf));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_debug_fclose(netwib_io *pio)
{
  netwib_priv_io_debug *ptr = (netwib_priv_io_debug *)pio->pcommon;
  netwib_byte bufarray[64];
  netwib_buf buf;

  netwib_er(netwib_buf_init_ext_arraysizeofempty(bufarray, &buf));
  netwib_er(netwib_buf_append_fmt(&buf, "CLOSE(%p)\n", ptr->pnormalio));
  netwib_er(netwib_io_write(ptr->pdebugio, &buf));

  netwib_er(netwib_priv_io_debug_close(ptr));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_debug(netwib_io *pnormalio,
                                netwib_io *pdebugio,
                                netwib_bool closeiosatend,
                                netwib_io **ppio)
{
  netwib_bool rdsup, wrsup;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_debug), &pcommon));
  ret = netwib_priv_io_debug_init(pnormalio, pdebugio, closeiosatend,
                                  &rdsup, &wrsup,
                                  (netwib_priv_io_debug *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdsup, wrsup,
                           pcommon,
                           &netwib_priv_io_debug_read,
                           &netwib_priv_io_debug_write,
                           &netwib_priv_io_debug_wait,
                           &netwib_priv_io_debug_unread,
                           &netwib_priv_io_debug_ctl_set,
                           &netwib_priv_io_debug_ctl_get,
                           &netwib_priv_io_debug_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

