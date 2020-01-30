#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_fmt_test(void)
{
  netwib_buf buf;
  netwib_uint64 ui64;
  netwib_int64 i64;

  puts("++++----\nTest of netwib_buf_append_fmt");

  netwib_er(netwib_buf_append_fmt(NULL, "aze"));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_fmt(&buf, "aze"));
  netwib_er(netwibtest_buf_content2(&buf, "aze"));

  netwib__buf_reinit(&buf);
  netwib_eir(netwib_buf_append_fmt(&buf, "aze%"), NETWIB_ERR_PAFMT);
  netwib_er(netwibtest_buf_content2(&buf, ""));

  netwib__buf_reinit(&buf);
  netwib_eir(netwib_buf_append_fmt(&buf, "aze%{*l 4;uint32}", 1),
             NETWIB_ERR_PAFMT);
  netwib_er(netwibtest_buf_content2(&buf, ""));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%%e"));
  netwib_er(netwibtest_buf_content2(&buf, "a%e"));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%ce", 'e'));
  netwib_er(netwibtest_buf_content2(&buf, "aee"));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%se", "TRE"));
  netwib_er(netwibtest_buf_content2(&buf, "aTREe"));
  netwib_er(netwib_buf_append_fmt(&buf, "b%sc", "T"));
  netwib_er(netwibtest_buf_content2(&buf, "aTREebTc"));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%pe", NULL));
#if NETWIBDEF_ARCH_BITS==32
  netwib_er(netwibtest_buf_content2(&buf, "a0x00000000e"));
#elif NETWIBDEF_ARCH_BITS==64
  netwib_er(netwibtest_buf_content2(&buf, "a0x0000000000000000e"));
#else
#error "not done"
#endif
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%pe", 0x121));
#if NETWIBDEF_ARCH_BITS==32
  netwib_er(netwibtest_buf_content2(&buf, "a0x00000121e"));
#elif NETWIBDEF_ARCH_BITS==64
  netwib_er(netwibtest_buf_content2(&buf, "a0x0000000000000121e"));
