/*
                                  NETWOX
                             Network toolbox
                Copyright(c) 1999-2012 Laurent CONSTANTIN
                      http://ntwox.sourceforge.net/
                        laurentconstantin@free.fr
                                  -----

  This file is part of Netwox.

  Netwox is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3
  as published by the Free Software Foundation.

  Netwox is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details (http://www.gnu.org/).

------------------------------------------------------------------------
*/

/*-------------------------------------------------------------*/
#include "../../netwox.h"

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_snmpmsg(netwox_constsnmpmsg *psnmpmsg,
                                     netwib_buf *ppkt)
{
  netwox_asn1data asn1data;
  netwib_err ret;

  netwib_er(netwox_asn1data_init(psnmpmsg->pbufpool, &asn1data));
  ret = netwox_asn1data_append_snmpmsg(psnmpmsg, &asn1data);
  if (ret == NETWIB_ERR_OK) {
    ret = netwox_pktber_append_asn1data(&asn1data, ppkt);
  }
  netwib_er(netwox_asn1data_close(&asn1data));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_decode_snmpmsg(netwib_buf *ppkt,
                                     netwox_snmpmsg *psnmpmsg)
{
  netwox_asn1data asn1data;
  netwib_err ret;

  netwib_er(netwox_asn1data_init(psnmpmsg->pbufpool, &asn1data));
  ret = netwox_pktber_decode_asn1data_snmp(ppkt, &asn1data);
  if (ret == NETWIB_ERR_OK) {
    ret = netwox_asn1data_decode_snmpmsg(&asn1data, psnmpmsg);
  }
  netwib_er(netwox_asn1data_close(&asn1data));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_snmp_show(netwib_bufpool *pbufpool,
                                netwib_constbuf *ppkt,
                                netwib_encodetype_context *pctx,
                                netwib_encodetype snmpencodetype,
                                netwib_encodetype dataencodetype,
                                netwib_buf *pbuf)
{
  netwox_snmpmsg snmpmsg;
  netwib_encodetype_context *pctxtouse, ctx;
  netwib_buf pkt;
  netwib_err ret;

  pctxtouse = pctx;
  if (pctx == NULL) {
    netwib_er(netwib_buf_encode_transition_init(&ctx));
    pctxtouse = &ctx;
  }

  switch(snmpencodetype) {
  case NETWIB_ENCODETYPE_SYNTH :
  case NETWIB_ENCODETYPE_ARRAY :
    break;
  default :
    netwib_er(netwib_pkt_data_show(ppkt, pctxtouse, snmpencodetype, pbuf));
    return(NETWIB_ERR_OK);
    break;
  }

  pkt = *ppkt;
  netwib_er(netwox_snmpmsg_init(pbufpool, &snmpmsg));
  ret = netwox_pkt_decode_snmpmsg(&pkt, &snmpmsg);
  if (ret == NETWIB_ERR_NOTCONVERTED || ret == NETWOX_ERR_NOTSUPPORTED
      || ret == NETWIB_ERR_DATAMISSING) {
    netwib_er(netwib_pkt_data_show(ppkt, pctxtouse, dataencodetype, pbuf));
    netwib_er(netwox_snmpmsg_close(&snmpmsg));
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_snmpmsg_close(&snmpmsg));
    return(ret);
  }

  netwib_er(netwib_buf_encode_transition(pctxtouse, snmpencodetype, pbuf));
  ret = netwox_snmpmsg_show(&snmpmsg, snmpencodetype, pbuf);

  if (netwib__buf_ref_data_size(&pkt)) {
    netwib_er(netwib_pkt_data_show(&pkt, pctxtouse, dataencodetype, pbuf));
  }

  netwib_er(netwox_snmpmsg_close(&snmpmsg));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_snmp_display(netwib_bufpool *pbufpool,
                                   netwib_constbuf *ppkt,
                                   netwib_encodetype_context *pctx,
                                   netwib_encodetype snmpencodetype,
                                   netwib_encodetype dataencodetype)
{
  netwib_buf *pbuf;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
  ret = netwox_pkt_snmp_show(pbufpool, ppkt, pctx, snmpencodetype,
                             dataencodetype, pbuf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_display(pbuf, NETWIB_ENCODETYPE_DATA));
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmppkt_test(netwib_constbuf *ppkt)
{
  netwib_bufpool *pbufpool;
  netwox_snmpmsg snmpmsg;
  netwib_buf *pbuf, pkt;
  netwib_cmp cmp;
  netwib_err ret;

  netwib_er(netwib_bufpool_initdefault(&pbufpool));

  pkt = *ppkt;
  netwib_er(netwox_pkt_snmp_display(pbufpool, &pkt, NULL,
                                    NETWIB_ENCODETYPE_ARRAY,
                                    NETWIB_ENCODETYPE_DUMP));

  pkt = *ppkt;
  netwib_er(netwox_snmpmsg_init(pbufpool, &snmpmsg));
  ret = netwox_pkt_decode_snmpmsg(&pkt, &snmpmsg);
  if (ret == NETWIB_ERR_NOTCONVERTED || ret == NETWOX_ERR_NOTSUPPORTED
      || ret == NETWIB_ERR_DATAMISSING) {
    netwib_er(netwib_fmt_display("SNMP not decoded/supported\n"));
    netwib_er(netwox_snmpmsg_close(&snmpmsg));
    netwib_er(netwib_bufpool_close(&pbufpool));
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_buf_display(ppkt, NETWIB_ENCODETYPE_DUMP));
    return(ret);
  }

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
  netwib_er(netwox_snmpmsg_show(&snmpmsg, NETWIB_ENCODETYPE_ARRAY, pbuf));
  netwib_er(netwib_buf_display(pbuf, NETWIB_ENCODETYPE_DATA));

  netwib__buf_reinit(pbuf);
  netwib_er(netwox_pkt_append_snmpmsg(&snmpmsg, pbuf));
  netwib_er(netwib_buf_cmp(pbuf, ppkt, &cmp));
  ret = NETWIB_ERR_OK;
  if (cmp != NETWIB_CMP_EQ) {
    netwib_er(netwib_fmt_display("Mismatch :\n"));
    netwib_er(netwib_buf_display(pbuf, NETWIB_ENCODETYPE_DUMP));
    netwib_er(netwib_fmt_display("Wanted :\n"));
    netwib_er(netwib_buf_display(ppkt, NETWIB_ENCODETYPE_DUMP));
    /*ret = 12345;*/
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));
  netwib_er(netwox_snmpmsg_close(&snmpmsg));
  netwib_er(netwib_bufpool_close(&pbufpool));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_snmppkt_test_static2(netwib_conststring pkt)
{
  netwib_buf buf1, buf2;

  netwib_er(netwib_buf_init_mallocdefault(&buf1));

  netwib_er(netwib_buf_init_ext_string(pkt, &buf2));
  netwib_er(netwib_buf_decode(&buf2, NETWIB_DECODETYPE_MIXED, &buf1));

  netwib_er(netwox_snmppkt_test(&buf1));

  netwib_er(netwib_buf_close(&buf1));

  return(NETWIB_ERR_OK);
}
netwib_err netwox_snmppkt_test_static(void)
{

  netwib_er(netwox_snmppkt_test_static2("302902010004067075626c6963a01c0204158347c9020100020100300e300c06082b060102010104000500"));

  netwib_er(netwox_snmppkt_test_static2("30290203410094067075626c6963a01c0204158347c9020100020100300e300c06082b060102010104000500"));

  netwib_er(netwox_snmppkt_test_static2("302b02010104067075626c6963a21e02047d1998d40201000201003010300e060a2b0601020101090104098200"));

  netwib_er(netwox_snmppkt_test_static2("303e020103301102046e02d415020300ffe30401040201030410300e0400020100020100040004000400301404000400a00e020420243efc0201000201003000"));
  netwib_er(netwox_snmppkt_test_static2("306a020103301102046e02d415020300ffe3040100020103041e301c040d800007e58040004311c8b55540020103020200cb0400040004003032040d800007e58040004311c8b555400400a81f020420243efc0201000201003011300f060a2b060106030f01010400410107"));
  netwib_er(netwox_snmppkt_test_static2("30818f020103301102046e02d416020300ffe3040107020103043d303b040d800007e58040004311c8b55540020103020200cb040b6c617572656e74726f6f6f040c3a8963f2b75f19e99a7bdf68040800000001eac315d70438715cd920810212832fe6392c7bd565307615fb0cbf6614818e6633f4c5e5d6057a0293f78f8041183af33626490dac44fa9b0ef99bb53ae1"));
  netwib_er(netwox_snmppkt_test_static2("30818f020103301102046e02d416020300ffe3040103020103043d303b040d800007e58040004311c8b55540020103020200cb040b6c617572656e74726f6f6f040cfd8661c04e987013b9ac9fc10408000000035af7ae890438dd07bfb77573d1522f8fb9eba2cfaea6d65c545a4909b8a889e66496f509fb68bd6435f7a2ef5e6c1bd1bfe900199d3d47c3972292a73538"));

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_snmppkt_test_samples2(netwib_bufpool *pbufpool,
                                               netwib_constbuf *ppathfilename)
{
  netwib_io *pio;
  netwib_buf pkt;
  netwib_err ret;

  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_io_init_file_read(ppathfilename, &pio));
  while (NETWIB_TRUE) {
    ret = netwib_io_read(pio, &pkt);
    if (ret == NETWIB_ERR_DATAEND) {
      break;
    }
  }
  /*netwib_er(netwib_buf_display(&pkt, NETWIB_ENCODETYPE_DUMP));*/
  /* netwib_er(netwox_pkt_snmp_display(pbufpool, &pkt, NULL,
                                    NETWIB_ENCODETYPE_ARRAY,
                                    NETWIB_ENCODETYPE_DUMP));*/
  netwib_er(netwox_snmppkt_test(&pkt));

  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_buf_close(&pkt));

  pbufpool = pbufpool; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
netwib_err netwox_snmppkt_test_samples(void)
{
  netwib_dir *pdir;
  netwib_bufpool *pbufpool;
  netwib_buf dirnamebuf, filename, pathfilename;
  netwib_uint32 i;
  netwib_err ret;

  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  if (0) {
    netwib_er(netwib_buf_init_ext_string("/f", &dirnamebuf));
    netwib_er(netwox_snmppkt_test_samples2(pbufpool, &dirnamebuf));
    netwib_er(netwib_bufpool_close(&pbufpool));
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_buf_init_ext_string("/d", &dirnamebuf));
  netwib_er(netwib_dir_init(&dirnamebuf, &pdir));
  netwib_er(netwib_buf_init_mallocdefault(&filename));
  netwib_er(netwib_buf_init_mallocdefault(&pathfilename));
  i = 0;
  while (NETWIB_TRUE) {
    netwib__buf_reinit(&filename);
    ret = netwib_dir_next(pdir, &filename);
    if (ret == NETWIB_ERR_DATAEND) {
      break;
    }
    i++;
    if (i <= 0 || i > 400000) {
      continue;
    }
    netwib_er(netwib_fmt_display("%{buf}\n", &filename));
    netwib__buf_reinit(&pathfilename);
    netwib_er(netwib_path_init_concat(&dirnamebuf, &filename, &pathfilename));
    netwib_er(netwox_snmppkt_test_samples2(pbufpool, &pathfilename));
  }

  netwib_er(netwib_buf_close(&pathfilename));
  netwib_er(netwib_buf_close(&filename));
  netwib_er(netwib_dir_close(&pdir));
  netwib_er(netwib_bufpool_close(&pbufpool));

  return(NETWIB_ERR_OK);
}
