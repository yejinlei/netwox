
/*-------------------------------------------------------------*/
/* ASN.1 defined in ITU-T X.690 */

/*-------------------------------------------------------------*/
/* This enum does NOT correspond to real values stored in packets.
   It permits to define all supported tags.
   It contains both ASN.1 generic tags and SNMP application tags */
typedef enum {
  NETWOX_ASN1TAG_UNSET = 0,
  NETWOX_ASN1TAG_ROOT, /* to initialize context */
  NETWOX_ASN1TAG_UNKNOWN,
  /* ASN.1 */
  NETWOX_ASN1TAG_END,
  NETWOX_ASN1TAG_BOOL,
  NETWOX_ASN1TAG_INTEGER,
  NETWOX_ASN1TAG_BITSTRING, /* constructed not implemented */
  NETWOX_ASN1TAG_OCTETSTRING, /* constructed not implemented */
  NETWOX_ASN1TAG_NULL,
  NETWOX_ASN1TAG_OID,
  NETWOX_ASN1TAG_REAL, /* not implemented */
  NETWOX_ASN1TAG_SEQUENCE,
  /* SMIv1 (rfc 1155) */
  NETWOX_ASN1TAG_IP4,
  NETWOX_ASN1TAG_COUNTER32,
  NETWOX_ASN1TAG_GAUGE32,
  NETWOX_ASN1TAG_TIMETICKS,
  NETWOX_ASN1TAG_OPAQUE,
  /* SMIv? (rfc 1442 - obsoleted by SMIv2) */
  NETWOX_ASN1TAG_NSAP,
  NETWOX_ASN1TAG_UINT32OLD,
  /* SMIv2 (rfc 1902) */
  NETWOX_ASN1TAG_UINT32, /* duplicate of GAUGE32 */
  NETWOX_ASN1TAG_COUNTER64,
  /* SNMPv1 (rfc 1157) */
  NETWOX_ASN1TAG_SNMPPDU_GETREQUEST,
  NETWOX_ASN1TAG_SNMPPDU_GETNEXTREQUEST,
  NETWOX_ASN1TAG_SNMPPDU_RESPONSE,
  NETWOX_ASN1TAG_SNMPPDU_SETREQUEST,
  NETWOX_ASN1TAG_SNMPPDU_TRAP,
  /* SNMPv2 (rfc 1905) */
  NETWOX_ASN1TAG_SNMPPDU_GETBULKREQUEST,
  NETWOX_ASN1TAG_SNMPPDU_INFORMREQUEST,
  NETWOX_ASN1TAG_SNMPPDU_TRAP2,
  NETWOX_ASN1TAG_SNMPPDU_REPORT,
  NETWOX_ASN1TAG_SNMPVAR_NOSUCHOBJECT,
  NETWOX_ASN1TAG_SNMPVAR_NOSUCHINSTANCE,
  NETWOX_ASN1TAG_SNMPVAR_ENDOFMIBVIEW,
} netwox_asn1tag;

/*-------------------------------------------------------------*/
/* Those values are stored in the packet */
typedef enum {
  NETWOX_ASN1TAG_CLASS_UNI = 0, /* universal */
  NETWOX_ASN1TAG_CLASS_APP = 1, /* application */
  NETWOX_ASN1TAG_CLASS_CTX = 2, /* context specific */
  NETWOX_ASN1TAG_CLASS_PRI = 3  /* private */
} netwox_asn1tag_class;

