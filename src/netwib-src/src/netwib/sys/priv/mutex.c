/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_mutex_init(netwib_priv_mutex *pmut)
{

#if defined NETWIBDEF_SYSNAME_Unix
  {
    int reti;
#if NETWIBDEF_LIBPTHREADFROMDCE == 1
    reti = pthread_mutex_init(&pmut->rdmut, pthread_mutexattr_default);
#else
    reti = pthread_mutex_init(&pmut->rdmut, NULL);
#endif
    if (reti) {
      return(NETWIB_ERR_FUPTHREADMUTEXINIT);
    }
#if NETWIBDEF_LIBPTHREADFROMDCE == 1
    reti = pthread_mutex_init(&pmut->wrmut, pthread_mutexattr_default);
#else
    reti = pthread_mutex_init(&pmut->wrmut, NULL);
#endif
    if (reti) {
      reti = pthread_mutex_destroy(&pmut->rdmut);
      return(NETWIB_ERR_FUPTHREADMUTEXINIT);
    }
  }
#elif defined NETWIBDEF_SYSNAME_Windows
   InitializeCriticalSection(&pmut->rdcs);
   InitializeCriticalSection(&pmut->wrcs);
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  pmut->numreaders = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_mutex_close(netwib_priv_mutex *pmut)
{

#if defined NETWIBDEF_SYSNAME_Unix
  {
    int reti;
    reti = pthread_mutex_destroy(&pmut->rdmut);
    if (reti) return(NETWIB_ERR_FUPTHREADMUTEXDESTROY);
    reti = pthread_mutex_destroy(&pmut->wrmut);
    if (reti) return(NETWIB_ERR_FUPTHREADMUTEXDESTROY);
  }
#elif defined NETWIBDEF_SYSNAME_Windows
   DeleteCriticalSection(&pmut->rdcs);
   DeleteCriticalSection(&pmut->wrcs);
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_mutex_rdlock(netwib_priv_mutex *pmut)
{

#define NETWIB_PRIV_MUTEX_DEBUG 0
#if NETWIB_PRIV_MUTEX_DEBUG == 1
  netwib_er(netwib_debug_disp_string("RDLOCK at\n"));
  netwib_er(netwib_debug_backtrace_disp());
#endif

#if defined NETWIBDEF_SYSNAME_Unix
  {
    int reti;
    reti = pthread_mutex_lock(&pmut->wrmut);
    if (reti) return(NETWIB_ERR_FUPTHREADMUTEXLOCK);
    reti = pthread_mutex_lock(&pmut->rdmut);
    if (reti) return(NETWIB_ERR_FUPTHREADMUTEXLOCK);
    pmut->numreaders++;
    reti = pthread_mutex_unlock(&pmut->rdmut);
    if (reti) return(NETWIB_ERR_FUPTHREADMUTEXUNLOCK);
    reti = pthread_mutex_unlock(&pmut->wrmut);
    if (reti) return(NETWIB_ERR_FUPTHREADMUTEXUNLOCK);
  }
#elif defined NETWIBDEF_SYSNAME_Windows
  EnterCriticalSection(&pmut->wrcs);
  EnterCriticalSection(&pmut->rdcs);
  pmut->numreaders++;
  LeaveCriticalSection(&pmut->rdcs);
  LeaveCriticalSection(&pmut->wrcs);
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_mutex_rdunlock(netwib_priv_mutex *pmut)
{

#if NETWIB_PRIV_MUTEX_DEBUG == 1
  netwib_er(netwib_debug_disp_string("RDUNLOCK at\n"));
  netwib_er(netwib_debug_backtrace_disp());
#endif

#if defined NETWIBDEF_SYSNAME_Unix
  {
    int reti;
    reti = pthread_mutex_lock(&pmut->rdmut);
    if (reti) return(NETWIB_ERR_FUPTHREADMUTEXLOCK);
    pmut->numreaders--;
    reti = pthread_mutex_unlock(&pmut->rdmut);
    if (reti) return(NETWIB_ERR_FUPTHREADMUTEXUNLOCK);
  }
#elif defined NETWIBDEF_SYSNAME_Windows
  EnterCriticalSection(&pmut->rdcs);
  pmut->numreaders--;
  LeaveCriticalSection(&pmut->rdcs);
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_mutex_wrlock2(netwib_priv_mutex *pmut,
                                            netwib_bool *plocked)
{
#if defined NETWIBDEF_SYSNAME_Unix
  {
    int reti;
    reti = pthread_mutex_lock(&pmut->wrmut);
    if (reti) return(NETWIB_ERR_FUPTHREADMUTEXLOCK);
    reti = pthread_mutex_lock(&pmut->rdmut);
    if (reti) return(NETWIB_ERR_FUPTHREADMUTEXLOCK);
    if (pmut->numreaders != 0) {
      reti = pthread_mutex_unlock(&pmut->wrmut);
      if (reti) return(NETWIB_ERR_FUPTHREADMUTEXUNLOCK);
      *plocked = NETWIB_FALSE;
    } else {
      *plocked = NETWIB_TRUE;
    }
    reti = pthread_mutex_unlock(&pmut->rdmut);
    if (reti) return(NETWIB_ERR_FUPTHREADMUTEXUNLOCK);
  }
#elif defined NETWIBDEF_SYSNAME_Windows
  EnterCriticalSection(&pmut->wrcs);
  EnterCriticalSection(&pmut->rdcs);
  if (pmut->numreaders != 0) {
    LeaveCriticalSection(&pmut->wrcs);
    *plocked = NETWIB_FALSE;
  } else {
    *plocked = NETWIB_TRUE;
  }
  LeaveCriticalSection(&pmut->rdcs);
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_mutex_wrlock(netwib_priv_mutex *pmut)
{
  netwib_uint32 numcalls;
  netwib_bool locked;

#if NETWIB_PRIV_MUTEX_DEBUG == 1
  netwib_er(netwib_debug_disp_string("WRLOCK at\n"));
  netwib_er(netwib_debug_backtrace_disp());
#endif

  numcalls = 0;
  while (NETWIB_TRUE) {
    netwib_er(netwib_priv_mutex_wrlock2(pmut, &locked));
    if (locked) {
      break;
    }
    /* small pause */
    netwib_er(netwib_priv_pause2(&numcalls));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_mutex_wrunlock(netwib_priv_mutex *pmut)
{

#if NETWIB_PRIV_MUTEX_DEBUG == 1
  netwib_er(netwib_debug_disp_string("WRUNLOCK at\n"));
  netwib_er(netwib_debug_backtrace_disp());
#endif

#if defined NETWIBDEF_SYSNAME_Unix
  {
    int reti;
    reti = pthread_mutex_unlock(&pmut->wrmut);
    if (reti) return(NETWIB_ERR_FUPTHREADMUTEXUNLOCK);
  }
#elif defined NETWIBDEF_SYSNAME_Windows
   LeaveCriticalSection(&pmut->wrcs);
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  return(NETWIB_ERR_OK);
}
