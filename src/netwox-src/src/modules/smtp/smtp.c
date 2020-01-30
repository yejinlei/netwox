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
netwib_err netwox_smtpcmd_init_buf(netwib_constbuf *pbuf,
                                   netwox_smtpcmd *psmtpcmd)
{
  netwib_data data;
  netwib_uint32 datasize;
  netwib_byte firstchar;

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);

  if (datasize == 0) {
    if (psmtpcmd != NULL) *psmtpcmd = NETWOX_SMTPCMD_UNKNOWN;
    return(NETWIB_ERR_OK);
  }

  firstchar = data[0];
#define netwox_smtpcmd_init_buf_text(item,v) if (!netwib_c_memcasecmp(data, (netwib_constdata)item, datasize)) { if (psmtpcmd != NULL) *psmtpcmd = v; return(NETWIB_ERR_OK);}
  switch(firstchar) {
  case 'd' :
  case 'D' :
    if (datasize == 4) {
      netwox_smtpcmd_init_buf_text("DATA", NETWOX_SMTPCMD_DATA);
    }
    break;
  case 'e' :
  case 'E' :
    if (datasize == 4) {
      netwox_smtpcmd_init_buf_text("EHLO", NETWOX_SMTPCMD_EHLO);
      netwox_smtpcmd_init_buf_text("EXPN", NETWOX_SMTPCMD_EXPN);
    }
    break;
  case 'h' :
  case 'H' :
    if (datasize == 4) {
      netwox_smtpcmd_init_buf_text("HELO", NETWOX_SMTPCMD_HELO);
      netwox_smtpcmd_init_buf_text("HELP", NETWOX_SMTPCMD_HELP);
    }
    break;
  case 'm' :
  case 'M' :
    if (datasize == 4) {
      netwox_smtpcmd_init_buf_text("MAIL", NETWOX_SMTPCMD_MAIL);
    }
    break;
  case 'n' :
  case 'N' :
    if (datasize == 4) {
      netwox_smtpcmd_init_buf_text("NOOP", NETWOX_SMTPCMD_NOOP);
    }
    break;
  case 'r' :
  case 'R' :
    if (datasize == 4) {
      netwox_smtpcmd_init_buf_text("RCPT", NETWOX_SMTPCMD_RCPT);
      netwox_smtpcmd_init_buf_text("RSET", NETWOX_SMTPCMD_RSET);
    }
    break;
  case 'q' :
  case 'Q' :
    if (datasize == 4) {
      netwox_smtpcmd_init_buf_text("QUIT", NETWOX_SMTPCMD_QUIT);
    }
    break;
  case 'v' :
  case 'V' :
    if (datasize == 4) {
      netwox_smtpcmd_init_buf_text("VRFY", NETWOX_SMTPCMD_VRFY);
    }
    break;
  }

  if (psmtpcmd != NULL) *psmtpcmd = NETWOX_SMTPCMD_UNKNOWN;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smtp_write_helo(netwib_io *pio,
                                  netwib_constbuf *pdomain)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));

  netwib_er(netwib_buf_append_string("HELO ", &buf));
  netwib_er(netwib_buf_append_buf(pdomain, &buf));
  netwib_er(netwib_buf_append_string("\r\n", &buf));

  ret = netwib_io_write(pio, &buf);

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smtp_write_email(netwib_io *pio,
                                   netwib_conststring cmd,
                                   netwib_constbuf *pemail)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));

  netwib_er(netwib_buf_append_string(cmd, &buf));
  netwib_er(netwib_buf_append_string(":<", &buf));
  netwib_er(netwib_buf_append_buf(pemail, &buf));
  netwib_er(netwib_buf_append_string(">\r\n", &buf));

  ret = netwib_io_write(pio, &buf);

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smtp_write_data_begin(netwib_io *pio)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_ext_string("DATA\r\n", &buf));
  netwib_er(netwib_io_write(pio, &buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smtp_write_data_content(netwib_io *pio,
                                          netwib_constbuf *pdata)
{
  netwib_buf buf;
  netwib_data data, pc;
  netwib_uint32 datasize, chunksize;

  data = netwib__buf_ref_data_ptr(pdata);
  datasize = netwib__buf_ref_data_size(pdata);
  while(NETWIB_TRUE) {
    pc = netwib_c_memmem(data, datasize, (netwib_constdata)"\n.", 2);
    if (pc != NULL) {
      chunksize = pc - data;
      netwib_er(netwib_buf_init_ext_arrayfilled(data, chunksize + 2, &buf));
      netwib_er(netwib_io_write(pio, &buf));
      /* +1 only to double the dot */
      data += chunksize + 1;
      datasize -= chunksize + 1;
    } else {
      netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, &buf));
      netwib_er(netwib_io_write(pio, &buf));
      break;
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smtp_write_data_end(netwib_io *pio)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_ext_string(".\r\n", &buf));
  netwib_er(netwib_io_write(pio, &buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smtp_write_quit(netwib_io *pio)
{
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string("QUIT\r\n", &buf));
  ret = netwib_io_write(pio, &buf);
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smtp_send(netwib_bufpool *pbufpool,
                            netwib_io *pio,
                            netwib_uint32 maxwaitms,
                            netwib_constbuf *pmailfrom,
                            netwib_constbuf *prcptto,
                            netwib_constbuf *pdata)
{
  netwib_buf *perrmsg, domain;
  netwib_uint32 errnum;
  netwib_err ret=NETWIB_ERR_OK;

  /* obtain domain "user@example.com" -> "example.com" */
  netwib_er(netwox_mail_domain_init_email(pmailfrom, &domain));

  /* send mail */
  netwib_er(netwib_bufpool_buf_init(pbufpool, &perrmsg));

  netwib_eg(netwox_txtproto_read_reply(pio, maxwaitms, &errnum, perrmsg));
  netwib_eg(netwox_txtproto_expect1(errnum, perrmsg,
                                    NETWOX_SMTPREPLY_220_SVCREADY));

  netwib_eg(netwox_smtp_write_helo(pio, &domain));
  netwib__buf_reinit(perrmsg);
  netwib_eg(netwox_txtproto_read_reply(pio, maxwaitms, &errnum, perrmsg));
  netwib_eg(netwox_txtproto_expect1(errnum, perrmsg,
                                    NETWOX_SMTPREPLY_250_OK));

  netwib_eg(netwox_smtp_write_email_mailfrom(pio, pmailfrom));
  netwib__buf_reinit(perrmsg);
  netwib_eg(netwox_txtproto_read_reply(pio, maxwaitms, &errnum, perrmsg));
  netwib_eg(netwox_txtproto_expect1(errnum, perrmsg,
                                    NETWOX_SMTPREPLY_250_OK));

  netwib_eg(netwox_smtp_write_email_rcptto(pio, prcptto));
  netwib__buf_reinit(perrmsg);
  netwib_eg(netwox_txtproto_read_reply(pio, maxwaitms, &errnum, perrmsg));
  netwib_eg(netwox_txtproto_expect2(errnum, perrmsg,
                                    NETWOX_SMTPREPLY_250_OK,
                                    NETWOX_SMTPREPLY_251_OKNOTLOCAL));

  netwib_eg(netwox_smtp_write_data_begin(pio));
  netwib__buf_reinit(perrmsg);
  netwib_eg(netwox_txtproto_read_reply(pio, maxwaitms, &errnum, perrmsg));
  netwib_eg(netwox_txtproto_expect1(errnum, perrmsg,
                                    NETWOX_SMTPREPLY_354_OKSENDDATA));

  netwib_eg(netwox_smtp_write_data_content(pio, pdata));
  netwib_eg(netwox_smtp_write_data_end(pio));
  netwib__buf_reinit(perrmsg);
  netwib_eg(netwox_txtproto_read_reply(pio, maxwaitms, &errnum, perrmsg));
  netwib_eg(netwox_txtproto_expect1(errnum, perrmsg,
                                    NETWOX_SMTPREPLY_250_OK));

  netwib_eg(netwox_smtp_write_quit(pio));
  netwib__buf_reinit(perrmsg);
  netwib_eg(netwox_txtproto_read_reply(pio, maxwaitms, &errnum, perrmsg));
  netwib_eg(netwox_txtproto_expect1(errnum, perrmsg,
                                    NETWOX_SMTPREPLY_221_SVCCLOSING));

 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(pbufpool, &perrmsg));
  return(ret);
}
