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
netwib_conststring t000120_description[] = {
  "This tool sends a HTTP request using method POST. HTTP protocol",
  "defines POST method to retrieve a document, giving some more",
  "parameters. For example, 'http://server/index.s?v=a1&v=2' can be",
  "converted to a POST request with '--uri http://server/index.s' and",
  "'--parameters v=a1&v=2'.",
  "Please note & is a special shell character, so it has to be escaped.",
  "",
  NETWOX_DESC_http_uri,
  NETWOX_DESC_http_proxy,
  NETWOX_DESC_http_useragent,
  NETWOX_DESC_http_status,
  NETWOX_DESC_http_headers,
  NETWOX_DESC_http_body,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000120_args[] = {
  NETWOX_TOOLARG_REQ_BUF_URI('u', "uri", "url/uri to download", NULL),
  NETWOX_TOOLARG_OPT_BUF('a', "parameters", "parameters", NULL),
  NETWOX_TOOLARG_OPTA_IP_DST('p', "proxy-ip", "IP address of proxy",
                            NULL),
  NETWOX_TOOLARG_OPTA_PORT_DST('P', "proxy-port", "port of proxy",
                              "3128"),
  NETWOX_TOOLARG_OPTA_BUF_LOGIN('l', "proxy-login", "login of proxy", NULL),
  NETWOX_TOOLARG_OPTA_BUF_PASSWORD('L', "proxy-password", "password of proxy",
                                   NULL),
  NETWOX_TOOLARG_OPTA_BUF_USERAGENT('U', "user-agent", NULL, NULL),
  NETWOX_TOOLARG_OPT_BOOL('s', "display-status",
                          "display statuscode to screen", NULL),
  NETWOX_TOOLARG_OPT_BOOL('h', "display-headers", "display headers to screen",
                          NULL),
  NETWOX_TOOLARG_OPTA_BUF_FILE_WR('H', "file-headers",
                                  "file receiving headers", "filehdr.txt"),
  NETWOX_TOOLARG_OPT_BOOL('b', "display-body", "display body to screen",
                          NULL),
  NETWOX_TOOLARG_OPTA_BUF_FILE_WR('B', "file-body", "file receiving body",
                                  "filebody.txt"),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in ms", "60000"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000120_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_HTTP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000120_info = {
  "HTTP POST",
  t000120_description,
  "web",
  t000120_args,
  t000120_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000120_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_httphdr_statuscode statuscode;
  netwox_httpclictx ctx;
  netwib_buf buf, url, parameters;
  netwib_io *phdrio, *pbodyio;
  netwib_bool isset, displaystatus, displayhdr, displaybody;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000120_info, &parg));
  netwib_er(netwox_httpclictx_init(&ctx));
  /* url */
  netwib_er(netwox_arg_buf(parg, 'u', &url));
  /* parameters */
  netwib_er(netwox_arg_buf(parg, 'a', &parameters));
  /* proxy */
  netwib_er(netwox_arg_port(parg, 'P', &ctx.proxy_port));
  netwib_er(netwox_arg_isset(parg, 'p', &isset));
  if (isset) {
    ctx.useproxy = NETWIB_TRUE;
    netwib_er(netwox_arg_ip(parg, 'p', &ctx.proxy_ip));
  }
  netwib_er(netwox_arg_isset(parg, 'l', &isset));
  if (isset) {
    netwib_er(netwox_arg_buf(parg, 'l', &buf));
    buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
    buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
    netwib_er(netwib_buf_append_buf(&buf, &ctx.proxy_login));
  }
  netwib_er(netwox_arg_isset(parg, 'L', &isset));
  if (isset) {
    netwib_er(netwox_arg_buf(parg, 'L', &buf));
    buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
    buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
    netwib_er(netwib_buf_append_buf(&buf, &ctx.proxy_password));
  }
  /* user agent */
  netwib_er(netwox_arg_isset(parg, 'U', &isset));
  if (isset) {
    netwib_er(netwox_arg_buf(parg, 'U', &buf));
    netwib_er(netwib_buf_append_buf(&buf, &ctx.useragent));
  }
  /* timeout */
  netwib_er(netwox_arg_isset(parg, 'T', &isset));
  if (isset) {
    netwib_er(netwox_arg_uint32(parg, 'T', &ctx.timeoutms));
  }
  /* what to display */
  netwib_er(netwox_arg_bool(parg, 's', &displaystatus));
  netwib_er(netwox_arg_bool(parg, 'h', &displayhdr));
  netwib_er(netwox_arg_bool(parg, 'b', &displaybody));
  /* files */
  phdrio = NULL;
  netwib_er(netwox_arg_isset(parg, 'H', &isset));
  if (isset) {
    netwib_er(netwox_arg_buf(parg, 'H', &buf));
    netwib_er(netwib_io_init_file_write(&buf, &phdrio));
  } else if (displayhdr) {
    netwib_er(netwib_io_init_screen(&phdrio));
  }
  pbodyio = NULL;
  netwib_er(netwox_arg_isset(parg, 'B', &isset));
  if (isset) {
    netwib_er(netwox_arg_buf(parg, 'B', &buf));
    netwib_er(netwib_io_init_file_write(&buf, &pbodyio));
  } else if (displaybody) {
    netwib_er(netwib_io_init_screen(&pbodyio));
  }

  /* request */
  netwib_er(netwox_httpcli_post(&ctx, &url, &parameters, &statuscode,
                                phdrio, pbodyio));
  if (displaystatus) {
    netwib_er(netwib_fmt_display("%{uint32}\n", statuscode));
  }

  /* close */
  if (phdrio != NULL) {
    netwib_er(netwib_io_close(&phdrio));
  }
  if (pbodyio != NULL) {
    netwib_er(netwib_io_close(&pbodyio));
  }
  netwib_er(netwox_httpclictx_close(&ctx));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
