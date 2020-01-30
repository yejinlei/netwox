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
netwib_err netwox_bufarraystore_create(netwib_ptr *ppitem) {
  netwib_array *parray;
  netwib_ptr ptr;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_array), &ptr));
  parray = (netwib_array *)ptr;
  *ppitem = ptr;

  netwib_er(netwox_bufarray_init(parray));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_bufarraystore_add_buf(netwib_ptr pitem,
                                        netwib_constbuf *pbuf) {
  netwib_array *parray;

  parray = (netwib_array *)pitem;
  netwib_er(netwox_bufarray_add_buf(parray, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_bufarraystore_erase(netwib_ptr pitem) {
  netwib_array *parray;

  parray = (netwib_array *)pitem;
  netwib_er(netwox_bufarray_close(parray));
  netwib_er(netwib_ptr_free(&pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_bufarraystore_duplicate(netwib_constptr pitem,
                                          netwib_ptr *pdupofitem) {
  netwib_constarray *parray;
  netwib_buf *pitembuf;
  netwib_uint32 i;

  netwib_er(netwox_bufarraystore_create(pdupofitem));

  parray = (netwib_constarray *)pitem;
  for (i = 0; i < parray->size; i++) {
    pitembuf = parray->p[i];
    netwib_er(netwox_bufarraystore_add_buf(pdupofitem, pitembuf));
  }

  return(NETWIB_ERR_OK);
}
