
/*-------------------------------------------------------------*/
/* rfc 1350 */

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_TFTPOPCODE_RRQ = 1,
  NETWOX_TFTPOPCODE_WRQ = 2,
  NETWOX_TFTPOPCODE_DATA = 3,
  NETWOX_TFTPOPCODE_ACK = 4,
  NETWOX_TFTPOPCODE_ERROR = 5
} netwox_tftpopcode;

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_TFTPERROR_NOTDEFINED = 0,
  NETWOX_TFTPERROR_FILENOTFOUND = 1,
  NETWOX_TFTPERROR_ACCESSVIOLATION = 2,
  NETWOX_TFTPERROR_DISKFULL = 3,
  NETWOX_TFTPERROR_ILLEGALOPERATION = 4,
  NETWOX_TFTPERROR_UNKNOWNTID = 5,
  NETWOX_TFTPERROR_FILEEXISTS = 6,
  NETWOX_TFTPERROR_NOSUCHUSER = 7
} netwox_tftperror;

/*-------------------------------------------------------------*/
netwib_err netwox_buf_append_tftpopcode(netwox_tftpopcode opcode,
                                        netwib_buf *pbuf);

/*-------------------------------------------------------------*/
typedef struct {
  netwox_tftpopcode opcode;
  union {
    struct {
      netwib_bufext filename;
      netwib_bufext mode;
    } rq;
    struct {
      netwib_uint16 block;
      netwib_bufext data;
    } data;
    struct {
      netwib_uint16 block;
    } ack;
    struct {
      netwib_uint16 code;
      netwib_bufext msg;
    } error;
  } pkt;
} netwox_tftppkt;
typedef netwox_tftppkt netwox_consttftppkt;

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_tftppkt(netwox_consttftppkt *ptftppkt,
                                     netwib_buf *ppkt);
netwib_err netwox_pkt_decode_tftppkt(netwib_constbuf *ppkt,
                                     netwox_tftppkt *ptftppkt);

/*-------------------------------------------------------------*/
netwib_err netwox_tftppkt_show(netwox_consttftppkt *ptftppkt,
                               netwib_encodetype encodetype,
                               netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_tftp_show(netwib_bufpool *pbufpool,
                                netwib_constbuf *ppkt,
                                netwib_encodetype_context *pctx,
                                netwib_encodetype tftpencodetype,
                                netwib_encodetype dataencodetype,
                                netwib_buf *pbuf);
netwib_err netwox_pkt_tftp_display(netwib_bufpool *pbufpool,
                                   netwib_constbuf *ppkt,
                                   netwib_encodetype_context *pctx,
                                   netwib_encodetype tftpencodetype,
                                   netwib_encodetype dataencodetype);
