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
struct netwib_eths_index {
  netwib_priv_ranges_index rangesindex;
  netwib_eths *peths;
};

/*-------------------------------------------------------------*/
netwib_err netwib_eths_index_init(netwib_consteths *peths,
                                  netwib_eths_index **ppethsindex)
{
  netwib_eths_index *pethsindex;
  netwib_err ret;

  /* parameter verification */
  if (ppethsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* allocate needed memory to store peths */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_eths_index),
                              (netwib_ptr*)&pethsindex));
  *ppethsindex = pethsindex;

  ret = netwib_priv_ranges_index_init(&peths->ranges,
                                      &pethsindex->rangesindex);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free((netwib_ptr*)&pethsindex));
    return(ret);
  }
  pethsindex->peths = netwib_priv_castptr(peths);

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_add_eths_index(pethsindex));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_eths_index_close(netwib_eths_index **ppethsindex)
{
  netwib_eths_index *pethsindex;

  /* parameter verification */
  if (ppethsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pethsindex = *ppethsindex;

  /* remove items */
  netwib_er(netwib_priv_ranges_index_close(&pethsindex->rangesindex));

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_del_eths_index(pethsindex));
#endif

  /* free memory */
  netwib_er(netwib_ptr_free((netwib_ptr*)&pethsindex));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_eths_index_ctl_set(netwib_eths_index *pethsindex,
                                     netwib_eths_index_ctltype type,
                                     netwib_ptr p,
                                     netwib_uint32 ui)
{
  /* parameter verification */
  if (pethsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_eths_index(pethsindex));
#endif

  switch(type) {
    case NETWIB_ETHS_INDEX_CTLTYPE_REWIND :
      netwib_er(netwib_priv_ranges_index_rewind(&pethsindex->rangesindex));
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_ETHS_INDEX_CTLTYPE_INDEX :
      netwib_er(netwib_priv_ranges_index_index(&pethsindex->rangesindex,
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
netwib_err netwib_eths_index_ctl_get(netwib_eths_index *pethsindex,
                                     netwib_eths_index_ctltype type,
                                     netwib_ptr p,
                                     netwib_uint32 *pui)
{
  /* parameter verification */
  if (pethsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_eths_index(pethsindex));
#endif

  switch(type) {
    case NETWIB_ETHS_INDEX_CTLTYPE_REWIND :
    case NETWIB_ETHS_INDEX_CTLTYPE_INDEX :
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
netwib_err netwib_eths_index_next_eth(netwib_eths_index *pethsindex,
                                      netwib_eth *peth)
{
  netwib_byte etharray[NETWIB_ETH_LEN];

  /* parameter verification */
  if (pethsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_eths_index(pethsindex));
#endif

  netwib_er(netwib_priv_ranges_index_next(&pethsindex->rangesindex, etharray));
  if (peth != NULL) {
    netwib_c_memcpy(peth->b, etharray, NETWIB_ETH_LEN);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_eths_index_next_ethrange(netwib_eths_index *pethsindex,
                                           netwib_eth *pinfeth,
                                           netwib_eth *psupeth)
{
  netwib_byte infetharray[NETWIB_ETH_LEN], supetharray[NETWIB_ETH_LEN];

  /* parameter verification */
  if (pethsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_eths_index(pethsindex));
#endif

  netwib_er(netwib_priv_ranges_index_next_range(&pethsindex->rangesindex,
                                                infetharray,
                                                supetharray));
  if (pinfeth != NULL) {
    netwib_c_memcpy(pinfeth->b, infetharray, NETWIB_ETH_LEN);
  }
  if (psupeth != NULL) {
    netwib_c_memcpy(psupeth->b, supetharray, NETWIB_ETH_LEN);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_eths_index_this_eth(netwib_eths_index *pethsindex,
                                      netwib_eth *peth)
{
  netwib_byte etharray[NETWIB_ETH_LEN];

  /* parameter verification */
  if (pethsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_eths_index(pethsindex));
#endif

  netwib_er(netwib_priv_ranges_index_this(&pethsindex->rangesindex, etharray));
  if (peth != NULL) {
    netwib_c_memcpy(peth->b, etharray, NETWIB_ETH_LEN);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_eths_index_this_ethrange(netwib_eths_index *pethsindex,
                                           netwib_eth *pinfeth,
                                           netwib_eth *psupeth)
{
  netwib_byte infetharray[NETWIB_ETH_LEN], supetharray[NETWIB_ETH_LEN];

  /* parameter verification */
  if (pethsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_eths_index(pethsindex));
#endif

  netwib_er(netwib_priv_ranges_index_this_range(&pethsindex->rangesindex,
                                                infetharray,
                                                supetharray));
  if (pinfeth != NULL) {
    netwib_c_memcpy(pinfeth->b, infetharray, NETWIB_ETH_LEN);
  }
  if (psupeth != NULL) {
    netwib_c_memcpy(psupeth->b, supetharray, NETWIB_ETH_LEN);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_eths_index_this_del(netwib_eths_index *pethsindex)
{
  /* parameter verification */
  if (pethsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_eths_index(pethsindex));
#endif

  netwib_er(netwib_priv_ranges_index_this_del(&pethsindex->rangesindex));

  return(NETWIB_ERR_OK);
}