/*-------------------------------------------------------------*/
/* Those values are stored in the packet */
typedef enum {
  /* ASN.1 (CLASS_UNI) */
  NETWOX_ASN1TAG_NUM_END = 0,
  NETWOX_ASN1TAG_NUM_BOOL = 1,
  NETWOX_ASN1TAG_NUM_INTEGER = 2,
  NETWOX_ASN1TAG_NUM_BITSTRING = 3,
  NETWOX_ASN1TAG_NUM_OCTETSTRING = 4,
  NETWOX_ASN1TAG_NUM_NULL = 5,
  NETWOX_ASN1TAG_NUM_OID = 6,
  NETWOX_ASN1TAG_NUM_REAL = 9,
  NETWOX_ASN1TAG_NUM_SEQUENCE = 16,
  /* SMIv1 (CLASS_APP) */
  NETWOX_ASN1TAG_NUM_IP4 = 0,
  NETWOX_ASN1TAG_NUM_COUNTER32 = 1,
  NETWOX_ASN1TAG_NUM_GAUGE32 = 2,
  NETWOX_ASN1TAG_NUM_TIMETICKS = 3,
  NETWOX_ASN1TAG_NUM_OPAQUE = 4,
  /* SMIv? (CLASS_APP) */
  NETWOX_ASN1TAG_NUM_NSAP = 5,
  NETWOX_ASN1TAG_NUM_UINT32OLD = 7,
  /* SMIv2 (CLASS_APP) */
  NETWOX_ASN1TAG_NUM_UINT32 = 2, /* duplicate of GAUGE32 */
  NETWOX_ASN1TAG_NUM_COUNTER64 = 6,
  /* SNMPv1 (CLASS_CTX) */
  NETWOX_ASN1TAG_NUM_SNMPPDU_GETREQUEST = 0,
  NETWOX_ASN1TAG_NUM_SNMPPDU_GETNEXTREQUEST = 1,
  NETWOX_ASN1TAG_NUM_SNMPPDU_RESPONSE = 2,
  NETWOX_ASN1TAG_NUM_SNMPPDU_SETREQUEST = 3,
  NETWOX_ASN1TAG_NUM_SNMPPDU_TRAP = 4,
  /* SNMPv2 (CLASS_CTX) */
  NETWOX_ASN1TAG_NUM_SNMPPDU_GETBULKREQUEST = 5,
  NETWOX_ASN1TAG_NUM_SNMPPDU_INFORMREQUEST = 6,
  NETWOX_ASN1TAG_NUM_SNMPPDU_TRAP2 = 7,
  NETWOX_ASN1TAG_NUM_SNMPPDU_REPORT = 8,
  NETWOX_ASN1TAG_NUM_SNMPVAR_NOSUCHOBJECT = 0,
  NETWOX_ASN1TAG_NUM_SNMPVAR_NOSUCHINSTANCE = 1,
  NETWOX_ASN1TAG_NUM_SNMPVAR_ENDOFMIBVIEW = 2,
} netwox_asn1tag_num;

/*-------------------------------------------------------------*/
/* extensions to ASN.1 provided by some protocols */
typedef enum {
  NETWOX_ASN1TAG_EXT_NONE = 1,
  NETWOX_ASN1TAG_EXT_SNMP,
  NETWOX_ASN1TAG_EXT_SNMP3SP, /* security parameters */
  NETWOX_ASN1TAG_EXT_LDAP /* not yet implemented */
} netwox_asn1tag_ext;

/*-------------------------------------------------------------*/
/* context for setting CLASS_CTX values. We need this because
   for example NETWOX_ASN1TAG_NUM_SNMPVAR_ENDOFMIBVIEW and
   NETWOX_ASN1TAG_NUM_SNMPPDU_RESPONSE have the same value.
   To distinguish them, we obtain the context : PDU or VAR.
   Array tag contains the path to current asn1tag. */
typedef struct {
  netwib_array tag;
} netwox_asn1tag_ctx;
typedef enum {
  NETWOX_ASN1TAG_CTXTYPE_UNKNOWN = 1,
  NETWOX_ASN1TAG_CTXTYPE_SNMPPDU,
  NETWOX_ASN1TAG_CTXTYPE_SNMPVAR
} netwox_asn1tag_ctxtype;
netwib_err netwox_asn1tag_ctx_init(netwox_asn1tag_ctx *pctx);
netwib_err netwox_asn1tag_ctx_close(netwox_asn1tag_ctx *pctx);
netwib_err netwox_asn1tag_ctx_add(netwox_asn1tag_ctx *pctx,
                                  netwox_asn1tag asn1tag);
netwib_err netwox_asn1tag_ctx_del(netwox_asn1tag_ctx *pctx);
netwib_err netwox_asn1tag_ctx_get(netwox_asn1tag_ctx *pctx,
                                  netwox_asn1tag_ctxtype *pctxtype);

/*-------------------------------------------------------------*/
netwib_err netwox_asn1tag_init(netwox_asn1tag_ext ext,
                               netwox_asn1tag_ctx *pctx,
                               netwox_asn1tag_class cl,
                               netwox_asn1tag_num num,
                               netwox_asn1tag *ptag);
netwib_err netwox_asn1tag_decode(netwox_asn1tag tag,
                                 netwox_asn1tag_class *pcl,
                                 netwib_bool *pconstructed,
                                 netwox_asn1tag_num *pnum);
