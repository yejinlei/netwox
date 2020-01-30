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
netwib_err netwox_snmpex_write(netwib_io *psockio,
                               netwox_constsnmpmsg *psnmpmsg,
                               netwib_bool display)
{
  netwox_asn1data asn1data;
  netwib_buf *ppkt, buf;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(psnmpmsg->pbufpool, &ppkt));
  netwib_er(netwox_asn1data_init(psnmpmsg->pbufpool, &asn1data));

  netwib_er(netwox_asn1data_append_snmpmsg(psnmpmsg, &asn1data));
  netwib_er(netwox_pktber_append_asn1data(&asn1data, ppkt));

  if (display) {
    netwib_er(netwib_buf_init_mallocdefault(&buf));
    ret = netwox_snmpmsg_show(psnmpmsg, NETWIB_ENCODETYPE_ARRAY, &buf);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_fmt_display("%{buf}\n", &buf));
    } else {
      netwib_er(netwib_fmt_display("Could not display this message\n"));
    }
    netwib_er(netwib_buf_close(&buf));
  }

  ret = netwib_io_write(psockio, ppkt);

  netwib_er(netwox_asn1data_close(&asn1data));
  netwib_er(netwib_bufpool_buf_close(psnmpmsg->pbufpool, &ppkt));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmpex_read(netwib_io *psockio,
                              netwib_time *pabstime,
                              netwox_snmpmsg *psnmpmsg,
                              netwib_bool display)
{
  netwox_asn1data asn1data;
  netwib_buf *ppkt, buf;
  netwib_bool event;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_bufpool_buf_init(psnmpmsg->pbufpool, &ppkt));

  while(NETWIB_TRUE) {
    netwib__buf_reinit(ppkt);
    ret = netwib_io_wait_read(psockio, pabstime, &event);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (!event) {
      ret = NETWOX_ERR_TIMEOUT;
      break;
    }
    ret = netwib_io_read(psockio, ppkt);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATANOTAVAIL) {
        ret = NETWIB_ERR_OK;
        continue;
      }
      if (ret == NETWIB_ERR_DATAEND) break;
      break;
    }
    netwib_er(netwox_asn1data_init(psnmpmsg->pbufpool, &asn1data));
    ret = netwox_pktber_decode_asn1data_snmp(ppkt, &asn1data);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwox_asn1data_close(&asn1data));
      continue;
    }
    ret = netwox_asn1data_decode_snmpmsg(&asn1data, psnmpmsg);
    netwib_er(netwox_asn1data_close(&asn1data));
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwox_snmpmsg_close(psnmpmsg));
      netwib_er(netwox_snmpmsg_init(psnmpmsg->pbufpool, psnmpmsg));
      continue;
    }
    break;
  }

  netwib_er(netwib_bufpool_buf_close(psnmpmsg->pbufpool, &ppkt));

  if (display && ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_init_mallocdefault(&buf));
    ret = netwox_snmpmsg_show(psnmpmsg, NETWIB_ENCODETYPE_ARRAY, &buf);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_fmt_display("%{buf}\n", &buf));
    } else {
      netwib_er(netwib_fmt_display("Could not display this message\n"));
      ret = NETWIB_ERR_OK;
    }
    netwib_er(netwib_buf_close(&buf));
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmpex_session(netwox_snmparg *psnmparg,
                                 netwib_io *psockio,
                                 netwox_snmpex_req_pf pfuncreq,
                                 netwox_snmpex_rep_pf pfuncrep,
                                 netwib_ptr infos)
{
  netwox_snmpmsg snmpmsg;
  netwox_snmppdu *psnmppdu = NULL;
  netwox_snmp3usmsp snmp3usmsp;
  netwib_time t;
  netwib_bool stoploop;
  netwib_err ret;

  /* send */
  netwib_er(netwox_snmpmsg_init(psnmparg->pbufpool, &snmpmsg));
  netwib_er(netwox_snmp3usmsp_init(psnmparg->pbufpool, &snmp3usmsp));
  netwib_er(netwox_snmparg_msg_init_begin(psnmparg, &snmpmsg, &snmp3usmsp,
                                          &psnmppdu));
  ret = (*pfuncreq)(psnmparg, &snmpmsg, &snmp3usmsp, psnmppdu, infos);
  if (ret == NETWIB_ERR_OK) {
   netwib_er(netwox_snmparg_msg_init_end(psnmparg, &snmpmsg, &snmp3usmsp));
    ret = netwox_snmpex_write(psockio, &snmpmsg, psnmparg->display);
  }
  netwib_er(netwox_snmp3usmsp_close(&snmp3usmsp));
  netwib_er(netwox_snmpmsg_close(&snmpmsg));
  netwib_er(ret);

  if (pfuncrep == NULL) {
    return(NETWIB_ERR_OK);
  }

  /* receive */
  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_plus_msec(&t, psnmparg->maxwaitms));
  stoploop= NETWIB_FALSE;
  while(NETWIB_TRUE) {
    netwib_er(netwox_snmpmsg_init(psnmparg->pbufpool, &snmpmsg));
    ret = netwox_snmpex_read(psockio, &t, &snmpmsg, psnmparg->display);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwox_snmpmsg_close(&snmpmsg));
      break;
    }
    netwib_er(netwox_snmp3usmsp_init(psnmparg->pbufpool, &snmp3usmsp));
    ret = netwox_snmparg_msg_decode(psnmparg, &snmpmsg, &snmp3usmsp,
                                    &psnmppdu);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwox_snmp3usmsp_close(&snmp3usmsp));
      netwib_er(netwox_snmpmsg_close(&snmpmsg));
      continue;
    }
    ret = (*pfuncrep)(psnmparg, &snmpmsg, &snmp3usmsp, psnmppdu, infos,
                      &stoploop);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwox_snmp3usmsp_close(&snmp3usmsp));
      netwib_er(netwox_snmpmsg_close(&snmpmsg));
      break;
    }
    netwib_er(netwox_snmp3usmsp_close(&snmp3usmsp));
    netwib_er(netwox_snmpmsg_close(&snmpmsg));
    if (stoploop) {
      break;
    }
  }

  return(ret);
}
