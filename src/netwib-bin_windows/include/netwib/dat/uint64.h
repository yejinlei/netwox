
/*-------------------------------------------------------------*/
/***************************************************************
 * Type netwib_uint64, can be represented as:                  *
 *  - a 64 bit integer                                         *
 *  - a structure containing two 32 bit integers               *
 * In the first case, math operations are supported by the     *
 * compiler.                                                   *
 * In the second case, following functions are needed.         *
 ***************************************************************/

/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
 #define netwib__uint64_init_8(a,b,c,d,e,f,g,h,x) x = netwib_c2_uint64_8(a,b,c,d,e,f,g,h)
 #define netwib__uint64_init_32(a,b,x) x = (((netwib_uint64)((netwib_uint32)(a))<<32)|(netwib_uint64)((netwib_uint32)(b)))
#else
 #define netwib__uint64_init_8(a,b,c,d,e,f,g,h,x) (x).high = netwib_c2_uint32_4(a,b,c,d); (x).low = netwib_c2_uint32_4(e,f,g,h)
 #define netwib__uint64_init_32(a,b,x) {(x).high = (a); (x).low = (b);}
#endif


/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
 #define netwib__uint64_init_uint32(a,x) x = ((netwib_uint64)(a))
#else
 #define netwib__uint64_init_uint32(a,x) netwib__uint64_init_32(0,a,x)
#endif

/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
 #define netwib__uint64_init_uintmax(a,x) x = ((netwib_uint64)(a))
#else
 #if NETWIB_INTMAX_BITS == 64
  #define netwib__uint64_init_uintmax(a,x) netwib__uint64_init_32((netwib_uint32)((a)>>32),(netwib_uint32)((a)&0xFFFFFFFFu),x)
 #else
  #define netwib__uint64_init_uintmax(a,x) netwib__uint64_init_uint32(a,x)
 #endif
#endif

/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
 #define netwib__uint64_init_uintptr(a,x) x = ((netwib_uint64)(a))
#else
 #if NETWIB_INTPTR_BITS == 64
  #define netwib__uint64_init_uintptr(a,x) netwib__uint64_init_32((netwib_uint32)((a)>>32),(netwib_uint32)((a)&0xFFFFFFFFu),x)
 #else
  #define netwib__uint64_init_uintptr(a,x) netwib__uint64_init_uint32(a,x)
 #endif
#endif


/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
 #define netwib__int64_init_int32(a,x) x = ((netwib_int64)(netwib_int32)(a))
#else
 #define netwib__int64_init_int32(a,x) { if ((netwib_int32)(a) >= 0) { (x).high = 0; (x).low = (netwib_uint32)(a); } else { (x).high = 0xFFFFFFFFu; (x).low = (netwib_uint32)(netwib_int32)(a); } }
#endif

/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
 #define netwib__int64_init_intmax(a,x) x = ((netwib_int64)(netwib_intmax)(a))
#else
 #if NETWIB_INTMAX_BITS == 64
  #define netwib__int64_init_intmax(a,x) netwib__uint64_init_32((netwib_uint32)((a)>>32),(netwib_uint32)((a)&0xFFFFFFFFu),x)
 #else
  #define netwib__int64_init_intmax(a,x) netwib__int64_init_int32(a,x)
 #endif
#endif

/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
 #define netwib__int64_init_intptr(a,x) x = ((netwib_int64)(netwib_intptr)(a))
#else
 #if NETWIB_INTPTR_BITS == 64
  #define netwib__int64_init_intptr(a,x) netwib__uint64_init_32((netwib_uint32)((a)>>32),(netwib_uint32)((a)&0xFFFFFFFFu),x)
 #else
  #define netwib__int64_init_intptr(a,x) netwib__int64_init_int32(a,x)
 #endif
#endif


/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
 #define netwib__uint32_init_uint64(a,x) x = ((netwib_uint32)(a))
#else
 #define netwib__uint32_init_uint64(a,x) x = ((a).low)
#endif

/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
 #define netwib__uintmax_init_uint64(a,x) x = ((netwib_uintmax)(a))
#else
 #if NETWIB_INTMAX_BITS == 64
  #define netwib__uintmax_init_uint64(a,x) x = (((netwib_uintmax)((netwib_uint32)((a).high))<<32)|(netwib_uint64)((netwib_uint32)((a).low)))
 #else
  #define netwib__uintmax_init_uint64(a,x) x = ((netwib_uintmax)(a).low)
 #endif
