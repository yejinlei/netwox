/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
typedef struct {
  netwib_io *preadio;
  netwib_io *pwriteio;
  netwib_bool closeiosatend;
} netwib_priv_io_rdwr;

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_rdwr_init(netwib_io *preadio,
                                           netwib_io *pwriteio,
                                           netwib_bool closeiosatend,
                                           netwib_bool *preadinitialized,
                                           netwib_bool *pwriteinitialized,
                                           netwib_priv_io_rdwr *ptr)
{

  ptr->preadio = preadio;
  ptr->pwriteio = pwriteio;
  ptr->closeiosatend = closeiosatend;
  *preadinitialized = NETWIB_TRUE;
  *pwriteinitialized = NETWIB_TRUE;

  /* we use them */
  preadio->rd.numusers++;
  pwriteio->wr.numusers++;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_rdwr_close(netwib_priv_io_rdwr *ptr)
{
  /* we do not use them */
  ptr->preadio->rd.numusers--;
  ptr->pwriteio->wr.numusers--;

  /* close io */
  if (ptr->closeiosatend) {
    netwib_er(netwib_io_close(&(ptr->preadio)));
    netwib_er(netwib_io_close(&(ptr->pwriteio)));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_rdwr_read(netwib_io *pio,
                                           netwib_buf *pbuf)
{
  netwib_priv_io_rdwr *ptr = (netwib_priv_io_rdwr *)pio->pcommon;

  netwib_er(netwib_io_read(ptr->preadio, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_rdwr_write(netwib_io *pio,
                                            netwib_constbuf *pbuf)
{
  netwib_priv_io_rdwr *ptr = (netwib_priv_io_rdwr *)pio->pcommon;

  netwib_er(netwib_io_write(ptr->pwriteio, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_rdwr_wait(netwib_io *pio,
                                           netwib_io_waytype way,
                                           netwib_consttime *pabstime,
                                           netwib_bool *pevent)
{
  netwib_priv_io_rdwr *ptr = (netwib_priv_io_rdwr *)pio->pcommon;

  switch (way) {
    case NETWIB_IO_WAYTYPE_READ :
      netwib_er(netwib_io_wait(ptr->preadio, NETWIB_IO_WAYTYPE_READ,
                               pabstime, pevent));
      break;
    case NETWIB_IO_WAYTYPE_WRITE :
      netwib_er(netwib_io_wait(ptr->pwriteio, NETWIB_IO_WAYTYPE_WRITE,
                               pabstime, pevent));
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
static netwib_err netwib_priv_io_rdwr_unread(netwib_io *pio,
                                             netwib_constbuf *pbuf)
{
  netwib_priv_io_rdwr *ptr = (netwib_priv_io_rdwr *)pio->pcommon;

  netwib_er(netwib_io_unread(ptr->preadio, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_rdwr_ctl_set(netwib_io *pio,
                                              netwib_io_waytype way,
                                              netwib_io_ctltype type,
                                              netwib_ptr p,
                                              netwib_uint32 ui)
{
  netwib_priv_io_rdwr *ptr = (netwib_priv_io_rdwr *)pio->pcommon;

  if (type == NETWIB_IO_CTLTYPE_END) {
    if (way != NETWIB_IO_WAYTYPE_WRITE) return(NETWIB_ERR_PAINVALIDTYPE);
    netwib_er(netwib_io_ctl_set(ptr->pwriteio, way, type, p, ui));
    netwib_er(netwib_io_ctl_set_support(pio, way, NETWIB_FALSE));
    return(NETWIB_ERR_OK);
  }

  switch(way) {
    case NETWIB_IO_WAYTYPE_READ :
      netwib_er(netwib_io_ctl_set(ptr->preadio, way, type, p, ui));
      break;
    case NETWIB_IO_WAYTYPE_WRITE :
      netwib_er(netwib_io_ctl_set(ptr->pwriteio, way, type, p, ui));
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
static netwib_err netwib_priv_io_rdwr_ctl_get(netwib_io *pio,
                                              netwib_io_waytype way,
                                              netwib_io_ctltype type,
                                              netwib_ptr p,
                                              netwib_uint32 *pui)
{
  netwib_priv_io_rdwr *ptr = (netwib_priv_io_rdwr *)pio->pcommon;

  switch(way) {
    case NETWIB_IO_WAYTYPE_READ :
      netwib_er(netwib_io_ctl_get(ptr->preadio, NETWIB_IO_WAYTYPE_READ,
                                  type, p, pui));
      break;
    case NETWIB_IO_WAYTYPE_WRITE :
      netwib_er(netwib_io_ctl_get(ptr->pwriteio, NETWIB_IO_WAYTYPE_WRITE,
                                  type, p, pui));
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
static netwib_err netwib_priv_io_rdwr_fclose(netwib_io *pio)
{
  netwib_priv_io_rdwr *ptr = (netwib_priv_io_rdwr *)pio->pcommon;

  netwib_er(netwib_priv_io_rdwr_close(ptr));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_rdwr(netwib_io *preadio,
                               netwib_io *pwriteio,
                               netwib_bool closeiosatend,
                               netwib_io **ppio)
{
  netwib_bool rdsup, wrsup;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_rdwr), &pcommon));
  ret = netwib_priv_io_rdwr_init(preadio, pwriteio, closeiosatend,
                                 &rdsup, &wrsup,
                                 (netwib_priv_io_rdwr *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdsup, wrsup,
                         pcommon,
                         &netwib_priv_io_rdwr_read,
                         &netwib_priv_io_rdwr_write,
                         &netwib_priv_io_rdwr_wait,
                         &netwib_priv_io_rdwr_unread,
                         &netwib_priv_io_rdwr_ctl_set,
                         &netwib_priv_io_rdwr_ctl_get,
                         &netwib_priv_io_rdwr_fclose,
                         ppio));

  return(NETWIB_ERR_OK);
}

