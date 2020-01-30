#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err test_c_strcasecmp2(netwib_conststring s1,
                                     netwib_conststring s2,
                                     int wantedint)
{ int i;

 printf("s1=%s, s2=%s\n", s1, s2);
 i = netwib_c_strcasecmp(s1, s2);
 if (i != wantedint) {
   printf("i=%d != wanted(%d)\n", i, wantedint);
   return((netwib_err)2345);
 }
 return(NETWIB_ERR_OK);
}
static netwib_err test_c_strcasecmp(void)
{
  puts("++++----\nTest of function netwib_c_strcasecmp");
  netwib_er(test_c_strcasecmp2("abc", "abc", 0));
  netwib_er(test_c_strcasecmp2("abc", "aBc", 0));
  netwib_er(test_c_strcasecmp2("aBC", "abc", 0));
  netwib_er(test_c_strcasecmp2("abc", "abD", 'c' - 'd'));
  netwib_er(test_c_strcasecmp2("abc", "ab", 'c'));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_c_strncasecmp2(netwib_conststring s1,
                                      netwib_conststring s2,
                                      netwib_uint32 n,
                                      int wantedint)
{ int i;

 printf("s1=%s, s2=%s, n=%d\n", s1, s2, n);
 i = netwib_c_strncasecmp(s1, s2, n);
 if (i != wantedint) {
   printf("i=%d != wanted(%d)\n", i, wantedint);
   return((netwib_err)2345);
 }
 return(NETWIB_ERR_OK);
}
static netwib_err test_c_strncasecmp(void)
{
  puts("++++----\nTest of function netwib_c_strncasecmp");
  netwib_er(test_c_strncasecmp2("abc", "abc", 2, 0));
  netwib_er(test_c_strncasecmp2("abc", "aBc", 1, 0));
  netwib_er(test_c_strncasecmp2("abc", "aBc", 2, 0));
  netwib_er(test_c_strncasecmp2("abc", "aBc", 3, 0));
  netwib_er(test_c_strncasecmp2("abc", "aBc", 4, 0));
  netwib_er(test_c_strncasecmp2("abc", "abD", 1, 0));
  netwib_er(test_c_strncasecmp2("abc", "abD", 2, 0));
  netwib_er(test_c_strncasecmp2("abc", "abD", 3, 'c' - 'd'));
  netwib_er(test_c_strncasecmp2("abc", "abD", 4, 'c' - 'd'));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_c_memcasecmp2(netwib_conststring s1,
                                     netwib_conststring s2,
                                     netwib_uint32 n,
                                     int wantedint)
{ int i;

 printf("s1=%s, s2=%s, n=%d\n", s1, s2, n);
 i = netwib_c_memcasecmp((netwib_constdata)s1, (netwib_constdata)s2, n);
 if (i != wantedint) {
   printf("i=%d != wanted(%d)\n", i, wantedint);
   return((netwib_err)2345);
 }
 return(NETWIB_ERR_OK);
}
static netwib_err test_c_memcasecmp(void)
{
  puts("++++----\nTest of function netwib_c_memcasecmp");
  netwib_er(test_c_memcasecmp2("abc", "abc", 2, 0));
  netwib_er(test_c_memcasecmp2("abc", "aBc", 1, 0));
  netwib_er(test_c_memcasecmp2("abc", "aBc", 2, 0));
  netwib_er(test_c_memcasecmp2("abc", "aBc", 3, 0));
  netwib_er(test_c_memcasecmp2("abc", "aBc", 4, 0));
  netwib_er(test_c_memcasecmp2("abc", "abD", 1, 0));
  netwib_er(test_c_memcasecmp2("abc", "abD", 2, 0));
  netwib_er(test_c_memcasecmp2("abc", "abD", 3, 'c' - 'd'));
  netwib_er(test_c_memcasecmp2("abc", "abD", 4, 'c' - 'd'));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_c_strcasestr2(netwib_conststring s1,
                                     netwib_conststring s2,
                                     netwib_conststring wantedp)
{ netwib_string p;

 printf("s1=%s, s2=%s\n", s1, s2);
 p = netwib_c_strcasestr(s1, s2);
 if (wantedp == NULL) {
   if (p != NULL) {
     printf("p=%s != NULL\n", p);
     return((netwib_err)2345);
   }
 } else {
   if (p == NULL) {
     printf("p=NULL != wanted(%s)\n", wantedp);
     return((netwib_err)2345);
   }
   else if (strcmp(p, wantedp)) {
     printf("p=%s != wanted(%s)\n", p, wantedp);
     return((netwib_err)2345);
   }
 }
 return(NETWIB_ERR_OK);
}
static netwib_err test_c_strcasestr(void)
{
  puts("++++----\nTest of function netwib_c_strcasestr");
  netwib_er(test_c_strcasestr2("abc", "abc", "abc"));
  netwib_er(test_c_strcasestr2("ababCd", "abc", "abCd"));
  netwib_er(test_c_strcasestr2("ababcd", "abxc", NULL));
  netwib_er(test_c_strcasestr2("ababcabcd", "abc", "abcabcd"));
  netwib_er(test_c_strcasestr2("babcbabcabcaabcabcabcacabc",
                               "abcabcacab",
                               "abcabcacabc"));
  netwib_er(test_c_strcasestr2("abc", "aBc", "abc"));
  netwib_er(test_c_strcasestr2("ababCd", "aBc", "abCd"));
  netwib_er(test_c_strcasestr2("ababcd", "aBxc", NULL));
  netwib_er(test_c_strcasestr2("ababcabcd", "aBc", "abcabcd"));
  netwib_er(test_c_strcasestr2("babcbabcabcaabcabcabcAcabc",
                               "abcaBcaCab",
                               "abcabcAcabc"));
  netwib_er(test_c_strcasestr2("aaaaabcr", "abc", "abcr"));
  netwib_er(test_c_strcasestr2("abbcd", "aBxc", NULL));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_c_memmem2(netwib_conststring s1,
                                 netwib_conststring s2,
                                 netwib_conststring wantedp)
{ netwib_data p;

 printf("s1=%s, s2=%s\n", s1, s2);
 p = netwib_c_memmem((netwib_constdata)s1, netwib_c_strlen(s1),
                     (netwib_constdata)s2, netwib_c_strlen(s2));
 if (wantedp == NULL) {
   if (p != NULL) {
     printf("p=%s != NULL\n", (netwib_string)p);
     return((netwib_err)2345);
   }
 } else {
   if (p == NULL) {
     printf("p=NULL != wanted(%s)\n", wantedp);
     return((netwib_err)2345);
   }
   else if (strcmp((netwib_conststring)p, wantedp)) {
     printf("p=%s != wanted(%s)\n", (netwib_string)p, wantedp);
     return((netwib_err)2345);
   }
 }
 return(NETWIB_ERR_OK);
}
static netwib_err test_c_memmem(void)
{
  puts("++++----\nTest of function netwib_c_memmem");

  netwib_er(test_c_memmem2("abc", "abc", "abc"));
  netwib_er(test_c_memmem2("ababCd", "abC", "abCd"));
  netwib_er(test_c_memmem2("ababcd", "abxc", NULL));
  netwib_er(test_c_memmem2("ababcabcd", "abc", "abcabcd"));
  netwib_er(test_c_memmem2("babcbabcabcaabcabcabcacabc",
                           "abcabcacab",
                           "abcabcacabc"));
  netwib_er(test_c_memmem2("aaaaabcr", "abc", "abcr"));
  netwib_er(test_c_memmem2("abbcd", "abxc", NULL));
  netwib_er(test_c_memmem2("abdc", "abc", NULL));

  netwib_er(test_c_memmem2("ab", "abc", NULL));
  netwib_er(test_c_memmem2("adc", "abc", NULL));
  netwib_er(test_c_memmem2("adc", "abce", NULL));
  netwib_er(test_c_memmem2("adc", "", "adc"));
  netwib_er(test_c_memmem2("", "abce", NULL));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_c_memcasemem2(netwib_conststring s1,
                                     netwib_conststring s2,
                                     netwib_conststring wantedp)
{ netwib_data p;

 printf("s1=%s, s2=%s\n", s1, s2);
 p = netwib_c_memcasemem((netwib_constdata)s1, netwib_c_strlen(s1),
                         (netwib_constdata)s2, netwib_c_strlen(s2));
 if (wantedp == NULL) {
   if (p != NULL) {
     printf("p=%s != NULL\n", (netwib_string)p);
     return((netwib_err)2345);
   }
 } else {
   if (p == NULL) {
     printf("p=NULL != wanted(%s)\n", wantedp);
     return((netwib_err)2345);
   }
   else if (strcmp((netwib_conststring)p, wantedp)) {
     printf("p=%s != wanted(%s)\n", (netwib_string)p, wantedp);
     return((netwib_err)2345);
   }
 }
 return(NETWIB_ERR_OK);
}
static netwib_err test_c_memcasemem(void)
{
  puts("++++----\nTest of function netwib_c_memcasemem");
  netwib_er(test_c_memcasemem2("abc", "abc", "abc"));
  netwib_er(test_c_memcasemem2("ababCd", "abC", "abCd"));
  netwib_er(test_c_memcasemem2("ababcd", "abxc", NULL));
  netwib_er(test_c_memcasemem2("ababcabcd", "abc", "abcabcd"));
  netwib_er(test_c_memcasemem2("babcbabcabcaabcabcabcacabc",
                               "abcabcacab",
                               "abcabcacabc"));
  netwib_er(test_c_memcasemem2("aaaaabcr", "abc", "abcr"));
  netwib_er(test_c_memcasemem2("abbcd", "abxc", NULL));
  netwib_er(test_c_memcasemem2("abdc", "abc", NULL));
  netwib_er(test_c_memcasemem2("ababcabcd", "aBc", "abcabcd"));
  netwib_er(test_c_memcasemem2("ababCabcd", "aBc", "abCabcd"));

  netwib_er(test_c_memcasemem2("ab", "abc", NULL));
  netwib_er(test_c_memcasemem2("adc", "abc", NULL));
  netwib_er(test_c_memcasemem2("adc", "abce", NULL));
  netwib_er(test_c_memcasemem2("adc", "", "adc"));
  netwib_er(test_c_memcasemem2("", "abce", NULL));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_dat_c(void)
{
  puts("###########################################\nTest of dat/c");
  netwib_er(test_c_strcasecmp());
  netwib_er(test_c_strncasecmp());
  netwib_er(test_c_memcasecmp());
  netwib_er(test_c_strcasestr());
  netwib_er(test_c_memmem());
  netwib_er(test_c_memcasemem());

  return(NETWIB_ERR_OK);
}