#endif

/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
 #define netwib__uintptr_init_uint64(a,x) x = ((netwib_uintptr)(a))
#else
 #if NETWIB_INTPTR_BITS == 64
  #define netwib__uintptr_init_uint64(a,x) x = (((netwib_uintptr)((netwib_uint32)((a).high))<<32)|(netwib_uintptr)((netwib_uint32)((a).low)))
 #else
  #define netwib__uintptr_init_uint64(a,x) x = ((netwib_uintptr)(a).low)
 #endif
#endif


/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
 #define netwib__int32_init_int64(a,x) x = ((netwib_int32)(a))
#else
 #define netwib__int32_init_int64(a,x) x = ((netwib_int32)(a).low)
#endif

/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
 #define netwib__intmax_init_int64(a,x) x = ((netwib_intmax)(a))
#else
 #if NETWIB_INTMAX_BITS == 64
  #define netwib__intmax_init_int64(a,x) x = (((netwib_intmax)((netwib_uint32)((a).high))<<32)|(netwib_uintmax)((netwib_uint32)((a).low)))
 #else
  #define netwib__intmax_init_int64(a,x) x = ((netwib_intmax)(a).low)
 #endif
#endif

/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
 #define netwib__intptr_init_int64(a,x) x = ((netwib_intptr)(a))
#else
 #if NETWIB_INTPTR_BITS == 64
  #define netwib__intptr_init_int64(a,x) x = (((netwib_intptr)((netwib_uint32)((a).high))<<32)|(netwib_uintptr)((netwib_uint32)((a).low)))
 #else
  #define netwib__intptr_init_int64(a,x) x = ((netwib_intptr)(a).low)
 #endif
#endif


/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
  #define netwib__uint64_add(a,b,x) x = (netwib_uint64)(a) + (netwib_uint64)(b);
#else
  #define netwib__uint64_add(a,b,x) { netwib_uint32 netwib__uint64_add_tmp, netwib__uint64_add_carry; netwib__uint64_add_tmp = (a).low + (b).low; netwib__uint64_add_carry = (netwib__uint64_add_tmp < (a).low) ? 1 : 0; (x).low = netwib__uint64_add_tmp; (x).high = (a).high + (b).high + netwib__uint64_add_carry; }
#endif
#define netwib__int64_add(a,b,x) netwib__uint64_add(a,b,x)

/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
  #define netwib__uint64_sub(a,b,x) x = (netwib_uint64)(a) - (netwib_uint64)(b);
#else
  #define netwib__uint64_sub(a,b,x) { netwib_uint32 netwib__uint64_add_tmp, netwib__uint64_add_carry; netwib__uint64_add_tmp = (a).low - (b).low; netwib__uint64_add_carry = (netwib__uint64_add_tmp > (a).low) ? 1 : 0; (x).low = netwib__uint64_add_tmp; (x).high = (a).high - (b).high - netwib__uint64_add_carry; }
#endif
#define netwib__int64_sub(a,b,x) netwib__uint64_sub(a,b,x)

/*-------------------------------------------------------------*/
void netwib__uint64_mul_private(netwib_uint64 a,
                                netwib_uint64 b,
                                netwib_uint64 *px);
#if NETWIB_INT64_FAKE == 0
  #define netwib__uint64_mul(a,b,px) *(px) = (netwib_uint64)(a) * (netwib_uint64)(b)
#else
  #define netwib__uint64_mul(a,b,px) netwib__uint64_mul_private(a,b,px)
#endif
#define netwib__int64_mul(a,b,px) netwib__uint64_mul(a,b,px)

/*-------------------------------------------------------------*/
void netwib__uint64_div_private(netwib_uint64 a,
                                netwib_uint64 b,
                                netwib_uint64 *pq,
                                netwib_uint64 *pr);
void netwib__int64_div_private(netwib_int64 a,
                               netwib_int64 b,
                               netwib_int64 *pq,
                               netwib_int64 *pr);
#if NETWIB_INT64_FAKE == 0
  #define netwib__uint64_div(a,b,pq,pr) { netwib_uint64 netwib__uint64_div_tmp; netwib__uint64_div_tmp = (netwib_uint64)(a) / (netwib_uint64)(b); *(pr) = (netwib_uint64)(a) % (netwib_uint64)(b); *(pq) = netwib__uint64_div_tmp; }
  #define netwib__int64_div(a,b,pq,pr) { netwib_int64 netwib__int64_div_tmp; netwib__int64_div_tmp = (netwib_int64)(a) / (netwib_int64)(b); *(pr) = (netwib_int64)(a) % (netwib_int64)(b); *(pq) = netwib__int64_div_tmp; }
