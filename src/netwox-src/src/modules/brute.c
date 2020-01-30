/*
                                  NETWOX
                             Network toolbox
                Copyright(c) 1999-2012 Laurent CONSTANTIN
                      http://ntwox.sourceforge.net/
                        laurentconstantin@free.fr
                                  -----

  This file is part of Netwox.

  Netwox is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3
  as published by the Free Software Foundation.

  Netwox is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details (http://www.gnu.org/).

------------------------------------------------------------------------
*/

/*-------------------------------------------------------------*/
#include "../netwox.h"

/*-------------------------------------------------------------*/
static netwib_err netwox_brute_nextcouple(netwib_ring_index *pringloginindex,
                                          netwib_ring_index *pringpasswordindex,
                                          netwib_bufext *plogin,
                                          netwib_bufext *ppassword)
{
  netwib_ptr pitemlogin, pitempassword;
  netwib_err ret;

  /* obtain items */
  ret = netwib_ring_index_next(pringpasswordindex, &pitempassword);
  if (ret == NETWIB_ERR_DATAEND) {
    netwib_er(netwib_ring_index_next(pringloginindex, NULL));
    netwib_er(netwib_ring_index_ctl_set_rewind(pringpasswordindex));
    netwib_er(netwib_ring_index_next(pringpasswordindex, &pitempassword));
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }
  netwib_er(netwib_ring_index_this_value(pringloginindex, &pitemlogin));

  /* obtain values */
  netwib_er(netwox_wordlist_item_decode(pitemlogin, plogin));
  plogin->flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  netwib_er(netwox_wordlist_item_decode(pitempassword, ppassword));
  ppassword->flags |= NETWIB_BUF_FLAGS_SENSITIVE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_brute_core_monothread(netwib_ring_index *pringloginindex,
                                               netwib_ring_index *pringpasswordindex,
                                               netwib_ptr infos,
                                               netwib_bool stopatfirstfound,
                                               netwib_bool verbose,
                                               netwox_brute_pf pfunc)
{
  netwox_brute_result result;
  netwib_buf login, password;
  netwib_err ret=NETWIB_ERR_OK;

  while(NETWIB_TRUE) {
    /* obtain next login/password */
    ret = netwox_brute_nextcouple(pringloginindex, pringpasswordindex, &login,&password);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    /* test */
    if (verbose) {
      netwib_er(netwib_fmt_display("Trying \"%{buf}\" - \"%{buf}\"\n",
                                   &login, &password));
    }
    do {
      ret = (*pfunc)(&login, &password, infos, &result);
      if (ret != NETWIB_ERR_OK) break;
    } while (result == NETWOX_BRUTE_RESULT_RETRY && ret == NETWIB_ERR_OK);
    if (ret != NETWIB_ERR_OK) break;
    if (verbose) {
      netwib_er(netwib_fmt_display("Couple \"%{buf}\" - \"%{buf}\" -> %s\n", &login, &password, result==NETWOX_BRUTE_RESULT_GOOD?"good":(result==NETWOX_BRUTE_RESULT_BAD?"bad":"retry")));
    } else if (result == NETWOX_BRUTE_RESULT_GOOD) {
      netwib_er(netwib_fmt_display("\"%{buf}\" \"%{buf}\"\n", &login, &password));
    }
    if (result == NETWOX_BRUTE_RESULT_GOOD && stopatfirstfound) {
      break;
    }
  }

  return(ret);
}

/*-------------------------------------------------------------*/
/*
  We have two solutions to implement a multithread brute force :
   - launch numthread threads. They communicate with manager thread
     asking for new couples, and send back their answer. They stay alive
     during all the program duration : each thread deals with 100 or
     more couples
   - launch as many threads as couples to test. We ensure
     to not have more than numthread at the same time. Each thread
     deals only with one couple, then terminates.
  I choose to implement the second solution which is easier and faster
  to implement. However, it is less performant than first solution.
  If you have time, do not hesitate to implement first solution.
*/

/*-------------------------------------------------------------*/
/* information from manager to thread : initialized by manager and freed
   by thread */
typedef struct {
  netwib_buf login;
  netwib_buf password;
  netwib_ptr infos;
  netwox_brute_pf pfunc;
  netwib_bool verbose;
  netwib_uint32 threadid;
} netwox_brute_mt_manathr;

static netwib_err netwox_brute_mt_manathr_init(netwib_constbuf *plogin,
                                               netwib_constbuf *ppassword,
                                               netwib_ptr infos,
                                               netwox_brute_pf pfunc,
                                               netwib_bool verbose,
                                               netwib_uint32 threadid,
                                               netwox_brute_mt_manathr **ppbmt)
{
  netwox_brute_mt_manathr *pbmt;

  netwib_er(netwib_ptr_malloc(sizeof(netwox_brute_mt_manathr),
                              (netwib_ptr*)&pbmt));
  *ppbmt = pbmt;

  netwib_er(netwib_buf_init_mallocdefault(&pbmt->login));
  netwib_er(netwib_buf_append_buf(plogin, &pbmt->login));
  netwib_er(netwib_buf_init_mallocdefault(&pbmt->password));
  netwib_er(netwib_buf_append_buf(ppassword, &pbmt->password));
  pbmt->infos = infos;
  pbmt->pfunc = pfunc;
  pbmt->verbose = verbose;
  pbmt->threadid = threadid;

  return(NETWIB_ERR_OK);
}

static netwib_err netwox_brute_mt_manathr_close(netwox_brute_mt_manathr **ppbmt){
  netwox_brute_mt_manathr *pbmt;

  pbmt = *ppbmt;

  netwib_er(netwib_buf_close(&pbmt->login));
  netwib_er(netwib_buf_close(&pbmt->password));

  netwib_er(netwib_ptr_free((netwib_ptr*)&pbmt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* information from thread to manager : initialized by thread and freed
   by manager */
typedef struct {
  netwib_buf login;
  netwib_buf password;
  netwox_brute_result result;
} netwox_brute_mt_thrmana;

static netwib_err netwox_brute_mt_thrmana_init(netwib_constbuf *plogin,
                                               netwib_constbuf *ppassword,
                                               netwox_brute_result result,
                                               netwox_brute_mt_thrmana **ppbtm)
{
  netwox_brute_mt_thrmana *pbtm;

  netwib_er(netwib_ptr_malloc(sizeof(netwox_brute_mt_thrmana),
                              (netwib_ptr*)&pbtm));
  *ppbtm = pbtm;

  netwib_er(netwib_buf_init_mallocdefault(&pbtm->login));
  netwib_er(netwib_buf_append_buf(plogin, &pbtm->login));
  netwib_er(netwib_buf_init_mallocdefault(&pbtm->password));
  netwib_er(netwib_buf_append_buf(ppassword, &pbtm->password));
  pbtm->result = result;

  return(NETWIB_ERR_OK);
}

static netwib_err netwox_brute_mt_thrmana_close(netwox_brute_mt_thrmana **ppbtm){
  netwox_brute_mt_thrmana *pbtm;

  pbtm = *ppbtm;

  netwib_er(netwib_buf_close(&pbtm->login));
  netwib_er(netwib_buf_close(&pbtm->password));

  netwib_er(netwib_ptr_free((netwib_ptr*)&pbtm));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* each thread executes this function */
static netwib_err netwox_brute_mt_core(netwib_ptr infosin,
                                       netwib_ptr *pinfosout)
{
  netwox_brute_mt_manathr *pbmt = infosin;
  netwox_brute_mt_thrmana *pbtm;
  netwox_brute_result result=NETWOX_BRUTE_RESULT_RETRY;

  /* brute force */
  while (NETWIB_TRUE) {
    netwib_er((*pbmt->pfunc)(&pbmt->login, &pbmt->password, pbmt->infos, &result));
    if (result != NETWOX_BRUTE_RESULT_RETRY) {
      break;
    }
    if (pbmt->verbose) {
      netwib_er(netwib_fmt_display("Couple(thread%{uint32}) \"%{buf}\" - \"%{buf}\" -> retry\n", pbmt->threadid, &pbmt->login, &pbmt->password));
    }
    netwib_er(netwib_time_sleep_sec(1));
  }

  /* send info to manager */
  netwib_er(netwox_brute_mt_thrmana_init(&pbmt->login, &pbmt->password, result,
                                         &pbtm));
  netwib_er(netwox_brute_mt_manathr_close(&pbmt));
  *pinfosout = pbtm;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* manager thread */
static netwib_err netwox_brute_core_multithread(netwib_ring_index *pringloginindex,
                                                netwib_ring_index *pringpasswordindex,
                                                netwib_ptr infos,
                                                netwib_uint32 numthread,
                                                netwib_bool stopatfirstfound,
                                                netwib_bool verbose,
                                                netwox_brute_pf pfunc)
{
  netwib_buf login, password;
  netwib_ring *pthreadlist;
  netwib_uint32 count, threadid;
  netwib_bool morecoupletotest, event, newthreadlaunched;
  netwib_thread *pthread;
  netwox_brute_mt_manathr *pbmt;
  netwox_brute_mt_thrmana *pbtm;
  netwib_err ret=NETWIB_ERR_OK, ret2;

  netwib_er(netwib_threadlist_init(&pthreadlist));
  morecoupletotest = NETWIB_TRUE;
  threadid = 1;
  while(NETWIB_TRUE) {
    /* eventually launch a new thread */
    newthreadlaunched = NETWIB_FALSE;
    if (morecoupletotest) {
      netwib_er(netwib_ring_ctl_get_count(pthreadlist, &count));
      if (count < numthread) {
        /* obtain next login/password */
        ret = netwox_brute_nextcouple(pringloginindex, pringpasswordindex,
                                      &login, &password);
        if (ret == NETWIB_ERR_DATAEND) {
          morecoupletotest = NETWIB_FALSE;
        } else if (ret != NETWIB_ERR_OK) {
          break;
        } else {
          /* create a new thread */
          if (verbose) {
            netwib_er(netwib_fmt_display("Trying(thread%{uint32}) \"%{buf}\" - \"%{buf}\"\n", threadid, &login, &password));
          }
          netwib_er(netwox_brute_mt_manathr_init(&login, &password, infos, pfunc,
                                                 verbose, threadid, &pbmt));
          netwib_er(netwib_thread_init(&netwox_brute_mt_core, (netwib_ptr)pbmt,
                                       &pthread));
          netwib_er(netwib_threadlist_add(pthreadlist, pthread, threadid));
          threadid++;
          newthreadlaunched = NETWIB_TRUE;
        }
      }
    }
    /* check if one thread exited */
    ret = netwib_threadlist_wait(pthreadlist, newthreadlaunched?NETWIB_TIME_ZERO:NETWIB_TIME_INFINITE, &event, &threadid, &ret2, (netwib_ptr*)&pbtm);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATANOTAVAIL) {
        ret = NETWIB_ERR_OK;
        continue;
      }
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    if (event) {
      if (ret2 != NETWIB_ERR_OK) {
        ret = ret2;
        break;
      }
      if (verbose) {
        netwib_er(netwib_fmt_display("Couple(thread%{uint32}) \"%{buf}\" - \"%{buf}\" -> %s\n", threadid, &pbtm->login, &pbtm->password, pbtm->result==NETWOX_BRUTE_RESULT_GOOD?"good":(pbtm->result==NETWOX_BRUTE_RESULT_BAD?"bad":"retry")));
      } else if (pbtm->result == NETWOX_BRUTE_RESULT_GOOD) {
        netwib_er(netwib_fmt_display("\"%{buf}\" \"%{buf}\"\n",
                                     &pbtm->login, &pbtm->password));
      }
      if (pbtm->result == NETWOX_BRUTE_RESULT_GOOD && stopatfirstfound) {
        morecoupletotest = NETWIB_FALSE;
      }
      netwib_er(netwox_brute_mt_thrmana_close(&pbtm));
    }
  }

  netwib_er(netwib_threadlist_close(&pthreadlist));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_brute(netwib_ring *pringlogin,
                        netwib_ring *pringpassword,
                        netwib_ptr infos,
                        netwib_uint32 numthread,
                        netwib_bool stopatfirstfound,
                        netwib_bool verbose,
                        netwox_brute_pf pfunc)
{
  netwib_ring_index *pringloginindex, *pringpasswordindex;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_ring_index_init(pringlogin, &pringloginindex));
  netwib_er(netwib_ring_index_next(pringloginindex, NULL));
  netwib_er(netwib_ring_index_init(pringpassword, &pringpasswordindex));

  if (numthread <= 1) {
    ret = netwox_brute_core_monothread(pringloginindex, pringpasswordindex,
                                       infos, stopatfirstfound,
                                       verbose, pfunc);
  } else {
    ret = netwox_brute_core_multithread(pringloginindex, pringpasswordindex,
                                        infos, numthread, stopatfirstfound,
                                        verbose, pfunc);
  }

  netwib_er(netwib_ring_index_close(&pringloginindex));
  netwib_er(netwib_ring_index_close(&pringpasswordindex));
  return(ret);
}
