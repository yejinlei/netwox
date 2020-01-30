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
netwib_err netwib_device_init_kbd(netwib_constbuf *pmessage,
                                  netwib_constbuf *pdefaultdevice,
                                  netwib_buf *pdevice)
{
  netwib_conf_devices conf;
  netwib_conf_devices_index *pconfindex;
  netwib_buf buf;
  netwib_uint32 choice, defaultchoice;
  netwib_cmp cmp;
  netwib_bool oneset;
  netwib_err ret;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  if (pmessage != NULL) {
    netwib_er(netwib_buf_append_buf(pmessage, &buf));
    netwib_er(netwib_buf_append_string("\n", &buf));
  } else {
    netwib_er(netwib_buf_append_string("Select device number :\n", &buf));
  }

  netwib_er(netwib_conf_devices_index_init(&conf, &pconfindex));
  ret = NETWIB_ERR_OK;
  defaultchoice = NETWIB_UINT32_INIT_KBD_NODEF;
  oneset = NETWIB_FALSE;
  while (NETWIB_TRUE) {
    ret = netwib_conf_devices_index_next(pconfindex);
    if (ret == NETWIB_ERR_DATAEND) {
      ret = NETWIB_ERR_OK;
      break;
    }
    netwib_eg(ret);
    oneset = NETWIB_TRUE;
    netwib_eg(netwib_buf_append_fmt(&buf, " %{r 3;uint32} - %{l 5;buf} (%{buf})\n", conf.devnum, &conf.deviceeasy, &conf.device));
    if (pdefaultdevice != NETWIB_DEVICE_INIT_KBD_NODEF) {
      netwib_eg(netwib_buf_cmp(pdefaultdevice, &conf.device, &cmp));
      if (cmp != NETWIB_CMP_EQ) {
        netwib_eg(netwib_buf_cmp(pdefaultdevice, &conf.deviceeasy, &cmp));
      }
      if (cmp == NETWIB_CMP_EQ) {
        defaultchoice = conf.devnum;
      }
    }
  }
  if (!oneset) {
    netwib_er(netwib_buf_append_kbd(pmessage, pdefaultdevice, pdevice));
    netwib_goto(NETWIB_ERR_OK);
  }
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_string("Your choice", &buf));
  netwib_er(netwib_uint32_init_kbd(&buf, 1, conf.devnum, defaultchoice,
                                   &choice));

  if (pdevice != NULL) {
    netwib_eg(netwib_conf_devices_index_close(&pconfindex));
    netwib_eg(netwib_conf_devices_index_init(&conf, &pconfindex));
    while (NETWIB_TRUE) {
      ret = netwib_conf_devices_index_next(pconfindex);
      if (ret == NETWIB_ERR_DATAEND) {
        /* configuration changed, so re-ask user */
        ret = netwib_device_init_kbd(pmessage, pdefaultdevice, pdevice);
        break;
      }
      netwib_eg(ret);
      if (conf.devnum == choice) {
        netwib_eg(netwib_buf_append_buf(&conf.deviceeasy, pdevice));
        break;
      }
    }
  }

 netwib_gotolabel:
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_conf_devices_index_close(&pconfindex));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_device_hwtype(netwib_device_hwtype type,
                                           netwib_buf *pbuf)
{
  netwib_conststring pc=NULL;

  switch(type) {
    case NETWIB_DEVICE_HWTYPE_UNKNOWN :
      pc = "unknown";
      break;
    case NETWIB_DEVICE_HWTYPE_ETHER :
      pc = "ethernet";
      break;
    case NETWIB_DEVICE_HWTYPE_LOOPBACK :
      pc = "loopback";
      break;
    case NETWIB_DEVICE_HWTYPE_PPP :
      pc = "ppp";
      break;
    case NETWIB_DEVICE_HWTYPE_PLIP :
      pc = "plip";
      break;
    case NETWIB_DEVICE_HWTYPE_SLIP :
      pc = "slip";
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  netwib_er(netwib_buf_append_string(pc, pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#define NETWIB_PRIV_DEVICE_HWTYPE_MAX 10
netwib_err netwib_device_hwtype_init_kbd(netwib_constbuf *pmessage,
                                         netwib_device_hwtype defaulthwtype,
                                         netwib_device_hwtype *phwtype)
{
  netwib_device_hwtype array[NETWIB_PRIV_DEVICE_HWTYPE_MAX];
  netwib_buf msg, buf;
  netwib_uint32 i, choice, defaultchoice;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  if (pmessage != NULL) {
    netwib_er(netwib_buf_append_buf(pmessage, &buf));
    netwib_er(netwib_buf_append_string("\n", &buf));
  }

  i = 0;
  defaultchoice = NETWIB_UINT32_INIT_KBD_NODEF;

#define netwib_hwtype_lih(ot) {netwib_er(netwib_buf_append_fmt(&buf, " %{r 2;uint32} - ", i)); netwib_er(netwib_buf_append_device_hwtype(ot, &buf)); netwib_er(netwib_buf_append_fmt(&buf, "\n")); if (defaulthwtype == ot) defaultchoice = i; array[i++] = ot; }

  netwib_hwtype_lih(NETWIB_DEVICE_HWTYPE_ETHER);
  netwib_hwtype_lih(NETWIB_DEVICE_HWTYPE_LOOPBACK);
  netwib_hwtype_lih(NETWIB_DEVICE_HWTYPE_PPP);
  netwib_hwtype_lih(NETWIB_DEVICE_HWTYPE_PLIP);
  netwib_hwtype_lih(NETWIB_DEVICE_HWTYPE_SLIP);

  if (i >= NETWIB_PRIV_DEVICE_HWTYPE_MAX) {
    return(NETWIB_ERR_LOINTERNALERROR);
  }
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_buf_close(&buf));

  if (defaulthwtype == NETWIB_DEVICE_HWTYPE_UNKNOWN) {
    defaultchoice = NETWIB_UINT32_INIT_KBD_NODEF;
  }

  netwib_er(netwib_buf_init_ext_string("Your choice", &msg));
  netwib_er(netwib_uint32_init_kbd(&msg, 0, i-1, defaultchoice,
                                 &choice));

  if (phwtype != NULL) *phwtype = array[choice];
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_device_dlttype(netwib_device_dlttype type,
                                            netwib_buf *pbuf)
{
  netwib_conststring pc=NULL;

  switch(type) {
    case NETWIB_DEVICE_DLTTYPE_UNKNOWN :
      pc = "unknown";
      break;
    case NETWIB_DEVICE_DLTTYPE_NULL :
      pc = "no link-layer encapsulation";
      break;
    case NETWIB_DEVICE_DLTTYPE_EN10MB :
      pc = "ethernet";
      break;
    case NETWIB_DEVICE_DLTTYPE_EN3MB :
      pc = "experimental ethernet";
      break;
    case NETWIB_DEVICE_DLTTYPE_AX25 :
      pc = "amateur radio";
      break;
    case NETWIB_DEVICE_DLTTYPE_PRONET :
      pc = "ProNET Token Ring";
      break;
    case NETWIB_DEVICE_DLTTYPE_CHAOS :
      pc = "chaos";
      break;
    case NETWIB_DEVICE_DLTTYPE_IEEE802 :
      pc = "IEEE 802";
      break;
    case NETWIB_DEVICE_DLTTYPE_ARCNET :
      pc = "ARCNET";
      break;
    case NETWIB_DEVICE_DLTTYPE_SLIP :
      pc = "slip";
      break;
    case NETWIB_DEVICE_DLTTYPE_PPP :
      pc = "ppp";
      break;
    case NETWIB_DEVICE_DLTTYPE_FDDI :
      pc = "FDDI";
      break;
    case NETWIB_DEVICE_DLTTYPE_ATM_RFC1483 :
      pc = "LLC/SNAP encapsulated atm";
      break;
    case NETWIB_DEVICE_DLTTYPE_RAW :
      pc = "raw IP";
      break;
    case NETWIB_DEVICE_DLTTYPE_RAW4 :
      pc = "raw IPv4";
      break;
    case NETWIB_DEVICE_DLTTYPE_RAW6 :
      pc = "raw IPv6";
      break;
    case NETWIB_DEVICE_DLTTYPE_SLIP_BSDOS :
      pc = "BSD/OS slip";
      break;
    case NETWIB_DEVICE_DLTTYPE_PPP_BSDOS :
      pc = "BSD/OS ppp";
      break;
    case NETWIB_DEVICE_DLTTYPE_ATM_CLIP :
      pc = "IP over ATM";
      break;
    case NETWIB_DEVICE_DLTTYPE_PPP_SERIAL :
      pc = "PPP over serial";
      break;
    case NETWIB_DEVICE_DLTTYPE_PPP_ETHER :
      pc = "PPP over ethernet";
      break;
    case NETWIB_DEVICE_DLTTYPE_C_HDLC :
      pc = "Cisco HDLC";
      break;
    case NETWIB_DEVICE_DLTTYPE_IEEE802_11 :
      pc = "IEEE 802.11";
      break;
    case NETWIB_DEVICE_DLTTYPE_LOOP :
      pc = "loop";
      break;
    case NETWIB_DEVICE_DLTTYPE_LINUX_SLL :
      pc = "linux cooked sockets";
      break;
    case NETWIB_DEVICE_DLTTYPE_LTALK :
      pc = "LocalTalk";
      break;
    case NETWIB_DEVICE_DLTTYPE_ECONET :
      pc = "Econet";
      break;
    case NETWIB_DEVICE_DLTTYPE_PRISM_HEADER :
      pc = "Prism II";
      break;
    case NETWIB_DEVICE_DLTTYPE_AIRONET_HEADER :
      pc = "Aironet";
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  netwib_er(netwib_buf_append_string(pc, pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#define NETWIB_PRIV_DEVICE_DLTTYPE_MAX 40
netwib_err netwib_device_dlttype_init_kbd(netwib_constbuf *pmessage,
                                          netwib_device_dlttype defaultdlttype,
                                          netwib_device_dlttype *pdlttype)
{
  netwib_device_dlttype array[NETWIB_PRIV_DEVICE_DLTTYPE_MAX];
  netwib_buf msg, buf;
  netwib_uint32 i, choice, defaultchoice;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  if (pmessage != NULL) {
    netwib_er(netwib_buf_append_buf(pmessage, &buf));
    netwib_er(netwib_buf_append_string("\n", &buf));
  }

  i = 0;
  defaultchoice = NETWIB_UINT32_INIT_KBD_NODEF;

#define netwib_lid(ot) {netwib_er(netwib_buf_append_fmt(&buf, " %{r 2;uint32} - ", i)); netwib_er(netwib_buf_append_device_dlttype(ot, &buf)); netwib_er(netwib_buf_append_fmt(&buf, "\n")); if (defaultdlttype == ot) defaultchoice = i; array[i++] = ot; }

  netwib_lid(NETWIB_DEVICE_DLTTYPE_NULL);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_ETHER);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_EN3MB);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_AX25);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_PRONET);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_CHAOS);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_IEEE802);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_ARCNET);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_SLIP);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_PPP);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_FDDI);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_ATM_RFC1483);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_RAW);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_RAW4);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_RAW6);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_SLIP_BSDOS);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_PPP_BSDOS);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_ATM_CLIP);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_PPP_SERIAL);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_PPP_ETHER);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_C_HDLC);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_IEEE802_11);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_LOOP);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_LINUX_SLL);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_LTALK);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_ECONET);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_PRISM_HEADER);
  netwib_lid(NETWIB_DEVICE_DLTTYPE_AIRONET_HEADER);

  if (i >= NETWIB_PRIV_DEVICE_DLTTYPE_MAX) {
    return(NETWIB_ERR_LOINTERNALERROR);
  }
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_buf_close(&buf));

  if (defaultdlttype == NETWIB_DEVICE_DLTTYPE_UNKNOWN) {
    defaultchoice = NETWIB_UINT32_INIT_KBD_NODEF;
  }

  netwib_er(netwib_buf_init_ext_string("Your choice", &msg));
  netwib_er(netwib_uint32_init_kbd(&msg, 0, i-1, defaultchoice,
                                 &choice));

  if (pdlttype != NULL) *pdlttype = array[choice];
  return(NETWIB_ERR_OK);
}
