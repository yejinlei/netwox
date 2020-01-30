/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_pause_yield(void)
{

#if defined NETWIBDEF_SYSNAME_Unix
 #if NETWIBDEF_HAVEFUNC_PTHREAD_YIELD == 1
  pthread_yield();
  return(NETWIB_ERR_OK);
 #elif NETWIBDEF_HAVEFUNC_SCHED_YIELD == 1
  sched_yield();
  return(NETWIB_ERR_OK);
 #endif
#elif defined NETWIBDEF_SYSNAME_Windows
  if (netwib_global_dll_kernel32.switchtothread_avail) {
    netwib_global_dll_kernel32.switchtothread();
    return(NETWIB_ERR_OK);
  }
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  return(NETWIB_ERR_LONOTIMPLEMENTED);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_pause(void)
{
  netwib_err ret;

  ret = netwib_priv_pause_yield();
  if (ret == NETWIB_ERR_LONOTIMPLEMENTED) {
    ret = netwib_priv_time_sleep(0, 0);
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_pause2(netwib_uint32 *pnumcalls)
{
  netwib_err ret;

  if (pnumcalls == NULL) {
    return(netwib_priv_time_sleep(0, 0));
  }

  if (*pnumcalls < 16) {
    (*pnumcalls)++;
    ret = netwib_priv_pause_yield();
    if (ret == NETWIB_ERR_LONOTIMPLEMENTED) {
      ret = netwib_priv_time_sleep(0, 0);
    }
  } else {
    ret = netwib_priv_time_sleep(0, 0);
  }

  return(ret);
}

