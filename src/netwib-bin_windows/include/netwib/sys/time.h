
/*-------------------------------------------------------------*/
/***************************************************************
 * A netwib_time contains a time duration (relative time) or a *
 * date (absolute time).                                       *
 * Fields of a netwib_time can be directly used, but when it is*
 * set nsec must be between 0 and 999999999.                   *
 ***************************************************************/

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 sec;  /* seconds */
  netwib_uint32 nsec; /* nanoseconds */
} netwib_time;
typedef const netwib_time netwib_consttime;

/*-------------------------------------------------------------*/
/***************************************************************
 * Every function should be prepared to receive as input :     *
 *  - netwib_time*                                             *
 *  - NETWIB_TIME_ZERO                                         *
 *  - NETWIB_TIME_INFINITE                                     *
 * However, NETWIB_TIME_ZERO and NETWIB_TIME_INFINITE cannot be*
 * returned as output by a function. Those defines exist to    *
 * quickly specify those specific inputs.                      *
 ***************************************************************/
/* Empty time duration or year 1970 */
#define NETWIB_TIME_ZERO ((netwib_time*)1)
/* Infinite time duration or year 2038/2108 */
#define NETWIB_TIME_INFINITE ((netwib_time*)2)

/*-------------------------------------------------------------*/
/* Name : netwib_time_init_time
   Description :
     Initialize a netwib_time from another netwib_time.
     This function is mainly useful to convert
     NETWIB_TIME_ZERO or NETWIB_TIME_INFINITE to a real structure.
   Input parameter(s) :
     ptimein : time to copy
   Input/output parameter(s) :
   Output parameter(s) :
     *ptimeout : netwib_time set
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_time_init_time(netwib_consttime *ptimein,
                                 netwib_time *ptimeout);

/*-------------------------------------------------------------*/
/* Name : netwib_time_init_fields
   Description :
     Initialize a netwib_time.
   Input parameter(s) :
     sec : number of seconds
     msec : number of milliseconds
     usec : number of microseconds
     nsec : number of nanoseconds
   Input/output parameter(s) :
   Output parameter(s) :
     *ptime : netwib_time set
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_time_init_fields(netwib_uint32 sec,
                                   netwib_uint32 msec,
                                   netwib_uint32 usec,
                                   netwib_uint32 nsec,
                                   netwib_time *ptime);
#define netwib_time_init_sec(sec,ptime) netwib_time_init_fields(sec,0,0,0,ptime)
#define netwib_time_init_msec(msec,ptime) netwib_time_init_fields(0,msec,0,0,ptime)
#define netwib_time_init_usec(usec,ptime) netwib_time_init_fields(0,0,usec,0,ptime)
#define netwib_time_init_nsec(nsec,ptime) netwib_time_init_fields(0,0,0,nsec,ptime)

/*-------------------------------------------------------------*/
/* Name : netwib_time_decode_xyz
   Description :
     Obtain time values stored in a netwib_time.
       function                 obtained ranges
       netwib_time_decode_sec    [0..2^32]
       netwib_time_decode_msec   [0..2^32]
       netwib_time_decode_usec   [0..2^32]
       netwib_time_decode_nsec   [0..2^32]
       netwib_time_decode_fields [0..2^32], [0-999], [0-999], [0-999]
   Input parameter(s) :
     *ptime : netwib_time
   Input/output parameter(s) :
   Output parameter(s) :
     *psec : seconds
     *pmsec : milliseconds
     *pusec : microseconds
     *pnsec : nanoseconds
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_NOTCONVERTED : too big to be decoded
*/
netwib_err netwib_time_decode_fields(netwib_consttime *ptime,
                                     netwib_uint32 *psec,
                                     netwib_uint32 *pmsec,
                                     netwib_uint32 *pusec,
                                     netwib_uint32 *pnsec);
netwib_err netwib_time_decode_sec(netwib_consttime *ptime,
                                  netwib_uint32 *psec);
