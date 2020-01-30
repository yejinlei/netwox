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
netwib_err netwib_ips_init(netwib_ips_inittype inittype,
                           netwib_ips **ppips)
{
  netwib_ips *pips;
  netwib_err ret;

  /* parameter verification */
  if (ppips == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* allocate needed memory to store pips */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_ips), (netwib_ptr*)&pips));
  *ppips = pips;

  ret = netwib_priv_ranges_init(inittype, NETWIB_IPS_LEN, &pips->ranges);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free((netwib_ptr*)&pips));
    return(ret);
  }

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_add_ips(pips));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ips_close(netwib_ips **ppips)
{
  netwib_ips *pips;

  /* parameter verification */
  if (ppips == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pips = *ppips;

  /* remove items */
  netwib_er(netwib_priv_ranges_close(&pips->ranges));

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_del_ips(pips));
#endif

  /* free memory */
  netwib_er(netwib_ptr_free((netwib_ptr*)&pips));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ips_add_ip(netwib_ips *pips,
                             netwib_constip *pip)
{
  netwib_byte iparray[NETWIB_IPS_LEN];

  /* parameter verification */
  if (pips == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ips(pips));
#endif

  /* add item */
  netwib_er(netwib_priv_ips_array_init_ip(pip, iparray));
  netwib_er(netwib_priv_ranges_add(&pips->ranges, iparray));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ips_add_iprange(netwib_ips *pips,
                                  netwib_constip *pinfip,
                                  netwib_constip *psupip)
{
  netwib_byte infiparray[NETWIB_IPS_LEN], supiparray[NETWIB_IPS_LEN];

  /* parameter verification */
  if (pips == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ips(pips));
#endif

  /* add item */
  netwib_er(netwib_priv_ips_array_init_ip(pinfip, infiparray));
  netwib_er(netwib_priv_ips_array_init_ip(psupip, supiparray));
  netwib_er(netwib_priv_ranges_add_range(&pips->ranges, infiparray,
                                         supiparray));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ips_add_ips(netwib_ips *pips,
                              netwib_constips *pipstoadd)
{
  netwib_priv_ranges_index rangesindex;
  netwib_byte infiparray[NETWIB_IPS_LEN], supiparray[NETWIB_IPS_LEN];
  netwib_err ret;

  /* parameter verification */
  if (pips == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ips(pips));
  netwib_er(netwib_debug_leak_valid_ips(pipstoadd));
#endif

  netwib_er(netwib_priv_ranges_index_init(&pipstoadd->ranges, &rangesindex));
  ret = NETWIB_ERR_OK;
  while(NETWIB_TRUE) {
    ret = netwib_priv_ranges_index_next_range(&rangesindex, infiparray,
                                              supiparray);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_er(netwib_priv_ranges_add_range(&pips->ranges, infiparray,
                                           supiparray));
  }
  netwib_er(netwib_priv_ranges_index_close(&rangesindex));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ips_add_buf(netwib_ips *pips,
                              netwib_constbuf *pbuf)
{
  netwib_uint32 itemsize;
  netwib_buf buf;
  netwib_string pc, pcstart, pcend, pstr;
  netwib_bool usedel;
  netwib_err ret;

  netwib__constbuf_ref_string(pbuf, pc, bufstorage,
                              netwib_ips_add_buf(pips, &bufstorage));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    /* skip spaces */
    while (*pc == ' ' || *pc == '\t') {
      pc++;
    }
    /* check if there is '!' which indicate to del instead of add */
    usedel = NETWIB_FALSE;
    if (*pc == '!') {
      pc++;
      usedel = NETWIB_TRUE;
    }
    /* skip spaces */
    while (*pc == ' ' || *pc == '\t') {
      pc++;
    }
    /* now the real start */
    pcstart = pc;
    /* find end */
    while (*pc != '\0' && *pc != ',' && *pc != ' ' && *pc != '\t') {
      pc++;
    }
    pcend = pc;
    /* put this data in the buffer */
    itemsize = pcend - pcstart;
    if (itemsize) {
      netwib_er(netwib_buf_wantspace(&buf, itemsize+1, (netwib_data*)&pstr));
      netwib_c_memcpy(pstr, pcstart, itemsize);
      pstr[itemsize] = '\0';
      ret = netwib_priv_ips_add_string(&pips->ranges, pstr, usedel);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
    }
    /* end */
    if (*pc == '\0') {
      break;
    }
    pc++;
    /* reinit for next call */
    netwib__buf_reinit(&buf);
  }
  netwib_er(netwib_buf_close(&buf));

  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_priv_errmsg_string("bad IP list: "));
    netwib_er(netwib_priv_errmsg_append_buf(pbuf));
  }
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ips_add_kbd(netwib_ips *pips,
                              netwib_constbuf *pmessage,
                              netwib_constbuf *pdefaultlist)
{
  netwib_buf buf;
  netwib_ips *plocalips;
  netwib_char prompt;
  netwib_err ret;

  netwib_er(netwib_ips_init(pips->ranges.inittype, &plocalips));
  if (pdefaultlist != NULL) {
    ret = netwib_ips_add_buf(plocalips, pdefaultlist);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwib_ips_close(&plocalips));
      return(ret);
    }
    netwib_er(netwib_priv_ranges_del_all(&plocalips->ranges));
  }

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  prompt = ':';
  while (NETWIB_TRUE) {
    netwib_er(netwib_priv_kbd_buf_append(pmessage, pdefaultlist,
                                         NETWIB_TRUE, prompt, NETWIB_FALSE,
                                         &buf));
    if (netwib__buf_ref_data_size(&buf) == 0) {
      if (pdefaultlist != NULL) {
        netwib_er(netwib_ips_add_buf(pips, pdefaultlist));
        break;
      }
    }
    ret = netwib_ips_add_buf(plocalips, &buf);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_ips_add_ips(pips, plocalips));
      break;
    }
    netwib_er(netwib_priv_ranges_del_all(&plocalips->ranges));
    netwib__buf_reinit(&buf);
    prompt = '>';
  }

  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_ips_close(&plocalips));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ips_del_ip(netwib_ips *pips,
                             netwib_constip *pip)
{
  netwib_byte iparray[NETWIB_IPS_LEN];

  /* parameter verification */
  if (pips == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ips(pips));
#endif

  /* add item */
  netwib_er(netwib_priv_ips_array_init_ip(pip, iparray));
  netwib_er(netwib_priv_ranges_del(&pips->ranges, iparray));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ips_del_iprange(netwib_ips *pips,
                                  netwib_constip *pinfip,
                                  netwib_constip *psupip)
{
  netwib_byte infiparray[NETWIB_IPS_LEN], supiparray[NETWIB_IPS_LEN];

  /* parameter verification */
  if (pips == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ips(pips));
#endif

  /* add item */
  netwib_er(netwib_priv_ips_array_init_ip(pinfip, infiparray));
  netwib_er(netwib_priv_ips_array_init_ip(psupip, supiparray));
  netwib_er(netwib_priv_ranges_del_range(&pips->ranges, infiparray,
                                         supiparray));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ips_del_ips(netwib_ips *pips,
                              netwib_constips *pipstodel)
{
  netwib_priv_ranges_index rangesindex;
  netwib_byte infiparray[NETWIB_IPS_LEN], supiparray[NETWIB_IPS_LEN];
  netwib_err ret;

  /* parameter verification */
  if (pips == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ips(pips));
  netwib_er(netwib_debug_leak_valid_ips(pipstodel));
#endif

  netwib_er(netwib_priv_ranges_index_init(&pipstodel->ranges, &rangesindex));
  ret = NETWIB_ERR_OK;
  while(NETWIB_TRUE) {
    ret = netwib_priv_ranges_index_next_range(&rangesindex, infiparray,
                                              supiparray);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_er(netwib_priv_ranges_del_range(&pips->ranges, infiparray,
                                           supiparray));
  }
  netwib_er(netwib_priv_ranges_index_close(&rangesindex));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ips_contains_ip(netwib_constips *pips,
                                  netwib_constip *pip,
                                  netwib_bool *pyes)
{
  netwib_byte iparray[NETWIB_IPS_LEN];

  /* parameter verification */
  if (pips == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ips(pips));
#endif

  /* add item */
  netwib_er(netwib_priv_ips_array_init_ip(pip, iparray));
  netwib_er(netwib_priv_ranges_contains(&pips->ranges, iparray, pyes));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ips_contains_iprange(netwib_constips *pips,
                                       netwib_constip *pinfip,
                                       netwib_constip *psupip,
                                       netwib_bool *pyes)
{
  netwib_byte infiparray[NETWIB_IPS_LEN], supiparray[NETWIB_IPS_LEN];

  /* parameter verification */
  if (pips == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ips(pips));
#endif

  /* add item */
  netwib_er(netwib_priv_ips_array_init_ip(pinfip, infiparray));
  netwib_er(netwib_priv_ips_array_init_ip(psupip, supiparray));
  netwib_er(netwib_priv_ranges_contains_range(&pips->ranges, infiparray,
                                              supiparray, pyes));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_buf_append_ips_ip(netwib_ip *pip,
                                                netwib_ips_encodetype encodetype,
                                                netwib_buf *pbuf)
{

  switch(encodetype) {
    case NETWIB_IPS_ENCODETYPE_HNRANGE :
      netwib_er(netwib_buf_append_ip(pip, NETWIB_IP_ENCODETYPE_HN, pbuf));
      break;
    case NETWIB_IPS_ENCODETYPE_IPRANGE :
    case NETWIB_IPS_ENCODETYPE_IPNUMBER :
    case NETWIB_IPS_ENCODETYPE_IPMASK :
      netwib_er(netwib_buf_append_ip(pip, NETWIB_IP_ENCODETYPE_IP, pbuf));
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_buf_append_ips_range(netwib_ip *pinfip,
                                                   netwib_ip *psupip,
                                                   netwib_ips_encodetype encodetype,
                                                   netwib_buf *pbuf)
{
  netwib_uint32 number;
  netwib_ip mask, infip;

  switch(encodetype) {
    case NETWIB_IPS_ENCODETYPE_HNRANGE :
      netwib_er(netwib_buf_append_ip(pinfip, NETWIB_IP_ENCODETYPE_HN,
                                     pbuf));
      netwib_er(netwib_buf_append_byte('=', pbuf));
      netwib_er(netwib_buf_append_ip(psupip, NETWIB_IP_ENCODETYPE_HN,
                                     pbuf));
      break;
    case NETWIB_IPS_ENCODETYPE_IPRANGE :
      netwib_er(netwib_buf_append_ip(pinfip, NETWIB_IP_ENCODETYPE_IP,
                                     pbuf));
      netwib_er(netwib_buf_append_byte('-', pbuf));
      netwib_er(netwib_buf_append_ip(psupip, NETWIB_IP_ENCODETYPE_IP,
                                     pbuf));
      break;
    case NETWIB_IPS_ENCODETYPE_IPNUMBER :
    case NETWIB_IPS_ENCODETYPE_IPMASK :
      /* only support IP4 */
      if (pinfip->iptype == NETWIB_IPTYPE_IP6 ||
          pinfip->iptype == NETWIB_IPTYPE_IP6 ) {
        netwib_er(netwib_buf_append_ip(pinfip, NETWIB_IP_ENCODETYPE_IP,
                                       pbuf));
        netwib_er(netwib_buf_append_byte('-', pbuf));
        netwib_er(netwib_buf_append_ip(psupip, NETWIB_IP_ENCODETYPE_IP,
                                       pbuf));
        break;
      }
      /* find mask/number */
      number = 32;
      netwib_er(netwib_ip_init_ip4(0xFFFFFFFFu, &mask));
      while (NETWIB_TRUE) {
        if ((pinfip->ipvalue.ip4 & mask.ipvalue.ip4) ==
            (psupip->ipvalue.ip4 & mask.ipvalue.ip4)) {
          if ((pinfip->ipvalue.ip4 & ~mask.ipvalue.ip4) == 0) {
            if ((psupip->ipvalue.ip4 & ~mask.ipvalue.ip4) ==
                ~mask.ipvalue.ip4) {
              break;
            }
          }
        }
        if (number == 0) {
          /* cannot be represented, so use a simple form */
          netwib_er(netwib_buf_append_ip(pinfip, NETWIB_IP_ENCODETYPE_IP,
                                         pbuf));
          netwib_er(netwib_buf_append_byte('-', pbuf));
          netwib_er(netwib_buf_append_ip(psupip, NETWIB_IP_ENCODETYPE_IP,
                                         pbuf));
          return(NETWIB_ERR_OK);
        }
        number--;
        mask.ipvalue.ip4 <<= 1;
      }
      /* append */
      netwib_er(netwib_ip_init_ip4(pinfip->ipvalue.ip4 & mask.ipvalue.ip4,
                                   &infip));
      if (encodetype == NETWIB_IPS_ENCODETYPE_IPNUMBER) {
        netwib_er(netwib_buf_append_fmt(pbuf, "%{ip}/%{uint32}", &infip,
                                        number));
      } else {
        netwib_er(netwib_buf_append_ip(&infip, NETWIB_IP_ENCODETYPE_IP,
                                       pbuf));
        netwib_er(netwib_buf_append_byte('/', pbuf));
        netwib_er(netwib_buf_append_ip(&mask, NETWIB_IP_ENCODETYPE_IP,
                                       pbuf));
      }
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_ips(netwib_constips *pips,
                                 netwib_ips_encodetype encodetype,
                                 netwib_buf *pbuf)
{
  netwib_priv_ranges_index rangesindex;
  netwib_byte infiparray[NETWIB_IPS_LEN], supiparray[NETWIB_IPS_LEN];
  netwib_ip infip, supip;
  netwib_bool needcomma;
  netwib_uint32 savedsize=0;
  netwib_cmp cmp;
  netwib_err ret;

  /* parameter verification */
  if (pips == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ips(pips));
#endif

  /* save position in case of error */
  if (pbuf != NULL) {
   savedsize = netwib__buf_ref_data_size(pbuf);
  }

  netwib_er(netwib_priv_ranges_index_init(&pips->ranges, &rangesindex));
  needcomma = NETWIB_FALSE;
  ret = NETWIB_ERR_OK;
  while(NETWIB_TRUE) {
    ret = netwib_priv_ranges_index_next_range(&rangesindex, infiparray,
                                              supiparray);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    if (needcomma) {
      ret = netwib_buf_append_byte(',', pbuf);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
    } else {
      needcomma = NETWIB_TRUE;
    }
    netwib_er(netwib_priv_ips_ip_init_array(infiparray, &infip));
    netwib_er(netwib_priv_ips_ip_init_array(supiparray, &supip));
    ret = netwib_ip_cmp(&infip, &supip, &cmp);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (cmp == NETWIB_CMP_EQ) {
      ret = netwib_priv_buf_append_ips_ip(&infip, encodetype, pbuf);
    } else {
      ret = netwib_priv_buf_append_ips_range(&infip, &supip, encodetype, pbuf);
    }
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_priv_ranges_index_close(&rangesindex));

  /* on space error, restore first pos */
  if (ret != NETWIB_ERR_OK) {
    if (pbuf != NULL) {
      pbuf->endoffset = pbuf->beginoffset + savedsize;
    }
  }
  return(ret);
}
