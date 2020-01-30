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
netwib_conststring t000134_description[] = {
  "This tool reads an HTML file and shows url/uri it contains.",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000134_args[] = {
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('f', "filename", "HTML file", "file.html"),
  NETWOX_TOOLARG_OPTA_BOOL('h', "display-http", "display http:// uris",
                           NULL),
  NETWOX_TOOLARG_OPTA_BOOL('s', "display-https", "display https:// uris",
                           NULL),
  NETWOX_TOOLARG_OPTA_BOOL('t', "display-ftp", "display ftp:// uris",
                           NULL),
  NETWOX_TOOLARG_OPTA_BOOL('n', "display-none", "display local uris",
                           NULL),
  NETWOX_TOOLARG_OPTA_BOOL('u', "display-unknown", "display unknown uris",
                           NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000134_nodes[] = {
  NETWOX_TOOLTREENODETYPE_CLIENT_TCP_HTTP,
  NETWOX_TOOLTREENODETYPE_NOTNET,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000134_info = {
  "Obtain urls/uris in a HMTL file",
  t000134_description,
  NULL,
  t000134_args,
  t000134_nodes,
};

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bool display_all;
  netwib_bool display_http;
  netwib_bool display_https;
  netwib_bool display_ftp;
  netwib_bool display_none;
  netwib_bool display_unknown;
} t000134_profile;

/*-------------------------------------------------------------*/
static netwib_err t000134_ring_display(netwib_ring *pring,
                                       t000134_profile *pprofile)
{
  netwib_ring_index *pringindex;
  netwox_htmlfile_urllist_item *pitem;
  netwib_bool display;
  netwox_url_scheme scheme;
  netwib_err ret;

  netwib_er(netwib_ring_index_init(pring, &pringindex));

  ret = NETWIB_ERR_OK;
  while(NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pitem);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    display = NETWIB_FALSE;
    if (pprofile->display_all) {
      display = NETWIB_TRUE;
    } else {
      netwib_er(netwox_url_decode_scheme(&pitem->value, &scheme, NULL));
      switch(scheme) {
      case NETWOX_URL_SCHEME_HTTP :
        if (pprofile->display_http) display = NETWIB_TRUE;
        break;
      case NETWOX_URL_SCHEME_HTTPS :
        if (pprofile->display_https) display = NETWIB_TRUE;
        break;
      case NETWOX_URL_SCHEME_FTP :
        if (pprofile->display_ftp) display = NETWIB_TRUE;
        break;
      case NETWOX_URL_SCHEME_NONE :
        if (pprofile->display_none) display = NETWIB_TRUE;
        break;
      case NETWOX_URL_SCHEME_UNKNOWN :
        if (pprofile->display_unknown) display = NETWIB_TRUE;
        break;
      default :
        break;
      }
    }
    if (display) {
      netwib_er(netwib_fmt_display("%{buf}\n", &pitem->value));
    }
  }

  netwib_er(netwib_ring_index_close(&pringindex));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000134_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_ring *pring;
  netwib_buf filename;
  t000134_profile profile;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000134_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'f', &filename));
  netwib_er(netwox_arg_bool(parg, 'h', &profile.display_http));
  netwib_er(netwox_arg_bool(parg, 's', &profile.display_https));
  netwib_er(netwox_arg_bool(parg, 't', &profile.display_ftp));
  netwib_er(netwox_arg_bool(parg, 'n', &profile.display_none));
  netwib_er(netwox_arg_bool(parg, 'u', &profile.display_unknown));
  profile.display_all = NETWIB_TRUE;
  if (profile.display_http || profile.display_https || profile.display_ftp ||
      profile.display_none || profile.display_unknown) {
    profile.display_all = NETWIB_FALSE;
  }

  /* display */
  netwib_er(netwox_htmlfile_urllist_ring_init(&pring));
  netwib_er(netwox_htmlfile_urllist_file_read(&filename, NETWIB_FALSE, pring));
  netwib_er(t000134_ring_display(pring, &profile));
  netwib_er(netwox_htmlfile_urllist_ring_close(&pring));

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
