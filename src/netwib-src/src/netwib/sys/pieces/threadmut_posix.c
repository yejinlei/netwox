/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
struct netwib_thread_mutex {
  pthread_mutex_t mut;
};

/*-------------------------------------------------------------*/
netwib_err netwib_thread_mutex_init(netwib_thread_mutex **ppmutex)
{
  netwib_thread_mutex *pmutex;
  int reti;

  /* parameter verification */
  if (ppmutex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* allocate needed memory to store mutex */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_thread_mutex),
                              (netwib_ptr*)&pmutex));
  *ppmutex = pmutex;

  /* initialize mutex */
#if NETWIBDEF_LIBPTHREADFROMDCE == 1
  reti = pthread_mutex_init(&pmutex->mut, pthread_mutexattr_default);
#else
  reti = pthread_mutex_init(&pmutex->mut, NULL);
#endif
  if (reti) {
    netwib_er(netwib_ptr_free((netwib_ptr*)&pmutex));
    return(NETWIB_ERR_FUPTHREADMUTEXINIT);
  }

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_add_thread_mutex(pmutex));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_thread_mutex_close(netwib_thread_mutex **ppmutex)
{
  netwib_thread_mutex *pmutex;
  int reti;

  /* parameter verification */
  if (ppmutex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pmutex = *ppmutex;
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_mutex(pmutex));
#endif

  /* close mutex */
  reti = pthread_mutex_destroy(&pmutex->mut);
  if (reti) return(NETWIB_ERR_FUPTHREADMUTEXDESTROY);

  /* free memory */
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_del_thread_mutex(pmutex));
#endif
  netwib_er(netwib_ptr_free((netwib_ptr*)&pmutex));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_thread_mutex_timedlock(netwib_thread_mutex *pmutex,
                                                     netwib_consttime *pabstime,
                                                     netwib_bool *plocked);
#if NETWIBDEF_HAVEFUNC_PTHREAD_MUTEX_TIMEDLOCK == 1
static netwib_err netwib_priv_thread_mutex_timedlock(netwib_thread_mutex *pmutex,
                                                     netwib_consttime *pabstime,
                                                     netwib_bool *plocked)
{
  struct timespec ts;
  int reti;

  netwib_er(netwib_priv_time_timeout_thread(pabstime, &ts));
  reti = pthread_mutex_timedlock(&pmutex->mut, &ts);
  if (reti == ETIMEDOUT) {
    if (plocked != NULL) *plocked = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  } else if (reti) {
    return(NETWIB_ERR_FUPTHREADMUTEXTIMEDLOCK);
  }

  if (plocked != NULL) *plocked = NETWIB_TRUE;
  return(NETWIB_ERR_OK);
}
#else
static netwib_err netwib_priv_thread_mutex_timedlock(netwib_thread_mutex *pmutex,
                                                     netwib_consttime *pabstime,
                                                     netwib_bool *plocked)
{
  netwib_uint32 numcalls;
  netwib_bool elapsed;
  int reti;

  numcalls = 0;
  while (NETWIB_TRUE) {
    /* check time */
    netwib_er(netwib_time_iselapsed(pabstime, &elapsed));
    if (elapsed) {
      if (plocked != NULL) *plocked = NETWIB_FALSE;
      break;
    }
    /* wait */
    reti = pthread_mutex_trylock(&pmutex->mut);
#if NETWIBDEF_LIBPTHREADFROMDCE == 1
    if (reti == 1) reti = 0;
    else if (reti == 0) reti = EBUSY;
#endif
    if (reti == EBUSY || reti == EAGAIN) {
      /* continue this loop */
    } else if (reti) {
      return(NETWIB_ERR_FUPTHREADMUTEXTRYLOCK);
    } else {
      /* obtained */
      if (plocked != NULL) *plocked = NETWIB_TRUE;
      break;
    }
    /* small pause */
    netwib_er(netwib_priv_pause2(&numcalls));
  }

  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_thread_mutex_lock(netwib_thread_mutex *pmutex,
                                    netwib_consttime *pabstime,
                                    netwib_bool *plocked)
{
  int reti;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_mutex(pmutex));
#endif

  if (pabstime == NETWIB_TIME_ZERO) {
    reti = pthread_mutex_trylock(&pmutex->mut);
#if NETWIBDEF_LIBPTHREADFROMDCE == 1
    if (reti == 1) reti = 0;
    else if (reti == 0) reti = EBUSY;
#endif
    if (reti == EBUSY || reti == EAGAIN) {
      if (plocked != NULL) *plocked = NETWIB_FALSE;
      return(NETWIB_ERR_OK);
    } else if (reti) {
      return(NETWIB_ERR_FUPTHREADMUTEXTRYLOCK);
    }
  } else if (pabstime == NETWIB_TIME_INFINITE) {
    reti = pthread_mutex_lock(&pmutex->mut);
    if (reti) return(NETWIB_ERR_FUPTHREADMUTEXLOCK);
      if (plocked != NULL) *plocked = NETWIB_TRUE;
  } else {
    netwib_er(netwib_priv_thread_mutex_timedlock(pmutex, pabstime, plocked));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_thread_mutex_unlock(netwib_thread_mutex *pmutex)
{
  int reti;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_mutex(pmutex));
#endif

  reti = pthread_mutex_unlock(&pmutex->mut);
  if (reti) return(NETWIB_ERR_FUPTHREADMUTEXUNLOCK);

  return(NETWIB_ERR_OK);
}
