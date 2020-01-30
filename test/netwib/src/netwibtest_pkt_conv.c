#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_convert_test2(netwib_device_dlttype dlttypein,
                                           netwib_constbuf *ppktin,
                                           netwib_device_dlttype dlttypeout,
                                           netwib_conststring wantedlinkhdr)
{
  netwib_linkhdr linkhdrout;

  netwib_er(netwib_pkt_convert(dlttypein, ppktin, NETWIB_PKT_NEWFIELD_BLANK,
                               NETWIB_PKT_NEWFIELD_BLANK,
                               dlttypeout, &linkhdrout, NULL));
  netwib_er(netwibtest_linkhdr_content(&linkhdrout, wantedlinkhdr));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pkt_convert_test(void)
{
  netwib_buf pkt;
  netwib_linkhdr linkhdrin;
  netwib_iphdr ipheader;

  puts("++++----\nTest of netwib_pkt_convert");

  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  linkhdrin.type = NETWIB_DEVICE_DLTTYPE_ETHER;
  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &linkhdrin.hdr.ether.src));
  netwib_er(netwib_eth_init_fields(2,2,3,4,5,6, &linkhdrin.hdr.ether.dst));
  linkhdrin.hdr.ether.type = NETWIB_ETHERHDRTYPE_IP4;
  netwib_er(netwib_pkt_append_linkhdr(&linkhdrin, &pkt));
  netwib_er(netwib_iphdr_initdefault(NETWIB_IPTYPE_IP4, &ipheader));
  netwib_er(netwib_pkt_append_iphdr(&ipheader, &pkt));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_ETHER, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_ETHER,
                                     "0202030405060102030405060800"));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_ETHER, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_NULL,
                                     "00000800"));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_ETHER, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_LOOP,
                                     "00000800"));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_ETHER, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_RAW,
                                     "''"));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_ETHER, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_PPP,
                                     "00000021"));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_ETHER, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_LINUX_SLL,
                                     "00000001000601020304050600000800"));
  netwib_er(netwib_buf_close(&pkt));

  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  linkhdrin.type = NETWIB_DEVICE_DLTTYPE_NULL;
  linkhdrin.hdr.null.type = NETWIB_ETHERHDRTYPE_IP4;
  netwib_er(netwib_pkt_append_linkhdr(&linkhdrin, &pkt));
  netwib_er(netwib_iphdr_initdefault(NETWIB_IPTYPE_IP4, &ipheader));
  netwib_er(netwib_pkt_append_iphdr(&ipheader, &pkt));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_NULL, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_ETHER,
                                     "0000000000000000000000000800"));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_NULL, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_NULL,
                                     "00000800"));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_NULL, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_LOOP,
                                     "00000800"));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_NULL, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_RAW,
                                     "''"));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_NULL, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_PPP,
                                     "00000021"));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_NULL, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_LINUX_SLL,
                                     "00000001000600000000000000000800"));
  netwib_er(netwib_buf_close(&pkt));

  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  linkhdrin.type = NETWIB_DEVICE_DLTTYPE_LOOP;
  linkhdrin.hdr.loop.type = NETWIB_ETHERHDRTYPE_IP4;
  netwib_er(netwib_pkt_append_linkhdr(&linkhdrin, &pkt));
  netwib_er(netwib_iphdr_initdefault(NETWIB_IPTYPE_IP4, &ipheader));
  netwib_er(netwib_pkt_append_iphdr(&ipheader, &pkt));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_LOOP, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_ETHER,
                                     "0000000000000000000000000800"));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_LOOP, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_NULL,
                                     "00000800"));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_LOOP, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_LOOP,
                                     "00000800"));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_LOOP, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_RAW,
                                     "''"));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_LOOP, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_PPP,
                                     "00000021"));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_LOOP, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_LINUX_SLL,
                                     "00000001000600000000000000000800"));
  netwib_er(netwib_buf_close(&pkt));

  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  linkhdrin.type = NETWIB_DEVICE_DLTTYPE_RAW;
  netwib_er(netwib_pkt_append_linkhdr(&linkhdrin, &pkt));
  netwib_er(netwib_iphdr_initdefault(NETWIB_IPTYPE_IP4, &ipheader));
  netwib_er(netwib_pkt_append_iphdr(&ipheader, &pkt));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_RAW, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_ETHER,
                                     "0000000000000000000000000800"));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_RAW, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_NULL,
                                     "00000800"));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_RAW, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_LOOP,
                                     "00000800"));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_RAW, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_RAW,
                                     "''"));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_RAW, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_PPP,
                                     "00000021"));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_RAW, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_LINUX_SLL,
                                     "00000001000600000000000000000800"));
  netwib_er(netwib_buf_close(&pkt));

  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  linkhdrin.type = NETWIB_DEVICE_DLTTYPE_LINUX_SLL;
  linkhdrin.hdr.linuxsll.pkttype = NETWIB_LINUXSLLHDRPKTTYPE_BROADCAST;
  linkhdrin.hdr.linuxsll.hatype = NETWIB_LINUXSLLHDRHATYPE_ETHER;
  linkhdrin.hdr.linuxsll.halen = NETWIB_ETH_LEN;
  linkhdrin.hdr.linuxsll.srcaddr[0] = 0;
  linkhdrin.hdr.linuxsll.srcaddr[1] = 1;
  linkhdrin.hdr.linuxsll.srcaddr[2] = 2;
  linkhdrin.hdr.linuxsll.srcaddr[3] = 3;
  linkhdrin.hdr.linuxsll.srcaddr[4] = 4;
  linkhdrin.hdr.linuxsll.srcaddr[5] = 5;
  linkhdrin.hdr.linuxsll.protocol = NETWIB_ETHERHDRTYPE_IP4;
  netwib_er(netwib_pkt_append_linkhdr(&linkhdrin, &pkt));
  netwib_er(netwib_iphdr_initdefault(NETWIB_IPTYPE_IP4, &ipheader));
  netwib_er(netwib_pkt_append_iphdr(&ipheader, &pkt));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_LINUX_SLL, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_ETHER,
                                     "FFFFFFFFFFFF0001020304050800"));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_LINUX_SLL, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_NULL,
                                     "00000800"));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_LINUX_SLL, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_LOOP,
                                     "00000800"));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_LINUX_SLL, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_RAW,
                                     "''"));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_LINUX_SLL, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_PPP,
                                     "00000021"));
  netwib_er(netwib_pkt_convert_test2(NETWIB_DEVICE_DLTTYPE_LINUX_SLL, &pkt,
                                     NETWIB_DEVICE_DLTTYPE_LINUX_SLL,
                                     "00010001000600010203040500000800"));
  netwib_er(netwib_buf_close(&pkt));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_pkt_conv(void)
{
  puts("###########################################\nTest of pkt/conv");
  netwib_er(netwib_pkt_convert_test());

  return(NETWIB_ERR_OK);
}
