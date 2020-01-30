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
  netwib_bool ethsrcset;
  netwib_eth ethsrc;
  netwib_bool ethdstset;
  netwib_eth ethdst;
} netwib_linkhdr_common;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_pkt_convert(netwib_device_dlttype dlttypein,
                                   netwib_constbuf *ppktin,
                                   netwib_pkt_newfield newfieldopt,
                                   netwib_pkt_newfield newfieldreq,
                                   netwib_ip *pgw,
                                   netwib_device_dlttype dlttypeout,
                                   netwib_linkhdr *plinkhdrout,
                                   netwib_bufext *ppktout,
                                   netwib_bool *pdevnumset,
                                   netwib_uint32 *pdevnum)
{
  netwib_linkhdr linkhdrin;
  netwib_linkhdrproto linkhdrprotoin;
  netwib_linkhdr_common linkhdr_common;
  netwib_arphdr arpheader;
  netwib_ip gw, ipsrc, ipdst;
  netwib_buf pktinpayload;
  netwib_bool needtocomputesrc, needtocomputedst;
  netwib_uint32 skipsize;
  netwib_err ret;

  if (plinkhdrout == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  if (pdevnumset != NULL) *pdevnumset = NETWIB_FALSE;

  /* Windows compiler warning : not initialized */
  netwib_er(netwib_eth_init_fields(0,0,0,0,0,0, &linkhdr_common.ethsrc));
  netwib_er(netwib_eth_init_fields(0,0,0,0,0,0, &linkhdr_common.ethdst));

  /* obtain link header */
  netwib_er(netwib_pkt_decode_linkhdr(dlttypein, ppktin, &linkhdrin,
                                      &skipsize));

  /* obtain payload protocol */
  netwib_er(netwib_linkhdr_get_proto(&linkhdrin, &linkhdrprotoin));

  /* obtain link header info */
  linkhdr_common.ethsrcset = NETWIB_FALSE;
  linkhdr_common.ethdstset = NETWIB_FALSE;
  switch (linkhdrin.type) {
    case NETWIB_DEVICE_DLTTYPE_ETHER :
      linkhdr_common.ethsrc = linkhdrin.hdr.ether.src;
      linkhdr_common.ethsrcset = NETWIB_TRUE;
      linkhdr_common.ethdst = linkhdrin.hdr.ether.dst;
      linkhdr_common.ethdstset = NETWIB_TRUE;
      break;
    case NETWIB_DEVICE_DLTTYPE_LINUX_SLL :
      if (linkhdrin.hdr.linuxsll.hatype == NETWIB_LINUXSLLHDRHATYPE_ETHER) {
        if (linkhdrin.hdr.linuxsll.halen == NETWIB_ETH_LEN) {
          netwib_c_memcpy(linkhdr_common.ethsrc.b,
                         linkhdrin.hdr.linuxsll.srcaddr,
                         NETWIB_ETH_LEN);
          linkhdr_common.ethsrcset = NETWIB_TRUE;
        }
        /* also try to set dst */
        if (linkhdrin.hdr.linuxsll.pkttype ==
            NETWIB_LINUXSLLHDRPKTTYPE_BROADCAST) {
          netwib_er(netwib_eth_init_fields(0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                         &linkhdr_common.ethdst));
          linkhdr_common.ethdstset = NETWIB_TRUE;
        }
      }
      break;
    default:
      break;
  }
  /* eventually try to obtain ethsrc and ethdst if they are needed */
  needtocomputesrc = NETWIB_FALSE;
  needtocomputedst = NETWIB_FALSE;
  switch(dlttypeout) {
    case NETWIB_DEVICE_DLTTYPE_ETHER :
    case NETWIB_DEVICE_DLTTYPE_LINUX_SLL :
      if (!linkhdr_common.ethsrcset) {
        switch(newfieldopt) {
          case NETWIB_PKT_NEWFIELD_BLANK :
            break;
          case NETWIB_PKT_NEWFIELD_FILLBLANK :
          case NETWIB_PKT_NEWFIELD_FILL :
            needtocomputesrc = NETWIB_TRUE;
            break;
          default :
            return(NETWIB_ERR_PAINVALIDTYPE);
        }
      }
      if (!linkhdr_common.ethdstset) {
        switch(newfieldreq) {
          case NETWIB_PKT_NEWFIELD_BLANK :
            break;
          case NETWIB_PKT_NEWFIELD_FILLBLANK :
          case NETWIB_PKT_NEWFIELD_FILL :
            needtocomputedst = NETWIB_TRUE;
            break;
          default :
            return(NETWIB_ERR_PAINVALIDTYPE);
        }
      }
      break;
    default :
      /* not needed */
      break;
  }
  if (needtocomputesrc || needtocomputedst) {
    pktinpayload = *ppktin;
    pktinpayload.beginoffset += skipsize;
    switch(linkhdrprotoin) {
      case NETWIB_LINKHDRPROTO_IP4 :
      case NETWIB_LINKHDRPROTO_IP6 :
        if (needtocomputedst) {
          if (pgw != NULL) {
            ret = netwib_eth_init_ip(pgw, &linkhdr_common.ethdst);
            if (ret == NETWIB_ERR_OK) {
              linkhdr_common.ethdstset = NETWIB_TRUE;
              needtocomputedst = NETWIB_FALSE;
            }
          }
        }
        if (needtocomputesrc || needtocomputedst) {
          ret = netwib_priv_ippkt_decode_srcdst(&pktinpayload, &ipsrc, &ipdst);
          if (ret == NETWIB_ERR_OK) {
            if (needtocomputesrc) {
              ret = netwib_eth_init_ip(&ipsrc, &linkhdr_common.ethsrc);
              if (ret == NETWIB_ERR_OK) {
                linkhdr_common.ethsrcset = NETWIB_TRUE;
              }
            }
            if (needtocomputedst) {
              ret = netwib_priv_conf_routes_gw(&ipdst, &gw, pdevnum);
              if (ret == NETWIB_ERR_OK) {
                if (pdevnumset != NULL) *pdevnumset = NETWIB_TRUE;
                ret = netwib_eth_init_ip(&gw, &linkhdr_common.ethdst);
                if (ret == NETWIB_ERR_OK) {
                  linkhdr_common.ethdstset = NETWIB_TRUE;
                }
              }
            }
          }
        }
        break;
      case NETWIB_LINKHDRPROTO_ARP :
      case NETWIB_LINKHDRPROTO_RARP :
        ret = netwib_pkt_decode_arphdr(&pktinpayload, &arpheader, NULL);
        if (ret == NETWIB_ERR_OK) {
          switch(arpheader.op) {
            case NETWIB_ARPHDROP_ARPREQ :
            case NETWIB_ARPHDROP_RARPREQ :
              if (needtocomputesrc) {
                linkhdr_common.ethsrc = arpheader.ethsrc;
                linkhdr_common.ethsrcset = NETWIB_TRUE;
              }
              if (needtocomputedst) {
                netwib_er(netwib_eth_init_fields(0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                               &linkhdr_common.ethdst));
                linkhdr_common.ethdstset = NETWIB_TRUE;
              }
              break;
            case NETWIB_ARPHDROP_ARPREP :
            case NETWIB_ARPHDROP_RARPREP :
              if (needtocomputesrc) {
                linkhdr_common.ethsrc = arpheader.ethsrc;
                linkhdr_common.ethsrcset = NETWIB_TRUE;
              }
              if (needtocomputedst) {
                linkhdr_common.ethdst = arpheader.ethdst;
                linkhdr_common.ethdstset = NETWIB_TRUE;
              }
              break;
          }
          break;
        }
        break;
        break;
      default :
        return(NETWIB_ERR_LONOTIMPLEMENTED);
    }
  }

  /* create linkhdrout, depending on input */
  netwib_er(netwib_linkhdr_initdefault(dlttypeout, plinkhdrout));
  netwib_er(netwib_linkhdr_set_proto(plinkhdrout, linkhdrprotoin));
  switch(dlttypeout) {
    case NETWIB_DEVICE_DLTTYPE_ETHER :
      if (linkhdr_common.ethsrcset) {
        plinkhdrout->hdr.ether.src = linkhdr_common.ethsrc;
      } else {
        switch(newfieldopt) {
          case NETWIB_PKT_NEWFIELD_BLANK :
          case NETWIB_PKT_NEWFIELD_FILLBLANK :
            netwib_c_memset(plinkhdrout->hdr.ether.src.b, 0, NETWIB_ETH_LEN);
            break;
          case NETWIB_PKT_NEWFIELD_FILL :
            /* don't know what to put */
            return(NETWIB_ERR_NOTCONVERTED);
            break;
          default :
            return(NETWIB_ERR_PAINVALIDTYPE);
        }
      }
      if (linkhdr_common.ethdstset) {
        plinkhdrout->hdr.ether.dst = linkhdr_common.ethdst;
      } else {
        switch(newfieldreq) {
          case NETWIB_PKT_NEWFIELD_BLANK :
          case NETWIB_PKT_NEWFIELD_FILLBLANK :
            netwib_c_memset(plinkhdrout->hdr.ether.dst.b, 0, NETWIB_ETH_LEN);
            break;
          case NETWIB_PKT_NEWFIELD_FILL :
            /* don't know what to put */
            return(NETWIB_ERR_NOTCONVERTED);
            break;
          default :
            return(NETWIB_ERR_PAINVALIDTYPE);
        }
      }
      break;
    case NETWIB_DEVICE_DLTTYPE_NULL :
      /* except type, there is nothing else to set */
      break;
    case NETWIB_DEVICE_DLTTYPE_LOOP :
      /* except type, there is nothing else to set */
      break;
    case NETWIB_DEVICE_DLTTYPE_RAW :
    case NETWIB_DEVICE_DLTTYPE_RAW4 :
    case NETWIB_DEVICE_DLTTYPE_RAW6 :
      /* there is nothing to set */
      break;
    case NETWIB_DEVICE_DLTTYPE_PPP :
      switch(newfieldreq) {
        case NETWIB_PKT_NEWFIELD_BLANK :
        case NETWIB_PKT_NEWFIELD_FILLBLANK :
          plinkhdrout->hdr.ppp.address = 0;
          plinkhdrout->hdr.ppp.control = 0;
          break;
        case NETWIB_PKT_NEWFIELD_FILL :
          /* don't know what to put in address and control */
          return(NETWIB_ERR_LONOTIMPLEMENTED);
          break;
        default :
          return(NETWIB_ERR_PAINVALIDTYPE);
      }
      break;
    case NETWIB_DEVICE_DLTTYPE_LINUX_SLL :
      if (linkhdr_common.ethsrcset) {
          plinkhdrout->hdr.linuxsll.hatype = NETWIB_LINUXSLLHDRHATYPE_ETHER;
          plinkhdrout->hdr.linuxsll.halen = NETWIB_ETH_LEN;
          netwib_c_memcpy(plinkhdrout->hdr.linuxsll.srcaddr,
                         linkhdr_common.ethsrc.b,
                         NETWIB_ETH_LEN);
      } else {
        switch(newfieldopt) {
          case NETWIB_PKT_NEWFIELD_BLANK :
          case NETWIB_PKT_NEWFIELD_FILLBLANK :
            plinkhdrout->hdr.linuxsll.hatype = NETWIB_LINUXSLLHDRHATYPE_ETHER;
            plinkhdrout->hdr.linuxsll.halen = NETWIB_ETH_LEN;
            netwib_c_memset(plinkhdrout->hdr.linuxsll.srcaddr, 0,
                           NETWIB_ETH_LEN);
            break;
          case NETWIB_PKT_NEWFIELD_FILL :
            /* don't know what to put */
            return(NETWIB_ERR_LONOTIMPLEMENTED);
            break;
          default :
            return(NETWIB_ERR_PAINVALIDTYPE);
        }
      }
      if (linkhdr_common.ethdstset) {
        if (linkhdr_common.ethdst.b[0] == 0xFF &&
            linkhdr_common.ethdst.b[1] == 0xFF &&
            linkhdr_common.ethdst.b[2] == 0xFF &&
            linkhdr_common.ethdst.b[3] == 0xFF &&
            linkhdr_common.ethdst.b[4] == 0xFF &&
            linkhdr_common.ethdst.b[5] == 0xFF ) {
          plinkhdrout->hdr.linuxsll.pkttype =
            NETWIB_LINUXSLLHDRPKTTYPE_BROADCAST;
        } else {
          plinkhdrout->hdr.linuxsll.pkttype = NETWIB_LINUXSLLHDRPKTTYPE_HOST;
        }
      } else {
        /* migth be wrong, but don't know what else to put */
        plinkhdrout->hdr.linuxsll.pkttype = NETWIB_LINUXSLLHDRPKTTYPE_HOST;
      }
      break;
    default :
      return(NETWIB_ERR_LONOTIMPLEMENTED);
      break;
  }

  if (ppktout != NULL) {
    netwib_er(netwib_buf_init_ext_buf(ppktin, ppktout));
    ppktout->beginoffset += skipsize;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pkt_convert(netwib_device_dlttype dlttypein,
                              netwib_constbuf *ppktin,
                              netwib_pkt_newfield newfieldopt,
                              netwib_pkt_newfield newfieldreq,
                              netwib_device_dlttype dlttypeout,
                              netwib_linkhdr *plinkhdrout,
                              netwib_bufext *ppktout)
{
  return(netwib_priv_pkt_convert(dlttypein, ppktin, newfieldopt, newfieldreq,
                                 NULL, dlttypeout, plinkhdrout, ppktout,
                                 NULL, NULL));

}
