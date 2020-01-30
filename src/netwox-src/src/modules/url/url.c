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
netwib_err netwox_url_part_encode(netwib_constbuf *pbuf,
                                  netwox_url_parttype parttype,
                                  netwib_buf *pencodedbuf)
{
  netwib_data data, encodeddata, encodeddataori;
  netwib_uint32 datasize, quartet;

  if (parttype == NETWOX_URL_PARTTYPE_CLEAN) {
    return(netwib_buf_append_buf(pbuf, pencodedbuf));
  }

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);

  netwib_er(netwib_buf_wantspace(pencodedbuf, 3*datasize, &encodeddata));
  encodeddataori = encodeddata;

#define netwox_url_part_encodehex(data) *encodeddata++ = '%'; quartet = (*data) >> 4; *encodeddata++ = netwib_c2_16toc(quartet); quartet = (*data) & 0xF; *encodeddata++ = netwib_c2_16toc(quartet);

  switch(parttype) {
  case NETWOX_URL_PARTTYPE_UNCLEAN :
    while(datasize--) {
      netwox_url_part_encodehex(data);
      data++;
    }
    break;
  case NETWOX_URL_PARTTYPE_PATH :
    while(datasize--) {
      if (netwib_c2_isalnum(*data) || *data == '_' || *data == '.'
          || *data == '-' || *data == '+' || *data == '/') {
        *encodeddata++ = *data;
      } else {
        netwox_url_part_encodehex(data);
      }
      data++;
    }
    break;
  case NETWOX_URL_PARTTYPE_QUERY :
    while(datasize--) {
      if (netwib_c2_isalnum(*data) || *data == '_' || *data == '.'
          || *data == '-' || *data == '=' || *data == '&') {
        *encodeddata++ = *data;
      } else {
        netwox_url_part_encodehex(data);
      }
      data++;
    }
    break;
  case NETWOX_URL_PARTTYPE_FRAGMENT :
    while(datasize--) {
      if (netwib_c2_isalnum(*data) || *data == '_' || *data == '.'
          || *data == '-') {
        *encodeddata++ = *data;
      } else {
        netwox_url_part_encodehex(data);
      }
      data++;
    }
    break;
  case NETWOX_URL_PARTTYPE_HOST :
    while(datasize--) {
      if (netwib_c2_isalnum(*data) || *data == '.' || *data == '-'
          || *data == '_') {
        *encodeddata++ = *data;
      } else {
        netwox_url_part_encodehex(data);
      }
      data++;
    }
    break;
  case NETWOX_URL_PARTTYPE_USRPWD :
    while(datasize--) {
      if (netwib_c2_isalnum(*data)) {
        *encodeddata++ = *data;
      } else {
        netwox_url_part_encodehex(data);
      }
      data++;
    }
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  pencodedbuf->endoffset += encodeddata - encodeddataori;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_url_part_decode(netwib_constbuf *pbuf,
                                  netwox_url_parttype parttype,
                                  netwib_buf *pdecodedbuf)
{
  netwib_data data, decodeddata, decodeddataori;
  netwib_uint32 datasize, quartet1, quartet2, c;

  if (pdecodedbuf == NULL) {
    return(NETWIB_ERR_OK);
  }

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);

  netwib_er(netwib_buf_wantspace(pdecodedbuf, datasize, &decodeddata));
  decodeddataori = decodeddata;

  while(datasize--) {
    if (*data == '%') {
      if (datasize < 2) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      c = data[1];
      netwib_c2_cto16_if(c, quartet1) else {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      c = data[2];
      netwib_c2_cto16_if(c, quartet2) else {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      *decodeddata++ = (netwib_byte)((quartet1 << 4) | quartet2);
      data += 2;
      datasize -= 2;
    } else if (*data == '+' && parttype == NETWOX_URL_PARTTYPE_QUERY) {
      /* '+' means space only in the query */
      *decodeddata++ = ' ';
    } else {
      *decodeddata++ = *data;
    }
    data++;
  }

  pdecodedbuf->endoffset += decodeddata - decodeddataori;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_url_encode_scheme(netwox_url_scheme scheme,
                                    netwib_buf *pbuf)
{
  netwib_conststring pc=NULL;

  switch(scheme) {
  case NETWOX_URL_SCHEME_HTTP : pc = "http://"; break;
  case NETWOX_URL_SCHEME_HTTPS : pc = "https://"; break;
  case NETWOX_URL_SCHEME_FTP : pc = "ftp://"; break;
  default : return(NETWIB_ERR_PAINVALIDTYPE);
  }
  netwib_er(netwib_buf_append_string(pc, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_url_decode_scheme(netwib_constbuf *pbuf,
                                    netwox_url_scheme *pscheme,
                                    netwib_uint32 *pskipsize)
{
  netwib_data data;
  netwib_uint32 datasize;

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);

  if (datasize >= 7) {
    if (!netwib_c_memcmp(data, "http://", 7)) {
      if (pscheme != NULL) *pscheme = NETWOX_URL_SCHEME_HTTP;
      if (pskipsize != NULL) *pskipsize = 7;
      return(NETWIB_ERR_OK);
    }
  }
  if (datasize >= 8) {
    if (!netwib_c_memcmp(data, "https://", 8)) {
      if (pscheme != NULL) *pscheme = NETWOX_URL_SCHEME_HTTPS;
      if (pskipsize != NULL) *pskipsize = 8;
      return(NETWIB_ERR_OK);
    }
  }
  if (datasize >= 6) {
    if (!netwib_c_memcmp(data, "ftp://", 6)) {
      if (pscheme != NULL) *pscheme = NETWOX_URL_SCHEME_FTP;
      if (pskipsize != NULL) *pskipsize = 6;
      return(NETWIB_ERR_OK);
    }
  }

  /* rfc 2396: scheme = alpha *( alpha | digit | "+" | "-" | "." ) */
  if (pscheme != NULL) *pscheme = NETWOX_URL_SCHEME_NONE;
  if (pskipsize != NULL) *pskipsize = 0;
  if (datasize == 0) {
    return(NETWIB_ERR_OK);
  }
  if (!netwib_c2_isalpha(data[0])) {
    return(NETWIB_ERR_OK);
  }
  datasize--;
  while(datasize--) {
    data++;
    if (netwib_c2_isalpha(*data)) continue;
    if (netwib_c2_isdigit(*data)) continue;
    if (*data == '+' || *data == '-' || *data == '.') continue;
    if (*data == ':') {
      if (pscheme != NULL) *pscheme = NETWOX_URL_SCHEME_UNKNOWN;
      if (pskipsize != NULL) *pskipsize = netwib__buf_ref_data_size(pbuf) -
                               datasize + 2;
    }
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_port_init_scheme(netwox_url_scheme scheme,
                                   netwib_port *pport)
{
  switch(scheme) {
  case NETWOX_URL_SCHEME_HTTP :
    if (pport  != NULL) *pport = NETWOX_SERVICES_HTTP;
    break;
  case NETWOX_URL_SCHEME_HTTPS :
    if (pport  != NULL) *pport = NETWOX_SERVICES_HTTPS;
    break;
  case NETWOX_URL_SCHEME_FTP :
    if (pport  != NULL) *pport = NETWOX_SERVICES_FTP;
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_url_encode(netwox_url_scheme scheme,
                             netwib_constbuf *pauthority,
                             netwib_constbuf *ppath,
                             netwib_constbuf *pquery,
                             netwib_constbuf *pfragment,
                             netwib_buf *pbuf)
{
  netwib_buf path;
  netwib_data data;
  netwib_uint32 datasize;

  if (scheme == NETWOX_URL_SCHEME_NONE) {
    netwib_er(netwox_url_pqf_encode(ppath, pquery, pfragment, pbuf));
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwox_url_encode_scheme(scheme, pbuf));

  netwib_er(netwib_buf_append_buf(pauthority, pbuf));

  switch(scheme) {
  case NETWOX_URL_SCHEME_HTTP :
  case NETWOX_URL_SCHEME_HTTPS :
    /* path must start with '/' */
    netwib_er(netwox_url_pqf_encode(ppath, pquery, pfragment, pbuf));
    break;
  case NETWOX_URL_SCHEME_FTP :
    /* for FTP, url is "ftp://s/./d/f" (suppress "./" in ppath)
       or "ftp://s//d/f" */
    netwib_er(netwib_buf_append_byte('/', pbuf));
    if (ppath != NULL) {
      data = netwib__buf_ref_data_ptr(ppath);
      datasize = netwib__buf_ref_data_size(ppath);
      path = *ppath;
      if (datasize == 1) {
        if (data[0] == '.') {
          path.beginoffset++;
        }
      } else if (datasize >= 2) {
        if (data[0] == '.' && data[1] == '/') {
          path.beginoffset += 2;
        }
      }
      netwib_er(netwox_url_part_encode(&path, NETWOX_URL_PARTTYPE_PATH, pbuf));
    }
    break;
  default :
    return(NETWOX_ERR_INTERNALERROR);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_url_decode_expected(netwib_constbuf *pbuf,
                                      netwox_url_scheme expectedscheme,
                                      netwox_url_scheme *pscheme,
                                      netwib_bufext *pauthority,
                                      netwib_buf *ppath,
                                      netwib_bufext *pquery,
                                      netwib_buf *pfragment)
{
  netwib_data data, pc;
  netwib_uint32 datasize, authoritysize, skipsize;
  netwox_url_scheme scheme;
  netwib_buf buf;

  netwib_er(netwox_url_decode_scheme(pbuf, &scheme, &skipsize));
  if (scheme == NETWOX_URL_SCHEME_UNKNOWN) {
    return(NETWOX_ERR_NOTSUPPORTED);
  }
  if (pscheme != NULL) *pscheme = scheme;
  if (expectedscheme != NETWOX_URL_SCHEME_ANY) {
    if (scheme != expectedscheme) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  if (scheme == NETWOX_URL_SCHEME_NONE) {
    netwib_er(netwib_buf_init_ext_empty(pauthority));
    netwib_er(netwox_url_pqf_decode(pbuf, ppath, pquery, pfragment));
    return(NETWIB_ERR_OK);
  }

  data = netwib__buf_ref_data_ptr(pbuf) + skipsize;
  datasize = netwib__buf_ref_data_size(pbuf) - skipsize;

  pc = netwib_c_memchr(data, '/', datasize);
  if (pc != NULL) {
    authoritysize = pc - data;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, authoritysize,
                                              pauthority));
    switch(scheme) {
    case NETWOX_URL_SCHEME_HTTP :
    case NETWOX_URL_SCHEME_HTTPS :
      data += authoritysize;
      datasize -= authoritysize;
      netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, &buf));
      netwib_er(netwox_url_pqf_decode(&buf, ppath, pquery, pfragment));
      break;
    case NETWOX_URL_SCHEME_FTP :
      authoritysize++; /* to ignore the leading '/' */
      data += authoritysize;
      datasize -= authoritysize;
      if (datasize == 0) {
        netwib_er(netwib_buf_append_string("./", ppath));
      } else {
        /* FTP does not have query or fragment, and path can end with
           a param such as ";type=i". This param is ignored */
        netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, &buf));
        pc = netwib_c_memchr(data, ';', datasize);
        if (pc != NULL) {
          buf.endoffset = buf.beginoffset + pc - data;
        }
        netwib_er(netwox_url_part_decode(&buf, NETWOX_URL_PARTTYPE_PATH,
                                         ppath));
      }
      netwib_er(netwib_buf_init_ext_empty(pquery));
      break;
    default :
      return(NETWOX_ERR_INTERNALERROR);
    }
  } else {
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, pauthority));
    netwib_er(netwib_buf_init_ext_empty(pquery));
    switch(scheme) {
    case NETWOX_URL_SCHEME_HTTP :
    case NETWOX_URL_SCHEME_HTTPS :
      netwib_er(netwib_buf_append_byte('/', ppath));
      break;
    case NETWOX_URL_SCHEME_FTP :
      netwib_er(netwib_buf_append_string("./", ppath));
      break;
    default :
      return(NETWOX_ERR_INTERNALERROR);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_url_pqf_encode(netwib_constbuf *ppath,
                                 netwib_constbuf *pquery,
                                 netwib_constbuf *pfragment,
                                 netwib_buf *pbuf)
{

  netwib_er(netwox_url_part_encode(ppath, NETWOX_URL_PARTTYPE_PATH, pbuf));
  if (netwib__buf_ref_data_sizenull(pquery) != 0) {
    netwib_er(netwib_buf_append_byte('?', pbuf));
    netwib_er(netwib_buf_append_buf(pquery, pbuf));
  }
  if (netwib__buf_ref_data_sizenull(pfragment) != 0) {
    netwib_er(netwib_buf_append_byte('#', pbuf));
    netwib_er(netwox_url_part_encode(pfragment, NETWOX_URL_PARTTYPE_FRAGMENT,
                                     pbuf));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_url_pqf_decode(netwib_constbuf *pbuf,
                                 netwib_buf *ppath,
                                 netwib_bufext *pquery,
                                 netwib_buf *pfragment)
{
  netwib_data data, pc;
  netwib_uint32 datasize, pathsize, querysize;
  netwib_buf buf;

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);

  pc = netwib_c_memchr(data, '?', datasize);
  if (pc != NULL) {
    pathsize = pc - data;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, pathsize, &buf));
    netwib_er(netwox_url_part_decode(&buf, NETWOX_URL_PARTTYPE_PATH, ppath));
    data += pathsize + 1;
    datasize -= pathsize + 1;
    pc = netwib_c_memchr(data, '#', datasize);
    if (pc != NULL) {
      querysize = pc - data;
      netwib_er(netwib_buf_init_ext_arrayfilled(data, querysize, pquery));
      netwib_er(netwib_buf_init_ext_arrayfilled(data + querysize + 1,
                                                datasize - querysize - 1,
                                                &buf));
      netwib_er(netwox_url_part_decode(&buf, NETWOX_URL_PARTTYPE_FRAGMENT,
                                       pfragment));
    } else {
      netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, pquery));
    }
  } else {
    netwib_er(netwib_buf_init_ext_empty(pquery));
    pc = netwib_c_memchr(data, '#', datasize);
    if (pc != NULL) {
      pathsize = pc - data;
      netwib_er(netwib_buf_init_ext_arrayfilled(data, pathsize, &buf));
      netwib_er(netwox_url_part_decode(&buf, NETWOX_URL_PARTTYPE_PATH, ppath));
      netwib_er(netwib_buf_init_ext_arrayfilled(data + pathsize + 1,
                                                datasize - pathsize - 1,
                                                &buf));
      netwib_er(netwox_url_part_decode(&buf, NETWOX_URL_PARTTYPE_FRAGMENT,
                                       pfragment));
    } else {
      netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, &buf));
      netwib_er(netwox_url_part_decode(&buf, NETWOX_URL_PARTTYPE_PATH, ppath));
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_url_authority_encode(netwox_url_scheme scheme,
                                       netwib_constbuf *puser,
                                       netwib_constbuf *ppassword,
                                       netwib_constbuf *phost,
                                       netwib_port port,
                                       netwib_buf *pbuf)
{
  netwib_ip ipad;
  netwib_err ret;
  netwib_port defaultport;

  if (netwib__buf_ref_data_sizenull(puser) != 0) {
    netwib_er(netwox_url_part_encode(puser, NETWOX_URL_PARTTYPE_USRPWD, pbuf));
    netwib_er(netwib_buf_append_byte(':', pbuf));
    netwib_er(netwox_url_part_encode(ppassword, NETWOX_URL_PARTTYPE_USRPWD,
                                     pbuf));
    netwib_er(netwib_buf_append_byte('@', pbuf));
  }

  ret = netwib_ip_init_buf(phost, NETWIB_IP_DECODETYPE_IP6, &ipad);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_append_byte('[', pbuf));
    netwib_er(netwib_buf_append_ip(&ipad, NETWIB_IP_ENCODETYPE_IP, pbuf));
    netwib_er(netwib_buf_append_byte(']', pbuf));
  } else {
    netwib_er(netwox_url_part_encode(phost, NETWOX_URL_PARTTYPE_HOST, pbuf));
  }

  netwib_er(netwox_port_init_scheme(scheme, &defaultport));
  if (port != defaultport) {
    netwib_er(netwib_buf_append_fmt(pbuf, ":%{port}", port));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_url_authority_decode(netwox_url_scheme scheme,
                                       netwib_constbuf *pbuf,
                                       netwib_buf *puser,
                                       netwib_buf *ppassword,
                                       netwib_buf *phost,
                                       netwib_port *pport)
{
  netwib_data data, pc;
  netwib_uint32 datasize, usrpwdsize, usrsize, hostsize;
  netwib_buf buf;
  netwib_port port;

  netwib_er(netwox_port_init_scheme(scheme, &port));

  datasize = netwib__buf_ref_data_size(pbuf);
  if (datasize == 0) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  data = netwib__buf_ref_data_ptr(pbuf);

  /* check for a login and password */
  pc = netwib_c_memchr(data, '@', datasize);
  if (pc != NULL) {
    usrpwdsize = pc - data;
    pc = netwib_c_memchr(data, ':', usrpwdsize);
    if (pc != NULL) {
      usrsize = pc - data;
      netwib_er(netwib_buf_init_ext_arrayfilled(data, usrsize, &buf));
      netwib_er(netwox_url_part_decode(&buf, NETWOX_URL_PARTTYPE_USRPWD,
                                       puser));
      netwib_er(netwib_buf_init_ext_arrayfilled(data + usrsize + 1,
                                                usrpwdsize - usrsize - 1,
                                                &buf));
      netwib_er(netwox_url_part_decode(&buf, NETWOX_URL_PARTTYPE_USRPWD,
                                       ppassword));
    } else {
      netwib_er(netwib_buf_init_ext_arrayfilled(data, usrpwdsize, &buf));
      netwib_er(netwox_url_part_decode(&buf, NETWOX_URL_PARTTYPE_USRPWD,
                                       puser));
    }
    datasize -= usrpwdsize + 1;
    if (datasize == 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    data += usrpwdsize + 1;
  }

  /* decode host and port (special case of IPv6) */
  if (data[0] == '[') {
    pc = netwib_c_memchr(data, ']', datasize);
    if (pc == NULL) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    hostsize = pc - data + 1;
    netwib_er(netwib_buf_init_ext_arrayfilled(data+1, hostsize-2, &buf));
    netwib_er(netwox_url_part_decode(&buf, NETWOX_URL_PARTTYPE_HOST, phost));
    datasize -= hostsize;
    if (datasize) {
      data += hostsize;
      if (data[0] != ':') {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib_er(netwib_buf_init_ext_arrayfilled(data + 1, datasize - 1, &buf));
      netwib_er(netwib_buf_decode_fmt(&buf, "%{port}%$", &port));
    }
    if (pport != NULL) *pport = port;
    return(NETWIB_ERR_OK);
  }

  /* decode host and port */
  pc = netwib_c_memchr(data, ':', datasize);
  if (pc != NULL) {
    hostsize = pc - data;
    netwib_er(netwib_buf_init_ext_arrayfilled(data, hostsize, &buf));
    netwib_er(netwox_url_part_decode(&buf, NETWOX_URL_PARTTYPE_HOST, phost));
    netwib_er(netwib_buf_init_ext_arrayfilled(data + hostsize + 1,
                                              datasize - hostsize - 1, &buf));
    netwib_er(netwib_buf_decode_fmt(&buf, "%{port}%$", &port));
  } else {
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, &buf));
    netwib_er(netwox_url_part_decode(&buf, NETWOX_URL_PARTTYPE_HOST, phost));
  }

  if (pport != NULL) *pport = port;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_url_queryvar_encode(netwib_constbuf *pname,
                                      netwib_bool valueset,
                                      netwib_constbuf *pvalue,
                                      netwib_buf *pbuf)
{
  netwib_er(netwox_url_part_encode(pname, NETWOX_URL_PARTTYPE_QUERY, pbuf));
  if (valueset) {
    netwib_er(netwib_buf_append_byte('=', pbuf));
    netwib_er(netwox_url_part_encode(pvalue, NETWOX_URL_PARTTYPE_QUERY, pbuf));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_url_queryvar_decode(netwib_buf *pbuf,
                                      netwib_buf *pname,
                                      netwib_bool *pvalueset,
                                      netwib_buf *pvalue)
{
  netwib_data data, pc;
  netwib_uint32 datasize, namevaluesize, namesize, valuesize;
  netwib_buf buf;

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);

  if (datasize == 0) {
    return(NETWIB_ERR_DATAEND);
  }

  pc = netwib_c_memchr(data, '&', datasize);
  if (pc == NULL) {
    namevaluesize = datasize;
    pbuf->beginoffset += namevaluesize;
  } else {
    namevaluesize = pc - data;
    pbuf->beginoffset += namevaluesize + 1;
  }

  pc = netwib_c_memchr(data, '=', namevaluesize);
  if (pc == NULL) {
    namesize = namevaluesize;
    valuesize = 0;
    if (pvalueset != NULL) *pvalueset = NETWIB_FALSE;
  } else {
    namesize = pc - data;
    valuesize = namevaluesize - namesize - 1;
    if (pvalueset != NULL) *pvalueset = NETWIB_TRUE;
  }

  if (namesize) {
    netwib_er(netwib_buf_init_ext_arrayfilled(data, namesize, &buf));
    netwib_er(netwox_url_part_decode(&buf, NETWOX_URL_PARTTYPE_QUERY, pname));
  }

  if (valuesize) {
    netwib_er(netwib_buf_init_ext_arrayfilled(pc + 1, valuesize, &buf));
    netwib_er(netwox_url_part_decode(&buf, NETWOX_URL_PARTTYPE_QUERY, pvalue));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_url_query_decode(netwib_constbuf *pquery,
                                   netwib_constbuf *pname,
                                   netwib_bool *pvalueset,
                                   netwib_buf *pvalue)
{
  netwib_data namearray[128];
  netwib_buf query, name;
  netwib_uint32 valuesize;
  netwib_cmp cmp;
  netwib_err ret;

  query = *pquery;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(namearray, &name));

  valuesize = netwib__buf_ref_data_size(pvalue);

  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    netwib__buf_reinit(&name);
    pvalue->endoffset = pvalue->beginoffset + valuesize;
    ret = netwox_url_queryvar_decode(&query, &name, pvalueset, pvalue);
    if (ret == NETWIB_ERR_DATAEND) {
      ret = NETWIB_ERR_NOTFOUND;
      break;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    netwib_er(netwib_buf_cmp(&name, pname, &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      break;
    }
  }

  if (ret != NETWIB_ERR_OK) {
    pvalue->endoffset = pvalue->beginoffset + valuesize;
  }

  netwib_er(netwib_buf_close(&name));

  return(ret);
}