#else
  #define netwib__uint64_div(a,b,pq,pr) netwib__uint64_div_private(a,b,pq,pr)
  #define netwib__int64_div(a,b,pq,pr) netwib__int64_div_private(a,b,pq,pr)
#endif


/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
  #define netwib__uint64_inc(x) x = (netwib_uint64)(x) + 1;
#else
  #define netwib__uint64_inc(x) { if ((x).low == 0xFFFFFFFFu) { (x).high++; (x).low = 0; } else { (x).low++; } }
#endif
#define netwib__int64_inc(x) netwib__uint64_inc(x)

/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
  #define netwib__uint64_dec(x) x = (netwib_uint64)(x) - 1;
#else
  #define netwib__uint64_dec(x) { if ((x).low == 0) { (x).high--; (x).low = 0xFFFFFFFFu; } else { (x).low--; } }
#endif
#define netwib__int64_dec(x) netwib__uint64_dec(x)

/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
  #define netwib__uint64_cmp_eq(a,b) ((netwib_uint64)(a) == (netwib_uint64)(b))
  #define netwib__uint64_cmp_ne(a,b) ((netwib_uint64)(a) != (netwib_uint64)(b))
  #define netwib__uint64_cmp_lt(a,b) ((netwib_uint64)(a) < (netwib_uint64)(b))
  #define netwib__uint64_cmp_le(a,b) ((netwib_uint64)(a) <= (netwib_uint64)(b))
  #define netwib__uint64_cmp_gt(a,b) ((netwib_uint64)(a) > (netwib_uint64)(b))
  #define netwib__uint64_cmp_ge(a,b) ((netwib_uint64)(a) >= (netwib_uint64)(b))
  #define netwib__int64_cmp_eq(a,b) ((netwib_int64)(a) == (netwib_int64)(b))
  #define netwib__int64_cmp_ne(a,b) ((netwib_int64)(a) != (netwib_int64)(b))
  #define netwib__int64_cmp_lt(a,b) ((netwib_int64)(a) < (netwib_int64)(b))
  #define netwib__int64_cmp_le(a,b) ((netwib_int64)(a) <= (netwib_int64)(b))
  #define netwib__int64_cmp_gt(a,b) ((netwib_int64)(a) > (netwib_int64)(b))
  #define netwib__int64_cmp_ge(a,b) ((netwib_int64)(a) >= (netwib_int64)(b))
#else
  #define netwib__uint64_cmp_eq(a,b) ( ((a).high == (b).high) && ((a).low == (b).low) )
  #define netwib__uint64_cmp_ne(a,b) ( ((a).high != (b).high) || ((a).low != (b).low) )
  #define netwib__uint64_cmp_lt(a,b) ( ((a).high < (b).high) || ( ((a).high == (b).high) && ((a).low < (b).low) ) )
  #define netwib__uint64_cmp_le(a,b) ( ((a).high < (b).high) || ( ((a).high == (b).high) && ((a).low <= (b).low) ) )
  #define netwib__uint64_cmp_gt(a,b) ( ((a).high > (b).high) || ( ((a).high == (b).high) && ((a).low > (b).low) ) )
  #define netwib__uint64_cmp_ge(a,b) ( ((a).high > (b).high) || ( ((a).high == (b).high) && ((a).low >= (b).low) ) )
  #define netwib__int64_cmp_eq(a,b) ( ((a).high == (b).high) && ((a).low == (b).low) )
  #define netwib__int64_cmp_ne(a,b) ( ((a).high != (b).high) || ((a).low != (b).low) )
  #define netwib__int64_cmp_lt(a,b) ( ((netwib_int32)(a).high < (netwib_int32)(b).high) || ( ((netwib_int32)(a).high == (netwib_int32)(b).high) && ((netwib_int32)(a).low < (netwib_int32)(b).low) ) )
  #define netwib__int64_cmp_le(a,b) ( ((netwib_int32)(a).high < (netwib_int32)(b).high) || ( ((netwib_int32)(a).high == (netwib_int32)(b).high) && ((netwib_int32)(a).low <= (netwib_int32)(b).low) ) )
  #define netwib__int64_cmp_gt(a,b) ( ((netwib_int32)(a).high > (netwib_int32)(b).high) || ( ((netwib_int32)(a).high == (netwib_int32)(b).high) && ((netwib_int32)(a).low > (netwib_int32)(b).low) ) )
  #define netwib__int64_cmp_ge(a,b) ( ((netwib_int32)(a).high > (netwib_int32)(b).high) || ( ((netwib_int32)(a).high == (netwib_int32)(b).high) && ((netwib_int32)(a).low >= (netwib_int32)(b).low) ) )
