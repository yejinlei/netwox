/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
struct netwib_thread_rwlock {
  netwib_thread_mutex *prdmut;
  netwib_thread_mutex *pwrmut;
  netwib_uint32 numreaders;
};

/*-------------------------------------------------------------*/
netwib_err netwib_thread_rwlock_init(netwib_thread_rwlock **pprwlock)
{
  netwib_thread_rwlock *prwlock;

  /* parameter verification */
  if (pprwlock == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* allocate needed memory to store rwlock */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_thread_rwlock),
                              (netwib_ptr*)&prwlock));
  *pprwlock = prwlock;

  /* initialize rwlock */
  netwib_er(netwib_thread_mutex_init(&prwlock->prdmut));
  netwib_er(netwib_thread_mutex_init(&prwlock->pwrmut));
  prwlock->numreaders = 0;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_add_thread_rwlock(prwlock));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_thread_rwlock_close(netwib_thread_rwlock **pprwlock)
{
  netwib_thread_rwlock *prwlock;

  /* parameter verification */
  if (pprwlock == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  prwlock = *pprwlock;
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_rwlock(prwlock));
#endif

  /* close rwlock */
  netwib_er(netwib_thread_mutex_close(&prwlock->prdmut));
  netwib_er(netwib_thread_mutex_close(&prwlock->pwrmut));

  /* free memory */
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_del_thread_rwlock(prwlock));
#endif
  netwib_er(netwib_ptr_free((netwib_ptr*)&prwlock));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_thread_rwlock_rdlock(netwib_thread_rwlock *prwlock,
                                       netwib_consttime *pabstime,
                                       netwib_bool *plocked)
{
  netwib_bool locked;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_rwlock(prwlock));
#endif

  netwib_er(netwib_thread_mutex_lock(prwlock->pwrmut, pabstime, &locked));
  if (!locked) {
    if (plocked != NULL) *plocked = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  }
  netwib_er(netwib_thread_mutex_lock(prwlock->prdmut,
                                     NETWIB_TIME_INFINITE,NULL));
  prwlock->numreaders++;
  netwib_er(netwib_thread_mutex_unlock(prwlock->prdmut));
  netwib_er(netwib_thread_mutex_unlock(prwlock->pwrmut));

  if (plocked != NULL) *plocked = NETWIB_TRUE;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_thread_rwlock_rdunlock(netwib_thread_rwlock *prwlock)
{

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_rwlock(prwlock));
#endif

  netwib_er(netwib_thread_mutex_lock(prwlock->prdmut,
                                     NETWIB_TIME_INFINITE,NULL));
  prwlock->numreaders--;
  netwib_er(netwib_thread_mutex_unlock(prwlock->prdmut));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_thread_rwlock_timedwrlock(netwib_thread_rwlock *prwlock,
                                                        netwib_consttime *pabstime,
                                                        netwib_bool *plocked)
{
  netwib_uint32 numcalls;
  netwib_bool locked, elapsed;

  locked = NETWIB_FALSE;
  numcalls = 0;
  while (NETWIB_TRUE) {
    /* check time */
    netwib_er(netwib_time_iselapsed(pabstime, &elapsed));
    if (elapsed) {
      break;
    }
    /* wait */
    netwib_er(netwib_thread_rwlock_wrlock(prwlock, NETWIB_TIME_ZERO, &locked));
    if (locked) {
      break;
    }
    /* small pause */
    netwib_er(netwib_priv_pause2(&numcalls));
  }

  if (plocked != NULL) *plocked = locked;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_thread_rwlock_wrlock(netwib_thread_rwlock *prwlock,
                                       netwib_consttime *pabstime,
                                       netwib_bool *plocked)
{
  netwib_uint32 numcalls;
  netwib_bool locked;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_rwlock(prwlock));
#endif

  if (pabstime == NETWIB_TIME_ZERO) {
    netwib_er(netwib_thread_mutex_lock(prwlock->pwrmut, NETWIB_TIME_ZERO,
                                       &locked));
    if (!locked) {
      if (plocked != NULL) *plocked = NETWIB_FALSE;
      return(NETWIB_ERR_OK);
    }
    netwib_er(netwib_thread_mutex_lock(prwlock->prdmut, NETWIB_TIME_INFINITE,
                                       NULL));
    locked = NETWIB_TRUE;
    if (prwlock->numreaders) {
      netwib_er(netwib_thread_mutex_unlock(prwlock->pwrmut));
      locked = NETWIB_FALSE;
    }
    netwib_er(netwib_thread_mutex_unlock(prwlock->prdmut));
    if (plocked != NULL) *plocked = locked;
    return(NETWIB_ERR_OK);
  }

  if (pabstime == NETWIB_TIME_INFINITE) {
    numcalls = 0;
    while (NETWIB_TRUE) {
      netwib_er(netwib_thread_mutex_lock(prwlock->pwrmut, NETWIB_TIME_INFINITE,
                                         NULL));
      netwib_er(netwib_thread_mutex_lock(prwlock->prdmut, NETWIB_TIME_INFINITE,
                                         NULL));
      locked = NETWIB_TRUE;
      if (prwlock->numreaders) {
        netwib_er(netwib_thread_mutex_unlock(prwlock->pwrmut));
        locked = NETWIB_FALSE;
      }
      netwib_er(netwib_thread_mutex_unlock(prwlock->prdmut));
      if (locked) {
        if (plocked != NULL) *plocked = NETWIB_TRUE;
        return(NETWIB_ERR_OK);
      }
      /* small pause */
      netwib_er(netwib_priv_pause2(&numcalls));
    }
  }

  netwib_er(netwib_priv_thread_rwlock_timedwrlock(prwlock, pabstime, plocked));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_thread_rwlock_wrunlock(netwib_thread_rwlock *prwlock)
{

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_rwlock(prwlock));
#endif

  netwib_er(netwib_thread_mutex_unlock(prwlock->pwrmut));

  return(NETWIB_ERR_OK);
}
