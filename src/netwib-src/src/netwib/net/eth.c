/*
                                  NETWIB
                             Network library
                Copyright(c) 1999-2012 Laurent CONSTANTIN
                      http://ntwib.sourceforge.net/
                        laurentconstantin@free.fr
                                  -----
  This file is part of Netwib.

  Netwib is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3
  as published by the Free Software Foundation.

  Netwib is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details (http://www.gnu.org/).

------------------------------------------------------------------------
*/

#include <netwib/inc/maininc.h>

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_eth_bad(netwib_conststring str)
{
  netwib_er(netwib_priv_errmsg_string("this is not an Ethernet address: "));
  netwib_er(netwib_priv_errmsg_append_string(str));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_eth_init_fields(netwib_byte a,
                                  netwib_byte b,
                                  netwib_byte c,
                                  netwib_byte d,
                                  netwib_byte e,
                                  netwib_byte f,
                                  netwib_eth *peth)
{
  if (peth != NULL) {
    peth->b[0] = a;
    peth->b[1] = b;
    peth->b[2] = c;
    peth->b[3] = d;
    peth->b[4] = e;
    peth->b[5] = f;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_eth_init_buf(netwib_constbuf *pbuf,
                               netwib_eth *peth)
{
  netwib_string pc, str;
  netwib_char c;
  netwib_uint32 numfieldsfound, numdigitfound;
  netwib_eth eth;

  netwib__constbuf_ref_string(pbuf, str, bufstorage,
                              netwib_eth_init_buf(&bufstorage, peth));

  /* first, skip spaces */
  pc = str;
  while (*pc == ' ' || *pc == '\t') {
    pc++;
  }

  /* main loop */
  eth.b[0] = 0; /* for compiler warning */
  numfieldsfound = 0;
  numdigitfound = 0;
  while (NETWIB_TRUE) {
    c = *pc;
    if (c == '\0' || c == ' ' || c == '\t' ) {
      break;
    }
    pc++;
    if (c == ':') {
      if (numdigitfound == 0) {
        netwib_er(netwib_priv_eth_bad(str));
        return(NETWIB_ERR_NOTCONVERTED);
      }
      if (numfieldsfound == 5) {
        netwib_er(netwib_priv_eth_bad(str));
        return(NETWIB_ERR_NOTCONVERTED);
      }
      numfieldsfound++;
      numdigitfound = 0;
    } else if (netwib_c2_isxdigit(c)) {
      switch(numdigitfound) {
      case 0 :
        eth.b[numfieldsfound] = (netwib_byte)netwib_c2_cto16(c);
        break;
      case 1 :
        eth.b[numfieldsfound] = (netwib_byte)((eth.b[numfieldsfound]<<4) | netwib_c2_cto16(c));
        break;
      default :
        netwib_er(netwib_priv_eth_bad(str));
        return(NETWIB_ERR_NOTCONVERTED);
      }
      numdigitfound++;
    } else {
      netwib_er(netwib_priv_eth_bad(str));
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  /* check values */
  if (numdigitfound == 0 || numfieldsfound != 5) {
    netwib_er(netwib_priv_eth_bad(str));
    return(NETWIB_ERR_NOTCONVERTED);
  }

  /* check there is only spaces */
  while (*pc == ' ' || *pc == '\t') {
    pc++;
  }
  if (*pc != '\0') {
    netwib_er(netwib_priv_eth_bad(str));
    return(NETWIB_ERR_NOTCONVERTED);
  }

  if (peth != NULL) {
    *peth = eth;
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#include "pieces/eth_init_ip.c"

/*-------------------------------------------------------------*/
netwib_err netwib_eth_init_kbd(netwib_constbuf *pmessage,
                               const netwib_eth *pdefaulteth,
                               netwib_eth *peth)
{
  netwib_buf buf, def;
  netwib_constbuf *pdef;
  netwib_char prompt;
  netwib_err ret;

  pdef = NETWIB_BUF_APPEND_KBD_NODEF;
  netwib_er(netwib_buf_init_mallocdefault(&def));
  if (pdefaulteth != NETWIB_ETH_INIT_KBD_NODEF) {
    netwib_er(netwib_buf_append_eth(pdefaulteth, &def));
    pdef = &def;
  }

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  prompt = ':';
  while (NETWIB_TRUE) {
    netwib_er(netwib_priv_kbd_buf_append(pmessage, pdef,
                                         NETWIB_TRUE, prompt, NETWIB_FALSE,
                                         &buf));
    if (netwib__buf_ref_data_size(&buf) == 0) {
      if (pdefaulteth != NETWIB_ETH_INIT_KBD_NODEF) {
        if (peth != NULL) {
          *peth = *pdefaulteth;
        }
        break;
      }
    }
    ret = netwib_eth_init_buf(&buf, peth);
    if (ret == NETWIB_ERR_OK) {
      break;
    }
    netwib__buf_reinit(&buf);
    prompt = '>';
  }

  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_close(&def));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_eth_cmp(netwib_consteth *peth1,
                          netwib_consteth *peth2,
                          netwib_cmp *pcmp)
{
  int reti;

  if (pcmp == NULL) {
    return(NETWIB_ERR_OK);
  }

  reti = netwib_c_memcmp(peth1->b, peth2->b, NETWIB_ETH_LEN);
  if (reti == 0) {
    *pcmp = NETWIB_CMP_EQ;
  } else if (reti > 0) {
    *pcmp = NETWIB_CMP_GT;
  } else {
    *pcmp = NETWIB_CMP_LT;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_eth(netwib_consteth *peth,
                                 netwib_buf *pbuf)
{
  netwib_data data, pc;
  netwib_uint32 i, u;

  /* max is "12:12:12:12:12:12" == 17+1 chars */
  netwib_er(netwib_buf_wantspace(pbuf, 18, &data));

  i = 0;
  pc = data;
  while (NETWIB_TRUE) {
    u = peth->b[i]>>4;
    *pc++ = netwib_c2_16toC(u);
    u = peth->b[i]&0xF;
    *pc++ = netwib_c2_16toC(u);
    i++;
    if (i >= NETWIB_ETH_LEN) {
      break;
    }
    *pc++ = ':';
  }

  pbuf->endoffset += pc-data;

  return(NETWIB_ERR_OK);
}
