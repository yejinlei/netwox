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
netwib_err netwox_dhcp4hdr_initdefault(netwox_dhcp4hdr *pdhcp4hdr)
{
  pdhcp4hdr->dhcp4opcode = NETWOX_DHCP4OPCODE_UNKNOWN;
  pdhcp4hdr->hops = 0;
  netwib_er(netwib_uint32_init_rand(0, 0xFFFFFFFFu, &pdhcp4hdr->xid));
  pdhcp4hdr->secs = 0;
  pdhcp4hdr->flags = 0;
  netwib_er(netwib_ip_init_ip4(0, &pdhcp4hdr->clientip));
  netwib_er(netwib_ip_init_ip4(0, &pdhcp4hdr->yourip));
  netwib_er(netwib_ip_init_ip4(0, &pdhcp4hdr->serverip));
  netwib_er(netwib_ip_init_ip4(0, &pdhcp4hdr->agentip));
  netwib_er(netwib_eth_init_fields(0,0,0,0,0,0, &pdhcp4hdr->clienteth));
  netwib_er(netwib_buf_init_ext_empty(&pdhcp4hdr->sname));
  netwib_er(netwib_buf_init_ext_empty(&pdhcp4hdr->file));

  netwib_er(netwib_buf_init_ext_empty(&pdhcp4hdr->opts));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_dhcp4hdr(netwox_constdhcp4hdr *pdhcp4hdr,
                                      netwib_buf *ppkt)
{
  netwib_data data;
  netwib_uint32 bufsize;

  netwib_er(netwib_buf_wantspace(ppkt, 236, &data));

  netwib__data_append_uint8(data, pdhcp4hdr->dhcp4opcode);
  netwib__data_append_uint8(data, 1);
  netwib__data_append_uint8(data, 6);
  netwib__data_append_uint8(data, pdhcp4hdr->hops);

  netwib__data_append_uint32(data, pdhcp4hdr->xid);

  netwib__data_append_uint16(data, pdhcp4hdr->secs);

  netwib__data_append_uint16(data, pdhcp4hdr->flags);

  if (pdhcp4hdr->clientip.iptype != NETWIB_IPTYPE_IP4) {
    return(NETWIB_ERR_PAIPTYPENOT4);
  }
  netwib__data_append_uint32(data, pdhcp4hdr->clientip.ipvalue.ip4);

  if (pdhcp4hdr->yourip.iptype != NETWIB_IPTYPE_IP4) {
    return(NETWIB_ERR_PAIPTYPENOT4);
  }
  netwib__data_append_uint32(data, pdhcp4hdr->yourip.ipvalue.ip4);

  if (pdhcp4hdr->serverip.iptype != NETWIB_IPTYPE_IP4) {
    return(NETWIB_ERR_PAIPTYPENOT4);
  }
  netwib__data_append_uint32(data, pdhcp4hdr->serverip.ipvalue.ip4);

  if (pdhcp4hdr->agentip.iptype != NETWIB_IPTYPE_IP4) {
    return(NETWIB_ERR_PAIPTYPENOT4);
  }
  netwib__data_append_uint32(data, pdhcp4hdr->agentip.ipvalue.ip4);

  netwib_c_memcpy(data, pdhcp4hdr->clienteth.b, NETWIB_ETH_LEN);
  netwib_c_memset(data+NETWIB_ETH_LEN, 0, 16-NETWIB_ETH_LEN);
  data += 16;

  bufsize = netwib__buf_ref_data_size(&pdhcp4hdr->sname);
  if (bufsize > 63) {
    return(NETWIB_ERR_PATOOHIGH);
  }
  netwib_c_memcpy(data, netwib__buf_ref_data_ptr(&pdhcp4hdr->sname), bufsize);
  netwib_c_memset(data+bufsize, 0, 64-bufsize);
  data += 64;
  bufsize = netwib__buf_ref_data_size(&pdhcp4hdr->file);
  if (bufsize > 127) {
    return(NETWIB_ERR_PATOOHIGH);
  }
  netwib_c_memcpy(data, netwib__buf_ref_data_ptr(&pdhcp4hdr->file), bufsize);
  netwib_c_memset(data+bufsize, 0, 128-bufsize);
  data += 128;

  ppkt->endoffset += 236;

  /* magic cookie */
  netwib__data_append_uint8(data, 99);
  netwib__data_append_uint8(data, 130);
  netwib__data_append_uint8(data, 83);
  netwib__data_append_uint8(data, 99);
  ppkt->endoffset += 4;

  /* options */
  netwib_er(netwib_buf_append_buf(&pdhcp4hdr->opts, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_decode_dhcp4hdr(netwib_constbuf *ppkt,
                                      netwox_dhcp4hdr *pdhcp4hdr)
{
  netwib_data data;
  netwib_uint32 datasize, i;

  datasize = netwib__buf_ref_data_size(ppkt);
  if (datasize < 236) {
    return(NETWIB_ERR_DATAMISSING);
  }
  data = netwib__buf_ref_data_ptr(ppkt);
  if (data[1] != 1 || data[2] != 6) {
    /* only support Ethernet */
    return(NETWIB_ERR_NOTCONVERTED);
  }

  pdhcp4hdr->dhcp4opcode = data[0];
  pdhcp4hdr->hops = data[3];
  pdhcp4hdr->xid = netwib_c2_uint32_4(data[4], data[5], data[6], data[7]);
  pdhcp4hdr->secs = netwib_c2_uint16_2(data[8], data[9]);
  pdhcp4hdr->flags = netwib_c2_uint16_2(data[10], data[11]);

  pdhcp4hdr->clientip.iptype = NETWIB_IPTYPE_IP4;
  pdhcp4hdr->clientip.ipvalue.ip4 = netwib_c2_uint32_4(data[12], data[13],
                                                     data[14], data[15]);
  pdhcp4hdr->yourip.iptype = NETWIB_IPTYPE_IP4;
  pdhcp4hdr->yourip.ipvalue.ip4 = netwib_c2_uint32_4(data[16], data[17],
                                                     data[18], data[19]);
  pdhcp4hdr->serverip.iptype = NETWIB_IPTYPE_IP4;
  pdhcp4hdr->serverip.ipvalue.ip4 = netwib_c2_uint32_4(data[20], data[21],
                                                     data[22], data[23]);
  pdhcp4hdr->agentip.iptype = NETWIB_IPTYPE_IP4;
  pdhcp4hdr->agentip.ipvalue.ip4 = netwib_c2_uint32_4(data[24], data[25],
                                                     data[26], data[27]);

  netwib_c_memcpy(pdhcp4hdr->clienteth.b, data+28, NETWIB_ETH_LEN);
  data += 44;
  datasize -= 44;

  i = 0;
  while (i < 64) {
    if (data[i] == '\0') {
      if (i == 0) {
        netwib_er(netwib_buf_init_ext_empty(&pdhcp4hdr->sname));
      } else {
        netwib_er(netwib_buf_init_ext_array(data, i+1, 0, i, &pdhcp4hdr->sname));
      }
      break;
    }
    i++;
  }
  if (i == 64) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  data += 64;
  datasize -= 64;

  i = 0;
  while (i < 128) {
    if (data[i] == '\0') {
      if (i == 0) {
        netwib_er(netwib_buf_init_ext_empty(&pdhcp4hdr->file));
      } else {
        netwib_er(netwib_buf_init_ext_array(data, i+1, 0, i, &pdhcp4hdr->file));
      }
      break;
    }
    i++;
  }
  if (i == 128) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  data += 128;
  datasize -= 128;

  /* if opts start with magic cookie, remove it */
  if (datasize >= 4) {
    if (data[0] == 99 && data[1] == 130 && data[2] == 83 && data[3] == 99) {
      data += 4;
      datasize -= 4;
    }
  }
  if (datasize == 0) {
    netwib_er(netwib_buf_init_ext_empty(&pdhcp4hdr->opts));
  } else {
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize,
                                              &pdhcp4hdr->opts));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dhcp4hdr_show(netwox_constdhcp4hdr *pdhcp4hdr,
                                netwib_encodetype encodetype,
                                netwib_buf *pbuf)
{
  netwib_byte array[80];
  netwib_buf bufarray;

  if (encodetype == NETWIB_ENCODETYPE_SYNTH) {
    if (pdhcp4hdr->dhcp4opcode == NETWOX_DHCP4OPCODE_BOOTREQUEST) {
      netwib_er(netwib_buf_append_string("DHCPrequest", pbuf));
    } else if (pdhcp4hdr->dhcp4opcode == NETWOX_DHCP4OPCODE_BOOTREPLY) {
      netwib_er(netwib_buf_append_string("DHCPreply", pbuf));
    } else {
      netwib_er(netwib_buf_append_string("DHCP", pbuf));
    }
    return(NETWIB_ERR_OK);
  } else if (encodetype != NETWIB_ENCODETYPE_ARRAY) {
    return(NETWOX_ERR_NOTIMPLEMENTED);
  }

  netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &bufarray));

  netwib_er(netwib_show_array_head("DHCP", pbuf));

  netwib_er(netwib_buf_append_string("op=", &bufarray));
  if (pdhcp4hdr->dhcp4opcode == NETWOX_DHCP4OPCODE_BOOTREQUEST) {
    netwib_er(netwib_buf_append_string("request", &bufarray));
  } else if (pdhcp4hdr->dhcp4opcode == NETWOX_DHCP4OPCODE_BOOTREPLY) {
    netwib_er(netwib_buf_append_string("reply ", &bufarray));
  } else {
    netwib_er(netwib_buf_append_string("?     ", &bufarray));
  }
  netwib_er(netwib_buf_append_fmt(&bufarray,
                                  "  hops=%{l 3;uint8}  xid=%{uint32:08X}",
                                  pdhcp4hdr->hops, pdhcp4hdr->xid));
  netwib_er(netwib_buf_append_fmt(&bufarray,
                                  "  secs=%{l 5;uint16}  flags=%{uint16:04X}",
                                  pdhcp4hdr->secs, pdhcp4hdr->flags));
  netwib_er(netwib_show_array_fmt32(pbuf, " %{buf}", &bufarray));

  netwib__buf_reinit(&bufarray);
  netwib_er(netwib_buf_append_fmt(&bufarray,
                                  "client=%{ip}  your=%{ip}",
                                  &pdhcp4hdr->clientip, &pdhcp4hdr->yourip));
  netwib_er(netwib_show_array_fmt32(pbuf, " %{buf}", &bufarray));

  netwib__buf_reinit(&bufarray);
  netwib_er(netwib_buf_append_fmt(&bufarray,
                                  "server=%{ip}  agent=%{ip}",
                                  &pdhcp4hdr->serverip, &pdhcp4hdr->agentip));
  netwib_er(netwib_show_array_fmt32(pbuf, " %{buf}", &bufarray));

  netwib__buf_reinit(&bufarray);
  netwib_er(netwib_buf_append_fmt(&bufarray, "clienteth=%{eth}",
                                  &pdhcp4hdr->clienteth));
  netwib_er(netwib_show_array_fmt32(pbuf, " %{buf}", &bufarray));

  netwib_er(netwib_show_array_data(" sname", &pdhcp4hdr->sname,
                                   NETWIB_ENCODETYPE_MIXED, ' ', pbuf));

  netwib_er(netwib_show_array_data(" file", &pdhcp4hdr->file,
                                   NETWIB_ENCODETYPE_MIXED, ' ', pbuf));

  netwib_er(netwox_dhcp4opts_show(&pdhcp4hdr->opts, encodetype, pbuf));

  netwib_er(netwib_show_array_tail(pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dhcp4hdr_test(void)
{
  netwib_bufpool *pbufpool;
  netwib_buf pkt, pktmix;

  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_buf_init_mallocdefault(&pktmix));
  netwib_er(netwib_buf_append_string("01010600", &pktmix));
  netwib_er(netwib_buf_append_string("12345678", &pktmix));
  netwib_er(netwib_buf_append_string("11223344", &pktmix));
  netwib_er(netwib_buf_append_string("00000001", &pktmix));
  netwib_er(netwib_buf_append_string("00000002", &pktmix));
  netwib_er(netwib_buf_append_string("00000003", &pktmix));
  netwib_er(netwib_buf_append_string("00000004", &pktmix));
  netwib_er(netwib_buf_append_string("0a0b0c0d0e0f00000000000000000000",
                                   &pktmix));
  netwib_er(netwib_buf_append_string("'azazazazazazazazazazazazazazazazazazazazazazazazazazazazazazer' 00 34", &pktmix));
  netwib_er(netwib_buf_append_string("'wxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwxwx' 00 34", &pktmix));
  netwib_er(netwib_buf_append_string("63825363", &pktmix));
  netwib_er(netwib_buf_append_string("00", &pktmix));
  netwib_er(netwib_buf_append_string("010401020304", &pktmix));
  netwib_er(netwib_buf_append_string("0400", &pktmix));
  netwib_er(netwib_buf_append_string("040401020304", &pktmix));
  netwib_er(netwib_buf_append_string("05080102030405060708", &pktmix));
  netwib_er(netwib_buf_append_string("0C00", &pktmix));
  netwib_er(netwib_buf_append_string("0C03 'abc'", &pktmix));
  netwib_er(netwib_buf_append_string("2108 0102030405060708", &pktmix));
  netwib_er(netwib_buf_append_string("2110 0102030405060708 0a0b0c0d0a0b0c0d",
                                   &pktmix));
  netwib_er(netwib_buf_append_string("3304 00000001", &pktmix));
  netwib_er(netwib_buf_append_string("3501 02", &pktmix));
  netwib_er(netwib_buf_append_string("3D03 02 adef", &pktmix));
  netwib_er(netwib_buf_append_string("3702 01 02", &pktmix));
  netwib_er(netwib_buf_append_string("FE01AE", &pktmix));
  netwib_er(netwib_buf_append_string("34", &pktmix));
  netwib_er(netwib_buf_decode(&pktmix, NETWIB_DECODETYPE_MIXED, &pkt));
  netwib_er(netwox_pkt_dhcp4_display(pbufpool, &pkt, NULL,
                                     NETWIB_ENCODETYPE_ARRAY,
                                     NETWIB_ENCODETYPE_DUMP));

  netwib_er(netwib_buf_close(&pkt));
  netwib_er(netwib_buf_close(&pktmix));
  netwib_er(netwib_bufpool_close(&pbufpool));
  return(NETWIB_ERR_OK);
}

