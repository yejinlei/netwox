#include "netwibtest.h"


/*-------------------------------------------------------------*/
static netwib_err netwib_eths_init_test(void)
{
  netwib_eths *peths;

  puts("++++----\nTest of netwib_eths_init");
  netwib_er(netwib_eths_initdefault(&peths));
  netwib_er(netwib_eths_close(&peths));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_eths_close_test(void)
{
  netwib_eths *peths;

  puts("++++----\nTest of netwib_eths_close");
  netwib_er(netwib_eths_initdefault(&peths));
  netwib_er(netwib_eths_close(&peths));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_eths_add_eth_test(void)
{
  netwib_eth eth;
  netwib_eths *peths;

  puts("++++----\nTest of netwib_eths_add_eth");
  netwib_er(netwib_eths_initdefault(&peths));
  netwib_er(netwibtest_eths_content(peths, ""));
  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &eth));
  netwib_er(netwib_eths_add_eth(peths, &eth));
  netwib_er(netwibtest_eths_content(peths, "01:02:03:04:05:06"));
  netwib_er(netwib_eths_close(&peths));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_eths_add_ethrange_test(void)
{
  netwib_eth eth1, eth2;
  netwib_eths *peths;

  puts("++++----\nTest of netwib_eths_add_ethrange");
  netwib_er(netwib_eths_initdefault(&peths));
  netwib_er(netwibtest_eths_content(peths, ""));
  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &eth1));
  netwib_er(netwib_eth_init_fields(3,2,3,4,5,6, &eth2));
  netwib_er(netwib_eths_add_ethrange(peths, &eth1, &eth2));
  netwib_er(netwibtest_eths_content(peths, "01:02:03:04:05:06-03:02:03:04:05:06"));
  netwib_er(netwib_eths_close(&peths));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_eths_add_eths_test(void)
{
  netwib_eth eth1, eth2;
  netwib_eths *peths1, *peths2;

  puts("++++----\nTest of netwib_eths_add_eths");
  netwib_er(netwib_eths_initdefault(&peths1));
  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &eth1));
  netwib_er(netwib_eth_init_fields(3,2,3,4,5,6, &eth2));
  netwib_er(netwib_eths_add_ethrange(peths1, &eth1, &eth2));
  netwib_er(netwibtest_eths_content(peths1, "01:02:03:04:05:06-03:02:03:04:05:06"));
  netwib_er(netwib_eths_initdefault(&peths2));
  netwib_er(netwib_eth_init_fields(2,2,3,4,5,6, &eth1));
  netwib_er(netwib_eth_init_fields(7,2,3,4,5,6, &eth2));
  netwib_er(netwib_eths_add_ethrange(peths2, &eth1, &eth2));
  netwib_er(netwibtest_eths_content(peths2, "02:02:03:04:05:06-07:02:03:04:05:06"));
  netwib_er(netwib_eths_add_eths(peths1, peths2));
  netwib_er(netwibtest_eths_content(peths1, "01:02:03:04:05:06-07:02:03:04:05:06"));
  netwib_er(netwibtest_eths_content(peths2, "02:02:03:04:05:06-07:02:03:04:05:06"));

  netwib_er(netwib_eths_close(&peths1));
  netwib_er(netwib_eths_close(&peths2));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_eths_add_buf_test2(netwib_conststring text,
                                            netwib_conststring wantedtext)
{
  netwib_buf buf;
  netwib_eths *peths;
  netwib_err ret;

  netwib_er(netwib_eths_initdefault(&peths));
  netwib_er(netwib_buf_init_ext_string(text, &buf));
  ret = netwib_eths_add_buf(peths, &buf);
  if (ret == NETWIB_ERR_OK) {
    ret = netwibtest_eths_content(peths, wantedtext);
  }
  netwib_er(netwib_eths_close(&peths));

  return(ret);
}
static netwib_err netwib_eths_add_buf_test(void)
{
  puts("++++----\nTest of netwib_eths_add_buf");
  netwib_er(netwib_eths_add_buf_test2("1:2:3:4:5:6,6:6:6:6:6:6", "01:02:03:04:05:06,06:06:06:06:06:06"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_eths_add_kbd_test(void)
{
  netwib_buf buf, defbuf;
  netwib_eths *peths;

  puts("++++----\nTest of netwib_eths_add_kbd");

  netwib_er(netwib_eths_initdefault(&peths));
  netwib_er(netwib_buf_init_ext_string("Enter \"a:b:c:d:e:f\"", &buf));
  netwib_er(netwib_eths_add_kbd(peths, &buf, NULL));
  netwib_er(netwibtest_eths_content(peths, "0A:0B:0C:0D:0E:0F"));
  netwib_er(netwib_eths_close(&peths));

  netwib_er(netwib_eths_initdefault(&peths));
  netwib_er(netwib_buf_init_ext_string("Press 'Enter'", &buf));
  netwib_er(netwib_buf_init_ext_string("0a:0b:0c:0d:0e:0f", &defbuf));
  netwib_er(netwib_eths_add_kbd(peths, &buf, &defbuf));
  netwib_er(netwibtest_eths_content(peths, "0A:0B:0C:0D:0E:0F"));
  netwib_er(netwib_eths_close(&peths));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_eths_del_eth_test(void)
{
  netwib_eth eth;
  netwib_eths *peths;

  puts("++++----\nTest of netwib_eths_del_eth");
  netwib_er(netwib_eths_initdefault(&peths));
  netwib_er(netwibtest_eths_content(peths, ""));
  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &eth));
  netwib_er(netwib_eths_add_eth(peths, &eth));
  netwib_er(netwibtest_eths_content(peths, "01:02:03:04:05:06"));
  netwib_er(netwib_eths_del_eth(peths, &eth));
  netwib_er(netwibtest_eths_content(peths, ""));
  netwib_er(netwib_eths_close(&peths));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_eths_del_ethrange_test(void)
{
  netwib_eth eth1, eth2;
  netwib_eths *peths;

  puts("++++----\nTest of netwib_eths_del_ethrange");
  netwib_er(netwib_eths_initdefault(&peths));
  netwib_er(netwibtest_eths_content(peths, ""));
  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &eth1));
  netwib_er(netwib_eth_init_fields(3,2,3,4,5,6, &eth2));
  netwib_er(netwib_eths_add_ethrange(peths, &eth1, &eth2));
  netwib_er(netwibtest_eths_content(peths, "01:02:03:04:05:06-03:02:03:04:05:06"));
  netwib_er(netwib_eths_del_ethrange(peths, &eth1, &eth2));
  netwib_er(netwibtest_eths_content(peths, ""));
  netwib_er(netwib_eths_close(&peths));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_eths_del_eths_test(void)
{
  netwib_eth eth1, eth2;
  netwib_eths *peths1, *peths2;

  puts("++++----\nTest of netwib_eths_del_eths");
  netwib_er(netwib_eths_initdefault(&peths1));
  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &eth1));
  netwib_er(netwib_eth_init_fields(3,2,3,4,5,6, &eth2));
  netwib_er(netwib_eths_add_ethrange(peths1, &eth1, &eth2));
  netwib_er(netwibtest_eths_content(peths1, "01:02:03:04:05:06-03:02:03:04:05:06"));
  netwib_er(netwib_eths_initdefault(&peths2));
  netwib_er(netwib_eth_init_fields(2,2,3,4,5,6, &eth1));
  netwib_er(netwib_eth_init_fields(7,2,3,4,5,6, &eth2));
  netwib_er(netwib_eths_add_ethrange(peths2, &eth1, &eth2));
  netwib_er(netwibtest_eths_content(peths2, "02:02:03:04:05:06-07:02:03:04:05:06"));
  netwib_er(netwib_eths_del_eths(peths1, peths2));
  netwib_er(netwibtest_eths_content(peths1, "01:02:03:04:05:06-02:02:03:04:05:05"));
  netwib_er(netwibtest_eths_content(peths2, "02:02:03:04:05:06-07:02:03:04:05:06"));

  netwib_er(netwib_eths_close(&peths1));
  netwib_er(netwib_eths_close(&peths2));


  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_eths_contains_eth_test(void)
{
  netwib_eth eth;
  netwib_eths *peths;
  netwib_bool b;

  puts("++++----\nTest of netwib_eths_contains_eth");
  netwib_er(netwib_eths_initdefault(&peths));
  netwib_er(netwibtest_eths_content(peths, ""));
  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &eth));
  netwib_er(netwib_eths_add_eth(peths, &eth));
  netwib_er(netwibtest_eths_content(peths, "01:02:03:04:05:06"));
  netwib_er(netwib_eths_contains_eth(peths, &eth, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_eths_close(&peths));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_eths_contains_ethrange_test(void)
{
  netwib_eth eth1, eth2;
  netwib_eths *peths;
  netwib_bool b;

  puts("++++----\nTest of netwib_eths_contains_ethrange");
  netwib_er(netwib_eths_initdefault(&peths));
  netwib_er(netwibtest_eths_content(peths, ""));
  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &eth1));
  netwib_er(netwib_eth_init_fields(3,2,3,4,5,6, &eth2));
  netwib_er(netwib_eths_add_ethrange(peths, &eth1, &eth2));
  netwib_er(netwibtest_eths_content(peths, "01:02:03:04:05:06-03:02:03:04:05:06"));
  netwib_er(netwib_eths_contains_ethrange(peths, &eth1, &eth2, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_eths_close(&peths));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_eths_test(void)
{
  netwib_eth eth1, eth2;
  netwib_eths *peths;
  netwib_buf buf;

  puts("++++----\nTest of netwib_buf_append_eths");
  netwib_er(netwib_eths_initdefault(&peths));
  netwib_er(netwibtest_eths_content(peths, ""));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_eths(peths, &buf));
  netwib_er(netwibtest_buf_content2(&buf, ""));
  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &eth1));
  netwib_er(netwib_eth_init_fields(3,2,3,4,5,6, &eth2));
  netwib_er(netwib_eths_add_ethrange(peths, &eth1, &eth2));
  netwib_er(netwib_buf_append_eths(peths, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "01:02:03:04:05:06-03:02:03:04:05:06"));
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_eths_close(&peths));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_net_eths(void)
{
  puts("###########################################\nTest of net/eths");
  netwib_er(netwib_eths_init_test());
  netwib_er(netwib_eths_close_test());

  netwib_er(netwib_eths_add_eth_test());
  netwib_er(netwib_eths_add_ethrange_test());
  netwib_er(netwib_eths_add_eths_test());

  netwib_er(netwib_eths_add_buf_test());
  if (netwibtest_testkbdlevel == 2) {
    netwib_er(netwib_eths_add_kbd_test());
  }

  netwib_er(netwib_eths_del_eth_test());
  netwib_er(netwib_eths_del_ethrange_test());
  netwib_er(netwib_eths_del_eths_test());

  netwib_er(netwib_eths_contains_eth_test());
  netwib_er(netwib_eths_contains_ethrange_test());

  netwib_er(netwib_buf_append_eths_test());

  return(NETWIB_ERR_OK);
}
