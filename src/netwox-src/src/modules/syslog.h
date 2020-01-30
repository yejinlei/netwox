
/*-------------------------------------------------------------*/
/* RFC 3164 */

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SYSLOG_FACILITY_KERN = 0,
  NETWOX_SYSLOG_FACILITY_USER = 1,
  NETWOX_SYSLOG_FACILITY_MAIL = 2,
  NETWOX_SYSLOG_FACILITY_DAEMON = 3,
  NETWOX_SYSLOG_FACILITY_AUTH = 4,
  NETWOX_SYSLOG_FACILITY_SYSLOG = 5,
  NETWOX_SYSLOG_FACILITY_LPR = 6,
  NETWOX_SYSLOG_FACILITY_NEWS = 7,
  NETWOX_SYSLOG_FACILITY_UUCP = 8,
  NETWOX_SYSLOG_FACILITY_CRON = 9,
  NETWOX_SYSLOG_FACILITY_AUTHPRIV = 10,
  NETWOX_SYSLOG_FACILITY_FTP = 11,
  NETWOX_SYSLOG_FACILITY_NTP = 12,
  NETWOX_SYSLOG_FACILITY_LOGAUDIT = 13,
  NETWOX_SYSLOG_FACILITY_LOGALERT = 14,
  NETWOX_SYSLOG_FACILITY_CLOCK = 15,
  NETWOX_SYSLOG_FACILITY_LOCAL0 = 16,
  NETWOX_SYSLOG_FACILITY_LOCAL1 = 17,
  NETWOX_SYSLOG_FACILITY_LOCAL2 = 18,
  NETWOX_SYSLOG_FACILITY_LOCAL3 = 19,
  NETWOX_SYSLOG_FACILITY_LOCAL4 = 20,
  NETWOX_SYSLOG_FACILITY_LOCAL5 = 21,
  NETWOX_SYSLOG_FACILITY_LOCAL6 = 22,
  NETWOX_SYSLOG_FACILITY_LOCAL7 = 23
} netwox_syslog_facility;

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SYSLOG_SEVERITY_EMERG = 0,
  NETWOX_SYSLOG_SEVERITY_ALERT = 1,
  NETWOX_SYSLOG_SEVERITY_CRIT = 2,
  NETWOX_SYSLOG_SEVERITY_ERR = 3,
  NETWOX_SYSLOG_SEVERITY_WARNING = 4,
  NETWOX_SYSLOG_SEVERITY_NOTICE = 5,
  NETWOX_SYSLOG_SEVERITY_INFO = 6,
  NETWOX_SYSLOG_SEVERITY_DEBUG = 7
} netwox_syslog_severity;

/*-------------------------------------------------------------*/
typedef netwib_uint32 netwox_syslog_priority;

/*-------------------------------------------------------------*/
#define netwox_syslog_priority_init(facility, severity) (((facility) << 3) | (severity))
#define netwox_syslog_priority_decode_facility(priority) ((priority) >> 3)
#define netwox_syslog_priority_decode_severity(priority) ((priority) & 0x7)

/*-------------------------------------------------------------*/
netwib_err netwox_syslog_msg_init(netwox_syslog_priority priority,
                                  netwib_time *ptime,
                                  netwib_buf *phostname,
                                  netwib_ip *pip,
                                  netwib_constbuf *pmessage,
                                  netwib_buf *pbuf);
