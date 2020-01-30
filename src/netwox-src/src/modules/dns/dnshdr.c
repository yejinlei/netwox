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
netwib_err netwox_buf_append_dnshdr_opcode(netwox_dnshdr_opcode opcode,
                                           netwib_buf *pbuf)
{
  netwib_conststring pc=NULL;

  switch(opcode) {
  case NETWOX_DNSHDR_OPCODE_QUERY : pc = "QUERY"; break;
  case NETWOX_DNSHDR_OPCODE_IQUERY : pc = "IQUERY"; break;
  case NETWOX_DNSHDR_OPCODE_STATUS : pc = "STATUS"; break;
  case NETWOX_DNSHDR_OPCODE_NOTIFY : pc = "NOTIFY"; break;
  case NETWOX_DNSHDR_OPCODE_UPDATE : pc = "UPDATE"; break;
  default :
    break;
  }

  if (pc != NULL) {
    netwib_er(netwib_buf_append_string(pc, pbuf));
  } else {
    netwib_er(netwib_buf_append_fmt(pbuf, "UNKNOWN(%{uint32})", opcode));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_buf_append_dnshdr_rcode(netwox_dnshdr_rcode rcode,
                                          netwib_buf *pbuf)
{
  netwib_conststring pc=NULL;

  switch(rcode) {
  case NETWOX_DNSHDR_RCODE_OK : pc = "OK"; break;
  case NETWOX_DNSHDR_RCODE_FMTERROR : pc = "FMTERROR"; break;
  case NETWOX_DNSHDR_RCODE_FAILURE : pc = "FAILURE"; break;
  case NETWOX_DNSHDR_RCODE_NAMEERROR : pc = "NAMEERROR"; break;
  case NETWOX_DNSHDR_RCODE_UNIMPLEMENTED : pc = "UNIMPLEMENTED"; break;
  case NETWOX_DNSHDR_RCODE_REFUSED : pc = "REFUSED"; break;
  case NETWOX_DNSHDR_RCODE_NAMEEXISTS : pc = "NAMEEXISTS"; break;
  case NETWOX_DNSHDR_RCODE_RRSETEXISTS : pc = "RRSETEXISTS"; break;
  case NETWOX_DNSHDR_RCODE_RRSETNOEXISTS : pc = "RRSETNOEXISTS"; break;
  case NETWOX_DNSHDR_RCODE_NOTAUTH : pc = "NOTAUTH"; break;
  case NETWOX_DNSHDR_RCODE_NOTZONE : pc = "NOTZONE"; break;
  default :
    break;
  }

  if (pc != NULL) {
    netwib_er(netwib_buf_append_string(pc, pbuf));
  } else {
    netwib_er(netwib_buf_append_fmt(pbuf, "UNKNOWN(%{uint32})", rcode));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnshdr_initdefault(netwox_dnshdr *pdnshdr)
{
  netwib_uint32 ui;

  netwib_er(netwib_uint32_init_rand(0, 0xFFFFu, &ui));
  pdnshdr->id = (netwib_uint16)ui;
  pdnshdr->isaresponse = NETWIB_FALSE;
  pdnshdr->opcode = NETWOX_DNSHDR_OPCODE_QUERY;
  pdnshdr->authanswer = NETWIB_FALSE;
  pdnshdr->truncation = NETWIB_FALSE;
  pdnshdr->recurdesired = NETWIB_FALSE;
  pdnshdr->recuravail = NETWIB_FALSE;
  pdnshdr->rcode = NETWOX_DNSHDR_RCODE_OK;
  pdnshdr->count_question = 0;
  pdnshdr->count_answer = 0;
  pdnshdr->count_authority = 0;
  pdnshdr->count_additional = 0;

  return(NETWIB_ERR_OK);
}
