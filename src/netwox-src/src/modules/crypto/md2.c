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
/* Permutation table: {0..255} => {0..255}.
   RFC says this table is constructed from the digits of PI, but I don't
   know how.
*/
static unsigned char S[256] = {
  41, 46, 67, 201, 162, 216, 124, 1,     61, 54, 84, 161, 236, 240, 6, 19,
  98, 167, 5, 243, 192, 199, 115, 140,   152, 147, 43, 217, 188, 76, 130, 202,
  30, 155, 87, 60, 253, 212, 224, 22,    103, 66, 111, 24, 138, 23, 229, 18,
  190, 78, 196, 214, 218, 158, 222, 73,  160, 251, 245, 142, 187, 47, 238, 122,
  169, 104, 121, 145, 21, 178, 7, 63,    148, 194, 16, 137, 11, 34, 95, 33,
  128, 127, 93, 154, 90, 144, 50, 39,    53, 62, 204, 231, 191, 247, 151, 3,
  255, 25, 48, 179, 72, 165, 181, 209,   215, 94, 146, 42, 172, 86, 170, 198,
  79, 184, 56, 210, 150, 164, 125, 182,  118, 252, 107, 226, 156, 116, 4, 241,
  69, 157, 112, 89, 100, 113, 135, 32,   134, 91, 207, 101, 230, 45, 168, 2,
  27, 96, 37, 173, 174, 176, 185, 246,   28, 70, 97, 105, 52, 64, 126, 15,
  85, 71, 163, 35, 221, 81, 175, 58,     195, 92, 249, 206, 186, 197, 234, 38,
  44, 83, 13, 110, 133, 40, 132, 9,      211, 223, 205, 244, 65, 129, 77, 82,
  106, 220, 55, 200, 108, 193, 171, 250, 36, 225, 123, 8, 12, 189, 177, 74,
  120, 136, 149, 139, 227, 99, 232, 109, 233, 203, 213, 254, 59, 0, 29, 57,
  242, 239, 183, 14, 102, 88, 208, 228,  166, 119, 114, 248, 235, 117, 75, 10,
  49, 68, 80, 180, 143, 237, 31, 26,     219, 153, 141, 51, 159, 17, 131, 20
};

