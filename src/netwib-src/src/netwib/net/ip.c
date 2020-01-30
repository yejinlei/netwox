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
netwib_err netwib_ip_init_ip4_fields(netwib_byte a,
                                     netwib_byte b,
                                     netwib_byte c,
                                     netwib_byte d,
                                     netwib_ip *pip)
{

  if (pip != NULL) {
    pip->iptype = NETWIB_IPTYPE_IP4;
    pip->ipvalue.ip4 = netwib_c2_uint32_4(a, b, c, d);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ip_init_ip4(netwib_ip4 ip4,
                              netwib_ip *pip)
{

  if (pip != NULL) {
    pip->iptype = NETWIB_IPTYPE_IP4;
    pip->ipvalue.ip4 = ip4;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ip4_init_ip(netwib_constip *pip,
                              netwib_ip4 *pip4)
{

  switch(pip->iptype) {
    case NETWIB_IPTYPE_IP4 :
      if (pip4 != NULL) {
        *pip4 = pip->ipvalue.ip4;
      }
      break;
    case NETWIB_IPTYPE_IP6 :
      netwib_er(netwib_priv_ip_ip4_init_ip6(&pip->ipvalue.ip6, pip4));
      break;
    default :
      return(NETWIB_ERR_PAIPTYPE);
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ip_init_ip6_fields(netwib_uint32 a,
                                     netwib_uint32 b,
                                     netwib_uint32 c,
                                     netwib_uint32 d,
                                     netwib_ip *pip)
{
  if (pip != NULL) {
    pip->iptype = NETWIB_IPTYPE_IP6;
    pip->ipvalue.ip6.b[0] = netwib_c2_uint32_0(a);
    pip->ipvalue.ip6.b[1] = netwib_c2_uint32_1(a);
    pip->ipvalue.ip6.b[2] = netwib_c2_uint32_2(a);
    pip->ipvalue.ip6.b[3] = netwib_c2_uint32_3(a);
    pip->ipvalue.ip6.b[4] = netwib_c2_uint32_0(b);
    pip->ipvalue.ip6.b[5] = netwib_c2_uint32_1(b);
    pip->ipvalue.ip6.b[6] = netwib_c2_uint32_2(b);
    pip->ipvalue.ip6.b[7] = netwib_c2_uint32_3(b);
    pip->ipvalue.ip6.b[8] = netwib_c2_uint32_0(c);
    pip->ipvalue.ip6.b[9] = netwib_c2_uint32_1(c);
    pip->ipvalue.ip6.b[10] = netwib_c2_uint32_2(c);
    pip->ipvalue.ip6.b[11] = netwib_c2_uint32_3(c);
    pip->ipvalue.ip6.b[12] = netwib_c2_uint32_0(d);
    pip->ipvalue.ip6.b[13] = netwib_c2_uint32_1(d);
    pip->ipvalue.ip6.b[14] = netwib_c2_uint32_2(d);
    pip->ipvalue.ip6.b[15] = netwib_c2_uint32_3(d);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ip_init_ip6(netwib_constip6 *pip6,
                              netwib_ip *pip)
{

  if (pip != NULL) {
    pip->iptype = NETWIB_IPTYPE_IP6;
    pip->ipvalue.ip6 = *pip6;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ip6_init_ip(netwib_constip *pip,
                              netwib_ip6 *pip6)
{

  switch(pip->iptype) {
    case NETWIB_IPTYPE_IP4 :
      netwib_er(netwib_priv_ip_ip6_init_ip4(pip->ipvalue.ip4, pip6));
      break;
    case NETWIB_IPTYPE_IP6 :
      if (pip6 != NULL) {
        netwib_c_memcpy(pip6->b, pip->ipvalue.ip6.b, NETWIB_IP6_LEN);
      }
      break;
    default :
      return(NETWIB_ERR_PAIPTYPE);
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ip_init_buf(netwib_constbuf *pbuf,
                              netwib_ip_decodetype decodetype,
                              netwib_ip *pip)
{
  netwib_string str;
  netwib_bool decodeip4, decodeip6, decodehn4, decodehn6;
  netwib_err ret;

  netwib__constbuf_ref_string(pbuf, str, bufstorage,
                              netwib_ip_init_buf(&bufstorage, decodetype, pip));

  /* decide what to decode */
  decodeip4 = NETWIB_FALSE;
  decodeip6 = NETWIB_FALSE;
  decodehn4 = NETWIB_FALSE;
  decodehn6 = NETWIB_FALSE;
  switch(decodetype) {
  case NETWIB_IP_DECODETYPE_IP4 :
    decodeip4 = NETWIB_TRUE;
    break;
  case NETWIB_IP_DECODETYPE_IP6 :
    decodeip6 = NETWIB_TRUE;
    break;
  case NETWIB_IP_DECODETYPE_IP :
    decodeip4 = NETWIB_TRUE;
    decodeip6 = NETWIB_TRUE;
    break;
  case NETWIB_IP_DECODETYPE_HN4 :
    decodehn4 = NETWIB_TRUE;
    break;
  case NETWIB_IP_DECODETYPE_HN6 :
    decodehn6 = NETWIB_TRUE;
    break;
  case NETWIB_IP_DECODETYPE_HN :
    decodehn4 = NETWIB_TRUE;
    decodehn6 = NETWIB_TRUE;
    break;
  case NETWIB_IP_DECODETYPE_IP4HN4 :
    decodeip4 = NETWIB_TRUE;
    decodehn4 = NETWIB_TRUE;
    break;
  case NETWIB_IP_DECODETYPE_IP6HN6 :
    decodeip6 = NETWIB_TRUE;
    decodehn6 = NETWIB_TRUE;
    break;
  case NETWIB_IP_DECODETYPE_IPHN :
    decodeip4 = NETWIB_TRUE;
    decodeip6 = NETWIB_TRUE;
    decodehn4 = NETWIB_TRUE;
    decodehn6 = NETWIB_TRUE;
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  if (decodeip4) {
    ret = netwib_priv_ip_init_sip4(str, pip);
    if (ret == NETWIB_ERR_OK) return(NETWIB_ERR_OK);
  }

  if (decodeip6) {
    ret = netwib_priv_ip_init_sip6(str, pip);
    if (ret == NETWIB_ERR_OK) return(NETWIB_ERR_OK);
  }

  if (decodehn4) {
    ret = netwib_priv_ip_init_hn4(str, pip);
    if (ret == NETWIB_ERR_OK) return(NETWIB_ERR_OK);
  }

  if (decodehn6) {
    ret = netwib_priv_ip_init_hn6(str, pip);
    if (ret == NETWIB_ERR_OK) return(NETWIB_ERR_OK);
  }

  return(NETWIB_ERR_NOTCONVERTED);
}

/*-------------------------------------------------------------*/
#include "pieces/ip_init_eth.c"

/*-------------------------------------------------------------*/
netwib_err netwib_ip_init_kbd(netwib_constbuf *pmessage,
                              netwib_constbuf *pdefaulthn,
                              netwib_ip *pip)
{
  netwib_buf buf;
  netwib_constbuf *pdef;
  netwib_ip defaultip;
  netwib_char prompt;
  netwib_err ret;

  defaultip.iptype = NETWIB_IPTYPE_IP4; /* for compiler warning */
  pdef = NETWIB_BUF_APPEND_KBD_NODEF;
  if (pdefaulthn != NETWIB_IP_INIT_KBD_NODEF) {
    netwib_er(netwib_ip_init_buf_best(pdefaulthn, &defaultip));
    pdef = pdefaulthn;
  }

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  prompt = ':';
  while (NETWIB_TRUE) {
    netwib_er(netwib_priv_kbd_buf_append(pmessage, pdef, NETWIB_TRUE, prompt,
                                         NETWIB_FALSE, &buf));
    if (netwib__buf_ref_data_size(&buf) == 0) {
      if (pdefaulthn != NETWIB_IP_INIT_KBD_NODEF) {
        if (pip != NULL) *pip = defaultip;
        break;
      }
    }
    ret = netwib_ip_init_buf_best(&buf, pip);
    if (ret == NETWIB_ERR_OK) {
      break;
    }
    netwib__buf_reinit(&buf);
    prompt = '>';
  }

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ip_cmp(netwib_constip *pip1,
                         netwib_constip *pip2,
                         netwib_cmp *pcmp)
{
  netwib_ip4 ip41=0, ip42=0;
  netwib_err ret;
  int reti;

  if (pcmp == NULL) {
    return(NETWIB_ERR_OK);
  }

  if (pip1->iptype == NETWIB_IPTYPE_IP6 && pip2->iptype == NETWIB_IPTYPE_IP6) {
    reti = netwib_c_memcmp(pip1->ipvalue.ip6.b, pip2->ipvalue.ip6.b,
                           NETWIB_IP6_LEN);
    if (reti == 0) {
      *pcmp = NETWIB_CMP_EQ;
    } else if (reti < 0) {
      *pcmp = NETWIB_CMP_LT;
    } else {
      *pcmp = NETWIB_CMP_GT;
    }
    return(NETWIB_ERR_OK);
  }

  switch(pip1->iptype) {
    case NETWIB_IPTYPE_IP4 :
      ip41 = pip1->ipvalue.ip4;
      break;
    case NETWIB_IPTYPE_IP6 :
      ret = netwib_priv_ip_ip4_init_ip6(&pip1->ipvalue.ip6, &ip41);
      if (ret != NETWIB_ERR_OK) {
        *pcmp = NETWIB_CMP_GT;
        return(NETWIB_ERR_OK);
      }
      break;
    default:
      return(NETWIB_ERR_PAIPTYPE);
  }

  switch(pip2->iptype) {
    case NETWIB_IPTYPE_IP4 :
      ip42 = pip2->ipvalue.ip4;
      break;
    case NETWIB_IPTYPE_IP6 :
      ret = netwib_priv_ip_ip4_init_ip6(&pip2->ipvalue.ip6, &ip42);
      if (ret != NETWIB_ERR_OK) {
        *pcmp = NETWIB_CMP_LT;
        return(NETWIB_ERR_OK);
      }
      break;
    default:
      return(NETWIB_ERR_PAIPTYPE);
  }

  if (ip41 == ip42) {
    *pcmp = NETWIB_CMP_EQ;
  } else if (ip41 < ip42) {
    *pcmp = NETWIB_CMP_LT;
  } else {
    *pcmp = NETWIB_CMP_GT;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_ip(netwib_constip *pip,
                                netwib_ip_encodetype encodetype,
                                netwib_buf *pbuf)
{
  netwib_uint32 savedsize;
  netwib_err ret = NETWIB_ERR_LOINTERNALERROR;

  if (pbuf == NULL) {
    netwib_buf buf;
    /* we work on a fake buf, just to test error */
    netwib_er(netwib_buf_init_mallocdefault(&buf));
    ret = netwib_buf_append_ip(pip, encodetype, &buf);
    netwib_er(netwib_buf_close(&buf));
    return(ret);
  }

  /* save position in case of error */
  savedsize = netwib__buf_ref_data_size(pbuf);

  switch(encodetype) {
  case NETWIB_IP_ENCODETYPE_IP :
    switch(pip->iptype) {
    case NETWIB_IPTYPE_IP4 :
      ret = netwib_priv_ip_buf_append_ip4(pip, pbuf);
      break;
    case NETWIB_IPTYPE_IP6 :
      ret = netwib_priv_ip_buf_append_ip6(pip, pbuf);
      break;
    default :
      return(NETWIB_ERR_PAIPTYPE);
    }
    break;
  case NETWIB_IP_ENCODETYPE_HN :
    ret = netwib_priv_ip_buf_append_hn(pip, pbuf);
    break;
  case NETWIB_IP_ENCODETYPE_HNIP :
    ret = netwib_priv_ip_buf_append_hn(pip, pbuf);
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      switch(pip->iptype) {
      case NETWIB_IPTYPE_IP4 :
        ret = netwib_priv_ip_buf_append_ip4(pip, pbuf);
        break;
      case NETWIB_IPTYPE_IP6 :
        ret = netwib_priv_ip_buf_append_ip6(pip, pbuf);
        break;
      default :
        return(NETWIB_ERR_PAIPTYPE);
      }
    }
    break;
  case NETWIB_IP_ENCODETYPE_HNS :
    ret = netwib_priv_ip_buf_append_hns(pip, pbuf);
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  /* on error, restore first pos */
  if (ret != NETWIB_ERR_OK) {
    pbuf->endoffset = pbuf->beginoffset + savedsize;
  }
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_iptype(netwib_iptype type,
                                    netwib_buf *pbuf)
{
  netwib_conststring pc=NULL;

  switch(type) {
    case NETWIB_IPTYPE_IP4 :
      pc = "IPv4";
      break;
    case NETWIB_IPTYPE_IP6 :
      pc = "IPv6";
      break;
    default :
      return(NETWIB_ERR_PAIPTYPE);
  }

  netwib_er(netwib_buf_append_string(pc, pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#define NETWIB_PRIV_IPTYPE_MAX 10
netwib_err netwib_iptype_init_kbd(netwib_constbuf *pmessage,
                                  netwib_iptype defaulttype,
                                  netwib_iptype *ptype)
{
  netwib_iptype array[NETWIB_PRIV_IPTYPE_MAX];
  netwib_buf msg, buf;
  netwib_uint32 i, choice, defaultchoice;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  if (pmessage != NULL) {
    netwib_er(netwib_buf_append_buf(pmessage, &buf));
    netwib_er(netwib_buf_append_string("\n", &buf));
  }

  i = 0;
  defaultchoice = NETWIB_UINT32_INIT_KBD_NODEF;

#define netwib_iptype_lih(ot) {netwib_er(netwib_buf_append_fmt(&buf, " %{r 2;uint32} - ", i)); netwib_er(netwib_buf_append_iptype(ot, &buf)); netwib_er(netwib_buf_append_fmt(&buf, "\n")); if (defaulttype == ot) defaultchoice = i; array[i++] = ot; }

  netwib_iptype_lih(NETWIB_IPTYPE_IP4);
  netwib_iptype_lih(NETWIB_IPTYPE_IP6);

  if (i >= NETWIB_PRIV_IPTYPE_MAX) {
    return(NETWIB_ERR_LOINTERNALERROR);
  }
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_buf_close(&buf));

  if (defaulttype == NETWIB_IPTYPE_UNKNOWN) {
    defaultchoice = NETWIB_UINT32_INIT_KBD_NODEF;
  }

  netwib_er(netwib_buf_init_ext_string("Your choice", &msg));
  netwib_er(netwib_uint32_init_kbd(&msg, 0, i-1, defaultchoice,
                                 &choice));

  if (ptype != NULL) *ptype = array[choice];
  return(NETWIB_ERR_OK);
}
