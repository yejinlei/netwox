#include "netwibtest.h"

/*-------------------------------------------------------------*/
netwib_err netwibtest_data_print_string(netwib_constdata data,
                                        netwib_uint32 datasize)
{
  netwib_uint32 i;

  for (i = 0; i < datasize ; i++) {
    printf("%c", data[i]);
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#define netwibtest_priv_isprinthexa(c) (c<32 || c>126 || c=='\\' || c=='"')
netwib_err netwibtest_data_print_mixed(netwib_constdata data,
                                       netwib_uint32 datasize)
{
  netwib_uint32 i;
  netwib_byte c;
  netwib_bool modehexa;

  modehexa = NETWIB_TRUE;
  for (i = 0; i < datasize ; i++) {
    c = data[i];
    if (netwibtest_priv_isprinthexa(c)) {
      if (!modehexa) {
        printf("' ");
        modehexa = NETWIB_TRUE;
      }
      printf("%02X", c);
    } else {
      if (modehexa) {
        printf(" '");
        modehexa = NETWIB_FALSE;
      }
      if (c == '\'') {
        printf("''");
      } else {
        printf("%c", c);
      }
    }
  }
  if (!modehexa) {
    printf("'");
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_data_print_hexa(netwib_constdata data,
                                      netwib_uint32 datasize)
{
  netwib_uint32 i;

  for (i = 0; i < datasize ; i++) {
    printf("%02X", data[i]);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_buf_content(netwib_constbuf *pbuf,
                                  netwib_conststring wanted,
                                  netwib_uint32 totalsize,
                                  netwib_uint32 beginoffset,
                                  netwib_uint32 endoffset)
{
  netwib_uint32 datasize;
  netwib_bool founderror;

#if 0
  if (wanted != NULL) {
    printf("::%s\n", wanted);
  }
#endif

  founderror = NETWIB_FALSE;

  if (pbuf->totalsize != totalsize) {
    printf("pbuf->totalsize(%d) != totalsize(%d)\n",
           pbuf->totalsize, totalsize);
    founderror = NETWIB_TRUE;
  }
  if (pbuf->beginoffset != beginoffset) {
    printf("pbuf->beginoffset(%d) != beginoffset(%d)\n",
           pbuf->beginoffset, beginoffset);
    founderror = NETWIB_TRUE;
  }
  if (pbuf->endoffset != endoffset) {
    printf("pbuf->endoffset(%d) != endoffset(%d)\n",
           pbuf->endoffset, endoffset);
    founderror = NETWIB_TRUE;
  }

  if (pbuf->totalptr == NULL) {
    if (wanted != NULL) {
      printf("pbuf->totalptr == NULL, but wanted=%s\n", wanted);
      founderror = NETWIB_TRUE;
    }
  } else {
    if (pbuf->endoffset >= pbuf->beginoffset) {
      datasize = pbuf->endoffset - pbuf->beginoffset;
    } else {
      datasize = 0;
    }
    if (wanted == NULL) {
      printf("wanted == NULL, but pbuf->data=");
      netwib_er(netwibtest_data_print_string(pbuf->totalptr+pbuf->beginoffset,
                                             datasize));
      printf("\n");
      founderror = NETWIB_TRUE;
    } else {
      if (netwib_c_strncmp((netwib_string)pbuf->totalptr+pbuf->beginoffset,
                           wanted,
                           netwib_c_strlen(wanted))) {
        printf("pbuf->data=");
        netwib_er(netwibtest_data_print_string(pbuf->totalptr+pbuf->beginoffset,
                                               datasize));
        printf(" != wanted=%s\n", wanted);
        founderror = NETWIB_TRUE;
      }
      if (pbuf->endoffset-pbuf->beginoffset != netwib_c_strlen(wanted)) {
        printf("pbuf->endoffset-pbuf->beginoffset != strlen(wanted) :\n");
        printf("%d %d != strlen(%s)=%d\n", pbuf->endoffset, pbuf->beginoffset,
               wanted, (int)netwib_c_strlen(wanted));
        founderror = NETWIB_TRUE;
      }
    }
  }

  if (founderror) {
    return(NETWIBTEST_ERR_0);
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_buf_content2(netwib_constbuf *pbuf,
                                   netwib_conststring wantedstring)
{
  netwib_uint32 strlenws, datasize;

  strlenws = netwib_c_strlen(wantedstring);
  if (pbuf->endoffset >= pbuf->beginoffset) {
    datasize = pbuf->endoffset - pbuf->beginoffset;
  } else {
    datasize = 0;
  }

  if (netwib_c_strncmp((netwib_string)pbuf->totalptr+pbuf->beginoffset,
                       wantedstring,
                       strlenws)) {
    printf("pbuf->data=");
    netwib_er(netwibtest_data_print_string(pbuf->totalptr+pbuf->beginoffset,
                                           datasize));
    printf(" != wantedstring=%s\n", wantedstring);
    return(NETWIBTEST_ERR_1);
  }

  if (datasize != strlenws) {
    printf("datasize(%d) != strlen(wantedstring)(%d)\n", datasize, strlenws);
    printf("pbuf->data=");
    netwib_er(netwibtest_data_print_string(pbuf->totalptr+pbuf->beginoffset,
                                           datasize));
    printf(" != wantedstring=%s\n", wantedstring);
    return(NETWIBTEST_ERR_0);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_buf_content3(netwib_constbuf *pbuf,
                                   netwib_conststring wantedmixed)
{
  netwib_buf wantedbuf, wantedmixedbuf;
  netwib_data storeddata, wanteddata;
  netwib_uint32 storeddatasize, wanteddatasize;

  /* stored data */
  storeddata = netwib__buf_ref_data_ptr(pbuf);
  storeddatasize = netwib__buf_ref_data_size(pbuf);

  /* wanteddata */
  netwib_er(netwib_buf_init_mallocdefault(&wantedbuf));
  netwib_er(netwib_buf_init_ext_string(wantedmixed, &wantedmixedbuf));
  netwib_er(netwib_buf_decode(&wantedmixedbuf, NETWIB_DECODETYPE_MIXED,
                              &wantedbuf));
  wanteddata = netwib__buf_ref_data_ptr(&wantedbuf);
  wanteddatasize = netwib__buf_ref_data_size(&wantedbuf);

  if (netwib_c_memcmp(storeddata, wanteddata, wanteddatasize)) {
    printf("pbuf->data=");
    netwib_er(netwibtest_data_print_mixed(storeddata, storeddatasize));
    printf("\npbuf->data=");
    netwib_er(netwibtest_data_print_hexa(storeddata, storeddatasize));
    printf("\nwanted     =%s\n", wantedmixed);
    netwib_er(netwib_buf_close(&wantedbuf));
    return(NETWIBTEST_ERR_1);
  }

  if (storeddatasize != wanteddatasize) {
    printf("storeddatasize(%d) != wanteddatasize(%d)\n", storeddatasize,
           wanteddatasize);
    printf("pbuf->data=");
    netwib_er(netwibtest_data_print_mixed(storeddata, storeddatasize));
    printf("\npbuf->data=");
    netwib_er(netwibtest_data_print_hexa(storeddata, storeddatasize));
    printf("\nwanted     =%s\n", wantedmixed);
    netwib_er(netwib_buf_close(&wantedbuf));
    return(NETWIBTEST_ERR_0);
  }

  netwib_er(netwib_buf_close(&wantedbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_buf_content4(netwib_constbuf *pbuf,
                                   netwib_uint16 wantedchecksum)
{
  netwib_uint16 sum;

  netwib_er(netwib_checksum_buf(pbuf, &sum));
  if (sum != wantedchecksum) {
    netwib_er(netwib_fmt_display("sum=%{uint32:04X} != wanted=%{uint32:04X}\n",
                                 sum, wantedchecksum));
    return(NETWIBTEST_ERR_7);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_ip_content(netwib_constip *pip,
                                 netwib_conststring wantedstring)
{
  netwib_buf buf;
  netwib_ip wantedip;
  netwib_cmp cmp;

  netwib_er(netwib_buf_init_ext_string(wantedstring, &buf));
  netwib_er(netwib_ip_init_buf_best(&buf, &wantedip));

  netwib_er(netwib_ip_cmp(&wantedip, pip, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    netwib_er(netwib_fmt_display("We wanted %s(%{ip}) != %{ip}\n",
                                 wantedstring, &wantedip, pip));
    return(NETWIBTEST_ERR_7);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_eth_content(netwib_consteth *peth,
                                  netwib_conststring wantedstring)
{
  netwib_buf buf;
  netwib_eth wantedeth;
  netwib_cmp cmp;

  netwib_er(netwib_buf_init_ext_string(wantedstring, &buf));
  netwib_er(netwib_eth_init_buf(&buf, &wantedeth));

  netwib_er(netwib_eth_cmp(&wantedeth, peth, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    netwib_er(netwib_fmt_display("Eth=%{uint32:02X}:%{uint32:02X}:%{uint32:02X}:%{uint32:02X}:%{uint32:02X}:%{uint32:02X} != ", peth->b[0], peth->b[1], peth->b[2], peth->b[3], peth->b[4], peth->b[5]));
    netwib_er(netwib_fmt_display("wantedEth=%{uint32:02X}:%{uint32:02X}:%{uint32:02X}:%{uint32:02X}:%{uint32:02X}:%{uint32:02X}\n", wantedeth.b[0], wantedeth.b[1], wantedeth.b[2], wantedeth.b[3], wantedeth.b[4], wantedeth.b[5]));
    return(NETWIBTEST_ERR_7);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_eths_content(netwib_eths *peths,
                                   netwib_conststring wantedstring)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_eths(peths, &buf));
  netwib_er(netwibtest_buf_content2(&buf, wantedstring));
  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_ips_content(netwib_ips *pips,
                                  netwib_conststring wantedstring)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_ips(pips, NETWIB_IPS_ENCODETYPE_BEST, &buf));
  netwib_er(netwibtest_buf_content2(&buf, wantedstring));
  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_ports_content(netwib_ports *pports,
                                    netwib_conststring wantedstring)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_ports(pports, &buf));
  netwib_er(netwibtest_buf_content2(&buf, wantedstring));
  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_linkhdr_content(netwib_constlinkhdr *plinkhdr,
                                      netwib_conststring wantedmixed)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_linkhdr_show(plinkhdr, NETWIB_ENCODETYPE_ARRAY, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_linkhdr(plinkhdr, &buf));
  netwib_er(netwibtest_buf_content3(&buf, wantedmixed));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_iphdr_content(netwib_constiphdr *piphdr,
                                    netwib_conststring wantedmixed)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_iphdr_show(piphdr, NETWIB_ENCODETYPE_ARRAY, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_iphdr(piphdr, &buf));
  netwib_er(netwibtest_buf_content3(&buf, wantedmixed));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_udphdr_content(netwib_constudphdr *pudphdr,
                                     netwib_conststring wantedmixed)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_udphdr_show(pudphdr, NETWIB_ENCODETYPE_ARRAY, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_udphdr(pudphdr, &buf));
  netwib_er(netwibtest_buf_content3(&buf, wantedmixed));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_tcphdr_content(netwib_consttcphdr *ptcphdr,
                                     netwib_conststring wantedmixed)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_tcphdr_show(ptcphdr, NETWIB_ENCODETYPE_ARRAY, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_tcphdr(ptcphdr, &buf));
  netwib_er(netwibtest_buf_content3(&buf, wantedmixed));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_arphdr_content(netwib_constarphdr *parphdr,
                                     netwib_conststring wantedmixed)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_arphdr_show(parphdr, NETWIB_ENCODETYPE_ARRAY, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_arphdr(parphdr, &buf));
  netwib_er(netwibtest_buf_content3(&buf, wantedmixed));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_ip4opt_content(netwib_constip4opt *pip4opt,
                                     netwib_conststring wantedmixed)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_ip4opt_show(pip4opt, NETWIB_ENCODETYPE_ARRAY, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_ip4opt(pip4opt, &buf));
  netwib_er(netwibtest_buf_content3(&buf, wantedmixed));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_ip6ext_content(netwib_constip6ext *pip6ext,
                                     netwib_conststring wantedmixed)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_ip6ext_show(pip6ext, NETWIB_ENCODETYPE_ARRAY, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_ip6ext(pip6ext, &buf));
  netwib_er(netwibtest_buf_content3(&buf, wantedmixed));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_tcpopt_content(netwib_consttcpopt *ptcpopt,
                                     netwib_conststring wantedmixed)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_tcpopt_show(ptcpopt, NETWIB_ENCODETYPE_ARRAY, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_tcpopt(ptcpopt, &buf));
  netwib_er(netwibtest_buf_content3(&buf, wantedmixed));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_icmp4_content(netwib_consticmp4 *picmp4,
                                    netwib_conststring wantedmixed)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_icmp4_show(picmp4, NETWIB_ENCODETYPE_ARRAY, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_icmp4(picmp4, &buf));
  netwib_er(netwibtest_buf_content3(&buf, wantedmixed));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_icmp6_content(netwib_consticmp6 *picmp6,
                                    netwib_conststring wantedmixed)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_icmp6_show(picmp6, NETWIB_ENCODETYPE_ARRAY, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_icmp6(picmp6, &buf));
  netwib_er(netwibtest_buf_content3(&buf, wantedmixed));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_icmp6nd_content(netwib_consticmp6nd *picmp6nd,
                                      netwib_conststring wantedmixed)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_icmp6nd_show(picmp6nd, NETWIB_ENCODETYPE_ARRAY, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_pkt_append_icmp6nd(picmp6nd, &buf));
  netwib_er(netwibtest_buf_content3(&buf, wantedmixed));

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_uint64_content(netwib_uint64 ui,
                                     netwib_conststring wanteddecimal)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

#if 0
  {
    netwib_data data;
    netwib_uint32 datasize;
    netwib_er(netwib_buf_wantspace(&buf, 80, &data));
 #if NETWIB_INT64_FAKE == 1
    datasize = snp____rintf
      (data, 79, "%08X%08X", ui.high, ui.low);
 #else
    datasize = snp____rintf
      (data, 79, "%16llX", ui);
 #endif
    buf.endoffset += datasize;
  }
#else
 #if NETWIB_INT64_FAKE == 1
  netwib_er(netwib_buf_append_fmt(&buf, "%{uint32:8X}%{uint32:8X}",
                                  ui.high, ui.low));
 #else
  netwib_er(netwib_buf_append_fmt(&buf, "%{uint64:16X}", ui));
 #endif
#endif

  netwib_er(netwibtest_buf_content2(&buf, wanteddecimal));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_int64_content(netwib_int64 i,
                                    netwib_conststring wanteddecimal)
{
  return(netwibtest_uint64_content((netwib_uint64)i, wanteddecimal));
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_linkhdr_init(netwib_linkhdr *plinkhdr)
{
  plinkhdr->type = NETWIB_DEVICE_DLTTYPE_EN10MB;
  netwib_er(netwib_eth_init_fields(0x11,0x22,0x33,0x44,0x55,0x66,
                                   &plinkhdr->hdr.ether.src));
  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &plinkhdr->hdr.ether.dst));
  plinkhdr->hdr.ether.type = NETWIB_ETHERHDRTYPE_IP4;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_ip4hdr_init(netwib_iphdr *piphdr)
{
  netwib_er(netwib_iphdr_initdefault(NETWIB_IPTYPE_IP4, piphdr));
  piphdr->header.ip4.id = 0xfade;
  piphdr->header.ip4.tos = 0x12;
  piphdr->protocol = NETWIB_IPPROTO_TCP;
  piphdr->header.ip4.check = 0x1234;
  netwib_er(netwib_ip_init_ip4(0x11223344, &piphdr->src));
  netwib_er(netwib_ip_init_ip4(0x55667788, &piphdr->dst));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_ip6hdr_init(netwib_iphdr *piphdr)
{
  netwib_er(netwib_iphdr_initdefault(NETWIB_IPTYPE_IP6, piphdr));
  piphdr->header.ip6.trafficclass = 129;
  piphdr->header.ip6.flowlabel = 1234;
  piphdr->header.ip6.payloadlength = 34;
  piphdr->protocol = NETWIB_IPPROTO_TCP;
  piphdr->ttl = 65;
  netwib_er(netwib_ip_init_ip6_fields(0x11223344,5,6,7, &piphdr->src));
  netwib_er(netwib_ip_init_ip6_fields(0x55667788,8,9,0, &piphdr->dst));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_udphdr_init(netwib_udphdr *pudphdr)
{
  netwib_er(netwib_udphdr_initdefault(pudphdr));
  pudphdr->src = 1;
  pudphdr->dst = 2;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_tcphdr_init(netwib_tcphdr *ptcphdr)
{
  netwib_er(netwib_tcphdr_initdefault(ptcphdr));
  ptcphdr->src = 1;
  ptcphdr->dst = 2;
  ptcphdr->seqnum = 0x11223344;
  ptcphdr->reserved1 = NETWIB_TRUE;
  ptcphdr->reserved3 = NETWIB_TRUE;
  ptcphdr->ece = NETWIB_TRUE;
  ptcphdr->ack = NETWIB_TRUE;
  ptcphdr->rst = NETWIB_TRUE;
  ptcphdr->fin = NETWIB_TRUE;
  ptcphdr->window = 0x3434;
  ptcphdr->urgptr = 0x1212;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_arphdr_init(netwib_arphdr *parphdr)
{
  netwib_er(netwib_arphdr_initdefault(parphdr));
  parphdr->op = NETWIB_ARPHDROP_ARPREQ;
  netwib_er(netwib_eth_init_fields(1,2,3,4,5,6, &parphdr->ethsrc));
  netwib_er(netwib_ip_init_ip4(0x11223344, &parphdr->ipsrc));
  netwib_er(netwib_ip_init_ip4(0x55667788, &parphdr->ipdst));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_ip6exts_init(netwib_ipproto lastproto,
                                   netwib_buf *ppkt)
{
  netwib_ip6ext ip6ext;

  ip6ext.proto = NETWIB_IPPROTO_HOPOPTS;
  netwib_er(netwib_buf_init_ext_string("hello", &ip6ext.ext.hopopts.options));
  ip6ext.nextproto = NETWIB_IPPROTO_DSTOPTS;
  netwib_er(netwib_pkt_append_ip6ext(&ip6ext, ppkt));

  ip6ext.proto = NETWIB_IPPROTO_DSTOPTS;
  netwib_er(netwib_buf_init_ext_string("olleh", &ip6ext.ext.hopopts.options));
  ip6ext.nextproto = lastproto;
  netwib_er(netwib_pkt_append_ip6ext(&ip6ext, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_icmp4_init(netwib_icmp4 *picmp4)
{
  netwib_er(netwib_icmp4_initdefault(NETWIB_ICMP4TYPE_ECHOREQ, picmp4));
  picmp4->msg.echo.id = 1;
  picmp4->msg.echo.seqnum = 2;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_icmp6_init(netwib_icmp6 *picmp6)
{
  netwib_er(netwib_icmp6_initdefault(NETWIB_ICMP6TYPE_ECHOREQ, picmp6));
  picmp6->msg.echo.id = 1;
  picmp6->msg.echo.seqnum = 2;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_uint64_init(netwib_conststring hexavalue,
                                  netwib_uint64 *pui)
{
  netwib_data data;
  netwib_int32 datasize;
  netwib_uint32 high, low;
  netwib_byte array[8];
  netwib_buf buf, bufhex;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string(hexavalue, &bufhex));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  ret = netwib_buf_decode(&bufhex, NETWIB_DECODETYPE_HEXA, &buf);
  if (ret == NETWIB_ERR_OK) {
    datasize = netwib__buf_ref_data_size(&buf);
    if (datasize > 8) {
      ret = NETWIB_ERR_PATOOHIGH;
    } else {
      data = netwib__buf_ref_data_ptr(&buf);
      netwib_c_memset(array, 0, 8);
      netwib_c_memcpy(array+8-datasize, data, datasize);
      high = netwib_c2_uint32_4(array[0], array[1], array[2], array[3]);
      low = netwib_c2_uint32_4(array[4], array[5], array[6], array[7]);
      if (pui != NULL) {
        netwib__uint64_init_32(high, low, *pui);
      }
    }
  }
  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_int64_init(netwib_conststring hexavalue,
                                 netwib_int64 *pi)
{
  return(netwibtest_uint64_init(hexavalue, (netwib_uint64*)pi));
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_something_to_do(netwib_conststring msg)
{
  printf("==================== YOU HAVE SOMETHING TO DO ==================\n");
  netwib_er(netwib_kbd_purge());
  netwib_er(netwib_beep());
  printf("%s\n", msg);
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwibtest_choose_device(netwib_buf *pbuf)
{
  netwib_conf_devices conf;
  netwib_conf_devices_index *pconfindex;
  netwib_err ret;

  netwib_er(netwib_conf_devices_index_init(&conf, &pconfindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_conf_devices_index_next(pconfindex);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_NOTFOUND;
      break;
    }
    if (conf.hwtype == NETWIB_DEVICE_HWTYPE_ETHER) {
      netwib_er(netwib_buf_append_buf(&conf.device, pbuf));
      break;
    }
  }
  netwib_er(netwib_conf_devices_index_close(&pconfindex));
  if (ret == NETWIB_ERR_NOTFOUND) {
    netwib_er(netwib_fmt_display("No Ethernet device found\n"));
  }

  return(ret);
}
