
/*-------------------------------------------------------------*/
/* indicate how to represent data */
typedef enum {
  NETWOX_ASN1DATASTORE_NOTHING = 1, /* end, null, snmpvar_xyz */
  NETWOX_ASN1DATASTORE_B, /* bool */
  NETWOX_ASN1DATASTORE_I, /* integer : 64 bits if supported */
  NETWOX_ASN1DATASTORE_UI32, /* uint32, uint32old, counter32, gauge32,
                                timeticks */
  NETWOX_ASN1DATASTORE_UI64, /* counter64 */
  NETWOX_ASN1DATASTORE_BUF, /* octetstring, opaque, nsap */
  NETWOX_ASN1DATASTORE_UNKNOWN, /* unknown */
  NETWOX_ASN1DATASTORE_IPAD, /* ip4 */
  NETWOX_ASN1DATASTORE_BITSTRING, /* bitstring */
  NETWOX_ASN1DATASTORE_OID, /* oid */
  NETWOX_ASN1DATASTORE_SEQUENCE /* sequence, snmppdu_xyz */
} netwox_asn1datastore;

/*-------------------------------------------------------------*/
/* real data storage */
typedef struct {
  netwox_asn1tag_class cl;
  netwib_bool constructed;
  netwox_asn1tag_num num;
  netwib_buf *pbuf;
} netwox_asn1data_unknown;
typedef struct {
  netwib_uint32 numbits;
  netwib_buf *pbuf;
} netwox_asn1data_bitstring;
struct netwox_asn1data;
typedef struct {
  netwib_array seq;
} netwox_asn1data_sequence;
typedef struct netwox_asn1data {
  netwib_bufpool *pbufpool;
  netwox_asn1tag asn1tag;
  netwox_asn1datastore asn1datastore;
  union {
    netwib_bool b;
    netwib_intmax i;
    netwib_uint32 ui32;
    netwib_uint64 ui64;
    netwib_buf *pbuf;
    netwox_asn1data_unknown unknown;
    netwib_ip ipad;
    netwox_asn1data_bitstring bitstring;
    netwox_asn1oid oid;
    netwox_asn1data_sequence sequence;
  } value;
} netwox_asn1data;
typedef const netwox_asn1data netwox_constasn1data;

/*-------------------------------------------------------------*/
netwib_err netwox_asn1data_init(netwib_bufpool *pbufpool,
                                netwox_asn1data *pasn1data);
netwib_err netwox_asn1data_close(netwox_asn1data *pasn1data);

/*-------------------------------------------------------------*/
netwib_err netwox_asn1data_set_type(netwox_asn1data *pasn1data,
                                    netwox_asn1tag asn1tag);

/*-------------------------------------------------------------*/
netwib_err netwox_asn1data_add_oid(netwox_asn1data *pasn1data,
                                   netwib_uint32 subid);
/* add subids of a buffer containing "1.2.3.4..." */
netwib_err netwox_asn1data_add_oid_buf(netwox_asn1data *pasn1data,
                                       netwib_constbuf *pbuf);

/*-------------------------------------------------------------*/
/* pdatatoadd is directly put in pdata, so it must not be closed
   after a call to this function */
netwib_err netwox_asn1data_add_sequence(netwox_asn1data *pasn1data,
                                        netwox_constasn1data *pdatatoadd);

/*-------------------------------------------------------------*/
/* netwox_asn1data_init must be called before on pasn1datadst */
netwib_err netwox_asn1data_dup(netwox_constasn1data *pasn1datasrc,
                               netwox_asn1data *pasn1datadst);

/*-------------------------------------------------------------*/
netwib_err netwox_asn1data_init_arg(netwib_bufpool *pbufpool,
                                    netwox_arg *parg,
                                    netwib_char key1,
                                    netwib_char key2,
                                    netwox_asn1data *pasn1data);

/*-------------------------------------------------------------*/
#define netwox__asn1data_decode_sequence(pasn1data,plevel) if ((pasn1data)->asn1tag != NETWOX_ASN1TAG_SEQUENCE) return(NETWIB_ERR_NOTCONVERTED); plevel = (netwox_asn1data **)(pasn1data)->value.sequence.seq.p;
#define netwox__asn1data_decode_sequencestore(pasn1data,plevel) if ((pasn1data)->asn1datastore != NETWOX_ASN1DATASTORE_SEQUENCE) return(NETWIB_ERR_NOTCONVERTED); plevel = (netwox_asn1data **)(pasn1data)->value.sequence.seq.p;
#define netwox__asn1data_check_sequence_size(pasn1data,wantedsize) if ((pasn1data)->value.sequence.seq.size != wantedsize) return(NETWIB_ERR_NOTCONVERTED);
#define netwox__asn1data_check_sequence_minsize(pasn1data,wantedsize) if ((pasn1data)->value.sequence.seq.size < wantedsize) return(NETWIB_ERR_NOTCONVERTED);

