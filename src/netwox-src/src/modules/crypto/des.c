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
/* This code is adapted from libgcrypt under LGPL license. */

/*-------------------------------------------------------------*/
/*
 * The s-box values are permuted according to the 'primitive function P'
 * and are rotated one bit to the left.
 */
static netwib_uint32 sbox1[64] =
{
  0x01010400, 0x00000000, 0x00010000, 0x01010404, 0x01010004, 0x00010404, 0x00000004, 0x00010000,
  0x00000400, 0x01010400, 0x01010404, 0x00000400, 0x01000404, 0x01010004, 0x01000000, 0x00000004,
  0x00000404, 0x01000400, 0x01000400, 0x00010400, 0x00010400, 0x01010000, 0x01010000, 0x01000404,
  0x00010004, 0x01000004, 0x01000004, 0x00010004, 0x00000000, 0x00000404, 0x00010404, 0x01000000,
  0x00010000, 0x01010404, 0x00000004, 0x01010000, 0x01010400, 0x01000000, 0x01000000, 0x00000400,
  0x01010004, 0x00010000, 0x00010400, 0x01000004, 0x00000400, 0x00000004, 0x01000404, 0x00010404,
  0x01010404, 0x00010004, 0x01010000, 0x01000404, 0x01000004, 0x00000404, 0x00010404, 0x01010400,
  0x00000404, 0x01000400, 0x01000400, 0x00000000, 0x00010004, 0x00010400, 0x00000000, 0x01010004
};

static netwib_uint32 sbox2[64] =
{
  0x80108020, 0x80008000, 0x00008000, 0x00108020, 0x00100000, 0x00000020, 0x80100020, 0x80008020,
  0x80000020, 0x80108020, 0x80108000, 0x80000000, 0x80008000, 0x00100000, 0x00000020, 0x80100020,
  0x00108000, 0x00100020, 0x80008020, 0x00000000, 0x80000000, 0x00008000, 0x00108020, 0x80100000,
  0x00100020, 0x80000020, 0x00000000, 0x00108000, 0x00008020, 0x80108000, 0x80100000, 0x00008020,
  0x00000000, 0x00108020, 0x80100020, 0x00100000, 0x80008020, 0x80100000, 0x80108000, 0x00008000,
  0x80100000, 0x80008000, 0x00000020, 0x80108020, 0x00108020, 0x00000020, 0x00008000, 0x80000000,
  0x00008020, 0x80108000, 0x00100000, 0x80000020, 0x00100020, 0x80008020, 0x80000020, 0x00100020,
  0x00108000, 0x00000000, 0x80008000, 0x00008020, 0x80000000, 0x80100020, 0x80108020, 0x00108000
};

static netwib_uint32 sbox3[64] =
{
  0x00000208, 0x08020200, 0x00000000, 0x08020008, 0x08000200, 0x00000000, 0x00020208, 0x08000200,
  0x00020008, 0x08000008, 0x08000008, 0x00020000, 0x08020208, 0x00020008, 0x08020000, 0x00000208,
  0x08000000, 0x00000008, 0x08020200, 0x00000200, 0x00020200, 0x08020000, 0x08020008, 0x00020208,
  0x08000208, 0x00020200, 0x00020000, 0x08000208, 0x00000008, 0x08020208, 0x00000200, 0x08000000,
  0x08020200, 0x08000000, 0x00020008, 0x00000208, 0x00020000, 0x08020200, 0x08000200, 0x00000000,
  0x00000200, 0x00020008, 0x08020208, 0x08000200, 0x08000008, 0x00000200, 0x00000000, 0x08020008,
  0x08000208, 0x00020000, 0x08000000, 0x08020208, 0x00000008, 0x00020208, 0x00020200, 0x08000008,
  0x08020000, 0x08000208, 0x00000208, 0x08020000, 0x00020208, 0x00000008, 0x08020008, 0x00020200
};

