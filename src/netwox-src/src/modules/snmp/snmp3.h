
/*-------------------------------------------------------------*/
/* specific to SNMPv3 */

/*-------------------------------------------------------------*/
/* rfc 3412 */
#define NETWOX_SNMP3_FLAGS_AUTH 1
#define NETWOX_SNMP3_FLAGS_PRIV (1<<1)
#define NETWOX_SNMP3_FLAGS_REPORTABLE (1<<2)
netwib_err netwox_buf_append_snmp3_flags(netwib_uint32 flags,
                                         netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* rfc 3411 */
typedef enum {
  NETWOX_SNMP3_SECURITYMODEL_ANY = 0,
  NETWOX_SNMP3_SECURITYMODEL_1 = 1,
  NETWOX_SNMP3_SECURITYMODEL_2C = 2,
  NETWOX_SNMP3_SECURITYMODEL_USM = 3
} netwox_snmp3_securitymodel;
netwib_err netwox_buf_append_snmp3_securitymodel(netwox_snmp3_securitymodel sm,
                                                 netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/* rfc 3414 */
typedef struct {
  netwib_bufpool *pbufpool;
  netwib_buf *pauthoritativeengineid;
  netwib_uint32 authoritativeengineboots;
  netwib_uint32 authoritativeenginetime;
  netwib_buf *pusername;
  netwib_buf *pauthenticationparameters;
  netwib_buf *pprivacyparameters;
} netwox_snmp3usmsp; /* usmsecurityparameters */
typedef netwox_snmp3usmsp netwox_constsnmp3usmsp;

/*-------------------------------------------------------------*/
netwib_err netwox_snmp3usmsp_init(netwib_bufpool *pbufpool,
                                  netwox_snmp3usmsp *pusmsp);
netwib_err netwox_snmp3usmsp_close(netwox_snmp3usmsp *pusmsp);

/*-------------------------------------------------------------*/
/* netwox_asn1data_init has to be called before */
netwib_err netwox_asn1data_append_snmp3usmsp(netwox_constsnmp3usmsp *pusmsp,
                                             netwox_asn1data *pasn1data);
/* netwox_snmp3usmsp_init must be called before */
netwib_err netwox_asn1data_decode_snmp3usmsp(netwox_constasn1data *pasn1data,
                                             netwox_snmp3usmsp *pusmsp);

/*-------------------------------------------------------------*/
netwib_err netwox_snmp3usmsp_show(netwox_constsnmp3usmsp *pusmsp,
                                  netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/* rfc 3412 */
typedef struct {
  netwib_bufpool *pbufpool;
  netwib_buf *pcontextengineid;
  netwib_buf *pcontextname;
  netwox_snmppdu snmppdu;
} netwox_snmp3scopedpdu;
typedef netwox_snmp3scopedpdu netwox_constsnmp3scopedpdu;

/*-------------------------------------------------------------*/
netwib_err netwox_snmp3scopedpdu_init(netwib_bufpool *pbufpool,
                                      netwox_snmp3scopedpdu *pusmsp);
netwib_err netwox_snmp3scopedpdu_close(netwox_snmp3scopedpdu *pusmsp);

/*-------------------------------------------------------------*/
/* netwox_asn1data_init has to be called before */
netwib_err netwox_asn1data_append_snmp3scopedpdu(netwox_constsnmp3scopedpdu *pusmsp,
                                                 netwox_asn1data *pasn1data);
/* netwox_snmp3scopedpdu_init must be called before */
netwib_err netwox_asn1data_decode_snmp3scopedpdu(netwox_constasn1data *pasn1data,
                                                netwox_snmp3scopedpdu *pusmsp);

/*-------------------------------------------------------------*/
netwib_err netwox_snmp3scopedpdu_show(netwox_constsnmp3scopedpdu *pusmsp,
                                      netwib_buf *pbuf);
