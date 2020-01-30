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
static netwib_err netwib_priv_threadringitem_create(netwib_thread *pthread,
                                                    netwib_uint32 threadid,
                                                    netwib_ptr *ppitem)
{
  netwib_ptr ptr;
  netwib_threadringitem *pthreadringitem;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_threadringitem), &ptr));
  pthreadringitem = (netwib_threadringitem *)ptr;

  pthreadringitem->pthread = pthread;
  pthreadringitem->threadid = threadid;

  *ppitem = ptr;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_threadringitem_erase(netwib_ptr pitem)
{
  netwib_er(netwib_ptr_free(&pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_threadringitem_duplicate(netwib_constptr pitem,
                                                       netwib_ptr *pdupofitem)
{
  const netwib_threadringitem *pthreadringitem = (const netwib_threadringitem *)pitem;

  netwib_er(netwib_priv_threadringitem_create(pthreadringitem->pthread,
                                              pthreadringitem->threadid,
                                              pdupofitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_threadlist_init(netwib_ring **ppring)
{
  netwib_er(netwib_ring_init(&netwib_priv_threadringitem_erase,
                             &netwib_priv_threadringitem_duplicate,
                             ppring));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_threadlist_close(netwib_ring **ppring)
{
  netwib_err ret=NETWIB_ERR_OK;

  /* wait for threads ignoring thread errors */
  while (NETWIB_TRUE) {
    ret = netwib_threadlist_wait(*ppring, NETWIB_TIME_INFINITE,
                                 NULL, NULL, NULL/*ignore*/, NULL);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
  }

  netwib_er(netwib_ring_close(ppring, NETWIB_TRUE));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_threadlist_add(netwib_ring *pring,
                                 netwib_thread *pthread,
                                 netwib_uint32 threadid)
{
  netwib_ptr pitem;

  netwib_er(netwib_priv_threadringitem_create(pthread, threadid, &pitem));
  netwib_er(netwib_ring_add_last(pring, pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_threadlist_del(netwib_constptr pitem,
                                             netwib_ptr pinfos,
                                             netwib_bool *pbool)
{
  const netwib_threadringitem *pthreadringitem = (const netwib_threadringitem *)pitem;
  netwib_uint32 threadid = *(netwib_uint32*)pinfos;

  if (pthreadringitem->threadid == threadid) {
    *pbool = NETWIB_TRUE;
  } else {
    *pbool = NETWIB_FALSE;
  }
  return(NETWIB_ERR_OK);
}
netwib_err netwib_threadlist_del(netwib_ring *pring,
                                 netwib_uint32 threadid)
{
  /* we remove all items having the same id : this is user's bug because
     id should be unique */
  netwib_er(netwib_ring_del_criteria(pring, &netwib_priv_threadlist_del,
                                     &threadid, NETWIB_TRUE));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_threadlist_wait(netwib_ring_index *pringindex,
                                              netwib_bool *pevent,
                                              netwib_uint32 *pthreadid,
                                              netwib_err *preturnederror,
                                              netwib_ptr *pinfosout)
{
  netwib_threadringitem *pthreadringitem;
  netwib_bool event;
  netwib_err ret;

  netwib_er(netwib_ring_index_ctl_set_rewind(pringindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pthreadringitem);
    if (ret == NETWIB_ERR_DATAEND) {
      ret = NETWIB_ERR_OK;
      break;
    } else if (ret != NETWIB_ERR_OK) {
      return(ret);
    }
    netwib_er(netwib_thread_wait(pthreadringitem->pthread, NETWIB_TIME_ZERO,
                                 &event, preturnederror, pinfosout));
    if (event) {
      if (pevent != NULL) *pevent = event;
      if (pthreadid != NULL) *pthreadid = pthreadringitem->threadid;
      netwib_er(netwib_thread_close(&pthreadringitem->pthread));
      netwib_er(netwib_ring_index_this_del(pringindex, NETWIB_TRUE));
      return(NETWIB_ERR_OK);
    }
  }

  if (pevent != NULL) *pevent = NETWIB_FALSE;
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_threadlist_wait(netwib_ring *pring,
                                  netwib_consttime *pabstime,
                                  netwib_bool *pevent,
                                  netwib_uint32 *pthreadid,
                                  netwib_err *preturnederror,
                                  netwib_ptr *pinfosout)
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
    netwib_er(netwib_priv_threadlist_wait(pringindex, pevent, pthreadid,
                                          preturnederror, pinfosout));
    netwib_er(netwib_ring_index_close(&pringindex));
    return(NETWIB_ERR_OK);
  }

  if (pabstime == NETWIB_TIME_INFINITE) {
    numcalls = 0;
    netwib_er(netwib_ring_index_init(pring, &pringindex));
    while (NETWIB_TRUE) {
      netwib_er(netwib_priv_threadlist_wait(pringindex, &event, pthreadid,
                                            preturnederror, pinfosout));
      if (event) {
        netwib_er(netwib_ring_index_close(&pringindex));
        if (pevent != NULL) *pevent = NETWIB_TRUE;
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
    ret = netwib_priv_threadlist_wait(pringindex, &event, pthreadid,
                                      preturnederror, pinfosout);
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

