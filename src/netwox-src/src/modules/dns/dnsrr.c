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
netwib_err netwox_dnsrr_type_init_buf(netwib_constbuf *pbuf,
                                      netwox_dnsrr_type *ptype)
{
  netwib_string pc;
  netwox_dnsrr_type type;

  netwib__constbuf_ref_string(pbuf, pc, bufstorage,
                              netwox_dnsrr_type_init_buf(&bufstorage, ptype));

  if (!netwib_c_strcasecmp(pc, "a")) {
    type = NETWOX_DNSRR_TYPE_A;
  } else if (!netwib_c_strcasecmp(pc, "ns")) {
    type = NETWOX_DNSRR_TYPE_NS;
  } else if (!netwib_c_strcasecmp(pc, "cname")) {
    type = NETWOX_DNSRR_TYPE_CNAME;
  } else if (!netwib_c_strcasecmp(pc, "soa")) {
    type = NETWOX_DNSRR_TYPE_SOA;
  } else if (!netwib_c_strcasecmp(pc, "wks")) {
    type = NETWOX_DNSRR_TYPE_WKS;
  } else if (!netwib_c_strcasecmp(pc, "ptr")) {
    type = NETWOX_DNSRR_TYPE_PTR;
  } else if (!netwib_c_strcasecmp(pc, "hinfo")) {
    type = NETWOX_DNSRR_TYPE_HINFO;
  } else if (!netwib_c_strcasecmp(pc, "mx")) {
    type = NETWOX_DNSRR_TYPE_MX;
  } else if (!netwib_c_strcasecmp(pc, "txt")) {
    type = NETWOX_DNSRR_TYPE_TXT;
  } else if (!netwib_c_strcasecmp(pc, "rp")) {
    type = NETWOX_DNSRR_TYPE_RP;
  } else if (!netwib_c_strcasecmp(pc, "aaaa")) {
    type = NETWOX_DNSRR_TYPE_AAAA;
  } else if (!netwib_c_strcasecmp(pc, "loc")) {
    type = NETWOX_DNSRR_TYPE_LOC;
  } else if (!netwib_c_strcasecmp(pc, "srv")) {
    type = NETWOX_DNSRR_TYPE_SRV;
  } else if (!netwib_c_strcasecmp(pc, "a6")) {
    type = NETWOX_DNSRR_TYPE_A6;
  } else if (!netwib_c_strcasecmp(pc, "opt")) {
    type = NETWOX_DNSRR_TYPE_OPT;
  } else if (!netwib_c_strcasecmp(pc, "axfr")) {
    type = NETWOX_DNSRR_TYPE_AXFR;
  } else if (!netwib_c_strcasecmp(pc, "any") ||
             !netwib_c_strcasecmp(pc, "all")) {
    type = NETWOX_DNSRR_TYPE_ANY;
  } else {
    type = NETWOX_DNSRR_TYPE_UNKNOWN;
  }

  if (ptype != NULL) *ptype = type;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_buf_append_dnsrr_type(netwox_dnsrr_type type,
                                        netwib_buf *pbuf)
{
  netwib_conststring pc=NULL;

  switch(type) {
  case NETWOX_DNSRR_TYPE_A : pc = "A"; break;
  case NETWOX_DNSRR_TYPE_NS : pc = "NS"; break;
  case NETWOX_DNSRR_TYPE_CNAME : pc = "CNAME"; break;
  case NETWOX_DNSRR_TYPE_SOA : pc = "SOA"; break;
  case NETWOX_DNSRR_TYPE_WKS : pc = "WKS"; break;
  case NETWOX_DNSRR_TYPE_PTR : pc = "PTR"; break;
  case NETWOX_DNSRR_TYPE_HINFO : pc = "HINFO"; break;
  case NETWOX_DNSRR_TYPE_MX : pc = "MX"; break;
  case NETWOX_DNSRR_TYPE_TXT : pc = "TXT"; break;
  case NETWOX_DNSRR_TYPE_RP : pc = "RP"; break;
  case NETWOX_DNSRR_TYPE_AAAA : pc = "AAAA"; break;
  case NETWOX_DNSRR_TYPE_LOC : pc = "LOC"; break;
  case NETWOX_DNSRR_TYPE_SRV : pc = "SRV"; break;
  case NETWOX_DNSRR_TYPE_A6 : pc = "A6"; break;
  case NETWOX_DNSRR_TYPE_OPT : pc = "OPT"; break;
  case NETWOX_DNSRR_TYPE_AXFR : pc = "AXFR"; break;
  case NETWOX_DNSRR_TYPE_ANY : pc = "ANY"; break;
  default :
    break;
  }

  if (pc != NULL) {
    netwib_er(netwib_buf_append_string(pc, pbuf));
  } else {
    netwib_er(netwib_buf_append_fmt(pbuf, "UNKNOWN(%{uint32})", type));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrr_class_init_buf(netwib_constbuf *pbuf,
                                       netwox_dnsrr_class *pclass)
{
  netwib_string pc;
  netwox_dnsrr_class rclass;

  netwib__constbuf_ref_string(pbuf, pc, bufstorage,
                              netwox_dnsrr_class_init_buf(&bufstorage, pclass));

  if (!netwib_c_strcasecmp(pc, "in")) {
    rclass = NETWOX_DNSRR_CLASS_IN;
  } else if (!netwib_c_strcasecmp(pc, "cs")) {
    rclass = NETWOX_DNSRR_CLASS_CS;
  } else if (!netwib_c_strcasecmp(pc, "ch")) {
    rclass = NETWOX_DNSRR_CLASS_CH;
  } else if (!netwib_c_strcasecmp(pc, "hs")) {
    rclass = NETWOX_DNSRR_CLASS_HS;
  } else if (!netwib_c_strcasecmp(pc, "all")) {
    rclass = NETWOX_DNSRR_CLASS_ALL;
  } else {
    rclass = NETWOX_DNSRR_CLASS_IN; /* logical default */
  }

  if (pclass != NULL) *pclass = rclass;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_buf_append_dnsrr_class(netwox_dnsrr_class rclass,
                                         netwib_buf *pbuf)
{
  netwib_conststring pc=NULL;

  switch(rclass) {
  case NETWOX_DNSRR_CLASS_IN : pc = "IN"; break;
  case NETWOX_DNSRR_CLASS_CS : pc = "CS"; break;
  case NETWOX_DNSRR_CLASS_CH : pc = "CH"; break;
  case NETWOX_DNSRR_CLASS_HS : pc = "HS"; break;
  case NETWOX_DNSRR_CLASS_ALL : pc = "ALL"; break;
  default :
    break;
  }

  if (pc != NULL) {
    netwib_er(netwib_buf_append_string(pc, pbuf));
  } else {
    netwib_er(netwib_buf_append_fmt(pbuf, "UNKNOWN(%{uint32})", rclass));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrr_init(netwox_dnsrr *prr)
{
  netwib_er(netwib_buf_init_mallocdefault(&prr->name));
  prr->type = NETWOX_DNSRR_TYPE_UNKNOWN;
  prr->rclass = NETWOX_DNSRR_CLASS_IN;
  prr->ttl = 0;
  netwib_er(netwib_buf_init_mallocdefault(&prr->rdata));
  prr->kind = NETWOX_DNSRR_KIND_UNKNOWN;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrr_close(netwox_dnsrr *prr)
{
  netwib_er(netwib_buf_close(&prr->name));
  netwib_er(netwib_buf_close(&prr->rdata));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrr_set(netwox_dnsrr *prr,
                            netwox_dnsrr_kind kind,
                            netwib_constbuf *pname,
                            netwox_dnsrr_type type,
                            netwox_dnsrr_class rclass,
                            netwib_uint32 ttl)
{

  prr->kind = kind;
  netwib__buf_reinit(&prr->name);
  netwib_er(netwib_buf_append_buf(pname, &prr->name));
  prr->type = type;
  prr->rclass = rclass;

  prr->ttl = ttl;
  netwib__buf_reinit(&prr->rdata);
  /* rdata will be added separately */

  return(NETWIB_ERR_OK);
}
