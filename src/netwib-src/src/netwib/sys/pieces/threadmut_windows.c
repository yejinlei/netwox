/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_PRIV_THREAD_MUTEXTYPE_CS = 1,
  NETWIB_PRIV_THREAD_MUTEXTYPE_HANDLE,
} netwib_priv_thread_mutextype;

/*-------------------------------------------------------------*/
struct netwib_thread_mutex {
  netwib_priv_thread_mutextype type;
  CRITICAL_SECTION cs;
  HANDLE mutexh;
};

/*-------------------------------------------------------------*/
netwib_err netwib_thread_mutex_init(netwib_thread_mutex **ppmutex)
{
  netwib_thread_mutex *pmutex;

  /* parameter verification */
  if (ppmutex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* allocate needed memory to store mutex */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_thread_mutex),
                              (netwib_ptr*)&pmutex));
  *ppmutex = pmutex;

  /* decide which kind of mutex to use */
  if (netwib_global_dll_kernel32.tryentercriticalsection_avail) {
    pmutex->type = NETWIB_PRIV_THREAD_MUTEXTYPE_CS;
  } else {
    pmutex->type = NETWIB_PRIV_THREAD_MUTEXTYPE_HANDLE;
  }

  /* initialize mutex */
  if (pmutex->type == NETWIB_PRIV_THREAD_MUTEXTYPE_CS) {
    InitializeCriticalSection(&pmutex->cs);
  } else {
    pmutex->mutexh = CreateMutex(NULL, FALSE, NULL);
    if (pmutex->mutexh == NULL) {
      netwib_er(netwib_ptr_free((netwib_ptr*)&pmutex));
      return(NETWIB_ERR_FUCREATEMUTEX);
    }
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
  BOOL err;;

  /* parameter verification */
  if (ppmutex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pmutex = *ppmutex;
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_mutex(pmutex));
#endif

  /* close mutex */
    if (pmutex->type == NETWIB_PRIV_THREAD_MUTEXTYPE_CS) {
    DeleteCriticalSection(&pmutex->cs);
  } else {
    err = CloseHandle(pmutex->mutexh);
    if (!err) {
      return(NETWIB_ERR_FUCLOSEHANDLE);
    }
  }

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
                                                     netwib_bool *plocked)
{
  netwib_uint32 numcalls;
  netwib_bool elapsed;
  BOOL err;

  numcalls = 0;
  while (NETWIB_TRUE) {
    /* check time */
    netwib_er(netwib_time_iselapsed(pabstime, &elapsed));
    if (elapsed) {
      if (plocked != NULL) *plocked = NETWIB_FALSE;
      break;
    }
    /* wait */
    err = netwib_global_dll_kernel32.tryentercriticalsection(&pmutex->cs);
    if (!err) {
      /* continue this loop */
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

/*-------------------------------------------------------------*/
netwib_err netwib_thread_mutex_lock(netwib_thread_mutex *pmutex,
                                    netwib_consttime *pabstime,
                                    netwib_bool *plocked)
{
  BOOL err;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_mutex(pmutex));
#endif

  if (pmutex->type == NETWIB_PRIV_THREAD_MUTEXTYPE_HANDLE) {
    /* use handle */
    netwib_er(netwib_priv_handle_waitsimple(pmutex->mutexh, pabstime,
                                            plocked));
    return(NETWIB_ERR_OK);
  }

  if (pabstime == NETWIB_TIME_ZERO) {
    err = netwib_global_dll_kernel32.tryentercriticalsection(&pmutex->cs);
    if (!err) {
      if (plocked != NULL) *plocked = NETWIB_FALSE;
      return(NETWIB_ERR_OK);
    }
  } else if (pabstime == NETWIB_TIME_INFINITE) {
    EnterCriticalSection(&pmutex->cs);
      if (plocked != NULL) *plocked = NETWIB_TRUE;
  } else {
    netwib_er(netwib_priv_thread_mutex_timedlock(pmutex, pabstime, plocked));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_thread_mutex_unlock(netwib_thread_mutex *pmutex)
{
  BOOL err;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_mutex(pmutex));
#endif

  if (pmutex->type == NETWIB_PRIV_THREAD_MUTEXTYPE_CS) {
    LeaveCriticalSection(&pmutex->cs);
  } else {
    err = ReleaseMutex(pmutex->mutexh);
    if (!err) {
      return(NETWIB_ERR_FURELEASEMUTEX);
    }
  }

  return(NETWIB_ERR_OK);
}