#endif


/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
  #define netwib__int64_neg(a,x) x = -(netwib_int64)(a);
#else
  #define netwib__int64_neg(a,x) { (x).high = ~(a).high; (x).low = ~(a).low; netwib__uint64_inc(x); }
#endif


/*-------------------------------------------------------------*/
/* For shl, shr, rol and rol, I encountered several problems with gcc
   on 32 bit architectures:
    - x << 64 = zero with "gcc -O"
    - x << 64 = unchanged with "gcc -O2"
   There are also problems under Solaris Sparc, with rol(0).
   I decided to normalize to have a coherent behavior.
*/
#if NETWIB_INT64_FAKE == 0
  #define netwib__uint64_shl(a,n,x) { netwib_uint32 netwib__uint64_shl_n = (n); if (netwib__uint64_shl_n == 0) { (x) = (a); } else if (netwib__uint64_shl_n >= 64) { (x) = 0; } else {(x) = (a) << netwib__uint64_shl_n;} }
#else
  #define netwib__uint64_shl(a,n,x) { netwib_uint32 netwib__uint64_shl_tmp, netwib__uint64_shl_n = n; if (netwib__uint64_shl_n == 0) { (x).high = (a).high; (x).low = (a).low; } else if (netwib__uint64_shl_n >= 64) { (x).high = 0; (x).low = 0; } else if (netwib__uint64_shl_n < 32) { netwib__uint64_shl_tmp = (a).high << netwib__uint64_shl_n | ((a).low >> (32 - netwib__uint64_shl_n)); (x).low = (a).low << netwib__uint64_shl_n; (x).high = netwib__uint64_shl_tmp; } else { netwib__uint64_shl_n -= 32; (x).high = (a).low << netwib__uint64_shl_n ; (x).low = 0; } }
#endif

/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
  #define netwib__uint64_shr(a,n,x) { netwib_uint32 netwib__uint64_shr_n = (n);  if (netwib__uint64_shr_n == 0) { (x) = (a); } else if (netwib__uint64_shr_n >= 64) { (x) = 0; } else { x = (a) >> netwib__uint64_shr_n; } }
#else
  #define netwib__uint64_shr(a,n,x) { netwib_uint32 netwib__uint64_shr_tmp, netwib__uint64_shr_n = n; if (netwib__uint64_shr_n == 0) { (x).high = (a).high; (x).low = (a).low; } else if (netwib__uint64_shr_n >= 64) { (x).high = 0; (x).low = 0; } else if (netwib__uint64_shr_n < 32) { netwib__uint64_shr_tmp = (a).low >> netwib__uint64_shr_n | ((a).high << (32 - netwib__uint64_shr_n)); (x).high = (a).high >> netwib__uint64_shr_n; (x).low = netwib__uint64_shr_tmp; } else { netwib__uint64_shr_n -= 32; (x).low = (a).high >> netwib__uint64_shr_n; (x).high = 0; } }
#endif

/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
  #define netwib__uint64_rol(a,n,x) { netwib_uint32 netwib__uint64_rol_n = (n)%64; if (netwib__uint64_rol_n == 0) { x = (a); } else { x = ((netwib_uint64)( (netwib_uint64)((a)<<netwib__uint64_rol_n) | (netwib_uint64)((a)>>(64-netwib__uint64_rol_n)) )); } }
