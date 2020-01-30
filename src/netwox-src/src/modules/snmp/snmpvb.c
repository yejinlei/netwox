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
netwib_err netwox_snmpvb_init(netwib_bufpool *pbufpool,
                              netwox_snmpvb *psnmpvb)
{
  psnmpvb->pbufpool = pbufpool;
  netwib_er(netwox_asn1oid_init(pbufpool, &psnmpvb->oid));
  netwib_er(netwox_asn1data_init(pbufpool, &psnmpvb->data));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmpvb_close(netwox_snmpvb *psnmpvb)
{
  netwib_er(netwox_asn1data_close(&psnmpvb->data));
  netwib_er(netwox_asn1oid_close(&psnmpvb->oid));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1data_append_snmpvb(netwox_constsnmpvb *psnmpvb,
                                         netwox_asn1data *pasn1data)
{
  netwox_asn1data asn1data;

  netwib_er(netwox_asn1data_set_type(pasn1data, NETWOX_ASN1TAG_SEQUENCE));

  netwox__asn1data_append_oid_dup(pasn1data, &psnmpvb->oid);
  netwox__asn1data_append_asn1data_dup(pasn1data, &psnmpvb->data);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1data_decode_snmpvb(netwox_constasn1data *pasn1data,
                                         netwox_snmpvb *psnmpvb)
{
  netwox_asn1data **plevel1;

  netwox__asn1data_decode_sequence(pasn1data, plevel1);
  netwox__asn1data_check_sequence_size(pasn1data, 2);

  netwox__asn1data_decode_oid_dup(plevel1[0], &psnmpvb->oid);
  netwib_er(netwox_asn1data_dup(plevel1[1], &psnmpvb->data));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmpvb_show(netwox_constsnmpvb *psnmpvb,
                              netwib_buf *pbuf)
{
  netwib_byte array1[80], array2[80];
  netwib_buf buf1, buf2;
  netwib_string str1;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array1, &buf1));
  netwib_er(netwib_buf_append_string("  ", &buf1));
  netwib_er(netwox_buf_append_asn1oid(&psnmpvb->oid, &buf1));
  netwib_er(netwib_buf_ref_string(&buf1, &str1));

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array2, &buf2));
  netwib_er(netwox_asn1data_show(&psnmpvb->data,
                                 NETWIB_ENCODETYPE_SYNTH, &buf2));

  netwib_er(netwib_show_array_data(str1, &buf2, NETWIB_ENCODETYPE_DATA,
                                   ' ', pbuf));

  netwib_er(netwib_buf_close(&buf1));
  netwib_er(netwib_buf_close(&buf2));

  return(NETWIB_ERR_OK);
}

