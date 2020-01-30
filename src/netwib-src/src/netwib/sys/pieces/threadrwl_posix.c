/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
struct netwib_thread_rwlock {
  pthread_rwlock_t rwl;
};

/*-------------------------------------------------------------*/
netwib_err netwib_thread_rwlock_init(netwib_thread_rwlock **pprwlock)
{
  netwib_thread_rwlock *prwlock;
  int reti;

  /* parameter verification */
  if (pprwlock == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* allocate needed memory to store rwlock */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_thread_rwlock),
                              (netwib_ptr*)&prwlock));
  *pprwlock = prwlock;

  /* initialize rwlock */
  reti = pthread_rwlock_init(&prwlock->rwl, NULL);
  if (reti) {
    netwib_er(netwib_ptr_free((netwib_ptr*)&prwlock));
    return(NETWIB_ERR_FUPTHREADRWLOCKINIT);
  }

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_add_thread_rwlock(prwlock));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_thread_rwlock_close(netwib_thread_rwlock **pprwlock)
{
  netwib_thread_rwlock *prwlock;
  int reti;

  /* parameter verification */
  if (pprwlock == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  prwlock = *pprwlock;
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_rwlock(prwlock));
#endif

  /* close rwlock */
  reti = pthread_rwlock_destroy(&prwlock->rwl);
  if (reti) return(NETWIB_ERR_FUPTHREADRWLOCKDESTROY);

  /* free memory */
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_del_thread_rwlock(prwlock));
#endif
  netwib_er(netwib_ptr_free((netwib_ptr*)&prwlock));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_thread_rwlock_timedrdlock(netwib_thread_rwlock *prwlock,
                                                        netwib_consttime *pabstime,
                                                        netwib_bool *plocked);
#if NETWIBDEF_HAVEFUNC_PTHREAD_RWLOCK_TIMEDRDLOCK == 1
static netwib_err netwib_priv_thread_rwlock_timedrdlock(netwib_thread_rwlock *prwlock,
                                                        netwib_consttime *pabstime,
                                                        netwib_bool *plocked)
{
  struct timespec ts;
  int reti;

  netwib_er(netwib_priv_time_timeout_thread(pabstime, &ts));
  reti = pthread_rwlock_timedrdlock(&prwlock->rwl, &ts);
  if (reti == ETIMEDOUT) {
    if (plocked != NULL) *plocked = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  } else if (reti) {
    return(NETWIB_ERR_FUPTHREADRWLOCKTIMEDRDLOCK);
  }

  if (plocked != NULL) *plocked = NETWIB_TRUE;
  return(NETWIB_ERR_OK);
}
#else
static netwib_err netwib_priv_thread_rwlock_timedrdlock(netwib_thread_rwlock *prwlock,
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
    reti = pthread_rwlock_tryrdlock(&prwlock->rwl);
    if (reti == EBUSY || reti == EAGAIN) {
      /* continue this loop */
    } else if (reti) {
      return(NETWIB_ERR_FUPTHREADRWLOCKTRYRDLOCK);
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
netwib_err netwib_thread_rwlock_rdlock(netwib_thread_rwlock *prwlock,
                                       netwib_consttime *pabstime,
                                       netwib_bool *plocked)
{
  int reti;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_rwlock(prwlock));
#endif

  if (pabstime == NETWIB_TIME_ZERO) {
    reti = pthread_rwlock_tryrdlock(&prwlock->rwl);
    if (reti == EBUSY || reti == EAGAIN) {
      if (plocked != NULL) *plocked = NETWIB_FALSE;
      return(NETWIB_ERR_OK);
    } else if (reti) {
      return(NETWIB_ERR_FUPTHREADRWLOCKTRYRDLOCK);
    }
  } else if (pabstime == NETWIB_TIME_INFINITE) {
    reti = pthread_rwlock_rdlock(&prwlock->rwl);
    if (reti) return(NETWIB_ERR_FUPTHREADRWLOCKRDLOCK);
    if (plocked != NULL) *plocked = NETWIB_TRUE;
  } else {
    netwib_er(netwib_priv_thread_rwlock_timedrdlock(prwlock, pabstime,
                                                    plocked));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_thread_rwlock_rdunlock(netwib_thread_rwlock *prwlock)
{
  int reti;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_rwlock(prwlock));
#endif

  reti = pthread_rwlock_unlock(&prwlock->rwl);
  if (reti) return(NETWIB_ERR_FUPTHREADRWLOCKUNLOCK);

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
static netwib_err netwib_priv_thread_rwlock_timedwrlock(netwib_thread_rwlock *prwlock,
                                                        netwib_consttime *pabstime,
                                                        netwib_bool *plocked);
#if NETWIBDEF_HAVEFUNC_PTHREAD_RWLOCK_TIMEDWRLOCK == 1
static netwib_err netwib_priv_thread_rwlock_timedwrlock(netwib_thread_rwlock *prwlock,
                                                        netwib_consttime *pabstime,
                                                        netwib_bool *plocked)
{
  struct timespec ts;
  int reti;

  netwib_er(netwib_priv_time_timeout_thread(pabstime, &ts));
  reti = pthread_rwlock_timedwrlock(&prwlock->rwl, &ts);
  if (reti == ETIMEDOUT) {
    if (plocked != NULL) *plocked = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  } else if (reti) {
    return(NETWIB_ERR_FUPTHREADRWLOCKTIMEDWRLOCK);
  }

  if (plocked != NULL) *plocked = NETWIB_TRUE;
  return(NETWIB_ERR_OK);
}
#else
static netwib_err netwib_priv_thread_rwlock_timedwrlock(netwib_thread_rwlock *prwlock,
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
    reti = pthread_rwlock_trywrlock(&prwlock->rwl);
    if (reti == EBUSY || reti == EAGAIN) {
      /* continue this loop */
    } else if (reti) {
      return(NETWIB_ERR_FUPTHREADRWLOCKTRYWRLOCK);
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
netwib_err netwib_thread_rwlock_wrlock(netwib_thread_rwlock *prwlock,
                                       netwib_consttime *pabstime,
                                       netwib_bool *plocked)
{
  int reti;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_rwlock(prwlock));
#endif

  if (pabstime == NETWIB_TIME_ZERO) {
    reti = pthread_rwlock_trywrlock(&prwlock->rwl);
    if (reti == EBUSY || reti == EAGAIN) {
      if (plocked != NULL) *plocked = NETWIB_FALSE;
      return(NETWIB_ERR_OK);
    } else if (reti) {
      return(NETWIB_ERR_FUPTHREADRWLOCKTRYWRLOCK);
    }
  } else if (pabstime == NETWIB_TIME_INFINITE) {
    reti = pthread_rwlock_wrlock(&prwlock->rwl);
    if (reti) return(NETWIB_ERR_FUPTHREADRWLOCKWRLOCK);
    if (plocked != NULL) *plocked = NETWIB_TRUE;
  } else {
    netwib_er(netwib_priv_thread_rwlock_timedwrlock(prwlock, pabstime,
                                                    plocked));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_thread_rwlock_wrunlock(netwib_thread_rwlock *prwlock)
{
  int reti;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_rwlock(prwlock));
#endif

  reti = pthread_rwlock_unlock(&prwlock->rwl);
  if (reti) return(NETWIB_ERR_FUPTHREADRWLOCKUNLOCK);

  return(NETWIB_ERR_OK);
}
