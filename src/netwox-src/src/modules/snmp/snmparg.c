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
static netwib_err netwox_snmparg_init_authei(netwox_snmparg *psnmparg,
                                             netwib_io *psockio)
{
  netwox_snmpmsg snmpmsg;
  netwox_snmp3usmsp snmp3usmsp;
  netwox_asn1data asn1data;
  netwib_time t;
  netwib_uint32 id;
  netwib_err ret;

  /* Send for example :
SNMP____________________________________________________________.
| version: 3 (SNMPv3)                                           |
| id: 1874602455                                                |
| maxsize: 1472                                                 |
| flags: 00000100 (reportable,!priv,!auth)                      |
| securitymodel: 3 (usm)                                        |
| securityparameters:                                           |
|  authoritativeengineid:                                       |
|  authoritativeengineboots: 0                                  |
|  authoritativeenginetime: 0                                   |
|  username:                                                    |
|  authenticationparameters:                                    |
|  privacyparameters:                                           |
| contextengineid:                                              |
| contextname:                                                  |
| GetRequest:                                                   |
|  requestid: 1129455247                                        |
|  errorstatus: 0 (no error)                                    |
|  errorindex: 0                                                |
|_______________________________________________________________|
  */
  netwib_er(netwox_snmpmsg_init(psnmparg->pbufpool, &snmpmsg));
  netwib_er(netwox_snmpmsg_set_type(&snmpmsg, NETWOX_SNMPVERSION_3));
  netwib_er(netwox_asn1data_init(psnmparg->pbufpool, &asn1data));
  netwib_er(netwox_snmp3usmsp_init(psnmparg->pbufpool, &snmp3usmsp));
  snmp3usmsp.authoritativeengineboots =psnmparg->v.v3.authoritativeengineboots;
  snmp3usmsp.authoritativeenginetime = psnmparg->v.v3.authoritativeenginetime;
  netwib_er(netwox_asn1data_append_snmp3usmsp(&snmp3usmsp, &asn1data));
  netwib_er(netwox_snmp3usmsp_close(&snmp3usmsp));
  netwib__buf_reinit(snmpmsg.v.v3.psecurityparameters);
  netwib_er(netwox_pktber_append_asn1data(&asn1data,
                                          snmpmsg.v.v3.psecurityparameters));
  netwib_er(netwox_asn1data_close(&asn1data));
  snmpmsg.v.v3.globalheader.flags |= NETWOX_SNMP3_FLAGS_REPORTABLE;
  netwib_er(netwox_snmppdu_set_type(&snmpmsg.v.v3.clearpdu.snmppdu,
                                    NETWOX_SNMPPDUTYPE_GETREQUEST));
  id = snmpmsg.v.v3.globalheader.id;
  ret = netwox_snmpex_write(psockio, &snmpmsg, psnmparg->display);
  netwib_er(netwox_snmpmsg_close(&snmpmsg));
  netwib_er(ret);

  /* read only once : one chance */
  netwib_er(netwox_snmpmsg_init(psnmparg->pbufpool, &snmpmsg));
  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_plus_msec(&t, psnmparg->maxwaitms));
  ret = netwox_snmpex_read(psockio, &t, &snmpmsg, psnmparg->display);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_snmpmsg_close(&snmpmsg));
    return(ret);
  }
  if (snmpmsg.version != NETWOX_SNMPVERSION_3) {
    return(NETWOX_ERR_PROTOCOL);
  }
  if (snmpmsg.v.v3.globalheader.id != id) {
    return(NETWOX_ERR_PROTOCOL);
  }
  if (snmpmsg.v.v3.globalheader.flags & NETWOX_SNMP3_FLAGS_REPORTABLE) {
    return(NETWOX_ERR_PROTOCOL);
  }
  if (snmpmsg.v.v3.globalheader.flags & NETWOX_SNMP3_FLAGS_PRIV) {
    return(NETWOX_ERR_PROTOCOL);
  }
  if (snmpmsg.v.v3.globalheader.securitymodel !=
      NETWOX_SNMP3_SECURITYMODEL_USM) {
    return(NETWOX_ERR_PROTOCOL);
  }
  netwib_er(netwox_asn1data_init(psnmparg->pbufpool, &asn1data));
  netwib_er(netwox_snmp3usmsp_init(psnmparg->pbufpool, &snmp3usmsp));
  ret = netwox_pktber_decode_asn1data_snmp3sp(snmpmsg.v.v3.psecurityparameters,
                                              &asn1data);
  if (ret == NETWIB_ERR_OK) {
    ret = netwox_asn1data_decode_snmp3usmsp(&asn1data, &snmp3usmsp);
    if (ret == NETWIB_ERR_OK) {
      psnmparg->v.v3.authoritativeengineboots =
        snmp3usmsp.authoritativeengineboots;
      psnmparg->v.v3.authoritativeenginetime =
        snmp3usmsp.authoritativeenginetime;
      netwib_er(netwib_buf_append_buf(snmp3usmsp.pauthoritativeengineid,
                                      psnmparg->v.v3.pauthoritativeengineid));
    }
  }
  netwib_er(netwox_snmp3usmsp_close(&snmp3usmsp));
  netwib_er(netwox_asn1data_close(&asn1data));

  netwib_er(netwox_snmpmsg_close(&snmpmsg));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmparg_init(netwox_arg *parg,
                               netwib_io *psockio,
                               netwox_snmparg *psnmparg)
{
  netwib_buf buf;
  netwib_uint32 version;

  netwib_er(netwib_bufpool_initdefault(&psnmparg->pbufpool));

  netwib_er(netwox_arg_uint32(parg, 'T', &psnmparg->maxwaitms));

  netwib_er(netwox_arg_bool(parg, 'y', &psnmparg->display));

  netwib_er(netwox_arg_uint32(parg, 'v', &version));
  switch(version) {
  case 1 : psnmparg->version = NETWOX_SNMPVERSION_1; break;
  case 2 : psnmparg->version = NETWOX_SNMPVERSION_2C; break;
  case 3 : psnmparg->version = NETWOX_SNMPVERSION_3; break;
  default : return(NETWIB_ERR_PAINVALIDTYPE); break;
  }

  switch(psnmparg->version) {
  case NETWOX_SNMPVERSION_1 :
  case NETWOX_SNMPVERSION_2C :
    netwib_er(netwib_bufpool_buf_init(psnmparg->pbufpool,
                                      &psnmparg->v.v12.pcommunity));
    netwib_er(netwox_arg_buf(parg, 'c', &buf));
    netwib_er(netwib_buf_append_buf(&buf, psnmparg->v.v12.pcommunity));
    break;
  case NETWOX_SNMPVERSION_3 :
    netwib_er(netwox_arg_bool(parg, 'm', &psnmparg->v.v3.md5auth));
    netwib_er(netwib_bufpool_buf_init(psnmparg->pbufpool,
                                      &psnmparg->v.v3.pusername));
    netwib_er(netwox_arg_buf(parg, 'u', &buf));
    netwib_er(netwib_buf_append_buf(&buf, psnmparg->v.v3.pusername));
    netwib_er(netwib_bufpool_buf_init(psnmparg->pbufpool,
                                      &psnmparg->v.v3.ppassword));
    netwib_er(netwox_arg_buf(parg, 'w', &buf));
    netwib_er(netwib_buf_append_buf(&buf, psnmparg->v.v3.ppassword));
    netwib_er(netwib_bufpool_buf_init(psnmparg->pbufpool,
                                      &psnmparg->v.v3.pauthoritativeengineid));
    netwib_er(netwox_arg_buf(parg, 'A', &buf));
    netwib_er(netwib_buf_append_buf(&buf,
                                    psnmparg->v.v3.pauthoritativeengineid));
    netwib_er(netwox_arg_uint32(parg, 'B',
                                &psnmparg->v.v3.authoritativeengineboots));
    netwib_er(netwox_arg_uint32(parg, 'M',
                                &psnmparg->v.v3.authoritativeenginetime));
    netwib_er(netwib_bufpool_buf_init(psnmparg->pbufpool,
                                      &psnmparg->v.v3.pcontextengineid));
    netwib_er(netwox_arg_buf(parg, 'C', &buf));
    netwib_er(netwib_buf_append_buf(&buf, psnmparg->v.v3.pcontextengineid));
    netwib_er(netwib_bufpool_buf_init(psnmparg->pbufpool,
                                      &psnmparg->v.v3.pcontextname));
    netwib_er(netwox_arg_buf(parg, 'N', &buf));
    netwib_er(netwib_buf_append_buf(&buf, psnmparg->v.v3.pcontextname));
    /* eventually set authoritativeengineid */
    if (netwib__buf_ref_data_size(psnmparg->v.v3.pauthoritativeengineid) == 0){
      netwib_er(netwox_snmparg_init_authei(psnmparg, psockio));
    }
    /* eventually set contextengineid */
    if (netwib__buf_ref_data_size(psnmparg->v.v3.pcontextengineid) == 0) {
      netwib_er(netwib_buf_append_buf(psnmparg->v.v3.pauthoritativeengineid,
                                      psnmparg->v.v3.pcontextengineid));
    }
    /* compute user key */
    netwib_er(netwib_bufpool_buf_init(psnmparg->pbufpool,
                                      &psnmparg->v.v3.pku));
    if (netwib__buf_ref_data_size(psnmparg->v.v3.ppassword)) {
      netwib_er(netwox_snmpmd5_ku(psnmparg->v.v3.ppassword,
                                  psnmparg->v.v3.pku));
    } else if (psnmparg->v.v3.md5auth) {
      netwib_er(netwib_fmt_display("Password must be set if md5auth is chosen\n"));
      return(NETWOX_ERR_TOOLARG_INVALID);
    }
    break;
  default :
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmparg_close(netwox_snmparg *psnmparg)
{

  switch(psnmparg->version) {
  case NETWOX_SNMPVERSION_1 :
  case NETWOX_SNMPVERSION_2C :
    netwib_er(netwib_bufpool_buf_close(psnmparg->pbufpool,
                                       &psnmparg->v.v12.pcommunity));
    break;
  case NETWOX_SNMPVERSION_3 :
    netwib_er(netwib_bufpool_buf_close(psnmparg->pbufpool,
                                       &psnmparg->v.v3.pusername));
    netwib_er(netwib_bufpool_buf_close(psnmparg->pbufpool,
                                       &psnmparg->v.v3.ppassword));
    netwib_er(netwib_bufpool_buf_close(psnmparg->pbufpool,
                                      &psnmparg->v.v3.pauthoritativeengineid));
    netwib_er(netwib_bufpool_buf_close(psnmparg->pbufpool,
                                       &psnmparg->v.v3.pcontextengineid));
    netwib_er(netwib_bufpool_buf_close(psnmparg->pbufpool,
                                       &psnmparg->v.v3.pcontextname));
    netwib_er(netwib_bufpool_buf_close(psnmparg->pbufpool,
                                       &psnmparg->v.v3.pku));
    break;
  default :
    break;
  }

  netwib_er(netwib_bufpool_close(&psnmparg->pbufpool));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmparg_display(netwox_constsnmparg *psnmparg)
{

  netwib_er(netwib_fmt_display("version: %{uint32}\n", psnmparg->version));

  switch(psnmparg->version) {
  case NETWOX_SNMPVERSION_1 :
  case NETWOX_SNMPVERSION_2C :
    netwib_er(netwib_fmt_display("community: "));
    netwib_er(netwib_buf_display(psnmparg->v.v12.pcommunity,
                                 NETWIB_ENCODETYPE_DATA));
    netwib_er(netwib_fmt_display("\n"));
    break;
  case NETWOX_SNMPVERSION_3 :
    netwib_er(netwib_fmt_display("username: "));
    netwib_er(netwib_buf_display(psnmparg->v.v3.pusername,
                                 NETWIB_ENCODETYPE_DATA));
    netwib_er(netwib_fmt_display("\npassword: "));
    netwib_er(netwib_buf_display(psnmparg->v.v3.ppassword,
                                 NETWIB_ENCODETYPE_DATA));
    netwib_er(netwib_fmt_display("\nauthoritativeengineid: "));
    netwib_er(netwib_buf_display(psnmparg->v.v3.pauthoritativeengineid,
                                 NETWIB_ENCODETYPE_DATA));
    netwib_er(netwib_fmt_display("\nauthoritativeengineboots: %{uint32}",
                                 psnmparg->v.v3.authoritativeengineboots));
    netwib_er(netwib_fmt_display("\nauthoritativeenginetime: %{uint32}",
                                 psnmparg->v.v3.authoritativeenginetime));
    netwib_er(netwib_fmt_display("\ncontextengineid: "));
    netwib_er(netwib_buf_display(psnmparg->v.v3.pcontextengineid,
                                 NETWIB_ENCODETYPE_DATA));
    netwib_er(netwib_fmt_display("\ncontextname: "));
    netwib_er(netwib_buf_display(psnmparg->v.v3.pcontextname,
                                 NETWIB_ENCODETYPE_DATA));
    netwib_er(netwib_fmt_display("\n"));
    break;
  default :
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmparg_msg_init_begin(netwox_constsnmparg *psnmparg,
                                         netwox_snmpmsg *psnmpmsg,
                                         netwox_snmp3usmsp *psnmp3usmsp,
                                         netwox_snmppdu **ppsnmppdu)
{

  netwib_er(netwox_snmpmsg_set_type(psnmpmsg, psnmparg->version));

  switch(psnmparg->version) {
  case NETWOX_SNMPVERSION_1 :
  case NETWOX_SNMPVERSION_2C :
    netwib_er(netwib_buf_append_buf(psnmparg->v.v12.pcommunity,
                                    psnmpmsg->v.v12.pcommunity));
    *ppsnmppdu = &psnmpmsg->v.v12.snmppdu;
    break;
  case NETWOX_SNMPVERSION_3 :
    netwib_er(netwox_snmpmsg_set_type(psnmpmsg, psnmparg->version));
    netwib_er(netwib_buf_append_buf(psnmparg->v.v3.pauthoritativeengineid,
                                    psnmp3usmsp->pauthoritativeengineid));
    psnmp3usmsp->authoritativeengineboots =
      psnmparg->v.v3.authoritativeengineboots;
    psnmp3usmsp->authoritativeenginetime =
      psnmparg->v.v3.authoritativeenginetime;
    netwib_er(netwib_buf_append_buf(psnmparg->v.v3.pusername,
                                    psnmp3usmsp->pusername));
    if (psnmparg->v.v3.md5auth) {
      psnmpmsg->v.v3.globalheader.flags |= NETWOX_SNMP3_FLAGS_AUTH;
    }
    netwib_er(netwib_buf_append_buf(psnmparg->v.v3.pcontextengineid,
                                    psnmpmsg->v.v3.clearpdu.pcontextengineid));
    netwib_er(netwib_buf_append_buf(psnmparg->v.v3.pcontextname,
                                    psnmpmsg->v.v3.clearpdu.pcontextname));
    *ppsnmppdu = &psnmpmsg->v.v3.clearpdu.snmppdu;
    break;
  default :
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmparg_msg_init_end(netwox_constsnmparg *psnmparg,
                                       netwox_snmpmsg *psnmpmsg,
                                       netwox_snmp3usmsp *psnmp3usmsp)
{
  netwox_asn1data asn1data;

  if (psnmparg->version != NETWOX_SNMPVERSION_3) {
    return(NETWIB_ERR_OK);
  }

  psnmparg->v.v3.id = psnmpmsg->v.v3.globalheader.id;

  if (psnmparg->v.v3.md5auth) {
    netwib_er(netwox_snmpmd5_compute(psnmpmsg, psnmp3usmsp,
                                     psnmparg->v.v3.pku));
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwox_asn1data_init(psnmparg->pbufpool, &asn1data));
  netwib_er(netwox_asn1data_append_snmp3usmsp(psnmp3usmsp, &asn1data));
  netwib__buf_reinit(psnmpmsg->v.v3.psecurityparameters);
  netwib_er(netwox_pktber_append_asn1data(&asn1data,
                                          psnmpmsg->v.v3.psecurityparameters));
  netwib_er(netwox_asn1data_close(&asn1data));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snmparg_msg_decode(netwox_constsnmparg *psnmparg,
                                     netwox_snmpmsg *psnmpmsg,
                                     netwox_snmp3usmsp *psnmp3usmsp,
                                     netwox_snmppdu **ppsnmppdu)
{
  netwox_asn1data asn1data;
  netwib_bool valid;
  netwib_err ret;

  if (psnmpmsg->version != psnmparg->version) {
    return(NETWOX_ERR_PROTOCOL);
  }

  switch(psnmparg->version) {
  case NETWOX_SNMPVERSION_1 :
  case NETWOX_SNMPVERSION_2C :
    *ppsnmppdu = &psnmpmsg->v.v12.snmppdu;
    break;
  case NETWOX_SNMPVERSION_3 :
    if (psnmpmsg->v.v3.globalheader.securitymodel !=
        NETWOX_SNMP3_SECURITYMODEL_USM) {
      return(NETWOX_ERR_PROTOCOL);
    }
    if (psnmpmsg->v.v3.globalheader.flags & NETWOX_SNMP3_FLAGS_PRIV) {
      return(NETWOX_ERR_PROTOCOL);
    }
    if (psnmparg->v.v3.md5auth) {
      if ( ! (psnmpmsg->v.v3.globalheader.flags & NETWOX_SNMP3_FLAGS_AUTH)) {
        return(NETWOX_ERR_PROTOCOL);
      }
    }
    if (psnmpmsg->v.v3.globalheader.id != psnmparg->v.v3.id) {
      return(NETWOX_ERR_PROTOCOL);
    }
    netwib_er(netwox_asn1data_init(psnmparg->pbufpool, &asn1data));
    ret =netwox_pktber_decode_asn1data_snmp3sp(psnmpmsg->v.v3.psecurityparameters,
                                               &asn1data);
    if (ret == NETWIB_ERR_OK) {
      ret = netwox_asn1data_decode_snmp3usmsp(&asn1data, psnmp3usmsp);
      if (ret == NETWIB_ERR_OK) {
        if (psnmpmsg->v.v3.globalheader.flags & NETWOX_SNMP3_FLAGS_AUTH) {
          netwib_er(netwox_snmpmd5_check(psnmpmsg, psnmp3usmsp,
                                         psnmparg->v.v3.pku, &valid));
          if (!valid) {
            ret = NETWOX_ERR_AUTHERROR;
          }
        }
      }
    }
    netwib_er(netwox_asn1data_close(&asn1data));
    netwib_er(ret);
    /* ignore a lot of checks on authoritativeengine which could be done here*/
    *ppsnmppdu = &psnmpmsg->v.v3.clearpdu.snmppdu;
    break;
  default :
    break;
  }

  return(NETWIB_ERR_OK);
}
