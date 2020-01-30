#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err test__uint64_init_832(void)
{
  netwib_uint64 ui64;
  netwib_uint64 array[1] = {NETWIB_UINT_STATIC(1, 2)};
  char a;
  int b;

  puts("++++----\nTest of uint64_init_8|32");

  netwib_er(netwibtest_uint64_content(array[0], "0000000100000002"));

  a = (char)(unsigned char)0xDA;
  b = (int)0xDCBADCBA;
  netwib__uint64_init_8(a,a,a,a,a,a,a,a,ui64);
  netwib_er(netwibtest_uint64_content(ui64, "DADADADADADADADA"));
  netwib__uint64_init_32(b,b,ui64);
  netwib_er(netwibtest_uint64_content(ui64, "DCBADCBADCBADCBA"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__uint64_init_uint32(void)
{
  netwib_uint64 ui64;

  puts("++++----\nTest of uint64_init_uint32");

  netwib__uint64_init_uint32(0, ui64);
  netwib_er(netwibtest_uint64_content(ui64, "0000000000000000"));
  netwib__uint64_init_uint32(0xFFFFFFFFu, ui64);
  netwib_er(netwibtest_uint64_content(ui64, "00000000FFFFFFFF"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__uint64_init_uintmax(void)
{
  netwib_uint64 ui64;

  puts("++++----\nTest of uint64_init_uintmax");

  netwib__uint64_init_uintmax(0, ui64);
  netwib_er(netwibtest_uint64_content(ui64, "0000000000000000"));
  netwib__uint64_init_uintmax(0xFFFFFFFFu, ui64);
  netwib_er(netwibtest_uint64_content(ui64, "00000000FFFFFFFF"));
#if NETWIB_INTMAX_BITS == 64
  netwib__uint64_init_uintmax(NETWIB_UINT_LL(0xFFFFFFFFFFFFFFFF), ui64);
  netwib_er(netwibtest_uint64_content(ui64, "FFFFFFFFFFFFFFFF"));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__uint64_init_uintptr(void)
{
  netwib_uint64 ui64;

  puts("++++----\nTest of uint64_init_uintptr");

  netwib__uint64_init_uintptr(0, ui64);
  netwib_er(netwibtest_uint64_content(ui64, "0000000000000000"));
  netwib__uint64_init_uintptr(0xFFFFFFFFu, ui64);
  netwib_er(netwibtest_uint64_content(ui64, "00000000FFFFFFFF"));
#if NETWIB_INTPTR_BITS == 64
  netwib__uint64_init_uintptr(NETWIB_UINT_LL(0xFFFFFFFFFFFFFFFF), ui64);
  netwib_er(netwibtest_uint64_content(ui64, "FFFFFFFFFFFFFFFF"));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__int64_init_int32(void)
{
  netwib_int64 i64;

  puts("++++----\nTest of int64_init_int32");

  netwib__int64_init_int32(0, i64);
  netwib_er(netwibtest_int64_content(i64, "0000000000000000"));
  netwib__int64_init_int32(0xFFFFFFFFu, i64);
  netwib_er(netwibtest_int64_content(i64, "FFFFFFFFFFFFFFFF"));
  netwib__int64_init_int32(-3, i64);
  netwib_er(netwibtest_int64_content(i64, "FFFFFFFFFFFFFFFD"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__int64_init_intmax(void)
{
  netwib_int64 i64;

  puts("++++----\nTest of int64_init_intmax");

  netwib__int64_init_intmax(0, i64);
  netwib_er(netwibtest_int64_content(i64, "0000000000000000"));
  netwib__int64_init_intmax(0x7FFFFFFF, i64);
  netwib_er(netwibtest_int64_content(i64, "000000007FFFFFFF"));
  netwib__int64_init_intmax(-3, i64);
  netwib_er(netwibtest_int64_content(i64, "FFFFFFFFFFFFFFFD"));
#if NETWIB_INTMAX_BITS == 64
  netwib__int64_init_intmax(NETWIB_UINT_LL(0xFFFFFFFFFF), i64);
  netwib_er(netwibtest_int64_content(i64, "000000FFFFFFFFFF"));
#endif
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__int64_init_intptr(void)
{
  netwib_int64 i64;

  puts("++++----\nTest of int64_init_intptr");

  netwib__int64_init_intptr(0, i64);
  netwib_er(netwibtest_int64_content(i64, "0000000000000000"));
  netwib__int64_init_intptr(0x7FFFFFFF, i64);
  netwib_er(netwibtest_int64_content(i64, "000000007FFFFFFF"));
  netwib__int64_init_intptr(-3, i64);
  netwib_er(netwibtest_int64_content(i64, "FFFFFFFFFFFFFFFD"));
#if NETWIB_INTPTR_BITS == 64
  netwib__int64_init_intptr(NETWIB_UINT_LL(0xFFFFFFFFFF), i64);
  netwib_er(netwibtest_int64_content(i64, "000000FFFFFFFFFF"));
#endif
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__uint32_init_uint64(void)
{
  netwib_uint64 ui64;
  netwib_uint32 ui32;

  puts("++++----\nTest of uint32_init_uint64");

  netwib_er(netwibtest_uint64_init("0000000000000000", &ui64));
  netwib__uint32_init_uint64(ui64, ui32);
  if (ui32 != 0) {
    return(NETWIBTEST_ERR_1);
  }
  netwib_er(netwibtest_uint64_init("00000000FFFFFFFF", &ui64));
  netwib__uint32_init_uint64(ui64, ui32);
  if (ui32 != 0xFFFFFFFFu) {
    return(NETWIBTEST_ERR_1);
  }
  netwib_er(netwibtest_uint64_init("00010000FFFFFFFF", &ui64));
  netwib__uint32_init_uint64(ui64, ui32);
  if (ui32 != 0xFFFFFFFFu) {
    return(NETWIBTEST_ERR_1);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__uintmax_init_uint64(void)
{
  netwib_uint64 ui64;
  netwib_uintmax uimax;

  puts("++++----\nTest of uintmax_init_uint64");

  netwib_er(netwibtest_uint64_init("0000000000000000", &ui64));
  netwib__uintmax_init_uint64(ui64, uimax);
  if (uimax != 0) {
    return(NETWIBTEST_ERR_1);
  }
  netwib_er(netwibtest_uint64_init("00000000FFFFFFFF", &ui64));
  netwib__uintmax_init_uint64(ui64, uimax);
  if (uimax != 0xFFFFFFFFu) {
    return(NETWIBTEST_ERR_1);
  }
  netwib_er(netwibtest_uint64_init("00010000FFFFFFFF", &ui64));
  netwib__uintmax_init_uint64(ui64, uimax);
#if NETWIB_INTMAX_BITS == 64
  if (uimax != NETWIB_UINT_LL(0x10000FFFFFFFF)) {
    return(NETWIBTEST_ERR_1);
  }
#else
  if (uimax != 0xFFFFFFFFu) {
    return(NETWIBTEST_ERR_1);
  }
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__uintptr_init_uint64(void)
{
  netwib_uint64 ui64;
  netwib_uintptr uiptr;

  puts("++++----\nTest of uintptr_init_uint64");

  netwib_er(netwibtest_uint64_init("0000000000000000", &ui64));
  netwib__uintptr_init_uint64(ui64, uiptr);
  if (uiptr != 0) {
    return(NETWIBTEST_ERR_1);
  }
  netwib_er(netwibtest_uint64_init("00000000FFFFFFFF", &ui64));
  netwib__uintptr_init_uint64(ui64, uiptr);
  if (uiptr != 0xFFFFFFFFu) {
    return(NETWIBTEST_ERR_1);
  }
  netwib_er(netwibtest_uint64_init("00010000FFFFFFFF", &ui64));
  netwib__uintptr_init_uint64(ui64, uiptr);
#if NETWIB_INTPTR_BITS == 64
  if (uiptr != NETWIB_UINT_LL(0x10000FFFFFFFF)) {
    return(NETWIBTEST_ERR_1);
  }
#else
  if (uiptr != 0xFFFFFFFFu) {
    return(NETWIBTEST_ERR_1);
  }
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__int32_init_int64(void)
{
  netwib_int64 i64;
  netwib_int32 i32;

  puts("++++----\nTest of int32_init_int64");

  netwib_er(netwibtest_int64_init("0000000000000000", &i64));
  netwib__int32_init_int64(i64, i32);
  if (i32 != 0) {
    return(NETWIBTEST_ERR_1);
  }
  netwib_er(netwibtest_int64_init("000000007FFFFFFF", &i64));
  netwib__int32_init_int64(i64, i32);
  if (i32 != 0x7FFFFFFF) {
    return(NETWIBTEST_ERR_1);
  }
  netwib_er(netwibtest_int64_init("FFFFFFFFFFFFFFFF", &i64));
  netwib__int32_init_int64(i64, i32);
  if (i32 != -1) {
    return(NETWIBTEST_ERR_1);
  }
  netwib_er(netwibtest_int64_init("FFFFFFFF80000000", &i64));
  netwib__int32_init_int64(i64, i32);
  if (i32 != -0x7FFFFFFF-1) {
    return(NETWIBTEST_ERR_1);
  }

  netwib_er(netwibtest_int64_init("00010000FFFFFFFF", &i64));
  netwib__int32_init_int64(i64, i32);
  if ((netwib_uint32)i32 != 0xFFFFFFFFu) {
    return(NETWIBTEST_ERR_1);
  }
  netwib_er(netwibtest_int64_init("80010000FFFFFFFF", &i64));
  netwib__int32_init_int64(i64, i32);
  if ((netwib_uint32)i32 != 0xFFFFFFFFu) {
    return(NETWIBTEST_ERR_1);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__intmax_init_int64(void)
{
  netwib_int64 i64;
  netwib_intmax imax;

  puts("++++----\nTest of intmax_init_int64");

  netwib_er(netwibtest_int64_init("0000000000000000", &i64));
  netwib__intmax_init_int64(i64, imax);
  if (imax != 0) {
    return(NETWIBTEST_ERR_1);
  }
  netwib_er(netwibtest_int64_init("000000007FFFFFFF", &i64));
  netwib__intmax_init_int64(i64, imax);
  if (imax != 0x7FFFFFFF) {
    return(NETWIBTEST_ERR_1);
  }
  netwib_er(netwibtest_int64_init("FFFFFFFFFFFFFFFF", &i64));
  netwib__intmax_init_int64(i64, imax);
  if (imax != -1) {
    return(NETWIBTEST_ERR_1);
  }
  netwib_er(netwibtest_int64_init("FFFFFFFF80000000", &i64));
  netwib__intmax_init_int64(i64, imax);
  if (imax != -0x7FFFFFFF-1) {
    return(NETWIBTEST_ERR_1);
  }
  netwib_er(netwibtest_int64_init("00010000FFFFFFFF", &i64));
  netwib__intmax_init_int64(i64, imax);
#if NETWIB_INTMAX_BITS == 64
  if (imax != NETWIB_INT_LL(0x10000FFFFFFFF)) {
    return(NETWIBTEST_ERR_1);
  }
#else
  if ((netwib_uint32)imax != 0xFFFFFFFFu) {
    return(NETWIBTEST_ERR_1);
  }
#endif
  netwib_er(netwibtest_int64_init("80010000FFFFFFFF", &i64));
  netwib__intmax_init_int64(i64, imax);
#if NETWIB_INTMAX_BITS == 64
  if (imax != (netwib_intmax)NETWIB_INT_LL(0x80010000FFFFFFFF)) {
    return(NETWIBTEST_ERR_1);
  }
#else
  if ((netwib_uint32)imax != 0xFFFFFFFFu) {
    return(NETWIBTEST_ERR_1);
  }
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__intptr_init_int64(void)
{
  netwib_int64 i64;
  netwib_intptr iptr;

  puts("++++----\nTest of intptr_init_int64");

  netwib_er(netwibtest_int64_init("0000000000000000", &i64));
  netwib__intptr_init_int64(i64, iptr);
  if (iptr != 0) {
    return(NETWIBTEST_ERR_1);
  }
  netwib_er(netwibtest_int64_init("000000007FFFFFFF", &i64));
  netwib__intptr_init_int64(i64, iptr);
  if (iptr != 0x7FFFFFFF) {
    return(NETWIBTEST_ERR_1);
  }
  netwib_er(netwibtest_int64_init("FFFFFFFFFFFFFFFF", &i64));
  netwib__intptr_init_int64(i64, iptr);
  if (iptr != -1) {
    return(NETWIBTEST_ERR_1);
  }
  netwib_er(netwibtest_int64_init("FFFFFFFF80000000", &i64));
  netwib__intptr_init_int64(i64, iptr);
  if (iptr != -0x7FFFFFFF-1) {
    return(NETWIBTEST_ERR_1);
  }
  netwib_er(netwibtest_int64_init("00010000FFFFFFFF", &i64));
  netwib__intptr_init_int64(i64, iptr);
#if NETWIB_INTPTR_BITS == 64
  if (iptr != NETWIB_INT_LL(0x10000FFFFFFFF)) {
    return(NETWIBTEST_ERR_1);
  }
#else
  if ((netwib_uint32)iptr != 0xFFFFFFFFu) {
    return(NETWIBTEST_ERR_1);
  }
#endif
  netwib_er(netwibtest_int64_init("80010000FFFFFFFF", &i64));
  netwib__intptr_init_int64(i64, iptr);
#if NETWIB_INTPTR_BITS == 64
  if (iptr != (netwib_intptr)NETWIB_INT_LL(0x80010000FFFFFFFF)) {
    return(NETWIBTEST_ERR_1);
  }
#else
  if ((netwib_uint32)iptr != 0xFFFFFFFFu) {
    return(NETWIBTEST_ERR_1);
  }
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__netwib_uint64_add(void)
{
  netwib_uint64 ui64a, ui64b, ui64c;

  puts("++++----\nTest of uint64_add");

  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib_er(netwibtest_uint64_init("1111111111111111", &ui64b));
  netwib__uint64_add(ui64a, ui64b, ui64c);
  netwib_er(netwibtest_uint64_content(ui64c, "2345678923456789"));

  netwib_er(netwibtest_uint64_init("1234567882345678", &ui64a));
  netwib_er(netwibtest_uint64_init("11111111A1111111", &ui64b));
  netwib__uint64_add(ui64a, ui64b, ui64c);
  netwib_er(netwibtest_uint64_content(ui64c, "2345678A23456789"));

  netwib_er(netwibtest_uint64_init("A234567882345678", &ui64a));
  netwib_er(netwibtest_uint64_init("B1111111A1111111", &ui64b));
  netwib__uint64_add(ui64a, ui64b, ui64c);
  netwib_er(netwibtest_uint64_content(ui64c, "5345678A23456789"));

  netwib_er(netwibtest_uint64_init("FFFFFFFFFFFFFFFF", &ui64a));
  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64b));
  netwib__uint64_add(ui64a, ui64b, ui64c);
  netwib_er(netwibtest_uint64_content(ui64c, "1234567812345677"));

  netwib_er(netwibtest_uint64_init("FFFFFFFFFFFFFFFF", &ui64a));
  netwib_er(netwibtest_uint64_init("FFFFFFFFFFFFFFFF", &ui64b));
  netwib__uint64_add(ui64a, ui64b, ui64c);
  netwib_er(netwibtest_uint64_content(ui64c, "FFFFFFFFFFFFFFFE"));

  /* use the same variable for output */
  netwib_er(netwibtest_uint64_init("1234567882345678", &ui64a));
  netwib__uint64_add(ui64a, ui64a, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "2468ACF10468ACF0"));
  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib__uint64_add(ui64a, ui64a, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "2468ACF02468ACF0"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__netwib_uint64_sub(void)
{
  netwib_uint64 ui64a, ui64b, ui64c;

  puts("++++----\nTest of uint64_sub");

  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib_er(netwibtest_uint64_init("1111111111111111", &ui64b));
  netwib__uint64_sub(ui64a, ui64b, ui64c);
  netwib_er(netwibtest_uint64_content(ui64c, "0123456701234567"));

  netwib_er(netwibtest_uint64_init("1234567882345678", &ui64a));
  netwib_er(netwibtest_uint64_init("11111111A1111111", &ui64b));
  netwib__uint64_sub(ui64a, ui64b, ui64c);
  netwib_er(netwibtest_uint64_content(ui64c, "01234566E1234567"));

  netwib_er(netwibtest_uint64_init("A234567882345678", &ui64a));
  netwib_er(netwibtest_uint64_init("B1111111A1111111", &ui64b));
  netwib__uint64_sub(ui64a, ui64b, ui64c);
  netwib_er(netwibtest_uint64_content(ui64c, "F1234566E1234567"));

  netwib_er(netwibtest_uint64_init("FFFFFFFFFFFFFFFF", &ui64a));
  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64b));
  netwib__uint64_sub(ui64a, ui64b, ui64c);
  netwib_er(netwibtest_uint64_content(ui64c, "EDCBA987EDCBA987"));

  netwib_er(netwibtest_uint64_init("FFFFFFFFFFFFFFFF", &ui64a));
  netwib_er(netwibtest_uint64_init("FFFFFFFFFFFFFFFF", &ui64b));
  netwib__uint64_sub(ui64a, ui64b, ui64c);
  netwib_er(netwibtest_uint64_content(ui64c, "0000000000000000"));

  /* use the same variable for output */
  netwib_er(netwibtest_uint64_init("1234567882345678", &ui64a));
  netwib__uint64_sub(ui64a, ui64a, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "0000000000000000"));
  netwib_er(netwibtest_uint64_init("1234567882345678", &ui64a));
  netwib_er(netwibtest_uint64_init("1234567882345679", &ui64b));
  netwib__uint64_sub(ui64a, ui64b, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "FFFFFFFFFFFFFFFF"));
  netwib_er(netwibtest_uint64_init("1234567882345678", &ui64a));
  netwib_er(netwibtest_uint64_init("1234567882345679", &ui64b));
  netwib__uint64_sub(ui64a, ui64b, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "FFFFFFFFFFFFFFFF"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__netwib_uint64_mul(void)
{
  netwib_uint64 ui64a, ui64b, ui64c;

  puts("++++----\nTest of uint64_mul");

  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib_er(netwibtest_uint64_init("0000000000000001", &ui64b));
  netwib__uint64_mul(ui64a, ui64b, &ui64c);
  netwib_er(netwibtest_uint64_content(ui64c, "1234567812345678"));

  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib_er(netwibtest_uint64_init("0000000000000002", &ui64b));
  netwib__uint64_mul(ui64a, ui64b, &ui64c);
  netwib_er(netwibtest_uint64_content(ui64c, "2468ACF02468ACF0"));

  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib_er(netwibtest_uint64_init("1111111111111111", &ui64b));
  netwib__uint64_mul(ui64a, ui64b, &ui64c);
  netwib_er(netwibtest_uint64_content(ui64c, "CB961C5E652FB5F8"));

  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib_er(netwibtest_uint64_init("FFFFFFFFFFFFFFFF", &ui64b));
  netwib__uint64_mul(ui64a, ui64b, &ui64c);
  netwib_er(netwibtest_uint64_content(ui64c, "EDCBA987EDCBA988"));

  netwib_er(netwibtest_uint64_init("FFFFFFFFFFFFFFFE", &ui64a));
  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64b));
  netwib__uint64_mul(ui64a, ui64b, &ui64c);
  netwib_er(netwibtest_uint64_content(ui64c, "DB97530FDB975310"));

  netwib_er(netwibtest_uint64_init("FFFFFFFFFFFFFFFF", &ui64a));
  netwib_er(netwibtest_uint64_init("FFFFFFFFFFFFFFFF", &ui64b));
  netwib__uint64_mul(ui64a, ui64b, &ui64c);
  netwib_er(netwibtest_uint64_content(ui64c, "0000000000000001"));

  /* use the same variable for output */
  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib__uint64_mul(ui64a, ui64a, &ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "3D35175C1DF4D840"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__netwib_uint64_inc(void)
{
  netwib_uint64 ui64a;

  puts("++++----\nTest of uint64_inc");

  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib__uint64_inc(ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "1234567812345679"));

  netwib_er(netwibtest_uint64_init("12345678FFFFFFFF", &ui64a));
  netwib__uint64_inc(ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "1234567900000000"));

  netwib_er(netwibtest_uint64_init("FFFFFFFFFFFFFFFF", &ui64a));
  netwib__uint64_inc(ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "0000000000000000"));

  netwib_er(netwibtest_uint64_init("FFFFFFFFFFFFFFFE", &ui64a));
  netwib__uint64_inc(ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "FFFFFFFFFFFFFFFF"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__netwib_uint64_dec(void)
{
  netwib_uint64 ui64a;

  puts("++++----\nTest of uint64_dec");

  netwib_er(netwibtest_uint64_init("1234567812345679", &ui64a));
  netwib__uint64_dec(ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "1234567812345678"));

  netwib_er(netwibtest_uint64_init("1234567900000000", &ui64a));
  netwib__uint64_dec(ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "12345678FFFFFFFF"));

  netwib_er(netwibtest_uint64_init("0000000000000000", &ui64a));
  netwib__uint64_dec(ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "FFFFFFFFFFFFFFFF"));

  netwib_er(netwibtest_uint64_init("FFFFFFFFFFFFFFFF", &ui64a));
  netwib__uint64_dec(ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "FFFFFFFFFFFFFFFE"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__netwib_uint64_cmp(void)
{
  netwib_uint64 ui64a, ui64b;

  puts("++++----\nTest of uint64_cmp");

  netwib_er(netwibtest_uint64_init("1234567812345679", &ui64a));
  netwib_er(netwibtest_uint64_init("1234567812345679", &ui64b));
  if (!netwib__uint64_cmp_eq(ui64a, ui64b)) {
    return(NETWIBTEST_ERR_3);
  }
  netwib_er(netwibtest_uint64_init("123456781234567A", &ui64b));
  if (!netwib__uint64_cmp_lt(ui64a, ui64b)) {
    return(NETWIBTEST_ERR_3);
  }
  if (!netwib__uint64_cmp_ne(ui64a, ui64b)) {
    return(NETWIBTEST_ERR_3);
  }
  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64b));
  if (!netwib__uint64_cmp_gt(ui64a, ui64b)) {
    return(NETWIBTEST_ERR_3);
  }

  netwib_er(netwibtest_uint64_init("F234567812345679", &ui64a));
  netwib_er(netwibtest_uint64_init("F234567812345679", &ui64b));
  if (!netwib__uint64_cmp_eq(ui64a, ui64b)) {
    return(NETWIBTEST_ERR_3);
  }
  netwib_er(netwibtest_uint64_init("F23456781234567A", &ui64b));
  if (!netwib__uint64_cmp_lt(ui64a, ui64b)) {
    return(NETWIBTEST_ERR_3);
  }
  netwib_er(netwibtest_uint64_init("F234567812345678", &ui64b));
  if (!netwib__uint64_cmp_gt(ui64a, ui64b)) {
    return(NETWIBTEST_ERR_3);
  }

  netwib_er(netwibtest_uint64_init("1234567812345679", &ui64a));
  netwib_er(netwibtest_uint64_init("F234567812345679", &ui64b));
  if (!netwib__uint64_cmp_lt(ui64a, ui64b)) {
    return(NETWIBTEST_ERR_3);
  }

  /* use the same variable for output */
  netwib_er(netwibtest_uint64_init("1234567812345679", &ui64a));
  if (!netwib__uint64_cmp_eq(ui64a, ui64a)) {
    return(NETWIBTEST_ERR_3);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__netwib_int64_cmp(void)
{
  netwib_int64 i64a, i64b;

  puts("++++----\nTest of int64_cmp");

  netwib_er(netwibtest_int64_init("1234567812345679", &i64a));
  netwib_er(netwibtest_int64_init("1234567812345679", &i64b));
  if (!netwib__int64_cmp_eq(i64a, i64b)) {
    return(NETWIBTEST_ERR_3);
  }
  netwib_er(netwibtest_int64_init("123456781234567A", &i64b));
  if (!netwib__int64_cmp_lt(i64a, i64b)) {
    return(NETWIBTEST_ERR_3);
  }
  if (!netwib__int64_cmp_ne(i64a, i64b)) {
    return(NETWIBTEST_ERR_3);
  }
  netwib_er(netwibtest_int64_init("1234567812345678", &i64b));
  if (!netwib__int64_cmp_gt(i64a, i64b)) {
    return(NETWIBTEST_ERR_3);
  }

  netwib_er(netwibtest_int64_init("F234567812345679", &i64a));
  netwib_er(netwibtest_int64_init("F234567812345679", &i64b));
  if (!netwib__int64_cmp_eq(i64a, i64b)) {
    return(NETWIBTEST_ERR_3);
  }
  netwib_er(netwibtest_int64_init("F23456781234567A", &i64b));
  if (!netwib__int64_cmp_lt(i64a, i64b)) {
    return(NETWIBTEST_ERR_3);
  }
  netwib_er(netwibtest_int64_init("F234567812345678", &i64b));
  if (!netwib__int64_cmp_gt(i64a, i64b)) {
    return(NETWIBTEST_ERR_3);
  }

  netwib_er(netwibtest_int64_init("1234567812345679", &i64a));
  netwib_er(netwibtest_int64_init("F234567812345679", &i64b));
  if (!netwib__int64_cmp_gt(i64a, i64b)) {
    return(NETWIBTEST_ERR_3);
  }

  /* use the same variable for output */
  netwib_er(netwibtest_int64_init("1234567812345679", &i64a));
  if (!netwib__int64_cmp_eq(i64a, i64a)) {
    return(NETWIBTEST_ERR_3);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__netwib_int64_neg(void)
{
  netwib_int64 i64a, i64b;

  puts("++++----\nTest of int64_neg");

  netwib_er(netwibtest_int64_init("FFFFFFFFFFFFFFFF", &i64a));
  netwib__int64_neg(i64a, i64b);
  netwib_er(netwibtest_int64_content(i64b, "0000000000000001"));

  netwib_er(netwibtest_int64_init("FFFFFFFFFFFFFFFE", &i64a));
  netwib__int64_neg(i64a, i64b);
  netwib_er(netwibtest_int64_content(i64b, "0000000000000002"));

  netwib_er(netwibtest_int64_init("0000000000000002", &i64a));
  netwib__int64_neg(i64a, i64b);
  netwib_er(netwibtest_int64_content(i64b, "FFFFFFFFFFFFFFFE"));

  /* use the same variable for output */
  netwib_er(netwibtest_int64_init("FFFFFFFFFFFFFFFE", &i64a));
  netwib__int64_neg(i64a, i64a);
  netwib_er(netwibtest_int64_content(i64a, "0000000000000002"));
  netwib_er(netwibtest_int64_init("0000000000000000", &i64a));
  netwib__int64_neg(i64a, i64a);
  netwib_er(netwibtest_int64_content(i64a, "0000000000000000"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__netwib_uint64_shl(void)
{
  netwib_uint64 ui64a, ui64b;

  puts("++++----\nTest of uint64_shl");

  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib__uint64_shl(ui64a, 0, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "1234567812345678"));

  netwib__uint64_shl(ui64a, 1, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "2468ACF02468ACF0"));

  netwib__uint64_shl(ui64a, 2, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "48D159E048D159E0"));

  netwib__uint64_shl(ui64a, 31, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "091A2B3C00000000"));

  netwib__uint64_shl(ui64a, 32, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "1234567800000000"));

  netwib__uint64_shl(ui64a, 33, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "2468ACF000000000"));

  netwib__uint64_shl(ui64a, 4, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "2345678123456780"));

  netwib__uint64_shl(ui64a, 60, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "8000000000000000"));

  netwib__uint64_shl(ui64a, 64, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "0000000000000000"));

  netwib__uint64_shl(ui64a, 70, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "0000000000000000"));

  /* use the same variable for output */
  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib__uint64_shl(ui64a, 0, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "1234567812345678"));
  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib__uint64_shl(ui64a, 31, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "091A2B3C00000000"));
  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib__uint64_shl(ui64a, 33, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "2468ACF000000000"));
  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib__uint64_shl(ui64a, 64, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "0000000000000000"));
  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib__uint64_shl(ui64a, 70, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "0000000000000000"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__netwib_uint64_shr(void)
{
  netwib_uint64 ui64a, ui64b;

  puts("++++----\nTest of uint64_shr");

  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib__uint64_shr(ui64a, 0, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "1234567812345678"));

  netwib__uint64_shr(ui64a, 1, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "091A2B3C091A2B3C"));

  netwib__uint64_shr(ui64a, 4, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "0123456781234567"));

  netwib__uint64_shr(ui64a, 31, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "000000002468ACF0"));

  netwib__uint64_shr(ui64a, 32, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "0000000012345678"));

  netwib__uint64_shr(ui64a, 33, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "00000000091A2B3C"));

  netwib__uint64_shr(ui64a, 60, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "0000000000000001"));

  netwib__uint64_shr(ui64a, 64, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "0000000000000000"));

  netwib__uint64_shr(ui64a, 70, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "0000000000000000"));

  /* use the same variable for output */
  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib__uint64_shr(ui64a, 0, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "1234567812345678"));
  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib__uint64_shr(ui64a, 31, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "000000002468ACF0"));
  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib__uint64_shr(ui64a, 33, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "00000000091A2B3C"));
  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib__uint64_shr(ui64a, 64, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "0000000000000000"));
  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib__uint64_shr(ui64a, 70, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "0000000000000000"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__netwib_uint64_rol(void)
{
  netwib_uint64 ui64a, ui64b;

  puts("++++----\nTest of uint64_rol");

  netwib_er(netwibtest_uint64_init("1234567890ABCDEF", &ui64a));
  netwib__uint64_rol(ui64a, 0, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "1234567890ABCDEF"));

  netwib__uint64_rol(ui64a, 1, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "2468ACF121579BDE"));

  netwib__uint64_rol(ui64a, 2, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "48D159E242AF37BC"));

  netwib__uint64_rol(ui64a, 31, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "4855E6F7891A2B3C"));

  netwib__uint64_rol(ui64a, 32, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "90ABCDEF12345678"));

  netwib__uint64_rol(ui64a, 33, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "21579BDE2468ACF1"));

  netwib__uint64_rol(ui64a, 4, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "234567890ABCDEF1"));

  netwib__uint64_rol(ui64a, 60, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "F1234567890ABCDE"));

  netwib__uint64_rol(ui64a, 64, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "1234567890ABCDEF"));

  netwib__uint64_rol(ui64a, 66, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "48D159E242AF37BC"));

  /* use the same variable for output */
  netwib_er(netwibtest_uint64_init("1234567890ABCDEF", &ui64a));
  netwib__uint64_rol(ui64a, 0, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "1234567890ABCDEF"));
  netwib_er(netwibtest_uint64_init("1234567890ABCDEF", &ui64a));
  netwib__uint64_rol(ui64a, 31, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "4855E6F7891A2B3C"));
  netwib_er(netwibtest_uint64_init("1234567890ABCDEF", &ui64a));
  netwib__uint64_rol(ui64a, 33, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "21579BDE2468ACF1"));
  netwib_er(netwibtest_uint64_init("1234567890ABCDEF", &ui64a));
  netwib__uint64_rol(ui64a, 64, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "1234567890ABCDEF"));
  netwib_er(netwibtest_uint64_init("1234567890ABCDEF", &ui64a));
  netwib__uint64_rol(ui64a, 66, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "48D159E242AF37BC"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__netwib_uint64_ror(void)
{
  netwib_uint64 ui64a, ui64b;

  puts("++++----\nTest of uint64_ror");

  netwib_er(netwibtest_uint64_init("1234567890ABCDEF", &ui64a));
  netwib__uint64_ror(ui64a, 0, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "1234567890ABCDEF"));

  netwib__uint64_ror(ui64a, 1, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "891A2B3C4855E6F7"));

  netwib__uint64_ror(ui64a, 4, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "F1234567890ABCDE"));

  netwib__uint64_ror(ui64a, 31, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "21579BDE2468ACF1"));

  netwib__uint64_ror(ui64a, 32, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "90ABCDEF12345678"));

  netwib__uint64_ror(ui64a, 33, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "4855E6F7891A2B3C"));

  netwib__uint64_ror(ui64a, 60, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "234567890ABCDEF1"));

  netwib__uint64_ror(ui64a, 64, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "1234567890ABCDEF"));

  netwib__uint64_ror(ui64a, 68, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "F1234567890ABCDE"));

  /* use the same variable for output */
  netwib_er(netwibtest_uint64_init("1234567890ABCDEF", &ui64a));
  netwib__uint64_ror(ui64a, 0, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "1234567890ABCDEF"));
  netwib_er(netwibtest_uint64_init("1234567890ABCDEF", &ui64a));
  netwib__uint64_ror(ui64a, 31, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "21579BDE2468ACF1"));
  netwib_er(netwibtest_uint64_init("1234567890ABCDEF", &ui64a));
  netwib__uint64_ror(ui64a, 33, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "4855E6F7891A2B3C"));
  netwib_er(netwibtest_uint64_init("1234567890ABCDEF", &ui64a));
  netwib__uint64_ror(ui64a, 64, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "1234567890ABCDEF"));
  netwib_er(netwibtest_uint64_init("1234567890ABCDEF", &ui64a));
  netwib__uint64_ror(ui64a, 68, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "F1234567890ABCDE"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__netwib_uint64_and(void)
{
  netwib_uint64 ui64a, ui64b, ui64c;

  puts("++++----\nTest of uint64_and");

  netwib_er(netwibtest_uint64_init("1234067812345678", &ui64a));
  netwib_er(netwibtest_uint64_init("120C5678123F5678", &ui64b));
  netwib__uint64_and(ui64a, ui64b, ui64c);
  netwib_er(netwibtest_uint64_content(ui64c, "1204067812345678"));

  /* use the same variable for output */
  netwib_er(netwibtest_uint64_init("1234067812345678", &ui64a));
  netwib__uint64_and(ui64a, ui64a, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "1234067812345678"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__netwib_uint64_or(void)
{
  netwib_uint64 ui64a, ui64b, ui64c;

  puts("++++----\nTest of uint64_or");

  netwib_er(netwibtest_uint64_init("1234067812345678", &ui64a));
  netwib_er(netwibtest_uint64_init("120C5678123F5678", &ui64b));
  netwib__uint64_or(ui64a, ui64b, ui64c);
  netwib_er(netwibtest_uint64_content(ui64c, "123C5678123F5678"));

  /* use the same variable for output */
  netwib_er(netwibtest_uint64_init("1234067812345678", &ui64a));
  netwib__uint64_or(ui64a, ui64a, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "1234067812345678"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__netwib_uint64_xor(void)
{
  netwib_uint64 ui64a, ui64b, ui64c;

  puts("++++----\nTest of uint64_xor");

  netwib_er(netwibtest_uint64_init("1234067812345678", &ui64a));
  netwib_er(netwibtest_uint64_init("120C5678123F5678", &ui64b));
  netwib__uint64_xor(ui64a, ui64b, ui64c);
  netwib_er(netwibtest_uint64_content(ui64c, "00385000000B0000"));

  /* use the same variable for output */
  netwib_er(netwibtest_uint64_init("1234067812345678", &ui64a));
  netwib__uint64_xor(ui64a, ui64a, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "0000000000000000"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__netwib_uint64_not(void)
{
  netwib_uint64 ui64a, ui64b;

  puts("++++----\nTest of uint64_not");

  netwib_er(netwibtest_uint64_init("FFFFFFFFFFFFFFFF", &ui64a));
  netwib__uint64_not(ui64a, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "0000000000000000"));

  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib__uint64_not(ui64a, ui64b);
  netwib_er(netwibtest_uint64_content(ui64b, "EDCBA987EDCBA987"));

  /* use the same variable for output */
  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib__uint64_not(ui64a, ui64a);
  netwib_er(netwibtest_uint64_content(ui64a, "EDCBA987EDCBA987"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__netwib_uint64_div(void)
{
  netwib_uint64 ui64a, ui64b, ui64c, ui64d;

  puts("++++----\nTest of uint64_div");

  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64b));
  netwib__uint64_div(ui64a, ui64b, &ui64c, &ui64d);
  netwib_er(netwibtest_uint64_content(ui64c, "0000000000000001"));
  netwib_er(netwibtest_uint64_content(ui64d, "0000000000000000"));

  netwib_er(netwibtest_uint64_init("0000000000000010", &ui64a));
  netwib_er(netwibtest_uint64_init("0000000000000004", &ui64b));
  netwib__uint64_div(ui64a, ui64b, &ui64c, &ui64d);
  netwib_er(netwibtest_uint64_content(ui64c, "0000000000000004"));
  netwib_er(netwibtest_uint64_content(ui64d, "0000000000000000"));

  netwib_er(netwibtest_uint64_init("0000000000384FB9", &ui64a));
  netwib_er(netwibtest_uint64_init("0000000000000A01", &ui64b));
  netwib__uint64_div(ui64a, ui64b, &ui64c, &ui64d);
  netwib_er(netwibtest_uint64_content(ui64c, "00000000000005A1"));
  netwib_er(netwibtest_uint64_content(ui64d, "0000000000000018"));

  netwib_er(netwibtest_uint64_init("0000000000384FB9", &ui64a));
  netwib_er(netwibtest_uint64_init("0000000000000A00", &ui64b));
  netwib__uint64_div(ui64a, ui64b, &ui64c, &ui64d);
  netwib_er(netwibtest_uint64_content(ui64c, "00000000000005A1"));
  netwib_er(netwibtest_uint64_content(ui64d, "00000000000005B9"));

  netwib_er(netwibtest_uint64_init("2468ACF02468ACF0", &ui64a));
  netwib_er(netwibtest_uint64_init("0000000000000002", &ui64b));
  netwib__uint64_div(ui64a, ui64b, &ui64c, &ui64d);
  netwib_er(netwibtest_uint64_content(ui64c, "1234567812345678"));
  netwib_er(netwibtest_uint64_content(ui64d, "0000000000000000"));

  netwib_er(netwibtest_uint64_init("2468ACF02468ACF1", &ui64a));
  netwib_er(netwibtest_uint64_init("0000000000000002", &ui64b));
  netwib__uint64_div(ui64a, ui64b, &ui64c, &ui64d);
  netwib_er(netwibtest_uint64_content(ui64c, "1234567812345678"));
  netwib_er(netwibtest_uint64_content(ui64d, "0000000000000001"));

  netwib_er(netwibtest_uint64_init("EDCBA987EDCBA988", &ui64a));
  netwib_er(netwibtest_uint64_init("FFFFFFFFFFFFFFFF", &ui64b));
  netwib__uint64_div(ui64a, ui64b, &ui64c, &ui64d);
  netwib_er(netwibtest_uint64_content(ui64c, "0000000000000000"));
  netwib_er(netwibtest_uint64_content(ui64d, "EDCBA987EDCBA988"));

  netwib_er(netwibtest_uint64_init("FFFFFFFFFFFFFFFE", &ui64a));
  netwib_er(netwibtest_uint64_init("FFFFFFFFFFFFFFFF", &ui64b));
  netwib__uint64_div(ui64a, ui64b, &ui64c, &ui64d);
  netwib_er(netwibtest_uint64_content(ui64c, "0000000000000000"));
  netwib_er(netwibtest_uint64_content(ui64d, "FFFFFFFFFFFFFFFE"));

  /* use the same variable for output */
  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib__uint64_div(ui64a, ui64a, &ui64a, &ui64b);
  netwib_er(netwibtest_uint64_content(ui64a, "0000000000000001"));
  netwib_er(netwibtest_uint64_content(ui64b, "0000000000000000"));
  netwib_er(netwibtest_uint64_init("1234567812345678", &ui64a));
  netwib__uint64_div(ui64a, ui64a, &ui64b, &ui64a);
  netwib_er(netwibtest_uint64_content(ui64b, "0000000000000001"));
  netwib_er(netwibtest_uint64_content(ui64a, "0000000000000000"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__netwib_int64_div(void)
{
  netwib_int64 i64a, i64b, i64c, i64d;

  puts("++++----\nTest of int64_div");

  netwib_er(netwibtest_int64_init("1234567812345678", &i64a));
  netwib_er(netwibtest_int64_init("1234567812345678", &i64b));
  netwib__int64_div(i64a, i64b, &i64c, &i64d);
  netwib_er(netwibtest_int64_content(i64c, "0000000000000001"));
  netwib_er(netwibtest_int64_content(i64d, "0000000000000000"));

  netwib_er(netwibtest_int64_init("2468ACF02468ACF0", &i64a));
  netwib_er(netwibtest_int64_init("0000000000000002", &i64b));
  netwib__int64_div(i64a, i64b, &i64c, &i64d);
  netwib_er(netwibtest_int64_content(i64c, "1234567812345678"));
  netwib_er(netwibtest_int64_content(i64d, "0000000000000000"));

  netwib_er(netwibtest_int64_init("2468ACF02468ACF1", &i64a));
  netwib_er(netwibtest_int64_init("0000000000000002", &i64b));
  netwib__int64_div(i64a, i64b, &i64c, &i64d);
  netwib_er(netwibtest_int64_content(i64c, "1234567812345678"));
  netwib_er(netwibtest_int64_content(i64d, "0000000000000001"));

  netwib_er(netwibtest_int64_init("F468ACF02468ACF1", &i64a));
  netwib_er(netwibtest_int64_init("0000000000000007", &i64b));
  netwib__int64_div(i64a, i64b, &i64c, &i64d);
  netwib_er(netwibtest_int64_content(i64c, "FE5818B4977CAAFE"));
  netwib_er(netwibtest_int64_content(i64d, "FFFFFFFFFFFFFFFF"));

  netwib_er(netwibtest_int64_init("F468ACF02468ACF0", &i64a));
  netwib_er(netwibtest_int64_init("0000000000000007", &i64b));
  netwib__int64_div(i64a, i64b, &i64c, &i64d);
  netwib_er(netwibtest_int64_content(i64c, "FE5818B4977CAAFE"));
  netwib_er(netwibtest_int64_content(i64d, "FFFFFFFFFFFFFFFE"));

  netwib_er(netwibtest_int64_init("EDCBA987EDCBA988", &i64a));
  netwib_er(netwibtest_int64_init("FFFFFFFFFFFFFFFF", &i64b));
  netwib__int64_div(i64a, i64b, &i64c, &i64d);
  netwib_er(netwibtest_int64_content(i64c, "1234567812345678"));
  netwib_er(netwibtest_int64_content(i64d, "0000000000000000"));

  netwib_er(netwibtest_int64_init("EDCBA987EDCBA988", &i64a));
  netwib_er(netwibtest_int64_init("FFFFFFFFFFFFFFF0", &i64b));
  netwib__int64_div(i64a, i64b, &i64c, &i64d);
  netwib_er(netwibtest_int64_content(i64c, "0123456781234567"));
  netwib_er(netwibtest_int64_content(i64d, "FFFFFFFFFFFFFFF8"));

  netwib_er(netwibtest_int64_init("3DCBA987EDCBA988", &i64a));
  netwib_er(netwibtest_int64_init("FFFFFFFFFFFFFFFF", &i64b));
  netwib__int64_div(i64a, i64b, &i64c, &i64d);
  netwib_er(netwibtest_int64_content(i64c, "C234567812345678"));
  netwib_er(netwibtest_int64_content(i64d, "0000000000000000"));

  netwib_er(netwibtest_int64_init("3DCBA987EDCBA988", &i64a));
  netwib_er(netwibtest_int64_init("FFFFFFFFFFFFFFEF", &i64b));
  netwib__int64_div(i64a, i64b, &i64c, &i64d);
  netwib_er(netwibtest_int64_content(i64c, "FC5D6E7F8899AABC"));
  netwib_er(netwibtest_int64_content(i64d, "0000000000000004"));

  netwib_er(netwibtest_int64_init("FFFFFFFFFFFFFFFE", &i64a));
  netwib_er(netwibtest_int64_init("FFFFFFFFFFFFFFFF", &i64b));
  netwib__int64_div(i64a, i64b, &i64c, &i64d);
  netwib_er(netwibtest_int64_content(i64c, "0000000000000002"));
  netwib_er(netwibtest_int64_content(i64d, "0000000000000000"));

  /* use the same variable for output */
  netwib_er(netwibtest_int64_init("1234567812345678", &i64a));
  netwib__int64_div(i64a, i64a, &i64a, &i64b);
  netwib_er(netwibtest_int64_content(i64a, "0000000000000001"));
  netwib_er(netwibtest_int64_content(i64b, "0000000000000000"));
  netwib_er(netwibtest_int64_init("1234567812345678", &i64a));
  netwib__int64_div(i64a, i64a, &i64b, &i64a);
  netwib_er(netwibtest_int64_content(i64b, "0000000000000001"));
  netwib_er(netwibtest_int64_content(i64a, "0000000000000000"));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_dat_uint64(void)
{
  puts("###########################################\nTest of dat/uint64");

  netwib_er(test__uint64_init_832());

  netwib_er(test__uint64_init_uint32());
  netwib_er(test__uint64_init_uintmax());
  netwib_er(test__uint64_init_uintptr());
  netwib_er(test__int64_init_int32());
  netwib_er(test__int64_init_intmax());
  netwib_er(test__int64_init_intptr());
  netwib_er(test__uint32_init_uint64());
  netwib_er(test__uintmax_init_uint64());
  netwib_er(test__uintptr_init_uint64());
  netwib_er(test__int32_init_int64());
  netwib_er(test__intmax_init_int64());
  netwib_er(test__intptr_init_int64());

  netwib_er(test__netwib_uint64_add());
  netwib_er(test__netwib_uint64_sub());
  netwib_er(test__netwib_uint64_mul());

  netwib_er(test__netwib_uint64_inc());
  netwib_er(test__netwib_uint64_dec());

  netwib_er(test__netwib_uint64_cmp());
  netwib_er(test__netwib_int64_cmp());

  netwib_er(test__netwib_int64_neg());

  netwib_er(test__netwib_uint64_shl());
  netwib_er(test__netwib_uint64_shr());
  netwib_er(test__netwib_uint64_rol());
  netwib_er(test__netwib_uint64_ror());
  netwib_er(test__netwib_uint64_and());
  netwib_er(test__netwib_uint64_or());
  netwib_er(test__netwib_uint64_xor());
  netwib_er(test__netwib_uint64_not());

  /* not done before because div depends on several functions which should
     be tested before */
  netwib_er(test__netwib_uint64_div());
  netwib_er(test__netwib_int64_div());

  return(NETWIB_ERR_OK);
}
