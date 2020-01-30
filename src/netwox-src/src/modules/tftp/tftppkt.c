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
netwib_err netwox_buf_append_tftpopcode(netwox_tftpopcode opcode,
                                        netwib_buf *pbuf)
{
  netwib_conststring pc;

  pc = NULL;
  switch(opcode) {
  case NETWOX_TFTPOPCODE_RRQ :
    pc = "read request";
    break;
  case NETWOX_TFTPOPCODE_WRQ :
    pc = "write request";
    break;
  case NETWOX_TFTPOPCODE_DATA :
    pc = "data";
    break;
  case NETWOX_TFTPOPCODE_ACK :
    pc = "ack";
    break;
  case NETWOX_TFTPOPCODE_ERROR :
    pc = "error";
    break;
  default :
    pc = "?";
    break;
  }

  if (pc != NULL) {
    netwib_er(netwib_buf_append_string(pc, pbuf));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_tftppkt(netwox_consttftppkt *ptftppkt,
                                     netwib_buf *ppkt)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(ppkt, 2, &data));
  netwib__data_append_uint16(data, ptftppkt->opcode);
  ppkt->endoffset += 2;

  switch(ptftppkt->opcode) {
  case NETWOX_TFTPOPCODE_RRQ :
  case NETWOX_TFTPOPCODE_WRQ :
    netwib_er(netwib_buf_append_buf(&ptftppkt->pkt.rq.filename, ppkt));
    netwib_er(netwib_buf_append_byte(0, ppkt));
    netwib_er(netwib_buf_append_buf(&ptftppkt->pkt.rq.mode, ppkt));
    netwib_er(netwib_buf_append_byte(0, ppkt));
    break;
  case NETWOX_TFTPOPCODE_DATA :
    netwib_er(netwib_buf_wantspace(ppkt, 2, &data));
    netwib__data_append_uint16(data, ptftppkt->pkt.data.block);
    ppkt->endoffset += 2;
    netwib_er(netwib_buf_append_buf(&ptftppkt->pkt.data.data, ppkt));
    break;
  case NETWOX_TFTPOPCODE_ACK :
    netwib_er(netwib_buf_wantspace(ppkt, 2, &data));
    netwib__data_append_uint16(data, ptftppkt->pkt.ack.block);
    ppkt->endoffset += 2;
    break;
  case NETWOX_TFTPOPCODE_ERROR :
    netwib_er(netwib_buf_wantspace(ppkt, 2, &data));
    netwib__data_append_uint16(data, ptftppkt->pkt.error.code);
    ppkt->endoffset += 2;
    netwib_er(netwib_buf_append_buf(&ptftppkt->pkt.error.msg, ppkt));
    netwib_er(netwib_buf_append_byte(0, ppkt));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_decode_tftppkt(netwib_constbuf *ppkt,
                                     netwox_tftppkt *ptftppkt)
{
  netwib_data data, pc;
  netwib_uint32 datasize;

  datasize = netwib__buf_ref_data_size(ppkt);
  if (datasize < 2) {
    return(NETWIB_ERR_DATAMISSING);
  }
  data = netwib__buf_ref_data_ptr(ppkt);
  netwib__data_decode_uint16(data, ptftppkt->opcode);
  datasize -= 2;

  switch(ptftppkt->opcode) {
  case NETWOX_TFTPOPCODE_RRQ :
  case NETWOX_TFTPOPCODE_WRQ :
    if (datasize < 4) {
      return(NETWIB_ERR_DATAMISSING);
    }
    pc = netwib_c_memchr(data, 0, datasize);
    if (pc == NULL) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwib_buf_init_ext_arrayfilled(data, pc - data,
                                              &ptftppkt->pkt.rq.filename));
    datasize -= pc - data + 1;
    data += pc - data + 1;
    pc = netwib_c_memchr(data, 0, datasize);
    if (pc == NULL) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwib_buf_init_ext_arrayfilled(data, pc - data,
                                              &ptftppkt->pkt.rq.mode));
    break;
  case NETWOX_TFTPOPCODE_DATA :
    if (datasize < 2) {
      return(NETWIB_ERR_DATAMISSING);
    }
    netwib__data_decode_uint16(data, ptftppkt->pkt.data.block);
    datasize -= 2;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize,
                                              &ptftppkt->pkt.data.data));
    break;
  case NETWOX_TFTPOPCODE_ACK :
    if (datasize < 2) {
      return(NETWIB_ERR_DATAMISSING);
    }
    netwib__data_decode_uint16(data, ptftppkt->pkt.ack.block);
    break;
  case NETWOX_TFTPOPCODE_ERROR :
    if (datasize < 3) {
      return(NETWIB_ERR_DATAMISSING);
    }
    netwib__data_decode_uint16(data, ptftppkt->pkt.error.code);
    datasize -= 2;
    pc = netwib_c_memchr(data, 0, datasize);
    if (pc == NULL) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwib_buf_init_ext_arrayfilled(data, pc - data,
                                              &ptftppkt->pkt.error.msg));
    break;
  default :
    return(NETWOX_ERR_NOTSUPPORTED);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_tftppkt_show(netwox_consttftppkt *ptftppkt,
                               netwib_encodetype encodetype,
                               netwib_buf *pbuf)
{
  netwib_byte array[80];
  netwib_buf buf;

  if (encodetype == NETWIB_ENCODETYPE_SYNTH) {
    netwib_er(netwib_buf_append_string("tftp ", pbuf));
    netwib_er(netwox_buf_append_tftpopcode(ptftppkt->opcode, pbuf));
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));

  netwib_er(netwib_show_array_head("TFTP", pbuf));
  netwib_er(netwox_buf_append_tftpopcode(ptftppkt->opcode, &buf));
  netwib_er(netwib_show_array_fmt32(pbuf, " opcode: %{uint16} (TFTP %{buf})",
                                    ptftppkt->opcode, &buf));

  switch(ptftppkt->opcode) {
  case NETWOX_TFTPOPCODE_RRQ :
  case NETWOX_TFTPOPCODE_WRQ :
    netwib_er(netwib_show_array_data(" filename", &ptftppkt->pkt.rq.filename,
                                     NETWIB_ENCODETYPE_DATA,
                                     ' ', pbuf));
    netwib_er(netwib_show_array_data(" mode", &ptftppkt->pkt.rq.mode,
                                     NETWIB_ENCODETYPE_DATA,
                                     ' ', pbuf));
    break;
  case NETWOX_TFTPOPCODE_DATA :
    netwib_er(netwib_show_array_fmt32(pbuf, " blocknumber: %{uint16}",
                                      ptftppkt->pkt.data.block));
    netwib_er(netwib_show_array_data(" data", &ptftppkt->pkt.data.data,
                                     NETWIB_ENCODETYPE_MIXED,
                                     ' ', pbuf));
    break;
  case NETWOX_TFTPOPCODE_ACK :
    netwib_er(netwib_show_array_fmt32(pbuf, " blocknumber: %{uint16}",
                                      ptftppkt->pkt.data.block));
    break;
  case NETWOX_TFTPOPCODE_ERROR :
    netwib_er(netwib_show_array_fmt32(pbuf, " code: %{uint16}",
                                      ptftppkt->pkt.error.code));
    netwib_er(netwib_show_array_data(" msg", &ptftppkt->pkt.error.msg,
                                     NETWIB_ENCODETYPE_MIXED,
                                     ' ', pbuf));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  netwib_er(netwib_show_array_tail(pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_tftp_show(netwib_bufpool *pbufpool,
                                netwib_constbuf *ppkt,
                                netwib_encodetype_context *pctx,
                                netwib_encodetype tftpencodetype,
                                netwib_encodetype dataencodetype,
                                netwib_buf *pbuf)
{
  netwox_tftppkt tftppkt;
  netwib_encodetype_context *pctxtouse, ctx;
  netwib_buf pkt;
  netwib_err ret;

  pctxtouse = pctx;
  if (pctx == NULL) {
    netwib_er(netwib_buf_encode_transition_init(&ctx));
    pctxtouse = &ctx;
  }

  switch(tftpencodetype) {
  case NETWIB_ENCODETYPE_SYNTH :
  case NETWIB_ENCODETYPE_ARRAY :
    break;
  default :
    netwib_er(netwib_pkt_data_show(ppkt, pctxtouse, tftpencodetype, pbuf));
    return(NETWIB_ERR_OK);
    break;
  }

  pkt = *ppkt;
  ret = netwox_pkt_decode_tftppkt(&pkt, &tftppkt);
  if (ret == NETWIB_ERR_NOTCONVERTED || ret == NETWOX_ERR_NOTSUPPORTED
      || ret == NETWIB_ERR_DATAMISSING) {
    netwib_er(netwib_pkt_data_show(ppkt, pctxtouse, dataencodetype, pbuf));
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  netwib_er(netwib_buf_encode_transition(pctxtouse, tftpencodetype, pbuf));
  ret = netwox_tftppkt_show(&tftppkt, tftpencodetype, pbuf);

  pbufpool = pbufpool; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_tftp_display(netwib_bufpool *pbufpool,
                                   netwib_constbuf *ppkt,
                                   netwib_encodetype_context *pctx,
                                   netwib_encodetype tftpencodetype,
                                   netwib_encodetype dataencodetype)
{
  netwib_buf *pbuf;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
  ret = netwox_pkt_tftp_show(pbufpool, ppkt, pctx, tftpencodetype,
                             dataencodetype, pbuf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_display(pbuf, NETWIB_ENCODETYPE_DATA));
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(ret);
}
