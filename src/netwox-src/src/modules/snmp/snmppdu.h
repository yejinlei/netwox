
/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SNMPPDUTYPE_UNSET = 0,
  NETWOX_SNMPPDUTYPE_GETREQUEST,
  NETWOX_SNMPPDUTYPE_GETNEXTREQUEST,
  NETWOX_SNMPPDUTYPE_RESPONSE,
  NETWOX_SNMPPDUTYPE_SETREQUEST,
  NETWOX_SNMPPDUTYPE_TRAP,
  NETWOX_SNMPPDUTYPE_GETBULKREQUEST,
  NETWOX_SNMPPDUTYPE_INFORMREQUEST,
  NETWOX_SNMPPDUTYPE_TRAP2,
  NETWOX_SNMPPDUTYPE_REPORT
} netwox_snmppdutype;
netwib_err netwox_buf_append_snmppdutype(netwox_snmppdutype type,
                                         netwib_buf *pbuf);

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SNMPPDU_PDU_ERRORSTATUS_NOERROR = 0,
  NETWOX_SNMPPDU_PDU_ERRORSTATUS_TOOBIG = 1,
  NETWOX_SNMPPDU_PDU_ERRORSTATUS_NOSUCHNAME = 2,
  NETWOX_SNMPPDU_PDU_ERRORSTATUS_BADVALUE = 3,
  NETWOX_SNMPPDU_PDU_ERRORSTATUS_READONLY = 4,
  NETWOX_SNMPPDU_PDU_ERRORSTATUS_GENERR = 5,
  NETWOX_SNMPPDU_PDU_ERRORSTATUS_NOACCESS = 6,
  NETWOX_SNMPPDU_PDU_ERRORSTATUS_WRONGTYPE = 7,
  NETWOX_SNMPPDU_PDU_ERRORSTATUS_WRONGLENGTH = 8,
  NETWOX_SNMPPDU_PDU_ERRORSTATUS_WRONGENCODING = 9,
  NETWOX_SNMPPDU_PDU_ERRORSTATUS_WRONGVALUE = 10,
  NETWOX_SNMPPDU_PDU_ERRORSTATUS_NOCREATION = 11,
  NETWOX_SNMPPDU_PDU_ERRORSTATUS_INCONSISTENTVALUE = 12,
  NETWOX_SNMPPDU_PDU_ERRORSTATUS_RESOURCEUNAVAILABLE = 13,
  NETWOX_SNMPPDU_PDU_ERRORSTATUS_COMMITFAILED = 14,
  NETWOX_SNMPPDU_PDU_ERRORSTATUS_UNDOFAILED = 15,
  NETWOX_SNMPPDU_PDU_ERRORSTATUS_AUTHORIZATIONERROR = 16,
  NETWOX_SNMPPDU_PDU_ERRORSTATUS_NOTWRITABLE = 17,
  NETWOX_SNMPPDU_PDU_ERRORSTATUS_INCONSISTENTNAME = 18
} netwox_snmppdu_pdu_errorstatus;
typedef struct {
  netwib_uint32 requestid;
  netwox_snmppdu_pdu_errorstatus errorstatus;
  netwib_uint32 errorindex;
  netwox_snmpvbl vbl;
} netwox_snmppdu_pdu;
netwib_err netwox_buf_append_snmppdu_pdu_errorstatus(netwox_snmppdu_pdu_errorstatus errorstatus,
                                                     netwib_buf *pbuf);

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SNMPPDU_TRAPPDU_TYPE_COLDSTART = 0,
  NETWOX_SNMPPDU_TRAPPDU_TYPE_WARMSTART = 1,
  NETWOX_SNMPPDU_TRAPPDU_TYPE_LINKDOWN = 2,
  NETWOX_SNMPPDU_TRAPPDU_TYPE_LINKUP = 3,
  NETWOX_SNMPPDU_TRAPPDU_TYPE_AUTHENTICATIONFAILURE = 4,
  NETWOX_SNMPPDU_TRAPPDU_TYPE_EGPNEIGHBORLOSS = 5,
  NETWOX_SNMPPDU_TRAPPDU_TYPE_ENTERPRISESPECIFIC = 6
} netwox_snmppdu_trappdu_type;
typedef struct {
  netwox_asn1oid enterprise;
  netwib_ip agentaddr;
  netwox_snmppdu_trappdu_type type;
  netwib_uint32 specifictrap;
  netwib_uint32 timestamp;
  netwox_snmpvbl vbl;
} netwox_snmppdu_trappdu;
netwib_err netwox_buf_append_snmppdu_trappdu_type(netwox_snmppdu_trappdu_type type,
                                                  netwib_buf *pbuf);

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 requestid;
  netwib_uint32 nonrepeaters;
  netwib_uint32 maxrepetitions;
  netwox_snmpvbl vbl;
} netwox_snmppdu_bulkpdu;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bufpool *pbufpool;
  netwox_snmppdutype type;
  union {
    netwox_snmppdu_pdu pdu; /* GetRequest, GetNextRequest,
                               Response, SetRequest, InformRequest,
                               Trap2, Report */
    netwox_snmppdu_trappdu trappdu; /* Trap */
    netwox_snmppdu_bulkpdu bulkpdu; /* GetBulkRequest */
  } pdu;
} netwox_snmppdu;
typedef netwox_snmppdu netwox_constsnmppdu;

/*-------------------------------------------------------------*/
netwib_err netwox_snmppdu_init(netwib_bufpool *pbufpool,
                               netwox_snmppdu *psnmppdu);
netwib_err netwox_snmppdu_close(netwox_snmppdu *psnmppdu);

/*-------------------------------------------------------------*/
netwib_err netwox_snmppdu_set_type(netwox_snmppdu *psnmppdu,
                                   netwox_snmppdutype type);

/*-------------------------------------------------------------*/
/* netwox_asn1data_init has to be called before */
netwib_err netwox_asn1data_append_snmppdu(netwox_constsnmppdu *psnmppdu,
                                          netwox_asn1data *pasn1data);
/* netwox_snmppdu_init must be called before */
netwib_err netwox_asn1data_decode_snmppdu(netwox_constasn1data *pasn1data,
                                          netwox_snmppdu *psnmppdu);

/*-------------------------------------------------------------*/
netwib_err netwox_snmppdu_show(netwox_constsnmppdu *psnmppdu,
                               netwib_buf *pbuf);