netwib_err netwib_time_decode_msec(netwib_consttime *ptime,
                                   netwib_uint32 *pmsec);
netwib_err netwib_time_decode_usec(netwib_consttime *ptime,
                                   netwib_uint32 *pusec);
netwib_err netwib_time_decode_nsec(netwib_consttime *ptime,
                                   netwib_uint32 *pnsec);

/*-------------------------------------------------------------*/
/* Name : netwib_time_plus_time
   Description :
     Initialize a netwib_time by adding two netwib_time.
   Input parameter(s) :
     ptimetoadd : time to add
   Input/output parameter(s) :
     *ptime : netwib_time incremented by ptime
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_time_plus_time(netwib_time *ptime,
                                 netwib_consttime *ptimetoadd);
netwib_err netwib_time_plus_fields(netwib_time *ptime,
                                   netwib_uint32 sec,
                                   netwib_uint32 msec,
                                   netwib_uint32 usec,
                                   netwib_uint32 nsec);
#define netwib_time_plus_sec(ptime,sec) netwib_time_plus_fields(ptime,sec,0,0,0)
#define netwib_time_plus_msec(ptime,msec) netwib_time_plus_fields(ptime,0,msec,0,0)
#define netwib_time_plus_usec(ptime,usec) netwib_time_plus_fields(ptime,0,0,usec,0)
#define netwib_time_plus_nsec(ptime,nsec) netwib_time_plus_fields(ptime,0,0,0,nsec)

/*-------------------------------------------------------------*/
/* Name : netwib_time_minus_time
   Description :
     Initialize a netwib_time by adding two netwib_time.
   Input parameter(s) :
     ptimetoadd : time to add
   Input/output parameter(s) :
     *ptime : netwib_time incremented by ptime
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_time_minus_time(netwib_time *ptime,
                                  netwib_consttime *ptimetosub);
netwib_err netwib_time_minus_fields(netwib_time *ptime,
                                    netwib_uint32 sec,
                                    netwib_uint32 msec,
                                    netwib_uint32 usec,
                                    netwib_uint32 nsec);
#define netwib_time_minus_sec(ptime,sec) netwib_time_minus_fields(ptime,sec,0,0,0)
#define netwib_time_minus_msec(ptime,msec) netwib_time_minus_fields(ptime,0,msec,0,0)
#define netwib_time_minus_usec(ptime,usec) netwib_time_minus_fields(ptime,0,0,usec,0)
#define netwib_time_minus_nsec(ptime,nsec) netwib_time_minus_fields(ptime,0,0,0,nsec)

/*-------------------------------------------------------------*/
/* Name : netwib_time_cmp
   Description :
     Compare two netwib_time.
   Input parameter(s) :
     ptime1 : time
     ptime2 : time
   Input/output parameter(s) :
   Output parameter(s) :
     *pcmp :
       - if time1<time2, *pcmp is set to NETWIB_CMP_LT
       - if time1>time2, *pcmp is set to NETWIB_CMP_GT
       - if time1==time2, *pcmp is set to NETWIB_CMP_EQ
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_time_cmp(netwib_consttime *ptime1,
                           netwib_consttime *ptime2,
                           netwib_cmp *pcmp);

/*-------------------------------------------------------------*/
/* Name : netwib_time_sleep_xyz
   Description :
     Sleep for a time duration.
     This is not a "real time" sleep. This sleep is not precise.
     On some systems, the duration might exceed the wanted time
     by several milliseconds.
   Input parameter(s) :
     *preltime : relative time
     sec : number of seconds to wait
     msec : number of milliseconds to wait
     usec : number of microseconds to wait
   Input/output parameter(s) :
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_time_sleep_time(netwib_consttime *preltime);
netwib_err netwib_time_sleep_fields(netwib_uint32 sec,
                                    netwib_uint32 msec,
                                    netwib_uint32 usec,
                                    netwib_uint32 nsec);
#define netwib_time_sleep_sec(sec) netwib_time_sleep_fields(sec,0,0,0)
#define netwib_time_sleep_msec(msec) netwib_time_sleep_fields(0,msec,0,0)
#define netwib_time_sleep_usec(usec) netwib_time_sleep_fields(0,0,usec,0)
#define netwib_time_sleep_nsec(nsec) netwib_time_sleep_fields(0,0,0,nsec)

/*-------------------------------------------------------------*/
/* Name : netwib_time_wait_time
   Description :
     Wait an absolute time value.
     This is not a "real time" wait. This wait is not precise.
     On some systems, the duration might exceed the wanted time
     by several milliseconds.
   Input parameter(s) :
     *pabstime : absolute time (from 1970)
   Input/output parameter(s) :
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_time_wait_time(netwib_consttime *pabstime);

/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_TIME_ENCODETYPE_NOTHING = 1, /* print nothing */
  NETWIB_TIME_ENCODETYPE_BEST,        /* best display of the form :
                                         2d:3h:4m:5s:6ms:7us:8ns*/
  NETWIB_TIME_ENCODETYPE_BEST2,       /* best display of the form :
                                         2 days 3 hours 4 minutes ...*/
  NETWIB_TIME_ENCODETYPE_SEC,         /* "123s" */
  NETWIB_TIME_ENCODETYPE_SEC2,        /* "123 second(s)" */
  NETWIB_TIME_ENCODETYPE_MSEC,        /* "123ms" */
  NETWIB_TIME_ENCODETYPE_MSEC2,       /* "123 millisecond(s)" */
  NETWIB_TIME_ENCODETYPE_USEC,        /* "123us" */
  NETWIB_TIME_ENCODETYPE_USEC2,       /* "123 microsecond(s)" */
  NETWIB_TIME_ENCODETYPE_NSEC,        /* "123ns" */
  NETWIB_TIME_ENCODETYPE_NSEC2        /* "123 nanosecond(s)" */
} netwib_time_encodetype;

