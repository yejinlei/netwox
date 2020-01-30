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
struct netwib_ring_index {
  netwib_ring *pring;
  netwib_ringitem *pcurrentitem;
  netwib_ringitem *pnextitem;
  netwib_ringitem *ppreviousitem;
};

/*-------------------------------------------------------------*/
netwib_err netwib_ring_index_init(netwib_constring *pring,
                                  netwib_ring_index **ppringindex)
{
  netwib_ring_index *pringindex;

  /* parameter verification */
  if (ppringindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* allocate needed memory to store pring */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_ring_index),
                              (netwib_ptr*)&pringindex));
  *ppringindex = pringindex;
  pringindex->pring = netwib_priv_castptr(pring);
  pringindex->pcurrentitem = NULL;
  pringindex->pnextitem = NULL;
  pringindex->ppreviousitem = NULL;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ring(pring));
  netwib_er(netwib_debug_leak_add_ringi(pringindex));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ring_index_close(netwib_ring_index **ppringindex)
{
  netwib_ring_index *pringindex;

  /* parameter verification */
  if (ppringindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pringindex = *ppringindex;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_del_ringi(pringindex));
#endif

  /* free memory */
  netwib_er(netwib_ptr_free((netwib_ptr*)&pringindex));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ring_index_ctl_set(netwib_ring_index *pringindex,
                                     netwib_ring_index_ctltype type,
                                     netwib_ptr p,
                                     netwib_uint32 ui)
{
  netwib_ring_index *pringindexref;

  /* parameter verification */
  if (pringindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ringi(pringindex));
#endif

  switch(type) {
    case NETWIB_RING_INDEX_CTLTYPE_REWIND :
      pringindex->pcurrentitem = NULL;
      pringindex->pnextitem = NULL;
      pringindex->ppreviousitem = NULL;
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_RING_INDEX_CTLTYPE_INDEX :
      pringindexref = (netwib_ring_index *)p;
      pringindex->pring = pringindexref->pring;
      pringindex->pcurrentitem = pringindexref->pcurrentitem;
      pringindex->pnextitem = pringindexref->pnextitem;
      pringindex->ppreviousitem = pringindexref->ppreviousitem;
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
netwib_err netwib_ring_index_ctl_get(netwib_ring_index *pringindex,
                                     netwib_ring_index_ctltype type,
                                     netwib_ptr p,
                                     netwib_uint32 *pui)
{
  /* parameter verification */
  if (pringindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ringi(pringindex));
#endif

  switch(type) {
    case NETWIB_RING_INDEX_CTLTYPE_REWIND :
    case NETWIB_RING_INDEX_CTLTYPE_INDEX :
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
netwib_err netwib_ring_index_next_criteria(netwib_ring_index *pringindex,
                                           netwib_ring_criteria_pf pfunc_criteria,
                                           netwib_ptr pinfos,
                                           netwib_ptr *ppitem)
{
  netwib_ring *pring;
  netwib_ringitem *pringitem;
  netwib_bool b;
  netwib_err ret;

  /* parameter verification */
  if (pringindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pring = pringindex->pring;
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ringi(pringindex));
  netwib_er(netwib_debug_leak_valid_ring(pring));
#endif

  /* point to first candidate */
  if (pringindex->pcurrentitem != NULL) {
    pringitem = pringindex->pcurrentitem->pnext;
  } else if (pringindex->pnextitem != NULL) {
    pringitem = pringindex->pnextitem;
  } else {
    pringitem = pring->pnext;
  }

  /* main loop */
  b = NETWIB_TRUE;
  while (NETWIB_TRUE) {
    if (pringitem == (netwib_ringitem*)pring) {
      return(NETWIB_ERR_DATAEND);
    }
    if (pfunc_criteria != NULL) {
      b = NETWIB_FALSE;
      ret = (*pfunc_criteria)(pringitem->pitem, pinfos, &b);
      if (ret != NETWIB_ERR_OK) {
        return(ret);
      }
    }
    if (b) {
      if (ppitem != NULL) *ppitem = pringitem->pitem;
      pringindex->pcurrentitem = pringitem;
      pringindex->pnextitem = pringindex->pcurrentitem->pnext;
      pringindex->ppreviousitem = pringindex->pcurrentitem->pprevious;
      return(NETWIB_ERR_OK);
    }
    pringitem = pringitem->pnext;
  }

  return(NETWIB_ERR_LOINTERNALERROR);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ring_index_previous_criteria(netwib_ring_index *pringindex,
                                               netwib_ring_criteria_pf pfunc_criteria,
                                               netwib_ptr pinfos,
                                               netwib_ptr *ppitem)
{
  netwib_ring *pring;
  netwib_ringitem *pringitem;
  netwib_bool b;
  netwib_err ret;

  /* parameter verification */
  if (pringindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pring = pringindex->pring;
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ringi(pringindex));
  netwib_er(netwib_debug_leak_valid_ring(pring));
#endif

  /* point to first candidate */
  if (pringindex->pcurrentitem != NULL) {
    pringitem = pringindex->pcurrentitem->pprevious;
  } else if (pringindex->ppreviousitem != NULL) {
    pringitem = pringindex->ppreviousitem;
  } else {
    pringitem = pring->pprevious;
  }

  /* main loop */
  b = NETWIB_TRUE;
  while (NETWIB_TRUE) {
    if (pringitem == (netwib_ringitem*)pring) {
      return(NETWIB_ERR_DATAEND);
    }
    if (pfunc_criteria != NULL) {
      b = NETWIB_FALSE;
      ret = (*pfunc_criteria)(pringitem->pitem, pinfos, &b);
      if (ret != NETWIB_ERR_OK) {
        return(ret);
      }
    }
    if (b) {
      if (ppitem != NULL) *ppitem = pringitem->pitem;
      pringindex->pcurrentitem = pringitem;
      pringindex->pnextitem = pringindex->pcurrentitem->pnext;
      pringindex->ppreviousitem = pringindex->pcurrentitem->pprevious;
      return(NETWIB_ERR_OK);
    }
    pringitem = pringitem->pprevious;
  }

  return(NETWIB_ERR_LOINTERNALERROR);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ring_index_add_before(netwib_ring_index *pringindex,
                                        netwib_constptr pitem)
{
  netwib_ring *pring;
  netwib_ringitem *pringitem=NULL, *pnew;

  /* parameter verification */
  if (pringindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pring = pringindex->pring;
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ringi(pringindex));
  netwib_er(netwib_debug_leak_valid_ring(pring));
#endif
  if (pring->numberofitems >= 0x7FFFFFFF) {
    return(NETWIB_ERR_PARINGFULL);
  }

  /* obtain current item */
  if (pringindex->pcurrentitem != NULL) {
    pringitem = pringindex->pcurrentitem;
  } else if (pringindex->ppreviousitem != NULL) {
    pringitem = pringindex->ppreviousitem->pnext;
  } else if (pringindex->pnextitem != NULL) {
    pringitem = pringindex->pnextitem->pprevious;
  } else {
    pringitem = (netwib_ringitem *)pring;
  }

  /* alloc memory for the item */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_ringitem), (netwib_ptr*)&pnew));
  pnew->pitem = netwib_priv_castpptr(pitem);

  /* add it */
  pnew->pnext = pringitem;
  pnew->pprevious = pringitem->pprevious;
  pringitem->pprevious->pnext = pnew;
  pringitem->pprevious = pnew;
  pring->numberofitems++;

  /* update index */
  pringindex->ppreviousitem = pnew;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ring_index_add_after(netwib_ring_index *pringindex,
                                       netwib_constptr pitem)
{
  netwib_ring *pring;
  netwib_ringitem *pringitem=NULL, *pnew;

  /* parameter verification */
  if (pringindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pring = pringindex->pring;
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ringi(pringindex));
  netwib_er(netwib_debug_leak_valid_ring(pring));
#endif
  if (pring->numberofitems >= 0x7FFFFFFF) {
    return(NETWIB_ERR_PARINGFULL);
  }

  /* obtain current item */
  if (pringindex->pcurrentitem != NULL) {
    pringitem = pringindex->pcurrentitem;
  } else if (pringindex->ppreviousitem != NULL) {
    pringitem = pringindex->ppreviousitem->pnext;
  } else if (pringindex->pnextitem != NULL) {
    pringitem = pringindex->pnextitem->pprevious;
  } else {
    pringitem = (netwib_ringitem *)pring;
  }

  /* alloc memory for the item */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_ringitem), (netwib_ptr*)&pnew));
  pnew->pitem = netwib_priv_castpptr(pitem);

  /* add it */
  pnew->pprevious = pringitem;
  pnew->pnext = pringitem->pnext;
  pringitem->pnext->pprevious = pnew;
  pringitem->pnext = pnew;
  pring->numberofitems++;

  /* update index */
  pringindex->pnextitem = pnew;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ring_index_this_value(netwib_ring_index *pringindex,
                                        netwib_ptr *ppitem)
{

  /* parameter verification */
  if (pringindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ringi(pringindex));
  netwib_er(netwib_debug_leak_valid_ring(pringindex->pring));
#endif

  if (pringindex->pcurrentitem == NULL) {
    return(NETWIB_ERR_PAINDEXNODATA);
  }

  if (ppitem != NULL) *ppitem = pringindex->pcurrentitem->pitem;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ring_index_this_del(netwib_ring_index *pringindex,
                                      netwib_bool eraseitem)
{
  netwib_ring *pring;
  netwib_ringitem *pwork1, *pwork2, *pold;
  netwib_err ret;

  /* parameter verification */
  if (pringindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pring = pringindex->pring;
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ringi(pringindex));
  netwib_er(netwib_debug_leak_valid_ring(pring));
#endif

  if (pringindex->pcurrentitem == NULL) {
    return(NETWIB_ERR_PAINDEXNODATA);
  }

  /* save previous and next */
  pold = pringindex->pcurrentitem;
  pwork1 = pold->pprevious;
  pwork2 = pold->pnext;

  /* free the memory */
  if (pring->pfunc_erase != NULL && eraseitem) {
    /* we have a removing function */
    ret = (*pring->pfunc_erase) (pold->pitem);
    if (ret != NETWIB_ERR_OK) {
      /* return without unlinking the item */
      return(ret);
    }
  }
  netwib_er(netwib_ptr_free((netwib_ptr*)&pold));

  /* unlink the item */
  pwork2->pprevious = pwork1;
  pwork1->pnext = pwork2;

  /* one less */
  pring->numberofitems--;

  /* update index */
  pringindex->pcurrentitem = NULL;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ring_index_this_replace(netwib_ring_index *pringindex,
                                          netwib_constptr pitem,
                                          netwib_bool erasepreviousitem)
{
  netwib_ring *pring;
  netwib_ringitem *pwork;
  netwib_err ret;

  /* parameter verification */
  if (pringindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pring = pringindex->pring;
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ringi(pringindex));
  netwib_er(netwib_debug_leak_valid_ring(pring));
#endif

  if (pringindex->pcurrentitem == NULL) {
    return(NETWIB_ERR_PAINDEXNODATA);
  }
  pwork = pringindex->pcurrentitem;

  if (pring->pfunc_erase != NULL && erasepreviousitem) {
    ret = (*pring->pfunc_erase) (pwork->pitem);
    if (ret != NETWIB_ERR_OK) {
      return(ret);
    }
  }
  pwork->pitem = netwib_priv_castpptr(pitem);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ring_index_add_ring_criteria(netwib_ring_index *pringindex,
                                               netwib_ring *pringtoadd,
                                               netwib_ring_criteria_pf pfunc_criteria,
                                               netwib_ptr pinfos,
                                               netwib_bool duplicateitems)
{
  netwib_ring *pring;
  netwib_ringitem *pworka1, *pworka2, *pworka3, *pworkb, *pworkanew;
  netwib_ptr pnewitem;
  netwib_bool b;
  netwib_err ret;

  /* parameter verification */
  if (pringindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pring = pringindex->pring;
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ringi(pringindex));
  netwib_er(netwib_debug_leak_valid_ring(pring));
#endif

  /* special case */
  if (pringtoadd->numberofitems == 0) {
    return(NETWIB_ERR_OK);
  }

  /* obtain current item */
  if (pringindex->pcurrentitem != NULL) {
    pworka1 = pringindex->pcurrentitem;
  } else if (pringindex->ppreviousitem != NULL) {
    pworka1 = pringindex->ppreviousitem->pnext;
  } else if (pringindex->pnextitem != NULL) {
    pworka1 = pringindex->pnextitem->pprevious;
  } else {
    pworka1 = (netwib_ringitem *)pring;
  }
  pworka2 = pworka1;
  pworka3 = pworka1->pnext;

  /* main loop */
  b = NETWIB_TRUE;
  ret = NETWIB_ERR_OK;
  pworkb = (netwib_ringitem *)pringtoadd;
  while (NETWIB_TRUE) {
    pworkb = pworkb->pnext;
    if (pworkb == (netwib_ringitem*)pringtoadd) {
      break;
    }
    if (pfunc_criteria != NULL) {
      b = NETWIB_FALSE;
      ret = (*pfunc_criteria) (pworkb->pitem, pinfos, &b);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
    }
    if (b) {
      if (pring->numberofitems >= 0x7FFFFFFF) {
        ret = NETWIB_ERR_PARINGFULL;
        break;
      }
      /* add the item */
      if (pringtoadd->pfunc_duplicate != NULL && duplicateitems) {
        ret = (*pringtoadd->pfunc_duplicate) (pworkb->pitem, &pnewitem);
        if (ret != NETWIB_ERR_OK) {
          break;
        }
      } else {
        /* only use the same pointer */
        pnewitem = pworkb->pitem;
      }
      /* alloc memory for the item */
      netwib_er(netwib_ptr_malloc(sizeof(netwib_ringitem),
                                  (netwib_ptr*)&pworkanew));
      pworkanew->pitem = pnewitem;
      /* link to previous */
      pworka2->pnext = pworkanew;
      pworkanew->pprevious = pworka2;
      pworka2 = pworkanew;
      pring->numberofitems++;
    }
  }

  /* update index */
  pringindex->pnextitem = pworka1->pnext;

  /* relink end */
  pworka2->pnext = pworka3;
  pworka3->pprevious = pworka2;
  return(ret);
}
