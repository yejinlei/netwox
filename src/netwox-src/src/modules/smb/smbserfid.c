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
netwib_err netwox_smbserfid_item_init(netwox_smbserfid_item *pitem)
{
  netwib_er(netwib_buf_init_mallocdefault(&pitem->realname));
  pitem->deleteonclose = NETWIB_FALSE;
  pitem->fidtype = NETWOX_SMBSERFID_FIDTYPE_UNKNOWN;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserfid_item_value_setdefault(netwox_smbserfid_item *pitem,
                                                  netwox_smbserfid_fidtype fidtype)
{

  pitem->fidtype = fidtype;
  switch(fidtype) {
  case NETWOX_SMBSERFID_FIDTYPE_UNKNOWN :
    break;
  case NETWOX_SMBSERFID_FIDTYPE_REG :
    pitem->value.itemreg.regiotype = NETWOX_SMBSERFID_REGIOTYPE_NOTOPEN;
    netwib__uint64_init_uint32(0, pitem->value.itemreg.offset);
    netwib_er(netwib_buf_init_mallocdefault(&pitem->value.itemreg.readbuf));
    pitem->value.itemreg.readbuf.flags |= NETWIB_BUF_FLAGS_CANSLIDE;
    pitem->value.itemreg.readeofreached = NETWIB_FALSE;
    break;
  case NETWOX_SMBSERFID_FIDTYPE_DIR :
    break;
  default :
    return(NETWOX_ERR_INTERNALERROR);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserfid_item_close(netwox_smbserfid_item *pitem)
{
  netwib_er(netwib_buf_close(&pitem->realname));
  switch(pitem->fidtype) {
  case NETWOX_SMBSERFID_FIDTYPE_UNKNOWN :
    /* netwox_smbserfid_item_value_setdefault() was not yet used,
       so nothing to close */
    break;
  case NETWOX_SMBSERFID_FIDTYPE_REG :
    netwib_er(netwib_buf_close(&pitem->value.itemreg.readbuf));
    if (pitem->value.itemreg.regiotype != NETWOX_SMBSERFID_REGIOTYPE_NOTOPEN) {
      netwib_er(netwib_io_close(&pitem->value.itemreg.pio));
    }
    break;
  case NETWOX_SMBSERFID_FIDTYPE_DIR :
    break;
  default :
    return(NETWOX_ERR_INTERNALERROR);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserfid_item_svcname(netwib_constbuf *prootdir,
                                         netwox_smbserfid_item *pitem,
                                         netwib_buf *psvcname)
{
  /* path always start from root with a \ */
  netwib_er(netwib_buf_append_byte('\\', psvcname));
  netwib_er(netwib_path_init_relb(prootdir, &pitem->realname, psvcname));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserfid_itemptr_erase(netwib_ptr ptr)
{
  netwib_er(netwox_smbserfid_item_close((netwox_smbserfid_item *)ptr));
  netwib_er(netwib_ptr_free(&ptr));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserfid_itemptr_duplicate(netwib_constptr ptr,
                                              netwib_ptr *pptrdup)
{
  const netwox_smbserfid_item *pitem = (const netwox_smbserfid_item *)ptr;
  netwox_smbserfid_item *pitemdup;
  netwib_ptr ptrdup;

  netwib_er(netwox_smbserfid_itemptr_malloc(&ptrdup));
  pitemdup = (netwox_smbserfid_item *)ptrdup;
  *pptrdup = ptrdup;

  netwib_er(netwox_smbserfid_item_init(pitemdup));
  netwib_er(netwib_buf_append_buf(&pitem->realname, &pitemdup->realname));
  pitemdup->deleteonclose = pitem->deleteonclose;
  netwib_er(netwox_smbserfid_item_value_setdefault(pitemdup, pitem->fidtype));
  switch(pitem->fidtype) {
  case NETWOX_SMBSERFID_FIDTYPE_UNKNOWN :
    break;
  case NETWOX_SMBSERFID_FIDTYPE_REG :
    /* no io dup */
    pitemdup->value.itemreg.offset = pitem->value.itemreg.offset;
    netwib_er(netwib_buf_append_buf(&pitem->value.itemreg.readbuf,
                                    &pitemdup->value.itemreg.readbuf));
    pitemdup->value.itemreg.readeofreached = pitem->value.itemreg.readeofreached;
    break;
  case NETWOX_SMBSERFID_FIDTYPE_DIR :
    break;
  default :
    return(NETWOX_ERR_INTERNALERROR);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserfid_init(netwox_smbserfid *psmbserfid)
{
  psmbserfid->nextnumber = 1;
  netwib_er(netwox_smbserfid_item_hash_init(&psmbserfid->phash));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserfid_close(netwox_smbserfid *psmbserfid)
{
  netwib_er(netwox_smbserfid_item_hash_close(&psmbserfid->phash));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserfid_new(netwox_smbserfid *psmbserfid,
                                netwib_uint16 *pfid,
                                netwox_smbserfid_item **ppitem)
{
  netwib_byte keyarray[NETWOX_BUFSTORE_HASH_KEY_SIZE];
  netwox_smbserfid_item *pitem;
  netwib_ptr ptr;
  netwib_buf key;

  netwib_er(netwox_bufstore_hash_key(keyarray, psmbserfid->nextnumber, &key));

  netwib_er(netwox_smbserfid_itemptr_malloc(&ptr));
  pitem = (netwox_smbserfid_item *)ptr;

  netwib_er(netwox_smbserfid_item_init(pitem));

  netwib_er(netwib_hash_add(psmbserfid->phash, &key, ptr, NETWIB_TRUE));

  *pfid = psmbserfid->nextnumber;
  *ppitem = pitem;
  psmbserfid->nextnumber++;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserfid_search(netwox_smbserfid *psmbserfid,
                                   netwib_uint16 fid,
                                   netwox_smbserfid_item **ppitem)
{
  netwib_byte keyarray[NETWOX_BUFSTORE_HASH_KEY_SIZE];
  netwib_buf key;
  netwib_ptr ptr;
  netwib_err ret;

  netwib_er(netwox_bufstore_hash_key(keyarray, fid, &key));
  ret = netwib_hash_value(psmbserfid->phash, &key, &ptr);
  if (ret == NETWIB_ERR_OK) {
    *ppitem = (netwox_smbserfid_item *)ptr;
  }
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserfid_del(netwox_smbserfid *psmbserfid,
                                netwib_uint16 fid)
{
  netwib_byte keyarray[NETWOX_BUFSTORE_HASH_KEY_SIZE];
  netwib_buf key;

  netwib_er(netwox_bufstore_hash_key(keyarray, fid, &key));
  netwib_er(netwib_hash_del(psmbserfid->phash, &key, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}
