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
netwib_err netwox_pkt_append_smbnaked_msghdr(netwib_uint32 msgsize,
                                             netwib_buf *ppkt)
{
  netwib_data data;

  if (msgsize > 0xFFFFFF) {
    return(NETWIB_ERR_PATOOHIGH);
  }

  netwib_er(netwib_buf_wantspace(ppkt, NETWOX_SMBNAKEDHDR_LEN, &data));
  netwib__data_append_uint32(data, msgsize);
  ppkt->endoffset += NETWOX_SMBNAKEDHDR_LEN;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_smbnaked(netwox_constsmbnaked *psmbnaked,
                                      netwib_buf *ppkt)
{
  netwib_uint32 msgsize;

  msgsize = netwib__buf_ref_data_size(&psmbnaked->smbpkt);
  netwib_er(netwox_pkt_append_smbnaked_msghdr(msgsize, ppkt));

  netwib_er(netwib_buf_append_buf(&psmbnaked->smbpkt, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_decode_smbnaked(netwib_constbuf *ppkt,
                                      netwox_smbnaked *psmbnaked,
                                      netwib_uint32 *pskipsize)
{
  netwib_data data;
  netwib_uint32 datasize, size;

  datasize = netwib__buf_ref_data_size(ppkt);
  if (datasize < NETWOX_SMBNAKEDHDR_LEN) {
    return(NETWIB_ERR_DATAMISSING);
  }
  if (pskipsize != NULL) *pskipsize = NETWOX_SMBNAKEDHDR_LEN;
  data = netwib__buf_ref_data_ptr(ppkt);

  netwib__data_decode_uint32(data, size);
  datasize -= NETWOX_SMBNAKEDHDR_LEN;
  if (datasize < size) {
    return(NETWIB_ERR_DATAMISSING);
  }
  if (pskipsize != NULL) *pskipsize = NETWOX_SMBNAKEDHDR_LEN + size;

  netwib_er(netwib_buf_init_ext_arrayfilled(data, size, &psmbnaked->smbpkt));

  return(NETWIB_ERR_OK);
}
