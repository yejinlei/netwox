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
netwib_err netwox_sha224256_init(netwox_sha224256version sha224256version,
                                 netwox_sha224256_context *pcontext)
{
  pcontext->sha224256version = sha224256version;

  switch(pcontext->sha224256version) {
  case NETWOX_SHA224256VERSION_224:
    pcontext->a = 0xc1059ed8u;
    pcontext->b = 0x367cd507u;
    pcontext->c = 0x3070dd17u;
    pcontext->d = 0xf70e5939u;
    pcontext->e = 0xffc00b31u;
    pcontext->f = 0x68581511u;
    pcontext->g = 0x64f98fa7u;
    pcontext->h = 0xbefa4fa4u;
    break;
  case NETWOX_SHA224256VERSION_256:
    pcontext->a = 0x6a09e667u;
    pcontext->b = 0xbb67ae85u;
    pcontext->c = 0x3c6ef372u;
    pcontext->d = 0xa54ff53au;
    pcontext->e = 0x510e527fu;
    pcontext->f = 0x9b05688cu;
    pcontext->g = 0x1f83d9abu;
    pcontext->h = 0x5be0cd19u;
    break;
  default:
    return(NETWOX_ERR_INTERNALERROR);
  }

  netwib__uint64_init_uint32(0, pcontext->totalbytes);

  pcontext->bufsize = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* update a 64 byte block */
static netwib_err netwox_priv_sha224256_update64(netwox_sha224256_context *pcontext,
                                                 netwib_constdata data)
{
  static const netwib_uint32 K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
  };
  netwib_uint32 a, b, c, d, e, f, g, h;
  netwib_uint32 t1, t2, i;
  netwib_uint32 in[16];
  netwib_uint32 w[64];

  a = pcontext->a;
  b = pcontext->b;
  c = pcontext->c;
  d = pcontext->d;
  e = pcontext->e;
  f = pcontext->f;
  g = pcontext->g;
  h = pcontext->h;

  for (i = 0; i < 16; i++) {
    netwib__data_decode_uint32(data, in[i]);
  }

  for (i = 0; i < 16; i++) {
    w[i] = in[i];
  }

#define SHA2x_S0(x) (netwib_c2_uint32_ror ((x), 7) ^ netwib_c2_uint32_ror ((x), 18) ^ ((x) >> 3))
#define SHA2x_S1(x) (netwib_c2_uint32_ror ((x), 17) ^ netwib_c2_uint32_ror ((x), 19) ^ ((x) >> 10))
  for (; i < 64; i++) {
    w[i] = SHA2x_S1(w[i-2]) + w[i-7] + SHA2x_S0(w[i-15]) + w[i-16];
  }

#define SHA2x_Ch(x,y,z) (z ^ (x & (y ^ z)))
#define SHA2x_Maj(x,y,z) ((x & y) | (z & (x|y)))
#define SHA2x_Sum0(x) (netwib_c2_uint32_ror ((x), 2) ^ netwib_c2_uint32_ror ((x), 13) ^ netwib_c2_uint32_ror ((x), 22))
#define SHA2x_Sum1(x) (netwib_c2_uint32_ror ((x), 6) ^ netwib_c2_uint32_ror ((x), 11) ^ netwib_c2_uint32_ror ((x), 25))
  for (i = 0; i < 64; i++) {
    t1 = (h) + SHA2x_Sum1((e)) + SHA2x_Ch((e),(f),(g)) + K[i] + w[i];
    t2 = SHA2x_Sum0((a)) + SHA2x_Maj((a),(b),(c));
    h = g;
    g = f;
    f = e;
    e = d + t1;
    d = c;
    c = b;
    b = a;
    a = t1 + t2;
  }

  pcontext->a += a;
  pcontext->b += b;
  pcontext->c += c;
  pcontext->d += d;
  pcontext->e += e;
  pcontext->f += f;
  pcontext->g += g;
  pcontext->h += h;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* take data from context */
static netwib_err netwox_priv_sha224256_update64_ctx(netwox_sha224256_context *pcontext)
{
  if (pcontext->bufsize != NETWOX_SHA224256_BLOCK_MAXLEN) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  netwib_er(netwox_priv_sha224256_update64(pcontext, pcontext->buf));
  pcontext->bufsize = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_sha224256_update(netwox_sha224256_context *pcontext,
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
    while (datasize && (pcontext->bufsize < NETWOX_SHA224256_BLOCK_MAXLEN)) {
      pcontext->buf[pcontext->bufsize++] = *data++;
      datasize--;
    }
    if (pcontext->bufsize == NETWOX_SHA224256_BLOCK_MAXLEN) {
      netwib_er(netwox_priv_sha224256_update64_ctx(pcontext));
    }
  }

  while (datasize >= NETWOX_SHA224256_BLOCK_MAXLEN) {
    netwib_er(netwox_priv_sha224256_update64(pcontext, data));
    data += NETWOX_SHA224256_BLOCK_MAXLEN;
    datasize -= NETWOX_SHA224256_BLOCK_MAXLEN;
  }
  while (datasize) {
    pcontext->buf[pcontext->bufsize++] = *data++;
    datasize--;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_sha224256_final(netwox_sha224256_context *pcontext,
                                  netwib_buf *psha224256)
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
    netwib_er(netwox_priv_sha224256_update64_ctx(pcontext));
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
  netwib_er(netwox_priv_sha224256_update64_ctx(pcontext));

  /* copy in psha224256 */
  netwib_er(netwib_buf_wantspace(psha224256, NETWOX_SHA256_HASH_LEN, &data));
  netwib__data_append_uint32(data, pcontext->a);
  netwib__data_append_uint32(data, pcontext->b);
  netwib__data_append_uint32(data, pcontext->c);
  netwib__data_append_uint32(data, pcontext->d);
  netwib__data_append_uint32(data, pcontext->e);
  netwib__data_append_uint32(data, pcontext->f);
  netwib__data_append_uint32(data, pcontext->g);
  switch(pcontext->sha224256version) {
  case NETWOX_SHA224256VERSION_224:
    psha224256->endoffset += NETWOX_SHA224_HASH_LEN;
    break;
  case NETWOX_SHA224256VERSION_256:
    netwib__data_append_uint32(data, pcontext->h);
    psha224256->endoffset += NETWOX_SHA256_HASH_LEN;
    break;
  default:
    return(NETWOX_ERR_INTERNALERROR);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_sha224256_compute(netwox_sha224256version sha224256version,
                                    netwib_constbuf *pbuf,
                                    netwib_buf *psha224256)
{
  netwox_sha224256_context ctx;

  netwib_er(netwox_sha224256_init(sha224256version, &ctx));
  netwib_er(netwox_sha224256_update(&ctx, pbuf));
  netwib_er(netwox_sha224256_final(&ctx, psha224256));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_sha224256_test(netwox_sha224256version sha224256version,
                                             netwib_conststring data,
                                             netwib_conststring shawanted)
{
  netwib_buf buf, sha;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&sha));

  netwib_er(netwox_buf_append_strmixed(data, &buf));
  netwib_er(netwox_sha224256_compute(sha224256version, &buf, &sha));
  netwib_er(netwox_buf_check_strmixed(&sha, shawanted));

  netwib_er(netwib_buf_close(&sha));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}
#define netwox_priv_sha224_test(data,shawanted) netwox_priv_sha224256_test(NETWOX_SHA224256VERSION_224,data,shawanted)
#define netwox_priv_sha256_test(data,shawanted) netwox_priv_sha224256_test(NETWOX_SHA224256VERSION_256,data,shawanted)

/*-------------------------------------------------------------*/
netwib_err netwox_sha224256_test()
{
  /* test suite from FIPS-180-2 */
  netwib_er(netwox_priv_sha256_test("'abc'", "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"));
  netwib_er(netwox_priv_sha256_test("'abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq'", "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1"));

  /* test suite from FIPS-180-2 with change notice */
  netwib_er(netwox_priv_sha224_test("'abc'", "23097d223405d8228642a477bda255b32aadbce4bda0b3f7e36c9da7"));
  netwib_er(netwox_priv_sha224_test("'abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq'", "75388b16512776cc5dba5da1fd890150b0c6455cb4f58b1952522525"));

  /* test suite from SHAVS (SHA Validation System) from NIST */
  netwib_er(netwox_priv_sha224_test("07", "00ecd5f138422b8ad74c9799fd826c531bad2fcabc7450bee2aa8c2a"));
  netwib_er(netwox_priv_sha224_test("18804005 dd4fbd15 56299d6f 9d93df62", "df90d78a a78821c9 9b40ba4c 966921ac cd8ffb1e 98ac388e 56191db1"));
  netwib_er(netwox_priv_sha224_test("55b21007 9c61b53a dd520622 d1ac97d5 cdbe8cb3 3aa0ae34 4517bee4 d7ba09ab c8533c52 50887a43 bebbac90 6c2e1837 f26b36a5 9ae3be78 14d50689 6b718b2a 383ecdac 16b96125 553f416f f32c6674 c74599a9 005386d9 ce111224 5f48ee47 0d396c1e d63b9267 0ca56ec8 4deea814 b6135eca 54392bde db9489bc 9b875a8b af0dc1ae 78573691 4ab7daa2 64bc079d 269f2c0d 7eddd810 a426145a 0776f67c 878273", "0b31894e c8937ad9 b91bdfbc ba294d9a defaa18e 09305e9f 20d5c3a4"));
  netwib_er(netwox_priv_sha224_test("d0569cb3 665a8a43 eb6ea23d 75a3c4d2 054a0d7d  66a9ca99 c9ceb027 d0569cb3 665a8a43 eb6ea23d 75a3c4d2 054a0d7d 66a9ca99 c9ceb027 d0569cb3 665a8a43 eb6ea23d 75a3c4d2 054a0d7d 66a9ca99 c9ceb027", "38cea69d 7be80c14 ab3becb1 e02e7b67 ced60a05 1c4a0100 e3e613b4"));
  netwib_er(netwox_priv_sha224_test("d0569cb3 665a8a43 eb6ea23d 75a3c4d2 054a0d7d 66a9ca99 c9ceb027 d0569cb3 665a8a43 eb6ea23d 75a3c4d2 054a0d7d 66a9ca99 c9ceb027 38cea69d 7be80c14 ab3becb1 e02e7b67 ced60a05 1c4a0100 e3e613b4", " ee210ffc 8db91566 6c2a4fcb 37d9e5c5 8993c01b dccf706a 4077f955"));
  netwib_er(netwox_priv_sha224_test("d0569cb3 665a8a43 eb6ea23d 75a3c4d2 054a0d7d 66a9ca99 c9ceb027 38cea69d 7be80c14 ab3becb1 e02e7b67 ced60a05 1c4a0100 e3e613b4 ee210ffc 8db91566 6c2a4fcb 37d9e5c5 8993c01b dccf706a 4077f955 ", "915eb09c 83e58cb8 ed11d8f4 bb361798 42146256 08de7de8 dcab4030"));
  netwib_er(netwox_priv_sha224_test("38cea69d 7be80c14 ab3becb1 e02e7b67 ced60a05 1c4a0100 e3e613b4 ee210ffc 8db91566 6c2a4fcb 37d9e5c5 8993c01b dccf706a 4077f955 915eb09c 83e58cb8 ed11d8f4 bb361798 42146256 08de7de8 dcab4030", "25867f03 8cfcf093 4c575ba5 67bb23a4 40e0fab9 84afbe23 a5e26b3b"));
  netwib_er(netwox_priv_sha256_test("19", "68aa2e2e e5dff96e 3355e6c7 ee373e3d 6a4e17f7 5f9518d8 43709c0c 9bc3e3d4"));
  netwib_er(netwox_priv_sha256_test("e3d72570 dcdd787c e3887ab2 cd684652", "175ee69b 02ba9b58 e2b0a5fd 13819cea 573f3940 a94f8251 28cf4209 beabb4e8"));
  netwib_er(netwox_priv_sha256_test("8326754e 2277372f 4fc12b20 527afef0 4d8a0569 71b11ad5 7123a7c1 37760000 d7bef6f3 c1f7a908 3aa39d81 0db31077 7dab8b1e 7f02b84a 26c77332 5f8b2374 de7a4b5a 58cb5c5c f35bcee6 fb946e5b d694fa59 3a8beb3f 9d6592ec edaa66ca 82a29d0c 51bcf933 6230e5d7 84e4c0a4 3f8d79a3 0a165cba be452b77 4b9c7109 a97d138f 12922896 6f6c0adc 106aad5a 9fdd3082 5769b2c6 71af6759 df28eb39 3d54d6", "97dbca7d f46d62c8 a422c941 dd7e835b 8ad33617 63f7e9b2 d95f4f0d a6e1ccbc"));
  netwib_er(netwox_priv_sha256_test("f41ece26 13e45739 15696b5a dcd51ca3 28be3bf5 66a9ca99 c9ceb027 9c1cb0a7 f41ece26 13e45739 15696b5a dcd51ca3 28be3bf5 66a9ca99 c9ceb027 9c1cb0a7 f41ece26 13e45739 15696b5a dcd51ca3 28be3bf5 66a9ca99 c9ceb027 9c1cb0a7", "fddf1b37 dd34b3b2 01d43c57 bcde1158 38f0df70 1da93c3b f2c9c868 96e7e6c7"));
  netwib_er(netwox_priv_sha256_test("f41ece26 13e45739 15696b5a dcd51ca3 28be3bf5 66a9ca99 c9ceb027 9c1cb0a7 f41ece26 13e45739 15696b5a dcd51ca3 28be3bf5 66a9ca99 c9ceb027 9c1cb0a7 fddf1b37 dd34b3b2 01d43c57 bcde1158 38f0df70 1da93c3b f2c9c868 96e7e6c7 ", "3b9e2613 dc71d499 25cc3258 a3a4201a ea4336c2 a648ca8d ffb45bbd ad4835e8"));
  netwib_er(netwox_priv_sha256_test("f41ece26 13e45739 15696b5a dcd51ca3 28be3bf5 66a9ca99 c9ceb027 9c1cb0a7 fddf1b37 dd34b3b2 01d43c57 bcde1158 38f0df70 1da93c3b f2c9c868 96e7e6c7 3b9e2613 dc71d499 25cc3258 a3a4201a ea4336c2 a648ca8d ffb45bbd ad4835e8", " 9fbac41c 7453a2c8 8fd3fed1 f685ef27 587bebcc 573209bc c1b9f9ee cf03c1fd"));
  netwib_er(netwox_priv_sha256_test("fddf1b37 dd34b3b2 01d43c57 bcde1158 38f0df70 1da93c3b f2c9c868 96e7e6c7 3b9e2613 dc71d499 25cc3258 a3a4201a ea4336c2 a648ca8d ffb45bbd ad4835e8 9fbac41c 7453a2c8 8fd3fed1 f685ef27 587bebcc 573209bc c1b9f9ee cf03c1fd", "b125c98b 1a9d25f3 37b5a788 15b6b7a7 f091d328 80e8681b dec8584b 92aa3bf8"));

  return(NETWIB_ERR_OK);
}
