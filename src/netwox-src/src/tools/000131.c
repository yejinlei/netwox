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
netwib_conststring t000131_description[] = {
  NETWOX_DESC_brute_title,
  "This tool tries to find password of web server (basic authentication).",
  "",
  NETWOX_DESC_http_uri,
  NETWOX_DESC_http_proxy,
  NETWOX_DESC_http_useragent,
  NETWOX_DESC_brute,
  NETWOX_DESC_client,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000131_args[] = {
  NETWOX_TOOLARG_REQ_BUF_URI('u', "uri", "url/uri to download",
                             "http://server/private/"),
  NETWOX_TOOLARG_OPTA_IP_DST('p', "proxy-ip", "IP address of proxy", NULL),
  NETWOX_TOOLARG_OPTA_PORT_DST('P', "proxy-port", "port of proxy", "3128"),
  NETWOX_TOOLARG_OPTA_BUF_LOGIN('g', "proxy-login", "login of proxy", NULL),
  NETWOX_TOOLARG_OPTA_BUF_PASSWORD('o', "proxy-password", "password of proxy",
                                   NULL),
  NETWOX_TOOLARG_OPTA_BUF_USERAGENT('U', "user-agent", NULL, NULL),
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
netwox_tooltreenodetype t000131_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_HTTP,
  NETWOX_TOOLTREENODETYPE_BRUTEFORCE,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000131_info = {
  "Brute force http client (site password)",
  t000131_description,
  "web",
  t000131_args,
  t000131_nodes,
};

/*-------------------------------------------------------------*/
typedef struct {
  netwox_httpclictx ctx;
  netwib_buf url;
  netwib_uint32 maxwaitms;
} t000131_threadinfo;

/*-------------------------------------------------------------*/
static netwib_err t000131_try(netwib_constbuf *plogin,
                              netwib_constbuf *ppassword,
                              netwib_ptr infos,
                              netwox_brute_result *presult)
{
  t000131_threadinfo *pthreadinfo = (t000131_threadinfo *)infos;
  netwox_httphdr_statuscode statuscode;
  netwib_buf authority, *ppath, query, *pfragment, *purl, *ptmp, *pserverhost;
  netwib_port serverport;
  netwib_bufpool *pbufpool;
  netwib_err ret;

  *presult = NETWOX_BRUTE_RESULT_BAD;

  /* construct url containing login:password */
  pbufpool = pthreadinfo->ctx.pbufpool;
  netwib_er(netwib_bufpool_buf_init(pbufpool, &ppath));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pfragment));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &purl));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &ptmp));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pserverhost));
  netwib_er(netwox_url_decode_http(&pthreadinfo->url, &authority, ppath,
                                   &query, pfragment));
  netwib_er(netwox_url_authority_decode(NETWOX_URL_SCHEME_HTTP,
                                        &authority, NULL, NULL, pserverhost,
                                        &serverport));
  netwib_er(netwox_url_authority_encode(NETWOX_URL_SCHEME_HTTP,
                                        plogin, ppassword, pserverhost, serverport,
                                        ptmp));
  netwib_er(netwox_url_encode_http(ptmp, ppath, &query, pfragment, purl));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &ppath));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pfragment));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &ptmp));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pserverhost));

  /* try */
  ret = netwox_httpcli_get(&pthreadinfo->ctx, purl, &statuscode, NULL, NULL);
  netwib_er(netwib_bufpool_buf_close(pbufpool, &purl));
  if (ret != NETWIB_ERR_OK) {
    *presult = NETWOX_BRUTE_RESULT_RETRY;
    return(NETWIB_ERR_OK);
  }

  switch (statuscode) {
  case NETWOX_HTTPHDR_STATUSCODE_OK :
  case NETWOX_HTTPHDR_STATUSCODE_MOVEDPERMANENTLY :
  case NETWOX_HTTPHDR_STATUSCODE_MOVEDTEMPORARILY :
  case NETWOX_HTTPHDR_STATUSCODE_NOTFOUND : /* file does not exist but login
                                               is ok */
    *presult = NETWOX_BRUTE_RESULT_GOOD;
    break;
  case NETWOX_HTTPHDR_STATUSCODE_UNAUTHORIZED :
  case NETWOX_HTTPHDR_STATUSCODE_FORBIDDEN :
    *presult = NETWOX_BRUTE_RESULT_BAD;
    break;
  default :
    *presult = NETWOX_BRUTE_RESULT_RETRY;
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err t000131_core(int argc, char *argv[])
{
  netwox_arg *parg;
  t000131_threadinfo threadinfo;
  netwib_ring *pringlogin, *pringpassword;
  netwib_buf login, loginfile, passwordfile, buf;
  netwib_bool isset, stopatfirstfound, verbose;
  netwib_uint32 numthread;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000131_info, &parg));
  netwib_er(netwox_httpclictx_init(&threadinfo.ctx));
  /* url */
  netwib_er(netwox_arg_buf(parg, 'u', &buf));
  netwib_er(netwib_buf_init_mallocdefault(&threadinfo.url));
  netwib_er(netwib_buf_append_buf(&buf, &threadinfo.url));
  /* proxy */
  netwib_er(netwox_arg_port(parg, 'P', &threadinfo.ctx.proxy_port));
  netwib_er(netwox_arg_isset(parg, 'p', &isset));
  if (isset) {
    threadinfo.ctx.useproxy = NETWIB_TRUE;
    netwib_er(netwox_arg_ip(parg, 'p', &threadinfo.ctx.proxy_ip));
  }
  netwib_er(netwox_arg_isset(parg, 'g', &isset));
  if (isset) {
    netwib_er(netwox_arg_buf(parg, 'g', &buf));
    netwib_er(netwib_buf_append_buf(&buf, &threadinfo.ctx.proxy_login));
  }
  netwib_er(netwox_arg_isset(parg, 'o', &isset));
  if (isset) {
    netwib_er(netwox_arg_buf(parg, 'o', &buf));
    netwib_er(netwib_buf_append_buf(&buf, &threadinfo.ctx.proxy_password));
  }
  /* user agent */
  netwib_er(netwox_arg_isset(parg, 'U', &isset));
  if (isset) {
    netwib_er(netwox_arg_buf(parg, 'U', &buf));
    netwib_er(netwib_buf_append_buf(&buf, &threadinfo.ctx.useragent));
  }
  /* other */
  netwib_er(netwox_arg_uint32(parg, 'T', &threadinfo.ctx.timeoutms));
  netwib_er(netwox_arg_uint32(parg, 'n', &numthread));
  netwib_er(netwox_arg_bool(parg, 's', &stopatfirstfound));
  netwib_er(netwox_arg_bool(parg, 'v', &verbose));

  /* initialize login and password list */
  netwib_er(netwox_wordlist_init(&pringlogin));
  netwib_er(netwox_wordlist_init(&pringpassword));
  netwib_er(netwox_arg_isset(parg, 'L', &isset));
  if (isset) {
    netwib_er(netwox_arg_buf(parg, 'L', &login));
    login.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
    login.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
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
                         stopatfirstfound, verbose, &t000131_try));

  /* close */
  netwib_er(netwox_wordlist_close(&pringlogin));
  netwib_er(netwox_wordlist_close(&pringpassword));
  netwib_er(netwox_httpclictx_close(&threadinfo.ctx));
  netwib_er(netwib_buf_close(&threadinfo.url));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
