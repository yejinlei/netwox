
/*-------------------------------------------------------------*/
/*
  rfc 1155-1157 : SNMPv1
  rfc 1441-1452 : SNMPv2 classic (not supported)
  rfc 1901-1908 : SNMPv2c Community-based
  no rfc        : SNMPv2 usec (not supported)
  no rfc        : SNMPv2* (not supported)
  no rfc        : SNMP-NG (not supported)
  rfc 3410-3418 : SNMPv3
*/
typedef enum {
  NETWOX_SNMPVERSION_1 = 0,
  NETWOX_SNMPVERSION_2C = 1,
  NETWOX_SNMPVERSION_2U = 2, /* usec : not supported */
  NETWOX_SNMPVERSION_3 = 3,
  NETWOX_SNMPVERSION_UNSET = 0xFFFF
} netwox_snmpversion;
netwib_err netwox_buf_append_snmpversion(netwox_snmpversion version,
                                         netwib_buf *pbuf);

/*-------------------------------------------------------------*/
typedef struct {
  /* common */
  netwib_bufpool *pbufpool;
  netwox_snmpversion version;
  /* version specific */
  union {
    /* v1 and v2 */
    struct {
      netwib_buf *pcommunity;
      netwox_snmppdu snmppdu;
    } v12;
    /* v3 */
    struct {
      struct {
        netwib_uint32 id;
        netwib_uint32 maxsize;
        netwib_uint32 flags;
        netwib_uint32 securitymodel;
      } globalheader;
      netwib_buf *psecurityparameters; /* generally netwox_snmp3usmsp
                                          encoded in ASN.1 BER */
      /* note : clearpdu and encryptedpdu are not in an union, even if they
         are opposite, because it's easier to have a place to store
         cypher/decypher result */
      netwox_snmp3scopedpdu clearpdu; /* if NETWOX_SNMP3_MSGFLAGS_PRIV unset */
      netwib_buf *pencryptedpdu; /* used if NETWOX_SNMP3_MSGFLAGS_PRIV set */
    } v3;
  } v;
} netwox_snmpmsg;
typedef netwox_snmpmsg netwox_constsnmpmsg;

/*-------------------------------------------------------------*/
netwib_err netwox_snmpmsg_init(netwib_bufpool *pbufpool,
                               netwox_snmpmsg *psnmpmsg);
netwib_err netwox_snmpmsg_close(netwox_snmpmsg *psnmpmsg);

/*-------------------------------------------------------------*/
netwib_err netwox_snmpmsg_set_type(netwox_snmpmsg *psnmpmsg,
                                   netwox_snmpversion version);

/*-------------------------------------------------------------*/
/* netwox_asn1data_init has to be called before */
netwib_err netwox_asn1data_append_snmpmsg(netwox_constsnmpmsg *psnmpmsg,
                                          netwox_asn1data *pasn1data);
/* netwox_snmpmsg_init must be called before */
netwib_err netwox_asn1data_decode_snmpmsg(netwox_constasn1data *pasn1data,
                                          netwox_snmpmsg *psnmpmsg);

/*-------------------------------------------------------------*/
netwib_err netwox_snmpmsg_show(netwox_constsnmpmsg *psnmpmsg,
                               netwib_encodetype encodetype,
                               netwib_buf *pbuf);


