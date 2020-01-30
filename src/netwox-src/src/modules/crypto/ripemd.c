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
netwib_err netwox_ripemd_init(netwox_ripemdversion ripemdversion,
                              netwox_ripemd_context *pcontext)
{
  pcontext->ripemdversion = ripemdversion;

  pcontext->a = 0x67452301u;
  pcontext->b = 0xefcdab89u;
  pcontext->c = 0x98badcfeu;
  pcontext->d = 0x10325476u;
  pcontext->e = 0xc3d2e1f0u; /* not used for 128 bits version */

  netwib__uint64_init_uint32(0, pcontext->totalbytes);

  pcontext->bufsize = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* update a 64 byte block */
static netwib_err netwox_priv_ripemd_update64(netwox_ripemd_context *pcontext,
                                              netwib_constdata data)
{
  register netwib_uint32 a, b, c, d, e, a2, b2, c2, d2, e2;
  netwib_uint32 in[16], i, tmp;

  a = pcontext->a;
  b = pcontext->b;
  c = pcontext->c;
  d = pcontext->d;
  e = pcontext->e; /* not used for 128 bits version */
  a2 = pcontext->a;
  b2 = pcontext->b;
  c2 = pcontext->c;
  d2 = pcontext->d;
  e2 = pcontext->e; /* not used for 128 bits version */

  for (i = 0; i < 16; i++) {
    netwib__data_decode_uint32_le(data, in[i]);
  }

  if (pcontext->ripemdversion == NETWOX_RIPEMDVERSION_128) {
#define RIPEMD128_F(x, y, z) ((x) ^ (y) ^ (z))
#define RIPEMD128_G(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define RIPEMD128_H(x, y, z) (((x) | ~(y)) ^ (z))
#define RIPEMD128_I(x, y, z) (((x) & (z)) | ((y) & ~(z)))
#define RIPEMD128_FF(a, b, c, d, x, s) { (a) += RIPEMD128_F((b), (c), (d)) + (x); (a) = netwib_c2_uint32_rol((a), (s)); }
#define RIPEMD128_GG(a, b, c, d, x, s) { (a) += RIPEMD128_G((b), (c), (d)) + (x) + 0x5a827999u; (a) = netwib_c2_uint32_rol((a), (s)); }
#define RIPEMD128_HH(a, b, c, d, x, s) { (a) += RIPEMD128_H((b), (c), (d)) + (x) + 0x6ed9eba1u; (a) = netwib_c2_uint32_rol((a), (s)); }
#define RIPEMD128_II(a, b, c, d, x, s) { (a) += RIPEMD128_I((b), (c), (d)) + (x) + 0x8f1bbcdcu; (a) = netwib_c2_uint32_rol((a), (s)); }
#define RIPEMD128_FFF(a, b, c, d, x, s) { (a) += RIPEMD128_F((b), (c), (d)) + (x); (a) = netwib_c2_uint32_rol((a), (s)); }
#define RIPEMD128_GGG(a, b, c, d, x, s) { (a) += RIPEMD128_G((b), (c), (d)) + (x) + 0x6d703ef3u; (a) = netwib_c2_uint32_rol((a), (s)); }
#define RIPEMD128_HHH(a, b, c, d, x, s) { (a) += RIPEMD128_H((b), (c), (d)) + (x) + 0x5c4dd124u; (a) = netwib_c2_uint32_rol((a), (s)); }
#define RIPEMD128_III(a, b, c, d, x, s) { (a) += RIPEMD128_I((b), (c), (d)) + (x) + 0x50a28be6u; (a) = netwib_c2_uint32_rol((a), (s)); }
    /* normal rounds */
    RIPEMD128_FF(a, b, c, d, in[0], 11);
    RIPEMD128_FF(d, a, b, c, in[1], 14);
    RIPEMD128_FF(c, d, a, b, in[2], 15);
    RIPEMD128_FF(b, c, d, a, in[3], 12);
    RIPEMD128_FF(a, b, c, d, in[4], 5);
    RIPEMD128_FF(d, a, b, c, in[5], 8);
    RIPEMD128_FF(c, d, a, b, in[6], 7);
    RIPEMD128_FF(b, c, d, a, in[7], 9);
    RIPEMD128_FF(a, b, c, d, in[8], 11);
    RIPEMD128_FF(d, a, b, c, in[9], 13);
    RIPEMD128_FF(c, d, a, b, in[10], 14);
    RIPEMD128_FF(b, c, d, a, in[11], 15);
    RIPEMD128_FF(a, b, c, d, in[12], 6);
    RIPEMD128_FF(d, a, b, c, in[13], 7);
    RIPEMD128_FF(c, d, a, b, in[14], 9);
    RIPEMD128_FF(b, c, d, a, in[15], 8);
    RIPEMD128_GG(a, b, c, d, in[7], 7);
    RIPEMD128_GG(d, a, b, c, in[4], 6);
    RIPEMD128_GG(c, d, a, b, in[13], 8);
    RIPEMD128_GG(b, c, d, a, in[1], 13);
    RIPEMD128_GG(a, b, c, d, in[10], 11);
    RIPEMD128_GG(d, a, b, c, in[6], 9);
    RIPEMD128_GG(c, d, a, b, in[15], 7);
    RIPEMD128_GG(b, c, d, a, in[3], 15);
    RIPEMD128_GG(a, b, c, d, in[12], 7);
    RIPEMD128_GG(d, a, b, c, in[0], 12);
    RIPEMD128_GG(c, d, a, b, in[9], 15);
    RIPEMD128_GG(b, c, d, a, in[5], 9);
    RIPEMD128_GG(a, b, c, d, in[2], 11);
    RIPEMD128_GG(d, a, b, c, in[14], 7);
    RIPEMD128_GG(c, d, a, b, in[11], 13);
    RIPEMD128_GG(b, c, d, a, in[8], 12);
    RIPEMD128_HH(a, b, c, d, in[3], 11);
    RIPEMD128_HH(d, a, b, c, in[10], 13);
    RIPEMD128_HH(c, d, a, b, in[14], 6);
    RIPEMD128_HH(b, c, d, a, in[4], 7);
    RIPEMD128_HH(a, b, c, d, in[9], 14);
    RIPEMD128_HH(d, a, b, c, in[15], 9);
    RIPEMD128_HH(c, d, a, b, in[8], 13);
    RIPEMD128_HH(b, c, d, a, in[1], 15);
    RIPEMD128_HH(a, b, c, d, in[2], 14);
    RIPEMD128_HH(d, a, b, c, in[7], 8);
    RIPEMD128_HH(c, d, a, b, in[0], 13);
    RIPEMD128_HH(b, c, d, a, in[6], 6);
    RIPEMD128_HH(a, b, c, d, in[13], 5);
    RIPEMD128_HH(d, a, b, c, in[11], 12);
    RIPEMD128_HH(c, d, a, b, in[5], 7);
    RIPEMD128_HH(b, c, d, a, in[12], 5);
    RIPEMD128_II(a, b, c, d, in[1], 11);
    RIPEMD128_II(d, a, b, c, in[9], 12);
    RIPEMD128_II(c, d, a, b, in[11], 14);
    RIPEMD128_II(b, c, d, a, in[10], 15);
    RIPEMD128_II(a, b, c, d, in[0], 14);
    RIPEMD128_II(d, a, b, c, in[8], 15);
    RIPEMD128_II(c, d, a, b, in[12], 9);
    RIPEMD128_II(b, c, d, a, in[4], 8);
    RIPEMD128_II(a, b, c, d, in[13], 9);
    RIPEMD128_II(d, a, b, c, in[3], 14);
    RIPEMD128_II(c, d, a, b, in[7], 5);
    RIPEMD128_II(b, c, d, a, in[15], 6);
    RIPEMD128_II(a, b, c, d, in[14], 8);
    RIPEMD128_II(d, a, b, c, in[5], 6);
    RIPEMD128_II(c, d, a, b, in[6], 5);
    RIPEMD128_II(b, c, d, a, in[2], 12);
    /* additional rounds */
    RIPEMD128_III(a2, b2, c2, d2, in[5], 8);
    RIPEMD128_III(d2, a2, b2, c2, in[14], 9);
    RIPEMD128_III(c2, d2, a2, b2, in[7], 9);
    RIPEMD128_III(b2, c2, d2, a2, in[0], 11);
    RIPEMD128_III(a2, b2, c2, d2, in[9], 13);
    RIPEMD128_III(d2, a2, b2, c2, in[2], 15);
    RIPEMD128_III(c2, d2, a2, b2, in[11], 15);
    RIPEMD128_III(b2, c2, d2, a2, in[4], 5);
    RIPEMD128_III(a2, b2, c2, d2, in[13], 7);
    RIPEMD128_III(d2, a2, b2, c2, in[6], 7);
    RIPEMD128_III(c2, d2, a2, b2, in[15], 8);
    RIPEMD128_III(b2, c2, d2, a2, in[8], 11);
    RIPEMD128_III(a2, b2, c2, d2, in[1], 14);
    RIPEMD128_III(d2, a2, b2, c2, in[10], 14);
    RIPEMD128_III(c2, d2, a2, b2, in[3], 12);
    RIPEMD128_III(b2, c2, d2, a2, in[12], 6);
    RIPEMD128_HHH(a2, b2, c2, d2, in[6], 9);
    RIPEMD128_HHH(d2, a2, b2, c2, in[11], 13);
    RIPEMD128_HHH(c2, d2, a2, b2, in[3], 15);
    RIPEMD128_HHH(b2, c2, d2, a2, in[7], 7);
    RIPEMD128_HHH(a2, b2, c2, d2, in[0], 12);
    RIPEMD128_HHH(d2, a2, b2, c2, in[13], 8);
    RIPEMD128_HHH(c2, d2, a2, b2, in[5], 9);
    RIPEMD128_HHH(b2, c2, d2, a2, in[10], 11);
    RIPEMD128_HHH(a2, b2, c2, d2, in[14], 7);
    RIPEMD128_HHH(d2, a2, b2, c2, in[15], 7);
    RIPEMD128_HHH(c2, d2, a2, b2, in[8], 12);
    RIPEMD128_HHH(b2, c2, d2, a2, in[12], 7);
    RIPEMD128_HHH(a2, b2, c2, d2, in[4], 6);
    RIPEMD128_HHH(d2, a2, b2, c2, in[9], 15);
    RIPEMD128_HHH(c2, d2, a2, b2, in[1], 13);
    RIPEMD128_HHH(b2, c2, d2, a2, in[2], 11);
    RIPEMD128_GGG(a2, b2, c2, d2, in[15], 9);
    RIPEMD128_GGG(d2, a2, b2, c2, in[5], 7);
    RIPEMD128_GGG(c2, d2, a2, b2, in[1], 15);
    RIPEMD128_GGG(b2, c2, d2, a2, in[3], 11);
    RIPEMD128_GGG(a2, b2, c2, d2, in[7], 8);
    RIPEMD128_GGG(d2, a2, b2, c2, in[14], 6);
    RIPEMD128_GGG(c2, d2, a2, b2, in[6], 6);
    RIPEMD128_GGG(b2, c2, d2, a2, in[9], 14);
    RIPEMD128_GGG(a2, b2, c2, d2, in[11], 12);
    RIPEMD128_GGG(d2, a2, b2, c2, in[8], 13);
    RIPEMD128_GGG(c2, d2, a2, b2, in[12], 5);
    RIPEMD128_GGG(b2, c2, d2, a2, in[2], 14);
    RIPEMD128_GGG(a2, b2, c2, d2, in[10], 13);
    RIPEMD128_GGG(d2, a2, b2, c2, in[0], 13);
    RIPEMD128_GGG(c2, d2, a2, b2, in[4], 7);
    RIPEMD128_GGG(b2, c2, d2, a2, in[13], 5);
    RIPEMD128_FFF(a2, b2, c2, d2, in[8], 15);
    RIPEMD128_FFF(d2, a2, b2, c2, in[6], 5);
    RIPEMD128_FFF(c2, d2, a2, b2, in[4], 8);
    RIPEMD128_FFF(b2, c2, d2, a2, in[1], 11);
    RIPEMD128_FFF(a2, b2, c2, d2, in[3], 14);
    RIPEMD128_FFF(d2, a2, b2, c2, in[11], 14);
    RIPEMD128_FFF(c2, d2, a2, b2, in[15], 6);
    RIPEMD128_FFF(b2, c2, d2, a2, in[0], 14);
    RIPEMD128_FFF(a2, b2, c2, d2, in[5], 6);
    RIPEMD128_FFF(d2, a2, b2, c2, in[12], 9);
    RIPEMD128_FFF(c2, d2, a2, b2, in[2], 12);
    RIPEMD128_FFF(b2, c2, d2, a2, in[13], 9);
    RIPEMD128_FFF(a2, b2, c2, d2, in[9], 12);
    RIPEMD128_FFF(d2, a2, b2, c2, in[7], 5);
    RIPEMD128_FFF(c2, d2, a2, b2, in[10], 15);
    RIPEMD128_FFF(b2, c2, d2, a2, in[14], 8);
  } else if (pcontext->ripemdversion == NETWOX_RIPEMDVERSION_160) {
#define RIPEMD160_F(x, y, z) ((x) ^ (y) ^ (z))
#define RIPEMD160_G(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define RIPEMD160_H(x, y, z) (((x) | ~(y)) ^ (z))
#define RIPEMD160_I(x, y, z) (((x) & (z)) | ((y) & ~(z)))
#define RIPEMD160_J(x, y, z) ((x) ^ ((y) | ~(z)))
#define RIPEMD160_FF(a, b, c, d, e, x, s) { (a) += RIPEMD160_F((b), (c), (d)) + (x); (a) = netwib_c2_uint32_rol((a), (s)) + (e); (c) = netwib_c2_uint32_rol((c), 10); }
#define RIPEMD160_GG(a, b, c, d, e, x, s) { (a) += RIPEMD160_G((b), (c), (d)) + (x) + 0x5a827999u; (a) = netwib_c2_uint32_rol((a), (s)) + (e); (c) = netwib_c2_uint32_rol((c), 10); }
#define RIPEMD160_HH(a, b, c, d, e, x, s) { (a) += RIPEMD160_H((b), (c), (d)) + (x) + 0x6ed9eba1u; (a) = netwib_c2_uint32_rol((a), (s)) + (e); (c) = netwib_c2_uint32_rol((c), 10); }
#define RIPEMD160_II(a, b, c, d, e, x, s) { (a) += RIPEMD160_I((b), (c), (d)) + (x) + 0x8f1bbcdcu; (a) = netwib_c2_uint32_rol((a), (s)) + (e); (c) = netwib_c2_uint32_rol((c), 10); }
#define RIPEMD160_JJ(a, b, c, d, e, x, s) { (a) += RIPEMD160_J((b), (c), (d)) + (x) + 0xa953fd4eu; (a) = netwib_c2_uint32_rol((a), (s)) + (e); (c) = netwib_c2_uint32_rol((c), 10); }
#define RIPEMD160_FFF(a, b, c, d, e, x, s) { (a) += RIPEMD160_F((b), (c), (d)) + (x); (a) = netwib_c2_uint32_rol((a), (s)) + (e); (c) = netwib_c2_uint32_rol((c), 10); }
#define RIPEMD160_GGG(a, b, c, d, e, x, s) { (a) += RIPEMD160_G((b), (c), (d)) + (x) + 0x7a6d76e9u; (a) = netwib_c2_uint32_rol((a), (s)) + (e); (c) = netwib_c2_uint32_rol((c), 10); }
#define RIPEMD160_HHH(a, b, c, d, e, x, s) { (a) += RIPEMD160_H((b), (c), (d)) + (x) + 0x6d703ef3u; (a) = netwib_c2_uint32_rol((a), (s)) + (e); (c) = netwib_c2_uint32_rol((c), 10); }
#define RIPEMD160_III(a, b, c, d, e, x, s) { (a) += RIPEMD160_I((b), (c), (d)) + (x) + 0x5c4dd124u; (a) = netwib_c2_uint32_rol((a), (s)) + (e); (c) = netwib_c2_uint32_rol((c), 10); }
#define RIPEMD160_JJJ(a, b, c, d, e, x, s) { (a) += RIPEMD160_J((b), (c), (d)) + (x) + 0x50a28be6u; (a) = netwib_c2_uint32_rol((a), (s)) + (e); (c) = netwib_c2_uint32_rol((c), 10); }
    /* normal rounds */
    RIPEMD160_FF(a, b, c, d, e, in[0], 11);
    RIPEMD160_FF(e, a, b, c, d, in[1], 14);
    RIPEMD160_FF(d, e, a, b, c, in[2], 15);
    RIPEMD160_FF(c, d, e, a, b, in[3], 12);
    RIPEMD160_FF(b, c, d, e, a, in[4], 5);
    RIPEMD160_FF(a, b, c, d, e, in[5], 8);
    RIPEMD160_FF(e, a, b, c, d, in[6], 7);
    RIPEMD160_FF(d, e, a, b, c, in[7], 9);
    RIPEMD160_FF(c, d, e, a, b, in[8], 11);
    RIPEMD160_FF(b, c, d, e, a, in[9], 13);
    RIPEMD160_FF(a, b, c, d, e, in[10], 14);
    RIPEMD160_FF(e, a, b, c, d, in[11], 15);
    RIPEMD160_FF(d, e, a, b, c, in[12], 6);
    RIPEMD160_FF(c, d, e, a, b, in[13], 7);
    RIPEMD160_FF(b, c, d, e, a, in[14], 9);
    RIPEMD160_FF(a, b, c, d, e, in[15], 8);
    RIPEMD160_GG(e, a, b, c, d, in[7], 7);
    RIPEMD160_GG(d, e, a, b, c, in[4], 6);
    RIPEMD160_GG(c, d, e, a, b, in[13], 8);
    RIPEMD160_GG(b, c, d, e, a, in[1], 13);
    RIPEMD160_GG(a, b, c, d, e, in[10], 11);
    RIPEMD160_GG(e, a, b, c, d, in[6], 9);
    RIPEMD160_GG(d, e, a, b, c, in[15], 7);
    RIPEMD160_GG(c, d, e, a, b, in[3], 15);
    RIPEMD160_GG(b, c, d, e, a, in[12], 7);
    RIPEMD160_GG(a, b, c, d, e, in[0], 12);
    RIPEMD160_GG(e, a, b, c, d, in[9], 15);
    RIPEMD160_GG(d, e, a, b, c, in[5], 9);
    RIPEMD160_GG(c, d, e, a, b, in[2], 11);
    RIPEMD160_GG(b, c, d, e, a, in[14], 7);
    RIPEMD160_GG(a, b, c, d, e, in[11], 13);
    RIPEMD160_GG(e, a, b, c, d, in[8], 12);
    RIPEMD160_HH(d, e, a, b, c, in[3], 11);
    RIPEMD160_HH(c, d, e, a, b, in[10], 13);
    RIPEMD160_HH(b, c, d, e, a, in[14], 6);
    RIPEMD160_HH(a, b, c, d, e, in[4], 7);
    RIPEMD160_HH(e, a, b, c, d, in[9], 14);
    RIPEMD160_HH(d, e, a, b, c, in[15], 9);
    RIPEMD160_HH(c, d, e, a, b, in[8], 13);
    RIPEMD160_HH(b, c, d, e, a, in[1], 15);
    RIPEMD160_HH(a, b, c, d, e, in[2], 14);
    RIPEMD160_HH(e, a, b, c, d, in[7], 8);
    RIPEMD160_HH(d, e, a, b, c, in[0], 13);
    RIPEMD160_HH(c, d, e, a, b, in[6], 6);
    RIPEMD160_HH(b, c, d, e, a, in[13], 5);
    RIPEMD160_HH(a, b, c, d, e, in[11], 12);
    RIPEMD160_HH(e, a, b, c, d, in[5], 7);
    RIPEMD160_HH(d, e, a, b, c, in[12], 5);
    RIPEMD160_II(c, d, e, a, b, in[1], 11);
    RIPEMD160_II(b, c, d, e, a, in[9], 12);
    RIPEMD160_II(a, b, c, d, e, in[11], 14);
    RIPEMD160_II(e, a, b, c, d, in[10], 15);
    RIPEMD160_II(d, e, a, b, c, in[0], 14);
    RIPEMD160_II(c, d, e, a, b, in[8], 15);
    RIPEMD160_II(b, c, d, e, a, in[12], 9);
    RIPEMD160_II(a, b, c, d, e, in[4], 8);
    RIPEMD160_II(e, a, b, c, d, in[13], 9);
    RIPEMD160_II(d, e, a, b, c, in[3], 14);
    RIPEMD160_II(c, d, e, a, b, in[7], 5);
    RIPEMD160_II(b, c, d, e, a, in[15], 6);
    RIPEMD160_II(a, b, c, d, e, in[14], 8);
    RIPEMD160_II(e, a, b, c, d, in[5], 6);
    RIPEMD160_II(d, e, a, b, c, in[6], 5);
    RIPEMD160_II(c, d, e, a, b, in[2], 12);
    RIPEMD160_JJ(b, c, d, e, a, in[4], 9);
    RIPEMD160_JJ(a, b, c, d, e, in[0], 15);
    RIPEMD160_JJ(e, a, b, c, d, in[5], 5);
    RIPEMD160_JJ(d, e, a, b, c, in[9], 11);
    RIPEMD160_JJ(c, d, e, a, b, in[7], 6);
    RIPEMD160_JJ(b, c, d, e, a, in[12], 8);
    RIPEMD160_JJ(a, b, c, d, e, in[2], 13);
    RIPEMD160_JJ(e, a, b, c, d, in[10], 12);
    RIPEMD160_JJ(d, e, a, b, c, in[14], 5);
    RIPEMD160_JJ(c, d, e, a, b, in[1], 12);
    RIPEMD160_JJ(b, c, d, e, a, in[3], 13);
    RIPEMD160_JJ(a, b, c, d, e, in[8], 14);
    RIPEMD160_JJ(e, a, b, c, d, in[11], 11);
    RIPEMD160_JJ(d, e, a, b, c, in[6], 8);
    RIPEMD160_JJ(c, d, e, a, b, in[15], 5);
    RIPEMD160_JJ(b, c, d, e, a, in[13], 6);
    /* additional rounds */
    RIPEMD160_JJJ(a2, b2, c2, d2, e2, in[5], 8);
    RIPEMD160_JJJ(e2, a2, b2, c2, d2, in[14], 9);
    RIPEMD160_JJJ(d2, e2, a2, b2, c2, in[7], 9);
    RIPEMD160_JJJ(c2, d2, e2, a2, b2, in[0], 11);
    RIPEMD160_JJJ(b2, c2, d2, e2, a2, in[9], 13);
    RIPEMD160_JJJ(a2, b2, c2, d2, e2, in[2], 15);
    RIPEMD160_JJJ(e2, a2, b2, c2, d2, in[11], 15);
    RIPEMD160_JJJ(d2, e2, a2, b2, c2, in[4], 5);
    RIPEMD160_JJJ(c2, d2, e2, a2, b2, in[13], 7);
    RIPEMD160_JJJ(b2, c2, d2, e2, a2, in[6], 7);
    RIPEMD160_JJJ(a2, b2, c2, d2, e2, in[15], 8);
    RIPEMD160_JJJ(e2, a2, b2, c2, d2, in[8], 11);
    RIPEMD160_JJJ(d2, e2, a2, b2, c2, in[1], 14);
    RIPEMD160_JJJ(c2, d2, e2, a2, b2, in[10], 14);
    RIPEMD160_JJJ(b2, c2, d2, e2, a2, in[3], 12);
    RIPEMD160_JJJ(a2, b2, c2, d2, e2, in[12], 6);
    RIPEMD160_III(e2, a2, b2, c2, d2, in[6], 9);
    RIPEMD160_III(d2, e2, a2, b2, c2, in[11], 13);
    RIPEMD160_III(c2, d2, e2, a2, b2, in[3], 15);
    RIPEMD160_III(b2, c2, d2, e2, a2, in[7], 7);
    RIPEMD160_III(a2, b2, c2, d2, e2, in[0], 12);
    RIPEMD160_III(e2, a2, b2, c2, d2, in[13], 8);
    RIPEMD160_III(d2, e2, a2, b2, c2, in[5], 9);
    RIPEMD160_III(c2, d2, e2, a2, b2, in[10], 11);
    RIPEMD160_III(b2, c2, d2, e2, a2, in[14], 7);
    RIPEMD160_III(a2, b2, c2, d2, e2, in[15], 7);
    RIPEMD160_III(e2, a2, b2, c2, d2, in[8], 12);
    RIPEMD160_III(d2, e2, a2, b2, c2, in[12], 7);
    RIPEMD160_III(c2, d2, e2, a2, b2, in[4], 6);
    RIPEMD160_III(b2, c2, d2, e2, a2, in[9], 15);
    RIPEMD160_III(a2, b2, c2, d2, e2, in[1], 13);
    RIPEMD160_III(e2, a2, b2, c2, d2, in[2], 11);
    RIPEMD160_HHH(d2, e2, a2, b2, c2, in[15], 9);
    RIPEMD160_HHH(c2, d2, e2, a2, b2, in[5], 7);
    RIPEMD160_HHH(b2, c2, d2, e2, a2, in[1], 15);
    RIPEMD160_HHH(a2, b2, c2, d2, e2, in[3], 11);
    RIPEMD160_HHH(e2, a2, b2, c2, d2, in[7], 8);
    RIPEMD160_HHH(d2, e2, a2, b2, c2, in[14], 6);
    RIPEMD160_HHH(c2, d2, e2, a2, b2, in[6], 6);
    RIPEMD160_HHH(b2, c2, d2, e2, a2, in[9], 14);
    RIPEMD160_HHH(a2, b2, c2, d2, e2, in[11], 12);
    RIPEMD160_HHH(e2, a2, b2, c2, d2, in[8], 13);
    RIPEMD160_HHH(d2, e2, a2, b2, c2, in[12], 5);
    RIPEMD160_HHH(c2, d2, e2, a2, b2, in[2], 14);
    RIPEMD160_HHH(b2, c2, d2, e2, a2, in[10], 13);
    RIPEMD160_HHH(a2, b2, c2, d2, e2, in[0], 13);
    RIPEMD160_HHH(e2, a2, b2, c2, d2, in[4], 7);
    RIPEMD160_HHH(d2, e2, a2, b2, c2, in[13], 5);
    RIPEMD160_GGG(c2, d2, e2, a2, b2, in[8], 15);
    RIPEMD160_GGG(b2, c2, d2, e2, a2, in[6], 5);
    RIPEMD160_GGG(a2, b2, c2, d2, e2, in[4], 8);
    RIPEMD160_GGG(e2, a2, b2, c2, d2, in[1], 11);
    RIPEMD160_GGG(d2, e2, a2, b2, c2, in[3], 14);
    RIPEMD160_GGG(c2, d2, e2, a2, b2, in[11], 14);
    RIPEMD160_GGG(b2, c2, d2, e2, a2, in[15], 6);
    RIPEMD160_GGG(a2, b2, c2, d2, e2, in[0], 14);
    RIPEMD160_GGG(e2, a2, b2, c2, d2, in[5], 6);
    RIPEMD160_GGG(d2, e2, a2, b2, c2, in[12], 9);
    RIPEMD160_GGG(c2, d2, e2, a2, b2, in[2], 12);
    RIPEMD160_GGG(b2, c2, d2, e2, a2, in[13], 9);
    RIPEMD160_GGG(a2, b2, c2, d2, e2, in[9], 12);
    RIPEMD160_GGG(e2, a2, b2, c2, d2, in[7], 5);
    RIPEMD160_GGG(d2, e2, a2, b2, c2, in[10], 15);
    RIPEMD160_GGG(c2, d2, e2, a2, b2, in[14], 8);
    RIPEMD160_FFF(b2, c2, d2, e2, a2, in[12], 8);
    RIPEMD160_FFF(a2, b2, c2, d2, e2, in[15], 5);
    RIPEMD160_FFF(e2, a2, b2, c2, d2, in[10], 12);
    RIPEMD160_FFF(d2, e2, a2, b2, c2, in[4], 9);
    RIPEMD160_FFF(c2, d2, e2, a2, b2, in[1], 12);
    RIPEMD160_FFF(b2, c2, d2, e2, a2, in[5], 5);
    RIPEMD160_FFF(a2, b2, c2, d2, e2, in[8], 14);
    RIPEMD160_FFF(e2, a2, b2, c2, d2, in[7], 6);
    RIPEMD160_FFF(d2, e2, a2, b2, c2, in[6], 8);
    RIPEMD160_FFF(c2, d2, e2, a2, b2, in[2], 13);
    RIPEMD160_FFF(b2, c2, d2, e2, a2, in[13], 6);
    RIPEMD160_FFF(a2, b2, c2, d2, e2, in[14], 5);
    RIPEMD160_FFF(e2, a2, b2, c2, d2, in[0], 15);
    RIPEMD160_FFF(d2, e2, a2, b2, c2, in[3], 13);
    RIPEMD160_FFF(c2, d2, e2, a2, b2, in[9], 11);
    RIPEMD160_FFF(b2, c2, d2, e2, a2, in[11], 11);
  } else {
    return(NETWOX_ERR_INTERNALERROR);
  }

  if (pcontext->ripemdversion == NETWOX_RIPEMDVERSION_128) {
    tmp = pcontext->b + c + d2;
    pcontext->b = pcontext->c + d + a2;
    pcontext->c = pcontext->d + a + b2;
    pcontext->d = pcontext->a + b + c2;
    pcontext->a = tmp;
  } else if (pcontext->ripemdversion == NETWOX_RIPEMDVERSION_160) {
    tmp = pcontext->b + c + d2;
    pcontext->b = pcontext->c + d + e2;
    pcontext->c = pcontext->d + e + a2;
    pcontext->d = pcontext->e + a + b2;
    pcontext->e = pcontext->a + b + c2;
    pcontext->a = tmp;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* take data from context */
static netwib_err netwox_priv_ripemd_update64_ctx(netwox_ripemd_context *pcontext)
{

  if (pcontext->bufsize != NETWOX_RIPEMD_BLOCK_MAXLEN) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  netwib_er(netwox_priv_ripemd_update64(pcontext, pcontext->buf));
  pcontext->bufsize = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ripemd_update(netwox_ripemd_context *pcontext,
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
    while (datasize && (pcontext->bufsize < NETWOX_RIPEMD_BLOCK_MAXLEN)) {
      pcontext->buf[pcontext->bufsize++] = *data++;
      datasize--;
    }
    if (pcontext->bufsize == NETWOX_RIPEMD_BLOCK_MAXLEN) {
      netwib_er(netwox_priv_ripemd_update64_ctx(pcontext));
    }
  }

  while (datasize >= NETWOX_RIPEMD_BLOCK_MAXLEN) {
    netwib_er(netwox_priv_ripemd_update64(pcontext, data));
    data += NETWOX_RIPEMD_BLOCK_MAXLEN;
    datasize -= NETWOX_RIPEMD_BLOCK_MAXLEN;
  }
  while (datasize) {
    pcontext->buf[pcontext->bufsize++] = *data++;
    datasize--;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ripemd_final(netwox_ripemd_context *pcontext,
                               netwib_buf *pripemd)
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
    netwib_er(netwox_priv_ripemd_update64_ctx(pcontext));
    netwib_c_memset(pcontext->buf, 0, 56);
    pcontext->bufsize = 56;
  }

  /* append the 64 bit count
     Note: this overflows if totalbytes > 0x1FFFFFFFFFFFFFFF, but we ignore
     because netwox will never use such a big value.
  */
  netwib__uint64_shl(pcontext->totalbytes, 3, totalbits);
  pcontext->buf[56] = netwib_c2_uint64_7(totalbits);
  pcontext->buf[57] = netwib_c2_uint64_6(totalbits);
  pcontext->buf[58] = netwib_c2_uint64_5(totalbits);
  pcontext->buf[59] = netwib_c2_uint64_4(totalbits);
  pcontext->buf[60] = netwib_c2_uint64_3(totalbits);
  pcontext->buf[61] = netwib_c2_uint64_2(totalbits);
  pcontext->buf[62] = netwib_c2_uint64_1(totalbits);
  pcontext->buf[63] = netwib_c2_uint64_0(totalbits);
  pcontext->bufsize += 8;
  netwib_er(netwox_priv_ripemd_update64_ctx(pcontext));

  /* copy in pripemd */
  netwib_er(netwib_buf_wantspace(pripemd, NETWOX_RIPEMD_HASH_MAXLEN, &data));
  netwib__data_append_uint32_le(data, pcontext->a);
  netwib__data_append_uint32_le(data, pcontext->b);
  netwib__data_append_uint32_le(data, pcontext->c);
  netwib__data_append_uint32_le(data, pcontext->d);
  switch(pcontext->ripemdversion) {
  case NETWOX_RIPEMDVERSION_128:
    pripemd->endoffset += NETWOX_RIPEMD128_HASH_LEN;
    break;
  case NETWOX_RIPEMDVERSION_160:
    netwib__data_append_uint32_le(data, pcontext->e);
    pripemd->endoffset += NETWOX_RIPEMD160_HASH_LEN;
    break;
  default:
    return(NETWOX_ERR_INTERNALERROR);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ripemd_compute(netwox_ripemdversion ripemdversion,
                                 netwib_constbuf *pbuf,
                                 netwib_buf *pripemd)
{
  netwox_ripemd_context ctx;

  netwib_er(netwox_ripemd_init(ripemdversion, &ctx));
  netwib_er(netwox_ripemd_update(&ctx, pbuf));
  netwib_er(netwox_ripemd_final(&ctx, pripemd));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_ripemd_test(netwox_ripemdversion ripemdversion,
                                          netwib_conststring data,
                                          netwib_conststring mdwanted)
{
  netwib_buf buf, md;

  netwib_er(netwib_buf_init_ext_string(data, &buf));
  netwib_er(netwib_buf_init_mallocdefault(&md));
  netwib_er(netwox_ripemd_compute(ripemdversion, &buf, &md));
  netwib_er(netwox_buf_check_strmixed(&md, mdwanted));
  netwib_er(netwib_buf_close(&md));

  return(NETWIB_ERR_OK);
}
#define netwox_priv_ripemd128_test(str,strwanted) netwox_priv_ripemd_test(NETWOX_RIPEMDVERSION_128,str,strwanted)
#define netwox_priv_ripemd160_test(str,strwanted) netwox_priv_ripemd_test(NETWOX_RIPEMDVERSION_160,str,strwanted)

/*-------------------------------------------------------------*/
netwib_err netwox_ripemd_test()
{
  /* test suite from http://www.esat.kuleuven.ac.be/~bosselae/ripemd160.html */
  netwib_er(netwox_priv_ripemd128_test("", "cdf26213a150dc3ecb610f18f6b38b46"));
  netwib_er(netwox_priv_ripemd128_test("a", "86be7afa339d0fc7cfc785e72f578d33"));
  netwib_er(netwox_priv_ripemd128_test("abc", "c14a12199c66e4ba84636b0f69144c77"));
  netwib_er(netwox_priv_ripemd128_test("message digest", "9e327b3d6e523062afc1132d7df9d1b8"));
  netwib_er(netwox_priv_ripemd128_test("abcdefghijklmnopqrstuvwxyz", "fd2aa607f71dc8f510714922b371834e"));
  netwib_er(netwox_priv_ripemd128_test("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", "a1aa0689d0fafa2ddc22e88b49133a06"));
  netwib_er(netwox_priv_ripemd128_test("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "d1e959eb179c911faea4624c60c5c702"));
  netwib_er(netwox_priv_ripemd128_test("12345678901234567890123456789012345678901234567890123456789012345678901234567890", "3f45ef194732c2dbb2c4a2c769795fa3"));
  /**/
  netwib_er(netwox_priv_ripemd160_test("", "9c1185a5c5e9fc54612808977ee8f548b2258d31"));
  netwib_er(netwox_priv_ripemd160_test("a", "0bdc9d2d256b3ee9daae347be6f4dc835a467ffe"));
  netwib_er(netwox_priv_ripemd160_test("abc", "8eb208f7e05d987a9b044a8e98c6b087f15a0bfc"));
  netwib_er(netwox_priv_ripemd160_test("message digest", "5d0689ef49d2fae572b881b123a85ffa21595f36"));
  netwib_er(netwox_priv_ripemd160_test("abcdefghijklmnopqrstuvwxyz", "f71c27109c692c1b56bbdceb5b9d2865b3708dbc"));
  netwib_er(netwox_priv_ripemd160_test("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", "12a053384a9c0c88e405a06c27dcf49ada62eb2b"));
  netwib_er(netwox_priv_ripemd160_test("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", "b0e20b6e3116640286ed3a87a5713079b21f5189"));
  netwib_er(netwox_priv_ripemd160_test("12345678901234567890123456789012345678901234567890123456789012345678901234567890", "9b752e45573d4b39f4dbd3323cab82bf63326bfb"));



  return(NETWIB_ERR_OK);
}
