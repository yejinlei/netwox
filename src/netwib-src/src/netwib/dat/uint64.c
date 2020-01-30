/*
                                  NETWIB
                             Network library
                Copyright(c) 1999-2012 Laurent CONSTANTIN
                      http://ntwib.sourceforge.net/
                        laurentconstantin@free.fr
                                  -----
  This file is part of Netwib.

  Netwib is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3
  as published by the Free Software Foundation.

  Netwib is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details (http://www.gnu.org/).

------------------------------------------------------------------------
*/

#include <netwib/inc/maininc.h>

/*-------------------------------------------------------------*/
/* netwib__uint64_add():
    Carry detection:
      If x + y overflows, the result is < x and < y. For example,
      using a byte:
        0x03 + 0xFF = 0x02
        We have: 0x02 < 0x03 and 0x02 < 0xFF
        So, this is a fast way to determine the value of carry. We use
        "tmp < a.low", but we could also use "tmp < b.low".
    Overflow of (a).high + (b).high + carry:
      This operation might again overflow, but we ignore
*/

/*-------------------------------------------------------------*/
/* netwib__uint64_sub():
    Carry detection:
     If x - y underflows, the result is > x. For example,
     using a byte:
       0x03 - 0xFF = 0x04
       We have: 0x04 > 0x03
       So, this is a fast way to determine the value of carry.
    Overflow of (a).high - (b).high - carry:
      This operation might again underflow, but we ignore
*/


/*-------------------------------------------------------------*/
/*
  A multiplication is :
                                             a.high         a.low
       *                                     b.high         b.low
       =============================================================
       +                |               | b.low*a.high | b.low*a.low
       +                | b.high*a.high | b.high*a.low |
       =============================================================
                               c2       |     c1       |     c0

  Normally:
    r.low = c0 + c1<<base
          = b.low*a.low + (b.low*a.high+b.high*a.low)<<base
    r.high = c2 + carry_from_r.low_above
           = b.high*a.high + carry_from_r.low_above
    So, we have 4 multiplications.

  Karatsuba algorithm:
    r.low = q0
    r.high = (q1 - q0 - q2) + (q2)<<base
    with:
      q0 = a.low * b.low
      q1 = (a.low + a.high) * (b.low + b.high)
      q2 = a.high * b.high
    So, we have only 3 multiplications (but more additions).
    Then applying recursively this math trick, we can optimize
    big multiplications. In our case, multiplication is small.

  Fast Fourier Transform algorithm:
    Not adapted for small multiplications.

  Booth algorithm:
    1111 * 11001 = 1111 * 10000 + 1111*1000 + 1111
    1111 * 10111 = 1111 * 10000 + 1111*1000 - 1111 <= Booth
    Adapted for binary multiplications:

  So, in this file, the normal algorithm is used.
*/

/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 1
static void netwib__uint32_mul_private(netwib_uint32 a,
                                       netwib_uint32 b,
                                       netwib_uint64 *pamulb)
{
  netwib_uint32 tmp, c0, c1, c2;

#define netwib__uint16_private_high(x) ((x)>>16)
#define netwib__uint16_private_low(x) ((x)&0xFFFF)

  /* We will compute 3 columns c0, c1 and c2.
     To prove overflow, we use those values:
       MM = max(mul) = 0xFFFF * 0xFFFF = 0xFFFE0001
       MH = max(high(MM)) = 0xFFFE
       ML = max(low(mul)) = 0xFFFF
  */
  c2 = netwib__uint16_private_high(b) * netwib__uint16_private_high(a);
  tmp = netwib__uint16_private_low(b) * netwib__uint16_private_low(a);
  c0 = netwib__uint16_private_low(tmp);
  c1 = netwib__uint16_private_high(tmp);
  /* c1 will not overflow because:
      max(c1) = old_max(c1) + MM = MH + MM = 0xFFFE + 0xFFFE0001
              = 0xFFFEFFFF < 0xFFFFFFFF
  */
  c1 += netwib__uint16_private_low(b) * netwib__uint16_private_high(a);
  tmp = netwib__uint16_private_high(b) * netwib__uint16_private_low(a);
  /* c1 might overflow because:
       max(c1) = old_max(c1) + MM = 0xFFFEFFFF + 0xFFFE0001
               = 0x1FFFD0000
  */
  c1 += tmp;
  if (c1 < tmp) {
    /* overflow occurred in c1, so put a carry in c2 (this carry
       will not cause an overflow in c2) */
    c2 += 0x10000;
  }
  /* to sum up, we have:
       max(c0) = ML = 0xFFFF
       max(c1)[if_over] = 0x[1]FFFD0000
       max(c1)[if_no_over] = 0xFFFFFFFF
       max(c2)[if_over] = MM + 0x10000 = 0xFFFE0001 + 0x10000 = 0xFFFF0001
       max(c2)[if_no_over] = MM = 0xFFFE0001
  */
  /* now, separate c1 and store it in c0 and c2 */
  /* c0 will not overflow because:
       max(c0) = old_max(c0) + low(c1)<<16 = 0xFFFF + 0xFFFF<<16
               = 0xFFFFFFFF
  */
  c0 += netwib__uint16_private_low(c1) << 16;
  /* c2 will not overflow because:
       max(c2)[if_over] = old_max(c2)[if_over] + high(c1)[if_over]
                        = 0xFFFF0001 + 0xFFFD = 0xFFFFFFFE
       max(c2)[if_no_over] = old_max(c2)[if_no_over] + high(c1)[if_no_over]
                        = 0xFFFE0001 + 0xFFFF = 0xFFFF0000
  */
  c2 += netwib__uint16_private_high(c1);
  /* set values */
  pamulb->high = c2;
  pamulb->low = c0;

  return;
}
#endif

