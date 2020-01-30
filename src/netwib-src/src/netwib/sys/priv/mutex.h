
/*-------------------------------------------------------------*/
typedef struct {
#if defined NETWIBDEF_SYSNAME_Unix
  pthread_mutex_t rdmut;
  pthread_mutex_t wrmut;
#elif defined NETWIBDEF_SYSNAME_Windows
  CRITICAL_SECTION rdcs;
  CRITICAL_SECTION wrcs;
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif
  netwib_uint32 numreaders;
} netwib_priv_mutex;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_mutex_init(netwib_priv_mutex *pmut);
netwib_err netwib_priv_mutex_close(netwib_priv_mutex *pmut);
netwib_err netwib_priv_mutex_rdlock(netwib_priv_mutex *pmut);
netwib_err netwib_priv_mutex_rdunlock(netwib_priv_mutex *pmut);
netwib_err netwib_priv_mutex_wrlock(netwib_priv_mutex *pmut);
netwib_err netwib_priv_mutex_wrunlock(netwib_priv_mutex *pmut);