static netwib_uint32 sbox4[64] =
{
  0x00802001, 0x00002081, 0x00002081, 0x00000080, 0x00802080, 0x00800081, 0x00800001, 0x00002001,
  0x00000000, 0x00802000, 0x00802000, 0x00802081, 0x00000081, 0x00000000, 0x00800080, 0x00800001,
  0x00000001, 0x00002000, 0x00800000, 0x00802001, 0x00000080, 0x00800000, 0x00002001, 0x00002080,
  0x00800081, 0x00000001, 0x00002080, 0x00800080, 0x00002000, 0x00802080, 0x00802081, 0x00000081,
  0x00800080, 0x00800001, 0x00802000, 0x00802081, 0x00000081, 0x00000000, 0x00000000, 0x00802000,
  0x00002080, 0x00800080, 0x00800081, 0x00000001, 0x00802001, 0x00002081, 0x00002081, 0x00000080,
  0x00802081, 0x00000081, 0x00000001, 0x00002000, 0x00800001, 0x00002001, 0x00802080, 0x00800081,
  0x00002001, 0x00002080, 0x00800000, 0x00802001, 0x00000080, 0x00800000, 0x00002000, 0x00802080
};

static netwib_uint32 sbox5[64] =
{
  0x00000100, 0x02080100, 0x02080000, 0x42000100, 0x00080000, 0x00000100, 0x40000000, 0x02080000,
  0x40080100, 0x00080000, 0x02000100, 0x40080100, 0x42000100, 0x42080000, 0x00080100, 0x40000000,
  0x02000000, 0x40080000, 0x40080000, 0x00000000, 0x40000100, 0x42080100, 0x42080100, 0x02000100,
  0x42080000, 0x40000100, 0x00000000, 0x42000000, 0x02080100, 0x02000000, 0x42000000, 0x00080100,
  0x00080000, 0x42000100, 0x00000100, 0x02000000, 0x40000000, 0x02080000, 0x42000100, 0x40080100,
  0x02000100, 0x40000000, 0x42080000, 0x02080100, 0x40080100, 0x00000100, 0x02000000, 0x42080000,
  0x42080100, 0x00080100, 0x42000000, 0x42080100, 0x02080000, 0x00000000, 0x40080000, 0x42000000,
  0x00080100, 0x02000100, 0x40000100, 0x00080000, 0x00000000, 0x40080000, 0x02080100, 0x40000100
};

static netwib_uint32 sbox6[64] =
{
  0x20000010, 0x20400000, 0x00004000, 0x20404010, 0x20400000, 0x00000010, 0x20404010, 0x00400000,
  0x20004000, 0x00404010, 0x00400000, 0x20000010, 0x00400010, 0x20004000, 0x20000000, 0x00004010,
  0x00000000, 0x00400010, 0x20004010, 0x00004000, 0x00404000, 0x20004010, 0x00000010, 0x20400010,
  0x20400010, 0x00000000, 0x00404010, 0x20404000, 0x00004010, 0x00404000, 0x20404000, 0x20000000,
  0x20004000, 0x00000010, 0x20400010, 0x00404000, 0x20404010, 0x00400000, 0x00004010, 0x20000010,
  0x00400000, 0x20004000, 0x20000000, 0x00004010, 0x20000010, 0x20404010, 0x00404000, 0x20400000,
  0x00404010, 0x20404000, 0x00000000, 0x20400010, 0x00000010, 0x00004000, 0x20400000, 0x00404010,
  0x00004000, 0x00400010, 0x20004010, 0x00000000, 0x20404000, 0x20000000, 0x00400010, 0x20004010
};

static netwib_uint32 sbox7[64] =
{
  0x00200000, 0x04200002, 0x04000802, 0x00000000, 0x00000800, 0x04000802, 0x00200802, 0x04200800,
  0x04200802, 0x00200000, 0x00000000, 0x04000002, 0x00000002, 0x04000000, 0x04200002, 0x00000802,
  0x04000800, 0x00200802, 0x00200002, 0x04000800, 0x04000002, 0x04200000, 0x04200800, 0x00200002,
  0x04200000, 0x00000800, 0x00000802, 0x04200802, 0x00200800, 0x00000002, 0x04000000, 0x00200800,
  0x04000000, 0x00200800, 0x00200000, 0x04000802, 0x04000802, 0x04200002, 0x04200002, 0x00000002,
  0x00200002, 0x04000000, 0x04000800, 0x00200000, 0x04200800, 0x00000802, 0x00200802, 0x04200800,
  0x00000802, 0x04000002, 0x04200802, 0x04200000, 0x00200800, 0x00000000, 0x00000002, 0x04200802,
  0x00000000, 0x00200802, 0x04200000, 0x00000800, 0x04000002, 0x04000800, 0x00000800, 0x00200002
};

