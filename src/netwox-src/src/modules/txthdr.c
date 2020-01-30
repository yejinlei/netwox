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
netwib_err netwox_txthdr_decode(netwib_constbuf *pline,
                                netwib_conststring hdr,
                                netwib_bool *pdecoded,
                                netwib_bufext *pvalue)
{
  netwib_data data;
  netwib_uint32 datasize, strlenhdr;

  strlenhdr = netwib_c_strlen(hdr);

  /* line too short "hdr: value" */
  datasize = netwib__buf_ref_data_size(pline);
  if (datasize < strlenhdr + 2) {
    *pdecoded = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  }

  data = netwib__buf_ref_data_ptr(pline);
  if (netwib_c_memcasecmp(data, (netwib_constdata)hdr, strlenhdr)) {
    *pdecoded = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  }
  if (netwib_c_memcmp(data + strlenhdr, (netwib_constdata)": ", 2)) {
    *pdecoded = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  }

  strlenhdr += 2;
  netwib_er(netwib_buf_init_ext_arrayfilled(data + strlenhdr,
                                            datasize - strlenhdr, pvalue));
  *pdecoded = NETWIB_TRUE;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_txthdr_decode_hostport(netwib_constbuf *pline,
                                         netwib_conststring hdr,
                                         netwib_bool *pdecoded,
                                         netwib_bufext *phost,
                                         netwib_port *pport)
{
  netwib_buf value, buf;
  netwib_data data, pc;
  netwib_uint32 datasize, hostsize;
  netwib_port port;
  netwib_err ret;

  netwib_er(netwox_txthdr_decode(pline, hdr, pdecoded, &value));
  if (!*pdecoded) {
    return(NETWIB_ERR_OK);
  }

  data = netwib__buf_ref_data_ptr(&value);
  datasize = netwib__buf_ref_data_size(&value);
  pc = netwib_c_memchr(data, ':', datasize);
  if (pc != NULL) {
    hostsize = pc - data;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, hostsize, phost));
    netwib_er(netwib_buf_init_ext_arrayfilled(pc + 1, datasize - hostsize - 1,
                                              &buf));
    ret = netwib_buf_decode_fmt(&buf, "%{port}%$", &port);
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      *pdecoded = NETWIB_FALSE;
      return(NETWIB_ERR_OK);
    } else if (ret != NETWIB_ERR_OK) {
      return(ret);
    }
  } else {
    netwib_er(netwib_buf_init_ext_buf(&value, phost));
    port = NETWOX_SERVICES_HTTP;
  }

  if (pport != NULL) *pport = port;
  *pdecoded = NETWIB_TRUE;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_txthdr_decode_authbasic(netwib_bufpool *pbufpool,
                                          netwib_constbuf *pline,
                                          netwib_conststring hdr,
                                          netwib_bool *pdecoded,
                                          netwib_buf *puser,
                                          netwib_buf *ppassword)
{
  netwib_buf value, *pbuf;
  netwib_data data, pc;
  netwib_uint32 datasize, usersize, strlenbasic;
  netwib_err ret;

  netwib_er(netwox_txthdr_decode(pline, hdr, pdecoded, &value));
  if (!*pdecoded) {
    return(NETWIB_ERR_OK);
  }

  datasize = netwib__buf_ref_data_size(&value);
  strlenbasic = netwib_c_strlen("Basic ");
  if (datasize < strlenbasic) {
    *pdecoded = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  }
  data = netwib__buf_ref_data_ptr(&value);
  if (netwib_c_memcmp(data, (netwib_constdata)"Basic ", strlenbasic)) {
    *pdecoded = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  }
  value.beginoffset += strlenbasic;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
  ret = netwib_buf_decode(&value, NETWIB_DECODETYPE_BASE64, pbuf);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));
    *pdecoded = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  }

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);
  pc = netwib_c_memchr(data, ':', datasize);
  if (pc != NULL) {
    usersize = pc - data;
    netwib_er(netwib_buf_append_data(data, usersize, puser));
    netwib_er(netwib_buf_append_data(data + usersize + 1,
                                     datasize - usersize - 1, ppassword));
  } else {
    netwib_er(netwib_buf_append_data(data, datasize, puser));
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  *pdecoded = NETWIB_TRUE;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_txthdr_decode_uint32(netwib_constbuf *pline,
                                       netwib_conststring hdr,
                                       netwib_bool *pdecoded,
                                       netwib_uint32 *pui)
{
  netwib_buf value;
  netwib_uint32 ui;
  netwib_err ret;

  netwib_er(netwox_txthdr_decode(pline, hdr, pdecoded, &value));
  if (!*pdecoded) {
    return(NETWIB_ERR_OK);
  }

  ret = netwib_buf_decode_fmt(&value, "%{uint32}%$", &ui);
  if (ret == NETWIB_ERR_NOTCONVERTED) {
    *pdecoded = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  if (pui != NULL) *pui = ui;
  *pdecoded = NETWIB_TRUE;
  return(NETWIB_ERR_OK);
}

