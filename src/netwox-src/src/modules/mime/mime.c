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
netwib_err netwox_mime_version(netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_string("MIME-Version: 1.0\r\n", pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* If word's encoded size is greater than 75 (rfc 2047 chapter 2.),
   we will generate "encoded-word" too long (ie not RFC compliant).
   As this case should be quite unusual, we ignore it. */
static netwib_err netwox_priv_mime_hdrencode_word(netwib_constbuf *ptext,
                                                  netwox_mime_hdrencode_type type,
                                                  netwib_buf *pbuf)
{
  netwib_data data;
  netwib_uint32 datasize, quartet;

  switch(type) {
  case NETWOX_MIME_HDRENCODE_TYPE_BASE64 :
    netwib_er(netwib_buf_append_string("=?ISO-8859-1?B?", pbuf));
    netwib_er(netwib_buf_encode(ptext, NETWIB_ENCODETYPE_BASE64, pbuf));
    netwib_er(netwib_buf_append_string("?=", pbuf));
    break;
  case NETWOX_MIME_HDRENCODE_TYPE_QUOTEDPRINTABLE :
    netwib_er(netwib_buf_append_string("=?ISO-8859-1?Q?", pbuf));
    data = netwib__buf_ref_data_ptr(ptext);
    datasize = netwib__buf_ref_data_size(ptext);
    while(datasize) {
      if (netwib_c2_isprint(*data) && (*data != '=') && (*data != '?')) {
        netwib_er(netwib_buf_append_byte(*data, pbuf));
      } else {
        netwib_er(netwib_buf_append_byte('=', pbuf));
        quartet = *data >> 4;
        netwib_er(netwib_buf_append_byte(netwib_c2_16toC(quartet), pbuf));
        quartet = *data & 0x0F;
        netwib_er(netwib_buf_append_byte(netwib_c2_16toC(quartet), pbuf));
      }
      data++;
      datasize--;
    }
    netwib_er(netwib_buf_append_string("?=", pbuf));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_mime_hdrencode(netwib_constbuf *ptext,
                                 netwox_mime_hdrencode_type type,
                                 netwib_buf *pbuf)
{
  netwib_data data, datastart;
  netwib_uint32 datasize, wordsize;
  netwib_buf text;
  netwib_bool needtoencode;

  data = netwib__buf_ref_data_ptr(ptext);
  datasize = netwib__buf_ref_data_size(ptext);
  datastart = data;
  needtoencode = NETWIB_FALSE;
  while(NETWIB_TRUE) {
    if (datasize == 0 || *data == ' ') {
      /* end or space delimiter */
      wordsize = data - datastart;
      if (wordsize) {
        if (needtoencode) {
          netwib_er(netwib_buf_init_ext_arrayfilled(datastart, wordsize,
                                                    &text));
          netwib_er(netwox_priv_mime_hdrencode_word(&text, type, pbuf));
        } else {
          netwib_er(netwib_buf_append_data(datastart, wordsize, pbuf));
        }
      }
      if (datasize == 0) {
        /* was end of data */
        break;
      }
      /* add a space, and prepare for next word */
      netwib_er(netwib_buf_append_byte(' ', pbuf));
      datastart = data + 1;
      needtoencode = NETWIB_FALSE;
    } else if ( ! netwib_c2_isprint(*data) ) {
      /* this word contain at least one bad char, so it will need to be
         encoded */
      needtoencode = NETWIB_TRUE;
    }
    data++;
    datasize--;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_mime_ct(netwib_conststring type,
                          netwib_constbuf *pname,
                          netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_string("Content-type: ", pbuf));
  netwib_er(netwib_buf_append_string(type, pbuf));
  if (pname != NULL) {
    netwib_er(netwib_buf_append_string("; name=\"", pbuf));
    netwib_er(netwib_buf_append_buf(pname, pbuf));
    netwib_er(netwib_buf_append_string("\"", pbuf));
  }
  netwib_er(netwib_buf_append_string("\r\n", pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_mime_cte(netwib_conststring type,
                           netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_string("Content-Transfer-Encoding: ", pbuf));
  netwib_er(netwib_buf_append_string(type, pbuf));
  netwib_er(netwib_buf_append_string("\r\n", pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_mime_cd(netwib_conststring type,
                          netwib_constbuf *pfilename,
                          netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_string("Content-Disposition: ", pbuf));
  netwib_er(netwib_buf_append_string(type, pbuf));
  if (pfilename != NULL) {
    netwib_er(netwib_buf_append_string("; filename=\"", pbuf));
    netwib_er(netwib_buf_append_buf(pfilename, pbuf));
    netwib_er(netwib_buf_append_string("\"", pbuf));
  }
  netwib_er(netwib_buf_append_string("\r\n", pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_mime_mp_begin(netwib_buf *pboundary,
                                netwib_buf *pbuf)
{
  /* generate boundary */
  netwib__buf_reinit(pboundary);
  netwib_er(netwib_buf_append_rand(32, 'a', 'z', pboundary));

  netwib_er(netwib_buf_append_string("Content-type: multipart/mixed; boundary=\"", pbuf));
  netwib_er(netwib_buf_append_buf(pboundary, pbuf));
  netwib_er(netwib_buf_append_string("\"\r\n", pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_mime_mp_next(netwib_constbuf *pboundary,
                               netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_string("--", pbuf));
  netwib_er(netwib_buf_append_buf(pboundary, pbuf));
  netwib_er(netwib_buf_append_string("\r\n", pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_mime_mp_end(netwib_constbuf *pboundary,
                              netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_string("--", pbuf));
  netwib_er(netwib_buf_append_buf(pboundary, pbuf));
  netwib_er(netwib_buf_append_string("--\r\n", pbuf));
  return(NETWIB_ERR_OK);
}

