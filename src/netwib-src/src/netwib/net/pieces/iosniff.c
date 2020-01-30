/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
typedef struct {
  netwib_priv_libpcap libpcap;
} netwib_priv_io_sniff;

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sniff_init(netwib_constbuf *pdevice,
                                            netwib_constbuf *pfilter,
                                            netwib_bool *preadinitialized,
                                            netwib_bool *pwriteinitialized,
                                            netwib_priv_io_sniff *ptr)
{
  netwib_err ret;

  /* open device */
  netwib_er(netwib_priv_libpcap_init_sniff(pdevice, &ptr->libpcap));

  /* set filter */
  netwib_eg(netwib_priv_libpcap_set_filter(&ptr->libpcap, pfilter));

  /* prepare for wait */
  netwib_eg(netwib_priv_libpcap_set_nonblock(&ptr->libpcap));

  /* obtain DLT type */
  netwib_eg(netwib_priv_libpcap_get_dlt(&ptr->libpcap));

  /* set io values */
  *preadinitialized = NETWIB_TRUE;
  *pwriteinitialized = NETWIB_FALSE;

  return(NETWIB_ERR_OK);

  /* reached on error */
 netwib_gotolabel:
  netwib_er(netwib_priv_libpcap_close(&ptr->libpcap));
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sniff_close(netwib_priv_io_sniff *ptr)
{
  netwib_er(netwib_priv_libpcap_close(&ptr->libpcap));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sniff_read(netwib_io *pio,
                                            netwib_buf *pbuf)
{
  netwib_priv_io_sniff *ptr = (netwib_priv_io_sniff *)pio->pcommon;

  netwib_er(netwib_priv_libpcap_read(&ptr->libpcap, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sniff_wait(netwib_io *pio,
                                            netwib_io_waytype way,
                                            netwib_consttime *pabstime,
                                            netwib_bool *pevent)
{
  netwib_priv_io_sniff *ptr = (netwib_priv_io_sniff *)pio->pcommon;

  netwib_er(netwib_priv_libpcap_wait(&ptr->libpcap, way, pabstime,
                                     pevent));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sniff_ctl_set(netwib_io *pio,
                                               netwib_io_waytype way,
                                               netwib_io_ctltype type,
                                               netwib_ptr p,
                                               netwib_uint32 ui)
{
  netwib_priv_io_sniff *ptr = (netwib_priv_io_sniff *)pio->pcommon;

  switch(type) {
    case NETWIB_IO_CTLTYPE_SNIFF_FILTER:
      netwib_er(netwib_priv_libpcap_set_filter(&ptr->libpcap,
                                               (netwib_constbuf*)p));
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IO_CTLTYPE_SNIFF_DLT:
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    default:
      return(NETWIB_ERR_PLEASETRYNEXT);
  }

  way = way; /* for compiler warning */
  ui = ui; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sniff_ctl_get(netwib_io *pio,
                                               netwib_io_waytype way,
                                               netwib_io_ctltype type,
                                               netwib_ptr p,
                                               netwib_uint32 *pui)
{
  netwib_priv_io_sniff *ptr = (netwib_priv_io_sniff *)pio->pcommon;

  switch(type) {
    case NETWIB_IO_CTLTYPE_SNIFF_FILTER:
      if (p != NULL) {
        netwib_er(netwib_buf_append_buf(&ptr->libpcap.filter,
                                        (netwib_buf*)&p));
      }
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IO_CTLTYPE_SNIFF_DLT:
      if (pui != NULL) *pui = ptr->libpcap.dlttype;
      return(NETWIB_ERR_OK);
    default:
      return(NETWIB_ERR_PLEASETRYNEXT);
  }

  way = way; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sniff_fclose(netwib_io *pio)
{
  netwib_priv_io_sniff *ptr = (netwib_priv_io_sniff *)pio->pcommon;

  netwib_er(netwib_priv_io_sniff_close(ptr));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_sniff(netwib_constbuf *pdevice,
                                netwib_constbuf *pfilter,
                                netwib_io **ppio)
{
  netwib_bool rdinit=NETWIB_FALSE, wrinit=NETWIB_FALSE;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_sniff), &pcommon));
  ret = netwib_priv_io_sniff_init(pdevice, pfilter, &rdinit, &wrinit,
                                  (netwib_priv_io_sniff *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           &netwib_priv_io_sniff_read,
                           NULL, /* write */
                           &netwib_priv_io_sniff_wait,
                           NULL, /* unread */
                           &netwib_priv_io_sniff_ctl_set,
                           &netwib_priv_io_sniff_ctl_get,
                           &netwib_priv_io_sniff_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

