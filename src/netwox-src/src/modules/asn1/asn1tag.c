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
netwib_err netwox_asn1tag_ctx_init(netwox_asn1tag_ctx *pctx)
{

  netwib_er(netwib_array_init(sizeof(netwox_asn1tag), 0, &pctx->tag));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1tag_ctx_close(netwox_asn1tag_ctx *pctx)
{
  netwib_er(netwib_array_close(&pctx->tag));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1tag_ctx_add(netwox_asn1tag_ctx *pctx,
                                  netwox_asn1tag asn1tag)
{
  netwox_asn1tag *pasn1tag;

  netwib_er(netwib_array_ctl_set_size(&pctx->tag, pctx->tag.size+1));
  pasn1tag = (netwox_asn1tag*)pctx->tag.p[pctx->tag.size-1];
  *pasn1tag = asn1tag;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1tag_ctx_del(netwox_asn1tag_ctx *pctx)
{
  if (pctx->tag.size == 0) {
    return(NETWOX_ERR_INTERNALERROR);
  }
  netwib_er(netwib_array_ctl_set_size(&pctx->tag, pctx->tag.size-1));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1tag_ctx_get(netwox_asn1tag_ctx *pctx,
                                  netwox_asn1tag_ctxtype *pctxtype)
{
  netwox_asn1tag_ctxtype ctxtype;
  netwox_asn1tag *pasn1tag0, *pasn1tag1=NULL;

  ctxtype = NETWOX_ASN1TAG_CTXTYPE_UNKNOWN;

  pasn1tag0 = pctx->tag.p[0];
  if (pctx->tag.size >= 2) {
    pasn1tag1 = pctx->tag.p[1];
  }

  if (pctx->tag.size == 1 && *pasn1tag0 == NETWOX_ASN1TAG_SEQUENCE) {
    /* SNMPv1 et v2 */
    ctxtype = NETWOX_ASN1TAG_CTXTYPE_SNMPPDU;
  } else if (pctx->tag.size == 2 && *pasn1tag0 == NETWOX_ASN1TAG_SEQUENCE &&
             *pasn1tag1 == NETWOX_ASN1TAG_SEQUENCE) {
    /* SNMPv3 */
    ctxtype = NETWOX_ASN1TAG_CTXTYPE_SNMPPDU;
  } else if (pctx->tag.size > 2) {
    if (*pasn1tag0 == NETWOX_ASN1TAG_SEQUENCE) {
      switch(*pasn1tag1) {
      case NETWOX_ASN1TAG_SNMPPDU_GETREQUEST :
      case NETWOX_ASN1TAG_SNMPPDU_GETNEXTREQUEST :
      case NETWOX_ASN1TAG_SNMPPDU_RESPONSE :
      case NETWOX_ASN1TAG_SNMPPDU_SETREQUEST :
      case NETWOX_ASN1TAG_SNMPPDU_TRAP :
      case NETWOX_ASN1TAG_SNMPPDU_GETBULKREQUEST :
      case NETWOX_ASN1TAG_SNMPPDU_INFORMREQUEST :
      case NETWOX_ASN1TAG_SNMPPDU_TRAP2 :
      case NETWOX_ASN1TAG_SNMPPDU_REPORT :
        ctxtype = NETWOX_ASN1TAG_CTXTYPE_SNMPVAR;
        break;
      default :
        break;
      }
    }
  }

#if 0
  if (ctxtype == NETWOX_ASN1TAG_CTXTYPE_UNKNOWN) {
    netwib_uint32 i;
    netwib_er(netwib_fmt_display("Context not decoded:"));
    for (i = 0; i < pctx->tag.size; i++) {
      pasn1tag0 = pctx->tag.p[i];
      netwib_er(netwib_fmt_display(" %{uint32}", *pasn1tag0));
    }
    netwib_er(netwib_fmt_display("\n"));
  }
#endif

  if (pctxtype != NULL) *pctxtype = ctxtype;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1tag_init(netwox_asn1tag_ext ext,
                               netwox_asn1tag_ctx *pctx,
                               netwox_asn1tag_class cl,
                               netwox_asn1tag_num num,
                               netwox_asn1tag *ptag)
{
  netwox_asn1tag_ctxtype ctxtype;
  netwox_asn1tag tag;

  tag = NETWOX_ASN1TAG_UNKNOWN;

  switch(cl) {
  case NETWOX_ASN1TAG_CLASS_UNI :
    switch(num) {
    case NETWOX_ASN1TAG_NUM_END : tag = NETWOX_ASN1TAG_END; break;
    case NETWOX_ASN1TAG_NUM_BOOL : tag = NETWOX_ASN1TAG_BOOL; break;
    case NETWOX_ASN1TAG_NUM_INTEGER : tag = NETWOX_ASN1TAG_INTEGER; break;
    case NETWOX_ASN1TAG_NUM_BITSTRING : tag = NETWOX_ASN1TAG_BITSTRING; break;
    case NETWOX_ASN1TAG_NUM_OCTETSTRING : tag=NETWOX_ASN1TAG_OCTETSTRING;break;
    case NETWOX_ASN1TAG_NUM_NULL : tag = NETWOX_ASN1TAG_NULL; break;
    case NETWOX_ASN1TAG_NUM_OID : tag = NETWOX_ASN1TAG_OID; break;
    case NETWOX_ASN1TAG_NUM_SEQUENCE : tag = NETWOX_ASN1TAG_SEQUENCE; break;
    default : break;
    }
    break;
  case NETWOX_ASN1TAG_CLASS_APP :
    if (ext == NETWOX_ASN1TAG_EXT_SNMP) {
      switch(num) {
      case NETWOX_ASN1TAG_NUM_IP4 : tag = NETWOX_ASN1TAG_IP4; break;
      case NETWOX_ASN1TAG_NUM_COUNTER32 : tag = NETWOX_ASN1TAG_COUNTER32;
        break;
      case NETWOX_ASN1TAG_NUM_GAUGE32 /*or UINT32*/:
        tag = NETWOX_ASN1TAG_GAUGE32; break;
      case NETWOX_ASN1TAG_NUM_TIMETICKS : tag = NETWOX_ASN1TAG_TIMETICKS;break;
      case NETWOX_ASN1TAG_NUM_OPAQUE : tag = NETWOX_ASN1TAG_OPAQUE; break;
      case NETWOX_ASN1TAG_NUM_NSAP : tag = NETWOX_ASN1TAG_NSAP; break;
      case NETWOX_ASN1TAG_NUM_UINT32OLD : tag = NETWOX_ASN1TAG_UINT32OLD;
        break;
      case NETWOX_ASN1TAG_NUM_COUNTER64 : tag = NETWOX_ASN1TAG_COUNTER64;break;
      default : break;
      }
    }
    break;
  case NETWOX_ASN1TAG_CLASS_CTX :
    if (ext == NETWOX_ASN1TAG_EXT_SNMP) {
      netwib_er(netwox_asn1tag_ctx_get(pctx, &ctxtype));
      switch(ctxtype) {
      case NETWOX_ASN1TAG_CTXTYPE_SNMPPDU :
        switch(num) {
        case NETWOX_ASN1TAG_NUM_SNMPPDU_GETREQUEST :
          tag = NETWOX_ASN1TAG_SNMPPDU_GETREQUEST; break;
        case NETWOX_ASN1TAG_NUM_SNMPPDU_GETNEXTREQUEST :
          tag = NETWOX_ASN1TAG_SNMPPDU_GETNEXTREQUEST; break;
        case NETWOX_ASN1TAG_NUM_SNMPPDU_RESPONSE :
          tag = NETWOX_ASN1TAG_SNMPPDU_RESPONSE; break;
        case NETWOX_ASN1TAG_NUM_SNMPPDU_SETREQUEST :
          tag = NETWOX_ASN1TAG_SNMPPDU_SETREQUEST; break;
        case NETWOX_ASN1TAG_NUM_SNMPPDU_TRAP :
          tag = NETWOX_ASN1TAG_SNMPPDU_TRAP; break;
        case NETWOX_ASN1TAG_NUM_SNMPPDU_GETBULKREQUEST :
          tag = NETWOX_ASN1TAG_SNMPPDU_GETBULKREQUEST; break;
        case NETWOX_ASN1TAG_NUM_SNMPPDU_INFORMREQUEST :
          tag = NETWOX_ASN1TAG_SNMPPDU_INFORMREQUEST; break;
        case NETWOX_ASN1TAG_NUM_SNMPPDU_TRAP2 :
          tag = NETWOX_ASN1TAG_SNMPPDU_TRAP2; break;
        case NETWOX_ASN1TAG_NUM_SNMPPDU_REPORT :
          tag = NETWOX_ASN1TAG_SNMPPDU_REPORT; break;
        default : break;
        }
        break;
      case NETWOX_ASN1TAG_CTXTYPE_SNMPVAR :
        switch(num) {
        case NETWOX_ASN1TAG_NUM_SNMPVAR_NOSUCHOBJECT :
          tag = NETWOX_ASN1TAG_SNMPVAR_NOSUCHOBJECT; break;
        case NETWOX_ASN1TAG_NUM_SNMPVAR_NOSUCHINSTANCE :
          tag = NETWOX_ASN1TAG_SNMPVAR_NOSUCHINSTANCE; break;
        case NETWOX_ASN1TAG_NUM_SNMPVAR_ENDOFMIBVIEW :
          tag = NETWOX_ASN1TAG_SNMPVAR_ENDOFMIBVIEW; break;
        default : break;
        }
        break;
      default : break;
      }
    }
    break;
  case NETWOX_ASN1TAG_CLASS_PRI :
    break;
  }

  if (ptag != NULL) *ptag = tag;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1tag_decode(netwox_asn1tag tag,
                                 netwox_asn1tag_class *pcl,
                                 netwib_bool *pconstructed,
                                 netwox_asn1tag_num *pnum)
{

#define netwib__ad(cl,constructed,num) if (pcl != NULL) *pcl = cl; if (pconstructed != NULL) *pconstructed = constructed; if (pnum != NULL) *pnum = num; break;

  switch (tag) {
  case NETWOX_ASN1TAG_END :
    netwib__ad(NETWOX_ASN1TAG_CLASS_UNI, NETWIB_FALSE,
              NETWOX_ASN1TAG_NUM_END);
  case NETWOX_ASN1TAG_BOOL :
    netwib__ad(NETWOX_ASN1TAG_CLASS_UNI, NETWIB_FALSE,
              NETWOX_ASN1TAG_NUM_BOOL);
  case NETWOX_ASN1TAG_INTEGER :
    netwib__ad(NETWOX_ASN1TAG_CLASS_UNI, NETWIB_FALSE,
              NETWOX_ASN1TAG_NUM_INTEGER);
  case NETWOX_ASN1TAG_BITSTRING :
    netwib__ad(NETWOX_ASN1TAG_CLASS_UNI, NETWIB_FALSE,
              NETWOX_ASN1TAG_NUM_BITSTRING);
  case NETWOX_ASN1TAG_OCTETSTRING :
    netwib__ad(NETWOX_ASN1TAG_CLASS_UNI, NETWIB_FALSE,
              NETWOX_ASN1TAG_NUM_OCTETSTRING);
  case NETWOX_ASN1TAG_NULL :
    netwib__ad(NETWOX_ASN1TAG_CLASS_UNI, NETWIB_FALSE,
              NETWOX_ASN1TAG_NUM_NULL);
  case NETWOX_ASN1TAG_OID :
    netwib__ad(NETWOX_ASN1TAG_CLASS_UNI, NETWIB_FALSE,
              NETWOX_ASN1TAG_NUM_OID);
  case NETWOX_ASN1TAG_SEQUENCE :
    netwib__ad(NETWOX_ASN1TAG_CLASS_UNI, NETWIB_TRUE,
              NETWOX_ASN1TAG_NUM_SEQUENCE);
  case NETWOX_ASN1TAG_IP4 :
    netwib__ad(NETWOX_ASN1TAG_CLASS_APP, NETWIB_FALSE,
              NETWOX_ASN1TAG_NUM_IP4);
  case NETWOX_ASN1TAG_COUNTER32 :
    netwib__ad(NETWOX_ASN1TAG_CLASS_APP, NETWIB_FALSE,
              NETWOX_ASN1TAG_NUM_COUNTER32);
  case NETWOX_ASN1TAG_GAUGE32 :
    netwib__ad(NETWOX_ASN1TAG_CLASS_APP, NETWIB_FALSE,
              NETWOX_ASN1TAG_NUM_GAUGE32);
  case NETWOX_ASN1TAG_TIMETICKS :
    netwib__ad(NETWOX_ASN1TAG_CLASS_APP, NETWIB_FALSE,
              NETWOX_ASN1TAG_NUM_TIMETICKS);
  case NETWOX_ASN1TAG_OPAQUE :
    netwib__ad(NETWOX_ASN1TAG_CLASS_APP, NETWIB_FALSE,
              NETWOX_ASN1TAG_NUM_OPAQUE);
  case NETWOX_ASN1TAG_NSAP :
    netwib__ad(NETWOX_ASN1TAG_CLASS_APP, NETWIB_FALSE,
              NETWOX_ASN1TAG_NUM_NSAP);
  case NETWOX_ASN1TAG_UINT32OLD :
    netwib__ad(NETWOX_ASN1TAG_CLASS_APP, NETWIB_FALSE,
              NETWOX_ASN1TAG_NUM_UINT32OLD);
  case NETWOX_ASN1TAG_COUNTER64 :
    netwib__ad(NETWOX_ASN1TAG_CLASS_APP, NETWIB_FALSE,
              NETWOX_ASN1TAG_NUM_COUNTER64);
  case NETWOX_ASN1TAG_UINT32 :
    netwib__ad(NETWOX_ASN1TAG_CLASS_APP, NETWIB_FALSE,
              NETWOX_ASN1TAG_NUM_UINT32);
  case NETWOX_ASN1TAG_SNMPPDU_GETREQUEST :
    netwib__ad(NETWOX_ASN1TAG_CLASS_CTX, NETWIB_TRUE,
              NETWOX_ASN1TAG_NUM_SNMPPDU_GETREQUEST);
  case NETWOX_ASN1TAG_SNMPPDU_GETNEXTREQUEST :
    netwib__ad(NETWOX_ASN1TAG_CLASS_CTX, NETWIB_TRUE,
              NETWOX_ASN1TAG_NUM_SNMPPDU_GETNEXTREQUEST);
  case NETWOX_ASN1TAG_SNMPPDU_RESPONSE :
    netwib__ad(NETWOX_ASN1TAG_CLASS_CTX, NETWIB_TRUE,
              NETWOX_ASN1TAG_NUM_SNMPPDU_RESPONSE);
  case NETWOX_ASN1TAG_SNMPPDU_SETREQUEST :
    netwib__ad(NETWOX_ASN1TAG_CLASS_CTX, NETWIB_TRUE,
              NETWOX_ASN1TAG_NUM_SNMPPDU_SETREQUEST);
  case NETWOX_ASN1TAG_SNMPPDU_TRAP :
    netwib__ad(NETWOX_ASN1TAG_CLASS_CTX, NETWIB_TRUE,
              NETWOX_ASN1TAG_NUM_SNMPPDU_TRAP);
  case NETWOX_ASN1TAG_SNMPPDU_GETBULKREQUEST :
    netwib__ad(NETWOX_ASN1TAG_CLASS_CTX, NETWIB_TRUE,
              NETWOX_ASN1TAG_NUM_SNMPPDU_GETBULKREQUEST);
  case NETWOX_ASN1TAG_SNMPPDU_INFORMREQUEST :
    netwib__ad(NETWOX_ASN1TAG_CLASS_CTX, NETWIB_TRUE,
              NETWOX_ASN1TAG_NUM_SNMPPDU_INFORMREQUEST);
  case NETWOX_ASN1TAG_SNMPPDU_TRAP2 :
    netwib__ad(NETWOX_ASN1TAG_CLASS_CTX, NETWIB_TRUE,
              NETWOX_ASN1TAG_NUM_SNMPPDU_TRAP2);
  case NETWOX_ASN1TAG_SNMPPDU_REPORT :
    netwib__ad(NETWOX_ASN1TAG_CLASS_CTX, NETWIB_TRUE,
              NETWOX_ASN1TAG_NUM_SNMPPDU_REPORT);
  case NETWOX_ASN1TAG_SNMPVAR_NOSUCHOBJECT :
    netwib__ad(NETWOX_ASN1TAG_CLASS_CTX, NETWIB_FALSE,
              NETWOX_ASN1TAG_NUM_SNMPVAR_NOSUCHOBJECT);
  case NETWOX_ASN1TAG_SNMPVAR_NOSUCHINSTANCE :
    netwib__ad(NETWOX_ASN1TAG_CLASS_CTX, NETWIB_FALSE,
              NETWOX_ASN1TAG_NUM_SNMPVAR_NOSUCHINSTANCE);
  case NETWOX_ASN1TAG_SNMPVAR_ENDOFMIBVIEW :
    netwib__ad(NETWOX_ASN1TAG_CLASS_CTX, NETWIB_FALSE,
              NETWOX_ASN1TAG_NUM_SNMPVAR_ENDOFMIBVIEW);
  default :
    return(NETWIB_ERR_NOTCONVERTED);
  }

  return(NETWIB_ERR_OK);
}
