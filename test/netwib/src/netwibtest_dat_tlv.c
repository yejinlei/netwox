#include "netwibtest.h"


/*-------------------------------------------------------------*/
static netwib_err netwib_tlv_append_buf_test(void)
{
  netwib_buf buf, tlv;

  puts("++++----\nTest of netwib_tlv_append_buf");

  netwib_er(netwib_buf_init_mallocdefault(&tlv));
  netwib_eir(netwib_tlv_append_buf(NULL, &tlv), NETWIB_ERR_PANULLPTR);
  netwib_er(netwib_buf_close(&tlv));

  netwib_er(netwib_buf_init_ext_string("abcdef", &buf));
  netwib_er(netwib_tlv_append_buf(&buf, NULL));

  netwib_er(netwib_buf_init_ext_string("abcdef", &buf));
  netwib_er(netwib_buf_init_mallocdefault(&tlv));
  netwib_er(netwib_tlv_append_buf(&buf, &tlv));
  netwib_er(netwibtest_buf_content3(&tlv, "0000000100000006 'abcdef'"));
  netwib_er(netwib_buf_close(&tlv));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_tlv_append_uint32_test(void)
{
  netwib_buf tlv;

  puts("++++----\nTest of netwib_tlv_append_uint32");
  netwib_er(netwib_tlv_append_uint32(1234, NULL));

  netwib_er(netwib_buf_init_mallocdefault(&tlv));
  netwib_er(netwib_tlv_append_uint32(0x12345678, &tlv));
  netwib_er(netwibtest_buf_content3(&tlv, "00000002 00000004 12345678"));
  netwib_er(netwib_buf_close(&tlv));

  netwib_er(netwib_buf_init_mallocdefault(&tlv));
  netwib_er(netwib_tlv_append_uint32(0x00123456, &tlv));
  netwib_er(netwibtest_buf_content3(&tlv, "00000002 00000004 00123456"));
  netwib_er(netwib_buf_close(&tlv));

  netwib_er(netwib_buf_init_mallocdefault(&tlv));
  netwib_er(netwib_tlv_append_uint32(0x00001234, &tlv));
  netwib_er(netwibtest_buf_content3(&tlv, "00000002 00000002 1234"));
  netwib_er(netwib_buf_close(&tlv));

  netwib_er(netwib_buf_init_mallocdefault(&tlv));
  netwib_er(netwib_tlv_append_uint32(0x00000034, &tlv));
  netwib_er(netwibtest_buf_content3(&tlv, "00000002 00000001 34"));
  netwib_er(netwib_buf_close(&tlv));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_tlv_append_uint64_test(void)
{
  netwib_buf tlv;
  netwib_uint64 ui;

  puts("++++----\nTest of netwib_tlv_append_uint64");

  netwib_er(netwib_buf_init_mallocdefault(&tlv));
  netwib_er(netwibtest_uint64_init("0000000012345678", &ui));
  netwib_er(netwib_tlv_append_uint64(ui, &tlv));
  netwib_er(netwibtest_buf_content3(&tlv, "00000002 00000004 12345678"));
  netwib_er(netwib_buf_close(&tlv));

  netwib_er(netwib_buf_init_mallocdefault(&tlv));
  netwib_er(netwibtest_uint64_init("000000123456789A", &ui));
  netwib_er(netwib_tlv_append_uint64(ui, &tlv));
  netwib_er(netwibtest_buf_content3(&tlv,
                                    "00000002 00000008 00000012 3456789A"));
  netwib_er(netwib_buf_close(&tlv));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_tlv_append_eth_test(void)
{
  netwib_buf tlv;
  netwib_eth eth;

  puts("++++----\nTest of netwib_tlv_append_eth");
  eth.b[0] = 0x12;
  eth.b[1] = 0x34;
  eth.b[2] = 0x56;
  eth.b[3] = 0x8D;
  eth.b[4] = 0x5C;
  eth.b[5] = 0x13;

  netwib_er(netwib_buf_init_mallocdefault(&tlv));
  netwib_er(netwib_buf_close(&tlv));

  netwib_er(netwib_tlv_append_eth(&eth, NULL));

  netwib_er(netwib_buf_init_mallocdefault(&tlv));
  netwib_er(netwib_tlv_append_eth(&eth, &tlv));
  netwib_er(netwibtest_buf_content3(&tlv, "0000000300000006 1234568D5C13"));
  netwib_er(netwib_buf_close(&tlv));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_tlv_append_ip_test(void)
{
  netwib_buf tlv;
  netwib_ip ipa;

  puts("++++----\nTest of netwib_tlv_append_ip");
  netwib_er(netwib_buf_init_mallocdefault(&tlv));
  netwib_er(netwib_ip_init_ip4(0x12345678, &ipa));
  netwib_er(netwib_tlv_append_ip(&ipa, &tlv));
  netwib_er(netwibtest_buf_content3(&tlv, "00000004 00000004 12345678"));
  netwib_er(netwib_buf_close(&tlv));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_tlv_append_end_test(void)
{
  netwib_buf tlv;

  puts("++++----\nTest of netwib_tlv_append_end");
  netwib_er(netwib_tlv_append_end(NULL));

  netwib_er(netwib_buf_init_mallocdefault(&tlv));
  netwib_er(netwib_tlv_append_end(&tlv));
  netwib_er(netwibtest_buf_content3(&tlv, "00000064 00000000"));
  netwib_er(netwib_buf_close(&tlv));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_tlv_append_newtype_test(void)
{
  netwib_buf buf, tlv;

  puts("++++----\nTest of netwib_tlv_append_newtype");

  netwib_er(netwib_buf_init_mallocdefault(&tlv));
  netwib_er(netwib_tlv_append_newtype(NETWIB_TLVTYPE_USER_BEGIN, NULL, &tlv));
  netwib_er(netwibtest_buf_content3(&tlv, "0000271000000000"));
  netwib_er(netwib_buf_close(&tlv));

  netwib_er(netwib_buf_init_ext_string("abcdef", &buf));
  netwib_er(netwib_tlv_append_newtype(NETWIB_TLVTYPE_USER_BEGIN, &buf, NULL));

  netwib_er(netwib_buf_init_ext_string("abcdef", &buf));
  netwib_er(netwib_buf_init_mallocdefault(&tlv));
  netwib_er(netwib_tlv_append_newtype(NETWIB_TLVTYPE_USER_BEGIN, &buf, &tlv));
  netwib_er(netwibtest_buf_content3(&tlv, "0000271000000006 'abcdef'"));
  netwib_er(netwib_buf_close(&tlv));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_tlv_append_tlv_test(void)
{
  netwib_buf tlv1, tlv2;

  puts("++++----\nTest of netwib_tlv_append_tlv");

  netwib_er(netwib_buf_init_mallocdefault(&tlv1));
  netwib_er(netwib_buf_init_mallocdefault(&tlv2));
  netwib_er(netwib_tlv_append_uint32(0x12345678, &tlv1));
  netwib_er(netwibtest_buf_content3(&tlv1, "00000002 00000004 12345678"));
  netwib_er(netwib_tlv_append_uint32(0x1212, &tlv2));
  netwib_er(netwibtest_buf_content3(&tlv2, "00000002 00000002 1212"));
  netwib_er(netwib_tlv_append_tlv(&tlv1, &tlv2));
  netwib_er(netwibtest_buf_content3(&tlv2, "00000002 00000002 1212  00000002 00000004 12345678"));
  netwib_er(netwib_buf_close(&tlv1));
  netwib_er(netwib_buf_close(&tlv2));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_tlv_prepend_tlv_test(void)
{
  netwib_buf tlv1, tlv2;

  puts("++++----\nTest of netwib_tlv_prepend_tlv");

  netwib_er(netwib_buf_init_mallocdefault(&tlv1));
  netwib_er(netwib_buf_init_mallocdefault(&tlv2));
  netwib_er(netwib_tlv_append_uint32(0x12345678, &tlv1));
  netwib_er(netwibtest_buf_content3(&tlv1, "00000002 00000004 12345678"));
  netwib_er(netwib_tlv_append_uint32(0x1212, &tlv2));
  netwib_er(netwibtest_buf_content3(&tlv2, "00000002 00000002 1212"));
  netwib_er(netwib_tlv_prepend_tlv(&tlv1, &tlv2));
  netwib_er(netwibtest_buf_content3(&tlv2, "00000002 00000004 12345678  00000002 00000002 1212"));
  netwib_er(netwib_buf_close(&tlv1));
  netwib_er(netwib_buf_close(&tlv2));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
static netwib_err netwib_tlv_entry_typelen_test(void)
{
  netwib_buf tlv;
  netwib_tlvtype type;
  netwib_uint32 length, skipsize;

  puts("++++----\nTest of netwib_tlv_entry_typelen");
  netwib_eir(netwib_tlv_entry_typelen(NULL, &type, &length, &skipsize),
             NETWIB_ERR_PANULLPTR);

  netwib_er(netwib_buf_init_mallocdefault(&tlv));
  netwib_er(netwib_tlv_append_uint32(0x12345678, &tlv));
  netwib_er(netwibtest_buf_content3(&tlv, "00000002 00000004 12345678"));
  netwib_er(netwib_tlv_entry_typelen(&tlv, &type, &length, &skipsize));
  netwib_er(netwib_buf_close(&tlv));
  if (type != NETWIB_TLVTYPE_UINT) {
    return(NETWIBTEST_ERR_0);
  }
  if (length != 4) {
    return(NETWIBTEST_ERR_1);
  }
  if (skipsize != 12) {
    return(NETWIBTEST_ERR_1);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_tlv_decode_buf_test(void)
{
  netwib_buf tlv, buf;
  netwib_uint32 skipsize;

  puts("++++----\nTest of netwib_tlv_decode_buf");
  netwib_er(netwib_buf_init_mallocdefault(&tlv));
  netwib_er(netwib_buf_init_ext_string("abcdef", &buf));
  netwib_er(netwib_tlv_append_buf(&buf, &tlv));
  netwib_er(netwib_tlv_append_uint32(0x1234, &tlv));
  netwib_er(netwib_tlv_append_end(&tlv));

  netwib_er(netwib_tlv_decode_buf(&tlv, &buf, &skipsize));
  netwib_er(netwibtest_buf_content2(&buf, "abcdef"));
  tlv.beginoffset += skipsize;

  netwib_eir(netwib_tlv_decode_buf(&tlv, &buf, &skipsize),
             NETWIB_ERR_DATAOTHERTYPE);
  tlv.beginoffset += skipsize;

  netwib_eir(netwib_tlv_decode_buf(&tlv, &buf, &skipsize),
             NETWIB_ERR_DATAEND);
  netwib_er(netwib_buf_close(&tlv));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_tlv_decode_uint32_test(void)
{
  netwib_buf tlv;
  netwib_uint32 ui, skipsize;
  netwib_ip ipa;

  puts("++++----\nTest of netwib_tlv_decode_uint32");
  netwib_er(netwib_buf_init_mallocdefault(&tlv));
  netwib_er(netwib_tlv_append_uint32(0x12345678, &tlv));
  netwib_er(netwib_tlv_append_uint32(0x1234, &tlv));
  netwib_er(netwib_tlv_append_uint32(0x12, &tlv));
  netwib_er(netwib_ip_init_ip4(0x7F000001, &ipa));
  netwib_er(netwib_tlv_append_ip(&ipa, &tlv));
  netwib_er(netwib_tlv_append_end(&tlv));

  netwib_er(netwib_tlv_decode_uint32(&tlv, &ui, &skipsize));
  if (ui != 0x12345678) {
    return(NETWIBTEST_ERR_0);
  }
  tlv.beginoffset += skipsize;
  netwib_er(netwib_tlv_decode_uint32(&tlv, &ui, &skipsize));
  if (ui != 0x1234) {
    return(NETWIBTEST_ERR_0);
  }
  tlv.beginoffset += skipsize;
  netwib_er(netwib_tlv_decode_uint32(&tlv, &ui, &skipsize));
  if (ui != 0x12) {
    return(NETWIBTEST_ERR_0);
  }
  tlv.beginoffset += skipsize;

  netwib_eir(netwib_tlv_decode_uint32(&tlv, &ui, &skipsize),
             NETWIB_ERR_DATAOTHERTYPE);
  tlv.beginoffset += skipsize;

  netwib_eir(netwib_tlv_decode_uint32(&tlv, &ui, &skipsize),
             NETWIB_ERR_DATAEND);
  netwib_er(netwib_buf_close(&tlv));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_tlv_decode_uint64_test(void)
{
  netwib_buf tlv;
  netwib_uint64 ui;
  netwib_uint32 skipsize;
  netwib_ip ipa;

  puts("++++----\nTest of netwib_tlv_decode_uint64");
  netwib_er(netwib_buf_init_mallocdefault(&tlv));
  netwib_er(netwibtest_uint64_init("0000000012345678", &ui));
  netwib_er(netwib_tlv_append_uint64(ui, &tlv));
  netwib_er(netwibtest_uint64_init("000000123456789A", &ui));
  netwib_er(netwib_tlv_append_uint64(ui, &tlv));
  netwib_er(netwib_ip_init_ip4(0x7F000001, &ipa));
  netwib_er(netwib_tlv_append_ip(&ipa, &tlv));
  netwib_er(netwib_tlv_append_end(&tlv));

  netwib_er(netwib_tlv_decode_uint64(&tlv, &ui, &skipsize));
  netwib_er(netwibtest_uint64_content(ui, "0000000012345678"));
  tlv.beginoffset += skipsize;
  netwib_er(netwib_tlv_decode_uint64(&tlv, &ui, &skipsize));
  netwib_er(netwibtest_uint64_content(ui, "000000123456789A"));
  tlv.beginoffset += skipsize;
  netwib_eir(netwib_tlv_decode_uint64(&tlv, &ui, &skipsize),
             NETWIB_ERR_DATAOTHERTYPE);
  tlv.beginoffset += skipsize;
  netwib_eir(netwib_tlv_decode_uint64(&tlv, &ui, &skipsize),
             NETWIB_ERR_DATAEND);
  netwib_er(netwib_buf_close(&tlv));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_tlv_decode_eth_test(void)
{
  netwib_buf tlv;
  netwib_uint32 skipsize;
  netwib_eth eth, eth2;

  puts("++++----\nTest of netwib_tlv_decode_eth");
  eth.b[0] = 0x12;
  eth.b[1] = 0x34;
  eth.b[2] = 0x56;
  eth.b[3] = 0x8D;
  eth.b[4] = 0x5C;
  eth.b[5] = 0x13;

  netwib_er(netwib_buf_init_mallocdefault(&tlv));
  netwib_er(netwib_tlv_append_eth(&eth, &tlv));
  netwib_er(netwib_tlv_append_uint32(0x1234, &tlv));
  netwib_er(netwib_tlv_append_end(&tlv));

  netwib_er(netwib_tlv_decode_eth(&tlv, &eth2, &skipsize));
  if (netwib_c_memcmp(eth2.b, eth.b, NETWIB_ETH_LEN)) {
    return(NETWIBTEST_ERR_3);
  }
  tlv.beginoffset += skipsize;

  netwib_eir(netwib_tlv_decode_eth(&tlv, &eth, &skipsize),
             NETWIB_ERR_DATAOTHERTYPE);
  tlv.beginoffset += skipsize;

  netwib_eir(netwib_tlv_decode_eth(&tlv, &eth, &skipsize),
             NETWIB_ERR_DATAEND);
  netwib_er(netwib_buf_close(&tlv));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_tlv_decode_ip_test(void)
{
  netwib_buf tlv;
  netwib_ip ip1, ip2;
  netwib_uint32 skipsize;
  netwib_cmp cmp;

  puts("++++----\nTest of netwib_tlv_decode_ip");
  netwib_er(netwib_buf_init_mallocdefault(&tlv));
  netwib_er(netwib_ip_init_ip4(0x7F000001, &ip1));
  netwib_er(netwib_tlv_append_ip(&ip1, &tlv));
  netwib_er(netwib_tlv_append_uint32(0x12, &tlv));
  netwib_er(netwib_tlv_append_end(&tlv));

  netwib_er(netwib_tlv_decode_ip(&tlv, &ip2, &skipsize));
  netwib_er(netwib_ip_cmp(&ip1, &ip2, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    return(NETWIBTEST_ERR_0);
  }
  tlv.beginoffset += skipsize;

  netwib_eir(netwib_tlv_decode_ip(&tlv, &ip2, &skipsize),
             NETWIB_ERR_DATAOTHERTYPE);
  tlv.beginoffset += skipsize;

  netwib_eir(netwib_tlv_decode_ip(&tlv, &ip2, &skipsize),
             NETWIB_ERR_DATAEND);
  netwib_er(netwib_buf_close(&tlv));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_tlv_decode_newtype_test(void)
{
  netwib_buf tlv, v;
  netwib_tlvtype t;
  netwib_uint32 skipsize;
  netwib_uint32 l;

  puts("++++----\nTest of netwib_tlv_decode_newtype");
  netwib_er(netwib_buf_init_mallocdefault(&tlv));
  netwib_er(netwib_buf_init_ext_string("abcdef", &v));
  netwib_er(netwib_tlv_append_newtype((netwib_tlvtype)1212, &v, &tlv));
  netwib_er(netwib_tlv_append_end(&tlv));

  netwib_er(netwib_tlv_decode_newtype(&tlv, &t, &l, &v, &skipsize));
  if (t != 1212) {
    return(NETWIBTEST_ERR_0);
  }
  if (l != 6) {
    return(NETWIBTEST_ERR_0);
  }
  netwib_er(netwibtest_buf_content2(&v, "abcdef"));
  tlv.beginoffset += skipsize;

  netwib_eir(netwib_tlv_decode_newtype(&tlv, &t, &l, &v, &skipsize),
             NETWIB_ERR_DATAEND);
  netwib_er(netwib_buf_close(&tlv));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_tlv_decode_tlv_test(void)
{
  netwib_buf tlv1, tlv2;
  netwib_uint32 skipsize;

  puts("++++----\nTest of netwib_tlv_decode_tlv");

  netwib_er(netwib_buf_init_mallocdefault(&tlv1));
  netwib_er(netwib_tlv_append_uint32(0x12345678, &tlv1));
  netwib_er(netwibtest_buf_content3(&tlv1, "00000002 00000004 12345678"));
  netwib_er(netwib_tlv_append_uint32(0x1212, &tlv1));
  netwib_er(netwibtest_buf_content3(&tlv1, "00000002 00000004 12345678  00000002 00000002 1212"));
  netwib_er(netwib_tlv_decode_tlv(&tlv1, &tlv2, &skipsize));
  netwib_er(netwibtest_buf_content3(&tlv2, "00000002 00000004 12345678"));
  netwib_er(netwib_buf_close(&tlv1));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_dat_tlv(void)
{
  puts("###########################################\nTest of dat/tlv");
  netwib_er(netwib_tlv_append_buf_test());
  netwib_er(netwib_tlv_append_uint32_test());
  netwib_er(netwib_tlv_append_uint64_test());
  netwib_er(netwib_tlv_append_eth_test());
  netwib_er(netwib_tlv_append_ip_test());
  netwib_er(netwib_tlv_append_end_test());
  netwib_er(netwib_tlv_append_newtype_test());

  netwib_er(netwib_tlv_append_tlv_test());
  netwib_er(netwib_tlv_prepend_tlv_test());

  netwib_er(netwib_tlv_entry_typelen_test());

  netwib_er(netwib_tlv_decode_buf_test());
  netwib_er(netwib_tlv_decode_uint32_test());
  netwib_er(netwib_tlv_decode_uint64_test());
  netwib_er(netwib_tlv_decode_eth_test());
  netwib_er(netwib_tlv_decode_ip_test());
  netwib_er(netwib_tlv_decode_newtype_test());

  netwib_er(netwib_tlv_decode_tlv_test());

  return(NETWIB_ERR_OK);
}

