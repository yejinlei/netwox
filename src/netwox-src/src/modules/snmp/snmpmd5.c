/*
                                  NETWOX
                             Network toolbox
                Copyright(c) 1999-2012 Laurent CONSTANTIN
                      http://ntwox.sourceforge.net/
                        laurentconstantin@free.fr
                                  -----

  This file is part of Netwox.

  Netwox is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3
  as published by the Free Software Foundation.

  Netwox is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details (http://www.gnu.org/).

------------------------------------------------------------------------
*/

/*-------------------------------------------------------------*/
#include "../../netwox.h"

/*-------------------------------------------------------------*/
netwib_err netwox_snmpmd5_ku(netwib_constbuf *ppassword,
                             netwib_buf *pku)
{
  netwib_buf password;
  netwox_md_context ctx;
  netwib_uint32 passwordsize, sizetodo;

  passwordsize = netwib__buf_ref_data_size(ppassword);
  if (passwordsize == 0) {
    return(NETWIB_ERR_PATOOLOW);
  }
  password = *ppassword;

  netwib_er(netwox_md5_init(&ctx));
  sizetodo = 1048576;
  while (sizetodo >= passwordsize) {
    netwib_er(netwox_md_update(&ctx, &password));
    sizetodo -= passwordsize;
  }
  if (sizetodo) {
    password.endoffset = password.beginoffset + sizetodo;
    netwib_er(netwox_md_update(&ctx, &password));
  }

  netwib_er(netwox_md_final(&ctx, pku));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmpmd5_kul(netwib_bufpool *pbufpool,
                              netwib_constbuf *pku,
                              netwib_constbuf *pengineid,
                              netwib_buf *pkul)
{
  netwib_buf *pbuf;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));

  netwib_er(netwib_buf_append_buf(pku, pbuf));
  netwib_er(netwib_buf_append_buf(pengineid, pbuf));
  netwib_er(netwib_buf_append_buf(pku, pbuf));

  netwib_er(netwox_md5_compute(pbuf, pkul));

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmpmd5_compute(netwox_snmpmsg *psnmpmsg,
                                  netwox_snmp3usmsp *psnmp3usmsp,
                                  netwib_constbuf *pku)
{
  netwox_asn1data asn1data;
  netwib_buf *pkul, *pk1, *pk2, *ppad;
  netwib_err ret=NETWIB_ERR_OK;

  if (psnmpmsg->version != NETWOX_SNMPVERSION_3) {
    return(NETWIB_ERR_PAINVALIDTYPE);
  }
  if (psnmpmsg->v.v3.globalheader.securitymodel !=
      NETWOX_SNMP3_SECURITYMODEL_USM) {
    return(NETWIB_ERR_PAINVALIDTYPE);
  }
  if ( ! (psnmpmsg->v.v3.globalheader.flags & NETWOX_SNMP3_FLAGS_AUTH)) {
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  netwib_er(netwib_bufpool_buf_init(psnmpmsg->pbufpool, &pkul));
  netwib_er(netwib_bufpool_buf_init(psnmpmsg->pbufpool, &pk1));
  netwib_er(netwib_bufpool_buf_init(psnmpmsg->pbufpool, &pk2));
  netwib_er(netwib_bufpool_buf_init(psnmpmsg->pbufpool, &ppad));
  netwib_er(netwox_asn1data_init(psnmpmsg->pbufpool, &asn1data));

  /* compute kul, K1 and K2 */
  netwib_er(netwox_snmpmd5_kul(psnmpmsg->pbufpool, pku,
                               psnmp3usmsp->pauthoritativeengineid, pkul));
  netwib_er(netwox_buf_append_pad(0, 48, pkul));
  netwib_er(netwox_buf_append_pad(0x36, 64, ppad));
  netwib_er(netwox_buf_append_xor(pkul, ppad, pk1));
  netwib__buf_reinit(ppad);
  netwib_er(netwox_buf_append_pad(0x5C, 64, ppad));
  netwib_er(netwox_buf_append_xor(pkul, ppad, pk2));

  /* append "packet with authpara of '\0'x12" to K1 */
  netwib__buf_reinit(psnmpmsg->v.v3.psecurityparameters);
  netwib__buf_reinit(psnmp3usmsp->pauthenticationparameters);
  netwib_er(netwox_buf_append_pad(0, 12,
                                  psnmp3usmsp->pauthenticationparameters));
  netwib_eg(netwox_asn1data_append_snmp3usmsp(psnmp3usmsp, &asn1data));
  netwib_eg(netwox_pktber_append_asn1data(&asn1data,
                                          psnmpmsg->v.v3.psecurityparameters));
  netwib_er(netwox_asn1data_close(&asn1data));
  netwib_er(netwox_asn1data_init(psnmpmsg->pbufpool, &asn1data));
  netwib_eg(netwox_asn1data_append_snmpmsg(psnmpmsg, &asn1data));
  netwib_eg(netwox_pktber_append_asn1data(&asn1data, pk1));

  /* append "md5(K1+packet)" to K2 */
  netwib_er(netwox_md5_compute(pk1, pk2));

  /* md5(K2+previousmd5) truncated to 12 bytes */
  netwib__buf_reinit(psnmp3usmsp->pauthenticationparameters);
  netwib_er(netwox_md5_compute(pk2, psnmp3usmsp->pauthenticationparameters));
  psnmp3usmsp->pauthenticationparameters->endoffset = 12;

  /* update psecurityparameters */
  netwib__buf_reinit(psnmpmsg->v.v3.psecurityparameters);
  netwib_er(netwox_asn1data_close(&asn1data));
  netwib_er(netwox_asn1data_init(psnmpmsg->pbufpool, &asn1data));
  netwib_eg(netwox_asn1data_append_snmp3usmsp(psnmp3usmsp, &asn1data));
  netwib_eg(netwox_pktber_append_asn1data(&asn1data,
                                          psnmpmsg->v.v3.psecurityparameters));

 netwib_gotolabel:
  netwib_er(netwox_asn1data_close(&asn1data));
  netwib_er(netwib_bufpool_buf_close(psnmpmsg->pbufpool, &pk1));
  netwib_er(netwib_bufpool_buf_close(psnmpmsg->pbufpool, &pk2));
  netwib_er(netwib_bufpool_buf_close(psnmpmsg->pbufpool, &ppad));
  netwib_er(netwib_bufpool_buf_close(psnmpmsg->pbufpool, &pkul));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmpmd5_check(netwox_snmpmsg *psnmpmsg,
                                netwox_snmp3usmsp *psnmp3usmsp,
                                netwib_constbuf *pku,
                                netwib_bool *pvalid)
{
  netwib_buf *psecurityparameters, *pauthenticationparameters;
  netwib_cmp cmp;
  netwib_err ret;

  if (pvalid != NULL) *pvalid = NETWIB_FALSE;

  if (netwib__buf_ref_data_size(psnmp3usmsp->pauthenticationparameters) != 12){
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_bufpool_buf_init(psnmpmsg->pbufpool, &psecurityparameters));
  netwib_er(netwib_bufpool_buf_init(psnmpmsg->pbufpool,
                                    &pauthenticationparameters));
  netwib_er(netwib_buf_append_buf(psnmpmsg->v.v3.psecurityparameters,
                                  psecurityparameters));
  netwib_er(netwib_buf_append_buf(psnmp3usmsp->pauthenticationparameters,
                                  pauthenticationparameters));

  ret = netwox_snmpmd5_compute(psnmpmsg, psnmp3usmsp, pku);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_cmp(pauthenticationparameters,
                             psnmp3usmsp->pauthenticationparameters, &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      if (pvalid != NULL) *pvalid = NETWIB_TRUE;
    }
  }

  netwib__buf_reinit(psnmpmsg->v.v3.psecurityparameters);
  netwib__buf_reinit(psnmp3usmsp->pauthenticationparameters);
  netwib_er(netwib_buf_append_buf(psecurityparameters,
                                  psnmpmsg->v.v3.psecurityparameters));
  netwib_er(netwib_buf_append_buf(pauthenticationparameters,
                                  psnmp3usmsp->pauthenticationparameters));
  netwib_er(netwib_bufpool_buf_close(psnmpmsg->pbufpool,
                                     &psecurityparameters));
  netwib_er(netwib_bufpool_buf_close(psnmpmsg->pbufpool,
                                     &pauthenticationparameters));

  return(NETWIB_ERR_OK);
}

