#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_show_array_head_test(void)
{
  netwib_buf buf;

  puts("++++----\nTest of netwib_show_array_head");
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_show_array_head(NULL, &buf));
  netwib_er(netwib_show_array_head("Hello", &buf));
  /*  netwib_er(netwib_show_array_tail(&buf));*/
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_show_array_tail_test(void)
{
  netwib_buf buf;

  puts("++++----\nTest of netwib_show_array_tail");
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_show_array_tail(&buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_show_array_line_begin_test(void)
{
  netwib_buf buf;

  puts("++++----\nTest of netwib_show_array_line_begin");
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_show_array_line_begin(&buf));
  netwib_er(netwib_show_array_line_begin(&buf));
  netwib_er(netwib_show_array_line_end(&buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_show_array_line_end_test(void)
{
  puts("++++----\nTest of netwib_show_array_line_end");
  puts("Redundant with netwib_show_array_line_begin");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_show_array_text_test(void)
{
  netwib_buf buf;

  puts("++++----\nTest of netwib_show_array_text");
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_show_array_text32("Hello", &buf));
  netwib_er(netwib_show_array_line_begin(&buf));
  netwib_er(netwib_show_array_text16("Hello", &buf));
  netwib_er(netwib_show_array_text8("Hello", &buf));
  netwib_er(netwib_show_array_text4("Hello", &buf));
  netwib_er(netwib_show_array_text1("H", &buf));
  netwib_er(netwib_show_array_text(3, "He", &buf));
  netwib_er(netwib_show_array_line_end(&buf));

  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_show_array_num_test2(netwib_uint32 size,
                                              netwib_uint32 ui)
{
  netwib_buf buf;
  netwib_show_array_numtype numtype;
  netwib_err ret;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_buf_append_fmt(&buf, "$$ size%{uint32} and ui%{uint32}$$\n",
                                  size, ui));
  for (numtype = NETWIB_SHOW_ARRAY_NUMTYPE_BIN;
       numtype <= NETWIB_SHOW_ARRAY_NUMTYPE_HEXADECCHAR;
       numtype++) {
    if (size != 32) {
      netwib_er(netwib_show_array_line_begin(&buf));
    }
    ret = netwib_show_array_num(size, ui, numtype, &buf);
    if (ret != NETWIB_ERR_OK && ret != NETWIB_ERR_PAINVALIDTYPE) {
      return(ret);
    }
    if (size != 32) {
      netwib_er(netwib_show_array_line_end(&buf));
    }
  }
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_show_array_num_test(void)
{
  puts("++++----\nTest of netwib_show_array_num");

  netwib_er(netwib_show_array_num_test2(32, 0x12345678));
  netwib_er(netwib_show_array_num_test2(16, 0x1234));
  netwib_er(netwib_show_array_num_test2(8, 0x12));
  netwib_er(netwib_show_array_num_test2(8, 'c'));
  netwib_er(netwib_show_array_num_test2(1, 1));
  netwib_er(netwib_show_array_num_test2(1, 0));
  netwib_er(netwib_show_array_num_test2(3, 7));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_show_array_fmt_test(void)
{
  netwib_buf buf;

  puts("++++----\nTest of netwib_show_array_fmt");
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_show_array_text32("Hello", &buf));
  netwib_er(netwib_show_array_fmt(32, NETWIB_SHOW_ARRAY_ALIGN_LEFT, '_',
                                  &buf, "hello%{uint32}", 53));
  netwib_er(netwib_show_array_line_begin(&buf));
  netwib_er(netwib_show_array_fmt(16, NETWIB_SHOW_ARRAY_ALIGN_CENTER, ' ',
                                  &buf, "hello%{uint32}", 53));
  netwib_er(netwib_show_array_fmt(16, NETWIB_SHOW_ARRAY_ALIGN_RIGHT, ' ',
                                  &buf, "hello%{uint32}", 53));
  netwib_er(netwib_show_array_line_end(&buf));

  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_show_array_fmt32_test(void)
{
  netwib_buf buf;

  puts("++++----\nTest of netwib_show_array_fmt32");
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_show_array_fmt32(&buf, "hello%{uint32}", 53));

  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_show_array_data_test(void)
{
  netwib_buf buf, data;

  puts("++++----\nTest of netwib_show_array_data");
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_buf_init_ext_string("data", &data));
  netwib_er(netwib_show_array_data("title", &data, NETWIB_ENCODETYPE_HEXA0,
                                   '_', &buf));
  netwib_er(netwib_buf_init_ext_string("data", &data));
  netwib_er(netwib_show_array_data(NULL, &data, NETWIB_ENCODETYPE_HEXA0,
                                   '_', &buf));
  netwib_er(netwib_buf_init_ext_string("datadatadatadatatadatadatatadatadatatadatadatadatadatada", &data));
  netwib_er(netwib_show_array_data("title", &data, NETWIB_ENCODETYPE_TEXT,
                                   '_', &buf));
  netwib_er(netwib_buf_init_ext_string("dataatadatadatatadatadatatadatadatatadatadatadatadatadaz", &data));
  netwib_er(netwib_show_array_data("title", &data, NETWIB_ENCODETYPE_TEXT,
                                   '_', &buf));
  netwib_er(netwib_buf_init_ext_string("datadatadatadatatadatadatatadatadatadatadatadatatadatadatatadatadatadatadatadatatadatadatatadatadatadatadatadatatadatadatatadatadatatadatadatadatadatadaz", &data));
  netwib_er(netwib_show_array_data("title", &data, NETWIB_ENCODETYPE_TEXT,
                                   '_', &buf));
  netwib_er(netwib_buf_init_ext_string("Netwib provides a kind of function format. It is not compatible with standard function, but it corresponds to netwib current usage.", &data));
  netwib_er(netwib_show_array_data(NULL, &data, NETWIB_ENCODETYPE_TEXT, '_', &buf));
  netwib_er(netwib_show_array_data("title", &data, NETWIB_ENCODETYPE_TEXT,
                                   '_', &buf));
  netwib_er(netwib_show_array_data("title very long aze aze aze aze aze aze aze aze aze", &data, NETWIB_ENCODETYPE_TEXT, '_', &buf));
  netwib_er(netwib_show_array_data(" title very long aze aze aze aze aze aze aze aze aze aze aze aze aze aze aze aze aze aze aze aze aze aze aze aze aze aze", &data, NETWIB_ENCODETYPE_TEXT, '_', &buf));
  netwib_er(netwib_buf_init_ext_string("Netwib", &data));
  netwib_er(netwib_show_array_data("title very long aze aze aze aze aze aze aze aze aze", &data, NETWIB_ENCODETYPE_TEXT, '_', &buf));

  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_shw_array(void)
{
  puts("###########################################\nTest of shw/array");
  netwib_er(netwib_show_array_head_test());
  netwib_er(netwib_show_array_tail_test());
  netwib_er(netwib_show_array_line_begin_test());
  netwib_er(netwib_show_array_line_end_test());
  netwib_er(netwib_show_array_text_test());
  netwib_er(netwib_show_array_num_test());
  netwib_er(netwib_show_array_fmt_test());
  netwib_er(netwib_show_array_fmt32_test());
  netwib_er(netwib_show_array_data_test());

  return(NETWIB_ERR_OK);
}

