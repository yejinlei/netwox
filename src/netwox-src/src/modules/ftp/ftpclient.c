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
netwib_err netwox_ftpclient_init(netwox_sockinfo *psockinfo,
                                 netwib_uint32 maxwaitms,
                                 netwox_ftpclient *pftpclient)
{
  netwib_io *pio;
  netwib_err ret;

  /* initialize structure */
  pftpclient->psockinfo = psockinfo;
  netwib_er(netwox_sock_init(psockinfo, &pio));
  netwib_er(netwox_txtproto_io_rd(pio, &pftpclient->pio));
  netwib_er(netwib_bufpool_initdefault(&pftpclient->pbufpool));
  pftpclient->maxwaitms = maxwaitms;
  netwib_er(netwib_buf_init_mallocdefault(&pftpclient->errmsg));
  pftpclient->passive = NETWIB_FALSE;

  /* read banner */
  ret = netwox_txtproto_read_reply(pftpclient->pio, pftpclient->maxwaitms,
                                   &pftpclient->errnum, &pftpclient->errmsg);
  if (ret == NETWIB_ERR_OK) {
    ret = netwox_txtproto_expect1(pftpclient->errnum, &pftpclient->errmsg,
                                  NETWOX_FTPREPLY_220_SVCREADY);
  }

  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_bufpool_close(&pftpclient->pbufpool));
    netwib_er(netwib_buf_close(&pftpclient->errmsg));
    netwib_er(netwib_io_close(&pftpclient->pio));
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpclient_close(netwox_ftpclient *pftpclient)
{
  netwib_err ret;

  /* send quit (ignore errors) */
  ret = netwox_ftpcli_quit(pftpclient->pio);
  if (ret == NETWIB_ERR_OK) {
    netwib__buf_reinit(&pftpclient->errmsg);
    ret = netwox_txtproto_read_reply(pftpclient->pio, pftpclient->maxwaitms,
                                     &pftpclient->errnum, &pftpclient->errmsg);
    if (ret == NETWIB_ERR_OK) {
      ret = netwox_txtproto_expect1(pftpclient->errnum, &pftpclient->errmsg,
                                    NETWOX_FTPREPLY_221_SVCCLOSING);
    }
  }

  /* close */
  netwib_er(netwib_bufpool_close(&pftpclient->pbufpool));
  netwib_er(netwib_buf_close(&pftpclient->errmsg));
  netwib_er(netwib_io_close(&pftpclient->pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_ftpclient_data_passive_ip4(netwox_ftpclient *pftpclient,
                                                    netwib_ip *premoteip,
                                                    netwib_port *premoteport)
{
  netwib_uint32 a, b, c, d, e, f;
  netwib_err ret;

  /* send command */
  netwib_er(netwox_ftpcli_pasv(pftpclient->pio));
  netwib__buf_reinit(&pftpclient->errmsg);
  netwib_er(netwox_txtproto_read_reply(pftpclient->pio, pftpclient->maxwaitms,
                                       &pftpclient->errnum,
                                       &pftpclient->errmsg));
  netwib_er(netwox_txtproto_expect1(pftpclient->errnum, &pftpclient->errmsg,
                                    NETWOX_FTPREPLY_227_ENTERINGPASSIVEMODE));

  /* obtain server ip and port */
  ret = netwib_buf_decode_fmt(&pftpclient->errmsg, "%{*;buf:glob}(%{uint32},%{uint32},%{uint32},%{uint32},%{uint32},%{uint32})", &a, &b, &c, &d, &e, &f);
  if (ret != NETWIB_ERR_OK) {
    /* rfc 1123 indicates parenthesis are optional */
    netwib_er(netwib_buf_decode_fmt(&pftpclient->errmsg, "%{*;buf:glob} %{uint32},%{uint32},%{uint32},%{uint32},%{uint32},%{uint32}", &a, &b, &c, &d, &e, &f));
  }
  netwib_er(netwib_ip_init_ip4(netwib_c2_uint32_4(a, b , c, d), premoteip));
  *premoteport = netwib_c2_uint16_2(e, f);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_ftpclient_data_passive_ip6(netwox_ftpclient *pftpclient,
                                                    netwib_ip *premoteip,
                                                    netwib_port *premoteport)
{

  /* send command */
  netwib_er(netwox_ftpcli_epsv(pftpclient->pio));
  netwib__buf_reinit(&pftpclient->errmsg);
  netwib_er(netwox_txtproto_read_reply(pftpclient->pio, pftpclient->maxwaitms,
                                       &pftpclient->errnum,
                                       &pftpclient->errmsg));
  netwib_er(netwox_txtproto_expect1(pftpclient->errnum, &pftpclient->errmsg,
                                 NETWOX_FTPREPLY_229_ENTERINGPASSIVEMODEEXT));

  /* obtain server ip and port */
  netwib_er(netwib_sock_ctl_get_remote(pftpclient->pio, premoteip, NULL));
  netwib_er(netwib_buf_decode_fmt(&pftpclient->errmsg, "%{*;buf:glob} (%{*;c}%{*;c}%{*;c}%{port}%{*;c})", premoteport));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_ftpclient_data_active_ip4(netwox_ftpclient *pftpclient,
                                                   netwib_ip *plocalip,
                                                   netwib_port localport)
{

  /* send port command */
  netwib_er(netwox_ftpcli_port(pftpclient->pio, plocalip, localport));
  netwib__buf_reinit(&pftpclient->errmsg);
  netwib_er(netwox_txtproto_read_reply(pftpclient->pio, pftpclient->maxwaitms,
                                       &pftpclient->errnum,
                                       &pftpclient->errmsg));
  netwib_er(netwox_txtproto_expect1(pftpclient->errnum, &pftpclient->errmsg,
                                    NETWOX_FTPREPLY_200_CMDOK));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_ftpclient_data_active_ip6(netwox_ftpclient *pftpclient,
                                                   netwib_ip *plocalip,
                                                   netwib_port localport)
{

  /* send port command */
  netwib_er(netwox_ftpcli_eprt(pftpclient->pio, plocalip, localport));
  netwib__buf_reinit(&pftpclient->errmsg);
  netwib_er(netwox_txtproto_read_reply(pftpclient->pio, pftpclient->maxwaitms,
                                       &pftpclient->errnum,
                                       &pftpclient->errmsg));
  netwib_er(netwox_txtproto_expect1(pftpclient->errnum, &pftpclient->errmsg,
                                    NETWOX_FTPREPLY_200_CMDOK));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpclient_data(netwox_ftpclient *pftpclient,
                                 netwib_io **ppdataio)
{
  netwib_ip localip, remoteip;
  netwib_port remoteport;
  netwox_sockinfo newsockinfo;
  netwib_err ret;

  if (pftpclient->passive) {
    /* obtain remote ip/port */
    netwib_er(netwib_sock_ctl_get_local(pftpclient->pio, &localip, NULL));
    if (localip.iptype == NETWIB_IPTYPE_IP4) {
      netwib_er(netwox_ftpclient_data_passive_ip4(pftpclient, &remoteip,
                                               &remoteport));
    } else {
      netwib_er(netwox_ftpclient_data_passive_ip6(pftpclient, &remoteip,
                                               &remoteport));
    }
    /* connect to server */
    netwib_er(netwox_sockinfo_init_connect(pftpclient->psockinfo,
                                           pftpclient->pio,
                                           remoteport, &newsockinfo));
    newsockinfo.remoteip = remoteip;
    ret = netwox_sock_init(&newsockinfo, ppdataio);
    netwib_er(netwox_sockinfo_close(&newsockinfo));
  } else {
    /* create server */
    netwib_er(netwox_sockinfo_init_listen(pftpclient->psockinfo,
                                          pftpclient->pio, 0, &newsockinfo));
    while(NETWIB_TRUE) {
      netwib_er(netwib_uint32_init_rand(1024, 0xFFFF, &newsockinfo.localport));
      ret = netwox_sock_init(&newsockinfo, ppdataio);
      if (ret == NETWIB_ERR_OK) {
        break;
      }
    }
    netwib_er(netwox_sockinfo_close(&newsockinfo));
    /* tell to server */
    if (newsockinfo.localip.iptype == NETWIB_IPTYPE_IP4) {
      netwib_er(netwox_ftpclient_data_active_ip4(pftpclient,
                                                 &newsockinfo.localip,
                                                 newsockinfo.localport));
    } else {
      netwib_er(netwox_ftpclient_data_active_ip6(pftpclient,
                                                 &newsockinfo.localip,
                                                 newsockinfo.localport));
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpclient_login(netwox_ftpclient *pftpclient,
                                  netwib_constbuf *puser,
                                  netwib_constbuf *ppassword)
{
  netwib_buf user, password;

  /* USER */
  user = *puser;
  user.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  netwib_er(netwox_ftpcli_user(pftpclient->pio, &user));
  netwib__buf_reinit(&pftpclient->errmsg);
  netwib_er(netwox_txtproto_read_reply(pftpclient->pio, pftpclient->maxwaitms,
                                       &pftpclient->errnum,
                                       &pftpclient->errmsg));
  netwib_er(netwox_txtproto_expect3(pftpclient->errnum, &pftpclient->errmsg,
                                    NETWOX_FTPREPLY_331_AUTHUSEROK,
                                    NETWOX_FTPREPLY_230_AUTHOK,
                                    NETWOX_FTPREPLY_530_AUTHBAD));
  if (pftpclient->errnum == NETWOX_FTPREPLY_530_AUTHBAD) {
    return(NETWOX_ERR_AUTHERROR);
  }
  if (pftpclient->errnum == NETWOX_FTPREPLY_230_AUTHOK) {
    return(NETWIB_ERR_OK);
  }

  /* PASS */
  password = *ppassword;
  password.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  netwib_er(netwox_ftpcli_pass(pftpclient->pio, &password));
  netwib__buf_reinit(&pftpclient->errmsg);
  netwib_er(netwox_txtproto_read_reply(pftpclient->pio, pftpclient->maxwaitms,
                                       &pftpclient->errnum,
                                       &pftpclient->errmsg));
  netwib_er(netwox_txtproto_expect2(pftpclient->errnum, &pftpclient->errmsg,
                                    NETWOX_FTPREPLY_230_AUTHOK,
                                    NETWOX_FTPREPLY_530_AUTHBAD));
  if (pftpclient->errnum == NETWOX_FTPREPLY_530_AUTHBAD) {
    return(NETWOX_ERR_AUTHERROR);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpclient_login_anonymous(netwox_ftpclient *pftpclient)
{
  netwib_buf user, password;

  netwib_er(netwib_buf_init_ext_string("anonymous", &user));
  netwib_er(netwib_buf_init_ext_string("anon@", &password));

  netwib_er(netwox_ftpclient_login(pftpclient, &user, &password));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpclient_passive(netwox_ftpclient *pftpclient)
{
  pftpclient->passive = NETWIB_TRUE;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpclient_active(netwox_ftpclient *pftpclient)
{
  pftpclient->passive = NETWIB_FALSE;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpclient_bin(netwox_ftpclient *pftpclient)
{
  netwib_er(netwox_ftpcli_type_bin(pftpclient->pio));
  netwib__buf_reinit(&pftpclient->errmsg);
  netwib_er(netwox_txtproto_read_reply(pftpclient->pio, pftpclient->maxwaitms,
                                       &pftpclient->errnum,
                                       &pftpclient->errmsg));
  netwib_er(netwox_txtproto_expect1(pftpclient->errnum, &pftpclient->errmsg,
                                    NETWOX_FTPREPLY_200_CMDOK));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpclient_ascii(netwox_ftpclient *pftpclient)
{
  netwib_er(netwox_ftpcli_type_ascii(pftpclient->pio));
  netwib__buf_reinit(&pftpclient->errmsg);
  netwib_er(netwox_txtproto_read_reply(pftpclient->pio, pftpclient->maxwaitms,
                                       &pftpclient->errnum,
                                       &pftpclient->errmsg));
  netwib_er(netwox_txtproto_expect1(pftpclient->errnum, &pftpclient->errmsg,
                                    NETWOX_FTPREPLY_200_CMDOK));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpclient_file_put(netwox_ftpclient *pftpclient,
                                     netwib_constbuf *plocalfile,
                                     netwib_constbuf *premotefile)
{
  netwib_io *pdataio, *pfileio;
  netwib_buf *pbuf;
  netwib_bool closedataio=NETWIB_FALSE;
  netwib_err ret=NETWIB_ERR_OK;

  /* init */
  netwib_er(netwib_io_init_file_read(plocalfile, &pfileio));
  netwib_eg(netwox_ftpclient_data(pftpclient, &pdataio));
  closedataio = NETWIB_TRUE;

  /* send stor */
  netwib_eg(netwox_ftpcli_stor(pftpclient->pio, premotefile));
  netwib__buf_reinit(&pftpclient->errmsg);
  netwib_eg(netwox_txtproto_read_reply(pftpclient->pio, pftpclient->maxwaitms,
                                       &pftpclient->errnum,
                                       &pftpclient->errmsg));
  netwib_eg(netwox_txtproto_expect1(pftpclient->errnum, &pftpclient->errmsg,
                                    NETWOX_FTPREPLY_150_WILLOPENDATA));

  /* read data */
  netwib_eg(netwib_bufpool_buf_init(pftpclient->pbufpool, &pbuf));
  while(NETWIB_TRUE) {
    netwib__buf_reinit(pbuf);
    ret = netwib_io_read(pfileio, pbuf);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    ret = netwib_io_write(pdataio, pbuf);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_bufpool_buf_close(pftpclient->pbufpool, &pbuf));
  netwib_er(netwib_io_close(&pfileio));
  netwib_er(netwib_io_close(&pdataio));

  /* read error */
  netwib__buf_reinit(&pftpclient->errmsg);
  netwib_er(netwox_txtproto_read_reply(pftpclient->pio, pftpclient->maxwaitms,
                                       &pftpclient->errnum,
                                       &pftpclient->errmsg));
  netwib_er(netwox_txtproto_expect1(pftpclient->errnum, &pftpclient->errmsg,
                                    NETWOX_FTPREPLY_226_DATAEND));
  return(ret);

 netwib_gotolabel:
  netwib_er(netwib_io_close(&pfileio));
  if (closedataio) {
    netwib_er(netwib_io_close(&pdataio));
  }
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpclient_file_get_io(netwox_ftpclient *pftpclient,
                                        netwib_constbuf *premotefile,
                                        netwib_io *plocalio)
{
  netwib_io *pdataio;
  netwib_buf *pbuf;
  netwib_err ret=NETWIB_ERR_OK;

  /* init */
  netwib_er(netwox_ftpclient_data(pftpclient, &pdataio));

  /* send retr */
  netwib_eg(netwox_ftpcli_retr(pftpclient->pio, premotefile));
  netwib__buf_reinit(&pftpclient->errmsg);
  netwib_eg(netwox_txtproto_read_reply(pftpclient->pio, pftpclient->maxwaitms,
                                       &pftpclient->errnum,
                                       &pftpclient->errmsg));
  netwib_eg(netwox_txtproto_expect2(pftpclient->errnum, &pftpclient->errmsg,
                                    NETWOX_FTPREPLY_150_WILLOPENDATA,
                                    NETWOX_FTPREPLY_550_ACTIONNOTTAKEN));
  if (pftpclient->errnum == NETWOX_FTPREPLY_550_ACTIONNOTTAKEN) {
    netwib_eg(NETWIB_ERR_NOTFOUND);
  }

  /* read data */
  netwib_eg(netwib_bufpool_buf_init(pftpclient->pbufpool, &pbuf));
  while(NETWIB_TRUE) {
    netwib__buf_reinit(pbuf);
    ret = netwib_io_read(pdataio, pbuf);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    ret = netwib_io_write(plocalio, pbuf);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_bufpool_buf_close(pftpclient->pbufpool, &pbuf));
  netwib_er(netwib_io_close(&pdataio));

  /* read error */
  netwib__buf_reinit(&pftpclient->errmsg);
  netwib_er(netwox_txtproto_read_reply(pftpclient->pio, pftpclient->maxwaitms,
                                       &pftpclient->errnum,
                                       &pftpclient->errmsg));
  netwib_er(netwox_txtproto_expect1(pftpclient->errnum, &pftpclient->errmsg,
                                    NETWOX_FTPREPLY_226_DATAEND));
  return(ret);

 netwib_gotolabel:
  netwib_er(netwib_io_close(&pdataio));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpclient_file_get(netwox_ftpclient *pftpclient,
                                     netwib_constbuf *premotefile,
                                     netwib_constbuf *plocalfile)
{
  netwib_io *pfileio;
  netwib_err ret;

  netwib_er(netwib_io_init_file_write(plocalfile, &pfileio));
  ret = netwox_ftpclient_file_get_io(pftpclient, premotefile, pfileio);
  netwib_er(netwib_io_close(&pfileio));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpclient_file_delete(netwox_ftpclient *pftpclient,
                                        netwib_constbuf *pfilename)
{
  netwib_er(netwox_ftpcli_dele(pftpclient->pio, pfilename));
  netwib__buf_reinit(&pftpclient->errmsg);
  netwib_er(netwox_txtproto_read_reply(pftpclient->pio, pftpclient->maxwaitms,
                                       &pftpclient->errnum,
                                       &pftpclient->errmsg));
  netwib_er(netwox_txtproto_expect1(pftpclient->errnum, &pftpclient->errmsg,
                                    NETWOX_FTPREPLY_250_FILEACTIONDONE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpclient_file_size(netwox_ftpclient *pftpclient,
                                      netwib_constbuf *pfilename,
                                      netwib_uint32 *psize)
{
  netwib_uint32 size;

  netwib_er(netwox_ftpcli_size(pftpclient->pio, pfilename));
  netwib__buf_reinit(&pftpclient->errmsg);
  netwib_er(netwox_txtproto_read_reply(pftpclient->pio, pftpclient->maxwaitms,
                                       &pftpclient->errnum,
                                       &pftpclient->errmsg));
  netwib_er(netwox_txtproto_expect1(pftpclient->errnum, &pftpclient->errmsg,
                                    NETWOX_FTPREPLY_213_FILESTATUS));
  netwib_er(netwib_buf_decode_fmt(&pftpclient->errmsg, "%{uint32}%$", &size));

  if (psize != NULL) *psize = size;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpclient_dir_mkdir(netwox_ftpclient *pftpclient,
                                      netwib_constbuf *pdirname)
{
  netwib_er(netwox_ftpcli_mkd(pftpclient->pio, pdirname));
  netwib__buf_reinit(&pftpclient->errmsg);
  netwib_er(netwox_txtproto_read_reply(pftpclient->pio, pftpclient->maxwaitms,
                                       &pftpclient->errnum,
                                       &pftpclient->errmsg));
  /* note : we ignore error DIREXISTS because it does not hurts
     (in this case, Proftpd returns FILEUNAVAILABLE) */
  netwib_er(netwox_txtproto_expect4(pftpclient->errnum, &pftpclient->errmsg,
                                    NETWOX_FTPREPLY_250_FILEACTIONDONE,
                                    NETWOX_FTPREPLY_257_FILECREATED,
                                    NETWOX_FTPREPLY_521_DIREXISTS,
                                    NETWOX_FTPREPLY_550_ACTIONNOTTAKEN));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpclient_dir_rmdir(netwox_ftpclient *pftpclient,
                                      netwib_constbuf *pdirname)
{
  netwib_er(netwox_ftpcli_rmd(pftpclient->pio, pdirname));
  netwib__buf_reinit(&pftpclient->errmsg);
  netwib_er(netwox_txtproto_read_reply(pftpclient->pio, pftpclient->maxwaitms,
                                       &pftpclient->errnum,
                                       &pftpclient->errmsg));
  netwib_er(netwox_txtproto_expect1(pftpclient->errnum, &pftpclient->errmsg,
                                    NETWOX_FTPREPLY_250_FILEACTIONDONE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpclient_dir_cd(netwox_ftpclient *pftpclient,
                                   netwib_constbuf *pdirname)
{
  netwib_er(netwox_ftpcli_cwd(pftpclient->pio, pdirname));
  netwib__buf_reinit(&pftpclient->errmsg);
  netwib_er(netwox_txtproto_read_reply(pftpclient->pio, pftpclient->maxwaitms,
                                       &pftpclient->errnum,
                                       &pftpclient->errmsg));
  netwib_er(netwox_txtproto_expect2(pftpclient->errnum, &pftpclient->errmsg,
                                    NETWOX_FTPREPLY_250_FILEACTIONDONE,
                                    NETWOX_FTPREPLY_550_ACTIONNOTTAKEN));
  if (pftpclient->errnum == NETWOX_FTPREPLY_550_ACTIONNOTTAKEN) {
    return(NETWIB_ERR_NOTFOUND);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_ftp_dirlist_init(netwox_ftp_dirlist *pdirlist)
{
  netwib_er(netwib_buf_init_mallocdefault(&pdirlist->name));
  netwib_er(netwib_buf_init_mallocdefault(&pdirlist->linkname));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_ftp_dirlist_close(netwox_ftp_dirlist *pdirlist)
{
  netwib_er(netwib_buf_close(&pdirlist->name));
  netwib_er(netwib_buf_close(&pdirlist->linkname));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftp_dirlist_erase(netwib_ptr pitem)
{
  netwib_er(netwox_ftp_dirlist_close((netwox_ftp_dirlist *)pitem));
  netwib_er(netwib_ptr_free(&pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftp_dirlist_duplicate(netwib_constptr pitem,
                                        netwib_ptr *pdupofitem)
{
  const netwox_ftp_dirlist *pdirlist = (const netwox_ftp_dirlist *)pitem;
  netwox_ftp_dirlist *pdirlistdup;
  netwib_ptr ptr;

  netwib_er(netwib_ptr_malloc(sizeof(netwox_ftp_dirlist), &ptr));
  pdirlistdup = (netwox_ftp_dirlist *)ptr;
  *pdupofitem = ptr;

  netwib_er(netwox_ftp_dirlist_init(pdirlistdup));
  netwib_er(netwib_buf_append_buf(&pdirlist->name, &pdirlistdup->name));
  pdirlistdup->pathstat = pdirlist->pathstat;
  netwib_er(netwib_buf_append_buf(&pdirlist->linkname,
                                  &pdirlistdup->linkname));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_ftpclient_dir_ls(netwib_bufpool *pbufpool,
                                               netwib_constbuf *pline,
                                               netwib_ring *pring)
{
  netwib_buf *pbufmode;
  netwox_ftp_dirlist *pdirlist;
  netwib_data data, pc;
  netwib_uint32 datasize, namesize;
  netwib_string name;
  netwib_ptr ptr;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbufmode));

  netwib_er(netwib_ptr_malloc(sizeof(netwox_ftp_dirlist), &ptr));
  pdirlist = (netwox_ftp_dirlist *)ptr;
  netwib_er(netwox_ftp_dirlist_init(pdirlist));

  /* "mode nlink usergroup size date date date name". Note we use
     "usergroup" with glob because under BSD, there is only one field. */
  ret = netwib_buf_decode_fmt(pline, "%{buf} %{*;uint32} %{*;buf:glob} %{uint32} %{*;buf} %{*;uint32} %{*;buf} %{buf:glob}%$", pbufmode, &pdirlist->pathstat.size, &pdirlist->name);
  if (ret != NETWIB_ERR_OK) {
    /* ignore bad lines */
    netwib_er(netwox_ftp_dirlist_erase(ptr));
    netwib_er(netwib_bufpool_buf_close(pbufpool, &pbufmode));
    return(NETWIB_ERR_OK);
  }

  /* determine pathnametype */
  pdirlist->pathstat.type = NETWIB_PATHSTAT_TYPE_UNKNOWN;
  if (netwib__buf_ref_data_size(pbufmode) != 0) {
    data = netwib__buf_ref_data_ptr(pbufmode);
    switch(data[0]) {
    case '-' : pdirlist->pathstat.type = NETWIB_PATHSTAT_TYPE_REG; break;
    case 'd' : pdirlist->pathstat.type = NETWIB_PATHSTAT_TYPE_DIR; break;
    case 'l' : pdirlist->pathstat.type = NETWIB_PATHSTAT_TYPE_LINK; break;
    case 's' : pdirlist->pathstat.type = NETWIB_PATHSTAT_TYPE_SOCK; break;
    case 'b' : pdirlist->pathstat.type = NETWIB_PATHSTAT_TYPE_BLOCK; break;
    case 'c' : pdirlist->pathstat.type = NETWIB_PATHSTAT_TYPE_CHAR; break;
    case 'f' : pdirlist->pathstat.type = NETWIB_PATHSTAT_TYPE_FIFO; break;
    }
  }

  /* check link */
  if (pdirlist->pathstat.type == NETWIB_PATHSTAT_TYPE_LINK) {
    data = netwib__buf_ref_data_ptr(&pdirlist->name);
    datasize = netwib__buf_ref_data_size(&pdirlist->name);
    pc = netwib_c_memmem(data, datasize, (netwib_constdata)" -> ", 4);
    if (pc != NULL) {
      namesize = pc - data;
      pdirlist->name.endoffset = pdirlist->name.beginoffset + namesize;
      netwib_er(netwib_buf_append_data(pc + 4, datasize - namesize - 4,
                                       &pdirlist->linkname));
    }
  }

  /* add in ring */
  netwib_er(netwib_buf_ref_string(&pdirlist->name, &name));
  if (!netwib_c_strcmp(name, ".") || !netwib_c_strcmp(name, "..")) {
    netwib_er(netwox_ftp_dirlist_erase(ptr));
  } else {
    netwib_er(netwib_ring_add_last(pring, ptr));
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbufmode));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpclient_dir_ls(netwox_ftpclient *pftpclient,
                                   netwib_constbuf *pdirname,
                                   netwib_ring *pring)
{
  netwib_io *pdataio, *plineio;
  netwib_buf *pbuf;
  netwib_err ret=NETWIB_ERR_OK;

  /* init */
  netwib_er(netwox_ftpclient_data(pftpclient, &pdataio));
  netwib_er(netwib_io_init_data_line(&plineio));
  netwib_er(netwib_io_plug_read(plineio, pdataio));

  /* send list */
  netwib_eg(netwox_ftpcli_list(pftpclient->pio, pdirname));
  netwib__buf_reinit(&pftpclient->errmsg);
  netwib_eg(netwox_txtproto_read_reply(pftpclient->pio, pftpclient->maxwaitms,
                                       &pftpclient->errnum,
                                       &pftpclient->errmsg));
  netwib_eg(netwox_txtproto_expect1(pftpclient->errnum, &pftpclient->errmsg,
                                    NETWOX_FTPREPLY_150_WILLOPENDATA));

  /* read data */
  netwib_eg(netwib_bufpool_buf_init(pftpclient->pbufpool, &pbuf));
  while(NETWIB_TRUE) {
    netwib__buf_reinit(pbuf);
    ret = netwib_io_read(plineio, pbuf);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    ret = netwox_priv_ftpclient_dir_ls(pftpclient->pbufpool, pbuf, pring);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_bufpool_buf_close(pftpclient->pbufpool, &pbuf));
  netwib_er(netwib_io_close(&plineio));

  /* read error */
  netwib__buf_reinit(&pftpclient->errmsg);
  netwib_er(netwox_txtproto_read_reply(pftpclient->pio, pftpclient->maxwaitms,
                                       &pftpclient->errnum,
                                       &pftpclient->errmsg));
  netwib_er(netwox_txtproto_expect1(pftpclient->errnum, &pftpclient->errmsg,
                                    NETWOX_FTPREPLY_226_DATAEND));
  return(ret);

 netwib_gotolabel:
  netwib_er(netwib_io_close(&plineio));
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_ftpclient_dirrec_put2(netwox_ftpclient *pftpclient,
                                               netwib_constbuf *plocaldir)
{
  netwib_dir *pdir;
  netwib_buf *pbuffilename, *pbuffullname, dotdot;
  netwib_pathstat pathstat;
  netwib_err ret;

  /* obtain file listing */
  netwib_er(netwib_dir_init(plocaldir, &pdir));
  netwib_er(netwib_bufpool_buf_init(pftpclient->pbufpool, &pbuffilename));
  netwib_er(netwib_bufpool_buf_init(pftpclient->pbufpool, &pbuffullname));

  /* loop on each file/dir/link */
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    /* local filename */
    netwib__buf_reinit(pbuffilename);
    ret = netwib_dir_next(pdir, pbuffilename);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    /* full local path */
    netwib__buf_reinit(pbuffullname);
    netwib_er(netwib_path_init_concat(plocaldir, pbuffilename, pbuffullname));
    /* type of local path */
    netwib_er(netwib_fmt_display("%{buf}\n", pbuffullname));
    netwib_er(netwib_pathstat_init(pbuffullname, &pathstat));
    switch(pathstat.type) {
    case NETWIB_PATHSTAT_TYPE_REG :
      ret = netwox_ftpclient_file_put(pftpclient, pbuffullname, pbuffilename);
      break;
    case NETWIB_PATHSTAT_TYPE_DIR :
      ret = netwox_ftpclient_dir_mkdir(pftpclient, pbuffilename);
      if (ret == NETWIB_ERR_OK) {
        ret = netwox_ftpclient_dir_cd(pftpclient, pbuffilename);
        if (ret == NETWIB_ERR_OK) {
          ret = netwox_ftpclient_dirrec_put2(pftpclient, pbuffullname);
          if (ret == NETWIB_ERR_OK) {
            netwib_er(netwib_buf_init_ext_string("..", &dotdot));
            ret = netwox_ftpclient_dir_cd(pftpclient, &dotdot);
          }
        }
      }
      break;
    default :
      break;
    }
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }

  /* close */
  netwib_er(netwib_bufpool_buf_close(pftpclient->pbufpool, &pbuffullname));
  netwib_er(netwib_bufpool_buf_close(pftpclient->pbufpool, &pbuffilename));
  netwib_er(netwib_dir_close(&pdir));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpclient_dirrec_put(netwox_ftpclient *pftpclient,
                                       netwib_constbuf *plocaldir,
                                       netwib_constbuf *premotedir)
{

  /* create and go in remote dir */
  netwib_er(netwox_ftpclient_dir_mkdir(pftpclient, premotedir));
  netwib_er(netwox_ftpclient_dir_cd(pftpclient, premotedir));

  /* call recursive function */
  netwib_er(netwox_ftpclient_dirrec_put2(pftpclient, plocaldir));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_ftpclient_dirrec_get2(netwox_ftpclient *pftpclient,
                                               netwib_constbuf *plocaldir)
{
  netwib_buf *pbuffullname, dotdot;
  netwib_ring *pring;
  netwib_ring_index *pringindex;
  netwox_ftp_dirlist *pdirlist;
  netwib_ptr pitem;
  netwib_err ret;

  /* obtain file listing */
  netwib_er(netwox_ftp_dirlist_ring_init(&pring));
  netwib_er(netwib_buf_init_ext_string(".", &dotdot));
  netwib_er(netwox_ftpclient_dir_ls(pftpclient, &dotdot, pring));

  netwib_er(netwib_bufpool_buf_init(pftpclient->pbufpool, &pbuffullname));

  /* loop on each file/dir/link */
  netwib_er(netwib_ring_index_init(pring, &pringindex));
  ret = NETWIB_ERR_OK;
  while(NETWIB_TRUE) {
    /* get remote filename */
    ret = netwib_ring_index_next(pringindex, &pitem);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    pdirlist = (netwox_ftp_dirlist *)pitem;
    /* full local path */
    netwib__buf_reinit(pbuffullname);
    netwib_er(netwib_path_init_concat(plocaldir, &pdirlist->name,
                                      pbuffullname));
    netwib_er(netwib_fmt_display("%{buf}\n", pbuffullname));
    /* type of local path */
    switch(pdirlist->pathstat.type) {
    case NETWIB_PATHSTAT_TYPE_REG :
      ret = netwox_ftpclient_file_get(pftpclient, &pdirlist->name,
                                      pbuffullname);
      break;
    case NETWIB_PATHSTAT_TYPE_DIR :
      ret = netwib_dirname_create(pbuffullname);
      if (ret == NETWIB_ERR_FUMKDIR) {
        /* local dir exist : it's ok for us */
        ret = NETWIB_ERR_OK;
      }
      if (ret == NETWIB_ERR_OK) {
        ret = netwox_ftpclient_dir_cd(pftpclient, &pdirlist->name);
        if (ret == NETWIB_ERR_OK) {
          ret = netwox_ftpclient_dirrec_get2(pftpclient, pbuffullname);
          if (ret == NETWIB_ERR_OK) {
            netwib_er(netwib_buf_init_ext_string("..", &dotdot));
            ret = netwox_ftpclient_dir_cd(pftpclient, &dotdot);
          }
        }
      }
      break;
    case NETWIB_PATHSTAT_TYPE_LINK :
      ret = netwib_unix_symlink(&pdirlist->linkname, pbuffullname);
      if (ret == NETWIB_ERR_LONOTSUPPORTED) {
        /* on Windows */
        ret = NETWIB_ERR_OK;
      }
      break;
    default :
      break;
    }
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwib_fmt_display("Ignore error: "));
      netwib_er(netwox_err_display(ret, NETWIB_ERR_ENCODETYPE_TEXT));
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));

  /* close */
  netwib_er(netwib_bufpool_buf_close(pftpclient->pbufpool, &pbuffullname));
  netwib_er(netwox_ftp_dirlist_ring_close(&pring));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpclient_dirrec_get(netwox_ftpclient *pftpclient,
                                       netwib_constbuf *premotedir,
                                       netwib_constbuf *plocaldir)
{
  /* go in remote dir */
  netwib_er(netwox_ftpclient_dir_cd(pftpclient, premotedir));

  /* call recursive function */
  netwib_er(netwox_ftpclient_dirrec_get2(pftpclient, plocaldir));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_ftpclient_dirrec_rmdir2(netwox_ftpclient *pftpclient)
{
  netwib_buf dotdot;
  netwib_ring *pring;
  netwib_ring_index *pringindex;
  netwox_ftp_dirlist *pdirlist;
  netwib_ptr pitem;
  netwib_err ret;

  /* obtain file listing */
  netwib_er(netwox_ftp_dirlist_ring_init(&pring));
  netwib_er(netwib_buf_init_ext_string(".", &dotdot));
  netwib_er(netwox_ftpclient_dir_ls(pftpclient, &dotdot, pring));

  /* loop on each file/dir/link */
  netwib_er(netwib_ring_index_init(pring, &pringindex));
  ret = NETWIB_ERR_OK;
  while(NETWIB_TRUE) {
    /* get remote filename */
    ret = netwib_ring_index_next(pringindex, &pitem);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    pdirlist = (netwox_ftp_dirlist *)pitem;
    /* partial display */
    /* type of local path */
    switch(pdirlist->pathstat.type) {
    case NETWIB_PATHSTAT_TYPE_REG :
    case NETWIB_PATHSTAT_TYPE_LINK :
      netwib_er(netwib_fmt_display("Deleting file %{buf}\n", &pdirlist->name));
      ret = netwox_ftpclient_file_delete(pftpclient, &pdirlist->name);
      break;
    case NETWIB_PATHSTAT_TYPE_DIR :
      netwib_er(netwib_fmt_display("Entering directory %{buf}\n", &pdirlist->name));
      ret = netwox_ftpclient_dir_cd(pftpclient, &pdirlist->name);
      if (ret == NETWIB_ERR_OK) {
        ret = netwox_ftpclient_dirrec_rmdir2(pftpclient);
        if (ret == NETWIB_ERR_OK) {
          netwib_er(netwib_fmt_display("Leaving directory %{buf}\n", &pdirlist->name));
          netwib_er(netwib_buf_init_ext_string("..", &dotdot));
          ret = netwox_ftpclient_dir_cd(pftpclient, &dotdot);
          if (ret == NETWIB_ERR_OK) {
            netwib_er(netwib_fmt_display("Deleting directory %{buf}\n",
                                         &pdirlist->name));
            ret = netwox_ftpclient_dir_rmdir(pftpclient, &pdirlist->name);
          }
        }
      }
      break;
    default :
      break;
    }
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_ring_index_close(&pringindex));

  /* close */
  netwib_er(netwox_ftp_dirlist_ring_close(&pring));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ftpclient_dirrec_rmdir(netwox_ftpclient *pftpclient,
                                         netwib_constbuf *pdirname)
{
  netwib_buf dotdot, lastdir;
  netwib_string name;
  netwib_err ret;

  /* go in remote dir */
  netwib_er(netwib_fmt_display("Entering directory %{buf}\n", pdirname));
  netwib_er(netwox_ftpclient_dir_cd(pftpclient, pdirname));

  /* call recursive function */
  netwib_er(netwox_ftpclient_dirrec_rmdir2(pftpclient));

  /* try to remove directory. "try" because / or . cause problems */
  netwib_er(netwib_constbuf_ref_string(pdirname, &name));
  if (netwib_c_strcmp(name, ".") && netwib_c_strcmp(name, "/")) {
    netwib_er(netwib_fmt_display("Deleting directory %{buf}\n", pdirname));
    netwib_er(netwib_buf_init_ext_string("..", &dotdot));
    ret = netwox_ftpclient_dir_cd(pftpclient, &dotdot);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_buf_init_mallocdefault(&lastdir));
      netwib_er(netwib_path_decode_child(pdirname, &lastdir));
      ret = netwox_ftpclient_dir_rmdir(pftpclient, &lastdir);
      netwib_er(netwib_buf_close(&lastdir));
    }
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwib_fmt_display("Directory not deleted.\n"));
    }
  }

  return(NETWIB_ERR_OK);
}
