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
netwib_err netwox_nntpses_init(netwib_io *pioserver,
                               netwib_constbuf *puser,
                               netwib_constbuf *ppassword,
                               netwib_uint32 maxwaitms,
                               netwox_nntpses *pnntpses)
{

  netwib_er(netwib_bufpool_initdefault(&pnntpses->pbufpool));

  netwib_er(netwox_txtproto_io_rd(pioserver, &pnntpses->pio));

  if (puser != NULL) {
    netwib_er(netwib_buf_init_ext_buf(puser, &pnntpses->user));
  } else {
    netwib_er(netwib_buf_init_ext_empty(&pnntpses->user));
  }

  if (ppassword != NULL) {
    netwib_er(netwib_buf_init_ext_buf(ppassword, &pnntpses->password));
  } else {
    netwib_er(netwib_buf_init_ext_empty(&pnntpses->password));
  }

  pnntpses->maxwaitms = maxwaitms;

  netwib_er(netwib_bufpool_buf_init(pnntpses->pbufpool, &pnntpses->perrmsg));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_nntpses_banner(netwox_nntpses *pnntpses)
{
  netwib__buf_reinit(pnntpses->perrmsg);
  netwib_er(netwox_txtproto_read_reply(pnntpses->pio, pnntpses->maxwaitms,
                                       &pnntpses->errnum, pnntpses->perrmsg));
  netwib_er(netwox_txtproto_expect2(pnntpses->errnum, pnntpses->perrmsg,
                                    NETWOX_NNTP_REPLYCODE_SVCREADY,
                                    NETWOX_NNTP_REPLYCODE_SVCREADYNOPOST));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_nntpses_modereader(netwox_nntpses *pnntpses)
{

  netwib_er(netwox_nntp_write_modereader(pnntpses->pio));

  netwib__buf_reinit(pnntpses->perrmsg);
  netwib_er(netwox_txtproto_read_reply(pnntpses->pio, pnntpses->maxwaitms,
                                       &pnntpses->errnum, pnntpses->perrmsg));
  if (pnntpses->errnum == NETWOX_NNTP_REPLYCODE_AUTHUSER) {
    netwib_er(netwox_nntpses_auth(pnntpses));
    netwib_er(netwox_nntp_write_modereader(pnntpses->pio));
    netwib__buf_reinit(pnntpses->perrmsg);
    netwib_er(netwox_txtproto_read_reply(pnntpses->pio, pnntpses->maxwaitms,
                                         &pnntpses->errnum,
                                         pnntpses->perrmsg));
  }
  netwib_er(netwox_txtproto_expect2(pnntpses->errnum, pnntpses->perrmsg,
                                    NETWOX_NNTP_REPLYCODE_SVCREADY,
                                    NETWOX_NNTP_REPLYCODE_SVCREADYNOPOST));


  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_nntpses_group(netwox_nntpses *pnntpses,
                                netwib_constbuf *pnewsgroup)
{
  netwib_er(netwox_nntp_write_group(pnntpses->pio, pnewsgroup));

  netwib__buf_reinit(pnntpses->perrmsg);
  netwib_er(netwox_txtproto_read_reply(pnntpses->pio, pnntpses->maxwaitms,
                                       &pnntpses->errnum, pnntpses->perrmsg));
  if (pnntpses->errnum == NETWOX_NNTP_REPLYCODE_AUTHUSER) {
    netwib_er(netwox_nntpses_auth(pnntpses));
    netwib_er(netwox_nntp_write_group(pnntpses->pio, pnewsgroup));
    netwib__buf_reinit(pnntpses->perrmsg);
    netwib_er(netwox_txtproto_read_reply(pnntpses->pio, pnntpses->maxwaitms,
                                         &pnntpses->errnum,
                                         pnntpses->perrmsg));
  }
  netwib_er(netwox_txtproto_expect1(pnntpses->errnum, pnntpses->perrmsg,
                                    NETWOX_NNTP_REPLYCODE_GROUPSELECTED));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_nntpses_article(netwox_nntpses *pnntpses,
                                  netwib_uint32 artnum)
{
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwox_nntp_write_article(pnntpses->pio, artnum));

  netwib__buf_reinit(pnntpses->perrmsg);
  netwib_er(netwox_txtproto_read_reply(pnntpses->pio, pnntpses->maxwaitms,
                                       &pnntpses->errnum, pnntpses->perrmsg));
  if (pnntpses->errnum == NETWOX_NNTP_REPLYCODE_AUTHUSER) {
    netwib_er(netwox_nntpses_auth(pnntpses));
    netwib_er(netwox_nntp_write_article(pnntpses->pio, artnum));
    netwib__buf_reinit(pnntpses->perrmsg);
    netwib_er(netwox_txtproto_read_reply(pnntpses->pio, pnntpses->maxwaitms,
                                         &pnntpses->errnum,
                                         pnntpses->perrmsg));
  }
  switch(pnntpses->errnum) {
  case NETWOX_NNTP_REPLYCODE_ARTICLEHEADBODY :
    ret = NETWIB_ERR_OK;
    break;
  case NETWOX_NNTP_REPLYCODE_NOARTICLENUMBER :
  case NETWOX_NNTP_REPLYCODE_NOARTICLEFOUND :
    ret = NETWIB_ERR_NOTFOUND;
    break;
  default:
    ret = netwox_txtproto_expect1(pnntpses->errnum, pnntpses->perrmsg,
                                  NETWOX_NNTP_REPLYCODE_ARTICLEHEADBODY);
    break;
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_nntpses_post(netwox_nntpses *pnntpses,
                               netwib_constbuf *pdata)
{

  netwib_er(netwox_nntp_write_post_begin(pnntpses->pio));

  netwib__buf_reinit(pnntpses->perrmsg);
  netwib_er(netwox_txtproto_read_reply(pnntpses->pio, pnntpses->maxwaitms,
                                       &pnntpses->errnum, pnntpses->perrmsg));
  if (pnntpses->errnum == NETWOX_NNTP_REPLYCODE_AUTHUSER) {
    netwib_er(netwox_nntpses_auth(pnntpses));
    netwib_er(netwox_nntp_write_post_begin(pnntpses->pio));
    netwib__buf_reinit(pnntpses->perrmsg);
    netwib_er(netwox_txtproto_read_reply(pnntpses->pio, pnntpses->maxwaitms,
                                         &pnntpses->errnum,
                                         pnntpses->perrmsg));
  }
  netwib_er(netwox_txtproto_expect1(pnntpses->errnum, pnntpses->perrmsg,
                                    NETWOX_NNTP_REPLYCODE_SENDDATA));

  netwib_er(netwox_nntp_write_post_content(pnntpses->pio, pdata));
  netwib_er(netwox_nntp_write_post_end(pnntpses->pio));

  netwib__buf_reinit(pnntpses->perrmsg);
  netwib_er(netwox_txtproto_read_reply(pnntpses->pio, pnntpses->maxwaitms,
                                       &pnntpses->errnum, pnntpses->perrmsg));
  netwib_er(netwox_txtproto_expect1(pnntpses->errnum, pnntpses->perrmsg,
                                    NETWOX_NNTP_REPLYCODE_POSTOK));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_nntpses_list(netwox_nntpses *pnntpses)
{
  netwib_er(netwox_nntp_write_list(pnntpses->pio));

  netwib__buf_reinit(pnntpses->perrmsg);
  netwib_er(netwox_txtproto_read_reply(pnntpses->pio, pnntpses->maxwaitms,
                                       &pnntpses->errnum, pnntpses->perrmsg));
  if (pnntpses->errnum == NETWOX_NNTP_REPLYCODE_AUTHUSER) {
    netwib_er(netwox_nntpses_auth(pnntpses));
    netwib_er(netwox_nntp_write_list(pnntpses->pio));
    netwib__buf_reinit(pnntpses->perrmsg);
    netwib_er(netwox_txtproto_read_reply(pnntpses->pio, pnntpses->maxwaitms,
                                         &pnntpses->errnum,
                                         pnntpses->perrmsg));
  }
  netwib_er(netwox_txtproto_expect1(pnntpses->errnum, pnntpses->perrmsg,
                                    NETWOX_NNTP_REPLYCODE_LISTFOLLOWS));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_nntpses_xover(netwox_nntpses *pnntpses,
                                netwib_uint32 firstartnum,
                                netwib_uint32 lastartnum)
{
  netwib_er(netwox_nntp_write_xover(pnntpses->pio, firstartnum, lastartnum));

  netwib__buf_reinit(pnntpses->perrmsg);
  netwib_er(netwox_txtproto_read_reply(pnntpses->pio, pnntpses->maxwaitms,
                                       &pnntpses->errnum, pnntpses->perrmsg));
  if (pnntpses->errnum == NETWOX_NNTP_REPLYCODE_AUTHUSER) {
    netwib_er(netwox_nntpses_auth(pnntpses));
    netwib_er(netwox_nntp_write_xover(pnntpses->pio, firstartnum, lastartnum));
    netwib__buf_reinit(pnntpses->perrmsg);
    netwib_er(netwox_txtproto_read_reply(pnntpses->pio, pnntpses->maxwaitms,
                                         &pnntpses->errnum,
                                         pnntpses->perrmsg));
  }
  netwib_er(netwox_txtproto_expect1(pnntpses->errnum, pnntpses->perrmsg,
                                    NETWOX_NNTP_REPLYCODE_XOVERFOLLOWS));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_nntpses_auth(netwox_nntpses *pnntpses)
{
  if (netwib__buf_ref_data_size(&pnntpses->user) == 0) {
    return(NETWOX_ERR_AUTHNEEDED);
  }

  netwib_er(netwox_nntp_write_authinfo_user(pnntpses->pio, &pnntpses->user));

  netwib__buf_reinit(pnntpses->perrmsg);
  netwib_er(netwox_txtproto_read_reply(pnntpses->pio, pnntpses->maxwaitms,
                                       &pnntpses->errnum, pnntpses->perrmsg));
  if (pnntpses->errnum == NETWOX_NNTP_REPLYCODE_AUTHOK) {
    return(NETWIB_ERR_OK);
  } else if (pnntpses->errnum != NETWOX_NNTP_REPLYCODE_AUTHPASS) {
    netwib_er(netwox_txtproto_expect2(pnntpses->errnum, pnntpses->perrmsg,
                                      NETWOX_NNTP_REPLYCODE_AUTHOK,
                                      NETWOX_NNTP_REPLYCODE_AUTHPASS));
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwox_nntp_write_authinfo_pass(pnntpses->pio, &pnntpses->password));

  netwib__buf_reinit(pnntpses->perrmsg);
  netwib_er(netwox_txtproto_read_reply(pnntpses->pio, pnntpses->maxwaitms,
                                       &pnntpses->errnum, pnntpses->perrmsg));
  netwib_er(netwox_txtproto_expect1(pnntpses->errnum, pnntpses->perrmsg,
                                    NETWOX_NNTP_REPLYCODE_AUTHOK));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_nntpses_quit(netwox_nntpses *pnntpses)
{
  netwib_er(netwox_nntp_write_quit(pnntpses->pio));
  netwib__buf_reinit(pnntpses->perrmsg);
  netwib_er(netwox_txtproto_read_reply(pnntpses->pio, pnntpses->maxwaitms,
                                       &pnntpses->errnum, pnntpses->perrmsg));
  netwib_er(netwox_txtproto_expect1(pnntpses->errnum, pnntpses->perrmsg,
                                    NETWOX_NNTP_REPLYCODE_SVCCLOSING));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_nntpses_close(netwox_nntpses *pnntpses)
{
  netwib_er(netwib_bufpool_buf_close(pnntpses->pbufpool, &pnntpses->perrmsg));
  netwib_er(netwib_io_unplug_next_supported(pnntpses->pio, NULL));
  netwib_er(netwib_io_close(&pnntpses->pio));

  netwib_er(netwib_bufpool_close(&pnntpses->pbufpool));

  return(NETWIB_ERR_OK);
}
