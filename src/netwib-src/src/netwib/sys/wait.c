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
struct netwib_wait {
  netwib_wait_event_pf pfuncevent;
  netwib_ptr infos;
  netwib_wait_close_pf pfuncclose;
};

/*-------------------------------------------------------------*/
netwib_err netwib_wait_init(netwib_wait_event_pf pfuncevent,
                            netwib_ptr infos,
                            netwib_wait_close_pf pfuncclose,
                            netwib_wait **ppwait)
{
  netwib_wait *pwait;

  /* allocate needed memory to store pwait */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_wait), (netwib_ptr*)&pwait));
  *ppwait = pwait;
  pwait->pfuncevent = pfuncevent;
  pwait->infos = infos;
  pwait->pfuncclose = pfuncclose;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_add_wait(pwait));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_wait_close(netwib_wait **ppwait)
{
  netwib_wait *pwait;

  /* parameter verification */
  if (ppwait == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pwait = *ppwait;

  if (pwait->pfuncclose != NULL) {
    netwib_er((*pwait->pfuncclose)(pwait->infos));
  }

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_del_wait(*ppwait));
#endif

  /* free memory */
  netwib_er(netwib_ptr_free((netwib_ptr*)ppwait));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
typedef struct {
  netwib_io *pio;
  netwib_io_waytype way;
} netwib_priv_wait_io;
static netwib_err netwib_priv_wait_event_io(netwib_ptr infos,
                                            netwib_consttime *pabstime,
                                            netwib_bool *pevent)
{
  netwib_priv_wait_io *pwio = (netwib_priv_wait_io *)infos;

  netwib_er(netwib_io_wait(pwio->pio, pwio->way, pabstime, pevent));
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_wait_close_io(netwib_ptr infos)
{
  netwib_er(netwib_ptr_free(&infos));
  return(NETWIB_ERR_OK);
}
netwib_err netwib_wait_init_io(netwib_io *pio,
                               netwib_io_waytype way,
                               netwib_wait **ppwait)
{
  netwib_priv_wait_io *pwio;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_wait_io),
                              (netwib_ptr*)&pwio));
  pwio->pio = pio;
  pwio->way = way;

  netwib_er(netwib_wait_init(&netwib_priv_wait_event_io, pwio,
                           &netwib_priv_wait_close_io, ppwait));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
typedef struct {
  netwib_thread *pthread;
  netwib_err *preturnederror;
  netwib_ptr *pinfosout;
} netwib_priv_wait_thread;
static netwib_err netwib_priv_wait_event_thread(netwib_ptr infos,
                                                netwib_consttime *pabstime,
                                                netwib_bool *pevent)
{
  netwib_priv_wait_thread *pwthread = (netwib_priv_wait_thread *)infos;

  netwib_er(netwib_thread_wait(pwthread->pthread, pabstime, pevent,
                               pwthread->preturnederror, pwthread->pinfosout));
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_wait_close_thread(netwib_ptr infos)
{
  netwib_er(netwib_ptr_free(&infos));
  return(NETWIB_ERR_OK);
}
netwib_err netwib_wait_init_thread_end(netwib_thread *pthread,
                                       netwib_err *preturnederror,
                                       netwib_ptr *pinfosout,
                                       netwib_wait **ppwait)
{
  netwib_priv_wait_thread *pwthread;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_wait_thread),
                              (netwib_ptr*)&pwthread));
  pwthread->pthread = pthread;
  pwthread->preturnederror = preturnederror;
  pwthread->pinfosout = pinfosout;

  netwib_er(netwib_wait_init(&netwib_priv_wait_event_thread, pwthread,
                             &netwib_priv_wait_close_thread, ppwait));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
