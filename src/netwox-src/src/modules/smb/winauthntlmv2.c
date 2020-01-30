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
netwib_err netwox_winauth_ntlmv2_blob_gene(netwib_constbuf *pservername,
                                           netwib_constbuf *pdomain,
                                           netwib_buf *pblob)
{
  netwib_time t;
  netwox_time1601 t1601;
  netwib_data data;

  /* signature */
  netwib_er(netwib_buf_wantspace(pblob, 16, &data));
  netwib__data_append_uint32(data, 0x01010000);

  /* reserved */
  netwib__data_append_uint32(data, 0);

  /* timestamp */
  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwox_time1601_init_time(&t, &t1601));
  netwib__data_append_uint64(data, t1601);
  pblob->endoffset += 16;

  /* client challenge */
  netwib_er(netwib_buf_append_rand_all(8, pblob));

  /* unknown */
  netwib_er(netwib_buf_wantspace(pblob, 4, &data));
  netwib__data_append_uint32(data, 0);
  pblob->endoffset += 4;

  /* target information */
  if (pservername != NULL) {
    netwib_er(netwib_buf_wantspace(pblob, 4, &data));
    netwib__data_append_uint16_le(data, 0x0001);
    netwib__data_append_uint16_le(data,
                                  netwib__buf_ref_data_size(pservername));
    pblob->endoffset += 4;
    netwib_er(netwox_buf_decode_ucs2le_uc(pservername, pblob));
  }
  if (pdomain != NULL) {
    netwib_er(netwib_buf_wantspace(pblob, 4, &data));
    netwib__data_append_uint16_le(data, 0x0002);
    netwib__data_append_uint16_le(data, netwib__buf_ref_data_size(pdomain));
    pblob->endoffset += 4;
    netwib_er(netwox_buf_decode_ucs2le_uc(pdomain, pblob));
  }
  netwib_er(netwib_buf_wantspace(pblob, 4, &data));
  netwib__data_append_uint16_le(data, 0x0000);
  netwib__data_append_uint16_le(data, 0x0000);
  pblob->endoffset += 4;

  /* unknown */
  netwib_er(netwib_buf_wantspace(pblob, 4, &data));
  netwib__data_append_uint32(data, 0);
  pblob->endoffset += 4;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_winauth_ntlmv2_blob_get(netwib_constbuf *pntlmv2answer,
                                          netwib_bufext *pblob)
{
  if (netwib__buf_ref_data_size(pntlmv2answer) != 16+24) {
    return(NETWIB_ERR_PATOOLOW);
  }

  netwib_er(netwib_buf_init_ext_buf(pntlmv2answer, pblob));
  pblob->beginoffset += 16;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_winauth_ntlmv2_chal_answer(netwib_constbuf *pchal,
                                             netwib_constbuf *pusername,
                                             netwib_constbuf *ppassword,
                                             netwib_constbuf *ptarget,
                                             netwib_constbuf *pblob,
                                             netwib_buf *pntlmv2answer,
                                             netwib_buf *plmv2answer)
{
  netwib_byte ntlmv1hasharray[NETWOX_WINAUTH_NTLMV1_HASH_LEN];
  netwib_byte hmacmdarray[NETWOX_HMAC_HASH_MAXLEN];
  netwib_buf ntlmv1hash, hmacmd;
  netwib_buf tmpbuf;
  netwox_hmac_keys keys;
  netwib_data clientchal;
  netwib_uint32 clientchalsize;

  /* alloc */
  netwib_er(netwib_buf_init_mallocdefault(&tmpbuf));

  /* extract client challenge from blob */
  if (netwib__buf_ref_data_size(pblob) < 24) {
    return(NETWIB_ERR_PATOOLOW);
  }
  clientchal = netwib__buf_ref_data_ptr(pblob) + 16;
  clientchalsize = 8;

  /* first, compute NTLMv1 hash */
  netwib_er(netwib_buf_init_ext_arraysizeofempty(ntlmv1hasharray,
                                                 &ntlmv1hash));
  netwib_er(netwox_winauth_ntlmv1_hash(ppassword, &ntlmv1hash));

  /* concatenate uppercase UNICODE username and target */
  netwib_er(netwox_buf_decode_ucs2le_uc(pusername, &tmpbuf));
  netwib_er(netwox_buf_decode_ucs2le_uc(ptarget, &tmpbuf));

  /* compute first HMAC-MD5 */
  netwib_er(netwox_hmac_keys_init(NETWOX_CRYPTOHASH_TYPE_MD5, &ntlmv1hash,
                                  &keys));
  netwib_er(netwib_buf_init_ext_arraysizeofempty(hmacmdarray, &hmacmd));
  netwib_er(netwox_hmac_compute(&keys, &tmpbuf, &hmacmd));

  /* create second data to HMAC-MD5-ize */
  netwib__buf_reinit(&tmpbuf);
  netwib_er(netwib_buf_append_buf(pchal, &tmpbuf));
  netwib_er(netwib_buf_append_buf(pblob, &tmpbuf));

  /* compute second HMAC-MD5 */
  netwib_er(netwox_hmac_keys_init(NETWOX_CRYPTOHASH_TYPE_MD5, &hmacmd, &keys));
  netwib_er(netwox_hmac_compute(&keys, &tmpbuf, pntlmv2answer));

  /* append blob */
  netwib_er(netwib_buf_append_buf(pblob, pntlmv2answer));

  /* now, we compute the LMv2, which is only 24 bytes */
  netwib__buf_reinit(&tmpbuf);
  netwib_er(netwib_buf_append_buf(pchal, &tmpbuf));
  netwib_er(netwib_buf_append_data(clientchal, clientchalsize, &tmpbuf));
  netwib_er(netwox_hmac_keys_init(NETWOX_CRYPTOHASH_TYPE_MD5, &hmacmd, &keys));
  netwib_er(netwox_hmac_compute(&keys, &tmpbuf, plmv2answer));
  netwib_er(netwib_buf_append_data(clientchal, clientchalsize, plmv2answer));

  /* close */
  netwib_er(netwib_buf_close(&tmpbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_winauth_ntlmv2_chal_answer_test(netwib_conststring mixedchal,
                                  netwib_conststring mixedpassword,
                                  netwib_conststring mixedusername,
                                  netwib_conststring mixedtarget,
                                  netwib_conststring mixedblob,
                                  netwib_conststring mixedntlmv2answerwanted,
                                  netwib_conststring mixedlmv2answerwanted)
{
  netwib_buf chal, password, username, target, blob;
  netwib_buf ntlmv2answer, lmv2answer;

  netwib_er(netwib_buf_init_mallocdefault(&chal));
  netwib_er(netwib_buf_init_mallocdefault(&password));
  netwib_er(netwib_buf_init_mallocdefault(&username));
  netwib_er(netwib_buf_init_mallocdefault(&target));
  netwib_er(netwib_buf_init_mallocdefault(&blob));
  netwib_er(netwib_buf_init_mallocdefault(&ntlmv2answer));
  netwib_er(netwib_buf_init_mallocdefault(&lmv2answer));

  netwib_er(netwox_buf_append_strmixed(mixedchal, &chal));
  netwib_er(netwox_buf_append_strmixed(mixedpassword, &password));
  netwib_er(netwox_buf_append_strmixed(mixedusername, &username));
  netwib_er(netwox_buf_append_strmixed(mixedtarget, &target));
  netwib_er(netwox_buf_append_strmixed(mixedblob, &blob));

  netwib_er(netwox_winauth_ntlmv2_chal_answer(&chal, &password, &username,
                                              &target, &blob,
                                              &ntlmv2answer, &lmv2answer));
  netwib_er(netwox_buf_check_strmixed(&ntlmv2answer, mixedntlmv2answerwanted));
  netwib_er(netwox_buf_check_strmixed(&lmv2answer, mixedlmv2answerwanted));

  netwib_er(netwib_buf_close(&lmv2answer));
  netwib_er(netwib_buf_close(&ntlmv2answer));
  netwib_er(netwib_buf_close(&blob));
  netwib_er(netwib_buf_close(&target));
  netwib_er(netwib_buf_close(&username));
  netwib_er(netwib_buf_close(&password));
  netwib_er(netwib_buf_close(&chal));

  return(NETWIB_ERR_OK);
}
netwib_err netwox_winauth_ntlmv2_chal_answer_test(void)
{
  /* test case found in http://davenport.sourceforge.net/ntlm.html */
  netwib_er(netwox_priv_winauth_ntlmv2_chal_answer_test("0123456789abcdef",
                                                        "'user'", "'SecREt01'",
                                                        "'DOMAIN'",
                                                        "01010000000000000090d336b734c301ffffff00112233440000000002000c0044004f004d00410049004e0001000c005300450052005600450052000400140064006f006d00610069006e002e0063006f006d00030022007300650072007600650072002e0064006f006d00610069006e002e0063006f006d000000000000000000",
                                                        "cbabbca713eb795d04c97abc01ee498301010000000000000090d336b734c301ffffff00112233440000000002000c0044004f004d00410049004e0001000c005300450052005600450052000400140064006f006d00610069006e002e0063006f006d00030022007300650072007600650072002e0064006f006d00610069006e002e0063006f006d000000000000000000",
                                                        "d6e6152ea25d03b7c6ba6629c2d6aaf0ffffff0011223344"));

  return(NETWIB_ERR_OK);
}


