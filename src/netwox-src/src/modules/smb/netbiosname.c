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
netwib_err netwox_buf_append_netbiosname(netwib_constbuf *pnetbiosname,
                                         netwib_buf *pbuf)
{
  netwib_data data, dataout;
  netwib_uint32 datasize, dataoutsize, byte, field;

  datasize = netwib__buf_ref_data_size(pnetbiosname);
  if (datasize % 2) {
    return(NETWOX_ERR_BADSIZE);
  }
  data = netwib__buf_ref_data_ptr(pnetbiosname);
  dataoutsize = datasize / 2;
  netwib_er(netwib_buf_wantspace(pbuf, dataoutsize, &dataout));

  while(dataoutsize--) {
    field = *data++;
    if (field < 'A') {
      return(NETWOX_ERR_BADVALUE);
    }
    field -= 'A';
    if (field > 0xF) {
      return(NETWOX_ERR_BADVALUE);
    }
    byte = field << 4;
    field = *data++;
    if (field < 'A') {
      return(NETWOX_ERR_BADVALUE);
    }
    field -= 'A';
    if (field > 0xF) {
      return(NETWOX_ERR_BADVALUE);
    }
    byte |= field;
    *dataout++ = (netwib_byte)byte;
  }

  pbuf->endoffset += datasize / 2;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_buf_decode_netbiosname(netwib_constbuf *pbuf,
                                         netwib_buf *pnetbiosname)
{
  netwib_data data, dataout;
  netwib_uint32 datasize, dataoutsize, byte;

  datasize = netwib__buf_ref_data_size(pbuf);
  data = netwib__buf_ref_data_ptr(pbuf);
  dataoutsize = datasize * 2;
  netwib_er(netwib_buf_wantspace(pnetbiosname, dataoutsize, &dataout));

  while(datasize--) {
    byte = *data++;
    *dataout++ = (netwib_byte)('A' + (byte>>4));
    *dataout++ = (netwib_byte)('A' + (byte&0xF));
  }

  pnetbiosname->endoffset += dataoutsize;

  return(NETWIB_ERR_OK);
}