typedef struct {
  netwib_thread_cond *pcond;
  netwib_uint32 *pvalue;
} netwib_priv_wait_thread_cond;
static netwib_err netwib_priv_wait_event_thread_cond(netwib_ptr infos,
                                                     netwib_consttime *pabstime,
                                                     netwib_bool *pevent)
{
  netwib_priv_wait_thread_cond *pwthread = (netwib_priv_wait_thread_cond *)infos;

  netwib_er(netwib_thread_cond_wait(pwthread->pcond, pabstime, pevent,
                                    pwthread->pvalue));
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_wait_close_thread_cond(netwib_ptr infos)
{
  netwib_er(netwib_ptr_free(&infos));
  return(NETWIB_ERR_OK);
}
netwib_err netwib_wait_init_thread_cond(netwib_thread_cond *pcond,
                                        netwib_uint32 *pvalue,
                                        netwib_wait **ppwait)
{
  netwib_priv_wait_thread_cond *pwthread;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_wait_thread_cond),
                              (netwib_ptr*)&pwthread));
  pwthread->pcond = pcond;
  pwthread->pvalue = pvalue;

  netwib_er(netwib_wait_init(&netwib_priv_wait_event_thread_cond, pwthread,
                             &netwib_priv_wait_close_thread_cond, ppwait));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_wait_wait1(netwib_wait *pwait,
                             netwib_consttime *pabstime,
                             netwib_bool *pevent)
{
  /* parameters verification */
  if (pwait == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_wait(pwait));
#endif

  netwib_er((*pwait->pfuncevent)(pwait->infos, pabstime, pevent));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_wait_wait5(netwib_wait *pwait1,
                                         netwib_wait *pwait2,
                                         netwib_wait *pwait3,
                                         netwib_wait *pwait4,
                                         netwib_wait *pwait5,
                                         netwib_bool *pevent,
                                         netwib_bool *pevent1,
                                         netwib_bool *pevent2,
                                         netwib_bool *pevent3,
                                         netwib_bool *pevent4,
                                         netwib_bool *pevent5)
{
  netwib_bool event;

  *pevent = NETWIB_FALSE;

  if (pwait1 != NULL) {
    netwib_er((*pwait1->pfuncevent)(pwait1->infos, NETWIB_TIME_ZERO, &event));
    if (pevent1 != NULL) *pevent1 = event;
    if (event) *pevent = NETWIB_TRUE;
  }
  if (pwait2 != NULL) {
    netwib_er((*pwait2->pfuncevent)(pwait2->infos, NETWIB_TIME_ZERO, &event));
    if (pevent2 != NULL) *pevent2 = event;
    if (event) *pevent = NETWIB_TRUE;
  }
  if (pwait3 != NULL) {
    netwib_er((*pwait3->pfuncevent)(pwait3->infos, NETWIB_TIME_ZERO, &event));
    if (pevent3 != NULL) *pevent3 = event;
    if (event) *pevent = NETWIB_TRUE;
  }
  if (pwait4 != NULL) {
    netwib_er((*pwait4->pfuncevent)(pwait4->infos, NETWIB_TIME_ZERO, &event));
    if (pevent4 != NULL) *pevent4 = event;
    if (event) *pevent = NETWIB_TRUE;
  }
  if (pwait5 != NULL) {
    netwib_er((*pwait5->pfuncevent)(pwait5->infos, NETWIB_TIME_ZERO, &event));
    if (pevent5 != NULL) *pevent5 = event;
    if (event) *pevent = NETWIB_TRUE;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_wait_wait5(netwib_wait *pwait1,
                             netwib_wait *pwait2,
                             netwib_wait *pwait3,
                             netwib_wait *pwait4,
                             netwib_wait *pwait5,
                             netwib_consttime *pabstime,
                             netwib_bool *pevent1,
                             netwib_bool *pevent2,
                             netwib_bool *pevent3,
                             netwib_bool *pevent4,
                             netwib_bool *pevent5)
{
  netwib_uint32 numcalls;
  netwib_bool event, elapsed;

#if NETWIB_DEBUG_LEAK==1
  if (pwait1 != NULL) {
    netwib_er(netwib_debug_leak_valid_wait(pwait1));
  }
  if (pwait2 != NULL) {
    netwib_er(netwib_debug_leak_valid_wait(pwait2));
  }
  if (pwait3 != NULL) {
    netwib_er(netwib_debug_leak_valid_wait(pwait3));
  }
  if (pwait4 != NULL) {
    netwib_er(netwib_debug_leak_valid_wait(pwait4));
  }
  if (pwait5 != NULL) {
    netwib_er(netwib_debug_leak_valid_wait(pwait5));
  }
#endif

  if (pabstime == NETWIB_TIME_ZERO) {
    netwib_er(netwib_priv_wait_wait5(pwait1, pwait2, pwait3, pwait4, pwait5,
                                     &event,
                                     pevent1, pevent2, pevent3, pevent4,
                                     pevent5));
    return(NETWIB_ERR_OK);
  }

  if (pabstime == NETWIB_TIME_INFINITE) {
    numcalls = 0;
    while (NETWIB_TRUE) {
      netwib_er(netwib_priv_wait_wait5(pwait1, pwait2, pwait3, pwait4, pwait5,
                                       &event,
                                       pevent1, pevent2, pevent3, pevent4,
                                       pevent5));
      if (event) {
        return(NETWIB_ERR_OK);
      }
      /* small pause */
      netwib_er(netwib_priv_pause2(&numcalls));
    }
  }

  numcalls = 0;
  while (NETWIB_TRUE) {
    /* check time */
    netwib_er(netwib_time_iselapsed(pabstime, &elapsed));
    if (elapsed) {
      if (pevent1 != NULL) *pevent1 = NETWIB_FALSE;
      if (pevent2 != NULL) *pevent2 = NETWIB_FALSE;
      if (pevent3 != NULL) *pevent3 = NETWIB_FALSE;
      if (pevent4 != NULL) *pevent4 = NETWIB_FALSE;
      if (pevent5 != NULL) *pevent5 = NETWIB_FALSE;
      break;
    }
    /* wait */
    netwib_er(netwib_priv_wait_wait5(pwait1, pwait2, pwait3, pwait4, pwait5,
                                     &event,
                                     pevent1, pevent2, pevent3, pevent4,
                                     pevent5));
    if (event) {
      break;
    }
    /* small pause */
    netwib_er(netwib_priv_pause2(&numcalls));
  }

  return(NETWIB_ERR_OK);
}
