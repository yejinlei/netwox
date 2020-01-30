
/*-------------------------------------------------------------*/
/* number of 100nanoseconds ("tenths of a microsecond") since
   January 1, 1601 */
typedef netwib_uint64 netwox_time1601;
netwib_err netwox_time1601_init_time(netwib_consttime *ptime,
                                     netwox_time1601 *ptime1601);
netwib_err netwox_time_init_time1601(netwox_time1601 time1601,
                                     netwib_time *ptime);
netwib_err netwox_time1601_test(void);

/*-------------------------------------------------------------*/
/* time since 1970, but local (instead of GMT) */
typedef netwib_uint32 netwox_time1970l;
netwib_err netwox_time1970l_init_time(netwib_consttime *ptime,
                                      netwox_time1970l *ptime1970l);
netwib_err netwox_time_init_time1970l(netwox_time1970l time1970l,
                                      netwib_time *ptime);
netwib_err netwox_time1970l_test(void);

/*-------------------------------------------------------------*/
/* MSDOS time */
typedef struct {
  netwib_uint16 msdosdate;
  netwib_uint16 msdostime;
} netwox_timemsdos;
netwib_err netwox_timemsdos_init_time(netwib_consttime *ptime,
                                      netwox_timemsdos *ptimemsdos);
netwib_err netwox_time_init_timemsdos(netwox_timemsdos timemsdos,
                                      netwib_time *ptime);
netwib_err netwox_timemsdos_test(void);
/* these functions set 0 on error */
netwib_err netwox_time1601_init_timemsdos(netwox_timemsdos timemsdos,
                                          netwox_time1601 *ptime1601);
netwib_err netwox_timemsdos_init_time1601(netwox_time1601 time1601,
                                          netwox_timemsdos *ptimemsdos);
