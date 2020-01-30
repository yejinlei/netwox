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
netwib_err netwox_winauth_ntlmv1_hash(netwib_constbuf *ppassword,
                                      netwib_buf *phash)
{
  netwib_byte passarray[28];
  netwib_buf passworda, passwordu;

  /* store unicode password */
  passworda = *ppassword;
  if (netwib__buf_ref_data_size(&passworda) > 14) {
    passworda.endoffset = passworda.beginoffset + 14;
  }
  netwib_er(netwib_buf_init_ext_arraysizeofempty(passarray, &passwordu));
  netwib_er(netwox_buf_decode_ucs2le(&passworda, &passwordu));

  /* compute MD4 */
  netwib_er(netwox_md4_compute(&passwordu, phash));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_winauth_ntlmv1_hash_test(netwib_conststring txtpassword,
                                                       netwib_conststring mixedhashwanted)
{
  netwib_buf hash, password;

  netwib_er(netwib_buf_init_ext_string(txtpassword, &password));
  netwib_er(netwib_buf_init_mallocdefault(&hash));
  netwib_er(netwox_winauth_ntlmv1_hash(&password, &hash));
  netwib_er(netwox_buf_check_strmixed(&hash, mixedhashwanted));
  netwib_er(netwib_buf_close(&hash));

  return(NETWIB_ERR_OK);
}
netwib_err netwox_winauth_ntlmv1_hash_test(void)
{
  /* test cases found in two documents on the web */
  netwib_er(netwox_priv_winauth_ntlmv1_hash_test("123456", "32ED87BDB5FDC5E9CBA88547376818D4"));
  netwib_er(netwox_priv_winauth_ntlmv1_hash_test("SecREt01", "cd06ca7c7e10c99b1d33b7485a2ed808"));

  return(NETWIB_ERR_OK);
}
