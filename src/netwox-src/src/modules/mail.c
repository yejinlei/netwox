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
netwib_err netwox_mail_date(netwib_buf *pbuf)
{
  netwib_time t;

  netwib_er(netwib_time_init_now(&t));

  netwib_er(netwib_buf_append_string("Date: ", pbuf));
  netwib_er(netwox_date_rfc822(&t, pbuf));
  netwib_er(netwib_buf_append_string("\r\n", pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_mail_subject(netwib_constbuf *psubject,
                               netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_string("Subject: ", pbuf));
  netwib_er(netwox_mime_hdrencode_q(psubject, pbuf));
  netwib_er(netwib_buf_append_string("\r\n", pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_mail_newsgroups(netwib_constbuf *pnewsgroups,
                                  netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_string("Newsgroups: ", pbuf));
  netwib_er(netwox_mime_hdrencode_q(pnewsgroups, pbuf));
  netwib_er(netwib_buf_append_string("\r\n", pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_mail_messageid(netwib_constbuf *pdomain,
                                 netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_string("Message-ID: <", pbuf));
  netwib_er(netwib_buf_append_rand(16, 'a', 'z', pbuf));
  netwib_er(netwib_buf_append_byte('.', pbuf));
  netwib_er(netwib_buf_append_rand(16, 'A', 'Z', pbuf));
  netwib_er(netwib_buf_append_byte('@', pbuf));
  netwib_er(netwib_buf_append_buf(pdomain, pbuf));
  netwib_er(netwib_buf_append_string(">\r\n", pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_mail_domain_init_email(netwib_constbuf *pemail,
                                         netwib_bufext *pdomain)
{
  netwib_data data, pc;
  netwib_uint32 datasize;

  data = netwib__buf_ref_data_ptr(pemail);
  datasize = netwib__buf_ref_data_size(pemail);
  pc = netwib_c_memchr(data, '@', datasize);
  if (pc != NULL) {
    datasize -= pc - data + 1;
    data = pc + 1;
  }
  netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, pdomain));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_mail_email(netwib_conststring hdr,
                             netwib_constbuf *pname,
                             netwib_constbuf *pemail,
                             netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_string(hdr, pbuf));
  if (pname != NULL && netwib__buf_ref_data_size(pname) != 0) {
    netwib_er(netwib_buf_append_string(": \"", pbuf));
    netwib_er(netwox_mime_hdrencode_q(pname, pbuf));
    netwib_er(netwib_buf_append_string("\" <", pbuf));
  } else {
    netwib_er(netwib_buf_append_string(": <", pbuf));
  }
  netwib_er(netwib_buf_append_buf(pemail, pbuf));
  netwib_er(netwib_buf_append_string(">\r\n", pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_mail_endheader(netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_string("\r\n", pbuf));
  return(NETWIB_ERR_OK);
}
