
/*-------------------------------------------------------------*/
netwib_err netwib_priv_time_timeout_select(netwib_consttime *pabstime,
                                           struct timeval *pto,
                                           struct timeval **ppto);
#if defined NETWIBDEF_SYSNAME_Unix
netwib_err netwib_priv_time_timeout_poll(netwib_consttime *pabstime,
                                         int *ptimeout);
netwib_err netwib_priv_time_timeout_thread(netwib_consttime *pabstime,
                                           struct timespec *pts);
#elif defined NETWIBDEF_SYSNAME_Windows
netwib_err netwib_priv_time_timeout_winwait(netwib_consttime *pabstime,
                                            DWORD *pwintime);
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_time_init_now(netwib_uint32 *psec,
                                     netwib_uint32 *pnsec);

netwib_err netwib_priv_time_sleep(netwib_uint32 sec,
                                  netwib_uint32 nsec);
