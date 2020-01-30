
/*-------------------------------------------------------------*/
/* rfc 3414 : auth using MD5 (SNMPv3, USM) */

/*-------------------------------------------------------------*/
/* compute user key (Ku in rfc) */
netwib_err netwox_snmpmd5_ku(netwib_constbuf *ppassword,
                             netwib_buf *pku);

/*-------------------------------------------------------------*/
/* compute localized user key (Kul in rfc) */
netwib_err netwox_snmpmd5_kul(netwib_bufpool *pbufpool,
                              netwib_constbuf *pku,
                              netwib_constbuf *pengineid,
                              netwib_buf *pkul);

/*-------------------------------------------------------------*/
/* Compute psnmp3usmsp->pauthenticationparameters and
   psnmpmsg->v.v3.psecurityparameters */
netwib_err netwox_snmpmd5_compute(netwox_snmpmsg *psnmpmsg,
                                  netwox_snmp3usmsp *psnmp3usmsp,
                                  netwib_constbuf *pku);
/* check it */
netwib_err netwox_snmpmd5_check(netwox_snmpmsg *psnmpmsg,
                                netwox_snmp3usmsp *psnmp3usmsp,
                                netwib_constbuf *pku,
                                netwib_bool *pvalid);


