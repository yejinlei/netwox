#include "netwibtest.h"

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_bool test_ring_randombug = NETWIB_FALSE;
static netwib_uint32 test_ring_randombug_n;

/*-------------------------------------------------------------*/
static netwib_err netwib_er_rr(netwib_err err)
{
  if (err == NETWIB_ERR_OK)
    return(NETWIB_ERR_OK);
  if (test_ring_randombug && err == NETWIBTEST_ERR_3) {
    puts("Random error ignored");
    return(NETWIB_ERR_OK);
  }
  netwib_err_display(err, NETWIB_ERR_ENCODETYPE_FULL);
  exit(err);
  return(NETWIB_ERR_LOINTERNALERROR);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_eir_rr(netwib_err err,
                                netwib_err wantederr)
{
  if (err == wantederr)
    return(NETWIB_ERR_OK);
  if (test_ring_randombug && err == NETWIBTEST_ERR_3) {
    puts("Random error ignored");
    return(NETWIB_ERR_OK);
  }
  netwib_err_display(err, NETWIB_ERR_ENCODETYPE_FULL);
  exit(err);
  return(NETWIB_ERR_LOINTERNALERROR);
}

/*-------------------------------------------------------------*/
static netwib_err test_ring_generaterandomerror(void)
{
  static netwib_uint32 i = 0;
  if (test_ring_randombug) {
    i++;
    if (!(i % test_ring_randombug_n)) {
      netwib_fmt_display("Generate random error %{uint32}\n", i);
      return(NETWIBTEST_ERR_3);
    }
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_netwib_ring_close(netwib_ring **ppring,
                                         netwib_bool eraseitems)
{
  netwib_err ret;

  if (test_ring_randombug) {
    test_ring_randombug = NETWIB_FALSE;
    ret = netwib_ring_close(ppring, eraseitems);
    test_ring_randombug = NETWIB_TRUE;
    return(ret);
  } else {
    netwib_er(netwib_ring_close(ppring, eraseitems));
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 code;
  netwib_string msg;
} test_ring_log;

/*-------------------------------------------------------------*/
static netwib_err test_ring_log_check1(netwib_ptr pitem,
                                       netwib_uint32 code,
                                       netwib_conststring msg)
{
  test_ring_log *plog;

  plog = (test_ring_log *)pitem;

  if (code != plog->code || netwib_c_strcmp(msg, plog->msg)) {
    netwib_er(netwib_fmt_display("code=%{uint32} msg=%s\n",
                                 plog->code,plog->msg));
    netwib_er(netwib_fmt_display("!= code=%{uint32} msg=%s\n", code, msg));
    return(NETWIBTEST_ERR_9);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_ring_log_checkall(netwib_ring *pring,
                                         netwib_conststring wanted)
{
  netwib_ring_index *pringindex;
  test_ring_log *plog;
  netwib_buf buf;
  netwib_ptr ptr;
  netwib_string str;
  netwib_bool addsemicolon;
  netwib_err ret;

  netwib_er(netwib_ring_index_init(pring, &pringindex));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  addsemicolon = NETWIB_FALSE;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, &ptr);
    if (ret == NETWIB_ERR_DATAEND) break;
    if (ret != NETWIB_ERR_OK) return(ret);
    plog = (test_ring_log *)ptr;
    if (addsemicolon) {
      netwib_er(netwib_buf_append_byte(';', &buf));
    } else {
      addsemicolon = NETWIB_TRUE;
    }
    netwib_er(netwib_buf_append_fmt(&buf, "%{uint32},%s", plog->code,
                                    plog->msg));
  }
  netwib_er(netwib_buf_ref_string(&buf, &str));
  netwib_er(netwib_ring_index_close(&pringindex));

  if (netwib_c_strcmp(str, wanted)) {
    if (test_ring_randombug) {
      puts("Consequence of a random error ignored");
    } else {
      netwib_er(netwib_fmt_display("Obtain : %s\n", str));
      netwib_er(netwib_fmt_display("Wanted : %s\n", wanted));
      netwib_er(netwib_buf_close(&buf));
      return(NETWIBTEST_ERR_9);
    }
  }
  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_ring_log_create(netwib_uint32 code,
                                       netwib_conststring msg,
                                       netwib_ptr *ppitem)
{
  netwib_ptr ptr;
  test_ring_log *plog;

  netwib_er(netwib_ptr_malloc(sizeof(test_ring_log), &ptr));
  plog = (test_ring_log *)ptr;
  plog->code = code;
  netwib_er(netwib_ptr_malloc(netwib_c_strlen(msg)+1, &ptr));
  netwib_c_strcpy((netwib_string)ptr, msg);
  plog->msg = (netwib_string)ptr;

  *ppitem = (netwib_ptr *)plog;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_ring_log_add_last(netwib_ring *pring,
                                         netwib_uint32 code,
                                         netwib_conststring msg)
{
  netwib_ptr pitem;

  netwib_er_rr(test_ring_log_create(code, msg, &pitem));
  netwib_er_rr(netwib_ring_add_last(pring, pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_ring_log_erase(netwib_ptr pitem)
{
  test_ring_log *plog;

  netwib_er(test_ring_generaterandomerror());

  plog = (test_ring_log *)pitem;

  netwib_er(netwib_ptr_free((netwib_ptr *)&plog->msg));
  netwib_er(netwib_ptr_free((netwib_ptr *)&plog));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_ring_log_duplicate(netwib_constptr pitem,
                                          netwib_ptr *pdupofitem)
{
  const test_ring_log *plog = (const test_ring_log *)pitem;

  netwib_er(test_ring_generaterandomerror());

  netwib_er(test_ring_log_create(plog->code, plog->msg, pdupofitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_ring_log_compare(netwib_constptr piteminf,
                                        netwib_constptr pitemsup,
                                        netwib_ptr pinfos,
                                        netwib_cmp *pcmp)
{
  const test_ring_log *ploginf = (const test_ring_log *)piteminf;
  const test_ring_log *plogsup = (const test_ring_log *)pitemsup;
  netwib_uint32 *pui = (netwib_uint32 *) pinfos;
  netwib_cmp cmp;

  netwib_er(test_ring_generaterandomerror());

  /*netwib_er(netwib_fmt_display("%{uint32} %{uint32}\n", ploginf->code,
    plogsup->code));
  */

  cmp = NETWIB_CMP_EQ;
  if (pinfos != NULL && ploginf->code == *pui) {
    cmp = NETWIB_CMP_EQ;
  } else if (pinfos != NULL && plogsup->code == *pui) {
    cmp = NETWIB_CMP_EQ;
  } else if (ploginf->code < plogsup->code) {
    cmp = NETWIB_CMP_LT;
  } else if (ploginf->code > plogsup->code) {
    cmp = NETWIB_CMP_GT;
  } else if (ploginf->code == plogsup->code) {
    cmp = NETWIB_CMP_EQ;
  }
  if (pcmp != NULL) *pcmp = cmp;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_ring_log_criteria(netwib_constptr pitem,
                                         netwib_ptr pinfos,
                                         netwib_bool *pbool)
{
  const test_ring_log *plog = (const test_ring_log *)pitem;
  netwib_uint32 *pui = (netwib_uint32 *) pinfos;
  netwib_bool b;

  netwib_er(test_ring_generaterandomerror());

  b = NETWIB_FALSE;
  if (plog->code == *pui) {
    b = NETWIB_TRUE;
  }

  if (pbool != NULL) *pbool = b;
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
static netwib_err netwib_ring_init_test(void)
{
  netwib_ring *pring;

  puts("++++----\nTest of netwib_ring_init");
  netwib_eir(netwib_ring_init(NULL, NULL, NULL), NETWIB_ERR_PANULLPTR);

  netwib_er_rr(netwib_ring_init(NULL, NULL, &pring));
  netwib_er_rr(test_netwib_ring_close(&pring, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ring_close_test(void)
{
  netwib_ring *pring;

  puts("++++----\nTest of netwib_ring_close");
  netwib_eir(test_netwib_ring_close(NULL, NETWIB_TRUE), NETWIB_ERR_PANULLPTR);

  netwib_er_rr(netwib_ring_init(NULL, NULL, &pring));
  netwib_er_rr(test_netwib_ring_close(&pring, NETWIB_TRUE));

  netwib_er_rr(netwib_ring_init(NULL, NULL, &pring));
  netwib_er_rr(test_netwib_ring_close(&pring, NETWIB_FALSE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ring_ctl_set_test(void)
{
  netwib_ring *pring;

  puts("++++----\nTest of netwib_ring_ctl_set");

  netwib_er_rr(netwib_ring_init(NULL, NULL, &pring));
  netwib_eir(netwib_ring_ctl_set(pring,
                                 NETWIB_RING_CTLTYPE_COUNT,
                                 NULL, 0),
             NETWIB_ERR_PAINVALIDTYPE);
  netwib_er_rr(test_netwib_ring_close(&pring, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ring_ctl_get_test(void)
{
  netwib_ring *pring;
  netwib_uint32 ui;

  puts("++++----\nTest of netwib_ring_ctl_get");
  netwib_eir(netwib_ring_ctl_get_count(NULL, NULL), NETWIB_ERR_PANULLPTR);

  netwib_er_rr(netwib_ring_init(NULL, NULL, &pring));
  netwib_er_rr(netwib_ring_ctl_get_count(pring, &ui));
  netwib_er_rr(test_netwib_ring_close(&pring, NETWIB_TRUE));
  if (ui != 0) {
    return(NETWIBTEST_ERR_9);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ring_add_first_test(void)
{
  netwib_ring *pring;
  netwib_uint32 ui;
  netwib_ptr pitem;

  puts("++++----\nTest of netwib_ring_add_first");
  netwib_er_rr(netwib_ring_init(NULL, NULL, &pring));
  netwib_er_rr(netwib_ring_add_first(pring, NULL));
  netwib_er_rr(netwib_ring_add_first(pring, NULL));
  netwib_er_rr(netwib_ring_add_last(pring, NULL));
  netwib_er_rr(netwib_ring_ctl_get_count(pring, &ui));
  if (ui != 3) {
    return(NETWIBTEST_ERR_9);
  }
  netwib_er_rr(test_netwib_ring_close(&pring, NETWIB_TRUE));

  netwib_er_rr(netwib_ring_init(&test_ring_log_erase,
                               &test_ring_log_duplicate, &pring));
  netwib_er_rr(test_ring_log_create(1, "one", &pitem));
  netwib_er_rr(netwib_ring_add_first(pring, pitem));
  netwib_er_rr(test_ring_log_create(2, "two", &pitem));
  netwib_er_rr(netwib_ring_add_last(pring, pitem));
  netwib_er_rr(test_ring_log_create(3, "three", &pitem));
  netwib_er_rr(netwib_ring_add_first(pring, pitem));
  netwib_er_rr(test_ring_log_create(4, "four", &pitem));
  netwib_er_rr(netwib_ring_add_last(pring, pitem));
  netwib_er(test_ring_log_checkall(pring, "3,three;1,one;2,two;4,four"));
  netwib_er_rr(test_netwib_ring_close(&pring, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ring_add_last_test(void)
{
  puts("++++----\nTest of netwib_ring_add_last");
  puts("Redundant with netwib_ring_add_first");

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ring_del_criteria_test(void)
{
  netwib_ring *pring;
  netwib_uint32 ui;

  puts("++++----\nTest of netwib_ring_del_criteria");
  netwib_er_rr(netwib_ring_init(&test_ring_log_erase,
                               &test_ring_log_duplicate, &pring));

  netwib_er_rr(test_ring_log_add_last(pring, 1, "one"));
  netwib_er_rr(test_ring_log_add_last(pring, 2, "two"));
  netwib_er_rr(test_ring_log_add_last(pring, 3, "three"));
  netwib_er_rr(test_ring_log_add_last(pring, 4, "four"));
  netwib_er_rr(test_ring_log_add_last(pring, 5, "five"));
  netwib_er(test_ring_log_checkall(pring, "1,one;2,two;3,three;4,four;5,five"));

  ui = 4;
  netwib_er_rr(netwib_ring_del_criteria(pring, &test_ring_log_criteria, &ui,
                                       NETWIB_TRUE));
  netwib_er(test_ring_log_checkall(pring, "1,one;2,two;3,three;5,five"));
  netwib_er_rr(netwib_ring_del_criteria(pring, &test_ring_log_criteria, &ui,
                                       NETWIB_TRUE));
  netwib_er(test_ring_log_checkall(pring, "1,one;2,two;3,three;5,five"));
  ui = 5;
  netwib_er_rr(netwib_ring_del_criteria(pring, &test_ring_log_criteria, &ui,
                                       NETWIB_TRUE));
  netwib_er(test_ring_log_checkall(pring, "1,one;2,two;3,three"));
  ui = 1;
  netwib_er_rr(netwib_ring_del_criteria(pring, &test_ring_log_criteria, &ui,
                                       NETWIB_TRUE));
  netwib_er(test_ring_log_checkall(pring, "2,two;3,three"));
  netwib_er_rr(netwib_ring_del_all(pring, NETWIB_TRUE));
  netwib_er(test_ring_log_checkall(pring, ""));

  netwib_er_rr(test_netwib_ring_close(&pring, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ring_del_duplicate_test(void)
{
  netwib_ring *pring;

  puts("++++----\nTest of netwib_ring_del_duplicate");
  netwib_er_rr(netwib_ring_init(&test_ring_log_erase,
                               &test_ring_log_duplicate, &pring));

  netwib_er_rr(test_ring_log_add_last(pring, 1, "one"));
  netwib_er_rr(test_ring_log_add_last(pring, 3, "three"));
  netwib_er_rr(test_ring_log_add_last(pring, 2, "two"));
  netwib_er_rr(test_ring_log_add_last(pring, 3, "three"));
  netwib_er_rr(test_ring_log_add_last(pring, 1, "one"));
  netwib_er_rr(test_ring_log_add_last(pring, 4, "four"));
  netwib_er_rr(test_ring_log_add_last(pring, 3, "three"));
  netwib_er_rr(test_ring_log_add_last(pring, 1, "one"));
  netwib_er_rr(test_ring_log_add_last(pring, 3, "three"));
  netwib_er_rr(test_ring_log_add_last(pring, 3, "three"));
  netwib_er_rr(test_ring_log_add_last(pring, 3, "three"));
  netwib_er_rr(test_ring_log_add_last(pring, 1, "one"));
  netwib_er_rr(test_ring_log_add_last(pring, 3, "three"));
  netwib_er_rr(test_ring_log_add_last(pring, 5, "five"));
  netwib_er_rr(test_ring_log_add_last(pring, 1, "one"));
  netwib_er(test_ring_log_checkall(pring, "1,one;3,three;2,two;3,three;1,one;4,four;3,three;1,one;3,three;3,three;3,three;1,one;3,three;5,five;1,one"));

  netwib_er_rr(netwib_ring_del_duplicate(pring, &test_ring_log_compare, NULL,
                                        NETWIB_TRUE));
  netwib_er(test_ring_log_checkall(pring, "1,one;3,three;2,two;4,four;5,five"));

  netwib_er_rr(test_netwib_ring_close(&pring, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ring_sort_test(void)
{
  netwib_ring *pring;

  puts("++++----\nTest of netwib_ring_sort");
  netwib_er_rr(netwib_ring_init(&test_ring_log_erase,
                               &test_ring_log_duplicate, &pring));

  netwib_er_rr(test_ring_log_add_last(pring, 1, "one"));
  netwib_er_rr(test_ring_log_add_last(pring, 3, "three"));
  netwib_er_rr(test_ring_log_add_last(pring, 2, "two"));
  netwib_er_rr(test_ring_log_add_last(pring, 5, "five"));
  netwib_er_rr(test_ring_log_add_last(pring, 4, "four"));
  netwib_er(test_ring_log_checkall(pring, "1,one;3,three;2,two;5,five;4,four"));
  netwib_er_rr(netwib_ring_sort(pring, &test_ring_log_compare, NULL));
  netwib_er(test_ring_log_checkall(pring, "1,one;2,two;3,three;4,four;5,five"));

  netwib_er_rr(test_netwib_ring_close(&pring, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ring_group_test(void)
{
  netwib_ring *pring;

  puts("++++----\nTest of netwib_ring_group");
  netwib_er_rr(netwib_ring_init(&test_ring_log_erase,
                               &test_ring_log_duplicate, &pring));

  netwib_er_rr(test_ring_log_add_last(pring, 1, "one"));
  netwib_er_rr(test_ring_log_add_last(pring, 3, "three"));
  netwib_er_rr(test_ring_log_add_last(pring, 2, "two"));
  netwib_er_rr(test_ring_log_add_last(pring, 5, "five"));
  netwib_er_rr(test_ring_log_add_last(pring, 4, "four"));
  netwib_er(test_ring_log_checkall(pring, "1,one;3,three;2,two;5,five;4,four"));
  netwib_er_rr(netwib_ring_group(pring, &test_ring_log_compare, NULL));
  netwib_er(test_ring_log_checkall(pring, "1,one;3,three;2,two;5,five;4,four"));
  netwib_er_rr(test_ring_log_add_last(pring, 2, "two_"));
  netwib_er_rr(test_ring_log_add_last(pring, 5, "five_"));
  netwib_er_rr(test_ring_log_add_last(pring, 4, "four_"));
  netwib_er(test_ring_log_checkall(pring, "1,one;3,three;2,two;5,five;4,four;2,two_;5,five_;4,four_"));
  netwib_er_rr(netwib_ring_group(pring, &test_ring_log_compare, NULL));
  netwib_er(test_ring_log_checkall(pring, "1,one;3,three;2,two;2,two_;5,five;5,five_;4,four;4,four_"));
  netwib_er_rr(test_ring_log_add_last(pring, 4, "four__"));
  netwib_er_rr(test_ring_log_add_last(pring, 4, "four___"));
  netwib_er(test_ring_log_checkall(pring, "1,one;3,three;2,two;2,two_;5,five;5,five_;4,four;4,four_;4,four__;4,four___"));
  netwib_er_rr(netwib_ring_group(pring, &test_ring_log_compare, NULL));
  netwib_er(test_ring_log_checkall(pring, "1,one;3,three;2,two;2,two_;5,five;5,five_;4,four;4,four_;4,four__;4,four___"));

  netwib_er_rr(test_netwib_ring_close(&pring, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwib_ring_index_init_test(void)
{
  netwib_ring_index *pringindex;
  netwib_ring *pring;

  puts("++++----\nTest of netwib_ring_index_init");
  netwib_er_rr(netwib_ring_init(NULL, NULL, &pring));
  netwib_er_rr(netwib_ring_index_init(pring, &pringindex));
  netwib_er_rr(netwib_ring_index_close(&pringindex));
  netwib_er_rr(test_netwib_ring_close(&pring, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ring_index_close_test(void)
{
  puts("++++----\nTest of netwib_ring_index_close");
  puts("Redundant with netwib_ring_index_init");

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ring_index_ctl_set_test(void)
{
  netwib_ring_index *pringindex, *pringindex2;
  netwib_ring *pring;

  puts("++++----\nTest of netwib_ring_index_ctl_set");
  netwib_er_rr(netwib_ring_init(&test_ring_log_erase, &test_ring_log_duplicate,
                               &pring));
  netwib_er_rr(netwib_ring_index_init(pring, &pringindex));

  netwib_er(netwib_ring_index_ctl_set_rewind(pringindex));
  netwib_er(netwib_ring_index_ctl_set_rewind(pringindex));

  netwib_er_rr(netwib_ring_index_init(pring, &pringindex2));
  netwib_er(netwib_ring_index_ctl_set_index(pringindex2, pringindex));
  netwib_er_rr(netwib_ring_index_close(&pringindex2));

  netwib_er_rr(netwib_ring_index_close(&pringindex));
  netwib_er_rr(test_netwib_ring_close(&pring, NETWIB_TRUE));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ring_index_ctl_get_test(void)
{
  netwib_ring_index *pringindex;
  netwib_ring *pring;

  puts("++++----\nTest of netwib_ring_index_ctl_get");
  netwib_er_rr(netwib_ring_init(&test_ring_log_erase, &test_ring_log_duplicate,
                               &pring));
  netwib_er_rr(netwib_ring_index_init(pring, &pringindex));

  netwib_eir(netwib_ring_index_ctl_get(pringindex,
                                       NETWIB_RING_INDEX_CTLTYPE_REWIND,
                                       NULL, NULL),
             NETWIB_ERR_PAINVALIDTYPE);
  netwib_eir(netwib_ring_index_ctl_get(pringindex,
                                       NETWIB_RING_INDEX_CTLTYPE_INDEX,
                                       NULL, NULL),
             NETWIB_ERR_PAINVALIDTYPE);

  netwib_er_rr(netwib_ring_index_close(&pringindex));
  netwib_er_rr(test_netwib_ring_close(&pring, NETWIB_TRUE));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ring_index_next_criteria_test(void)
{
  netwib_ring_index *pringindex;
  netwib_ring *pring;
  netwib_uint32 ui;
  netwib_ptr pitem;

  puts("++++----\nTest of netwib_ring_index_next_criteria");
  netwib_er_rr(netwib_ring_init(&test_ring_log_erase, &test_ring_log_duplicate,
                               &pring));
  netwib_er_rr(netwib_ring_index_init(pring, &pringindex));

  netwib_er_rr(test_ring_log_add_last(pring, 1, "one"));
  netwib_er_rr(test_ring_log_add_last(pring, 2, "two"));
  netwib_er_rr(test_ring_log_add_last(pring, 3, "three"));
  netwib_er(test_ring_log_checkall(pring, "1,one;2,two;3,three"));

  netwib_er_rr(netwib_ring_index_next(pringindex, &pitem));
  netwib_er(test_ring_log_check1(pitem, 1, "one"));
  netwib_er_rr(netwib_ring_index_next(pringindex, &pitem));
  netwib_er(test_ring_log_check1(pitem, 2, "two"));
  netwib_er_rr(netwib_ring_index_next(pringindex, &pitem));
  netwib_er(test_ring_log_check1(pitem, 3, "three"));
  netwib_eir_rr(netwib_ring_index_next(pringindex, &pitem), NETWIB_ERR_DATAEND);

  ui = 2;
  netwib_er(netwib_ring_index_ctl_set_rewind(pringindex));
  netwib_er_rr(netwib_ring_index_next_criteria(pringindex,
                                              &test_ring_log_criteria, &ui,
                                              &pitem));
  if (!test_ring_randombug) {
    netwib_er(test_ring_log_check1(pitem, 2, "two"));
    netwib_eir_rr(netwib_ring_index_next_criteria(pringindex,
                                                 &test_ring_log_criteria, &ui,
                                                 &pitem), NETWIB_ERR_DATAEND);
  }

  netwib_er_rr(netwib_ring_index_close(&pringindex));
  netwib_er_rr(test_netwib_ring_close(&pring, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ring_index_previous_criteria_test(void)
{
  netwib_ring_index *pringindex;
  netwib_ring *pring;
  netwib_uint32 ui;
  netwib_ptr pitem;

  puts("++++----\nTest of netwib_ring_index_previous_criteria");
  netwib_er_rr(netwib_ring_init(&test_ring_log_erase, &test_ring_log_duplicate,
                               &pring));
  netwib_er_rr(netwib_ring_index_init(pring, &pringindex));

  netwib_er_rr(test_ring_log_add_last(pring, 1, "one"));
  netwib_er_rr(test_ring_log_add_last(pring, 2, "two"));
  netwib_er_rr(test_ring_log_add_last(pring, 3, "three"));
  netwib_er(test_ring_log_checkall(pring, "1,one;2,two;3,three"));

  netwib_er_rr(netwib_ring_index_previous(pringindex, &pitem));
  netwib_er(test_ring_log_check1(pitem, 3, "three"));
  netwib_er_rr(netwib_ring_index_previous(pringindex, &pitem));
  netwib_er(test_ring_log_check1(pitem, 2, "two"));
  netwib_er_rr(netwib_ring_index_previous(pringindex, &pitem));
  netwib_er(test_ring_log_check1(pitem, 1, "one"));
  netwib_eir_rr(netwib_ring_index_previous(pringindex, &pitem),
               NETWIB_ERR_DATAEND);

  ui = 2;
  netwib_er(netwib_ring_index_ctl_set_rewind(pringindex));
  netwib_er_rr(netwib_ring_index_previous_criteria(pringindex,
                                                  &test_ring_log_criteria, &ui,
                                                  &pitem));
  if (!test_ring_randombug) {
    netwib_er(test_ring_log_check1(pitem, 2, "two"));
    netwib_eir_rr(netwib_ring_index_previous_criteria(pringindex,
                                                     &test_ring_log_criteria,
                                                     &ui, &pitem),
                 NETWIB_ERR_DATAEND);
  }

  netwib_er_rr(netwib_ring_index_close(&pringindex));
  netwib_er_rr(test_netwib_ring_close(&pring, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ring_index_add_before_test(void)
{
  netwib_ring_index *pringindex;
  netwib_ring *pring;
  netwib_ptr pitem;

  puts("++++----\nTest of netwib_ring_index_add_before");
  netwib_er_rr(netwib_ring_init(&test_ring_log_erase, &test_ring_log_duplicate,
                               &pring));
  netwib_er_rr(netwib_ring_index_init(pring, &pringindex));

  netwib_er_rr(test_ring_log_add_last(pring, 1, "one"));
  netwib_er_rr(test_ring_log_add_last(pring, 2, "two"));
  netwib_er_rr(test_ring_log_add_last(pring, 3, "three"));
  netwib_er(test_ring_log_checkall(pring, "1,one;2,two;3,three"));

  netwib_er_rr(test_ring_log_create(4, "four", &pitem));
  netwib_er_rr(netwib_ring_index_add_before(pringindex, pitem));
  netwib_er(test_ring_log_checkall(pring, "1,one;2,two;3,three;4,four"));

  netwib_er_rr(netwib_ring_index_next(pringindex, &pitem));
  netwib_er(test_ring_log_check1(pitem, 1, "one"));
  netwib_er_rr(test_ring_log_create(5, "five", &pitem));
  netwib_er_rr(netwib_ring_index_add_before(pringindex, pitem));
  netwib_er(test_ring_log_checkall(pring, "5,five;1,one;2,two;3,three;4,four"));

  netwib_er_rr(netwib_ring_index_next(pringindex, NULL));
  netwib_er_rr(netwib_ring_index_next(pringindex, NULL));
  netwib_er_rr(test_ring_log_create(6, "six", &pitem));
  netwib_er_rr(netwib_ring_index_add_before(pringindex, pitem));
  netwib_er(test_ring_log_checkall(pring, "5,five;1,one;2,two;6,six;3,three;4,four"));

  netwib_er_rr(netwib_ring_index_close(&pringindex));
  netwib_er_rr(test_netwib_ring_close(&pring, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ring_index_add_after_test(void)
{
  netwib_ring_index *pringindex;
  netwib_ring *pring;
  netwib_ptr pitem;

  puts("++++----\nTest of netwib_ring_index_add_after");
  netwib_er_rr(netwib_ring_init(&test_ring_log_erase, &test_ring_log_duplicate,
                               &pring));
  netwib_er_rr(netwib_ring_index_init(pring, &pringindex));

  netwib_er_rr(test_ring_log_add_last(pring, 1, "one"));
  netwib_er_rr(test_ring_log_add_last(pring, 2, "two"));
  netwib_er_rr(test_ring_log_add_last(pring, 3, "three"));
  netwib_er(test_ring_log_checkall(pring, "1,one;2,two;3,three"));

  netwib_er_rr(test_ring_log_create(4, "four", &pitem));
  netwib_er_rr(netwib_ring_index_add_after(pringindex, pitem));
  netwib_er(test_ring_log_checkall(pring, "4,four;1,one;2,two;3,three"));

  netwib_er_rr(netwib_ring_index_next(pringindex, NULL));
  netwib_er_rr(test_ring_log_create(5, "five", &pitem));
  netwib_er_rr(netwib_ring_index_add_after(pringindex, pitem));
  netwib_er(test_ring_log_checkall(pring, "4,four;5,five;1,one;2,two;3,three"));

  netwib_er_rr(netwib_ring_index_next(pringindex, NULL));
  netwib_er_rr(netwib_ring_index_next(pringindex, NULL));
  netwib_er_rr(test_ring_log_create(6, "six", &pitem));
  netwib_er_rr(netwib_ring_index_add_after(pringindex, pitem));
  netwib_er(test_ring_log_checkall(pring, "4,four;5,five;1,one;6,six;2,two;3,three"));

  netwib_er_rr(netwib_ring_index_close(&pringindex));
  netwib_er_rr(test_netwib_ring_close(&pring, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ring_index_this_value_test(void)
{
  netwib_ring_index *pringindex;
  netwib_ring *pring;
  netwib_ptr pitem;

  puts("++++----\nTest of netwib_ring_index_this_value");
  netwib_er_rr(netwib_ring_init(&test_ring_log_erase, &test_ring_log_duplicate,
                               &pring));
  netwib_er_rr(netwib_ring_index_init(pring, &pringindex));

  netwib_er_rr(test_ring_log_add_last(pring, 1, "one"));
  netwib_er_rr(test_ring_log_add_last(pring, 2, "two"));
  netwib_er_rr(test_ring_log_add_last(pring, 3, "three"));
  netwib_er(test_ring_log_checkall(pring, "1,one;2,two;3,three"));

  netwib_eir_rr(netwib_ring_index_this_value(pringindex, &pitem),
               NETWIB_ERR_PAINDEXNODATA);

  netwib_er_rr(netwib_ring_index_next(pringindex, NULL));
  netwib_er_rr(netwib_ring_index_this_value(pringindex, &pitem));
  netwib_er(test_ring_log_check1(pitem, 1, "one"));
  netwib_er_rr(netwib_ring_index_this_value(pringindex, &pitem));
  netwib_er(test_ring_log_check1(pitem, 1, "one"));

  netwib_er_rr(netwib_ring_index_close(&pringindex));
  netwib_er_rr(test_netwib_ring_close(&pring, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ring_index_this_del_test(void)
{
  netwib_ring_index *pringindex;
  netwib_ring *pring;

  puts("++++----\nTest of netwib_ring_index_this_del");
  netwib_er_rr(netwib_ring_init(&test_ring_log_erase, &test_ring_log_duplicate,
                               &pring));
  netwib_er_rr(netwib_ring_index_init(pring, &pringindex));

  netwib_er_rr(test_ring_log_add_last(pring, 1, "one"));
  netwib_er_rr(test_ring_log_add_last(pring, 2, "two"));
  netwib_er_rr(test_ring_log_add_last(pring, 3, "three"));
  netwib_er(test_ring_log_checkall(pring, "1,one;2,two;3,three"));

  netwib_eir_rr(netwib_ring_index_this_del(pringindex, NETWIB_TRUE),
               NETWIB_ERR_PAINDEXNODATA);

  netwib_er_rr(netwib_ring_index_next(pringindex, NULL));
  netwib_er_rr(netwib_ring_index_this_del(pringindex, NETWIB_TRUE));
  netwib_er(test_ring_log_checkall(pring, "2,two;3,three"));
  netwib_er_rr(netwib_ring_index_next(pringindex, NULL));
  netwib_er_rr(netwib_ring_index_next(pringindex, NULL));
  netwib_er_rr(netwib_ring_index_this_del(pringindex, NETWIB_TRUE));
  netwib_er(test_ring_log_checkall(pring, "2,two"));

  netwib_er_rr(netwib_ring_index_close(&pringindex));
  netwib_er_rr(test_netwib_ring_close(&pring, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ring_index_this_replace_test(void)
{
  netwib_ring_index *pringindex;
  netwib_ring *pring;
  netwib_ptr pitem;
  netwib_err ret;

  puts("++++----\nTest of netwib_ring_index_this_replace");
  netwib_er_rr(netwib_ring_init(&test_ring_log_erase, &test_ring_log_duplicate,
                               &pring));
  netwib_er_rr(netwib_ring_index_init(pring, &pringindex));

  netwib_er_rr(test_ring_log_add_last(pring, 1, "one"));
  netwib_er_rr(test_ring_log_add_last(pring, 2, "two"));
  netwib_er_rr(test_ring_log_add_last(pring, 3, "three"));
  netwib_er(test_ring_log_checkall(pring, "1,one;2,two;3,three"));

  netwib_eir_rr(netwib_ring_index_this_replace(pringindex, NULL, NETWIB_TRUE),
               NETWIB_ERR_PAINDEXNODATA);

  netwib_er_rr(netwib_ring_index_next(pringindex, NULL));
  netwib_er_rr(test_ring_log_create(4, "four", &pitem));
  ret = netwib_ring_index_this_replace(pringindex, pitem, NETWIB_TRUE);
  if (test_ring_randombug && ret == NETWIBTEST_ERR_3) {
    test_ring_randombug = NETWIB_FALSE;
    netwib_er_rr(test_ring_log_erase(pitem));
    test_ring_randombug = NETWIB_TRUE;
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  } else {
    netwib_er(test_ring_log_checkall(pring, "4,four;2,two;3,three"));
  }

  netwib_er_rr(netwib_ring_index_close(&pringindex));
  netwib_er_rr(test_netwib_ring_close(&pring, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
static netwib_err netwib_ring_index_add_ring_criteria_test(void)
{
  netwib_ring_index *pringindex;
  netwib_ring *pring1, *pring2;

  puts("++++----\nTest of netwib_ring_index_add_ring_criteria");
  netwib_er_rr(netwib_ring_init(&test_ring_log_erase, &test_ring_log_duplicate,
                               &pring1));
  netwib_er_rr(test_ring_log_add_last(pring1, 1, "one"));
  netwib_er_rr(test_ring_log_add_last(pring1, 2, "two"));
  netwib_er_rr(test_ring_log_add_last(pring1, 3, "three"));
  netwib_er(test_ring_log_checkall(pring1, "1,one;2,two;3,three"));

  netwib_er_rr(netwib_ring_index_init(pring1, &pringindex));
  netwib_er_rr(netwib_ring_index_next(pringindex, NULL));

  netwib_er_rr(netwib_ring_init(&test_ring_log_erase, &test_ring_log_duplicate,
                               &pring2));
  netwib_er_rr(test_ring_log_add_last(pring2, 11, "el"));
  netwib_er_rr(test_ring_log_add_last(pring2, 22, "tw"));
  netwib_er_rr(test_ring_log_add_last(pring2, 33, "th"));
  netwib_er(test_ring_log_checkall(pring2, "11,el;22,tw;33,th"));

  netwib_er_rr(netwib_ring_index_add_ring_all(pringindex, pring2, NETWIB_TRUE));
  netwib_er(test_ring_log_checkall(pring1, "1,one;11,el;22,tw;33,th;2,two;3,three"));
  netwib_er(test_ring_log_checkall(pring2, "11,el;22,tw;33,th"));

  netwib_er_rr(netwib_ring_index_close(&pringindex));
  netwib_er_rr(test_netwib_ring_close(&pring1, NETWIB_TRUE));
  netwib_er_rr(test_netwib_ring_close(&pring2, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwibtest_dat_ring2(void)
{
  netwib_er(netwib_ring_init_test());
  netwib_er(netwib_ring_close_test());
  netwib_er(netwib_ring_ctl_set_test());
  netwib_er(netwib_ring_ctl_get_test());
  netwib_er(netwib_ring_add_first_test());
  netwib_er(netwib_ring_add_last_test());
  netwib_er(netwib_ring_del_criteria_test());
  netwib_er(netwib_ring_del_duplicate_test());
  netwib_er(netwib_ring_sort_test());
  netwib_er(netwib_ring_group_test());

  netwib_er(netwib_ring_index_init_test());
  netwib_er(netwib_ring_index_close_test());
  netwib_er(netwib_ring_index_ctl_set_test());
  netwib_er(netwib_ring_index_ctl_get_test());
  netwib_er(netwib_ring_index_next_criteria_test());
  netwib_er(netwib_ring_index_previous_criteria_test());
  netwib_er(netwib_ring_index_add_before_test());
  netwib_er(netwib_ring_index_add_after_test());
  netwib_er(netwib_ring_index_this_value_test());
  netwib_er(netwib_ring_index_this_del_test());
  netwib_er(netwib_ring_index_this_replace_test());
  netwib_er(netwib_ring_index_add_ring_criteria_test());

  return(NETWIB_ERR_OK);
}

netwib_err netwibtest_dat_ring(void)
{
  puts("###########################################\nTest of dat/ring");
  test_ring_randombug = NETWIB_FALSE;
  netwib_er(netwibtest_dat_ring2());

  test_ring_randombug = NETWIB_TRUE;
  for (test_ring_randombug_n = 1;
       test_ring_randombug_n < 5;
       test_ring_randombug_n++) {
    netwib_er(netwibtest_dat_ring2());
  }

  return(NETWIB_ERR_OK);
}