#else
#error "not done"
#endif
  netwib__buf_reinit(&buf);
  netwib_eir(netwib_buf_append_fmt(&buf, "a%{"), NETWIB_ERR_PAFMT);
  netwib_er(netwibtest_buf_content2(&buf, ""));

  netwib__buf_reinit(&buf);
  netwib_eir(netwib_buf_append_fmt(&buf, "a%{}"), NETWIB_ERR_PAFMT);
  netwib_er(netwib_err_display(NETWIB_ERR_PAFMT, NETWIB_ERR_ENCODETYPE_FULL));
  netwib_eir(netwib_buf_append_fmt(&buf, "a%{:spec}"), NETWIB_ERR_PAFMT);
  netwib_er(netwib_err_display(NETWIB_ERR_PAFMT, NETWIB_ERR_ENCODETYPE_FULL));
  netwib_eir(netwib_buf_append_fmt(&buf, "a%{gene;}"), NETWIB_ERR_PAFMT);
  netwib_er(netwib_err_display(NETWIB_ERR_PAFMT, NETWIB_ERR_ENCODETYPE_FULL));
  netwib_eir(netwib_buf_append_fmt(&buf, "a%{gene;:sp}"), NETWIB_ERR_PAFMT);
  netwib_er(netwib_err_display(NETWIB_ERR_PAFMT, NETWIB_ERR_ENCODETYPE_FULL));
  netwib_eir(netwib_buf_append_fmt(&buf, "a%{;:sp}"), NETWIB_ERR_PAFMT);
  netwib_er(netwib_err_display(NETWIB_ERR_PAFMT, NETWIB_ERR_ENCODETYPE_FULL));
  netwib_eir(netwib_buf_append_fmt(&buf, "a%{gene;:}"), NETWIB_ERR_PAFMT);
  netwib_er(netwib_err_display(NETWIB_ERR_PAFMT, NETWIB_ERR_ENCODETYPE_FULL));
  netwib_eir(netwib_buf_append_fmt(&buf, "a%{;:}"), NETWIB_ERR_PAFMT);
  netwib_er(netwib_err_display(NETWIB_ERR_PAFMT, NETWIB_ERR_ENCODETYPE_FULL));
  netwib_eir(netwib_buf_append_fmt(&buf, "a%{abcddddddddddddddddddddddddd}"), NETWIB_ERR_PAFMT);
  netwib_er(netwib_err_display(NETWIB_ERR_PAFMT, NETWIB_ERR_ENCODETYPE_FULL));
  netwib_eir(netwib_buf_append_fmt(&buf, "a%{gessssnesssssssssssssss;type}"), NETWIB_ERR_PAFMT);
  netwib_er(netwib_err_display(NETWIB_ERR_PAFMT, NETWIB_ERR_ENCODETYPE_FULL));
  netwib_eir(netwib_buf_append_fmt(&buf, "a%{gene;type;tt}"), NETWIB_ERR_PAFMT);
  netwib_er(netwib_err_display(NETWIB_ERR_PAFMT, NETWIB_ERR_ENCODETYPE_FULL));
  netwib_eir(netwib_buf_append_fmt(&buf, "a%{gene:type;tt}"), NETWIB_ERR_PAFMT);
  netwib_er(netwib_err_display(NETWIB_ERR_PAFMT, NETWIB_ERR_ENCODETYPE_FULL));
  netwib_eir(netwib_buf_append_fmt(&buf, "a%$"), NETWIB_ERR_PAFMT);
  netwib_er(netwib_err_display(NETWIB_ERR_PAFMT, NETWIB_ERR_ENCODETYPE_FULL));

  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint32}", 0));
  netwib_er(netwib_buf_append_fmt(&buf, "a%{r 3;uint32}",0));
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint32:20u}", 0));
  netwib_er(netwib_buf_append_fmt(&buf,"a%{r.4;uint32:u}",0));
  netwib_er(netwib_buf_append_fmt(&buf, "a%{;uint32}", 0));
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint32:}", 0));
  netwib_er(netwib_buf_append_fmt(&buf,"a%{c-67;uint32:}",0));
  netwib_er(netwib_buf_append_fmt(&buf, "a%{;uint32:u}", 0));
  netwib_er(netwib_buf_append_fmt(&buf, "a%{;uint32:}", 0));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint32}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a123b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint32:b}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a1111011b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint32:o}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a173b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint32:u}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a123b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint32:x}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a7bb"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint32:X}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a7Bb"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint32:1u}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a123b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint32:3u}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a123b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint32:4u}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a 123b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint32:14u}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a           123b"));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint32:#b}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "ab1111011b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint32:#}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a123b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint32:#o}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a0173b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint32:#x}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a0x7bb"));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint16}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a123b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint8}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a123b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uintmax}b", (netwib_uintmax)123));
  netwib_er(netwibtest_buf_content2(&buf, "a123b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uintmax:b}b",
                                  (netwib_uintmax)123));
  netwib_er(netwibtest_buf_content2(&buf, "a1111011b"));
#if NETWIB_INTMAX_BITS == 64
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uintmax}b",
                                  (netwib_uintmax)NETWIB_UINT_LL(1234567890123456789)));
  netwib_er(netwibtest_buf_content2(&buf, "a1234567890123456789b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uintmax:b}b",
                                  (netwib_uintmax)NETWIB_UINT_LL(1234567890123456789)));
  netwib_er(netwibtest_buf_content2(&buf, "a1000100100010000100001111010001111101111010011000000100010101b"));
#endif
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uintptr}b", (netwib_uintptr)123));
  netwib_er(netwibtest_buf_content2(&buf, "a123b"));
#if NETWIB_INTPTR_BITS == 64
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uintptr}b",
                                  (netwib_uintptr)NETWIB_UINT_LL(1234567890123456789)));
  netwib_er(netwibtest_buf_content2(&buf, "a1234567890123456789b"));
