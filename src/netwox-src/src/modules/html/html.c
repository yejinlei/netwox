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
netwib_err netwox_html_data_encode(netwib_constbuf *pbuf,
                                   netwib_bool encodemostchars,
                                   netwib_buf *pencodedbuf)
{
  netwib_data data;
  netwib_uint32 datasize;

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);

  if (encodemostchars) {
    while(datasize--) {
      if (netwib_c2_isalnum(*data)) {
        netwib_er(netwib_buf_append_byte(*data, pencodedbuf));
      } else {
        netwib_er(netwib_buf_append_fmt(pencodedbuf, "&#%{uint32};", *data));
      }
      data++;
    }
  } else {
    while(datasize--) {
      if (*data == '"' || *data == '&' || *data == '<' || *data == '>') {
        netwib_er(netwib_buf_append_fmt(pencodedbuf, "&#%{uint32};", *data));
      } else if (netwib_c2_isprint(*data)) {
        netwib_er(netwib_buf_append_byte(*data, pencodedbuf));
      } else {
        netwib_er(netwib_buf_append_fmt(pencodedbuf, "&#%{uint32};", *data));
      }
      data++;
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_html_data_decode2(netwib_constdata data,
                                           netwib_uint32 datasize,
                                           netwib_byte *pc)
{
  netwib_byte firstchar;

  if (datasize == 0) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  firstchar = data[0];

  /* decode numeric character references ("&#123;") */
  if (firstchar == '#') {
    switch(datasize) {
    case 2 :
      if (!netwib_c2_isdigit(data[1])) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      *pc = (netwib_char)(netwib_c2_cto9(data[1]));
      break;
    case 3 :
      if (!netwib_c2_isdigit(data[1]) || !netwib_c2_isdigit(data[2])) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      *pc = (netwib_char)(10*netwib_c2_cto9(data[1]) + netwib_c2_cto9(data[2]));
      break;
    case 4 :
      if (!netwib_c2_isdigit(data[1]) || !netwib_c2_isdigit(data[2])
          || !netwib_c2_isdigit(data[3])) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      *pc = (netwib_char)(100*netwib_c2_cto9(data[1]) + 10*netwib_c2_cto9(data[2])
        + netwib_c2_cto9(data[3]));
      break;
    default :
      return(NETWIB_ERR_NOTCONVERTED);
    }
    return(NETWIB_ERR_OK);
  }

  /* decode common entities */
#define netwox_html_data_decode_entity(tag,c) if (!netwib_c_memcmp(data, tag, datasize)) { *pc = c; return(NETWIB_ERR_OK);}
  switch(firstchar) {
  case 'A' :
    switch(datasize) {
    case 4 :
      netwox_html_data_decode_entity("Auml", 196);
      break;
    case 5 :
      netwox_html_data_decode_entity("AElig", 198);
      netwox_html_data_decode_entity("Acirc", 194);
      netwox_html_data_decode_entity("Aring", 197);
      break;
    case 6 :
      netwox_html_data_decode_entity("Aacute", 193);
      netwox_html_data_decode_entity("Agrave", 192);
      netwox_html_data_decode_entity("Atilde", 195);
      break;
    }
    break;
  case 'C' :
    switch(datasize) {
    case 6 :
      netwox_html_data_decode_entity("Ccedil", 199);
      break;
    }
    break;
  case 'E' :
    switch(datasize) {
    case 3 :
      netwox_html_data_decode_entity("ETH", 208);
      break;
    case 4 :
      netwox_html_data_decode_entity("Euml", 203);
      break;
    case 5 :
      netwox_html_data_decode_entity("Ecirc", 202);
      break;
    case 6 :
      netwox_html_data_decode_entity("Eacute", 201);
      netwox_html_data_decode_entity("Egrave", 200);
      break;
    }
    break;
  case 'I' :
    switch(datasize) {
    case 3 :
      break;
    case 4 :
      netwox_html_data_decode_entity("Iuml", 207);
      break;
    case 5 :
      netwox_html_data_decode_entity("Icirc", 206);
      break;
    case 6 :
      netwox_html_data_decode_entity("Iacute", 205);
      netwox_html_data_decode_entity("Igrave", 204);
      break;
    }
    break;
  case 'N' :
    switch(datasize) {
    case 6 :
      netwox_html_data_decode_entity("Ntilde", 209);
      break;
    }
    break;
  case 'O' :
    switch(datasize) {
    case 4 :
      netwox_html_data_decode_entity("Ouml", 214);
      break;
    case 5 :
      netwox_html_data_decode_entity("Ocirc", 212);
      break;
    case 6 :
      netwox_html_data_decode_entity("Oacute", 211);
      netwox_html_data_decode_entity("Ograve", 210);
      netwox_html_data_decode_entity("Oslash", 216);
      netwox_html_data_decode_entity("Otilde", 213);
      break;
    }
    break;
  case 'T' :
    switch(datasize) {
    case 5 :
     netwox_html_data_decode_entity("THORN", 222);
     break;
    }
    break;
  case 'U' :
    switch(datasize) {
    case 4 :
      netwox_html_data_decode_entity("Uuml", 220);
      break;
    case 5 :
      netwox_html_data_decode_entity("Ucirc", 219);
      break;
    case 6 :
      netwox_html_data_decode_entity("Uacute", 218);
      netwox_html_data_decode_entity("Ugrave", 217);
      break;
    }
    break;
  case 'Y' :
    switch(datasize) {
    case 6 :
      netwox_html_data_decode_entity("Yacute", 221);
      break;
    }
    break;
  case 'a' :
    switch(datasize) {
    case 3 :
      netwox_html_data_decode_entity("amp", '&');
      break;
    case 4 :
      netwox_html_data_decode_entity("auml", 228);
      break;
    case 5 :
      netwox_html_data_decode_entity("acirc", 226);
      netwox_html_data_decode_entity("acute", 180);
      netwox_html_data_decode_entity("aelig", 230);
      netwox_html_data_decode_entity("aring", 229);
      break;
    case 6 :
      netwox_html_data_decode_entity("aacute", 225);
      netwox_html_data_decode_entity("agrave", 224);
      netwox_html_data_decode_entity("atilde", 227);
      break;
    }
    break;
  case 'b' :
    switch(datasize) {
    case 6 :
      netwox_html_data_decode_entity("brvbar", 166);
      break;
    }
    break;
  case 'c' :
    switch(datasize) {
    case 4 :
      netwox_html_data_decode_entity("cent", 162);
      netwox_html_data_decode_entity("copy", 169);
      break;
    case 5 :
      netwox_html_data_decode_entity("cedil", 184);
      break;
    case 6 :
      netwox_html_data_decode_entity("ccedil", 231);
      netwox_html_data_decode_entity("curren", 164);
      break;
    }
    break;
  case 'd' :
    switch(datasize) {
    case 3 :
      netwox_html_data_decode_entity("deg", 176);
      break;
    case 6 :
      netwox_html_data_decode_entity("divide", 247);
      break;
    }
    break;
  case 'e' :
    switch(datasize) {
    case 3 :
      netwox_html_data_decode_entity("eth", 240);
      break;
    case 4 :
      netwox_html_data_decode_entity("euml", 235);
      break;
    case 5 :
      netwox_html_data_decode_entity("ecirc", 234);
      break;
    case 6 :
      netwox_html_data_decode_entity("eacute", 233);
      netwox_html_data_decode_entity("egrave", 232);
      break;
    }
    break;
  case 'f' :
    switch(datasize) {
    case 6 :
      netwox_html_data_decode_entity("frac12", 189);
      netwox_html_data_decode_entity("frac14", 188);
      netwox_html_data_decode_entity("frac34", 190);
      break;
    }
    break;
  case 'g' :
    switch(datasize) {
    case 2 :
      netwox_html_data_decode_entity("gt", '>');
      break;
    }
    break;
  case 'i' :
    switch(datasize) {
    case 4 :
      netwox_html_data_decode_entity("iuml", 239);
      break;
    case 5 :
      netwox_html_data_decode_entity("icirc", 238);
      netwox_html_data_decode_entity("iexcl", 161);
      break;
    case 6 :
      netwox_html_data_decode_entity("iacute", 237);
      netwox_html_data_decode_entity("igrave", 236);
      netwox_html_data_decode_entity("iquest", 191);
      break;
    }
    break;
  case 'l' :
    switch(datasize) {
    case 2 :
      netwox_html_data_decode_entity("lt", '<');
      break;
    case 5 :
      netwox_html_data_decode_entity("laquo", 171);
      break;
    }
    break;
  case 'm' :
    switch(datasize) {
    case 4 :
      netwox_html_data_decode_entity("macr", 175);
      break;
    case 5 :
      netwox_html_data_decode_entity("micro", 181);
      break;
    case 6 :
      netwox_html_data_decode_entity("middot", 183);
      break;
    }
    break;
  case 'n' :
    switch(datasize) {
    case 3 :
      netwox_html_data_decode_entity("not", 172);
      break;
    case 4 :
      netwox_html_data_decode_entity("nbsp", ' ');
      break;
    case 6 :
      netwox_html_data_decode_entity("ntilde", 241);
      break;
    }
    break;
  case 'o' :
    switch(datasize) {
    case 4 :
      netwox_html_data_decode_entity("ordf", 170);
      netwox_html_data_decode_entity("ordm", 186);
      netwox_html_data_decode_entity("ouml", 246);
      break;
    case 5 :
      netwox_html_data_decode_entity("ocirc", 244);
      break;
    case 6 :
      netwox_html_data_decode_entity("oacute", 243);
      netwox_html_data_decode_entity("ograve", 242);
      netwox_html_data_decode_entity("oslash", 248);
      netwox_html_data_decode_entity("otilde", 245);
      break;
    }
    break;
  case 'p' :
    switch(datasize) {
    case 4 :
      netwox_html_data_decode_entity("para", 182);
      break;
    case 5 :
      netwox_html_data_decode_entity("pound", 163);
      break;
    case 6 :
      netwox_html_data_decode_entity("plusmn", 177);
      break;
    }
    break;
  case 'q' :
    switch(datasize) {
    case 4 :
      netwox_html_data_decode_entity("quot", '"');
      break;
    }
    break;
  case 'r' :
    switch(datasize) {
    case 3 :
      netwox_html_data_decode_entity("reg", 174);
      break;
    case 5 :
      netwox_html_data_decode_entity("raquo", 187);
      break;
    }
    break;
  case 's' :
    switch(datasize) {
    case 3 :
      netwox_html_data_decode_entity("shy", 173);
     break;
    case 4 :
      netwox_html_data_decode_entity("sect", 167);
      netwox_html_data_decode_entity("sup1", 185);
      netwox_html_data_decode_entity("sup2", 178);
      netwox_html_data_decode_entity("sup3", 179);
      break;
    case 5 :
      netwox_html_data_decode_entity("szlig", 223);
      break;
    }
    break;
  case 't' :
    switch(datasize) {
    case 5 :
      netwox_html_data_decode_entity("thorn", 254);
      netwox_html_data_decode_entity("times", 215);
      break;
    }
    break;
  case 'u' :
    switch(datasize) {
    case 3 :
      netwox_html_data_decode_entity("uml", 168);
      break;
    case 4 :
      netwox_html_data_decode_entity("uuml", 252);
      break;
    case 5 :
      netwox_html_data_decode_entity("ucirc", 251);
      break;
    case 6 :
      netwox_html_data_decode_entity("uacute", 250);
      netwox_html_data_decode_entity("ugrave", 249);
      break;
    }
    break;
  case 'y' :
    switch(datasize) {
    case 3 :
      netwox_html_data_decode_entity("yen", 165);
      break;
    case 4 :
      netwox_html_data_decode_entity("yuml", 255);
      break;
    case 6 :
      netwox_html_data_decode_entity("yacute", 253);
      break;
    }
    break;
  }

  return(NETWIB_ERR_NOTCONVERTED);
}

/*-------------------------------------------------------------*/
netwib_err netwox_html_data_decode(netwib_buf *pbuf,
                                   netwib_buf *pdecodedbuf)
{
  netwib_data datain, dataout, datainsave, dataoutsave;
  netwib_uint32 datainsize, tagsize;
  netwib_byte c, *pc;
  netwib_err ret;

  datain = netwib__buf_ref_data_ptr(pbuf);
  datainsize = netwib__buf_ref_data_size(pbuf);
  netwib_er(netwib_buf_wantspace(pdecodedbuf, datainsize, &dataout));
  datainsave = datain;
  dataoutsave = dataout;

  ret = NETWIB_ERR_OK;
  while(datainsize) {
    c = *datain;
    if (c == '&') {
      pc = netwib_c_memchr(datain, ';', datainsize);
      if (pc == NULL) {
        /* so, we really have no sufficient data, or HTML document is
           badly formatted. */
        pc = netwib_c_memchr(datain, ' ', datainsize);
        if (pc == NULL) {
          pc = netwib_c_memchr(datain, '<', datainsize);
          if (pc == NULL) {
            if (datainsize < 11) {
              /* we really have no sufficient data */
              ret = NETWIB_ERR_DATANOTAVAIL;
              break;
            }
          }
        }
        /* HTML document is badly formatted : skip '&', and continue */
        *dataout++ = c;
        datain++;
        datainsize--;
        continue;
      }
      tagsize = pc - datain - 1;
      ret = netwox_html_data_decode2(datain+1, tagsize, &c);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
      *dataout++ = c;
      datain += tagsize + 2;
      datainsize -= tagsize + 2;
    } else {
      *dataout++ = c;
      datain++;
      datainsize--;
    }
  }

  pbuf->beginoffset += datain - datainsave;
  pdecodedbuf->endoffset += dataout - dataoutsave;

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_html_data_decode_best(netwib_constbuf *pbuf,
                                        netwib_buf *pdecodedbuf)
{
  netwib_buf buf;
  netwib_err ret;

  buf = *pbuf;

  while (NETWIB_TRUE) {
    if (netwib__buf_ref_data_size(&buf) == 0) {
      break;
    }
    ret = netwox_html_data_decode(&buf, pdecodedbuf);
    if (ret == NETWIB_ERR_OK) {
      break;
    }
    netwib_er(netwib_buf_append_byte('&', pdecodedbuf));
    buf.beginoffset++;
  }

  return(NETWIB_ERR_OK);
}

