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
static netwib_err netwib_priv_waitringitem_create(netwib_wait *pwait,
                                                  netwib_uint32 waitident,
                                                  netwib_ptr *ppitem)
{
  netwib_ptr ptr;
  netwib_waitringitem *pwaitringitem;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_waitringitem), &ptr));
  pwaitringitem = (netwib_waitringitem *)ptr;

  pwaitringitem->pwait = pwait;
  pwaitringitem->waitident = waitident;

  *ppitem = ptr;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_waitringitem_erase(netwib_ptr pitem)
{
  netwib_er(netwib_ptr_free(&pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_waitringitem_duplicate(netwib_constptr pitem,
                                                     netwib_ptr *pdupofitem)
{
  const netwib_waitringitem *pwaitringitem = (const netwib_waitringitem *)pitem;

  netwib_er(netwib_priv_waitringitem_create(pwaitringitem->pwait,
                                            pwaitringitem->waitident,
                                            pdupofitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_waitlist_init(netwib_ring **ppring)
{
  netwib_er(netwib_ring_init(&netwib_priv_waitringitem_erase,
                             &netwib_priv_waitringitem_duplicate,
                             ppring));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_waitlist_add(netwib_ring *pring,
                               netwib_wait *pwait,
                               netwib_uint32 waitident)
{
  netwib_ptr pitem;

  netwib_er(netwib_priv_waitringitem_create(pwait, waitident, &pitem));
  netwib_er(netwib_ring_add_last(pring, pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_waitlist_del(netwib_constptr pitem,
                                           netwib_ptr pinfos,
                                           netwib_bool *pbool)
{
  const netwib_waitringitem *pwaitringitem = (const netwib_waitringitem *)pitem;
  netwib_uint32 waitident = *(netwib_uint32*)pinfos;

  if (pwaitringitem->waitident == waitident) {
    *pbool = NETWIB_TRUE;
  } else {
    *pbool = NETWIB_FALSE;
  }
  return(NETWIB_ERR_OK);
}
netwib_err netwib_waitlist_del(netwib_ring *pring,
                               netwib_uint32 waitident)
{
  /* we remove all items having the same id : this is user's bug because
     id should be unique */
  netwib_er(netwib_ring_del_criteria(pring, &netwib_priv_waitlist_del,
                                     &waitident, NETWIB_TRUE));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_waitlist_wait(netwib_ring_index *pringindex,
                                            netwib_bool *pevent,
                                            netwib_ring **ppringofwaitid)
{
  netwib_waitringitem *pwaitringitem;
  netwib_bool event, atleastoneevent;
  netwib_err ret;

  netwib_er(netwib_ring_index_ctl_set_rewind(pringindex));

  atleastoneevent = NETWIB_FALSE;
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pwaitringitem);
    if (ret == NETWIB_ERR_DATAEND) {
      ret = NETWIB_ERR_OK;
      break;
    } else if (ret != NETWIB_ERR_OK) {
      return(ret);
    }
    netwib_er(netwib_wait_wait1(pwaitringitem->pwait, NETWIB_TIME_ZERO,
                                &event));
    if (event) {
      if (ppringofwaitid != NULL) {
        if (!atleastoneevent) {
          netwib_er(netwib_ring_init(NULL, NULL, ppringofwaitid));
        }
        netwib_er(netwib_ring_add_last(*ppringofwaitid,
                                       (netwib_ptr)(netwib_uintptr)
                                       pwaitringitem->waitident));
      }
      atleastoneevent = NETWIB_TRUE;
    }
  }

  if (pevent != NULL) *pevent = atleastoneevent;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_waitlist_wait(netwib_ring *pring,
                                netwib_consttime *pabstime,
                                netwib_bool *pevent,
                                netwib_ring **ppringofwaitid)
{
  netwib_ring_index *pringindex;
  netwib_uint32 numcalls, count;
  netwib_bool event, elapsed;
  netwib_err ret;

  netwib_er(netwib_ring_ctl_get_count(pring, &count));
  if (count == 0) {
    return(NETWIB_ERR_DATAEND);
  }

  if (pabstime == NETWIB_TIME_ZERO) {
    netwib_er(netwib_ring_index_init(pring, &pringindex));
    netwib_er(netwib_priv_waitlist_wait(pringindex, pevent,
                                        ppringofwaitid));
    netwib_er(netwib_ring_index_close(&pringindex));
  }

  if (pabstime == NETWIB_TIME_INFINITE) {
    numcalls = 0;
    netwib_er(netwib_ring_index_init(pring, &pringindex));
    while (NETWIB_TRUE) {
      netwib_er(netwib_priv_waitlist_wait(pringindex, &event, ppringofwaitid));
      if (event) {
        if (pevent != NULL) *pevent = NETWIB_TRUE;
        netwib_er(netwib_ring_index_close(&pringindex));
        return(NETWIB_ERR_OK);
      }
      netwib_er(netwib_priv_pause2(&numcalls));
    }
  }

  netwib_er(netwib_ring_index_init(pring, &pringindex));
  ret = NETWIB_ERR_LOINTERNALERROR;
  event = NETWIB_FALSE;
  numcalls = 0;
  while (NETWIB_TRUE) {
    /* check time */
    netwib_er(netwib_time_iselapsed(pabstime, &elapsed));
    if (elapsed) {
      event = NETWIB_FALSE;
      break;
    }
    /* wait */
    ret = netwib_priv_waitlist_wait(pringindex, &event, ppringofwaitid);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (event) {
      break;
    }
    /* small pause */
    netwib_er(netwib_priv_pause2(&numcalls));
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  if (pevent != NULL) *pevent = event;
  return(ret);
}
