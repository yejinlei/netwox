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
struct netwib_ips_index {
  netwib_priv_ranges_index rangesindex;
  netwib_ips *pips;
};

/*-------------------------------------------------------------*/
netwib_err netwib_ips_index_init(netwib_constips *pips,
                                 netwib_ips_index **ppipsindex)
{
  netwib_ips_index *pipsindex;
  netwib_err ret;

  /* parameter verification */
  if (ppipsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* allocate needed memory to store pips */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_ips_index),
                              (netwib_ptr*)&pipsindex));
  *ppipsindex = pipsindex;

  ret = netwib_priv_ranges_index_init(&pips->ranges,
                                      &pipsindex->rangesindex);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free((netwib_ptr*)&pipsindex));
    return(ret);
  }
  pipsindex->pips = netwib_priv_castptr(pips);

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_add_ips_index(pipsindex));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ips_index_close(netwib_ips_index **ppipsindex)
{
  netwib_ips_index *pipsindex;

  /* parameter verification */
  if (ppipsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pipsindex = *ppipsindex;

  /* remove items */
  netwib_er(netwib_priv_ranges_index_close(&pipsindex->rangesindex));

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_del_ips_index(pipsindex));
#endif

  /* free memory */
  netwib_er(netwib_ptr_free((netwib_ptr*)&pipsindex));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ips_index_ctl_set(netwib_ips_index *pipsindex,
                                    netwib_ips_index_ctltype type,
                                    netwib_ptr p,
                                    netwib_uint32 ui)
{
  /* parameter verification */
  if (pipsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ips_index(pipsindex));
#endif

  switch(type) {
    case NETWIB_IPS_INDEX_CTLTYPE_REWIND :
      netwib_er(netwib_priv_ranges_index_rewind(&pipsindex->rangesindex));
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IPS_INDEX_CTLTYPE_INDEX :
      netwib_er(netwib_priv_ranges_index_index(&pipsindex->rangesindex,
                                               (netwib_priv_ranges_index *)p));
      return(NETWIB_ERR_OK);
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
      /* perhaps used in those cases */
      ui = ui;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ips_index_ctl_get(netwib_ips_index *pipsindex,
                                    netwib_ips_index_ctltype type,
                                    netwib_ptr p,
                                    netwib_uint32 *pui)
{
  /* parameter verification */
  if (pipsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ips_index(pipsindex));
#endif

  switch(type) {
    case NETWIB_IPS_INDEX_CTLTYPE_REWIND :
    case NETWIB_IPS_INDEX_CTLTYPE_INDEX :
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
      /* perhaps used in those cases */
      p = p;
      pui = pui;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ips_index_next_ip(netwib_ips_index *pipsindex,
                                    netwib_ip *pip)
{
  netwib_byte iparray[NETWIB_IPS_LEN];

  /* parameter verification */
  if (pipsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ips_index(pipsindex));
#endif

  netwib_er(netwib_priv_ranges_index_next(&pipsindex->rangesindex, iparray));
  netwib_er(netwib_priv_ips_ip_init_array(iparray, pip));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ips_index_next_iprange(netwib_ips_index *pipsindex,
                                         netwib_ip *pinfip,
                                         netwib_ip *psupip)
{
  netwib_byte infiparray[NETWIB_IPS_LEN], supiparray[NETWIB_IPS_LEN];

  /* parameter verification */
  if (pipsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ips_index(pipsindex));
#endif

  netwib_er(netwib_priv_ranges_index_next_range(&pipsindex->rangesindex,
                                                infiparray, supiparray));
  netwib_er(netwib_priv_ips_ip_init_array(infiparray, pinfip));
  netwib_er(netwib_priv_ips_ip_init_array(supiparray, psupip));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ips_index_this_ip(netwib_ips_index *pipsindex,
                                    netwib_ip *pip)
{
  netwib_byte iparray[NETWIB_IPS_LEN];

  /* parameter verification */
  if (pipsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ips_index(pipsindex));
#endif

  netwib_er(netwib_priv_ranges_index_this(&pipsindex->rangesindex, iparray));
  netwib_er(netwib_priv_ips_ip_init_array(iparray, pip));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ips_index_this_iprange(netwib_ips_index *pipsindex,
                                         netwib_ip *pinfip,
                                         netwib_ip *psupip)
{
  netwib_byte infiparray[NETWIB_IPS_LEN], supiparray[NETWIB_IPS_LEN];

  /* parameter verification */
  if (pipsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ips_index(pipsindex));
#endif

  netwib_er(netwib_priv_ranges_index_this_range(&pipsindex->rangesindex,
                                                infiparray, supiparray));
  netwib_er(netwib_priv_ips_ip_init_array(infiparray, pinfip));
  netwib_er(netwib_priv_ips_ip_init_array(supiparray, psupip));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ips_index_this_del(netwib_ips_index *pipsindex)
{
  /* parameter verification */
  if (pipsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ips_index(pipsindex));
#endif

  netwib_er(netwib_priv_ranges_index_this_del(&pipsindex->rangesindex));

  return(NETWIB_ERR_OK);
}
