/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
typedef struct {
  netwib_buf readpkt;
  netwib_io *pio;
  netwib_device_dlttype dlt;
} netwib_priv_io_sniff_ip;

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sniff_ip_init(netwib_constbuf *pdevice,
                                               netwib_constbuf *pfilter,
                                               netwib_bool ipreas,
                                               netwib_bool tcpreord,
                                               netwib_bool *preadinitialized,
                                               netwib_bool *pwriteinitialized,
                                               netwib_priv_io_sniff_ip *ptr)
{
  netwib_io *pio2;

  /* create chain */
  netwib_er(netwib_io_init_sniff(pdevice, pfilter, &ptr->pio));
  if (ipreas || tcpreord) {
    netwib_er(netwib_io_init_sniff_ipreas(&pio2));
    netwib_er(netwib_io_plug_read(pio2, ptr->pio));
    ptr->pio = pio2;
  }
  if (tcpreord) {
    netwib_er(netwib_io_init_sniff_tcpreord(&pio2));
    netwib_er(netwib_io_plug_read(pio2, ptr->pio));
    ptr->pio = pio2;
  }

  /* set values */
  netwib_er(netwib_sniff_ctl_get_dlt(ptr->pio, &ptr->dlt));

  /* buffer used to retrieve packets */
  netwib_er(netwib_buf_init_mallocdefault(&ptr->readpkt));

  /* set io values */
  *preadinitialized = NETWIB_TRUE;
  *pwriteinitialized = NETWIB_FALSE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sniff_ip_close(netwib_priv_io_sniff_ip *ptr)
{
  netwib_er(netwib_buf_close(&ptr->readpkt));
  netwib_er(netwib_io_close(&ptr->pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sniff_ip_read(netwib_io *pio,
                                               netwib_buf *pbuf)
{
  netwib_priv_io_sniff_ip *ptr = (netwib_priv_io_sniff_ip *)pio->pcommon;
  netwib_linkhdrproto linkhdrproto;
  netwib_linkhdr linkhdr;
  netwib_err ret;

  netwib__buf_reinit(&ptr->readpkt);
  netwib_er(netwib_io_read(ptr->pio, &ptr->readpkt));

  ret = netwib_pkt_decode_layer_link(ptr->dlt, &ptr->readpkt, &linkhdr);
  if (ret == NETWIB_ERR_NOTCONVERTED || ret == NETWIB_ERR_DATAMISSING
      || ret == NETWIB_ERR_LONOTIMPLEMENTED) {
    /* ignore this packet */
    return(NETWIB_ERR_DATANOTAVAIL);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }
  netwib_er(netwib_linkhdr_get_proto(&linkhdr, &linkhdrproto));
  if (linkhdrproto != NETWIB_LINKHDRPROTO_IP4 &&
      linkhdrproto != NETWIB_LINKHDRPROTO_IP6) {
    /* ignore this packet */
    return(NETWIB_ERR_DATANOTAVAIL);
  }

  netwib_er(netwib_buf_append_buf(&ptr->readpkt, pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sniff_ip_wait(netwib_io *pio,
                                               netwib_io_waytype way,
                                               netwib_consttime *pabstime,
                                               netwib_bool *pevent)
{
  netwib_priv_io_sniff_ip *ptr = (netwib_priv_io_sniff_ip *)pio->pcommon;

  netwib_er(netwib_io_wait(ptr->pio, way, pabstime, pevent));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sniff_ip_ctl_set(netwib_io *pio,
                                                  netwib_io_waytype way,
                                                  netwib_io_ctltype type,
                                                  netwib_ptr p,
                                                  netwib_uint32 ui)
{
  netwib_priv_io_sniff_ip *ptr = (netwib_priv_io_sniff_ip *)pio->pcommon;

  netwib_er(netwib_io_ctl_set(ptr->pio, way, type, p, ui));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sniff_ip_ctl_get(netwib_io *pio,
                                                  netwib_io_waytype way,
                                                  netwib_io_ctltype type,
                                                  netwib_ptr p,
                                                  netwib_uint32 *pui)
{

  switch(type) {
    case NETWIB_IO_CTLTYPE_SNIFF_DLT:
      if (pui != NULL) {
        *pui = NETWIB_DEVICE_DLTTYPE_RAW;
      }
      return(NETWIB_ERR_OK);
    default:
      return(NETWIB_ERR_PLEASETRYNEXT);
  }

  pio = pio; /* for compiler warning */
  way = way; /* for compiler warning */
  p = p; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sniff_ip_fclose(netwib_io *pio)
{
  netwib_priv_io_sniff_ip *ptr = (netwib_priv_io_sniff_ip *)pio->pcommon;

  netwib_er(netwib_priv_io_sniff_ip_close(ptr));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_sniff_ip(netwib_constbuf *pdevice,
                                   netwib_constbuf *pfilter,
                                   netwib_bool ipreas,
                                   netwib_bool tcpreord,
                                   netwib_io **ppio)
{
  netwib_bool rdinit, wrinit;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_sniff_ip), &pcommon));
  ret = netwib_priv_io_sniff_ip_init(pdevice, pfilter, ipreas, tcpreord,
                                     &rdinit, &wrinit,
                                     (netwib_priv_io_sniff_ip *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           &netwib_priv_io_sniff_ip_read,
                           NULL, /* write */
                           &netwib_priv_io_sniff_ip_wait,
                           NULL, /* unread */
                           &netwib_priv_io_sniff_ip_ctl_set,
                           &netwib_priv_io_sniff_ip_ctl_get,
                           &netwib_priv_io_sniff_ip_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

