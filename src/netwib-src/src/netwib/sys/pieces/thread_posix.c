/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
struct netwib_thread {
  pthread_t thu;
  pthread_cond_t cond;
  pthread_mutex_t condmut;
  netwib_bool condreached;
  netwib_bool threadjoined;
  netwib_thread_pf pfunc;
  netwib_ptr infosin;
  netwib_ptr infosout;
  netwib_err returnederror;
};

/*-------------------------------------------------------------*/
void * netwib_priv_thread_init(void *arg);
void * netwib_priv_thread_init(void *arg)
{
  netwib_thread *pth = (netwib_thread *)arg;

  /* of course, this works only if netwib_thread_close is not called
     before end of this function */
  pth->returnederror = (*pth->pfunc)(pth->infosin, &pth->infosout);

  pthread_mutex_lock(&pth->condmut);
  pth->condreached = NETWIB_TRUE;
  pthread_cond_broadcast(&pth->cond);
  pthread_mutex_unlock(&pth->condmut);

  return(NULL);
}

/*-------------------------------------------------------------*/
netwib_err netwib_thread_init(netwib_thread_pf pfunc,
                              netwib_ptr infosin,
                              netwib_thread **ppthread)
{
  netwib_thread *pthread;
  int reti;

  /* parameter verification */
  if (ppthread == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* allocate needed memory to store pthread */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_thread), (netwib_ptr*)&pthread));
  *ppthread = pthread;

#if NETWIBDEF_LIBPTHREADFROMDCE == 1
  /* initialize cond which will be used for joining */
  pthread->condreached = NETWIB_FALSE;
  pthread->threadjoined = NETWIB_FALSE;
  reti = pthread_mutex_init(&pthread->condmut, pthread_mutexattr_default);
  if (reti) {
    netwib_er(netwib_ptr_free((netwib_ptr*)&pthread));
    return(NETWIB_ERR_FUPTHREADMUTEXINIT);
  }
  reti = pthread_cond_init(&pthread->cond, pthread_condattr_default);
  if (reti) {
    pthread_mutex_destroy(&pthread->condmut);
    netwib_er(netwib_ptr_free((netwib_ptr*)&pthread));
    return(NETWIB_ERR_FUPTHREADCONDINIT);
  }
  /* try to launch thread */
  pthread->pfunc = pfunc;
  pthread->infosin = infosin;
  reti = pthread_create(&pthread->thu, pthread_attr_default,
                        &netwib_priv_thread_init, pthread);
  if (reti) {
    pthread_mutex_destroy(&pthread->condmut);
    pthread_cond_destroy(&pthread->cond);
    netwib_er(netwib_ptr_free((netwib_ptr*)&pthread));
    return(NETWIB_ERR_FUPTHREADCREATE);
  }
#else
 {
  pthread_attr_t attr;
  /* prepare attributes */
  reti = pthread_attr_init(&attr);
  if (reti) {
    netwib_er(netwib_ptr_free((netwib_ptr*)&pthread));
    return(NETWIB_ERR_FUPTHREADATTRINIT);
  }
  reti = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  if (reti) {
    pthread_attr_destroy(&attr);
    netwib_er(netwib_ptr_free((netwib_ptr*)&pthread));
    return(NETWIB_ERR_FUPTHREADATTRSETDETACHSTATE);
  }
  /* initialize cond which will be used for joining */
  pthread->condreached = NETWIB_FALSE;
  pthread->threadjoined = NETWIB_FALSE;
  reti = pthread_mutex_init(&pthread->condmut, NULL);
  if (reti) {
    pthread_attr_destroy(&attr);
    netwib_er(netwib_ptr_free((netwib_ptr*)&pthread));
    return(NETWIB_ERR_FUPTHREADMUTEXINIT);
  }
  reti = pthread_cond_init(&pthread->cond, NULL);
  if (reti) {
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&pthread->condmut);
    netwib_er(netwib_ptr_free((netwib_ptr*)&pthread));
    return(NETWIB_ERR_FUPTHREADCONDINIT);
  }
  /* try to launch thread */
  pthread->pfunc = pfunc;
  pthread->infosin = infosin;
  reti = pthread_create(&pthread->thu, &attr, &netwib_priv_thread_init,
                        pthread);
  if (reti) {
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&pthread->condmut);
    pthread_cond_destroy(&pthread->cond);
    netwib_er(netwib_ptr_free((netwib_ptr*)&pthread));
    return(NETWIB_ERR_FUPTHREADCREATE);
  }
  reti = pthread_attr_destroy(&attr);
  if (reti) {
    pthread_mutex_destroy(&pthread->condmut);
    pthread_cond_destroy(&pthread->cond);
    netwib_er(netwib_ptr_free((netwib_ptr*)&pthread));
    return(NETWIB_ERR_FUPTHREADATTRDESTROY);
  }
 }
