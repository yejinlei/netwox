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
static netwib_err netwox_pkt_append_smbnbtsshdr(netwox_smbnbtsstype smbnbtsstype,
                                                netwib_uint32 size,
                                                netwib_buf *ppkt)
{
  netwib_data data;

  if (size > 0x1FFFF) {
    return(NETWIB_ERR_PATOOHIGH);
  }

  netwib_er(netwib_buf_wantspace(ppkt, NETWOX_SMBNBTSSHDR_LEN, &data));
  netwib__data_append_uint8(data, smbnbtsstype);
  netwib__data_append_uint8(data, size >> 16);
  netwib__data_append_uint16(data, size & 0xFFFF);
  ppkt->endoffset += NETWOX_SMBNBTSSHDR_LEN;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_smbnbtss_msghdr(netwib_uint32 msgsize,
                                             netwib_buf *ppkt)
{
  netwib_er(netwox_pkt_append_smbnbtsshdr(NETWOX_SMBNBTSSTYPE_MSG, msgsize,
                                          ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_smbnbtss(netwox_constsmbnbtss *psmbnbtss,
                                      netwib_buf *ppkt)
{
  netwib_data data;

  switch(psmbnbtss->smbnbtsstype) {
  case NETWOX_SMBNBTSSTYPE_MSG :
    netwib_er(netwox_pkt_append_smbnbtsshdr(psmbnbtss->smbnbtsstype, netwib__buf_ref_data_size(&psmbnbtss->payload.msg.smbpkt), ppkt));
    netwib_er(netwib_buf_append_buf(&psmbnbtss->payload.msg.smbpkt, ppkt));
    break;
  case NETWOX_SMBNBTSSTYPE_REQUEST :
    netwib_er(netwox_pkt_append_smbnbtsshdr(psmbnbtss->smbnbtsstype, 68,
                                            ppkt));
    netwib_er(netwib_buf_append_byte(psmbnbtss->payload.request.callednamelabel, ppkt));
    netwib_er(netwib_buf_append_data(psmbnbtss->payload.request.calledname,
                                     32, ppkt));
    netwib_er(netwib_buf_append_byte(0, ppkt));
    netwib_er(netwib_buf_append_byte(psmbnbtss->payload.request.callingnamelabel, ppkt));
    netwib_er(netwib_buf_append_data(psmbnbtss->payload.request.callingname,
                                     32, ppkt));
    netwib_er(netwib_buf_append_byte(0, ppkt));
    break;
  case NETWOX_SMBNBTSSTYPE_RESPPOSITIVE :
    netwib_er(netwox_pkt_append_smbnbtsshdr(psmbnbtss->smbnbtsstype, 0,
                                            ppkt));
    break;
  case NETWOX_SMBNBTSSTYPE_RESPNEGATIVE :
    netwib_er(netwox_pkt_append_smbnbtsshdr(psmbnbtss->smbnbtsstype, 1,
                                            ppkt));
    netwib_er(netwib_buf_append_byte((netwib_byte)psmbnbtss->payload.respnegative.error,
                                     ppkt));

    break;
  case NETWOX_SMBNBTSSTYPE_RESPRETARGET :
    if (psmbnbtss->payload.respretarget.host.iptype != NETWIB_IPTYPE_IP4) {
      return(NETWIB_ERR_PAIPTYPENOT4);
    }
    netwib_er(netwox_pkt_append_smbnbtsshdr(psmbnbtss->smbnbtsstype, 6,
                                            ppkt));
    netwib_er(netwib_buf_wantspace(ppkt, 6, &data));
    netwib__data_append_uint32(data, psmbnbtss->payload.respretarget.host.ipvalue.ip4);
    netwib__data_append_uint16(data, psmbnbtss->payload.respretarget.port);
    ppkt->endoffset += 6;
    break;
  case NETWOX_SMBNBTSSTYPE_KEEPALIVE :
    netwib_er(netwox_pkt_append_smbnbtsshdr(psmbnbtss->smbnbtsstype, 0,
                                            ppkt));
    break;
  case NETWOX_SMBNBTSSTYPE_KEEPALIVE95 :
    netwib_er(netwib_buf_wantspace(ppkt, 1, &data));
    data[0] = 0x00;
    ppkt->endoffset += 1;
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_decode_smbnbtss(netwib_constbuf *ppkt,
                                      netwox_smbnbtss *psmbnbtss,
                                      netwib_uint32 *pskipsize)
{
  netwib_data data;
  netwib_uint32 datasize, size, reserved;

  data = netwib__buf_ref_data_ptr(ppkt);
  datasize = netwib__buf_ref_data_size(ppkt);

  /* special case of KEEPALIVE95 */
  if (datasize == 1) {
    if (data[0] == 0x00) {
      psmbnbtss->smbnbtsstype = NETWOX_SMBNBTSSTYPE_KEEPALIVE95;
      if (pskipsize != NULL) *pskipsize = 1;
      return(NETWIB_ERR_OK);
    }
  }

  /* decode header */
  if (datasize < NETWOX_SMBNBTSSHDR_LEN) {
    return(NETWIB_ERR_DATAMISSING);
  }
  if (pskipsize != NULL) *pskipsize = NETWOX_SMBNBTSSHDR_LEN;

  netwib__data_decode_uint8t(data, psmbnbtss->smbnbtsstype,
                             netwox_smbnbtsstype);
  netwib__data_decode_uint8(data, reserved);
  netwib__data_decode_uint16(data, size);
  if (reserved & 0x1) {
    size = size | 0x10000;
  }
  datasize -= NETWOX_SMBNBTSSHDR_LEN;
  if (datasize < size) {
    return(NETWIB_ERR_DATAMISSING);
  }
  if (pskipsize != NULL) *pskipsize = NETWOX_SMBNBTSSHDR_LEN + size;

  /* decode body */
  switch(psmbnbtss->smbnbtsstype) {
  case NETWOX_SMBNBTSSTYPE_MSG :
    netwib_er(netwib_buf_init_ext_arrayfilled(data, size,
                                              &psmbnbtss->payload.msg.smbpkt));
    break;
  case NETWOX_SMBNBTSSTYPE_REQUEST :
    if (size != 68 && size != 72 /* Samba 2 ends with 4 zero bytes */) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (data[33] != 0 || data[67] != 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    psmbnbtss->payload.request.callednamelabel = data[0];
    netwib_c_memcpy(psmbnbtss->payload.request.calledname, data+1, 32);
    psmbnbtss->payload.request.callingnamelabel = data[34];
    netwib_c_memcpy(psmbnbtss->payload.request.callingname, data+35, 32);
    if (size == 72) {
      /* ensure 4 bytes added by Samba 2 are zero */
      if (data[68] != 0 || data[69] != 0 || data[70] != 0 || data[71] != 0) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
    }
    break;
  case NETWOX_SMBNBTSSTYPE_RESPPOSITIVE :
    if (size != 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBNBTSSTYPE_RESPNEGATIVE :
    if (size != 1) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    psmbnbtss->payload.respnegative.error = data[0];
    break;
  case NETWOX_SMBNBTSSTYPE_RESPRETARGET :
    if (size != 6) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    psmbnbtss->payload.respretarget.host.iptype = NETWIB_IPTYPE_IP4;
    netwib__data_decode_uint32(data, psmbnbtss->payload.respretarget.host.ipvalue.ip4);
    netwib__data_decode_uint16(data, psmbnbtss->payload.respretarget.port);
    break;
  case NETWOX_SMBNBTSSTYPE_KEEPALIVE :
    if (size != 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}