/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_time
   Description :
     Append a string representing a netwib_time duration (relative
     time). To display an absolute time, use function
     netwib_time_decode_localtime.
   Input parameter(s) :
     *ptime : time to print
     encodetype : netwib_time_encodetype to use
   Input/output parameter(s) :
   Output parameter(s) :
     pbuf : buffer updated
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_append_time(netwib_consttime *preltime,
                                  netwib_time_encodetype encodetype,
                                  netwib_buf *pbuf);

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 nsec; /* nanoseconds [0-999999999]*/
  netwib_int32 sec; /* seconds [0-59] */
  netwib_int32 min; /* minutes [0-59] */
  netwib_int32 hour; /* hours [0-23] */
  netwib_int32 mday; /* day [1-31] */
  netwib_int32 mon; /* month [1-12] (!= from struct tm) */
  netwib_uint32 year; /* year (start at 0) (!= from struct tm) */
  netwib_uint32 wday; /* day of the week [0(sun)-6(sat)] */
  netwib_uint32 yday; /* day of the year [1-365/366] (!= from struct tm) */
  netwib_int32 zoneoffset; /* offset in seconds between localtime
                              and GMT (for example GMT+1 is
                              +3600). This field is used only by
                              netwib_time_decode_localtime */
} netwib_localtime;
typedef const netwib_localtime netwib_constlocaltime;

