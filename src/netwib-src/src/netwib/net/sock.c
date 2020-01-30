/*
                                  NETWIB
                             Network library
                Copyright(c) 1999-2012 Laurent CONSTANTIN
                      http://ntwib.sourceforge.net/
                        laurentconstantin@free.fr
                                  -----
  This file is part of Netwib.

  Netwib is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3
  as published by the Free Software Foundation.

  Netwib is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details (http://www.gnu.org/).

------------------------------------------------------------------------
*/

#include <netwib/inc/maininc.h>

/*-------------------------------------------------------------*/
typedef struct {
  netwib_io_socktype socktype;
  int fd;
  netwib_ip localip;
  netwib_ip remoteip;
  netwib_port localport;
  netwib_port remoteport;
  netwib_buf ip4opts;
  netwib_ipproto ip6extsproto;
  netwib_buf ip6exts;
  netwib_bool clientconnected;
} netwib_priv_io_sock;

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sock_init(netwib_io_socktype socktype,
                                           netwib_constip *plocalip,
                                           netwib_constip *premoteip,
                                           netwib_iptype iptype,
                                           netwib_port localport,
                                           netwib_port remoteport,
                                           netwib_constbuf *pip4opts,
                                           netwib_ipproto ip6extsproto,
                                           netwib_constbuf *pip6exts,
                                           netwib_bool *preadinitialized,
                                           netwib_bool *pwriteinitialized,
                                           netwib_priv_io_sock *ptr)
{
  netwib_data ip4opts;
  netwib_uint32 ip4optssize;
  netwib_priv_sockaddr_align sa;
  netwib_priv_sockaddr_len salen;
  netwib_err ret;
  int reti, sockettype=0, socketproto=0, socketdomain=0, one;

  ptr->socktype = socktype;
  ptr->clientconnected = NETWIB_FALSE;

  /* check types */
  if (plocalip != NULL && premoteip != NULL) {
    if (plocalip->iptype != premoteip->iptype) {
      return(NETWIB_ERR_PAINVALIDTYPE);
    }
    iptype = plocalip->iptype;
  } else if (plocalip != NULL) {
    iptype = plocalip->iptype;
  } else if (premoteip != NULL) {
    iptype = premoteip->iptype;
  }

  /* open socket */
  switch(iptype) {
    case NETWIB_IPTYPE_IP4 :
      socketdomain = PF_INET;
      break;
#if NETWIBDEF_HAVEVAR_AF_INET6 == 1
    case NETWIB_IPTYPE_IP6 :
      socketdomain = PF_INET6;
      break;
#endif
    default :
      return(NETWIB_ERR_PAIPTYPE);
      break;
  }
  switch(ptr->socktype) {
    case NETWIB_IO_SOCKTYPE_UDP_CLI :
    case NETWIB_IO_SOCKTYPE_UDP_SER :
    case NETWIB_IO_SOCKTYPE_UDP_MULSER :
      sockettype = SOCK_DGRAM;
      socketproto = IPPROTO_UDP;
      break;
    case NETWIB_IO_SOCKTYPE_TCP_CLI :
    case NETWIB_IO_SOCKTYPE_TCP_SER :
    case NETWIB_IO_SOCKTYPE_TCP_MULSER :
      sockettype = SOCK_STREAM;
      socketproto = IPPROTO_TCP;
      break;
  }
  ptr->fd = socket(socketdomain, sockettype, socketproto);
  netwib_ir(ptr->fd, NETWIB_ERR_FUSOCKET);

  /* eventually set options */
  netwib_eg(netwib_buf_init_mallocdefault(&ptr->ip4opts));
  if (pip4opts != NULL) {
    ip4opts = netwib__buf_ref_data_ptr(pip4opts);
    ip4optssize = netwib__buf_ref_data_size(pip4opts);
    reti = netwib_c_setsockopt(ptr->fd, IPPROTO_IP, IP_OPTIONS,
                               ip4opts, ip4optssize);
    netwib_ig(reti, NETWIB_ERR_FUSETSOCKOPT);
    netwib_eg(netwib_buf_append_buf(pip4opts, &ptr->ip4opts));
  }

  /* eventually set extensions (not yet implemented) */
  ptr->ip6extsproto = ip6extsproto;
  netwib_eg(netwib_buf_init_mallocdefault(&ptr->ip6exts));
  if (pip6exts != NULL) {
    netwib_eg(netwib_buf_append_buf(pip6exts, &ptr->ip6exts));
  }

  /* set local ip/port */
  if (plocalip != NULL || localport != 0) {
    one = 1;
    reti = netwib_c_setsockopt(ptr->fd, SOL_SOCKET, SO_REUSEADDR,
                               &one, sizeof(one));
    netwib_ig(reti, NETWIB_ERR_FUSETSOCKOPT);
    netwib_eg(netwib_priv_sa_sal_init_iptport(plocalip, iptype, localport,
                                              &sa, &salen));
    reti = bind(ptr->fd, (struct sockaddr *)&sa, salen);
    netwib_ig(reti, NETWIB_ERR_FUBIND);
  }

  /* connect or listen/accept */
  switch(ptr->socktype) {
    case NETWIB_IO_SOCKTYPE_UDP_CLI :
    case NETWIB_IO_SOCKTYPE_TCP_CLI :
      if (premoteip != NULL || remoteport != 0) {
        netwib_eg(netwib_priv_sa_sal_init_iptport(premoteip, iptype,
                                                  remoteport, &sa, &salen));
        reti = connect(ptr->fd, (struct sockaddr *)&sa, salen);
        netwib_ig(reti, NETWIB_ERR_FUCONNECT);
      }
      break;
    case NETWIB_IO_SOCKTYPE_TCP_SER :
      reti = listen(ptr->fd, 1);
      netwib_ig(reti, NETWIB_ERR_FULISTEN);
      if (pip4opts != NULL) {
        ip4opts = netwib__buf_ref_data_ptr(pip4opts);
        ip4optssize = netwib__buf_ref_data_size(pip4opts);
        reti = netwib_c_setsockopt(ptr->fd, IPPROTO_IP, IP_OPTIONS,
                                   ip4opts, ip4optssize);
        netwib_ig(reti, NETWIB_ERR_FUSETSOCKOPT);
      }
      break;
    case NETWIB_IO_SOCKTYPE_UDP_SER :
    case NETWIB_IO_SOCKTYPE_UDP_MULSER :
    case NETWIB_IO_SOCKTYPE_TCP_MULSER :
      /* nothing to do */
      break;
  }

  /* set io values */
  *preadinitialized = NETWIB_TRUE;
  *pwriteinitialized = NETWIB_TRUE;

  return(NETWIB_ERR_OK);

  /* reached on error */
 netwib_gotolabel:
  netwib_c_closesocket(ptr->fd);
  netwib_er(netwib_buf_close(&ptr->ip4opts));
  netwib_er(netwib_buf_close(&ptr->ip6exts));
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sock_init_tms(int fd,
                                               netwib_constbuf *pip4opts,
                                               netwib_ipproto ip6extsproto,
                                               netwib_constbuf *pip6exts,
                                               netwib_bool *preadinitialized,
                                               netwib_bool *pwriteinitialized,
                                               netwib_priv_io_sock *ptr)
{
  netwib_data ip4opts;
  netwib_uint32 ip4optssize;
  int reti;

  /* init resources */
  ptr->socktype = NETWIB_IO_SOCKTYPE_TCP_MULSER;
  ptr->fd = fd;
  ptr->clientconnected = NETWIB_FALSE;

  /* eventually set options */
  netwib_er(netwib_buf_init_mallocdefault(&ptr->ip4opts));
  if (pip4opts != NULL) {
    ip4opts = netwib__buf_ref_data_ptr(pip4opts);
    ip4optssize = netwib__buf_ref_data_size(pip4opts);
    reti = netwib_c_setsockopt(fd, IPPROTO_IP, IP_OPTIONS,
                               ip4opts, ip4optssize);
    netwib_ir(reti, NETWIB_ERR_FUSETSOCKOPT);
    netwib_er(netwib_buf_append_buf(pip4opts, &ptr->ip4opts));
  }

  /* eventually set extensions (not yet implemented) */
  ptr->ip6extsproto = ip6extsproto;
  netwib_er(netwib_buf_init_mallocdefault(&ptr->ip6exts));
  if (pip6exts != NULL) {
    netwib_er(netwib_buf_append_buf(pip6exts, &ptr->ip6exts));
  }

  /* set io values */
  *preadinitialized = NETWIB_TRUE;
  *pwriteinitialized = NETWIB_TRUE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sock_close(netwib_priv_io_sock *ptr)
{
  int reti;

  netwib_er(netwib_buf_close(&ptr->ip4opts));
  netwib_er(netwib_buf_close(&ptr->ip6exts));

  reti = netwib_c_closesocket(ptr->fd);
  netwib_ir(reti, NETWIB_ERR_FUCLOSE);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sock_tcpaccept(netwib_priv_io_sock *ptr)
{
  int fd, reti;

  fd = accept(ptr->fd, NULL, NULL);
  netwib_ir(fd, NETWIB_ERR_FUACCEPT);

  reti = netwib_c_closesocket(ptr->fd);
  netwib_ir(reti, NETWIB_ERR_FUCLOSE);

  ptr->fd = fd;
  ptr->clientconnected = NETWIB_TRUE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sock_read(netwib_io *pio,
                                           netwib_buf *pbuf)
{
  netwib_priv_io_sock *ptr = (netwib_priv_io_sock *)pio->pcommon;
  netwib_priv_sockaddr_align sa;
  netwib_priv_sockaddr_len salen;
  int reti;

  switch(ptr->socktype) {
    case NETWIB_IO_SOCKTYPE_TCP_SER :
      if (! ptr->clientconnected) {
        netwib_er(netwib_priv_io_sock_tcpaccept(ptr));
        /* the client connection event might not indicate data. If there
           is really data, this function will be called twice. */
        return(NETWIB_ERR_DATANOTAVAIL);
      }
      /* no break */
    case NETWIB_IO_SOCKTYPE_UDP_CLI :
    case NETWIB_IO_SOCKTYPE_TCP_CLI :
    case NETWIB_IO_SOCKTYPE_TCP_MULSER :
      netwib_er(netwib_priv_sa_recv(ptr->fd, pbuf));
      break;
    case NETWIB_IO_SOCKTYPE_UDP_SER :
      netwib_er(netwib_priv_sa_recvfrom(ptr->fd, pbuf, &sa, &salen));
      if (!ptr->clientconnected) {
        /* connect back on client to be able to write */
        reti = connect(ptr->fd, (struct sockaddr *)&sa, salen);
        netwib_ir(reti, NETWIB_ERR_FUCONNECT);
        ptr->clientconnected = NETWIB_TRUE;
      }
      break;
    case NETWIB_IO_SOCKTYPE_UDP_MULSER :
      netwib_er(netwib_priv_sa_recvfrom(ptr->fd, pbuf, &sa, &salen));
      netwib_er(netwib_priv_sa_ipport_init_sal((netwib_priv_sockaddr_unalign*)&sa, salen, &ptr->remoteip, &ptr->remoteport));
      ptr->clientconnected = NETWIB_TRUE;
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sock_write(netwib_io *pio,
                                            netwib_constbuf *pbuf)
{
  netwib_priv_io_sock *ptr = (netwib_priv_io_sock *)pio->pcommon;
  netwib_priv_sockaddr_align sa;
  netwib_priv_sockaddr_len salen;

  switch(ptr->socktype) {
    case NETWIB_IO_SOCKTYPE_UDP_SER :
    case NETWIB_IO_SOCKTYPE_UDP_MULSER :
      if (!ptr->clientconnected) {
        return(NETWIB_ERR_LOUDPSERREADBEFWRITE);
      }
      break;
    case NETWIB_IO_SOCKTYPE_TCP_SER :
      if (! ptr->clientconnected) {
        netwib_er(netwib_priv_io_sock_tcpaccept(ptr));
      }
      break;
    default:
      break;
  }

  switch(ptr->socktype) {
    case NETWIB_IO_SOCKTYPE_UDP_CLI :
    case NETWIB_IO_SOCKTYPE_TCP_CLI :
    case NETWIB_IO_SOCKTYPE_TCP_SER :
    case NETWIB_IO_SOCKTYPE_TCP_MULSER :
    case NETWIB_IO_SOCKTYPE_UDP_SER :
      netwib_er(netwib_priv_sa_send(ptr->fd, pbuf));
      break;
    case NETWIB_IO_SOCKTYPE_UDP_MULSER :
      netwib_er(netwib_priv_sa_sal_init_ipport(&ptr->remoteip, ptr->remoteport,
                                               &sa, &salen));
      netwib_er(netwib_priv_sa_sendto(ptr->fd, pbuf, (netwib_priv_sockaddr_unalign*)&sa, salen));
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sock_wait(netwib_io *pio,
                                           netwib_io_waytype way,
                                           netwib_consttime *pabstime,
                                           netwib_bool *pevent)
{
  netwib_priv_io_sock *ptr = (netwib_priv_io_sock *)pio->pcommon;
  netwib_bool event;

  netwib_er(netwib_priv_fd_wait(ptr->fd, way, pabstime, &event));
  if (event && ptr->socktype == NETWIB_IO_SOCKTYPE_TCP_SER) {
    if (! ptr->clientconnected) {
      netwib_er(netwib_priv_io_sock_tcpaccept(ptr));
    }
  }

  if (pevent != NULL) *pevent = event;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sock_ctl_set(netwib_io *pio,
                                              netwib_io_waytype way,
                                              netwib_io_ctltype type,
                                              netwib_ptr p,
                                              netwib_uint32 ui)
{
  netwib_priv_io_sock *ptr = (netwib_priv_io_sock *)pio->pcommon;
  netwib_buf *pip4opts, *pip6exts;
  netwib_data ip4opts;
  netwib_uint32 ip4optssize, ttl;
  int reti;

  switch(type) {
    case NETWIB_IO_CTLTYPE_SOCK_IP4OPTS:
      pip4opts = (netwib_buf*)p;
      ip4opts = netwib__buf_ref_data_ptr(pip4opts);
      ip4optssize = netwib__buf_ref_data_size(pip4opts);
      reti = netwib_c_setsockopt(ptr->fd, IPPROTO_IP, IP_OPTIONS,
                                 ip4opts, ip4optssize);
      netwib_ir(reti, NETWIB_ERR_FUSETSOCKOPT);
      netwib__buf_reinit(&ptr->ip4opts);
      netwib_er(netwib_buf_append_buf(pip4opts, &ptr->ip4opts));
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IO_CTLTYPE_SOCK_IP6EXTS:
      /* not fully implemented */
      pip6exts = (netwib_buf*)p;
      ptr->ip6extsproto = ui;
      netwib__buf_reinit(&ptr->ip6exts);
      netwib_er(netwib_buf_append_buf(pip6exts, &ptr->ip6exts));
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IO_CTLTYPE_SOCK_LOCAL:
    case NETWIB_IO_CTLTYPE_SOCK_REMOTE:
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    case NETWIB_IO_CTLTYPE_SOCK_MULTICASTTTL:
      ttl = ui;
      reti = netwib_c_setsockopt(ptr->fd, IPPROTO_IP, IP_MULTICAST_TTL,
                                 &ttl, sizeof(ttl));
      netwib_ir(reti, NETWIB_ERR_FUSETSOCKOPT);
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IO_CTLTYPE_SOCK_SOCKTYPE:
      return(NETWIB_ERR_PAINVALIDTYPE);
    default:
      return(NETWIB_ERR_PLEASETRYNEXT);
  }

  way = way; /* for compiler warning */
  ui = ui; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sock_ctl_get(netwib_io *pio,
                                              netwib_io_waytype way,
                                              netwib_io_ctltype type,
                                              netwib_ptr p,
                                              netwib_uint32 *pui)
{
  netwib_priv_io_sock *ptr = (netwib_priv_io_sock *)pio->pcommon;
  netwib_priv_sockaddr_align sa;
  netwib_priv_sockaddr_len salen;
  int reti;

  switch(type) {
    case NETWIB_IO_CTLTYPE_SOCK_IP4OPTS:
      if (p != NULL) {
        netwib_er(netwib_buf_append_buf(&ptr->ip4opts, (netwib_buf*)&p));
      }
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IO_CTLTYPE_SOCK_IP6EXTS:
      if (p != NULL) {
        netwib_er(netwib_buf_append_buf(&ptr->ip6exts, (netwib_buf*)&p));
      }
      if (pui != NULL) *pui = ptr->ip6extsproto;
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IO_CTLTYPE_SOCK_LOCAL:
      salen = sizeof(sa);
      reti = netwib_c_getsockname(ptr->fd, (struct sockaddr *)&sa, &salen);
      netwib_ir(reti, NETWIB_ERR_FUGETSOCKNAME);
      netwib_er(netwib_priv_sa_ipport_init_sal((netwib_priv_sockaddr_unalign*)&sa, salen, &ptr->localip, &ptr->localport));
      if (p != NULL) *(netwib_ip*)p = ptr->localip;
      if (pui != NULL) *pui = ptr->localport;
      return(NETWIB_ERR_OK);
    case NETWIB_IO_CTLTYPE_SOCK_REMOTE:
      if (ptr->socktype != NETWIB_IO_SOCKTYPE_UDP_MULSER) {
        salen = sizeof(sa);
        reti = netwib_c_getpeername(ptr->fd, (struct sockaddr *)&sa, &salen);
        netwib_ir(reti, NETWIB_ERR_FUGETPEERNAME);
        netwib_er(netwib_priv_sa_ipport_init_sal((netwib_priv_sockaddr_unalign*)&sa, salen, &ptr->remoteip, &ptr->remoteport));
      }
      if (p != NULL) *(netwib_ip*)p = ptr->remoteip;
      if (pui != NULL) *pui = ptr->remoteport;
      return(NETWIB_ERR_OK);
    case NETWIB_IO_CTLTYPE_SOCK_MULTICASTTTL:
      return(NETWIB_ERR_PAINVALIDTYPE);
    case NETWIB_IO_CTLTYPE_SOCK_SOCKTYPE:
      if (p != NULL) *(netwib_io_socktype*)p = ptr->socktype;
      return(NETWIB_ERR_OK);
    default:
      return(NETWIB_ERR_PLEASETRYNEXT);
  }

  way = way; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sock_fclose(netwib_io *pio)
{
  netwib_priv_io_sock *ptr = (netwib_priv_io_sock *)pio->pcommon;

  netwib_er(netwib_priv_io_sock_close(ptr));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_sock_udp_cli_full(netwib_constip *plocalip,
                                            netwib_constip *premoteip,
                                            netwib_port localport,
                                            netwib_port remoteport,
                                            netwib_constbuf *pip4opts,
                                            netwib_ipproto ip6extsproto,
                                            netwib_constbuf *pip6exts,
                                            netwib_io **ppio)
{
  netwib_bool rdinit, wrinit;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_sock), &pcommon));
  ret = netwib_priv_io_sock_init(NETWIB_IO_SOCKTYPE_UDP_CLI,
                                 plocalip, premoteip, (netwib_iptype)0,
                                 localport, remoteport,
                                 pip4opts, ip6extsproto, pip6exts,
                                 &rdinit, &wrinit,
                                 (netwib_priv_io_sock *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           &netwib_priv_io_sock_read,
                           &netwib_priv_io_sock_write,
                           &netwib_priv_io_sock_wait,
                           NULL, /* unread */
                           &netwib_priv_io_sock_ctl_set,
                           &netwib_priv_io_sock_ctl_get,
                           &netwib_priv_io_sock_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_sock_tcp_cli_full(netwib_constip *plocalip,
                                            netwib_constip *premoteip,
                                            netwib_port localport,
                                            netwib_port remoteport,
                                            netwib_constbuf *pip4opts,
                                            netwib_ipproto ip6extsproto,
                                            netwib_constbuf *pip6exts,
                                            netwib_io **ppio)
{
  netwib_bool rdinit, wrinit;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_sock), &pcommon));
  ret = netwib_priv_io_sock_init(NETWIB_IO_SOCKTYPE_TCP_CLI,
                                 plocalip, premoteip, (netwib_iptype)0,
                                 localport, remoteport,
                                 pip4opts, ip6extsproto, pip6exts,
                                 &rdinit, &wrinit,
                                 (netwib_priv_io_sock *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           &netwib_priv_io_sock_read,
                           &netwib_priv_io_sock_write,
                           &netwib_priv_io_sock_wait,
                           NULL, /* unread */
                           &netwib_priv_io_sock_ctl_set,
                           &netwib_priv_io_sock_ctl_get,
                           &netwib_priv_io_sock_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_sock_udp_ser_full(netwib_constip *plocalip,
                                            netwib_port localport,
                                            netwib_constbuf *pip4opts,
                                            netwib_ipproto ip6extsproto,
                                            netwib_constbuf *pip6exts,
                                            netwib_io **ppio)
{
  netwib_bool rdinit, wrinit;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_sock), &pcommon));
  ret = netwib_priv_io_sock_init(NETWIB_IO_SOCKTYPE_UDP_SER,
                                 plocalip, NULL, (netwib_iptype)0,
                                 localport, 0,
                                 pip4opts, ip6extsproto, pip6exts,
                                 &rdinit, &wrinit,
                                 (netwib_priv_io_sock *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           &netwib_priv_io_sock_read,
                           &netwib_priv_io_sock_write,
                           &netwib_priv_io_sock_wait,
                           NULL, /* unread */
                           &netwib_priv_io_sock_ctl_set,
                           &netwib_priv_io_sock_ctl_get,
                           &netwib_priv_io_sock_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_sock_udp_ser_easy(netwib_iptype iptype,
                                            netwib_port localport,
                                            netwib_io **ppio)
{
  netwib_bool rdinit, wrinit;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_sock), &pcommon));
  ret = netwib_priv_io_sock_init(NETWIB_IO_SOCKTYPE_UDP_SER,
                                 NULL, NULL, iptype,
                                 localport, 0,
                                 NULL, NETWIB_IPPROTO_NONE, NULL,
                                 &rdinit, &wrinit,
                                 (netwib_priv_io_sock *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           &netwib_priv_io_sock_read,
                           &netwib_priv_io_sock_write,
                           &netwib_priv_io_sock_wait,
                           NULL, /* unread */
                           &netwib_priv_io_sock_ctl_set,
                           &netwib_priv_io_sock_ctl_get,
                           &netwib_priv_io_sock_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_sock_tcp_ser_full(netwib_constip *plocalip,
                                            netwib_port localport,
                                            netwib_constbuf *pip4opts,
                                            netwib_ipproto ip6extsproto,
                                            netwib_constbuf *pip6exts,
                                            netwib_io **ppio)
{
  netwib_bool rdinit, wrinit;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_sock), &pcommon));
  ret = netwib_priv_io_sock_init(NETWIB_IO_SOCKTYPE_TCP_SER,
                                 plocalip, NULL, (netwib_iptype)0,
                                 localport, 0,
                                 pip4opts, ip6extsproto, pip6exts,
                                 &rdinit, &wrinit,
                                 (netwib_priv_io_sock *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           &netwib_priv_io_sock_read,
                           &netwib_priv_io_sock_write,
                           &netwib_priv_io_sock_wait,
                           NULL, /* unread */
                           &netwib_priv_io_sock_ctl_set,
                           &netwib_priv_io_sock_ctl_get,
                           &netwib_priv_io_sock_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_sock_tcp_ser_easy(netwib_iptype iptype,
                                            netwib_port localport,
                                            netwib_io **ppio)
{
  netwib_bool rdinit, wrinit;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_sock), &pcommon));
  ret = netwib_priv_io_sock_init(NETWIB_IO_SOCKTYPE_TCP_SER,
                                 NULL, NULL, iptype,
                                 localport, 0,
                                 NULL, NETWIB_IPPROTO_NONE, NULL,
                                 &rdinit, &wrinit,
                                 (netwib_priv_io_sock *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           &netwib_priv_io_sock_read,
                           &netwib_priv_io_sock_write,
                           &netwib_priv_io_sock_wait,
                           NULL, /* unread */
                           &netwib_priv_io_sock_ctl_set,
                           &netwib_priv_io_sock_ctl_get,
                           &netwib_priv_io_sock_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_sock_udp_mulser_full(netwib_constip *plocalip,
                                               netwib_port localport,
                                               netwib_constbuf *pip4opts,
                                               netwib_ipproto ip6extsproto,
                                               netwib_constbuf *pip6exts,
                                               netwib_io **ppio)
{
  netwib_bool rdinit, wrinit;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_sock), &pcommon));
  ret = netwib_priv_io_sock_init(NETWIB_IO_SOCKTYPE_UDP_MULSER,
                                 plocalip, NULL, (netwib_iptype)0,
                                 localport, 0,
                                 pip4opts, ip6extsproto, pip6exts,
                                 &rdinit, &wrinit,
                                 (netwib_priv_io_sock *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           &netwib_priv_io_sock_read,
                           &netwib_priv_io_sock_write,
                           &netwib_priv_io_sock_wait,
                           NULL, /* unread */
                           &netwib_priv_io_sock_ctl_set,
                           &netwib_priv_io_sock_ctl_get,
                           &netwib_priv_io_sock_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_sock_udp_mulser_easy(netwib_iptype iptype,
                                               netwib_port localport,
                                               netwib_io **ppio)
{
  netwib_bool rdinit, wrinit;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_sock), &pcommon));
  ret = netwib_priv_io_sock_init(NETWIB_IO_SOCKTYPE_UDP_MULSER,
                                 NULL, NULL, iptype,
                                 localport, 0,
                                 NULL, NETWIB_IPPROTO_NONE, NULL,
                                 &rdinit, &wrinit,
                                 (netwib_priv_io_sock *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           &netwib_priv_io_sock_read,
                           &netwib_priv_io_sock_write,
                           &netwib_priv_io_sock_wait,
                           NULL, /* unread */
                           &netwib_priv_io_sock_ctl_set,
                           &netwib_priv_io_sock_ctl_get,
                           &netwib_priv_io_sock_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
typedef struct {
  int fd;
  netwib_constbuf *pip4opts;
  netwib_ipproto ip6extsproto;
  netwib_constbuf *pip6exts;
  netwib_sock_tcp_mulser_pf pfunc;
  netwib_ptr pinfos;
} netwib_priv_sock_tcp_mulser_info;

/*-------------------------------------------------------------*/
static netwib_err netwib_sock_tcp_mulser_cli(netwib_ptr infosin,
                                             netwib_ptr *pinfosout)
{
  netwib_priv_sock_tcp_mulser_info *ptmi =
    (netwib_priv_sock_tcp_mulser_info *)infosin;
  netwib_bool rdinit, wrinit;
  netwib_ptr pcommon;
  netwib_io *pio;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_sock), &pcommon));
  ret = netwib_priv_io_sock_init_tms(ptmi->fd, ptmi->pip4opts,
                                     ptmi->ip6extsproto, ptmi->pip6exts,
                                     &rdinit, &wrinit,
                                     (netwib_priv_io_sock *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    netwib_er(netwib_ptr_free((netwib_ptr*)&ptmi));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           &netwib_priv_io_sock_read,
                           &netwib_priv_io_sock_write,
                           &netwib_priv_io_sock_wait,
                           NULL, /* unread */
                           &netwib_priv_io_sock_ctl_set,
                           &netwib_priv_io_sock_ctl_get,
                           &netwib_priv_io_sock_fclose,
                           &pio));

  /* call user function */
  ret = (*ptmi->pfunc)(pio, ptmi->pinfos);

  netwib_er(netwib_io_close(&pio));

  netwib_er(netwib_ptr_free((netwib_ptr*)&ptmi));
  pinfosout = pinfosout; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_sock_tcp_mulser2(netwib_constip *plocalip,
                                          netwib_iptype iptype,
                                          netwib_port localport,
                                          netwib_constbuf *pip4opts,
                                          netwib_ipproto ip6extsproto,
                                          netwib_constbuf *pip6exts,
                                          netwib_sock_tcp_mulser_pf pfunc,
                                          netwib_ptr pinfos)
{
  netwib_data ip4opts;
  netwib_uint32 ip4optssize;
  netwib_uint32 threadid;
  netwib_bool ringset, event, ctrlcpressed;
  netwib_priv_sockaddr_align sa;
  netwib_priv_sockaddr_len salen;
  netwib_ring *pring;
  netwib_priv_sock_tcp_mulser_info *ptmi;
  netwib_thread *pthread;
  netwib_err ret, threadret;
  int fd, fdcli, reti, socketdomain=0, one;

  ret = NETWIB_ERR_OK;
  ringset = NETWIB_FALSE;

  /* check types */
  if (plocalip != NULL) {
    iptype = plocalip->iptype;
  }

  /* open socket */
  switch(iptype) {
    case NETWIB_IPTYPE_IP4 :
      socketdomain = PF_INET;
      break;
#if NETWIBDEF_HAVEVAR_AF_INET6 == 1
    case NETWIB_IPTYPE_IP6 :
      socketdomain = PF_INET6;
      break;
#endif
    default :
      return(NETWIB_ERR_PAIPTYPE);
      break;
  }
  fd = socket(socketdomain, SOCK_STREAM, IPPROTO_TCP);
  netwib_ir(fd, NETWIB_ERR_FUSOCKET);

  /* eventually set options */
  if (pip4opts != NULL) {
    ip4opts = netwib__buf_ref_data_ptr(pip4opts);
    ip4optssize = netwib__buf_ref_data_size(pip4opts);
    reti = netwib_c_setsockopt(fd, IPPROTO_IP, IP_OPTIONS,
                               ip4opts, ip4optssize);
    netwib_ig(reti, NETWIB_ERR_FUSETSOCKOPT);
  }

  /* set local ip/port */
  one = 1;
  reti = netwib_c_setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,
                             &one, sizeof(one));
  netwib_ig(reti, NETWIB_ERR_FUSETSOCKOPT);
  netwib_eg(netwib_priv_sa_sal_init_iptport(plocalip, iptype, localport,
                                            &sa, &salen));
  reti = bind(fd, (struct sockaddr *)&sa, salen);
  netwib_ig(reti, NETWIB_ERR_FUBIND);

  /* listen */
  reti = listen(fd, 16);
  netwib_ig(reti, NETWIB_ERR_FULISTEN);

  /* accept each client */
  netwib_eg(netwib_threadlist_init(&pring));
  ringset = NETWIB_TRUE;
  threadid = 1;
  while (NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
    /* accept */
    fdcli = accept(fd, NULL, NULL);
    netwib_ig(fdcli, NETWIB_ERR_FUACCEPT);
    /* as the accept() was blocking, we also check now if we have to exit */
    ret = netwib_global_ctl_get_debug_ctrlc_pressed(&ctrlcpressed);
    if (ret == NETWIB_ERR_OK && ctrlcpressed) {
      netwib_c_closesocket(fdcli);
      break;
    }
    /* create thread to deal with the client */
    netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_sock_tcp_mulser_info),
                                (netwib_ptr*)&ptmi));
    ptmi->fd = fdcli;
    ptmi->pip4opts = pip4opts;
    ptmi->ip6extsproto = ip6extsproto;
    ptmi->pip6exts = pip6exts;
    ptmi->pfunc = pfunc;
    ptmi->pinfos = pinfos;
    netwib_eg(netwib_thread_init(&netwib_sock_tcp_mulser_cli, ptmi, &pthread));
    /* add this thread in the list */
    netwib_eg(netwib_threadlist_add(pring, pthread, threadid++));
    /* look if another thread exited */
    while (NETWIB_TRUE) {
      ret = netwib_threadlist_wait(pring, NETWIB_TIME_ZERO,
                                   &event, NULL, &threadret, NULL);
      if (ret == NETWIB_ERR_DATAEND) {
        ret = NETWIB_ERR_OK;
        break;
      }
      netwib_eg(ret);
      if (!event) break;
      netwib_eg(threadret);
    }
  }

  /* reached on error */
 netwib_gotolabel:
  if (ringset) {
    netwib_er(netwib_threadlist_close(&pring));
  }
  netwib_c_closesocket(fd);
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_sock_tcp_mulser_full(netwib_constip *plocalip,
                                       netwib_port localport,
                                       netwib_constbuf *pip4opts,
                                       netwib_ipproto ip6extsproto,
                                       netwib_constbuf *pip6exts,
                                       netwib_sock_tcp_mulser_pf pfunc,
                                       netwib_ptr pinfos)
{
  return(netwib_sock_tcp_mulser2(plocalip, (netwib_iptype)0, localport,
                                 pip4opts, ip6extsproto, pip6exts, pfunc,
                                 pinfos));
}

/*-------------------------------------------------------------*/
netwib_err netwib_sock_tcp_mulser_easy(netwib_iptype iptype,
                                       netwib_port localport,
                                       netwib_sock_tcp_mulser_pf pfunc,
                                       netwib_ptr pinfos)
{
  return(netwib_sock_tcp_mulser2(NULL, iptype, localport,
                                 NULL, NETWIB_IPPROTO_NONE, NULL,
                                 pfunc, pinfos));
}
