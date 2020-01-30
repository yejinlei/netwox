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
netwib_err netwib_ports_init(netwib_ports_inittype inittype,
                             netwib_ports **ppports)
{
  netwib_ports *pports;
  netwib_err ret;

  /* parameter verification */
  if (ppports == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* allocate needed memory to store pports */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_ports), (netwib_ptr*)&pports));
  *ppports = pports;

  ret = netwib_priv_ranges_init(inittype, NETWIB_PORT_LEN, &pports->ranges);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free((netwib_ptr*)&pports));
    return(ret);
  }

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_add_ports(pports));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ports_close(netwib_ports **ppports)
{
  netwib_ports *pports;

  /* parameter verification */
  if (ppports == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pports = *ppports;

  /* remove items */
  netwib_er(netwib_priv_ranges_close(&pports->ranges));

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_del_ports(pports));
#endif

  /* free memory */
  netwib_er(netwib_ptr_free((netwib_ptr*)&pports));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ports_add_port(netwib_ports *pports,
                                 netwib_port port)
{
  netwib_byte portarray[NETWIB_PORT_LEN];

  /* parameter verification */
  if (pports == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ports(pports));
#endif

  /* add item */
  portarray[0] = netwib_c2_uint16_0(port);
  portarray[1] = netwib_c2_uint16_1(port);
  netwib_er(netwib_priv_ranges_add(&pports->ranges, portarray));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ports_add_portrange(netwib_ports *pports,
                                      netwib_port infport,
                                      netwib_port support)
{
  netwib_byte infportarray[NETWIB_PORT_LEN], supportarray[NETWIB_PORT_LEN];

  /* parameter verification */
  if (pports == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ports(pports));
#endif

  /* add item */
  infportarray[0] = netwib_c2_uint16_0(infport);
  infportarray[1] = netwib_c2_uint16_1(infport);
  supportarray[0] = netwib_c2_uint16_0(support);
  supportarray[1] = netwib_c2_uint16_1(support);
  netwib_er(netwib_priv_ranges_add_range(&pports->ranges, infportarray,
                                         supportarray));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ports_add_ports(netwib_ports *pports,
                                  netwib_constports *pportstoadd)
{
  netwib_priv_ranges_index rangesindex;
  netwib_byte infportarray[NETWIB_PORT_LEN], supportarray[NETWIB_PORT_LEN];
  netwib_err ret;

  /* parameter verification */
  if (pports == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ports(pports));
  netwib_er(netwib_debug_leak_valid_ports(pportstoadd));
#endif

  netwib_er(netwib_priv_ranges_index_init(&pportstoadd->ranges, &rangesindex));
  ret = NETWIB_ERR_OK;
  while(NETWIB_TRUE) {
    ret = netwib_priv_ranges_index_next_range(&rangesindex, infportarray,
                                             supportarray);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_er(netwib_priv_ranges_add_range(&pports->ranges, infportarray,
                                         supportarray));
  }
  netwib_er(netwib_priv_ranges_index_close(&rangesindex));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_ports_add_buf(netwib_conststring str,
                                            netwib_data array,
                                            netwib_priv_ranges_separator
                                            *pseparator)
{
  netwib_conststring pc;
  netwib_uint32 infport, support, numdigitfound, *pport;
  netwib_priv_ranges_separator separator;
  netwib_char c;

  pc = str;
  numdigitfound = 0;
  separator = NETWIB_PRIV_RANGES_SEPARATOR_NONE;
  infport = 0;
  support = 0;
  pport = &infport;
  while(NETWIB_TRUE) {
    c = *pc;
    if (c == '\0') {
      if (numdigitfound == 0) return(NETWIB_ERR_NOTCONVERTED);
      break;
    } else if (c == '-') {
      if (separator == NETWIB_PRIV_RANGES_SEPARATOR_DASH) {
        return(NETWIB_ERR_NOTCONVERTED); /* forbid two separators */
      }
      separator = NETWIB_PRIV_RANGES_SEPARATOR_DASH;
      numdigitfound = 0;
      pport = &support;
    } else if (netwib_c2_isdigit(c)) {
      if (numdigitfound == 5) return(NETWIB_ERR_NOTCONVERTED);
      numdigitfound++;
      *pport = 10*(*pport) + netwib_c2_cto9(c);
      if (*pport > 0xFFFF) return(NETWIB_ERR_NOTCONVERTED);
    } else {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    pc++;
  }

  array[0] = netwib_c2_uint16_0(infport);
  array[1] = netwib_c2_uint16_1(infport);
  *pseparator = separator;
  if (separator == NETWIB_PRIV_RANGES_SEPARATOR_DASH) {
    if (infport > support) return(NETWIB_ERR_NOTCONVERTED);
    array[2] = netwib_c2_uint16_0(support);
    array[3] = netwib_c2_uint16_1(support);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ports_add_buf(netwib_ports *pports,
                                netwib_constbuf *pbuf)
{
  netwib_err ret;

  ret = netwib_priv_ranges_add_buf(&pports->ranges, pbuf,
                                   &netwib_priv_ports_add_buf);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_priv_errmsg_string("bad port list: "));
    netwib_er(netwib_priv_errmsg_append_buf(pbuf));
  }
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ports_add_kbd(netwib_ports *pports,
                                netwib_constbuf *pmessage,
                                netwib_constbuf *pdefaultlist)
{
  netwib_buf buf;
  netwib_ports *plocalports;
  netwib_char prompt;
  netwib_err ret;

  netwib_er(netwib_ports_init(pports->ranges.inittype, &plocalports));
  if (pdefaultlist != NULL) {
    ret = netwib_ports_add_buf(plocalports, pdefaultlist);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwib_ports_close(&plocalports));
      return(ret);
    }
    netwib_er(netwib_priv_ranges_del_all(&plocalports->ranges));
  }

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  prompt = ':';
  while (NETWIB_TRUE) {
    netwib_er(netwib_priv_kbd_buf_append(pmessage, pdefaultlist,
                                         NETWIB_TRUE, prompt, NETWIB_FALSE,
                                         &buf));
    if (netwib__buf_ref_data_size(&buf) == 0) {
      if (pdefaultlist != NULL) {
        netwib_er(netwib_ports_add_buf(pports, pdefaultlist));
        break;
      }
    }
    ret = netwib_ports_add_buf(plocalports, &buf);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_ports_add_ports(pports, plocalports));
      break;
    }
    netwib_er(netwib_priv_ranges_del_all(&plocalports->ranges));
    netwib__buf_reinit(&buf);
    prompt = '>';
  }

  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_ports_close(&plocalports));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ports_del_port(netwib_ports *pports,
                                 netwib_port port)
{
  netwib_byte portarray[NETWIB_PORT_LEN];

  /* parameter verification */
  if (pports == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ports(pports));
#endif

  /* add item */
  portarray[0] = netwib_c2_uint16_0(port);
  portarray[1] = netwib_c2_uint16_1(port);
  netwib_er(netwib_priv_ranges_del(&pports->ranges, portarray));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ports_del_portrange(netwib_ports *pports,
                                      netwib_port infport,
                                      netwib_port support)
{
  netwib_byte infportarray[NETWIB_PORT_LEN], supportarray[NETWIB_PORT_LEN];

  /* parameter verification */
  if (pports == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ports(pports));
#endif

  /* add item */
  infportarray[0] = netwib_c2_uint16_0(infport);
  infportarray[1] = netwib_c2_uint16_1(infport);
  supportarray[0] = netwib_c2_uint16_0(support);
  supportarray[1] = netwib_c2_uint16_1(support);
  netwib_er(netwib_priv_ranges_del_range(&pports->ranges, infportarray,
                                         supportarray));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ports_del_ports(netwib_ports *pports,
                                  netwib_constports *pportstodel)
{
  netwib_priv_ranges_index rangesindex;
  netwib_byte infportarray[NETWIB_PORT_LEN], supportarray[NETWIB_PORT_LEN];
  netwib_err ret;

  /* parameter verification */
  if (pports == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ports(pports));
  netwib_er(netwib_debug_leak_valid_ports(pportstodel));
#endif

  netwib_er(netwib_priv_ranges_index_init(&pportstodel->ranges, &rangesindex));
  ret = NETWIB_ERR_OK;
  while(NETWIB_TRUE) {
    ret = netwib_priv_ranges_index_next_range(&rangesindex, infportarray,
                                              supportarray);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_er(netwib_priv_ranges_del_range(&pports->ranges, infportarray,
                                           supportarray));
  }
  netwib_er(netwib_priv_ranges_index_close(&rangesindex));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ports_contains_port(netwib_constports *pports,
                                      netwib_port port,
                                      netwib_bool *pyes)
{
  netwib_byte portarray[NETWIB_PORT_LEN];

  /* parameter verification */
  if (pports == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ports(pports));
#endif

  /* add item */
  portarray[0] = netwib_c2_uint16_0(port);
  portarray[1] = netwib_c2_uint16_1(port);
  netwib_er(netwib_priv_ranges_contains(&pports->ranges, portarray, pyes));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ports_contains_portrange(netwib_constports *pports,
                                           netwib_port infport,
                                           netwib_port support,
                                           netwib_bool *pyes)
{
  netwib_byte infportarray[NETWIB_PORT_LEN], supportarray[NETWIB_PORT_LEN];

  /* parameter verification */
  if (pports == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ports(pports));
#endif

  /* add item */
  infportarray[0] = netwib_c2_uint16_0(infport);
  infportarray[1] = netwib_c2_uint16_1(infport);
  supportarray[0] = netwib_c2_uint16_0(support);
  supportarray[1] = netwib_c2_uint16_1(support);
  netwib_er(netwib_priv_ranges_contains_range(&pports->ranges, infportarray,
                                              supportarray, pyes));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_ports(netwib_constports *pports,
                                   netwib_buf *pbuf)
{
  netwib_priv_ranges_index rangesindex;
  netwib_byte infportarray[NETWIB_PORT_LEN], supportarray[NETWIB_PORT_LEN];
  netwib_port infport, support;
  netwib_bool needcomma;
  netwib_uint32 savedsize=0;
  netwib_err ret;

  /* parameter verification */
  if (pports == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ports(pports));
#endif

  /* save position in case of error */
  if (pbuf != NULL) {
    savedsize = netwib__buf_ref_data_size(pbuf);
  }

  netwib_er(netwib_priv_ranges_index_init(&pports->ranges, &rangesindex));
  ret = NETWIB_ERR_OK;
  needcomma = NETWIB_FALSE;
  while(NETWIB_TRUE) {
    ret = netwib_priv_ranges_index_next_range(&rangesindex, infportarray,
                                              supportarray);
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
    infport = netwib_c2_uint16_2(infportarray[0], infportarray[1]);
    support = netwib_c2_uint16_2(supportarray[0], supportarray[1]);
    ret = netwib_buf_append_port(infport, pbuf);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (infport != support) {
      ret = netwib_buf_append_byte('-', pbuf);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
      ret = netwib_buf_append_port(support, pbuf);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
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
