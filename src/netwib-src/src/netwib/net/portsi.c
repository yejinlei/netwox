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
struct netwib_ports_index {
  netwib_priv_ranges_index rangesindex;
  netwib_ports *pports;
};

/*-------------------------------------------------------------*/
netwib_err netwib_ports_index_init(netwib_constports *pports,
                                   netwib_ports_index **ppportsindex)
{
  netwib_ports_index *pportsindex;
  netwib_err ret;

  /* parameter verification */
  if (ppportsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* allocate needed memory to store pports */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_ports_index),
                              (netwib_ptr*)&pportsindex));
  *ppportsindex = pportsindex;

  ret = netwib_priv_ranges_index_init(&pports->ranges,
                                      &pportsindex->rangesindex);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free((netwib_ptr*)&pportsindex));
    return(ret);
  }
  pportsindex->pports = netwib_priv_castptr(pports);

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_add_ports_index(pportsindex));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ports_index_close(netwib_ports_index **ppportsindex)
{
  netwib_ports_index *pportsindex;

  /* parameter verification */
  if (ppportsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pportsindex = *ppportsindex;

  /* remove items */
  netwib_er(netwib_priv_ranges_index_close(&pportsindex->rangesindex));

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_del_ports_index(pportsindex));
#endif

  /* free memory */
  netwib_er(netwib_ptr_free((netwib_ptr*)&pportsindex));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ports_index_ctl_set(netwib_ports_index *pportsindex,
                                      netwib_ports_index_ctltype type,
                                      netwib_ptr p,
                                      netwib_uint32 ui)
{
  /* parameter verification */
  if (pportsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ports_index(pportsindex));
#endif

  switch(type) {
    case NETWIB_PORTS_INDEX_CTLTYPE_REWIND :
      netwib_er(netwib_priv_ranges_index_rewind(&pportsindex->rangesindex));
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_PORTS_INDEX_CTLTYPE_INDEX :
      netwib_er(netwib_priv_ranges_index_index(&pportsindex->rangesindex,
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
netwib_err netwib_ports_index_ctl_get(netwib_ports_index *pportsindex,
                                      netwib_ports_index_ctltype type,
                                      netwib_ptr p,
                                      netwib_uint32 *pui)
{
  /* parameter verification */
  if (pportsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ports_index(pportsindex));
#endif

  switch(type) {
    case NETWIB_PORTS_INDEX_CTLTYPE_REWIND :
    case NETWIB_PORTS_INDEX_CTLTYPE_INDEX :
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
netwib_err netwib_ports_index_next_port(netwib_ports_index *pportsindex,
                                        netwib_port *pport)
{
  netwib_byte portarray[NETWIB_PORT_LEN];

  /* parameter verification */
  if (pportsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ports_index(pportsindex));
#endif

  netwib_er(netwib_priv_ranges_index_next(&pportsindex->rangesindex,
                                          portarray));
  if (pport != NULL) {
    *pport = netwib_c2_uint16_2(portarray[0], portarray[1]);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ports_index_next_portrange(netwib_ports_index *pportsindex,
                                             netwib_port *pinfport,
                                             netwib_port *psupport)
{
  netwib_byte infportarray[NETWIB_PORT_LEN], supportarray[NETWIB_PORT_LEN];

  /* parameter verification */
  if (pportsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ports_index(pportsindex));
#endif

  netwib_er(netwib_priv_ranges_index_next_range(&pportsindex->rangesindex,
                                                infportarray,
                                                supportarray));
  if (pinfport != NULL) {
    *pinfport = netwib_c2_uint16_2(infportarray[0], infportarray[1]);
  }
  if (psupport != NULL) {
    *psupport = netwib_c2_uint16_2(supportarray[0], supportarray[1]);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ports_index_this_port(netwib_ports_index *pportsindex,
                                        netwib_port *pport)
{
  netwib_byte portarray[NETWIB_PORT_LEN];

  /* parameter verification */
  if (pportsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ports_index(pportsindex));
#endif

  netwib_er(netwib_priv_ranges_index_this(&pportsindex->rangesindex,
                                          portarray));
  if (pport != NULL) {
    *pport = netwib_c2_uint16_2(portarray[0], portarray[1]);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ports_index_this_portrange(netwib_ports_index *pportsindex,
                                             netwib_port *pinfport,
                                             netwib_port *psupport)
{
  netwib_byte infportarray[NETWIB_PORT_LEN], supportarray[NETWIB_PORT_LEN];

  /* parameter verification */
  if (pportsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ports_index(pportsindex));
#endif

  netwib_er(netwib_priv_ranges_index_this_range(&pportsindex->rangesindex,
                                                infportarray,
                                                supportarray));
  if (pinfport != NULL) {
    *pinfport = netwib_c2_uint16_2(infportarray[0], infportarray[1]);
  }
  if (psupport != NULL) {
    *psupport = netwib_c2_uint16_2(supportarray[0], supportarray[1]);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ports_index_this_del(netwib_ports_index *pportsindex)
{
  /* parameter verification */
  if (pportsindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ports_index(pportsindex));
#endif

  netwib_er(netwib_priv_ranges_index_this_del(&pportsindex->rangesindex));

  return(NETWIB_ERR_OK);
}
