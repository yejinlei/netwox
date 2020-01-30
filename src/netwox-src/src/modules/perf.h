
/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_PERFMSGTYPE_HELLO = 1,
  NETWOX_PERFMSGTYPE_DATA,
  NETWOX_PERFMSGTYPE_BYE,
  NETWOX_PERFMSGTYPE_BYEACK
} netwox_perfmsgtype;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 size; /* message size including this field
                         (it is common to all messages, but does
                         not need to be set by user) */
  netwox_perfmsgtype perfmsgtype;
  union {
    struct {
      netwib_uint32 version; /* currently 1 */
      netwib_uint32 durationms; /* requested duration in ms */
      netwib_uint32 datasize; /* requested data size */
    } hello;
    struct {
      netwib_time sendtime;
      netwib_time recvtime;
    } data;
    struct {
      netwib_uint32 numdatasent;
    } bye;
    struct {
      netwib_uint32 unused;
    } byeack;
  } msg;
} netwox_perfmsg;
typedef const netwox_perfmsg netwox_constperfmsg;

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_perfmsg(netwox_constperfmsg *pmsg,
                                     netwib_buf *ppkt);
netwib_err netwox_pkt_decode_perfmsg(netwib_buf *ppkt,
                                     netwox_perfmsg *pmsg);

/*-------------------------------------------------------------*/
typedef struct {
  netwib_io *pio;
  netwib_uint32 durationms;
  netwib_uint32 size;
} netwox_perfinfo;

/*-------------------------------------------------------------*/
netwib_err netwox_perf_ser(netwib_io *pio);
netwib_err netwox_perf_cli(netwox_perfinfo *pperfinfo);
