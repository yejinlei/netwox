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
netwib_err netwox_sha1_init(netwox_sha1_context *pcontext)
{

  pcontext->a = 0x67452301u;
  pcontext->b = 0xefcdab89u;
  pcontext->c = 0x98badcfeu;
  pcontext->d = 0x10325476u;
  pcontext->e = 0xc3d2e1f0u;

  netwib__uint64_init_uint32(0, pcontext->totalbytes);

  pcontext->bufsize = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* update a 64 byte block */
static netwib_err netwox_priv_sha1_update64(netwox_sha1_context *pcontext,
                                            netwib_constdata data)
{
  register netwib_uint32 a, b, c, d, e, m;
  netwib_uint32 in[16], i;

  a = pcontext->a;
  b = pcontext->b;
  c = pcontext->c;
  d = pcontext->d;
  e = pcontext->e;

  for (i = 0; i < 16; i++) {
    netwib__data_decode_uint32(data, in[i]);
  }

#define SHA1_K1 0x5A827999u
#define SHA1_K2 0x6ED9EBA1u
#define SHA1_K3 0x8F1BBCDCu
#define SHA1_K4 0xCA62C1D6u
#define SHA1_F1(x,y,z) ( z ^ ( x & ( y ^ z ) ) )
#define SHA1_F2(x,y,z) ( x ^ y ^ z )
#define SHA1_F3(x,y,z) ( ( x & y ) | ( z & ( x | y ) ) )
#define SHA1_F4(x,y,z) ( x ^ y ^ z )

#define SHA1_M(i) ( m = in[i&0x0f] ^ in[(i-14)&0x0f] ^ in[(i-8)&0x0f] ^ in[(i-3)&0x0f] , (in[i&0x0f] = netwib_c2_uint32_rol(m, 1)) )
#define SHA1_R(a,b,c,d,e,f,k,m) { e += netwib_c2_uint32_rol(a, 5) + f(b, c, d) + k + m; b = netwib_c2_uint32_rol(b, 30); }
  SHA1_R(a, b, c, d, e, SHA1_F1, SHA1_K1, in[ 0]);
  SHA1_R(e, a, b, c, d, SHA1_F1, SHA1_K1, in[ 1]);
  SHA1_R(d, e, a, b, c, SHA1_F1, SHA1_K1, in[ 2]);
  SHA1_R(c, d, e, a, b, SHA1_F1, SHA1_K1, in[ 3]);
  SHA1_R(b, c, d, e, a, SHA1_F1, SHA1_K1, in[ 4]);
  SHA1_R(a, b, c, d, e, SHA1_F1, SHA1_K1, in[ 5]);
  SHA1_R(e, a, b, c, d, SHA1_F1, SHA1_K1, in[ 6]);
  SHA1_R(d, e, a, b, c, SHA1_F1, SHA1_K1, in[ 7]);
  SHA1_R(c, d, e, a, b, SHA1_F1, SHA1_K1, in[ 8]);
  SHA1_R(b, c, d, e, a, SHA1_F1, SHA1_K1, in[ 9]);
  SHA1_R(a, b, c, d, e, SHA1_F1, SHA1_K1, in[10]);
  SHA1_R(e, a, b, c, d, SHA1_F1, SHA1_K1, in[11]);
  SHA1_R(d, e, a, b, c, SHA1_F1, SHA1_K1, in[12]);
  SHA1_R(c, d, e, a, b, SHA1_F1, SHA1_K1, in[13]);
  SHA1_R(b, c, d, e, a, SHA1_F1, SHA1_K1, in[14]);
  SHA1_R(a, b, c, d, e, SHA1_F1, SHA1_K1, in[15]);
  SHA1_R(e, a, b, c, d, SHA1_F1, SHA1_K1, SHA1_M(16));
  SHA1_R(d, e, a, b, c, SHA1_F1, SHA1_K1, SHA1_M(17));
  SHA1_R(c, d, e, a, b, SHA1_F1, SHA1_K1, SHA1_M(18));
  SHA1_R(b, c, d, e, a, SHA1_F1, SHA1_K1, SHA1_M(19));
  SHA1_R(a, b, c, d, e, SHA1_F2, SHA1_K2, SHA1_M(20));
  SHA1_R(e, a, b, c, d, SHA1_F2, SHA1_K2, SHA1_M(21));
  SHA1_R(d, e, a, b, c, SHA1_F2, SHA1_K2, SHA1_M(22));
  SHA1_R(c, d, e, a, b, SHA1_F2, SHA1_K2, SHA1_M(23));
  SHA1_R(b, c, d, e, a, SHA1_F2, SHA1_K2, SHA1_M(24));
  SHA1_R(a, b, c, d, e, SHA1_F2, SHA1_K2, SHA1_M(25));
  SHA1_R(e, a, b, c, d, SHA1_F2, SHA1_K2, SHA1_M(26));
  SHA1_R(d, e, a, b, c, SHA1_F2, SHA1_K2, SHA1_M(27));
  SHA1_R(c, d, e, a, b, SHA1_F2, SHA1_K2, SHA1_M(28));
  SHA1_R(b, c, d, e, a, SHA1_F2, SHA1_K2, SHA1_M(29));
  SHA1_R(a, b, c, d, e, SHA1_F2, SHA1_K2, SHA1_M(30));
  SHA1_R(e, a, b, c, d, SHA1_F2, SHA1_K2, SHA1_M(31));
  SHA1_R(d, e, a, b, c, SHA1_F2, SHA1_K2, SHA1_M(32));
  SHA1_R(c, d, e, a, b, SHA1_F2, SHA1_K2, SHA1_M(33));
  SHA1_R(b, c, d, e, a, SHA1_F2, SHA1_K2, SHA1_M(34));
  SHA1_R(a, b, c, d, e, SHA1_F2, SHA1_K2, SHA1_M(35));
  SHA1_R(e, a, b, c, d, SHA1_F2, SHA1_K2, SHA1_M(36));
  SHA1_R(d, e, a, b, c, SHA1_F2, SHA1_K2, SHA1_M(37));
  SHA1_R(c, d, e, a, b, SHA1_F2, SHA1_K2, SHA1_M(38));
  SHA1_R(b, c, d, e, a, SHA1_F2, SHA1_K2, SHA1_M(39));
  SHA1_R(a, b, c, d, e, SHA1_F3, SHA1_K3, SHA1_M(40));
  SHA1_R(e, a, b, c, d, SHA1_F3, SHA1_K3, SHA1_M(41));
  SHA1_R(d, e, a, b, c, SHA1_F3, SHA1_K3, SHA1_M(42));
  SHA1_R(c, d, e, a, b, SHA1_F3, SHA1_K3, SHA1_M(43));
  SHA1_R(b, c, d, e, a, SHA1_F3, SHA1_K3, SHA1_M(44));
  SHA1_R(a, b, c, d, e, SHA1_F3, SHA1_K3, SHA1_M(45));
  SHA1_R(e, a, b, c, d, SHA1_F3, SHA1_K3, SHA1_M(46));
  SHA1_R(d, e, a, b, c, SHA1_F3, SHA1_K3, SHA1_M(47));
  SHA1_R(c, d, e, a, b, SHA1_F3, SHA1_K3, SHA1_M(48));
  SHA1_R(b, c, d, e, a, SHA1_F3, SHA1_K3, SHA1_M(49));
  SHA1_R(a, b, c, d, e, SHA1_F3, SHA1_K3, SHA1_M(50));
  SHA1_R(e, a, b, c, d, SHA1_F3, SHA1_K3, SHA1_M(51));
  SHA1_R(d, e, a, b, c, SHA1_F3, SHA1_K3, SHA1_M(52));
  SHA1_R(c, d, e, a, b, SHA1_F3, SHA1_K3, SHA1_M(53));
  SHA1_R(b, c, d, e, a, SHA1_F3, SHA1_K3, SHA1_M(54));
  SHA1_R(a, b, c, d, e, SHA1_F3, SHA1_K3, SHA1_M(55));
  SHA1_R(e, a, b, c, d, SHA1_F3, SHA1_K3, SHA1_M(56));
  SHA1_R(d, e, a, b, c, SHA1_F3, SHA1_K3, SHA1_M(57));
  SHA1_R(c, d, e, a, b, SHA1_F3, SHA1_K3, SHA1_M(58));
  SHA1_R(b, c, d, e, a, SHA1_F3, SHA1_K3, SHA1_M(59));
  SHA1_R(a, b, c, d, e, SHA1_F4, SHA1_K4, SHA1_M(60));
  SHA1_R(e, a, b, c, d, SHA1_F4, SHA1_K4, SHA1_M(61));
  SHA1_R(d, e, a, b, c, SHA1_F4, SHA1_K4, SHA1_M(62));
  SHA1_R(c, d, e, a, b, SHA1_F4, SHA1_K4, SHA1_M(63));
  SHA1_R(b, c, d, e, a, SHA1_F4, SHA1_K4, SHA1_M(64));
  SHA1_R(a, b, c, d, e, SHA1_F4, SHA1_K4, SHA1_M(65));
  SHA1_R(e, a, b, c, d, SHA1_F4, SHA1_K4, SHA1_M(66));
  SHA1_R(d, e, a, b, c, SHA1_F4, SHA1_K4, SHA1_M(67));
  SHA1_R(c, d, e, a, b, SHA1_F4, SHA1_K4, SHA1_M(68));
  SHA1_R(b, c, d, e, a, SHA1_F4, SHA1_K4, SHA1_M(69));
  SHA1_R(a, b, c, d, e, SHA1_F4, SHA1_K4, SHA1_M(70));
  SHA1_R(e, a, b, c, d, SHA1_F4, SHA1_K4, SHA1_M(71));
  SHA1_R(d, e, a, b, c, SHA1_F4, SHA1_K4, SHA1_M(72));
  SHA1_R(c, d, e, a, b, SHA1_F4, SHA1_K4, SHA1_M(73));
  SHA1_R(b, c, d, e, a, SHA1_F4, SHA1_K4, SHA1_M(74));
  SHA1_R(a, b, c, d, e, SHA1_F4, SHA1_K4, SHA1_M(75));
  SHA1_R(e, a, b, c, d, SHA1_F4, SHA1_K4, SHA1_M(76));
  SHA1_R(d, e, a, b, c, SHA1_F4, SHA1_K4, SHA1_M(77));
  SHA1_R(c, d, e, a, b, SHA1_F4, SHA1_K4, SHA1_M(78));
  SHA1_R(b, c, d, e, a, SHA1_F4, SHA1_K4, SHA1_M(79));

  pcontext->a += a;
  pcontext->b += b;
  pcontext->c += c;
  pcontext->d += d;
  pcontext->e += e;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* take data from context */
static netwib_err netwox_priv_sha1_update64_ctx(netwox_sha1_context *pcontext)
{

  if (pcontext->bufsize != NETWOX_SHA1_BLOCK_LEN) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  netwib_er(netwox_priv_sha1_update64(pcontext, pcontext->buf));
  pcontext->bufsize = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_sha1_update(netwox_sha1_context *pcontext,
                              netwib_constbuf *pbuf)
{
  netwib_data data;
  netwib_uint32 datasize;
  netwib_uint64 datasize64;

  datasize = netwib__buf_ref_data_sizenull(pbuf);
  if (datasize == 0) {
    return(NETWIB_ERR_OK);
  }
  data = netwib__buf_ref_data_ptr(pbuf);

  netwib__uint64_init_uint32(datasize, datasize64);
  netwib__uint64_add(datasize64, pcontext->totalbytes, pcontext->totalbytes);

  if (pcontext->bufsize) {
    while (datasize && (pcontext->bufsize < NETWOX_SHA1_BLOCK_LEN)) {
      pcontext->buf[pcontext->bufsize++] = *data++;
      datasize--;
    }
    if (pcontext->bufsize == NETWOX_SHA1_BLOCK_LEN) {
      netwib_er(netwox_priv_sha1_update64_ctx(pcontext));
    }
  }

  while (datasize >= NETWOX_SHA1_BLOCK_LEN) {
    netwib_er(netwox_priv_sha1_update64(pcontext, data));
    data += NETWOX_SHA1_BLOCK_LEN;
    datasize -= NETWOX_SHA1_BLOCK_LEN;
  }
  while (datasize) {
    pcontext->buf[pcontext->bufsize++] = *data++;
    datasize--;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_sha1_final(netwox_sha1_context *pcontext,
                             netwib_buf *psha1)
{
  netwib_uint64 totalbits;
  netwib_data data;

  /* create a 56 byte block */
  if (pcontext->bufsize < 56) {
    pcontext->buf[pcontext->bufsize++] = 0x80; /* first pad character */
    while (pcontext->bufsize < 56) {
      pcontext->buf[pcontext->bufsize++] = 0; /* next pad characters */
    }
  } else {
    pcontext->buf[pcontext->bufsize++] = 0x80; /* first pad character */
    while (pcontext->bufsize < 64) {
      pcontext->buf[pcontext->bufsize++] = 0; /* next pad characters */
    }
    netwib_er(netwox_priv_sha1_update64_ctx(pcontext));
    netwib_c_memset(pcontext->buf, 0, 56);
    pcontext->bufsize = 56;
  }

  /* append the 64 bit count
     Note: this overflows if totalbytes > 0x1FFFFFFFFFFFFFFF, but we ignore
     because netwox will never use such a big value.
  */
  netwib__uint64_shl(pcontext->totalbytes, 3, totalbits);
  pcontext->buf[56] = netwib_c2_uint64_0(totalbits);
  pcontext->buf[57] = netwib_c2_uint64_1(totalbits);
  pcontext->buf[58] = netwib_c2_uint64_2(totalbits);
  pcontext->buf[59] = netwib_c2_uint64_3(totalbits);
  pcontext->buf[60] = netwib_c2_uint64_4(totalbits);
  pcontext->buf[61] = netwib_c2_uint64_5(totalbits);
  pcontext->buf[62] = netwib_c2_uint64_6(totalbits);
  pcontext->buf[63] = netwib_c2_uint64_7(totalbits);
  pcontext->bufsize += 8;
  netwib_er(netwox_priv_sha1_update64_ctx(pcontext));

  /* copy in psha1 */
  netwib_er(netwib_buf_wantspace(psha1, NETWOX_SHA1_HASH_LEN, &data));
  netwib__data_append_uint32(data, pcontext->a);
  netwib__data_append_uint32(data, pcontext->b);
  netwib__data_append_uint32(data, pcontext->c);
  netwib__data_append_uint32(data, pcontext->d);
  netwib__data_append_uint32(data, pcontext->e);
  psha1->endoffset += NETWOX_SHA1_HASH_LEN;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_sha1_compute(netwib_constbuf *pbuf,
                               netwib_buf *psha1)
{
  netwox_sha1_context ctx;

  netwib_er(netwox_sha1_init(&ctx));
  netwib_er(netwox_sha1_update(&ctx, pbuf));
  netwib_er(netwox_sha1_final(&ctx, psha1));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_sha1_test(netwib_conststring data,
                                        netwib_conststring shawanted)
{
  netwib_buf buf, sha;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&sha));

  netwib_er(netwox_buf_append_strmixed(data, &buf));
  netwib_er(netwox_sha1_compute(&buf, &sha));
  netwib_er(netwox_buf_check_strmixed(&sha, shawanted));

  netwib_er(netwib_buf_close(&sha));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_sha1_test()
{
  /* test suite from FIPS-180-2 */
  netwib_er(netwox_priv_sha1_test("'abc'", "a9993e364706816aba3e25717850c26c9cd0d89d"));
  netwib_er(netwox_priv_sha1_test("'abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq'", "84983e441c3bd26ebaae4aa1f95129e5e54670f1"));

  /* test suite from SHAVS (SHA Validation System) from NIST */
  netwib_er(netwox_priv_sha1_test("5e", "5e6f80a34a9798cafc6a5db96cc57ba4c4db59c2"));
  netwib_er(netwox_priv_sha1_test("9a7dfdf1 ecead06e d646aa55 fe757146", "82abff6605dbe1c17def12a394fa22a82b544a35"));
  netwib_er(netwox_priv_sha1_test("f78f9214 1bcd170a e89b4fba 15a1d59f 3fd84d22  3c9251bd acbbae61 d05ed115 a06a7ce1 17b7beea d24421de d9c32592  bd57edea e39c39fa 1fe8946a 84d0cf1f 7beead17 13e2e095 9897347f 67c80b04 00c20981 5d6b10a6 83836fd5 562a56ca b1a28e81 b6576654 631cf165 66b86e3b 33a108b0 5307c00a ff14a768 ed735060 6a0f85e6 a91d396f 5b5cbe57 7f9b3880 7c7d523d 6d792f6e bc24a4ec f2b3a427 cdbbfb", "cb0082c8 f197d260 991ba6a4 60e76e20 2bad27b3"));
  netwib_er(netwox_priv_sha1_test("d0569cb3 665a8a43 eb6ea23d 75a3c4d2 054a0d7d  d0569cb3 665a8a43 eb6ea23d 75a3c4d2 054a0d7d d0569cb3 665a8a43 eb6ea23d 75a3c4d2 054a0d7d", "5c1f6ab1 dd1a1b92 313ef55b d94e5d90 eee5fd42"));
  netwib_er(netwox_priv_sha1_test("d0569cb3 665a8a43 eb6ea23d 75a3c4d2 054a0d7d d0569cb3 665a8a43 eb6ea23d 75a3c4d2 054a0d7d 5c1f6ab1 dd1a1b92 313ef55b d94e5d90 eee5fd42", "3dc3b220 871bb348 8a66c39d ea577c2d 1ce40168"));
  netwib_er(netwox_priv_sha1_test("d0569cb3 665a8a43 eb6ea23d 75a3c4d2 054a0d7d 5c1f6ab1 dd1a1b92 313ef55b d94e5d90 eee5fd42 3dc3b220 871bb348 8a66c39d ea577c2d 1ce40168", "dd4b2577 7737c82c f228ac14 cf04b905 b081ac72"));
  netwib_er(netwox_priv_sha1_test("5c1f6ab1 dd1a1b92 313ef55b d94e5d90 eee5fd42  3dc3b220 871bb348 8a66c39d ea577c2d 1ce40168 dd4b2577 7737c82c f228ac14 cf04b905 b081ac72", "129aef9d 88d256d5 a667637e a215d7d8 4d710156"));

  return(NETWIB_ERR_OK);
}
