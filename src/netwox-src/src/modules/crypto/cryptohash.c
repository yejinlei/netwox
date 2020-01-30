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
netwib_err netwox_cryptohash_size(netwox_cryptohash_type hashtype,
                                  netwib_uint32 *pblocksize,
                                  netwib_uint32 *phashsize)
{
  netwib_uint32 blocksize=0, hashsize=0;

  switch(hashtype) {
  case NETWOX_CRYPTOHASH_TYPE_MD2:
    blocksize = NETWOX_MD2_BLOCK_LEN;
    hashsize = NETWOX_MD2_HASH_LEN;
    break;
  case NETWOX_CRYPTOHASH_TYPE_MD4:
    blocksize = NETWOX_MD4_BLOCK_LEN;
    hashsize = NETWOX_MD4_HASH_LEN;
    break;
  case NETWOX_CRYPTOHASH_TYPE_MD5:
    blocksize = NETWOX_MD5_BLOCK_LEN;
    hashsize = NETWOX_MD5_HASH_LEN;
    break;
  case NETWOX_CRYPTOHASH_TYPE_RIPEMD128:
    blocksize = NETWOX_RIPEMD128_BLOCK_LEN;
    hashsize = NETWOX_RIPEMD128_HASH_LEN;
    break;
  case NETWOX_CRYPTOHASH_TYPE_RIPEMD160:
    blocksize = NETWOX_RIPEMD160_BLOCK_LEN;
    hashsize = NETWOX_RIPEMD160_HASH_LEN;
    break;
  case NETWOX_CRYPTOHASH_TYPE_SHA1:
    blocksize = NETWOX_SHA1_BLOCK_LEN;
    hashsize = NETWOX_SHA1_HASH_LEN;
    break;
  case NETWOX_CRYPTOHASH_TYPE_SHA224:
    blocksize = NETWOX_SHA224_BLOCK_LEN;
    hashsize = NETWOX_SHA224_HASH_LEN;
    break;
  case NETWOX_CRYPTOHASH_TYPE_SHA256:
    blocksize = NETWOX_SHA256_BLOCK_LEN;
    hashsize = NETWOX_SHA256_HASH_LEN;
    break;
  case NETWOX_CRYPTOHASH_TYPE_SHA384:
    blocksize = NETWOX_SHA384_BLOCK_LEN;
    hashsize = NETWOX_SHA384_HASH_LEN;
    break;
  case NETWOX_CRYPTOHASH_TYPE_SHA512:
    blocksize = NETWOX_SHA512_BLOCK_LEN;
    hashsize = NETWOX_SHA512_HASH_LEN;
    break;
  default:
    return(NETWOX_ERR_NOTIMPLEMENTED);
  }

  if (blocksize > NETWOX_CRYPTOHASH_BLOCK_MAXLEN) {
    return(NETWOX_ERR_INTERNALERROR);
  }
  if (hashsize > NETWOX_CRYPTOHASH_HASH_MAXLEN) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  if (pblocksize != NULL) {
    *pblocksize = blocksize;
  }
  if (phashsize != NULL) {
    *phashsize = hashsize;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_buf_append_cryptohash_type(netwox_cryptohash_type hashtype,
                                             netwib_buf *pbuf)
{
  netwib_conststring name = NULL;

  switch(hashtype) {
  case NETWOX_CRYPTOHASH_TYPE_MD2: name = "md2"; break;
  case NETWOX_CRYPTOHASH_TYPE_MD4: name = "md4"; break;
  case NETWOX_CRYPTOHASH_TYPE_MD5: name = "md5"; break;
  case NETWOX_CRYPTOHASH_TYPE_RIPEMD128: name = "ripemd128"; break;
  case NETWOX_CRYPTOHASH_TYPE_RIPEMD160: name = "ripemd160"; break;
  case NETWOX_CRYPTOHASH_TYPE_SHA1: name = "sha1"; break;
  case NETWOX_CRYPTOHASH_TYPE_SHA224: name = "sha224"; break;
  case NETWOX_CRYPTOHASH_TYPE_SHA256: name = "sha256"; break;
  case NETWOX_CRYPTOHASH_TYPE_SHA384: name = "sha384"; break;
  case NETWOX_CRYPTOHASH_TYPE_SHA512: name = "sha512"; break;
  default: return(NETWOX_ERR_INTERNALERROR);
  }

  netwib_er(netwib_buf_append_string(name, pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_cryptohash_init(netwox_cryptohash_type hashtype,
                                  netwox_cryptohash_context *pcontext)
{

  pcontext->hashtype = hashtype;
  switch(hashtype) {
  case NETWOX_CRYPTOHASH_TYPE_MD2:
    netwib_er(netwox_md2_init(&pcontext->value.md2));
    break;
  case NETWOX_CRYPTOHASH_TYPE_MD4:
    netwib_er(netwox_md4_init(&pcontext->value.md));
    break;
  case NETWOX_CRYPTOHASH_TYPE_MD5:
    netwib_er(netwox_md5_init(&pcontext->value.md));
    break;
  case NETWOX_CRYPTOHASH_TYPE_RIPEMD128:
    netwib_er(netwox_ripemd128_init(&pcontext->value.ripemd));
    break;
  case NETWOX_CRYPTOHASH_TYPE_RIPEMD160:
    netwib_er(netwox_ripemd160_init(&pcontext->value.ripemd));
    break;
  case NETWOX_CRYPTOHASH_TYPE_SHA1:
    netwib_er(netwox_sha1_init(&pcontext->value.sha1));
    break;
  case NETWOX_CRYPTOHASH_TYPE_SHA224:
    netwib_er(netwox_sha224_init(&pcontext->value.sha224256));
    break;
  case NETWOX_CRYPTOHASH_TYPE_SHA256:
    netwib_er(netwox_sha256_init(&pcontext->value.sha224256));
    break;
  case NETWOX_CRYPTOHASH_TYPE_SHA384:
    netwib_er(netwox_sha384_init(&pcontext->value.sha384512));
    break;
  case NETWOX_CRYPTOHASH_TYPE_SHA512:
    netwib_er(netwox_sha512_init(&pcontext->value.sha384512));
    break;
  default:
    return(NETWOX_ERR_NOTIMPLEMENTED);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_cryptohash_update(netwox_cryptohash_context *pcontext,
                                    netwib_constbuf *pbuf)
{

  switch(pcontext->hashtype) {
  case NETWOX_CRYPTOHASH_TYPE_MD2:
    netwib_er(netwox_md2_update(&pcontext->value.md2, pbuf));
    break;
  case NETWOX_CRYPTOHASH_TYPE_MD4:
  case NETWOX_CRYPTOHASH_TYPE_MD5:
    netwib_er(netwox_md_update(&pcontext->value.md, pbuf));
    break;
  case NETWOX_CRYPTOHASH_TYPE_RIPEMD128:
  case NETWOX_CRYPTOHASH_TYPE_RIPEMD160:
    netwib_er(netwox_ripemd_update(&pcontext->value.ripemd, pbuf));
    break;
  case NETWOX_CRYPTOHASH_TYPE_SHA1:
    netwib_er(netwox_sha1_update(&pcontext->value.sha1, pbuf));
    break;
  case NETWOX_CRYPTOHASH_TYPE_SHA224:
  case NETWOX_CRYPTOHASH_TYPE_SHA256:
    netwib_er(netwox_sha224256_update(&pcontext->value.sha224256, pbuf));
    break;
  case NETWOX_CRYPTOHASH_TYPE_SHA384:
  case NETWOX_CRYPTOHASH_TYPE_SHA512:
    netwib_er(netwox_sha384512_update(&pcontext->value.sha384512, pbuf));
    break;
  default:
    return(NETWOX_ERR_NOTIMPLEMENTED);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_cryptohash_final(netwox_cryptohash_context *pcontext,
                                   netwib_buf *phash)
{

  switch(pcontext->hashtype) {
  case NETWOX_CRYPTOHASH_TYPE_MD2:
    netwib_er(netwox_md2_final(&pcontext->value.md2, phash));
    break;
  case NETWOX_CRYPTOHASH_TYPE_MD4:
  case NETWOX_CRYPTOHASH_TYPE_MD5:
    netwib_er(netwox_md_final(&pcontext->value.md, phash));
    break;
  case NETWOX_CRYPTOHASH_TYPE_RIPEMD128:
  case NETWOX_CRYPTOHASH_TYPE_RIPEMD160:
    netwib_er(netwox_ripemd_final(&pcontext->value.ripemd, phash));
    break;
  case NETWOX_CRYPTOHASH_TYPE_SHA1:
    netwib_er(netwox_sha1_final(&pcontext->value.sha1, phash));
    break;
  case NETWOX_CRYPTOHASH_TYPE_SHA224:
  case NETWOX_CRYPTOHASH_TYPE_SHA256:
    netwib_er(netwox_sha224256_final(&pcontext->value.sha224256, phash));
    break;
  case NETWOX_CRYPTOHASH_TYPE_SHA384:
  case NETWOX_CRYPTOHASH_TYPE_SHA512:
    netwib_er(netwox_sha384512_final(&pcontext->value.sha384512, phash));
    break;
  default:
    return(NETWOX_ERR_NOTIMPLEMENTED);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_cryptohash_final_hexa(netwox_cryptohash_context *pcontext,
                                        netwib_buf *phexahash)
{
  netwib_byte digest[NETWOX_CRYPTOHASH_HASH_MAXLEN];
  netwib_buf buf;

  netwib_er(netwib_buf_init_ext_arraysizeofempty(digest, &buf));
  netwib_er(netwox_cryptohash_final(pcontext, &buf));
  netwib_er(netwib_buf_encode(&buf, NETWIB_ENCODETYPE_HEXA0, phexahash));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_cryptohash_compute(netwox_cryptohash_type hashtype,
                                     netwib_constbuf *pbuf,
                                     netwib_buf *phash)
{
  netwox_cryptohash_context ctx;

  netwib_er(netwox_cryptohash_init(hashtype, &ctx));
  netwib_er(netwox_cryptohash_update(&ctx, pbuf));
  netwib_er(netwox_cryptohash_final(&ctx, phash));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_cryptohash_compute_hexa(netwox_cryptohash_type hashtype,
                                          netwib_constbuf *pbuf,
                                          netwib_buf *phexahash)
{
  netwox_cryptohash_context ctx;

  netwib_er(netwox_cryptohash_init(hashtype, &ctx));
  netwib_er(netwox_cryptohash_update(&ctx, pbuf));
  netwib_er(netwox_cryptohash_final_hexa(&ctx, phexahash));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_cryptohash_compute_file(netwox_cryptohash_type hashtype,
                                          netwib_constbuf *pfilename,
                                          netwib_buf *phexahash)
{
  netwib_buf buf;
  netwib_io *pio;
  netwox_cryptohash_context ctx;
  netwib_err ret=NETWIB_ERR_OK;

  /* initialize */
  netwib_er(netwib_io_init_file_read(pfilename, &pio));
  netwib_er(netwox_cryptohash_init(hashtype, &ctx));

  /* main loop */
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  while (NETWIB_TRUE) {
    /* read */
    netwib__buf_reinit(&buf);
    ret = netwib_io_read(pio, &buf);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret == NETWIB_ERR_DATAEND) {
      ret = NETWIB_ERR_OK;
      break;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* update */
    netwib_er(netwox_cryptohash_update(&ctx, &buf));
  }

  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwox_cryptohash_final_hexa(&ctx, phexahash));
  }

  /* close */
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_io_close(&pio));

  return(ret);
}