/*-------------------------------------------------------------*/
netwib_err netwox_md2_init(netwox_md2_context *pcontext)
{
  netwib_c_memset(pcontext->state, 0, sizeof(pcontext->state));
  netwib_c_memset(pcontext->checksum, 0, sizeof(pcontext->checksum));
  pcontext->totalbytes = 0;
  pcontext->bufsize = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* update a 16 bytes block */
static netwib_err netwox_priv_md2_update16(netwox_md2_context *pcontext,
                                           netwib_constdata data)
{
  netwib_uint8 x[48];
  netwib_uint32 i, j, k;

  /* update state of the block */
  /* create a 48 bytes array */
  netwib_c_memcpy(x, pcontext->state, 16);
  netwib_c_memcpy(x+16, data, 16);
  for (i = 0; i < 16; i++) {
    x[i+32] = (netwib_uint8)(pcontext->state[i] ^ data[i]);
  }
  /* encrypt */
  k = 0;
  for (i = 0; i < 18; i++) {
    for (j = 0; j < 48; j++) {
      x[j] ^= S[k];
      k = x[j];
    }
    k = (k + i) & 0xff; /* increment modulo 256 */
  }
  /* save state */
  netwib_c_memcpy(pcontext->state, x, 16);

  /* update checksum of the block */
  k = pcontext->checksum[15];
  for (i = 0; i < 16; i++) {
    pcontext->checksum[i] ^= S[data[i] ^ k];
    k = pcontext->checksum[i];
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* take data from context */
static netwib_err netwox_priv_md2_update16_ctx(netwox_md2_context *pcontext)
{

  if (pcontext->bufsize != NETWOX_MD2_BLOCK_LEN) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  netwib_er(netwox_priv_md2_update16(pcontext, pcontext->buf));
  pcontext->bufsize = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_md2_update(netwox_md2_context *pcontext,
                             netwib_constbuf *pbuf)
{
  netwib_data data;
  netwib_uint32 datasize;

  datasize = netwib__buf_ref_data_sizenull(pbuf);
  if (datasize == 0) {
    return(NETWIB_ERR_OK);
  }
  data = netwib__buf_ref_data_ptr(pbuf);

  pcontext->totalbytes += datasize;

  if (pcontext->bufsize) {
    while (datasize && (pcontext->bufsize < NETWOX_MD2_BLOCK_LEN)) {
      pcontext->buf[pcontext->bufsize++] = *data++;
      datasize--;
    }
    if (pcontext->bufsize == NETWOX_MD2_BLOCK_LEN) {
      netwib_er(netwox_priv_md2_update16_ctx(pcontext));
    }
  }

  while (datasize >= NETWOX_MD2_BLOCK_LEN) {
    netwib_er(netwox_priv_md2_update16(pcontext, data));
    data += NETWOX_MD2_BLOCK_LEN;
    datasize -= NETWOX_MD2_BLOCK_LEN;
  }
  while (datasize) {
    pcontext->buf[pcontext->bufsize++] = *data++;
    datasize--;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_md2_final(netwox_md2_context *pcontext,
                            netwib_buf *pmd)
{
  netwib_uint8 pad[NETWOX_MD2_BLOCK_LEN];
  netwib_uint32 padsize, i;
  netwib_buf buf;

  /* add padding (even if not needed) */
  padsize = NETWOX_MD2_BLOCK_LEN - pcontext->bufsize;
  for (i = 0; i < padsize; i++) {
    pad[i] = (netwib_uint8)padsize; /* store size in the value */
  }
  netwib_er(netwib_buf_init_ext_arrayfilled(pad, padsize, &buf));
  netwib_er(netwox_md2_update(pcontext, &buf));

  /* add checksum */
  netwib_er(netwox_priv_md2_update16(pcontext, pcontext->checksum));

  /* state contains the result */
  netwib_er(netwib_buf_append_data(pcontext->state, 16, pmd));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_md2_compute(netwib_constbuf *pbuf,
                              netwib_buf *pmd)
{
  netwox_md2_context ctx;

  netwib_er(netwox_md2_init(&ctx));
  netwib_er(netwox_md2_update(&ctx, pbuf));
  netwib_er(netwox_md2_final(&ctx, pmd));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_md2_test(netwib_conststring data,
                                       netwib_conststring mdwanted)
{
  netwib_buf buf, md;

  netwib_er(netwib_buf_init_ext_string(data, &buf));
  netwib_er(netwib_buf_init_mallocdefault(&md));
  netwib_er(netwox_md2_compute(&buf, &md));
  netwib_er(netwox_buf_check_strmixed(&md, mdwanted));
  netwib_er(netwib_buf_close(&md));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_md2_test()
{
  /* rfc test suite */
  netwib_er(netwox_priv_md2_test("", "8350e5a3e24c153df2275c9f80692773"));
  netwib_er(netwox_priv_md2_test("a", "32ec01ec4a6dac72c0ab96fb34c0b5d1"));
  netwib_er(netwox_priv_md2_test("abc", "da853b0d3f88d99b30283a69e6ded6bb"));
  netwib_er(netwox_priv_md2_test("message digest", "ab4f496bfb2a530b219ff33031fe06b0"));
  netwib_er(netwox_priv_md2_test("abcdefghijklmnopqrstuvwxyz", "4e8ddff3650292ab5a4108c3aa47940b"));
  netwib_er(netwox_priv_md2_test("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "da33def2a42df13975352846c30338cd"));
  netwib_er(netwox_priv_md2_test("12345678901234567890123456789012345678901234567890123456789012345678901234567890", "d5976f79d83d3a0dc9806c3c66f3efd8"));

  return(NETWIB_ERR_OK);
}
