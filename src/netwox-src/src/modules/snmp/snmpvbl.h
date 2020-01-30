
/*-------------------------------------------------------------*/
typedef struct {
  netwib_array vb;
  netwib_bufpool *pbufpool;
} netwox_snmpvbl; /* varbindlist */
typedef const netwox_snmpvbl netwox_constsnmpvbl;

/*-------------------------------------------------------------*/
netwib_err netwox_snmpvbl_init(netwib_bufpool *pbufpool,
                               netwox_snmpvbl *psnmpvbl);
netwib_err netwox_snmpvbl_close(netwox_snmpvbl *psnmpvbl);

/*-------------------------------------------------------------*/
/* psnmpvb is directly put in psnmpvbl, so its items must not be closed
   after a call to this function */
netwib_err netwox_snmpvbl_add(netwox_snmpvbl *psnmpvbl,
                              netwox_constsnmpvb *psnmpvb);

/*-------------------------------------------------------------*/
/* netwox_asn1data_init has to be called before */
netwib_err netwox_asn1data_append_snmpvbl(netwox_constsnmpvbl *psnmpvbl,
                                          netwox_asn1data *pasn1data);
/* netwox_snmpvbl_init must be called before */
netwib_err netwox_asn1data_decode_snmpvbl(netwox_constasn1data *pasn1data,
                                          netwox_snmpvbl *psnmpvbl);

/*-------------------------------------------------------------*/
netwib_err netwox_snmpvbl_show(netwox_constsnmpvbl *psnmpvbl,
                               netwib_buf *pbuf);
