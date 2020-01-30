/*
                                  NETWOX
                             Network toolbox
                Copyright(c) 1999-2012 Laurent CONSTANTIN
                      http://ntwox.sourceforge.net/
                        laurentconstantin@free.fr
                                  -----

  This file is part of Netwox.

  Netwox is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3
  as published by the Free Software Foundation.

  Netwox is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details (http://www.gnu.org/).

------------------------------------------------------------------------
*/

/*-------------------------------------------------------------*/
#include "../../netwox.h"

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_io_sockv_close(netwox_priv_io_sockv *pisv)
{
  netwib_err ret;

  /* close thread (ignore error code of thread) */
  netwib_er(netwib_thread_cond_broadcast(pisv->pendcond, 1));
  netwib_er(netwib_thread_wait(pisv->pthread, NETWIB_TIME_INFINITE, NULL,
                               &ret, NULL));
  netwib_er(netwib_thread_close(&pisv->pthread));
  netwib_er(netwib_thread_cond_close(&pisv->pendcond));

  /* close other */
  netwib_er(netwib_io_close(&pisv->ptlvsysio));
  netwib_er(netwib_io_close(&pisv->ptlvusrio));
  netwib_er(netwib_buf_close(&pisv->device));
  netwib_er(netwib_buf_close(&pisv->ip4opts));
  netwib_er(netwib_buf_close(&pisv->ip6exts));
  if (pisv->infoinitialized) {
    netwib_er(netwib_thread_rwlock_close(&pisv->pinfolockrd));
    netwib_er(netwib_thread_rwlock_close(&pisv->pinfolockwr));
    netwib_er(netwib_buf_close(&pisv->infobufrd));
    netwib_er(netwib_buf_close(&pisv->infobufwr));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_io_sockv_tcpaccept(netwox_priv_io_sockv *pisv)
{
  netwib_err ret;

  netwib_er(netwox_sockv_info_read_err(pisv, &ret));
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  if (pisv->socktype == NETWOX_SOCKTYPE_TCP_SER) {
    /* the server also send us who is the client */
    netwib_er(netwox_sockv_info_read_cli(pisv, NETWIB_TRUE,
                                         &pisv->remoteeth,
                                         &pisv->remoteip,
                                         &pisv->remoteport));
  }

  pisv->clientconnected = NETWIB_TRUE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_io_sockv_init(netwox_socktype socktype,
                                            netwib_constbuf *pdevice,
                                            netwib_consteth *plocaleth,
                                            netwib_consteth *premoteeth,
                                            netwib_constip *plocalip,
                                            netwib_constip *premoteip,
                                            netwib_port localport,
                                            netwib_port remoteport,
                                            netwib_constbuf *pip4opts,
                                            netwib_ipproto ip6extsproto,
                                            netwib_constbuf *pip6exts,
                                            netwib_bool answeralive,
                                            netwib_bool *preadinitialized,
                                            netwib_bool *pwriteinitialized,
                                            netwox_priv_io_sockv *pisv)
{

  /* allocate resources */
  pisv->socktype = socktype;
  pisv->clientconnected = NETWIB_FALSE;
  netwib_er(netwox_sockfamily_init_socktype(socktype, &pisv->sockfamily));
  netwib_er(netwib_buf_init_mallocdefault(&pisv->device));
  netwib_er(netwib_buf_append_buf(pdevice, &pisv->device));
  pisv->localeth = *plocaleth;
  pisv->localip = *plocalip;
  pisv->localport = localport;
  netwib_er(netwib_buf_init_mallocdefault(&pisv->ip4opts));
  if (pip4opts != NULL) {
    netwib_er(netwib_buf_append_buf(pip4opts, &pisv->ip4opts));
  }
  pisv->ip6extsproto = ip6extsproto;
  netwib_er(netwib_buf_init_mallocdefault(&pisv->ip6exts));
  if (pip6exts != NULL) {
    netwib_er(netwib_buf_append_buf(pip6exts, &pisv->ip6exts));
  }
  pisv->answeralive = answeralive;

  /* for clients */
  if (pisv->sockfamily == NETWOX_SOCKFAMILY_CLI) {
    pisv->remoteeth = *premoteeth;
    pisv->remoteip = *premoteip;
    pisv->remoteport = remoteport;
  }

  /* for servers */
  pisv->infoinitialized = NETWIB_FALSE;
  if (pisv->sockfamily == NETWOX_SOCKFAMILY_SER ||
      pisv->socktype == NETWOX_SOCKTYPE_TCP_CLI) {
    /* information channel */
    netwib_er(netwib_buf_init_mallocdefault(&pisv->infobufrd));
    pisv->infobufrd.flags |= NETWIB_BUF_FLAGS_CANSLIDE;
    netwib_er(netwib_buf_init_mallocdefault(&pisv->infobufwr));
    pisv->infobufwr.flags |= NETWIB_BUF_FLAGS_CANSLIDE;
    netwib_er(netwib_thread_rwlock_init(&pisv->pinfolockrd));
    netwib_er(netwib_thread_rwlock_init(&pisv->pinfolockwr));
    pisv->infoinitialized = NETWIB_TRUE;
  }

  /* initialize TLV tunnel */
  netwib_er(netwib_buf_init_mallocdefault(&pisv->tlvbufrd));
  pisv->tlvbufrd.flags |= NETWIB_BUF_FLAGS_CANSLIDE;
  netwib_er(netwib_buf_init_mallocdefault(&pisv->tlvbufwr));
  pisv->tlvbufwr.flags |= NETWIB_BUF_FLAGS_CANSLIDE;
  netwib_er(netwib_thread_rwlock_init(&pisv->ptlvlockrd));
  netwib_er(netwib_thread_rwlock_init(&pisv->ptlvlockwr));
  netwib_er(netwib_io_init_tlv_lock(pisv->ptlvlockrd, &pisv->tlvbufrd,
                                    pisv->ptlvlockwr, &pisv->tlvbufwr,
                                    NETWIB_TRUE, &pisv->ptlvusrio));
  netwib_er(netwib_io_init_tlv_lock(pisv->ptlvlockwr, &pisv->tlvbufwr,
                                    pisv->ptlvlockrd, &pisv->tlvbufrd,
                                    NETWIB_FALSE, &pisv->ptlvsysio));

  /* create condition */
  netwib_er(netwib_thread_cond_init(&pisv->pendcond));

  /* init depending on type */
  switch(socktype) {
    case NETWOX_SOCKTYPE_UDP_CLI :
      netwib_er(netwib_thread_init(&netwox_sockv_thread_udp, pisv,
                                   &pisv->pthread));
      break;
    case NETWOX_SOCKTYPE_UDP_SER :
    case NETWOX_SOCKTYPE_UDP_MULSER :
      netwib_er(netwib_thread_init(&netwox_sockv_thread_udp, pisv,
                                   &pisv->pthread));
      break;
    case NETWOX_SOCKTYPE_TCP_CLI :
    case NETWOX_SOCKTYPE_TCP_SER :
      netwib_er(netwib_thread_init(&netwox_sockv_thread_tcp, pisv,
                                   &pisv->pthread));
      break;
    default :
      return(NETWIB_ERR_LOINTERNALERROR);
      break;
  }

  /* set io values */
  *preadinitialized = NETWIB_TRUE;
  *pwriteinitialized = NETWIB_TRUE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_io_sockv_read(netwib_io *pio,
                                            netwib_buf *pbuf)
{
  netwox_priv_io_sockv *pisv = (netwox_priv_io_sockv *)pio->pcommon;

  switch(pisv->socktype) {
    case NETWOX_SOCKTYPE_TCP_SER :
    case NETWOX_SOCKTYPE_TCP_CLI :
      if (! pisv->clientconnected) {
        netwib_er(netwox_priv_io_sockv_tcpaccept(pisv));
        /* the client connection event might not indicate data. If there
           is really data, this function will be called twice. */
        return(NETWIB_ERR_DATANOTAVAIL);
      }
      /* no break */
    case NETWOX_SOCKTYPE_UDP_CLI :
    case NETWOX_SOCKTYPE_TCP_MULSER :
      netwib_er(netwib_io_read(pisv->ptlvusrio, pbuf));
      break;
    case NETWOX_SOCKTYPE_UDP_SER :
      netwib_er(netwib_io_read(pisv->ptlvusrio, pbuf));
      if (!pisv->clientconnected) {
        netwib_er(netwox_sockv_info_read_cli(pisv, NETWIB_TRUE,
                                             &pisv->remoteeth,
                                             &pisv->remoteip,
                                             &pisv->remoteport));
        pisv->clientconnected = NETWIB_TRUE;
      }
      break;
    case NETWOX_SOCKTYPE_UDP_MULSER :
      netwib_er(netwib_io_read(pisv->ptlvusrio, pbuf));
      netwib_er(netwox_sockv_info_read_cli(pisv, NETWIB_TRUE, &pisv->remoteeth,
                                           &pisv->remoteip,
                                           &pisv->remoteport));
      pisv->clientconnected = NETWIB_TRUE;
      break;
  }

  pbuf = pbuf; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_io_sockv_write(netwib_io *pio,
                                             netwib_constbuf *pbuf)
{
  netwox_priv_io_sockv *pisv = (netwox_priv_io_sockv *)pio->pcommon;

  switch(pisv->socktype) {
    case NETWOX_SOCKTYPE_UDP_SER :
      if (!pisv->clientconnected) {
        return(NETWIB_ERR_LOUDPSERREADBEFWRITE);
      }
      break;
    case NETWOX_SOCKTYPE_UDP_MULSER :
      if (!pisv->clientconnected) {
        return(NETWIB_ERR_LOUDPSERREADBEFWRITE);
      }
      netwib_er(netwox_sockv_info_write_cli(pisv, NETWIB_FALSE,
                                            &pisv->remoteeth,
                                            &pisv->remoteip,
                                            pisv->remoteport));
      break;
    case NETWOX_SOCKTYPE_TCP_CLI :
    case NETWOX_SOCKTYPE_TCP_SER :
      if (!pisv->clientconnected) {
        netwib_er(netwox_priv_io_sockv_tcpaccept(pisv));
      }
      break;
    default:
      break;
  }

  netwib_er(netwib_io_write(pisv->ptlvusrio, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_io_sockv_wait(netwib_io *pio,
                                            netwib_io_waytype way,
                                            netwib_consttime *pabstime,
                                            netwib_bool *pevent)
{
  netwox_priv_io_sockv *pisv = (netwox_priv_io_sockv *)pio->pcommon;
  netwib_bool event;
  netwib_err ret;

  if (! pisv->clientconnected) {
    if (pisv->socktype == NETWOX_SOCKTYPE_TCP_SER ||
        pisv->socktype == NETWOX_SOCKTYPE_TCP_CLI ) {
      netwib_er(netwox_sockv_info_read_err_avail(pisv, &event));
      if (event) {
        netwib_er(netwox_priv_io_sockv_tcpaccept(pisv));
      }
      if (pevent != NULL) *pevent = event;
      return(NETWIB_ERR_OK);
    }
  }

  netwib_er(netwib_io_wait(pisv->ptlvusrio, way, NETWIB_TIME_ZERO, &event));
  if (event) {
    if (pevent != NULL) *pevent = event;
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_thread_wait(pisv->pthread, NETWIB_TIME_ZERO, &event,
                               &ret, NULL));
  if (event) {
    return(ret);
  }

  pabstime = pabstime; /* for compiler warning */
  return(NETWIB_ERR_PLEASELOOPTIME);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_io_sockv_ctl_set(netwib_io *pio,
                                               netwib_io_waytype way,
                                               netwib_io_ctltype type,
                                               netwib_ptr p,
                                               netwib_uint32 ui)
{

  switch((int)type) {
    case NETWIB_IO_CTLTYPE_SOCK_IP4OPTS:
    case NETWIB_IO_CTLTYPE_SOCK_IP6EXTS:
      return(NETWIB_ERR_LONOTIMPLEMENTED);
      break;
    case NETWIB_IO_CTLTYPE_SOCK_LOCAL:
    case NETWIB_IO_CTLTYPE_SOCK_REMOTE:
    case NETWOX_IO_CTLTYPE_SOCK_LOCALETH:
    case NETWOX_IO_CTLTYPE_SOCK_REMOTEETH:
    case NETWOX_IO_CTLTYPE_SOCK_DEVICE:
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
static netwib_err netwox_priv_io_sockv_ctl_get(netwib_io *pio,
                                               netwib_io_waytype way,
                                               netwib_io_ctltype type,
                                               netwib_ptr p,
                                               netwib_uint32 *pui)
{
  netwox_priv_io_sockv *pisv = (netwox_priv_io_sockv *)pio->pcommon;

  switch((int)type) {
    case NETWIB_IO_CTLTYPE_SOCK_IP4OPTS:
      if (p != NULL) {
        netwib_er(netwib_buf_append_buf(&pisv->ip4opts, (netwib_buf*)&p));
      }
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IO_CTLTYPE_SOCK_IP6EXTS:
      if (p != NULL) {
        netwib_er(netwib_buf_append_buf(&pisv->ip6exts, (netwib_buf*)&p));
      }
      if (pui != NULL) *pui = pisv->ip6extsproto;
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IO_CTLTYPE_SOCK_LOCAL:
      if (p != NULL) *(netwib_ip*)p = pisv->localip;
      if (pui != NULL) *pui = pisv->localport;
      return(NETWIB_ERR_OK);
    case NETWIB_IO_CTLTYPE_SOCK_REMOTE:
      if (pisv->socktype == NETWOX_SOCKTYPE_TCP_SER ||
          pisv->socktype == NETWOX_SOCKTYPE_UDP_SER ||
          pisv->socktype == NETWOX_SOCKTYPE_UDP_MULSER) {
        if (!pisv->clientconnected) {
          return(NETWIB_ERR_FUGETPEERNAME);
        }
      }
      if (p != NULL) *(netwib_ip*)p = pisv->remoteip;
      if (pui != NULL) *pui = pisv->remoteport;
      return(NETWIB_ERR_OK);
    case NETWOX_IO_CTLTYPE_SOCK_LOCALETH:
      if (p != NULL) *(netwib_eth*)p = pisv->localeth;
      return(NETWIB_ERR_OK);
    case NETWOX_IO_CTLTYPE_SOCK_REMOTEETH:
      if (pisv->socktype == NETWOX_SOCKTYPE_TCP_SER ||
          pisv->socktype == NETWOX_SOCKTYPE_UDP_SER ||
          pisv->socktype == NETWOX_SOCKTYPE_UDP_MULSER) {
        if (!pisv->clientconnected) {
          return(NETWIB_ERR_FUGETPEERNAME);
        }
      }
      if (p != NULL) *(netwib_eth*)p = pisv->remoteeth;
      return(NETWIB_ERR_OK);
    case NETWOX_IO_CTLTYPE_SOCK_DEVICE:
      if (p != NULL) {
        netwib_er(netwib_buf_append_buf(&pisv->device, (netwib_buf*)&p));
      }
      return(NETWIB_ERR_OK);
    default:
      return(NETWIB_ERR_PLEASETRYNEXT);
  }

  way = way; /* for compiler warning */
  pui = pui; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_io_sockv_fclose(netwib_io *pio)
{
  netwox_priv_io_sockv *pisv = (netwox_priv_io_sockv *)pio->pcommon;
  netwib_er(netwox_priv_io_sockv_close(pisv));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_io_init_sockv_udp_cli_full(netwib_constbuf *pdevice,
                                             netwib_consteth *plocaleth,
                                             netwib_consteth *premoteeth,
                                             netwib_constip *plocalip,
                                             netwib_constip *premoteip,
                                             netwib_port localport,
                                             netwib_port remoteport,
                                             netwib_constbuf *pip4opts,
                                             netwib_ipproto ip6extsproto,
                                             netwib_constbuf *pip6exts,
                                             netwib_bool answeralive,
                                             netwib_io **ppio)
{
  netwib_bool rdinit, wrinit;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwox_priv_io_sockv), &pcommon));
  ret = netwox_priv_io_sockv_init(NETWOX_SOCKTYPE_UDP_CLI,
                                  pdevice, plocaleth, premoteeth,
                                  plocalip, premoteip,
                                  localport, remoteport,
                                  pip4opts, ip6extsproto,
                                  pip6exts, answeralive,
                                  &rdinit, &wrinit,
                                  (netwox_priv_io_sockv *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           &netwox_priv_io_sockv_read,
                           &netwox_priv_io_sockv_write,
                           &netwox_priv_io_sockv_wait,
                           NULL, /* unread */
                           &netwox_priv_io_sockv_ctl_set,
                           &netwox_priv_io_sockv_ctl_get,
                           &netwox_priv_io_sockv_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_io_init_sockv_tcp_cli_full(netwib_constbuf *pdevice,
                                             netwib_consteth *plocaleth,
                                             netwib_consteth *premoteeth,
                                             netwib_constip *plocalip,
                                             netwib_constip *premoteip,
                                             netwib_port localport,
                                             netwib_port remoteport,
                                             netwib_constbuf *pip4opts,
                                             netwib_ipproto ip6extsproto,
                                             netwib_constbuf *pip6exts,
                                             netwib_bool answeralive,
                                             netwib_io **ppio)
{
  netwib_bool rdinit, wrinit;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwox_priv_io_sockv), &pcommon));
  ret = netwox_priv_io_sockv_init(NETWOX_SOCKTYPE_TCP_CLI,
                                  pdevice, plocaleth, premoteeth,
                                  plocalip, premoteip,
                                  localport, remoteport,
                                  pip4opts, ip6extsproto,
                                  pip6exts, answeralive,
                                  &rdinit, &wrinit,
                                  (netwox_priv_io_sockv *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           &netwox_priv_io_sockv_read,
                           &netwox_priv_io_sockv_write,
                           &netwox_priv_io_sockv_wait,
                           NULL, /* unread */
                           &netwox_priv_io_sockv_ctl_set,
                           &netwox_priv_io_sockv_ctl_get,
                           &netwox_priv_io_sockv_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_io_init_sockv_udp_ser_full(netwib_constbuf *pdevice,
                                             netwib_consteth *plocaleth,
                                             netwib_constip *plocalip,
                                             netwib_port localport,
                                             netwib_constbuf *pip4opts,
                                             netwib_ipproto ip6extsproto,
                                             netwib_constbuf *pip6exts,
                                             netwib_bool answeralive,
                                             netwib_io **ppio)
{
  netwib_bool rdinit, wrinit;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwox_priv_io_sockv), &pcommon));
  ret = netwox_priv_io_sockv_init(NETWOX_SOCKTYPE_UDP_SER,
                                  pdevice, plocaleth, NULL, plocalip, NULL,
                                  localport, 0, pip4opts, ip6extsproto,
                                  pip6exts, answeralive,
                                  &rdinit, &wrinit,
                                  (netwox_priv_io_sockv *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           &netwox_priv_io_sockv_read,
                           &netwox_priv_io_sockv_write,
                           &netwox_priv_io_sockv_wait,
                           NULL, /* unread */
                           &netwox_priv_io_sockv_ctl_set,
                           &netwox_priv_io_sockv_ctl_get,
                           &netwox_priv_io_sockv_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_io_init_sockv_tcp_ser_full(netwib_constbuf *pdevice,
                                             netwib_consteth *plocaleth,
                                             netwib_constip *plocalip,
                                             netwib_port localport,
                                             netwib_constbuf *pip4opts,
                                             netwib_ipproto ip6extsproto,
                                             netwib_constbuf *pip6exts,
                                             netwib_bool answeralive,
                                             netwib_io **ppio)
{
  netwib_bool rdinit, wrinit;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwox_priv_io_sockv), &pcommon));
  ret = netwox_priv_io_sockv_init(NETWOX_SOCKTYPE_TCP_SER,
                                  pdevice, plocaleth, NULL, plocalip, NULL,
                                  localport, 0, pip4opts, ip6extsproto,
                                  pip6exts, answeralive,
                                  &rdinit, &wrinit,
                                  (netwox_priv_io_sockv *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           &netwox_priv_io_sockv_read,
                           &netwox_priv_io_sockv_write,
                           &netwox_priv_io_sockv_wait,
                           NULL, /* unread */
                           &netwox_priv_io_sockv_ctl_set,
                           &netwox_priv_io_sockv_ctl_get,
                           &netwox_priv_io_sockv_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_io_init_sockv_udp_mulser_full(netwib_constbuf *pdevice,
                                                netwib_consteth *plocaleth,
                                                netwib_constip *plocalip,
                                                netwib_port localport,
                                                netwib_constbuf *pip4opts,
                                                netwib_ipproto ip6extsproto,
                                                netwib_constbuf *pip6exts,
                                                netwib_bool answeralive,
                                                netwib_io **ppio)
{
  netwib_bool rdinit, wrinit;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwox_priv_io_sockv), &pcommon));
  ret = netwox_priv_io_sockv_init(NETWOX_SOCKTYPE_UDP_MULSER,
                                  pdevice, plocaleth, NULL, plocalip, NULL,
                                  localport, 0, pip4opts, ip6extsproto,
                                  pip6exts, answeralive,
                                  &rdinit, &wrinit,
                                  (netwox_priv_io_sockv *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           &netwox_priv_io_sockv_read,
                           &netwox_priv_io_sockv_write,
                           &netwox_priv_io_sockv_wait,
                           NULL, /* unread */
                           &netwox_priv_io_sockv_ctl_set,
                           &netwox_priv_io_sockv_ctl_get,
                           &netwox_priv_io_sockv_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_sockv_tcp_mulser_full(netwib_constbuf *pdevice,
                                        netwib_consteth *plocaleth,
                                        netwib_constip *plocalip,
                                        netwib_port localport,
                                        netwib_constbuf *pip4opts,
                                        netwib_ipproto ip6extsproto,
                                        netwib_constbuf *pip6exts,
                                        netwib_bool answeralive,
                                        netwib_sock_tcp_mulser_pf pfunc,
                                        netwib_ptr pinfos)
{
  netwib_io *pio;
  netwib_err ret=NETWIB_ERR_OK;

  while (NETWIB_TRUE) {
    netwib_er(netwox_io_init_sockv_tcp_ser_full(pdevice, plocaleth, plocalip,
                                                localport, pip4opts,
                                                ip6extsproto, pip6exts,
                                                answeralive, &pio));
    netwib_er(netwib_io_wait_read(pio, NETWIB_TIME_INFINITE, NULL));
    ret = (*pfunc)(pio, pinfos);
    netwib_er(netwib_io_close(&pio));
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }

  return(ret);
}
