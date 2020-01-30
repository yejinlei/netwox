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
netwib_conststring t000125_description[] = {
  "This tool is a simple web server.",
  "",
  NETWOX_DESC_rootdir,
  "Parameters --login and --password defines the login and password which",
  "will be needed to connect on this server.",
  "Parameter --allowed-clients defines the list of computer allowed to",
  "connect on the web server.",
  NETWOX_DESC_servertcpmul,
  NETWOX_DESC_toolpriv_port1024,
  NULL
};
netwox_toolarg t000125_args[] = {
  NETWOX_SOCK_ARG_TCP_MULSERPORT("80"),
  NETWOX_TOOLARG_OPT_BUF_DIR_RD('r', "rootdir", "root directory", NULL),
  NETWOX_TOOLARG_OPT_BUF_LOGIN('l', "login", "login needed to access", NULL),
  NETWOX_TOOLARG_OPT_BUF_PASSWORD('L', "password", "password needed to access",
                                  NULL),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in ms", "60000"),
  NETWOX_TOOLARG_OPTA_IPS_SU('c', "allowed-clients",
                             "clients allowed to connect", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000125_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SERVER_TCP_HTTP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000125_info = {
  "HTTP server",
  t000125_description,
  "web, apache",
  t000125_args,
  t000125_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000125_send_error(netwox_httpserctx *pctx,
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
  netwib_er(netwib_buf_append_string("</PRE></BODY></HTML>\n", pbuf));
  netwib_er(netwib_io_write(pio, pbuf));

  /* close */
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000125_send_dir(netwox_httpserctx *pctx,
                                   netwib_io *pio,
                                   netwib_constbuf *puri,
                                   netwib_constbuf *pdirname)
{
  netwib_buf *pbuf, bufext, *pfilename, *pfullfilename;
  netwib_pathstat pathstat;
  netwib_dir *pdir;
  netwib_cmp cmp;
  netwib_err ret;

  netwib_er(netwib_dir_init(pdirname, &pdir));
  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pbuf));
  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pfilename));
  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pfullfilename));

  /* write headers */
  netwib_er(netwox_httpserresphdrs_init(pctx, pio,
                                        NETWOX_HTTPHDR_STATUSCODE_OK, "Ok"));
  netwib_er(netwox_httphdr_contenttype("text/html", pbuf));
  netwib_er(netwox_httphdr_endheader(pbuf));
  netwib_er(netwib_io_write(pio, pbuf));

  /* write body */
  netwib__buf_reinit(pbuf);
  netwib_er(netwib_buf_append_string("<HTML><BODY><H1>", pbuf));
  netwib_er(netwib_buf_append_fmt(pbuf, "Directory %{buf}", puri));
  netwib_er(netwib_buf_append_string("</H1><HR><PRE>", pbuf));
  netwib_er(netwib_buf_init_ext_string("/", &bufext));
  netwib_er(netwib_buf_cmp(puri, &bufext, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    netwib_er(netwib_buf_append_string("<A HREF=\"..\">..</A>\n", pbuf));
  }
  while (NETWIB_TRUE) {
    netwib__buf_reinit(pfilename);
    ret = netwib_dir_next(pdir, pfilename);
    if (ret == NETWIB_ERR_DATAEND) break;
    netwib__buf_reinit(pfullfilename);
    netwib_er(netwib_path_init_concat(pdirname, pfilename, pfullfilename));
    netwib_er(netwib_pathstat_init(pfullfilename, &pathstat));
    if (pathstat.type == NETWIB_PATHSTAT_TYPE_DIR) {
      netwib_er(netwib_buf_append_byte('/', pfilename));
    }
    netwib_er(netwib_buf_append_fmt(pbuf, "<A HREF=\"%{buf}\">%{buf}</A>\n",
                                    pfilename, pfilename));
  }
  netwib_er(netwib_buf_append_string("</PRE></BODY></HTML>\n", pbuf));
  netwib_er(netwib_io_write(pio, pbuf));

  /* close */
  netwib_er(netwib_dir_close(&pdir));
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pbuf));
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pfilename));
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pfullfilename));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000125_send_dir_redir(netwox_httpserctx *pctx,
                                         netwib_io *pio,
                                         netwib_constbuf *puri)
{
  netwib_buf *pbuf;

  /* write headers */
  netwib_er(netwox_httpserresphdrs_init(pctx, pio, NETWOX_HTTPHDR_STATUSCODE_MOVEDPERMANENTLY, "Permanently moved"));
  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pbuf));
  netwib_er(netwox_httphdr_contenttype("text/html", pbuf));
  netwib_er(netwox_httphdr_location(puri, pbuf));
  netwib_er(netwox_httphdr_endheader(pbuf));
  netwib_er(netwib_io_write(pio, pbuf));

  /* write body */
  netwib__buf_reinit(pbuf);
  netwib_er(netwib_buf_append_string("<HTML><BODY><PRE>", pbuf));
  netwib_er(netwib_buf_append_fmt(pbuf, "Directory is at : <A HREF=\"%{buf}\">%{buf}</A>\n", puri, puri));
  netwib_er(netwib_buf_append_string("</PRE></BODY></HTML>\n", pbuf));
  netwib_er(netwib_io_write(pio, pbuf));

  /* close */
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000125_send_reg(netwox_httpserctx *pctx,
                                   netwib_io *pio,
                                   netwib_constbuf *pfilename)
{
  netwib_buf *pbuf;
  netwib_io *pfileio;
  netwib_uint32 filesize;

  netwib_er(netwib_filename_size(pfilename, &filesize));

  netwib_er(netwib_io_init_file_read(pfilename, &pfileio));

  /* write headers */
  netwib_er(netwox_httpserresphdrs_init(pctx, pio,
                                        NETWOX_HTTPHDR_STATUSCODE_OK, "Ok"));
  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pbuf));
  netwib_er(netwox_httphdr_endheader(pbuf));
  netwib_er(netwib_io_write(pio, pbuf));
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pbuf));

  /* write body */
  netwib_er(netwox_httpbody_write_io_close(pctx->pbufpool, pfileio, pio));

  /* close */
  netwib_er(netwib_io_close(&pfileio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000125_send_link(netwox_httpserctx *pctx,
                                    netwib_io *pio,
                                    netwib_constbuf *pfilename)
{
  netwib_buf *pbuf, *plink;

  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pbuf));

  /* write headers */
  netwib_er(netwox_httpserresphdrs_init(pctx, pio,
                                        NETWOX_HTTPHDR_STATUSCODE_OK, "Ok"));
  netwib_er(netwox_httphdr_contenttype("text/html", pbuf));
  netwib_er(netwox_httphdr_endheader(pbuf));
  netwib_er(netwib_io_write(pio, pbuf));

  /* do not bother creating a correct link : if it points outside
     root, it will be unreachable */

  /* write body */
  netwib__buf_reinit(pbuf);
  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &plink));
  netwib_er(netwib_unix_readlink(pfilename, plink));
  netwib_er(netwib_buf_append_string("<HTML><BODY><PRE>", pbuf));
  netwib_er(netwib_buf_append_fmt(pbuf, "Link points to : <A HREF=\"%{buf}\">%{buf}</A>\n", plink, plink));
  netwib_er(netwib_buf_append_string("</PRE></BODY></HTML>\n", pbuf));
  netwib_er(netwib_io_write(pio, pbuf));

  /* close */
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &plink));
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000125_answer(netwox_httpserctx *pctx,
                                 netwib_io *pio,
                                 netwox_httpserreqhdrs *preqhdrs)
{
  netwib_buf *puri, *pfilename;
  netwib_data data;
  netwib_uint32 datasize;
  netwib_bool auth;
  netwib_pathstat pathstat;
  netwib_cmp cmp;
  netwib_err ret;

  /* check method */
  if (preqhdrs->method != NETWOX_HTTPHDR_METHOD_GET) {
    return(t000125_send_error(pctx, pio,
                              NETWOX_HTTPHDR_STATUSCODE_NOTIMPLEMENTED,
                              "Only GET method is implemented"));
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
      return(t000125_send_error(pctx, pio,
                                NETWOX_HTTPHDR_STATUSCODE_UNAUTHORIZED,
                                "Bad login and password"));
    }
  }

  /* check uri */
  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &puri));
  ret = netwox_url_pqf_decode(&preqhdrs->uri, puri, NULL, NULL);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &puri));
    return(t000125_send_error(pctx, pio, NETWOX_HTTPHDR_STATUSCODE_BADREQUEST,
                              "URI not decoded"));
  }
  netwib_er(netwib_bufpool_buf_init(pctx->pbufpool, &pfilename));
  ret = netwib_path_init_jail(&pctx->rootdir, puri, pfilename);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &puri));
    netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pfilename));
    return(t000125_send_error(pctx, pio, NETWOX_HTTPHDR_STATUSCODE_BADREQUEST,
                              "URI not canonized"));
  }

  /*netwib_er(netwib_fmt_display("%{buf} %{buf}\n", puricanon, pfilename));*/

  /* determine filename type */
  ret = netwib_pathstat_init(pfilename, &pathstat);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(t000125_send_error(pctx, pio, NETWOX_HTTPHDR_STATUSCODE_NOTFOUND,
                                 "not found"));
  } else {
    switch(pathstat.type) {
    case NETWIB_PATHSTAT_TYPE_REG :
      ret = t000125_send_reg(pctx, pio, pfilename);
      break;
    case NETWIB_PATHSTAT_TYPE_LINK :
      ret = t000125_send_link(pctx, pio, pfilename);
      break;
    case NETWIB_PATHSTAT_TYPE_DIR :
      data = netwib__buf_ref_data_ptr(puri);
      datasize = netwib__buf_ref_data_size(puri);
      if (data[datasize-1] == '/') {
        ret = t000125_send_dir(pctx, pio, puri, pfilename);
      } else {
        netwib_er(netwib_buf_append_byte('/', puri));
        ret = t000125_send_dir_redir(pctx, pio, puri);
      }
      break;
    default :
      netwib_er(t000125_send_error(pctx, pio,
                                   NETWOX_HTTPHDR_STATUSCODE_NOTIMPLEMENTED,
                                   "Unsupported filename type"));
      break;
    }
  }

  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &pfilename));
  netwib_er(netwib_bufpool_buf_close(pctx->pbufpool, &puri));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err t000125_srv(netwib_io *psockio,
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
      /*ignore*/t000125_send_error(pctx, psockio,
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
    ret = t000125_answer(pctx, pio, &reqhdrs);
  }
  if (ret == NETWIB_ERR_OK) {
    /*
      Cannot display it, because under Windows, this slows the server
      (display is only 100 lines per second).
    netwib_er(netwib_fmt_display("%{buf}\n", &reqhdrs.uri));
    */
  } else {
    netwib_er(netwox_err_display(ret, NETWIB_ERR_ENCODETYPE_TEXT));
  }

  /* close */
  netwib_er(netwox_httpserreqhdrs_close(&reqhdrs));
  netwib_er(netwib_io_unplug_next_supported(pio, NULL));
  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err t000125_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwox_sockinfo sockinfo;
  netwox_httpserctx ctx;
  netwib_buf buf;
  netwib_bool isset;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000125_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_tcp_mulser(parg, &sockinfo));
  netwib_er(netwox_httpserctx_init(&ctx));
  netwib_er(netwox_arg_isset(parg, 'r', &isset));
  if (isset) {
    netwib__buf_reinit(&ctx.rootdir);
    netwib_er(netwox_arg_buf(parg, 'r', &buf));
    netwib_er(netwib_path_canon(&buf, &ctx.rootdir));
  }
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
  netwib_er(netwox_sock_tcp_mulser(&sockinfo, &t000125_srv, &ctx));

  /* close */
  netwib_er(netwox_httpserctx_close(&ctx));
  netwib_er(netwox_sockinfo_close(&sockinfo));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
