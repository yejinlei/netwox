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
netwib_err netwox_dhcp4opts_show(netwib_constbuf *pdhcp4opts,
                                 netwib_encodetype encodetype,
                                 netwib_buf *pbuf)
{
  netwib_buf dhcp4opts, badopt;
  netwox_dhcp4opt dhcp4opt;
  netwib_uint32 skipsize;
  netwib_err ret;

  dhcp4opts = *pdhcp4opts;
  while(NETWIB_TRUE) {
    if (dhcp4opts.beginoffset >= dhcp4opts.endoffset) {
      break;
    }
    ret = netwox_pkt_decode_dhcp4opt(&dhcp4opts, &dhcp4opt, &skipsize);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwox_dhcp4opt_show(&dhcp4opt, encodetype, pbuf));
      if (dhcp4opt.type == NETWOX_DHCP4OPTTYPE_END) {
        break;
      }
    } else {
      badopt = dhcp4opts;
      badopt.endoffset = badopt.beginoffset + skipsize;
      netwib_er(netwib_show_array_data(" undecoded option", &badopt,
                                       NETWIB_ENCODETYPE_HEXA0, ' ', pbuf));
    }
    dhcp4opts.beginoffset += skipsize;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dhcp4opts_decode_common(netwib_constbuf *pdhcp4opts,
                                          netwox_dhcp4optscommon *pcommon)
{
  netwib_buf dhcp4opts;
  netwox_dhcp4opt dhcp4opt;
  netwib_uint32 skipsize;
  netwib_err ret;

  pcommon->msgtypeset = NETWIB_FALSE;
  pcommon->serveridset = NETWIB_FALSE;
  pcommon->ipadleasetimeset = NETWIB_FALSE;

  dhcp4opts = *pdhcp4opts;
  while(NETWIB_TRUE) {
    if (dhcp4opts.beginoffset >= dhcp4opts.endoffset) {
      break;
    }
    ret = netwox_pkt_decode_dhcp4opt(&dhcp4opts, &dhcp4opt, &skipsize);
    if (ret == NETWIB_ERR_OK) {
      if (dhcp4opt.type == NETWOX_DHCP4OPTTYPE_MSGTYPE) {
        pcommon->msgtype = dhcp4opt.opt.msgtype;
        pcommon->msgtypeset = NETWIB_TRUE;
      } else if (dhcp4opt.type == NETWOX_DHCP4OPTTYPE_SERVERID) {
         pcommon->serverid = dhcp4opt.opt.serverid;
         pcommon->serveridset = NETWIB_TRUE;
      } else if (dhcp4opt.type == NETWOX_DHCP4OPTTYPE_IPADLEASETIME) {
        pcommon->ipadleasetime = dhcp4opt.opt.ipadleasetime;
        pcommon->ipadleasetimeset = NETWIB_TRUE;
      } else if (dhcp4opt.type == NETWOX_DHCP4OPTTYPE_END) {
        break;
      }
    }
    dhcp4opts.beginoffset += skipsize;
  }

  return(NETWIB_ERR_OK);
}

