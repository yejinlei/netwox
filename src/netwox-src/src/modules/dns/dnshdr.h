
/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_DNSHDR_OPCODE_QUERY = 0,
  NETWOX_DNSHDR_OPCODE_IQUERY = 1,
  NETWOX_DNSHDR_OPCODE_STATUS = 2,
  NETWOX_DNSHDR_OPCODE_NOTIFY = 4,
  NETWOX_DNSHDR_OPCODE_UPDATE = 5
} netwox_dnshdr_opcode;
netwib_err netwox_buf_append_dnshdr_opcode(netwox_dnshdr_opcode opcode,
                                           netwib_buf *pbuf);

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_DNSHDR_RCODE_OK = 0,
  NETWOX_DNSHDR_RCODE_FMTERROR = 1,
  NETWOX_DNSHDR_RCODE_FAILURE = 2,
  NETWOX_DNSHDR_RCODE_NAMEERROR = 3,
  NETWOX_DNSHDR_RCODE_UNIMPLEMENTED = 4,
  NETWOX_DNSHDR_RCODE_REFUSED = 5,
  NETWOX_DNSHDR_RCODE_NAMEEXISTS = 6,
  NETWOX_DNSHDR_RCODE_RRSETEXISTS = 7,
  NETWOX_DNSHDR_RCODE_RRSETNOEXISTS = 8,
  NETWOX_DNSHDR_RCODE_NOTAUTH = 9,
  NETWOX_DNSHDR_RCODE_NOTZONE = 10
} netwox_dnshdr_rcode;
netwib_err netwox_buf_append_dnshdr_rcode(netwox_dnshdr_rcode rcode,
                                          netwib_buf *pbuf);

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint16 id;
  netwib_bool isaresponse;
  netwox_dnshdr_opcode opcode;
  netwib_bool authanswer;
  netwib_bool truncation;
  netwib_bool recurdesired;
  netwib_bool recuravail;
  netwox_dnshdr_rcode rcode;
  netwib_uint16 count_question;
  netwib_uint16 count_answer;
  netwib_uint16 count_authority;
  netwib_uint16 count_additional;
} netwox_dnshdr;
typedef const netwox_dnshdr netwox_constdnshdr;

/*-------------------------------------------------------------*/
netwib_err netwox_dnshdr_initdefault(netwox_dnshdr *pdnshdr);
