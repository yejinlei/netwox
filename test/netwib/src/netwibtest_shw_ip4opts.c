#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_ip4opts_show_test(void)
{
  netwib_buf pkt, buf, pkthexa, buftext;
  netwib_string text;

  puts("++++----\nTest of netwib_ip4opts_show");

  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_buf_init_ext_string("00 00 01 0717101234567822345678323456780000000000000000 2302 00 01", &pkthexa));
  netwib_er(netwib_buf_decode(&pkthexa, NETWIB_DECODETYPE_HEXA, &pkt));

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_ip4opts_show(&pkt, NETWIB_ENCODETYPE_SYNTH, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_fmt_display("\n"));
  netwib_er(netwibtest_buf_content3(&buf, "'ip4opts'"));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_ip4opts_show(&pkt, NETWIB_ENCODETYPE_ARRAY, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_buf_init_mallocdefault(&buftext));
  netwib_er(netwib_buf_append_string("'IP4OPTS_________________________________________________________.' 0A '| end                                                           |' 0A '| end                                                           |' 0A '| noop                                                          |' 0", &buftext));
  netwib_er(netwib_buf_append_string("A '| rr (storagesize=5 storedvalues=3) :                           |' 0A '|   18.52.86.120                                                |' 0A '|   34.52.86.120                                                |' 0A '|   50.52.86.12", &buftext));
  netwib_er(netwib_buf_append_string("0                                                |' 0A '| undecoded option: 23020001                                    |' 0A '|_______________________________________________________________|' 0A", &buftext));
  netwib_er(netwib_buf_ref_string(&buftext, &text));
  netwib_er(netwibtest_buf_content3(&buf, text));
  netwib_er(netwib_buf_close(&buftext));

  netwib_er(netwib_buf_close(&pkt));
  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_shw_ip4opts(void)
{
  puts("###########################################\nTest of shw/ip4opts");
  netwib_er(netwib_ip4opts_show_test());

  return(NETWIB_ERR_OK);
}
