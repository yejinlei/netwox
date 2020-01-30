#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_ports_index_init_test(void)
{
  netwib_ports *pports;
  netwib_ports_index *pportsi;

  puts("++++----\nTest of netwib_ports_index_init");
  netwib_er(netwib_ports_initdefault(&pports));
  netwib_er(netwib_ports_index_init(pports, &pportsi));
  netwib_er(netwib_ports_index_close(&pportsi));
  netwib_er(netwib_ports_close(&pports));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ports_index_close_test(void)
{
  netwib_ports *pports;
  netwib_ports_index *pportsi;

  puts("++++----\nTest of netwib_ports_index_close");
  netwib_er(netwib_ports_initdefault(&pports));
  netwib_er(netwib_ports_index_init(pports, &pportsi));
  netwib_er(netwib_ports_index_close(&pportsi));
  netwib_er(netwib_ports_close(&pports));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ports_index_ctl_set_test(void)
{
  netwib_ports *pports;
  netwib_ports_index *pportsi;

  puts("++++----\nTest of netwib_ports_index_ctl_set");
  netwib_er(netwib_ports_initdefault(&pports));
  netwib_er(netwib_ports_index_init(pports, &pportsi));
  netwib_er(netwib_ports_index_ctl_set_rewind(pportsi));
  netwib_er(netwib_ports_index_ctl_set_rewind(pportsi));
  netwib_er(netwib_ports_index_ctl_set_index(pportsi, pportsi));
  netwib_er(netwib_ports_index_close(&pportsi));
  netwib_er(netwib_ports_close(&pports));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ports_index_ctl_get_test(void)
{
  netwib_ports *pports;
  netwib_ports_index *pportsi;

  puts("++++----\nTest of netwib_ports_index_ctl_get");
  netwib_er(netwib_ports_initdefault(&pports));
  netwib_er(netwib_ports_index_init(pports, &pportsi));
  netwib_eir(netwib_ports_index_ctl_get(pportsi,
                                        NETWIB_PORTS_INDEX_CTLTYPE_REWIND,
                                        NULL, NULL),
             NETWIB_ERR_PAINVALIDTYPE);
  netwib_eir(netwib_ports_index_ctl_get(pportsi,
                                        NETWIB_PORTS_INDEX_CTLTYPE_INDEX,
                                        NULL, NULL),
             NETWIB_ERR_PAINVALIDTYPE);
  netwib_er(netwib_ports_index_close(&pportsi));
  netwib_er(netwib_ports_close(&pports));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ports_index_next_port_test(void)
{
  netwib_ports *pports;
  netwib_ports_index *pportsi;
  netwib_port port;

  puts("++++----\nTest of netwib_ports_index_next_port");
  netwib_er(netwib_ports_initdefault(&pports));
  netwib_er(netwib_ports_index_init(pports, &pportsi));
  netwib_eir(netwib_ports_index_next_port(pportsi, &port), NETWIB_ERR_DATAEND);
  netwib_er(netwib_ports_add_portrange(pports, 123, 124));
  netwib_er(netwib_ports_add_port(pports, 234));
  netwib_er(netwibtest_ports_content(pports, "123-124,234"));
  netwib_er(netwib_ports_index_ctl_set_rewind(pportsi));
  netwib_er(netwib_ports_index_next_port(pportsi, &port));
  if (port != 123) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_er(netwib_ports_index_next_port(pportsi, &port));
  if (port != 124) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_er(netwib_ports_index_next_port(pportsi, &port));
  if (port != 234) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_eir(netwib_ports_index_next_port(pportsi, &port), NETWIB_ERR_DATAEND);
  netwib_er(netwib_ports_index_close(&pportsi));
  netwib_er(netwib_ports_close(&pports));

  netwib_er(netwib_ports_init(NETWIB_PORTS_INITTYPE_NOTSORTNOTUNIQ, &pports));
  netwib_er(netwib_ports_index_init(pports, &pportsi));
  netwib_eir(netwib_ports_index_next_port(pportsi, &port), NETWIB_ERR_DATAEND);
  netwib_er(netwib_ports_add_port(pports, 234));
  netwib_er(netwib_ports_add_portrange(pports, 123, 124));
  netwib_er(netwibtest_ports_content(pports, "234,123-124"));
  netwib_er(netwib_ports_index_ctl_set_rewind(pportsi));
  netwib_er(netwib_ports_index_next_port(pportsi, &port));
  if (port != 234) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_er(netwib_ports_index_next_port(pportsi, &port));
  if (port != 123) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_er(netwib_ports_index_next_port(pportsi, &port));
  if (port != 124) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_eir(netwib_ports_index_next_port(pportsi, &port), NETWIB_ERR_DATAEND);
  netwib_er(netwib_ports_index_close(&pportsi));
  netwib_er(netwib_ports_close(&pports));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ports_index_next_portrange_test(void)
{
  netwib_ports *pports;
  netwib_ports_index *pportsi;
  netwib_port portinf, portsup;

  puts("++++----\nTest of netwib_ports_index_next_portrange");
  netwib_er(netwib_ports_initdefault(&pports));
  netwib_er(netwib_ports_index_init(pports, &pportsi));
  netwib_er(netwib_ports_index_ctl_set_rewind(pportsi));
  netwib_eir(netwib_ports_index_next_portrange(pportsi, &portinf, &portsup),
             NETWIB_ERR_DATAEND);
  netwib_er(netwib_ports_add_portrange(pports, 123, 124));
  netwib_er(netwib_ports_add_port(pports, 234));
  netwib_er(netwib_ports_add_portrange(pports, 345, 348));
  netwib_er(netwibtest_ports_content(pports, "123-124,234,345-348"));
  netwib_er(netwib_ports_index_ctl_set_rewind(pportsi));
  netwib_er(netwib_ports_index_next_portrange(pportsi, &portinf, &portsup));
  if (portinf != 123 || portsup != 124) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_er(netwib_ports_index_next_portrange(pportsi, &portinf, &portsup));
  if (portinf != 234 || portsup != 234) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_er(netwib_ports_index_next_port(pportsi, &portinf));
  if (portinf != 345) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_er(netwib_ports_index_next_portrange(pportsi, &portinf, &portsup));
  if (portinf != 346 || portsup != 348) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_eir(netwib_ports_index_next_portrange(pportsi, &portinf, &portsup),
             NETWIB_ERR_DATAEND);
  netwib_er(netwib_ports_close(&pports));
  netwib_er(netwib_ports_index_close(&pportsi));

  netwib_er(netwib_ports_init(NETWIB_PORTS_INITTYPE_NOTSORTNOTUNIQ, &pports));
  netwib_er(netwib_ports_index_init(pports, &pportsi));
  netwib_er(netwib_ports_index_ctl_set_rewind(pportsi));
  netwib_eir(netwib_ports_index_next_portrange(pportsi, &portinf, &portsup),
             NETWIB_ERR_DATAEND);
  netwib_er(netwib_ports_add_portrange(pports, 345, 348));
  netwib_er(netwib_ports_add_port(pports, 234));
  netwib_er(netwib_ports_add_portrange(pports, 123, 124));
  netwib_er(netwibtest_ports_content(pports, "345-348,234,123-124"));
  netwib_er(netwib_ports_index_ctl_set_rewind(pportsi));
  netwib_er(netwib_ports_index_next_port(pportsi, &portinf));
  if (portinf != 345) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_er(netwib_ports_index_next_portrange(pportsi, &portinf, &portsup));
  if (portinf != 346 || portsup != 348) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_er(netwib_ports_index_next_portrange(pportsi, &portinf, &portsup));
  if (portinf != 234 || portsup != 234) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_er(netwib_ports_index_next_portrange(pportsi, &portinf, &portsup));
  if (portinf != 123 || portsup != 124) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_eir(netwib_ports_index_next_portrange(pportsi, &portinf, &portsup),
             NETWIB_ERR_DATAEND);
  netwib_er(netwib_ports_close(&pports));
  netwib_er(netwib_ports_index_close(&pportsi));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ports_index_this_port_test(void)
{
  netwib_ports *pports;
  netwib_ports_index *pportsi;
  netwib_port port;

  puts("++++----\nTest of netwib_ports_index_this_port");
  netwib_er(netwib_ports_initdefault(&pports));
  netwib_er(netwib_ports_index_init(pports, &pportsi));
  netwib_eir(netwib_ports_index_this_port(pportsi, &port),
             NETWIB_ERR_PAINDEXNODATA);

  netwib_er(netwib_ports_add_portrange(pports, 123, 124));
  netwib_er(netwib_ports_add_port(pports, 234));
  netwib_er(netwibtest_ports_content(pports, "123-124,234"));
  netwib_er(netwib_ports_index_ctl_set_rewind(pportsi));
  netwib_er(netwib_ports_index_next_port(pportsi, &port));
  if (port != 123) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_er(netwib_ports_index_next_port(pportsi, &port));
  if (port != 124) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_er(netwib_ports_index_this_port(pportsi, &port));
  if (port != 124) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_er(netwib_ports_index_close(&pportsi));
  netwib_er(netwib_ports_close(&pports));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ports_index_this_portrange_test(void)
{
  netwib_ports *pports;
  netwib_ports_index *pportsi;
  netwib_port portinf, portsup;

  puts("++++----\nTest of netwib_ports_index_this_portrange");
  netwib_er(netwib_ports_initdefault(&pports));
  netwib_er(netwib_ports_index_init(pports, &pportsi));
  netwib_er(netwib_ports_index_ctl_set_rewind(pportsi));
  netwib_eir(netwib_ports_index_this_portrange(pportsi, &portinf, &portsup),
             NETWIB_ERR_PAINDEXNODATA);

  netwib_er(netwib_ports_add_portrange(pports, 123, 124));
  netwib_er(netwib_ports_add_port(pports, 234));
  netwib_er(netwib_ports_add_portrange(pports, 345, 348));
  netwib_er(netwibtest_ports_content(pports, "123-124,234,345-348"));
  netwib_er(netwib_ports_index_ctl_set_rewind(pportsi));
  netwib_er(netwib_ports_index_next_portrange(pportsi, &portinf, &portsup));
  if (portinf != 123 || portsup != 124) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_er(netwib_ports_index_next_portrange(pportsi, &portinf, &portsup));
  if (portinf != 234 || portsup != 234) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_er(netwib_ports_index_this_portrange(pportsi, &portinf, &portsup));
  if (portinf != 234 || portsup != 234) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_er(netwib_ports_close(&pports));
  netwib_er(netwib_ports_index_close(&pportsi));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ports_index_this_del_test(void)
{
  netwib_ports *pports;
  netwib_ports_index *pportsi;
  netwib_port port;

  puts("++++----\nTest of netwib_ports_index_this_del");
  netwib_er(netwib_ports_initdefault(&pports));
  netwib_er(netwib_ports_index_init(pports, &pportsi));
  netwib_eir(netwib_ports_index_this_del(pportsi), NETWIB_ERR_PAINDEXNODATA);

  netwib_er(netwib_ports_add_portrange(pports, 123, 124));
  netwib_er(netwib_ports_add_port(pports, 234));
  netwib_er(netwibtest_ports_content(pports, "123-124,234"));
  netwib_er(netwib_ports_index_ctl_set_rewind(pportsi));
  netwib_er(netwib_ports_index_next_port(pportsi, &port));
  if (port != 123) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_er(netwib_ports_index_next_port(pportsi, &port));
  if (port != 124) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_er(netwib_ports_index_this_del(pportsi));
  netwib_er(netwibtest_ports_content(pports, "123,234"));
  netwib_er(netwib_ports_index_next_port(pportsi, &port));
  if (port != 234) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_er(netwib_ports_index_close(&pportsi));
  netwib_er(netwib_ports_close(&pports));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_net_portsi(void)
{
  puts("###########################################\nTest of net/portsi");
  netwib_er(netwib_ports_index_init_test());
  netwib_er(netwib_ports_index_close_test());

  netwib_er(netwib_ports_index_ctl_set_test());
  netwib_er(netwib_ports_index_ctl_get_test());

  netwib_er(netwib_ports_index_next_port_test());
  netwib_er(netwib_ports_index_next_portrange_test());

  netwib_er(netwib_ports_index_this_port_test());
  netwib_er(netwib_ports_index_this_portrange_test());

  netwib_er(netwib_ports_index_this_del_test());

  return(NETWIB_ERR_OK);
}