#endif
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{byte}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a123b"));

  netwib__buf_reinit(&buf);
  netwib_er(netwibtest_uint64_init("00000000 00000123", &ui64));
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint64}b", ui64));
  netwib_er(netwibtest_buf_content2(&buf, "a291b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwibtest_uint64_init("000000123456789A", &ui64));
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint64}b", ui64));
  netwib_er(netwibtest_buf_content2(&buf, "a78187493530b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint64:b}b", ui64));
  netwib_er(netwibtest_buf_content2(&buf,
                                   "a1001000110100010101100111100010011010b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint64:o}b", ui64));
  netwib_er(netwibtest_buf_content2(&buf, "a1106425474232b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint64:x}b", ui64));
  netwib_er(netwibtest_buf_content2(&buf, "a123456789ab"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint64:#b}b", ui64));
  netwib_er(netwibtest_buf_content2(&buf,
                                "ab1001000110100010101100111100010011010b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint64:#o}b", ui64));
  netwib_er(netwibtest_buf_content2(&buf, "a01106425474232b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{uint64:#x}b", ui64));
  netwib_er(netwibtest_buf_content2(&buf, "a0x123456789ab"));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{bool}b", NETWIB_TRUE));
  netwib_er(netwibtest_buf_content2(&buf, "atrueb"));
  netwib_er(netwib_buf_append_fmt(&buf, "a%{bool}b", NETWIB_FALSE));
  netwib_er(netwibtest_buf_content2(&buf, "atruebafalseb"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{bool:0}b", NETWIB_TRUE));
  netwib_er(netwibtest_buf_content2(&buf, "a1b"));
  netwib_er(netwib_buf_append_fmt(&buf, "a%{bool:0}b", NETWIB_FALSE));
  netwib_er(netwibtest_buf_content2(&buf, "a1ba0b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{bool:y}b", NETWIB_TRUE));
  netwib_er(netwibtest_buf_content2(&buf, "ayesb"));
  netwib_er(netwib_buf_append_fmt(&buf, "a%{bool:Y}b", NETWIB_FALSE));
  netwib_er(netwibtest_buf_content2(&buf, "ayesbaNOb"));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{l.10;uint32}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a123.......b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{c.10;uint32}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a...123....b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{c.9;uint32}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a...123...b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{r.9;uint32}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a......123b"));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{int32}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a123b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{int32}b", -123));
  netwib_er(netwibtest_buf_content2(&buf, "a-123b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{int32:+}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a+123b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{int32:+}b", -123));
  netwib_er(netwibtest_buf_content2(&buf, "a-123b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{int32:4}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a 123b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{int32:+4}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a+123b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{int32:5}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a  123b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{int32:+5}b", 123));
  netwib_er(netwibtest_buf_content2(&buf, "a+ 123b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{int32:5}b", -123));
  netwib_er(netwibtest_buf_content2(&buf, "a- 123b"));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{int16}b", -123));
  netwib_er(netwibtest_buf_content2(&buf, "a-123b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{int8}b", -123));
  netwib_er(netwibtest_buf_content2(&buf, "a-123b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{intmax}b", (netwib_intmax)-123));
  netwib_er(netwibtest_buf_content2(&buf, "a-123b"));
#if NETWIB_INTMAX_BITS == 64
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{intmax}b",
                                  (netwib_intmax)NETWIB_INT_LL(-1234567890123456789)));
  netwib_er(netwibtest_buf_content2(&buf, "a-1234567890123456789b"));
  netwib__buf_reinit(&buf);
#endif
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{intptr}b", (netwib_intptr)123));
  netwib_er(netwibtest_buf_content2(&buf, "a123b"));
#if NETWIB_INTPTR_BITS == 64
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{intptr}b",
                                  (netwib_intptr)NETWIB_INT_LL(-1234567890123456789)));
  netwib_er(netwibtest_buf_content2(&buf, "a-1234567890123456789b"));
