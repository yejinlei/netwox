/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_time_timeout_select(netwib_consttime *pabstime,
                                           struct timeval *pto,
                                           struct timeval **ppto)
{
  netwib_uint32 sec, msec, usec;
  netwib_time now, diff;
  netwib_err ret;

  /* set timeout */
  if (pabstime == NETWIB_TIME_ZERO) {
    pto->tv_sec = 0;
    pto->tv_usec = 0;
    *ppto = pto;
  } else if (pabstime == NETWIB_TIME_INFINITE) {
    *ppto = NULL;
  } else {
    netwib_er(netwib_priv_time_init_now(&now.sec, &now.nsec));
    diff = *pabstime;
    ret = netwib_time_minus_time(&diff, &now);
    if (ret == NETWIB_ERR_PATIMEDIFFNEG) {
      pto->tv_sec = 0;
      pto->tv_usec = 0;
      *ppto = pto;
    } else if (ret != NETWIB_ERR_OK) {
      return(ret);
    } else {
      netwib_er(netwib_time_decode_fields(&diff, &sec, &msec, &usec, NULL));
      pto->tv_sec = sec;
      pto->tv_usec = NETWIB_PRIV_NUM_1K*msec + usec;
      *ppto = pto;
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
netwib_err netwib_priv_time_timeout_poll(netwib_consttime *pabstime,
                                         int *ptimeout)
{
  netwib_time now, diff;
  netwib_uint32 msec;
  int timeoutint;
  netwib_err ret;

  if (pabstime == NETWIB_TIME_ZERO) {
    timeoutint = 0;
  } else if (pabstime == NETWIB_TIME_INFINITE) {
    timeoutint = -1;
  } else {
    netwib_er(netwib_priv_time_init_now(&now.sec, &now.nsec));
    diff = *pabstime;
    ret = netwib_time_minus_time(&diff, &now);
    if (ret == NETWIB_ERR_PATIMEDIFFNEG) {
      timeoutint = 0;
    } else if (ret != NETWIB_ERR_OK) {
      return(ret);
    } else {
      ret = netwib_time_decode_msec(&diff, &msec);
      if (ret == NETWIB_ERR_OK) {
        if (msec > 0x7FFFFFFF)
          timeoutint = -1;
        else
          timeoutint = msec;
      } else if (ret == NETWIB_ERR_NOTCONVERTED) {
        timeoutint = -1;
      } else {
        return(ret);
      }
    }
  }

  if (ptimeout != NULL) *ptimeout = timeoutint;
  return(NETWIB_ERR_OK);
}
#elif defined NETWIBDEF_SYSNAME_Windows
  /* this function is not needed */
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
netwib_err netwib_priv_time_timeout_thread(netwib_consttime *pabstime,
                                           struct timespec *pts)
{

  if (pabstime == NETWIB_TIME_ZERO) {
    /* we have to set 1 instead of 0 because NetBSD 2.0 does not
       work with 0 */
    pts->tv_sec = 1;
    pts->tv_nsec = 0;
  } else if (pabstime == NETWIB_TIME_INFINITE) {
    pts->tv_sec = 0x7FFFFFFF;
    pts->tv_nsec = 0;
  } else {
    pts->tv_sec = pabstime->sec;
    pts->tv_nsec = pabstime->nsec;
  }

  return(NETWIB_ERR_OK);
}
#elif defined NETWIBDEF_SYSNAME_Windows
  /* this function is not needed */
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
  /* this function is not needed */
#elif defined NETWIBDEF_SYSNAME_Windows
netwib_err netwib_priv_time_timeout_winwait(netwib_consttime *pabstime,
                                            DWORD *pwintime)
{
  DWORD wintime;
  netwib_uint32 msec;
  netwib_time now, diff;
  netwib_err ret;

  netwib_er(netwib_priv_time_init_now(&now.sec, &now.nsec));
  netwib_er(netwib_time_init_time(pabstime, &diff));
  ret = netwib_time_minus_time(&diff, &now);
  if (ret == NETWIB_ERR_PATIMEDIFFNEG) {
    wintime = 0;
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  } else {
    ret = netwib_time_decode_msec(&diff, &msec);
    if (ret == NETWIB_ERR_OK) {
      wintime = msec;
    } else if (ret == NETWIB_ERR_NOTCONVERTED) {
      wintime = INFINITE;
    } else {
      return(ret);
    }
  }

  if (pwintime != NULL) *pwintime = wintime;
  return(NETWIB_ERR_OK);
}
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_time_init_now(netwib_uint32 *psec,
                                     netwib_uint32 *pnsec)
{

#if defined NETWIBDEF_SYSNAME_Unix
 #if NETWIBDEF_HAVEFUNC_CLOCK_GETTIME == 1
  {
    struct timespec tp;
    int reti;
    reti = clock_gettime(CLOCK_REALTIME, &tp);
    if (reti) {
      return(NETWIB_ERR_FUCLOCKGETTIME);
    }
    *psec = (netwib_uint32) tp.tv_sec;
    *pnsec = (netwib_uint32) tp.tv_nsec;
  }
 #elif NETWIBDEF_HAVEFUNC_GETTIMEOFDAY == 1
  {
    struct timeval tv;
    int reti;
    reti = gettimeofday(&tv, NULL);
    if (reti) {
      return(NETWIB_ERR_FUGETTIMEOFDAY);
    }
    *psec = (netwib_uint32) tv.tv_sec;
    *pnsec = (netwib_uint32) tv.tv_usec * NETWIB_PRIV_NUM_1K;
  }
 #else
  #error "I don't know how to retrieve time"
 #endif
#elif defined NETWIBDEF_SYSNAME_Windows
  {
    struct _timeb tstruct;
    _ftime(&tstruct);
    *psec = tstruct.time;
    *pnsec = tstruct.millitm * NETWIB_PRIV_NUM_1M;
  }
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
#if NETWIBDEF_LIBPTHREADFROMDCE == 1
static netwib_err netwib_priv_time_sleep_dce(netwib_uint32 sec,
                                             netwib_uint32 nsec)
{
  pthread_cond_t c;
  pthread_mutex_t m;
  struct timespec ts1, ts2;
  int reti;

  ts1.tv_sec = sec;
  ts1.tv_nsec = nsec;

  reti = pthread_get_expiration_np(&ts1, &ts2);
  if (reti) return(NETWIB_ERR_FUPTHREADGETEXPIRATIONNP);

  reti = pthread_mutex_init(&m, pthread_mutexattr_default);
  if (reti) return(NETWIB_ERR_FUPTHREADMUTEXINIT);
  reti = pthread_cond_init(&c, pthread_condattr_default);
  if (reti) {
    pthread_mutex_destroy(&m);
    return(NETWIB_ERR_FUPTHREADCONDINIT);
  }

  reti = pthread_mutex_lock(&m);
  if (reti) {
    pthread_cond_destroy(&c);
    pthread_mutex_destroy(&m);
    return(NETWIB_ERR_FUPTHREADMUTEXLOCK);
  }
  reti = pthread_cond_timedwait(&c, &m, &ts2);
  if (! (reti == -1 && errno == EAGAIN) ) {
    pthread_cond_destroy(&c);
    pthread_mutex_destroy(&m);
    return(NETWIB_ERR_FUPTHREADCONDTIMEDWAIT);
  }
  reti = pthread_mutex_unlock(&m);
  if (reti) {
    pthread_cond_destroy(&c);
    pthread_mutex_destroy(&m);
    return(NETWIB_ERR_FUPTHREADMUTEXUNLOCK);
  }

  reti = pthread_cond_destroy(&c);
  if (reti) {
    pthread_mutex_destroy(&m);
    return(NETWIB_ERR_FUPTHREADCONDDESTROY);
  }
  reti = pthread_mutex_destroy(&m);
  if (reti) {
    return(NETWIB_ERR_FUPTHREADMUTEXDESTROY);
  }

  return(NETWIB_ERR_OK);
}
#endif
#endif

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Tru64UNIX
#if NETWIBDEF_HAVEFUNC_PTHREAD_DELAY_NP==1
static netwib_err netwib_priv_time_sleep_delaynp(netwib_uint32 sec,
                                                 netwib_uint32 nsec)
{
  struct timespec req;
  int reti;

  req.tv_sec = sec;
  req.tv_nsec = nsec;
  reti = pthread_delay_np(&req);
  if (reti) {
    return(NETWIB_ERR_FUPTHREADDELAYNP);
  }

  return(NETWIB_ERR_OK);
}
#endif
#endif

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
#if NETWIBDEF_HAVEFUNC_NANOSLEEP==1
static netwib_err netwib_priv_time_sleep_nanosleep(netwib_uint32 sec,
                                                   netwib_uint32 nsec)
{
  struct timespec req, rem;
  int reti;

  req.tv_sec = sec;
  req.tv_nsec = nsec;
  while (NETWIB_TRUE) {
    reti = nanosleep(&req, &rem);
    if (reti == 0) {
      break;
    } else {
      if (errno != EINTR) {
        return(NETWIB_ERR_FUNANOSLEEP);
      }
      req.tv_sec = rem.tv_sec;
      req.tv_nsec = rem.tv_nsec;
    }
  }

  return(NETWIB_ERR_OK);
}
#endif
#endif

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
#if NETWIBDEF_HAVEFUNC_USLEEP==1
static netwib_err netwib_priv_time_sleep_sleepusleep(netwib_uint32 sec,
                                                     netwib_uint32 nsec)
{
  if (sec == 0 && nsec == 0) {
    /* minimum sleep */
    sleep(0);
    return(NETWIB_ERR_OK);
  }

  if (sec) {
    sleep(sec);
  }

  if (nsec) {
    /* if nsec < NETWIB_PRIV_NUM_1K, usleep for 0, which might be too long... */
    usleep(nsec / NETWIB_PRIV_NUM_1K);
  }

  return(NETWIB_ERR_OK);
}
#endif
#endif

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Windows
static netwib_err netwib_priv_time_sleep_windows(netwib_uint32 sec,
                                                 netwib_uint32 nsec)
{
  netwib_uint32 i, msec;

  if (sec == 0 && nsec == 0) {
    /* minimum sleep */
    Sleep(0);
    return(NETWIB_ERR_OK);
  }

  if (sec == 0) {
    Sleep(nsec / NETWIB_PRIV_NUM_1M);
    return(NETWIB_ERR_OK);
  }

  if (sec > NETWIB_PRIV_NUM_1M) {
    /* ignore nsec */
    /* equivalent to "1000*sec" but without overflow */
    for (i = 0; i < 1000; i++) {
      Sleep(sec);
    }
    return(NETWIB_ERR_OK);
  }

  msec = NETWIB_PRIV_NUM_1K*sec + nsec/NETWIB_PRIV_NUM_1M;
  Sleep(msec);
  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_time_sleep(netwib_uint32 sec,
                                  netwib_uint32 nsec)
{
#if defined NETWIBDEF_SYSNAME_Unix

 #if NETWIBDEF_LIBPTHREADFROMDCE == 1
  /* Under HP-UX, nanosleep conflicts with pthread_cond_timedwait,
     so we use this function */
  return(netwib_priv_time_sleep_dce(sec, nsec));
  #define NETWIB_PRIV_TIME_SLEEP_DEF 1
 #endif

 #if NETWIBDEF_LIBPTHREADFROMMIT == 1
  /* because nanosleep does not work with this version of pthread */
  #if NETWIBDEF_HAVEFUNC_USLEEP==1
   return(netwib_priv_time_sleep_sleepusleep(sec, nsec));
   #define NETWIB_PRIV_TIME_SLEEP_DEF 1
  #endif
 #endif

 #if defined NETWIBDEF_SYSNAME_Tru64UNIX
  /* because nanosleep does not work under Tru64 UNIX */
  #if NETWIBDEF_HAVEFUNC_PTHREAD_DELAY_NP==1
   return(netwib_priv_time_sleep_delaynp(sec, nsec));
   #define NETWIB_PRIV_TIME_SLEEP_DEF 1
  #endif
 #endif

 #if NETWIBDEF_HAVEFUNC_NANOSLEEP==1
  return(netwib_priv_time_sleep_nanosleep(sec, nsec));
  #define NETWIB_PRIV_TIME_SLEEP_DEF 1
 #endif

 #if NETWIBDEF_HAVEFUNC_USLEEP==1
  return(netwib_priv_time_sleep_sleepusleep(sec, nsec));
  #define NETWIB_PRIV_TIME_SLEEP_DEF 1
 #endif

 #if ! defined NETWIB_PRIV_TIME_SLEEP_DEF
  #error "I don't known how to sleep"
 #endif

#elif defined NETWIBDEF_SYSNAME_Windows
  return(netwib_priv_time_sleep_windows(sec, nsec));
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  return(NETWIB_ERR_OK);
}

