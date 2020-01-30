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
netwib_err netwib_eths_init(netwib_eths_inittype inittype,
                            netwib_eths **ppeths)
{
  netwib_eths *peths;
  netwib_err ret;

  /* parameter verification */
  if (ppeths == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* allocate needed memory to store peths */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_eths), (netwib_ptr*)&peths));
  *ppeths = peths;

  ret = netwib_priv_ranges_init(inittype, NETWIB_ETH_LEN, &peths->ranges);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free((netwib_ptr*)&peths));
    return(ret);
  }

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_add_eths(peths));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_eths_close(netwib_eths **ppeths)
{
  netwib_eths *peths;

  /* parameter verification */
  if (ppeths == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  peths = *ppeths;

  /* remove items */
  netwib_er(netwib_priv_ranges_close(&peths->ranges));

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_del_eths(peths));
#endif

  /* free memory */
  netwib_er(netwib_ptr_free((netwib_ptr*)&peths));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_eths_add_eth(netwib_eths *peths,
                               netwib_consteth *peth)
{

  /* parameter verification */
  if (peths == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_eths(peths));
#endif

  /* add item */
  netwib_er(netwib_priv_ranges_add(&peths->ranges, peth->b));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_eths_add_ethrange(netwib_eths *peths,
                                    netwib_consteth *pinfeth,
                                    netwib_consteth *psupeth)
{

  /* parameter verification */
  if (peths == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_eths(peths));
#endif

  /* add item */
  netwib_er(netwib_priv_ranges_add_range(&peths->ranges,
                                         pinfeth->b, psupeth->b));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_eths_add_eths(netwib_eths *peths,
                                netwib_consteths *pethstoadd)
{
  netwib_priv_ranges_index rangesindex;
  netwib_byte infarray[NETWIB_ETH_LEN], suparray[NETWIB_ETH_LEN];
  netwib_err ret;

  /* parameter verification */
  if (peths == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_eths(peths));
  netwib_er(netwib_debug_leak_valid_eths(pethstoadd));
#endif

  netwib_er(netwib_priv_ranges_index_init(&pethstoadd->ranges, &rangesindex));
  ret = NETWIB_ERR_OK;
  while(NETWIB_TRUE) {
    ret = netwib_priv_ranges_index_next_range(&rangesindex, infarray,
                                              suparray);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_er(netwib_priv_ranges_add_range(&peths->ranges, infarray,
                                           suparray));
  }
  netwib_er(netwib_priv_ranges_index_close(&rangesindex));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_eths_add_buf(netwib_conststring str,
                                           netwib_data array,
                                           netwib_priv_ranges_separator
                                           *pseparator)
{
  netwib_conststring pc;
  netwib_uint32 i, numdigitfound, fieldvalue;
  netwib_priv_ranges_separator separator;
  netwib_char c;

  /* a range is of the form :
       1:2:3:4:5:6
       1:2:3:4:5:6-5:6:7:8:5:6
       1:2:3:0:5:6/FF:FF:FF:FF:FF:0
       1:2:3:0:5:6/24
       1:2:3/FF:FF:FF:FF:FF:0
       1:2:3/24
       1:2:3:0%FF:FF:FF:FF:FF:O
       1:2:3:0%24
       1:2:3%FF:FF:FF:FF:FF:O
       1:2:3%24
   */

  /* first set default (not needed for mask) */
  netwib_c_memset(array, 0, NETWIB_ETH_LEN);

  /* main loop */
  i = 0;
  separator = NETWIB_PRIV_RANGES_SEPARATOR_NONE;
  numdigitfound = 0;
  fieldvalue = 0;
  pc = str;
  while(NETWIB_TRUE) {
    c = *pc;
    if (c == ':' || c == '\0' || c == '-' || c == '/' || c == '%') {
      /* generic error checking */
      if (i >= 2*NETWIB_ETH_LEN || numdigitfound == 0) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      array[i++] = (netwib_byte)fieldvalue;
      fieldvalue = 0;
      numdigitfound = 0;
      /* specific error checking */
      if (c == '\0') {
        break;
      } else if (c == ':') {
        if (i == NETWIB_ETH_LEN) return(NETWIB_ERR_NOTCONVERTED);
      } else if (c == '-') {
        if (i != NETWIB_ETH_LEN) return(NETWIB_ERR_NOTCONVERTED);
        if (separator != NETWIB_PRIV_RANGES_SEPARATOR_NONE) {
          return(NETWIB_ERR_NOTCONVERTED); /* forbid two separators */
        }
        separator = NETWIB_PRIV_RANGES_SEPARATOR_DASH;
      } else if (c == '/') {
        if (i > NETWIB_ETH_LEN) return(NETWIB_ERR_NOTCONVERTED);
        if (separator != NETWIB_PRIV_RANGES_SEPARATOR_NONE) {
          return(NETWIB_ERR_NOTCONVERTED); /* forbid two separators */
        }
        separator = NETWIB_PRIV_RANGES_SEPARATOR_SLASH;
        i = NETWIB_ETH_LEN;
      } else if (c == '%') {
        if (i > NETWIB_ETH_LEN) return(NETWIB_ERR_NOTCONVERTED);
        if (separator != NETWIB_PRIV_RANGES_SEPARATOR_NONE) {
          return(NETWIB_ERR_NOTCONVERTED); /* forbid two separators */
        }
        separator = NETWIB_PRIV_RANGES_SEPARATOR_PERCENT;
        i = NETWIB_ETH_LEN;
      }
    } else if (netwib_c2_isxdigit(c)) {
      if (numdigitfound == 2) return(NETWIB_ERR_NOTCONVERTED);
      fieldvalue = (fieldvalue<<4) + netwib_c2_cto16(c);
      numdigitfound++;
    } else {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    pc++;
  }

  /* final error checking */
  switch(separator) {
    case NETWIB_PRIV_RANGES_SEPARATOR_NONE :
      if (i != NETWIB_ETH_LEN) return(NETWIB_ERR_NOTCONVERTED);
      break;
    case NETWIB_PRIV_RANGES_SEPARATOR_DASH :
      if (i != 2*NETWIB_ETH_LEN) return(NETWIB_ERR_NOTCONVERTED);
      break;
    case NETWIB_PRIV_RANGES_SEPARATOR_SLASH :
      if (i == NETWIB_ETH_LEN+1) {
        separator = NETWIB_PRIV_RANGES_SEPARATOR_SLASH_NUM;
      } else if (i != 2*NETWIB_ETH_LEN) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      break;
    case NETWIB_PRIV_RANGES_SEPARATOR_PERCENT :
      if (i == NETWIB_ETH_LEN+1) {
        separator = NETWIB_PRIV_RANGES_SEPARATOR_PERCENT_NUM;
      } else if (i != 2*NETWIB_ETH_LEN) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      break;
    default :
      return(NETWIB_ERR_LOINTERNALERROR);
  }

  *pseparator = separator;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_eths_add_buf(netwib_eths *peths,
                               netwib_constbuf *pbuf)
{
  netwib_err ret;

  ret = netwib_priv_ranges_add_buf(&peths->ranges, pbuf,
                                   &netwib_priv_eths_add_buf);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_priv_errmsg_string("bad Ethernet list: "));
    netwib_er(netwib_priv_errmsg_append_buf(pbuf));
  }
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_eths_add_kbd(netwib_eths *peths,
                               netwib_constbuf *pmessage,
                               netwib_constbuf *pdefaultlist)
{
  netwib_buf buf;
  netwib_eths *plocaleths;
  netwib_char prompt;
  netwib_err ret;

  netwib_er(netwib_eths_init(peths->ranges.inittype, &plocaleths));
  if (pdefaultlist != NULL) {
    ret = netwib_eths_add_buf(plocaleths, pdefaultlist);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwib_eths_close(&plocaleths));
      return(ret);
    }
    netwib_er(netwib_priv_ranges_del_all(&plocaleths->ranges));
  }

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  prompt = ':';
  while (NETWIB_TRUE) {
    netwib_er(netwib_priv_kbd_buf_append(pmessage, pdefaultlist,
                                         NETWIB_TRUE, prompt, NETWIB_FALSE,
                                         &buf));
    if (netwib__buf_ref_data_size(&buf) == 0) {
      if (pdefaultlist != NULL) {
        netwib_er(netwib_eths_add_buf(peths, pdefaultlist));
        break;
      }
    }
    ret = netwib_eths_add_buf(plocaleths, &buf);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_eths_add_eths(peths, plocaleths));
      break;
    }
    netwib_er(netwib_priv_ranges_del_all(&plocaleths->ranges));
    netwib__buf_reinit(&buf);
    prompt = '>';
  }

  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_eths_close(&plocaleths));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_eths_del_eth(netwib_eths *peths,
                               netwib_consteth *peth)
{

  /* parameter verification */
  if (peths == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_eths(peths));
#endif

  /* add item */
  netwib_er(netwib_priv_ranges_del(&peths->ranges, peth->b));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_eths_del_ethrange(netwib_eths *peths,
                                    netwib_consteth *pinfeth,
                                    netwib_consteth *psupeth)
{

  /* parameter verification */
  if (peths == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_eths(peths));
#endif

  /* add item */
  netwib_er(netwib_priv_ranges_del_range(&peths->ranges,
                                         pinfeth->b, psupeth->b));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_eths_del_eths(netwib_eths *peths,
                                netwib_consteths *pethstodel)
{
  netwib_priv_ranges_index rangesindex;
  netwib_byte infarray[NETWIB_ETH_LEN], suparray[NETWIB_ETH_LEN];
  netwib_err ret;

  /* parameter verification */
  if (peths == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_eths(peths));
  netwib_er(netwib_debug_leak_valid_eths(pethstodel));
#endif

  netwib_er(netwib_priv_ranges_index_init(&pethstodel->ranges, &rangesindex));
  ret = NETWIB_ERR_OK;
  while(NETWIB_TRUE) {
    ret = netwib_priv_ranges_index_next_range(&rangesindex, infarray,
                                              suparray);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_er(netwib_priv_ranges_del_range(&peths->ranges, infarray,
                                           suparray));
  }
  netwib_er(netwib_priv_ranges_index_close(&rangesindex));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_eths_contains_eth(netwib_consteths *peths,
                                    netwib_consteth *peth,
                                    netwib_bool *pyes)
{

  /* parameter verification */
  if (peths == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_eths(peths));
#endif

  /* add item */
  netwib_er(netwib_priv_ranges_contains(&peths->ranges, peth->b, pyes));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_eths_contains_ethrange(netwib_consteths *peths,
                                         netwib_consteth *pinfeth,
                                         netwib_consteth *psupeth,
                                         netwib_bool *pyes)
{

  /* parameter verification */
  if (peths == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_eths(peths));
#endif

  /* add item */
  netwib_er(netwib_priv_ranges_contains_range(&peths->ranges, pinfeth->b,
                                              psupeth->b, pyes));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_eths(netwib_consteths *peths,
                                  netwib_buf *pbuf)
{
  netwib_priv_ranges_index rangesindex;
  netwib_eth infeth, supeth;
  netwib_bool needcomma;
  netwib_cmp cmp;
  netwib_uint32 savedsize=0;
  netwib_err ret;

  /* parameter verification */
  if (peths == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_eths(peths));
#endif

  /* save position in case of error */
  if (pbuf != NULL) {
    savedsize = netwib__buf_ref_data_size(pbuf);
  }

  netwib_er(netwib_priv_ranges_index_init(&peths->ranges, &rangesindex));
  needcomma = NETWIB_FALSE;
  ret = NETWIB_ERR_OK;
  while(NETWIB_TRUE) {
    ret = netwib_priv_ranges_index_next_range(&rangesindex, infeth.b,
                                              supeth.b);
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
    ret = netwib_buf_append_eth(&infeth, pbuf);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    netwib_er(netwib_eth_cmp(&infeth, &supeth, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      ret = netwib_buf_append_byte('-', pbuf);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
      ret = netwib_buf_append_eth(&supeth, pbuf);
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


