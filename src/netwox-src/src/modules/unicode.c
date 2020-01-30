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
netwib_err netwox_buf_decode_ucs2le(netwib_constbuf *pbuf8bit,
                                    netwib_buf *pbufucs2le)
{
  netwib_data dataa, datau;
  netwib_uint32 dataasize;

  dataasize = netwib__buf_ref_data_size(pbuf8bit);
  dataa = netwib__buf_ref_data_ptr(pbuf8bit);
  netwib_er(netwib_buf_wantspace(pbufucs2le, 2*dataasize, &datau));
  pbufucs2le->endoffset += 2 * dataasize;
  while (dataasize--) {
    *datau++ = *dataa++;
    *datau++ = 0;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_buf_decode_ucs2le_uc(netwib_constbuf *pbuf8bit,
                                       netwib_buf *pbufucs2le)
{
  netwib_data dataa, datau;
  netwib_uint32 dataasize;

  dataasize = netwib__buf_ref_data_size(pbuf8bit);
  dataa = netwib__buf_ref_data_ptr(pbuf8bit);
  netwib_er(netwib_buf_wantspace(pbufucs2le, 2*dataasize, &datau));
  pbufucs2le->endoffset += 2 * dataasize;
  while (dataasize--) {
    *datau++ = (netwib_byte)netwib_c2_uc(*dataa);
    dataa++;
    *datau++ = 0;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_buf_append_ucs2le(netwib_constbuf *pbufucs2le,
                                    netwib_buf *pbuf8bit)
{
  netwib_data dataa, datau;
  netwib_uint32 dataasize, datausize;

  datausize = netwib__buf_ref_data_size(pbufucs2le);
  if (datausize % 2) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  dataasize = datausize / 2;
  datau = netwib__buf_ref_data_ptr(pbufucs2le);

  netwib_er(netwib_buf_wantspace(pbuf8bit, dataasize, &dataa));
  while (dataasize--) {
    *dataa++ = *datau++;
    if (*datau) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    datau++;
  }
  pbuf8bit->endoffset += datausize / 2;

  return(NETWIB_ERR_OK);
}

