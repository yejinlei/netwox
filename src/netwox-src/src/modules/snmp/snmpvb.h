
/*-------------------------------------------------------------*/
typedef struct {
  netwib_bufpool *pbufpool;
  netwox_asn1oid oid;
  netwox_asn1data data;
} netwox_snmpvb; /* varbind */
typedef const netwox_snmpvb netwox_constsnmpvb;

/*-------------------------------------------------------------*/
netwib_err netwox_snmpvb_init(netwib_bufpool *pbufpool,
                              netwox_snmpvb *psnmpvb);
netwib_err netwox_snmpvb_close(netwox_snmpvb *psnmpvb);

/*-------------------------------------------------------------*/
/* netwox_asn1data_init has to be called before */
netwib_err netwox_asn1data_append_snmpvb(netwox_constsnmpvb *psnmpvb,
                                         netwox_asn1data *pasn1data);
/* netwox_snmpvb_init must be called before */
netwib_err netwox_asn1data_decode_snmpvb(netwox_constasn1data *pasn1data,
                                         netwox_snmpvb *psnmpvb);

/*-------------------------------------------------------------*/
netwib_err netwox_snmpvb_show(netwox_constsnmpvb *psnmpvb,
                              netwib_buf *pbuf);
