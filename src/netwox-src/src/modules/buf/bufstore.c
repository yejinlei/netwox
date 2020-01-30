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
netwib_err netwox_bufstore_create(netwib_constbuf *pbuf,
                                  netwib_ptr *ppitem)
{
  netwox_bufstore *pbufstore;
  netwib_data data;
  netwib_uint32 datasize;
  netwib_ptr ptr;

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);

  netwib_er(netwib_ptr_malloc(NETWOX_BUFSTORE_ALIGN+datasize+1, &ptr));
  pbufstore = (netwox_bufstore *)ptr;
  *ppitem = ptr;

  pbufstore->data = (netwib_data)ptr + NETWOX_BUFSTORE_ALIGN;
  pbufstore->datasize = datasize;
  netwib_c_memcpy(pbufstore->data, data, datasize);
  pbufstore->data[datasize] = '\0'; /* for easy netwib_buf_ref_string */

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_bufstore_decode(netwib_constptr pitem,
                                  netwib_bufext *pbuf)
{
  const netwox_bufstore *pbufstore = (const netwox_bufstore *)pitem;

  netwib_er(netwib_buf_init_ext_array(pbufstore->data,
                                      pbufstore->datasize+1,
                                      0, pbufstore->datasize, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_bufstore_erase(netwib_ptr pitem)
{
  netwib_er(netwib_ptr_free(&pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_bufstore_duplicate(netwib_constptr pitem,
                                     netwib_ptr *pdupofitem)
{
  const netwox_bufstore *pbufstore = (const netwox_bufstore *)pitem;
  netwox_bufstore *pbufstoredup;
  netwib_ptr ptr;

  netwib_er(netwib_ptr_malloc(NETWOX_BUFSTORE_ALIGN+pbufstore->datasize+1, &ptr));
  pbufstoredup = (netwox_bufstore *)ptr;
  *pdupofitem = ptr;

  pbufstoredup->data = (netwib_data)ptr + NETWOX_BUFSTORE_ALIGN;
  pbufstoredup->datasize = pbufstore->datasize;
  netwib_c_memcpy(pbufstoredup->data, pbufstore->data, pbufstore->datasize);
  pbufstoredup->data[pbufstoredup->datasize] = '\0';

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_bufstore_hash_key(netwib_byte keyarray[NETWOX_BUFSTORE_HASH_KEY_SIZE],
                                    netwib_uint32 id,
                                    netwib_buf *pkey)
{
  keyarray[0] = netwib_c2_uint32_0(id);
  keyarray[1] = netwib_c2_uint32_1(id);
  keyarray[2] = netwib_c2_uint32_2(id);
  keyarray[3] = netwib_c2_uint32_3(id);
  netwib_er(netwib_buf_init_ext_arrayfilled(keyarray, NETWOX_BUFSTORE_HASH_KEY_SIZE, pkey));
  return(NETWIB_ERR_OK);
}