/*-------------------------------------------------------------*/
/* Name : netwib_time_init_now
   Description :
     Initialize a netwib_time with the current UTC/GMT time.
     It uses the number of seconds elapsed since 00:00:00 on
     January 1, 1970, Coordinated Universal Time (UTC)
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     *ptime : netwib_time set to the current time (absolute time)
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_time_init_now(netwib_time *pabstime);

/*-------------------------------------------------------------*/
/* Name : netwib_time_init_localtime
   Description :
     Initialize a netwib_time using localtime fields
   Input parameter(s) :
     *plocaltime : structure containing day. Fields nsec, wday,
                   and yday are ignored. Other fields
                   are normalized before converting.
   Input/output parameter(s) :
   Output parameter(s) :
     *ptime : netwib_time set to the current local time
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_time_init_localtime(netwib_constlocaltime *plocaltime,
                                      netwib_time *pabstime);

/*-------------------------------------------------------------*/
/* Name : netwib_time_decode_localtime
   Description :
     Initialize a netwib_localtime using time fields
   Input parameter(s) :
     *ptime : structure containing current time (UTC)
   Input/output parameter(s) :
   Output parameter(s) :
     *plocaltime : netwib_localtime set
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_time_decode_localtime(netwib_consttime *pabstime,
                                        netwib_localtime *plocaltime);

/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_LOCALTIME_ENCODETYPE_NOTHING = 1, /* print nothing */
  NETWIB_LOCALTIME_ENCODETYPE_NSEC_ZERO,   /* 000000000-999999999 */
  NETWIB_LOCALTIME_ENCODETYPE_NSEC_SPACE,  /* ________0-999999999 */
  NETWIB_LOCALTIME_ENCODETYPE_SEC_ZERO,    /* 00-59 */
  NETWIB_LOCALTIME_ENCODETYPE_SEC_SPACE,   /* _0-59 */
  NETWIB_LOCALTIME_ENCODETYPE_MIN_ZERO,    /* 00-59 */
  NETWIB_LOCALTIME_ENCODETYPE_MIN_SPACE,   /* _0-59 */
  NETWIB_LOCALTIME_ENCODETYPE_HOUR_ZERO,   /* 00-23 */
  NETWIB_LOCALTIME_ENCODETYPE_HOUR_SPACE,  /* _0-23 */
  NETWIB_LOCALTIME_ENCODETYPE_MDAY_ZERO,   /* 01-31 */
  NETWIB_LOCALTIME_ENCODETYPE_MDAY_SPACE,  /* _1-31 */
  NETWIB_LOCALTIME_ENCODETYPE_MON_ZERO,    /* 01-12 */
  NETWIB_LOCALTIME_ENCODETYPE_MON_SPACE,   /* _1-12 */
  NETWIB_LOCALTIME_ENCODETYPE_MON_SHORT,   /* Jan */
  NETWIB_LOCALTIME_ENCODETYPE_MON_LONG,    /* January */
  NETWIB_LOCALTIME_ENCODETYPE_YEAR_SHORT,  /* 04 */
  NETWIB_LOCALTIME_ENCODETYPE_YEAR_LONG,   /* 2004 */
  NETWIB_LOCALTIME_ENCODETYPE_WDAY_SHORT,  /* Mon */
  NETWIB_LOCALTIME_ENCODETYPE_WDAY_LONG,   /* Monday */
  NETWIB_LOCALTIME_ENCODETYPE_YDAY_ZERO,   /* 001-365 */
  NETWIB_LOCALTIME_ENCODETYPE_YDAY_SPACE,  /* __1-365 */
  NETWIB_LOCALTIME_ENCODETYPE_ZONE_SEC,    /* +3600 */
  NETWIB_LOCALTIME_ENCODETYPE_ZONE_GMT,    /* GMT+1 */
  NETWIB_LOCALTIME_ENCODETYPE_ZONE_HM,     /* +0100 */
} netwib_localtime_encodetype;

/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_localtime
   Description :
     Append a string representing a field of a netwib_localtime.
   Input parameter(s) :
     *plocaltime : localtime to print
     encodetype : netwib_localtime_encodetype to use
   Input/output parameter(s) :
   Output parameter(s) :
     pbuf : buffer updated
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_append_localtime(netwib_constlocaltime *plocaltime,
                                       netwib_localtime_encodetype encodetype,
                                       netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* Name : netwib_time_iselapsed
   Description :
     Check if an absolute time has elapsed.
   Input parameter(s) :
     *ptime : time
   Input/output parameter(s) :
   Output parameter(s) :
     *pyes : true if elapsed or equal
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_time_iselapsed(netwib_consttime *pabstime,
                                 netwib_bool *pyes);
