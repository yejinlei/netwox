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
netwib_err netwox_mailex_io_part_8bit(netwib_io *piobody,
                                      netwib_buf *pemail)
{
  netwib_io *pioline;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_io_init_data_line(&pioline));
  netwib_er(netwib_io_plug_read(pioline, piobody));

  while(NETWIB_TRUE) {
    ret = netwib_io_read(pioline, pemail);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_er(netwib_buf_append_string("\r\n", pemail));
  }

  netwib_er(netwib_io_unplug_next_read(pioline, NULL));
  netwib_er(netwib_io_close(&pioline));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_mailex_io_part_base64(netwib_io *pioatt,
                                        netwib_buf *pemail)
{
  netwib_buf buf;
  netwib_uint32 endoffset;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  buf.flags |= NETWIB_BUF_FLAGS_CANSLIDE;

  while(NETWIB_TRUE) {
    /* read */
    ret = netwib_io_read(pioatt, &buf);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    /* line are 72 bytes length (54 bytes encoded) : only encode
       complete lines */
#define NETWOX_ATT_B64_LEN 54
    if (netwib__buf_ref_data_size(&buf) >= NETWOX_ATT_B64_LEN) {
      endoffset = buf.endoffset;
      buf.endoffset = buf.beginoffset + NETWOX_ATT_B64_LEN;
      do {
        netwib_er(netwib_buf_encode(&buf, NETWIB_ENCODETYPE_BASE64, pemail));
        netwib_er(netwib_buf_append_string("\r\n", pemail));
        buf.beginoffset += NETWOX_ATT_B64_LEN;
        buf.endoffset += NETWOX_ATT_B64_LEN;
      } while (buf.endoffset <= endoffset);
      buf.endoffset = endoffset;
    }
  }

  if (netwib__buf_ref_data_size(&buf)) {
    /* last part */
    netwib_er(netwib_buf_encode(&buf, NETWIB_ENCODETYPE_BASE64, pemail));
    netwib_er(netwib_buf_append_string("\r\n", pemail));
  }

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_mailex_att(netwib_constbuf *pattfilename,
                                         netwib_bufext *pattfile)
{
  netwib_data data;
  netwib_uint32 datasize;

  if (pattfilename == NULL) {
    netwib_er(netwib_buf_init_ext_string("filename", pattfile));
    return(NETWIB_ERR_OK);
  }

  *pattfile = *pattfilename;

  data = netwib__buf_ref_data_ptr(pattfilename);
  datasize = netwib__buf_ref_data_size(pattfilename);
  while (datasize--) {
    if (data[datasize] == '/' || data[datasize] == '\\' ||
        data[datasize] == ':') {
      pattfile->beginoffset += datasize + 1;
      break;
    }
  }

  if (netwib__buf_ref_data_size(pattfile) == 0) {
    netwib_er(netwib_buf_init_ext_string("filename", pattfile));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_mailex_sub(netwib_constbuf *pfrom,
                             netwib_constbuf *pfromname,
                             netwib_bool toisanewgroup,
                             netwib_constbuf *pto,
                             netwib_constbuf *psubject,
                             netwib_buf *pemail)
{
  netwib_buf domain;

  netwib_er(netwox_mail_date(pemail));
  netwib_er(netwox_mail_domain_init_email(pfrom, &domain));
  netwib_er(netwox_mail_messageid(&domain, pemail));
  netwib_er(netwox_mime_version(pemail));
  netwib_er(netwox_mail_email_from(pfromname, pfrom, pemail));
  if (toisanewgroup) {
    netwib_er(netwox_mail_newsgroups(pto, pemail));
  } else {
    netwib_er(netwox_mail_email_to(NULL, pto, pemail));
  }
  netwib_er(netwox_mail_subject(psubject, pemail));
  netwib_er(netwox_mail_endheader(pemail));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_mailex_subbody(netwib_constbuf *pfrom,
                                 netwib_constbuf *pfromname,
                                 netwib_bool toisanewgroup,
                                 netwib_constbuf *pto,
                                 netwib_constbuf *psubject,
                                 netwib_io *piobody,
                                 netwib_buf *pemail)
{
  netwib_buf domain;

  netwib_er(netwox_mail_date(pemail));
  netwib_er(netwox_mail_domain_init_email(pfrom, &domain));
  netwib_er(netwox_mail_messageid(&domain, pemail));
  netwib_er(netwox_mime_version(pemail));
  netwib_er(netwox_mail_email_from(pfromname, pfrom, pemail));
  if (toisanewgroup) {
    netwib_er(netwox_mail_newsgroups(pto, pemail));
  } else {
    netwib_er(netwox_mail_email_to(NULL, pto, pemail));
  }
  netwib_er(netwox_mail_subject(psubject, pemail));
  netwib_er(netwox_mime_ct_textplainiso88591(NULL, pemail));
  netwib_er(netwox_mime_cte_8bit(pemail));
  netwib_er(netwox_mail_endheader(pemail));

  netwib_er(netwox_mailex_io_part_8bit(piobody, pemail));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_mailex_subatt(netwib_constbuf *pfrom,
                                netwib_constbuf *pfromname,
                                netwib_bool toisanewgroup,
                                netwib_constbuf *pto,
                                netwib_constbuf *psubject,
                                netwib_constbuf *pattfilename,
                                netwib_io *pioatt,
                                netwib_buf *pemail)
{
  netwib_buf domain, attfilename;

  netwib_er(netwox_priv_mailex_att(pattfilename, &attfilename));

  netwib_er(netwox_mail_date(pemail));
  netwib_er(netwox_mail_domain_init_email(pfrom, &domain));
  netwib_er(netwox_mail_messageid(&domain, pemail));
  netwib_er(netwox_mime_version(pemail));
  netwib_er(netwox_mail_email_from(pfromname, pfrom, pemail));
  if (toisanewgroup) {
    netwib_er(netwox_mail_newsgroups(pto, pemail));
  } else {
    netwib_er(netwox_mail_email_to(NULL, pto, pemail));
  }
  netwib_er(netwox_mail_subject(psubject, pemail));
  netwib_er(netwox_mime_ct_appoctetstream(&attfilename, pemail));
  netwib_er(netwox_mime_cte_base64(pemail));
  netwib_er(netwox_mime_cd_attachment(&attfilename, pemail));
  netwib_er(netwox_mail_endheader(pemail));

  netwib_er(netwox_mailex_io_part_base64(pioatt, pemail));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_mailex_subfwd(netwib_constbuf *pfrom,
                                netwib_constbuf *pfromname,
                                netwib_bool toisanewgroup,
                                netwib_constbuf *pto,
                                netwib_constbuf *psubject,
                                netwib_constbuf *pfwdfilename,
                                netwib_io *piofwd,
                                netwib_buf *pemail)
{
  netwib_buf domain, fwdfilename;

  netwib_er(netwox_priv_mailex_att(pfwdfilename, &fwdfilename));

  netwib_er(netwox_mail_date(pemail));
  netwib_er(netwox_mail_domain_init_email(pfrom, &domain));
  netwib_er(netwox_mail_messageid(&domain, pemail));
  netwib_er(netwox_mime_version(pemail));
  netwib_er(netwox_mail_email_from(pfromname, pfrom, pemail));
  if (toisanewgroup) {
    netwib_er(netwox_mail_newsgroups(pto, pemail));
  } else {
    netwib_er(netwox_mail_email_to(NULL, pto, pemail));
  }
  netwib_er(netwox_mail_subject(psubject, pemail));
  netwib_er(netwox_mime_ct_messagerfc822iso88591(&fwdfilename, pemail));
  netwib_er(netwox_mime_cte_8bit(pemail));
  netwib_er(netwox_mail_endheader(pemail));

  netwib_er(netwox_mailex_io_part_8bit(piofwd, pemail));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_mailex_subbodyattfwd(netwib_constbuf *pfrom,
                                       netwib_constbuf *pfromname,
                                       netwib_bool toisanewgroup,
                                       netwib_constbuf *pto,
                                       netwib_constbuf *psubject,
                                       netwib_io *piobody,
                                       netwib_constbuf *pattfilename,
                                       netwib_io *pioatt,
                                       netwib_constbuf *pfwdfilename,
                                       netwib_io *piofwd,
                                       netwib_buf *pemail)
{
  netwib_buf boundary, domain, attfilename, fwdfilename;

  netwib_er(netwox_priv_mailex_att(pattfilename, &attfilename));
  netwib_er(netwox_priv_mailex_att(pfwdfilename, &fwdfilename));

  netwib_er(netwib_buf_init_mallocdefault(&boundary));

  netwib_er(netwox_mail_date(pemail));
  netwib_er(netwox_mail_domain_init_email(pfrom, &domain));
  netwib_er(netwox_mail_messageid(&domain, pemail));
  netwib_er(netwox_mime_version(pemail));
  netwib_er(netwox_mail_email_from(pfromname, pfrom, pemail));
  if (toisanewgroup) {
    netwib_er(netwox_mail_newsgroups(pto, pemail));
  } else {
    netwib_er(netwox_mail_email_to(NULL, pto, pemail));
  }
  netwib_er(netwox_mail_subject(psubject, pemail));
  netwib_er(netwox_mime_mp_begin(&boundary, pemail));
  netwib_er(netwox_mail_endheader(pemail));

  if (piobody != NULL) {
    netwib_er(netwox_mime_mp_next(&boundary, pemail));
    netwib_er(netwox_mime_ct_textplainiso88591(NULL, pemail));
    netwib_er(netwox_mime_cte_8bit(pemail));
    netwib_er(netwox_mail_endheader(pemail));
    netwib_er(netwox_mailex_io_part_8bit(piobody, pemail));
  }

  if (pioatt != NULL) {
    netwib_er(netwox_mime_mp_next(&boundary, pemail));
    netwib_er(netwox_mime_ct_appoctetstream(&attfilename, pemail));
    netwib_er(netwox_mime_cte_base64(pemail));
    netwib_er(netwox_mime_cd_attachment(&attfilename, pemail));
    netwib_er(netwox_mail_endheader(pemail));
    netwib_er(netwox_mailex_io_part_base64(pioatt, pemail));
  }

  if (piofwd != NULL) {
    /* rfc 2046 says that message/rfc822 can only be in 7bit/8bit
       (not base64)*/
    netwib_er(netwox_mime_mp_next(&boundary, pemail));
    netwib_er(netwox_mime_ct_messagerfc822iso88591(&fwdfilename, pemail));
    netwib_er(netwox_mime_cd_inline(pemail));
    netwib_er(netwox_mime_cte_8bit(pemail));
    netwib_er(netwox_mail_endheader(pemail));
    netwib_er(netwox_mailex_io_part_8bit(piofwd, pemail));
  }

  netwib_er(netwox_mime_mp_end(&boundary, pemail));

  netwib_er(netwib_buf_close(&boundary));

  return(NETWIB_ERR_OK);
}
