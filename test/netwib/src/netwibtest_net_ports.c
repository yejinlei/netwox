#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_ports_init_test(void)
{
  netwib_ports *pports;

  puts("++++----\nTest of netwib_ports_init");
  netwib_er(netwib_ports_initdefault(&pports));
  netwib_er(netwib_ports_close(&pports));
  netwib_er(netwib_ports_init(NETWIB_PORTS_INITTYPE_NOTSORTNOTUNIQ, &pports));
  netwib_er(netwib_ports_close(&pports));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ports_close_test(void)
{
  netwib_ports *pports;

  puts("++++----\nTest of netwib_ports_close");
  netwib_er(netwib_ports_initdefault(&pports));
  netwib_er(netwib_ports_close(&pports));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ports_add_port_test(void)
{
  netwib_ports *pports;

  puts("++++----\nTest of netwib_ports_add_port");
  netwib_er(netwib_ports_initdefault(&pports));
  netwib_er(netwibtest_ports_content(pports, ""));
  netwib_er(netwib_ports_add_port(pports, 1234));
  netwib_er(netwibtest_ports_content(pports, "1234"));
  netwib_er(netwib_ports_add_port(pports, 45));
  netwib_er(netwibtest_ports_content(pports, "45,1234"));
  netwib_er(netwib_ports_add_port(pports, 45));
  netwib_er(netwibtest_ports_content(pports, "45,1234"));
  netwib_er(netwib_ports_close(&pports));

  netwib_er(netwib_ports_init(NETWIB_PORTS_INITTYPE_NOTSORTNOTUNIQ, &pports));
  netwib_er(netwibtest_ports_content(pports, ""));
  netwib_er(netwib_ports_add_port(pports, 1234));
  netwib_er(netwibtest_ports_content(pports, "1234"));
  netwib_er(netwib_ports_add_port(pports, 45));
  netwib_er(netwibtest_ports_content(pports, "1234,45"));
  netwib_er(netwib_ports_add_port(pports, 68));
  netwib_er(netwibtest_ports_content(pports, "1234,45,68"));
  netwib_er(netwib_ports_add_port(pports, 68));
  netwib_er(netwibtest_ports_content(pports, "1234,45,68,68"));
  netwib_er(netwib_ports_close(&pports));

  netwib_er(netwib_ports_init(NETWIB_PORTS_INITTYPE_NOTSORTUNIQ, &pports));
  netwib_er(netwibtest_ports_content(pports, ""));
  netwib_er(netwib_ports_add_port(pports, 1234));
  netwib_er(netwibtest_ports_content(pports, "1234"));
  netwib_er(netwib_ports_add_port(pports, 45));
  netwib_er(netwibtest_ports_content(pports, "1234,45"));
  netwib_er(netwib_ports_add_port(pports, 68));
  netwib_er(netwibtest_ports_content(pports, "1234,45,68"));
  netwib_er(netwib_ports_add_port(pports, 68));
  netwib_er(netwibtest_ports_content(pports, "1234,45,68"));
  netwib_er(netwib_ports_close(&pports));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ports_add_portrange_test(void)
{
  netwib_ports *pports;

  puts("++++----\nTest of netwib_ports_add_portrange");
  netwib_er(netwib_ports_initdefault(&pports));
  netwib_er(netwibtest_ports_content(pports, ""));
  netwib_eir(netwib_ports_add_portrange(pports, 35, 34),
            NETWIB_ERR_PAINVALIDRANGE);
  netwib_er(netwib_ports_add_portrange(pports, 23, 23));
  netwib_er(netwibtest_ports_content(pports, "23"));
  netwib_er(netwib_ports_add_portrange(pports, 25, 27));
  netwib_er(netwibtest_ports_content(pports, "23,25-27"));
  netwib_er(netwib_ports_close(&pports));

  netwib_er(netwib_ports_init(NETWIB_PORTS_INITTYPE_NOTSORTNOTUNIQ, &pports));
  netwib_er(netwib_ports_add_portrange(pports, 23, 23));
  netwib_er(netwibtest_ports_content(pports, "23"));
  netwib_er(netwib_ports_add_portrange(pports, 2, 3));
  netwib_er(netwibtest_ports_content(pports, "23,2-3"));
  netwib_er(netwib_ports_add_portrange(pports, 25, 27));
  netwib_er(netwibtest_ports_content(pports, "23,2-3,25-27"));
  netwib_er(netwib_ports_close(&pports));

  netwib_er(netwib_ports_init(NETWIB_PORTS_INITTYPE_NOTSORTUNIQ, &pports));
  netwib_er(netwib_ports_add_portrange(pports, 23, 23));
  netwib_er(netwibtest_ports_content(pports, "23"));
  netwib_er(netwib_ports_add_portrange(pports, 2, 3));
  netwib_er(netwibtest_ports_content(pports, "23,2-3"));
  netwib_er(netwib_ports_add_portrange(pports, 25, 27));
  netwib_er(netwibtest_ports_content(pports, "23,2-3,25-27"));
  netwib_er(netwib_ports_add_portrange(pports, 26, 26));
  netwib_er(netwibtest_ports_content(pports, "23,2-3,25,27,26"));
  netwib_er(netwib_ports_add_portrange(pports, 26, 27));
  netwib_er(netwibtest_ports_content(pports, "23,2-3,25,26-27"));
  netwib_er(netwib_ports_add_portrange(pports, 26, 28));
  netwib_er(netwibtest_ports_content(pports, "23,2-3,25,26-28"));
  netwib_er(netwib_ports_add_portrange(pports, 24, 26));
  netwib_er(netwibtest_ports_content(pports, "23,2-3,27-28,24-26"));
  netwib_er(netwib_ports_close(&pports));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ports_add_ports_test(void)
{
  netwib_ports *pports1, *pports2;

  puts("++++----\nTest of netwib_ports_add_ports");
  netwib_er(netwib_ports_initdefault(&pports1));
  netwib_er(netwib_ports_add_portrange(pports1, 1, 3));
  netwib_er(netwib_ports_add_portrange(pports1, 5, 7));
  netwib_er(netwibtest_ports_content(pports1, "1-3,5-7"));
  netwib_er(netwib_ports_initdefault(&pports2));
  netwib_er(netwib_ports_add_portrange(pports2, 2, 6));
  netwib_er(netwib_ports_add_portrange(pports2, 10, 11));
  netwib_er(netwibtest_ports_content(pports2, "2-6,10-11"));
  netwib_er(netwib_ports_add_ports(pports1, pports2));
  netwib_er(netwibtest_ports_content(pports1, "1-7,10-11"));
  netwib_er(netwibtest_ports_content(pports2, "2-6,10-11"));
  netwib_er(netwib_ports_close(&pports1));
  netwib_er(netwib_ports_close(&pports2));

  netwib_er(netwib_ports_init(NETWIB_PORTS_INITTYPE_NOTSORTNOTUNIQ, &pports1));
  netwib_er(netwib_ports_initdefault(&pports2));
  netwib_er(netwib_ports_add_portrange(pports1, 5, 7));
  netwib_er(netwib_ports_add_portrange(pports1, 1, 3));
  netwib_er(netwibtest_ports_content(pports1, "5-7,1-3"));
  netwib_er(netwib_ports_add_portrange(pports2, 2, 6));
  netwib_er(netwib_ports_add_portrange(pports2, 10, 11));
  netwib_er(netwibtest_ports_content(pports2, "2-6,10-11"));
  netwib_er(netwib_ports_add_ports(pports1, pports2));
  netwib_er(netwibtest_ports_content(pports1, "5-7,1-3,2-6,10-11"));
  netwib_er(netwibtest_ports_content(pports2, "2-6,10-11"));
  netwib_er(netwib_ports_close(&pports1));
  netwib_er(netwib_ports_close(&pports2));

  netwib_er(netwib_ports_init(NETWIB_PORTS_INITTYPE_NOTSORTNOTUNIQ, &pports1));
  netwib_er(netwib_ports_init(NETWIB_PORTS_INITTYPE_NOTSORTNOTUNIQ, &pports2));
  netwib_er(netwib_ports_add_portrange(pports1, 5, 7));
  netwib_er(netwib_ports_add_portrange(pports1, 1, 3));
  netwib_er(netwibtest_ports_content(pports1, "5-7,1-3"));
  netwib_er(netwib_ports_add_portrange(pports2, 10, 11));
  netwib_er(netwib_ports_add_portrange(pports2, 2, 6));
  netwib_er(netwibtest_ports_content(pports2, "10-11,2-6"));
  netwib_er(netwib_ports_add_ports(pports1, pports2));
  netwib_er(netwibtest_ports_content(pports1, "5-7,1-3,10-11,2-6"));
  netwib_er(netwibtest_ports_content(pports2, "10-11,2-6"));
  netwib_er(netwib_ports_close(&pports1));
  netwib_er(netwib_ports_close(&pports2));

  netwib_er(netwib_ports_initdefault(&pports1));
  netwib_er(netwib_ports_init(NETWIB_PORTS_INITTYPE_NOTSORTNOTUNIQ, &pports2));
  netwib_er(netwib_ports_add_portrange(pports1, 5, 7));
  netwib_er(netwib_ports_add_portrange(pports1, 1, 3));
  netwib_er(netwibtest_ports_content(pports1, "1-3,5-7"));
  netwib_er(netwib_ports_add_portrange(pports2, 10, 11));
  netwib_er(netwib_ports_add_portrange(pports2, 2, 6));
  netwib_er(netwibtest_ports_content(pports2, "10-11,2-6"));
  netwib_er(netwib_ports_add_ports(pports1, pports2));
  netwib_er(netwibtest_ports_content(pports1, "1-7,10-11"));
  netwib_er(netwibtest_ports_content(pports2, "10-11,2-6"));
  netwib_er(netwib_ports_close(&pports1));
  netwib_er(netwib_ports_close(&pports2));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ports_add_buf_test2(netwib_ports_inittype inittype,
                                             netwib_conststring text,
                                             netwib_conststring wantedtext)
{
  netwib_buf buf;
  netwib_ports *pports;
  netwib_err ret;

  netwib_er(netwib_ports_init(inittype, &pports));
  netwib_er(netwib_buf_init_ext_string(text, &buf));
  ret = netwib_ports_add_buf(pports, &buf);
  if (ret == NETWIB_ERR_OK) {
    ret = netwibtest_ports_content(pports, wantedtext);
  }
  netwib_er(netwib_ports_close(&pports));

  return(ret);
}
static netwib_err netwib_ports_add_buf_test(void)
{
  puts("++++----\nTest of netwib_ports_add_buf");
  netwib_er(netwib_ports_add_buf_test2(NETWIB_PORTS_INITTYPE_SORTUNIQ,
                                       "4556", "4556"));
  netwib_er(netwib_ports_add_buf_test2(NETWIB_PORTS_INITTYPE_SORTUNIQ,
                                       "45-56", "45-56"));
  netwib_er(netwib_ports_add_buf_test2(NETWIB_PORTS_INITTYPE_SORTUNIQ,
                                       "0-65535", "0-65535"));
  netwib_er(netwib_ports_add_buf_test2(NETWIB_PORTS_INITTYPE_SORTUNIQ,
                                       "all", "0-65535"));
  netwib_er(netwib_ports_add_buf_test2(NETWIB_PORTS_INITTYPE_SORTUNIQ,
                                       "45-56,!48,!50-51", "45-47,49,52-56"));

  netwib_eir(netwib_ports_add_buf_test2(NETWIB_PORTS_INITTYPE_SORTUNIQ,
                                        "345566", ""),
             NETWIB_ERR_NOTCONVERTED);
  netwib_eir(netwib_ports_add_buf_test2(NETWIB_PORTS_INITTYPE_SORTUNIQ,
                                        "65566", ""),
             NETWIB_ERR_NOTCONVERTED);
  netwib_eir(netwib_ports_add_buf_test2(NETWIB_PORTS_INITTYPE_SORTUNIQ,
                                        "5f66", ""),
             NETWIB_ERR_NOTCONVERTED);

  netwib_er(netwib_ports_add_buf_test2(NETWIB_PORTS_INITTYPE_NOTSORTNOTUNIQ,
                                       "4556", "4556"));
  netwib_er(netwib_ports_add_buf_test2(NETWIB_PORTS_INITTYPE_NOTSORTNOTUNIQ,
                                       "45-56", "45-56"));
  netwib_er(netwib_ports_add_buf_test2(NETWIB_PORTS_INITTYPE_NOTSORTNOTUNIQ,
                                       "0-65535", "0-65535"));
  netwib_er(netwib_ports_add_buf_test2(NETWIB_PORTS_INITTYPE_NOTSORTNOTUNIQ,
                                       "all", "0-65535"));
  netwib_er(netwib_ports_add_buf_test2(NETWIB_PORTS_INITTYPE_NOTSORTNOTUNIQ,
                                       "4-5,1-2", "4-5,1-2"));
  netwib_er(netwib_ports_add_buf_test2(NETWIB_PORTS_INITTYPE_NOTSORTNOTUNIQ,
                                       "45-56,!48,!50-51", "45-47,49,52-56"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ports_add_kbd_test(void)
{
  netwib_buf buf, defbuf;
  netwib_ports *pports;

  puts("++++----\nTest of netwib_ports_add_kbd");

  netwib_er(netwib_ports_initdefault(&pports));
  netwib_er(netwib_buf_init_ext_string("Enter \"123-456\"", &buf));
  netwib_er(netwib_ports_add_kbd(pports, &buf, NULL));
  netwib_er(netwibtest_ports_content(pports, "123-456"));
  netwib_er(netwib_ports_close(&pports));

  netwib_er(netwib_ports_init(NETWIB_PORTS_INITTYPE_NOTSORTNOTUNIQ, &pports));
  netwib_er(netwib_buf_init_ext_string("Press 'Enter'", &buf));
  netwib_er(netwib_buf_init_ext_string("768,123", &defbuf));
  netwib_er(netwib_ports_add_kbd(pports, &buf, &defbuf));
  netwib_er(netwibtest_ports_content(pports, "768,123"));
  netwib_er(netwib_ports_close(&pports));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ports_del_port_test(void)
{
  netwib_ports *pports;

  puts("++++----\nTest of netwib_ports_del_port");
  netwib_er(netwib_ports_initdefault(&pports));
  netwib_er(netwib_ports_add_portrange(pports, 4, 7));
  netwib_er(netwib_ports_add_port(pports, 9));
  netwib_er(netwibtest_ports_content(pports, "4-7,9"));
  netwib_er(netwib_ports_del_port(pports, 5));
  netwib_er(netwibtest_ports_content(pports, "4,6-7,9"));
  netwib_er(netwib_ports_del_port(pports, 6));
  netwib_er(netwibtest_ports_content(pports, "4,7,9"));
  netwib_er(netwib_ports_del_port(pports, 7));
  netwib_er(netwibtest_ports_content(pports, "4,9"));
  netwib_er(netwib_ports_close(&pports));

  netwib_er(netwib_ports_init(NETWIB_PORTS_INITTYPE_NOTSORTNOTUNIQ, &pports));
  netwib_er(netwib_ports_add_portrange(pports, 4, 7));
  netwib_er(netwib_ports_add_portrange(pports, 4, 7));
  netwib_er(netwib_ports_add_port(pports, 9));
  netwib_er(netwibtest_ports_content(pports, "4-7,4-7,9"));
  netwib_er(netwib_ports_del_port(pports, 5));
  netwib_er(netwibtest_ports_content(pports, "4,6-7,4,6-7,9"));
  netwib_er(netwib_ports_del_port(pports, 6));
  netwib_er(netwibtest_ports_content(pports, "4,7,4,7,9"));
  netwib_er(netwib_ports_del_port(pports, 7));
  netwib_er(netwibtest_ports_content(pports, "4,4,9"));
  netwib_er(netwib_ports_del_port(pports, 7));
  netwib_er(netwibtest_ports_content(pports, "4,4,9"));
  netwib_er(netwib_ports_del_port(pports, 4));
  netwib_er(netwibtest_ports_content(pports, "9"));
  netwib_er(netwib_ports_close(&pports));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ports_del_portrange_test(void)
{
  netwib_ports *pports;

  puts("++++----\nTest of netwib_ports_del_portrange");
  netwib_er(netwib_ports_initdefault(&pports));
  netwib_er(netwib_ports_add_portrange(pports, 2, 20));
  netwib_er(netwibtest_ports_content(pports, "2-20"));
  netwib_er(netwib_ports_del_portrange(pports, 1, 2));
  netwib_er(netwibtest_ports_content(pports, "3-20"));
  netwib_er(netwib_ports_del_portrange(pports, 1, 4));
  netwib_er(netwibtest_ports_content(pports, "5-20"));
  netwib_er(netwib_ports_del_portrange(pports, 20, 21));
  netwib_er(netwibtest_ports_content(pports, "5-19"));
  netwib_er(netwib_ports_del_portrange(pports, 18, 21));
  netwib_er(netwibtest_ports_content(pports, "5-17"));
  netwib_er(netwib_ports_del_portrange(pports, 10, 11));
  netwib_er(netwibtest_ports_content(pports, "5-9,12-17"));
  netwib_er(netwib_ports_del_portrange(pports, 9, 12));
  netwib_er(netwibtest_ports_content(pports, "5-8,13-17"));
  netwib_er(netwib_ports_del_portrange(pports, 7, 14));
  netwib_er(netwibtest_ports_content(pports, "5-6,15-17"));
  netwib_er(netwib_ports_del_portrange(pports, 16, 16));
  netwib_er(netwibtest_ports_content(pports, "5-6,15,17"));
  netwib_er(netwib_ports_del_portrange(pports, 13, 16));
  netwib_er(netwibtest_ports_content(pports, "5-6,17"));
  netwib_er(netwib_ports_del_portrange(pports, 17, 17));
  netwib_er(netwibtest_ports_content(pports, "5-6"));
  netwib_er(netwib_ports_del_portrange(pports, 6, 17));
  netwib_er(netwibtest_ports_content(pports, "5"));
  netwib_er(netwib_ports_del_portrange(pports, 4, 17));
  netwib_er(netwibtest_ports_content(pports, ""));
  netwib_er(netwib_ports_close(&pports));

  netwib_er(netwib_ports_init(NETWIB_PORTS_INITTYPE_NOTSORTNOTUNIQ, &pports));
  netwib_er(netwib_ports_add_portrange(pports, 3, 6));
  netwib_er(netwib_ports_add_port(pports, 2));
  netwib_er(netwib_ports_add_portrange(pports, 12, 14));
  netwib_er(netwib_ports_add_port(pports, 7));
  netwib_er(netwib_ports_add_portrange(pports, 7, 9));
  netwib_er(netwib_ports_add_port(pports, 8));
  netwib_er(netwib_ports_add_portrange(pports, 10, 13));
  netwib_er(netwib_ports_add_port(pports, 20));
  netwib_er(netwib_ports_add_portrange(pports, 15, 19));
  netwib_er(netwib_ports_add_port(pports, 17));
  netwib_er(netwibtest_ports_content(pports,
                                     "3-6,2,12-14,7,7-9,8,10-13,20,15-19,17"));
  netwib_er(netwib_ports_del_portrange(pports, 1, 2));
  netwib_er(netwibtest_ports_content(pports,
                                     "3-6,12-14,7,7-9,8,10-13,20,15-19,17"));
  netwib_er(netwib_ports_del_portrange(pports, 1, 4));
  netwib_er(netwibtest_ports_content(pports,
                                     "5-6,12-14,7,7-9,8,10-13,20,15-19,17"));
  netwib_er(netwib_ports_del_portrange(pports, 20, 21));
  netwib_er(netwibtest_ports_content(pports,
                                     "5-6,12-14,7,7-9,8,10-13,15-19,17"));
  netwib_er(netwib_ports_del_portrange(pports, 18, 21));
  netwib_er(netwibtest_ports_content(pports,
                                     "5-6,12-14,7,7-9,8,10-13,15-17,17"));
  netwib_er(netwib_ports_del_portrange(pports, 10, 11));
  netwib_er(netwibtest_ports_content(pports,
                                     "5-6,12-14,7,7-9,8,12-13,15-17,17"));
  netwib_er(netwib_ports_del_portrange(pports, 9, 12));
  netwib_er(netwibtest_ports_content(pports, "5-6,13-14,7,7-8,8,13,15-17,17"));
  netwib_er(netwib_ports_del_portrange(pports, 7, 14));
  netwib_er(netwibtest_ports_content(pports, "5-6,15-17,17"));
  netwib_er(netwib_ports_del_portrange(pports, 16, 16));
  netwib_er(netwibtest_ports_content(pports, "5-6,15,17,17"));
  netwib_er(netwib_ports_del_portrange(pports, 13, 16));
  netwib_er(netwibtest_ports_content(pports, "5-6,17,17"));
  netwib_er(netwib_ports_del_portrange(pports, 17, 17));
  netwib_er(netwibtest_ports_content(pports, "5-6"));
  netwib_er(netwib_ports_del_portrange(pports, 6, 17));
  netwib_er(netwibtest_ports_content(pports, "5"));
  netwib_er(netwib_ports_del_portrange(pports, 4, 17));
  netwib_er(netwibtest_ports_content(pports, ""));
  netwib_er(netwib_ports_close(&pports));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ports_del_ports_test(void)
{
  netwib_ports *pports1, *pports2;

  puts("++++----\nTest of netwib_ports_del_ports");
  netwib_er(netwib_ports_initdefault(&pports1));
  netwib_er(netwib_ports_add_portrange(pports1, 1, 3));
  netwib_er(netwib_ports_add_portrange(pports1, 5, 7));
  netwib_er(netwibtest_ports_content(pports1, "1-3,5-7"));
  netwib_er(netwib_ports_initdefault(&pports2));
  netwib_er(netwib_ports_add_portrange(pports2, 2, 6));
  netwib_er(netwib_ports_add_portrange(pports2, 10, 11));
  netwib_er(netwibtest_ports_content(pports2, "2-6,10-11"));
  netwib_er(netwib_ports_del_ports(pports1, pports2));
  netwib_er(netwibtest_ports_content(pports1, "1,7"));
  netwib_er(netwibtest_ports_content(pports2, "2-6,10-11"));
  netwib_er(netwib_ports_close(&pports1));
  netwib_er(netwib_ports_close(&pports2));

  netwib_er(netwib_ports_init(NETWIB_PORTS_INITTYPE_NOTSORTNOTUNIQ, &pports1));
  netwib_er(netwib_ports_add_portrange(pports1, 5, 7));
  netwib_er(netwib_ports_add_portrange(pports1, 1, 3));
  netwib_er(netwibtest_ports_content(pports1, "5-7,1-3"));
  netwib_er(netwib_ports_init(NETWIB_PORTS_INITTYPE_NOTSORTNOTUNIQ, &pports2));
  netwib_er(netwib_ports_add_portrange(pports2, 10, 11));
  netwib_er(netwib_ports_add_portrange(pports2, 2, 6));
  netwib_er(netwibtest_ports_content(pports2, "10-11,2-6"));
  netwib_er(netwib_ports_del_ports(pports1, pports2));
  netwib_er(netwibtest_ports_content(pports1, "7,1"));
  netwib_er(netwibtest_ports_content(pports2, "10-11,2-6"));
  netwib_er(netwib_ports_close(&pports1));
  netwib_er(netwib_ports_close(&pports2));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ports_contains_port_test(void)
{
  netwib_ports *pports;
  netwib_bool b;

  puts("++++----\nTest of netwib_ports_contains_port");
  netwib_er(netwib_ports_initdefault(&pports));
  netwib_er(netwib_ports_add_portrange(pports, 5, 7));

  netwib_er(netwib_ports_contains_port(pports, 6, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_ports_contains_port(pports, 3, &b));
  if (b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_ports_close(&pports));

  netwib_er(netwib_ports_init(NETWIB_PORTS_INITTYPE_NOTSORTNOTUNIQ, &pports));
  netwib_er(netwib_ports_add_portrange(pports, 5, 7));

  netwib_er(netwib_ports_contains_port(pports, 6, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_ports_contains_port(pports, 3, &b));
  if (b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_ports_close(&pports));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ports_contains_portrange_test(void)
{
  netwib_ports *pports;
  netwib_bool b;

  puts("++++----\nTest of netwib_ports_contains_portrange");
  netwib_er(netwib_ports_initdefault(&pports));
  netwib_er(netwib_ports_add_portrange(pports, 5, 7));
  netwib_er(netwib_ports_add_portrange(pports, 8, 9));
  netwib_er(netwib_ports_contains_portrange(pports, 6, 7, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_ports_contains_portrange(pports, 6, 19, &b));
  if (b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_ports_close(&pports));

  netwib_er(netwib_ports_init(NETWIB_PORTS_INITTYPE_NOTSORTNOTUNIQ, &pports));
  netwib_er(netwib_ports_add_portrange(pports, 8, 9));
  netwib_er(netwib_ports_add_portrange(pports, 5, 7));
  netwib_er(netwib_ports_contains_portrange(pports, 6, 7, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_ports_contains_portrange(pports, 6, 8, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_ports_contains_portrange(pports, 6, 19, &b));
  if (b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_ports_close(&pports));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_ports_test(void)
{
  netwib_ports *pports;
  netwib_buf buf;

  puts("++++----\nTest of netwib_buf_append_ports");
  netwib_er(netwib_ports_initdefault(&pports));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_ports(pports, &buf));
  netwib_er(netwibtest_buf_content2(&buf, ""));
  netwib_er(netwib_ports_add_portrange(pports, 4, 6));
  netwib_er(netwib_buf_append_ports(pports, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "4-6"));
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_ports_close(&pports));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_net_ports(void)
{
  puts("###########################################\nTest of net/ports");
  netwib_er(netwib_ports_init_test());
  netwib_er(netwib_ports_close_test());

  netwib_er(netwib_ports_add_port_test());
  netwib_er(netwib_ports_add_portrange_test());
  netwib_er(netwib_ports_add_ports_test());

  netwib_er(netwib_ports_add_buf_test());
  if (netwibtest_testkbdlevel == 2) {
    netwib_er(netwib_ports_add_kbd_test());
  }

  netwib_er(netwib_ports_del_port_test());
  netwib_er(netwib_ports_del_portrange_test());
  netwib_er(netwib_ports_del_ports_test());

  netwib_er(netwib_ports_contains_port_test());
  netwib_er(netwib_ports_contains_portrange_test());

  netwib_er(netwib_buf_append_ports_test());

  return(NETWIB_ERR_OK);
}
