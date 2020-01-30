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
netwib_conststring t000101_description[] = {
  NETWOX_DESC_brute_title,
  "",
  NETWOX_DESC_brute,
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000101_args[] = {
  NETWOX_SOCK_ARG_TCP_CLIPORT("23"),
  NETWOX_TOOLARG_OPT_BUF_LOGIN('L', "login", "login", NULL),
  NETWOX_TOOLARG_OPTA_BUF_FILE_RD('l', "login-file", "login file",
                                  "loginfile.txt"),
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('w', "password-file", "password file",
                                 "passwordfile.txt"),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in milliseconds",
                             "60000"),
  NETWOX_TOOLARG_OPTA_BOOL('s', "stopatfirst",
                           "stop at first valid login found", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('n', "numthread", "number of threads to launch",
                             "5"),
  NETWOX_TOOLARG_OPTA_BOOL('v', "verbose", "be verbose", "1"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000101_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_TELNET,
  NETWOX_TOOLTREENODETYPE_BRUTEFORCE,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000101_info = {
  "Brute force telnet client",
  t000101_description,
  NULL,
  t000101_args,
  t000101_nodes,
};

/*-------------------------------------------------------------*/
typedef struct {
  netwox_sockinfo sockinfo;
  netwib_uint32 maxwaitms;
} t000101_threadinfo;

/*-------------------------------------------------------------*/
static netwib_err t000101_try(netwib_constbuf *plogin,
                              netwib_constbuf *ppassword,
                              netwib_ptr infos,
                              netwox_brute_result *presult)
{
  t000101_threadinfo *pthreadinfo = (t000101_threadinfo *)infos;
  netwib_io *ptcpio, *ptelnetio;
  netwib_time abstime;
  netwib_err ret;

  *presult = NETWOX_BRUTE_RESULT_BAD;

  /* create a telnet io, and plug it on top of tcpio */
  ret = netwox_sock_init(&pthreadinfo->sockinfo, &ptcpio);
  if (ret != NETWIB_ERR_OK) {
    *presult = NETWOX_BRUTE_RESULT_RETRY;
    return(NETWIB_ERR_OK);
  }
  netwib_er(netwox_telnet_init(NETWIB_TRUE, &ptelnetio));
  netwib_er(netwib_io_plug_rdwr(ptelnetio, ptcpio));
  netwib_er(netwox_telnet_negotiate(ptelnetio,
                                    NETWOX_TELNET_NEGOMODE_CHARACTER));

  /* try to login */
  netwib_er(netwib_time_init_now(&abstime));
  netwib_er(netwib_time_plus_msec(&abstime, pthreadinfo->maxwaitms));
  ret = netwox_telnetses_login(ptelnetio, &abstime, plogin, ppassword);
  if (ret == NETWIB_ERR_OK) {
    *presult = NETWOX_BRUTE_RESULT_GOOD;
    netwib_er(netwox_telnetses_logout(ptelnetio));
  } else if (ret == NETWOX_ERR_TIMEOUT) {
    *presult = NETWOX_BRUTE_RESULT_RETRY;
  } else if (ret == NETWOX_ERR_AUTHERROR) {
    *presult = NETWOX_BRUTE_RESULT_BAD;
  } else {
    *presult = NETWOX_BRUTE_RESULT_RETRY;
  }

  /* close */
  netwib_er(netwib_io_close(&ptelnetio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err t000101_core(int argc, char *argv[])
{
  netwox_arg *parg;
  t000101_threadinfo threadinfo;
  netwib_ring *pringlogin, *pringpassword;
  netwib_buf login, loginfile, passwordfile;
  netwib_bool isset, stopatfirstfound, verbose;
  netwib_uint32 numthread;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000101_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_tcp_cli(parg, &threadinfo.sockinfo));
  netwib_er(netwox_arg_uint32(parg, 'T', &threadinfo.maxwaitms));
  netwib_er(netwox_arg_uint32(parg, 'n', &numthread));
  netwib_er(netwox_arg_bool(parg, 's', &stopatfirstfound));
  netwib_er(netwox_arg_bool(parg, 'v', &verbose));

  /* forbids setting of local port */
  threadinfo.sockinfo.localport = 0;

  /* initialize login and password list */
  netwib_er(netwox_wordlist_init(&pringlogin));
  netwib_er(netwox_wordlist_init(&pringpassword));
  netwib_er(netwox_arg_isset(parg, 'L', &isset));
  if (isset) {
    netwib_er(netwox_arg_buf(parg, 'L', &login));
    netwib_er(netwox_wordlist_add_buf(pringlogin, &login));
  } else {
    netwib_er(netwox_arg_isset(parg, 'l', &isset));
    if (!isset) {
      netwib_er(netwib_fmt_display("One of 'l' or 'L' must be set\n"));
      return(NETWOX_ERR_TOOLARG_INVALID);
    }
    netwib_er(netwox_arg_buf(parg, 'l', &loginfile));
    netwib_er(netwox_wordlist_add_file(pringlogin, &loginfile));
  }
  netwib_er(netwox_arg_buf(parg, 'w', &passwordfile));
  netwib_er(netwox_wordlist_add_file(pringpassword, &passwordfile));

  /* try passwords */
  netwib_er(netwox_brute(pringlogin, pringpassword, &threadinfo, numthread,
                         stopatfirstfound, verbose, &t000101_try));

  /* close */
  netwib_er(netwox_wordlist_close(&pringlogin));
  netwib_er(netwox_wordlist_close(&pringpassword));
  netwib_er(netwox_sockinfo_close(&threadinfo.sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
