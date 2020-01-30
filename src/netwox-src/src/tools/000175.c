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
netwib_conststring t000175_description[] = {
  "This tool downloads a HTTP or FTP file and check its MD5.",
  NETWOX_DESC_check_work,
  "",
  NETWOX_DESC_http_uri,
  NETWOX_DESC_check_md5,
  NETWOX_DESC_http_proxy,
  NETWOX_DESC_http_useragent,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000175_args[] = {
  NETWOX_TOOLARG_REQ_BUF_URI('u', "uri", "url/uri to download",
                             "http://www.example.com/"),
  NETWOX_TOOLARG_REQ_BUF_MD5('m', "md5", NULL, NULL),
  NETWOX_TOOLARG_OPTA_IP_DST('p', "proxy-ip", "IP address of proxy", NULL),
  NETWOX_TOOLARG_OPTA_PORT_DST('P', "proxy-port", "port of proxy", "3128"),
  NETWOX_TOOLARG_OPTA_BUF_LOGIN('l', "proxy-login", "login of proxy", NULL),
  NETWOX_TOOLARG_OPTA_BUF_PASSWORD('L', "proxy-password", "password of proxy",
                                   NULL),
  NETWOX_TOOLARG_OPTA_BUF_USERAGENT('U', "user-agent", NULL, NULL),
  NETWOX_TOOLARG_OPTA_BOOL('f', "ftp-passive", "use passive FTP", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in ms", "60000"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000175_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_FTP,
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_HTTP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000175_info = {
  "Web download (http://... or ftp://...) and check its MD5",
  t000175_description,
  NULL,
  t000175_args,
  t000175_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000175_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_httphdr_statuscode statuscode;
  netwox_httpclictx ctx;
  netwib_buf buf, url, tmpfilename, hexamd5, wantedmd5;
  netwib_io *ptmpio;
  netwib_bool isset;
  netwib_cmp cmp;
  netwib_err ret;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000175_info, &parg));
  netwib_er(netwox_httpclictx_init(&ctx));
  /* url */
  netwib_er(netwox_arg_buf(parg, 'u', &url));
  netwib_er(netwox_arg_buf(parg, 'm', &wantedmd5));
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
  /* FTP passive */
  netwib_er(netwox_arg_bool(parg, 'f', &ctx.ftp_passive));
  /* timeout */
  netwib_er(netwox_arg_isset(parg, 'T', &isset));
  if (isset) {
    netwib_er(netwox_arg_uint32(parg, 'T', &ctx.timeoutms));
  }

  /* request */
  netwib_er(netwib_buf_init_mallocdefault(&tmpfilename));
  netwib_er(netwib_io_init_filetemp(NETWIB_FALSE, &tmpfilename, &ptmpio));
  ret = netwox_url_download_io(&ctx, &url, ptmpio, &statuscode);
  netwib_er(netwib_io_close(&ptmpio));

  /* check md5 */
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_init_mallocdefault(&hexamd5));
    ret = netwox_cryptohash_compute_file(NETWOX_CRYPTOHASH_TYPE_MD5,
                                         &tmpfilename, &hexamd5);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_buf_cmp(&hexamd5, &wantedmd5, &cmp));
      if (cmp != NETWIB_CMP_EQ) {
        netwib_er(netwib_fmt_display("MD5 is %{buf} instead of %{buf}\n",
                                     &hexamd5, &wantedmd5));
        ret = NETWOX_ERR_BADVALUE;
      }
    }
    netwib_er(netwib_buf_close(&hexamd5));
  }
  netwib_er(netwib_filename_remove(&tmpfilename));

  /* close */
  netwib_er(netwib_buf_close(&tmpfilename));
  netwib_er(netwox_httpclictx_close(&ctx));
  netwib_er(netwox_arg_close(&parg));

  return(ret);
}
