
/*-------------------------------------------------------------*/
/*
  List of documents defining NTP:
   - NTP version 0 (called 0 in Appendix D of rfc 1305):
       rfc 958
   - NTP version 1:
       rfc 1059
   - NTP version 2:
       rfc 1119
   - NTP version 3:
       rfc 1305
   - NTP version 4:
       no rfc exists, but it is "defined" in
       ntp-4.x.tgz/html/release.html (http://www.ntp.org/)

  List of documents defining SNTP:
   - SNTP version 3
       rfc 1361, 1769
   - SNTP version 4
       rfc 2030

  What's supported by netwox:
   - NTP 0 format is different, so it is not implemented in netwox
   - NTP 1 is supported
   - NTP 2 is supported
   - NTP 3 is supported, except authentication
   - NTP 4 is not implemented
   - SNTP 3 is supported, except authentication
   - SNTP 4 is supported, except authentication
*/

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_NTPLEAPINDIC_NORMAL = 0,
  NETWOX_NTPLEAPINDIC_LAST61 = 1,
  NETWOX_NTPLEAPINDIC_LAST59 = 2,
  NETWOX_NTPLEAPINDIC_NOTSYNC = 3
} netwox_ntpleapindic;

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_NTPVERSION_0 = 0,
  NETWOX_NTPVERSION_1 = 1,
  NETWOX_NTPVERSION_2 = 2,
  NETWOX_NTPVERSION_3 = 3,
  NETWOX_NTPVERSION_4 = 4
} netwox_ntpversion;

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_NTPMODE_RESERVED = 0,
  NETWOX_NTPMODE_SYMACTIVE = 1,
  NETWOX_NTPMODE_SYMPASSIVE = 2,
  NETWOX_NTPMODE_CLIENT = 3,
  NETWOX_NTPMODE_SERVER = 4,
  NETWOX_NTPMODE_BROADCAST = 5,
  NETWOX_NTPMODE_CTL = 6, /* not implemented */
  NETWOX_NTPMODE_PRIV = 7 /* not implemented */
} netwox_ntpmode;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 sec;
  netwib_uint32 fraction;
} netwox_ntptimestamp;
typedef const netwox_ntptimestamp netwox_constntptimestamp;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_ntpleapindic ntpleapindic;
  netwox_ntpversion ntpversion;
  netwox_ntpmode ntpmode;
  netwib_uint8 stratum;
  netwib_int8 pool;
  netwib_int8 precision;
  netwib_int32 rootdelay;
  netwib_uint32 rootdispersion;
  netwib_byte refid[4];
  netwox_ntptimestamp refts;
  netwox_ntptimestamp origts;
  netwox_ntptimestamp recvts;
  netwox_ntptimestamp transts;
  netwib_bufext extsauth; /* extensions and authentication are
                             not implemented: only stored in a buffer */
} netwox_ntphdr;
typedef const netwox_ntphdr netwox_constntphdr;

/*-------------------------------------------------------------*/
netwib_err netwox_ntphdr_initdefault(netwox_ntphdr *pntphdr);

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_ntphdr(netwox_constntphdr *pntphdr,
                                    netwib_buf *ppkt);
netwib_err netwox_pkt_decode_ntphdr(netwib_constbuf *ppkt,
                                    netwox_ntphdr *pntphdr);

/*-------------------------------------------------------------*/
netwib_err netwox_ntphdr_show(netwox_constntphdr *pntphdr,
                              netwib_encodetype encodetype,
                              netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_ntphdr_test(void);

/*-------------------------------------------------------------*/
/* needed because NTP start on 1900 (and Unix 1970) */
netwib_err netwox_time_init_ntptimestamp(netwox_constntptimestamp *pts,
                                         netwib_time *pt);
netwib_err netwox_ntptimestamp_init_time(netwib_consttime *pt,
                                         netwox_ntptimestamp *pts);
