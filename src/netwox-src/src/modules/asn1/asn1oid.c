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
netwib_err netwox_asn1oid_init(netwib_bufpool *pbufpool,
                               netwox_asn1oid *pasn1oid)
{
  pasn1oid->pbufpool = pbufpool;
  netwib_er(netwib_array_init(sizeof(netwib_uint32), 0, &pasn1oid->id));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1oid_close(netwox_asn1oid *pasn1oid)
{
  netwib_er(netwib_array_close(&pasn1oid->id));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1oid_add_id(netwox_asn1oid *pasn1oid,
                                 netwib_uint32 subid)
{
  netwib_uint32 *pui;

  netwib_er(netwib_array_ctl_set_size(&pasn1oid->id, pasn1oid->id.size+1));
  pui = (netwib_uint32*)pasn1oid->id.p[pasn1oid->id.size-1];
  *pui = subid;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_buf_decode_asn1oid(netwib_constbuf *pbuf,
                                     netwox_asn1oid *pasn1oid)
{
  netwib_buf buf;
  netwib_data data, pc;
  netwib_uint32 datasize, subid;

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);

  if (datasize && data[0] == '.') {
    data++;
    datasize--;
  }

  while(NETWIB_TRUE) {
    if (datasize == 0) break;
    pc = netwib_c_memchr(data, '.', datasize);
    if (pc != NULL) {
      netwib_er(netwib_buf_init_ext_arrayfilled(data, pc - data, &buf));
      datasize -= pc - data + 1;
      data = pc + 1;
    } else {
      netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, &buf));
      datasize = 0;
    }
    netwib_er(netwib_buf_decode_fmt(&buf, "%{uint32}%$", &subid));
    netwib_er(netwox_asn1oid_add_id(pasn1oid, subid));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_buf_append_asn1oid(netwox_constasn1oid *pasn1oid,
                                     netwib_buf *pbuf)
{
  netwib_uint32 i, *pui;

  for (i = 0; i < pasn1oid->id.size; i++) {
    pui = (netwib_uint32*)pasn1oid->id.p[i];
    netwib_er(netwib_buf_append_fmt(pbuf, ".%{uint32}", *pui));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1oid_dup(netwox_constasn1oid *pasn1oidsrc,
                              netwox_asn1oid *pasn1oiddst)
{
  netwib_uint32 i, *pui;

  for (i = 0; i < pasn1oidsrc->id.size; i++) {
    pui = (netwib_uint32*)pasn1oidsrc->id.p[i];
    netwib_er(netwox_asn1oid_add_id(pasn1oiddst, *pui));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1oid_cmp(netwox_constasn1oid *pasn1oid1,
                              netwox_constasn1oid *pasn1oid2,
                              netwib_cmp *pcmp)
{
  netwib_uint32 i;

  if (pasn1oid1->id.size > pasn1oid2->id.size) {
    *pcmp = NETWIB_CMP_GT;
    return(NETWIB_ERR_OK);
  }
  if (pasn1oid1->id.size < pasn1oid2->id.size) {
    *pcmp = NETWIB_CMP_LT;
    return(NETWIB_ERR_OK);
  }

  for (i = 0; i < pasn1oid1->id.size; i++) {
    if (*((netwib_uint32*)(pasn1oid1->id.p[i])) >
        *((netwib_uint32*)(pasn1oid2->id.p[i]))) {
      *pcmp = NETWIB_CMP_GT;
      return(NETWIB_ERR_OK);
    }
    if (*((netwib_uint32*)(pasn1oid1->id.p[i])) <
        *((netwib_uint32*)(pasn1oid2->id.p[i]))) {
      *pcmp = NETWIB_CMP_LT;
      return(NETWIB_ERR_OK);
    }
  }

  *pcmp = NETWIB_CMP_EQ;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_asn1oid_init_arg(netwib_bufpool *pbufpool,
                                   netwox_arg *parg,
                                   netwib_char key,
                                   netwox_asn1oid *pasn1oid)
{
  netwib_buf buf;

  netwib_er(netwox_asn1oid_init(pbufpool, pasn1oid));
  netwib_er(netwox_arg_buf(parg, key, &buf));
  netwib_er(netwox_buf_decode_asn1oid(&buf, pasn1oid));

  return(NETWIB_ERR_OK);
}
