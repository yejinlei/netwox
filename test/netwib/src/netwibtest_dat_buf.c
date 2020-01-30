#include "netwibtest.h"

/*-------------------------------------------------------------*/
#define BUF_MEM_INIT 1024
#define BUF_MEM_APP1 1024

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_init_malloc_test(void)
{
  netwib_buf buf;

  puts("++++----\nTest of netwib_buf_init_malloc");
  netwib_er(netwib_buf_init_malloc(0, NULL));
  netwib_er(netwib_buf_init_malloc(10, NULL));
  netwib_er(netwib_buf_init_malloc(20, &buf));
  netwib_er(netwibtest_buf_content(&buf, "", 20, 0, 0));
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwibtest_buf_content(&buf, "", BUF_MEM_INIT, 0, 0));
  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_init_ext_array_test(void)
{
  netwib_buf buf;
  netwib_char array[5];

  puts("++++----\nTest of netwib_buf_init_ext_array");
  netwib_er(netwib_buf_init_ext_array(NULL, 0, 0, 0, NULL));
  netwib_eir(netwib_buf_init_ext_array(NULL, 1, 0, 0, NULL),
             NETWIB_ERR_PANULLPTRSIZE);
  netwib_eir(netwib_buf_init_ext_array(NULL, 0, 5, 0, NULL),
             NETWIB_ERR_PATOOHIGH);

  netwib_er(netwib_buf_init_ext_array(array, 5, 0, 0, &buf));
  netwib_er(netwibtest_buf_content(&buf, "", 5, 0, 0));
  netwib_er(netwib_buf_init_ext_array(array, 5, 3, 3, &buf));
  netwib_er(netwibtest_buf_content(&buf, "", 5, 3, 3));
  netwib_eir(netwib_buf_init_ext_array(array, 5, 6, 0, &buf),
             NETWIB_ERR_PATOOHIGH);

  netwib_er(netwib_buf_init_ext_arrayempty(array, 5, &buf));
  netwib_er(netwibtest_buf_content(&buf, "", 5, 0, 0));

  netwib_c_memcpy(array, (netwib_constdata)"abcde", 5);
  netwib_er(netwib_buf_init_ext_arrayfilled(array, 5, &buf));
  netwib_er(netwibtest_buf_content(&buf, "abcde", 5, 0, 5));

  netwib_er(netwib_buf_init_ext_empty(&buf));
  netwib_er(netwibtest_buf_content(&buf, NULL, 0, 0, 0));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_init_ext_storagearray_test(void)
{
  netwib_buf buf;
  netwib_char array[5];

  puts("++++----\nTest of netwib_buf_init_ext_storagearray");
  netwib_er(netwib_buf_init_ext_storagearray(NULL, 0, NULL));
  netwib_eir(netwib_buf_init_ext_storagearray(NULL, 1, NULL),
             NETWIB_ERR_PANULLPTRSIZE);

  netwib_er(netwib_buf_init_ext_storagearray(array, 5, &buf));
  netwib_er(netwibtest_buf_content(&buf, "", 5, 0, 0));
  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));
  netwib_er(netwibtest_buf_content(&buf, "", 5, 0, 0));

  netwib_er(netwib_buf_init_ext_storage(&buf));
  netwib_er(netwibtest_buf_content(&buf, NULL, 0, 0, 0));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_init_ext_buf_test(void)
{
  netwib_buf buf, buf2;

  puts("++++----\nTest of netwib_buf_init_ext_buf");
  netwib_er(netwib_buf_init_ext_buf(NULL, NULL));

  netwib_er(netwib_buf_init_ext_string("hello", &buf));
  buf.beginoffset++;
  buf.endoffset--;
  netwib_er(netwib_buf_init_ext_buf(&buf, &buf2));
  netwib_er(netwibtest_buf_content(&buf2, "ell", 3, 0, 3));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_init_ext_string_test(void)
{
  netwib_buf buf;

  puts("++++----\nTest of netwib_buf_init_ext_string");
  netwib_er(netwib_buf_init_ext_string(NULL, NULL));

  netwib_er(netwib_buf_init_ext_string("hello", &buf));
  netwib_er(netwibtest_buf_content(&buf, "hello", 6, 0, 5));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_netwib__buf_reinit(void)
{
  netwib_buf buf;

  puts("++++----\nTest of define netwib__buf_reinit");

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwibtest_buf_content(&buf, "", BUF_MEM_INIT, 0, 0));
  netwib_er(netwib_buf_append_string("AB", &buf));
  netwib_er(netwibtest_buf_content(&buf, "AB", BUF_MEM_INIT, 0, 2));
  netwib__buf_reinit(&buf);
  netwib_er(netwibtest_buf_content(&buf, "", BUF_MEM_INIT, 0, 0));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_ext_empty(&buf));
  netwib__buf_reinit(&buf);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_shift_test(void)
{
  netwib_buf buf;
  netwib_char array[5];

  puts("++++----\nTest of netwib_buf_shift");

  netwib_er(netwib_buf_init_malloc(20, &buf));
  netwib_er(netwibtest_buf_content(&buf, "", 20, 0, 0));
  netwib_er(netwib_buf_append_string("AB", &buf));
  netwib_er(netwibtest_buf_content(&buf, "AB", 20, 0, 2));
  netwib_er(netwib_buf_shift(&buf, 1, NETWIB_FALSE));
  netwib_er(netwibtest_buf_content(&buf, "AB", 20, 1, 3));
  netwib_er(netwib_buf_shift(&buf, -1, NETWIB_FALSE));
  netwib_er(netwibtest_buf_content(&buf, "AB", 20, 0, 2));
  netwib_er(netwib_buf_shift(&buf, -1, NETWIB_FALSE));
  netwib_er(netwibtest_buf_content(&buf, "B", 20, 0, 1));
  netwib_er(netwib_buf_shift(&buf, 20, NETWIB_FALSE));
  netwib_er(netwibtest_buf_content(&buf, "B", 20+BUF_MEM_APP1, 20, 21));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_malloc(20, &buf));
  netwib_er(netwibtest_buf_content(&buf, "", 20, 0, 0));
  netwib_er(netwib_buf_append_string("AB", &buf));
  netwib_er(netwibtest_buf_content(&buf, "AB", 20, 0, 2));
  netwib_er(netwib_buf_shift(&buf, 1, NETWIB_TRUE));
  netwib_er(netwibtest_buf_content(&buf, "A", 20, 1, 2));
  netwib_er(netwib_buf_shift(&buf, -1, NETWIB_TRUE));
  netwib_er(netwibtest_buf_content(&buf, "", 20, 1, 1));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_ext_arrayempty(array, 5, &buf));
  netwib_er(netwibtest_buf_content(&buf, "", 5, 0, 0));
  netwib_er(netwib_buf_append_string("AB", &buf));
  netwib_er(netwibtest_buf_content(&buf, "AB", 5, 0, 2));
  netwib_er(netwib_buf_shift(&buf, 4, NETWIB_FALSE));
  netwib_er(netwibtest_buf_content(&buf, "A", 5, 4, 5));

  netwib_er(netwib_buf_init_ext_arrayempty(array, 5, &buf));
  netwib_er(netwibtest_buf_content(&buf, "", 5, 0, 0));
  netwib_er(netwib_buf_append_string("AB", &buf));
  netwib_er(netwibtest_buf_content(&buf, "AB", 5, 0, 2));
  netwib_er(netwib_buf_shift(&buf, 6, NETWIB_FALSE));
  netwib_er(netwibtest_buf_content(&buf, "", 5, 5, 5));

  netwib_er(netwib_buf_init_ext_empty(&buf));
  netwib_er(netwibtest_buf_content(&buf, NULL, 0, 0, 0));
  netwib_er(netwib_buf_shift(&buf, 6, NETWIB_FALSE));
  netwib_er(netwibtest_buf_content(&buf, NULL, 0, 0, 0));
  netwib_er(netwib_buf_shift(&buf, 6, NETWIB_TRUE));
  netwib_er(netwibtest_buf_content(&buf, NULL, 0, 0, 0));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_data_test(void)
{
  netwib_buf buf;
  netwib_char array[5];
  netwib_ptr ptr;

  puts("++++----\nTest of netwib_buf_append_data");

  netwib_er(netwib_buf_append_data((netwib_constdata)"AB", 2, NULL));

  netwib_er(netwib_buf_init_malloc(5, &buf));
  netwib_er(netwib_buf_append_data((netwib_constdata)"AB", 2, &buf));
  netwib_er(netwibtest_buf_content(&buf, "AB", 5, 0, 2));
  netwib_er(netwib_buf_append_data(NULL, 0, &buf));
  netwib_er(netwibtest_buf_content(&buf, "AB", 5, 0, 2));
  netwib_er(netwib_buf_append_data((netwib_constdata)"CD", 2, &buf));
  netwib_er(netwibtest_buf_content(&buf, "ABCD", 5, 0, 4));
  netwib_er(netwib_buf_append_data((netwib_constdata)"EF", 2, &buf));
  netwib_er(netwibtest_buf_content(&buf, "ABCDEF", BUF_MEM_APP1+5, 0, 6));
  netwib_er(netwib_buf_append_data((netwib_constdata)"GH", 2, &buf));
  netwib_er(netwibtest_buf_content(&buf, "ABCDEFGH", BUF_MEM_APP1+5, 0, 8));
  netwib_er(netwib_buf_append_data((netwib_constdata)"IJ", 2, &buf));
  netwib_er(netwibtest_buf_content(&buf, "ABCDEFGHIJ", BUF_MEM_APP1+5, 0, 10));
  netwib_er(netwib_buf_append_data((netwib_constdata)"KL", 2, &buf));
  netwib_er(netwibtest_buf_content(&buf, "ABCDEFGHIJKL", BUF_MEM_APP1+5, 0, 12));
  netwib_er(netwib_buf_append_data((netwib_constdata)"MN", 2, &buf));
  netwib_er(netwibtest_buf_content(&buf, "ABCDEFGHIJKLMN", BUF_MEM_APP1+5, 0, 14));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_ext_arrayempty(array, 5, &buf));
  netwib_er(netwib_buf_append_data((netwib_constdata)"ABC", 3, &buf));
  netwib_er(netwibtest_buf_content(&buf, "ABC", 5, 0, 3));
  netwib_er(netwib_buf_append_data((netwib_constdata)"DE", 2, &buf));
  netwib_er(netwibtest_buf_content(&buf, "ABCDE", 5, 0, 5));
  netwib_eir(netwib_buf_append_data((netwib_constdata)"FG", 2, &buf),
             NETWIB_ERR_DATANOSPACE);
  netwib_er(netwibtest_buf_content(&buf, "ABCDE", 5, 0, 5));
  buf.flags |= NETWIB_BUF_FLAGS_CANALLOC;
  netwib_er(netwib_buf_append_data((netwib_constdata)"FG", 2, &buf));
  netwib_er(netwibtest_buf_content(&buf, "ABCDEFG", BUF_MEM_APP1+5, 0, 7));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_ptr_malloc(6, &ptr));
  netwib_er(netwib_buf_init_ext_arrayempty(ptr, 6, &buf));
  netwib_er(netwib_buf_append_data((netwib_constdata)"ABC", 3, &buf));
  netwib_er(netwibtest_buf_content(&buf, "ABC", 6, 0, 3));
  netwib_er(netwib_buf_append_data((netwib_constdata)"DE", 2, &buf));
  netwib_er(netwibtest_buf_content(&buf, "ABCDE", 6, 0, 5));
  netwib_eir(netwib_buf_append_data((netwib_constdata)"FG", 2, &buf),
             NETWIB_ERR_DATANOSPACE);
  netwib_er(netwibtest_buf_content(&buf, "ABCDE", 6, 0, 5));
  netwib_er(netwib_ptr_free(&ptr));

  netwib_er(netwib_buf_init_ext_empty(&buf));
  netwib_eir(netwib_buf_append_data((netwib_constdata)"FG", 2, &buf),
             NETWIB_ERR_DATANOSPACE);
  netwib_er(netwibtest_buf_content(&buf, NULL, 0, 0, 0));

  netwib_er(netwib_buf_init_ext_empty(&buf));
  buf.flags |= NETWIB_BUF_FLAGS_CANALLOC;
  netwib_er(netwib_buf_append_data((netwib_constdata)"FG", 2, &buf));
  netwib_er(netwibtest_buf_content(&buf, "FG", BUF_MEM_APP1, 0, 2));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_string_test(void)
{
  netwib_buf buf;
  netwib_char array[5];
  netwib_ptr ptr;

  puts("++++----\nTest of netwib_buf_append_string");

  netwib_er(netwib_buf_append_string("AB", NULL));

  netwib_er(netwib_buf_init_malloc(5, &buf));
  netwib_er(netwib_buf_append_string("AB", &buf));
  netwib_er(netwibtest_buf_content(&buf, "AB", 5, 0, 2));
  netwib_er(netwib_buf_append_string(NULL, &buf));
  netwib_er(netwibtest_buf_content(&buf, "AB", 5, 0, 2));
  netwib_er(netwib_buf_append_string("CD", &buf));
  netwib_er(netwibtest_buf_content(&buf, "ABCD", 5, 0, 4));
  netwib_er(netwib_buf_append_string("EF", &buf));
  netwib_er(netwibtest_buf_content(&buf, "ABCDEF", 5+BUF_MEM_APP1, 0, 6));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_malloc(5, &buf));
  netwib_er(netwib_buf_append_string("AB", &buf));
  netwib_er(netwibtest_buf_content(&buf, "AB", 5, 0, 2));
  netwib_er(netwib_buf_append_string("CDEFGHIJKLM", &buf));
  netwib_er(netwibtest_buf_content(&buf, "ABCDEFGHIJKLM", 5+BUF_MEM_APP1,
                                   0, 13));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_ext_arrayempty(array, 5, &buf));
  netwib_er(netwib_buf_append_string("ABC", &buf));
  netwib_er(netwibtest_buf_content(&buf, "ABC", 5, 0, 3));
  netwib_er(netwib_buf_append_string("DE", &buf));
  netwib_er(netwibtest_buf_content(&buf, "ABCDE", 5, 0, 5));
  netwib_eir(netwib_buf_append_string("FG", &buf),
             NETWIB_ERR_DATANOSPACE);
  netwib_er(netwibtest_buf_content(&buf, "ABCDE", 5, 0, 5));

  netwib_er(netwib_ptr_malloc(6, &ptr));
  netwib_er(netwib_buf_init_ext_arrayempty(ptr, 6, &buf));
  netwib_er(netwib_buf_append_string("ABC", &buf));
  netwib_er(netwibtest_buf_content(&buf, "ABC", 6, 0, 3));
  netwib_er(netwib_buf_append_string("DE", &buf));
  netwib_er(netwibtest_buf_content(&buf, "ABCDE", 6, 0, 5));
  netwib_eir(netwib_buf_append_string("FG", &buf),
             NETWIB_ERR_DATANOSPACE);
  netwib_er(netwibtest_buf_content(&buf, "ABCDE", 6, 0, 5));
  netwib_er(netwib_ptr_free(&ptr));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_buf_test(void)
{
  netwib_buf buf1, buf2;

  puts("++++----\nTest of netwib_buf_append_buf");

  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_buf_append_string("AB", &buf1));

  netwib_er(netwib_buf_init_malloc(5, &buf2));
  netwib_er(netwib_buf_append_string("abc", &buf2));

  netwib_er(netwib_buf_append_buf(&buf1, NULL));
  netwib_er(netwib_buf_append_buf(&buf1, &buf2));
  netwib_er(netwibtest_buf_content(&buf2, "abcAB", 5, 0, 5));
  netwib_er(netwib_buf_append_buf(NULL, &buf2));
  netwib_er(netwibtest_buf_content(&buf2, "abcAB", 5, 0, 5));
  netwib_er(netwib_buf_append_buf(&buf1, &buf2));
  netwib_er(netwibtest_buf_content(&buf2, "abcABAB", 5+BUF_MEM_APP1, 0, 7));

  netwib_er(netwib_buf_close(&buf1));
  netwib_er(netwib_buf_close(&buf2));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_byte_test(void)
{
  netwib_buf buf;
  netwib_char array[5];

  puts("++++----\nTest of netwib_buf_append_byte");

  netwib_er(netwib_buf_append_byte('A', NULL));

  netwib_er(netwib_buf_init_malloc(1, &buf));
  netwib_er(netwib_buf_append_byte('A', &buf));
  netwib_er(netwibtest_buf_content(&buf, "A", 1, 0, 1));
  netwib_er(netwib_buf_append_byte('B', &buf));
  netwib_er(netwibtest_buf_content(&buf, "AB", 1+BUF_MEM_APP1, 0, 2));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_ext_arrayempty(array, 2, &buf));
  netwib_er(netwib_buf_append_byte('A', &buf));
  netwib_er(netwibtest_buf_content(&buf, "A", 2, 0, 1));
  netwib_er(netwib_buf_append_byte('B', &buf));
  netwib_er(netwibtest_buf_content(&buf, "AB", 2, 0, 2));
  netwib_eir(netwib_buf_append_byte('C', &buf), NETWIB_ERR_DATANOSPACE);

  netwib_er(netwib_buf_init_ext_empty(&buf));
  netwib_eir(netwib_buf_append_byte('C', &buf), NETWIB_ERR_DATANOSPACE);
  netwib_er(netwibtest_buf_content(&buf, NULL, 0, 0, 0));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_prepend_buf_test(void)
{
  netwib_buf buf1, buf2;

  puts("++++----\nTest of netwib_buf_prepend_buf");

  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_buf_append_string("AB", &buf1));

  netwib_er(netwib_buf_init_malloc(5, &buf2));
  netwib_er(netwib_buf_append_string("abc", &buf2));

  netwib_er(netwib_buf_prepend_buf(&buf1, &buf2));
  netwib_er(netwibtest_buf_content(&buf2, "ABabc", 5, 0, 5));
  netwib_er(netwib_buf_append_buf(NULL, &buf2));
  netwib_er(netwibtest_buf_content(&buf2, "ABabc", 5, 0, 5));
  netwib_er(netwib_buf_append_string("C", &buf1));
  netwib_er(netwib_buf_prepend_buf(&buf1, &buf2));
  netwib_er(netwibtest_buf_content(&buf2, "ABCABabc", 5+BUF_MEM_APP1, 0, 8));

  netwib_er(netwib_buf_close(&buf1));
  netwib_er(netwib_buf_close(&buf2));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_ref_string_test(void)
{
  netwib_buf buf;
  netwib_string str;

  puts("++++----\nTest of netwib_buf_ref_string");

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwibtest_buf_content(&buf, "", BUF_MEM_INIT, 0, 0));
  netwib_er(netwib_buf_append_string("AB", &buf));
  buf.beginoffset = 1;
  netwib_er(netwibtest_buf_content(&buf, "B", BUF_MEM_INIT, 1, 2));
  netwib_er(netwib_buf_ref_string(&buf, &str));
  if (netwib_c_strcmp(str, "B")) {
    return(NETWIBTEST_ERR_9);
  }
  netwib_er(netwib_buf_close(&buf));

  /**/
  netwib_er(netwib_buf_init_malloc(2, &buf));
  netwib_er(netwibtest_buf_content(&buf, "", 2, 0, 0));
  netwib_er(netwib_buf_append_string("AB", &buf));
  netwib_er(netwibtest_buf_content(&buf, "AB", 2, 0, 2));
  netwib_er(netwib_buf_ref_string(&buf, &str));
  if (netwib_c_strcmp(str, "AB")) {
    return(NETWIBTEST_ERR_9);
  }
  netwib_er(netwibtest_buf_content(&buf, "AB", 2+BUF_MEM_APP1, 0, 2));
  netwib_er(netwib_buf_close(&buf));

  /**/
  netwib_er(netwib_buf_init_malloc(3, &buf));
  netwib_er(netwibtest_buf_content(&buf, "", 3, 0, 0));
  netwib_er(netwib_buf_append_string("AB", &buf));
  netwib_er(netwibtest_buf_content(&buf, "AB", 3, 0, 2));
  netwib_er(netwib_buf_ref_string(&buf, &str));
  if (netwib_c_strcmp(str, "AB")) {
    return(NETWIBTEST_ERR_9);
  }
  netwib_er(netwibtest_buf_content(&buf, "AB", 3, 0, 2));
  netwib_er(netwib_buf_close(&buf));

  /**/
  netwib_er(netwib_buf_init_ext_string("AB", &buf));
  netwib_er(netwibtest_buf_content(&buf, "AB", 3, 0, 2));
  netwib_er(netwib_buf_ref_string(&buf, &str));
  if (netwib_c_strcmp(str, "AB")) {
    return(NETWIBTEST_ERR_9);
  }
  netwib_er(netwibtest_buf_content(&buf, "AB", 3, 0, 2));

  /**/
  netwib_er(netwib_buf_init_ext_string("AB", &buf));
  netwib_er(netwibtest_buf_content(&buf, "AB", 3, 0, 2));
  buf.totalsize = 2;
  netwib_er(netwibtest_buf_content(&buf, "AB", 2, 0, 2));
  netwib_eir(netwib_buf_ref_string(&buf, &str),
             NETWIB_ERR_DATANOSPACE);

  netwib_er(netwib_buf_init_ext_empty(&buf));
  netwib_eir(netwib_buf_ref_string(&buf, &str),
             NETWIB_ERR_DATANOSPACE);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_constbuf_ref_string_test(void)
{
  netwib_buf buf;
  netwib_string str;

  puts("++++----\nTest of netwib_constbuf_ref_string");
  netwib_er(netwib_buf_init_malloc(2, &buf));
  netwib_er(netwibtest_buf_content(&buf, "", 2, 0, 0));
  netwib_er(netwib_buf_append_string("AB", &buf));
  netwib_er(netwibtest_buf_content(&buf, "AB", 2, 0, 2));
  netwib_eir(netwib_constbuf_ref_string(&buf, &str),
             NETWIB_ERR_DATANOSPACE);
  netwib_er(netwib_buf_append_byte('\0', &buf));
  buf.endoffset--;
  netwib_er(netwib_constbuf_ref_string(&buf, &str));
  if (netwib_c_strcmp(str, "AB")) {
    return(NETWIBTEST_ERR_9);
  }
  netwib_er(netwibtest_buf_content(&buf, "AB", 2+BUF_MEM_APP1, 0, 2));
  netwib_er(netwib_buf_close(&buf));

  /**/
  netwib_er(netwib_buf_init_malloc(3, &buf));
  netwib_er(netwibtest_buf_content(&buf, "", 3, 0, 0));
  netwib_er(netwib_buf_append_data((netwib_data)"ABX", 3, &buf));
  netwib_er(netwibtest_buf_content(&buf, "ABX", 3, 0, 3));
  buf.endoffset--;
  netwib_er(netwibtest_buf_content(&buf, "AB", 3, 0, 2));
  netwib_eir(netwib_constbuf_ref_string(&buf, &str),
             NETWIB_ERR_DATANOSPACE);
  netwib_er(netwib_buf_append_byte('\0', &buf));
  buf.endoffset--;
  netwib_er(netwib_constbuf_ref_string(&buf, &str));
  if (netwib_c_strcmp(str, "AB")) {
    return(NETWIBTEST_ERR_9);
  }
  netwib_er(netwibtest_buf_content(&buf, "AB", 3, 0, 2));
  netwib_er(netwib_buf_close(&buf));

  /**/
  netwib_er(netwib_buf_init_ext_string("AB", &buf));
  netwib_er(netwibtest_buf_content(&buf, "AB", 3, 0, 2));
  netwib_er(netwib_constbuf_ref_string(&buf, &str));
  if (netwib_c_strcmp(str, "AB")) {
    return(NETWIBTEST_ERR_9);
  }
  netwib_er(netwibtest_buf_content(&buf, "AB", 3, 0, 2));

  /**/
  netwib_er(netwib_buf_init_ext_string("AB", &buf));
  netwib_er(netwibtest_buf_content(&buf, "AB", 3, 0, 2));
  buf.totalsize = 2;
  netwib_er(netwibtest_buf_content(&buf, "AB", 2, 0, 2));
  netwib_eir(netwib_constbuf_ref_string(&buf, &str),
             NETWIB_ERR_DATANOSPACE);

  netwib_er(netwib_buf_init_ext_empty(&buf));
  netwib_eir(netwib_constbuf_ref_string(&buf, &str),
             NETWIB_ERR_DATANOSPACE);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_wantspace_test(void)
{
  netwib_buf buf;
  netwib_char array[5];

  puts("++++----\nTest of netwib_buf_wantspace");

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwibtest_buf_content(&buf, "", BUF_MEM_INIT, 0, 0));
  netwib_er(netwib_buf_append_string("AB", &buf));
  netwib_er(netwibtest_buf_content(&buf, "AB", BUF_MEM_INIT, 0, 2));
  netwib_er(netwib_buf_wantspace(&buf, 12, NULL));
  netwib_er(netwibtest_buf_content(&buf, "AB", BUF_MEM_INIT, 0, 2));
  netwib_er(netwib_buf_wantspace(&buf, BUF_MEM_INIT, NULL));
  netwib_er(netwibtest_buf_content(&buf, "AB", BUF_MEM_INIT+BUF_MEM_APP1, 0, 2));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_ext_arrayempty(array, 5, &buf));
  netwib_er(netwib_buf_append_data((netwib_constdata)"ABC", 3, &buf));
  netwib_er(netwibtest_buf_content(&buf, "ABC", 5, 0, 3));
  netwib_er(netwib_buf_wantspace(&buf, 1, NULL));
  netwib_er(netwibtest_buf_content(&buf, "ABC", 5, 0, 3));
  netwib_er(netwib_buf_wantspace(&buf, 2, NULL));
  netwib_er(netwibtest_buf_content(&buf, "ABC", 5, 0, 3))
    netwib_eir(netwib_buf_wantspace(&buf, 3, NULL), NETWIB_ERR_DATANOSPACE);

  netwib_er(netwib_buf_init_ext_empty(&buf));
  netwib_eir(netwib_buf_wantspace(&buf, 3, NULL), NETWIB_ERR_DATANOSPACE);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_wishspace_test(void)
{
  netwib_buf buf;
  netwib_char array[5];
  netwib_uint32 ui;

  puts("++++----\nTest of netwib_buf_wishspace");

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwibtest_buf_content(&buf, "", BUF_MEM_INIT, 0, 0));
  netwib_er(netwib_buf_append_string("AB", &buf));
  netwib_er(netwibtest_buf_content(&buf, "AB", BUF_MEM_INIT, 0, 2));
  netwib_er(netwib_buf_wishspace(&buf, 12, NULL, &ui));
  netwib_er(netwibtest_buf_content(&buf, "AB", BUF_MEM_INIT, 0, 2));
  if (ui != BUF_MEM_INIT-2) {
    return(NETWIBTEST_ERR_9);
  }
  netwib_er(netwib_buf_wishspace(&buf, BUF_MEM_INIT, NULL, &ui));
  netwib_er(netwibtest_buf_content(&buf, "AB", BUF_MEM_INIT+BUF_MEM_APP1, 0, 2));
  if (ui != BUF_MEM_INIT+BUF_MEM_APP1-2) {
    return(NETWIBTEST_ERR_9);
  }
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_ext_arrayempty(array, 5, &buf));
  netwib_er(netwib_buf_append_data((netwib_constdata)"ABC", 3, &buf));
  netwib_er(netwibtest_buf_content(&buf, "ABC", 5, 0, 3));
  netwib_er(netwib_buf_wishspace(&buf, 1, NULL, &ui));
  netwib_er(netwibtest_buf_content(&buf, "ABC", 5, 0, 3));
  if (ui != 2) {
    return(NETWIBTEST_ERR_9);
  }
  netwib_er(netwib_buf_wishspace(&buf, 2, NULL, &ui));
  netwib_er(netwibtest_buf_content(&buf, "ABC", 5, 0, 3));
  if (ui != 2) {
    return(NETWIBTEST_ERR_9);
  }
  netwib_er(netwib_buf_wishspace(&buf, 3, NULL, &ui));
  if (ui != 2) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_init_ext_empty(&buf));
  netwib_er(netwib_buf_wishspace(&buf, 3, NULL, &ui));
  if (ui != 0) {
    return(NETWIBTEST_ERR_9);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_cmp_test(void)
{
  netwib_buf buf1, buf2;
  netwib_cmp cmp;

  puts("++++----\nTest of netwib_buf_cmp");
  netwib_er(netwib_buf_init_ext_string("hello", &buf1));
  netwib_er(netwib_buf_init_ext_string("hello", &buf2));
  netwib_er(netwib_buf_cmp(&buf1, &buf2, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_init_ext_string("hell_o", &buf1));
  netwib_er(netwib_buf_init_ext_string("hell_p", &buf2));
  netwib_er(netwib_buf_cmp(&buf1, &buf2, &cmp));
  if (cmp != NETWIB_CMP_LT) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_init_ext_string("hello", &buf1));
  netwib_er(netwib_buf_init_ext_string("hell", &buf2));
  netwib_er(netwib_buf_cmp(&buf1, &buf2, &cmp));
  if (cmp != NETWIB_CMP_GT) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_init_ext_empty(&buf1));
  netwib_er(netwib_buf_init_ext_string("hell", &buf2));
  netwib_er(netwib_buf_cmp(&buf1, &buf2, &cmp));
  if (cmp != NETWIB_CMP_LT) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_init_ext_empty(&buf1));
  netwib_er(netwib_buf_init_ext_empty(&buf2));
  netwib_er(netwib_buf_cmp(&buf1, &buf2, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_cmp(NULL, NULL, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    return(NETWIBTEST_ERR_9);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_casecmp_test(void)
{
  netwib_buf buf1, buf2;
  netwib_cmp cmp;

  puts("++++----\nTest of netwib_buf_casecmp");
  netwib_er(netwib_buf_init_ext_string("heLlo", &buf1));
  netwib_er(netwib_buf_init_ext_string("hEllo", &buf2));
  netwib_er(netwib_buf_casecmp(&buf1, &buf2, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_init_ext_string("hell_O", &buf1));
  netwib_er(netwib_buf_init_ext_string("hEll_p", &buf2));
  netwib_er(netwib_buf_casecmp(&buf1, &buf2, &cmp));
  if (cmp != NETWIB_CMP_LT) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_init_ext_string("heLlo", &buf1));
  netwib_er(netwib_buf_init_ext_string("hEll", &buf2));
  netwib_er(netwib_buf_casecmp(&buf1, &buf2, &cmp));
  if (cmp != NETWIB_CMP_GT) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_init_ext_empty(&buf1));
  netwib_er(netwib_buf_init_ext_string("heLl", &buf2));
  netwib_er(netwib_buf_casecmp(&buf1, &buf2, &cmp));
  if (cmp != NETWIB_CMP_LT) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_init_ext_empty(&buf1));
  netwib_er(netwib_buf_init_ext_empty(&buf2));
  netwib_er(netwib_buf_casecmp(&buf1, &buf2, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_casecmp(NULL, NULL, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    return(NETWIBTEST_ERR_9);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_cmp_string_test(void)
{
  netwib_buf buf1;
  netwib_cmp cmp;

  puts("++++----\nTest of netwib_buf_cmp_string");
  netwib_er(netwib_buf_init_ext_string("hello", &buf1));
  netwib_er(netwib_buf_cmp_string(&buf1, "hello", &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_init_ext_string("hell_o", &buf1));
  netwib_er(netwib_buf_cmp_string(&buf1, "hell_p", &cmp));
  if (cmp != NETWIB_CMP_LT) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_init_ext_string("hello", &buf1));
  netwib_er(netwib_buf_cmp_string(&buf1, "hell", &cmp));
  if (cmp != NETWIB_CMP_GT) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_init_ext_empty(&buf1));
  netwib_er(netwib_buf_cmp_string(&buf1, "hell", &cmp));
  if (cmp != NETWIB_CMP_LT) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_init_ext_empty(&buf1));
  netwib_er(netwib_buf_cmp_string(&buf1, "", &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_cmp_string(NULL, "", &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    return(NETWIBTEST_ERR_9);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_casecmp_string_test(void)
{
  netwib_buf buf1;
  netwib_cmp cmp;

  puts("++++----\nTest of netwib_buf_casecmp_string");
  netwib_er(netwib_buf_init_ext_string("hEllo", &buf1));
  netwib_er(netwib_buf_casecmp_string(&buf1, "heLlo", &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_init_ext_string("hEll_o", &buf1));
  netwib_er(netwib_buf_casecmp_string(&buf1, "hell_p", &cmp));
  if (cmp != NETWIB_CMP_LT) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_init_ext_string("hello", &buf1));
  netwib_er(netwib_buf_casecmp_string(&buf1, "heLl", &cmp));
  if (cmp != NETWIB_CMP_GT) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_init_ext_empty(&buf1));
  netwib_er(netwib_buf_casecmp_string(&buf1, "heLl", &cmp));
  if (cmp != NETWIB_CMP_LT) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_init_ext_empty(&buf1));
  netwib_er(netwib_buf_casecmp_string(&buf1, "", &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_buf_casecmp_string(NULL, "", &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    return(NETWIBTEST_ERR_9);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_close_test(void)
{
  netwib_buf buf;
  netwib_char array[5];
  netwib_ptr ptr;

  puts("++++----\nTest of netwib_buf_close");

  netwib_er(netwib_buf_close(NULL));

  netwib_er(netwib_buf_init_malloc(5, &buf));
  netwib_er(netwib_buf_close(&buf));
  netwib_eir(netwib_buf_close(&buf), NETWIB_ERR_LOOBJCLOSEALREADYCLOSED);

  netwib_er(netwib_buf_init_ext_array(array, 5, 2, 2, &buf));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_ptr_malloc(6, &ptr));
  netwib_er(netwib_buf_init_ext_array(ptr, 6, 3, 2, &buf));
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_ptr_free(&ptr));

  netwib_er(netwib_buf_init_ext_empty(&buf));
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_buf_init_ext_empty(&buf));
  buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  netwib_er(netwib_buf_close(&buf));
  if (buf.flags&NETWIB_BUF_FLAGS_SENSITIVE) {
    return(NETWIBTEST_ERR_5);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_netwib__buf_transfersensitive(void)
{
  netwib_buf buf, buf2, *pbuf, *pbuf2;
  netwib_char array[5];

  puts("++++----\nTest of define netwib__buf_transfersensitive");

  netwib_c_memcpy(array, (netwib_constdata)"abcde", 5);
  netwib_er(netwib_buf_init_ext_arraysizeoffilled(array, &buf));
  netwib_er(netwib_buf_close(&buf));
  if (netwib_c_memcmp(array, (netwib_constdata)"abcde", 5)) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_c_memcpy(array, (netwib_constdata)"abcde", 5);
  netwib_er(netwib_buf_init_ext_arraysizeoffilled(array, &buf));
  buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  netwib_er(netwib_buf_close(&buf));
  if (netwib_c_memcmp(array, (netwib_constdata)"\0\0\0\0\0", 5)) {
    return(NETWIBTEST_ERR_8);
  }

  netwib_c_memcpy(array, (netwib_constdata)"abcde", 5);
  netwib_er(netwib_buf_init_ext_arraysizeoffilled(array, &buf));
  buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE_READONLY;
  netwib_er(netwib_buf_close(&buf));
  if (netwib_c_memcmp(array, (netwib_constdata)"abcde", 5)) {
    return(NETWIBTEST_ERR_7);
  }

  buf.flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  buf2.flags = 0;
  pbuf = &buf; /* Use pointers for netwib__buf_transfersensitive() otherwise */
  pbuf2 = &buf2; /* gcc throws a warning because &buf cannot be NULL. */
  netwib__buf_transfersensitive(pbuf, pbuf2);
  if (!(buf2.flags&NETWIB_BUF_FLAGS_SENSITIVE)) {
    return(NETWIBTEST_ERR_6);
  }
  buf.flags &= ~NETWIB_BUF_FLAGS_SENSITIVE;
  buf2.flags = 0;
  pbuf = &buf;
  pbuf2 = &buf2;
  netwib__buf_transfersensitive(pbuf, pbuf2);
  if (buf2.flags&NETWIB_BUF_FLAGS_SENSITIVE) {
    return(NETWIBTEST_ERR_5);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_dat_buf(void)
{
  puts("###########################################\nTest of dat/buf");

  netwib_er(netwib_buf_init_malloc_test());
  netwib_er(netwib_buf_init_ext_array_test());
  netwib_er(netwib_buf_init_ext_storagearray_test());
  netwib_er(netwib_buf_init_ext_buf_test());
  netwib_er(netwib_buf_init_ext_string_test());

  netwib_er(test_netwib__buf_reinit());
  netwib_er(netwib_buf_shift_test());

  netwib_er(netwib_buf_append_data_test());
  netwib_er(netwib_buf_append_string_test());
  netwib_er(netwib_buf_append_buf_test());
  netwib_er(netwib_buf_append_byte_test());

  netwib_er(netwib_buf_prepend_buf_test());

  netwib_er(netwib_buf_ref_string_test());
  netwib_er(netwib_constbuf_ref_string_test());

  netwib_er(netwib_buf_wantspace_test());
  netwib_er(netwib_buf_wishspace_test());

  netwib_er(netwib_buf_cmp_test());
  netwib_er(netwib_buf_casecmp_test());
  netwib_er(netwib_buf_cmp_string_test());
  netwib_er(netwib_buf_casecmp_string_test());

  netwib_er(netwib_buf_close_test());

  netwib_er(test_netwib__buf_transfersensitive());

  return(NETWIB_ERR_OK);
}
