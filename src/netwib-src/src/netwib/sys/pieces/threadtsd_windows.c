/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
struct netwib_thread_tsd {
  DWORD tlsindex;
};

/*-------------------------------------------------------------*/
netwib_err netwib_thread_tsd_init(netwib_thread_tsd **pptsd)
{
  netwib_thread_tsd *ptsd;

  /* parameter verification */
  if (pptsd == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* allocate needed memory to store tsd */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_thread_tsd),
                              (netwib_ptr*)&ptsd));
  *pptsd = ptsd;

  /* initialize tsd */
  ptsd->tlsindex = TlsAlloc();
  if (ptsd->tlsindex == TLS_OUT_OF_INDEXES) {
    netwib_er(netwib_ptr_free((netwib_ptr*)&ptsd));
    return(NETWIB_ERR_FUTLSALLOC);
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
  BOOL err;

  /* parameter verification */
  if (pptsd == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  ptsd = *pptsd;
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_tsd(ptsd));
#endif

  /* close tsd */
  err = TlsFree(ptsd->tlsindex);
  if (!err) {
    return(NETWIB_ERR_FUTLSFREE);
  }

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
  BOOL err;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_thread_tsd(ptsd));
#endif

  switch(type) {
  case NETWIB_THREAD_TSD_CTLTYPE_VALUE:
    err = TlsSetValue(ptsd->tlsindex, p);
    if (!err) {
      return(NETWIB_ERR_FUTLSSETVALUE);
    }
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
      *(netwib_ptr*)p = TlsGetValue(ptsd->tlsindex);
    }
    break;
  default:
    return(NETWIB_ERR_PAINVALIDTYPE);
    pui = pui;
  }

  return(NETWIB_ERR_OK);
}


