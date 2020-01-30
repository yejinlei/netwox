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
netwib_err netwox_winauth_lm_hash(netwib_constbuf *ppassword,
                                  netwib_buf *phash)
{
  netwib_byte passarray[14];
  netwib_data data;
  netwib_uint32 datasize, i;
  netwib_buf key, lmstring;
  netwox_des_context context;

  /* store password and pad it */
  datasize = netwib__buf_ref_data_size(ppassword);
  if (datasize > 14) {
    datasize = 14;
  }
  data = netwib__buf_ref_data_ptr(ppassword);
  for (i = 0; i < datasize; i++) {
    passarray[i] = (netwib_byte)netwib_c2_uc(*data);
    data++;
  }
  netwib_c_memset(passarray+datasize, 0, 14-datasize);

  /* compute DES */
  /* 0x4b47532140232425 == "KGS!@#$%" is a constant hardcoded in Windows */
  netwib_er(netwib_buf_init_ext_string("KGS!@#$%", &lmstring));
  netwib_er(netwib_buf_init_ext_arrayfilled(passarray, 7, &key));
  netwib_er(netwox_des_context_init7(&key, NETWIB_FALSE, &context));
  netwib_er(netwox_des_encrypt(&context, &lmstring, phash));
  netwib_er(netwib_buf_init_ext_arrayfilled(passarray+7, 7, &key));
  netwib_er(netwox_des_context_init7(&key, NETWIB_FALSE, &context));
  netwib_er(netwox_des_encrypt(&context, &lmstring, phash));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_winauth_lm_hash_test(netwib_conststring txtpassword,
                                                   netwib_conststring mixedhashwanted)
{
  netwib_buf hash, password;

  netwib_er(netwib_buf_init_ext_string(txtpassword, &password));
  netwib_er(netwib_buf_init_mallocdefault(&hash));
  netwib_er(netwox_winauth_lm_hash(&password, &hash));
  netwib_er(netwox_buf_check_strmixed(&hash, mixedhashwanted));
  netwib_er(netwib_buf_close(&hash));

  return(NETWIB_ERR_OK);
}
netwib_err netwox_winauth_lm_hash_test(void)
{
  /* test cases found in various documents on the web */
  netwib_er(netwox_priv_winauth_lm_hash_test("WELCOME", "c23413a8a1e7665faad3b435b51404ee"));
  netwib_er(netwox_priv_winauth_lm_hash_test("123456", "44efce164ab921caaad3b435b51404ee"));
  netwib_er(netwox_priv_winauth_lm_hash_test("SecREt01", "ff3750bcc2b22412c2265b23734e0dac"));
  netwib_er(netwox_priv_winauth_lm_hash_test("aa", "89d42a44e77140aaaad3b435b51404ee"));
  netwib_er(netwox_priv_winauth_lm_hash_test("aaa", "1c3a2b6d939a1021aad3b435b51404ee"));
  netwib_er(netwox_priv_winauth_lm_hash_test("aaaa", "dcf9caa6dbc2f2dfaad3b435b51404ee"));
  netwib_er(netwox_priv_winauth_lm_hash_test("aaaaa", "a80f6e6a87ba6ac2aad3b435b51404ee"));
  netwib_er(netwox_priv_winauth_lm_hash_test("aaaaaa", "a9c604d244c4e99daad3b435b51404ee"));
  netwib_er(netwox_priv_winauth_lm_hash_test("aaaaaaa", "cbc501a4d2227783aad3b435b51404ee"));
  netwib_er(netwox_priv_winauth_lm_hash_test("aaaaaaaa", "cbc501a4d22277837584248b8d2c9f9e"));
  netwib_er(netwox_priv_winauth_lm_hash_test("z", "1d91a081d4b37861aad3b435b51404ee"));
  netwib_er(netwox_priv_winauth_lm_hash_test("zz", "e1c862b468d1c6d6aad3b435b51404ee"));
  netwib_er(netwox_priv_winauth_lm_hash_test("zzz", "cf78d4607d48813aaad3b435b51404ee"));
  netwib_er(netwox_priv_winauth_lm_hash_test("zzzz", "ce045ff77f9d89e2aad3b435b51404ee"));
  netwib_er(netwox_priv_winauth_lm_hash_test("zzzzz", "9a0a242740e1e245aad3b435b51404ee"));
  netwib_er(netwox_priv_winauth_lm_hash_test("zzzzzz", "1fb363feb834c12daad3b435b51404ee"));
  netwib_er(netwox_priv_winauth_lm_hash_test("zzzzzzz", "a5e6066de61c3e35aad3b435b51404ee"));
  netwib_er(netwox_priv_winauth_lm_hash_test("zzzzzzzz", "a5e6066de61c3e351d91a081d4b37861"));
  netwib_er(netwox_priv_winauth_lm_hash_test("zzzzzzzzz", "a5e6066de61c3e35e1c862b468d1c6d6"));
  netwib_er(netwox_priv_winauth_lm_hash_test("m", "1486235a2333e4d2aad3b435b51404ee"));
  netwib_er(netwox_priv_winauth_lm_hash_test("mm", "fbf69fcd34281a77aad3b435b51404ee"));
  netwib_er(netwox_priv_winauth_lm_hash_test("mmm", "8f3e3862ebbccc49aad3b435b51404ee"));
  netwib_er(netwox_priv_winauth_lm_hash_test("mmmm", "4983aa449f48595caad3b435b51404ee"));
  netwib_er(netwox_priv_winauth_lm_hash_test("mmmmm", "d5c546473739d83aaad3b435b51404ee"));
  netwib_er(netwox_priv_winauth_lm_hash_test("mmmmmm", "81a28589262a0004aad3b435b51404ee"));
  netwib_er(netwox_priv_winauth_lm_hash_test("mmmmmmm", "f6f2fa9986d540cfaad3b435b51404ee"));
  netwib_er(netwox_priv_winauth_lm_hash_test("mmmmmmmm", "f6f2fa9986d540cf1486235a2333e4d2"));
  netwib_er(netwox_priv_winauth_lm_hash_test("mmmmmmmmm", "f6f2fa9986d540cffbf69fcd34281a77"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_winauth_lm_chal_gene(netwib_buf *pchal)
{
  netwib_er(netwib_buf_append_rand_all(8, pchal));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_winauth_lm_chal_answer(netwib_constbuf *pchal,
                                         netwib_constbuf *phash,
                                         netwib_buf *panswer)
{
  netwib_byte array[8];
  netwib_buf key;
  netwib_data data;
  netwox_des_context context;

  if (netwib__buf_ref_data_size(pchal) != NETWOX_WINAUTH_LM_CHAL_GENE_LEN) {
    return(NETWOX_ERR_BADSIZE);
  }
  if (netwib__buf_ref_data_size(phash) != NETWOX_WINAUTH_LM_HASH_LEN) {
    return(NETWOX_ERR_BADSIZE);
  }

  data = netwib__buf_ref_data_ptr(phash);
  netwib_er(netwib_buf_init_ext_arrayfilled(array, 7, &key));

  netwib_c_memcpy(array, data, 7);
  netwib_er(netwox_des_context_init7(&key, NETWIB_FALSE, &context));
  netwib_er(netwox_des_encrypt(&context, pchal, panswer));

  netwib_c_memcpy(array, data+7, 7);
  netwib_er(netwox_des_context_init7(&key, NETWIB_FALSE, &context));
  netwib_er(netwox_des_encrypt(&context, pchal, panswer));

  netwib_c_memcpy(array, data+14, 2);
  netwib_c_memset(array+2, 0, 5);
  netwib_er(netwox_des_context_init7(&key, NETWIB_FALSE, &context));
  netwib_er(netwox_des_encrypt(&context, pchal, panswer));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_winauth_lm_chal_answer_test(netwib_conststring mixedchal,
                                          netwib_conststring mixedhash,
                                          netwib_conststring mixedanswerwanted)
{
  netwib_buf bufchal, bufhash, bufanswer;

  netwib_er(netwib_buf_init_mallocdefault(&bufchal));
  netwib_er(netwox_buf_append_strmixed(mixedchal, &bufchal));
  netwib_er(netwib_buf_init_mallocdefault(&bufhash));
  netwib_er(netwox_buf_append_strmixed(mixedhash, &bufhash));

  netwib_er(netwib_buf_init_mallocdefault(&bufanswer));
  netwib_er(netwox_winauth_lm_chal_answer(&bufchal, &bufhash, &bufanswer));
  netwib_er(netwox_buf_check_strmixed(&bufanswer, mixedanswerwanted));

  netwib_er(netwib_buf_close(&bufanswer));
  netwib_er(netwib_buf_close(&bufhash));
  netwib_er(netwib_buf_close(&bufchal));

  return(NETWIB_ERR_OK);
}
netwib_err netwox_winauth_lm_chal_answer_test(void)
{

  /* test suite taken from two documents on the web */
  netwib_er(netwox_priv_winauth_lm_chal_answer_test("46b0f8ab00335086", "44efce164ab921caaad3b435b51404ee", "154e0de15a54bc54008d80188e37fd9c8cde93e6961c6915"));
  netwib_er(netwox_priv_winauth_lm_chal_answer_test("46B0F8AB00335086", "32ED87BDB5FDC5E9CBA88547376818D4", "D611B7C5DBE8EA849FDBBAA60A956E95078D16F0B906D509"));
  netwib_er(netwox_priv_winauth_lm_chal_answer_test("0123456789abcdef", "ff3750bcc2b22412c2265b23734e0dac", "c337cd5cbd44fc9782a667af6d427c6de67c20c2d3e77c56"));

  return(NETWIB_ERR_OK);
}