static netwib_uint32 sbox8[64] =
{
  0x10001040, 0x00001000, 0x00040000, 0x10041040, 0x10000000, 0x10001040, 0x00000040, 0x10000000,
  0x00040040, 0x10040000, 0x10041040, 0x00041000, 0x10041000, 0x00041040, 0x00001000, 0x00000040,
  0x10040000, 0x10000040, 0x10001000, 0x00001040, 0x00041000, 0x00040040, 0x10040040, 0x10041000,
  0x00001040, 0x00000000, 0x00000000, 0x10040040, 0x10000040, 0x10001000, 0x00041040, 0x00040000,
  0x00041040, 0x00040000, 0x10041000, 0x00001000, 0x00000040, 0x10040040, 0x00001000, 0x00041040,
  0x10001000, 0x00000040, 0x10000040, 0x10040000, 0x10040040, 0x10000000, 0x00040000, 0x10001040,
  0x00000000, 0x10041040, 0x00040040, 0x10000040, 0x10040000, 0x10001000, 0x10001040, 0x00000000,
  0x10041040, 0x00041000, 0x00041000, 0x00001040, 0x00001040, 0x00040040, 0x10000000, 0x10041000
};

/*-------------------------------------------------------------*/
/*
 * These two tables are part of the 'permuted choice 1' function.
 * In this implementation several speed improvements are done.
 */
static netwib_uint32 leftkey_swap[16] =
{
  0x00000000, 0x00000001, 0x00000100, 0x00000101,
  0x00010000, 0x00010001, 0x00010100, 0x00010101,
  0x01000000, 0x01000001, 0x01000100, 0x01000101,
  0x01010000, 0x01010001, 0x01010100, 0x01010101
};

static netwib_uint32 rightkey_swap[16] =
{
  0x00000000, 0x01000000, 0x00010000, 0x01010000,
  0x00000100, 0x01000100, 0x00010100, 0x01010100,
  0x00000001, 0x01000001, 0x00010001, 0x01010001,
  0x00000101, 0x01000101, 0x00010101, 0x01010101,
};

/*-------------------------------------------------------------*/
/*
 * Numbers of left shifts per round for encryption subkeys.
 * To calculate the decryption subkeys we just reverse the
 * ordering of the calculated encryption subkeys. So their
 * is no need for a decryption rotate tab.
 */
