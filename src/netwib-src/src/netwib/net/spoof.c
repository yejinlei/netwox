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
#include "pieces/iospoof.c"

/*-------------------------------------------------------------*/
#include "pieces/iospoof_ip.c"

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_spoof_ip_inittype(netwib_spoof_ip_inittype type,
                                               netwib_buf *pbuf)
{
  netwib_conststring pc=NULL;

  switch(type) {
    case NETWIB_SPOOF_IP_INITTYPE_UNKNOWN :
      pc = "unknown";
      break;
    case NETWIB_SPOOF_IP_INITTYPE_RAW :
      pc = "only try raw IP4/6 spoof";
      break;
    case NETWIB_SPOOF_IP_INITTYPE_LINKB :
      pc = "only try link spoof (blank source address)";
      break;
    case NETWIB_SPOOF_IP_INITTYPE_LINKF :
      pc = "only try link spoof (fill source address)";
      break;
    case NETWIB_SPOOF_IP_INITTYPE_LINKFB :
      pc = "only try link spoof (try filling source address)";
      break;
    case NETWIB_SPOOF_IP_INITTYPE_RAWLINKB :
      pc = "try raw spoof, if error use link spoof (blank source address)";
      break;
    case NETWIB_SPOOF_IP_INITTYPE_RAWLINKF :
      pc = "try raw spoof, if error use link spoof (fill source address)";
      break;
    case NETWIB_SPOOF_IP_INITTYPE_RAWLINKFB :
      pc = "try raw spoof, if error use link spoof (try filling source address)";
      break;
    case NETWIB_SPOOF_IP_INITTYPE_LINKBRAW :
      pc = "try link spoof, if error use raw spoof (blank source address)";
      break;
    case NETWIB_SPOOF_IP_INITTYPE_LINKFRAW :
      pc = "try link spoof, if error use raw spoof (fill source address)";
      break;
    case NETWIB_SPOOF_IP_INITTYPE_LINKFBRAW :
      pc = "try link spoof, if error use raw spoof (try filling source address)";
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  netwib_er(netwib_buf_append_string(pc, pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#define NETWIB_PRIV_SPOOF_IP_INITTYPE_MAX 20
netwib_err netwib_spoof_ip_inittype_init_kbd(netwib_constbuf *pmessage,
                                             netwib_spoof_ip_inittype defaulttype,
                                             netwib_spoof_ip_inittype *ptype)
{
  netwib_spoof_ip_inittype array[NETWIB_PRIV_SPOOF_IP_INITTYPE_MAX];
  netwib_buf msg, buf;
  netwib_uint32 i, choice, defaultchoice;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  if (pmessage != NULL) {
    netwib_er(netwib_buf_append_buf(pmessage, &buf));
    netwib_er(netwib_buf_append_string("\n", &buf));
  }

  i = 0;
  defaultchoice = NETWIB_UINT32_INIT_KBD_NODEF;

#define netwib_sii_lih(ot) {netwib_er(netwib_buf_append_fmt(&buf, " %{r 2;uint32} - ", i)); netwib_er(netwib_buf_append_spoof_ip_inittype(ot, &buf)); netwib_er(netwib_buf_append_fmt(&buf, "\n")); if (defaulttype == ot) defaultchoice = i; array[i++] = ot; }

  netwib_sii_lih(NETWIB_SPOOF_IP_INITTYPE_RAW);
  netwib_sii_lih(NETWIB_SPOOF_IP_INITTYPE_LINKB);
  netwib_sii_lih(NETWIB_SPOOF_IP_INITTYPE_LINKF);
  netwib_sii_lih(NETWIB_SPOOF_IP_INITTYPE_LINKFB);
  netwib_sii_lih(NETWIB_SPOOF_IP_INITTYPE_RAWLINKB);
  netwib_sii_lih(NETWIB_SPOOF_IP_INITTYPE_RAWLINKF);
  netwib_sii_lih(NETWIB_SPOOF_IP_INITTYPE_RAWLINKFB);
  netwib_sii_lih(NETWIB_SPOOF_IP_INITTYPE_LINKBRAW);
  netwib_sii_lih(NETWIB_SPOOF_IP_INITTYPE_LINKFRAW);
  netwib_sii_lih(NETWIB_SPOOF_IP_INITTYPE_LINKFBRAW);

  if (i >= NETWIB_PRIV_SPOOF_IP_INITTYPE_MAX) {
    return(NETWIB_ERR_LOINTERNALERROR);
  }
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_buf_close(&buf));

  if (defaulttype == NETWIB_SPOOF_IP_INITTYPE_UNKNOWN) {
    defaultchoice = NETWIB_UINT32_INIT_KBD_NODEF;
  }

  netwib_er(netwib_buf_init_ext_string("Your choice", &msg));
  netwib_er(netwib_uint32_init_kbd(&msg, 0, i-1, defaultchoice, &choice));

  if (ptype != NULL) *ptype = array[choice];
  return(NETWIB_ERR_OK);
}
