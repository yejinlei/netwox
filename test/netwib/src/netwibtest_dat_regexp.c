#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_search_re_test2(netwib_conststring str,
                                             netwib_conststring strre,
                                             netwib_bool casesensitive,
                                             netwib_conststring want)
{
  netwib_buf buf, bufre, found;

  netwib_er(netwib_buf_init_ext_string(str, &buf));
  buf.totalsize--;
  netwib_er(netwib_buf_init_ext_string(strre, &bufre));
  bufre.totalsize--;
  netwib_er(netwib_buf_search_re(&buf, &bufre, casesensitive, &found));
  netwib_er(netwibtest_buf_content2(&found, want));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_search_re_test(void)
{
  netwib_buf buf, re;

  puts("++++----\nTest of netwib_buf_search_re");

  netwib_er(netwib_buf_init_ext_string("abcdef", &buf));
  netwib_er(netwib_buf_init_ext_string("ab[cdef", &re));
  netwib_eir(netwib_buf_search_re(&buf, &re, NETWIB_TRUE, NULL),
             NETWIB_ERR_FUREGCOMP);
  netwib_er(netwib_err_display(NETWIB_ERR_FUREGCOMP, NETWIB_ERR_ENCODETYPE_FULL));

  netwib_er(netwib_buf_init_ext_string("abcdef", &buf));
  netwib_er(netwib_buf_init_ext_string("abdef", &re));
  netwib_eir(netwib_buf_search_re(&buf, &re, NETWIB_TRUE, NULL),
             NETWIB_ERR_NOTFOUND);

  netwib_er(netwib_buf_search_re_test2("abcde", "abcde", NETWIB_TRUE, "abcde"));
  netwib_er(netwib_buf_search_re_test2("abc", "^abc",NETWIB_TRUE, "abc"));
  netwib_er(netwib_buf_search_re_test2("abc", "abc$",NETWIB_TRUE, "abc"));
  netwib_er(netwib_buf_search_re_test2("abc", "a.c",NETWIB_TRUE, "abc"));
  netwib_er(netwib_buf_search_re_test2("abc", "bc",NETWIB_TRUE, "bc"));
  netwib_er(netwib_buf_search_re_test2("abc", ".c",NETWIB_TRUE, "bc"));
  netwib_er(netwib_buf_search_re_test2("abc", "[zb]c",NETWIB_TRUE, "bc"));
  netwib_er(netwib_buf_search_re_test2("abc", "Bc",NETWIB_FALSE, "bc"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_search_regexp_test2(netwib_conststring str,
                                                 netwib_conststring strre,
                                                 netwib_uint32 numset,
                                                 netwib_conststring want0,
                                                 netwib_conststring want1,
                                                 netwib_conststring want2,
                                                 netwib_bool testnospace)
{
  netwib_buf buf, bufre;
  netwib_regexp re;

  netwib_er(netwib_buf_init_ext_string(str, &buf));
  if (testnospace) {
    buf.totalsize--; /* to force netwib_constbuf_ref_string error */
    buf.beginoffset++; /* to check if recomputation is ok */
  }
  netwib_er(netwib_buf_init_ext_string(strre, &bufre));
  if (testnospace) {
    bufre.totalsize--; /* to force netwib_constbuf_ref_string error */
  }
  netwib_er(netwib_buf_search_regexp(&buf, &bufre, NETWIB_TRUE, &re));

  if (re.numset != numset) {
    netwib_er(netwib_fmt_display("numset=%{uint32}!=wanted=%{uint32})\n",
                                 re.numset, numset));
    return(NETWIBTEST_ERR_6);
  }

  netwib_er(netwibtest_buf_content2(&re.array[0], want0));
  if (numset > 1) {
    netwib_er(netwibtest_buf_content2(&re.array[1], want1));
  }
  if (numset > 2) {
    netwib_er(netwibtest_buf_content2(&re.array[2], want2));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_search_regexp_test(void)
{
  puts("++++----\nTest of netwib_buf_search_regexp");

  netwib_er(netwib_buf_search_regexp_test2("abcdef", "abcdef",
                                           1, "abcdef", "", "",
                                           NETWIB_FALSE));
  netwib_er(netwib_buf_search_regexp_test2("abcdef", "ab(cd)ef",
                                           2, "abcdef", "cd", "",
                                           NETWIB_FALSE));
  netwib_er(netwib_buf_search_regexp_test2("abcdef", "ab(cd)(e)f",
                                           3, "abcdef", "cd", "e",
                                           NETWIB_FALSE));
  netwib_er(netwib_buf_search_regexp_test2("abcdef", "ab((cd)e)f",
                                           3, "abcdef", "cde", "cd",
                                           NETWIB_FALSE));

  netwib_er(netwib_buf_search_regexp_test2("abcdef", "bcdef",
                                           1, "bcdef", "", "",
                                           NETWIB_TRUE));
  netwib_er(netwib_buf_search_regexp_test2("abcdef", "b(cd)ef",
                                           2, "bcdef", "cd", "",
                                           NETWIB_TRUE));
  netwib_er(netwib_buf_search_regexp_test2("abcdef", "b(cd)(e)f",
                                           3, "bcdef", "cd", "e",
                                           NETWIB_TRUE));
  netwib_er(netwib_buf_search_regexp_test2("abcdef", "b((cd)e)f",
                                           3, "bcdef", "cde", "cd",
                                           NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_dat_regexp(void)
{
  puts("###########################################\nTest of dat/regexp");
  netwib_er(netwib_buf_search_re_test());
  netwib_er(netwib_buf_search_regexp_test());

  return(NETWIB_ERR_OK);
}

