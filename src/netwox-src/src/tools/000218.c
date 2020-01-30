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
#include "../netwox.h"

/*-------------------------------------------------------------*/
netwib_conststring t000218_description[] = {
  "This tool is very special. It runs internal test suites of netwox.",
  "",
  "It is only useful for netwox developers. It does not test every",
  "module, but only those with independent test cases.",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000218_args[] = {
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000218_nodes[] = {
  NETWOX_TOOLTREENODETYPE_INFO_NETW,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000218_info = {
  "Netwox internal validation suite",
  t000218_description,
  NULL,
  t000218_args,
  t000218_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000218_title(netwib_conststring title)
{
  netwib_er(netwib_fmt_display("##########################################\nTesting %s module...\n", title));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err t000218_core(int argc, char *argv[])
{
  netwox_arg *parg;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000218_info, &parg));

  /* run tests */
  netwib_er(t000218_title("asn1/asn1pktber"));
  netwib_er(netwox_pktber_test_static());
  netwib_er(t000218_title("crypto/des"));
  netwib_er(netwox_des_test());
  netwib_er(t000218_title("crypto/hmac"));
  netwib_er(netwox_hmac_test());
  netwib_er(t000218_title("crypto/md"));
  netwib_er(netwox_md_test());
  netwib_er(t000218_title("crypto/md2"));
  netwib_er(netwox_md2_test());
  netwib_er(t000218_title("crypto/ripemd"));
  netwib_er(netwox_ripemd_test());
  netwib_er(t000218_title("crypto/sha1"));
  netwib_er(netwox_sha1_test());
  netwib_er(t000218_title("crypto/sha224256"));
  netwib_er(netwox_sha224256_test());
  netwib_er(t000218_title("crypto/sha384512"));
  netwib_er(netwox_sha384512_test());
  netwib_er(t000218_title("dhcp4/dhcp4hdr"));
  netwib_er(netwox_dhcp4hdr_test());
  netwib_er(t000218_title("mimetypes/mimetypes"));
  netwib_er(netwox_mimetypes_test());
  netwib_er(t000218_title("ntp/ntphdr"));
  netwib_er(netwox_ntphdr_test());
  netwib_er(t000218_title("rules/rulesfile"));
  netwib_er(netwox_rulesfile_example());
  netwib_er(t000218_title("rules/rules"));
  netwib_er(netwox_rulesre_example());
  netwib_er(t000218_title("smb/smbser83"));
  netwib_er(netwox_smbser83_test());
  netwib_er(t000218_title("smb/smbsercase"));
  netwib_er(netwox_smbsercase_test());
  netwib_er(t000218_title("smb/smbserglob"));
  netwib_er(netwox_smbserglob_re_test());
  netwib_er(t000218_title("smb/winauthlm"));
  netwib_er(netwox_winauth_lm_hash_test());
  netwib_er(netwox_winauth_lm_chal_answer_test());
  netwib_er(t000218_title("smb/winauthntlmv1"));
  netwib_er(netwox_winauth_ntlmv1_hash_test());
  netwib_er(t000218_title("smb/winauthntlmv2"));
  netwib_er(netwox_winauth_ntlmv2_chal_answer_test());
  netwib_er(t000218_title("snmp/snmppkt"));
  netwib_er(netwox_snmppkt_test_static());
  netwib_er(t000218_title("time"));
  netwib_er(netwox_time1601_test());
  netwib_er(netwox_time1970l_test());
  netwib_er(netwox_timemsdos_test());
  netwib_er(t000218_title("tracertdisc"));
  {
    netwox_tracertdisc tracertdisc;
    tracertdisc.addinfo.resolve = NETWIB_FALSE;
    tracertdisc.verbose = NETWIB_TRUE;
    netwib_er(netwox_tracertdisc_graph_test(&tracertdisc));
  }
  netwib_er(t000218_title("url/urllocal"));
  netwib_er(netwox_urllocal_test());

  netwib_er(netwib_fmt_display("#########################################\n"));
  netwib_er(netwib_fmt_display("Normal end of tests.\n"));

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