/*-------------------------------------------------------------*/
void netwib__uint64_mul_private(netwib_uint64 a,
                                netwib_uint64 b,
                                netwib_uint64 *pamulb)
{

#if NETWIB_INT64_FAKE == 0
  *pamulb = a * b;
#else
  /* To sum up :
       pamulb->low = b.low*a.low
       pamulb->high = b.low*a.high + b.high*a.low
       Ignore b.high*a.high.
   */
  netwib_uint64 tmp;
  netwib__uint32_mul_private(b.low, a.low, pamulb);
  netwib__uint32_mul_private(b.low, a.high, &tmp);
  pamulb->high += tmp.low; /* might overflow, but ignore */
  netwib__uint32_mul_private(b.high, a.low, &tmp);
  pamulb->high += tmp.low; /* might overflow, but ignore */
#endif

  return;
}

/*-------------------------------------------------------------*/
void netwib__uint64_div_private(netwib_uint64 n,
                                netwib_uint64 d,
                                netwib_uint64 *pq,
                                netwib_uint64 *pr)
{

#if NETWIB_INT64_FAKE == 0
  *pq = n / d;
  *pr = n % d;
#else
  netwib_uint64 q, r, tmp;
  netwib_uint32 sh;
  if (d.high == 0 && d.low == 0) {
    /* generate a crash, like for processor */
    d.high = 2 / d.low;
    return;
  }
  /*
     A binary division is for example:
        100110010 || 10101 = 21
        .         ||======
        -10101    || 0111* = 1110 = 14
         10001010 ||
         -10101   ||
           110110 ||
          -10101  ||
             1100 ||
                * ||
     => n=306, d=21, q=14, r=12  (21*14+12=306)

     We could improve it using Radix-4 SRT algorithm. This algorithm is
     complex. Not implemented for now.
  */
  netwib__uint64_init_uint32(0, q);
  sh = 64;
  while(NETWIB_TRUE) {
    if (netwib__uint64_cmp_lt(n, d)) {
      netwib__uint64_shl(q, sh, q);
      r = n;
      break;
    }
    if (sh == 0) {
      /* Internal Error: generate a crash */
      d.high = 2 / sh;
      return;
    }
    sh--;
    netwib__uint64_shl(q, 1, q);
    netwib__uint64_shr(n, sh, tmp);
    if (netwib__uint64_cmp_ge(tmp, d)) {
      netwib__uint64_shl(d, sh, tmp);
      netwib__uint64_sub(n, tmp, n);
      q.low |= 1;
    }
  }
  *pq = q;
  *pr = r;
#endif

  return;
}

/*-------------------------------------------------------------*/
void netwib__int64_div_private(netwib_int64 n,
                               netwib_int64 d,
                               netwib_int64 *pq,
                               netwib_int64 *pr)
{

#if NETWIB_INT64_FAKE == 0
  *pq = n / d;
  *pr = n % d;
#else
  netwib_uint64 n2, d2, q, r;
  netwib_bool changesign;
  changesign = NETWIB_FALSE;
  n2 = n;
  if (n.high & 0x80000000u) {
    changesign = ! changesign;
    netwib__int64_neg(n, n2);
  }
  d2 = d;
  if (d.high & 0x80000000u) {
    changesign = ! changesign;
    netwib__int64_neg(d, d2);
  }
  netwib__uint64_div(n2, d2, &q, &r);
  if (changesign) {
    netwib__int64_neg(q, q);
  }
  if (n.high & 0x80000000u) {
    netwib__int64_neg(r, r);
  }
  *pq = q;
  *pr = r;
#endif

  return;
}
