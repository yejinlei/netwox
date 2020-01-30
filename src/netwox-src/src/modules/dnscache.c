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
static netwib_err netwox_dnscache_key_init_ip(netwib_constip *pip,
                                              netwib_buf *pbuf)
{
  netwib_data data;

  switch(pip->iptype) {
  case NETWIB_IPTYPE_IP4 :
    netwib_er(netwib_buf_wantspace(pbuf, NETWIB_IP4_LEN + 1, &data));
    netwib__data_append_uint8(data, NETWIB_IP4_LEN + 1);
    netwib__data_append_uint32(data, pip->ipvalue.ip4);
    pbuf->endoffset += NETWIB_IP4_LEN + 1;
    break;
  case NETWIB_IPTYPE_IP6 :
    netwib_er(netwib_buf_wantspace(pbuf, NETWIB_IP6_LEN + 1, &data));
    netwib__data_append_uint8(data, NETWIB_IP6_LEN + 1);
    netwib_c_memcpy(data, pip->ipvalue.ip6.b, NETWIB_IP6_LEN);
    pbuf->endoffset += NETWIB_IP6_LEN + 1;
    break;
  default :
    return(NETWIB_ERR_PAIPTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_dnscache_ip_init_key(netwib_constbuf *pbuf,
                                              netwib_ip *pip)
{
  netwib_data data;
  netwib_uint32 datasize, keylen;

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);
  if (datasize == 0) {
    return(NETWIB_ERR_DATAMISSING);
  }

  netwib__data_decode_uint8(data, keylen);
  if (datasize < keylen) {
    return(NETWIB_ERR_DATAMISSING);
  }

  switch(keylen) {
  case NETWIB_IP4_LEN + 1 :
    pip->iptype = NETWIB_IPTYPE_IP4;
    netwib__data_decode_uint32(data, pip->ipvalue.ip4);
    break;
  case NETWIB_IP6_LEN + 1 :
    pip->iptype = NETWIB_IPTYPE_IP6;
    netwib_c_memcpy(pip->ipvalue.ip6.b, data, NETWIB_IP6_LEN);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_dnscache_item_init(netwib_constip *pip,
                                            netwib_constbuf *phost,
                                            netwib_buf *pbuf)
{
  netwib_er(netwox_dnscache_key_init_ip(pip, pbuf));
  netwib_er(netwib_buf_append_buf(phost, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_dnscache_item_decode(netwib_constbuf *pbuf,
                                              netwib_ip *pip,
                                              netwib_bufext *phost)
{
  netwib_data data;
  netwib_uint32 datasize, keylen;

  if (pip != NULL) {
    netwib_er(netwox_dnscache_ip_init_key(pbuf, pip));
  }

  if (phost != NULL) {
    data = netwib__buf_ref_data_ptr(pbuf);
    datasize = netwib__buf_ref_data_size(pbuf);
    keylen = data[0];
    netwib_er(netwib_buf_init_ext_arrayfilled(data + keylen,
                                              datasize - keylen, phost));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnscache_init(netwib_bool supportipfunction,
                                netwib_bool supporthostfunction,
                                netwox_dnscache *pdnscache)
{

  if (supportipfunction) {
    netwib_er(netwox_bufstore_hash_init(&pdnscache->phashhost));
  } else {
    pdnscache->phashhost = NULL;
  }
  if (supporthostfunction) {
    netwib_er(netwox_bufstore_hash_init(&pdnscache->phaship));
  } else {
    pdnscache->phaship = NULL;
  }

  netwib_er(netwib_thread_rwlock_init(&pdnscache->prwlock));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnscache_close(netwox_dnscache *pdnscache)
{

  if (pdnscache->phashhost != NULL) {
    netwib_er(netwox_bufstore_hash_close(&pdnscache->phashhost));
  }
  if (pdnscache->phaship != NULL) {
    netwib_er(netwox_bufstore_hash_close(&pdnscache->phaship));
  }

  netwib_er(netwib_thread_rwlock_close(&pdnscache->prwlock));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnscache_ip(netwox_dnscache *pdnscache,
                              netwib_constbuf *phost,
                              netwib_ip *pip)
{
  netwib_byte array[512];
  netwib_ptr pitem, pitem2=NULL;
  netwib_buf buf;
  netwib_err ret;

  if (pdnscache->phashhost == NULL) {
    return(netwib_ip_init_buf_best(phost, pip));
  }

  /* search host in the hash table */
  netwib_er(netwib_thread_rwlock_rdlock(pdnscache->prwlock,
                                        NETWIB_TIME_INFINITE, NULL));
  ret = netwib_hash_value(pdnscache->phashhost, phost, &pitem);
  if (ret == NETWIB_ERR_OK) {
    ret = netwox_bufstore_decode(pitem, &buf);
    if (ret == NETWIB_ERR_OK) {
      ret = netwox_dnscache_item_decode(&buf, pip, NULL);
    }
  }
  netwib_er(netwib_thread_rwlock_rdunlock(pdnscache->prwlock));
  if (ret != NETWIB_ERR_NOTFOUND) {
    return(ret);
  }

  /* query system */
  netwib_er(netwib_ip_init_buf_best(phost, pip));

  /* add value in both hash tables */
  /* create item to store in hashes */
  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));
  netwib_er(netwox_dnscache_item_init(pip, phost, &buf));
  netwib_er(netwox_bufstore_create(&buf, &pitem));
  if (pdnscache->phaship != NULL) {
    netwib_er(netwox_bufstore_create(&buf, &pitem2));
  }
  netwib_er(netwib_buf_close(&buf));
  /* create key from ip (for host hash, host is used) */
  netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));
  netwib_er(netwox_dnscache_key_init_ip(pip, &buf));
  /* add in hashes */
  netwib_er(netwib_thread_rwlock_wrlock(pdnscache->prwlock,
                                        NETWIB_TIME_INFINITE, NULL));
  ret = netwib_hash_add(pdnscache->phashhost, phost, pitem, NETWIB_TRUE);
  if (ret == NETWIB_ERR_OK) {
    if (pdnscache->phaship != NULL) {
      ret = netwib_hash_add(pdnscache->phaship, &buf, pitem2, NETWIB_TRUE);
    }
  }
  netwib_er(netwib_thread_rwlock_wrunlock(pdnscache->prwlock));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnscache_host(netwox_dnscache *pdnscache,
                                netwib_constip *pip,
                                netwib_buf *phost)
{
  netwib_byte array[512];
  netwib_ptr pitem, pitem2=NULL;
  netwib_buf buf, bufhost;
  netwib_uint32 originalsize;
  netwib_err ret;

  if (pdnscache->phaship == NULL) {
    return(netwib_buf_append_ip(pip, NETWIB_IP_ENCODETYPE_HN, phost));
  }

  originalsize = netwib__buf_ref_data_size(phost);

  /* search host in the hash table */
  netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));
  netwib_er(netwox_dnscache_key_init_ip(pip, &buf));
  netwib_er(netwib_thread_rwlock_rdlock(pdnscache->prwlock,
                                        NETWIB_TIME_INFINITE, NULL));
  ret = netwib_hash_value(pdnscache->phaship, &buf, &pitem);
  if (ret == NETWIB_ERR_OK) {
    ret = netwox_bufstore_decode(pitem, &buf);
    if (ret == NETWIB_ERR_OK) {
      ret = netwox_dnscache_item_decode(&buf, NULL, phost);
    }
  }
  netwib_er(netwib_thread_rwlock_rdunlock(pdnscache->prwlock));
  if (ret != NETWIB_ERR_NOTFOUND) {
    return(ret);
  }

  /* query system */
  netwib_er(netwib_buf_append_ip(pip, NETWIB_IP_ENCODETYPE_HN, phost));

  /* create a buffer containing only the name (and not
     "previousdata_host") */
  bufhost = *phost;
  bufhost.beginoffset += originalsize;

  /* add value in both hash tables */
  /* create item to store in hashes */
  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));
  netwib_er(netwox_dnscache_item_init(pip, &bufhost, &buf));
  netwib_er(netwox_bufstore_create(&buf, &pitem));
  if (pdnscache->phashhost != NULL) {
    netwib_er(netwox_bufstore_create(&buf, &pitem2));
  }
  netwib_er(netwib_buf_close(&buf));
  /* create key from ip (for host hash, host is used) */
  netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));
  netwib_er(netwox_dnscache_key_init_ip(pip, &buf));
  /* add in hashes */
  netwib_er(netwib_thread_rwlock_wrlock(pdnscache->prwlock,
                                        NETWIB_TIME_INFINITE, NULL));
  ret = netwib_hash_add(pdnscache->phaship, &buf, pitem, NETWIB_TRUE);
  if (ret == NETWIB_ERR_OK) {
    if (pdnscache->phashhost != NULL) {
      ret = netwib_hash_add(pdnscache->phashhost, &bufhost, pitem2,
                            NETWIB_TRUE);
    }
  }
  netwib_er(netwib_thread_rwlock_wrunlock(pdnscache->prwlock));

  return(ret);
}
