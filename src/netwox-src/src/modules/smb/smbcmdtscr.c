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
netwib_err netwox_smbcmdtscrstorage_init(netwib_bool isunicode,
                                         netwox_smbcmdtscrstorage *pstorage)
{

  pstorage->isunicode = isunicode;
  pstorage->setupcount = 0;

  netwib_er(netwib_buf_init_ext_storage(&pstorage->para));
  netwib_er(netwib_buf_init_ext_storage(&pstorage->data));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscrstorage_close(netwox_smbcmdtscrstorage *pstorage)
{
  netwib_er(netwib_buf_close(&pstorage->para));
  netwib_er(netwib_buf_close(&pstorage->data));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscrstorage_append_smbcmd(netwox_constsmbcmd *psmbcmd,
                                          netwox_smbcmdtscrstorage *pstorage,
                                                   netwib_bool *pneedmoremsg)
{
  netwib_uint32 i;
  netwib_bool isfirst, isalone;

  if (psmbcmd->type != NETWOX_SMBCMD_TYPE_TRANSACTION_R) {
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  isfirst = NETWIB_TRUE;
  if (psmbcmd->value.transaction_r.paradisplacement ||
      psmbcmd->value.transaction_r.datadisplacement) {
    isfirst = NETWIB_FALSE;
  }
  isalone = NETWIB_TRUE;
  if ( (netwib__buf_ref_data_size(&psmbcmd->value.transaction_r.para) !=
        psmbcmd->value.transaction_r.totalparasize) ||
       (netwib__buf_ref_data_size(&psmbcmd->value.transaction_r.data) !=
        psmbcmd->value.transaction_r.totaldatasize) ) {
    isalone = NETWIB_FALSE;
  }
  if ( (netwib__buf_ref_data_size(&psmbcmd->value.transaction_r.para) >
        psmbcmd->value.transaction_r.totalparasize) ||
       (netwib__buf_ref_data_size(&psmbcmd->value.transaction_r.data) >
        psmbcmd->value.transaction_r.totaldatasize) ) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  if (isfirst) {
    if (psmbcmd->value.transaction_r.setupcount > NETWOX_SMBCMD_TRANSACTION_SETUP_MAX) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    pstorage->setupcount = psmbcmd->value.transaction_r.setupcount;
    for (i = 0; i < pstorage->setupcount; i++) {
      pstorage->setup[i] = psmbcmd->value.transaction_r.setup[i];
    }
  } else {
    if (psmbcmd->value.transaction_r.setupcount != 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  if (isalone) {
    netwib_er(netwib_buf_init_ext_buf(&psmbcmd->value.transaction_r.para, &pstorage->para));
    netwib_er(netwib_buf_init_ext_buf(&psmbcmd->value.transaction_r.data, &pstorage->data));
    *pneedmoremsg = NETWIB_FALSE;
  } else {
    if (netwib__buf_ref_data_size(&psmbcmd->value.transaction_r.para)) {
      /* if size of psmbcmd->value.transaction_r.para is zero, we can't
         do this check under WinME, because paradisplacement is incorrectly
         set to zero. */
      if (psmbcmd->value.transaction_r.paradisplacement !=
          netwib__buf_ref_data_size(&pstorage->para)) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
    }
    if (netwib__buf_ref_data_size(&psmbcmd->value.transaction_r.data)) {
      /* if size of psmbcmd->value.transaction_r.data is zero, we can't
         do this check under WinME, because datadisplacement is incorrectly
         set to zero. */
      if (psmbcmd->value.transaction_r.datadisplacement !=
          netwib__buf_ref_data_size(&pstorage->data)) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
    }
    netwib_er(netwib_buf_append_buf(&psmbcmd->value.transaction_r.para, &pstorage->para));
    netwib_er(netwib_buf_append_buf(&psmbcmd->value.transaction_r.data, &pstorage->data));
    if ( (netwib__buf_ref_data_size(&pstorage->para) >
          psmbcmd->value.transaction_r.totalparasize) ||
         (netwib__buf_ref_data_size(&pstorage->data) >
          psmbcmd->value.transaction_r.totaldatasize) ) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    *pneedmoremsg = NETWIB_FALSE;
    if ( (netwib__buf_ref_data_size(&pstorage->para) !=
          psmbcmd->value.transaction_r.totalparasize) ||
         (netwib__buf_ref_data_size(&pstorage->data) !=
          psmbcmd->value.transaction_r.totaldatasize) ) {
      *pneedmoremsg = NETWIB_TRUE;
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscrstorage_decode_smbcmd(netwox_constsmbcmdtscrstorage *pstorage,
                                                  netwib_uint32 *poffset,
                                                  netwib_uint32 maxsize,
                                                  netwox_smbcmd *psmbcmd,
                                                  netwib_bool *pneedmoremsg)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize, i, threshold1, threshold2, storedsize;

  *pneedmoremsg = NETWIB_FALSE;

  psmbcmd->value.transaction_r.totalparasize = (netwib_uint16)netwib__buf_ref_data_size(&pstorage->para);
  psmbcmd->value.transaction_r.totaldatasize = (netwib_uint16)netwib__buf_ref_data_size(&pstorage->data);
  psmbcmd->value.transaction_r.paradisplacement = 0;
  psmbcmd->value.transaction_r.datadisplacement = 0;
  netwib_er(netwib_buf_init_ext_empty(&psmbcmd->value.transaction_r.para));
  netwib_er(netwib_buf_init_ext_empty(&psmbcmd->value.transaction_r.data));

  para = netwib__buf_ref_data_ptr(&pstorage->para);
  parasize = netwib__buf_ref_data_size(&pstorage->para);
  data = netwib__buf_ref_data_ptr(&pstorage->data);
  datasize = netwib__buf_ref_data_size(&pstorage->data);

  storedsize = 0;
  threshold1 = pstorage->setupcount*2;
  threshold2 = threshold1 + parasize;

  if (*poffset == 0) {
    if (pstorage->setupcount) {
      /* suppose all setup integers feeds in first packet */
      if (pstorage->setupcount > NETWOX_SMBCMD_TRANSACTION_SETUP_MAX) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      if (maxsize < (netwib_uint32)(pstorage->setupcount*2)) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      psmbcmd->value.transaction_r.setupcount = pstorage->setupcount;
      for (i = 0; i < pstorage->setupcount; i++) {
        psmbcmd->value.transaction_r.setup[i] = pstorage->setup[i];
      }
      *poffset += threshold1;
      storedsize += threshold1;
    }
  }

  if (parasize) {
    if ((storedsize < maxsize) && (*poffset < threshold1 + parasize)) {
      para += *poffset - threshold1;
      parasize -= *poffset - threshold1;
      if (storedsize + parasize > maxsize) {
        parasize = maxsize - storedsize;
        *pneedmoremsg = NETWIB_TRUE;
      }
      psmbcmd->value.transaction_r.paradisplacement = (netwib_uint16)(*poffset - threshold1);
      netwib_er(netwib_buf_init_ext_arrayfilled(para, parasize, &psmbcmd->value.transaction_r.para));
      *poffset += parasize;
      storedsize += parasize;
    } else {
      psmbcmd->value.transaction2_r.paradisplacement = (netwib_uint16)parasize;
    }
  }

  if (datasize &&
      (storedsize < maxsize) &&
      (*poffset >= threshold2)) {
    data += *poffset - threshold2;
    datasize -= *poffset - threshold2;
    if (storedsize + datasize > maxsize) {
      datasize = maxsize - storedsize;
      *pneedmoremsg = NETWIB_TRUE;
    }
    psmbcmd->value.transaction_r.datadisplacement = (netwib_uint16)(*poffset - threshold2);
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, &psmbcmd->value.transaction_r.data));
    *poffset += datasize;
    storedsize += datasize;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscr_netshareenum_share_init(netwox_smbcmdtscr_netshareenum_share *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_initdefault(&pitem->name));
  netwib_er(netwox_smbcmdcmn_data_initdefault(&pitem->comment));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscr_netshareenum_share_setdefault(netwox_smbcmdtscr_netshareenum_share *pitem)
{

  netwib_er(netwox_smbcmdcmn_data_setdefault(&pitem->name));
  pitem->sharetype = NETWOX_SMBCMDCMN_SHARETYPE_DIR;
  pitem->unknown = 0;
  netwib_er(netwox_smbcmdcmn_data_setdefault(&pitem->comment));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscr_netshareenum_share_close(netwox_smbcmdtscr_netshareenum_share *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_close(&pitem->name));
  netwib_er(netwox_smbcmdcmn_data_close(&pitem->comment));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscr_netserverenum2_server_init(netwox_smbcmdtscr_netserverenum2_server *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_initdefault(&pitem->name));
  netwib_er(netwox_smbcmdcmn_data_initdefault(&pitem->comment));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscr_netserverenum2_server_setdefault(netwox_smbcmdtscr_netserverenum2_server *pitem)
{

  netwib_er(netwox_smbcmdcmn_data_setdefault(&pitem->name));
  pitem->majorversion = NETWOX_SMBCMDTSCR_MAJORVERSION_2000;
  pitem->minorversion = NETWOX_SMBCMDTSCR_MINORVERSION_2000;
  netwox__smbcmdcmn_servertype_initdefault(pitem->servertype);
  netwib_er(netwox_smbcmdcmn_data_setdefault(&pitem->comment));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscr_netserverenum2_server_close(netwox_smbcmdtscr_netserverenum2_server *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_close(&pitem->name));
  netwib_er(netwox_smbcmdcmn_data_close(&pitem->comment));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscrfmt_init_smbcmdtscq(netwox_constsmbcmdtscq *psmbcmdtscq,
                                                netwox_smbcmdtscrfmt *psmbcmdtscrfmt)
{

  switch(psmbcmdtscq->type) {
  case NETWOX_SMBCMDTSCQ_TYPE_NETSHAREENUM :
    psmbcmdtscrfmt->type = NETWOX_SMBCMDTSCR_TYPE_NETSHAREENUM;
    if (psmbcmdtscq->value.netshareenum.vfmt == NETWOX_SMBCMDTSC_NETSHAREENUM_VFMT_1) {
      psmbcmdtscrfmt->value.netshareenum.vfmt = psmbcmdtscq->value.netshareenum.vfmt;
      psmbcmdtscrfmt->value.netshareenum.sharenamelen = 14;
    } else {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETSHAREGETINFO :
    psmbcmdtscrfmt->type = NETWOX_SMBCMDTSCR_TYPE_NETSHAREGETINFO;
    if (psmbcmdtscq->value.netsharegetinfo.vfmt == NETWOX_SMBCMDTSC_NETSHAREGETINFO_VFMT_1) {
      psmbcmdtscrfmt->value.netsharegetinfo.vfmt = psmbcmdtscq->value.netsharegetinfo.vfmt;
      psmbcmdtscrfmt->value.netsharegetinfo.sharenamelen = 14;
    } else {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETSERVERGETINFO :
    psmbcmdtscrfmt->type = NETWOX_SMBCMDTSCR_TYPE_NETSERVERGETINFO;
    if (psmbcmdtscq->value.netservergetinfo.vfmt == NETWOX_SMBCMDTSC_NETSERVERGETINFO_VFMT_1) {
      psmbcmdtscrfmt->value.netservergetinfo.vfmt = psmbcmdtscq->value.netservergetinfo.vfmt;
      psmbcmdtscrfmt->value.netservergetinfo.servernamelen = 16;
    } else {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETWKSTAGETINFO :
    psmbcmdtscrfmt->type = NETWOX_SMBCMDTSCR_TYPE_NETWKSTAGETINFO;
    if (psmbcmdtscq->value.netwkstagetinfo.vfmt == NETWOX_SMBCMDTSC_NETWKSTAGETINFO_VFMT_1) {
      psmbcmdtscrfmt->value.netwkstagetinfo.vfmt = psmbcmdtscq->value.netwkstagetinfo.vfmt;
    } else {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBCMDTSCQ_TYPE_NETSERVERENUM2 :
    psmbcmdtscrfmt->type = NETWOX_SMBCMDTSCR_TYPE_NETSERVERENUM2;
    if (psmbcmdtscq->value.netserverenum2.vfmt == NETWOX_SMBCMDTSC_NETSERVERENUM2_VFMT_1) {
      psmbcmdtscrfmt->value.netserverenum2.vfmt = psmbcmdtscq->value.netserverenum2.vfmt;
      psmbcmdtscrfmt->value.netserverenum2.servernamelen = 16;
    } else {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscr_init(netwox_smbcmdtscr *psmbcmdtscr)
{
  psmbcmdtscr->type = NETWOX_SMBCMDTSCR_TYPE_UNKNOWN;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscr_selecttype(netwox_smbcmdtscr *psmbcmdtscr,
                                        netwox_smbcmdtscr_type type)
{

  if (psmbcmdtscr->type != NETWOX_SMBCMDTSCR_TYPE_UNKNOWN) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  psmbcmdtscr->type = type;
  switch(type) {
  case NETWOX_SMBCMDTSCR_TYPE_NETSHAREENUM :
    netwib_er(netwib_array_init(sizeof(netwox_smbcmdtscr_netshareenum_share),
                                0, &psmbcmdtscr->value.netshareenum.shares));
    break;
  case NETWOX_SMBCMDTSCR_TYPE_NETSHAREGETINFO :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmdtscr->value.netsharegetinfo.name));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmdtscr->value.netsharegetinfo.comment));
    break;
  case NETWOX_SMBCMDTSCR_TYPE_NETSERVERGETINFO :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmdtscr->value.netservergetinfo.name));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmdtscr->value.netservergetinfo.comment));
    break;
  case NETWOX_SMBCMDTSCR_TYPE_NETWKSTAGETINFO :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmdtscr->value.netwkstagetinfo.computername));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmdtscr->value.netwkstagetinfo.username));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmdtscr->value.netwkstagetinfo.domain));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmdtscr->value.netwkstagetinfo.logondomain));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmdtscr->value.netwkstagetinfo.otherdomains));
    break;
  case NETWOX_SMBCMDTSCR_TYPE_NETSERVERENUM2 :
    netwib_er(netwib_array_init(sizeof(netwox_smbcmdtscr_netserverenum2_server),
                              0, &psmbcmdtscr->value.netserverenum2.servers));
    break;
  default :
    psmbcmdtscr->type = NETWOX_SMBCMDTSCR_TYPE_UNKNOWN;
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscr_setdefault(netwox_smbcmdtscr *psmbcmdtscr)
{
  netwox_smbcmdtscr_netshareenum_share *pitemshare;
  netwox_smbcmdtscr_netserverenum2_server *pitemserver;
  netwib_uint32 i;

  switch(psmbcmdtscr->type) {
  case NETWOX_SMBCMDTSCR_TYPE_NETSHAREENUM :
    psmbcmdtscr->value.netshareenum.status = 0;
    psmbcmdtscr->value.netshareenum.totalshares = 0;
    for (i = 0; i < psmbcmdtscr->value.netshareenum.shares.size; i++) {
      pitemshare = (netwox_smbcmdtscr_netshareenum_share*)psmbcmdtscr->value.netshareenum.shares.p[i];
      netwib_er(netwox_smbcmdtscr_netshareenum_share_close(pitemshare));
    }
    netwib_er(netwib_array_ctl_set_size(&psmbcmdtscr->value.netshareenum.shares, 0));
    break;
  case NETWOX_SMBCMDTSCR_TYPE_NETSHAREGETINFO :
    psmbcmdtscr->value.netsharegetinfo.status = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmdtscr->value.netsharegetinfo.name));
    psmbcmdtscr->value.netsharegetinfo.sharetype = NETWOX_SMBCMDCMN_SHARETYPE_DIR;
    psmbcmdtscr->value.netsharegetinfo.unknown = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmdtscr->value.netsharegetinfo.comment));
    break;
  case NETWOX_SMBCMDTSCR_TYPE_NETSERVERGETINFO :
    psmbcmdtscr->value.netservergetinfo.status = 0;
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmdtscr->value.netservergetinfo.name));
    psmbcmdtscr->value.netservergetinfo.majorversion = NETWOX_SMBCMDTSCR_MAJORVERSION_98;
    psmbcmdtscr->value.netservergetinfo.minorversion = NETWOX_SMBCMDTSCR_MINORVERSION_98;
    netwox__smbcmdcmn_servertype_initdefault(psmbcmdtscr->value.netservergetinfo.servertype);
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmdtscr->value.netservergetinfo.comment));
    break;
  case NETWOX_SMBCMDTSCR_TYPE_NETWKSTAGETINFO :
    psmbcmdtscr->value.netwkstagetinfo.status = 0;
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmdtscr->value.netwkstagetinfo.computername));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmdtscr->value.netwkstagetinfo.username));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmdtscr->value.netwkstagetinfo.domain));
    psmbcmdtscr->value.netwkstagetinfo.majorversion = 0;
    psmbcmdtscr->value.netwkstagetinfo.minorversion = 0;
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmdtscr->value.netwkstagetinfo.logondomain));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmdtscr->value.netwkstagetinfo.otherdomains));
    break;
  case NETWOX_SMBCMDTSCR_TYPE_NETSERVERENUM2 :
    psmbcmdtscr->value.netserverenum2.status = 0;
    psmbcmdtscr->value.netserverenum2.totalservers = 0;
    for (i = 0; i < psmbcmdtscr->value.netserverenum2.servers.size; i++) {
      pitemserver = (netwox_smbcmdtscr_netserverenum2_server*)psmbcmdtscr->value.netserverenum2.servers.p[i];
      netwib_er(netwox_smbcmdtscr_netserverenum2_server_close(pitemserver));
    }
    netwib_er(netwib_array_ctl_set_size(&psmbcmdtscr->value.netserverenum2.servers, 0));
    break;
  default :
    psmbcmdtscr->type = NETWOX_SMBCMDTSCR_TYPE_UNKNOWN;
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscr_close(netwox_smbcmdtscr *psmbcmdtscr)
{
  netwox_smbcmdtscr_netshareenum_share *pitemshare;
  netwox_smbcmdtscr_netserverenum2_server *pitemserver;
  netwib_uint32 i;

  switch(psmbcmdtscr->type) {
  case NETWOX_SMBCMDTSCR_TYPE_UNKNOWN :
    break;
  case NETWOX_SMBCMDTSCR_TYPE_NETSHAREENUM :
    for (i = 0; i < psmbcmdtscr->value.netshareenum.shares.size; i++) {
      pitemshare = (netwox_smbcmdtscr_netshareenum_share*)psmbcmdtscr->value.netshareenum.shares.p[i];
      netwib_er(netwox_smbcmdtscr_netshareenum_share_close(pitemshare));
    }
    netwib_er(netwib_array_close(&psmbcmdtscr->value.netshareenum.shares));
    break;
  case NETWOX_SMBCMDTSCR_TYPE_NETSHAREGETINFO :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmdtscr->value.netsharegetinfo.name));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmdtscr->value.netsharegetinfo.comment));
    break;
  case NETWOX_SMBCMDTSCR_TYPE_NETSERVERGETINFO :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmdtscr->value.netservergetinfo.name));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmdtscr->value.netservergetinfo.comment));
    break;
  case NETWOX_SMBCMDTSCR_TYPE_NETWKSTAGETINFO :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmdtscr->value.netwkstagetinfo.computername));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmdtscr->value.netwkstagetinfo.username));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmdtscr->value.netwkstagetinfo.domain));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmdtscr->value.netwkstagetinfo.logondomain));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmdtscr->value.netwkstagetinfo.otherdomains));
    break;
  case NETWOX_SMBCMDTSCR_TYPE_NETSERVERENUM2 :
    for (i = 0; i < psmbcmdtscr->value.netserverenum2.servers.size; i++) {
      pitemserver = (netwox_smbcmdtscr_netserverenum2_server*)psmbcmdtscr->value.netserverenum2.servers.p[i];
      netwib_er(netwox_smbcmdtscr_netserverenum2_server_close(pitemserver));
    }
    netwib_er(netwib_array_close(&psmbcmdtscr->value.netserverenum2.servers));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcmdtscr_encode_name(netwib_data *pdata,
                                                netwib_uint32 *pdatasize,
                                                netwib_uint32 fieldsize,
                                                netwib_bufext *pbuf)
{
  netwib_data pc;
  netwib_er(netwox_smbcmdcmn_data_decodeext(pdata, pdatasize,
                                            fieldsize, pbuf));
  /* suppress ending zeroes in name */
  pc = netwib_c_memchr(netwib__buf_ref_data_ptr(pbuf), '\0',
                       netwib__buf_ref_data_size(pbuf));
  if (pc != NULL) {
    pbuf->endoffset = pc - pbuf->totalptr;
  } else {
    /* a zero must be present */
    return(NETWIB_ERR_NOTCONVERTED);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcmdtscr_encode_cmt(netwib_uint32 convert,
                                               netwib_uint32 txtoffset,
                                               netwib_data dataori,
                                               netwib_uint32 datasizeori,
                                               netwib_bufext *pbuf)
{
  netwib_data datatmp;
  netwib_uint32 datasizetmp;

  /* offset of comment is a bit strange to compute */
  if (txtoffset < convert) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  txtoffset = txtoffset - convert;
  if (txtoffset > datasizeori) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  /* we don't check for overlapping comments */
  datatmp = dataori + txtoffset;
  datasizetmp = datasizeori - txtoffset;
  netwib_er(netwox_smbcmdcmn_data_decodestr(&datatmp, &datasizetmp,
                                            NETWIB_FALSE,
                                            pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscr_encode_storager(netwox_constsmbcmdtscrstorage *pstorage,
                                    netwox_constsmbcmdtscrfmt *psmbcmdtscrfmt,
                                    netwox_smbcmdtscr *psmbcmdtscr)
{
  netwib_data para, data, dataori;
  netwib_uint32 parasize, datasize, datasizeori;
  netwib_uint32 convert, i, txtoffset;

  para = netwib__buf_ref_data_ptr(&pstorage->para);
  parasize = netwib__buf_ref_data_size(&pstorage->para);
  data = netwib__buf_ref_data_ptr(&pstorage->data);
  dataori = data;
  datasize = netwib__buf_ref_data_size(&pstorage->data);
  datasizeori = datasize;

  switch(psmbcmdtscrfmt->type) {
  case NETWOX_SMBCMDTSCR_TYPE_NETSHAREENUM :
    {
      netwib_uint32 countshares, sharenamelen;
      netwib_er(netwox_smbcmdtscr_selecttype(psmbcmdtscr, psmbcmdtscrfmt->type));
      if (pstorage->setupcount) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      if (parasize < 8) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib__data_decode_uint16_le(para, psmbcmdtscr->value.netshareenum.status);
      netwib__data_decode_uint16_le(para, convert);
      netwib__data_decode_uint16_le(para, countshares);
      netwib__data_decode_uint16_le(para, psmbcmdtscr->value.netshareenum.totalshares);
      parasize -= 8;
      /* under Win98, para can contain up to 1024 bytes of garbage
        if (parasize) [
          return(NETWIB_ERR_NOTCONVERTED);
        ]
      */
      sharenamelen = psmbcmdtscrfmt->value.netshareenum.sharenamelen;
      for (i = 0; i < countshares; i++) {
        netwox_smbcmdtscr_netshareenum_share *pitem;
        netwib_er(netwib_array_ctl_set_size(&psmbcmdtscr->value.netshareenum.shares, i+1));
        pitem = (netwox_smbcmdtscr_netshareenum_share*)psmbcmdtscr->value.netshareenum.shares.p[i];
        netwib_er(netwox_smbcmdtscr_netshareenum_share_init(pitem));
        netwib_er(netwox_smbcmdtscr_encode_name(&data, &datasize, sharenamelen, &pitem->name));
        if (datasize < 6) {
          return(NETWIB_ERR_DATAMISSING);
        }
        netwox__smbcmdcmn_sharetype_decode(data, pitem->sharetype);
        netwib__data_decode_uint16_le(data, txtoffset);
        netwib_er(netwox_smbcmdtscr_encode_cmt(convert, txtoffset,
                                               dataori, datasizeori,
                                               &pitem->comment));
        netwib__data_decode_uint16_le(data, pitem->unknown);
        datasize -= 6;
      }
      /* no check on datasize because it contains comments */
    }
    break;
  case NETWOX_SMBCMDTSCR_TYPE_NETSHAREGETINFO :
    {
      netwib_uint16 availablebytes;
      netwib_er(netwox_smbcmdtscr_selecttype(psmbcmdtscr, psmbcmdtscrfmt->type));
      if (pstorage->setupcount) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      if (parasize < 6) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib__data_decode_uint16_le(para, psmbcmdtscr->value.netsharegetinfo.status);
      netwib__data_decode_uint16_le(para, convert);
      netwib__data_decode_uint16_le(para, availablebytes);
      parasize -= 6;
      if (availablebytes != datasize) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      if (parasize) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      if (psmbcmdtscr->value.netsharegetinfo.status != 0 &&
          datasize == 0) {
        /* on error, values are unset */
        netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmdtscr->value.netsharegetinfo.name));
        psmbcmdtscr->value.netsharegetinfo.sharetype = NETWOX_SMBCMDCMN_SHARETYPE_DIR;
        psmbcmdtscr->value.netsharegetinfo.unknown = 0;
        netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmdtscr->value.netsharegetinfo.comment));
      } else {
        netwib_er(netwox_smbcmdtscr_encode_name(&data, &datasize, psmbcmdtscrfmt->value.netsharegetinfo.sharenamelen, &psmbcmdtscr->value.netsharegetinfo.name));
        if (datasize < 6) {
          return(NETWIB_ERR_DATAMISSING);
        }
        netwox__smbcmdcmn_sharetype_decode(data, psmbcmdtscr->value.netsharegetinfo.sharetype);
        netwib__data_decode_uint16_le(data, txtoffset);
        netwib_er(netwox_smbcmdtscr_encode_cmt(convert, txtoffset,
                                               dataori, datasizeori,
                                               &psmbcmdtscr->value.netsharegetinfo.comment));
        netwib__data_decode_uint16_le(data, psmbcmdtscr->value.netsharegetinfo.unknown);
        datasize -= 6;
        /* no check on datasize because it contains comments */
      }
    }
    break;
  case NETWOX_SMBCMDTSCR_TYPE_NETSERVERGETINFO :
    {
      netwib_uint16 availablebytes;
      netwib_er(netwox_smbcmdtscr_selecttype(psmbcmdtscr, psmbcmdtscrfmt->type));
      if (pstorage->setupcount) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      if (parasize < 6) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib__data_decode_uint16_le(para, psmbcmdtscr->value.netservergetinfo.status);
      netwib__data_decode_uint16_le(para, convert);
      netwib__data_decode_uint16_le(para, availablebytes);
      parasize -= 6;
      if (availablebytes != datasize) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      if (parasize) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib_er(netwox_smbcmdtscr_encode_name(&data, &datasize, psmbcmdtscrfmt->value.netservergetinfo.servernamelen, &psmbcmdtscr->value.netservergetinfo.name));
      if (datasize < 10) {
        return(NETWIB_ERR_DATAMISSING);
      }
      netwib__data_decode_uint8_le(data, psmbcmdtscr->value.netservergetinfo.majorversion);
      netwib__data_decode_uint8_le(data, psmbcmdtscr->value.netservergetinfo.minorversion);
      netwox__smbcmdcmn_servertype_decode(data, psmbcmdtscr->value.netservergetinfo.servertype);
      netwib__data_decode_uint16_le(data, txtoffset);
      netwib_er(netwox_smbcmdtscr_encode_cmt(convert, txtoffset,
                                             dataori, datasizeori,
                                             &psmbcmdtscr->value.netservergetinfo.comment));
      data += 2; /* unknown */
      datasize -= 10;
      /* no check on datasize because it contains comments */
    }
    break;
  case NETWOX_SMBCMDTSCR_TYPE_NETWKSTAGETINFO :
    {
      netwib_uint16 availablebytes;
      netwib_er(netwox_smbcmdtscr_selecttype(psmbcmdtscr, psmbcmdtscrfmt->type));
      if (pstorage->setupcount) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      if (parasize < 6) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib__data_decode_uint16_le(para, psmbcmdtscr->value.netwkstagetinfo.status);
      netwib__data_decode_uint16_le(para, convert);
      netwib__data_decode_uint16_le(para, availablebytes);
      parasize -= 6;
      if (availablebytes != datasize) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      if (parasize) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      if (datasize < 22) {
        return(NETWIB_ERR_DATAMISSING);
      }
      netwib__data_decode_uint16_le(data, txtoffset);
      netwib_er(netwox_smbcmdtscr_encode_cmt(convert, txtoffset,
                                             dataori, datasizeori,
                                             &psmbcmdtscr->value.netwkstagetinfo.computername));
      data += 2; /* unknown */
      netwib__data_decode_uint16_le(data, txtoffset);
      netwib_er(netwox_smbcmdtscr_encode_cmt(convert, txtoffset,
                                             dataori, datasizeori,
                                             &psmbcmdtscr->value.netwkstagetinfo.username));
      data += 2; /* unknown */
      netwib__data_decode_uint16_le(data, txtoffset);
      netwib_er(netwox_smbcmdtscr_encode_cmt(convert, txtoffset,
                                             dataori, datasizeori,
                                             &psmbcmdtscr->value.netwkstagetinfo.domain));
      data += 2; /* unknown */
      netwib__data_decode_uint8_le(data, psmbcmdtscr->value.netwkstagetinfo.majorversion);
      netwib__data_decode_uint8_le(data, psmbcmdtscr->value.netwkstagetinfo.minorversion);
      netwib__data_decode_uint16_le(data, txtoffset);
      netwib_er(netwox_smbcmdtscr_encode_cmt(convert, txtoffset,
                                             dataori, datasizeori,
                                             &psmbcmdtscr->value.netwkstagetinfo.logondomain));
      data += 2; /* unknown */
      netwib__data_decode_uint16_le(data, txtoffset);
      netwib_er(netwox_smbcmdtscr_encode_cmt(convert, txtoffset,
                                             dataori, datasizeori,
                                             &psmbcmdtscr->value.netwkstagetinfo.otherdomains));
      data += 2; /* unknown */
      datasize -= 22;
      /* no check on datasize because it contains comments */
    }
    break;
  case NETWOX_SMBCMDTSCR_TYPE_NETSERVERENUM2 :
    {
      netwib_uint32 countservers, servernamelen;
      netwib_er(netwox_smbcmdtscr_selecttype(psmbcmdtscr, psmbcmdtscrfmt->type));
      if (pstorage->setupcount) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      if (parasize < 8) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib__data_decode_uint16_le(para, psmbcmdtscr->value.netserverenum2.status);
      netwib__data_decode_uint16_le(para, convert);
      netwib__data_decode_uint16_le(para, countservers);
      netwib__data_decode_uint16_le(para, psmbcmdtscr->value.netserverenum2.totalservers);
      parasize -= 8;
      if (parasize) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      servernamelen = psmbcmdtscrfmt->value.netserverenum2.servernamelen;
      for (i = 0; i < countservers; i++) {
        netwox_smbcmdtscr_netserverenum2_server *pitem;
        netwib_er(netwib_array_ctl_set_size(&psmbcmdtscr->value.netserverenum2.servers, i+1));
        pitem = (netwox_smbcmdtscr_netserverenum2_server*)psmbcmdtscr->value.netserverenum2.servers.p[i];
        netwib_er(netwox_smbcmdtscr_netserverenum2_server_init(pitem));
        netwib_er(netwox_smbcmdtscr_encode_name(&data, &datasize, servernamelen, &pitem->name));
        if (datasize < 10) {
          return(NETWIB_ERR_DATAMISSING);
        }
        netwib__data_decode_uint8_le(data, pitem->majorversion);
        netwib__data_decode_uint8_le(data, pitem->minorversion);
        netwox__smbcmdcmn_servertype_decode(data, pitem->servertype);
        netwib__data_decode_uint32_le(data, txtoffset);/*32 bit not 16*/
        netwib_er(netwox_smbcmdtscr_encode_cmt(convert, txtoffset,
                                               dataori, datasizeori,
                                               &pitem->comment));
        datasize -= 10;
      }
      /* no check on datasize because it contains comments */
    }
    break;
  default :
    return(NETWIB_ERR_NOTCONVERTED);
  }


  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcmdtscr_decode_name(netwib_data *pdata,
                                                netwib_uint32 fieldsize,
                                                netwib_constbuf *pbuf)
{
  netwib_c_memset((*pdata), 0, fieldsize);
  if (netwib__buf_ref_data_size(pbuf) > fieldsize-1) {
    return(NETWIB_ERR_PATOOHIGH);
  }
  netwib_c_memcpy((*pdata), netwib__buf_ref_data_ptr(pbuf),
                  netwib__buf_ref_data_size(pbuf));
  (*pdata) += fieldsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcmdtscr_encode_cmt1(netwib_bool use32,
                                                netwib_uint32 convert,
                                                netwib_uint32 *ptxtoffset,
                                                netwib_data *pdata,
                                                netwib_constbuf *pbufcmt)
{
  if (use32) {
    netwib__data_append_uint32_le((*pdata), convert+(*ptxtoffset));
  } else {
    netwib__data_append_uint16_le((*pdata), convert+(*ptxtoffset));
  }
  *ptxtoffset += netwib__buf_ref_data_size(pbufcmt) + 1/*for '\0'*/;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcmdtscr_encode_cmt2(netwib_constbuf *pbufcmt,
                                                netwib_buf *pbufstorage)
{
  netwib_er(netwib_buf_append_buf(pbufcmt, pbufstorage));
  netwib_er(netwib_buf_append_byte('\0', pbufstorage));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscr_decode_storager(netwox_constsmbcmdtscr *psmbcmdtscr,
                                    netwox_constsmbcmdtscrfmt *psmbcmdtscrfmt,
                                    netwox_smbcmdtscrstorage *pstorage)
{
  netwib_data para, data;
  netwib_uint32 convert, txtoffset, i, itemsize;

  switch(psmbcmdtscr->type) {
  case NETWOX_SMBCMDTSCR_TYPE_NETSHAREENUM :
    {
      netwib_uint32 sharenamelen;
      convert = 0; /* 0 should be ok */
      netwib_er(netwib_buf_wantspace(&pstorage->para, 8, &para));
      netwib__data_append_uint16_le(para, psmbcmdtscr->value.netshareenum.status);
      netwib__data_append_uint16_le(para, convert);
      netwib__data_append_uint16_le(para, psmbcmdtscr->value.netshareenum.shares.size);
      netwib__data_append_uint16_le(para, psmbcmdtscr->value.netshareenum.totalshares);
      pstorage->para.endoffset += 8;
      sharenamelen = psmbcmdtscrfmt->value.netshareenum.sharenamelen;
      /* comments will start to be stored after share items */
      itemsize = sharenamelen + 6;
      txtoffset = psmbcmdtscr->value.netshareenum.shares.size * itemsize;
      /* add share items */
      for (i = 0; i < psmbcmdtscr->value.netshareenum.shares.size; i++) {
        netwox_smbcmdtscr_netshareenum_share *pitem;
        pitem = (netwox_smbcmdtscr_netshareenum_share*)psmbcmdtscr->value.netshareenum.shares.p[i];
        netwib_er(netwib_buf_wantspace(&pstorage->data, itemsize, &data));
        netwib_er(netwox_smbcmdtscr_decode_name(&data, sharenamelen, &pitem->name));
        netwox__smbcmdcmn_sharetype_append(data, pitem->sharetype);
        netwib_er(netwox_smbcmdtscr_encode_cmt1(NETWIB_FALSE, convert, &txtoffset, &data, &pitem->comment));
        netwib__data_append_uint16_le(data, pitem->unknown);
        pstorage->data.endoffset += itemsize;
      }
      /* add comments */
      for (i = 0; i < psmbcmdtscr->value.netshareenum.shares.size; i++) {
        netwox_smbcmdtscr_netshareenum_share *pitem;
        pitem = (netwox_smbcmdtscr_netshareenum_share*)psmbcmdtscr->value.netshareenum.shares.p[i];
        netwib_er(netwox_smbcmdtscr_encode_cmt2(&pitem->comment, &pstorage->data));
      }
    }
    break;
  case NETWOX_SMBCMDTSCR_TYPE_NETSHAREGETINFO :
    {
      netwib_uint32 availablebytes, sharenamelen;
      sharenamelen = psmbcmdtscrfmt->value.netsharegetinfo.sharenamelen;
      /* compute availablebytes */
      itemsize = sharenamelen + 6;
      availablebytes = itemsize;
      availablebytes += netwib__buf_ref_data_size(&psmbcmdtscr->value.netsharegetinfo.comment) + 1;
      /* construct packet */
      convert = 0; /* 0 should be ok */
      netwib_er(netwib_buf_wantspace(&pstorage->para, 6, &para));
      netwib__data_append_uint16_le(para, psmbcmdtscr->value.netsharegetinfo.status);
      netwib__data_append_uint16_le(para, convert);
      netwib__data_append_uint16_le(para, availablebytes);
      pstorage->para.endoffset += 6;
      /* comments will start to be stored after share item */
      txtoffset = itemsize;
      /* add share item */
      netwib_er(netwib_buf_wantspace(&pstorage->data, itemsize, &data));
      netwib_er(netwox_smbcmdtscr_decode_name(&data, sharenamelen, &psmbcmdtscr->value.netsharegetinfo.name));
      netwox__smbcmdcmn_sharetype_append(data, psmbcmdtscr->value.netsharegetinfo.sharetype);
      netwib_er(netwox_smbcmdtscr_encode_cmt1(NETWIB_FALSE, convert, &txtoffset, &data, &psmbcmdtscr->value.netsharegetinfo.comment));
      netwib__data_append_uint16_le(data, psmbcmdtscr->value.netsharegetinfo.unknown);
      pstorage->data.endoffset += itemsize;
      /* add comment */
      netwib_er(netwox_smbcmdtscr_encode_cmt2(&psmbcmdtscr->value.netsharegetinfo.comment, &pstorage->data));
    }
    break;
  case NETWOX_SMBCMDTSCR_TYPE_NETSERVERGETINFO :
    {
      netwib_uint32 availablebytes, servernamelen;
      servernamelen = psmbcmdtscrfmt->value.netservergetinfo.servernamelen;
      /* compute availablebytes */
      itemsize = servernamelen + 10;
      availablebytes = itemsize;
      availablebytes += netwib__buf_ref_data_size(&psmbcmdtscr->value.netservergetinfo.comment) + 1;
      /* construct packet */
      convert = 0; /* 0 should be ok */
      netwib_er(netwib_buf_wantspace(&pstorage->para, 6, &para));
      netwib__data_append_uint16_le(para, psmbcmdtscr->value.netservergetinfo.status);
      netwib__data_append_uint16_le(para, convert);
      netwib__data_append_uint16_le(para, availablebytes);
      pstorage->para.endoffset += 6;
      /* comments will start to be stored after server item */
      txtoffset = itemsize;
      /* add server item */
      netwib_er(netwib_buf_wantspace(&pstorage->data, itemsize, &data));
      netwib_er(netwox_smbcmdtscr_decode_name(&data, servernamelen, &psmbcmdtscr->value.netservergetinfo.name));
      netwib__data_append_uint8_le(data, psmbcmdtscr->value.netservergetinfo.majorversion);
      netwib__data_append_uint8_le(data, psmbcmdtscr->value.netservergetinfo.minorversion);
      netwox__smbcmdcmn_servertype_append(data, psmbcmdtscr->value.netservergetinfo.servertype);
      netwib_er(netwox_smbcmdtscr_encode_cmt1(NETWIB_FALSE, convert, &txtoffset, &data, &psmbcmdtscr->value.netservergetinfo.comment));
      netwib_c_memset(data, 0, 2); data += 2; /* unknown */
      pstorage->data.endoffset += itemsize;
      /* add comment */
      netwib_er(netwox_smbcmdtscr_encode_cmt2(&psmbcmdtscr->value.netservergetinfo.comment, &pstorage->data));
    }
    break;
  case NETWOX_SMBCMDTSCR_TYPE_NETWKSTAGETINFO :
    {
      netwib_uint32 availablebytes;
      /* compute availablebytes */
      itemsize = 22;
      availablebytes = itemsize;
      availablebytes += netwib__buf_ref_data_size(&psmbcmdtscr->value.netwkstagetinfo.computername) + 1;
      availablebytes += netwib__buf_ref_data_size(&psmbcmdtscr->value.netwkstagetinfo.username) + 1;
      availablebytes += netwib__buf_ref_data_size(&psmbcmdtscr->value.netwkstagetinfo.domain) + 1;
      availablebytes += netwib__buf_ref_data_size(&psmbcmdtscr->value.netwkstagetinfo.logondomain) + 1;
      availablebytes += netwib__buf_ref_data_size(&psmbcmdtscr->value.netwkstagetinfo.otherdomains) + 1;
      /* construct packet */
      convert = 0; /* 0 should be ok */
      netwib_er(netwib_buf_wantspace(&pstorage->para, 6, &para));
      netwib__data_append_uint16_le(para, psmbcmdtscr->value.netwkstagetinfo.status);
      netwib__data_append_uint16_le(para, convert);
      netwib__data_append_uint16_le(para, availablebytes);
      pstorage->para.endoffset += 6;
      /* comments will start to be stored after workstation item */
      txtoffset = itemsize;
      /* add workstation item */
      netwib_er(netwib_buf_wantspace(&pstorage->data, itemsize, &data));
      netwib_er(netwox_smbcmdtscr_encode_cmt1(NETWIB_FALSE, convert, &txtoffset, &data, &psmbcmdtscr->value.netwkstagetinfo.computername));
      netwib_c_memset(data, 0, 2); data += 2; /* unknown */
      netwib_er(netwox_smbcmdtscr_encode_cmt1(NETWIB_FALSE, convert, &txtoffset, &data, &psmbcmdtscr->value.netwkstagetinfo.username));
      netwib_c_memset(data, 0, 2); data += 2; /* unknown */
      netwib_er(netwox_smbcmdtscr_encode_cmt1(NETWIB_FALSE, convert, &txtoffset, &data, &psmbcmdtscr->value.netwkstagetinfo.domain));
      netwib_c_memset(data, 0, 2); data += 2; /* unknown */
      netwib__data_append_uint8_le(data, psmbcmdtscr->value.netwkstagetinfo.majorversion);
      netwib__data_append_uint8_le(data, psmbcmdtscr->value.netwkstagetinfo.minorversion);
      netwib_er(netwox_smbcmdtscr_encode_cmt1(NETWIB_FALSE, convert, &txtoffset, &data, &psmbcmdtscr->value.netwkstagetinfo.logondomain));
      netwib_c_memset(data, 0, 2); data += 2; /* unknown */
      netwib_er(netwox_smbcmdtscr_encode_cmt1(NETWIB_FALSE, convert, &txtoffset, &data, &psmbcmdtscr->value.netwkstagetinfo.otherdomains));
      netwib_c_memset(data, 0, 2); data += 2; /* unknown */
      pstorage->data.endoffset += itemsize;
      /* add comments */
      netwib_er(netwox_smbcmdtscr_encode_cmt2(&psmbcmdtscr->value.netwkstagetinfo.computername, &pstorage->data));
      netwib_er(netwox_smbcmdtscr_encode_cmt2(&psmbcmdtscr->value.netwkstagetinfo.username, &pstorage->data));
      netwib_er(netwox_smbcmdtscr_encode_cmt2(&psmbcmdtscr->value.netwkstagetinfo.domain, &pstorage->data));
      netwib_er(netwox_smbcmdtscr_encode_cmt2(&psmbcmdtscr->value.netwkstagetinfo.logondomain, &pstorage->data));
      netwib_er(netwox_smbcmdtscr_encode_cmt2(&psmbcmdtscr->value.netwkstagetinfo.otherdomains, &pstorage->data));
    }
    break;
  case NETWOX_SMBCMDTSCR_TYPE_NETSERVERENUM2 :
    {
      netwib_uint32 servernamelen;
      convert = 0; /* 0 should be ok */
      netwib_er(netwib_buf_wantspace(&pstorage->para, 8, &para));
      netwib__data_append_uint16_le(para, psmbcmdtscr->value.netserverenum2.status);
      netwib__data_append_uint16_le(para, convert);
      netwib__data_append_uint16_le(para, psmbcmdtscr->value.netserverenum2.servers.size);
      netwib__data_append_uint16_le(para, psmbcmdtscr->value.netserverenum2.totalservers);
      pstorage->para.endoffset += 8;
      servernamelen = psmbcmdtscrfmt->value.netserverenum2.servernamelen;
      /* comments will start to be stored after server items */
      itemsize = servernamelen + 10;
      txtoffset = psmbcmdtscr->value.netserverenum2.servers.size * itemsize;
      /* add server items */
      for (i = 0; i < psmbcmdtscr->value.netserverenum2.servers.size; i++) {
        netwox_smbcmdtscr_netserverenum2_server *pitem;
        pitem = (netwox_smbcmdtscr_netserverenum2_server*)psmbcmdtscr->value.netserverenum2.servers.p[i];
        netwib_er(netwib_buf_wantspace(&pstorage->data, itemsize, &data));
        netwib_er(netwox_smbcmdtscr_decode_name(&data, servernamelen, &pitem->name));
        netwib__data_append_uint8_le(data, pitem->majorversion);
        netwib__data_append_uint8_le(data, pitem->minorversion);
        netwox__smbcmdcmn_servertype_append(data, pitem->servertype);
        netwib_er(netwox_smbcmdtscr_encode_cmt1(NETWIB_TRUE, convert, &txtoffset, &data, &pitem->comment));
        pstorage->data.endoffset += itemsize;
      }
      /* add comments */
      for (i = 0; i < psmbcmdtscr->value.netserverenum2.servers.size; i++) {
        netwox_smbcmdtscr_netserverenum2_server *pitem;
        pitem = (netwox_smbcmdtscr_netserverenum2_server*)psmbcmdtscr->value.netserverenum2.servers.p[i];
        netwib_er(netwox_smbcmdtscr_encode_cmt2(&pitem->comment, &pstorage->data));
      }
    }
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscr_show(netwox_constsmbcmdtscr *psmbcmdtscr,
                                  netwib_buf *pbuf)
{
  netwib_uint32 i;

  switch(psmbcmdtscr->type) {
  case NETWOX_SMBCMDTSCR_TYPE_NETSHAREENUM :
    netwib_er(netwib_show_array_fmt32(pbuf, " Net Share Enum Reply"));
    netwib_er(netwib_show_array_fmt32(pbuf, " status=%{uint16}", psmbcmdtscr->value.netshareenum.status));
    netwib_er(netwib_show_array_fmt32(pbuf, " totalshares=%{uint16}", psmbcmdtscr->value.netshareenum.totalshares));
    for (i = 0; i < psmbcmdtscr->value.netshareenum.shares.size; i++) {
      netwox_smbcmdtscr_netshareenum_share *pitem;
      pitem = (netwox_smbcmdtscr_netshareenum_share*)psmbcmdtscr->value.netshareenum.shares.p[i];
      netwib_er(netwib_show_array_fmt32(pbuf, " share %{uint32}:", i));
      netwib_er(netwox_smbcmdcmn_data_show("  name", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI, &pitem->name, pbuf));
      netwib_er(netwox_smbcmdcmn_sharetype_show("  sharetype", pitem->sharetype, pbuf));
      netwib_er(netwib_show_array_fmt32(pbuf, "  unknown=%{uint16:#04X}", pitem->unknown));
      netwib_er(netwox_smbcmdcmn_data_show("  comment", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI, &pitem->comment, pbuf));
    }
    break;
  case NETWOX_SMBCMDTSCR_TYPE_NETSHAREGETINFO :
    netwib_er(netwib_show_array_fmt32(pbuf, " Net Share Get Info Reply"));
    netwib_er(netwib_show_array_fmt32(pbuf, " status=%{uint16}", psmbcmdtscr->value.netsharegetinfo.status));
    netwib_er(netwox_smbcmdcmn_data_show("  name", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI, &psmbcmdtscr->value.netsharegetinfo.name, pbuf));
    netwib_er(netwox_smbcmdcmn_sharetype_show("  sharetype", psmbcmdtscr->value.netsharegetinfo.sharetype, pbuf));
    netwib_er(netwib_show_array_fmt32(pbuf, "  unknown=%{uint16:#04X}", psmbcmdtscr->value.netsharegetinfo.unknown));
    netwib_er(netwox_smbcmdcmn_data_show("  comment", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI, &psmbcmdtscr->value.netsharegetinfo.comment, pbuf));
    break;
  case NETWOX_SMBCMDTSCR_TYPE_NETSERVERGETINFO :
    netwib_er(netwib_show_array_fmt32(pbuf, " Net Server Get Info Reply"));
    netwib_er(netwib_show_array_fmt32(pbuf, " status=%{uint16}", psmbcmdtscr->value.netservergetinfo.status));
    netwib_er(netwox_smbcmdcmn_data_show("  name", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI, &psmbcmdtscr->value.netservergetinfo.name, pbuf));
    netwib_er(netwib_show_array_fmt32(pbuf, "  majorversion=%{uint8}", psmbcmdtscr->value.netservergetinfo.majorversion));
    netwib_er(netwib_show_array_fmt32(pbuf, "  minorversion=%{uint8}", psmbcmdtscr->value.netservergetinfo.minorversion));
    netwib_er(netwox_smbcmdcmn_servertype_show("  servertype", psmbcmdtscr->value.netservergetinfo.servertype, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show("  comment", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI, &psmbcmdtscr->value.netservergetinfo.comment, pbuf));
    break;
  case NETWOX_SMBCMDTSCR_TYPE_NETWKSTAGETINFO :
    netwib_er(netwib_show_array_fmt32(pbuf, " Net Workstation Get Info Reply"));
    netwib_er(netwib_show_array_fmt32(pbuf, " status=%{uint16}", psmbcmdtscr->value.netwkstagetinfo.status));
    netwib_er(netwox_smbcmdcmn_data_show("  computername", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI, &psmbcmdtscr->value.netwkstagetinfo.computername, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show("  username", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI, &psmbcmdtscr->value.netwkstagetinfo.username, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show("  domain", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI, &psmbcmdtscr->value.netwkstagetinfo.domain, pbuf));
    netwib_er(netwib_show_array_fmt32(pbuf, "  majorversion=%{uint8}", psmbcmdtscr->value.netwkstagetinfo.majorversion));
    netwib_er(netwib_show_array_fmt32(pbuf, "  minorversion=%{uint8}", psmbcmdtscr->value.netwkstagetinfo.minorversion));
    netwib_er(netwox_smbcmdcmn_data_show("  logondomain", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI, &psmbcmdtscr->value.netwkstagetinfo.logondomain, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show("  otherdomains", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI, &psmbcmdtscr->value.netwkstagetinfo.otherdomains, pbuf));
    break;
  case NETWOX_SMBCMDTSCR_TYPE_NETSERVERENUM2 :
    netwib_er(netwib_show_array_fmt32(pbuf, " Net Server Enum2 Reply"));
    netwib_er(netwib_show_array_fmt32(pbuf, " status=%{uint16}", psmbcmdtscr->value.netserverenum2.status));
    netwib_er(netwib_show_array_fmt32(pbuf, " totalservers=%{uint16}", psmbcmdtscr->value.netserverenum2.totalservers));
    for (i = 0; i < psmbcmdtscr->value.netserverenum2.servers.size; i++) {
      netwox_smbcmdtscr_netserverenum2_server *pitem;
      pitem = (netwox_smbcmdtscr_netserverenum2_server*)psmbcmdtscr->value.netserverenum2.servers.p[i];
      netwib_er(netwib_show_array_fmt32(pbuf, " server %{uint32}:", i));
      netwib_er(netwox_smbcmdcmn_data_show("  name", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI, &pitem->name, pbuf));
      netwib_er(netwib_show_array_fmt32(pbuf, "  majorversion=%{uint8}", pitem->majorversion));
      netwib_er(netwib_show_array_fmt32(pbuf, "  minorversion=%{uint8}", pitem->minorversion));
      netwib_er(netwox_smbcmdcmn_servertype_show("  servertype", pitem->servertype, pbuf));
      netwib_er(netwox_smbcmdcmn_data_show("  comment", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI, &pitem->comment, pbuf));
    }
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscr_display(netwib_bufpool *pbufpool,
                                     netwox_constsmbcmdtscr *psmbcmdtscr)
{
  netwib_buf *pbuf;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));

  ret = netwox_smbcmdtscr_show(psmbcmdtscr, pbuf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_display(pbuf, NETWIB_ENCODETYPE_DATA));
  } else {
    netwib_er(netwib_fmt_display("This Transaction Reply could not be decoded\n"));
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(ret);
}
