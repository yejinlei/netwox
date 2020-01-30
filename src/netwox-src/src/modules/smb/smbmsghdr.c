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
netwib_err netwox_pkt_append_smbmsghdr_begin(netwib_bool isnaked,
                                             netwib_buf *ppkt)
{

  if (isnaked) {
    netwib_er(netwib_buf_wantspace(ppkt, NETWOX_SMBNAKEDHDR_LEN, NULL));
    ppkt->endoffset += NETWOX_SMBNAKEDHDR_LEN;
  } else {
    netwib_er(netwib_buf_wantspace(ppkt, NETWOX_SMBNBTSSHDR_LEN, NULL));
    ppkt->endoffset += NETWOX_SMBNBTSSHDR_LEN;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_smbmsghdr_end(netwib_bool isnaked,
                                           netwib_buf *ppkt)
{
  netwib_uint32 msgsize;

  msgsize = netwib__buf_ref_data_size(ppkt);

  if (isnaked) {
    if (msgsize < NETWOX_SMBNAKEDHDR_LEN) {
      return(NETWOX_ERR_INTERNALERROR);
    }
    msgsize -= NETWOX_SMBNAKEDHDR_LEN;
    ppkt->endoffset = ppkt->beginoffset;
    netwib_er(netwox_pkt_append_smbnaked_msghdr(msgsize, ppkt));
    ppkt->endoffset = ppkt->beginoffset + NETWOX_SMBNAKEDHDR_LEN + msgsize;
  } else {
    if (msgsize < NETWOX_SMBNBTSSHDR_LEN) {
      return(NETWOX_ERR_INTERNALERROR);
    }
    msgsize -= NETWOX_SMBNBTSSHDR_LEN;
    ppkt->endoffset = ppkt->beginoffset;
    netwib_er(netwox_pkt_append_smbnbtss_msghdr(msgsize, ppkt));
    ppkt->endoffset = ppkt->beginoffset + NETWOX_SMBNBTSSHDR_LEN + msgsize;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_decode_smbmsghdr(netwib_constbuf *ppkt,
                                       netwib_bool isnaked,
                                       netwib_bufext *psmbmsg,
                                       netwib_uint32 *pskipsize)
{

  if (isnaked) {
    netwox_smbnaked smbnaked;
    netwib_er(netwox_pkt_decode_smbnaked(ppkt, &smbnaked, pskipsize));
    *psmbmsg = smbnaked.smbpkt;
  } else {
    netwox_smbnbtss smbnbtss;
    netwib_er(netwox_pkt_decode_smbnbtss(ppkt, &smbnbtss, pskipsize));
    if (smbnbtss.smbnbtsstype != NETWOX_SMBNBTSSTYPE_MSG) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    *psmbmsg = smbnbtss.payload.msg.smbpkt;
  }

  return(NETWIB_ERR_OK);
}
