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
netwib_err netwox_toolargstore_init(const netwox_toolarg *ptoolarg,
                                    netwox_toolargstore *ptoolargstore)
{
  switch (ptoolarg->toolargtype) {
    case NETWOX_TOOLARGTYPE_BOOL :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO1 :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO2 :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO3 :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_BOOL;
      break;
    case NETWOX_TOOLARGTYPE_IP :
    case NETWOX_TOOLARGTYPE_IP_SRC :
    case NETWOX_TOOLARGTYPE_IP_DST :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_IP;
      break;
    case NETWOX_TOOLARGTYPE_IP4 :
    case NETWOX_TOOLARGTYPE_IP4_SRC :
    case NETWOX_TOOLARGTYPE_IP4_DST :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_IP4;
      break;
    case NETWOX_TOOLARGTYPE_IP6 :
    case NETWOX_TOOLARGTYPE_IP6_SRC :
    case NETWOX_TOOLARGTYPE_IP6_DST :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_IP6;
      break;
    case NETWOX_TOOLARGTYPE_ETH :
    case NETWOX_TOOLARGTYPE_ETH_SRC :
    case NETWOX_TOOLARGTYPE_ETH_DST :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_ETH;
      break;
    case NETWOX_TOOLARGTYPE_PORT :
    case NETWOX_TOOLARGTYPE_PORT_SRC :
    case NETWOX_TOOLARGTYPE_PORT_DST :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_PORT;
      break;
    case NETWOX_TOOLARGTYPE_IPS_SU :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_IPS_SU;
      netwib_er(netwib_ips_init(NETWIB_IPS_INITTYPE_SORTUNIQ,
                                &ptoolargstore->value.pips));
      break;
    case NETWOX_TOOLARGTYPE_IPS_U :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_IPS_U;
      netwib_er(netwib_ips_init(NETWIB_IPS_INITTYPE_NOTSORTUNIQ,
                                &ptoolargstore->value.pips));
      break;
    case NETWOX_TOOLARGTYPE_IPS_N :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_IPS_N;
      netwib_er(netwib_ips_init(NETWIB_IPS_INITTYPE_NOTSORTNOTUNIQ,
                                &ptoolargstore->value.pips));
      break;
    case NETWOX_TOOLARGTYPE_ETHS_SU :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_ETHS_SU;
      netwib_er(netwib_eths_init(NETWIB_ETHS_INITTYPE_SORTUNIQ,
                                 &ptoolargstore->value.peths));
      break;
    case NETWOX_TOOLARGTYPE_ETHS_U :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_ETHS_U;
      netwib_er(netwib_eths_init(NETWIB_ETHS_INITTYPE_NOTSORTUNIQ,
                                 &ptoolargstore->value.peths));
      break;
    case NETWOX_TOOLARGTYPE_ETHS_N :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_ETHS_N;
      netwib_er(netwib_eths_init(NETWIB_ETHS_INITTYPE_NOTSORTNOTUNIQ,
                                 &ptoolargstore->value.peths));
      break;
    case NETWOX_TOOLARGTYPE_PORTS_SU :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_PORTS_SU;
      netwib_er(netwib_ports_init(NETWIB_PORTS_INITTYPE_SORTUNIQ,
                                  &ptoolargstore->value.pports));
      break;
    case NETWOX_TOOLARGTYPE_PORTS_U :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_PORTS_U;
      netwib_er(netwib_ports_init(NETWIB_PORTS_INITTYPE_NOTSORTUNIQ,
                                  &ptoolargstore->value.pports));
      break;
    case NETWOX_TOOLARGTYPE_PORTS_N :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_PORTS_N;
      netwib_er(netwib_ports_init(NETWIB_PORTS_INITTYPE_NOTSORTNOTUNIQ,
                                  &ptoolargstore->value.pports));
      break;
    case NETWOX_TOOLARGTYPE_UINT32 :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_UINT32;
      break;
    case NETWOX_TOOLARGTYPE_INT32 :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_INT32;
      break;
    case NETWOX_TOOLARGTYPE_UINT64 :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_UINT64;
      break;
    case NETWOX_TOOLARGTYPE_INT64 :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_INT64;
      break;
    case NETWOX_TOOLARGTYPE_BUF :
    case NETWOX_TOOLARGTYPE_BUF_LOGIN :
    case NETWOX_TOOLARGTYPE_BUF_PASSWORD :
    case NETWOX_TOOLARGTYPE_BUF_DEVICE :
    case NETWOX_TOOLARGTYPE_BUF_FILE :
    case NETWOX_TOOLARGTYPE_BUF_FILE_RD :
    case NETWOX_TOOLARGTYPE_BUF_FILE_WR :
    case NETWOX_TOOLARGTYPE_BUF_DIR :
    case NETWOX_TOOLARGTYPE_BUF_DIR_RD :
    case NETWOX_TOOLARGTYPE_BUF_DIR_WR :
    case NETWOX_TOOLARGTYPE_BUF_FILTER :
    case NETWOX_TOOLARGTYPE_BUF_OID :
    case NETWOX_TOOLARGTYPE_BUF_OIDTYPE :
    case NETWOX_TOOLARGTYPE_BUF_EMAIL :
    case NETWOX_TOOLARGTYPE_BUF_URI :
    case NETWOX_TOOLARGTYPE_BUF_NEWSGROUP :
    case NETWOX_TOOLARGTYPE_BUF_HOSTNAME :
    case NETWOX_TOOLARGTYPE_BUF_MD5 :
    case NETWOX_TOOLARGTYPE_BUF_USERAGENT :
    case NETWOX_TOOLARGTYPE_BUF_NETBIOSNAME :
    case NETWOX_TOOLARGTYPE_BUF_SMBSHARE :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_BUF;
      netwib_er(netwib_buf_init_mallocdefault(&ptoolargstore->value.buf));
      break;
    case NETWOX_TOOLARGTYPE_BUF_MIXED :
    case NETWOX_TOOLARGTYPE_BUF_IP4OPTS :
    case NETWOX_TOOLARGTYPE_BUF_IP6EXTS :
    case NETWOX_TOOLARGTYPE_BUF_TCPOPTS :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_BUF_MIXED;
      netwib_er(netwib_buf_init_mallocdefault(&ptoolargstore->value.buf));
      break;
    case NETWOX_TOOLARGTYPE_ENCODE :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_ENCODE;
      break;
    case NETWOX_TOOLARGTYPE_DECODE :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_DECODE;
      break;
    case NETWOX_TOOLARGTYPE_RECORDENCODE :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_RECORDENCODE;
      break;
    case NETWOX_TOOLARGTYPE_DLT :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_DLT;
      break;
    case NETWOX_TOOLARGTYPE_SPOOFIP :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_SPOOFIP;
      break;
    case NETWOX_TOOLARGTYPE_IPTYPE :
      ptoolargstore->type = NETWOX_TOOLARGTYPE_IPTYPE;
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_toolargstore_close(netwox_toolargstore *ptoolargstore)

{
  switch (ptoolargstore->type) {
    case NETWOX_TOOLARGTYPE_BOOL :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO1 :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO2 :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO3 :
    case NETWOX_TOOLARGTYPE_IP :
    case NETWOX_TOOLARGTYPE_IP4 :
    case NETWOX_TOOLARGTYPE_IP6 :
    case NETWOX_TOOLARGTYPE_ETH :
    case NETWOX_TOOLARGTYPE_PORT :
    case NETWOX_TOOLARGTYPE_UINT32 :
    case NETWOX_TOOLARGTYPE_INT32 :
    case NETWOX_TOOLARGTYPE_UINT64 :
    case NETWOX_TOOLARGTYPE_INT64 :
    case NETWOX_TOOLARGTYPE_ENCODE :
    case NETWOX_TOOLARGTYPE_RECORDENCODE :
    case NETWOX_TOOLARGTYPE_DECODE :
    case NETWOX_TOOLARGTYPE_DLT :
    case NETWOX_TOOLARGTYPE_SPOOFIP :
    case NETWOX_TOOLARGTYPE_IPTYPE :
      /* nothing to close */
      break;
    case NETWOX_TOOLARGTYPE_IPS_SU :
    case NETWOX_TOOLARGTYPE_IPS_U :
    case NETWOX_TOOLARGTYPE_IPS_N :
      netwib_er(netwib_ips_close(&ptoolargstore->value.pips));
      break;
    case NETWOX_TOOLARGTYPE_ETHS_SU :
    case NETWOX_TOOLARGTYPE_ETHS_U :
    case NETWOX_TOOLARGTYPE_ETHS_N :
      netwib_er(netwib_eths_close(&ptoolargstore->value.peths));
      break;
    case NETWOX_TOOLARGTYPE_PORTS_SU :
    case NETWOX_TOOLARGTYPE_PORTS_U :
    case NETWOX_TOOLARGTYPE_PORTS_N :
      netwib_er(netwib_ports_close(&ptoolargstore->value.pports));
      break;
    case NETWOX_TOOLARGTYPE_BUF :
    case NETWOX_TOOLARGTYPE_BUF_MIXED :
      netwib_er(netwib_buf_close(&ptoolargstore->value.buf));
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_toolargstore_reset(netwox_toolargstore *ptoolargstore)
{
  netwib_buf buf;
  netwib_conststring pcnotall = "!all";

  switch (ptoolargstore->type) {
    case NETWOX_TOOLARGTYPE_BOOL :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO1 :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO2 :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO3 :
      ptoolargstore->value.b = NETWIB_FALSE;
      break;
    case NETWOX_TOOLARGTYPE_IP :
    case NETWOX_TOOLARGTYPE_IP4 :
    case NETWOX_TOOLARGTYPE_IP6 :
    case NETWOX_TOOLARGTYPE_ETH :
    case NETWOX_TOOLARGTYPE_PORT :
    case NETWOX_TOOLARGTYPE_UINT32 :
    case NETWOX_TOOLARGTYPE_INT32 :
    case NETWOX_TOOLARGTYPE_UINT64 :
    case NETWOX_TOOLARGTYPE_INT64 :
    case NETWOX_TOOLARGTYPE_ENCODE :
    case NETWOX_TOOLARGTYPE_RECORDENCODE :
    case NETWOX_TOOLARGTYPE_DECODE :
    case NETWOX_TOOLARGTYPE_DLT :
    case NETWOX_TOOLARGTYPE_SPOOFIP :
    case NETWOX_TOOLARGTYPE_IPTYPE :
      /* nothing to reset */
      break;
    case NETWOX_TOOLARGTYPE_IPS_SU :
    case NETWOX_TOOLARGTYPE_IPS_U :
    case NETWOX_TOOLARGTYPE_IPS_N :
      netwib_er(netwib_buf_init_ext_string(pcnotall, &buf));
      netwib_er(netwib_ips_add_buf(ptoolargstore->value.pips, &buf));
      break;
    case NETWOX_TOOLARGTYPE_ETHS_SU :
    case NETWOX_TOOLARGTYPE_ETHS_U :
    case NETWOX_TOOLARGTYPE_ETHS_N :
      netwib_er(netwib_buf_init_ext_string(pcnotall, &buf));
      netwib_er(netwib_eths_add_buf(ptoolargstore->value.peths, &buf));
      break;
    case NETWOX_TOOLARGTYPE_PORTS_SU :
    case NETWOX_TOOLARGTYPE_PORTS_U :
    case NETWOX_TOOLARGTYPE_PORTS_N :
      netwib_er(netwib_buf_init_ext_string(pcnotall, &buf));
      netwib_er(netwib_ports_add_buf(ptoolargstore->value.pports, &buf));
      break;
    case NETWOX_TOOLARGTYPE_BUF :
    case NETWOX_TOOLARGTYPE_BUF_MIXED :
      netwib__buf_reinit(&ptoolargstore->value.buf);
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_toolargstore_set(netwib_constbuf *pbuf,
                                   netwox_toolargstore *ptoolargstore)
{
  netwib_string str;
  netwib_ip ipad;
  netwib_data data;
  netwib_buf buf;
  netwib_err ret;

  netwib__constbuf_ref_string(pbuf, str, bufstorage,
                              netwox_toolargstore_set(&bufstorage, ptoolargstore));

  switch (ptoolargstore->type) {
    case NETWOX_TOOLARGTYPE_BOOL :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO1 :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO2 :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO3 :
      if (!netwib_c_strcmp(str, "0")) {
        ptoolargstore->value.b = NETWIB_FALSE;
      } else if (!netwib_c_strcmp(str, "1")) {
        ptoolargstore->value.b = NETWIB_TRUE;
      } else {
        netwib_er(netwib_fmt_display("Boolean must be 0 or 1 (instead of %s)\n",
                                   str));
        return(NETWOX_ERR_TOOLARG_NOTDECODED);
      }
      break;
    case NETWOX_TOOLARGTYPE_IP :
      netwib_er(netwib_ip_init_buf_best(pbuf, &ptoolargstore->value.ip));
      break;
    case NETWOX_TOOLARGTYPE_IP4 :
      netwib_er(netwib_ip_init_buf(pbuf, NETWIB_IP_DECODETYPE_IP4HN4,
                                   &ptoolargstore->value.ip));
      if (ptoolargstore->value.ip.iptype != NETWIB_IPTYPE_IP4) {
        return(NETWIB_ERR_PAIPTYPENOT4);
      }
      break;
    case NETWOX_TOOLARGTYPE_IP6 :
      netwib_er(netwib_ip_init_buf(pbuf, NETWIB_IP_DECODETYPE_IP6HN6,
                                   &ptoolargstore->value.ip));
      if (ptoolargstore->value.ip.iptype != NETWIB_IPTYPE_IP6) {
        return(NETWIB_ERR_PAIPTYPENOT6);
      }
      break;
    case NETWOX_TOOLARGTYPE_ETH :
      if (netwib__buf_ref_data_size(pbuf)) {
        data = netwib__buf_ref_data_ptr(pbuf);
        if (data[0] == '@') {
          /* "@ip" trick */
          buf = *pbuf;
          buf.beginoffset++;
          netwib_er(netwib_ip_init_buf_best(&buf, &ipad));
          netwib_er(netwib_eth_init_ip(&ipad, &ptoolargstore->value.eth));
          break;
        }
      }
      netwib_er(netwib_eth_init_buf(pbuf, &ptoolargstore->value.eth));
      break;
    case NETWOX_TOOLARGTYPE_PORT :
      netwib_er(netwib_port_init_buf(pbuf, &ptoolargstore->value.port));
      break;
    case NETWOX_TOOLARGTYPE_UINT32 :
      netwib_er(netwib_buf_decode_fmt(pbuf, "%{uint32}%$",
                                      &ptoolargstore->value.u32));
      break;
    case NETWOX_TOOLARGTYPE_UINT64 :
      netwib_er(netwib_buf_decode_fmt(pbuf, "%{uint64}%$",
                                      &ptoolargstore->value.u64));
      break;
    case NETWOX_TOOLARGTYPE_INT32 :
      netwib_er(netwib_buf_decode_fmt(pbuf, "%{int32}%$",
                                      &ptoolargstore->value.i32));
      break;
    case NETWOX_TOOLARGTYPE_INT64 :
      netwib_er(netwib_buf_decode_fmt(pbuf, "%{int64}%$",
                                      &ptoolargstore->value.i64));
      break;
    case NETWOX_TOOLARGTYPE_IPS_SU :
    case NETWOX_TOOLARGTYPE_IPS_U :
    case NETWOX_TOOLARGTYPE_IPS_N :
      netwib_er(netwib_ips_add_buf(ptoolargstore->value.pips, pbuf));
      break;
    case NETWOX_TOOLARGTYPE_ETHS_SU :
    case NETWOX_TOOLARGTYPE_ETHS_U :
    case NETWOX_TOOLARGTYPE_ETHS_N :
      netwib_er(netwib_eths_add_buf(ptoolargstore->value.peths, pbuf));
      break;
    case NETWOX_TOOLARGTYPE_PORTS_SU :
    case NETWOX_TOOLARGTYPE_PORTS_U :
    case NETWOX_TOOLARGTYPE_PORTS_N :
      netwib_er(netwib_ports_add_buf(ptoolargstore->value.pports, pbuf));
      break;
    case NETWOX_TOOLARGTYPE_BUF :
      netwib_er(netwib_buf_append_buf(pbuf, &ptoolargstore->value.buf));
      break;
    case NETWOX_TOOLARGTYPE_BUF_MIXED :
      ret = netwib_buf_decode(pbuf, NETWIB_DECODETYPE_MIXED,
                              &ptoolargstore->value.buf);
      if (ret != NETWIB_ERR_OK) {
        netwib_er(netwox_err_display(ret, NETWIB_ERR_ENCODETYPE_TEXT));
        return(ret);
      }
      break;
    case NETWOX_TOOLARGTYPE_ENCODE :
      if (!netwib_c_strcmp(str, "data")) {
        ptoolargstore->value.encode = NETWIB_ENCODETYPE_DATA;
      } else if (!netwib_c_strcmp(str, "hexa")) {
        ptoolargstore->value.encode = NETWIB_ENCODETYPE_HEXA;
      } else if (!netwib_c_strcmp(str, "mixed")) {
        ptoolargstore->value.encode = NETWIB_ENCODETYPE_MIXED;
      } else if (!netwib_c_strcmp(str, "text")) {
        ptoolargstore->value.encode = NETWIB_ENCODETYPE_TEXT;
      } else if (!netwib_c_strcmp(str, "base64")) {
        ptoolargstore->value.encode = NETWIB_ENCODETYPE_BASE64;
      } else if (!netwib_c_strcmp(str, "nothing")) {
        ptoolargstore->value.encode = NETWIB_ENCODETYPE_NOTHING;
      } else if (!netwib_c_strcmp(str, "synth")) {
        ptoolargstore->value.encode = NETWIB_ENCODETYPE_SYNTH;
      } else if (!netwib_c_strcmp(str, "data_wrap")) {
        ptoolargstore->value.encode = NETWIB_ENCODETYPE_DATA_WRAP;
      } else if (!netwib_c_strcmp(str, "hexa_wrap")) {
        ptoolargstore->value.encode = NETWIB_ENCODETYPE_HEXA_WRAP;
      } else if (!netwib_c_strcmp(str, "mixed_wrap")) {
        ptoolargstore->value.encode = NETWIB_ENCODETYPE_MIXED_WRAP;
      } else if (!netwib_c_strcmp(str, "text_wrap")) {
        ptoolargstore->value.encode = NETWIB_ENCODETYPE_TEXT_WRAP;
      } else if (!netwib_c_strcmp(str, "base64_wrap")) {
        ptoolargstore->value.encode = NETWIB_ENCODETYPE_BASE64_WRAP;
      } else if (!netwib_c_strcmp(str, "array")) {
        ptoolargstore->value.encode = NETWIB_ENCODETYPE_ARRAY;
      } else if (!netwib_c_strcmp(str, "dump")) {
        ptoolargstore->value.encode = NETWIB_ENCODETYPE_DUMP;
      } else if (!netwib_c_strcmp(str, "lowercase")) {
        ptoolargstore->value.encode = NETWIB_ENCODETYPE_LOWERCASE;
      } else if (!netwib_c_strcmp(str, "uppercase")) {
        ptoolargstore->value.encode = NETWIB_ENCODETYPE_UPPERCASE;
      } else if (!netwib_c_strcmp(str, "mixedh_wrap")) {
        ptoolargstore->value.encode = NETWIB_ENCODETYPE_MIXEDH_WRAP;
      } else {
        netwib_er(netwib_fmt_display("'%s' is not a valid encodetype\n", str));
        return(NETWOX_ERR_TOOLARG_NOTDECODED);
      }
      break;
    case NETWOX_TOOLARGTYPE_RECORDENCODE :
      if (!netwib_c_strcmp(str, "hexa")) {
        ptoolargstore->value.recordencode = NETWIB_RECORD_ENCODETYPE_HEXA;
      } else if (!netwib_c_strcmp(str, "mixed")) {
        ptoolargstore->value.recordencode = NETWIB_RECORD_ENCODETYPE_MIXED;
      } else if (!netwib_c_strcmp(str, "hexa_wrap")) {
        ptoolargstore->value.recordencode = NETWIB_RECORD_ENCODETYPE_HEXA_WRAP;
      } else if (!netwib_c_strcmp(str, "mixed_wrap")) {
        ptoolargstore->value.recordencode = NETWIB_RECORD_ENCODETYPE_MIXED_WRAP;
      } else if (!netwib_c_strcmp(str, "dump")) {
        ptoolargstore->value.recordencode = NETWIB_RECORD_ENCODETYPE_DUMP;
      } else if (!netwib_c_strcmp(str, "mixedh_wrap")) {
        ptoolargstore->value.recordencode = NETWIB_RECORD_ENCODETYPE_MIXEDH_WRAP;
      } else if (!netwib_c_strcmp(str, "bin")) {
        ptoolargstore->value.recordencode = NETWIB_RECORD_ENCODETYPE_BIN;
      } else if (!netwib_c_strcmp(str, "pcap")) {
        ptoolargstore->value.recordencode = NETWIB_RECORD_ENCODETYPE_PCAP;
      } else {
        netwib_er(netwib_fmt_display("'%s' is not a valid record encodetype\n",
                                   str));
        return(NETWOX_ERR_TOOLARG_NOTDECODED);
      }
      break;
    case NETWOX_TOOLARGTYPE_DECODE :
      if (!netwib_c_strcmp(str, "data")) {
        ptoolargstore->value.decode = NETWIB_DECODETYPE_DATA;
      } else if (!netwib_c_strcmp(str, "hexa")) {
        ptoolargstore->value.decode = NETWIB_DECODETYPE_HEXA;
      } else if (!netwib_c_strcmp(str, "mixed")) {
        ptoolargstore->value.decode = NETWIB_DECODETYPE_MIXED;
      } else if (!netwib_c_strcmp(str, "base64")) {
        ptoolargstore->value.decode = NETWIB_DECODETYPE_BASE64;
      } else {
        netwib_er(netwib_fmt_display("'%s' is not a valid decodetype\n", str));
        return(NETWOX_ERR_TOOLARG_NOTDECODED);
      }
      break;
    case NETWOX_TOOLARGTYPE_DLT :
      if (!netwib_c_strcmp(str, "null")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_NULL;
      } else if (!netwib_c_strcmp(str, "ether")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_ETHER;
      } else if (!netwib_c_strcmp(str, "en3mb")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_EN3MB;
      } else if (!netwib_c_strcmp(str, "ax25")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_AX25;
      } else if (!netwib_c_strcmp(str, "pronet")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_PRONET;
      } else if (!netwib_c_strcmp(str, "chaos")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_CHAOS;
      } else if (!netwib_c_strcmp(str, "ieee802")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_IEEE802;
      } else if (!netwib_c_strcmp(str, "arcnet")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_ARCNET;
      } else if (!netwib_c_strcmp(str, "slip")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_SLIP;
      } else if (!netwib_c_strcmp(str, "ppp")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_PPP;
      } else if (!netwib_c_strcmp(str, "fddi")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_FDDI;
      } else if (!netwib_c_strcmp(str, "atm-rfc1483")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_ATM_RFC1483;
      } else if (!netwib_c_strcmp(str, "raw")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_RAW;
      } else if (!netwib_c_strcmp(str, "slip-bsdos")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_SLIP_BSDOS;
      } else if (!netwib_c_strcmp(str, "ppp-bsdos")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_PPP_BSDOS;
      } else if (!netwib_c_strcmp(str, "atm-clip")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_ATM_CLIP;
      } else if (!netwib_c_strcmp(str, "ppp-serial")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_PPP_SERIAL;
      } else if (!netwib_c_strcmp(str, "ppp-ether")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_PPP_ETHER;
      } else if (!netwib_c_strcmp(str, "c-hdlc")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_C_HDLC;
      } else if (!netwib_c_strcmp(str, "ieee802-11")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_IEEE802_11;
      } else if (!netwib_c_strcmp(str, "loop")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_LOOP;
      } else if (!netwib_c_strcmp(str, "linux-sll")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_LINUX_SLL;
      } else if (!netwib_c_strcmp(str, "ltalk")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_LTALK;
      } else if (!netwib_c_strcmp(str, "econet")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_ECONET;
      } else if (!netwib_c_strcmp(str, "prism-header")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_PRISM_HEADER;
      } else if (!netwib_c_strcmp(str, "aironet-header")) {
        ptoolargstore->value.dlt = NETWIB_DEVICE_DLTTYPE_AIRONET_HEADER;
      } else {
        netwib_er(netwib_fmt_display("'%s' is not a valid dlt\n", str));
        return(NETWOX_ERR_TOOLARG_NOTDECODED);
      }
      break;
    case NETWOX_TOOLARGTYPE_SPOOFIP :
      if (!netwib_c_strcmp(str, "raw")) {
        ptoolargstore->value.spoofip = NETWIB_SPOOF_IP_INITTYPE_RAW;
      } else if (!netwib_c_strcmp(str, "linkf")) {
        ptoolargstore->value.spoofip = NETWIB_SPOOF_IP_INITTYPE_LINKF;
      } else if (!netwib_c_strcmp(str, "linkb")) {
        ptoolargstore->value.spoofip = NETWIB_SPOOF_IP_INITTYPE_LINKB;
      } else if (!netwib_c_strcmp(str, "linkfb")) {
        ptoolargstore->value.spoofip = NETWIB_SPOOF_IP_INITTYPE_LINKFB;
      } else if (!netwib_c_strcmp(str, "rawlinkf")) {
        ptoolargstore->value.spoofip = NETWIB_SPOOF_IP_INITTYPE_RAWLINKF;
      } else if (!netwib_c_strcmp(str, "rawlinkb")) {
        ptoolargstore->value.spoofip = NETWIB_SPOOF_IP_INITTYPE_RAWLINKB;
      } else if (!netwib_c_strcmp(str, "rawlinkfb")) {
        ptoolargstore->value.spoofip = NETWIB_SPOOF_IP_INITTYPE_RAWLINKFB;
      } else if (!netwib_c_strcmp(str, "linkfraw")) {
        ptoolargstore->value.spoofip = NETWIB_SPOOF_IP_INITTYPE_LINKFRAW;
      } else if (!netwib_c_strcmp(str, "linkbraw")) {
        ptoolargstore->value.spoofip = NETWIB_SPOOF_IP_INITTYPE_LINKBRAW;
      } else if (!netwib_c_strcmp(str, "linkfbraw")) {
        ptoolargstore->value.spoofip = NETWIB_SPOOF_IP_INITTYPE_LINKFBRAW;
      } else if (!netwib_c_strcmp(str, "link")) {
        ptoolargstore->value.spoofip = NETWIB_SPOOF_IP_INITTYPE_LINK;
      } else if (!netwib_c_strcmp(str, "rawlink")) {
        ptoolargstore->value.spoofip = NETWIB_SPOOF_IP_INITTYPE_RAWLINK;
      } else if (!netwib_c_strcmp(str, "linkraw")) {
        ptoolargstore->value.spoofip = NETWIB_SPOOF_IP_INITTYPE_LINKRAW;
      } else if (!netwib_c_strcmp(str, "best")) {
        ptoolargstore->value.spoofip = NETWIB_SPOOF_IP_INITTYPE_BEST;
      } else {
        netwib_er(netwib_fmt_display("'%s' is not a valid spoof_ip_inittype\n",
                                     str));
        return(NETWOX_ERR_TOOLARG_NOTDECODED);
      }
      break;
    case NETWOX_TOOLARGTYPE_IPTYPE :
      if (!netwib_c_strcmp(str, "ip4")) {
        ptoolargstore->value.iptype = NETWIB_IPTYPE_IP4;
      } else if (!netwib_c_strcmp(str, "ip6")) {
        ptoolargstore->value.iptype = NETWIB_IPTYPE_IP6;
      } else {
        netwib_er(netwib_fmt_display("'%s' is not a valid iptype\n", str));
        return(NETWOX_ERR_TOOLARG_NOTDECODED);
      }
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_toolargstore_set_string(netwib_conststring value,
                                          netwox_toolargstore *ptoolargstore)
{
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string(value, &buf));
  ret = netwox_toolargstore_set(&buf, ptoolargstore);

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_toolargstore_set_kbd(const netwox_toolarg *ptoolarg,
                                       netwox_toolargstore *ptoolargstore)
{
  netwib_buf message, allowedchars, defaultvalue, buf;
  netwib_string pc;
  netwib_eth defaulteth;
  netwib_port defaultport;
  netwib_char c;
  netwib_uint32 defaultuint32;
  netwib_uint64 defaultuint64;
  netwib_int32 defaultint32;
  netwib_int64 defaultint64;
  netwib_err ret;

  netwib_er(netwib_buf_init_mallocdefault(&message));
  netwib_er(netwib_buf_init_mallocdefault(&defaultvalue));

  netwib_er(netwox_buf_append_toolarg(ptoolarg, NETWOX_TOOLARG_ENCODETYPE_KBD2,
                                      &message));
  netwib_er(netwox_toolarg_get_default(ptoolarg, &defaultvalue));

  switch (ptoolarg->toolargtype) {
    case NETWOX_TOOLARGTYPE_BOOL :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO1 :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO2 :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO3 :
      netwib_er(netwib_buf_init_ext_string("01", &allowedchars));
      netwib_er(netwib_buf_ref_string(&defaultvalue, &pc));
      netwib_er(netwib_char_init_kbd(&message, &allowedchars, pc[0], &c));
      if (c == '1') {
        ptoolargstore->value.b = NETWIB_TRUE;
      } else {
        ptoolargstore->value.b = NETWIB_FALSE;
      }
      break;
    case NETWOX_TOOLARGTYPE_IP :
    case NETWOX_TOOLARGTYPE_IP_SRC :
    case NETWOX_TOOLARGTYPE_IP_DST :
      netwib_er(netwib_ip_init_kbd(&message, &defaultvalue,
                                   &ptoolargstore->value.ip));
      break;
    case NETWOX_TOOLARGTYPE_IP4 :
    case NETWOX_TOOLARGTYPE_IP4_SRC :
    case NETWOX_TOOLARGTYPE_IP4_DST :
      while (NETWIB_TRUE) {
        netwib_er(netwib_ip_init_kbd(&message, &defaultvalue,
                                     &ptoolargstore->value.ip));
        if (ptoolargstore->value.ip.iptype == NETWIB_IPTYPE_IP4) {
          break;
        }
        netwib_er(netwox_err_display(NETWIB_ERR_PAIPTYPENOT4,
                                     NETWIB_ERR_ENCODETYPE_TEXT));
      }
      break;
    case NETWOX_TOOLARGTYPE_IP6 :
    case NETWOX_TOOLARGTYPE_IP6_SRC :
    case NETWOX_TOOLARGTYPE_IP6_DST :
      while (NETWIB_TRUE) {
        netwib_er(netwib_ip_init_kbd(&message, &defaultvalue,
                                     &ptoolargstore->value.ip));
        if (ptoolargstore->value.ip.iptype == NETWIB_IPTYPE_IP6) {
          break;
        }
        netwib_er(netwox_err_display(NETWIB_ERR_PAIPTYPENOT6,
                                     NETWIB_ERR_ENCODETYPE_TEXT));
      }
      break;
    case NETWOX_TOOLARGTYPE_ETH :
    case NETWOX_TOOLARGTYPE_ETH_SRC :
    case NETWOX_TOOLARGTYPE_ETH_DST :
      netwib_er(netwib_eth_init_buf(&defaultvalue, &defaulteth));
      netwib_er(netwib_eth_init_kbd(&message, &defaulteth,
                                    &ptoolargstore->value.eth));
      break;
    case NETWOX_TOOLARGTYPE_PORT :
    case NETWOX_TOOLARGTYPE_PORT_SRC :
    case NETWOX_TOOLARGTYPE_PORT_DST :
      netwib_er(netwib_port_init_buf(&defaultvalue, &defaultport));
      netwib_er(netwib_port_init_kbd(&message,
                                     NETWIB_PORT_INIT_KBD_NOMIN,
                                     NETWIB_PORT_INIT_KBD_NOMAX,
                                     defaultport, &ptoolargstore->value.port));
      break;
    case NETWOX_TOOLARGTYPE_IPS_SU :
    case NETWOX_TOOLARGTYPE_IPS_U :
    case NETWOX_TOOLARGTYPE_IPS_N :
      netwib_er(netwib_ips_add_kbd(ptoolargstore->value.pips, &message,
                                   &defaultvalue));
      break;
    case NETWOX_TOOLARGTYPE_ETHS_SU :
    case NETWOX_TOOLARGTYPE_ETHS_U :
    case NETWOX_TOOLARGTYPE_ETHS_N :
      netwib_er(netwib_eths_add_kbd(ptoolargstore->value.peths, &message,
                                    &defaultvalue));
      break;
    case NETWOX_TOOLARGTYPE_PORTS_SU :
    case NETWOX_TOOLARGTYPE_PORTS_U :
    case NETWOX_TOOLARGTYPE_PORTS_N :
      netwib_er(netwib_ports_add_kbd(ptoolargstore->value.pports, &message,
                                     &defaultvalue));
      break;
    case NETWOX_TOOLARGTYPE_UINT32 :
      netwib_er(netwib_buf_decode_fmt(&defaultvalue, "%{uint32}%$",
                                      &defaultuint32));
      netwib_er(netwib_uint32_init_kbd(&message,
                                       NETWIB_UINT32_INIT_KBD_NOMIN,
                                       NETWIB_UINT32_INIT_KBD_NOMAX,
                                       defaultuint32,
                                       &ptoolargstore->value.u32));
      break;
    case NETWOX_TOOLARGTYPE_UINT64 :
      netwib_er(netwib_buf_decode_fmt(&defaultvalue, "%{uint64}%$",
                                      &defaultuint64));
      netwib_er(netwib_uint64_init_kbd(&message, defaultuint64,
                                       &ptoolargstore->value.u64));
      break;
    case NETWOX_TOOLARGTYPE_INT32 :
      netwib_er(netwib_buf_decode_fmt(&defaultvalue, "%{int32}%$",
                                      &defaultint32));
      netwib_er(netwib_int32_init_kbd(&message,
                                      NETWIB_INT32_INIT_KBD_NOMIN,
                                      NETWIB_INT32_INIT_KBD_NOMAX,
                                      defaultint32,
                                      &ptoolargstore->value.i32));
      break;
    case NETWOX_TOOLARGTYPE_INT64 :
      netwib_er(netwib_buf_decode_fmt(&defaultvalue, "%{int64}%$",
                                      &defaultint64));
      netwib_er(netwib_int64_init_kbd(&message, defaultint64,
                                      &ptoolargstore->value.i64));
      break;
    case NETWOX_TOOLARGTYPE_BUF_DEVICE :
      netwib_er(netwib_device_init_kbd(&message, &defaultvalue,
                                       &ptoolargstore->value.buf));
      break;
    case NETWOX_TOOLARGTYPE_BUF :
    case NETWOX_TOOLARGTYPE_BUF_LOGIN :
    case NETWOX_TOOLARGTYPE_BUF_FILE :
    case NETWOX_TOOLARGTYPE_BUF_FILE_RD :
    case NETWOX_TOOLARGTYPE_BUF_FILE_WR :
    case NETWOX_TOOLARGTYPE_BUF_DIR :
    case NETWOX_TOOLARGTYPE_BUF_DIR_RD :
    case NETWOX_TOOLARGTYPE_BUF_DIR_WR :
    case NETWOX_TOOLARGTYPE_BUF_FILTER :
    case NETWOX_TOOLARGTYPE_BUF_OID :
    case NETWOX_TOOLARGTYPE_BUF_OIDTYPE :
    case NETWOX_TOOLARGTYPE_BUF_EMAIL :
    case NETWOX_TOOLARGTYPE_BUF_URI :
    case NETWOX_TOOLARGTYPE_BUF_NEWSGROUP :
    case NETWOX_TOOLARGTYPE_BUF_HOSTNAME :
    case NETWOX_TOOLARGTYPE_BUF_MD5 :
    case NETWOX_TOOLARGTYPE_BUF_USERAGENT :
    case NETWOX_TOOLARGTYPE_BUF_NETBIOSNAME :
    case NETWOX_TOOLARGTYPE_BUF_SMBSHARE :
      netwib_er(netwib_buf_append_kbd(&message, &defaultvalue,
                                      &ptoolargstore->value.buf));
      break;
    case NETWOX_TOOLARGTYPE_BUF_PASSWORD :
      netwib_er(netwib_buf_append_password(&message, &defaultvalue,
                                           &ptoolargstore->value.buf));
      break;
    case NETWOX_TOOLARGTYPE_BUF_MIXED :
    case NETWOX_TOOLARGTYPE_BUF_IP4OPTS :
    case NETWOX_TOOLARGTYPE_BUF_IP6EXTS :
    case NETWOX_TOOLARGTYPE_BUF_TCPOPTS :
      netwib_er(netwib_buf_init_mallocdefault(&buf));
      while (NETWIB_TRUE) {
        netwib_er(netwib_buf_append_kbd(&message, &defaultvalue,
                                        &buf));
        ret = netwib_buf_decode(&buf, NETWIB_DECODETYPE_MIXED,
                                &ptoolargstore->value.buf);
        if (ret == NETWIB_ERR_OK) {
          break;
        } else {
          netwib_er(netwox_err_display(ret, NETWIB_ERR_ENCODETYPE_TEXT));
          netwib__buf_reinit(&buf);
        }
      }
      netwib_er(netwib_buf_close(&buf));
      break;
    case NETWOX_TOOLARGTYPE_ENCODE :
      netwib_er(netwib_encodetype_init_kbd(&message, NETWIB_TRUE,
                                           NETWIB_ENCODETYPE_INIT_KBD_NODEF,
                                           &ptoolargstore->value.encode));
      break;
    case NETWOX_TOOLARGTYPE_RECORDENCODE :
      netwib_er(netwib_record_encodetype_init_kbd(&message, NETWIB_TRUE,
                                  NETWIB_RECORD_ENCODETYPE_INIT_KBD_NODEF,
                                  &ptoolargstore->value.recordencode));
      break;
    case NETWOX_TOOLARGTYPE_DECODE :
      netwib_er(netwib_decodetype_init_kbd(&message,
                                           NETWIB_DECODETYPE_INIT_KBD_NODEF,
                                           &ptoolargstore->value.decode));
      break;
    case NETWOX_TOOLARGTYPE_DLT :
      netwib_er(netwib_device_dlttype_init_kbd(&message,
                                           NETWIB_DEVICE_DLTTYPE_INIT_KBD_NODEF,
                                               &ptoolargstore->value.dlt));
      break;
    case NETWOX_TOOLARGTYPE_SPOOFIP :
      netwib_er(netwib_spoof_ip_inittype_init_kbd(&message,
                                       NETWIB_SPOOF_IP_INITTYPE_INIT_KBD_NODEF,
                                             &ptoolargstore->value.spoofip));
      break;
    case NETWOX_TOOLARGTYPE_IPTYPE :
      netwib_er(netwib_iptype_init_kbd(&message,
                                       NETWIB_IPTYPE_INIT_KBD_NODEF,
                                       &ptoolargstore->value.iptype));
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  netwib_er(netwib_buf_close(&defaultvalue));
  netwib_er(netwib_buf_close(&message));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_buf_append_toolargstore(const netwox_toolargstore *ptoolargstore,
                                          netwib_buf *pbuf)
{
  switch (ptoolargstore->type) {
    case NETWOX_TOOLARGTYPE_BOOL :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO1 :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO2 :
    case NETWOX_TOOLARGTYPE_BOOL_RADIO3 :
      netwib_er(netwib_buf_append_fmt(pbuf, "%{bool:0}", ptoolargstore->value.b));
      break;
    case NETWOX_TOOLARGTYPE_IP :
    case NETWOX_TOOLARGTYPE_IP4 :
    case NETWOX_TOOLARGTYPE_IP6 :
      netwib_er(netwib_buf_append_ip(&ptoolargstore->value.ip,
                                     NETWIB_IP_ENCODETYPE_IP, pbuf));
      break;
    case NETWOX_TOOLARGTYPE_ETH :
      netwib_er(netwib_buf_append_eth(&ptoolargstore->value.eth, pbuf));
      break;
    case NETWOX_TOOLARGTYPE_PORT :
      netwib_er(netwib_buf_append_port(ptoolargstore->value.port, pbuf));
      break;
    case NETWOX_TOOLARGTYPE_IPS_SU :
    case NETWOX_TOOLARGTYPE_IPS_U :
    case NETWOX_TOOLARGTYPE_IPS_N :
      netwib_er(netwib_buf_append_ips(ptoolargstore->value.pips,
                                      NETWIB_IPS_ENCODETYPE_IPBEST, pbuf));
      break;
    case NETWOX_TOOLARGTYPE_ETHS_SU :
    case NETWOX_TOOLARGTYPE_ETHS_U :
    case NETWOX_TOOLARGTYPE_ETHS_N :
      netwib_er(netwib_buf_append_eths(ptoolargstore->value.peths, pbuf));
      break;
    case NETWOX_TOOLARGTYPE_PORTS_SU :
    case NETWOX_TOOLARGTYPE_PORTS_U :
    case NETWOX_TOOLARGTYPE_PORTS_N :
      netwib_er(netwib_buf_append_ports(ptoolargstore->value.pports, pbuf));
      break;
    case NETWOX_TOOLARGTYPE_UINT32 :
      netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32}",
                                      ptoolargstore->value.u32));
      break;
    case NETWOX_TOOLARGTYPE_UINT64 :
      netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32}",
                                      ptoolargstore->value.u64));
      break;
    case NETWOX_TOOLARGTYPE_INT32 :
      netwib_er(netwib_buf_append_fmt(pbuf, "%{int32}",
                                      ptoolargstore->value.i32));
      break;
    case NETWOX_TOOLARGTYPE_INT64 :
      netwib_er(netwib_buf_append_fmt(pbuf, "%{int32}",
                                      ptoolargstore->value.i64));
      break;
    case NETWOX_TOOLARGTYPE_BUF :
      netwib_er(netwib_buf_append_buf(&ptoolargstore->value.buf, pbuf));
      break;
    case NETWOX_TOOLARGTYPE_BUF_MIXED :
      netwib_er(netwib_buf_encode(&ptoolargstore->value.buf,
                                  NETWIB_ENCODETYPE_MIXED, pbuf));
      break;
    case NETWOX_TOOLARGTYPE_ENCODE :
      switch(ptoolargstore->value.encode) {
      case NETWIB_ENCODETYPE_DATA :
        netwib_er(netwib_buf_append_string("data", pbuf));
        break;
      case NETWIB_ENCODETYPE_HEXA :
        netwib_er(netwib_buf_append_string("hexa", pbuf));
        break;
      case NETWIB_ENCODETYPE_MIXED :
        netwib_er(netwib_buf_append_string("mixed", pbuf));
        break;
      case NETWIB_ENCODETYPE_TEXT :
        netwib_er(netwib_buf_append_string("text", pbuf));
        break;
      case NETWIB_ENCODETYPE_BASE64 :
        netwib_er(netwib_buf_append_string("base64", pbuf));
        break;
      case NETWIB_ENCODETYPE_NOTHING :
        netwib_er(netwib_buf_append_string("nothing", pbuf));
        break;
      case NETWIB_ENCODETYPE_SYNTH :
        netwib_er(netwib_buf_append_string("synth", pbuf));
        break;
      case NETWIB_ENCODETYPE_DATA_WRAP :
        netwib_er(netwib_buf_append_string("data_wrap", pbuf));
        break;
      case NETWIB_ENCODETYPE_HEXA_WRAP :
        netwib_er(netwib_buf_append_string("hexa_wrap", pbuf));
        break;
      case NETWIB_ENCODETYPE_MIXED_WRAP :
        netwib_er(netwib_buf_append_string("mixed_wrap", pbuf));
        break;
      case NETWIB_ENCODETYPE_TEXT_WRAP :
        netwib_er(netwib_buf_append_string("text_wrap", pbuf));
        break;
      case NETWIB_ENCODETYPE_BASE64_WRAP :
        netwib_er(netwib_buf_append_string("base64_wrap", pbuf));
        break;
      case NETWIB_ENCODETYPE_ARRAY :
        netwib_er(netwib_buf_append_string("array", pbuf));
        break;
      case NETWIB_ENCODETYPE_DUMP :
        netwib_er(netwib_buf_append_string("dump", pbuf));
        break;
      case NETWIB_ENCODETYPE_LOWERCASE :
        netwib_er(netwib_buf_append_string("lowercase", pbuf));
        break;
      case NETWIB_ENCODETYPE_UPPERCASE :
        netwib_er(netwib_buf_append_string("uppercase", pbuf));
        break;
      case NETWIB_ENCODETYPE_MIXEDH_WRAP :
        netwib_er(netwib_buf_append_string("mixedh_wrap", pbuf));
        break;
      default :
        netwib_er(netwib_buf_append_encodetype(ptoolargstore->value.encode,
                                               pbuf));
        break;
      }
      break;
    case NETWOX_TOOLARGTYPE_RECORDENCODE :
      switch(ptoolargstore->value.recordencode) {
      case NETWIB_RECORD_ENCODETYPE_HEXA :
        netwib_er(netwib_buf_append_string("hexa", pbuf));
        break;
      case NETWIB_RECORD_ENCODETYPE_MIXED :
        netwib_er(netwib_buf_append_string("mixed", pbuf));
        break;
      case NETWIB_RECORD_ENCODETYPE_HEXA_WRAP :
        netwib_er(netwib_buf_append_string("hexa_wrap", pbuf));
        break;
      case NETWIB_RECORD_ENCODETYPE_MIXED_WRAP :
        netwib_er(netwib_buf_append_string("mixed_wrap", pbuf));
        break;
      case NETWIB_RECORD_ENCODETYPE_DUMP :
        netwib_er(netwib_buf_append_string("dump", pbuf));
        break;
      case NETWIB_RECORD_ENCODETYPE_MIXEDH_WRAP :
        netwib_er(netwib_buf_append_string("mixedh_wrap", pbuf));
        break;
      case NETWIB_RECORD_ENCODETYPE_BIN :
        netwib_er(netwib_buf_append_string("bin", pbuf));
        break;
      case NETWIB_RECORD_ENCODETYPE_PCAP :
        netwib_er(netwib_buf_append_string("pcap", pbuf));
        break;
      default :
        netwib_er(netwib_buf_append_record_encodetype(ptoolargstore->value.recordencode, pbuf));
        break;
      }
      break;
    case NETWOX_TOOLARGTYPE_DECODE :
      switch(ptoolargstore->value.decode) {
      case NETWIB_DECODETYPE_DATA :
        netwib_er(netwib_buf_append_string("data", pbuf));
        break;
      case NETWIB_DECODETYPE_HEXA :
        netwib_er(netwib_buf_append_string("hexa", pbuf));
        break;
      case NETWIB_DECODETYPE_MIXED :
        netwib_er(netwib_buf_append_string("mixed", pbuf));
        break;
      case NETWIB_DECODETYPE_BASE64 :
        netwib_er(netwib_buf_append_string("base64", pbuf));
        break;
      default :
        netwib_er(netwib_buf_append_decodetype(ptoolargstore->value.decode,
                                               pbuf));
        break;
      }
      break;
    case NETWOX_TOOLARGTYPE_DLT :
      switch(ptoolargstore->value.dlt) {
      case NETWIB_DEVICE_DLTTYPE_NULL :
        netwib_er(netwib_buf_append_string("null", pbuf));
        break;
      case NETWIB_DEVICE_DLTTYPE_ETHER :
        netwib_er(netwib_buf_append_string("ether", pbuf));
        break;
      case NETWIB_DEVICE_DLTTYPE_EN3MB :
        netwib_er(netwib_buf_append_string("en3mb", pbuf));
        break;
      case NETWIB_DEVICE_DLTTYPE_AX25 :
        netwib_er(netwib_buf_append_string("ax25", pbuf));
        break;
      case NETWIB_DEVICE_DLTTYPE_PRONET :
        netwib_er(netwib_buf_append_string("pronet", pbuf));
        break;
      case NETWIB_DEVICE_DLTTYPE_CHAOS :
        netwib_er(netwib_buf_append_string("chaos", pbuf));
        break;
      case NETWIB_DEVICE_DLTTYPE_IEEE802 :
        netwib_er(netwib_buf_append_string("ieee802", pbuf));
        break;
      case NETWIB_DEVICE_DLTTYPE_ARCNET :
        netwib_er(netwib_buf_append_string("arcnet", pbuf));
        break;
      case NETWIB_DEVICE_DLTTYPE_SLIP :
        netwib_er(netwib_buf_append_string("slip", pbuf));
        break;
      case NETWIB_DEVICE_DLTTYPE_PPP :
        netwib_er(netwib_buf_append_string("ppp", pbuf));
        break;
      case NETWIB_DEVICE_DLTTYPE_FDDI :
        netwib_er(netwib_buf_append_string("fddi", pbuf));
        break;
      case NETWIB_DEVICE_DLTTYPE_ATM_RFC1483 :
        netwib_er(netwib_buf_append_string("atm-rfc1483", pbuf));
        break;
      case NETWIB_DEVICE_DLTTYPE_RAW :
        netwib_er(netwib_buf_append_string("raw", pbuf));
        break;
      case NETWIB_DEVICE_DLTTYPE_SLIP_BSDOS :
        netwib_er(netwib_buf_append_string("slip-bsdos", pbuf));
        break;
      case NETWIB_DEVICE_DLTTYPE_PPP_BSDOS :
        netwib_er(netwib_buf_append_string("ppp-bsdos", pbuf));
        break;
      case NETWIB_DEVICE_DLTTYPE_ATM_CLIP :
        netwib_er(netwib_buf_append_string("atm-clip", pbuf));
        break;
      case NETWIB_DEVICE_DLTTYPE_PPP_SERIAL :
        netwib_er(netwib_buf_append_string("ppp-serial", pbuf));
        break;
      case NETWIB_DEVICE_DLTTYPE_PPP_ETHER :
        netwib_er(netwib_buf_append_string("ppp-ether", pbuf));
        break;
      case NETWIB_DEVICE_DLTTYPE_C_HDLC :
        netwib_er(netwib_buf_append_string("c-hdlc", pbuf));
        break;
      case NETWIB_DEVICE_DLTTYPE_IEEE802_11 :
        netwib_er(netwib_buf_append_string("ieee802-11", pbuf));
        break;
      case NETWIB_DEVICE_DLTTYPE_LOOP :
        netwib_er(netwib_buf_append_string("loop", pbuf));
        break;
      case NETWIB_DEVICE_DLTTYPE_LINUX_SLL :
        netwib_er(netwib_buf_append_string("linux-sll", pbuf));
        break;
      case NETWIB_DEVICE_DLTTYPE_LTALK :
        netwib_er(netwib_buf_append_string("ltalk", pbuf));
        break;
      case NETWIB_DEVICE_DLTTYPE_ECONET :
        netwib_er(netwib_buf_append_string("econet", pbuf));
        break;
      case NETWIB_DEVICE_DLTTYPE_PRISM_HEADER :
        netwib_er(netwib_buf_append_string("prism-header", pbuf));
        break;
      case NETWIB_DEVICE_DLTTYPE_AIRONET_HEADER :
        netwib_er(netwib_buf_append_string("aironet-header", pbuf));
        break;
      default :
        netwib_er(netwib_buf_append_device_dlttype(ptoolargstore->value.dlt,
                                                   pbuf));
        break;
      }
      break;
    case NETWOX_TOOLARGTYPE_SPOOFIP :
      switch(ptoolargstore->value.spoofip) {
      case NETWIB_SPOOF_IP_INITTYPE_RAW :
        netwib_er(netwib_buf_append_string("raw", pbuf));
        break;
      case NETWIB_SPOOF_IP_INITTYPE_LINKF :
        netwib_er(netwib_buf_append_string("linkf", pbuf));
        break;
      case NETWIB_SPOOF_IP_INITTYPE_LINKB :
        netwib_er(netwib_buf_append_string("linkb", pbuf));
        break;
      case NETWIB_SPOOF_IP_INITTYPE_LINKFB :
        netwib_er(netwib_buf_append_string("linkfb", pbuf));
        break;
      case NETWIB_SPOOF_IP_INITTYPE_RAWLINKF :
        netwib_er(netwib_buf_append_string("rawlinkf", pbuf));
        break;
      case NETWIB_SPOOF_IP_INITTYPE_RAWLINKB :
        netwib_er(netwib_buf_append_string("rawlinkb", pbuf));
        break;
      case NETWIB_SPOOF_IP_INITTYPE_RAWLINKFB :
        netwib_er(netwib_buf_append_string("rawlinkfb", pbuf));
        break;
      case NETWIB_SPOOF_IP_INITTYPE_LINKFRAW :
        netwib_er(netwib_buf_append_string("linkfraw", pbuf));
        break;
      case NETWIB_SPOOF_IP_INITTYPE_LINKBRAW :
        netwib_er(netwib_buf_append_string("linkbraw", pbuf));
        break;
      case NETWIB_SPOOF_IP_INITTYPE_LINKFBRAW :
        netwib_er(netwib_buf_append_string("linkfbraw", pbuf));
        break;
      default :
        netwib_er(netwib_buf_append_spoof_ip_inittype(ptoolargstore->value.spoofip, pbuf));
        break;
      }
      break;
    case NETWOX_TOOLARGTYPE_IPTYPE :
      switch(ptoolargstore->value.iptype) {
      case NETWIB_IPTYPE_IP4 :
        netwib_er(netwib_buf_append_string("ip4", pbuf));
        break;
      case NETWIB_IPTYPE_IP6 :
        netwib_er(netwib_buf_append_string("ip6", pbuf));
        break;
      default :
        break;
      }
      netwib_er(netwib_buf_append_iptype(ptoolargstore->value.iptype, pbuf));
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_toolargstore_display(const netwox_toolargstore *ptoolargstore)
{
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  ret = netwox_buf_append_toolargstore(ptoolargstore, &buf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  }
  netwib_er(netwib_buf_close(&buf));

  return(ret);
}