static netwib_byte encrypt_rotate_tab[16] =
{
  1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

/*-------------------------------------------------------------*/
/*
 * Macro to swap bits across two words.
 */
#define DO_PERMUTATION(a, temp, b, offset, mask) temp = ((a>>offset) ^ b) & mask; b ^= temp; a ^= temp<<offset;

/*
 * This performs the 'initial permutation' of the data to be encrypted
 * or decrypted. Additionally the resulting two words are rotated one bit
 * to the left.
 */
#define INITIAL_PERMUTATION(left, temp, right) DO_PERMUTATION(left, temp, right, 4, 0x0f0f0f0f) DO_PERMUTATION(left, temp, right, 16, 0x0000ffff) DO_PERMUTATION(right, temp, left, 2, 0x33333333) DO_PERMUTATION(right, temp, left, 8, 0x00ff00ff) right = (right << 1) | (right >> 31); temp = (left ^ right) & 0xaaaaaaaa; right ^= temp; left ^= temp; left = (left << 1) | (left >> 31);

/*
 * The 'inverse initial permutation'.
 */
#define FINAL_PERMUTATION(left, temp, right) left = (left << 31) | (left >> 1); temp = (left ^ right) & 0xaaaaaaaa; left ^= temp; right ^= temp; right = (right << 31) | (right >> 1); DO_PERMUTATION(right, temp, left, 8, 0x00ff00ff) DO_PERMUTATION(right, temp, left, 2, 0x33333333) DO_PERMUTATION(left, temp, right, 16, 0x0000ffff) DO_PERMUTATION(left, temp, right, 4, 0x0f0f0f0f)

/*
 * A full DES round including 'expansion function', 'sbox substitution'
 * and 'primitive function P' but without swapping the left and right word.
 * Please note: The data in 'from' and 'to' is already rotated one bit to
 * the left, done in the initial permutation.
 */
#define DES_ROUND(from, to, work, subkey) work = from ^ *subkey++; to ^= sbox8[ work & 0x3f ]; to ^= sbox6[ (work>>8) & 0x3f ]; to ^= sbox4[ (work>>16) & 0x3f ]; to ^= sbox2[ (work>>24) & 0x3f ];work = ((from << 28) | (from >> 4)) ^ *subkey++; to ^= sbox7[ work & 0x3f ]; to ^= sbox5[ (work>>8) & 0x3f ]; to ^= sbox3[ (work>>16) & 0x3f ]; to ^= sbox1[ (work>>24) & 0x3f ];

/*
 * Macros to convert 8 bytes from/to 32bit words.
 */
#define READ_64BIT_DATA(data, left, right) left = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3]; right = (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7];

#define WRITE_64BIT_DATA(data, left, right) data[0] = (netwib_byte)((left >> 24) &0xff); data[1] = (netwib_byte)((left >> 16) &0xff); data[2] = (netwib_byte)((left >> 8) &0xff); data[3] = (netwib_byte)(left &0xff); data[4] = (netwib_byte)((right >> 24) &0xff); data[5] = (netwib_byte)((right >> 16) &0xff); data[6] = (netwib_byte)((right >> 8) &0xff); data[7] = (netwib_byte)(right &0xff);

/*-------------------------------------------------------------*/
/*
 * des_key_schedule():    Calculate 16 subkeys pairs (even/odd) for
 *                        16 encryption rounds.
 *                        To calculate subkeys for decryption the caller
 *                        have to reorder the generated subkeys.
 *
 *    rawkey:       8 Bytes of key data
 *    subkey:       Array of at least 32 netwib_uint32s. Will be filled
 *                  with calculated subkeys.
 *
 */
static netwib_err netwox_des_key_schedule(netwib_constdata rawkey,
                                          netwib_uint32 *subkey)
{
  netwib_uint32 left, right, work;
  netwib_int32 desround;

  READ_64BIT_DATA(rawkey, left, right);

  DO_PERMUTATION(right, work, left, 4, 0x0f0f0f0f);
  DO_PERMUTATION(right, work, left, 0, 0x10101010);

  left = ((leftkey_swap[(left >> 0) & 0xf] << 3)
          | (leftkey_swap[(left >> 8) & 0xf] << 2)
          | (leftkey_swap[(left >> 16) & 0xf] << 1)
          | (leftkey_swap[(left >> 24) & 0xf])
          | (leftkey_swap[(left >> 5) & 0xf] << 7)
          | (leftkey_swap[(left >> 13) & 0xf] << 6)
          | (leftkey_swap[(left >> 21) & 0xf] << 5)
          | (leftkey_swap[(left >> 29) & 0xf] << 4));

  left &= 0x0fffffff;

  right = ((rightkey_swap[(right >> 1) & 0xf] << 3)
           | (rightkey_swap[(right >> 9) & 0xf] << 2)
           | (rightkey_swap[(right >> 17) & 0xf] << 1)
           | (rightkey_swap[(right >> 25) & 0xf])
           | (rightkey_swap[(right >> 4) & 0xf] << 7)
           | (rightkey_swap[(right >> 12) & 0xf] << 6)
           | (rightkey_swap[(right >> 20) & 0xf] << 5)
           | (rightkey_swap[(right >> 28) & 0xf] << 4));

  right &= 0x0fffffff;

  for (desround = 0; desround < 16; ++desround) {
    left = ((left << encrypt_rotate_tab[desround])
            | (left >> (28 - encrypt_rotate_tab[desround]))) & 0x0fffffff;
    right = ((right << encrypt_rotate_tab[desround])
             | (right >> (28 - encrypt_rotate_tab[desround]))) & 0x0fffffff;

    *subkey++ = (((left << 4) & 0x24000000)
                 | ((left << 28) & 0x10000000)
                 | ((left << 14) & 0x08000000)
                 | ((left << 18) & 0x02080000)
                 | ((left << 6) & 0x01000000)
                 | ((left << 9) & 0x00200000)
                 | ((left >> 1) & 0x00100000)
                 | ((left << 10) & 0x00040000)
                 | ((left << 2) & 0x00020000)
                 | ((left >> 10) & 0x00010000)
                 | ((right >> 13) & 0x00002000)
                 | ((right >> 4) & 0x00001000)
                 | ((right << 6) & 0x00000800)
                 | ((right >> 1) & 0x00000400)
                 | ((right >> 14) & 0x00000200)
                 | (right & 0x00000100)
                 | ((right >> 5) & 0x00000020)
                 | ((right >> 10) & 0x00000010)
                 | ((right >> 3) & 0x00000008)
                 | ((right >> 18) & 0x00000004)
                 | ((right >> 26) & 0x00000002)
                 | ((right >> 24) & 0x00000001));

    *subkey++ = (((left << 15) & 0x20000000)
                 | ((left << 17) & 0x10000000)
                 | ((left << 10) & 0x08000000)
                 | ((left << 22) & 0x04000000)
                 | ((left >> 2) & 0x02000000)
                 | ((left << 1) & 0x01000000)
                 | ((left << 16) & 0x00200000)
                 | ((left << 11) & 0x00100000)
                 | ((left << 3) & 0x00080000)
                 | ((left >> 6) & 0x00040000)
                 | ((left << 15) & 0x00020000)
                 | ((left >> 4) & 0x00010000)
                 | ((right >> 2) & 0x00002000)
                 | ((right << 8) & 0x00001000)
                 | ((right >> 14) & 0x00000808)
                 | ((right >> 9) & 0x00000400)
                 | ((right) & 0x00000200)
                 | ((right << 7) & 0x00000100)
                 | ((right >> 7) & 0x00000020)
                 | ((right >> 3) & 0x00000011)
                 | ((right << 2) & 0x00000004)
                 | ((right >> 21) & 0x00000002));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*
 * Table with weak DES keys sorted in ascending order.
 * In DES their are 64 known keys wich are weak. They are weak
 * because they produce only one, two or four different
 * subkeys in the subkey scheduling process.
 * The keys in this table have all their parity bits cleared.
 */
static netwib_byte weak_keys[64][8] =
{
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, /*w*/
  { 0x00, 0x00, 0x1e, 0x1e, 0x00, 0x00, 0x0e, 0x0e },
  { 0x00, 0x00, 0xe0, 0xe0, 0x00, 0x00, 0xf0, 0xf0 },
  { 0x00, 0x00, 0xfe, 0xfe, 0x00, 0x00, 0xfe, 0xfe },
  { 0x00, 0x1e, 0x00, 0x1e, 0x00, 0x0e, 0x00, 0x0e }, /*sw*/
  { 0x00, 0x1e, 0x1e, 0x00, 0x00, 0x0e, 0x0e, 0x00 },
  { 0x00, 0x1e, 0xe0, 0xfe, 0x00, 0x0e, 0xf0, 0xfe },
  { 0x00, 0x1e, 0xfe, 0xe0, 0x00, 0x0e, 0xfe, 0xf0 },
  { 0x00, 0xe0, 0x00, 0xe0, 0x00, 0xf0, 0x00, 0xf0 }, /*sw*/
  { 0x00, 0xe0, 0x1e, 0xfe, 0x00, 0xf0, 0x0e, 0xfe },
  { 0x00, 0xe0, 0xe0, 0x00, 0x00, 0xf0, 0xf0, 0x00 },
  { 0x00, 0xe0, 0xfe, 0x1e, 0x00, 0xf0, 0xfe, 0x0e },
  { 0x00, 0xfe, 0x00, 0xfe, 0x00, 0xfe, 0x00, 0xfe }, /*sw*/
  { 0x00, 0xfe, 0x1e, 0xe0, 0x00, 0xfe, 0x0e, 0xf0 },
  { 0x00, 0xfe, 0xe0, 0x1e, 0x00, 0xfe, 0xf0, 0x0e },
  { 0x00, 0xfe, 0xfe, 0x00, 0x00, 0xfe, 0xfe, 0x00 },
  { 0x1e, 0x00, 0x00, 0x1e, 0x0e, 0x00, 0x00, 0x0e },
  { 0x1e, 0x00, 0x1e, 0x00, 0x0e, 0x00, 0x0e, 0x00 }, /*sw*/
  { 0x1e, 0x00, 0xe0, 0xfe, 0x0e, 0x00, 0xf0, 0xfe },
  { 0x1e, 0x00, 0xfe, 0xe0, 0x0e, 0x00, 0xfe, 0xf0 },
  { 0x1e, 0x1e, 0x00, 0x00, 0x0e, 0x0e, 0x00, 0x00 },
  { 0x1e, 0x1e, 0x1e, 0x1e, 0x0e, 0x0e, 0x0e, 0x0e }, /*w*/
  { 0x1e, 0x1e, 0xe0, 0xe0, 0x0e, 0x0e, 0xf0, 0xf0 },
  { 0x1e, 0x1e, 0xfe, 0xfe, 0x0e, 0x0e, 0xfe, 0xfe },
  { 0x1e, 0xe0, 0x00, 0xfe, 0x0e, 0xf0, 0x00, 0xfe },
  { 0x1e, 0xe0, 0x1e, 0xe0, 0x0e, 0xf0, 0x0e, 0xf0 }, /*sw*/
  { 0x1e, 0xe0, 0xe0, 0x1e, 0x0e, 0xf0, 0xf0, 0x0e },
  { 0x1e, 0xe0, 0xfe, 0x00, 0x0e, 0xf0, 0xfe, 0x00 },
  { 0x1e, 0xfe, 0x00, 0xe0, 0x0e, 0xfe, 0x00, 0xf0 },
  { 0x1e, 0xfe, 0x1e, 0xfe, 0x0e, 0xfe, 0x0e, 0xfe }, /*sw*/
  { 0x1e, 0xfe, 0xe0, 0x00, 0x0e, 0xfe, 0xf0, 0x00 },
  { 0x1e, 0xfe, 0xfe, 0x1e, 0x0e, 0xfe, 0xfe, 0x0e },
  { 0xe0, 0x00, 0x00, 0xe0, 0xf0, 0x00, 0x00, 0xf0 },
  { 0xe0, 0x00, 0x1e, 0xfe, 0xf0, 0x00, 0x0e, 0xfe },
  { 0xe0, 0x00, 0xe0, 0x00, 0xf0, 0x00, 0xf0, 0x00 }, /*sw*/
  { 0xe0, 0x00, 0xfe, 0x1e, 0xf0, 0x00, 0xfe, 0x0e },
  { 0xe0, 0x1e, 0x00, 0xfe, 0xf0, 0x0e, 0x00, 0xfe },
  { 0xe0, 0x1e, 0x1e, 0xe0, 0xf0, 0x0e, 0x0e, 0xf0 },
  { 0xe0, 0x1e, 0xe0, 0x1e, 0xf0, 0x0e, 0xf0, 0x0e }, /*sw*/
  { 0xe0, 0x1e, 0xfe, 0x00, 0xf0, 0x0e, 0xfe, 0x00 },
  { 0xe0, 0xe0, 0x00, 0x00, 0xf0, 0xf0, 0x00, 0x00 },
  { 0xe0, 0xe0, 0x1e, 0x1e, 0xf0, 0xf0, 0x0e, 0x0e },
  { 0xe0, 0xe0, 0xe0, 0xe0, 0xf0, 0xf0, 0xf0, 0xf0 }, /*w*/
  { 0xe0, 0xe0, 0xfe, 0xfe, 0xf0, 0xf0, 0xfe, 0xfe },
  { 0xe0, 0xfe, 0x00, 0x1e, 0xf0, 0xfe, 0x00, 0x0e },
  { 0xe0, 0xfe, 0x1e, 0x00, 0xf0, 0xfe, 0x0e, 0x00 },
  { 0xe0, 0xfe, 0xe0, 0xfe, 0xf0, 0xfe, 0xf0, 0xfe }, /*sw*/
  { 0xe0, 0xfe, 0xfe, 0xe0, 0xf0, 0xfe, 0xfe, 0xf0 },
  { 0xfe, 0x00, 0x00, 0xfe, 0xfe, 0x00, 0x00, 0xfe },
  { 0xfe, 0x00, 0x1e, 0xe0, 0xfe, 0x00, 0x0e, 0xf0 },
  { 0xfe, 0x00, 0xe0, 0x1e, 0xfe, 0x00, 0xf0, 0x0e },
  { 0xfe, 0x00, 0xfe, 0x00, 0xfe, 0x00, 0xfe, 0x00 }, /*sw*/
  { 0xfe, 0x1e, 0x00, 0xe0, 0xfe, 0x0e, 0x00, 0xf0 },
  { 0xfe, 0x1e, 0x1e, 0xfe, 0xfe, 0x0e, 0x0e, 0xfe },
  { 0xfe, 0x1e, 0xe0, 0x00, 0xfe, 0x0e, 0xf0, 0x00 },
  { 0xfe, 0x1e, 0xfe, 0x1e, 0xfe, 0x0e, 0xfe, 0x0e }, /*sw*/
  { 0xfe, 0xe0, 0x00, 0x1e, 0xfe, 0xf0, 0x00, 0x0e },
  { 0xfe, 0xe0, 0x1e, 0x00, 0xfe, 0xf0, 0x0e, 0x00 },
  { 0xfe, 0xe0, 0xe0, 0xfe, 0xfe, 0xf0, 0xf0, 0xfe },
  { 0xfe, 0xe0, 0xfe, 0xe0, 0xfe, 0xf0, 0xfe, 0xf0 }, /*sw*/
  { 0xfe, 0xfe, 0x00, 0x00, 0xfe, 0xfe, 0x00, 0x00 },
  { 0xfe, 0xfe, 0x1e, 0x1e, 0xfe, 0xfe, 0x0e, 0x0e },
  { 0xfe, 0xfe, 0xe0, 0xe0, 0xfe, 0xfe, 0xf0, 0xf0 },
  { 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe }  /*w*/
};
/*
 * Check whether the 8 byte key is weak.
 * Does not check the parity bits of the key but simple ignore them.
 */
static netwib_err netwox_des_is_weak_key(netwib_constdata key,
                                         netwib_bool *pisweak)
{
  netwib_byte work[8];
  netwib_int32 i, left, right, middle, cmp_result;

  *pisweak = NETWIB_FALSE;

  /* clear parity bits */
  for (i = 0; i < 8; ++i) {
    work[i] = (netwib_byte)(key[i] & 0xfe);
  }

  /* binary search in the weak key table */
  left = 0;
  right = 63;
  while(left <= right) {
    middle = (left + right) / 2;
    cmp_result = netwib_c_memcmp(work, weak_keys[middle], 8);
    if (cmp_result == 0) {
      *pisweak = NETWIB_TRUE;
      break;
    } else if (cmp_result > 0) {
      left = middle + 1;
    } else {
      right = middle - 1;
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*
 * Fill a DES context with subkeys calculated from a 64bit key.
 * Does not check parity bits, but simply ignore them.
 */
static netwib_err netwox_des_context_init(netwib_constdata key,
                                          netwib_bool forbidweakkeys,
                                          netwox_des_context *pcontext)
{
  netwib_bool weak;
  netwib_int32 i;

  netwib_er(netwox_des_key_schedule(key, pcontext->encrypt_subkeys));

  for(i = 0; i < 32; i += 2) {
    pcontext->decrypt_subkeys[i] = pcontext->encrypt_subkeys[30-i];
    pcontext->decrypt_subkeys[i+1] = pcontext->encrypt_subkeys[31-i];
  }

  if (forbidweakkeys) {
    netwib_er(netwox_des_is_weak_key(key, &weak));
    if (weak) {
      return(NETWOX_ERR_BADVALUE);
    }
  }

  return(NETWIB_ERR_OK);
}
netwib_err netwox_des_context_init8(netwib_constbuf *pkey,
                                    netwib_bool forbidweakkeys,
                                    netwox_des_context *pcontext)
{
  netwib_data key;
  netwib_uint32 keysize;

  keysize = netwib__buf_ref_data_size(pkey);
  if (keysize != 8) {
    return(NETWOX_ERR_BADSIZE);
  }
  key = netwib__buf_ref_data_ptr(pkey);

  netwib_er(netwox_des_context_init(key, forbidweakkeys, pcontext));

  return(NETWIB_ERR_OK);
}
netwib_err netwox_des_context_init7(netwib_constbuf *pkey,
                                    netwib_bool forbidweakkeys,
                                    netwox_des_context *pcontext)
{
  netwib_byte key8[8];
  netwib_data key;
  netwib_uint32 keysize;
  netwib_int32 i;

  keysize = netwib__buf_ref_data_size(pkey);
  if (keysize != 7) {
    return(NETWOX_ERR_BADSIZE);
  }
  key = netwib__buf_ref_data_ptr(pkey);

  key8[0] = (netwib_byte)(key[0] >> 1);
  key8[1] = (netwib_byte)(((key[0]&0x01)<<6) | (key[1]>>2));
  key8[2] = (netwib_byte)(((key[1]&0x03)<<5) | (key[2]>>3));
  key8[3] = (netwib_byte)(((key[2]&0x07)<<4) | (key[3]>>4));
  key8[4] = (netwib_byte)(((key[3]&0x0F)<<3) | (key[4]>>5));
  key8[5] = (netwib_byte)(((key[4]&0x1F)<<2) | (key[5]>>6));
  key8[6] = (netwib_byte)(((key[5]&0x3F)<<1) | (key[6]>>7));
  key8[7] = (netwib_byte)(key[6] & 0x7F);
  for (i = 0; i < 8; i++) {
    /* parity is the LSB. It will always be 0, but this is not
       important because netwox_des_context_init does not check parity. */
    key8[i] = (netwib_byte)(key8[i] << 1);
  }

  netwib_er(netwox_des_context_init(key8, forbidweakkeys, pcontext));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*
 * Electronic Codebook Mode DES encryption/decryption of data.
 */
static netwib_err netwox_des_compute(netwib_uint32 *keys,
                                     netwib_constbuf *pfrom,
                                     netwib_buf *pto)
{
  netwib_uint32 left, right, work;
  netwib_data from, to;
  netwib_uint32 fromsize;

  fromsize = netwib__buf_ref_data_size(pfrom);
  if (fromsize != NETWOX_DES_BLOCK_LEN) {
    return(NETWOX_ERR_BADSIZE);
  }
  from = netwib__buf_ref_data_ptr(pfrom);

  netwib_er(netwib_buf_wantspace(pto, NETWOX_DES_BLOCK_LEN, &to));

  READ_64BIT_DATA(from, left, right);
  INITIAL_PERMUTATION(left, work, right);

  DES_ROUND(right, left, work, keys); DES_ROUND(left, right, work, keys);
  DES_ROUND(right, left, work, keys); DES_ROUND(left, right, work, keys);
  DES_ROUND(right, left, work, keys); DES_ROUND(left, right, work, keys);
  DES_ROUND(right, left, work, keys); DES_ROUND(left, right, work, keys);
  DES_ROUND(right, left, work, keys); DES_ROUND(left, right, work, keys);
  DES_ROUND(right, left, work, keys); DES_ROUND(left, right, work, keys);
  DES_ROUND(right, left, work, keys); DES_ROUND(left, right, work, keys);
  DES_ROUND(right, left, work, keys); DES_ROUND(left, right, work, keys);

  FINAL_PERMUTATION(right, work, left);
  WRITE_64BIT_DATA(to, right, left);

  pto->endoffset += NETWOX_DES_BLOCK_LEN;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_des_encrypt(netwox_des_context *pcontext,
                              netwib_constbuf *pfrom,
                              netwib_buf *pto)
{
  return(netwox_des_compute(pcontext->encrypt_subkeys, pfrom, pto));
}

/*-------------------------------------------------------------*/
netwib_err netwox_des_decrypt(netwox_des_context *pcontext,
                              netwib_constbuf *pfrom,
                              netwib_buf *pto)
{
  return(netwox_des_compute(pcontext->decrypt_subkeys, pfrom, pto));
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_des_test(netwib_conststring mixedkey8,
                                       netwib_conststring mixeddata,
                                       netwib_conststring hexadataencwanted)
{
  netwib_buf key8, data, dataenc, datadec;
  netwox_des_context context;

  netwib_er(netwib_buf_init_mallocdefault(&key8));
  netwib_er(netwox_buf_append_strmixed(mixedkey8, &key8));
  netwib_er(netwox_des_context_init8(&key8, NETWIB_TRUE, &context));
  netwib_er(netwib_buf_close(&key8));

  netwib_er(netwib_buf_init_mallocdefault(&data));
  netwib_er(netwox_buf_append_strmixed(mixeddata, &data));

  netwib_er(netwib_buf_init_mallocdefault(&dataenc));
  netwib_er(netwib_buf_init_mallocdefault(&datadec));
  netwib_er(netwox_des_encrypt(&context, &data, &dataenc));
  netwib_er(netwox_des_decrypt(&context, &dataenc, &datadec));

  netwib_er(netwox_buf_check_strmixed(&dataenc, hexadataencwanted));
  netwib_er(netwox_buf_check_strmixed(&datadec, mixeddata));

  netwib_er(netwib_buf_close(&datadec));
  netwib_er(netwib_buf_close(&dataenc));
  netwib_er(netwib_buf_close(&data));

  return(NETWIB_ERR_OK);
}
netwib_err netwox_des_test(void)
{

  /* test suite from FIPS81 document */
  netwib_er(netwox_priv_des_test("0123456789abcdef", "4e6f772069732074", "3fa40e8a984d4815"));
  netwib_er(netwox_priv_des_test("0123456789abcdef", "68652074696d6520", "6a271787ab8883f9"));
  netwib_er(netwox_priv_des_test("0123456789abcdef", "666f7220616c6c20", "893d51ec4b563b53"));

  return(NETWIB_ERR_OK);
}
