/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
struct netwib_thread {
  HANDLE hthread;
  netwib_thread_pf pfunc;
  netwib_ptr infosin;
  netwib_ptr infosout;
  netwib_err returnederror;
};

/*-------------------------------------------------------------*/
static DWORD WINAPI netwib_priv_thread_init(LPVOID arg)
{
  netwib_thread *pth = arg;

  /* of course, this works only if netwib_thread_close is not called
     before end of this function */
  pth->returnederror = (*pth->pfunc)(pth->infosin, &pth->infosout);

  return(0);
}

/*-------------------------------------------------------------*/
netwib_err netwib_thread_init(netwib_thread_pf pfunc,
                              netwib_ptr infosin,
                              netwib_thread **ppthread)
{
  netwib_thread *pthread;
  DWORD threadid;

  /* parameter verification */
  if (ppthread == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* allocate needed memory to store pthread */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_thread), (netwib_ptr*)&pthread));
  *ppthread = pthread;

  /* try to launch thread */
  pthread->pfunc = pfunc;
  pthread->infosin = infosin;
  pthread->hthread = CreateThread(NULL, 0, netwib_priv_thread_init,
                                  pthread, 0, &threadid);
  if (pthread->hthread == NULL) {
    netwib_er(netwib_ptr_free((netwib_ptr*)&pthread));
    return(NETWIB_ERR_FUCREATETHREAD);
  }

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_add_thread(pthread));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_thread_close(netwib_thread **ppthread)
{
  netwib_thread *pthread;
  BOOL err;

  /* parameter verification */
  if (ppthread == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pthread = *ppthread;
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread(pthread));
#endif

  /* close handle */
  err = CloseHandle(pthread->hthread);
  if (!err) return(NETWIB_ERR_FUCLOSEHANDLE);

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
  netwib_bool event;
  DWORD threadreturn;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread(pthread));
#endif

  /* wait */
  netwib_er(netwib_priv_handle_waitsimple(pthread->hthread,
                                          pabstime, &event));
  if (!event) {
    if (pevent != NULL) *pevent = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  }

  /* check if thread really exited */
  if (!GetExitCodeThread(pthread->hthread, &threadreturn)) {
    return(NETWIB_ERR_FUGETEXITCODETHREAD);
  }
  if (threadreturn == STILL_ACTIVE) {
    if (pevent != NULL) *pevent = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  }

  /* set return values */
  if (pevent != NULL) *pevent = NETWIB_TRUE;
  if (preturnederror != NULL) *preturnederror = pthread->returnederror;
  if (pinfosout != NULL) *pinfosout = pthread->infosout;

  return(NETWIB_ERR_OK);
}
