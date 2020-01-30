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
netwib_err netwox_xor_init(netwib_constbuf *pcypherkey,
                           netwox_xor_context *pcontext)
{
  if (netwib__buf_ref_data_size(pcypherkey) == 0) {
    return(NETWIB_ERR_PATOOLOW);
  }

  netwib_er(netwib_buf_init_mallocdefault(&pcontext->cypherkey));
  pcontext->cypherkey.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  netwib_er(netwib_buf_append_buf(pcypherkey, &pcontext->cypherkey));

  pcontext->nextoffset = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_xor_close(netwox_xor_context *pcontext)
{
  netwib_er(netwib_buf_close(&pcontext->cypherkey));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_xor_cypher(netwox_xor_context *pcontext,
                             netwib_buf *pbuf)
{
  netwib_uint32 bufsize, keysize, i;
  netwib_data key, buf;

  buf = netwib__buf_ref_data_ptr(pbuf);
  bufsize = netwib__buf_ref_data_size(pbuf);
  key = netwib__buf_ref_data_ptr(&pcontext->cypherkey);
  keysize = netwib__buf_ref_data_size(&pcontext->cypherkey);
  i = pcontext->nextoffset;
  while (bufsize--) {
    *buf++ ^= key[i];
    key[i] = (netwib_byte)(key[i] + 2*i + 1);
    i++;
    if (i == keysize) {
      i = 0;
    }
  }
  pcontext->nextoffset = i;
  pbuf->flags |= NETWIB_BUF_FLAGS_SENSITIVE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_xor_gene_key(netwib_bufpool *pbufpool,
                               netwib_constbuf *pinitseed,
                               netwib_uint32 minbytesize,
                               netwib_buf *pkey)
{
  netwib_buf *poldhash, *pnewhash;
  netwib_data data;
  netwib_byte tmp;
  netwib_uint32 i, datasize;

  /* recommended value */
  if (minbytesize == 0) {
    minbytesize = 1024;
  }

  /* generate a key */
  netwib_er(netwib_bufpool_buf_init(pbufpool, &poldhash));
  poldhash->flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pnewhash));
  pnewhash->flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  netwib_er(netwib_buf_append_buf(pinitseed, poldhash));
  datasize = (minbytesize+15)/16;
  for (i = 0; i < datasize; i++) {
    netwib__buf_reinit(pnewhash);
    netwib_er(netwox_md5_compute(poldhash, pnewhash));
    netwib_er(netwib_buf_append_buf(pnewhash, pkey));
    netwib__buf_reinit(poldhash);
    netwib_er(netwib_buf_append_buf(pnewhash, poldhash));
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pnewhash));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &poldhash));

  /* revert this key */
  data = netwib__buf_ref_data_ptr(pkey);
  datasize = netwib__buf_ref_data_size(pkey);
  for (i = 0; i < datasize/2; i ++) {
    tmp = data[i];
    data[i] = data[datasize-1-i];
    data[datasize-1-i] = tmp;
  }

  return(NETWIB_ERR_OK);
}
