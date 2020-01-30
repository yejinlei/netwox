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
struct netwib_hash_index {
  netwib_hash *phash;
  netwib_hashitem *pcurrentitem;
  netwib_bool nextitemisend;
  netwib_hashitem *pnextitem;
};

/*-------------------------------------------------------------*/
netwib_err netwib_hash_index_init(netwib_consthash *phash,
                                  netwib_hash_index **pphashindex)
{
  netwib_hash_index *phashindex;

  /* parameter verification */
  if (pphashindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* allocate needed memory to store phash */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_hash_index),
                              (netwib_ptr*)&phashindex));
  *pphashindex = phashindex;
  phashindex->phash = netwib_priv_castptr(phash);
  phashindex->pcurrentitem = NULL;
  phashindex->nextitemisend = NETWIB_FALSE;
  phashindex->pnextitem = NULL;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_hash(phash));
  netwib_er(netwib_debug_leak_add_hashi(phashindex));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_hash_index_close(netwib_hash_index **pphashindex)
{
  netwib_hash_index *phashindex;

  /* parameter verification */
  if (pphashindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  phashindex = *pphashindex;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_del_hashi(phashindex));
#endif

  /* free memory */
  netwib_er(netwib_ptr_free((netwib_ptr*)&phashindex));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_hash_index_next(netwib_hash_index *phashindex,
                                              netwib_hashitem *phashitem,
                                              netwib_hashitem **ppnexthashitem) {
  netwib_uint32 hashitempos;

  if (phashitem != NULL) {
    hashitempos = phashitem->currenttablepos;
    phashitem = phashitem->pnext;
  } else {
    if (phashindex->phash->table[0] != NULL) {
      *ppnexthashitem = (netwib_hashitem *)phashindex->phash->table[0];
      return(NETWIB_ERR_OK);
    }
    hashitempos = 0;
    phashitem = NULL;
  }

  /* eventually skip empty table positions */
  while (phashitem == NULL) {
    hashitempos++;
    if (hashitempos > phashindex->phash->tablemax) {
      *ppnexthashitem = NULL;
      return(NETWIB_ERR_DATAEND);
    }
    phashitem = phashindex->phash->table[hashitempos];
  }

  *ppnexthashitem = phashitem;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_hash_index_pnextitem_update(netwib_hash_index *phashindex) {
  netwib_err ret;

  ret = netwib_priv_hash_index_next(phashindex, phashindex->pcurrentitem,
                                    &phashindex->pnextitem);
  if (ret == NETWIB_ERR_OK) {
    phashindex->nextitemisend = NETWIB_FALSE;
  } else if (ret == NETWIB_ERR_DATAEND) {
    phashindex->nextitemisend = NETWIB_TRUE;
  } else {
    return(ret);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_hash_index_ctl_set(netwib_hash_index *phashindex,
                                     netwib_hash_index_ctltype type,
                                     netwib_ptr p,
                                     netwib_uint32 ui)
{
  netwib_hash_index *phashindexref;

  /* parameter verification */
  if (phashindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_hashi(phashindex));
#endif

  switch(type) {
    case NETWIB_HASH_INDEX_CTLTYPE_REWIND :
      phashindex->pcurrentitem = NULL;
      phashindex->nextitemisend = NETWIB_FALSE;
      phashindex->pnextitem = NULL;
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_HASH_INDEX_CTLTYPE_INDEX :
      phashindexref = (netwib_hash_index *)p;
      phashindex->phash = phashindexref->phash;
      phashindex->pcurrentitem = phashindexref->pcurrentitem;
      phashindex->nextitemisend = phashindexref->nextitemisend;
      phashindex->pnextitem = phashindexref->pnextitem;
      return(NETWIB_ERR_OK);
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
      /* perhaps used in those cases */
      p = p;
      ui = ui;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_hash_index_ctl_get(netwib_hash_index *phashindex,
                                     netwib_hash_index_ctltype type,
                                     netwib_ptr p,
                                     netwib_uint32 *pui)
{
  /* parameter verification */
  if (phashindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_hashi(phashindex));
#endif

  switch(type) {
    case NETWIB_HASH_INDEX_CTLTYPE_REWIND :
    case NETWIB_HASH_INDEX_CTLTYPE_INDEX :
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
netwib_err netwib_hash_index_next_criteria(netwib_hash_index *phashindex,
                                           netwib_hash_criteria_pf pfunc_search,
                                           netwib_ptr pinfos,
                                           netwib_buf *pkey,
                                           netwib_ptr *ppitem)
{
  netwib_hashitem *phashitem;
  netwib_bool b;
  netwib_buf key;
  netwib_err ret;

  /* parameter verification */
  if (phashindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_hashi(phashindex));
  netwib_er(netwib_debug_leak_valid_hash(phashindex->phash));
#endif

  /* point to first candidate (or return NETWIB_ERR_DATAEND) */
  if (phashindex->pcurrentitem != NULL) {
    netwib_er(netwib_priv_hash_index_next(phashindex, phashindex->pcurrentitem,
                                          &phashitem));
  } else if (phashindex->nextitemisend) {
    return(NETWIB_ERR_DATAEND);
  } else if (phashindex->pnextitem != NULL) {
    phashitem = phashindex->pnextitem;
  } else {
    netwib_er(netwib_priv_hash_index_next(phashindex, NULL, &phashitem));
  }

  /* main loop */
  b = NETWIB_TRUE;
  while (NETWIB_TRUE) {
    /* test it */
    if (pfunc_search != NULL) {
      netwib_er(netwib_buf_init_ext_array(phashitem->key, phashitem->keysize+1,
                                          0, phashitem->keysize, &key));
      b = NETWIB_FALSE;
      ret = (*pfunc_search)(&key, phashitem->pitem, pinfos, &b);
      if (ret != NETWIB_ERR_OK) return(ret);
    }
    if (b) {
      netwib_er(netwib_buf_append_data(phashitem->key, phashitem->keysize,
                                       pkey));
      if (ppitem != NULL) *ppitem = phashitem->pitem;
      phashindex->pcurrentitem = phashitem;
      netwib_er(netwib_priv_hash_index_pnextitem_update(phashindex));
      return(NETWIB_ERR_OK);
    }
    /* jump to next (or return NETWIB_ERR_DATAEND) */
    netwib_er(netwib_priv_hash_index_next(phashindex, phashitem, &phashitem));
  }

  return(NETWIB_ERR_LOINTERNALERROR);
}

/*-------------------------------------------------------------*/
netwib_err netwib_hash_index_this_value(netwib_hash_index *phashindex,
                                        netwib_buf *pkey,
                                        netwib_ptr *ppitem)
{

  /* parameter verification */
  if (phashindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_hashi(phashindex));
  netwib_er(netwib_debug_leak_valid_hash(phashindex->phash));
#endif

  if (phashindex->pcurrentitem == NULL) {
    return(NETWIB_ERR_PAINDEXNODATA);
  }

  netwib_er(netwib_buf_append_data(phashindex->pcurrentitem->key,
                                   phashindex->pcurrentitem->keysize, pkey));
  if (ppitem != NULL) *ppitem = phashindex->pcurrentitem->pitem;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_hash_index_this_del(netwib_hash_index *phashindex,
                                      netwib_bool eraseitem)
{
  netwib_hash *phash;
  netwib_hashitem *phashitem, **addressprevious;

  /* parameter verification */
  if (phashindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  phash = phashindex->phash;
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_hashi(phashindex));
  netwib_er(netwib_debug_leak_valid_hash(phash));
#endif

  if (phashindex->pcurrentitem == NULL) {
    return(NETWIB_ERR_PAINDEXNODATA);
  }

  /* obtain address of previous item */
  phashitem = phash->table[phashindex->pcurrentitem->currenttablepos];
  addressprevious = &(phash->table[phashindex->pcurrentitem->currenttablepos]);
  while (phashitem != NULL) {
    if (phashitem == phashindex->pcurrentitem) {
      break;
    }
    addressprevious = &(phashitem->pnext);
    phashitem = phashitem->pnext;
  }
  if (phashitem == NULL) {
    return(NETWIB_ERR_LOINTERNALERROR);
  }

  /* now remove the item */
  if (eraseitem && phash->pfunc_erase != NULL) {
    netwib_er((*phash->pfunc_erase)(phashindex->pcurrentitem->pitem));
  }
  *addressprevious = phashindex->pcurrentitem->pnext;
  netwib_er(netwib_ptr_free((netwib_ptr*)&phashindex->pcurrentitem));
  phash->numberofitems--;

  /* update index (no need to update pnextitem which is already ok) */
  phashindex->pcurrentitem = NULL;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_hash_index_this_replace(netwib_hash_index *phashindex,
                                          netwib_constptr pitem,
                                          netwib_bool erasepreviousitem)
{
  netwib_hash *phash;

  /* parameter verification */
  if (phashindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  phash = phashindex->phash;
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_hashi(phashindex));
  netwib_er(netwib_debug_leak_valid_hash(phash));
#endif

  if (phashindex->pcurrentitem == NULL) {
    return(NETWIB_ERR_PAINDEXNODATA);
  }

  /* replace the item */
  if (erasepreviousitem && phash->pfunc_erase != NULL) {
    netwib_er((*phash->pfunc_erase)(phashindex->pcurrentitem->pitem));
  }
  phashindex->pcurrentitem->pitem = netwib_priv_castpptr(pitem);

  return(NETWIB_ERR_OK);
}
