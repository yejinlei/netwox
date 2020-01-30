/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
typedef struct {
  netwib_io *pio1;
  netwib_io *pio2;
  netwib_bool closeiosatend;
} netwib_priv_io_tee;

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_tee_init(netwib_io *pio1,
                                          netwib_io *pio2,
                                          netwib_bool closeiosatend,
                                          netwib_bool *preadinitialized,
                                          netwib_bool *pwriteinitialized,
                                          netwib_priv_io_tee *ptr)
{

  ptr->pio1 = pio1;
  ptr->pio2 = pio2;
  ptr->closeiosatend = closeiosatend;
  *preadinitialized = NETWIB_TRUE;
  *pwriteinitialized = NETWIB_TRUE;

  /* we use them */
  netwib_er(netwib_io_ctl_set_numusers_inc(pio1, NETWIB_IO_WAYTYPE_RDWR));
  netwib_er(netwib_io_ctl_set_numusers_inc(pio2, NETWIB_IO_WAYTYPE_RDWR));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_tee_close(netwib_priv_io_tee *ptr)
{

  /* we do not use them */
  netwib_er(netwib_io_ctl_set_numusers_dec(ptr->pio1, NETWIB_IO_WAYTYPE_RDWR));
  netwib_er(netwib_io_ctl_set_numusers_dec(ptr->pio2, NETWIB_IO_WAYTYPE_RDWR));

  /* close io */
  if (ptr->closeiosatend) {
    netwib_er(netwib_io_close(&(ptr->pio1)));
    netwib_er(netwib_io_close(&(ptr->pio2)));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_tee_read(netwib_io *pio,
                                          netwib_buf *pbuf)
{
  netwib_priv_io_tee *ptr = (netwib_priv_io_tee *)pio->pcommon;
  netwib_bool b;

  netwib_er(netwib_io_wait(ptr->pio1, NETWIB_IO_WAYTYPE_READ,
                           NETWIB_TIME_ZERO, &b));
  if (b) {
    netwib_er(netwib_io_read(ptr->pio1, pbuf));
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_io_wait(ptr->pio2, NETWIB_IO_WAYTYPE_READ,
                           NETWIB_TIME_ZERO, &b));
  if (b) {
    netwib_er(netwib_io_read(ptr->pio2, pbuf));
    return(NETWIB_ERR_OK);
  }

  return(NETWIB_ERR_DATANOTAVAIL);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_tee_write(netwib_io *pio,
                                           netwib_constbuf *pbuf)
{
  netwib_priv_io_tee *ptr = (netwib_priv_io_tee *)pio->pcommon;

  netwib_er(netwib_io_write(ptr->pio1, pbuf));
  netwib_er(netwib_io_write(ptr->pio2, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_tee_wait(netwib_io *pio,
                                          netwib_io_waytype way,
                                          netwib_consttime *pabstime,
                                          netwib_bool *pevent)
{
  netwib_priv_io_tee *ptr = (netwib_priv_io_tee *)pio->pcommon;
  netwib_bool b;

  netwib_er(netwib_io_wait(ptr->pio1, way, pabstime, &b));
  if (b) {
    if (pevent != NULL) *pevent = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_io_wait(ptr->pio2, way, pabstime, &b));
  if (b) {
    if (pevent != NULL) *pevent = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  if (pevent != NULL) *pevent = NETWIB_FALSE;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_tee_unread(netwib_io *pio,
                                            netwib_constbuf *pbuf)
{
  netwib_priv_io_tee *ptr = (netwib_priv_io_tee *)pio->pcommon;

  netwib_er(netwib_io_unread(ptr->pio1, pbuf));
  netwib_er(netwib_io_unread(ptr->pio2, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_tee_ctl_set(netwib_io *pio,
                                             netwib_io_waytype way,
                                             netwib_io_ctltype type,
                                             netwib_ptr p,
                                             netwib_uint32 ui)
{
  netwib_priv_io_tee *ptr = (netwib_priv_io_tee *)pio->pcommon;

  netwib_er(netwib_io_ctl_set(ptr->pio1, way, type, p, ui));
  netwib_er(netwib_io_ctl_set(ptr->pio2, way, type, p, ui));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_tee_ctl_get(netwib_io *pio,
                                             netwib_io_waytype way,
                                             netwib_io_ctltype type,
                                             netwib_ptr p,
                                             netwib_uint32 *pui)
{
  netwib_priv_io_tee *ptr = (netwib_priv_io_tee *)pio->pcommon;
  netwib_err ret;

  ret = netwib_io_ctl_get(ptr->pio1, way, type, p, pui);
  if (ret == NETWIB_ERR_OK) return(NETWIB_ERR_OK);

  netwib_er(netwib_io_ctl_get(ptr->pio2, way, type, p, pui));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_tee_fclose(netwib_io *pio)
{
  netwib_priv_io_tee *ptr = (netwib_priv_io_tee *)pio->pcommon;

  netwib_er(netwib_priv_io_tee_close(ptr));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_tee(netwib_io *pio1,
                              netwib_io *pio2,
                              netwib_bool closeiosatend,
                              netwib_io **ppio)
{
  netwib_bool rdsup, wrsup;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_tee), &pcommon));
  ret = netwib_priv_io_tee_init(pio1, pio2, closeiosatend,
                                &rdsup, &wrsup,
                                (netwib_priv_io_tee *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdsup, wrsup,
                           pcommon,
                           &netwib_priv_io_tee_read,
                           &netwib_priv_io_tee_write,
                           &netwib_priv_io_tee_wait,
                           &netwib_priv_io_tee_unread,
                           &netwib_priv_io_tee_ctl_set,
                           &netwib_priv_io_tee_ctl_get,
                           &netwib_priv_io_tee_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

