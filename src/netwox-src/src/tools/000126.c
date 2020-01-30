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
netwib_conststring t000126_description[] = {
  "This tool is a web server permitting clients to run a command.",
  "",
  NETWOX_DESC_allowedclients,
  NETWOX_DESC_servertcpmul,
  NETWOX_DESC_toolpriv_port1024,
  NULL
};
netwox_toolarg t000126_args[] = {
  NETWOX_SOCK_ARG_TCP_MULSERPORT("80"),
  NETWOX_TOOLARG_OPT_BUF_LOGIN('l', "login", "login needed to access", NULL),
  NETWOX_TOOLARG_OPT_BUF_PASSWORD('L', "password", "password needed to access",
                                  NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in ms", "60000"),
  NETWOX_TOOLARG_OPTA_IPS_SU('c', "allowed-clients",
                             "clients allowed to connect", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000126_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SERVER_TCP_HTTP,
  NETWOX_TOOLTREENODETYPE_REMADM,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000126_info = {
  "HTTP remote administration server",
  t000126_description,
  "exec, run, web",
  t000126_args,
  t000126_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000126_send_error(netwox_httpserctx *pctx,
                                     netwib_io *pio,
                                     netwox_httphdr_statuscode statuscode,
                                     netwib_conststring statusmessage)
{
  netwib_buf *pbuf;

  /* write headers */
  netwib_er(netwox_httpserresphdrs_init(pctx, pio, statuscode, statusmessage));
  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pbuf));
  netwib_er(netwox_httphdr_contenttype("text/html", pbuf));
  netwib_er(netwox_httphdr_endheader(pbuf));
  netwib_er(netwib_io_write(pio, pbuf));

  /* write body */
  netwib__buf_reinit(pbuf);
  netwib_er(netwib_buf_append_string("<HTML><BODY><PRE>", pbuf));
  netwib_er(netwib_buf_append_fmt(pbuf, "Error %{uint32} : %s", statuscode,
                                  statusmessage));
  netwib_er(netwib_buf_append_string("</PRE></BODY></HTML>", pbuf));
  netwib_er(netwib_io_write(pio, pbuf));

  /* close */
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000126_send_form(netwox_httpserctx *pctx,
                                    netwib_io *pio,
                                    netwib_constbuf *pcmd)
{
  netwib_buf *pbuf;
  netwib_io *pexecio;
  netwib_bool exitednormally;
  netwib_uint32 returnedvalue;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pbuf));

  /* write headers */
  netwib_er(netwox_httpserresphdrs_init(pctx, pio,
                                        NETWOX_HTTPHDR_STATUSCODE_OK, "Ok"));
  netwib_er(netwox_httphdr_contenttype("text/html", pbuf));
  netwib_er(netwox_httphdr_endheader(pbuf));
  netwib_er(netwib_io_write(pio, pbuf));

  /* write form */
  netwib__buf_reinit(pbuf);
  netwib_er(netwib_buf_append_string("<HTML><BODY><BR>\nCommand : ", pbuf));
  netwib_er(netwib_buf_append_string("<FORM METHOD=POST ACTION=\"/exec\">",
                                   pbuf));
  netwib_er(netwib_buf_append_string("<INPUT NAME=\"cmd\" TYPE=text SIZE=80 VALUE=\"", pbuf));
  netwib_er(netwox_html_data_encode(pcmd, NETWIB_TRUE, pbuf));
  netwib_er(netwib_buf_append_string("\">", pbuf));
  netwib_er(netwib_buf_append_string("<INPUT TYPE=submit VALUE=\"Run\">",
                                   pbuf));
  netwib_er(netwib_buf_append_string("</FORM><BR>\n", pbuf));
  netwib_er(netwib_buf_append_string("Examples: /bin/ls, /bin/sh -c \"pwd;ls\", cmd /c dir c:<BR>\n", pbuf));
  netwib_er(netwib_buf_append_string("Note: full path has to be specified<BR>\n", pbuf));
  netwib_er(netwib_buf_append_string("Note: cannot use 'prog1 | prog2', 'program &gt; file' or 'program &lt; file'<BR>\n", pbuf));
  netwib_er(netwib_buf_append_string("Note: program must not expect keyboard presses (stdin) from user<BR>\n", pbuf));
  netwib_er(netwib_buf_append_string("<HR><PRE>", pbuf));
  netwib_er(netwib_io_write(pio, pbuf));

  /* leave now if command is empty */
  if (netwib__buf_ref_data_size(pcmd) == 0) {
    netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pbuf));
    return(NETWIB_ERR_OK);
  }

  /* write body */
  ret = netwib_io_init_exec(pcmd, NETWIB_IO_WAYTYPE_READ, NETWIB_FALSE,
                            &exitednormally, &returnedvalue, &pexecio);
  if (ret != NETWIB_ERR_OK) {
    netwib__buf_reinit(pbuf);
    netwib_er(netwib_buf_append_string("Cannot run this command", pbuf));
    netwib_er(netwib_io_write(pio, pbuf));
    netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pbuf));
    return(NETWIB_ERR_OK);
  }
  netwib_er(netwox_httpbody_write_io_close(pctx->pbufpool, pexecio, pio));
  netwib_er(netwib_io_close(&pexecio));

  /* write result */
  netwib__buf_reinit(pbuf);
  netwib_er(netwib_buf_append_string("</PRE><HR>", pbuf));
  if (exitednormally) {
    netwib_er(netwib_buf_append_fmt(pbuf, "Program exited with return code %{uint32}<BR>\n", returnedvalue));
  } else {
    netwib_er(netwib_buf_append_string("Program was interrupted<BR>\n", pbuf));
  }
  netwib_er(netwib_buf_append_string("</BODY></HTML>", pbuf));
  netwib_er(netwib_io_write(pio, pbuf));

  /* close */
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000126_answer(netwox_httpserctx *pctx,
                                 netwib_io *pio,
                                 netwox_httpserreqhdrs *preqhdrs)
{
  netwib_buf query, *pquery, cmd, *pcmd;
  netwib_bool auth;
  netwib_cmp cmp;
  netwib_err ret;

  /* check method */
  if (preqhdrs->method != NETWOX_HTTPHDR_METHOD_GET &&
      preqhdrs->method != NETWOX_HTTPHDR_METHOD_POST) {
    return(t000126_send_error(pctx, pio,
                              NETWOX_HTTPHDR_STATUSCODE_NOTIMPLEMENTED,
                              "Only GET and POST methods are implemented"));
  }

  /* check authentication */
  if (netwib__buf_ref_data_size(&pctx->login)) {
    auth = NETWIB_FALSE;
    if (preqhdrs->authbasicset) {
      netwib_er(netwib_buf_cmp(&pctx->login, &preqhdrs->authbasiclogin, &cmp));
      if (cmp == NETWIB_CMP_EQ) {
        netwib_er(netwib_buf_cmp(&pctx->password, &preqhdrs->authbasicpassword,
                                 &cmp));
        if (cmp == NETWIB_CMP_EQ) {
          auth = NETWIB_TRUE;
        }
      }
    }
    if (!auth) {
      return(t000126_send_error(pctx, pio,
                                NETWOX_HTTPHDR_STATUSCODE_UNAUTHORIZED,
                                "Bad login and password"));
    }
  }

  /* obtain query */
  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pquery));
  if (preqhdrs->method == NETWOX_HTTPHDR_METHOD_GET) {
    ret = netwox_url_pqf_decode(&preqhdrs->uri, NULL, &query, NULL);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pquery));
      return(t000126_send_error(pctx, pio,
                                NETWOX_HTTPHDR_STATUSCODE_BADREQUEST,
                                "URI not decoded"));
    }
    netwib_er(netwib_buf_append_buf(&query, pquery));
  } else {
    ret = netwox_httpbody_read_buf(pctx->timeoutms,
                                   preqhdrs->contentlengthset,
                                   preqhdrs->contentlength, pio, pquery);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pquery));
      return(t000126_send_error(pctx, pio,
                                NETWOX_HTTPHDR_STATUSCODE_BADREQUEST,
                                "Timeout"));
    }
  }
  /*netwib_er(netwib_fmt_display("query=%{buf}\n", pquery));*/

  /* obtain command parameter */
  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pcmd));
  if (netwib__buf_ref_data_size(pquery)) {
    netwib_er(netwib_buf_init_ext_string("cmd", &cmd));
    ret = netwox_url_query_decode(pquery, &cmd, NULL, pcmd);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pcmd));
      netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pquery));
      return(t000126_send_error(pctx, pio,
                                NETWOX_HTTPHDR_STATUSCODE_BADREQUEST,
                                "URI/POST does not contain a valid command"));
    }
  }
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pquery));
  /*netwib_er(netwib_fmt_display("cmd=%{buf}\n", pcmd));*/

  /* display form and run command */
  netwib_er(t000126_send_form(pctx, pio, pcmd));

  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pcmd));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000126_srv(netwib_io *psockio,
                              netwib_ptr pinfos)
{
  netwox_httpserctx *pctx = (netwox_httpserctx *)pinfos;
  netwib_io *pio;
  netwox_httpserreqhdrs reqhdrs;
  netwib_ip remoteip;
  netwib_port remoteport;
  netwib_bool allowed;
  netwib_err ret=NETWIB_ERR_OK;

  /* check client */
  if (pctx->pallowedclients != NULL) {
    netwib_er(netwib_sock_ctl_get_remote(psockio, &remoteip, &remoteport));
    netwib_er(netwib_ips_contains_ip(pctx->pallowedclients, &remoteip,
                                     &allowed));
    if (!allowed) {
      netwib_fmt_display("IP address %{ip} is not allowed\n", &remoteip);
      /*ignore*/t000126_send_error(pctx, psockio,
                                   NETWOX_HTTPHDR_STATUSCODE_FORBIDDEN,
                                   "Your address is forbidden");
      return(NETWIB_ERR_OK);
    }
  }

  /* prepare a read by line io */
  netwib_er(netwib_io_init_data(NETWIB_IO_INIT_DATA_TYPE_LINE,
                                NETWIB_IO_INIT_DATA_TYPE_TRANSPARENT, &pio));
  netwib_er(netwib_io_plug_rdwr(pio, psockio));

  /* read request */
  netwib_er(netwox_httpserreqhdrs_init(&reqhdrs));
  ret = netwox_httpserreqhdrs_decode(pctx, pio, &reqhdrs);

  /* answer */
  if (ret == NETWIB_ERR_OK) {
    ret = t000126_answer(pctx, pio, &reqhdrs);
  }
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_err_display(ret, NETWIB_ERR_ENCODETYPE_TEXT));
  }

  /* close */
  netwib_er(netwox_httpserreqhdrs_close(&reqhdrs));
  netwib_er(netwib_io_unplug_next_supported(pio, NULL));
  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err t000126_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwox_httpserctx ctx;
  netwib_buf buf;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000126_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_tcp_mulser(parg, &sockinfo));
  netwib_er(netwox_httpserctx_init(&ctx));
  netwib_er(netwox_arg_buf(parg, 'l', &buf));
  buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwib_buf_append_buf(&buf, &ctx.login));
  netwib_er(netwox_arg_buf(parg, 'L', &buf));
  buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwib_buf_append_buf(&buf, &ctx.password));
  netwib_er(netwox_arg_uint32(parg, 'T', &ctx.timeoutms));
  netwib_er(netwox_arg_ips(parg, 'c', &ctx.pallowedclients));
  netwib_er(netwib_buf_append_string("HTTP Server", &ctx.realm));

  /* main loop */
  netwib_er(netwox_sock_tcp_mulser(&sockinfo, &t000126_srv, &ctx));

  /* close */
  netwib_er(netwox_httpserctx_close(&ctx));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
