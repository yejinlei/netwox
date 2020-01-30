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
netwib_err netwib_checksum_buf(netwib_constbuf *pbuf,
                               netwib_uint16 *pchecksum)
{
  register netwib_uint32 sum;
  netwib_data data;
  netwib_uint32 datasize;
  netwib_uint16 resu;
  netwib_bool isodd;

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);

  /* compute sum */
  sum = 0;
  isodd = NETWIB_FALSE;
  if (datasize & 1) {
    isodd = NETWIB_TRUE;
    datasize--;
  }
  datasize = datasize / 2;
  /* work by multiples of two */
  while (datasize--) {
    sum += *data++;
    sum += (*data++) << 8;
  }
  /* work on last byte */
  if (isodd) {
    sum += *data;
  }

  /* we add low word, high word and carry */
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  /* result is the complement */
  resu = (netwib_uint16) ~ sum;
  /* reverse to have host format */
  if (pchecksum != NULL) {
    *pchecksum = (netwib_uint16) ((resu >> 8) | (resu << 8));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_checksum_init(netwib_uint32 *ptmpchecksum)
{
  *ptmpchecksum = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_checksum_update_buf(netwib_constbuf *pbuf,
                                      netwib_uint32 *ptmpchecksum)
{

  if (pbuf != NULL) {
    netwib_er(netwib_checksum_update_data(netwib__buf_ref_data_ptr(pbuf),
                                          netwib__buf_ref_data_size(pbuf),
                                          ptmpchecksum));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_checksum_update_data(netwib_constdata data,
                                       netwib_uint32 datasize,
                                       netwib_uint32 *ptmpchecksum)
{
  register netwib_uint32 sum;
  netwib_bool isodd;

  /* compute sum */
  sum = *ptmpchecksum;
  isodd = NETWIB_FALSE;
  if (datasize & 1) {
    isodd = NETWIB_TRUE;
    datasize--;
  }
  /* work by multiples of two */
  while (datasize) {
    sum += *data++;
    sum += (*data++) << 8;
    datasize -= 2;
  }
  /* work on last byte */
  if (isodd) {
    sum += *data;
  }
  *ptmpchecksum = sum;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_checksum_close(netwib_uint32 tmpchecksum,
                                 netwib_uint16 *pchecksum)
{
  netwib_uint32 sum;
  netwib_uint16 resu;

  /* we add low word, high word and carry */
  sum = tmpchecksum;
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  /* result is the complement */
  resu = (netwib_uint16) ~ sum;
  /* reverse to have host format */
  if (pchecksum != NULL) {
    *pchecksum = (netwib_uint16) ((resu >> 8) | (resu << 8));
  }

  return(NETWIB_ERR_OK);
}
