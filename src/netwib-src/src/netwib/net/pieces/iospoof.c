/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
typedef struct {
  netwib_priv_libnet libnet;
} netwib_priv_io_spoof;

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_spoof_init(netwib_spoof_inittype inittype,
                                            netwib_constbuf *pdevice,
                                            netwib_bool *preadinitialized,
                                            netwib_bool *pwriteinitialized,
                                            netwib_priv_io_spoof *ptr)
{
  netwib_bool libnetset, yes;
  netwib_err ret;

  /* check if requested type is supported */
  netwib_er(netwib_priv_libnet_supports(pdevice, inittype, &yes));
  if (!yes) {
    return(NETWIB_ERR_LONOTSUPPORTED);
  }

  /* open device */
  libnetset = NETWIB_FALSE;
  netwib_eg(netwib_priv_libnet_init(pdevice, inittype, &ptr->libnet));
  libnetset = NETWIB_TRUE;

  /* obtain DLT type */
  if (inittype == NETWIB_SPOOF_INITTYPE_LINK) {
    netwib_eg(netwib_priv_libnet_get_dlt(&ptr->libnet));
  }

  /* set io values */
  *preadinitialized = NETWIB_FALSE;
  *pwriteinitialized = NETWIB_TRUE;

  return(NETWIB_ERR_OK);

  /* reached on error */
 netwib_gotolabel:
  if (libnetset) {
    netwib_er(netwib_priv_libnet_close(&ptr->libnet));
  }
  if (ret == NETWIB_ERR_FULIBNETINIT) {
    ret = NETWIB_ERR_LONOTSUPPORTED;
  }
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_spoof_close(netwib_priv_io_spoof *ptr)
{
  netwib_er(netwib_priv_libnet_close(&ptr->libnet));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_spoof_write(netwib_io *pio,
                                             netwib_constbuf *pbuf)
{
  netwib_priv_io_spoof *ptr = (netwib_priv_io_spoof *)pio->pcommon;

  netwib_er(netwib_priv_libnet_write(&ptr->libnet, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_spoof_wait(netwib_io *pio,
                                            netwib_io_waytype way,
                                            netwib_consttime *pabstime,
                                            netwib_bool *pevent)
{
  netwib_priv_io_spoof *ptr = (netwib_priv_io_spoof *)pio->pcommon;

  netwib_er(netwib_priv_libnet_wait(&ptr->libnet, way, pabstime, pevent));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_spoof_ctl_set(netwib_io *pio,
                                               netwib_io_waytype way,
                                               netwib_io_ctltype type,
                                               netwib_ptr p,
                                               netwib_uint32 ui)
{

  switch(type) {
    case NETWIB_IO_CTLTYPE_SPOOF_DLT:
      return(NETWIB_ERR_PAINVALIDTYPE);
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
static netwib_err netwib_priv_io_spoof_ctl_get(netwib_io *pio,
                                               netwib_io_waytype way,
                                               netwib_io_ctltype type,
                                               netwib_ptr p,
                                               netwib_uint32 *pui)
{
  netwib_priv_io_spoof *ptr = (netwib_priv_io_spoof *)pio->pcommon;

  switch(type) {
    case NETWIB_IO_CTLTYPE_SPOOF_DLT:
      if (pui != NULL) {
        if (ptr->libnet.inittype == NETWIB_SPOOF_INITTYPE_LINK) {
          *pui = ptr->libnet.dlttype;
        } else {
          *pui = NETWIB_DEVICE_DLTTYPE_RAW;
        }
      }
      return(NETWIB_ERR_OK);
    default:
      return(NETWIB_ERR_PLEASETRYNEXT);
  }

  way = way; /* for compiler warning */
  p = p; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_spoof_fclose(netwib_io *pio)
{
  netwib_priv_io_spoof *ptr = (netwib_priv_io_spoof *)pio->pcommon;

  netwib_er(netwib_priv_io_spoof_close(ptr));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_spoof(netwib_spoof_inittype inittype,
                                netwib_constbuf *pdevice,
                                netwib_io **ppio)
{
  netwib_bool rdinit=NETWIB_FALSE, wrinit=NETWIB_FALSE;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_spoof), &pcommon));
  ret = netwib_priv_io_spoof_init(inittype, pdevice,
                                  &rdinit, &wrinit,
                                  (netwib_priv_io_spoof *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           NULL, /* read */
                           &netwib_priv_io_spoof_write,
                           &netwib_priv_io_spoof_wait,
                           NULL, /* unread */
                           &netwib_priv_io_spoof_ctl_set,
                           &netwib_priv_io_spoof_ctl_get,
                           &netwib_priv_io_spoof_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

