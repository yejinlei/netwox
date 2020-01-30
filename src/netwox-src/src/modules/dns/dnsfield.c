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
netwib_err netwox_dnsfield_dom_initcomp(netwib_constbuf *pfullpkt,
                                        netwib_constbuf *pdatain,
                                        netwib_bool datainisemail,
                                        netwib_buf *pdataout)
{
  netwib_data datain, dataout, pkt, chunk;
  netwib_uint32 datainsize, dataoutsize=0, pktsize, chunksize;
  netwib_uint32 i, j, size, sizepos, offset;
  netwib_byte c, *pc;

  datain = netwib__buf_ref_data_ptr(pdatain);
  datainsize = netwib__buf_ref_data_size(pdatain);

  /* worst case is "a" -> "1'a'0" (n->2n+1) */
  netwib_er(netwib_buf_wantspace(pdataout, 2*datainsize+1, &dataout));

  /* for emails, search '@' in "a.b@c" and ignore '.' in "a.b" */
  i = 0;
  j = 1;
  sizepos = 0;
  if (datainisemail) {
    pc = netwib_c_memchr(datain, '@', datainsize);
    if (pc != NULL) {
      size = pc - datain;
      if (size > 63) {
        return(NETWIB_ERR_PATOOHIGH);
      }
      if (size) {
        dataout[sizepos] = (netwib_byte)size;
        netwib_c_memcpy(dataout+j, datain, size);
        i += size + 1;
        j += size + 1;
        sizepos = size + 1;
      }
    }
  }

  /* separate in chunks */
  size = 0;
  while (i < datainsize) {
    c = datain[i];
    if (c == '.') {
      if (size) {
        dataout[sizepos] = (netwib_byte)size;
        sizepos = j;
        size = 0;
        j++;
      }
    } else {
      dataout[j] = c;
      size++;
      j++;
    }
    if (size > 63) {
      return(NETWIB_ERR_PATOOHIGH);
    }
    i++;
  }

  /* last part */
  if (size == 0) {
    dataout[sizepos] = 0;
    dataoutsize = sizepos + 1;
  } else {
    dataout[sizepos] = (netwib_byte)size;
    dataout[j] = 0;
    dataoutsize = j + 1;
  }

  /* leave if compression was not requested */
  if (pfullpkt == NULL) {
    pdataout->endoffset += dataoutsize;
    return(NETWIB_ERR_OK);
  }

  /* now, look if this data was not already in pkt (compression) */
  pkt = netwib__buf_ref_data_ptr(pfullpkt);
  pktsize = netwib__buf_ref_data_size(pfullpkt);
  chunk = dataout;
  chunksize = dataoutsize;
  while (NETWIB_TRUE) {
    if (chunksize == 1) {
      /* case of last 0x00 */
      break;
    }
    pc = netwib_c_memmem(pkt, pktsize, chunk, chunksize);
    if (pc != NULL) {
      if (pc != chunk) {
        offset = pc - pkt;
        if (offset < 0x3FFF) {
          dataoutsize = chunk - dataout + 2;
          dataout[dataoutsize-2] = (netwib_byte)(0xC0 | (offset>>8));
          dataout[dataoutsize-1] = (netwib_byte)(offset & 0xFF);
          break;
        }
      }
    }
    size = chunk[0];
    chunk += size + 1;
    chunksize -= size + 1;
  }

  pdataout->endoffset += dataoutsize;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsfield_dom_uncdecode(netwib_constbuf *pfullpkt,
                                         netwib_buf *pdatain,
                                         netwib_bool datainisemail,
                                         netwib_buf *pdataout)
{
  netwib_data datain, dataout, datainori;
  netwib_uint32 datainsize, dataoutsize, dataoutsizemax;
  netwib_uint32 type, offset, chunksize;
  netwib_byte c;
  netwib_char dot;
  netwib_bool adddot, datainhasskipedtopointer, lastwaspointer;

  datainsize = netwib__buf_ref_data_size(pdatain);
  if (datainsize == 0) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  datain = netwib__buf_ref_data_ptr(pdatain);
  datainori = datain;

  /* We allocate the size of the full packet. A valid field cannot have
     a size greater than that. However, an attacker can create a loop with
     pointers in a DNS packet, so we'll check if we don't exceed the allocated
     size with "if (dataoutsize + x > dataoutsizemax)".
  */
  if (pfullpkt == NULL) {
    /* cf. dnsrdata.h which explains when pfullpkt can be NULL ; in this
       case, the size of pdatain is sufficient */
    dataoutsizemax = netwib__buf_ref_data_size(pdatain);
  } else {
    dataoutsizemax = netwib__buf_ref_data_size(pfullpkt);
  }
  netwib_er(netwib_buf_wantspace(pdataout, dataoutsizemax, &dataout));
  dataoutsize = 0;

  /* for emails, the first item to add is a '@' and not a dot */
  dot = '.';
  if (datainisemail) dot = '@';

  /* reassemble chunks */
  adddot = NETWIB_FALSE;
  datainhasskipedtopointer = NETWIB_FALSE;
  lastwaspointer = NETWIB_FALSE;
  while(NETWIB_TRUE) {
    /* first byte indicates type|offset or type|chunksize */
    if (datainsize == 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    c = *datain++;
    if (c == 0) {
      if (dataoutsize + 1 > dataoutsizemax) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      *dataout++ = '.';
      dataoutsize++;
      break;
    }
    datainsize--;
    type = c >> 6;
    if (type == 0) {
      /* chunk */
      chunksize = c & 0x3F;
      if (chunksize > datainsize) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      if (adddot) {
        if (dataoutsize + 1 > dataoutsizemax) {
          return(NETWIB_ERR_NOTCONVERTED);
        }
        *dataout++ = dot;
        dataoutsize++;
        dot = '.';
      }
      if (dataoutsize + chunksize > dataoutsizemax) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib_c_memcpy(dataout, datain, chunksize);
      datain += chunksize;
      datainsize -= chunksize;
      dataout += chunksize;
      dataoutsize += chunksize;
      adddot = NETWIB_TRUE;
      lastwaspointer = NETWIB_FALSE;
    } else if (type == 3) {
      /* pointer */
      if (lastwaspointer) {
        /* we cannot have a pointer pointing to a pointer */
        return(NETWIB_ERR_NOTCONVERTED);
      }
      if (pfullpkt == NULL) {
        /* cf. dnsrdata.h which explains when pfullpkt can be NULL */
        return(NETWIB_ERR_NOTCONVERTED);
      }
      offset = (c & 0x3F) << 8;
      if (datainsize == 0) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      offset |= *datain++;
      datainsize--;
      if (offset >= netwib__buf_ref_data_size(pfullpkt)) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      /* we'll now reuse datain with a new meaning : save position */
      if (!datainhasskipedtopointer) {
        pdatain->beginoffset += datain - datainori;
        datainhasskipedtopointer = NETWIB_TRUE;
      }
      datain = netwib__buf_ref_data_ptr(pfullpkt) + offset;
      datainsize = netwib__buf_ref_data_size(pfullpkt) - offset;
      lastwaspointer = NETWIB_TRUE;
    } else {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }
  pdataout->endoffset += dataoutsize;
  if (!datainhasskipedtopointer) {
    pdatain->beginoffset += datain - datainori;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsfield_text_init(netwib_constbuf *pdatain,
                                     netwib_buf *pdataout)
{
  netwib_data datain, dataout;
  netwib_uint32 datainsize;

  datainsize = netwib__buf_ref_data_size(pdatain);
  if (datainsize > 255) {
    return(NETWIB_ERR_PATOOHIGH);
  }
  datain = netwib__buf_ref_data_ptr(pdatain);

  netwib_er(netwib_buf_wantspace(pdataout, datainsize+1, &dataout));
  dataout[0] = (netwib_byte)datainsize;
  netwib_c_memcpy(dataout+1, datain, datainsize);
  pdataout->endoffset += datainsize + 1;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsfield_text_decode(netwib_buf *pdatain,
                                       netwib_buf *pdataout)
{
  netwib_data datain, dataout;
  netwib_uint32 datainsize, size;

  datainsize = netwib__buf_ref_data_size(pdatain);

  if (datainsize == 0) {
    return(NETWIB_ERR_DATAMISSING);
  }
  datain = netwib__buf_ref_data_ptr(pdatain);
  size = datain[0];
  if (size >= datainsize) {
    return(NETWIB_ERR_DATAMISSING);
  }

  netwib_er(netwib_buf_wantspace(pdataout, size, &dataout));
  netwib_c_memcpy(dataout, datain+1, size);
  pdatain->beginoffset += size + 1;
  pdataout->endoffset += size;

  return(NETWIB_ERR_OK);
}

