/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
struct netwib_thread_cond {
  HANDLE eventh;
  HANDLE mutexh;
  netwib_bool reached;
  netwib_uint32 value;
};

/*-------------------------------------------------------------*/
netwib_err netwib_thread_cond_init(netwib_thread_cond **ppcond)
{
  netwib_thread_cond *pcond;

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
  pcond->mutexh = CreateMutex(NULL, FALSE, NULL);
  if (pcond->mutexh == NULL) {
    netwib_er(netwib_ptr_free((netwib_ptr*)&pcond));
    return(NETWIB_ERR_FUCREATEMUTEX);
  }
  pcond->eventh = CreateEvent(NULL, TRUE, FALSE, NULL);
  if (pcond->eventh == NULL) {
    CloseHandle(pcond->mutexh);
    netwib_er(netwib_ptr_free((netwib_ptr*)&pcond));
    return(NETWIB_ERR_FUCREATEEVENT);
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
  BOOL err;

  /* parameter verification */
  if (ppcond == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pcond = *ppcond;
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_cond(pcond));
#endif

  /* otherwise, we might close while another thread is trying to send
     a cond */
  netwib_er(netwib_priv_handle_waitsimple(pcond->mutexh, NETWIB_TIME_INFINITE,
                                          NULL));

  /* close cond */
  err = CloseHandle(pcond->eventh);
  if (!err) {
    return(NETWIB_ERR_FUCLOSEHANDLE);
  }
  err = CloseHandle(pcond->mutexh);
  if (!err) {
    return(NETWIB_ERR_FUCLOSEHANDLE);
  }

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
  BOOL err;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_cond(pcond));
#endif

  netwib_er(netwib_priv_handle_waitsimple(pcond->mutexh, NETWIB_TIME_INFINITE,
                                          NULL));
  pcond->reached = NETWIB_TRUE;
  pcond->value = value;
  err = SetEvent(pcond->eventh);
  if (!err) {
    ReleaseMutex(pcond->mutexh);
    return(NETWIB_ERR_FUSETEVENT);
  }
  err = ReleaseMutex(pcond->mutexh);
  if (!err) {
    return(NETWIB_ERR_FURELEASEMUTEX);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_thread_cond_wait(netwib_thread_cond *pcond,
                                   netwib_consttime *pabstime,
                                   netwib_bool *pevent,
                                   netwib_uint32 *pvalue)
{
  netwib_bool event;
  BOOL err;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_cond(pcond));
#endif

  /* first check if we already have an event */
  netwib_er(netwib_priv_handle_waitsimple(pcond->mutexh, NETWIB_TIME_INFINITE,
                                          NULL));
  event = pcond->reached;
  if (event) {
    if (pevent != NULL) *pevent = NETWIB_TRUE;
    if (pvalue != NULL) *pvalue = pcond->value;
  }
  err = ReleaseMutex(pcond->mutexh);
  if (!err) {
    return(NETWIB_ERR_FURELEASEMUTEX);
  }
  if (event) {
    return(NETWIB_ERR_OK);
  }

  /* important note : with POSIX, pthread_cond_wait atomically
     unlock the mutex. This is compulsory because an event is
     very fast, so we might miss it here.
     But, with Windows, we created an event which has to
     be manually reset. So, we do not have a timing race.
  */

  /* now wait on the event */
  netwib_er(netwib_priv_handle_waitsimple(pcond->eventh, pabstime, &event));
  if (event) {
    if (pevent != NULL) *pevent = NETWIB_TRUE;
    if (pvalue != NULL) *pvalue = pcond->value;
  } else {
    if (pevent != NULL) *pevent = NETWIB_FALSE;
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_thread_cond_reinit(netwib_thread_cond *pcond)
{
  BOOL err;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_cond(pcond));
#endif

  netwib_er(netwib_priv_handle_waitsimple(pcond->mutexh, NETWIB_TIME_INFINITE,
                                          NULL));

  pcond->reached = NETWIB_FALSE;
  netwib_debug_notset_uint32(pcond->value);
  err = ResetEvent(pcond->eventh);
  if (!err) {
    ReleaseMutex(pcond->mutexh);
    return(NETWIB_ERR_FURESETEVENT);
  }

  err = ReleaseMutex(pcond->mutexh);
  if (!err) {
    return(NETWIB_ERR_FURELEASEMUTEX);
  }

  return(NETWIB_ERR_OK);
}