#define netwox__asn1data_decode_integer(pasn1data,ii) if ((pasn1data)->asn1tag != NETWOX_ASN1TAG_INTEGER) return(NETWIB_ERR_NOTCONVERTED); ii = (pasn1data)->value.i;
/* we could also check if value.i can feet into sizeof(type) */
#define netwox__asn1data_decode_integert(pasn1data,type,ii) if ((pasn1data)->asn1tag != NETWOX_ASN1TAG_INTEGER) return(NETWIB_ERR_NOTCONVERTED); ii = (type)((pasn1data)->value.i);
#define netwox__asn1data_decode_timeticks(pasn1data,ii) if ((pasn1data)->asn1tag != NETWOX_ASN1TAG_TIMETICKS) return(NETWIB_ERR_NOTCONVERTED); ii = (pasn1data)->value.ui32;
#define netwox__asn1data_decode_ip(pasn1data,ipa) if ((pasn1data)->asn1tag != NETWOX_ASN1TAG_IP4) return(NETWIB_ERR_NOTCONVERTED); ipa = (pasn1data)->value.ipad;
#define netwox__asn1data_decode_octetstring(pasn1data,pbufcopy) if ((pasn1data)->asn1tag != NETWOX_ASN1TAG_OCTETSTRING) return(NETWIB_ERR_NOTCONVERTED); pbufcopy = (pasn1data)->value.pbuf;
#define netwox__asn1data_decode_octetstring_dup(pasn1data,pbufappend) if ((pasn1data)->asn1tag != NETWOX_ASN1TAG_OCTETSTRING) return(NETWIB_ERR_NOTCONVERTED); netwib_er(netwib_buf_append_buf((pasn1data)->value.pbuf, pbufappend));
#define netwox__asn1data_decode_oid_dup(pasn1data,poid) if ((pasn1data)->asn1tag != NETWOX_ASN1TAG_OID) return(NETWIB_ERR_NOTCONVERTED); netwib_er(netwox_asn1oid_dup(&(pasn1data)->value.oid, poid));

/*-------------------------------------------------------------*/
#define netwox__asn1data_append_integer(pasn1data,ii) netwib_er(netwox_asn1data_init((pasn1data)->pbufpool, &asn1data)); netwib_er(netwox_asn1data_set_type(&asn1data, NETWOX_ASN1TAG_INTEGER)); asn1data.value.i = ii; netwib_er(netwox_asn1data_add_sequence((pasn1data), &asn1data));
#define netwox__asn1data_append_timeticks(pasn1data,ii) netwib_er(netwox_asn1data_init((pasn1data)->pbufpool, &asn1data)); netwib_er(netwox_asn1data_set_type(&asn1data, NETWOX_ASN1TAG_TIMETICKS)); asn1data.value.ui32 = ii; netwib_er(netwox_asn1data_add_sequence((pasn1data), &asn1data));
#define netwox__asn1data_append_octetstring_dup(pasn1data,pbufappend) netwib_er(netwox_asn1data_init((pasn1data)->pbufpool, &asn1data)); netwib_er(netwox_asn1data_set_type(&asn1data, NETWOX_ASN1TAG_OCTETSTRING)); netwib_er(netwib_buf_append_buf(pbufappend, asn1data.value.pbuf)); netwib_er(netwox_asn1data_add_sequence((pasn1data), &asn1data));
#define netwox__asn1data_append_oid_dup(pasn1data,poid) netwib_er(netwox_asn1data_init((pasn1data)->pbufpool, &asn1data)); netwib_er(netwox_asn1data_set_type(&asn1data, NETWOX_ASN1TAG_OID)); netwib_er(netwox_asn1oid_dup(poid, &asn1data.value.oid)); netwib_er(netwox_asn1data_add_sequence((pasn1data), &asn1data));
#define netwox__asn1data_append_ip(pasn1data,ipa) netwib_er(netwox_asn1data_init((pasn1data)->pbufpool, &asn1data)); netwib_er(netwox_asn1data_set_type(&asn1data, NETWOX_ASN1TAG_IP4)); asn1data.value.ipad = ipa; netwib_er(netwox_asn1data_add_sequence((pasn1data), &asn1data));
#define netwox__asn1data_append_asn1data_dup(pasn1data,pad) netwib_er(netwox_asn1data_init((pasn1data)->pbufpool, &asn1data)); netwib_er(netwox_asn1data_dup(pad, &asn1data)); netwib_er(netwox_asn1data_add_sequence((pasn1data), &asn1data));


