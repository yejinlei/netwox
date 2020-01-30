/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
struct netwib_thread_tsd {
  pthread_key_t key;
};

/*-------------------------------------------------------------*/
netwib_err netwib_thread_tsd_init(netwib_thread_tsd **pptsd)
{
  netwib_thread_tsd *ptsd;
  int reti;

  /* parameter verification */
  if (pptsd == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* allocate needed memory to store tsd */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_thread_tsd),
                              (netwib_ptr*)&ptsd));
  *pptsd = ptsd;

  /* initialize tsd */
#if NETWIBDEF_LIBPTHREADFROMDCE == 1
  reti = pthread_keycreate(&ptsd->key, NULL);
#else
  reti = pthread_key_create(&ptsd->key, NULL);
#endif
  if (reti) {
    netwib_er(netwib_ptr_free((netwib_ptr*)&ptsd));
    return(NETWIB_ERR_FUPTHREADKEYCREATE);
  }

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_add_thread_tsd(ptsd));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_thread_tsd_close(netwib_thread_tsd **pptsd)
{
  netwib_thread_tsd *ptsd;
  int reti;

  /* parameter verification */
  if (pptsd == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  ptsd = *pptsd;
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_tsd(ptsd));
#endif

  /* close tsd */
#if NETWIBDEF_LIBPTHREADFROMDCE == 1
  reti = 0; /* no function */
#elif NETWIBDEF_LIBPTHREADFROMMIT == 1
  /* function always return an error */
  /* ignore */ pthread_key_delete(ptsd->key);
  reti = 0;
#else
  reti = pthread_key_delete(ptsd->key);
#endif
  if (reti) return(NETWIB_ERR_FUPTHREADKEYDELETE);

  /* free memory */
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_del_thread_tsd(ptsd));
#endif
  netwib_er(netwib_ptr_free((netwib_ptr*)&ptsd));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_thread_tsd_ctl_set(netwib_thread_tsd *ptsd,
                                     netwib_thread_tsd_ctltype type,
                                     netwib_ptr p,
                                     netwib_uint32 ui)
{
  int reti;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_tsd(ptsd));
#endif

  switch(type) {
  case NETWIB_THREAD_TSD_CTLTYPE_VALUE:
    reti = pthread_setspecific(ptsd->key, p);
    if (reti) return(NETWIB_ERR_FUPTHREADSETSPECIFIC);
    break;
  default:
    return(NETWIB_ERR_PAINVALIDTYPE);
    ui = ui;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_thread_tsd_ctl_get(netwib_thread_tsd *ptsd,
                                     netwib_thread_tsd_ctltype type,
                                     netwib_ptr p,
                                     netwib_uint32 *pui)
{

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_tsd(ptsd));
#endif

  switch(type) {
  case NETWIB_THREAD_TSD_CTLTYPE_VALUE:
    if (p != NULL) {
#if NETWIBDEF_LIBPTHREADFROMDCE == 1
      int reti;
      reti = pthread_getspecific(ptsd->key, p);
      if (reti) {
        return(NETWIB_ERR_FUPTHREADGETSPECIFIC);
      }
#else
      *(netwib_ptr*)p = pthread_getspecific(ptsd->key);
#endif
    }
    break;
  default:
    return(NETWIB_ERR_PAINVALIDTYPE);
    pui = pui;
  }

  return(NETWIB_ERR_OK);
}


