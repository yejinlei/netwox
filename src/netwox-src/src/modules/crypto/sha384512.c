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
netwib_err netwox_sha384512_init(netwox_sha384512version sha384512version,
                                 netwox_sha384512_context *pcontext)
{
  pcontext->sha384512version = sha384512version;

  switch(sha384512version) {
  case NETWOX_SHA384512VERSION_384:
    netwib__uint64_init_32(0xcbbb9d5d, 0xc1059ed8, pcontext->a);
    netwib__uint64_init_32(0x629a292a, 0x367cd507, pcontext->b);
    netwib__uint64_init_32(0x9159015a, 0x3070dd17, pcontext->c);
    netwib__uint64_init_32(0x152fecd8, 0xf70e5939, pcontext->d);
    netwib__uint64_init_32(0x67332667, 0xffc00b31, pcontext->e);
    netwib__uint64_init_32(0x8eb44a87, 0x68581511, pcontext->f);
    netwib__uint64_init_32(0xdb0c2e0d, 0x64f98fa7, pcontext->g);
    netwib__uint64_init_32(0x47b5481d, 0xbefa4fa4, pcontext->h);
    break;
  case NETWOX_SHA384512VERSION_512:
    netwib__uint64_init_32(0x6a09e667, 0xf3bcc908, pcontext->a);
    netwib__uint64_init_32(0xbb67ae85, 0x84caa73b, pcontext->b);
    netwib__uint64_init_32(0x3c6ef372, 0xfe94f82b, pcontext->c);
    netwib__uint64_init_32(0xa54ff53a, 0x5f1d36f1, pcontext->d);
    netwib__uint64_init_32(0x510e527f, 0xade682d1, pcontext->e);
    netwib__uint64_init_32(0x9b05688c, 0x2b3e6c1f, pcontext->f);
    netwib__uint64_init_32(0x1f83d9ab, 0xfb41bd6b, pcontext->g);
    netwib__uint64_init_32(0x5be0cd19, 0x137e2179, pcontext->h);
    break;
  default:
    return(NETWOX_ERR_INTERNALERROR);
  }

  netwib__uint64_init_uint32(0, pcontext->totalbytes);

  pcontext->bufsize = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* update a 128 bytes block */
static netwib_err netwox_priv_sha384512_update128(netwox_sha384512_context *pcontext,
                                                  netwib_constdata data)
{
  static const netwib_uint64 k[] =
    {
      NETWIB_UINT_STATIC(0x428a2f98, 0xd728ae22),
      NETWIB_UINT_STATIC(0x71374491, 0x23ef65cd),
      NETWIB_UINT_STATIC(0xb5c0fbcf, 0xec4d3b2f),
      NETWIB_UINT_STATIC(0xe9b5dba5, 0x8189dbbc),
      NETWIB_UINT_STATIC(0x3956c25b, 0xf348b538),
      NETWIB_UINT_STATIC(0x59f111f1, 0xb605d019),
      NETWIB_UINT_STATIC(0x923f82a4, 0xaf194f9b),
      NETWIB_UINT_STATIC(0xab1c5ed5, 0xda6d8118),
      NETWIB_UINT_STATIC(0xd807aa98, 0xa3030242),
      NETWIB_UINT_STATIC(0x12835b01, 0x45706fbe),
      NETWIB_UINT_STATIC(0x243185be, 0x4ee4b28c),
      NETWIB_UINT_STATIC(0x550c7dc3, 0xd5ffb4e2),
      NETWIB_UINT_STATIC(0x72be5d74, 0xf27b896f),
      NETWIB_UINT_STATIC(0x80deb1fe, 0x3b1696b1),
      NETWIB_UINT_STATIC(0x9bdc06a7, 0x25c71235),
      NETWIB_UINT_STATIC(0xc19bf174, 0xcf692694),
      NETWIB_UINT_STATIC(0xe49b69c1, 0x9ef14ad2),
      NETWIB_UINT_STATIC(0xefbe4786, 0x384f25e3),
      NETWIB_UINT_STATIC(0x0fc19dc6, 0x8b8cd5b5),
      NETWIB_UINT_STATIC(0x240ca1cc, 0x77ac9c65),
      NETWIB_UINT_STATIC(0x2de92c6f, 0x592b0275),
      NETWIB_UINT_STATIC(0x4a7484aa, 0x6ea6e483),
      NETWIB_UINT_STATIC(0x5cb0a9dc, 0xbd41fbd4),
      NETWIB_UINT_STATIC(0x76f988da, 0x831153b5),
      NETWIB_UINT_STATIC(0x983e5152, 0xee66dfab),
      NETWIB_UINT_STATIC(0xa831c66d, 0x2db43210),
      NETWIB_UINT_STATIC(0xb00327c8, 0x98fb213f),
      NETWIB_UINT_STATIC(0xbf597fc7, 0xbeef0ee4),
      NETWIB_UINT_STATIC(0xc6e00bf3, 0x3da88fc2),
      NETWIB_UINT_STATIC(0xd5a79147, 0x930aa725),
      NETWIB_UINT_STATIC(0x06ca6351, 0xe003826f),
      NETWIB_UINT_STATIC(0x14292967, 0x0a0e6e70),
      NETWIB_UINT_STATIC(0x27b70a85, 0x46d22ffc),
      NETWIB_UINT_STATIC(0x2e1b2138, 0x5c26c926),
      NETWIB_UINT_STATIC(0x4d2c6dfc, 0x5ac42aed),
      NETWIB_UINT_STATIC(0x53380d13, 0x9d95b3df),
      NETWIB_UINT_STATIC(0x650a7354, 0x8baf63de),
      NETWIB_UINT_STATIC(0x766a0abb, 0x3c77b2a8),
      NETWIB_UINT_STATIC(0x81c2c92e, 0x47edaee6),
      NETWIB_UINT_STATIC(0x92722c85, 0x1482353b),
      NETWIB_UINT_STATIC(0xa2bfe8a1, 0x4cf10364),
      NETWIB_UINT_STATIC(0xa81a664b, 0xbc423001),
      NETWIB_UINT_STATIC(0xc24b8b70, 0xd0f89791),
      NETWIB_UINT_STATIC(0xc76c51a3, 0x0654be30),
      NETWIB_UINT_STATIC(0xd192e819, 0xd6ef5218),
      NETWIB_UINT_STATIC(0xd6990624, 0x5565a910),
      NETWIB_UINT_STATIC(0xf40e3585, 0x5771202a),
      NETWIB_UINT_STATIC(0x106aa070, 0x32bbd1b8),
      NETWIB_UINT_STATIC(0x19a4c116, 0xb8d2d0c8),
      NETWIB_UINT_STATIC(0x1e376c08, 0x5141ab53),
      NETWIB_UINT_STATIC(0x2748774c, 0xdf8eeb99),
      NETWIB_UINT_STATIC(0x34b0bcb5, 0xe19b48a8),
      NETWIB_UINT_STATIC(0x391c0cb3, 0xc5c95a63),
      NETWIB_UINT_STATIC(0x4ed8aa4a, 0xe3418acb),
      NETWIB_UINT_STATIC(0x5b9cca4f, 0x7763e373),
      NETWIB_UINT_STATIC(0x682e6ff3, 0xd6b2b8a3),
      NETWIB_UINT_STATIC(0x748f82ee, 0x5defb2fc),
      NETWIB_UINT_STATIC(0x78a5636f, 0x43172f60),
      NETWIB_UINT_STATIC(0x84c87814, 0xa1f0ab72),
      NETWIB_UINT_STATIC(0x8cc70208, 0x1a6439ec),
      NETWIB_UINT_STATIC(0x90befffa, 0x23631e28),
      NETWIB_UINT_STATIC(0xa4506ceb, 0xde82bde9),
      NETWIB_UINT_STATIC(0xbef9a3f7, 0xb2c67915),
      NETWIB_UINT_STATIC(0xc67178f2, 0xe372532b),
      NETWIB_UINT_STATIC(0xca273ece, 0xea26619c),
      NETWIB_UINT_STATIC(0xd186b8c7, 0x21c0c207),
      NETWIB_UINT_STATIC(0xeada7dd6, 0xcde0eb1e),
      NETWIB_UINT_STATIC(0xf57d4f7f, 0xee6ed178),
      NETWIB_UINT_STATIC(0x06f067aa, 0x72176fba),
      NETWIB_UINT_STATIC(0x0a637dc5, 0xa2c898a6),
      NETWIB_UINT_STATIC(0x113f9804, 0xbef90dae),
      NETWIB_UINT_STATIC(0x1b710b35, 0x131c471b),
      NETWIB_UINT_STATIC(0x28db77f5, 0x23047d84),
      NETWIB_UINT_STATIC(0x32caab7b, 0x40c72493),
      NETWIB_UINT_STATIC(0x3c9ebe0a, 0x15c9bebc),
      NETWIB_UINT_STATIC(0x431d67c4, 0x9c100d4c),
      NETWIB_UINT_STATIC(0x4cc5d4be, 0xcb3e42b6),
      NETWIB_UINT_STATIC(0x597f299c, 0xfc657e2a),
      NETWIB_UINT_STATIC(0x5fcb6fab, 0x3ad6faec),
      NETWIB_UINT_STATIC(0x6c44198c, 0x4a475817)
    };
  netwib_uint64 a, b, c, d, e, f, g, h;
  netwib_uint64 w[80];
  netwib_uint64 F_tmp1, F_tmp2, F_tmp3;
  netwib_uint64 L_tmp1, L_tmp2, L_tmp3;
  netwib_uint64 t1, t2;
  netwib_uint32 i;

  a = pcontext->a;
  b = pcontext->b;
  c = pcontext->c;
  d = pcontext->d;
  e = pcontext->e;
  f = pcontext->f;
  g = pcontext->g;
  h = pcontext->h;

  for (i = 0; i < 16; i++) {
    netwib__data_decode_uint64(data, w[i]);
  }

#define SHA3x_S0(x,r) { netwib__uint64_ror((x), 1, L_tmp1); netwib__uint64_ror((x), 8, L_tmp2); netwib__uint64_shr((x), 7, L_tmp3); netwib__uint64_xor(L_tmp1, L_tmp2, r); netwib__uint64_xor(r, L_tmp3, r); }
#define SHA3x_S1(x,r) { netwib__uint64_ror((x), 19, L_tmp1); netwib__uint64_ror((x), 61, L_tmp2); netwib__uint64_shr((x), 6, L_tmp3); netwib__uint64_xor(L_tmp1, L_tmp2, r); netwib__uint64_xor(r, L_tmp3, r); }
  for (i = 16; i < 80; i++) {
    SHA3x_S1(w[i - 2], F_tmp1);
    SHA3x_S0(w[i - 15], F_tmp2);
    netwib__uint64_add(F_tmp1, F_tmp2, F_tmp3);
    netwib__uint64_add(F_tmp3, w[i - 7], F_tmp3);
    netwib__uint64_add(F_tmp3, w[i - 16], w[i]);
  }

#define SHA3x_Sum0(x,r) { netwib__uint64_ror((x), 28, L_tmp1); netwib__uint64_ror((x), 34, L_tmp2); netwib__uint64_ror((x), 39, L_tmp3); netwib__uint64_xor(L_tmp1, L_tmp2, r); netwib__uint64_xor(r, L_tmp3, r); }
#define SHA3x_Sum1(x,r) { netwib__uint64_ror((x), 14, L_tmp1); netwib__uint64_ror((x), 18, L_tmp2); netwib__uint64_ror((x), 41, L_tmp3); netwib__uint64_xor(L_tmp1, L_tmp2, r); netwib__uint64_xor(r, L_tmp3, r); }
#define SHA3x_Ch(x,y,z,r) { netwib__uint64_and((x), (y), L_tmp1); netwib__uint64_not((x), L_tmp2); netwib__uint64_and(L_tmp2, (z), L_tmp3); netwib__uint64_xor(L_tmp1, L_tmp3, r); }
#define SHA3x_Maj(x,y,z,r) { netwib__uint64_and((x), (y), L_tmp1); netwib__uint64_and((x), (z), L_tmp2); netwib__uint64_xor(L_tmp1, L_tmp2, L_tmp1); netwib__uint64_and((y), (z), L_tmp2); netwib__uint64_xor(L_tmp1, L_tmp2, r); }
  for (i = 0; i < 80; i++) {
    SHA3x_Sum1(e, F_tmp1);
    SHA3x_Ch(e, f, g, F_tmp2);
    netwib__uint64_add(F_tmp1, F_tmp2, F_tmp3);
    netwib__uint64_add(F_tmp3, h, F_tmp3);
    netwib__uint64_add(F_tmp3, k[i], F_tmp3);
    netwib__uint64_add(F_tmp3, w[i], t1);
    SHA3x_Sum0(a, F_tmp1);
    SHA3x_Maj(a, b, c, F_tmp2);
    netwib__uint64_add(F_tmp1, F_tmp2, t2);
    h = g;
    g = f;
    f = e;
    netwib__uint64_add(d, t1, e);
    d = c;
    c = b;
    b = a;
    netwib__uint64_add(t1, t2, a);
  }

  netwib__uint64_add(pcontext->a, a, pcontext->a);
  netwib__uint64_add(pcontext->b, b, pcontext->b);
  netwib__uint64_add(pcontext->c, c, pcontext->c);
  netwib__uint64_add(pcontext->d, d, pcontext->d);
  netwib__uint64_add(pcontext->e, e, pcontext->e);
  netwib__uint64_add(pcontext->f, f, pcontext->f);
  netwib__uint64_add(pcontext->g, g, pcontext->g);
  netwib__uint64_add(pcontext->h, h, pcontext->h);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* take data from context */
static netwib_err netwox_priv_sha384512_update128_ctx(netwox_sha384512_context *pcontext)
{
  if (pcontext->bufsize != NETWOX_SHA384512_BLOCK_MAXLEN) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  netwib_er(netwox_priv_sha384512_update128(pcontext, pcontext->buf));
  pcontext->bufsize = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_sha384512_update(netwox_sha384512_context *pcontext,
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
    while (datasize && (pcontext->bufsize < NETWOX_SHA384512_BLOCK_MAXLEN)) {
      pcontext->buf[pcontext->bufsize++] = *data++;
      datasize--;
    }
    if (pcontext->bufsize == NETWOX_SHA384512_BLOCK_MAXLEN) {
      netwib_er(netwox_priv_sha384512_update128_ctx(pcontext));
    }
  }

  while (datasize >= NETWOX_SHA384512_BLOCK_MAXLEN) {
    netwib_er(netwox_priv_sha384512_update128(pcontext, data));
    data += NETWOX_SHA384512_BLOCK_MAXLEN;
    datasize -= NETWOX_SHA384512_BLOCK_MAXLEN;
  }
  while (datasize) {
    pcontext->buf[pcontext->bufsize++] = *data++;
    datasize--;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_sha384512_final(netwox_sha384512_context *pcontext,
                                  netwib_buf *psha384512)
{
  netwib_uint64 totalbits;
  netwib_data data;

  /* create a 112 byte block */
  if (pcontext->bufsize < 112) {
    pcontext->buf[pcontext->bufsize++] = 0x80; /* first pad character */
    while (pcontext->bufsize < 112) {
      pcontext->buf[pcontext->bufsize++] = 0; /* next pad characters */
    }
  } else {
    pcontext->buf[pcontext->bufsize++] = 0x80; /* first pad character */
    while (pcontext->bufsize < 128) {
      pcontext->buf[pcontext->bufsize++] = 0; /* next pad characters */
    }
    netwib_er(netwox_priv_sha384512_update128_ctx(pcontext));
    netwib_c_memset(pcontext->buf, 0, 112);
    pcontext->bufsize = 112;
  }

  /* append the 128 bit count
     Note: this overflows if totalbytes > 0x1FFFFFFFFFFFFFFF, but we ignore
     because netwox will never use such a big value.
     We also ignore the 64 MSB bits (set to 0).
  */
  netwib__uint64_shl(pcontext->totalbytes, 3, totalbits);
  pcontext->buf[112] = 0;
  pcontext->buf[113] = 0;
  pcontext->buf[114] = 0;
  pcontext->buf[115] = 0;
  pcontext->buf[116] = 0;
  pcontext->buf[117] = 0;
  pcontext->buf[118] = 0;
  pcontext->buf[119] = 0;
  pcontext->buf[120] = netwib_c2_uint64_0(totalbits);
  pcontext->buf[121] = netwib_c2_uint64_1(totalbits);
  pcontext->buf[122] = netwib_c2_uint64_2(totalbits);
  pcontext->buf[123] = netwib_c2_uint64_3(totalbits);
  pcontext->buf[124] = netwib_c2_uint64_4(totalbits);
  pcontext->buf[125] = netwib_c2_uint64_5(totalbits);
  pcontext->buf[126] = netwib_c2_uint64_6(totalbits);
  pcontext->buf[127] = netwib_c2_uint64_7(totalbits);
  pcontext->bufsize += 16;
  netwib_er(netwox_priv_sha384512_update128_ctx(pcontext));

  netwib_er(netwib_buf_wantspace(psha384512, NETWOX_SHA512_HASH_LEN, &data));
  netwib__data_append_uint64(data, pcontext->a);
  netwib__data_append_uint64(data, pcontext->b);
  netwib__data_append_uint64(data, pcontext->c);
  netwib__data_append_uint64(data, pcontext->d);
  netwib__data_append_uint64(data, pcontext->e);
  netwib__data_append_uint64(data, pcontext->f);
  switch(pcontext->sha384512version) {
  case NETWOX_SHA384512VERSION_384:
    psha384512->endoffset += NETWOX_SHA384_HASH_LEN;
    break;
  case NETWOX_SHA384512VERSION_512:
    netwib__data_append_uint64(data, pcontext->g);
    netwib__data_append_uint64(data, pcontext->h);
    psha384512->endoffset += NETWOX_SHA512_HASH_LEN;
    break;
  default:
    return(NETWOX_ERR_INTERNALERROR);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_sha384512_compute(netwox_sha384512version sha384512version,
                                    netwib_constbuf *pbuf,
                                    netwib_buf *psha384512)
{
  netwox_sha384512_context ctx;

  netwib_er(netwox_sha384512_init(sha384512version, &ctx));
  netwib_er(netwox_sha384512_update(&ctx, pbuf));
  netwib_er(netwox_sha384512_final(&ctx, psha384512));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_sha384512_test(netwox_sha384512version sha384512version,
                                          netwib_conststring data,
                                          netwib_conststring shawanted)
{
  netwib_buf buf, sha;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&sha));

  netwib_er(netwox_buf_append_strmixed(data, &buf));
  netwib_er(netwox_sha384512_compute(sha384512version, &buf, &sha));
  netwib_er(netwox_buf_check_strmixed(&sha, shawanted));

  netwib_er(netwib_buf_close(&sha));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}
#define netwox_priv_sha384_test(data,sha384512wanted) netwox_priv_sha384512_test(NETWOX_SHA384512VERSION_384,data,sha384512wanted)
#define netwox_priv_sha512_test(data,sha384512wanted) netwox_priv_sha384512_test(NETWOX_SHA384512VERSION_512,data,sha384512wanted)

/*-------------------------------------------------------------*/
netwib_err netwox_sha384512_test()
{
  /* test suite from FIPS-180-2 */
  netwib_er(netwox_priv_sha384_test("'abc'", "cb00753f45a35e8bb5a03d699ac65007272c32ab0eded1631a8b605a43ff5bed8086072ba1e7cc2358baeca134c825a7"));
  netwib_er(netwox_priv_sha512_test("'abc'", "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f"));
  netwib_er(netwox_priv_sha384_test("'abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu'", "09330c33f71147e83d192fc782cd1b4753111b173b3b05d22fa08086e3b0f712fcc7c71a557e2db966c3e9fa91746039"));
  netwib_er(netwox_priv_sha512_test("'abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu'", "8e959b75dae313da8cf4f72814fc143f8f7779c6eb9f7fa17299aeadb6889018501d289e4900f7e4331b99dec4b5433ac7d329eeb6dd26545e96e55b874be909"));

  /* test suite from SHAVS (SHA Validation System) from NIST */
  netwib_er(netwox_priv_sha384_test("b9", "bc8089a1 9007c0b1 4195f4ec c74094fe c64f01f9 0929282c 2fb39288 1578208a d466828b 1c6c283d 2722cf0a d1ab6938"));
  netwib_er(netwox_priv_sha384_test("a41c4977 79c0375f f10a7f4e 08591739", "c9a68443 a0058122 56b8ec76 b00516f0 dbb74fab 26d66591 3f194b6f fb0e91ea 9967566b 58109cbc 675cc208 e4c823f7"));
  netwib_er(netwox_priv_sha384_test("399669e2 8f6b9c6d bcbb6912 ec10ffcf 74790349 b7dc8fbe 4a8e7b3b 5621db0f 3e7dc87f 823264bb e40d1811 c9ea2061 e1c84ad1 0a23fac1 727e7202 fc3f5042 e6bf58cb a8a2746e 1f64f9b9 ea352c71 1507053c f4e5339d 52865f25 cc22b5e8 7784a12f c961d66c b6e89573 199a2ce6 565cbdf1 3dca4038 32cfcb0e 8b7211e8 3af32a11 ac17929f f1c073a5 1cc027aa edeff85a ad7c2b7c 5a803e24 04d96d2a 77357bda 1a6daeed 17151cb9 bc5125a4 22e941de 0ca0fc50 11c23ecf fefdd096 76711cf3 db0a3440 720e1615 c1f22fbc 3c721de5 21e1b99b a1bd5577 40864214 7ed096", "4f440db1 e6edd289 9fa335f0 9515aa02 5ee177a7 9f4b4aaf 38e42b5c 4de660f5 de8fb2a5 b2fbd2a3 cbffd20c ff1288c0"));
  netwib_er(netwox_priv_sha384_test("8240bc51 e4ec7ef7 6d18e352 04a19f51 a5213a73 a81d6f94 4680d307 5948b7e4 63804ea3 d26e13ea 820d65a4 84be7453 8240bc51 e4ec7ef7 6d18e352 04a19f51 a5213a73 a81d6f94 4680d307 5948b7e4 63804ea3 d26e13ea 820d65a4 84be7453 8240bc51 e4ec7ef7 6d18e352 04a19f51 a5213a73 a81d6f94 4680d307 5948b7e4 63804ea3 d26e13ea 820d65a4 84be7453", "8ff53918 fb4da556 6f074f64 1f2ce018 a688fcbc b2efdb13 ba6b3578 bcd5a489 8d0b2d88 702dbbd8 310bea5a 975adf20"));
  netwib_er(netwox_priv_sha384_test("8240bc51 e4ec7ef7 6d18e352 04a19f51 a5213a73 a81d6f94 4680d307 5948b7e4 63804ea3 d26e13ea 820d65a4 84be7453 8240bc51 e4ec7ef7 6d18e352 04a19f51 a5213a73 a81d6f94 4680d307 5948b7e4 63804ea3 d26e13ea 820d65a4 84be7453 8ff53918 fb4da556 6f074f64 1f2ce018 a688fcbc b2efdb13 ba6b3578 bcd5a489 8d0b2d88 702dbbd8 310bea5a 975adf20", "b5d6eb75 ae9bd90a 27e46f0d 732b53bc b74b8ff2 f5df4d53 4ea78e8d beb31273 78bfafb1 35d59ec5 3d1f15f7 c839cfaf"));
  netwib_er(netwox_priv_sha384_test("8240bc51 e4ec7ef7 6d18e352 04a19f51 a5213a73 a81d6f94 4680d307 5948b7e4 63804ea3 d26e13ea 820d65a4 84be7453 8ff53918 fb4da556 6f074f64 1f2ce018 a688fcbc b2efdb13 ba6b3578 bcd5a489 8d0b2d88 702dbbd8 310bea5a 975adf20 b5d6eb75 ae9bd90a 27e46f0d 732b53bc b74b8ff2 f5df4d53 4ea78e8d beb31273 78bfafb1 35d59ec5 3d1f15f7 c839cfaf", "7870a434 da5d941c 4cdde45a de8e4fd1 178ea3d3 6ecd1231 fe675eb9 a5e61c01 34e495ba 2a4c3b11 1a861abc 679f9a37"));
  netwib_er(netwox_priv_sha384_test("8ff53918 fb4da556 6f074f64 1f2ce018 a688fcbc b2efdb13 ba6b3578 bcd5a489 8d0b2d88 702dbbd8 310bea5a 975adf20 b5d6eb75 ae9bd90a 27e46f0d 732b53bc b74b8ff2 f5df4d53 4ea78e8d beb31273 78bfafb1 35d59ec5 3d1f15f7 c839cfaf 7870a434 da5d941c 4cdde45a de8e4fd1 178ea3d3 6ecd1231 fe675eb9 a5e61c01 34e495ba 2a4c3b11 1a861abc 679f9a37", "1ae75e92 d9a24acc fe1bc1b6 abe0b643 76efbb19 cc49fa08 fbd3bc17 724fb774 5045e826 daa4f0de fca7adcf 165561a4"));
  netwib_er(netwox_priv_sha512_test("d0", "99922029 38e882e7 3e20f6b6 9e68a0a7 14909042 3d93c81b ab3f2167 8d4aceee e50e4e8c afada4c8 5a54ea83 06826c4a d6e74cec e9631bfa 8a549b4a b3fbba15"));
  netwib_er(netwox_priv_sha512_test("8d4e3c0e 38891914 91816e9d 98bff0a0", "cb0b67a4 b8712cd7 3c9aabc0 b199e926 9b20844a fb75acbd d1c153c9 828924c3 ddedaafe 669c5fdd 0bc66f63 0f677398 8213eb1b 16f517ad 0de4b2f0 c95c90f8"));
  netwib_er(netwox_priv_sha512_test("a55f20c4 11aad132 807a502d 65824e31 a2305432 aa3d06d3 e282a8d8 4e0de1de 6974bf49 5469fc7f 338f8054 d58c26c4 9360c3e8 7af56523 acf6d89d 03e56ff2 f868002b c3e431ed c44df2f0 223d4bb3 b243586e 1a7d9249 36694fcb baf88d95 19e4eb50 a644f8e4 f95eb0ea 95bc4465 c8821aac d2fe15ab 4981164b bb6dc32f 969087a1 45b0d9cc 9c67c22b 76329941 9cc4128b e9a077b3 ace63406 4e6d9928 3513dc06 e7515d0d 73132e9a 0dc6d3b1 f8b246f1 a98a3fc7 2941b1e3 bb2098e8 bf16f268 d64f0b0f 4707fe1e a1a1791b a2f3c0c7 58e5f551 863a96c9 49ad47d7 fb40d2", "c665befb 36da189d 78822d10 528cbf3b 12b3eef7 26039909 c1a16a27 0d487193 77966b95 7a878e72 0584779a 62825c18 da26415e 49a7176a 894e7510 fd1451f5"));
  netwib_er(netwox_priv_sha512_test("473ff1b9 b3ffdfa1 26699ac7 ef9e8e78 77730958 24c64255 7c1399d9 8e422044 8dc35b99 bfdd4477 9543924c 1ce93bc5 94153889 5db98826 1b00774b 12272039 473ff1b9 b3ffdfa1 26699ac7 ef9e8e78 77730958 24c64255 7c1399d9 8e422044 8dc35b99 bfdd4477 9543924c 1ce93bc5 94153889 5db98826 1b00774b 12272039 473ff1b9 b3ffdfa1 26699ac7 ef9e8e78 77730958 24c64255 7c1399d9 8e422044 8dc35b99 bfdd4477 9543924c 1ce93bc5 94153889 5db98826 1b00774b 12272039", "ab96e447 e4a23028 35b06c3e c33da499 72bfef0b 8678068b c5585945 2292eaaf cc0be757 2c2d14a5 2591fa92 357e5897 0f39ebfc d25877ae 1ba5159e b251d029"));
  netwib_er(netwox_priv_sha512_test("473ff1b9 b3ffdfa1 26699ac7 ef9e8e78 77730958 24c64255 7c1399d9 8e422044 8dc35b99 bfdd4477 9543924c 1ce93bc5 94153889 5db98826 1b00774b 12272039 473ff1b9 b3ffdfa1 26699ac7 ef9e8e78 77730958 24c64255 7c1399d9 8e422044 8dc35b99 bfdd4477 9543924c 1ce93bc5 94153889 5db98826 1b00774b 12272039 ab96e447 e4a23028 35b06c3e c33da499 72bfef0b 8678068b c5585945 2292eaaf cc0be757 2c2d14a5 2591fa92 357e5897 0f39ebfc d25877ae 1ba5159e b251d029", "94b957d0 fa320e59 38fa342a 753b16ae 74a6f3f1 e27e2260 40ab7ce1 1a3ea232 413539d5 dc08a6d0 74673f63 975628d1 30da35fd fce322a8 fe3f63de 44532669"));
  netwib_er(netwox_priv_sha512_test("473ff1b9 b3ffdfa1 26699ac7 ef9e8e78 77730958 24c64255 7c1399d9 8e422044 8dc35b99 bfdd4477 9543924c 1ce93bc5 94153889 5db98826 1b00774b 12272039 ab96e447 e4a23028 35b06c3e c33da499 72bfef0b 8678068b c5585945 2292eaaf cc0be757 2c2d14a5 2591fa92 357e5897 0f39ebfc d25877ae 1ba5159e b251d029 94b957d0 fa320e59 38fa342a 753b16ae 74a6f3f1 e27e2260 40ab7ce1 1a3ea232 413539d5 dc08a6d0 74673f63 975628d1 30da35fd fce322a8 fe3f63de 44532669", "8d6751b5 554672e2 3219132d ad6bb041 b650bea9 35012c41 f488e847 20a1310e ad9133a1 6109627c fe9895f2 4d5fee88 4aa754a3 4c6549e8 d17bd2c6 83261b43"));
  netwib_er(netwox_priv_sha512_test("ab96e447 e4a23028 35b06c3e c33da499 72bfef0b 8678068b c5585945 2292eaaf cc0be757 2c2d14a5 2591fa92 357e5897 0f39ebfc d25877ae 1ba5159e b251d029 94b957d0 fa320e59 38fa342a 753b16ae 74a6f3f1 e27e2260 40ab7ce1 1a3ea232 413539d5 dc08a6d0 74673f63 975628d1 30da35fd fce322a8 fe3f63de 44532669 8d6751b5 554672e2 3219132d ad6bb041 b650bea9 35012c41 f488e847 20a1310e ad9133a1 6109627c fe9895f2 4d5fee88 4aa754a3 4c6549e8 d17bd2c6 83261b43", "69a635e9 14950e8e e15c27db d833cb87 a4166951 5049938a 3acf7ef3 a8c6c5ec e13300b1 0bb02b8d fcc7d0bf 1b504d3a 45cc5623 f07d938d 1aa9902e beae48ec"));

  return(NETWIB_ERR_OK);
}
