
#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_encode_test2(netwib_conststring in,
                                          netwib_encodetype encodetype,
                                          netwib_conststring wantedmixed)
{
  netwib_buf buf, bufin;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string(in, &bufin));
  netwib_er(netwib_buf_encode(&bufin, encodetype, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwibtest_buf_content3(&buf, wantedmixed));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_encode_test(void)
{

  puts("++++----\nTest of netwib_buf_encode");

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_DATA, "''"));
  netwib_er(netwib_buf_encode_test2("hello", NETWIB_ENCODETYPE_DATA, "'hello'"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_HEXA0, "''"));
  netwib_er(netwib_buf_encode_test2("h", NETWIB_ENCODETYPE_HEXA0,
                                    "'68'"));
  netwib_er(netwib_buf_encode_test2("he", NETWIB_ENCODETYPE_HEXA0,
                                    "'6865'"));
  netwib_er(netwib_buf_encode_test2("hel", NETWIB_ENCODETYPE_HEXA0,
                                    "'68656c'"));
  netwib_er(netwib_buf_encode_test2("hell", NETWIB_ENCODETYPE_HEXA0,
                                    "'68656c6c'"));
  netwib_er(netwib_buf_encode_test2("hello", NETWIB_ENCODETYPE_HEXA0,
                                    "'68656c6c6f'"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_HEXA1, "''"));
  netwib_er(netwib_buf_encode_test2("h", NETWIB_ENCODETYPE_HEXA1,
                                    "'68'"));
  netwib_er(netwib_buf_encode_test2("he", NETWIB_ENCODETYPE_HEXA1,
                                    "'68 65'"));
  netwib_er(netwib_buf_encode_test2("hel", NETWIB_ENCODETYPE_HEXA1,
                                    "'68 65 6c'"));
  netwib_er(netwib_buf_encode_test2("hell", NETWIB_ENCODETYPE_HEXA1,
                                    "'68 65 6c 6c'"));
  netwib_er(netwib_buf_encode_test2("hello", NETWIB_ENCODETYPE_HEXA1,
                                    "'68 65 6c 6c 6f'"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_HEXA2, "''"));
  netwib_er(netwib_buf_encode_test2("h", NETWIB_ENCODETYPE_HEXA2,
                                    "'68'"));
  netwib_er(netwib_buf_encode_test2("he", NETWIB_ENCODETYPE_HEXA2,
                                    "'6865'"));
  netwib_er(netwib_buf_encode_test2("hel", NETWIB_ENCODETYPE_HEXA2,
                                    "'6865 6c'"));
  netwib_er(netwib_buf_encode_test2("hell", NETWIB_ENCODETYPE_HEXA2,
                                    "'6865 6c6c'"));
  netwib_er(netwib_buf_encode_test2("hello", NETWIB_ENCODETYPE_HEXA2,
                                    "'6865 6c6c 6f'"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_HEXA4, "''"));
  netwib_er(netwib_buf_encode_test2("h", NETWIB_ENCODETYPE_HEXA4,
                                    "'68'"));
  netwib_er(netwib_buf_encode_test2("he", NETWIB_ENCODETYPE_HEXA4,
                                    "'6865'"));
  netwib_er(netwib_buf_encode_test2("hel", NETWIB_ENCODETYPE_HEXA4,
                                    "'68656c'"));
  netwib_er(netwib_buf_encode_test2("hell", NETWIB_ENCODETYPE_HEXA4,
                                    "'68656c6c'"));
  netwib_er(netwib_buf_encode_test2("hello", NETWIB_ENCODETYPE_HEXA4,
                                    "'68656c6c 6f'"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_MIXED0, "''"));
  netwib_er(netwib_buf_encode_test2("a", NETWIB_ENCODETYPE_MIXED0, "'''a'''"));
  netwib_er(netwib_buf_encode_test2("ab", NETWIB_ENCODETYPE_MIXED0, "'''ab'''"));
  netwib_er(netwib_buf_encode_test2("ab\"", NETWIB_ENCODETYPE_MIXED0,
                                    "'''ab'' 22'"));
  netwib_er(netwib_buf_encode_test2("ab\"cd", NETWIB_ENCODETYPE_MIXED0,
                                    "'''ab'' 22 ''cd'''"));
  netwib_er(netwib_buf_encode_test2("ab\"c\"d", NETWIB_ENCODETYPE_MIXED0,
                                    "'''ab'' 22 ''c'' 22 ''d'''"));
  netwib_er(netwib_buf_encode_test2("ab\"\"d", NETWIB_ENCODETYPE_MIXED0,
                                    "'''ab'' 2222 ''d'''"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_MIXED1, "''"));
  netwib_er(netwib_buf_encode_test2("a", NETWIB_ENCODETYPE_MIXED1, "'''a'''"));
  netwib_er(netwib_buf_encode_test2("ab", NETWIB_ENCODETYPE_MIXED1, "'''ab'''"));
  netwib_er(netwib_buf_encode_test2("ab\"", NETWIB_ENCODETYPE_MIXED1,
                                    "'''ab'' 22'"));
  netwib_er(netwib_buf_encode_test2("ab\"cd", NETWIB_ENCODETYPE_MIXED1,
                                    "'''ab'' 22 ''cd'''"));
  netwib_er(netwib_buf_encode_test2("ab\"c\"d", NETWIB_ENCODETYPE_MIXED1,
                                    "'''ab'' 22 ''c'' 22 ''d'''"));
  netwib_er(netwib_buf_encode_test2("ab\"\"d", NETWIB_ENCODETYPE_MIXED1,
                                    "'''ab'' 22 22 ''d'''"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_NOTHING, "''"));
  netwib_er(netwib_buf_encode_test2("hello", NETWIB_ENCODETYPE_NOTHING, "''"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_SYNTH, "'0 byte'"));
  netwib_er(netwib_buf_encode_test2("a", NETWIB_ENCODETYPE_SYNTH, "'1 byte'"));
  netwib_er(netwib_buf_encode_test2("hello", NETWIB_ENCODETYPE_SYNTH, "'5 bytes'"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_TEXT, "''"));
  netwib_er(netwib_buf_encode_test2("a\tb", NETWIB_ENCODETYPE_TEXT, "'a' 09 'b'"));
  netwib_er(netwib_buf_encode_test2("a\tb\f", NETWIB_ENCODETYPE_TEXT,
                                    "'a' 09 'b.'"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_BASE64, "''"));
  netwib_er(netwib_buf_encode_test2("a", NETWIB_ENCODETYPE_BASE64, "'YQ=='"));
  netwib_er(netwib_buf_encode_test2("ab", NETWIB_ENCODETYPE_BASE64, "'YWI='"));
  netwib_er(netwib_buf_encode_test2("abc", NETWIB_ENCODETYPE_BASE64, "'YWJj'"));
  netwib_er(netwib_buf_encode_test2("abcd", NETWIB_ENCODETYPE_BASE64,
                                    "'YWJjZA=='"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_QUOTED,
                                    "'\"\"'"));
  netwib_er(netwib_buf_encode_test2("abcd", NETWIB_ENCODETYPE_QUOTED,
                                    "'\"abcd\"'"));
  netwib_er(netwib_buf_encode_test2("abc d \\ \t \n \r \" £ z",
                                    NETWIB_ENCODETYPE_QUOTED,
                                "'\"abc d \\\\ \\t \\n \\r \\\" \\xa3 z\"'"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_DATA_WRAP, ""));
  netwib_er(netwib_buf_encode_test2("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", NETWIB_ENCODETYPE_DATA_WRAP, "'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' 0A"));
  netwib_er(netwib_buf_encode_test2("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab", NETWIB_ENCODETYPE_DATA_WRAP, "'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab' 0A"));
  netwib_er(netwib_buf_encode_test2("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabc", NETWIB_ENCODETYPE_DATA_WRAP, "'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab' 0A 'c' 0A"));
  netwib_er(netwib_buf_encode_test2("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab", NETWIB_ENCODETYPE_DATA_WRAP, "'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab' 0A'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab' 0A"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_HEXA0_WRAP, ""));
  netwib_er(netwib_buf_encode_test2("12345678901234567890123456789012", NETWIB_ENCODETYPE_HEXA0_WRAP, "'3132333435363738393031323334353637383930313233343536373839303132' 0A"));
  netwib_er(netwib_buf_encode_test2("123456789012345678901234567890123", NETWIB_ENCODETYPE_HEXA0_WRAP, "'3132333435363738393031323334353637383930313233343536373839303132' 0A '33' 0A"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_HEXA1_WRAP, ""));
  netwib_er(netwib_buf_encode_test2("1234567890123456", NETWIB_ENCODETYPE_HEXA1_WRAP, "'31 32 33 34 35 36 37 38 39 30 31 32 33 34 35 36' 0A"));
  netwib_er(netwib_buf_encode_test2("12345678901234567", NETWIB_ENCODETYPE_HEXA1_WRAP, "'31 32 33 34 35 36 37 38 39 30 31 32 33 34 35 36' 0A '37' 0A"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_HEXA2_WRAP, ""));
  netwib_er(netwib_buf_encode_test2("12345678901234567890123456789012", NETWIB_ENCODETYPE_HEXA2_WRAP, "'3132 3334 3536 3738 3930 3132 3334 3536 3738 3930 3132 3334 3536 3738 3930 3132' 0A"));
  netwib_er(netwib_buf_encode_test2("123456789012345678901234567890123", NETWIB_ENCODETYPE_HEXA2_WRAP, "'3132 3334 3536 3738 3930 3132 3334 3536 3738 3930 3132 3334 3536 3738 3930 3132' 0A '33' 0A"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_HEXA4_WRAP, ""));
  netwib_er(netwib_buf_encode_test2("12345678901234567890123456789012", NETWIB_ENCODETYPE_HEXA4_WRAP, "'31323334 35363738 39303132 33343536 37383930 31323334 35363738 39303132' 0A"));
  netwib_er(netwib_buf_encode_test2("123456789012345678901234567890123", NETWIB_ENCODETYPE_HEXA4_WRAP, "'31323334 35363738 39303132 33343536 37383930 31323334 35363738 39303132' 0A '33' 0A"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_MIXED0_WRAP, ""));
  netwib_er(netwib_buf_encode_test2("1234567890123456", NETWIB_ENCODETYPE_MIXED0_WRAP, "'''1234567890123456''' 0A"));
  netwib_er(netwib_buf_encode_test2("12\"\"\"\"\"\"56\"\"\"\"\"56", NETWIB_ENCODETYPE_MIXED0_WRAP, "'''12'' 222222222222 ''56'' 2222222222 ''5''' 0A '''6''' 0A"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_MIXED1_WRAP, ""));
  netwib_er(netwib_buf_encode_test2("1234567890123456", NETWIB_ENCODETYPE_MIXED1_WRAP, "'''1234567890123456''' 0A"));
  netwib_er(netwib_buf_encode_test2("12\"\"\"\"\"\"56\"\"\"\"\"56", NETWIB_ENCODETYPE_MIXED1_WRAP, "'''12'' 22 22 22 22 22 22 ''56'' 22 22 22 22 22 ''5''' 0A '''6''' 0A"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_TEXT_WRAP, ""));
  netwib_er(netwib_buf_encode_test2("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", NETWIB_ENCODETYPE_TEXT_WRAP, "'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' 0A"));
  netwib_er(netwib_buf_encode_test2("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab", NETWIB_ENCODETYPE_TEXT_WRAP, "'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab' 0A"));
  netwib_er(netwib_buf_encode_test2("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabc", NETWIB_ENCODETYPE_TEXT_WRAP, "'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab' 0A 'c' 0A"));
  netwib_er(netwib_buf_encode_test2("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\naaaaaaaaaaaaaaabc", NETWIB_ENCODETYPE_TEXT_WRAP, "'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' 0A 'aaaaaaaaaaaaaaabc' 0A"));
  netwib_er(netwib_buf_encode_test2("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\naaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabc", NETWIB_ENCODETYPE_TEXT_WRAP, "'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' 0A 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' 0A 'aaaaaaaaaabc' 0A"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_BASE64_WRAP, ""));
  netwib_er(netwib_buf_encode_test2("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", NETWIB_ENCODETYPE_BASE64_WRAP, "'YWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFhYWFh' 0A 'YWFhYWFhYWFhYWFhYWFhYWE=' 0A"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_ARRAY1, ""));
  netwib_er(netwib_buf_encode_test2("a", NETWIB_ENCODETYPE_ARRAY1, "'._______________________________________________________________.' 0A '|1|0|0|0|0|1|1|0|_______________|_______________|_______________|' 0A"));
  netwib_er(netwib_buf_encode_test2("ab", NETWIB_ENCODETYPE_ARRAY1, "'._______________________________________________________________.' 0A '|1|0|0|0|0|1|1|0|0|1|0|0|0|1|1|0|_______________|_______________|' 0A"));
  netwib_er(netwib_buf_encode_test2("abc", NETWIB_ENCODETYPE_ARRAY1, "'._______________________________________________________________.' 0A '|1|0|0|0|0|1|1|0|0|1|0|0|0|1|1|0|1|1|0|0|0|1|1|0|_______________|' 0A"));
  netwib_er(netwib_buf_encode_test2("abcd", NETWIB_ENCODETYPE_ARRAY1, "'._______________________________________________________________.' 0A '|1|0|0|0|0|1|1|0|0|1|0|0|0|1|1|0|1|1|0|0|0|1|1|0|0|0|1|0|0|1|1|0|' 0A"));
  netwib_er(netwib_buf_encode_test2("abcde", NETWIB_ENCODETYPE_ARRAY1, "'._______________________________________________________________.' 0A '|1|0|0|0|0|1|1|0|0|1|0|0|0|1|1|0|1|1|0|0|0|1|1|0|0|0|1|0|0|1|1|0|' 0A '|1|0|1|0|0|1|1|0|_______________|_______________|_______________|' 0A"));

  netwib_er(netwib_buf_encode_test2("a", NETWIB_ENCODETYPE_ARRAY4, "'._______________________________________________________________.' 0A '|__0x1__|__0x6__|_______________|_______________|_______________|' 0A"));
  netwib_er(netwib_buf_encode_test2("a", NETWIB_ENCODETYPE_ARRAY8, "'._______________________________________________________________.' 0A '|__0x61=97=''a''__|_______________|_______________|_______________|' 0A"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_ARRAY16, ""));
  netwib_er(netwib_buf_encode_test2("a", NETWIB_ENCODETYPE_ARRAY16, "'._______________________________________________________________.' 0A '|____0x61=97____|_______________|_______________________________|' 0A"));
  netwib_er(netwib_buf_encode_test2("ab", NETWIB_ENCODETYPE_ARRAY16, "'._______________________________________________________________.' 0A '|_________0x6162=24930__________|_______________________________|' 0A"));
  netwib_er(netwib_buf_encode_test2("abc", NETWIB_ENCODETYPE_ARRAY16, "'._______________________________________________________________.' 0A '|_________0x6162=24930__________|____0x63=99____|_______________|' 0A"));
  netwib_er(netwib_buf_encode_test2("abcd", NETWIB_ENCODETYPE_ARRAY16, "'._______________________________________________________________.' 0A '|_________0x6162=24930__________|_________0x6364=25444__________|' 0A"));
  netwib_er(netwib_buf_encode_test2("abcde", NETWIB_ENCODETYPE_ARRAY16, "'._______________________________________________________________.' 0A '|_________0x6162=24930__________|_________0x6364=25444__________|' 0A '|___0x65=101____|_______________|_______________________________|' 0A"));

  netwib_er(netwib_buf_encode_test2("abcde", NETWIB_ENCODETYPE_ARRAY32, "'._______________________________________________________________.' 0A '|_____________________0x61626364=1633837924_____________________|' 0A '|___0x65=101____|_______________|_______________________________|' 0A"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_DUMP, ""));
  netwib_er(netwib_buf_encode_test2("abcdefghijklmnopqrstuvw", NETWIB_ENCODETYPE_DUMP, "'61 62 63 64  65 66 67 68  69 6a 6b 6c  6d 6e 6f 70  # abcdefghijklmnop' 0A '71 72 73 74  75 76 77                               # qrstuvw' 0A"));
  netwib_er(netwib_buf_encode_test2("abcdefghijklmnopqrstuvwx", NETWIB_ENCODETYPE_DUMP, "'61 62 63 64  65 66 67 68  69 6a 6b 6c  6d 6e 6f 70  # abcdefghijklmnop' 0A '71 72 73 74  75 76 77 78                            # qrstuvwx' 0A"));
  netwib_er(netwib_buf_encode_test2("abcdefghijklmnopqrstuvwxyz", NETWIB_ENCODETYPE_DUMP, "'61 62 63 64  65 66 67 68  69 6a 6b 6c  6d 6e 6f 70  # abcdefghijklmnop' 0A '71 72 73 74  75 76 77 78  79 7a                     # qrstuvwxyz' 0A"));
  netwib_er(netwib_buf_encode_test2("0123456789ABCDE", NETWIB_ENCODETYPE_DUMP, "'30 31 32 33  34 35 36 37  38 39 41 42  43 44 45     # 0123456789ABCDE' 0A"));
  netwib_er(netwib_buf_encode_test2("0123456789ABCDEF", NETWIB_ENCODETYPE_DUMP, "'30 31 32 33  34 35 36 37  38 39 41 42  43 44 45 46  # 0123456789ABCDEF' 0A"));
  netwib_er(netwib_buf_encode_test2("0123456789ABCDEFG", NETWIB_ENCODETYPE_DUMP, "'30 31 32 33  34 35 36 37  38 39 41 42  43 44 45 46  # 0123456789ABCDEF' 0A '47                                                  # G' 0A"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_LOWERCASE, ""));
  netwib_er(netwib_buf_encode_test2("azA", NETWIB_ENCODETYPE_LOWERCASE, "'aza'"));
  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_UPPERCASE, ""));
  netwib_er(netwib_buf_encode_test2("azA", NETWIB_ENCODETYPE_UPPERCASE, "'AZA'"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_MIXED0H_WRAP, ""));
  netwib_er(netwib_buf_encode_test2("abcdefghijklmnopqrstuvw", NETWIB_ENCODETYPE_MIXED0H_WRAP, "'''abcdefgh''                               # 61 62 63 64 65 66 67 68' 0A '''ijklmnop''                               # 69 6a 6b 6c 6d 6e 6f 70' 0A '''qrstuvw''                                # 71 72 73 74 75 76 77' 0A"));
  netwib_er(netwib_buf_encode_test2("\"\"\"", NETWIB_ENCODETYPE_MIXED0H_WRAP, "'222222                                   # 22 22 22' 0A"));

  netwib_er(netwib_buf_encode_test2("", NETWIB_ENCODETYPE_MIXED1H_WRAP, ""));
  netwib_er(netwib_buf_encode_test2("abcdefghijklmnopqrstuvw", NETWIB_ENCODETYPE_MIXED1H_WRAP, "'''abcdefgh''                               # 61 62 63 64 65 66 67 68' 0A '''ijklmnop''                               # 69 6a 6b 6c 6d 6e 6f 70' 0A '''qrstuvw''                                # 71 72 73 74 75 76 77' 0A"));
  netwib_er(netwib_buf_encode_test2("\"\"\"", NETWIB_ENCODETYPE_MIXED1H_WRAP, "'22 22 22                                 # 22 22 22' 0A"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_encode_transition_test3(netwib_encodetype et1,
                                                     netwib_encodetype et2,
                                                     netwib_buf *pbuf)
{
  netwib_buf buf2;
  netwib_encodetype_context ctx;

  netwib_er(netwib_buf_append_string("\n*******************************\n", pbuf));
  netwib_er(netwib_buf_append_encodetype(et1, pbuf));
  netwib_er(netwib_buf_append_string(" -> ", pbuf));
  netwib_er(netwib_buf_append_encodetype(et2, pbuf));
  netwib_er(netwib_buf_append_string(" :\n", pbuf));

  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  netwib_er(netwib_buf_append_string("abcd", &buf2));

  netwib_er(netwib_buf_encode_transition_init(&ctx));
  netwib_er(netwib_buf_encode_transition(&ctx, et1, pbuf));
  netwib_er(netwib_buf_encode(&buf2, et1, pbuf));
  netwib_er(netwib_buf_encode_transition(&ctx, et2, pbuf));
  netwib_er(netwib_buf_encode(&buf2, et2, pbuf));
  netwib_er(netwib_buf_encode_transition_end(&ctx, pbuf));

  netwib_er(netwib_buf_close(&buf2));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_buf_encode_transition_test2(netwib_uint32 i,
                                                     netwib_encodetype *pet)
{

  switch(i) {
  case 1 : *pet = NETWIB_ENCODETYPE_DATA; break;
  case 2 : *pet = NETWIB_ENCODETYPE_HEXA0; break;
  case 3 : *pet = NETWIB_ENCODETYPE_HEXA1; break;
  case 4 : *pet = NETWIB_ENCODETYPE_HEXA2; break;
  case 5 : *pet = NETWIB_ENCODETYPE_HEXA4; break;
  case 6 : *pet = NETWIB_ENCODETYPE_MIXED0; break;
  case 7 : *pet = NETWIB_ENCODETYPE_MIXED1; break;
  case 8 : *pet = NETWIB_ENCODETYPE_TEXT; break;
  case 9 : *pet = NETWIB_ENCODETYPE_BASE64; break;
  case 10 : *pet = NETWIB_ENCODETYPE_NOTHING; break;
  case 11 : *pet = NETWIB_ENCODETYPE_SYNTH; break;
  case 12 : *pet = NETWIB_ENCODETYPE_DATA_WRAP; break;
  case 13 : *pet = NETWIB_ENCODETYPE_HEXA0_WRAP; break;
  case 14 : *pet = NETWIB_ENCODETYPE_HEXA1_WRAP; break;
  case 15 : *pet = NETWIB_ENCODETYPE_HEXA2_WRAP; break;
  case 16 : *pet = NETWIB_ENCODETYPE_HEXA4_WRAP; break;
  case 17 : *pet = NETWIB_ENCODETYPE_MIXED0_WRAP; break;
  case 18 : *pet = NETWIB_ENCODETYPE_MIXED1_WRAP; break;
  case 19 : *pet = NETWIB_ENCODETYPE_TEXT_WRAP; break;
  case 20 : *pet = NETWIB_ENCODETYPE_BASE64_WRAP; break;
  case 21 : *pet = NETWIB_ENCODETYPE_ARRAY1; break;
  case 22 : *pet = NETWIB_ENCODETYPE_ARRAY4; break;
  case 23 : *pet = NETWIB_ENCODETYPE_ARRAY8; break;
  case 24 : *pet = NETWIB_ENCODETYPE_ARRAY16; break;
  case 25 : *pet = NETWIB_ENCODETYPE_ARRAY32; break;
  case 26 : *pet = NETWIB_ENCODETYPE_DUMP; break;
  case 27 : *pet = NETWIB_ENCODETYPE_LOWERCASE; break;
  case 28 : *pet = NETWIB_ENCODETYPE_UPPERCASE; break;
  case 29 : *pet = NETWIB_ENCODETYPE_MIXED0H_WRAP; break;
  case 0 : *pet = NETWIB_ENCODETYPE_MIXED1H_WRAP; break;
  default:
    return(NETWIB_ERR_DATAEND);
  }
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_buf_encode_transition_test(void)
{
  netwib_uint32 i, j;
  netwib_encodetype et1, et2;
  netwib_err ret1, ret2;
  netwib_buf buf;

  puts("++++----\nTest of netwib_buf_encode_transition");
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  i = 0;
  do {
    ret1 = netwib_buf_encode_transition_test2(i++, &et1);
    j = 0;
    do {
      ret2 = netwib_buf_encode_transition_test2(j++, &et2);
      netwib_er(netwib_buf_encode_transition_test3(et1, et2, &buf));
    } while (ret2 == NETWIB_ERR_OK);
  } while (ret1 == NETWIB_ERR_OK);

  /*netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));*/
  netwib_er(netwibtest_buf_content4(&buf, 0xBB4F));

  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_dat_bufenc(void)
{
  puts("###########################################\nTest of dat/bufenc");
  netwib_er(netwib_buf_encode_test());
  netwib_er(netwib_buf_encode_transition_test());

  return(NETWIB_ERR_OK);
}
