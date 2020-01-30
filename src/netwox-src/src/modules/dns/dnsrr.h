
/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_DNSRR_TYPE_UNKNOWN = 0,
  NETWOX_DNSRR_TYPE_A = 1,
  NETWOX_DNSRR_TYPE_NS = 2,
  NETWOX_DNSRR_TYPE_CNAME = 5,
  NETWOX_DNSRR_TYPE_SOA = 6,
  NETWOX_DNSRR_TYPE_WKS = 11, /* rfc 1035, but not impl (NI) - well known s. */
  NETWOX_DNSRR_TYPE_PTR = 12,
  NETWOX_DNSRR_TYPE_HINFO = 13,
  NETWOX_DNSRR_TYPE_MX = 15,
  NETWOX_DNSRR_TYPE_TXT = 16,
  NETWOX_DNSRR_TYPE_RP = 17, /* rfc 1183 - responsible person - NI */
  NETWOX_DNSRR_TYPE_AAAA = 28, /* rfc 1886 */
  NETWOX_DNSRR_TYPE_LOC = 29, /* rfc 1876 - geo. location - NI */
  NETWOX_DNSRR_TYPE_SRV = 33, /* rfc 2052 - service location - NI */
  NETWOX_DNSRR_TYPE_A6 = 38, /* rfc 2874 */
  NETWOX_DNSRR_TYPE_OPT = 41, /* rfc 2671 - options - NI */
  NETWOX_DNSRR_TYPE_AXFR = 252,
  NETWOX_DNSRR_TYPE_ANY = 255
} netwox_dnsrr_type;
netwib_err netwox_dnsrr_type_init_buf(netwib_constbuf *pbuf,
                                      netwox_dnsrr_type *ptype);
netwib_err netwox_buf_append_dnsrr_type(netwox_dnsrr_type type,
                                        netwib_buf *pbuf);

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_DNSRR_CLASS_IN = 1,
  NETWOX_DNSRR_CLASS_CS = 2,
  NETWOX_DNSRR_CLASS_CH = 3,
  NETWOX_DNSRR_CLASS_HS = 4,
  NETWOX_DNSRR_CLASS_ALL = 255
} netwox_dnsrr_class;
netwib_err netwox_dnsrr_class_init_buf(netwib_constbuf *pbuf,
                                       netwox_dnsrr_class *pclass);
netwib_err netwox_buf_append_dnsrr_class(netwox_dnsrr_class rclass,
                                         netwib_buf *pbuf);

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_DNSRR_KIND_UNKNOWN = 0,
  NETWOX_DNSRR_KIND_QUESTION = 1,
  NETWOX_DNSRR_KIND_ANSWER = 2,
  NETWOX_DNSRR_KIND_AUTHORITY = 3,
  NETWOX_DNSRR_KIND_ADDITIONAL = 4
} netwox_dnsrr_kind;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_dnsrr_kind kind;
  netwib_buf name;
  netwox_dnsrr_type type;
  netwox_dnsrr_class rclass; /* cannot use "class" because of C++ */
  netwib_uint32 ttl;
  netwib_buf rdata; /* this field is always uncompressed */
} netwox_dnsrr;
typedef const netwox_dnsrr netwox_constdnsrr;

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrr_init(netwox_dnsrr *prr);
netwib_err netwox_dnsrr_close(netwox_dnsrr *prr);

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrr_set(netwox_dnsrr *prr,
                            netwox_dnsrr_kind kind,
                            netwib_constbuf *pname,
                            netwox_dnsrr_type type,
                            netwox_dnsrr_class rclass,
                            netwib_uint32 ttl);
#define netwox_dnsrr_set_question(prr,pname,type,rclass) netwox_dnsrr_set(prr,NETWOX_DNSRR_KIND_QUESTION,pname,type,rclass,0)
#define netwox_dnsrr_set_answer(prr,pname,type,rclass,ttl) netwox_dnsrr_set(prr,NETWOX_DNSRR_KIND_ANSWER,pname,type,rclass,ttl)
#define netwox_dnsrr_set_authority(prr,pname,type,rclass,ttl) netwox_dnsrr_set(prr,NETWOX_DNSRR_KIND_AUTHORITY,pname,type,rclass,ttl)
#define netwox_dnsrr_set_additional(prr,pname,type,rclass,ttl) netwox_dnsrr_set(prr,NETWOX_DNSRR_KIND_ADDITIONAL,pname,type,rclass,ttl)
