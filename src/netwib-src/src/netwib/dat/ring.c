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
#if NETWIB_DEBUG_RING_CHECK==1
static netwib_err netwib_debug_ring_check(netwib_conststring functionname,
                                          netwib_ring *pring)
{
  netwib_uint32 i;
  netwib_ringitem *pcurrent, *pnext;
  netwib_bool founderror;
  netwib_buf errbuf;

  /* check the ring by looping through it */
  netwib_er(netwib_buf_init_mallocdefault(&errbuf));
  pcurrent = (netwib_ringitem *) pring;
  founderror = NETWIB_FALSE;
  for (i = 0; i < pring->numberofitems; i++) {
    pnext = pcurrent->pnext;
    if (pnext->pprevious != pcurrent) {
      netwib_er(netwib_buf_append_fmt(&errbuf,
                                    "pnext->pprevious != pcurrent[%{int32}]\n",
                                    i + 1));
      founderror = NETWIB_TRUE;
    }
    pcurrent = pnext;
  }
  if (founderror) {
    netwib_er(netwib_priv_errmsg_func_buf(functionname, &errbuf));
    netwib_er(netwib_buf_close(&errbuf));
    return(NETWIB_ERR_LOINTERNALERROR);
  }
  netwib_er(netwib_buf_close(&errbuf));
  if (pcurrent->pnext != (netwib_ringitem *) pring) {
    netwib_er(netwib_priv_errmsg_func_string(functionname,
                                           "plast->pnext != pring"));
    return(NETWIB_ERR_LOINTERNALERROR);
  }
  if (pring->pprevious != pcurrent) {
    netwib_er(netwib_priv_errmsg_func_string(functionname,
                                           "pring->pprevious != plast"));
    return(NETWIB_ERR_LOINTERNALERROR);
  }

  return(NETWIB_ERR_OK);
}
 #define netwib__debug_ring_check(a,b) {netwib_er(netwib_debug_ring_check(a,b));}
#else
 #define netwib__debug_ring_check(a,b) {}
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_ring_init(netwib_ring_erase_pf pfunc_erase,
                            netwib_ring_duplicate_pf pfunc_duplicate,
                            netwib_ring **ppring)
{
  netwib_ring *pring;

  /* parameter verification */
  if (ppring == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* allocate needed memory to store pring */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_ring), (netwib_ptr*)&pring));
  *ppring = pring;

  /* set parameters */
  pring->pnext = (netwib_ringitem *) pring;
  pring->pprevious = (netwib_ringitem *) pring;
  pring->numberofitems = 0;
  pring->pfunc_erase = pfunc_erase;
  pring->pfunc_duplicate = pfunc_duplicate;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_add_ring(pring));
