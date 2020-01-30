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
netwib_err netwox_snmpvbl_init(netwib_bufpool *pbufpool,
                               netwox_snmpvbl *psnmpvbl)
{
  psnmpvbl->pbufpool = pbufpool;
  netwib_er(netwib_array_init(sizeof(netwox_snmpvb), 0, &psnmpvbl->vb));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmpvbl_close(netwox_snmpvbl *psnmpvbl)
{
  netwib_uint32 i;

  for (i = 0; i < psnmpvbl->vb.size; i++) {
    netwib_er(netwox_snmpvb_close((netwox_snmpvb *)psnmpvbl->vb.p[i]));
  }

  netwib_er(netwib_array_close(&psnmpvbl->vb));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmpvbl_add(netwox_snmpvbl *psnmpvbl,
                              netwox_constsnmpvb *psnmpvb)
{
  netwox_snmpvb *psnmpvbi;

  netwib_er(netwib_array_ctl_set_size(&psnmpvbl->vb, psnmpvbl->vb.size+1));
  psnmpvbi = (netwox_snmpvb *)psnmpvbl->vb.p[psnmpvbl->vb.size-1];
  *psnmpvbi = *psnmpvb;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1data_append_snmpvbl(netwox_constsnmpvbl *psnmpvbl,
                                          netwox_asn1data *pasn1data)
{
  netwox_asn1data asn1data1;
  netwib_uint32 i;

  netwib_er(netwox_asn1data_set_type(pasn1data, NETWOX_ASN1TAG_SEQUENCE));

  for (i = 0; i < psnmpvbl->vb.size; i++) {
    netwib_er(netwox_asn1data_init(pasn1data->pbufpool, &asn1data1));
    netwib_er(netwox_asn1data_append_snmpvb((netwox_snmpvb*)psnmpvbl->vb.p[i],
                                            &asn1data1));
    netwib_er(netwox_asn1data_add_sequence(pasn1data, &asn1data1));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1data_decode_snmpvbl(netwox_constasn1data *pasn1data,
                                          netwox_snmpvbl *psnmpvbl)
{
  netwox_asn1data **plevel1;
  netwox_snmpvb snmpvb;
  netwib_uint32 i;
  netwib_err ret;

  netwox__asn1data_decode_sequence(pasn1data, plevel1);
  for (i = 0; i < pasn1data->value.sequence.seq.size; i++) {
    netwib_er(netwox_snmpvb_init(psnmpvbl->pbufpool, &snmpvb));
    ret = netwox_asn1data_decode_snmpvb(plevel1[i], &snmpvb);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwox_snmpvb_close(&snmpvb));
      return(ret);
    }
    netwib_er(netwox_snmpvbl_add(psnmpvbl, &snmpvb));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmpvbl_show(netwox_constsnmpvbl *psnmpvbl,
                               netwib_buf *pbuf)
{
  netwib_uint32 i;

  for (i = 0; i < psnmpvbl->vb.size; i++) {
    netwib_er(netwox_snmpvb_show((netwox_snmpvb*)psnmpvbl->vb.p[i], pbuf));
  }

  return(NETWIB_ERR_OK);
}
