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
#include "../netwox.h"

/*-------------------------------------------------------------*/
netwib_err netwox_sockfamily_init_socktype(netwox_socktype socktype,
                                           netwox_sockfamily *psockfamily)
{
  switch(socktype) {
    case NETWOX_SOCKTYPE_UDP_CLI :
    case NETWOX_SOCKTYPE_TCP_CLI :
      *psockfamily = NETWOX_SOCKFAMILY_CLI;
      break;
    case NETWOX_SOCKTYPE_UDP_SER :
    case NETWOX_SOCKTYPE_TCP_SER :
    case NETWOX_SOCKTYPE_UDP_MULSER :
    case NETWOX_SOCKTYPE_TCP_MULSER :
      *psockfamily = NETWOX_SOCKFAMILY_SER;
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_sockinfo_init(netwox_socktype socktype,
                                netwox_sockinfo *psockinfo)
{

  /* initialize standard information */
  psockinfo->answeralive = NETWIB_TRUE;
  psockinfo->socktype = socktype;
  netwib_er(netwox_sockfamily_init_socktype(socktype, &psockinfo->sockfamily));
  psockinfo->sockinit = NETWOX_SOCKINIT_REALEASY;

  /* set default values for pointers and ports */
  psockinfo->plocalip = NULL;
  psockinfo->premoteip = NULL;
  psockinfo->localport = 0;
  psockinfo->remoteport = 0;
  psockinfo->pip4opts = NULL;
  psockinfo->pip6exts = NULL;

  /* initialize buffers */
  netwib_er(netwib_buf_init_mallocdefault(&psockinfo->device));
  netwib_er(netwib_buf_init_mallocdefault(&psockinfo->ip4opts));
  netwib_er(netwib_buf_init_mallocdefault(&psockinfo->ip6exts));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_sockinfo_close(netwox_sockinfo *psockinfo)
{
  netwib_er(netwib_buf_close(&psockinfo->device));
  netwib_er(netwib_buf_close(&psockinfo->ip4opts));
  netwib_er(netwib_buf_close(&psockinfo->ip6exts));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_sockinfo_init_arg(netwox_socktype socktype,
                                    netwox_arg *parg,
                                    netwox_sockinfo *psockinfo)
{
  netwib_buf buf;
  netwib_bool devicesetbyuser = NETWIB_FALSE;
  netwib_bool localethsetbyuser = NETWIB_FALSE;
  netwib_bool remoteethsetbyuser = NETWIB_FALSE;
  netwib_bool localipsetbyuser = NETWIB_FALSE;
  netwib_bool remoteipsetbyuser = NETWIB_FALSE;
  netwib_bool localportsetbyuser = NETWIB_FALSE;
  netwib_bool remoteportsetbyuser = NETWIB_FALSE;
  netwib_bool ip4optssetbyuser = NETWIB_FALSE;
  netwib_bool ip6extssetbyuser = NETWIB_FALSE;
  netwib_bool iptypesetbyuser = NETWIB_FALSE;
  netwib_bool deviceneeded = NETWIB_FALSE;
  netwib_bool remoteethneeded = NETWIB_FALSE;
  netwib_bool localethneeded = NETWIB_FALSE;
  netwib_bool localipneeded = NETWIB_FALSE;
  netwib_bool localportneeded = NETWIB_FALSE;

  /* initialize standard information */
  netwib_er(netwox_sockinfo_init(socktype, psockinfo));

  /* obtain what's set by user */
  netwib_er(netwox_arg_isset(parg, NETWOX_SOCK_ARG_KEY_DEVICE,
                             &devicesetbyuser));
  if (devicesetbyuser) {
    netwib_er(netwox_arg_buf(parg, NETWOX_SOCK_ARG_KEY_DEVICE, &buf));
    netwib_er(netwib_buf_append_buf(&buf, &psockinfo->device));
  }
  netwib_er(netwox_arg_isset(parg, NETWOX_SOCK_ARG_KEY_ETH_SRC,
                             &localethsetbyuser));
  if (localethsetbyuser) {
    netwib_er(netwox_arg_eth(parg, NETWOX_SOCK_ARG_KEY_ETH_SRC,
                             &psockinfo->localeth));
  }
  netwib_er(netwox_arg_isset(parg, NETWOX_SOCK_ARG_KEY_IP_SRC,
                             &localipsetbyuser));
  if (localipsetbyuser) {
    netwib_er(netwox_arg_ip(parg, NETWOX_SOCK_ARG_KEY_IP_SRC,
                            &psockinfo->localip));
    psockinfo->plocalip = &psockinfo->localip;
  }
  netwib_er(netwox_arg_isset(parg, NETWOX_SOCK_ARG_KEY_PORT_SRC,
                             &localportsetbyuser));
  if (localportsetbyuser) {
    netwib_er(netwox_arg_port(parg, NETWOX_SOCK_ARG_KEY_PORT_SRC,
                              &psockinfo->localport));
  }
  netwib_er(netwox_arg_isset(parg, NETWOX_SOCK_ARG_KEY_IP4OPTS,
                             &ip4optssetbyuser));
  if (ip4optssetbyuser) {
    netwib_er(netwox_arg_buf(parg, NETWOX_SOCK_ARG_KEY_IP4OPTS, &buf));
    netwib_er(netwib_buf_append_buf(&buf, &psockinfo->ip4opts));
    psockinfo->pip4opts = &psockinfo->ip4opts;
  }
  netwib_er(netwox_arg_isset(parg, NETWOX_SOCK_ARG_KEY_IP6EXTS,
                             &ip6extssetbyuser));
  if (ip6extssetbyuser) {
    netwib_er(netwox_arg_buf(parg, NETWOX_SOCK_ARG_KEY_IP6EXTS, &buf));
    netwib_er(netwox_ip6exts_decode_para(&buf, &psockinfo->ip6extsproto,
                                         &psockinfo->ip6exts));
    psockinfo->pip6exts = &psockinfo->ip6exts;
  }
  /* now, only special cases */
  switch(psockinfo->sockfamily) {
    case NETWOX_SOCKFAMILY_CLI :
      netwib_er(netwox_arg_isset(parg, NETWOX_SOCK_ARG_KEY_ETH_DST,
                                 &remoteethsetbyuser));
      if (remoteethsetbyuser) {
        netwib_er(netwox_arg_eth(parg, NETWOX_SOCK_ARG_KEY_ETH_DST,
                                 &psockinfo->remoteeth));
      }
      netwib_er(netwox_arg_isset(parg, NETWOX_SOCK_ARG_KEY_IP_DST,
                                 &remoteipsetbyuser));
      if (remoteipsetbyuser) {
        netwib_er(netwox_arg_ip(parg, NETWOX_SOCK_ARG_KEY_IP_DST,
                                &psockinfo->remoteip));
        psockinfo->premoteip = &psockinfo->remoteip;
      }
      netwib_er(netwox_arg_isset(parg, NETWOX_SOCK_ARG_KEY_PORT_DST,
                                 &remoteportsetbyuser));
      if (remoteportsetbyuser) {
        netwib_er(netwox_arg_port(parg, NETWOX_SOCK_ARG_KEY_PORT_DST,
                                  &psockinfo->remoteport));
      }
      break;
    case NETWOX_SOCKFAMILY_SER :
      netwib_er(netwox_arg_isset(parg, NETWOX_SOCK_ARG_KEY_IPTYPE,
                                 &iptypesetbyuser));
      break;
  }

  /* set what's always needed */
  switch(psockinfo->sockfamily) {
    case NETWOX_SOCKFAMILY_CLI :
      /* remoteip is always needed */
      if (!remoteipsetbyuser) {
        netwib_er(netwox_arg_ip(parg, NETWOX_SOCK_ARG_KEY_IP_DST,
                                &psockinfo->remoteip));
        psockinfo->premoteip = &psockinfo->remoteip;
      }
      /* remoteport is always needed */
      if (!remoteportsetbyuser) {
        netwib_er(netwox_arg_port(parg, NETWOX_SOCK_ARG_KEY_PORT_DST,
                                  &psockinfo->remoteport));
      }
      break;
    case NETWOX_SOCKFAMILY_SER :
      /* localport is always needed */
      if (!localportsetbyuser) {
        netwib_er(netwox_arg_port(parg, NETWOX_SOCK_ARG_KEY_PORT_SRC,
                                  &psockinfo->localport));
      }
      /* iptype and localip are always needed */
      if (localipsetbyuser) {
        psockinfo->iptype = psockinfo->localip.iptype;
      } else {
        netwib_er(netwox_arg_iptype(parg, NETWOX_SOCK_ARG_KEY_IPTYPE,
                                    &psockinfo->iptype));
        if (psockinfo->iptype == NETWIB_IPTYPE_IP6) {
          netwib_er(netwib_ip_init_ip6_fields(0,0,0,0, &psockinfo->localip));
        } else {
          netwib_er(netwib_ip_init_ip4(0, &psockinfo->localip));
        }
      }
      psockinfo->plocalip = &psockinfo->localip;
      break;
  }

  /* determine what we need more */
  switch(psockinfo->sockfamily) {
    case NETWOX_SOCKFAMILY_CLI :
      if (devicesetbyuser || localethsetbyuser || remoteethsetbyuser) {
        psockinfo->sockinit = NETWOX_SOCKINIT_VIRT;
        deviceneeded = NETWIB_TRUE;
        localethneeded = NETWIB_TRUE;
        remoteethneeded = NETWIB_TRUE;
        localipneeded = NETWIB_TRUE;
        localportneeded = NETWIB_TRUE;
      } else {
        psockinfo->sockinit = NETWOX_SOCKINIT_REAL;
      }
      break;
    case NETWOX_SOCKFAMILY_SER :
      if (devicesetbyuser || localethsetbyuser) {
        psockinfo->sockinit = NETWOX_SOCKINIT_VIRT;
        deviceneeded = NETWIB_TRUE;
        localethneeded = NETWIB_TRUE;
      } else if (localipsetbyuser || ip4optssetbyuser) {
        psockinfo->sockinit = NETWOX_SOCKINIT_REAL;
      } else {
        psockinfo->sockinit = NETWOX_SOCKINIT_REALEASY;
      }
      break;
  }

  /* obtain what's needed (for the moment, only for virtual) */
  if (psockinfo->sockinit == NETWOX_SOCKINIT_VIRT) {
    if (deviceneeded && !devicesetbyuser) {
      netwib_er(netwox_arg_buf(parg, NETWOX_SOCK_ARG_KEY_DEVICE, &buf));
      netwib_er(netwib_buf_append_buf(&buf, &psockinfo->device));
    }
    if (localethneeded && !localethsetbyuser) {
      netwib_er(netwox_arg_eth(parg, NETWOX_SOCK_ARG_KEY_ETH_SRC,
                               &psockinfo->localeth));
    }
    if (remoteethneeded && !remoteethsetbyuser) {
      netwib_er(netwox_arg_eth(parg, NETWOX_SOCK_ARG_KEY_ETH_DST,
                               &psockinfo->remoteeth));
    }
    if (localipneeded && !localipsetbyuser) {
      netwib_er(netwox_arg_ip(parg, NETWOX_SOCK_ARG_KEY_IP_SRC,
                              &psockinfo->localip));
      psockinfo->plocalip = &psockinfo->localip;
    }
    if (localportneeded && !localportsetbyuser) {
      netwib_er(netwox_arg_port(parg, NETWOX_SOCK_ARG_KEY_PORT_SRC,
                                &psockinfo->localport));
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_sockinfo_init_values(netwox_socktype socktype,
                                       netwox_sockinit sockinit,
                                       netwib_iptype iptype,
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
                                       netwox_sockinfo *psockinfo)
{
  /* initialize standard information */
  netwib_er(netwox_sockinfo_init(socktype, psockinfo));
  psockinfo->sockinit = sockinit;

  /* set values */
  psockinfo->iptype = iptype;
  if (pdevice != NULL) {
    netwib_er(netwib_buf_append_buf(pdevice, &psockinfo->device));
  }
  if (plocaleth != NULL) {
    psockinfo->localeth = *plocaleth;
  }
  if (premoteeth != NULL) {
    psockinfo->remoteeth = *premoteeth;
  }
  if (plocalip != NULL) {
    psockinfo->localip = *plocalip;
    psockinfo->plocalip = &psockinfo->localip;
    psockinfo->iptype = plocalip->iptype;
  }
  if (premoteip != NULL) {
    psockinfo->remoteip = *premoteip;
    psockinfo->premoteip = &psockinfo->remoteip;
  }
  psockinfo->localport = localport;
  psockinfo->remoteport = remoteport;
  if (pip4opts != NULL) {
    netwib_er(netwib_buf_append_buf(pip4opts, &psockinfo->ip4opts));
  }
  psockinfo->ip6extsproto = ip6extsproto;
  if (pip6exts != NULL) {
    netwib_er(netwib_buf_append_buf(pip6exts, &psockinfo->ip6exts));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_sockinfo_init_listen(const netwox_sockinfo *pcurrentsockinfo,
                                       netwib_io *pcurrentio,
                                       netwib_port localport,
                                       netwox_sockinfo *pnewsockinfo)
{
  netwox_socktype socktype=NETWOX_SOCKTYPE_UDP_SER;
  netwox_sockinit sockinit;
  netwib_constbuf *pdevice;
  netwib_consteth *plocaleth;
  netwib_ip localip;

  switch(pcurrentsockinfo->socktype) {
  case NETWOX_SOCKTYPE_UDP_CLI :
  case NETWOX_SOCKTYPE_UDP_SER :
  case NETWOX_SOCKTYPE_UDP_MULSER :
    socktype = NETWOX_SOCKTYPE_UDP_SER;
    break;
  case NETWOX_SOCKTYPE_TCP_CLI :
  case NETWOX_SOCKTYPE_TCP_SER :
  case NETWOX_SOCKTYPE_TCP_MULSER :
    socktype = NETWOX_SOCKTYPE_TCP_SER;
    break;
  }

  pdevice = NULL;
  plocaleth = NULL;
  sockinit = pcurrentsockinfo->sockinit;
  switch(sockinit) {
  case NETWOX_SOCKINIT_REALEASY :
    sockinit = NETWOX_SOCKINIT_REAL;
    break;
  case NETWOX_SOCKINIT_VIRT :
    pdevice = &pcurrentsockinfo->device;
    plocaleth = &pcurrentsockinfo->localeth;
    break;
  case NETWOX_SOCKINIT_REAL :
    break;
  }
  netwib_er(netwib_sock_ctl_get_local(pcurrentio, &localip, NULL));

  netwib_er(netwox_sockinfo_init_values(socktype, sockinit,
                                        NETWIB_IPTYPE_UNKNOWN,
                                        pdevice, plocaleth, NULL,
                                        &localip, NULL, localport, 0,
                                        pcurrentsockinfo->pip4opts,
                                        pcurrentsockinfo->ip6extsproto,
                                        pcurrentsockinfo->pip6exts,
                                        pnewsockinfo));
  pnewsockinfo->answeralive = NETWIB_FALSE;

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
netwib_err netwox_sockinfo_init_connect(const netwox_sockinfo *pcurrentsockinfo,
                                        netwib_io *pcurrentio,
                                        netwib_port remoteport,
                                        netwox_sockinfo *pnewsockinfo)
{
  netwox_socktype socktype=NETWOX_SOCKTYPE_UDP_CLI;
  netwox_sockinit sockinit;
  netwib_constbuf *pdevice;
  netwib_consteth *plocaleth;
  netwib_eth *premoteeth, remoteeth;
  netwib_ip localip, remoteip;
  netwib_port localport;

  switch(pcurrentsockinfo->socktype) {
  case NETWOX_SOCKTYPE_UDP_CLI :
  case NETWOX_SOCKTYPE_UDP_SER :
  case NETWOX_SOCKTYPE_UDP_MULSER :
    socktype = NETWOX_SOCKTYPE_UDP_CLI;
    break;
  case NETWOX_SOCKTYPE_TCP_CLI :
  case NETWOX_SOCKTYPE_TCP_SER :
  case NETWOX_SOCKTYPE_TCP_MULSER :
    socktype = NETWOX_SOCKTYPE_TCP_CLI;
    break;
  }

  pdevice = NULL;
  plocaleth = NULL;
  premoteeth = NULL;
  localport = 0;
  sockinit = pcurrentsockinfo->sockinit;
  switch(sockinit) {
  case NETWOX_SOCKINIT_REALEASY :
    sockinit = NETWOX_SOCKINIT_REAL;
    break;
  case NETWOX_SOCKINIT_VIRT :
    pdevice = &pcurrentsockinfo->device;
    plocaleth = &pcurrentsockinfo->localeth;
    netwib_er(netwib_io_ctl_get(pcurrentio,
                                NETWIB_IO_WAYTYPE_SUPPORTED,
                                NETWOX_IO_CTLTYPE_SOCK_REMOTEETH,
                                &remoteeth, NULL));
    premoteeth = &remoteeth;
    netwib_er(netwib_uint32_init_rand(1024, 0xFFFF, &localport));
    break;
  case NETWOX_SOCKINIT_REAL :
    break;
  }
  netwib_er(netwib_sock_ctl_get_local(pcurrentio, &localip, NULL));
  netwib_er(netwib_sock_ctl_get_remote(pcurrentio, &remoteip, NULL));

  netwib_er(netwox_sockinfo_init_values(socktype, sockinit,
                                        NETWIB_IPTYPE_UNKNOWN,
                                        pdevice, plocaleth, premoteeth,
                                        &localip, &remoteip,
                                        localport, remoteport,
                                        pcurrentsockinfo->pip4opts,
                                        pcurrentsockinfo->ip6extsproto,
                                        pcurrentsockinfo->pip6exts,
                                        pnewsockinfo));
  pnewsockinfo->answeralive = NETWIB_FALSE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_sock_init(netwox_sockinfo *psockinfo,
                            netwib_io **ppio)
{
  netwib_port localport;
  netwib_err ret=NETWIB_ERR_OK;

  switch(psockinfo->socktype) {
    case NETWOX_SOCKTYPE_UDP_CLI :
      if (psockinfo->sockinit == NETWOX_SOCKINIT_VIRT) {
        localport = psockinfo->localport;
        if (localport == 0) {
          netwib_er(netwib_uint32_init_rand(1024, 0xFFFFu, &localport));
        }
        ret = netwox_io_init_sockv_udp_cli_full(&psockinfo->device,
                                                &psockinfo->localeth,
                                                &psockinfo->remoteeth,
                                                &psockinfo->localip,
                                                &psockinfo->remoteip,
                                                localport,
                                                psockinfo->remoteport,
                                                psockinfo->pip4opts,
                                                psockinfo->ip6extsproto,
                                                psockinfo->pip6exts,
                                                psockinfo->answeralive, ppio);
      } else {
        ret = netwib_io_init_sock_udp_cli_full(psockinfo->plocalip,
                                               &psockinfo->remoteip,
                                               psockinfo->localport,
                                               psockinfo->remoteport,
                                               psockinfo->pip4opts,
                                               psockinfo->ip6extsproto,
                                               psockinfo->pip6exts,
                                               ppio);
      }
      break;
    case NETWOX_SOCKTYPE_TCP_CLI :
      if (psockinfo->sockinit == NETWOX_SOCKINIT_VIRT) {
        localport = psockinfo->localport;
        if (localport == 0) {
          netwib_er(netwib_uint32_init_rand(1024, 0xFFFFu, &localport));
        }
        ret = netwox_io_init_sockv_tcp_cli_full(&psockinfo->device,
                                                &psockinfo->localeth,
                                                &psockinfo->remoteeth,
                                                psockinfo->plocalip,
                                                psockinfo->premoteip,
                                                localport,
                                                psockinfo->remoteport,
                                                psockinfo->pip4opts,
                                                psockinfo->ip6extsproto,
                                                psockinfo->pip6exts,
                                                psockinfo->answeralive, ppio);
      } else {
        ret = netwib_io_init_sock_tcp_cli_full(psockinfo->plocalip,
                                               &psockinfo->remoteip,
                                               psockinfo->localport,
                                               psockinfo->remoteport,
                                               psockinfo->pip4opts,
                                               psockinfo->ip6extsproto,
                                               psockinfo->pip6exts,
                                               ppio);
      }
      break;
    case NETWOX_SOCKTYPE_UDP_SER :
      if (psockinfo->sockinit == NETWOX_SOCKINIT_VIRT) {
        ret = netwox_io_init_sockv_udp_ser_full(&psockinfo->device,
                                                &psockinfo->localeth,
                                                &psockinfo->localip,
                                                psockinfo->localport,
                                                psockinfo->pip4opts,
                                                psockinfo->ip6extsproto,
                                                psockinfo->pip6exts,
                                                psockinfo->answeralive, ppio);
      } else if (psockinfo->sockinit == NETWOX_SOCKINIT_REAL) {
        ret = netwib_io_init_sock_udp_ser_full(&psockinfo->localip,
                                               psockinfo->localport,
                                               psockinfo->pip4opts,
                                               psockinfo->ip6extsproto,
                                               psockinfo->pip6exts,
                                               ppio);
      } else {
        ret = netwib_io_init_sock_udp_ser_easy(psockinfo->iptype,
                                               psockinfo->localport,
                                               ppio);
      }
      break;
    case NETWOX_SOCKTYPE_TCP_SER :
      if (psockinfo->sockinit == NETWOX_SOCKINIT_VIRT) {
        ret = netwox_io_init_sockv_tcp_ser_full(&psockinfo->device,
                                                &psockinfo->localeth,
                                                &psockinfo->localip,
                                                psockinfo->localport,
                                                psockinfo->pip4opts,
                                                psockinfo->ip6extsproto,
                                                psockinfo->pip6exts,
                                                psockinfo->answeralive, ppio);
      } else if (psockinfo->sockinit == NETWOX_SOCKINIT_REAL) {
        ret = netwib_io_init_sock_tcp_ser_full(&psockinfo->localip,
                                               psockinfo->localport,
                                               psockinfo->pip4opts,
                                               psockinfo->ip6extsproto,
                                               psockinfo->pip6exts,
                                               ppio);
      } else {
        ret = netwib_io_init_sock_tcp_ser_easy(psockinfo->iptype,
                                               psockinfo->localport,
                                               ppio);
      }
      break;
    case NETWOX_SOCKTYPE_UDP_MULSER :
      if (psockinfo->sockinit == NETWOX_SOCKINIT_VIRT) {
        ret = netwox_io_init_sockv_udp_mulser_full(&psockinfo->device,
                                                   &psockinfo->localeth,
                                                   &psockinfo->localip,
                                                   psockinfo->localport,
                                                   psockinfo->pip4opts,
                                                   psockinfo->ip6extsproto,
                                                   psockinfo->pip6exts,
                                                   psockinfo->answeralive,
                                                   ppio);
      } else if (psockinfo->sockinit == NETWOX_SOCKINIT_REAL) {
        ret = netwib_io_init_sock_udp_mulser_full(&psockinfo->localip,
                                                  psockinfo->localport,
                                                  psockinfo->pip4opts,
                                                  psockinfo->ip6extsproto,
                                                  psockinfo->pip6exts,
                                                  ppio);
      } else {
        ret = netwib_io_init_sock_udp_mulser_easy(psockinfo->iptype,
                                                  psockinfo->localport,
                                                  ppio);
      }
      break;
    case NETWOX_SOCKTYPE_TCP_MULSER :
      ret = NETWIB_ERR_PAINVALIDTYPE;
      break;
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_sock_tcp_mulser(netwox_sockinfo *psockinfo,
                                  netwib_sock_tcp_mulser_pf pfunc,
                                  netwib_ptr pinfos)
{
  netwib_err ret=NETWIB_ERR_OK;

  if (psockinfo->socktype != NETWOX_SOCKTYPE_TCP_MULSER) {
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  switch(psockinfo->sockinit) {
  case NETWOX_SOCKINIT_REAL :
    ret = netwib_sock_tcp_mulser_full(&psockinfo->localip,
                                      psockinfo->localport,
                                      psockinfo->pip4opts,
                                      psockinfo->ip6extsproto,
                                      psockinfo->pip6exts,
                                      pfunc,
                                      pinfos);
    break;
  case NETWOX_SOCKINIT_REALEASY :
    ret = netwib_sock_tcp_mulser_easy(psockinfo->iptype,
                                      psockinfo->localport, pfunc,
                                      pinfos);
    break;
  case NETWOX_SOCKINIT_VIRT :
    ret = netwox_sockv_tcp_mulser_full(&psockinfo->device,
                                       &psockinfo->localeth,
                                       &psockinfo->localip,
                                       psockinfo->localport,
                                       psockinfo->pip4opts,
                                       psockinfo->ip6extsproto,
                                       psockinfo->pip6exts,
                                       psockinfo->answeralive, pfunc,
                                       pinfos);
    break;
  }

  return(ret);
}