#endif

  netwib__debug_ring_check("netwib_ring_init", pring);
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ring_close(netwib_ring **ppring,
                             netwib_bool eraseitems)
{
  netwib_ring *pring;

  /* parameter verification */
  if (ppring == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pring = *ppring;

  netwib__debug_ring_check("netwib_ring_close", pring);

  /* remove items */
  netwib_er(netwib_ring_del_all(pring, eraseitems));

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_del_ring(pring));
#endif

  /* free memory */
  netwib_er(netwib_ptr_free((netwib_ptr*)&pring));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ring_ctl_set(netwib_ring *pring,
                               netwib_ring_ctltype type,
                               netwib_ptr p,
                               netwib_uint32 ui)
{
  /* parameter verification */
  if (pring == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ring(pring));
#endif

  switch(type) {
    case NETWIB_RING_CTLTYPE_COUNT:
      return(NETWIB_ERR_PAINVALIDTYPE);
    default:
      return(NETWIB_ERR_PAINVALIDTYPE);
      p = p;
      ui = ui;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ring_ctl_get(netwib_ring *pring,
                               netwib_ring_ctltype type,
                               netwib_ptr p,
                               netwib_uint32 *pui)
{
  /* parameter verification */
  if (pring == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ring(pring));
#endif

  switch(type) {
    case NETWIB_RING_CTLTYPE_COUNT:
      if (pui != NULL) {
        *pui = pring->numberofitems;
      }
      break;
    default:
      return(NETWIB_ERR_PAINVALIDTYPE);
      p = p;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ring_add_first(netwib_ring *pring,
                                 netwib_constptr pitem)
{
  netwib_ringitem *pnew, *pfirst;

  /* parameters verification */
  if (pring == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ring(pring));
#endif
  if (pring->numberofitems >= 0x7FFFFFFF) {
    return(NETWIB_ERR_PARINGFULL);
  }
  netwib__debug_ring_check("netwib_ring_add_first a", pring);

  /* alloc memory for the item */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_ringitem), (netwib_ptr*)&pnew));
  pnew->pitem = netwib_priv_castpptr(pitem);

  /* add it */
  pfirst = pring->pnext;
  pnew->pnext = pfirst;
  pnew->pprevious = (netwib_ringitem *)pring;
  pfirst->pprevious = pnew;
  pring->pnext = pnew;

  /* we added one item */
  pring->numberofitems++;

  netwib__debug_ring_check("netwib_ring_add_first b", pring);
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ring_add_last(netwib_ring *pring,
                                netwib_constptr pitem)
{
  netwib_ringitem *pnew, *plast;

  /* parameters verification */
  if (pring == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ring(pring));
#endif
  if (pring->numberofitems >= 0x7FFFFFFF) {
    return(NETWIB_ERR_PARINGFULL);
  }
  netwib__debug_ring_check("netwib_ring_add_last a", pring);

  /* alloc memory for the item */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_ringitem), (netwib_ptr*)&pnew));
  pnew->pitem = netwib_priv_castpptr(pitem);

  /* add it */
  plast = pring->pprevious;
  pnew->pprevious = plast;
  pnew->pnext = (netwib_ringitem *)pring;
  plast->pnext = pnew;
  pring->pprevious = pnew;

  /* we added one item */
  pring->numberofitems++;

  netwib__debug_ring_check("netwib_ring_add_last b", pring);
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ring_del_criteria(netwib_ring *pring,
                                    netwib_ring_criteria_pf pfunc_criteria,
                                    netwib_ptr pinfos,
                                    netwib_bool eraseitems)
{
  netwib_ringitem *pwork1, *pwork2, *pwork3;
  netwib_bool b;
  netwib_err ret;

  /* parameters verification */
  if (pring == NULL)
    return(NETWIB_ERR_PANULLPTR);
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ring(pring));
#endif
  netwib__debug_ring_check("netwib_ring_del_criteria a", pring);

  /* special case */
  if (pring->numberofitems == 0) {
    return(NETWIB_ERR_OK);
  }

  /* loop */
  b = NETWIB_TRUE;
  pwork1 = (netwib_ringitem*)pring;
  while (NETWIB_TRUE) {
    pwork2 = pwork1->pnext;
    if (pwork2 == (netwib_ringitem*)pring) {
      break;
    }
    if (pfunc_criteria != NULL) {
      b = NETWIB_FALSE;
      ret = (*pfunc_criteria)(pwork2->pitem, pinfos, &b);
      if (ret != NETWIB_ERR_OK) {
        /* do not unlink the item */
        netwib__debug_ring_check("netwib_ring_del_criteria c", pring);
        return(ret);
      }
    }
    if (b) {
      /* remove the item */
      if (pring->pfunc_erase != NULL && eraseitems) {
        ret = (*pring->pfunc_erase)(pwork2->pitem);
        if (ret != NETWIB_ERR_OK) {
          /* do not unlink the item */
          netwib__debug_ring_check("netwib_ring_del_criteria d", pring);
          return(ret);
        }
      }
      pwork3 = pwork2->pnext;
      netwib_er(netwib_ptr_free((netwib_ptr*)&pwork2));
      pwork1->pnext = pwork3;
      pwork3->pprevious = pwork1;
      pring->numberofitems--;
    } else {
      /* do not remove the item */
      pwork1 = pwork2;
    }
  }

  netwib__debug_ring_check("netwib_ring_del_criteria b", pring);
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ring_del_duplicate(netwib_ring *pring,
                                     netwib_ring_compare_pf pfunc_compare,
                                     netwib_ptr pinfos,
                                     netwib_bool eraseitems)
{
  netwib_ringitem *pworka, *pworkb1, *pworkb2, *pworkb3;
  netwib_cmp cmp;
  netwib_err ret;

  /* parameters verification */
  if (pring == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  if (pfunc_compare == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ring(pring));
#endif
  netwib__debug_ring_check("netwib_ring_del_duplicate a", pring);

  /* special case */
  if (pring->numberofitems < 2) {
    return(NETWIB_ERR_OK);
  }

  /* loop */
  pworka = (netwib_ringitem*)pring;
  while (NETWIB_TRUE) {
    pworka = pworka->pnext;
    if (pworka == (netwib_ringitem*)pring) {
      break;
    }
    pworkb1 = pworka;
    if (pworkb1->pnext == (netwib_ringitem*)pring) {
      break;
    }
    while (NETWIB_TRUE) {
      pworkb2 = pworkb1->pnext;
      if (pworkb2 == (netwib_ringitem*)pring) {
        break;
      }
      cmp = NETWIB_CMP_LT;
      ret = (*pfunc_compare) (pworka->pitem, pworkb2->pitem, pinfos, &cmp);
      if (ret != NETWIB_ERR_OK) {
        /* do not unlink the item */
        netwib__debug_ring_check("netwib_ring_del_duplicate c", pring);
        return(ret);
      }
      if (cmp == NETWIB_CMP_EQ) {
        /* remove the item */
        if (pring->pfunc_erase != NULL && eraseitems) {
          ret = (*pring->pfunc_erase) (pworkb2->pitem);
          if (ret != NETWIB_ERR_OK) {
            /* do not unlink the item */
            netwib__debug_ring_check("netwib_ring_del_duplicate d", pring);
            return(ret);
          }
        }
        pworkb3 = pworkb2->pnext;
        netwib_er(netwib_ptr_free((netwib_ptr*)&pworkb2));
        pworkb1->pnext = pworkb3;
        pworkb3->pprevious = pworkb1;
        pring->numberofitems--;
      } else {
        /* do not remove the item */
        pworkb1 = pworkb2;
      }
    }
  }

  netwib__debug_ring_check("netwib_ring_remove_duplicate_range b", pring);
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ring_sort(netwib_ring *pring,
                            netwib_ring_compare_pf pfunc_compare,
                            netwib_ptr pinfos)
{
  netwib_ringitem *pwork1, *pwork2, *psorted;
  netwib_ringitem *pl1, *pl2, *pl1last;
  netwib_cmp cmp;
  netwib_uint32 numloops, tmpvar, numinhalfring, numinhalfring2;
  netwib_uint32 itemstosort, numsubloops, l1size, l2size, l1l2size, i, j, k;
  netwib_bool comefroml1;
  netwib_err ret;

  /* parameters verification */
  if (pring == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  if (pfunc_compare == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ring(pring));
#endif
  netwib__debug_ring_check("netwib_ring_sort a", pring);

  /* special case */
  if (pring->numberofitems < 2) {
    return(NETWIB_ERR_OK);
  }

  /* determine the number of loops */
  tmpvar = 1;
  numloops = 0;
  itemstosort = pring->numberofitems;
  while (tmpvar < itemstosort) {
    numloops++;
    tmpvar <<= 1;
  }

  /* main loop */
  pwork1 = (netwib_ringitem *)pring;
  pwork2 = NULL;
  for (i = 0, numinhalfring = 1; i < numloops; i++, numinhalfring <<= 1) {
    psorted = pwork1;
    /* compute the number of subloops */
    numinhalfring2 = 2 * numinhalfring;
    numsubloops = (itemstosort + numinhalfring2 - 1) / numinhalfring2;
    /* sub-loop */
    for (j = 0; j < numsubloops; j++) {
      /* determine halfring sizes and go to working positions */
      if (j < numsubloops - 1) {
        l1size = numinhalfring;
        l2size = numinhalfring;
      } else {
        tmpvar = itemstosort - j * numinhalfring2;
        if (tmpvar > numinhalfring) {
          l1size = numinhalfring;
          l2size = tmpvar - numinhalfring;
        } else {
          l1size = tmpvar;
          l2size = 0;
        }
      }
      pl1 = psorted->pnext;
      pl1last = pl1;
      for (k = 1; k < l1size; k++) {
        pl1last = pl1last->pnext;
      }
      if (l2size) {
        pl2 = pl1last->pnext;
        l1l2size = l1size + l2size;
      } else {
        pl2 = NULL;
        l1l2size = l1size;
      }
      /* rearrange the ring using pl1 and pl2 */
      for (k = 0; k < l1l2size; k++) {
        /* find from where to come */
        comefroml1 = NETWIB_FALSE;
        if (l1size == 0) {
          if (l2size == 0) return(NETWIB_ERR_LOINTERNALERROR);
          /* comefroml1=NETWIB_FALSE; already set */
        } else if (l2size == 0) {
          if (l1size == 0) return(NETWIB_ERR_LOINTERNALERROR);
          comefroml1 = NETWIB_TRUE;
        } else {
          cmp = NETWIB_CMP_LT;
          ret = (*pfunc_compare)(pl1->pitem, pl2->pitem, pinfos, &cmp);
          if (ret != NETWIB_ERR_OK) {
            /* need to relink the ring */
            if (l1size != 0) {
              psorted->pnext = pl1;
              pl1->pprevious = psorted;
              psorted = pl1last;
            }
            if (l2size != 0) {
              psorted->pnext = pl2;
              pl2->pprevious = psorted;
            }
            netwib__debug_ring_check("netwib_ring_sort c", pring);
            return(ret);
          }
          if (cmp == NETWIB_CMP_LT || cmp == NETWIB_CMP_EQ) {
            comefroml1 = NETWIB_TRUE;
          }
          /* else comefroml1=NETWIB_FALSE; already set */
        }
        /* append the good item */
        if (comefroml1) {
          psorted->pnext = pl1;
          pl1->pprevious = psorted;
          pl1 = pl1->pnext;
          l1size--;
          if (pl2 == NULL && l1size == 0) pwork2 = pl1; /* save end */
        } else {
          psorted->pnext = pl2;
          pl2->pprevious = psorted;
          pl2 = pl2->pnext;
          l2size--;
          if (l2size == 0) pwork2 = pl2; /* save end */
        }
        psorted = psorted->pnext;
      }
      if (l1size || l2size) return(NETWIB_ERR_LOINTERNALERROR);
      /* just link the last item */
      psorted->pnext = pwork2;
      pwork2->pprevious = psorted;
#if NETWIB_DEBUG_RING_CHECK==1
      /* to debug : the ring should be OK at this point */
      netwib__debug_ring_check("netwib_ring_sort z1", pring);
#endif
    }
  }

  netwib__debug_ring_check("netwib_ring_sort b", pring);
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ring_group(netwib_ring *pring,
                             netwib_ring_compare_pf pfunc_compare,
                             netwib_ptr pinfos)
{
  netwib_ringitem *pworka, *pworkb1, *pworkb2, *pworkb3;
  netwib_cmp cmp;
  netwib_err ret;

  /* parameters verification */
  if (pring == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  if (pfunc_compare == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_ring(pring));
#endif
  netwib__debug_ring_check("netwib_ring_group a", pring);

  /* special case */
  if (pring->numberofitems < 2) {
    return(NETWIB_ERR_OK);
  }

  /* loop */
  pworka = (netwib_ringitem*)pring;
  while (NETWIB_TRUE) {
    pworka = pworka->pnext;
    if (pworka == (netwib_ringitem*)pring) {
      break;
    }
    pworkb1 = pworka;
    if (pworkb1->pnext == (netwib_ringitem*)pring) {
      break;
    }
    while (NETWIB_TRUE) {
      pworkb2 = pworkb1->pnext;
      if (pworkb2 == (netwib_ringitem*)pring) {
        break;
      }
      cmp = NETWIB_CMP_LT;
      ret = (*pfunc_compare) (pworka->pitem, pworkb2->pitem, pinfos, &cmp);
      if (ret != NETWIB_ERR_OK) {
        /* do not unlink the item */
        netwib__debug_ring_check("netwib_ring_group c", pring);
        return(ret);
      }
      if (cmp == NETWIB_CMP_EQ) {
        /* group the item */
        if (pworkb2 != pworka->pnext) {
          pworkb3 = pworkb2->pnext;
          pworkb1->pnext = pworkb3;
          pworkb3->pprevious = pworkb1;
          pworkb2->pnext = pworka->pnext;
          pworkb2->pprevious = pworka;
          pworka->pnext->pprevious = pworkb2;
          pworka->pnext = pworkb2;
          pworka = pworkb2;
        } else {
          pworkb1 = pworkb2;
          pworka = pworkb2;
        }
      } else {
        /* do not group the item */
        pworkb1 = pworkb2;
      }
    }
  }

  netwib__debug_ring_check("netwib_ring_group b", pring);
  return(NETWIB_ERR_OK);
}
