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
#define NETWOX_SMB_CAPTURE_FILE "file.pcap"

/*-------------------------------------------------------------*/
#define NETWOX_SMB_TEST_SNIFFEDPKT      0
#define NETWOX_SMB_TEST_SNIFFEDPKTNUM 1
#define NETWOX_SMB_TEST_SMBNBTSS        0
#define NETWOX_SMB_TEST_FLOW          1
#define NETWOX_SMB_TEST_SMBSIZE         0
#define NETWOX_SMB_TEST_SMBMSG        1
#define NETWOX_SMB_TEST_SMBCMD        1
#define NETWOX_SMB_TEST_SMBCMDTSC     1
#define NETWOX_SMB_TEST_SMBINIT       1

#define NETWOX_SMB_TEST_REDUNDANTDISPLAY 0

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_smb_test_flow(netwib_bufpool *pbufpool,
                                            netwib_constbuf *ppkt,
                                            netwib_buf *psmb,
                                            netwib_ip *plastip,
                                            netwib_port *plastport)
{
  netwib_linkhdr linkheader;
  netwib_iphdr ipheader;
  netwib_tcphdr tcpheader;
  netwib_buf tcpdata;
  netwib_cmp cmp;
  netwib_err ret;

#if NETWOX_SMB_TEST_SNIFFEDPKT == 1
  netwib_er(netwox_pkt_link_display(pbufpool, NETWIB_DEVICE_DLTTYPE_ETHER,
                                    ppkt, NULL, NETWIB_ENCODETYPE_ARRAY,
                                    NETWIB_ENCODETYPE_DUMP));
#endif

  ret = netwib_pkt_decode_linkiptcpdata(NETWIB_DEVICE_DLTTYPE_ETHER, ppkt,
                                        &linkheader, &ipheader,
                                        &tcpheader, &tcpdata);
  if (ret != NETWIB_ERR_OK) {
    /* a non TCP packet was sniffed */
    return(NETWIB_ERR_OK);
  }

  if (tcpheader.src == NETWOX_SERVICES_SMBNBTSS ||
      tcpheader.dst == NETWOX_SERVICES_SMBNBTSS) {
    if (*plastport == NETWOX_SERVICES_SMBNAKED) {
      if (netwib__buf_ref_data_size(psmb)) {
        netwib_er(netwib_fmt_display("Data from other port is present (%{uint32}).\n", netwib__buf_ref_data_size(psmb)));
        return(NETWOX_ERR_INTERNALERROR);
      }
    }
    *plastport = NETWOX_SERVICES_SMBNBTSS;
  } else if (tcpheader.src == NETWOX_SERVICES_SMBNAKED ||
             tcpheader.dst == NETWOX_SERVICES_SMBNAKED) {
    if (*plastport == NETWOX_SERVICES_SMBNBTSS) {
      if (netwib__buf_ref_data_size(psmb)) {
        netwib_er(netwib_fmt_display("Data from other port is present (%{uint32}).\n", netwib__buf_ref_data_size(psmb)));
        return(NETWOX_ERR_INTERNALERROR);
      }
    }
    *plastport = NETWOX_SERVICES_SMBNAKED;
  } else {
    /* not SMB */
    return(NETWIB_ERR_OK);
  }
  /* check source ip */
  netwib_er(netwib_ip_cmp(&ipheader.src, plastip, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    if (netwib__buf_ref_data_size(psmb) &&
        netwib__buf_ref_data_size(&tcpdata)) {
      netwib_er(netwib_fmt_display("A packet from other side was received during continuation data.\n"));
      return(NETWOX_ERR_INTERNALERROR);
    }
    if (netwib__buf_ref_data_size(&tcpdata)) {
      /* only change on real data (because can be an ACK) */
      *plastip = ipheader.src;
    }
  }
  /* append */
  netwib_er(netwib_buf_append_buf(&tcpdata, psmb));
  pbufpool = pbufpool; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#if NETWOX_SMB_TEST_SMBCMDTSC == 1
static netwib_err netwox_priv_smb_test_smbcmdsearchq(netwox_constsmbcmd *psmbcmd)
{
  netwox_smbcmdsearch_resumekey resumekey;
  netwox_smbcmd smbcmd;
  netwib_buf buf1, buf2;
  netwib_cmp cmp;

  if (netwib__buf_ref_data_size(&psmbcmd->value.search_q.resumekey) == 0) {
    netwib_er(netwib_fmt_display("There is no resume key to decode.\n"));
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));

  netwib_er(netwox_smbcmdsearch_resumekey_init(&resumekey));
  netwib_er(netwox_smbcmdsearch_resumekey_encode_smbcmd(psmbcmd, &resumekey));
  netwib_er(netwox_smbcmdsearch_resumekey_show(&resumekey, &buf1));
  netwib_er(netwib_buf_display(&buf1, NETWIB_ENCODETYPE_DATA));

  netwib_er(netwox_smbcmd_init(&smbcmd));
  netwib_er(netwox_smbcmd_selecttype(&smbcmd, NETWOX_SMBCMD_TYPE_SEARCH_Q));
  netwib_er(netwox_smbcmd_setdefault(&smbcmd));
  netwib_er(netwox_smbcmdsearch_resumekey_decode_smbcmd(&resumekey, &smbcmd));
  netwib_er(netwox_smbcmdsearch_resumekey_close(&resumekey));
  netwib_er(netwox_smbcmdsearch_resumekey_init(&resumekey));
  netwib_er(netwox_smbcmdsearch_resumekey_encode_smbcmd(&smbcmd, &resumekey));
  netwib_er(netwox_smbcmdsearch_resumekey_show(&resumekey, &buf2));
  netwib_er(netwox_smbcmd_close(&smbcmd));
 #if NETWOX_SMB_TEST_REDUNDANTDISPLAY == 1
  netwib_er(netwib_buf_display(&buf2, NETWIB_ENCODETYPE_DATA));
 #else
  netwib_er(netwib_fmt_display("No REDUNDANTDISPLAY\n"));
 #endif
  netwib_er(netwib_buf_cmp(&buf1, &buf2, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  netwib_er(netwib_buf_close(&buf2));
  netwib_er(netwib_buf_close(&buf1));

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_priv_smb_test_smbcmdsearchr(netwox_constsmbcmd *psmbcmd)
{
  netwox_smbcmdsearch_entries entries;
  netwox_smbcmd smbcmd;
  netwib_buf buf1, buf2;
  netwib_cmp cmp;

  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));

  netwib_er(netwox_smbcmdsearch_entries_init(&entries));
  netwib_er(netwox_smbcmdsearch_entries_encode_smbcmd(psmbcmd, &entries));
  netwib_er(netwox_smbcmdsearch_entries_show(&entries, &buf1));
  netwib_er(netwib_buf_display(&buf1, NETWIB_ENCODETYPE_DATA));

  netwib_er(netwox_smbcmd_init(&smbcmd));
  netwib_er(netwox_smbcmd_selecttype(&smbcmd, NETWOX_SMBCMD_TYPE_SEARCH_R));
  netwib_er(netwox_smbcmd_setdefault(&smbcmd));
  netwib_er(netwox_smbcmdsearch_entries_decode_smbcmd(&entries, &smbcmd));
  netwib_er(netwox_smbcmdsearch_entries_close(&entries));
  netwib_er(netwox_smbcmdsearch_entries_init(&entries));
  netwib_er(netwox_smbcmdsearch_entries_encode_smbcmd(&smbcmd, &entries));
  netwib_er(netwox_smbcmdsearch_entries_show(&entries, &buf2));
  netwib_er(netwox_smbcmd_close(&smbcmd));
 #if NETWOX_SMB_TEST_REDUNDANTDISPLAY == 1
  netwib_er(netwib_buf_display(&buf2, NETWIB_ENCODETYPE_DATA));
 #else
  netwib_er(netwib_fmt_display("No REDUNDANTDISPLAY\n"));
 #endif
  netwib_er(netwib_buf_cmp(&buf1, &buf2, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    return(NETWOX_ERR_INTERNALERROR);
  }
  netwib_er(netwox_smbcmdsearch_entries_close(&entries));

  netwib_er(netwib_buf_close(&buf2));
  netwib_er(netwib_buf_close(&buf1));

  return(NETWIB_ERR_OK);
}
netwib_bool cmdisunicode;
netwox_smbcmdtscrfmt smbcmdtscrfmt;
netwib_bool smbcmdtscqlastnotimplemented;
static netwib_err netwox_priv_smb_test_smbcmdtscq(netwox_constsmbcmd *psmbcmd)
{
  netwox_smbcmdtscq smbcmdtscq;
  netwox_smbcmd smbcmd;
  netwib_buf buf1, buf2;
  netwib_cmp cmp;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwox_smbcmdtscq_init(&smbcmdtscq));
  ret = netwox_smbcmdtscq_encode_smbcmd(psmbcmd, &smbcmdtscq);
  if (ret == NETWOX_ERR_NOTIMPLEMENTED) {
    /* NETWOX_ERR_NOTIMPLEMENTED is used for commands which I decided
       to not implement because I don't need them. Needed commands
       return NETWOX_ERR_NOTCONVERTED on error. */
    smbcmdtscqlastnotimplemented = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }
  smbcmdtscqlastnotimplemented = NETWIB_FALSE;

  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));

  netwib_er(netwox_smbcmdtscq_show(&smbcmdtscq, &buf1));
  netwib_er(netwib_buf_display(&buf1, NETWIB_ENCODETYPE_DATA));

  netwib_er(netwox_smbcmdtscrfmt_init_smbcmdtscq(&smbcmdtscq,
                                                 &smbcmdtscrfmt));

  netwib_er(netwox_smbcmd_init(&smbcmd));
  netwib_er(netwox_smbcmd_selecttype(&smbcmd,
                                     NETWOX_SMBCMD_TYPE_TRANSACTION_Q));
  netwib_er(netwox_smbcmd_setdefault(&smbcmd));
  smbcmd.value.transaction_q.isunicode = psmbcmd->value.transaction_q.isunicode;
  netwib_er(netwox_smbcmdtscq_decode_smbcmd(&smbcmdtscq, &smbcmd));
  netwib_er(netwox_smbcmdtscq_close(&smbcmdtscq));

  netwib_er(netwox_smbcmdtscq_init(&smbcmdtscq));
  netwib_er(netwox_smbcmdtscq_encode_smbcmd(&smbcmd, &smbcmdtscq));
  netwib_er(netwox_smbcmdtscq_show(&smbcmdtscq, &buf2));
  netwib_er(netwox_smbcmdtscq_close(&smbcmdtscq));
  netwib_er(netwox_smbcmd_close(&smbcmd));
 #if NETWOX_SMB_TEST_REDUNDANTDISPLAY == 1
  netwib_er(netwib_buf_display(&buf2, NETWIB_ENCODETYPE_DATA));
 #else
  netwib_er(netwib_fmt_display("No REDUNDANTDISPLAY\n"));
 #endif
  netwib_er(netwib_buf_cmp(&buf1, &buf2, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  netwib_er(netwib_buf_close(&buf2));
  netwib_er(netwib_buf_close(&buf1));

  return(NETWIB_ERR_OK);
}
netwox_smbcmdtscrstorage smbcmdtscrstoragestorage1;
netwib_bool smbcmdtscrstoragestorage_inuse = NETWIB_FALSE;
static netwib_err netwox_priv_smb_test_smbcmdtscr(netwox_constsmbcmd *psmbcmd)
{
  netwox_smbcmdtscrstorage storage2;
  netwox_smbcmdtscr smbcmdtscr;
  netwib_buf buf1, buf2;
  netwib_bool needmoremsg;
  netwib_cmp cmp;

  if (smbcmdtscqlastnotimplemented) {
    return(NETWIB_ERR_OK);
  }

  if (!smbcmdtscrstoragestorage_inuse) {
    netwib_er(netwox_smbcmdtscrstorage_init(cmdisunicode,
                                            &smbcmdtscrstoragestorage1));
  } else {
    netwib_er(netwib_fmt_display("No init because it is a TSC continuation.\n"));
  }
  netwib_er(netwox_smbcmdtscrstorage_append_smbcmd(psmbcmd,
                                                   &smbcmdtscrstoragestorage1,
                                                   &needmoremsg));
  if (needmoremsg) {
    netwib_er(netwib_fmt_display("We need another TSC message.\n"));
    smbcmdtscrstoragestorage_inuse = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));

  netwib_er(netwox_smbcmdtscr_init(&smbcmdtscr));
  netwib_er(netwox_smbcmdtscr_encode_storager(&smbcmdtscrstoragestorage1,
                                              &smbcmdtscrfmt,
                                              &smbcmdtscr));

  netwib_er(netwox_smbcmdtscr_show(&smbcmdtscr, &buf1));
  netwib_er(netwib_buf_display(&buf1, NETWIB_ENCODETYPE_DATA));

  netwib_er(netwox_smbcmdtscrstorage_init(cmdisunicode, &storage2));
  netwib_er(netwox_smbcmdtscr_decode_storager(&smbcmdtscr, &smbcmdtscrfmt,
                                              &storage2));
  netwib_er(netwox_smbcmdtscr_close(&smbcmdtscr));

  netwib_er(netwox_smbcmdtscr_init(&smbcmdtscr));
  netwib_er(netwox_smbcmdtscr_encode_storager(&storage2, &smbcmdtscrfmt,
                                              &smbcmdtscr));

  netwib_er(netwox_smbcmdtscr_show(&smbcmdtscr, &buf2));
  netwib_er(netwox_smbcmdtscr_close(&smbcmdtscr));
  netwib_er(netwox_smbcmdtscrstorage_close(&storage2));
 #if NETWOX_SMB_TEST_REDUNDANTDISPLAY == 1
  netwib_er(netwib_buf_display(&buf2, NETWIB_ENCODETYPE_DATA));
 #else
  netwib_er(netwib_fmt_display("No REDUNDANTDISPLAY\n"));
 #endif
  netwib_er(netwib_buf_cmp(&buf1, &buf2, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  netwib_er(netwib_buf_close(&buf2));
  netwib_er(netwib_buf_close(&buf1));

  netwib_er(netwox_smbcmdtscrstorage_close(&smbcmdtscrstoragestorage1));
  smbcmdtscrstoragestorage_inuse = NETWIB_FALSE;
  return(NETWIB_ERR_OK);
}
netwox_smbcmdtsc2rfmt smbcmdtsc2rfmt;
netwib_bool smbcmdtsc2qlastnotimplemented;
static netwib_err netwox_priv_smb_test_smbcmdtsc2q(netwox_constsmbcmd *psmbcmd)
{
  netwox_smbcmdtsc2q smbcmdtsc2q;
  netwox_smbcmd smbcmd;
  netwib_buf buf1, buf2;
  netwib_cmp cmp;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwox_smbcmdtsc2q_init(&smbcmdtsc2q));
  ret = netwox_smbcmdtsc2q_encode_smbcmd(psmbcmd, &smbcmdtsc2q);
  if (ret == NETWOX_ERR_NOTIMPLEMENTED) {
    smbcmdtsc2qlastnotimplemented = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }
  smbcmdtsc2qlastnotimplemented = NETWIB_FALSE;

  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));

  netwib_er(netwox_smbcmdtsc2q_show(&smbcmdtsc2q, &buf1));
  netwib_er(netwib_buf_display(&buf1, NETWIB_ENCODETYPE_DATA));

  netwib_er(netwox_smbcmdtsc2rfmt_init_smbcmdtsc2q(&smbcmdtsc2q,
                                                   &smbcmdtsc2rfmt));

  netwib_er(netwox_smbcmd_init(&smbcmd));
  netwib_er(netwox_smbcmd_selecttype(&smbcmd,
                                     NETWOX_SMBCMD_TYPE_TRANSACTION2_Q));
  netwib_er(netwox_smbcmd_setdefault(&smbcmd));
  smbcmd.value.transaction2_q.isunicode = psmbcmd->value.transaction2_q.isunicode;
  netwib_er(netwox_smbcmdtsc2q_decode_smbcmd(&smbcmdtsc2q, &smbcmd));
  netwib_er(netwox_smbcmdtsc2q_close(&smbcmdtsc2q));

  netwib_er(netwox_smbcmdtsc2q_init(&smbcmdtsc2q));
  netwib_er(netwox_smbcmdtsc2q_encode_smbcmd(&smbcmd, &smbcmdtsc2q));
  netwib_er(netwox_smbcmdtsc2q_show(&smbcmdtsc2q, &buf2));
  netwib_er(netwox_smbcmdtsc2q_close(&smbcmdtsc2q));
  netwib_er(netwox_smbcmd_close(&smbcmd));
 #if NETWOX_SMB_TEST_REDUNDANTDISPLAY == 1
  netwib_er(netwib_buf_display(&buf2, NETWIB_ENCODETYPE_DATA));
 #else
  netwib_er(netwib_fmt_display("No REDUNDANTDISPLAY\n"));
 #endif
  netwib_er(netwib_buf_cmp(&buf1, &buf2, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  netwib_er(netwib_buf_close(&buf2));
  netwib_er(netwib_buf_close(&buf1));

  return(NETWIB_ERR_OK);
}
netwox_smbcmdtsc2rstorage smbcmdtsc2rstoragestorage1;
netwib_bool smbcmdtsc2rstoragestorage_inuse = NETWIB_FALSE;
static netwib_err netwox_priv_smb_test_smbcmdtsc2r(netwox_constsmbcmd *psmbcmd)
{
  netwox_smbcmdtsc2rstorage storage2;
  netwox_smbcmdtsc2r smbcmdtsc2r;
  netwib_buf buf1, buf2;
  netwib_bool needmoremsg;
  netwib_cmp cmp;

  if (smbcmdtsc2qlastnotimplemented) {
    return(NETWIB_ERR_OK);
  }

  if (!smbcmdtsc2rstoragestorage_inuse) {
    netwib_er(netwox_smbcmdtsc2rstorage_init(cmdisunicode,
                                             &smbcmdtsc2rstoragestorage1));
  } else {
    netwib_er(netwib_fmt_display("No init because it is a TSC2 continuation.\n"));
  }
  netwib_er(netwox_smbcmdtsc2rstorage_append_smbcmd(psmbcmd,
                                                   &smbcmdtsc2rstoragestorage1,
                                                    &needmoremsg));
  if (needmoremsg) {
    netwib_er(netwib_fmt_display("We need another TSC2 message.\n"));
    smbcmdtsc2rstoragestorage_inuse = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));

  netwib_er(netwox_smbcmdtsc2r_init(&smbcmdtsc2r));
  netwib_er(netwox_smbcmdtsc2r_encode_storager(&smbcmdtsc2rstoragestorage1,
                                               &smbcmdtsc2rfmt,
                                               &smbcmdtsc2r));

  netwib_er(netwox_smbcmdtsc2r_show(&smbcmdtsc2r, &buf1));
  netwib_er(netwib_buf_display(&buf1, NETWIB_ENCODETYPE_DATA));

  netwib_er(netwox_smbcmdtsc2rstorage_init(cmdisunicode, &storage2));
  netwib_er(netwox_smbcmdtsc2r_decode_storager(&smbcmdtsc2r, &smbcmdtsc2rfmt,
                                               &storage2));
  netwib_er(netwox_smbcmdtsc2r_close(&smbcmdtsc2r));

  netwib_er(netwox_smbcmdtsc2r_init(&smbcmdtsc2r));
  netwib_er(netwox_smbcmdtsc2r_encode_storager(&storage2, &smbcmdtsc2rfmt,
                                               &smbcmdtsc2r));

  netwib_er(netwox_smbcmdtsc2r_show(&smbcmdtsc2r, &buf2));
  netwib_er(netwox_smbcmdtsc2r_close(&smbcmdtsc2r));
  netwib_er(netwox_smbcmdtsc2rstorage_close(&storage2));
 #if NETWOX_SMB_TEST_REDUNDANTDISPLAY == 1
  netwib_er(netwib_buf_display(&buf2, NETWIB_ENCODETYPE_DATA));
 #else
  netwib_er(netwib_fmt_display("No REDUNDANTDISPLAY\n"));
 #endif
  netwib_er(netwib_buf_cmp(&buf1, &buf2, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  netwib_er(netwib_buf_close(&buf2));
  netwib_er(netwib_buf_close(&buf1));

  netwib_er(netwox_smbcmdtsc2rstorage_close(&smbcmdtsc2rstoragestorage1));
  smbcmdtsc2rstoragestorage_inuse = NETWIB_FALSE;
  return(NETWIB_ERR_OK);
}
netwox_smbcmdtscntrfmt smbcmdtscntrfmt;
netwib_bool smbcmdtscntqlastnotimplemented;
/* for quick hack */
#define NETWOX_MULTIPLEXID_LASTNTNOTIFY_MAX 10
netwib_uint32 multiplexid_lastntnotify[NETWOX_MULTIPLEXID_LASTNTNOTIFY_MAX] = {0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu};
netwib_uint32 multiplexid_lastntnotify_toset = 0;
static netwib_err netwox_priv_smb_test_smbcmdtscntq(netwox_constsmbcmd *psmbcmd,
                                                    netwib_bool *pisntnotify)
{
  netwox_smbcmdtscntq smbcmdtscntq;
  netwox_smbcmd smbcmd;
  netwib_buf buf1, buf2;
  netwib_cmp cmp;
  netwib_err ret=NETWIB_ERR_OK;

  *pisntnotify = NETWIB_FALSE;

  netwib_er(netwox_smbcmdtscntq_init(&smbcmdtscntq));
  ret = netwox_smbcmdtscntq_encode_smbcmd(psmbcmd, &smbcmdtscntq);
  if (ret == NETWOX_ERR_NOTIMPLEMENTED) {
    smbcmdtscntqlastnotimplemented = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }
  smbcmdtscntqlastnotimplemented = NETWIB_FALSE;

  if (smbcmdtscntq.type == NETWOX_SMBCMDTSCNTQ_TYPE_NTNOTIFY) {
    *pisntnotify = NETWIB_TRUE;
  }

  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));

  netwib_er(netwox_smbcmdtscntq_show(&smbcmdtscntq, &buf1));
  netwib_er(netwib_buf_display(&buf1, NETWIB_ENCODETYPE_DATA));

  netwib_er(netwox_smbcmdtscntrfmt_init_smbcmdtscntq(&smbcmdtscntq,
                                                     &smbcmdtscntrfmt));

  netwib_er(netwox_smbcmd_init(&smbcmd));
  netwib_er(netwox_smbcmd_selecttype(&smbcmd,
                                     NETWOX_SMBCMD_TYPE_NTTRANSACT_Q));
  netwib_er(netwox_smbcmd_setdefault(&smbcmd));
  smbcmd.value.nttransact_q.isunicode = psmbcmd->value.nttransact_q.isunicode;
  netwib_er(netwox_smbcmdtscntq_decode_smbcmd(&smbcmdtscntq, &smbcmd));
  netwib_er(netwox_smbcmdtscntq_close(&smbcmdtscntq));

  netwib_er(netwox_smbcmdtscntq_init(&smbcmdtscntq));
  netwib_er(netwox_smbcmdtscntq_encode_smbcmd(&smbcmd, &smbcmdtscntq));
  netwib_er(netwox_smbcmdtscntq_show(&smbcmdtscntq, &buf2));
  netwib_er(netwox_smbcmdtscntq_close(&smbcmdtscntq));
  netwib_er(netwox_smbcmd_close(&smbcmd));
 #if NETWOX_SMB_TEST_REDUNDANTDISPLAY == 1
  netwib_er(netwib_buf_display(&buf2, NETWIB_ENCODETYPE_DATA));
 #else
  netwib_er(netwib_fmt_display("No REDUNDANTDISPLAY\n"));
 #endif
  netwib_er(netwib_buf_cmp(&buf1, &buf2, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  netwib_er(netwib_buf_close(&buf2));
  netwib_er(netwib_buf_close(&buf1));

  return(NETWIB_ERR_OK);
}
netwox_smbcmdtscntrstorage smbcmdtscntrstoragestorage1;
netwib_bool smbcmdtscntrstoragestorage_inuse = NETWIB_FALSE;
static netwib_err netwox_priv_smb_test_smbcmdtscntr(netwox_constsmbcmd *psmbcmd)
{
  netwox_smbcmdtscntrstorage storage2;
  netwox_smbcmdtscntr smbcmdtscntr;
  netwib_buf buf1, buf2;
  netwib_bool needmoremsg;
  netwib_cmp cmp;

  if (smbcmdtscntqlastnotimplemented) {
    return(NETWIB_ERR_OK);
  }

  if (!smbcmdtscntrstoragestorage_inuse) {
    netwib_er(netwox_smbcmdtscntrstorage_init(cmdisunicode,
                                              &smbcmdtscntrstoragestorage1));
  } else {
    netwib_er(netwib_fmt_display("No init because it is a TSCNT continuation.\n"));
  }
  netwib_er(netwox_smbcmdtscntrstorage_append_smbcmd(psmbcmd,
                                                  &smbcmdtscntrstoragestorage1,
                                                     &needmoremsg));
  if (needmoremsg) {
    netwib_er(netwib_fmt_display("We need another TSCNT message.\n"));
    smbcmdtscntrstoragestorage_inuse = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));

  netwib_er(netwox_smbcmdtscntr_init(&smbcmdtscntr));
  netwib_er(netwox_smbcmdtscntr_encode_storager(&smbcmdtscntrstoragestorage1,
                                                &smbcmdtscntrfmt,
                                                &smbcmdtscntr));

  netwib_er(netwox_smbcmdtscntr_show(&smbcmdtscntr, &buf1));
  netwib_er(netwib_buf_display(&buf1, NETWIB_ENCODETYPE_DATA));

  netwib_er(netwox_smbcmdtscntrstorage_init(cmdisunicode, &storage2));
  netwib_er(netwox_smbcmdtscntr_decode_storager(&smbcmdtscntr,
                                                &smbcmdtscntrfmt,
                                                &storage2));
  netwib_er(netwox_smbcmdtscntr_close(&smbcmdtscntr));

  netwib_er(netwox_smbcmdtscntr_init(&smbcmdtscntr));
  netwib_er(netwox_smbcmdtscntr_encode_storager(&storage2, &smbcmdtscntrfmt,
                                                &smbcmdtscntr));

  netwib_er(netwox_smbcmdtscntr_show(&smbcmdtscntr, &buf2));
  netwib_er(netwox_smbcmdtscntr_close(&smbcmdtscntr));
  netwib_er(netwox_smbcmdtscntrstorage_close(&storage2));
 #if NETWOX_SMB_TEST_REDUNDANTDISPLAY == 1
  netwib_er(netwib_buf_display(&buf2, NETWIB_ENCODETYPE_DATA));
 #else
  netwib_er(netwib_fmt_display("No REDUNDANTDISPLAY\n"));
 #endif
  netwib_er(netwib_buf_cmp(&buf1, &buf2, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  netwib_er(netwib_buf_close(&buf2));
  netwib_er(netwib_buf_close(&buf1));

  netwib_er(netwox_smbcmdtscntrstorage_close(&smbcmdtscntrstoragestorage1));
  smbcmdtscntrstoragestorage_inuse = NETWIB_FALSE;
  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
netwib_bool nextisraw = NETWIB_FALSE;
static netwib_err netwox_priv_smb_test_analyze(netwib_buf *psmb,
                                               netwib_port lastport)
{
  netwib_buf smbpkt, tmp, label, buf;
  netwib_uint32 skipsize;
  netwox_smbnbtss smbnbtss;
  netwox_smbnaked smbnaked;
  netwox_smbmsg smbmsg;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_buf_init_ext_empty(&smbpkt));
  if (lastport == NETWOX_SERVICES_SMBNBTSS) {
    netwib_er(netwox_pkt_decode_smbnbtss(psmb, &smbnbtss, &skipsize));
    psmb->beginoffset += skipsize;
#if NETWOX_SMB_TEST_SMBNBTSS == 1
    netwib_er(netwib_fmt_display("type = %{uint32}\n", smbnbtss.smbnbtsstype));
#endif
    switch(smbnbtss.smbnbtsstype) {
    case NETWOX_SMBNBTSSTYPE_MSG :
      smbpkt = smbnbtss.payload.msg.smbpkt;
      break;
    case NETWOX_SMBNBTSSTYPE_REQUEST :
      netwib_er(netwib_buf_init_mallocdefault(&tmp));
#if NETWOX_SMB_TEST_SMBNBTSS == 1
      netwib_er(netwib_fmt_display("called  = %{byte} ",
                                   smbnbtss.payload.request.callednamelabel));
#endif
      netwib_er(netwib_buf_init_ext_arraysizeoffilled(smbnbtss.payload.request.calledname, &label));
      netwib_er(netwox_buf_append_netbiosname(&label, &tmp));
#if NETWOX_SMB_TEST_SMBNBTSS == 1
      netwib_er(netwib_buf_display(&tmp, NETWIB_ENCODETYPE_MIXED));
      netwib_er(netwib_fmt_display("\ncalling = %{byte} ",
                                   smbnbtss.payload.request.callingnamelabel));
#endif
      netwib_er(netwib_buf_init_ext_arraysizeoffilled(smbnbtss.payload.request.callingname, &label));
      netwib__buf_reinit(&tmp);
      netwib_er(netwox_buf_append_netbiosname(&label, &tmp));
#if NETWOX_SMB_TEST_SMBNBTSS == 1
      netwib_er(netwib_buf_display(&tmp, NETWIB_ENCODETYPE_MIXED));
      netwib_er(netwib_fmt_display("\n"));
#endif
      netwib_er(netwib_buf_close(&tmp));
      return(NETWIB_ERR_OK);
    case NETWOX_SMBNBTSSTYPE_RESPPOSITIVE :
      return(NETWIB_ERR_OK);
    case NETWOX_SMBNBTSSTYPE_RESPNEGATIVE :
#if NETWOX_SMB_TEST_SMBNBTSS == 1
      netwib_er(netwib_fmt_display("error = %{uint32}\n",
                                   smbnbtss.payload.respnegative.error));
#endif
      return(NETWIB_ERR_OK);
    case NETWOX_SMBNBTSSTYPE_RESPRETARGET :
#if NETWOX_SMB_TEST_SMBNBTSS == 1
      netwib_er(netwib_fmt_display("target = %{ip}:%{port}\n",
                                   smbnbtss.payload.respretarget.host,
                                   smbnbtss.payload.respretarget.port));
#endif
      return(NETWIB_ERR_OK);
    case NETWOX_SMBNBTSSTYPE_KEEPALIVE :
    case NETWOX_SMBNBTSSTYPE_KEEPALIVE95 :
      return(NETWIB_ERR_OK);
    default :
      return(NETWOX_ERR_INTERNALERROR);
    }
  } else if (lastport == NETWOX_SERVICES_SMBNAKED) {
    netwib_er(netwox_pkt_decode_smbnaked(psmb, &smbnaked, &skipsize));
    psmb->beginoffset += skipsize;
    smbpkt = smbnaked.smbpkt;
  } else {
    return(NETWOX_ERR_INTERNALERROR);
  }

  if (nextisraw) {
    netwib_er(netwib_fmt_display("This message contains raw data. Skip.\n"));
    nextisraw = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  }

#if NETWOX_SMB_TEST_SMBSIZE == 1
  netwib_er(netwib_fmt_display("SMB pkt size = %{uint32}\n",
                               netwib__buf_ref_data_size(&smbpkt)));
#endif
  netwib_er(netwox_pkt_decode_smbmsg(&smbpkt, &smbmsg));
  netwib_eg(netwib_buf_init_mallocdefault(&buf));
  netwib_eg(netwox_smbmsg_show(&smbmsg, &buf));
  netwib_eg(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
#if NETWOX_SMB_TEST_SMBMSG == 1 || NETWOX_SMB_TEST_SMBCMD == 1
  {
    netwib_buf buf2;
    netwox_smbmsg smbmsg2;
    netwib_cmp cmp;
    netwib_eg(netwib_buf_init_mallocdefault(&buf2));
#if NETWOX_SMB_TEST_SMBMSG == 1
    {
      netwib_buf pkt;
      netwib_er(netwib_fmt_display("smbmsg test\n"));
      netwib_eg(netwib_buf_init_mallocdefault(&pkt));
      netwib_eg(netwox_pkt_append_smbmsg(&smbmsg, &pkt));
     #if NETWOX_SMB_TEST_REDUNDANTDISPLAY == 1
      netwib_eg(netwib_buf_display(&pkt, NETWIB_ENCODETYPE_DUMP));
     #else
      netwib_er(netwib_fmt_display("No REDUNDANTDISPLAY\n"));
     #endif
      netwib_eg(netwox_pkt_decode_smbmsg(&pkt, &smbmsg2));
      netwib_eg(netwox_smbmsg_show(&smbmsg2, &buf2));
     #if NETWOX_SMB_TEST_REDUNDANTDISPLAY == 1
      netwib_eg(netwib_buf_display(&buf2, NETWIB_ENCODETYPE_DATA));
     #else
      netwib_er(netwib_fmt_display("No REDUNDANTDISPLAY\n"));
     #endif
      netwib_eg(netwib_buf_cmp(&buf, &buf2, &cmp));
      if (cmp != NETWIB_CMP_EQ) {
        return(NETWOX_ERR_INTERNALERROR);
      }
      netwib_eg(netwib_buf_close(&pkt));
      if (smbmsg.hdr.error) {
        netwib_eg(netwib_fmt_display("Note: smbmsg.hdr.error is set.\n"));
      }
    }
#endif
#if NETWOX_SMB_TEST_SMBCMD == 1
    {
      netwox_smbcmd smbcmd;
      netwib_uint32 j;
      netwib_bool symetric, noerror;
      netwib_eg(netwib_fmt_display("smbcmd test\n"));
      netwib_eg(netwox_smbmsg_init(&smbmsg2));
      smbmsg2.hdr = smbmsg.hdr;
      symetric = NETWIB_TRUE;
      noerror = NETWIB_TRUE;
      for (j = 0; j < smbmsg.numcmds; j++) {
        netwib_eg(netwib_fmt_display("j = %{uint32}\n", j));
        netwib_eg(netwox_smbcmd_init(&smbcmd));
        ret = netwox_smbmsg_decode_smbcmd(&smbmsg, j, &smbcmd);
        if (ret != NETWIB_ERR_OK) {
          if (smbmsg.hdr.error &&
              netwib__buf_ref_data_size(&smbmsg.cmds[j].para) == 0 &&
              netwib__buf_ref_data_size(&smbmsg.cmds[j].data) == 0) {
            netwib_eg(netwib_fmt_display(" ignore error because smbmsg.hdr.error is set\n"));
            noerror = NETWIB_FALSE;
            ret = NETWIB_ERR_OK;
          } else {
            netwib_goto(ret);
          }
        } else {
          if (smbcmd.type == NETWOX_SMBCMD_TYPE_NEGOTIATE_Q) {
            symetric = NETWIB_FALSE;
          }
          nextisraw = NETWIB_FALSE;
          if (smbcmd.type == NETWOX_SMBCMD_TYPE_READRAW_Q ||
              smbcmd.type == NETWOX_SMBCMD_TYPE_WRITERAW_R) {
            nextisraw = NETWIB_TRUE;
          }
          netwib__buf_reinit(&buf2);
          netwib_eg(netwox_smbcmd_show(&smbcmd, &buf2));
         #if NETWOX_SMB_TEST_REDUNDANTDISPLAY == 1
          netwib_eg(netwib_buf_display(&buf2, NETWIB_ENCODETYPE_DATA));
         #else
          netwib_er(netwib_fmt_display("No REDUNDANTDISPLAY\n"));
         #endif
          smbmsg2.cmds[j].dataisaligned = smbmsg.cmds[j].dataisaligned;
          netwib_eg(netwox_smbmsg_append_smbcmd(&smbcmd, &smbmsg2));
        }
        netwib_eg(netwox_smbcmd_close(&smbcmd));
      }
      if (noerror) {
        netwib__buf_reinit(&buf2);
        netwib_eg(netwox_smbmsg_show(&smbmsg2, &buf2));
       #if NETWOX_SMB_TEST_REDUNDANTDISPLAY == 1
        netwib_eg(netwib_buf_display(&buf2, NETWIB_ENCODETYPE_DATA));
       #else
        netwib_er(netwib_fmt_display("No REDUNDANTDISPLAY\n"));
       #endif
        if (symetric) {
          netwib_eg(netwib_buf_cmp(&buf, &buf2, &cmp));
          if (cmp != NETWIB_CMP_EQ) {
            return(NETWOX_ERR_INTERNALERROR);
          }
        }
      }
      netwib_eg(netwox_smbmsg_close(&smbmsg2));
    }
#endif
#if NETWOX_SMB_TEST_SMBCMDTSC == 1
    {
      netwox_smbcmd smbcmd;
      netwib_uint32 j;
      netwib_bool isntnotify;
      /* quick hack for NT NOTIFY which is asynchronous :
          - we can have NT NOTIFY query,
          - then for example NT IOCTL query and answer,
          - we can have NT NOTIFY query2,
          - then for example another NT IOCTL query and answer,
          - then NT NOTIFY reply
          - then NT NOTIFY reply2
      */
      isntnotify = NETWIB_FALSE;
      for (j = 0; j < NETWOX_MULTIPLEXID_LASTNTNOTIFY_MAX; j++) {
        if ((netwib_uint32)smbmsg.hdr.multiplexid == multiplexid_lastntnotify[j]) {
          isntnotify = NETWIB_TRUE;
          break;
        }
      }
      if (isntnotify) {
        if (smbcmdtscntrfmt.type != NETWOX_SMBCMDTSCNTR_TYPE_NTNOTIFY) {
          netwib_er(netwib_fmt_display("Quick NT NOTIFY hack activated.\n"));
          smbcmdtscntrfmt.type = NETWOX_SMBCMDTSCNTR_TYPE_NTNOTIFY;
        }
      }
      /* end of hack */
      cmdisunicode = NETWIB_FALSE;
      if (smbmsg.hdr.flags2 & NETWOX_SMBMSG_FLAGS2_UNICODESTRINGS) {
        cmdisunicode = NETWIB_TRUE;
      }
      netwib_eg(netwib_fmt_display("smbcmdtsc test\n"));
      for (j = 0; j < smbmsg.numcmds; j++) {
        netwib_eg(netwib_fmt_display("j = %{uint32}\n", j));
        netwib_eg(netwox_smbcmd_init(&smbcmd));
        ret = netwox_smbmsg_decode_smbcmd(&smbmsg, j, &smbcmd);
        if (ret == NETWIB_ERR_OK) {
          if (smbcmd.type == NETWOX_SMBCMD_TYPE_SEARCH_Q) {
            netwib_eg(netwox_priv_smb_test_smbcmdsearchq(&smbcmd));
          } else if (smbcmd.type == NETWOX_SMBCMD_TYPE_SEARCH_R) {
            netwib_eg(netwox_priv_smb_test_smbcmdsearchr(&smbcmd));
          } else if (smbcmd.type == NETWOX_SMBCMD_TYPE_TRANSACTION_Q) {
            netwib_eg(netwox_priv_smb_test_smbcmdtscq(&smbcmd));
          } else if (smbcmd.type == NETWOX_SMBCMD_TYPE_TRANSACTION_R) {
            netwib_eg(netwox_priv_smb_test_smbcmdtscr(&smbcmd));
          } else if (smbcmd.type == NETWOX_SMBCMD_TYPE_TRANSACTION2_Q) {
            netwib_eg(netwox_priv_smb_test_smbcmdtsc2q(&smbcmd));
          } else if (smbcmd.type == NETWOX_SMBCMD_TYPE_TRANSACTION2_R) {
            if (smbmsg.hdr.error) {
              netwib_eg(netwib_fmt_display("Ignore msg with error\n", ret));
            } else {
              netwib_eg(netwox_priv_smb_test_smbcmdtsc2r(&smbcmd));
            }
          } else if (smbcmd.type == NETWOX_SMBCMD_TYPE_NTTRANSACT_Q) {
            netwib_eg(netwox_priv_smb_test_smbcmdtscntq(&smbcmd, &isntnotify));
            if (isntnotify) {
              /* for quick hack */
              multiplexid_lastntnotify[multiplexid_lastntnotify_toset++] = smbmsg.hdr.multiplexid;
              if (multiplexid_lastntnotify_toset == NETWOX_MULTIPLEXID_LASTNTNOTIFY_MAX) {
                multiplexid_lastntnotify_toset = 0;
              }
            }
          } else if (smbcmd.type == NETWOX_SMBCMD_TYPE_NTTRANSACT_R) {
            if (smbmsg.hdr.error) {
              netwib_eg(netwib_fmt_display("Ignore msg with error\n", ret));
            } else {
              netwib_eg(netwox_priv_smb_test_smbcmdtscntr(&smbcmd));
            }
          }
        } else {
          netwib_eg(netwib_fmt_display("error = %{uint32} (ignore)\n", ret));
          ret = NETWIB_ERR_OK;
        }
        netwib_eg(netwox_smbcmd_close(&smbcmd));
      }
    }
#endif
    netwib_eg(netwib_buf_close(&buf2));
  }
#endif
  netwib_eg(netwib_buf_close(&buf));

 netwib_gotolabel:
  if (ret == NETWIB_ERR_DATAMISSING) {
    ret = NETWIB_ERR_NOTCONVERTED;
  }
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smb_test(void)
{
  netwib_buf pkt, smb, filename;
  netwib_bufpool *pbufpool;
  netwib_io *piorecord;
  netwib_ip lastip;
  netwib_port lastport;
  netwib_uint32 i;
  netwib_err ret=NETWIB_ERR_OK;

#if NETWOX_SMB_TEST_SMBINIT == 1
  {
    netwib_buf buf;
    netwox_smbmsg smbmsg;
    netwox_smbcmd smbcmd;
    netwib_er(netwib_buf_init_mallocdefault(&buf));
    netwib_er(netwox_smbmsg_init(&smbmsg));
    netwib_er(netwox_smbmsg_setdefault(&smbmsg));
    netwib_er(netwox_smbmsg_show(&smbmsg, &buf));
    netwib_er(netwox_smbmsg_close(&smbmsg));
    netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
    for (i = 1; i <= 255; i++) {
      netwib__buf_reinit(&buf);
      netwib_er(netwib_fmt_display("\n"));
      netwib_er(netwox_smbcmd_init(&smbcmd));
      ret = netwox_smbcmd_selecttype(&smbcmd, i);
      if (ret != NETWIB_ERR_OK) {
        netwib_er(netwib_fmt_display("i %{uint32} not yet implemented\n", i));
      } else {
        netwib_er(netwox_smbcmd_setdefault(&smbcmd));
        netwib_er(netwox_smbcmd_show(&smbcmd, &buf));
        netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
      }
      netwib_er(netwox_smbcmd_close(&smbcmd));
    }
    netwib_er(netwib_buf_close(&buf));
  }
#endif

  /* initialize record */
  netwib_er(netwib_buf_init_ext_string(NETWOX_SMB_CAPTURE_FILE, &filename));
  netwib_er(netwib_io_init_record_read(&filename, &piorecord));

  /* main loop */
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_buf_init_mallocdefault(&smb));
  smb.flags |= NETWIB_BUF_FLAGS_CANSLIDE;
  netwib_er(netwib_ip_init_ip4(0, &lastip));
  lastport = NETWOX_SERVICES_SMBNBTSS;
  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  i = 0;
  while (NETWIB_TRUE) {
    i++;
#if NETWOX_SMB_TEST_SNIFFEDPKTNUM == 1
    netwib_er(netwib_fmt_display("\ni=%{uint32}\n", i));
#endif
    /* read packet */
    netwib__buf_reinit(&pkt);
    ret = netwib_io_read(piorecord, &pkt);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }

#if 0
    if (i < 975) {
      continue;
    }
#endif
    /* add its data to flow (group both ways) */
    netwib_er(netwox_priv_smb_test_flow(pbufpool, &pkt, &smb, &lastip,
                                        &lastport));
    /* analyze (loop because we can have several messages in one packet) */
    while (NETWIB_TRUE) {
      ret = netwox_priv_smb_test_analyze(&smb, lastport);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
      if (netwib__buf_ref_data_size(&smb) == 0) {
        break;
      }
    }
    if (ret == NETWIB_ERR_DATAMISSING) {
#if NETWOX_SMB_TEST_FLOW == 1
      netwib_er(netwib_fmt_display("Data is missing.\n"));
#endif
      continue;
    }
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }

  /* check for left stored data */
  if (netwib__buf_ref_data_size(&smb)) {
    netwib_er(netwib_fmt_display("There is continuation data in buffer.\n"));
    if (ret == NETWIB_ERR_OK) {
      ret = NETWOX_ERR_INTERNALERROR;
    }
  }

  /* close */
  netwib_er(netwib_bufpool_close(&pbufpool));
  netwib_er(netwib_buf_close(&smb));
  netwib_er(netwib_buf_close(&pkt));
  netwib_er(netwib_io_close(&piorecord));

  return(ret);
}
