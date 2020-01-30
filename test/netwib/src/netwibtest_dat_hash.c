#include "netwibtest.h"

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_bool test_hash_randombug = NETWIB_FALSE;
static netwib_uint32 test_hash_randombug_n;

/*-------------------------------------------------------------*/
static netwib_err netwib_er_hr(netwib_err err)
{
  if (err == NETWIB_ERR_OK)
    return(NETWIB_ERR_OK);
  if (test_hash_randombug && err == NETWIBTEST_ERR_3) {
    puts("Random error ignored");
    return(NETWIB_ERR_OK);
  }
  netwib_err_display(err, NETWIB_ERR_ENCODETYPE_FULL);
  exit(err);
  return(NETWIB_ERR_LOINTERNALERROR);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_eir_hr(netwib_err err,
                                netwib_err wantederr)
{
  if (err == wantederr)
    return(NETWIB_ERR_OK);
  if (test_hash_randombug && err == NETWIBTEST_ERR_3) {
    puts("Random error ignored");
    return(NETWIB_ERR_OK);
  }
  netwib_err_display(err, NETWIB_ERR_ENCODETYPE_FULL);
  exit(err);
  return(NETWIB_ERR_LOINTERNALERROR);
}

/*-------------------------------------------------------------*/
static netwib_err test_hash_generaterandomerror(void)
{
  static netwib_uint32 i = 0;
  if (test_hash_randombug) {
    i++;
    if (!(i % test_hash_randombug_n)) {
      netwib_er(netwib_fmt_display("Generate random error %{uint32}\n", i));
      return(NETWIBTEST_ERR_3);
    }
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_netwib_hash_close(netwib_hash **pphash,
                                         netwib_bool eraseitems)
{
  netwib_err ret;

  if (test_hash_randombug) {
    test_hash_randombug = NETWIB_FALSE;
    ret = netwib_hash_close(pphash, eraseitems);
    test_hash_randombug = NETWIB_TRUE;
    return(ret);
  } else {
    netwib_er(netwib_hash_close(pphash, eraseitems));
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 code;
  netwib_string msg;
} test_hash_log;

/*-------------------------------------------------------------*/
static netwib_err test_hash_log_check1(netwib_ptr pitem,
                                       netwib_uint32 code,
                                       netwib_conststring msg)
{
  test_hash_log *plog;

  plog = (test_hash_log *)pitem;

  if (code != plog->code || netwib_c_strcmp(msg, plog->msg)) {
    netwib_er(netwib_fmt_display("code=%{uint32} msg=%s\n",
                                 plog->code, plog->msg));
    netwib_er(netwib_fmt_display("!= code=%{uint32} msg=%s\n", code, msg));
    return(NETWIBTEST_ERR_9);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_hash_log_checkall3(netwib_conststring str,
                                          netwib_uint32 *psum)
{
  netwib_conststring p;
  netwib_uint32 sum, sum2;

  p = str;
  sum = 0;
  sum2 = 0;
  while(*p != '\0') {
    if (*p == ';') {
      sum2 += sum;
      sum = 0;
    } else {
      sum = 33*sum + *p;
    }
    p++;
  }
  *psum = sum + sum2;

  return(NETWIB_ERR_OK);
}
static netwib_err test_hash_log_checkall2(netwib_conststring str,
                                          netwib_conststring wanted,
                                          netwib_bool *peq)
{
  netwib_uint32 sum1, sum2;

  /* this is not correct. This is just a quick hack (because of the
     internalrandom, items can be mixed/reordered; it would be too long
     to test it correctly). So I choose to only test if characters
     are presents in both strings. */
  netwib_er(test_hash_log_checkall3(str, &sum1));
  netwib_er(test_hash_log_checkall3(wanted, &sum2));
  *peq = (sum1==sum2)?NETWIB_TRUE:NETWIB_FALSE;

  return(NETWIB_ERR_OK);
}
static netwib_err test_hash_log_checkall(netwib_hash *phash,
                                         netwib_conststring wanted,
                                         netwib_bool *pwillcauseerror)
{
  netwib_buf buf, key;
  netwib_ptr ptr;
  netwib_string str;
  netwib_bool needsemicolon, eq;
  netwib_err ret;
  test_hash_log *plog;
  netwib_hash_index *phashindex;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_hash_index_init(phash, &phashindex));
  netwib_er(netwib_hash_index_ctl_set_rewind(phashindex));
  needsemicolon = NETWIB_FALSE;
  while (NETWIB_TRUE) {
    netwib_er(netwib_buf_init_mallocdefault(&key));
    ret = netwib_hash_index_next(phashindex, &key, &ptr);
    if (ret == NETWIB_ERR_DATAEND) {
      netwib_er(netwib_buf_close(&key));
      break;
    } else if (ret != NETWIB_ERR_OK) {
      netwib_er(netwib_buf_close(&key));
      netwib_er(netwib_buf_close(&buf));
      return(ret);
    }
    if (needsemicolon) {
      netwib_er(netwib_buf_append_byte(';', &buf));
    }
    needsemicolon = NETWIB_TRUE;
    netwib_er(netwib_buf_append_buf(&key,&buf));
    netwib_er(netwib_buf_close(&key));
    plog = (test_hash_log *)ptr;
    netwib_er(netwib_buf_append_fmt(&buf, "=%{uint32},%s", plog->code,
                                    plog->msg));
  }
  netwib_er(netwib_buf_ref_string(&buf, &str));

  eq = NETWIB_FALSE;
  if (pwillcauseerror != NULL) *pwillcauseerror = NETWIB_FALSE;
  if (!netwib_c_strcmp(str, wanted)) {
    eq = NETWIB_TRUE;
  } else {
    netwib_er(test_hash_log_checkall2(str, wanted, &eq));
    if (eq) {
      /* because they are not really equal */
      if (pwillcauseerror != NULL) *pwillcauseerror = NETWIB_TRUE;
    }
  }
  if (!eq) {
    if (test_hash_randombug) {
      puts("Consequence of a random error ignored");
    } else {
      netwib_er(netwib_fmt_display("Obtain : %s\n", str));
      netwib_er(netwib_fmt_display("Wanted : %s\n", wanted));
      netwib_er(netwib_buf_close(&buf));
      return(NETWIBTEST_ERR_9);
    }
  }
  netwib_er(netwib_hash_index_close(&phashindex));
  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_hash_log_create(netwib_uint32 code,
                                       netwib_conststring msg,
                                       netwib_ptr *ppitem)
{
  netwib_ptr ptr;
  test_hash_log *plog;

  netwib_er(netwib_ptr_malloc(sizeof(test_hash_log), &ptr));
  plog = (test_hash_log *)ptr;
  plog->code = code;
  netwib_er(netwib_ptr_malloc(netwib_c_strlen(msg)+1, &ptr));
  netwib_c_strcpy((netwib_string)ptr, msg);
  plog->msg = (netwib_string)ptr;

  *ppitem = (netwib_ptr *)plog;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_hash_log_erase(netwib_ptr pitem);
static netwib_err test_hash_log_add(netwib_hash *phash,
                                    netwib_conststring key,
                                    netwib_uint32 code,
                                    netwib_conststring msg)
{
  netwib_ptr pitem;
  netwib_buf bufkey;
  netwib_err ret;

  netwib_er_hr(test_hash_log_create(code, msg, &pitem));
  netwib_er(netwib_buf_init_ext_string(key, &bufkey));
  ret = netwib_hash_add(phash, &bufkey, pitem, NETWIB_TRUE);
  if (ret == NETWIBTEST_ERR_3) {
    test_hash_randombug = NETWIB_FALSE;
    test_hash_log_erase(pitem);
    test_hash_randombug = NETWIB_TRUE;
  }

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err test_hash_log_erase(netwib_ptr pitem)
{
  test_hash_log *plog;

  netwib_er(test_hash_generaterandomerror());

  plog = (test_hash_log *)pitem;

  netwib_er(netwib_ptr_free((netwib_ptr *)&plog->msg));
  netwib_er(netwib_ptr_free((netwib_ptr *)&plog));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_hash_log_criteria(netwib_constbuf *pkey,
                                         netwib_constptr pitem,
                                         netwib_ptr pinfos,
                                         netwib_bool *pbool)
{
  const test_hash_log *plog = (const test_hash_log *)pitem;
  netwib_uint32 *pui = (netwib_uint32 *) pinfos;
  netwib_bool b;
  netwib_cmp cmp;

  netwib_er(test_hash_generaterandomerror());

  b = NETWIB_FALSE;
  if (plog->code == *pui) {
    b = NETWIB_TRUE;
  }
  netwib_er(netwib_buf_cmp_string(pkey, "rem", &cmp));
  if (cmp == NETWIB_CMP_EQ) {
    b = NETWIB_TRUE;
  }
  netwib_er(netwib_fmt_display("%{uint32} %{buf} -> %{uint32}\n",
                               plog->code, pkey, b));

  if (pbool != NULL) *pbool = b;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_hash_log_duplicate(netwib_constptr pitem,
                                          netwib_ptr *pdupofitem)
{
  const test_hash_log *plog = (const test_hash_log *)pitem;

  netwib_er(test_hash_generaterandomerror());

  netwib_er_hr(test_hash_log_create(plog->code, plog->msg, pdupofitem));
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwib_hash_init_test(void)
{
  netwib_hash *phash;

  puts("++++----\nTest of netwib_hash_init");
  netwib_eir(netwib_hash_init(NULL, NULL, NULL), NETWIB_ERR_PANULLPTR);

  netwib_er_hr(netwib_hash_init(NULL, NULL,&phash));
  netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_hash_close_test(void)
{
  netwib_hash *phash;

  puts("++++----\nTest of netwib_hash_close");
  netwib_eir_hr(test_netwib_hash_close(NULL, NETWIB_TRUE), NETWIB_ERR_PANULLPTR);

  netwib_er_hr(netwib_hash_init(NULL, NULL, &phash));
  netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_TRUE));

  netwib_er_hr(netwib_hash_init(NULL, NULL, &phash));
  netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_FALSE));

  netwib_er_hr(netwib_hash_init(&test_hash_log_erase, NULL, &phash));
  netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_TRUE));

  netwib_er_hr(netwib_hash_init(&test_hash_log_erase, NULL, &phash));
  netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_FALSE));

  netwib_er_hr(netwib_hash_init(&test_hash_log_erase, &test_hash_log_duplicate,
                               &phash));
  netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_TRUE));

  netwib_er_hr(netwib_hash_init(&test_hash_log_erase, &test_hash_log_duplicate,
                               &phash));
  netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_FALSE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_hash_ctl_set_test(void)
{
  netwib_hash *phash;

  puts("++++----\nTest of netwib_hash_ctl_set");

  netwib_er_hr(netwib_hash_init(NULL, NULL, &phash));
  netwib_eir(netwib_hash_ctl_set(phash,
                                 NETWIB_HASH_CTLTYPE_COUNT,
                                 NULL, 0),
             NETWIB_ERR_PAINVALIDTYPE);
  netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_hash_ctl_get_test(void)
{
  netwib_hash *phash;
  netwib_uint32 ui;

  puts("++++----\nTest of netwib_hash_ctl_get");
  netwib_eir(netwib_hash_ctl_get_count(NULL, NULL), NETWIB_ERR_PANULLPTR);

  netwib_er_hr(netwib_hash_init(NULL, NULL, &phash));
  netwib_er_hr(netwib_hash_ctl_get_count(phash, &ui));
  if (ui != 0) {
    return(NETWIBTEST_ERR_9);
  }
  netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_hash_add_test(void)
{
  netwib_hash *phash;

  puts("++++----\nTest of netwib_hash_add");
  netwib_er_hr(netwib_hash_init(&test_hash_log_erase, &test_hash_log_duplicate,
                               &phash));

  netwib_er(test_hash_log_checkall(phash, "", NULL));
  netwib_er_hr(test_hash_log_add(phash, "k1", 1, "one"));
  netwib_er(test_hash_log_checkall(phash, "k1=1,one", NULL));
  netwib_er_hr(test_hash_log_add(phash, "k2", 2, "two"));
  netwib_er(test_hash_log_checkall(phash, "k1=1,one;k2=2,two", NULL));
  netwib_er_hr(test_hash_log_add(phash, "k3", 3, "three"));
  netwib_er(test_hash_log_checkall(phash, "k1=1,one;k2=2,two;k3=3,three",
                                   NULL));
  netwib_er_hr(test_hash_log_add(phash, "k1", 11, "eleven"));
  netwib_er(test_hash_log_checkall(phash, "k1=11,eleven;k2=2,two;k3=3,three",
                                   NULL));
  netwib_er_hr(test_hash_log_add(phash, "k2", 11, "eleven"));
  netwib_er(test_hash_log_checkall(phash,
                                   "k1=11,eleven;k2=11,eleven;k3=3,three",
                                   NULL));
  netwib_er_hr(test_hash_log_add(phash, "k2", 11, "eln"));
  netwib_er(test_hash_log_checkall(phash, "k1=11,eleven;k2=11,eln;k3=3,three",
                                   NULL));

  netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_hash_add_hash_criteria_test(void)
{
  netwib_hash *phash1, *phash2;
  netwib_uint32 ui;

  puts("++++----\nTest of netwib_hash_add_hash_criteria");
  netwib_er_hr(netwib_hash_init(&test_hash_log_erase, &test_hash_log_duplicate,
                               &phash1));
  netwib_er_hr(netwib_hash_init(&test_hash_log_erase, &test_hash_log_duplicate,
                               &phash2));

  netwib_er_hr(test_hash_log_add(phash1, "k1", 1, "one"));
  netwib_er_hr(test_hash_log_add(phash1, "k2", 2, "two"));
  netwib_er_hr(test_hash_log_add(phash1, "k3", 3, "three"));
  netwib_er(test_hash_log_checkall(phash1, "k1=1,one;k2=2,two;k3=3,three",
                                   NULL));
  netwib_er_hr(test_hash_log_add(phash2, "l1", 1, "one"));
  netwib_er_hr(test_hash_log_add(phash2, "k2", 2, "twXo"));
  netwib_er_hr(test_hash_log_add(phash2, "rem", 3, "three"));
  netwib_er(test_hash_log_checkall(phash2, "k2=2,twXo;rem=3,three;l1=1,one",
                                   NULL));

  netwib_er_hr(netwib_hash_add_hash_all(phash1, phash2, NETWIB_TRUE));
  netwib_er(test_hash_log_checkall(phash1, "rem=3,three;k1=1,one;l1=1,one;k2=2,twXo;k3=3,three", NULL));
  netwib_er(test_hash_log_checkall(phash2, "k2=2,twXo;rem=3,three;l1=1,one",
                                   NULL));
  ui=7;

  netwib_er_hr(test_hash_log_add(phash2, "l1", 1, "one_"));
  netwib_er_hr(test_hash_log_add(phash2, "k2", 2, "twoo_"));
  netwib_er_hr(test_hash_log_add(phash2, "rem", 3, "three_"));
  netwib_er(test_hash_log_checkall(phash2, "rem=3,three_;k2=2,twoo_;l1=1,one_",
                                   NULL));
  ui = 1;
  netwib_er_hr(netwib_hash_add_hash_criteria(phash1, phash2,
                                            &test_hash_log_criteria, &ui,
                                            NETWIB_TRUE));
  netwib_er(test_hash_log_checkall(phash1, "rem=3,three_;k1=1,one;l1=1,one_;k2=2,twXo;k3=3,three", NULL));
  netwib_er(test_hash_log_checkall(phash2, "rem=3,three_;k2=2,twoo_;l1=1,one_",
                                   NULL));

  netwib_er_hr(test_netwib_hash_close(&phash1, NETWIB_TRUE));
  netwib_er_hr(test_netwib_hash_close(&phash2, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_hash_contains_test(void)
{
  netwib_hash *phash;
  netwib_buf key;
  netwib_bool b;

  puts("++++----\nTest of netwib_hash_contains");
  netwib_er_hr(netwib_hash_init(&test_hash_log_erase, &test_hash_log_duplicate,
                               &phash));

  netwib_er(test_hash_log_checkall(phash, "", NULL));
  netwib_er_hr(test_hash_log_add(phash, "k1", 1, "one"));
  netwib_er_hr(test_hash_log_add(phash, "k2", 2, "two"));
  netwib_er_hr(test_hash_log_add(phash, "k3", 3, "three"));
  netwib_er(test_hash_log_checkall(phash, "k1=1,one;k2=2,two;k3=3,three",
                                   NULL));

  netwib_er(netwib_buf_init_ext_string("k2", &key));
  netwib_er_hr(netwib_hash_contains(phash, &key, &b));
  if (!b) {
    return(NETWIBTEST_ERR_4);
  }

  netwib_er(netwib_buf_init_ext_string("k2rrr", &key));
  netwib_er_hr(netwib_hash_contains(phash, &key, &b));
  if (b) {
    return(NETWIBTEST_ERR_4);
  }

  netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_hash_value_test(void)
{
  netwib_hash *phash;
  netwib_buf key;
  netwib_ptr pitem;

  puts("++++----\nTest of netwib_hash_value");
  netwib_er_hr(netwib_hash_init(&test_hash_log_erase, &test_hash_log_duplicate,
                               &phash));

  netwib_er(test_hash_log_checkall(phash, "", NULL));
  netwib_er_hr(test_hash_log_add(phash, "k1", 1, "one"));
  netwib_er_hr(test_hash_log_add(phash, "k2", 2, "two"));
  netwib_er_hr(test_hash_log_add(phash, "k3", 3, "three"));
  netwib_er(test_hash_log_checkall(phash, "k1=1,one;k2=2,two;k3=3,three",
                                   NULL));

  netwib_er(netwib_buf_init_ext_string("k2", &key));
  netwib_er_hr(netwib_hash_value(phash, &key, &pitem));
  netwib_er(test_hash_log_check1(pitem, 2, "two"));
  netwib_er(netwib_buf_init_ext_string("k3", &key));
  netwib_er_hr(netwib_hash_value(phash, &key, &pitem));
  netwib_er(test_hash_log_check1(pitem, 3, "three"));

  netwib_er(netwib_buf_init_ext_string("k4", &key));
  netwib_eir_hr(netwib_hash_value(phash, &key, &pitem), NETWIB_ERR_NOTFOUND);

  netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_hash_del_test(void)
{
  netwib_hash *phash;
  netwib_buf key;

  puts("++++----\nTest of netwib_hash_del");
  netwib_er_hr(netwib_hash_init(&test_hash_log_erase, &test_hash_log_duplicate,
                               &phash));

  netwib_er(test_hash_log_checkall(phash, "", NULL));
  netwib_er_hr(test_hash_log_add(phash, "k1", 1, "one"));
  netwib_er_hr(test_hash_log_add(phash, "k2", 2, "two"));
  netwib_er_hr(test_hash_log_add(phash, "k3", 3, "three"));
  netwib_er(test_hash_log_checkall(phash, "k1=1,one;k2=2,two;k3=3,three",
                                   NULL));

  netwib_er(netwib_buf_init_ext_string("k2", &key));
  netwib_er_hr(netwib_hash_del(phash, &key, NETWIB_TRUE));
  netwib_er(test_hash_log_checkall(phash, "k1=1,one;k3=3,three", NULL));

  netwib_er(netwib_buf_init_ext_string("k4", &key));
  netwib_eir_hr(netwib_hash_del(phash, &key, NETWIB_TRUE),
               NETWIB_ERR_NOTFOUND);
  netwib_er(test_hash_log_checkall(phash, "k1=1,one;k3=3,three", NULL));

  netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_hash_del_criteria_test(void)
{
  netwib_hash *phash;
  netwib_uint32 ui;

  puts("++++----\nTest of netwib_hash_del_criteria");
  netwib_er_hr(netwib_hash_init(&test_hash_log_erase, &test_hash_log_duplicate,
                               &phash));

  netwib_er(test_hash_log_checkall(phash, "", NULL));
  netwib_er_hr(test_hash_log_add(phash, "k1", 1, "one"));
  netwib_er_hr(test_hash_log_add(phash, "k2", 2, "two"));
  netwib_er_hr(test_hash_log_add(phash, "k3", 3, "three"));
  netwib_er(test_hash_log_checkall(phash, "k1=1,one;k2=2,two;k3=3,three",
                                   NULL));

  ui = 3;
  netwib_er_hr(netwib_hash_del_criteria(phash, &test_hash_log_criteria,
                                       &ui, NETWIB_TRUE));
  netwib_er(test_hash_log_checkall(phash, "k1=1,one;k2=2,two", NULL));

  netwib_er_hr(test_hash_log_add(phash, "rem", 12, "twelve"));
  netwib_er(test_hash_log_checkall(phash, "rem=12,twelve;k1=1,one;k2=2,two",
                                   NULL));
  ui = 3;
  netwib_er_hr(netwib_hash_del_criteria(phash, &test_hash_log_criteria,
                                       &ui, NETWIB_TRUE));
  netwib_er(test_hash_log_checkall(phash, "k1=1,one;k2=2,two", NULL));

  netwib_er_hr(netwib_hash_del_all(phash, NETWIB_TRUE));
  netwib_er(test_hash_log_checkall(phash, "", NULL));

  netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwib_hash_index_init_test(void)
{
  netwib_hash_index *phashindex;
  netwib_hash *phash;

  puts("++++----\nTest of netwib_hash_index_init");
  netwib_er_hr(netwib_hash_init(NULL, NULL, &phash));
  netwib_er_hr(netwib_hash_index_init(phash, &phashindex));
  netwib_er_hr(netwib_hash_index_close(&phashindex));
  netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_hash_index_close_test(void)
{
  puts("++++----\nTest of netwib_hash_index_close");
  puts("Redundant with netwib_hash_index_init");

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_hash_index_ctl_set_test(void)
{
  netwib_hash_index *phashindex, *phashindex2;
  netwib_hash *phash;
  netwib_buf key;
  netwib_ptr pitem;
  netwib_bool willcauseerror;

  puts("++++----\nTest of netwib_hash_index_ctl_set");
  netwib_er_hr(netwib_hash_init(&test_hash_log_erase, &test_hash_log_duplicate,
                               &phash));
  netwib_er_hr(netwib_hash_index_init(phash, &phashindex));

  netwib_er_hr(test_hash_log_add(phash, "k1", 1, "one"));
  netwib_er_hr(test_hash_log_add(phash, "k2", 2, "two"));
  netwib_er_hr(test_hash_log_add(phash, "rem", 12, "twelve"));
  netwib_er(test_hash_log_checkall(phash, "rem=12,twelve;k1=1,one;k2=2,two",
                                   &willcauseerror));
  if (willcauseerror) {
    puts("Ignore this test : it will cause errors");
    netwib_er_hr(netwib_hash_index_close(&phashindex));
    netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_TRUE));
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_hash_index_ctl_set_rewind(phashindex));
  netwib_er(netwib_hash_index_ctl_set_rewind(phashindex));

  netwib_er(netwib_buf_init_mallocdefault(&key));
  netwib_er_hr(netwib_hash_index_next(phashindex, &key, &pitem));
  netwib_er(netwibtest_buf_content2(&key, "rem"));
  netwib_er(test_hash_log_check1(pitem, 12, "twelve"));
  netwib_er(netwib_buf_close(&key));
  netwib_er_hr(netwib_hash_index_init(phash, &phashindex2));
  netwib_er(netwib_hash_index_ctl_set_index(phashindex2, phashindex));
  netwib_er(netwib_buf_init_mallocdefault(&key));
  netwib_er_hr(netwib_hash_index_next(phashindex2, &key, &pitem));
  netwib_er(netwibtest_buf_content2(&key, "k1"));
  netwib_er(test_hash_log_check1(pitem, 1, "one"));
  netwib_er(netwib_buf_close(&key));
  netwib_er_hr(netwib_hash_index_close(&phashindex2));
  netwib_er(netwib_hash_index_ctl_set_rewind(phashindex));
  netwib_er(netwib_buf_init_mallocdefault(&key));
  netwib_er_hr(netwib_hash_index_next(phashindex, &key, &pitem));
  netwib_er(netwibtest_buf_content2(&key, "rem"));
  netwib_er(test_hash_log_check1(pitem, 12, "twelve"));
  netwib_er(netwib_buf_close(&key));

  netwib_er_hr(netwib_hash_index_close(&phashindex));
  netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_TRUE));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_hash_index_ctl_get_test(void)
{
  netwib_hash_index *phashindex;
  netwib_hash *phash;

  puts("++++----\nTest of netwib_hash_index_ctl_get");
  netwib_er_hr(netwib_hash_init(&test_hash_log_erase, &test_hash_log_duplicate,
                               &phash));
  netwib_er_hr(netwib_hash_index_init(phash, &phashindex));

  netwib_eir(netwib_hash_index_ctl_get(phashindex,
                                       NETWIB_HASH_INDEX_CTLTYPE_REWIND,
                                       NULL, NULL),
             NETWIB_ERR_PAINVALIDTYPE);
  netwib_eir(netwib_hash_index_ctl_get(phashindex,
                                       NETWIB_HASH_INDEX_CTLTYPE_INDEX,
                                       NULL, NULL),
             NETWIB_ERR_PAINVALIDTYPE);

  netwib_er_hr(netwib_hash_index_close(&phashindex));
  netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_TRUE));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_hash_index_next_criteria_test(void)
{
  netwib_hash_index *phashindex;
  netwib_hash *phash;
  netwib_uint32 ui;
  netwib_buf key;
  netwib_ptr pitem;
  netwib_bool willcauseerror;

  puts("++++----\nTest of netwib_hash_index_next_criteria");
  netwib_er_hr(netwib_hash_init(&test_hash_log_erase, &test_hash_log_duplicate,
                               &phash));
  netwib_er_hr(netwib_hash_index_init(phash, &phashindex));

  netwib_er_hr(test_hash_log_add(phash, "k1", 1, "one"));
  netwib_er_hr(test_hash_log_add(phash, "k2", 2, "two"));
  netwib_er_hr(test_hash_log_add(phash, "rem", 12, "twelve"));
  netwib_er(test_hash_log_checkall(phash, "rem=12,twelve;k1=1,one;k2=2,two",
                                   &willcauseerror));
  if (willcauseerror) {
    puts("Ignore this test : it will cause errors");
    netwib_er_hr(netwib_hash_index_close(&phashindex));
    netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_TRUE));
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_hash_index_ctl_set_rewind(phashindex));
  netwib_er(netwib_buf_init_mallocdefault(&key));
  netwib_er_hr(netwib_hash_index_next(phashindex, &key, &pitem));
  netwib_er(netwibtest_buf_content2(&key, "rem"));
  netwib_er(test_hash_log_check1(pitem, 12, "twelve"));
  netwib_er(netwib_buf_close(&key));
  netwib_er(netwib_buf_init_mallocdefault(&key));
  netwib_er_hr(netwib_hash_index_next(phashindex, &key, &pitem));
  netwib_er(netwibtest_buf_content2(&key, "k1"));
  netwib_er(test_hash_log_check1(pitem, 1, "one"));
  netwib_er(netwib_buf_close(&key));
  netwib_er(netwib_buf_init_mallocdefault(&key));
  netwib_er_hr(netwib_hash_index_next(phashindex, &key, &pitem));
  netwib_er(netwibtest_buf_content2(&key, "k2"));
  netwib_er(test_hash_log_check1(pitem, 2, "two"));
  netwib_er(netwib_buf_close(&key));
  netwib_eir_hr(netwib_hash_index_next(phashindex, &key, &pitem),
               NETWIB_ERR_DATAEND);

  if (test_hash_randombug) {
    netwib_er_hr(netwib_hash_index_close(&phashindex));
    netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_TRUE));
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_hash_index_ctl_set_rewind(phashindex));
  ui = 2;
  netwib_er(netwib_buf_init_mallocdefault(&key));
  netwib_er(netwib_hash_index_next_criteria(phashindex, &test_hash_log_criteria,
                                            &ui, &key, &pitem));
  netwib_er(netwibtest_buf_content2(&key, "rem"));
  netwib_er(test_hash_log_check1(pitem, 12, "twelve"));
  netwib_er(netwib_buf_close(&key));
  netwib_er(netwib_buf_init_mallocdefault(&key));
  netwib_er(netwib_hash_index_next_criteria(phashindex, &test_hash_log_criteria,
                                            &ui, &key, &pitem));
  netwib_er(netwibtest_buf_content2(&key, "k2"));
  netwib_er(test_hash_log_check1(pitem, 2, "two"));
  netwib_er(netwib_buf_close(&key));
  netwib_eir_hr(netwib_hash_index_next(phashindex, &key, &pitem),
               NETWIB_ERR_DATAEND);

  netwib_er_hr(netwib_hash_index_close(&phashindex));
  netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_hash_index_this_value_test(void)
{
  netwib_hash_index *phashindex;
  netwib_hash *phash;
  netwib_buf key;
  netwib_ptr pitem;
  netwib_bool willcauseerror;

  puts("++++----\nTest of netwib_hash_index_this_value");
  netwib_er_hr(netwib_hash_init(&test_hash_log_erase, &test_hash_log_duplicate,
                               &phash));
  netwib_er_hr(netwib_hash_index_init(phash, &phashindex));

  netwib_er_hr(test_hash_log_add(phash, "k1", 1, "one"));
  netwib_er_hr(test_hash_log_add(phash, "k2", 2, "two"));
  netwib_er_hr(test_hash_log_add(phash, "rem", 12, "twelve"));
  netwib_er(test_hash_log_checkall(phash, "rem=12,twelve;k1=1,one;k2=2,two",
                                   &willcauseerror));
  if (willcauseerror) {
    puts("Ignore this test : it will cause errors");
    netwib_er_hr(netwib_hash_index_close(&phashindex));
    netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_TRUE));
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_buf_init_mallocdefault(&key));
  netwib_er_hr(netwib_hash_index_next(phashindex, NULL, NULL));
  netwib_er_hr(netwib_hash_index_this_value(phashindex, &key, &pitem));
  netwib_er(netwibtest_buf_content2(&key, "rem"));
  netwib_er(test_hash_log_check1(pitem, 12, "twelve"));
  netwib_er(netwib_buf_close(&key));

  netwib_er_hr(netwib_hash_index_close(&phashindex));
  netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_hash_index_this_del_test(void)
{
  netwib_hash_index *phashindex;
  netwib_hash *phash;
  netwib_buf key;
  netwib_ptr pitem;
  netwib_bool willcauseerror;

  puts("++++----\nTest of netwib_hash_index_this_del");
  netwib_er_hr(netwib_hash_init(&test_hash_log_erase, &test_hash_log_duplicate,
                               &phash));
  netwib_er_hr(netwib_hash_index_init(phash, &phashindex));

  netwib_er_hr(test_hash_log_add(phash, "k1", 1, "one"));
  netwib_er_hr(test_hash_log_add(phash, "k2", 2, "two"));
  netwib_er_hr(test_hash_log_add(phash, "rem", 12, "twelve"));
  netwib_er(test_hash_log_checkall(phash, "rem=12,twelve;k1=1,one;k2=2,two",
                                   &willcauseerror));
  if (willcauseerror) {
    puts("Ignore this test : it will cause errors");
    netwib_er_hr(netwib_hash_index_close(&phashindex));
    netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_TRUE));
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_buf_init_mallocdefault(&key));
  netwib_er_hr(netwib_hash_index_next(phashindex, NULL, NULL));
  netwib_er_hr(netwib_hash_index_this_del(phashindex, NETWIB_TRUE));
  netwib_er_hr(netwib_hash_index_next(phashindex, &key, &pitem));
  netwib_er(netwibtest_buf_content2(&key, "k1"));
  netwib_er(test_hash_log_check1(pitem, 1, "one"));
  netwib_er(netwib_buf_close(&key));

  netwib_er_hr(netwib_hash_index_close(&phashindex));
  netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_hash_index_this_replace_test(void)
{
  netwib_hash_index *phashindex;
  netwib_hash *phash;
  netwib_buf key;
  netwib_ptr pitem;
  netwib_err ret;
  netwib_bool willcauseerror;

  puts("++++----\nTest of netwib_hash_index_this_replace");
  netwib_er_hr(netwib_hash_init(&test_hash_log_erase, &test_hash_log_duplicate,
                               &phash));
  netwib_er_hr(netwib_hash_index_init(phash, &phashindex));

  netwib_er_hr(test_hash_log_add(phash, "k1", 1, "one"));
  netwib_er_hr(test_hash_log_add(phash, "k2", 2, "two"));
  netwib_er_hr(test_hash_log_add(phash, "rem", 12, "twelve"));
  netwib_er(test_hash_log_checkall(phash, "rem=12,twelve;k1=1,one;k2=2,two",
                                   &willcauseerror));
  if (willcauseerror) {
    puts("Ignore this test : it will cause errors");
    netwib_er_hr(netwib_hash_index_close(&phashindex));
    netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_TRUE));
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_buf_init_mallocdefault(&key));
  netwib_er_hr(netwib_hash_index_next(phashindex, NULL, NULL));
  netwib_er(test_hash_log_create(38, "hello", &pitem));
  ret = netwib_hash_index_this_replace(phashindex, pitem, NETWIB_TRUE);
  if (ret == NETWIB_ERR_OK) {
    netwib_er_hr(netwib_hash_index_this_value(phashindex, &key, &pitem));
    netwib_er(netwibtest_buf_content2(&key, "rem"));
    netwib_er(test_hash_log_check1(pitem, 38, "hello"));
    netwib_er(test_hash_log_checkall(phash, "rem=38,hello;k1=1,one;k2=2,two",
                                     NULL));
  } else {
    if (test_hash_randombug && ret == NETWIBTEST_ERR_3) {
      test_hash_randombug = NETWIB_FALSE;
      netwib_er(test_hash_log_erase(pitem));
      test_hash_randombug = NETWIB_TRUE;
    } else {
      netwib_er(netwib_buf_close(&key));
      return(ret);
    }
  }
  netwib_er(netwib_buf_close(&key));

  netwib_er_hr(netwib_hash_index_close(&phashindex));
  netwib_er_hr(test_netwib_hash_close(&phash, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwibtest_dat_hash2(void)
{
  netwib_er(netwib_hash_init_test());
  netwib_er(netwib_hash_close_test());
  netwib_er(netwib_hash_ctl_set_test());
  netwib_er(netwib_hash_ctl_get_test());
  netwib_er(netwib_hash_add_test());
  netwib_er(netwib_hash_add_hash_criteria_test());
  netwib_er(netwib_hash_contains_test());
  netwib_er(netwib_hash_value_test());
  netwib_er(netwib_hash_del_test());
  netwib_er(netwib_hash_del_criteria_test());

  netwib_er(netwib_hash_index_init_test());
  netwib_er(netwib_hash_index_close_test());
  netwib_er(netwib_hash_index_ctl_set_test());
  netwib_er(netwib_hash_index_ctl_get_test());
  netwib_er(netwib_hash_index_next_criteria_test());
  netwib_er(netwib_hash_index_this_value_test());
  netwib_er(netwib_hash_index_this_del_test());
  netwib_er(netwib_hash_index_this_replace_test());

  return(NETWIB_ERR_OK);
}
netwib_err netwibtest_dat_hash(void)
{
  puts("###########################################\nTest of dat/hash");
  test_hash_randombug = NETWIB_FALSE;
  netwib_er(netwibtest_dat_hash2());

  test_hash_randombug = NETWIB_TRUE;
  for (test_hash_randombug_n = 1;
       test_hash_randombug_n < 5;
       test_hash_randombug_n++) {
    netwib_er(netwibtest_dat_hash2());
  }

  return(NETWIB_ERR_OK);
}

