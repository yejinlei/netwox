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
static netwib_err netwox_dhcp4opt_decode_string(netwox_dhcp4opttype type,
                                                netwib_conststring *ppc)
{
  switch(type) {
  case NETWOX_DHCP4OPTTYPE_UNKNOWN : *ppc = "unknown"; break;
  case NETWOX_DHCP4OPTTYPE_PAD : *ppc = "padding"; break;
  case NETWOX_DHCP4OPTTYPE_END : *ppc = "end"; break;
  case NETWOX_DHCP4OPTTYPE_SUBNETMASK : *ppc = "subnetmask"; break;
  case NETWOX_DHCP4OPTTYPE_GATEWAYS : *ppc = "gateways"; break;
  case NETWOX_DHCP4OPTTYPE_TIMESERVERS : *ppc = "timeservers"; break;
  case NETWOX_DHCP4OPTTYPE_NAMESERVERS : *ppc = "nameservers"; break;
  case NETWOX_DHCP4OPTTYPE_DNSSERVERS : *ppc = "dnsservers"; break;
  case NETWOX_DHCP4OPTTYPE_LOGSERVERS : *ppc = "logservers"; break;
  case NETWOX_DHCP4OPTTYPE_LPRSERVERS : *ppc = "lprservers"; break;
  case NETWOX_DHCP4OPTTYPE_HOSTNAME : *ppc = "hostname"; break;
  case NETWOX_DHCP4OPTTYPE_DOMAINNAME : *ppc = "domainname"; break;
  case NETWOX_DHCP4OPTTYPE_BROADCASTAD : *ppc = "broadcastad"; break;
  case NETWOX_DHCP4OPTTYPE_PERFORMROUTDISC : *ppc = "performroutdisc"; break;
  case NETWOX_DHCP4OPTTYPE_STATICROUTES : *ppc = "staticroutes"; break;
  case NETWOX_DHCP4OPTTYPE_NISDOMAIN : *ppc = "nisdomain"; break;
  case NETWOX_DHCP4OPTTYPE_NISSERVERS : *ppc = "nisservers"; break;
  case NETWOX_DHCP4OPTTYPE_NTPSERVERS : *ppc = "ntpservers"; break;
  case NETWOX_DHCP4OPTTYPE_VENDORINFO : *ppc = "vendorinfo"; break;
  case NETWOX_DHCP4OPTTYPE_NETBIOSNS : *ppc = "netbiosns"; break;
  case NETWOX_DHCP4OPTTYPE_NETBIOSDD : *ppc = "netbiosdd"; break;
  case NETWOX_DHCP4OPTTYPE_NETBIOSNODETYPE : *ppc = "netbiosnodetype"; break;
  case NETWOX_DHCP4OPTTYPE_NETBIOSSCOPE : *ppc = "netbiosscope"; break;
  case NETWOX_DHCP4OPTTYPE_REQUESTEDIPAD : *ppc = "requestedipad"; break;
  case NETWOX_DHCP4OPTTYPE_IPADLEASETIME : *ppc = "ipadleasetime"; break;
  case NETWOX_DHCP4OPTTYPE_MSGTYPE : *ppc = "msgtype"; break;
  case NETWOX_DHCP4OPTTYPE_VENDORID : *ppc = "vendorid"; break;
  case NETWOX_DHCP4OPTTYPE_SERVERID : *ppc = "serverid"; break;
  case NETWOX_DHCP4OPTTYPE_REQLIST : *ppc = "reqlist"; break;
  case NETWOX_DHCP4OPTTYPE_ERRORMSG : *ppc = "errormsg"; break;
  case NETWOX_DHCP4OPTTYPE_RENEWALTIME : *ppc = "renewaltime"; break;
  case NETWOX_DHCP4OPTTYPE_REBINDINGTIME : *ppc = "rebindingtime"; break;
  case NETWOX_DHCP4OPTTYPE_CLIENTID : *ppc = "clientid"; break;
  case NETWOX_DHCP4OPTTYPE_NISPDOMAIN : *ppc = "nispdomain"; break;
  case NETWOX_DHCP4OPTTYPE_NISPSERVERS : *ppc = "nispserver"; break;
  case NETWOX_DHCP4OPTTYPE_TFTPSERVERS : *ppc = "tftpservers"; break;
  case NETWOX_DHCP4OPTTYPE_SMTPSERVERS : *ppc = "smtpservers"; break;
  case NETWOX_DHCP4OPTTYPE_POP3SERVERS : *ppc = "pop3servers"; break;
  case NETWOX_DHCP4OPTTYPE_NNTPSERVERS : *ppc = "nntpservers"; break;
  case NETWOX_DHCP4OPTTYPE_WWWSERVERS : *ppc = "wwwservers"; break;
  case NETWOX_DHCP4OPTTYPE_IRCSERVERS : *ppc = "ircservers"; break;
  default : *ppc = "unknown"; break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_dhcp4opt_initdefault_ip(netwib_ip *pip)
{
  pip->iptype = NETWIB_IPTYPE_IP4;
  pip->ipvalue.ip4 = 0;
  return(NETWIB_ERR_OK);
}
static netwib_err netwox_dhcp4opt_initdefault_iparray(netwox_dhcp4opt_iparray *pia)
{
  pia->storedvalues = 0;
  return(NETWIB_ERR_OK);
}
static netwib_err netwox_dhcp4opt_initdefault_bufext(netwib_bufext *pbuf)
{
  netwib_er(netwib_buf_init_ext_empty(pbuf));
  return(NETWIB_ERR_OK);
}
static netwib_err netwox_dhcp4opt_initdefault_staticroutes(netwox_dhcp4opt_staticroutes *psr)
{
  psr->storedvalues = 0;
  return(NETWIB_ERR_OK);
}
static netwib_err netwox_dhcp4opt_initdefault_uint32(netwib_uint32 *pui)
{
  *pui = 0;
  return(NETWIB_ERR_OK);
}
static netwib_err netwox_dhcp4opt_initdefault_bool(netwib_bool *pb)
{
  *pb = NETWIB_FALSE;
  return(NETWIB_ERR_OK);
}
static netwib_err netwox_dhcp4opt_initdefault_msgtype(netwox_dhcp4opt_msgtype *ptype)
{
  *ptype = (netwox_dhcp4opt_msgtype)0;
  return(NETWIB_ERR_OK);
}
static netwib_err netwox_dhcp4opt_initdefault_netbiosnodetype(netwox_dhcp4opt_netbiosnodetype *ptype)
{
  *ptype = (netwox_dhcp4opt_netbiosnodetype)0;
  return(NETWIB_ERR_OK);
}
static netwib_err netwox_dhcp4opt_initdefault_clientid(netwox_dhcp4opt_clientid *pci)
{
  pci->type = 0;
  netwib_er(netwib_buf_init_ext_empty(&pci->id));
  return(NETWIB_ERR_OK);
}
static netwib_err netwox_dhcp4opt_initdefault_reqlist(netwox_dhcp4opt_reqlist *prl)
{
  prl->storedvalues = 0;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dhcp4opt_initdefault(netwox_dhcp4opttype dhcp4opttype,
                                       netwox_dhcp4opt *pdhcp4opt)
{
  pdhcp4opt->type = dhcp4opttype;
  switch(dhcp4opttype) {
  case NETWOX_DHCP4OPTTYPE_UNKNOWN :
    netwib_er(netwib_buf_init_ext_empty(&pdhcp4opt->opt.unknown));
    break;
  case NETWOX_DHCP4OPTTYPE_PAD :
  case NETWOX_DHCP4OPTTYPE_END :
    /* nothing to set */
    break;
  case NETWOX_DHCP4OPTTYPE_SUBNETMASK :
    netwib_er(netwox_dhcp4opt_initdefault_ip(&pdhcp4opt->opt.subnetmask));
    break;
  case NETWOX_DHCP4OPTTYPE_GATEWAYS :
    netwib_er(netwox_dhcp4opt_initdefault_iparray(&pdhcp4opt->opt.gateways));
    break;
  case NETWOX_DHCP4OPTTYPE_TIMESERVERS :
    netwib_er(netwox_dhcp4opt_initdefault_iparray(&pdhcp4opt->opt.timeservers));
    break;
  case NETWOX_DHCP4OPTTYPE_NAMESERVERS :
    netwib_er(netwox_dhcp4opt_initdefault_iparray(&pdhcp4opt->opt.nameservers));
    break;
  case NETWOX_DHCP4OPTTYPE_DNSSERVERS :
    netwib_er(netwox_dhcp4opt_initdefault_iparray(&pdhcp4opt->opt.dnsservers));
    break;
  case NETWOX_DHCP4OPTTYPE_LOGSERVERS :
    netwib_er(netwox_dhcp4opt_initdefault_iparray(&pdhcp4opt->opt.logservers));
    break;
  case NETWOX_DHCP4OPTTYPE_LPRSERVERS :
    netwib_er(netwox_dhcp4opt_initdefault_iparray(&pdhcp4opt->opt.lprservers));
    break;
  case NETWOX_DHCP4OPTTYPE_HOSTNAME :
    netwib_er(netwox_dhcp4opt_initdefault_bufext(&pdhcp4opt->opt.hostname));
    break;
  case NETWOX_DHCP4OPTTYPE_DOMAINNAME :
    netwib_er(netwox_dhcp4opt_initdefault_bufext(&pdhcp4opt->opt.domainname));
    break;
  case NETWOX_DHCP4OPTTYPE_BROADCASTAD :
    netwib_er(netwox_dhcp4opt_initdefault_ip(&pdhcp4opt->opt.broadcastad));
    break;
  case NETWOX_DHCP4OPTTYPE_PERFORMROUTDISC :
    netwib_er(netwox_dhcp4opt_initdefault_bool(&pdhcp4opt->opt.performroutdisc));
    break;
  case NETWOX_DHCP4OPTTYPE_STATICROUTES :
    netwib_er(netwox_dhcp4opt_initdefault_staticroutes(&pdhcp4opt->opt.staticroutes));
    break;
  case NETWOX_DHCP4OPTTYPE_NISDOMAIN :
    netwib_er(netwox_dhcp4opt_initdefault_bufext(&pdhcp4opt->opt.nisdomain));
    break;
  case NETWOX_DHCP4OPTTYPE_NISSERVERS :
    netwib_er(netwox_dhcp4opt_initdefault_iparray(&pdhcp4opt->opt.nisservers));
    break;
  case NETWOX_DHCP4OPTTYPE_NTPSERVERS :
    netwib_er(netwox_dhcp4opt_initdefault_iparray(&pdhcp4opt->opt.ntpservers));
    break;
  case NETWOX_DHCP4OPTTYPE_VENDORINFO :
    netwib_er(netwox_dhcp4opt_initdefault_bufext(&pdhcp4opt->opt.vendorinfo));
    break;
  case NETWOX_DHCP4OPTTYPE_NETBIOSNS :
    netwib_er(netwox_dhcp4opt_initdefault_iparray(&pdhcp4opt->opt.netbiosns));
    break;
  case NETWOX_DHCP4OPTTYPE_NETBIOSDD :
    netwib_er(netwox_dhcp4opt_initdefault_iparray(&pdhcp4opt->opt.netbiosdd));
    break;
  case NETWOX_DHCP4OPTTYPE_NETBIOSNODETYPE :
    netwib_er(netwox_dhcp4opt_initdefault_netbiosnodetype(&pdhcp4opt->opt.netbiosnodetype));
    break;
  case NETWOX_DHCP4OPTTYPE_NETBIOSSCOPE :
    netwib_er(netwox_dhcp4opt_initdefault_bufext(&pdhcp4opt->opt.netbiosscope));
    break;
  case NETWOX_DHCP4OPTTYPE_REQUESTEDIPAD :
    netwib_er(netwox_dhcp4opt_initdefault_ip(&pdhcp4opt->opt.requestedipad));
    break;
  case NETWOX_DHCP4OPTTYPE_IPADLEASETIME :
     netwib_er(netwox_dhcp4opt_initdefault_uint32(&pdhcp4opt->opt.ipadleasetime));
   break;
  case NETWOX_DHCP4OPTTYPE_MSGTYPE :
    netwib_er(netwox_dhcp4opt_initdefault_msgtype(&pdhcp4opt->opt.msgtype));
    break;
  case NETWOX_DHCP4OPTTYPE_SERVERID :
    netwib_er(netwox_dhcp4opt_initdefault_ip(&pdhcp4opt->opt.serverid));
    break;
  case NETWOX_DHCP4OPTTYPE_REQLIST :
    netwib_er(netwox_dhcp4opt_initdefault_reqlist(&pdhcp4opt->opt.reqlist));
    break;
  case NETWOX_DHCP4OPTTYPE_ERRORMSG :
    netwib_er(netwox_dhcp4opt_initdefault_bufext(&pdhcp4opt->opt.errormsg));
    break;
  case NETWOX_DHCP4OPTTYPE_RENEWALTIME :
     netwib_er(netwox_dhcp4opt_initdefault_uint32(&pdhcp4opt->opt.renewaltime));
    break;
  case NETWOX_DHCP4OPTTYPE_REBINDINGTIME :
     netwib_er(netwox_dhcp4opt_initdefault_uint32(&pdhcp4opt->opt.rebindingtime));
    break;
  case NETWOX_DHCP4OPTTYPE_VENDORID :
    netwib_er(netwox_dhcp4opt_initdefault_bufext(&pdhcp4opt->opt.vendorid));
    break;
  case NETWOX_DHCP4OPTTYPE_CLIENTID :
    netwib_er(netwox_dhcp4opt_initdefault_clientid(&pdhcp4opt->opt.clientid));
    break;
  case NETWOX_DHCP4OPTTYPE_NISPDOMAIN :
    netwib_er(netwox_dhcp4opt_initdefault_bufext(&pdhcp4opt->opt.nispdomain));
    break;
  case NETWOX_DHCP4OPTTYPE_NISPSERVERS :
    netwib_er(netwox_dhcp4opt_initdefault_iparray(&pdhcp4opt->opt.nispservers));
    break;
  case NETWOX_DHCP4OPTTYPE_TFTPSERVERS :
    netwib_er(netwox_dhcp4opt_initdefault_iparray(&pdhcp4opt->opt.tftpservers));
    break;
  case NETWOX_DHCP4OPTTYPE_SMTPSERVERS :
    netwib_er(netwox_dhcp4opt_initdefault_iparray(&pdhcp4opt->opt.smtpservers));
    break;
  case NETWOX_DHCP4OPTTYPE_POP3SERVERS :
    netwib_er(netwox_dhcp4opt_initdefault_iparray(&pdhcp4opt->opt.pop3servers));
    break;
  case NETWOX_DHCP4OPTTYPE_NNTPSERVERS :
    netwib_er(netwox_dhcp4opt_initdefault_iparray(&pdhcp4opt->opt.nntpservers));
    break;
  case NETWOX_DHCP4OPTTYPE_WWWSERVERS :
    netwib_er(netwox_dhcp4opt_initdefault_iparray(&pdhcp4opt->opt.wwwservers));
    break;
  case NETWOX_DHCP4OPTTYPE_IRCSERVERS :
    netwib_er(netwox_dhcp4opt_initdefault_iparray(&pdhcp4opt->opt.ircservers));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_pkt_append_dhcp4opt_ip(netwib_constip *pip,
                                                netwib_buf *ppkt)
{
  netwib_data data;

  if (pip->iptype != NETWIB_IPTYPE_IP4) {
    return(NETWIB_ERR_PAIPTYPENOT4);
  }

  netwib_er(netwib_buf_wantspace(ppkt, 5, &data));
  netwib__data_append_uint8(data, 4);
  netwib__data_append_uint32(data, pip->ipvalue.ip4);
  ppkt->endoffset += 5;

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_pkt_append_dhcp4opt_iparray(const netwox_dhcp4opt_iparray *pia,
                                                     netwib_buf *ppkt)
{
  netwib_data data;
  netwib_uint32 i;

  if (pia->storedvalues > NETWOX_DHCP4OPT_IPARRAY_LEN) {
    return(NETWIB_ERR_PATOOHIGH);
  }
  if (pia->storedvalues == 0) {
    return(NETWIB_ERR_PATOOLOW);
  }

  netwib_er(netwib_buf_wantspace(ppkt, 1+4*pia->storedvalues, &data));
  netwib__data_append_uint8(data, 4*pia->storedvalues);
  for (i = 0; i < pia->storedvalues; i++) {
    if (pia->ip[i].iptype != NETWIB_IPTYPE_IP4) {
      return(NETWIB_ERR_PAIPTYPENOT4);
    }
    netwib__data_append_uint32(data, pia->ip[i].ipvalue.ip4);
  }
  ppkt->endoffset += 1+4*pia->storedvalues;

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_pkt_append_dhcp4opt_bufext(netwib_constbuf *pbuf,
                                                    netwib_buf *ppkt)
{
  netwib_data data, bufptr;
  netwib_uint32 bufsize;

  bufsize = netwib__buf_ref_data_size(pbuf);
  if (bufsize > 0xFF) {
    return(NETWIB_ERR_PATOOHIGH);
  }
  bufptr = netwib__buf_ref_data_ptr(pbuf);

  netwib_er(netwib_buf_wantspace(ppkt, 1+bufsize, &data));
  netwib__data_append_uint8(data, bufsize);
  netwib_c_memcpy(data, bufptr, bufsize);
  ppkt->endoffset += 1+bufsize;

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_pkt_append_dhcp4opt_staticroutes(const netwox_dhcp4opt_staticroutes *psr,
                                                          netwib_buf *ppkt)
{
  netwib_data data;
  netwib_uint32 i;

  if (psr->storedvalues > NETWOX_DHCP4OPT_STATICROUTE_LEN) {
    return(NETWIB_ERR_PATOOHIGH);
  }
  if (psr->storedvalues == 0) {
    return(NETWIB_ERR_PATOOLOW);
  }

  netwib_er(netwib_buf_wantspace(ppkt, 1+8*psr->storedvalues, &data));
  netwib__data_append_uint8(data, 8*psr->storedvalues);
  for (i = 0; i < psr->storedvalues; i++) {
    if (psr->dst[i].iptype != NETWIB_IPTYPE_IP4) {
      return(NETWIB_ERR_PAIPTYPENOT4);
    }
    netwib__data_append_uint32(data, psr->dst[i].ipvalue.ip4);
    if (psr->gw[i].iptype != NETWIB_IPTYPE_IP4) {
      return(NETWIB_ERR_PAIPTYPENOT4);
    }
    netwib__data_append_uint32(data, psr->gw[i].ipvalue.ip4);
  }
  ppkt->endoffset += 1+8*psr->storedvalues;

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_pkt_append_dhcp4opt_uint32(netwib_uint32 ui,
                                                    netwib_buf *ppkt)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(ppkt, 5, &data));
  netwib__data_append_uint8(data, 4);
  netwib__data_append_uint32(data, ui);
  ppkt->endoffset += 5;

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_pkt_append_dhcp4opt_bool(netwib_bool b,
                                                  netwib_buf *ppkt)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(ppkt, 2, &data));
  data[0] = 4;
  if (b) {
    data[1] = 1;
  } else {
    data[1] = 0;
  }
  ppkt->endoffset += 2;

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_pkt_append_dhcp4opt_msgtype(netwox_dhcp4opt_msgtype type,
                                                     netwib_buf *ppkt)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(ppkt, 2, &data));
  data[0] = 1;
  data[1] = (netwib_byte)type;
  ppkt->endoffset += 2;

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_pkt_append_dhcp4opt_netbiosnodetype(netwox_dhcp4opt_netbiosnodetype type,
                                                             netwib_buf *ppkt)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(ppkt, 2, &data));
  data[0] = 1;
  data[1] = (netwib_byte)type;
  ppkt->endoffset += 2;

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_pkt_append_dhcp4opt_clientid(const netwox_dhcp4opt_clientid *pci,
                                                      netwib_buf *ppkt)
{
  netwib_data data, bufptr;
  netwib_uint32 bufsize;

  bufsize = netwib__buf_ref_data_size(&pci->id);
  if (bufsize > 0xFE) {
    return(NETWIB_ERR_PATOOHIGH);
  }
  bufptr = netwib__buf_ref_data_ptr(&pci->id);

  netwib_er(netwib_buf_wantspace(ppkt, 2+bufsize, &data));
  *data++ = (netwib_byte)(1+bufsize);
  *data++ = (netwib_byte)pci->type;
  netwib_c_memcpy(data, bufptr, bufsize);
  ppkt->endoffset += 2+bufsize;

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_pkt_append_dhcp4opt_reqlist(const netwox_dhcp4opt_reqlist *prl,
                                                     netwib_buf *ppkt)
{
  netwib_data data;
  netwib_uint32 i;

  if (prl->storedvalues > NETWOX_DHCP4OPT_REQLIST_LEN) {
    return(NETWIB_ERR_PATOOHIGH);
  }
  if (prl->storedvalues == 0) {
    return(NETWIB_ERR_PATOOLOW);
  }

  netwib_er(netwib_buf_wantspace(ppkt, 1+prl->storedvalues, &data));
  *data++ = (netwib_byte)prl->storedvalues;
  for (i = 0; i < prl->storedvalues; i++) {
    *data++ = (netwib_byte)prl->type[i];
  }
  ppkt->endoffset += 1+prl->storedvalues;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_dhcp4opt(netwox_constdhcp4opt *pdhcp4opt,
                                      netwib_buf *ppkt)
{
  netwib_data data;

  if (pdhcp4opt->type == NETWOX_DHCP4OPTTYPE_UNKNOWN) {
    /* special case: all is stored in the buffer */
    netwib_er(netwib_buf_append_buf(&pdhcp4opt->opt.unknown, ppkt));
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_buf_wantspace(ppkt, 1, &data));
  *data = (netwib_byte)pdhcp4opt->type;
  ppkt->endoffset++;

  switch(pdhcp4opt->type) {
  case NETWOX_DHCP4OPTTYPE_PAD :
  case NETWOX_DHCP4OPTTYPE_END :
    /* nothing to add */
    break;
  case NETWOX_DHCP4OPTTYPE_SUBNETMASK :
    netwib_er(netwox_pkt_append_dhcp4opt_ip(&pdhcp4opt->opt.subnetmask, ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_GATEWAYS :
    netwib_er(netwox_pkt_append_dhcp4opt_iparray(&pdhcp4opt->opt.gateways,
                                                 ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_TIMESERVERS :
    netwib_er(netwox_pkt_append_dhcp4opt_iparray(&pdhcp4opt->opt.timeservers,
                                                 ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_NAMESERVERS :
    netwib_er(netwox_pkt_append_dhcp4opt_iparray(&pdhcp4opt->opt.nameservers,
                                                 ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_DNSSERVERS :
    netwib_er(netwox_pkt_append_dhcp4opt_iparray(&pdhcp4opt->opt.dnsservers,
                                                 ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_LOGSERVERS :
    netwib_er(netwox_pkt_append_dhcp4opt_iparray(&pdhcp4opt->opt.logservers,
                                                 ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_LPRSERVERS :
    netwib_er(netwox_pkt_append_dhcp4opt_iparray(&pdhcp4opt->opt.lprservers,
                                                 ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_HOSTNAME :
    netwib_er(netwox_pkt_append_dhcp4opt_bufext(&pdhcp4opt->opt.hostname,
                                                ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_DOMAINNAME :
    netwib_er(netwox_pkt_append_dhcp4opt_bufext(&pdhcp4opt->opt.domainname,
                                                ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_BROADCASTAD :
    netwib_er(netwox_pkt_append_dhcp4opt_ip(&pdhcp4opt->opt.broadcastad,
                                            ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_PERFORMROUTDISC :
    netwib_er(netwox_pkt_append_dhcp4opt_bool(pdhcp4opt->opt.performroutdisc,
                                              ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_STATICROUTES :
    netwib_er(netwox_pkt_append_dhcp4opt_staticroutes(&pdhcp4opt->opt.staticroutes, ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_NISDOMAIN :
    netwib_er(netwox_pkt_append_dhcp4opt_bufext(&pdhcp4opt->opt.nisdomain,
                                                ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_NISSERVERS :
    netwib_er(netwox_pkt_append_dhcp4opt_iparray(&pdhcp4opt->opt.nisservers,
                                                 ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_NTPSERVERS :
    netwib_er(netwox_pkt_append_dhcp4opt_iparray(&pdhcp4opt->opt.ntpservers,
                                                 ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_VENDORINFO :
    netwib_er(netwox_pkt_append_dhcp4opt_bufext(&pdhcp4opt->opt.vendorinfo,
                                                ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_NETBIOSNS :
    netwib_er(netwox_pkt_append_dhcp4opt_iparray(&pdhcp4opt->opt.netbiosns,
                                                 ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_NETBIOSDD :
    netwib_er(netwox_pkt_append_dhcp4opt_iparray(&pdhcp4opt->opt.netbiosdd,
                                                 ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_NETBIOSNODETYPE :
    netwib_er(netwox_pkt_append_dhcp4opt_netbiosnodetype(pdhcp4opt->opt.netbiosnodetype, ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_NETBIOSSCOPE :
    netwib_er(netwox_pkt_append_dhcp4opt_bufext(&pdhcp4opt->opt.netbiosscope,
                                                ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_REQUESTEDIPAD :
    netwib_er(netwox_pkt_append_dhcp4opt_ip(&pdhcp4opt->opt.requestedipad,
                                            ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_IPADLEASETIME :
    netwib_er(netwox_pkt_append_dhcp4opt_uint32(pdhcp4opt->opt.ipadleasetime,
                                                ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_MSGTYPE :
    netwib_er(netwox_pkt_append_dhcp4opt_msgtype(pdhcp4opt->opt.msgtype,
                                                 ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_VENDORID :
    netwib_er(netwox_pkt_append_dhcp4opt_bufext(&pdhcp4opt->opt.vendorid,
                                                ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_SERVERID :
    netwib_er(netwox_pkt_append_dhcp4opt_ip(&pdhcp4opt->opt.serverid,
                                            ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_REQLIST :
    netwib_er(netwox_pkt_append_dhcp4opt_reqlist(&pdhcp4opt->opt.reqlist,
                                                 ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_ERRORMSG :
    netwib_er(netwox_pkt_append_dhcp4opt_bufext(&pdhcp4opt->opt.errormsg,
                                                ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_RENEWALTIME :
    netwib_er(netwox_pkt_append_dhcp4opt_uint32(pdhcp4opt->opt.renewaltime,
                                                ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_REBINDINGTIME :
    netwib_er(netwox_pkt_append_dhcp4opt_uint32(pdhcp4opt->opt.rebindingtime,
                                                ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_CLIENTID :
    netwib_er(netwox_pkt_append_dhcp4opt_clientid(&pdhcp4opt->opt.clientid,
                                                  ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_NISPDOMAIN :
    netwib_er(netwox_pkt_append_dhcp4opt_bufext(&pdhcp4opt->opt.nispdomain,
                                                ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_NISPSERVERS :
    netwib_er(netwox_pkt_append_dhcp4opt_iparray(&pdhcp4opt->opt.nispservers,
                                                 ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_TFTPSERVERS :
    netwib_er(netwox_pkt_append_dhcp4opt_iparray(&pdhcp4opt->opt.tftpservers,
                                                 ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_SMTPSERVERS :
    netwib_er(netwox_pkt_append_dhcp4opt_iparray(&pdhcp4opt->opt.smtpservers,
                                                 ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_POP3SERVERS :
    netwib_er(netwox_pkt_append_dhcp4opt_iparray(&pdhcp4opt->opt.pop3servers,
                                                 ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_NNTPSERVERS :
    netwib_er(netwox_pkt_append_dhcp4opt_iparray(&pdhcp4opt->opt.nntpservers,
                                                 ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_WWWSERVERS :
    netwib_er(netwox_pkt_append_dhcp4opt_iparray(&pdhcp4opt->opt.wwwservers,
                                                 ppkt));
    break;
  case NETWOX_DHCP4OPTTYPE_IRCSERVERS :
    netwib_er(netwox_pkt_append_dhcp4opt_iparray(&pdhcp4opt->opt.ircservers,
                                                 ppkt));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_pkt_decode_dhcp4opt_ip(netwib_constdata data,
                                                netwib_uint32 datasize,
                                                netwib_ip *pip)
{
  if (datasize != 4) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  pip->iptype = NETWIB_IPTYPE_IP4;
  pip->ipvalue.ip4 = netwib_c2_uint32_4(data[0], data[1], data[2], data[3]);

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_pkt_decode_dhcp4opt_iparray(netwib_constdata data,
                                                     netwib_uint32 datasize,
                                                     netwox_dhcp4opt_iparray *pia)
{
  netwib_uint32 i, i4;

  if (datasize == 0) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  if (datasize % 4) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  if (datasize > 4*NETWOX_DHCP4OPT_IPARRAY_LEN) {
    /* we could return an error, but it's better to give back only firsts */
    datasize = 4*NETWOX_DHCP4OPT_IPARRAY_LEN;
  }

  pia->storedvalues = datasize/4;
  for (i = 0; i < pia->storedvalues; i++) {
    i4 = 4 * i;
    pia->ip[i].iptype = NETWIB_IPTYPE_IP4;
    pia->ip[i].ipvalue.ip4 = netwib_c2_uint32_4(data[i4], data[i4+1],
                                                data[i4+2], data[i4+3]);
  }

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_pkt_decode_dhcp4opt_bufext(netwib_data data,
                                                    netwib_uint32 datasize,
                                                    netwib_bufext *pbuf)
{

  if (datasize == 0) {
    netwib_er(netwib_buf_init_ext_empty(pbuf));
  } else {
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, pbuf));
  }

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_pkt_decode_dhcp4opt_staticroutes(netwib_constdata data,
                                                          netwib_uint32 datasize,
                                                          netwox_dhcp4opt_staticroutes *psr)
{
  netwib_uint32 i, i8;

  if (datasize == 0) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  if (datasize % 8) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  if (datasize > 8*NETWOX_DHCP4OPT_STATICROUTE_LEN) {
    /* we could return an error, but it's better to give back only firsts */
    datasize = 8*NETWOX_DHCP4OPT_STATICROUTE_LEN;
  }

  psr->storedvalues = datasize/8;
  for (i = 0; i < psr->storedvalues; i++) {
    i8 = 8 * i;
    psr->dst[i].iptype = NETWIB_IPTYPE_IP4;
    psr->dst[i].ipvalue.ip4 = netwib_c2_uint32_4(data[i8], data[i8+1],
                                                 data[i8+2], data[i8+3]);
    psr->gw[i].iptype = NETWIB_IPTYPE_IP4;
    psr->gw[i].ipvalue.ip4 = netwib_c2_uint32_4(data[i8+4], data[i8+5],
                                                data[i8+6], data[i8+7]);
  }

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_pkt_decode_dhcp4opt_uint32(netwib_constdata data,
                                                    netwib_uint32 datasize,
                                                    netwib_uint32 *pui)
{
  if (datasize != 4) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  *pui = netwib_c2_uint32_4(data[0], data[1], data[2], data[3]);

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_pkt_decode_dhcp4opt_bool(netwib_constdata data,
                                                  netwib_uint32 datasize,
                                                  netwib_bool *pb)
{
  if (datasize != 1) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  *pb = data[0];

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_pkt_decode_dhcp4opt_msgtype(netwib_constdata data,
                                                     netwib_uint32 datasize,
                                                     netwox_dhcp4opt_msgtype *ptype)
{
  if (datasize != 1) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  *ptype = data[0];

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_pkt_decode_dhcp4opt_netbiosnodetype(netwib_constdata data,
                                                             netwib_uint32 datasize,
                                                             netwox_dhcp4opt_netbiosnodetype *ptype)
{
  if (datasize != 1) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  *ptype = data[0];

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_pkt_decode_dhcp4opt_clientid(netwib_data data,
                                                      netwib_uint32 datasize,
                                                      netwox_dhcp4opt_clientid *pci)
{

  if (datasize == 0) {
    pci->type = 0;
    netwib_er(netwib_buf_init_ext_empty(&pci->id));
  } else {
    pci->type = data[0];
    netwib_er(netwib_buf_init_ext_arrayfilled(data+1, datasize-1, &pci->id));
  }

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_pkt_decode_dhcp4opt_reqlist(netwib_constdata data,
                                                     netwib_uint32 datasize,
                                                     netwox_dhcp4opt_reqlist *prl)
{
  netwib_uint32 i;

  if (datasize == 0) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  if (datasize > NETWOX_DHCP4OPT_REQLIST_LEN) {
    /* we could return an error, but it's better to give back only firsts */
    datasize = NETWOX_DHCP4OPT_REQLIST_LEN;
  }

  prl->storedvalues = datasize;
  for (i = 0; i < prl->storedvalues; i++) {
    prl->type[i] = data[i];
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_decode_dhcp4opt(netwib_constbuf *ppkt,
                                      netwox_dhcp4opt *pdhcp4opt,
                                      netwib_uint32 *pskipsize)
{
  netwib_data data;
  netwib_uint32 datasize, skipsize, len;

  datasize = netwib__buf_ref_data_size(ppkt);
  if (datasize == 0) {
    if (pskipsize != NULL) *pskipsize = 0;
    return(NETWIB_ERR_NOTCONVERTED);
  }
  data = netwib__buf_ref_data_ptr(ppkt);

  pdhcp4opt->type = data[0];
  switch(pdhcp4opt->type) {
  case NETWOX_DHCP4OPTTYPE_PAD :
  case NETWOX_DHCP4OPTTYPE_END :
    if (pskipsize != NULL) *pskipsize = 1;
    return(NETWIB_ERR_OK);
    break;
  case NETWOX_DHCP4OPTTYPE_SUBNETMASK :
  case NETWOX_DHCP4OPTTYPE_GATEWAYS :
  case NETWOX_DHCP4OPTTYPE_TIMESERVERS :
  case NETWOX_DHCP4OPTTYPE_NAMESERVERS :
  case NETWOX_DHCP4OPTTYPE_DNSSERVERS :
  case NETWOX_DHCP4OPTTYPE_LOGSERVERS :
  case NETWOX_DHCP4OPTTYPE_LPRSERVERS :
  case NETWOX_DHCP4OPTTYPE_HOSTNAME :
  case NETWOX_DHCP4OPTTYPE_DOMAINNAME :
  case NETWOX_DHCP4OPTTYPE_BROADCASTAD :
  case NETWOX_DHCP4OPTTYPE_PERFORMROUTDISC :
  case NETWOX_DHCP4OPTTYPE_STATICROUTES :
  case NETWOX_DHCP4OPTTYPE_NISDOMAIN :
  case NETWOX_DHCP4OPTTYPE_NISSERVERS :
  case NETWOX_DHCP4OPTTYPE_NTPSERVERS :
  case NETWOX_DHCP4OPTTYPE_VENDORINFO :
  case NETWOX_DHCP4OPTTYPE_NETBIOSNS :
  case NETWOX_DHCP4OPTTYPE_NETBIOSDD :
  case NETWOX_DHCP4OPTTYPE_NETBIOSNODETYPE :
  case NETWOX_DHCP4OPTTYPE_NETBIOSSCOPE :
  case NETWOX_DHCP4OPTTYPE_REQUESTEDIPAD :
  case NETWOX_DHCP4OPTTYPE_IPADLEASETIME :
  case NETWOX_DHCP4OPTTYPE_MSGTYPE :
  case NETWOX_DHCP4OPTTYPE_VENDORID :
  case NETWOX_DHCP4OPTTYPE_SERVERID :
  case NETWOX_DHCP4OPTTYPE_REQLIST :
  case NETWOX_DHCP4OPTTYPE_ERRORMSG :
  case NETWOX_DHCP4OPTTYPE_RENEWALTIME :
  case NETWOX_DHCP4OPTTYPE_REBINDINGTIME :
  case NETWOX_DHCP4OPTTYPE_CLIENTID :
  case NETWOX_DHCP4OPTTYPE_NISPDOMAIN :
  case NETWOX_DHCP4OPTTYPE_NISPSERVERS :
  case NETWOX_DHCP4OPTTYPE_TFTPSERVERS :
  case NETWOX_DHCP4OPTTYPE_SMTPSERVERS :
  case NETWOX_DHCP4OPTTYPE_POP3SERVERS :
  case NETWOX_DHCP4OPTTYPE_NNTPSERVERS :
  case NETWOX_DHCP4OPTTYPE_WWWSERVERS :
  case NETWOX_DHCP4OPTTYPE_IRCSERVERS :
    if (datasize == 1) {
      if (pskipsize != NULL) *pskipsize = 1;
      return(NETWIB_ERR_NOTCONVERTED);
    }
    len = data[1];
    if (len > datasize-2) {
      if (pskipsize != NULL) *pskipsize = datasize;
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (pskipsize != NULL) *pskipsize = 2 + len;
    data += 2;
    datasize = len;
    break;
  default :
    skipsize = datasize;
    if (datasize > 1) {
      skipsize = data[1] + 2;
      if (skipsize > datasize) {
        skipsize = datasize;
      }
    }
    pdhcp4opt->type = NETWOX_DHCP4OPTTYPE_UNKNOWN;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, skipsize,
                                              &pdhcp4opt->opt.unknown));
    if (pskipsize != NULL) *pskipsize = skipsize;
    return(NETWIB_ERR_OK);
    break;
  }

  switch(pdhcp4opt->type) {
  case NETWOX_DHCP4OPTTYPE_SUBNETMASK :
    netwib_er(netwox_pkt_decode_dhcp4opt_ip(data, datasize,
                                            &pdhcp4opt->opt.subnetmask));
    break;
  case NETWOX_DHCP4OPTTYPE_GATEWAYS :
    netwib_er(netwox_pkt_decode_dhcp4opt_iparray(data, datasize,
                                                 &pdhcp4opt->opt.gateways));
    break;
  case NETWOX_DHCP4OPTTYPE_TIMESERVERS :
    netwib_er(netwox_pkt_decode_dhcp4opt_iparray(data, datasize,
                                                 &pdhcp4opt->opt.timeservers));
    break;
  case NETWOX_DHCP4OPTTYPE_NAMESERVERS :
    netwib_er(netwox_pkt_decode_dhcp4opt_iparray(data, datasize,
                                                 &pdhcp4opt->opt.nameservers));
    break;
  case NETWOX_DHCP4OPTTYPE_DNSSERVERS :
    netwib_er(netwox_pkt_decode_dhcp4opt_iparray(data, datasize,
                                               &pdhcp4opt->opt.dnsservers));
    break;
  case NETWOX_DHCP4OPTTYPE_LOGSERVERS :
    netwib_er(netwox_pkt_decode_dhcp4opt_iparray(data, datasize,
                                                 &pdhcp4opt->opt.logservers));
    break;
  case NETWOX_DHCP4OPTTYPE_LPRSERVERS :
    netwib_er(netwox_pkt_decode_dhcp4opt_iparray(data, datasize,
                                                 &pdhcp4opt->opt.lprservers));
    break;
  case NETWOX_DHCP4OPTTYPE_HOSTNAME :
    netwib_er(netwox_pkt_decode_dhcp4opt_bufext(data, datasize,
                                                &pdhcp4opt->opt.hostname));
    break;
  case NETWOX_DHCP4OPTTYPE_DOMAINNAME :
    netwib_er(netwox_pkt_decode_dhcp4opt_bufext(data, datasize,
                                                &pdhcp4opt->opt.domainname));
    break;
  case NETWOX_DHCP4OPTTYPE_BROADCASTAD :
    netwib_er(netwox_pkt_decode_dhcp4opt_ip(data, datasize,
                                            &pdhcp4opt->opt.broadcastad));
    break;
  case NETWOX_DHCP4OPTTYPE_PERFORMROUTDISC :
    netwib_er(netwox_pkt_decode_dhcp4opt_bool(data, datasize, &pdhcp4opt->opt.performroutdisc));
    break;
  case NETWOX_DHCP4OPTTYPE_STATICROUTES :
    netwib_er(netwox_pkt_decode_dhcp4opt_staticroutes(data, datasize, &pdhcp4opt->opt.staticroutes));
    break;
  case NETWOX_DHCP4OPTTYPE_NISDOMAIN :
    netwib_er(netwox_pkt_decode_dhcp4opt_bufext(data, datasize,
                                                &pdhcp4opt->opt.nisdomain));
    break;
  case NETWOX_DHCP4OPTTYPE_NISSERVERS :
    netwib_er(netwox_pkt_decode_dhcp4opt_iparray(data, datasize,
                                                 &pdhcp4opt->opt.nisservers));
    break;
  case NETWOX_DHCP4OPTTYPE_NTPSERVERS :
    netwib_er(netwox_pkt_decode_dhcp4opt_iparray(data, datasize,
                                                 &pdhcp4opt->opt.ntpservers));
    break;
  case NETWOX_DHCP4OPTTYPE_VENDORINFO :
    netwib_er(netwox_pkt_decode_dhcp4opt_bufext(data, datasize,
                                                &pdhcp4opt->opt.vendorinfo));
    break;
  case NETWOX_DHCP4OPTTYPE_NETBIOSNS :
    netwib_er(netwox_pkt_decode_dhcp4opt_iparray(data, datasize,
                                                 &pdhcp4opt->opt.netbiosns));
    break;
  case NETWOX_DHCP4OPTTYPE_NETBIOSDD :
    netwib_er(netwox_pkt_decode_dhcp4opt_iparray(data, datasize,
                                                 &pdhcp4opt->opt.netbiosdd));
    break;
  case NETWOX_DHCP4OPTTYPE_NETBIOSNODETYPE :
    netwib_er(netwox_pkt_decode_dhcp4opt_netbiosnodetype(data, datasize,
                                       &pdhcp4opt->opt.netbiosnodetype));
    break;
  case NETWOX_DHCP4OPTTYPE_NETBIOSSCOPE :
    netwib_er(netwox_pkt_decode_dhcp4opt_bufext(data, datasize,
                                                &pdhcp4opt->opt.netbiosscope));
    break;
  case NETWOX_DHCP4OPTTYPE_REQUESTEDIPAD :
    netwib_er(netwox_pkt_decode_dhcp4opt_ip(data, datasize,
                                            &pdhcp4opt->opt.requestedipad));
    break;
  case NETWOX_DHCP4OPTTYPE_IPADLEASETIME :
    netwib_er(netwox_pkt_decode_dhcp4opt_uint32(data, datasize,
                                              &pdhcp4opt->opt.ipadleasetime));
    break;
  case NETWOX_DHCP4OPTTYPE_MSGTYPE :
    netwib_er(netwox_pkt_decode_dhcp4opt_msgtype(data, datasize,
                                                 &pdhcp4opt->opt.msgtype));
    break;
  case NETWOX_DHCP4OPTTYPE_VENDORID :
    netwib_er(netwox_pkt_decode_dhcp4opt_bufext(data, datasize,
                                                &pdhcp4opt->opt.vendorid));
    break;
  case NETWOX_DHCP4OPTTYPE_SERVERID :
    netwib_er(netwox_pkt_decode_dhcp4opt_ip(data, datasize,
                                            &pdhcp4opt->opt.serverid));
    break;
  case NETWOX_DHCP4OPTTYPE_REQLIST :
    netwib_er(netwox_pkt_decode_dhcp4opt_reqlist(data, datasize,
                                                 &pdhcp4opt->opt.reqlist));
    break;
  case NETWOX_DHCP4OPTTYPE_ERRORMSG :
    netwib_er(netwox_pkt_decode_dhcp4opt_bufext(data, datasize,
                                                &pdhcp4opt->opt.errormsg));
    break;
  case NETWOX_DHCP4OPTTYPE_RENEWALTIME :
    netwib_er(netwox_pkt_decode_dhcp4opt_uint32(data, datasize,
                                              &pdhcp4opt->opt.renewaltime));
    break;
  case NETWOX_DHCP4OPTTYPE_REBINDINGTIME :
    netwib_er(netwox_pkt_decode_dhcp4opt_uint32(data, datasize,
                                              &pdhcp4opt->opt.rebindingtime));
    break;
  case NETWOX_DHCP4OPTTYPE_CLIENTID :
    netwib_er(netwox_pkt_decode_dhcp4opt_clientid(data, datasize,
                                                  &pdhcp4opt->opt.clientid));
    break;
  case NETWOX_DHCP4OPTTYPE_NISPDOMAIN :
    netwib_er(netwox_pkt_decode_dhcp4opt_bufext(data, datasize,
                                                &pdhcp4opt->opt.nispdomain));
    break;
  case NETWOX_DHCP4OPTTYPE_NISPSERVERS :
    netwib_er(netwox_pkt_decode_dhcp4opt_iparray(data, datasize,
                                                 &pdhcp4opt->opt.nispservers));
    break;
  case NETWOX_DHCP4OPTTYPE_TFTPSERVERS :
    netwib_er(netwox_pkt_decode_dhcp4opt_iparray(data, datasize,
                                                 &pdhcp4opt->opt.tftpservers));
    break;
  case NETWOX_DHCP4OPTTYPE_SMTPSERVERS :
    netwib_er(netwox_pkt_decode_dhcp4opt_iparray(data, datasize,
                                                 &pdhcp4opt->opt.smtpservers));
    break;
  case NETWOX_DHCP4OPTTYPE_POP3SERVERS :
    netwib_er(netwox_pkt_decode_dhcp4opt_iparray(data, datasize,
                                                 &pdhcp4opt->opt.pop3servers));
    break;
  case NETWOX_DHCP4OPTTYPE_NNTPSERVERS :
    netwib_er(netwox_pkt_decode_dhcp4opt_iparray(data, datasize,
                                                 &pdhcp4opt->opt.nntpservers));
    break;
  case NETWOX_DHCP4OPTTYPE_WWWSERVERS :
    netwib_er(netwox_pkt_decode_dhcp4opt_iparray(data, datasize,
                                                 &pdhcp4opt->opt.wwwservers));
    break;
  case NETWOX_DHCP4OPTTYPE_IRCSERVERS :
    netwib_er(netwox_pkt_decode_dhcp4opt_iparray(data, datasize,
                                                 &pdhcp4opt->opt.ircservers));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_dhcp4opt_show_ip(netwib_conststring txt,
                                          netwib_constip *pip,
                                          netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_fmt32(pbuf, " %s: %{ip}", txt, pip));

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_dhcp4opt_show_iparray(netwib_conststring txt,
                                               const netwox_dhcp4opt_iparray *pia,
                                               netwib_buf *pbuf)
{
  netwib_uint32 i;

  if (pia->storedvalues > NETWOX_DHCP4OPT_IPARRAY_LEN) {
    return(NETWIB_ERR_PATOOHIGH);
  }

  for (i = 0; i < pia->storedvalues; i++) {
    netwib_er(netwib_show_array_fmt32(pbuf, " %s[%{uint32}]: %{ip}", txt,
                                      i, &pia->ip[i]));
  }

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_dhcp4opt_show_bufext(netwib_conststring txt,
                                              netwib_constbuf *pbufext,
                                              netwib_buf *pbuf)
{
  netwib_char array[80];

  if (netwib_c_strlen(txt) > sizeof(array)-2) {
    return(NETWOX_ERR_INTERNALERROR);
  }
  netwib_c_strcpy(array, " ");
  netwib_c_strcat(array, txt);

  netwib_er(netwib_show_array_data(array, pbufext, NETWIB_ENCODETYPE_MIXED,
                                   ' ', pbuf));

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_dhcp4opt_show_staticroutes(netwib_conststring txt,
                                                    const netwox_dhcp4opt_staticroutes *psr,
                                                    netwib_buf *pbuf)
{
  netwib_uint32 i;

  if (psr->storedvalues > NETWOX_DHCP4OPT_STATICROUTE_LEN) {
    return(NETWIB_ERR_PATOOHIGH);
  }

  for (i = 0; i < psr->storedvalues; i++) {
    netwib_er(netwib_show_array_fmt32(pbuf,
                                      " %s[%{uint32}]: dst=%{ip} gw=%{ip}",
                                      txt, i, &psr->dst[i], &psr->gw[i]));
  }

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_dhcp4opt_show_uint32(netwib_conststring txt,
                                              netwib_uint32 ui,
                                              netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_fmt32(pbuf, " %s: %{uint32}", txt, ui));

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_dhcp4opt_show_bool(netwib_conststring txt,
                                            netwib_bool b,
                                            netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_fmt32(pbuf, " %s: %{bool}", txt, b));

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_dhcp4opt_show_msgtype(netwib_conststring txt,
                                               netwox_dhcp4opt_msgtype type,
                                               netwib_buf *pbuf)
{
  netwib_conststring pc=NULL;

  switch(type) {
  case NETWOX_DHCP4OPT_MSGTYPE_DISCOVER : pc = "discover"; break;
  case NETWOX_DHCP4OPT_MSGTYPE_OFFER : pc = "offer"; break;
  case NETWOX_DHCP4OPT_MSGTYPE_REQUEST : pc = "request"; break;
  case NETWOX_DHCP4OPT_MSGTYPE_DECLINE : pc = "decline"; break;
  case NETWOX_DHCP4OPT_MSGTYPE_ACK : pc = "ack"; break;
  case NETWOX_DHCP4OPT_MSGTYPE_NAK : pc = "nak"; break;
  case NETWOX_DHCP4OPT_MSGTYPE_RELEASE : pc = "release"; break;
  case NETWOX_DHCP4OPT_MSGTYPE_INFORM : pc = "inform"; break;
  default : pc = "unknown"; break;
  }

  netwib_er(netwib_show_array_fmt32(pbuf, " %s: %s", txt, pc));

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_dhcp4opt_show_netbiosnodetype(netwib_conststring txt,
                                         netwox_dhcp4opt_netbiosnodetype type,
                                                       netwib_buf *pbuf)
{
  netwib_conststring pc=NULL;

  switch(type) {
  case NETWOX_DHCP4OPT_NETBIOSNODETYPE_B : pc = "B-node"; break;
  case NETWOX_DHCP4OPT_NETBIOSNODETYPE_P : pc = "P-node"; break;
  case NETWOX_DHCP4OPT_NETBIOSNODETYPE_M : pc = "M-node"; break;
  case NETWOX_DHCP4OPT_NETBIOSNODETYPE_H : pc = "H-node"; break;
  default : pc = "unknown"; break;
  }

  netwib_er(netwib_show_array_fmt32(pbuf, " %s: %s", txt, pc));

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_dhcp4opt_show_clientid(netwib_conststring txt,
                                          const netwox_dhcp4opt_clientid *pci,
                                                netwib_buf *pbuf)
{
  netwib_char array[80];

  if (netwib_c_strlen(txt) > sizeof(array)-2) {
    return(NETWOX_ERR_INTERNALERROR);
  }
  netwib_c_strcpy(array, " ");
  netwib_c_strcat(array, txt);

  netwib_er(netwib_show_array_fmt32(pbuf, "%stype: %{uint8}", array,
                                    pci->type));
  netwib_er(netwib_show_array_data(array, &pci->id, NETWIB_ENCODETYPE_HEXA0,
                                   ' ', pbuf));

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_dhcp4opt_show_reqlist(netwib_conststring txt,
                                               const netwox_dhcp4opt_reqlist *prl,
                                               netwib_buf *pbuf)
{
  netwib_conststring pc;
  netwib_uint32 i;

  if (prl->storedvalues > NETWOX_DHCP4OPT_REQLIST_LEN) {
    return(NETWIB_ERR_PATOOHIGH);
  }

  for (i = 0; i < prl->storedvalues; i++) {
    netwib_er(netwox_dhcp4opt_decode_string(prl->type[i], &pc));
    netwib_er(netwib_show_array_fmt32(pbuf, " %s[%{uint32}]: %{uint32} (%s)",
                                      txt, i, prl->type[i], pc));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dhcp4opt_show(netwox_constdhcp4opt *pdhcp4opt,
                                netwib_encodetype encodetype,
                                netwib_buf *pbuf)
{
  netwib_conststring name;

  if (encodetype != NETWIB_ENCODETYPE_ARRAY) {
    return(NETWOX_ERR_NOTIMPLEMENTED);
  }

  netwib_er(netwox_dhcp4opt_decode_string(pdhcp4opt->type, &name));

  switch(pdhcp4opt->type) {
  case NETWOX_DHCP4OPTTYPE_UNKNOWN :
    netwib_er(netwib_show_array_data(" unknown", &pdhcp4opt->opt.unknown,
                                     NETWIB_ENCODETYPE_HEXA0, ' ', pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_PAD :
    netwib_er(netwib_show_array_fmt32(pbuf, " padding"));
    break;
  case NETWOX_DHCP4OPTTYPE_END :
    netwib_er(netwib_show_array_fmt32(pbuf, " end"));
    break;
  case NETWOX_DHCP4OPTTYPE_SUBNETMASK :
    netwib_er(netwox_dhcp4opt_show_ip(name, &pdhcp4opt->opt.subnetmask, pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_GATEWAYS :
    netwib_er(netwox_dhcp4opt_show_iparray(name, &pdhcp4opt->opt.gateways,
                                           pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_TIMESERVERS :
    netwib_er(netwox_dhcp4opt_show_iparray(name, &pdhcp4opt->opt.timeservers,
                                           pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_NAMESERVERS :
    netwib_er(netwox_dhcp4opt_show_iparray(name, &pdhcp4opt->opt.nameservers,
                                           pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_DNSSERVERS :
    netwib_er(netwox_dhcp4opt_show_iparray(name, &pdhcp4opt->opt.dnsservers,
                                           pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_LOGSERVERS :
    netwib_er(netwox_dhcp4opt_show_iparray(name, &pdhcp4opt->opt.logservers,
                                           pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_LPRSERVERS :
    netwib_er(netwox_dhcp4opt_show_iparray(name, &pdhcp4opt->opt.lprservers,
                                           pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_HOSTNAME :
    netwib_er(netwox_dhcp4opt_show_bufext(name, &pdhcp4opt->opt.hostname,
                                          pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_DOMAINNAME :
    netwib_er(netwox_dhcp4opt_show_bufext(name, &pdhcp4opt->opt.domainname,
                                          pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_BROADCASTAD :
    netwib_er(netwox_dhcp4opt_show_ip(name, &pdhcp4opt->opt.broadcastad,
                                      pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_PERFORMROUTDISC :
    netwib_er(netwox_dhcp4opt_show_bool(name, pdhcp4opt->opt.performroutdisc,
                                        pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_STATICROUTES :
    netwib_er(netwox_dhcp4opt_show_staticroutes(name,
                                                &pdhcp4opt->opt.staticroutes,
                                                pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_NISDOMAIN :
    netwib_er(netwox_dhcp4opt_show_bufext(name, &pdhcp4opt->opt.nisdomain,
                                          pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_NISSERVERS :
    netwib_er(netwox_dhcp4opt_show_iparray(name, &pdhcp4opt->opt.nisservers,
                                           pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_NTPSERVERS :
    netwib_er(netwox_dhcp4opt_show_iparray(name, &pdhcp4opt->opt.ntpservers,
                                           pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_VENDORINFO :
    netwib_er(netwox_dhcp4opt_show_bufext(name, &pdhcp4opt->opt.vendorinfo,
                                          pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_NETBIOSNS :
    netwib_er(netwox_dhcp4opt_show_iparray(name, &pdhcp4opt->opt.netbiosns,
                                           pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_NETBIOSDD :
    netwib_er(netwox_dhcp4opt_show_iparray(name, &pdhcp4opt->opt.netbiosdd,
                                           pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_NETBIOSNODETYPE :
    netwib_er(netwox_dhcp4opt_show_netbiosnodetype(name,
                                      pdhcp4opt->opt.netbiosnodetype, pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_NETBIOSSCOPE :
    netwib_er(netwox_dhcp4opt_show_bufext(name, &pdhcp4opt->opt.netbiosscope,
                                          pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_REQUESTEDIPAD :
    netwib_er(netwox_dhcp4opt_show_ip(name, &pdhcp4opt->opt.requestedipad,
                                      pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_IPADLEASETIME :
    netwib_er(netwox_dhcp4opt_show_uint32(name, pdhcp4opt->opt.ipadleasetime,
                                          pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_MSGTYPE :
    netwib_er(netwox_dhcp4opt_show_msgtype(name, pdhcp4opt->opt.msgtype,
                                           pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_SERVERID :
    netwib_er(netwox_dhcp4opt_show_ip(name, &pdhcp4opt->opt.serverid,
                                      pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_REQLIST :
    netwib_er(netwox_dhcp4opt_show_reqlist(name, &pdhcp4opt->opt.reqlist,
                                           pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_ERRORMSG :
    netwib_er(netwox_dhcp4opt_show_bufext(name, &pdhcp4opt->opt.errormsg,
                                          pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_RENEWALTIME :
    netwib_er(netwox_dhcp4opt_show_uint32(name, pdhcp4opt->opt.renewaltime,
                                          pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_REBINDINGTIME :
    netwib_er(netwox_dhcp4opt_show_uint32(name, pdhcp4opt->opt.rebindingtime,
                                          pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_VENDORID :
    netwib_er(netwox_dhcp4opt_show_bufext(name, &pdhcp4opt->opt.vendorid,
                                          pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_CLIENTID :
    netwib_er(netwox_dhcp4opt_show_clientid(name, &pdhcp4opt->opt.clientid,
                                            pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_NISPDOMAIN :
    netwib_er(netwox_dhcp4opt_show_bufext(name, &pdhcp4opt->opt.nispdomain,
                                          pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_NISPSERVERS :
    netwib_er(netwox_dhcp4opt_show_iparray(name, &pdhcp4opt->opt.nispservers,
                                           pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_TFTPSERVERS :
    netwib_er(netwox_dhcp4opt_show_iparray(name, &pdhcp4opt->opt.tftpservers,
                                           pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_SMTPSERVERS :
    netwib_er(netwox_dhcp4opt_show_iparray(name, &pdhcp4opt->opt.smtpservers,
                                           pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_POP3SERVERS :
    netwib_er(netwox_dhcp4opt_show_iparray(name, &pdhcp4opt->opt.pop3servers,
                                           pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_NNTPSERVERS :
    netwib_er(netwox_dhcp4opt_show_iparray(name, &pdhcp4opt->opt.nntpservers,
                                           pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_WWWSERVERS :
    netwib_er(netwox_dhcp4opt_show_iparray(name, &pdhcp4opt->opt.wwwservers,
                                           pbuf));
    break;
  case NETWOX_DHCP4OPTTYPE_IRCSERVERS :
    netwib_er(netwox_dhcp4opt_show_iparray(name, &pdhcp4opt->opt.ircservers,
                                           pbuf));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dhcp4opt_reqlist_initall(netwox_dhcp4opt *pdhcp4opt)
{

  netwib_er(netwox_dhcp4opt_initdefault(NETWOX_DHCP4OPTTYPE_REQLIST,
                                        pdhcp4opt));

  pdhcp4opt->opt.reqlist.type[pdhcp4opt->opt.reqlist.storedvalues++] =
    NETWOX_DHCP4OPTTYPE_SUBNETMASK;
  pdhcp4opt->opt.reqlist.type[pdhcp4opt->opt.reqlist.storedvalues++] =
    NETWOX_DHCP4OPTTYPE_GATEWAYS;
  pdhcp4opt->opt.reqlist.type[pdhcp4opt->opt.reqlist.storedvalues++] =
    NETWOX_DHCP4OPTTYPE_TIMESERVERS;
  pdhcp4opt->opt.reqlist.type[pdhcp4opt->opt.reqlist.storedvalues++] =
    NETWOX_DHCP4OPTTYPE_NAMESERVERS;
  pdhcp4opt->opt.reqlist.type[pdhcp4opt->opt.reqlist.storedvalues++] =
    NETWOX_DHCP4OPTTYPE_DNSSERVERS;
  pdhcp4opt->opt.reqlist.type[pdhcp4opt->opt.reqlist.storedvalues++] =
    NETWOX_DHCP4OPTTYPE_LOGSERVERS;
  pdhcp4opt->opt.reqlist.type[pdhcp4opt->opt.reqlist.storedvalues++] =
    NETWOX_DHCP4OPTTYPE_LPRSERVERS;
  pdhcp4opt->opt.reqlist.type[pdhcp4opt->opt.reqlist.storedvalues++] =
    NETWOX_DHCP4OPTTYPE_HOSTNAME;
  pdhcp4opt->opt.reqlist.type[pdhcp4opt->opt.reqlist.storedvalues++] =
    NETWOX_DHCP4OPTTYPE_DOMAINNAME;
  pdhcp4opt->opt.reqlist.type[pdhcp4opt->opt.reqlist.storedvalues++] =
    NETWOX_DHCP4OPTTYPE_BROADCASTAD;
  pdhcp4opt->opt.reqlist.type[pdhcp4opt->opt.reqlist.storedvalues++] =
    NETWOX_DHCP4OPTTYPE_PERFORMROUTDISC;
  pdhcp4opt->opt.reqlist.type[pdhcp4opt->opt.reqlist.storedvalues++] =
    NETWOX_DHCP4OPTTYPE_STATICROUTES;
  pdhcp4opt->opt.reqlist.type[pdhcp4opt->opt.reqlist.storedvalues++] =
    NETWOX_DHCP4OPTTYPE_NISDOMAIN;
  pdhcp4opt->opt.reqlist.type[pdhcp4opt->opt.reqlist.storedvalues++] =
    NETWOX_DHCP4OPTTYPE_NISSERVERS;
  pdhcp4opt->opt.reqlist.type[pdhcp4opt->opt.reqlist.storedvalues++] =
    NETWOX_DHCP4OPTTYPE_IPADLEASETIME;
  pdhcp4opt->opt.reqlist.type[pdhcp4opt->opt.reqlist.storedvalues++] =
    NETWOX_DHCP4OPTTYPE_RENEWALTIME;
  pdhcp4opt->opt.reqlist.type[pdhcp4opt->opt.reqlist.storedvalues++] =
    NETWOX_DHCP4OPTTYPE_REBINDINGTIME;
  pdhcp4opt->opt.reqlist.type[pdhcp4opt->opt.reqlist.storedvalues++] =
    NETWOX_DHCP4OPTTYPE_NISPDOMAIN;
  pdhcp4opt->opt.reqlist.type[pdhcp4opt->opt.reqlist.storedvalues++] =
    NETWOX_DHCP4OPTTYPE_NISPSERVERS;
  pdhcp4opt->opt.reqlist.type[pdhcp4opt->opt.reqlist.storedvalues++] =
    NETWOX_DHCP4OPTTYPE_SMTPSERVERS;
  pdhcp4opt->opt.reqlist.type[pdhcp4opt->opt.reqlist.storedvalues++] =
    NETWOX_DHCP4OPTTYPE_POP3SERVERS;
  pdhcp4opt->opt.reqlist.type[pdhcp4opt->opt.reqlist.storedvalues++] =
    NETWOX_DHCP4OPTTYPE_NNTPSERVERS;
  pdhcp4opt->opt.reqlist.type[pdhcp4opt->opt.reqlist.storedvalues++] =
    NETWOX_DHCP4OPTTYPE_WWWSERVERS;
  pdhcp4opt->opt.reqlist.type[pdhcp4opt->opt.reqlist.storedvalues++] =
    NETWOX_DHCP4OPTTYPE_IRCSERVERS;

  return(NETWIB_ERR_OK);
}