#else
  #define netwib__uint64_rol(a,n,x) { netwib_uint32 netwib__uint64_rol_tmp, netwib__uint64_rol_n = (n)%64; if (netwib__uint64_rol_n == 0) { (x).high = (a).high; (x).low = (a).low; } else if (netwib__uint64_rol_n < 32) { netwib__uint64_rol_tmp = (a).high << netwib__uint64_rol_n | ((a).low >> (32 - netwib__uint64_rol_n)); (x).low = (a).low << netwib__uint64_rol_n | ((a).high >> (32 - netwib__uint64_rol_n)); (x).high = netwib__uint64_rol_tmp; } else { netwib__uint64_rol_n -= 32; netwib__uint64_rol_tmp = (a).low << netwib__uint64_rol_n | ((a).high >> (32 - netwib__uint64_rol_n)); (x).low = (a).high << netwib__uint64_rol_n | ((a).low >> (32 - netwib__uint64_rol_n)); (x).high = netwib__uint64_rol_tmp; } }
#endif

/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
  #define netwib__uint64_ror(a,n,x) { netwib_uint32 netwib__uint64_ror_n = (n)%64; if (netwib__uint64_ror_n == 0) { x = (a); } else { x = ((netwib_uint64)( (netwib_uint64)((a)>>netwib__uint64_ror_n) | (netwib_uint64)((a)<<(64-netwib__uint64_ror_n)) )); } }
#else
  #define netwib__uint64_ror(a,n,x) { netwib_uint32 netwib__uint64_ror_tmp, netwib__uint64_ror_n = (n)%64; if (netwib__uint64_ror_n == 0) { (x).high = (a).high; (x).low = (a).low; } else if (netwib__uint64_ror_n < 32) { netwib__uint64_ror_tmp = (a).low >> netwib__uint64_ror_n | ((a).high << (32 - netwib__uint64_ror_n)); (x).high = (a).high >> netwib__uint64_ror_n | ((a).low << (32 - netwib__uint64_ror_n)); (x).low = netwib__uint64_ror_tmp; } else { netwib__uint64_ror_n -= 32; netwib__uint64_ror_tmp = (a).high >> netwib__uint64_ror_n | ((a).low << (32 - netwib__uint64_ror_n)); (x).high = (a).low >> netwib__uint64_ror_n | ((a).high << (32 - netwib__uint64_ror_n)); (x).low = netwib__uint64_ror_tmp;} }
#endif


/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
  #define netwib__uint64_and(a,b,x) x = (netwib_uint64)(a) & (netwib_uint64)(b);
#else
  #define netwib__uint64_and(a,b,x) { (x).high = (a).high & (b).high; (x).low = (a).low & (b).low; }
#endif

/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
  #define netwib__uint64_or(a,b,x) x = (netwib_uint64)(a) | (netwib_uint64)(b);
#else
  #define netwib__uint64_or(a,b,x) { (x).high = (a).high | (b).high; (x).low = (a).low | (b).low; }
#endif

/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
  #define netwib__uint64_xor(a,b,x) x = (netwib_uint64)(a) ^ (netwib_uint64)(b);
#else
  #define netwib__uint64_xor(a,b,x) { (x).high = (a).high ^ (b).high; (x).low = (a).low ^ (b).low; }
#endif

/*-------------------------------------------------------------*/
#if NETWIB_INT64_FAKE == 0
  #define netwib__uint64_not(a,x) x = ~(netwib_uint64)(a);
#else
  #define netwib__uint64_not(a,x) { (x).high = ~(a).high; (x).low = ~(a).low; }
#endif


/*-------------------------------------------------------------*/
/* To represent values > 0xFFFFFFFF :
    - gcc/mingw needs 0xf....fLL
    - Microsoft Visual 6 does not support LL
 */
#if defined NETWIBDEF_PROGCC_VISUALC
  #define NETWIB_UINT_LL(x) (x)
  #define NETWIB_INT_LL(x) (x)
#else
  #define NETWIB_UINT_LL(x) (x ## LLU)
  #define NETWIB_INT_LL(x) (x ## LL)
#endif

/*-------------------------------------------------------------*/
/* To initialize a static variable */
#if NETWIB_INT64_FAKE == 0
  #define NETWIB_UINT_STATIC(ahigh,alow) (((netwib_uint64)((netwib_uint32)(ahigh))<<32)|(netwib_uint64)((netwib_uint32)(alow)))
  #define NETWIB_INT_STATIC(ahigh,alow) (((netwib_int64)((netwib_uint32)(ahigh))<<32)|(netwib_uint64)((netwib_uint32)(alow)))
#else
  #define NETWIB_UINT_STATIC(ahigh,alow) {(ahigh), (alow)}
  #define NETWIB_INT_STATIC(ahigh,alow) {(ahigh), (alow)}
#endif
