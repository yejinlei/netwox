#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_eths_index_init_test(void)
{
  netwib_eths *peths;
  netwib_eths_index *pethsi;

  puts("++++----\nTest of netwib_eths_index_init");
  netwib_er(netwib_eths_initdefault(&peths));
  netwib_er(netwib_eths_index_init(peths, &pethsi));
  netwib_er(netwib_eths_index_close(&pethsi));
  netwib_er(netwib_eths_close(&peths));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_eths_index_close_test(void)
{
  netwib_eths *peths;
  netwib_eths_index *pethsi;

  puts("++++----\nTest of netwib_eths_index_close");
  netwib_er(netwib_eths_initdefault(&peths));
  netwib_er(netwib_eths_index_init(peths, &pethsi));
  netwib_er(netwib_eths_index_close(&pethsi));
  netwib_er(netwib_eths_close(&peths));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_eths_index_ctl_set_test(void)
{
  netwib_eths *peths;
  netwib_eths_index *pethsi;

  puts("++++----\nTest of netwib_eths_index_ctl_set");
  netwib_er(netwib_eths_initdefault(&peths));
  netwib_er(netwib_eths_index_init(peths, &pethsi));
  netwib_er(netwib_eths_index_ctl_set_rewind(pethsi));
  netwib_er(netwib_eths_index_ctl_set_rewind(pethsi));
  netwib_er(netwib_eths_index_ctl_set_index(pethsi, pethsi));
  netwib_er(netwib_eths_index_close(&pethsi));
  netwib_er(netwib_eths_close(&peths));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_eths_index_ctl_get_test(void)
{
  netwib_eths *peths;
  netwib_eths_index *pethsi;

  puts("++++----\nTest of netwib_eths_index_ctl_get");
  netwib_er(netwib_eths_initdefault(&peths));
  netwib_er(netwib_eths_index_init(peths, &pethsi));
  netwib_eir(netwib_eths_index_ctl_get(pethsi, NETWIB_ETHS_INDEX_CTLTYPE_REWIND,
                                       NULL, NULL),
             NETWIB_ERR_PAINVALIDTYPE);
  netwib_eir(netwib_eths_index_ctl_get(pethsi, NETWIB_ETHS_INDEX_CTLTYPE_INDEX,
                                       NULL, NULL),
             NETWIB_ERR_PAINVALIDTYPE);
  netwib_er(netwib_eths_index_close(&pethsi));
  netwib_er(netwib_eths_close(&peths));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_eths_index_next_eth_test(void)
{
  netwib_eths *peths;
  netwib_eths_index *pethsi;
  netwib_eth eth1, eth2;

  puts("++++----\nTest of netwib_eths_index_next_eth");
  netwib_er(netwib_eths_initdefault(&peths));
  netwib_er(netwib_eths_index_init(peths, &pethsi));
  netwib_eir(netwib_eths_index_next_eth(pethsi, &eth1), NETWIB_ERR_DATAEND);

  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &eth1));
  netwib_er(netwib_eth_init_fields(1,2,3,4,5,7, &eth2));
  netwib_er(netwib_eths_add_ethrange(peths, &eth1, &eth2));
  netwib_er(netwibtest_eths_content(peths, "01:02:03:04:05:06-01:02:03:04:05:07"));
  netwib_er(netwib_eths_index_ctl_set_rewind(pethsi));
  netwib_er(netwib_eths_index_next_eth(pethsi, &eth1));
  netwib_er(netwibtest_eth_content(&eth1, "01:02:03:04:05:06"));
  netwib_er(netwib_eths_index_next_eth(pethsi, &eth1));
  netwib_er(netwibtest_eth_content(&eth1, "01:02:03:04:05:07"));
  netwib_eir(netwib_eths_index_next_eth(pethsi, &eth1), NETWIB_ERR_DATAEND);
  netwib_er(netwib_eths_index_close(&pethsi));
  netwib_er(netwib_eths_close(&peths));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_eths_index_next_ethrange_test(void)
{
  netwib_eths *peths;
  netwib_eths_index *pethsi;
  netwib_eth eth1, eth2;

  puts("++++----\nTest of netwib_eths_index_next_ethrange");
  netwib_er(netwib_eths_initdefault(&peths));
  netwib_er(netwib_eths_index_init(peths, &pethsi));
  netwib_eir(netwib_eths_index_next_ethrange(pethsi, &eth1, &eth2),
             NETWIB_ERR_DATAEND);

  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &eth1));
  netwib_er(netwib_eth_init_fields(1,2,3,4,5,7, &eth2));
  netwib_er(netwib_eths_add_ethrange(peths, &eth1, &eth2));
  netwib_er(netwibtest_eths_content(peths, "01:02:03:04:05:06-01:02:03:04:05:07"));
  netwib_er(netwib_eths_index_ctl_set_rewind(pethsi));
  netwib_er(netwib_eths_index_next_ethrange(pethsi, &eth1, &eth2));
  netwib_er(netwibtest_eth_content(&eth1, "01:02:03:04:05:06"));
  netwib_er(netwibtest_eth_content(&eth2, "01:02:03:04:05:07"));
  netwib_eir(netwib_eths_index_next_eth(pethsi, &eth1), NETWIB_ERR_DATAEND);
  netwib_er(netwib_eths_index_close(&pethsi));
  netwib_er(netwib_eths_close(&peths));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_eths_index_this_eth_test(void)
{
  netwib_eths *peths;
  netwib_eths_index *pethsi;
  netwib_eth eth1, eth2;

  puts("++++----\nTest of netwib_eths_index_this_eth");
  netwib_er(netwib_eths_initdefault(&peths));
  netwib_er(netwib_eths_index_init(peths, &pethsi));
  netwib_eir(netwib_eths_index_this_eth(pethsi, &eth1), NETWIB_ERR_PAINDEXNODATA);

  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &eth1));
  netwib_er(netwib_eth_init_fields(1,2,3,4,5,7, &eth2));
  netwib_er(netwib_eths_add_ethrange(peths, &eth1, &eth2));
  netwib_er(netwibtest_eths_content(peths, "01:02:03:04:05:06-01:02:03:04:05:07"));
  netwib_er(netwib_eths_index_ctl_set_rewind(pethsi));
  netwib_er(netwib_eths_index_next_eth(pethsi, &eth1));
  netwib_er(netwibtest_eth_content(&eth1, "01:02:03:04:05:06"));
  netwib_er(netwib_eths_index_this_eth(pethsi, &eth1));
  netwib_er(netwibtest_eth_content(&eth1, "01:02:03:04:05:06"));
  netwib_er(netwib_eths_index_close(&pethsi));
  netwib_er(netwib_eths_close(&peths));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_eths_index_this_ethrange_test(void)
{
  netwib_eths *peths;
  netwib_eths_index *pethsi;
  netwib_eth eth1, eth2;

  puts("++++----\nTest of netwib_eths_index_this_ethrange");
  netwib_er(netwib_eths_initdefault(&peths));
  netwib_er(netwib_eths_index_init(peths, &pethsi));
  netwib_eir(netwib_eths_index_this_ethrange(pethsi, &eth1, &eth2),
             NETWIB_ERR_PAINDEXNODATA);

  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &eth1));
  netwib_er(netwib_eth_init_fields(1,2,3,4,5,7, &eth2));
  netwib_er(netwib_eths_add_ethrange(peths, &eth1, &eth2));
  netwib_er(netwibtest_eths_content(peths, "01:02:03:04:05:06-01:02:03:04:05:07"));
  netwib_er(netwib_eths_index_ctl_set_rewind(pethsi));
  netwib_er(netwib_eths_index_next_ethrange(pethsi, &eth1, &eth2));
  netwib_er(netwibtest_eth_content(&eth1, "01:02:03:04:05:06"));
  netwib_er(netwibtest_eth_content(&eth2, "01:02:03:04:05:07"));
  netwib_er(netwib_eths_index_this_ethrange(pethsi, &eth1, &eth2));
  netwib_er(netwibtest_eth_content(&eth1, "01:02:03:04:05:06"));
  netwib_er(netwibtest_eth_content(&eth2, "01:02:03:04:05:07"));
  netwib_er(netwib_eths_index_close(&pethsi));
  netwib_er(netwib_eths_close(&peths));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_eths_index_this_del_test(void)
{
  netwib_eths *peths;
  netwib_eths_index *pethsi;
  netwib_eth eth1, eth2;

  puts("++++----\nTest of netwib_eths_index_this_del");
  netwib_er(netwib_eths_initdefault(&peths));
  netwib_er(netwib_eths_index_init(peths, &pethsi));
  netwib_eir(netwib_eths_index_this_del(pethsi), NETWIB_ERR_PAINDEXNODATA);

  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &eth1));
  netwib_er(netwib_eth_init_fields(1,2,3,4,5,7, &eth2));
  netwib_er(netwib_eths_add_ethrange(peths, &eth1, &eth2));
  netwib_er(netwibtest_eths_content(peths, "01:02:03:04:05:06-01:02:03:04:05:07"));
  netwib_er(netwib_eths_index_ctl_set_rewind(pethsi));
  netwib_er(netwib_eths_index_next_eth(pethsi, &eth1));
  netwib_er(netwibtest_eth_content(&eth1, "01:02:03:04:05:06"));
  netwib_er(netwib_eths_index_this_del(pethsi));
  netwib_er(netwibtest_eths_content(peths, "01:02:03:04:05:07"));
  netwib_er(netwib_eths_index_next_eth(pethsi, &eth1));
  netwib_er(netwibtest_eth_content(&eth1, "01:02:03:04:05:07"));
  netwib_er(netwib_eths_index_close(&pethsi));
  netwib_er(netwib_eths_close(&peths));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_net_ethsi(void)
{
  puts("###########################################\nTest of net/ethsi");
  netwib_er(netwib_eths_index_init_test());
  netwib_er(netwib_eths_index_close_test());

  netwib_er(netwib_eths_index_ctl_set_test());
  netwib_er(netwib_eths_index_ctl_get_test());

  netwib_er(netwib_eths_index_next_eth_test());
  netwib_er(netwib_eths_index_next_ethrange_test());

  netwib_er(netwib_eths_index_this_eth_test());
  netwib_er(netwib_eths_index_this_ethrange_test());

  netwib_er(netwib_eths_index_this_del_test());

  return(NETWIB_ERR_OK);
}
