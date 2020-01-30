/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_null_read(netwib_io *pio,
                                           netwib_buf *pbuf)
{
  pio = pio; /* for compiler warning */
  pbuf = pbuf; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_null_write(netwib_io *pio,
                                            netwib_constbuf *pbuf)
{
  pio = pio; /* for compiler warning */
  pbuf = pbuf; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_null_wait(netwib_io *pio,
                                           netwib_io_waytype way,
                                           netwib_consttime *pabstime,
                                           netwib_bool *pevent)
{
  if (pevent != NULL) *pevent = NETWIB_TRUE;

  pio = pio; /* for compiler warning */
  way = way; /* for compiler warning */
  pabstime = pabstime; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_null_unread(netwib_io *pio,
                                             netwib_constbuf *pbuf)
{
  pio = pio; /* for compiler warning */
  pbuf = pbuf; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_null_ctl_set(netwib_io *pio,
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
      if (way != NETWIB_IO_WAYTYPE_WRITE) return(NETWIB_ERR_PAINVALIDTYPE);
      netwib_er(netwib_io_ctl_set_support(pio, way, NETWIB_FALSE));
      return(NETWIB_ERR_OK);
      break;
    default:
      return(NETWIB_ERR_PLEASETRYNEXT);
  }

  pio = pio; /* for compiler warning */
  way = way; /* for compiler warning */
  p = p; /* for compiler warning */
  ui = ui; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_null_ctl_get(netwib_io *pio,
                                              netwib_io_waytype way,
                                              netwib_io_ctltype type,
                                              netwib_ptr p,
                                              netwib_uint32 *pui)
{
  switch(type) {
    case NETWIB_IO_CTLTYPE_RES:
      if (pui != NULL) *pui = NETWIB_IO_RESTYPE_NULL;
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
static netwib_err netwib_priv_io_null_close(netwib_io *pio)
{
  pio = pio; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_null(netwib_io **ppio)
{
  netwib_er(netwib_io_init(NETWIB_TRUE, NETWIB_TRUE,
                           NULL,
                           netwib_priv_io_null_read,
                           netwib_priv_io_null_write,
                           netwib_priv_io_null_wait,
                           netwib_priv_io_null_unread,
                           netwib_priv_io_null_ctl_set,
                           netwib_priv_io_null_ctl_get,
                           netwib_priv_io_null_close,
                           ppio));

  return(NETWIB_ERR_OK);
}
