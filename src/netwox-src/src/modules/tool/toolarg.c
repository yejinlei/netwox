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
netwib_err netwox_buf_append_toolargtype(netwox_toolargtype toolargtype,
                                         netwib_buf *pbuf)
{
  netwib_conststring pc = NULL;

  switch (toolargtype) {
    case NETWOX_TOOLARGTYPE_BOOL :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO1 :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO2 :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO3 :
      pc = "bool";
      break;
    case NETWOX_TOOLARGTYPE_IP :
    case NETWOX_TOOLARGTYPE_IP4 :
    case NETWOX_TOOLARGTYPE_IP6 :
    case NETWOX_TOOLARGTYPE_IP_SRC :
    case NETWOX_TOOLARGTYPE_IP4_SRC :
    case NETWOX_TOOLARGTYPE_IP6_SRC :
    case NETWOX_TOOLARGTYPE_IP_DST :
    case NETWOX_TOOLARGTYPE_IP4_DST :
    case NETWOX_TOOLARGTYPE_IP6_DST :
      pc = "ip";
      break;
    case NETWOX_TOOLARGTYPE_ETH :
    case NETWOX_TOOLARGTYPE_ETH_SRC :
    case NETWOX_TOOLARGTYPE_ETH_DST :
      pc = "eth";
      break;
    case NETWOX_TOOLARGTYPE_PORT :
    case NETWOX_TOOLARGTYPE_PORT_SRC :
    case NETWOX_TOOLARGTYPE_PORT_DST :
      pc = "port";
      break;
    case NETWOX_TOOLARGTYPE_IPS_SU :
    case NETWOX_TOOLARGTYPE_IPS_U :
    case NETWOX_TOOLARGTYPE_IPS_N :
      pc = "ips";
      break;
    case NETWOX_TOOLARGTYPE_ETHS_SU :
    case NETWOX_TOOLARGTYPE_ETHS_U :
    case NETWOX_TOOLARGTYPE_ETHS_N :
      pc = "eths";
      break;
    case NETWOX_TOOLARGTYPE_PORTS_SU :
    case NETWOX_TOOLARGTYPE_PORTS_U :
    case NETWOX_TOOLARGTYPE_PORTS_N :
      pc = "ports";
      break;
    case NETWOX_TOOLARGTYPE_UINT32 :
      pc = "uint32";
      break;
    case NETWOX_TOOLARGTYPE_INT32 :
      pc = "int32";
      break;
    case NETWOX_TOOLARGTYPE_UINT64 :
      pc = "uint64";
      break;
    case NETWOX_TOOLARGTYPE_INT64 :
      pc = "int64";
      break;
    case NETWOX_TOOLARGTYPE_BUF :
      pc = "data";
      break;
    case NETWOX_TOOLARGTYPE_BUF_LOGIN :
      pc = "login";
      break;
    case NETWOX_TOOLARGTYPE_BUF_PASSWORD :
      pc = "password";
      break;
    case NETWOX_TOOLARGTYPE_BUF_DEVICE :
      pc = "device";
      break;
    case NETWOX_TOOLARGTYPE_BUF_FILE :
      pc = "file";
      break;
    case NETWOX_TOOLARGTYPE_BUF_FILE_RD :
      pc = "file";
      break;
    case NETWOX_TOOLARGTYPE_BUF_FILE_WR :
      pc = "file";
      break;
    case NETWOX_TOOLARGTYPE_BUF_DIR :
      pc = "dir";
      break;
    case NETWOX_TOOLARGTYPE_BUF_DIR_RD :
      pc = "dir";
      break;
    case NETWOX_TOOLARGTYPE_BUF_DIR_WR :
      pc = "dir";
      break;
    case NETWOX_TOOLARGTYPE_BUF_FILTER :
      pc = "filter";
      break;
    case NETWOX_TOOLARGTYPE_BUF_OID :
      pc = "oid";
      break;
    case NETWOX_TOOLARGTYPE_BUF_OIDTYPE :
      pc = "oidtype";
      break;
    case NETWOX_TOOLARGTYPE_BUF_MIXED :
      pc = "mixed_data";
      break;
    case NETWOX_TOOLARGTYPE_BUF_IP4OPTS :
      pc = "ip4opts";
      break;
    case NETWOX_TOOLARGTYPE_BUF_IP6EXTS :
      pc = "ip6exts";
      break;
    case NETWOX_TOOLARGTYPE_BUF_TCPOPTS :
      pc = "tcpopts";
      break;
    case NETWOX_TOOLARGTYPE_BUF_EMAIL :
      pc = "email";
      break;
    case NETWOX_TOOLARGTYPE_BUF_URI :
      pc = "uri";
      break;
    case NETWOX_TOOLARGTYPE_BUF_NEWSGROUP :
      pc = "newsgroup";
      break;
    case NETWOX_TOOLARGTYPE_BUF_HOSTNAME :
      pc = "hostname";
      break;
    case NETWOX_TOOLARGTYPE_BUF_MD5 :
      pc = "md5";
      break;
    case NETWOX_TOOLARGTYPE_BUF_USERAGENT :
      pc = "useragent";
      break;
    case NETWOX_TOOLARGTYPE_BUF_NETBIOSNAME :
      pc = "netbiosname";
      break;
    case NETWOX_TOOLARGTYPE_BUF_SMBSHARE :
      pc = "smbshare";
      break;
    case NETWOX_TOOLARGTYPE_ENCODE :
      pc = "encode";
      break;
    case NETWOX_TOOLARGTYPE_RECORDENCODE :
      pc = "recordencode";
      break;
    case NETWOX_TOOLARGTYPE_DECODE :
      pc = "decode";
      break;
    case NETWOX_TOOLARGTYPE_DLT :
      pc = "dlt";
      break;
    case NETWOX_TOOLARGTYPE_SPOOFIP :
      pc = "spoofip";
      break;
    case NETWOX_TOOLARGTYPE_IPTYPE :
      pc = "iptype";
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  netwib_er(netwib_buf_append_string(pc, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_buf_toolargtype_help(void)
{

  netwib_er(netwib_fmt_display("Netwox supports different kinds of options.\n"));
  netwib_er(netwib_fmt_display("They are all described here :\n"));

  netwib_er(netwib_fmt_display("bool : boolean\n"));
  netwib_er(netwib_fmt_display("  0 : false\n"));
  netwib_er(netwib_fmt_display("  1 : true\n"));
  netwib_er(netwib_fmt_display("ip : IP address (IPv4 or IPv6)\n"));
  netwib_er(netwib_fmt_display("  1.2.3.4\n"));
  netwib_er(netwib_fmt_display("  ::1\n"));
  netwib_er(netwib_fmt_display("eth : Ethernet address\n"));
  netwib_er(netwib_fmt_display("  0:2:3:4:5:6\n"));
  netwib_er(netwib_fmt_display("port : port number\n"));
  netwib_er(netwib_fmt_display("  123\n"));
  netwib_er(netwib_fmt_display("ips : IP addresses\n"));
  netwib_er(netwib_fmt_display("  1.2.3.4\n"));
  netwib_er(netwib_fmt_display("  1.2.3.4-1.2.3.6\n"));
  netwib_er(netwib_fmt_display("  1.2.3.4-1.2.3.6,3.4.5.6\n"));
  netwib_er(netwib_fmt_display("eths : Ethernet addresses\n"));
  netwib_er(netwib_fmt_display("  0:2:3:4:5:6-0:2:3:4:5:9\n"));
  netwib_er(netwib_fmt_display("ports : port numbers\n"));
  netwib_er(netwib_fmt_display("  123-456\n"));
  netwib_er(netwib_fmt_display("  123-456,678\n"));
  netwib_er(netwib_fmt_display("uint32/uint64 : unsigned integer\n"));
  netwib_er(netwib_fmt_display("  123\n"));
  netwib_er(netwib_fmt_display("int32/int64 : signed integer\n"));
  netwib_er(netwib_fmt_display("  -123\n"));
  netwib_er(netwib_fmt_display("data : arbitrary data\n"));
  netwib_er(netwib_fmt_display("  hello\n"));
  netwib_er(netwib_fmt_display("device : a valid device on the system\n"));
  netwib_er(netwib_fmt_display("  eth0\n"));
  netwib_er(netwib_fmt_display("file : filename\n"));
  netwib_er(netwib_fmt_display("  file.txt\n"));
  netwib_er(netwib_fmt_display("dir : directory\n"));
  netwib_er(netwib_fmt_display("  /tmp or c:/windows\n"));
  netwib_er(netwib_fmt_display("filter : a pcap filter\n"));
  netwib_er(netwib_fmt_display("  \"host 1.2.3.4\"\n"));
  netwib_er(netwib_fmt_display("encode : an encoding type\n"));
  netwib_er(netwib_fmt_display("  data, hexa, mixed, text, base64, nothing, synth, data_wrap, hexa_wrap\n  mixed_wrap, text_wrap, base64_wrap, array, dump, lowercase, uppercase, mixedh_wrap\n"));
  netwib_er(netwib_fmt_display("recordencode : a record encoding type\n"));
  netwib_er(netwib_fmt_display("  hexa, mixed, hexa_wrap, mixed_wrap, dump, mixedh_wrap, bin, pcap\n"));
  netwib_er(netwib_fmt_display("decode : a decoding type\n"));
  netwib_er(netwib_fmt_display("  data, hexa, mixed, base64\n"));
  netwib_er(netwib_fmt_display("dlt : device dlt type\n"));
  netwib_er(netwib_fmt_display("  null, ether, em3mb, ax25, pronet, chaos, ieee802, arcnet, slip, ppp\n  fddi, atm-rfc1483, raw, slip-bsdos, ppp-bsdos, atm-clip, ppp-serial,\n  ppp-ether, c-hdlc, ieee802-11, loop, linux-sll, ltalk, econet,\n  prism-header, aironet-header\n"));
  netwib_er(netwib_fmt_display("spoofip : spoof_ip_inittype\n"));
  netwib_er(netwib_fmt_display("  raw linkf linkb linkfb rawlinkf rawlinkb rawlinkfb linkfraw linkbraw linkfbraw link rawlink linkraw best\n"));
  netwib_er(netwib_fmt_display("sample : a packet sample number\n"));
  netwib_er(netwib_fmt_display("  1 : NETWOX_SAMPLE_IP4UDPSYSLOG\n"));
  netwib_er(netwib_fmt_display("  2 : NETWOX_SAMPLE_IP4TCPSYN\n"));
  netwib_er(netwib_fmt_display("  3 : NETWOX_SAMPLE_IP4TCPSYNACK\n"));
  netwib_er(netwib_fmt_display("  4 : NETWOX_SAMPLE_IP4TCPACK\n"));
  netwib_er(netwib_fmt_display("  5 : NETWOX_SAMPLE_IP4ICMP4PING\n"));
  netwib_er(netwib_fmt_display("iptype : ip type\n"));
  netwib_er(netwib_fmt_display("  ip4, ip6\n"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_toolarg_get_name(const netwox_toolarg *ptoolarg,
                                   netwib_buf *pbuf)
{
  netwib_conststring pc = NULL;

  /* user specified value */
  if (ptoolarg->name != NULL) {
    return(netwib_buf_append_string(ptoolarg->name, pbuf));
  }

  /* now, deduct */
  switch (ptoolarg->toolargtype) {
    case NETWOX_TOOLARGTYPE_BOOL :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO1 :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO2 :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO3 :
      netwib_er(netwib_buf_append_byte(ptoolarg->key, pbuf));
      break;
    case NETWOX_TOOLARGTYPE_IP :
    case NETWOX_TOOLARGTYPE_IP4 :
    case NETWOX_TOOLARGTYPE_IP6 :
      pc = "ip";
      break;
    case NETWOX_TOOLARGTYPE_IP_SRC :
    case NETWOX_TOOLARGTYPE_IP4_SRC :
    case NETWOX_TOOLARGTYPE_IP6_SRC :
      pc = "src-ip";
      break;
    case NETWOX_TOOLARGTYPE_IP_DST :
    case NETWOX_TOOLARGTYPE_IP4_DST :
    case NETWOX_TOOLARGTYPE_IP6_DST :
      pc = "dst-ip";
      break;
    case NETWOX_TOOLARGTYPE_ETH :
      pc = "eth";
      break;
    case NETWOX_TOOLARGTYPE_ETH_SRC :
      pc = "src-eth";
      break;
    case NETWOX_TOOLARGTYPE_ETH_DST :
      pc = "dst-eth";
      break;
    case NETWOX_TOOLARGTYPE_PORT :
      pc = "port";
      break;
    case NETWOX_TOOLARGTYPE_PORT_SRC :
      pc = "src-port";
      break;
    case NETWOX_TOOLARGTYPE_PORT_DST :
      pc = "dst-port";
      break;
    case NETWOX_TOOLARGTYPE_IPS_SU :
    case NETWOX_TOOLARGTYPE_IPS_U :
    case NETWOX_TOOLARGTYPE_IPS_N :
      pc = "ips";
      break;
    case NETWOX_TOOLARGTYPE_ETHS_SU :
    case NETWOX_TOOLARGTYPE_ETHS_U :
    case NETWOX_TOOLARGTYPE_ETHS_N :
      pc = "eths";
      break;
    case NETWOX_TOOLARGTYPE_PORTS_SU :
    case NETWOX_TOOLARGTYPE_PORTS_U :
    case NETWOX_TOOLARGTYPE_PORTS_N :
      pc = "ports";
      break;
    case NETWOX_TOOLARGTYPE_UINT32 :
    case NETWOX_TOOLARGTYPE_UINT64 :
      pc = "uint";
      break;
    case NETWOX_TOOLARGTYPE_INT32 :
    case NETWOX_TOOLARGTYPE_INT64 :
      pc = "int";
      break;
    case NETWOX_TOOLARGTYPE_BUF :
      pc = "data";
      break;
    case NETWOX_TOOLARGTYPE_BUF_LOGIN :
      pc = "login";
      break;
    case NETWOX_TOOLARGTYPE_BUF_PASSWORD :
      pc = "password";
      break;
    case NETWOX_TOOLARGTYPE_BUF_DEVICE :
      pc = "device";
      break;
    case NETWOX_TOOLARGTYPE_BUF_FILE :
      pc = "file";
      break;
    case NETWOX_TOOLARGTYPE_BUF_FILE_RD :
      pc = "src-file";
      break;
    case NETWOX_TOOLARGTYPE_BUF_FILE_WR :
      pc = "dst-file";
      break;
    case NETWOX_TOOLARGTYPE_BUF_DIR :
      pc = "dir";
      break;
    case NETWOX_TOOLARGTYPE_BUF_DIR_RD :
      pc = "src-dir";
      break;
    case NETWOX_TOOLARGTYPE_BUF_DIR_WR :
      pc = "dst-dir";
      break;
    case NETWOX_TOOLARGTYPE_BUF_FILTER :
      pc = "filter";
      break;
    case NETWOX_TOOLARGTYPE_BUF_OID :
      pc = "oid";
      break;
    case NETWOX_TOOLARGTYPE_BUF_OIDTYPE :
      pc = "oidtype";
      break;
    case NETWOX_TOOLARGTYPE_BUF_MIXED :
      pc = "mixed";
      break;
    case NETWOX_TOOLARGTYPE_BUF_IP4OPTS :
      pc = "ip4opts";
      break;
    case NETWOX_TOOLARGTYPE_BUF_IP6EXTS :
      pc = "ip6exts";
      break;
    case NETWOX_TOOLARGTYPE_BUF_TCPOPTS :
      pc = "tcpopts";
      break;
    case NETWOX_TOOLARGTYPE_BUF_EMAIL :
      pc = "email";
      break;
    case NETWOX_TOOLARGTYPE_BUF_URI :
      pc = "uri";
      break;
    case NETWOX_TOOLARGTYPE_BUF_NEWSGROUP :
      pc = "newsgroup";
      break;
    case NETWOX_TOOLARGTYPE_BUF_HOSTNAME :
      pc = "hostname";
      break;
    case NETWOX_TOOLARGTYPE_BUF_MD5 :
      pc = "md5";
      break;
    case NETWOX_TOOLARGTYPE_BUF_USERAGENT :
      pc = "useragent";
      break;
    case NETWOX_TOOLARGTYPE_BUF_NETBIOSNAME :
      pc = "netbiosname";
      break;
    case NETWOX_TOOLARGTYPE_BUF_SMBSHARE :
      pc = "share";
      break;
    case NETWOX_TOOLARGTYPE_ENCODE :
      pc = "encode";
      break;
    case NETWOX_TOOLARGTYPE_RECORDENCODE :
      pc = "recordencode";
      break;
    case NETWOX_TOOLARGTYPE_DECODE :
      pc = "decode";
      break;
    case NETWOX_TOOLARGTYPE_DLT :
      pc = "dlt";
      break;
    case NETWOX_TOOLARGTYPE_SPOOFIP :
      pc = "spoofip";
      break;
    case NETWOX_TOOLARGTYPE_IPTYPE :
      pc = "iptype";
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  netwib_er(netwib_buf_append_string(pc, pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_toolarg_get_description(const netwox_toolarg *ptoolarg,
                                          netwib_buf *pbuf)
{
  netwib_conststring pc = NULL;

  /* user specified value */
  if (ptoolarg->description != NULL) {
    return(netwib_buf_append_string(ptoolarg->description, pbuf));
  }

  /* now, deduct */
  switch (ptoolarg->toolargtype) {
    case NETWOX_TOOLARGTYPE_BOOL :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO1 :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO2 :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO3 :
      pc = "boolean";
      break;
    case NETWOX_TOOLARGTYPE_IP :
      pc = "IP address";
      break;
    case NETWOX_TOOLARGTYPE_IP_SRC :
      pc = "source IP address";
      break;
    case NETWOX_TOOLARGTYPE_IP_DST :
      pc = "destination IP address";
      break;
    case NETWOX_TOOLARGTYPE_IP4 :
      pc = "IPv4 address";
      break;
    case NETWOX_TOOLARGTYPE_IP4_SRC :
      pc = "source IPv4 address";
      break;
    case NETWOX_TOOLARGTYPE_IP4_DST :
      pc = "destination IPv4 address";
      break;
    case NETWOX_TOOLARGTYPE_IP6 :
      pc = "IPv6 address";
      break;
    case NETWOX_TOOLARGTYPE_IP6_SRC :
      pc = "source IPv6 address";
      break;
    case NETWOX_TOOLARGTYPE_IP6_DST :
      pc = "destination IPv6 address";
      break;
    case NETWOX_TOOLARGTYPE_ETH :
      pc = "ethernet address";
      break;
    case NETWOX_TOOLARGTYPE_ETH_SRC :
      pc = "source ethernet address";
      break;
    case NETWOX_TOOLARGTYPE_ETH_DST :
      pc = "destination ethernet address";
      break;
    case NETWOX_TOOLARGTYPE_PORT :
      pc = "port number";
      break;
    case NETWOX_TOOLARGTYPE_PORT_SRC :
      pc = "source port number";
      break;
    case NETWOX_TOOLARGTYPE_PORT_DST :
      pc = "destination port number";
      break;
    case NETWOX_TOOLARGTYPE_IPS_SU :
    case NETWOX_TOOLARGTYPE_IPS_U :
    case NETWOX_TOOLARGTYPE_IPS_N :
      pc = "list/range of IP addresses";
      break;
    case NETWOX_TOOLARGTYPE_ETHS_SU :
    case NETWOX_TOOLARGTYPE_ETHS_U :
    case NETWOX_TOOLARGTYPE_ETHS_N :
      pc = "list/range of ethernet addresses";
      break;
    case NETWOX_TOOLARGTYPE_PORTS_SU :
    case NETWOX_TOOLARGTYPE_PORTS_U :
    case NETWOX_TOOLARGTYPE_PORTS_N :
      pc = "list/range of port number";
      break;
    case NETWOX_TOOLARGTYPE_UINT32 :
    case NETWOX_TOOLARGTYPE_UINT64 :
      pc = "unsigned number";
      break;
    case NETWOX_TOOLARGTYPE_INT32 :
    case NETWOX_TOOLARGTYPE_INT64 :
      pc = "signed number";
      break;
    case NETWOX_TOOLARGTYPE_BUF :
      pc = "user specified data";
      break;
    case NETWOX_TOOLARGTYPE_BUF_LOGIN :
      pc = "login";
      break;
    case NETWOX_TOOLARGTYPE_BUF_PASSWORD :
      pc = "password";
      break;
    case NETWOX_TOOLARGTYPE_BUF_DEVICE :
      pc = "device name";
      break;
    case NETWOX_TOOLARGTYPE_BUF_FILE :
      pc = "filename";
      break;
    case NETWOX_TOOLARGTYPE_BUF_FILE_RD :
      pc = "source filename";
      break;
    case NETWOX_TOOLARGTYPE_BUF_FILE_WR :
      pc = "destination filename";
      break;
    case NETWOX_TOOLARGTYPE_BUF_DIR :
      pc = "directory";
      break;
    case NETWOX_TOOLARGTYPE_BUF_DIR_RD :
      pc = "source directory";
      break;
    case NETWOX_TOOLARGTYPE_BUF_DIR_WR :
      pc = "destination directory";
      break;
    case NETWOX_TOOLARGTYPE_BUF_FILTER :
      pc = "pcap filter";
      break;
    case NETWOX_TOOLARGTYPE_BUF_OID :
      pc = "asn.1 (snmp) oid";
      break;
    case NETWOX_TOOLARGTYPE_BUF_OIDTYPE :
      pc = "asn.1 (snmp) oid type";
      break;
    case NETWOX_TOOLARGTYPE_BUF_MIXED :
      pc = "mixed data";
      break;
    case NETWOX_TOOLARGTYPE_BUF_IP4OPTS :
      pc = "IPv4 options";
      break;
    case NETWOX_TOOLARGTYPE_BUF_IP6EXTS :
      pc = "IPv6 extensions";
      break;
    case NETWOX_TOOLARGTYPE_BUF_TCPOPTS :
      pc = "TCP options";
      break;
    case NETWOX_TOOLARGTYPE_BUF_EMAIL :
      pc = "email address";
      break;
    case NETWOX_TOOLARGTYPE_BUF_URI :
      pc = "uri";
      break;
    case NETWOX_TOOLARGTYPE_BUF_NEWSGROUP :
      pc = "newsgroup name";
      break;
    case NETWOX_TOOLARGTYPE_BUF_HOSTNAME :
      pc = "hostname";
      break;
    case NETWOX_TOOLARGTYPE_BUF_MD5 :
      pc = "wanted md5";
      break;
    case NETWOX_TOOLARGTYPE_BUF_USERAGENT :
      pc = "user agent";
      break;
    case NETWOX_TOOLARGTYPE_BUF_NETBIOSNAME :
      pc = "NetBIOS name";
      break;
    case NETWOX_TOOLARGTYPE_BUF_SMBSHARE :
      pc = "share";
      break;
    case NETWOX_TOOLARGTYPE_ENCODE :
      pc = "encoding type";
      break;
    case NETWOX_TOOLARGTYPE_RECORDENCODE :
      pc = "record encoding type";
      break;
    case NETWOX_TOOLARGTYPE_DECODE :
      pc = "decoding type";
      break;
    case NETWOX_TOOLARGTYPE_DLT :
      pc = "device dlt type";
      break;
    case NETWOX_TOOLARGTYPE_SPOOFIP :
      pc = "IP spoof initialization type";
      break;
    case NETWOX_TOOLARGTYPE_IPTYPE :
      pc = "IP type";
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  netwib_er(netwib_buf_append_string(pc, pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_toolarg_get_default_sys_ip(netwib_uint32 devnum,
                                               netwox_toolargtype toolargtype,
                                                    netwib_bool *pfound,
                                                    netwib_buf *pbuf)
{
  netwib_conf_ip conf;
  netwib_conf_ip_index *pconfindex;
  netwib_err ret;

  *pfound = NETWIB_FALSE;
  netwib_er(netwib_conf_ip_index_init(&conf, &pconfindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_conf_ip_index_next(pconfindex);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) {
        ret = NETWIB_ERR_OK;
      }
      break;
    }
    if (conf.devnum == devnum) {
      if (toolargtype == NETWOX_TOOLARGTYPE_IP4 &&
          conf.ip.iptype != NETWIB_IPTYPE_IP4) {
        continue;
      }
      if (toolargtype == NETWOX_TOOLARGTYPE_IP6 &&
          conf.ip.iptype != NETWIB_IPTYPE_IP6) {
        continue;
      }
      netwib_er(netwib_buf_append_ip(&conf.ip, NETWIB_IP_ENCODETYPE_IP, pbuf));
      *pfound = NETWIB_TRUE;
      break;
    }
  }
  netwib_er(netwib_conf_ip_index_close(&pconfindex));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_toolarg_get_default_sys(netwox_toolargtype toolargtype,
                                                 netwib_buf *pbuf)
{
  netwib_conf_devices conf;
  netwib_conf_devices_index *pconfindex;
  netwib_uint32 pointsobtained, points, previousendoffset, originalendoffset;
  netwib_bool found;
  netwib_err ret;

  pointsobtained = 0;
  originalendoffset = pbuf->endoffset;
  netwib_er(netwib_conf_devices_index_init(&conf, &pconfindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_conf_devices_index_next(pconfindex);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    points = 0;
    switch(conf.hwtype) {
      case NETWIB_DEVICE_HWTYPE_LOOPBACK : points = 1; break;
      case NETWIB_DEVICE_HWTYPE_PPP : points = 2; break;
      case NETWIB_DEVICE_HWTYPE_ETHER : points = 3; break;
      default: break;
    }
    if (points > pointsobtained) {
      previousendoffset = pbuf->endoffset;
      pbuf->endoffset = originalendoffset;
      switch (toolargtype) {
        case NETWOX_TOOLARGTYPE_IP :
        case NETWOX_TOOLARGTYPE_IP4 :
        case NETWOX_TOOLARGTYPE_IP6 :
          netwib_er(netwox_toolarg_get_default_sys_ip(conf.devnum, toolargtype,
                                                      &found, pbuf));
          if (!found) {
            /* this device does not have IP address, keep previous found */
            points = pointsobtained;
            pbuf->endoffset = previousendoffset;
          }
          break;
        case NETWOX_TOOLARGTYPE_ETH :
          netwib_er(netwib_buf_append_eth(&conf.eth, pbuf));
          break;
        case NETWOX_TOOLARGTYPE_BUF_DEVICE :
          netwib_er(netwib_buf_append_buf(&conf.deviceeasy, pbuf));
          break;
        default:
          break;
      }
      pointsobtained = points;
      if (pointsobtained == 3) { /* cannot have more */
        break;
      }
    }
  }
  netwib_er(netwib_conf_devices_index_close(&pconfindex));

  if (pointsobtained == 0) {
    switch (toolargtype) {
      case NETWOX_TOOLARGTYPE_IP :
      case NETWOX_TOOLARGTYPE_IP4 :
        netwib_er(netwib_buf_append_string("1.2.3.4", pbuf));
        break;
      case NETWOX_TOOLARGTYPE_IP6 :
        netwib_er(netwib_buf_append_string("fec0:0:0:1::1", pbuf));
        break;
      case NETWOX_TOOLARGTYPE_ETH :
        netwib_er(netwib_buf_append_string("0:2:3:4:5:6", pbuf));
        break;
      case NETWOX_TOOLARGTYPE_BUF_DEVICE :
        netwib_er(netwib_buf_append_string("eth0", pbuf));
        break;
      default :
        return(NETWOX_ERR_INTERNALERROR);
    }
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_toolarg_get_default(const netwox_toolarg *ptoolarg,
                                      netwib_buf *pbuf)
{
  /* user specified value */
  if (ptoolarg->defaultvalue != NULL) {
    return(netwib_buf_append_string(ptoolarg->defaultvalue, pbuf));
  }

  /*
    IMPORTANT :
      Default value is a value permitting tool to work. It is not
      an example. It's the most generic value.
      For example, a file is "file.txt". If it were "", tools will
      not work because a filename must have a length.
      For example, a password is "". An empty password is a valid value,
      and will work everywhere. A value of "my password" is correct,
      but it is not sufficiently generic.
  */

  /* now, deduct */
  switch (ptoolarg->toolargtype) {
    case NETWOX_TOOLARGTYPE_BOOL :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO1 :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO2 :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO3 :
      netwib_er(netwib_buf_append_string("0", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_IP :
    case NETWOX_TOOLARGTYPE_IP_SRC :
      netwib_er(netwox_toolarg_get_default_sys(NETWOX_TOOLARGTYPE_IP, pbuf));
      break;
    case NETWOX_TOOLARGTYPE_IP4 :
    case NETWOX_TOOLARGTYPE_IP4_SRC :
      netwib_er(netwox_toolarg_get_default_sys(NETWOX_TOOLARGTYPE_IP4, pbuf));
      break;
    case NETWOX_TOOLARGTYPE_IP6 :
    case NETWOX_TOOLARGTYPE_IP6_SRC :
      netwib_er(netwox_toolarg_get_default_sys(NETWOX_TOOLARGTYPE_IP6, pbuf));
      break;
    case NETWOX_TOOLARGTYPE_IP_DST :
    case NETWOX_TOOLARGTYPE_IP4_DST :
      netwib_er(netwib_buf_append_string("5.6.7.8", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_IP6_DST :
      netwib_er(netwib_buf_append_string("fec0:0:0:1::2", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_ETH :
    case NETWOX_TOOLARGTYPE_ETH_SRC :
      netwib_er(netwox_toolarg_get_default_sys(NETWOX_TOOLARGTYPE_ETH, pbuf));
      break;
    case NETWOX_TOOLARGTYPE_ETH_DST :
      netwib_er(netwib_buf_append_string("0:8:9:a:b:c", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_PORT :
      netwib_er(netwib_buf_append_string("123", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_PORT_SRC :
      netwib_er(netwib_buf_append_string("1234", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_PORT_DST :
      netwib_er(netwib_buf_append_string("80", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_IPS_SU :
    case NETWOX_TOOLARGTYPE_IPS_U :
    case NETWOX_TOOLARGTYPE_IPS_N :
    case NETWOX_TOOLARGTYPE_ETHS_SU :
    case NETWOX_TOOLARGTYPE_ETHS_U :
    case NETWOX_TOOLARGTYPE_ETHS_N :
    case NETWOX_TOOLARGTYPE_PORTS_SU :
    case NETWOX_TOOLARGTYPE_PORTS_U :
    case NETWOX_TOOLARGTYPE_PORTS_N :
      netwib_er(netwib_buf_append_string("all", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_UINT32 :
    case NETWOX_TOOLARGTYPE_UINT64 :
    case NETWOX_TOOLARGTYPE_INT32 :
    case NETWOX_TOOLARGTYPE_INT64 :
      netwib_er(netwib_buf_append_string("0", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_BUF :
    case NETWOX_TOOLARGTYPE_BUF_LOGIN :
    case NETWOX_TOOLARGTYPE_BUF_PASSWORD :
    case NETWOX_TOOLARGTYPE_BUF_FILTER :
      netwib_er(netwib_buf_append_string("", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_BUF_DEVICE :
      netwib_er(netwox_toolarg_get_default_sys(NETWOX_TOOLARGTYPE_BUF_DEVICE,
                                               pbuf));
      break;
    case NETWOX_TOOLARGTYPE_BUF_FILE :
      netwib_er(netwib_buf_append_string("file.txt", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_BUF_FILE_RD :
      netwib_er(netwib_buf_append_string("srcfile.txt", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_BUF_FILE_WR :
      netwib_er(netwib_buf_append_string("dstfile.txt", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_BUF_DIR :
      netwib_er(netwib_buf_append_string(NETWOX_DIREX, pbuf));
      break;
    case NETWOX_TOOLARGTYPE_BUF_DIR_RD :
      netwib_er(netwib_buf_append_string(NETWOX_DIREX_RD, pbuf));
      break;
    case NETWOX_TOOLARGTYPE_BUF_DIR_WR :
      netwib_er(netwib_buf_append_string(NETWOX_DIREX_WR, pbuf));
      break;
    case NETWOX_TOOLARGTYPE_BUF_OID :
      netwib_er(netwib_buf_append_string(".1.3.6.1.2.1.1.4.0", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_BUF_OIDTYPE :
      netwib_er(netwib_buf_append_string("s", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_BUF_MIXED :
      netwib_er(netwib_buf_append_string("", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_BUF_EMAIL :
      netwib_er(netwib_buf_append_string("user@example.com", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_BUF_URI :
      netwib_er(netwib_buf_append_string("http://www.example.com/", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_BUF_NEWSGROUP :
      netwib_er(netwib_buf_append_string("comp.lang.c", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_BUF_HOSTNAME :
      netwib_er(netwib_buf_append_string("www.example.com", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_BUF_MD5 :
      netwib_er(netwib_buf_append_string("12345678901234567890123456789012",
                                       pbuf));
      break;
    case NETWOX_TOOLARGTYPE_BUF_USERAGENT :
      netwib_er(netwib_buf_append_string("Mozilla/5.0", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_BUF_NETBIOSNAME :
      netwib_er(netwib_buf_append_string("", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_BUF_SMBSHARE :
      netwib_er(netwib_buf_append_string("C$", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_BUF_IP4OPTS :
    case NETWOX_TOOLARGTYPE_BUF_IP6EXTS :
    case NETWOX_TOOLARGTYPE_BUF_TCPOPTS :
      netwib_er(netwib_buf_append_string("", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_ENCODE :
      netwib_er(netwib_buf_append_string("data", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_RECORDENCODE :
      netwib_er(netwib_buf_append_string("bin", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_DECODE :
      netwib_er(netwib_buf_append_string("data", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_DLT :
      netwib_er(netwib_buf_append_string("ether", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_SPOOFIP :
      netwib_er(netwib_buf_append_string("best", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_IPTYPE :
      netwib_er(netwib_buf_append_string("ip4", pbuf));
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_buf_append_toolarg_form(const netwox_toolarg *ptoolarg,
                                                 netwib_buf *pbuf)
{
  netwib_conststring enttype=NULL;
  netwib_bool isreq;

  switch(ptoolarg->toolargclass) {
    case NETWOX_TOOLARGCLASS_OPT :
      isreq = NETWIB_FALSE;
      break;
    case NETWOX_TOOLARGCLASS_REQ :
      isreq = NETWIB_TRUE;
      break;
    case NETWOX_TOOLARGCLASS_MORE :
      netwib_er(netwib_buf_append_string("  netwag_toolform_add_more \"", pbuf));
      netwib_er(netwox_toolarg_get_description(ptoolarg, pbuf));
      netwib_er(netwib_buf_append_string("\" \"", pbuf));
      netwib_er(netwox_toolarg_get_default(ptoolarg, pbuf));
      netwib_er(netwib_buf_append_string("\"\n", pbuf));
      return(NETWIB_ERR_OK);
      break;
    default :
      return(NETWOX_ERR_TOOLARG_NOTDECODED);
      break;
  }

#define netwox__toolarg_form_common() netwib_er(netwib_buf_append_fmt(pbuf, " %{uint32}", isreq)); netwib_er(netwib_buf_append_string(" \"", pbuf)); netwib_er(netwib_buf_append_byte(ptoolarg->key, pbuf)); netwib_er(netwib_buf_append_string("\" \"", pbuf)); netwib_er(netwox_toolarg_get_name(ptoolarg, pbuf)); netwib_er(netwib_buf_append_string("\" \"", pbuf)); netwib_er(netwox_toolarg_get_name(ptoolarg, pbuf)); netwib_er(netwib_buf_append_string(": ", pbuf)); netwib_er(netwox_toolarg_get_description(ptoolarg, pbuf)); netwib_er(netwib_buf_append_string("\"", pbuf));

  switch(ptoolarg->toolargtype) {
    case NETWOX_TOOLARGTYPE_BOOL :
      netwib_er(netwib_buf_append_string("  netwag_toolform_add_button", pbuf));
      netwox__toolarg_form_common();
      netwib_er(netwib_buf_append_string(" \"", pbuf));
      netwib_er(netwox_toolarg_get_default(ptoolarg, pbuf));
      netwib_er(netwib_buf_append_string("\"\n", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_BOOL_RADIO1 :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO2 :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO3 :
      netwib_er(netwib_buf_append_string("  netwag_toolform_add_radio", pbuf));
      switch(ptoolarg->toolargtype) {
      case NETWOX_TOOLARGTYPE_BOOL_RADIO1 :
        netwib_er(netwib_buf_append_string(" 1", pbuf));
        break;
      case NETWOX_TOOLARGTYPE_BOOL_RADIO2 :
        netwib_er(netwib_buf_append_string(" 2", pbuf));
        break;
      case NETWOX_TOOLARGTYPE_BOOL_RADIO3 :
        netwib_er(netwib_buf_append_string(" 3", pbuf));
        break;
      default :
        return(NETWOX_ERR_INTERNALERROR);
      }
      netwox__toolarg_form_common();
      netwib_er(netwib_buf_append_string(" \"", pbuf));
      netwib_er(netwox_toolarg_get_default(ptoolarg, pbuf));
      netwib_er(netwib_buf_append_string("\"\n", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_IP :
    case NETWOX_TOOLARGTYPE_IP_SRC :
    case NETWOX_TOOLARGTYPE_IP_DST :
    case NETWOX_TOOLARGTYPE_IP4 :
    case NETWOX_TOOLARGTYPE_IP4_SRC :
    case NETWOX_TOOLARGTYPE_IP4_DST :
    case NETWOX_TOOLARGTYPE_IP6 :
    case NETWOX_TOOLARGTYPE_IP6_SRC :
    case NETWOX_TOOLARGTYPE_IP6_DST :
    case NETWOX_TOOLARGTYPE_ETH :
    case NETWOX_TOOLARGTYPE_ETH_SRC :
    case NETWOX_TOOLARGTYPE_ETH_DST :
    case NETWOX_TOOLARGTYPE_PORT :
    case NETWOX_TOOLARGTYPE_PORT_SRC :
    case NETWOX_TOOLARGTYPE_PORT_DST :
    case NETWOX_TOOLARGTYPE_IPS_SU :
    case NETWOX_TOOLARGTYPE_IPS_U :
    case NETWOX_TOOLARGTYPE_IPS_N :
    case NETWOX_TOOLARGTYPE_ETHS_SU :
    case NETWOX_TOOLARGTYPE_ETHS_U :
    case NETWOX_TOOLARGTYPE_ETHS_N :
    case NETWOX_TOOLARGTYPE_PORTS_SU :
    case NETWOX_TOOLARGTYPE_PORTS_U :
    case NETWOX_TOOLARGTYPE_PORTS_N :
    case NETWOX_TOOLARGTYPE_UINT32 :
    case NETWOX_TOOLARGTYPE_INT32 :
    case NETWOX_TOOLARGTYPE_UINT64 :
    case NETWOX_TOOLARGTYPE_INT64 :
    case NETWOX_TOOLARGTYPE_BUF :
    case NETWOX_TOOLARGTYPE_BUF_LOGIN :
    case NETWOX_TOOLARGTYPE_BUF_PASSWORD :
    case NETWOX_TOOLARGTYPE_BUF_FILE :
    case NETWOX_TOOLARGTYPE_BUF_FILE_RD :
    case NETWOX_TOOLARGTYPE_BUF_FILE_WR :
    case NETWOX_TOOLARGTYPE_BUF_DIR :
    case NETWOX_TOOLARGTYPE_BUF_DIR_RD :
    case NETWOX_TOOLARGTYPE_BUF_DIR_WR :
    case NETWOX_TOOLARGTYPE_BUF_FILTER :
    case NETWOX_TOOLARGTYPE_BUF_OID :
    case NETWOX_TOOLARGTYPE_BUF_MIXED :
    case NETWOX_TOOLARGTYPE_BUF_IP4OPTS :
    case NETWOX_TOOLARGTYPE_BUF_IP6EXTS :
    case NETWOX_TOOLARGTYPE_BUF_TCPOPTS :
    case NETWOX_TOOLARGTYPE_BUF_EMAIL :
    case NETWOX_TOOLARGTYPE_BUF_URI :
    case NETWOX_TOOLARGTYPE_BUF_NEWSGROUP :
    case NETWOX_TOOLARGTYPE_BUF_HOSTNAME :
    case NETWOX_TOOLARGTYPE_BUF_MD5 :
    case NETWOX_TOOLARGTYPE_BUF_USERAGENT :
    case NETWOX_TOOLARGTYPE_BUF_NETBIOSNAME :
    case NETWOX_TOOLARGTYPE_BUF_SMBSHARE :
      netwib_er(netwib_buf_append_string("  netwag_toolform_add_entry", pbuf));
      switch(ptoolarg->toolargtype) {
      case NETWOX_TOOLARGTYPE_IP :
      case NETWOX_TOOLARGTYPE_IP_SRC :
      case NETWOX_TOOLARGTYPE_IP_DST :
        enttype = "ip";
        break;
      case NETWOX_TOOLARGTYPE_IP4 :
      case NETWOX_TOOLARGTYPE_IP4_SRC :
      case NETWOX_TOOLARGTYPE_IP4_DST :
        enttype = "ip4";
        break;
      case NETWOX_TOOLARGTYPE_IP6 :
      case NETWOX_TOOLARGTYPE_IP6_SRC :
      case NETWOX_TOOLARGTYPE_IP6_DST :
        enttype = "ip6";
        break;
      case NETWOX_TOOLARGTYPE_ETH :
      case NETWOX_TOOLARGTYPE_ETH_SRC :
      case NETWOX_TOOLARGTYPE_ETH_DST :
        enttype = "eth";
        break;
      case NETWOX_TOOLARGTYPE_PORT :
      case NETWOX_TOOLARGTYPE_PORT_SRC :
      case NETWOX_TOOLARGTYPE_PORT_DST :
        enttype = "port";
        break;
      case NETWOX_TOOLARGTYPE_IPS_SU :
      case NETWOX_TOOLARGTYPE_IPS_U :
      case NETWOX_TOOLARGTYPE_IPS_N :
        enttype = "ips";
        break;
      case NETWOX_TOOLARGTYPE_ETHS_SU :
      case NETWOX_TOOLARGTYPE_ETHS_U :
      case NETWOX_TOOLARGTYPE_ETHS_N :
        enttype = "eths";
        break;
      case NETWOX_TOOLARGTYPE_PORTS_SU :
      case NETWOX_TOOLARGTYPE_PORTS_U :
      case NETWOX_TOOLARGTYPE_PORTS_N :
        enttype = "ports";
        break;
      case NETWOX_TOOLARGTYPE_UINT32 :
        enttype = "uint32";
        break;
      case NETWOX_TOOLARGTYPE_UINT64 :
        enttype = "uint64";
        break;
      case NETWOX_TOOLARGTYPE_INT32 :
        enttype = "int32";
        break;
      case NETWOX_TOOLARGTYPE_INT64 :
        enttype = "int64";
        break;
      case NETWOX_TOOLARGTYPE_BUF :
        enttype = "buf";
        break;
      case NETWOX_TOOLARGTYPE_BUF_LOGIN :
        enttype = "login";
        break;
      case NETWOX_TOOLARGTYPE_BUF_PASSWORD :
        enttype = "password";
        break;
      case NETWOX_TOOLARGTYPE_BUF_FILE :
        enttype = "file";
        break;
      case NETWOX_TOOLARGTYPE_BUF_FILE_RD :
        enttype = "filerd";
        break;
      case NETWOX_TOOLARGTYPE_BUF_FILE_WR :
        enttype = "filewr";
        break;
      case NETWOX_TOOLARGTYPE_BUF_DIR :
        enttype = "dir";
        break;
      case NETWOX_TOOLARGTYPE_BUF_DIR_RD :
        enttype = "dirrd";
        break;
      case NETWOX_TOOLARGTYPE_BUF_DIR_WR :
        enttype = "dirwr";
        break;
      case NETWOX_TOOLARGTYPE_BUF_FILTER :
        enttype = "filter";
        break;
      case NETWOX_TOOLARGTYPE_BUF_OID :
        enttype = "oid";
        break;
      case NETWOX_TOOLARGTYPE_BUF_OIDTYPE :
        enttype = "oidtype";
        break;
      case NETWOX_TOOLARGTYPE_BUF_MIXED :
        enttype = "mixed";
        break;
      case NETWOX_TOOLARGTYPE_BUF_IP4OPTS :
        enttype = "ip4opts";
        break;
      case NETWOX_TOOLARGTYPE_BUF_IP6EXTS :
        enttype = "ip6exts";
        break;
      case NETWOX_TOOLARGTYPE_BUF_TCPOPTS :
        enttype = "tcpopts";
        break;
      case NETWOX_TOOLARGTYPE_BUF_EMAIL :
        enttype = "email";
        break;
      case NETWOX_TOOLARGTYPE_BUF_URI :
        enttype = "uri";
        break;
      case NETWOX_TOOLARGTYPE_BUF_NEWSGROUP :
        enttype = "newsgroup";
        break;
      case NETWOX_TOOLARGTYPE_BUF_HOSTNAME :
        enttype = "hostname";
        break;
      case NETWOX_TOOLARGTYPE_BUF_MD5 :
        enttype = "md5";
        break;
      case NETWOX_TOOLARGTYPE_BUF_USERAGENT :
        enttype = "useragent";
        break;
      case NETWOX_TOOLARGTYPE_BUF_NETBIOSNAME :
        enttype = "netbiosname";
        break;
      case NETWOX_TOOLARGTYPE_BUF_SMBSHARE :
        enttype = "smbshare";
        break;
      default :
        return(NETWIB_ERR_PAINVALIDTYPE);
      }
      netwib_er(netwib_buf_append_fmt(pbuf, " %s", enttype));
      netwox__toolarg_form_common();
      netwib_er(netwib_buf_append_string(" \"", pbuf));
      netwib_er(netwox_toolarg_get_default(ptoolarg, pbuf));
      netwib_er(netwib_buf_append_string("\"\n", pbuf));
      break;
    case NETWOX_TOOLARGTYPE_BUF_DEVICE :
    case NETWOX_TOOLARGTYPE_ENCODE :
    case NETWOX_TOOLARGTYPE_RECORDENCODE :
    case NETWOX_TOOLARGTYPE_DECODE :
    case NETWOX_TOOLARGTYPE_DLT :
    case NETWOX_TOOLARGTYPE_SPOOFIP :
    case NETWOX_TOOLARGTYPE_IPTYPE :
    case NETWOX_TOOLARGTYPE_BUF_OIDTYPE :
      netwib_er(netwib_buf_append_string("  netwag_toolform_add_listbox", pbuf));
      netwox__toolarg_form_common();
      netwib_er(netwib_buf_append_string(" {", pbuf));
      switch(ptoolarg->toolargtype) {
        case NETWOX_TOOLARGTYPE_BUF_DEVICE :
          {
            netwib_conf_devices conf;
            netwib_conf_devices_index *pconfindex;
            netwib_err ret;
            netwib_er(netwib_conf_devices_index_init(&conf, &pconfindex));
            ret = NETWIB_ERR_OK;
            while (NETWIB_TRUE) {
              ret = netwib_conf_devices_index_next(pconfindex);
              if (ret != NETWIB_ERR_OK) {
                if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
                break;
              }
              netwib_er(netwib_buf_append_string(" ", pbuf));
              netwib_er(netwib_buf_append_buf(&conf.deviceeasy, pbuf));
            }
            netwib_er(netwib_conf_devices_index_close(&pconfindex));
          }
          break;
        case NETWOX_TOOLARGTYPE_ENCODE :
          netwib_er(netwib_buf_append_string("array base64 base64_wrap data data_wrap dump hexa hexa_wrap mixed mixed_wrap mixedh_wrap nothing synth text text_wrap lowercase uppercase", pbuf));
          break;
        case NETWOX_TOOLARGTYPE_RECORDENCODE :
          netwib_er(netwib_buf_append_string("bin dump hexa hexa_wrap mixed mixed_wrap mixedh_wrap pcap", pbuf));
          break;
        case NETWOX_TOOLARGTYPE_DECODE :
          netwib_er(netwib_buf_append_string("base64 data hexa mixed", pbuf));
          break;
        case NETWOX_TOOLARGTYPE_DLT :
          netwib_er(netwib_buf_append_string("aironet-header arcnet atm-clip atm-rfc1483 ax25 c-hdlc chaos econet en3mb ether fddi ieee802 ieee802-11 linux-sll loop ltalk null ppp ppp-bsdos ppp-ether ppp-serial prism-header pronet raw slip slip-bsdos", pbuf));
          break;
        case NETWOX_TOOLARGTYPE_SPOOFIP :
          netwib_er(netwib_buf_append_string("raw linkf linkb linkfb rawlinkf rawlinkb rawlinkfb linkfraw linkbraw linkfbraw link rawlink linkraw best", pbuf));
          break;
        case NETWOX_TOOLARGTYPE_IPTYPE :
          netwib_er(netwib_buf_append_string("ip4 ip6", pbuf));
          break;
        case NETWOX_TOOLARGTYPE_BUF_OIDTYPE :
          netwib_er(netwib_buf_append_string("i s m n o a c g t M u U", pbuf));
          break;
        default :
          break;
      }
      netwib_er(netwib_buf_append_string("} \"", pbuf));
      netwib_er(netwox_toolarg_get_default(ptoolarg, pbuf));
      netwib_er(netwib_buf_append_string("\"\n", pbuf));
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_buf_append_toolarg(const netwox_toolarg *ptoolarg,
                                     netwox_toolarg_encodetype encodetype,
                                     netwib_buf *pbuf)
{
  netwib_buf buf;

  switch(encodetype) {
    case NETWOX_TOOLARG_ENCODETYPE_HELPUSAGE :
      switch(ptoolarg->toolargclass) {
        case NETWOX_TOOLARGCLASS_OPT :
        case NETWOX_TOOLARGCLASS_REQ :
          netwib_er(netwib_buf_append_byte(' ', pbuf));
          if (ptoolarg->toolargclass == NETWOX_TOOLARGCLASS_OPT) {
            netwib_er(netwib_buf_append_byte('[', pbuf));
          }
          netwib_er(netwib_buf_append_byte('-', pbuf));
          netwib_er(netwib_buf_append_byte(ptoolarg->key, pbuf));
          if (ptoolarg->toolargtype == NETWOX_TOOLARGTYPE_BOOL ||
              ptoolarg->toolargtype == NETWOX_TOOLARGTYPE_BOOL_RADIO1 ||
              ptoolarg->toolargtype == NETWOX_TOOLARGTYPE_BOOL_RADIO2 ||
              ptoolarg->toolargtype == NETWOX_TOOLARGTYPE_BOOL_RADIO3) {
            netwib_er(netwib_buf_append_byte('|', pbuf));
            netwib_er(netwib_buf_append_byte('+', pbuf));
            netwib_er(netwib_buf_append_byte(ptoolarg->key, pbuf));
          } else {
            netwib_er(netwib_buf_append_byte(' ', pbuf));
            netwib_er(netwox_buf_append_toolargtype(ptoolarg->toolargtype,
                                                    pbuf));
          }
          if (ptoolarg->toolargclass == NETWOX_TOOLARGCLASS_OPT) {
            netwib_er(netwib_buf_append_byte(']', pbuf));
          }
          break;
        case NETWOX_TOOLARGCLASS_MORE :
          netwib_er(netwib_buf_append_string(" [", pbuf));
          if (ptoolarg->name != NULL) {
            netwib_er(netwib_buf_append_string(ptoolarg->name, pbuf));
          } else {
            netwib_er(netwib_buf_append_string("...", pbuf));
          }
          netwib_er(netwib_buf_append_byte(']', pbuf));
          break;
        default :
          return(NETWOX_ERR_TOOLARG_NOTDECODED);
          break;
      }
      break;
    case NETWOX_TOOLARG_ENCODETYPE_HELPLONG :
      netwib_er(netwib_buf_init_mallocdefault(&buf));
      switch(ptoolarg->toolargclass) {
        case NETWOX_TOOLARGCLASS_OPT :
        case NETWOX_TOOLARGCLASS_REQ :
          netwib_er(netwib_buf_append_byte('-', &buf));
          netwib_er(netwib_buf_append_byte(ptoolarg->key, &buf));
          netwib_er(netwib_buf_append_string("|--", &buf));
          netwib_er(netwox_toolarg_get_name(ptoolarg, &buf));
          if (ptoolarg->toolargtype == NETWOX_TOOLARGTYPE_BOOL ||
              ptoolarg->toolargtype == NETWOX_TOOLARGTYPE_BOOL_RADIO1 ||
              ptoolarg->toolargtype == NETWOX_TOOLARGTYPE_BOOL_RADIO2 ||
              ptoolarg->toolargtype == NETWOX_TOOLARGTYPE_BOOL_RADIO3) {
            netwib_er(netwib_buf_append_string("|+", &buf));
            netwib_er(netwib_buf_append_byte(ptoolarg->key, &buf));
            netwib_er(netwib_buf_append_string("|--no-", &buf));
            netwib_er(netwox_toolarg_get_name(ptoolarg, &buf));
            if (ptoolarg->toolargclass == NETWOX_TOOLARGCLASS_REQ) {
              netwib_er(netwib_buf_append_string("|1|0", &buf));
            }
          } else {
            netwib_er(netwib_buf_append_byte(' ', &buf));
            netwib_er(netwox_buf_append_toolargtype(ptoolarg->toolargtype,
                                                    &buf));
          }
          netwib_er(netwib_buf_append_fmt(pbuf, " %{l 30;buf} ", &buf));
          netwib_er(netwox_toolarg_get_description(ptoolarg, pbuf));
          netwib__buf_reinit(&buf);
          netwib_er(netwox_toolarg_get_default(ptoolarg, &buf));
          if (netwib__buf_ref_data_size(&buf)) {
            if (ptoolarg->toolargtype == NETWOX_TOOLARGTYPE_BOOL ||
                ptoolarg->toolargtype == NETWOX_TOOLARGTYPE_BOOL_RADIO1 ||
                ptoolarg->toolargtype == NETWOX_TOOLARGTYPE_BOOL_RADIO2 ||
                ptoolarg->toolargtype == NETWOX_TOOLARGTYPE_BOOL_RADIO3) {
              netwib_data data;
              data = netwib__buf_ref_data_ptr(&buf);
              if (data[0] != '0') {
                netwib_er(netwib_buf_append_string(" {SET}", pbuf));
              }
            } else {
              netwib_er(netwib_buf_append_string(" {", pbuf));
              netwib_er(netwib_buf_append_buf(&buf, pbuf));
              netwib_er(netwib_buf_append_string("}", pbuf));
            }
          }
          break;
        case NETWOX_TOOLARGCLASS_MORE :
          if (ptoolarg->name != NULL) {
            netwib_er(netwib_buf_append_string(ptoolarg->name, &buf));
          } else {
            netwib_er(netwib_buf_append_string("...", &buf));
          }
          netwib_er(netwib_buf_append_fmt(pbuf, " %{l 30;buf} ", &buf));
          if (ptoolarg->description != NULL) {
            netwib_er(netwib_buf_append_string(ptoolarg->description, pbuf));
          } else {
            netwib_er(netwib_buf_append_string("optional parameters", pbuf));
          }
          if (ptoolarg->defaultvalue != NULL) {
            if (ptoolarg->description != NULL) {
              netwib_er(netwib_buf_append_byte(' ', pbuf));
            }
            netwib_er(netwib_buf_append_fmt(pbuf, "{%s}",
                                            ptoolarg->defaultvalue));
          }
          break;
        default :
          return(NETWOX_ERR_TOOLARG_NOTDECODED);
          break;
      }
      netwib_er(netwib_buf_close(&buf));
      break;
    case NETWOX_TOOLARG_ENCODETYPE_HELPEXAMPLE1 :
      switch(ptoolarg->toolargclass) {
        case NETWOX_TOOLARGCLASS_OPT :
          break;
        case NETWOX_TOOLARGCLASS_MORE :
          if (ptoolarg->name != NULL) {
            netwib_er(netwib_buf_append_string(" ", pbuf));
            netwib_er(netwib_buf_append_string(ptoolarg->name, pbuf));
          } else {
            netwib_er(netwib_buf_append_string(" ...", &buf));
          }
          break;
        case NETWOX_TOOLARGCLASS_REQ :
          netwib_er(netwib_buf_append_string(" -", pbuf));
          netwib_er(netwib_buf_append_byte(ptoolarg->key, pbuf));
          if (ptoolarg->toolargtype != NETWOX_TOOLARGTYPE_BOOL &&
              ptoolarg->toolargtype != NETWOX_TOOLARGTYPE_BOOL_RADIO1 &&
              ptoolarg->toolargtype != NETWOX_TOOLARGTYPE_BOOL_RADIO2 &&
              ptoolarg->toolargtype != NETWOX_TOOLARGTYPE_BOOL_RADIO3) {
            netwib_er(netwib_buf_append_string(" \"", pbuf));
            netwib_er(netwox_toolarg_get_default(ptoolarg, pbuf));
            netwib_er(netwib_buf_append_byte('"', pbuf));
          }
          break;
        default :
          return(NETWOX_ERR_TOOLARG_NOTDECODED);
          break;
      }
      break;
    case NETWOX_TOOLARG_ENCODETYPE_HELPEXAMPLE2 :
      switch(ptoolarg->toolargclass) {
        case NETWOX_TOOLARGCLASS_OPT :
          break;
        case NETWOX_TOOLARGCLASS_MORE :
          if (ptoolarg->name != NULL) {
            netwib_er(netwib_buf_append_string(" ", pbuf));
            netwib_er(netwib_buf_append_string(ptoolarg->name, pbuf));
          } else {
            netwib_er(netwib_buf_append_string(" ...", &buf));
          }
          break;
        case NETWOX_TOOLARGCLASS_REQ :
          netwib_er(netwib_buf_append_string(" --", pbuf));
          netwib_er(netwox_toolarg_get_name(ptoolarg, pbuf));
          if (ptoolarg->toolargtype != NETWOX_TOOLARGTYPE_BOOL &&
              ptoolarg->toolargtype != NETWOX_TOOLARGTYPE_BOOL_RADIO1 &&
              ptoolarg->toolargtype != NETWOX_TOOLARGTYPE_BOOL_RADIO2 &&
              ptoolarg->toolargtype != NETWOX_TOOLARGTYPE_BOOL_RADIO3) {
            netwib_er(netwib_buf_append_string(" \"", pbuf));
            netwib_er(netwox_toolarg_get_default(ptoolarg, pbuf));
            netwib_er(netwib_buf_append_byte('"', pbuf));
          }
          break;
        default :
          return(NETWOX_ERR_TOOLARG_NOTDECODED);
          break;
      }
      break;
    case NETWOX_TOOLARG_ENCODETYPE_KBD1 :
      netwib_er(netwox_toolarg_get_description(ptoolarg, pbuf));
      netwib_er(netwib_buf_append_string(" (--", pbuf));
      netwib_er(netwox_toolarg_get_name(ptoolarg, pbuf));
      netwib_er(netwib_buf_append_byte(')', pbuf));
      break;
    case NETWOX_TOOLARG_ENCODETYPE_KBD2 :
      netwib_er(netwox_toolarg_get_description(ptoolarg, pbuf));
      netwib_er(netwib_buf_append_string(" (", pbuf));
      netwib_er(netwib_buf_append_string("argument ", pbuf));
      netwib_er(netwib_buf_append_string("-", pbuf));
      netwib_er(netwib_buf_append_byte(ptoolarg->key, pbuf));
      netwib_er(netwib_buf_append_string("|--", pbuf));
      netwib_er(netwox_toolarg_get_name(ptoolarg, pbuf));
      netwib_er(netwib_buf_append_byte(')', pbuf));
      break;
    case NETWOX_TOOLARG_ENCODETYPE_FORM :
      netwib_er(netwox_buf_append_toolarg_form(ptoolarg, pbuf));
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}