#endif

  netwib__buf_reinit(&buf);
  netwib_er(netwibtest_int64_init("00000000 00000123", &i64));
  netwib_er(netwib_buf_append_fmt(&buf, "a%{int64}b", i64));
  netwib_er(netwibtest_buf_content2(&buf, "a291b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwibtest_int64_init("00000000 00000123", &i64));
  netwib_er(netwib_buf_append_fmt(&buf, "a%{int64:+}b", i64));
  netwib_er(netwibtest_buf_content2(&buf, "a+291b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwibtest_int64_init("F0000000 00000123", &i64));
  netwib_er(netwib_buf_append_fmt(&buf, "a%{int64}b", i64));
  netwib_er(netwibtest_buf_content2(&buf, "a-1152921504606846685b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwibtest_int64_init("00000000 00000000", &i64));
  netwib_er(netwib_buf_append_fmt(&buf, "a%{int64}b", i64));
  netwib_er(netwibtest_buf_content2(&buf, "a0b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwibtest_int64_init("00000000 00000000", &i64));
  netwib_er(netwib_buf_append_fmt(&buf, "a%{int64:+}b", i64));
  netwib_er(netwibtest_buf_content2(&buf, "a0b"));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{cmp}b", NETWIB_CMP_EQ));
  netwib_er(netwibtest_buf_content2(&buf, "a=b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{cmp:0}b", NETWIB_CMP_EQ));
  netwib_er(netwibtest_buf_content2(&buf, "a0b"));
  netwib_er(netwib_buf_append_fmt(&buf, "a%{cmp:0}b", NETWIB_CMP_LT));
  netwib_er(netwibtest_buf_content2(&buf, "a0ba-b"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{cmp:e}b", NETWIB_CMP_EQ));
  netwib_er(netwibtest_buf_content2(&buf, "aeqb"));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{c.9;c}b", 'd'));
  netwib_er(netwibtest_buf_content2(&buf, "a....d....b"));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{r.9;s}b", "eeeeeeeeeeeeee"));
  netwib_er(netwibtest_buf_content2(&buf, "aeeeeeeeeeeeeeeb"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{s}b", "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"));
  netwib_er(netwibtest_buf_content2(&buf, "aeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeb"));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "a%{c.20;p}b", 0x123));
#if NETWIBDEF_ARCH_BITS==32
  netwib_er(netwibtest_buf_content2(&buf, "a.....0x00000123.....b"));
#elif NETWIBDEF_ARCH_BITS==64
  netwib_er(netwibtest_buf_content2(&buf, "a.0x0000000000000123.b"));
#else
#error "not done"
#endif

  {
    netwib_buf buf2;
    netwib__buf_reinit(&buf);
    netwib_er(netwib_buf_init_mallocdefault(&buf2));
    netwib_er(netwib_buf_append_string("hello", &buf2));
    netwib_er(netwib_buf_append_fmt(&buf, "a%{buf}b", &buf2));
    netwib_er(netwibtest_buf_content2(&buf, "ahellob"));
    netwib__buf_reinit(&buf);
    netwib_er(netwib_buf_append_fmt(&buf, "a%{c-8;buf}b", &buf2));
    netwib_er(netwibtest_buf_content2(&buf, "a-hello--b"));
    netwib_er(netwib_buf_close(&buf2));
  }

  {
    netwib_eth eth;
    netwib_ip ipa;
    eth.b[0] = 0x12;
    eth.b[1] = 0x34;
    eth.b[2] = 0x56;
    eth.b[3] = 0x8D;
    eth.b[4] = 0x5C;
    eth.b[5] = 0x13;
    netwib__buf_reinit(&buf);
    netwib_er(netwib_buf_append_fmt(&buf, "a%{eth}b", &eth));
    netwib_er(netwibtest_buf_content2(&buf, "a12:34:56:8D:5C:13b"));
    netwib__buf_reinit(&buf);
    netwib_er(netwib_buf_append_fmt(&buf, "a%{r-20;eth}b", &eth));
    netwib_er(netwibtest_buf_content2(&buf, "a---12:34:56:8D:5C:13b"));
    netwib__buf_reinit(&buf);
    netwib_er(netwib_ip_init_ip4(0x7F000001, &ipa));
    netwib_er(netwib_buf_append_fmt(&buf, "a%{ip}b", &ipa));
    netwib_er(netwibtest_buf_content2(&buf, "a127.0.0.1b"));
    netwib__buf_reinit(&buf);
    netwib_er(netwib_buf_append_fmt(&buf, "a%{r-15;ip}b", &ipa));
    netwib_er(netwibtest_buf_content2(&buf, "a------127.0.0.1b"));
    netwib__buf_reinit(&buf);
    netwib_er(netwib_buf_append_fmt(&buf, "a%{port}b", 123));
    netwib_er(netwibtest_buf_content2(&buf, "a123b"));
    netwib__buf_reinit(&buf);
    netwib_er(netwib_buf_append_fmt(&buf, "a%{c-5;port}b", 123));
    netwib_er(netwibtest_buf_content2(&buf, "a-123-b"));
  }

  {
    netwib_eth eth1, eth2;
    netwib_eths *peths;
    netwib__buf_reinit(&buf);
    netwib_er(netwib_eths_initdefault(&peths));
    netwib_er(netwib_eth_init_fields(2,2,3,4,5,6, &eth1));
    netwib_er(netwib_eth_init_fields(7,2,3,4,5,6, &eth2));
    netwib_er(netwib_eths_add_ethrange(peths, &eth1, &eth2));
    netwib_er(netwib_eth_init_fields(8,2,3,4,5,6, &eth2));
    netwib_er(netwib_eths_add_eth(peths, &eth2));
    netwib_er(netwib_buf_append_fmt(&buf, "a%{eths}b", peths));
    netwib_er(netwibtest_buf_content2(&buf, "a02:02:03:04:05:06-07:02:03:04:05:06,08:02:03:04:05:06b"));
    netwib__buf_reinit(&buf);
    netwib_er(netwib_buf_append_fmt(&buf, "a%{c*80;eths}b", peths));
    netwib_er(netwibtest_buf_content2(&buf, "a*************02:02:03:04:05:06-07:02:03:04:05:06,08:02:03:04:05:06**************b"));
    netwib_er(netwib_eths_close(&peths));
  }
  {
    netwib_ips *pips;
    netwib_ip ip1, ip2;
    netwib__buf_reinit(&buf);
    netwib_er(netwib_ips_initdefault(&pips));
    netwib_er(netwib_ip_init_ip4(0x01020304, &ip1));
    netwib_er(netwib_ip_init_ip4(0x01020305, &ip2));
    netwib_er(netwib_ips_add_iprange(pips, &ip1, &ip2));
    netwib_er(netwib_ip_init_ip4(0x01020307, &ip1));
    netwib_er(netwib_ips_add_ip(pips, &ip1));
    netwib_er(netwib_buf_append_fmt(&buf, "a%{ips}b", pips));
    netwib_er(netwibtest_buf_content2(&buf, "a1.2.3.4-1.2.3.5,1.2.3.7b"));
    netwib__buf_reinit(&buf);
    netwib_er(netwib_buf_append_fmt(&buf, "a%{c_30;ips}b", pips));
    netwib_er(netwibtest_buf_content2(&buf, "a___1.2.3.4-1.2.3.5,1.2.3.7____b"));
    netwib_er(netwib_ips_close(&pips));
  }
  {
    netwib_ports *pports;
    netwib__buf_reinit(&buf);
    netwib_er(netwib_ports_initdefault(&pports));
    netwib_er(netwib_ports_add_portrange(pports, 12, 24));
    netwib_er(netwib_ports_add_port(pports, 56));
    netwib_er(netwib_buf_append_fmt(&buf, "a%{ports}b", pports));
    netwib_er(netwibtest_buf_content2(&buf, "a12-24,56b"));
    netwib__buf_reinit(&buf);
    netwib_er(netwib_buf_append_fmt(&buf, "a%{lA10;ports}b", pports));
    netwib_er(netwibtest_buf_content2(&buf, "a12-24,56AAb"));
    netwib_er(netwib_ports_close(&pports));
  }

  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_global_ctl_set_err_purge());
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_decode_fmt_test(void)
{
  netwib_buf buf;
  netwib_uint64 ui64;
  netwib_int64 i64;
  netwib_uint32 ui32;
  netwib_uint8 ui8;
  netwib_int32 i32;
  netwib_ptr p;
  netwib_bool b;
  netwib_cmp cmp;
  char c, c2, s[80], s2[80];

  puts("++++----\nTest of netwib_buf_decode_fmt");

  netwib_er(netwib_buf_init_ext_string("abc", &buf));
  netwib_eir(netwib_buf_decode_fmt(&buf, "a%%e"), NETWIB_ERR_NOTCONVERTED);
  netwib_er(netwib_buf_init_ext_string("a%c", &buf));
  netwib_eir(netwib_buf_decode_fmt(&buf, "a%%e"), NETWIB_ERR_NOTCONVERTED);

  netwib_er(netwib_buf_init_ext_string("abc", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%cc", &c));
  if (c != 'b') {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_init_ext_string("abcd", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%cc%c", &c, &c2));
  if (c != 'b') {
    return(NETWIBTEST_ERR_9);
  }
  if (c2 != 'd') {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_init_ext_string("abc", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{c.23;c}c", &c));
  if (c != 'b') {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_init_ext_string("abc", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{l_23;c}c", &c));
  if (c != 'b') {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_init_ext_string("abbbc", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%sc", s));
  if (netwib_c_strcmp(s, "bbb")) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_init_ext_string("a  bbb bbbb  c", &buf));
  netwib_eir(netwib_buf_decode_fmt(&buf, "a%sc", s), NETWIB_ERR_NOTCONVERTED);

  netwib_er(netwib_buf_init_ext_string("a  bbb   c", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%sc", s));
  if (netwib_c_strcmp(s, "bbb")) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_init_ext_string("a  bbb  \t    bbbb  c", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%s %sc", s, s2));
  if (netwib_c_strcmp(s, "bbb")) {
    return(NETWIBTEST_ERR_9);
  }
  if (netwib_c_strcmp(s2, "bbbb")) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_buf_init_ext_string("a  bbb  \t    bbbb  c", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{s:glob}c", s, s2));
  if (netwib_c_strcmp(s, "  bbb  \t    bbbb  ")) {
    return(NETWIBTEST_ERR_9);
  }

  {
    netwib_buf buf2;
    netwib_er(netwib_buf_init_mallocdefault(&buf2));
    netwib_er(netwib_buf_init_ext_string("a  bbb  c", &buf));
    netwib_er(netwib_buf_decode_fmt(&buf, "a%{buf}c", &buf2));
    netwib_er(netwibtest_buf_content2(&buf2, "bbb"));
    netwib_er(netwib_buf_close(&buf2));
    netwib_er(netwib_buf_init_mallocdefault(&buf2));
    netwib_er(netwib_buf_init_ext_string("a  bb b  c", &buf));
    netwib_er(netwib_buf_decode_fmt(&buf, "a%{buf:glob}c", &buf2));
    netwib_er(netwibtest_buf_content2(&buf2, "  bb b  "));
    netwib_er(netwib_buf_close(&buf2));
  }

  netwib_er(netwib_buf_init_ext_string("a33c", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{uint32}c", &ui32));
  if (ui32 != 33) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_buf_init_ext_string("a33z", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{uint32:X}z", &ui32));
  if (ui32 != 0x33) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_buf_init_ext_string("a1010z", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{uint32:b}z", &ui32));
  if (ui32 != 10) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_buf_init_ext_string("a1010z", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{uint32}", &ui32));
  if (ui32 != 1010) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_eir(netwib_buf_decode_fmt(&buf, "a%{uint32}%$", &ui32),
             NETWIB_ERR_NOTCONVERTED);

  netwib_er(netwib_buf_init_ext_string("az", &buf));
  netwib_eir(netwib_buf_decode_fmt(&buf, "a%{uint32}", &ui32),
             NETWIB_ERR_NOTCONVERTED);

  netwib_er(netwib_buf_init_ext_string("a1010", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{uint32}%$", &ui32));
  if (ui32 != 1010) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_eir(netwib_buf_decode_fmt(&buf, "a%{uint32}%$p", &ui32),
             NETWIB_ERR_PAFMT);

  netwib_er(netwib_buf_init_ext_string("a101z", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{uint8}", &ui8));
  if (ui8 != 101) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_buf_init_ext_string("a1018z", &buf));
  netwib_eir(netwib_buf_decode_fmt(&buf, "a%{uint8}", &ui8),
             NETWIB_ERR_NOTCONVERTED);

  netwib_er(netwib_buf_init_ext_string("a1010", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{uint64}%$", &ui64));
  netwib_er(netwibtest_uint64_content(ui64, "00000000000003F2"));

  netwib_er(netwib_buf_init_ext_string("a+1010", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{uint64}%$", &ui64));
  netwib_er(netwibtest_uint64_content(ui64, "00000000000003F2"));

  netwib_er(netwib_buf_init_ext_string("a1010", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{uint64:b}%$", &ui64));
  netwib_er(netwibtest_uint64_content(ui64, "000000000000000A"));

  netwib_er(netwib_buf_init_ext_string("a1010", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{uint64:X}%$", &ui64));
  netwib_er(netwibtest_uint64_content(ui64, "0000000000001010"));

  netwib_er(netwib_buf_init_ext_string("ab1010", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{uint64:b}%$", &ui64));
  netwib_er(netwibtest_uint64_content(ui64, "000000000000000A"));

  netwib_er(netwib_buf_init_ext_string("a0x1010", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{uint64:X}%$", &ui64));
  netwib_er(netwibtest_uint64_content(ui64, "0000000000001010"));

  netwib_er(netwib_buf_init_ext_string("a0xF000000000001010", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{uint64:X}%$", &ui64));
  netwib_er(netwibtest_uint64_content(ui64, "F000000000001010"));

  netwib_er(netwib_buf_init_ext_string("a0xF0000000000010108", &buf));
  netwib_eir(netwib_buf_decode_fmt(&buf, "a%{uint64:X}%$", &ui64),
             NETWIB_ERR_NOTCONVERTED);

  netwib_er(netwib_buf_init_ext_string("a-1010z", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{int32}z", &i32));
  if (i32 != -1010) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_buf_init_ext_string("a1010", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{int64}%$", &i64));
  netwib_er(netwibtest_uint64_content(i64, "00000000000003F2"));

  netwib_er(netwib_buf_init_ext_string("a-1010", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{int64}%$", &i64));
  netwib_er(netwibtest_uint64_content(i64, "FFFFFFFFFFFFFC0E"));

  netwib_er(netwib_buf_init_ext_string("a0x1234z", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{p}z", &p));
  if ((netwib_uintptr)p != 0x1234) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_buf_init_ext_string("atruez", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{bool}z", &b));
  if (b != NETWIB_TRUE) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_buf_init_ext_string("ayesz", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{bool}z", &b));
  if (b != NETWIB_TRUE) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_buf_init_ext_string("ayesz", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{bool:y}z", &b));
  if (b != NETWIB_TRUE) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_buf_init_ext_string("ayesz", &buf));
  buf.totalsize--;
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{bool:y}z", &b));
  if (b != NETWIB_TRUE) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_buf_init_ext_string("a-z", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{cmp}z", &cmp));
  if (cmp != NETWIB_CMP_LT) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_buf_init_ext_string("a>z", &buf));
  netwib_er(netwib_buf_decode_fmt(&buf, "a%{cmp}z", &cmp));
  if (cmp != NETWIB_CMP_GT) {
    return(NETWIBTEST_ERR_7);
  }

  {
    netwib_eth eth;
    netwib_er(netwib_buf_init_ext_string("aab:23:5:ef:23:12c", &buf));
    netwib_er(netwib_buf_decode_fmt(&buf, "a%{eth}c", &eth));
    netwib_er(netwibtest_eth_content(&eth, "ab:23:05:ef:23:12"));
  }

  {
    netwib_ip ipa;
    netwib_er(netwib_buf_init_ext_string("a1.2.3.4c", &buf));
    netwib_er(netwib_buf_decode_fmt(&buf, "a%{ip}c", &ipa));
    netwib_er(netwibtest_ip_content(&ipa, "1.2.3.4"));
  }

  {
    netwib_port port;
    netwib_er(netwib_buf_init_ext_string("a123c", &buf));
    netwib_er(netwib_buf_decode_fmt(&buf, "a%{port}c", &port));
    if (port != 123) {
      return(NETWIBTEST_ERR_9);
    }
  }

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_dat_fmt(void)
{
  puts("###########################################\nTest of dat/fmt");
  netwib_er(netwib_buf_append_fmt_test());
  netwib_er(netwib_buf_decode_fmt_test());

  return(NETWIB_ERR_OK);
}
