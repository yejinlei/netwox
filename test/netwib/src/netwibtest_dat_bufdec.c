#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_decode_test(void)
{
  netwib_buf buf, buf2;

  puts("++++----\nTest of netwib_buf_decode");

  /**/
  netwib_er(netwib_buf_decode(NULL, NETWIB_DECODETYPE_HEXA, NULL));

  netwib_er(netwib_buf_init_ext_string("41 4d 4E", &buf2));
  netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_HEXA, NULL));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("", &buf2));
  netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_HEXA, &buf));
  netwib_er(netwibtest_buf_content2(&buf, ""));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("41 4 2 4\t3\r\n", &buf2));
  netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_HEXA, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "ABC"));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("4 ", &buf2));
  netwib_eir(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_HEXA, &buf),
             NETWIB_ERR_PAHEXAODD);
  netwib_er(netwibtest_buf_content2(&buf, ""));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("4M", &buf2));
  netwib_eir(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_HEXA, &buf),
             NETWIB_ERR_PAHEXACHAR);
  netwib_er(netwibtest_buf_content2(&buf, ""));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("41 4d 4E", &buf2));
  netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_HEXA, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "AMN"));
  netwib_er(netwib_buf_close(&buf));

  /**/
  netwib_er(netwib_buf_decode(NULL, NETWIB_DECODETYPE_MIXED, NULL));

  netwib_er(netwib_buf_init_ext_string("41 4d 4E", &buf2));
  netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_MIXED, NULL));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("", &buf2));
  netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_MIXED, &buf));
  netwib_er(netwibtest_buf_content2(&buf, ""));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("41 4 2 4\t3\r\n", &buf2));
  netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_MIXED, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "ABC"));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("4 ", &buf2));
  netwib_eir(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_MIXED, &buf),
             NETWIB_ERR_PAHEXAODD);
  netwib_er(netwibtest_buf_content2(&buf, ""));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("4M", &buf2));
  netwib_eir(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_MIXED, &buf),
             NETWIB_ERR_PAHEXACHAR);
  netwib_er(netwibtest_buf_content2(&buf, ""));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("41 4d 4E", &buf2));
  netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_MIXED, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "AMN"));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("41 4d 4E 'a'", &buf2));
  netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_MIXED, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "AMNa"));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("41 4d 4E 'a'' b' 'e' '' 61 'r'",
                                     &buf2));
  netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_MIXED, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "AMNa' bear"));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("'a'' b' 'e' '' 61 'r'",
                                     &buf2));
  netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_MIXED, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "a' bear"));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("4 'e' 5", &buf2));
  netwib_eir(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_MIXED, &buf),
             NETWIB_ERR_PAHEXAODD);
  netwib_er(netwibtest_buf_content2(&buf, ""));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("44 'e   ", &buf2));
  netwib_eir(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_MIXED, &buf),
             NETWIB_ERR_PAMIXED);
  netwib_er(netwibtest_buf_content2(&buf, ""));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("'a''''b '' '''",
                                     &buf2));
  netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_MIXED, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "a''b ' '"));
  netwib_er(netwib_buf_close(&buf));

  /**/
  netwib_er(netwib_buf_decode(NULL, NETWIB_DECODETYPE_BASE64, NULL));

  netwib_er(netwib_buf_init_ext_string("YQ==", &buf2));
  netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_BASE64, NULL));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("", &buf2));
  netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_BASE64, &buf));
  netwib_er(netwibtest_buf_content2(&buf, ""));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("YQ==", &buf2));
  netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_BASE64, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "a"));
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("YWI=", &buf2));
  netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_BASE64, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "ab"));
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("YWJj", &buf2));
  netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_BASE64, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "abc"));
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("YWJjZA==", &buf2));
  netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_BASE64, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "abcd"));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("4", &buf2));
  netwib_eir(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_BASE64, &buf),
             NETWIB_ERR_PABASE64X4);
  netwib_er(netwibtest_buf_content2(&buf, ""));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("444$", &buf2));
  netwib_eir(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_BASE64, &buf),
             NETWIB_ERR_PABASE64CHAR);
  netwib_er(netwibtest_buf_content2(&buf, ""));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("44==", &buf2));
  netwib_eir(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_BASE64, &buf),
             NETWIB_ERR_PABASE64LO);
  netwib_er(netwibtest_buf_content2(&buf, ""));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_decode_quoted_test(void)
{
  netwib_buf buf, buf2;

  puts("++++----\nTest of netwib_buf_decode_quoted");

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("", &buf2));
  netwib_eir(netwib_buf_decode_quoted(&buf2, &buf),
             NETWIB_ERR_DATAEND);
  netwib_er(netwibtest_buf_content2(&buf, ""));
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string("  \t \t ", &buf2));
  netwib_eir(netwib_buf_decode_quoted(&buf2, &buf),
             NETWIB_ERR_DATAEND);
  netwib_er(netwibtest_buf_content2(&buf, ""));
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string(" hello ", &buf2));
  netwib_er(netwib_buf_decode_quoted(&buf2, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "hello"));
  netwib_er(netwibtest_buf_content2(&buf2, ""));
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string(" hello\tbob ", &buf2));
  netwib_er(netwib_buf_decode_quoted(&buf2, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "hello"));
  netwib_er(netwibtest_buf_content2(&buf2, "bob "));
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string(" \"hello\"bob", &buf2));
  netwib_er(netwib_buf_decode_quoted(&buf2, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "hello"));
  netwib_er(netwibtest_buf_content2(&buf2, "bob"));
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string(" \"hello\"    bob", &buf2));
  netwib_er(netwib_buf_decode_quoted(&buf2, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "hello"));
  netwib_er(netwibtest_buf_content2(&buf2, "bob"));
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string(" \"he\\l\\\\l\no\"    bob", &buf2));
  netwib_er(netwib_buf_decode_quoted(&buf2, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "hel\\l\no"));
  netwib_er(netwibtest_buf_content2(&buf2, "bob"));
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string(" \"he\\x31o\"    bob", &buf2));
  netwib_er(netwib_buf_decode_quoted(&buf2, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "he1o"));
  netwib_er(netwibtest_buf_content2(&buf2, "bob"));
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string(" \"he", &buf2));
  netwib_eir(netwib_buf_decode_quoted(&buf2, &buf),
             NETWIB_ERR_DATANOTAVAIL);
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_ext_string(" \"he\\", &buf2));
  netwib_eir(netwib_buf_decode_quoted(&buf2, &buf),
             NETWIB_ERR_DATANOTAVAIL);
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_dat_bufdec(void)
{
  puts("###########################################\nTest of dat/bufdec");
  netwib_er(netwib_buf_decode_test());
  netwib_er(netwib_buf_decode_quoted_test());

  return(NETWIB_ERR_OK);
}