#endif

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_add_thread(pthread));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_thread_close(netwib_thread **ppthread)
{
  netwib_thread *pthread;
  int reti;

  /* parameter verification */
  if (ppthread == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pthread = *ppthread;
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread(pthread));
#endif

  /* destroy cond */
  reti = pthread_cond_destroy(&pthread->cond);
  if (reti) return(NETWIB_ERR_FUPTHREADCONDDESTROY);
  reti = pthread_mutex_destroy(&pthread->condmut);
  if (reti) return(NETWIB_ERR_FUPTHREADMUTEXDESTROY);

  /* free memory */
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_del_thread(pthread));
#endif
  netwib_er(netwib_ptr_free((netwib_ptr*)&pthread));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_thread_wait(netwib_thread *pthread,
                              netwib_consttime *pabstime,
                              netwib_bool *pevent,
                              netwib_err *preturnederror,
                              netwib_ptr *pinfosout)
{
  netwib_err ret;
  struct timespec ts;
  int reti;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread(pthread));
#endif

  /* wait for thread end */
  reti = pthread_mutex_lock(&pthread->condmut);
  if (reti) return(NETWIB_ERR_FUPTHREADMUTEXLOCK);
  if (!pthread->condreached) {
    /* wait for the condition */
    if (pabstime == NETWIB_TIME_INFINITE) {
      reti = pthread_cond_wait(&pthread->cond, &pthread->condmut);
    } else if (pabstime == NETWIB_TIME_ZERO) {
      /* we could try pthread_cond_timedwait with a zero time to give
         one more chance, but NETWIBDEF_LIBPTHREADFROMMIT does not support it.
         So, we decide thread end has not yet occurred.
      */
      reti = ETIMEDOUT;
    } else {
      ret = netwib_priv_time_timeout_thread(pabstime, &ts);
      if (ret != NETWIB_ERR_OK) {
        pthread_mutex_unlock(&pthread->condmut);
        return(ret);
      }
      reti = pthread_cond_timedwait(&pthread->cond, &pthread->condmut, &ts);
    }
#if NETWIBDEF_LIBPTHREADFROMDCE == 1
    if (reti == -1) {
      if (errno == EAGAIN) reti = ETIMEDOUT;
    }
#endif
    if (reti == 0) {
      /* condition occurred, so return an event below */
    } else if (reti == ETIMEDOUT) {
      pthread_mutex_unlock(&pthread->condmut);
      if (pevent != NULL) *pevent = NETWIB_FALSE;
      return(NETWIB_ERR_OK);
    } else {
      pthread_mutex_unlock(&pthread->condmut);
      return(NETWIB_ERR_FUPTHREADCONDTIMEDWAIT);
    }
  }
  reti = pthread_mutex_unlock(&pthread->condmut);
  if (reti) return(NETWIB_ERR_FUPTHREADMUTEXUNLOCK);

  /* free thread resources only once */
  reti = pthread_mutex_lock(&pthread->condmut);
  if (reti) return(NETWIB_ERR_FUPTHREADMUTEXLOCK);
  if (!pthread->threadjoined) {
    reti = pthread_join(pthread->thu, NULL);
    if (reti) {
      pthread_mutex_unlock(&pthread->condmut);
      return(NETWIB_ERR_FUPTHREADJOIN);
    }
    pthread->threadjoined = NETWIB_TRUE;
  }
  reti = pthread_mutex_unlock(&pthread->condmut);
  if (reti) return(NETWIB_ERR_FUPTHREADMUTEXUNLOCK);

  if (pevent != NULL) *pevent = NETWIB_TRUE;
  if (preturnederror != NULL) *preturnederror = pthread->returnederror;
  if (pinfosout != NULL) *pinfosout = pthread->infosout;

  return(NETWIB_ERR_OK);
}

