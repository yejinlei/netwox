/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
struct netwib_thread_cond {
  pthread_cond_t cond;
  pthread_mutex_t mutex;
  netwib_bool reached;
  netwib_uint32 value;
};

/*-------------------------------------------------------------*/
netwib_err netwib_thread_cond_init(netwib_thread_cond **ppcond)
{
  netwib_thread_cond *pcond;
  int reti;

  /* parameter verification */
  if (ppcond == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* allocate needed memory to store cond */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_thread_cond),
                              (netwib_ptr*)&pcond));
  *ppcond = pcond;

  /* initialize cond */
  pcond->reached = NETWIB_FALSE;
  netwib_debug_notset_uint32(pcond->value);
#if NETWIBDEF_LIBPTHREADFROMDCE == 1
  reti = pthread_mutex_init(&pcond->mutex, pthread_mutexattr_default);
#else
  reti = pthread_mutex_init(&pcond->mutex, NULL);
#endif
  if (reti) {
    netwib_er(netwib_ptr_free((netwib_ptr*)&pcond));
    return(NETWIB_ERR_FUPTHREADMUTEXINIT);
  }
#if NETWIBDEF_LIBPTHREADFROMDCE == 1
  reti = pthread_cond_init(&pcond->cond, pthread_condattr_default);
#else
  reti = pthread_cond_init(&pcond->cond, NULL);
#endif
  if (reti) {
    pthread_mutex_destroy(&pcond->mutex);
    netwib_er(netwib_ptr_free((netwib_ptr*)&pcond));
    return(NETWIB_ERR_FUPTHREADCONDINIT);
  }

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_add_thread_cond(pcond));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_thread_cond_close(netwib_thread_cond **ppcond)
{
  netwib_thread_cond *pcond;
  int reti;

  /* parameter verification */
  if (ppcond == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pcond = *ppcond;
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_cond(pcond));
#endif

  /* close cond */
  reti = pthread_cond_destroy(&pcond->cond);
  if (reti) return(NETWIB_ERR_FUPTHREADCONDDESTROY);
  reti = pthread_mutex_destroy(&pcond->mutex);
  if (reti) return(NETWIB_ERR_FUPTHREADMUTEXDESTROY);

  /* free memory */
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_del_thread_cond(pcond));
#endif
  netwib_er(netwib_ptr_free((netwib_ptr*)&pcond));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_thread_cond_broadcast(netwib_thread_cond *pcond,
                                        netwib_uint32 value)
{
  int reti;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_cond(pcond));
#endif

  reti = pthread_mutex_lock(&pcond->mutex);
  if (reti) return(NETWIB_ERR_FUPTHREADMUTEXLOCK);
  pcond->reached = NETWIB_TRUE;
  pcond->value = value;
  reti = pthread_cond_broadcast(&pcond->cond);
  if (reti) {
    pthread_mutex_unlock(&pcond->mutex);
    return(NETWIB_ERR_FUPTHREADCONDBROADCAST);
  }
  reti = pthread_mutex_unlock(&pcond->mutex);
  if (reti) return(NETWIB_ERR_FUPTHREADMUTEXUNLOCK);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_thread_cond_wait(netwib_thread_cond *pcond,
                                   netwib_consttime *pabstime,
                                   netwib_bool *pevent,
                                   netwib_uint32 *pvalue)
{
  netwib_err ret;
  struct timespec ts;
  int reti;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_cond(pcond));
#endif

  reti = pthread_mutex_lock(&pcond->mutex);
  if (reti) return(NETWIB_ERR_FUPTHREADMUTEXLOCK);

  if (!pcond->reached) {
    if (pabstime == NETWIB_TIME_INFINITE) {
      reti = pthread_cond_wait(&pcond->cond, &pcond->mutex);
    } else {
      ret = netwib_priv_time_timeout_thread(pabstime, &ts);
      if (ret != NETWIB_ERR_OK) {
        pthread_mutex_unlock(&pcond->mutex);
        return(ret);
      }
      reti = pthread_cond_timedwait(&pcond->cond, &pcond->mutex, &ts);
#if NETWIBDEF_LIBPTHREADFROMDCE == 1
      if (reti == -1) {
        if (errno == EAGAIN) reti = ETIMEDOUT;
      }
#endif
    }
    if (reti == ETIMEDOUT) {
      pthread_mutex_unlock(&pcond->mutex);
      if (pevent != NULL) *pevent = NETWIB_FALSE;
      return(NETWIB_ERR_OK);
    } else if (reti) {
      pthread_mutex_unlock(&pcond->mutex);
      if (pabstime == NETWIB_TIME_INFINITE) {
        return(NETWIB_ERR_FUPTHREADCONDWAIT);
      }
      return(NETWIB_ERR_FUPTHREADCONDTIMEDWAIT);
    }
  }
  if (pevent != NULL) *pevent = NETWIB_TRUE;
  if (pvalue != NULL) *pvalue = pcond->value;

  reti = pthread_mutex_unlock(&pcond->mutex);
  if (reti) return(NETWIB_ERR_FUPTHREADMUTEXUNLOCK);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_thread_cond_reinit(netwib_thread_cond *pcond)
{
  int reti;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_cond(pcond));
#endif

  reti = pthread_mutex_lock(&pcond->mutex);
  if (reti) return(NETWIB_ERR_FUPTHREADMUTEXLOCK);

  pcond->reached = NETWIB_FALSE;
  netwib_debug_notset_uint32(pcond->value);

  reti = pthread_mutex_unlock(&pcond->mutex);
  if (reti) return(NETWIB_ERR_FUPTHREADMUTEXUNLOCK);

  return(NETWIB_ERR_OK);
}
