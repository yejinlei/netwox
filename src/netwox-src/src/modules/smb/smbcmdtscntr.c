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
netwib_err netwox_smbcmdtscntr_ntnotify_item_init(netwox_smbcmdtscntr_ntnotify_item *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_initdefault(&pitem->filename));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntr_ntnotify_item_setdefault(netwox_smbcmdtscntr_ntnotify_item *pitem)
{
  pitem->action = 0;
  netwib_er(netwox_smbcmdcmn_data_setdefault(&pitem->filename));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntr_ntnotify_item_close(netwox_smbcmdtscntr_ntnotify_item *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_close(&pitem->filename));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcmdtscntr_ntnotify_item_decode(netwib_data *pdata,
                                                    netwib_uint32 *pdatasize,
                             netwox_smbcmdtscntr_ntnotify_item *pitem)
{
  netwib_data data;
  netwib_uint32 datasize, filenamesize;

  data = *pdata;
  datasize = *pdatasize;

  if (datasize < 8) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwib__data_decode_uint32_le(data, pitem->action);
  netwib__data_decode_uint32_le(data, filenamesize);
  datasize -= 8;

  netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, filenamesize,
                                            &pitem->filename));

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcmdtscntr_ntnotify_item_size(netwox_smbcmdtscntr_ntnotify_item *pitem,
                                                  netwib_uint32 *psize)
{
  *psize = 8 + netwib__buf_ref_data_size(&pitem->filename);
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcmdtscntr_ntnotify_item_append(const netwox_smbcmdtscntr_ntnotify_item *pitem,
                                                    netwib_buf *pbuf)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(pbuf, 8, &data));
  netwib__data_append_uint32_le(data, pitem->action);
  netwib__data_append_uint32_le(data, netwib__buf_ref_data_size(&pitem->filename));
  pbuf->endoffset += 8;
  netwib_er(netwib_buf_append_buf(&pitem->filename, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcmdtscntr_ntnotify_item_show(const netwox_smbcmdtscntr_ntnotify_item *pitem,
                                                         netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_fmt32(pbuf, "  action=%{uint32}", pitem->action));
  netwib_er(netwox_smbcmdcmn_data_show("  filename", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &pitem->filename, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntrstorage_init(netwib_bool isunicode,
                                         netwox_smbcmdtscntrstorage *pstorage)
{

  pstorage->isunicode = isunicode;
  pstorage->setupcount = 0;

  netwib_er(netwib_buf_init_ext_storage(&pstorage->para));
  netwib_er(netwib_buf_init_ext_storage(&pstorage->data));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntrstorage_close(netwox_smbcmdtscntrstorage *pstorage)
{
  netwib_er(netwib_buf_close(&pstorage->para));
  netwib_er(netwib_buf_close(&pstorage->data));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntrstorage_append_smbcmd(netwox_constsmbcmd *psmbcmd,
                                          netwox_smbcmdtscntrstorage *pstorage,
                                                    netwib_bool *pneedmoremsg)
{
  netwib_uint32 i;
  netwib_bool isfirst, isalone;

  if (psmbcmd->type != NETWOX_SMBCMD_TYPE_NTTRANSACT_R) {
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  isfirst = NETWIB_TRUE;
  if (psmbcmd->value.nttransact_r.paradisplacement ||
      psmbcmd->value.nttransact_r.datadisplacement) {
    isfirst = NETWIB_FALSE;
  }
  isalone = NETWIB_TRUE;
  if ( (netwib__buf_ref_data_size(&psmbcmd->value.nttransact_r.para) !=
        psmbcmd->value.nttransact_r.totalparasize) ||
       (netwib__buf_ref_data_size(&psmbcmd->value.nttransact_r.data) !=
        psmbcmd->value.nttransact_r.totaldatasize) ) {
    isalone = NETWIB_FALSE;
  }
  if ( (netwib__buf_ref_data_size(&psmbcmd->value.nttransact_r.para) >
        psmbcmd->value.nttransact_r.totalparasize) ||
       (netwib__buf_ref_data_size(&psmbcmd->value.nttransact_r.data) >
        psmbcmd->value.nttransact_r.totaldatasize) ) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  if (isfirst) {
    if (psmbcmd->value.nttransact_r.setupcount > NETWOX_SMBCMD_NTTRANSACT_SETUP_MAX) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    pstorage->setupcount = psmbcmd->value.nttransact_r.setupcount;
    for (i = 0; i < pstorage->setupcount; i++) {
      pstorage->setup[i] = psmbcmd->value.nttransact_r.setup[i];
    }
  } else {
    if (psmbcmd->value.nttransact_r.setupcount != 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  if (isalone) {
    netwib_er(netwib_buf_init_ext_buf(&psmbcmd->value.nttransact_r.para, &pstorage->para));
    netwib_er(netwib_buf_init_ext_buf(&psmbcmd->value.nttransact_r.data, &pstorage->data));
    *pneedmoremsg = NETWIB_FALSE;
  } else {
    if (netwib__buf_ref_data_size(&psmbcmd->value.nttransact_r.para)) {
      /* if size of psmbcmd->value.nttransact_r.para is zero, we can't
         do this check under WinME, because paradisplacement is incorrectly
         set to zero. */
      if (psmbcmd->value.nttransact_r.paradisplacement !=
          netwib__buf_ref_data_size(&pstorage->para)) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
    }
    if (netwib__buf_ref_data_size(&psmbcmd->value.nttransact_r.data)) {
      /* if size of psmbcmd->value.nttransact_r.data is zero, we can't
         do this check under WinME, because datadisplacement is incorrectly
         set to zero. */
      if (psmbcmd->value.nttransact_r.datadisplacement !=
          netwib__buf_ref_data_size(&pstorage->data)) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
    }
    netwib_er(netwib_buf_append_buf(&psmbcmd->value.nttransact_r.para, &pstorage->para));
    netwib_er(netwib_buf_append_buf(&psmbcmd->value.nttransact_r.data, &pstorage->data));
    if ( (netwib__buf_ref_data_size(&pstorage->para) >
          psmbcmd->value.nttransact_r.totalparasize) ||
         (netwib__buf_ref_data_size(&pstorage->data) >
          psmbcmd->value.nttransact_r.totaldatasize) ) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    *pneedmoremsg = NETWIB_FALSE;
    if ( (netwib__buf_ref_data_size(&pstorage->para) !=
          psmbcmd->value.nttransact_r.totalparasize) ||
         (netwib__buf_ref_data_size(&pstorage->data) !=
          psmbcmd->value.nttransact_r.totaldatasize) ) {
      *pneedmoremsg = NETWIB_TRUE;
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntrstorage_decode_smbcmd(netwox_constsmbcmdtscntrstorage *pstorage,
                                                    netwib_uint32 *poffset,
                                                    netwib_uint32 maxsize,
                                                    netwox_smbcmd *psmbcmd,
                                                    netwib_bool *pneedmoremsg)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize, i, threshold1, threshold2, storedsize;

  *pneedmoremsg = NETWIB_FALSE;

  psmbcmd->value.nttransact_r.totalparasize = (netwib_uint16)netwib__buf_ref_data_size(&pstorage->para);
  psmbcmd->value.nttransact_r.totaldatasize = (netwib_uint16)netwib__buf_ref_data_size(&pstorage->data);
  psmbcmd->value.nttransact_r.paradisplacement = 0;
  psmbcmd->value.nttransact_r.datadisplacement = 0;
  netwib_er(netwib_buf_init_ext_empty(&psmbcmd->value.nttransact_r.para));
  netwib_er(netwib_buf_init_ext_empty(&psmbcmd->value.nttransact_r.data));

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
      if (pstorage->setupcount > NETWOX_SMBCMD_NTTRANSACT_SETUP_MAX) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      if (maxsize < (netwib_uint32)(pstorage->setupcount*2)) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      psmbcmd->value.nttransact_r.setupcount = pstorage->setupcount;
      for (i = 0; i < pstorage->setupcount; i++) {
        psmbcmd->value.nttransact_r.setup[i] = pstorage->setup[i];
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
      psmbcmd->value.nttransact_r.paradisplacement = (netwib_uint16)(*poffset - threshold1);
      netwib_er(netwib_buf_init_ext_arrayfilled(para, parasize, &psmbcmd->value.nttransact_r.para));
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
    psmbcmd->value.nttransact_r.datadisplacement = (netwib_uint16)(*poffset - threshold2);
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, &psmbcmd->value.nttransact_r.data));
    *poffset += datasize;
    storedsize += datasize;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntrfmt_init_smbcmdtscntq(netwox_constsmbcmdtscntq *psmbcmdtscntq,
                                                  netwox_smbcmdtscntrfmt *psmbcmdtscntrfmt)
{

  switch(psmbcmdtscntq->type) {
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTIOCTL :
    psmbcmdtscntrfmt->type = NETWOX_SMBCMDTSCNTR_TYPE_NTIOCTL;
    break;
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTNOTIFY :
    psmbcmdtscntrfmt->type = NETWOX_SMBCMDTSCNTR_TYPE_NTNOTIFY;
    break;
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTQUERYSECUDESC :
    psmbcmdtscntrfmt->type = NETWOX_SMBCMDTSCNTR_TYPE_NTQUERYSECUDESC;
    break;
  case NETWOX_SMBCMDTSCNTQ_TYPE_NTSETSECUDESC :
    psmbcmdtscntrfmt->type = NETWOX_SMBCMDTSCNTR_TYPE_NTSETSECUDESC;
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntr_init(netwox_smbcmdtscntr *psmbcmdtscntr)
{
  psmbcmdtscntr->type = NETWOX_SMBCMDTSCNTR_TYPE_UNKNOWN;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntr_selecttype(netwox_smbcmdtscntr *psmbcmdtscntr,
                                         netwox_smbcmdtscntr_type type)
{

  if (psmbcmdtscntr->type != NETWOX_SMBCMDTSCNTR_TYPE_UNKNOWN) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  psmbcmdtscntr->type = type;
  switch(type) {
  case NETWOX_SMBCMDTSCNTR_TYPE_NTIOCTL :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmdtscntr->value.ntioctl.data));
    break;
  case NETWOX_SMBCMDTSCNTR_TYPE_NTNOTIFY :
    netwib_er(netwib_array_init(sizeof(netwox_smbcmdtscntr_ntnotify_item), 0, &psmbcmdtscntr->value.ntnotify.infos));
    break;
  case NETWOX_SMBCMDTSCNTR_TYPE_NTQUERYSECUDESC :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmdtscntr->value.ntquerysecudesc.secudesc));
    break;
  case NETWOX_SMBCMDTSCNTR_TYPE_NTSETSECUDESC :
    break;
  default :
    psmbcmdtscntr->type = NETWOX_SMBCMDTSCQ_TYPE_UNKNOWN;
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntr_setdefault(netwox_smbcmdtscntr *psmbcmdtscntr)
{
  netwib_uint32 i;

  switch(psmbcmdtscntr->type) {
  case NETWOX_SMBCMDTSCNTR_TYPE_NTIOCTL :
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmdtscntr->value.ntioctl.data));
    break;
  case NETWOX_SMBCMDTSCNTR_TYPE_NTNOTIFY :
    {
      netwox_smbcmdtscntr_ntnotify_item *pitem;
      for (i = 0; i < psmbcmdtscntr->value.ntnotify.infos.size; i++) {
        pitem = (netwox_smbcmdtscntr_ntnotify_item*)psmbcmdtscntr->value.ntnotify.infos.p[i];
        netwib_er(netwox_smbcmdtscntr_ntnotify_item_close(pitem));
      }
    }
    netwib_er(netwib_array_ctl_set_size(&psmbcmdtscntr->value.ntnotify.infos, 0));
    break;
  case NETWOX_SMBCMDTSCNTR_TYPE_NTQUERYSECUDESC :
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmdtscntr->value.ntquerysecudesc.secudesc));
    break;
  case NETWOX_SMBCMDTSCNTR_TYPE_NTSETSECUDESC :
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntr_close(netwox_smbcmdtscntr *psmbcmdtscntr)
{
  netwib_uint32 i;

  switch(psmbcmdtscntr->type) {
  case NETWOX_SMBCMDTSCNTR_TYPE_UNKNOWN :
    break;
  case NETWOX_SMBCMDTSCNTR_TYPE_NTIOCTL :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmdtscntr->value.ntioctl.data));
    break;
  case NETWOX_SMBCMDTSCNTR_TYPE_NTNOTIFY :
    {
      netwox_smbcmdtscntr_ntnotify_item *pitem;
      for (i = 0; i < psmbcmdtscntr->value.ntnotify.infos.size; i++) {
        pitem = (netwox_smbcmdtscntr_ntnotify_item*)psmbcmdtscntr->value.ntnotify.infos.p[i];
        netwib_er(netwox_smbcmdtscntr_ntnotify_item_close(pitem));
      }
      netwib_er(netwib_array_close(&psmbcmdtscntr->value.ntnotify.infos));
    }
    break;
  case NETWOX_SMBCMDTSCNTR_TYPE_NTQUERYSECUDESC :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmdtscntr->value.ntquerysecudesc.secudesc));
    break;
  case NETWOX_SMBCMDTSCNTR_TYPE_NTSETSECUDESC :
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntr_encode_storager(netwox_constsmbcmdtscntrstorage *pstorage,
                                  netwox_constsmbcmdtscntrfmt *psmbcmdtscntrfmt,
                                  netwox_smbcmdtscntr *psmbcmdtscntr)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize;
  netwib_uint32 i, pktdatasize;

  para = netwib__buf_ref_data_ptr(&pstorage->para);
  parasize = netwib__buf_ref_data_size(&pstorage->para);
  data = netwib__buf_ref_data_ptr(&pstorage->data);
  datasize = netwib__buf_ref_data_size(&pstorage->data);

  netwib_er(netwox_smbcmdtscntr_selecttype(psmbcmdtscntr, psmbcmdtscntrfmt->type));

  switch(psmbcmdtscntrfmt->type) {
  case NETWOX_SMBCMDTSCNTR_TYPE_NTIOCTL :
    if (pstorage->setupcount == 0) {
      /* Samba 3 does not set a setup: simulate a nul unknown */
      pktdatasize = 0;
    } else if (pstorage->setupcount == 1) {
      pktdatasize = pstorage->setup[0];
    } else {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize != 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize != pktdatasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, pktdatasize, &psmbcmdtscntr->value.ntioctl.data));
    break;
  case NETWOX_SMBCMDTSCNTR_TYPE_NTNOTIFY :
    if (pstorage->setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize != 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize != 0) {
      netwib_data parainfo;
      netwib_uint32 parainfosize, nextentryoffset;
      i = 0;
      while(NETWIB_TRUE) {
        netwox_smbcmdtscntr_ntnotify_item *pitem;
        netwib_er(netwib_array_ctl_set_size(&psmbcmdtscntr->value.ntnotify.infos, i+1));
        pitem = (netwox_smbcmdtscntr_ntnotify_item*)psmbcmdtscntr->value.ntnotify.infos.p[i];
        netwib_er(netwox_smbcmdtscntr_ntnotify_item_init(pitem));
        if (parasize < 4) {
          return(NETWIB_ERR_DATAMISSING);
        }
        parainfo = para;
        netwib__data_decode_uint32_le(parainfo, nextentryoffset);
        if (nextentryoffset == 0) {
          parainfosize = parasize - 4;
        } else {
          if (nextentryoffset > parasize) {
            return(NETWIB_ERR_DATAMISSING);
          }
          parainfosize = nextentryoffset - 4;
        }
        netwib_er(netwox_smbcmdtscntr_ntnotify_item_decode(&parainfo, &parainfosize, pitem));
        /* next entry */
        if (nextentryoffset == 0) {
          para = parainfo;
          parasize = parainfosize;
          break;
        } else {
          para += nextentryoffset;
          parasize -= nextentryoffset;
          if (parainfosize/* too many padding */) {
            return(NETWIB_ERR_NOTCONVERTED);
          }
        }
        i++;
      }
      if (parasize > 2/* too many padding */) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
    }
    break;
  case NETWOX_SMBCMDTSCNTR_TYPE_NTQUERYSECUDESC :
    {
      netwib_uint32 secudescsize;
      if (pstorage->setupcount) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      if (parasize != 4) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib__data_decode_uint16_le(para, secudescsize);
      netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, secudescsize, &psmbcmdtscntr->value.ntquerysecudesc.secudesc));
      if (datasize != 0) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
    }
    break;
  case NETWOX_SMBCMDTSCNTR_TYPE_NTSETSECUDESC :
    if (pstorage->setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize != 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize != 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  default :
    return(NETWIB_ERR_NOTCONVERTED);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntr_decode_storager(netwox_constsmbcmdtscntr *psmbcmdtscntr,
                                  netwox_constsmbcmdtscntrfmt *psmbcmdtscntrfmt,
                                  netwox_smbcmdtscntrstorage *pstorage)
{
  netwib_data para;

  if (psmbcmdtscntrfmt->type != psmbcmdtscntr->type) {
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  switch(psmbcmdtscntr->type) {
  case NETWOX_SMBCMDTSCNTR_TYPE_NTIOCTL :
    pstorage->setupcount = 1;
    pstorage->setup[0] = (netwib_uint16)netwib__buf_ref_data_size(&psmbcmdtscntr->value.ntioctl.data);
    netwib_er(netwib_buf_append_buf(&psmbcmdtscntr->value.ntioctl.data, &pstorage->data));
    break;
  case NETWOX_SMBCMDTSCNTR_TYPE_NTNOTIFY :
    {
      netwib_uint32 i, nextentryoffset, itemsize;
      for (i = 0; i < psmbcmdtscntr->value.ntnotify.infos.size; i++) {
        netwox_smbcmdtscntr_ntnotify_item *pitem;
        pitem = (netwox_smbcmdtscntr_ntnotify_item*)psmbcmdtscntr->value.ntnotify.infos.p[i];
        netwib_er(netwib_buf_wantspace(&pstorage->para, 4, &para));
        if (i+1 == psmbcmdtscntr->value.ntnotify.infos.size) {
          nextentryoffset = 0;
        } else {
          netwib_er(netwox_smbcmdtscntr_ntnotify_item_size(pitem, &itemsize));
          nextentryoffset = 4 + itemsize;
        }
        netwib__data_append_uint32_le(para, nextentryoffset);
        pstorage->para.endoffset += 4;
        netwib_er(netwox_smbcmdtscntr_ntnotify_item_append(pitem, &pstorage->para));
      }
    }
    break;
  case NETWOX_SMBCMDTSCNTR_TYPE_NTQUERYSECUDESC :
    netwib_er(netwib_buf_wantspace(&pstorage->para, 4, &para));
    netwib__data_append_uint32_le(para, netwib__buf_ref_data_size(&psmbcmdtscntr->value.ntquerysecudesc.secudesc));
    pstorage->para.endoffset += 4;
    netwib_er(netwib_buf_append_buf(&psmbcmdtscntr->value.ntquerysecudesc.secudesc, &pstorage->data));
    break;
  case NETWOX_SMBCMDTSCNTR_TYPE_NTSETSECUDESC :
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntr_show(netwox_constsmbcmdtscntr *psmbcmdtscntr,
                                  netwib_buf *pbuf)
{
  netwib_uint32 i;

  switch(psmbcmdtscntr->type) {
  case NETWOX_SMBCMDTSCNTR_TYPE_NTIOCTL :
    netwib_er(netwib_show_array_fmt32(pbuf, " NTIOCTL Reply"));
    netwib_er(netwox_smbcmdcmn_data_show("  data", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmdtscntr->value.ntioctl.data, pbuf));
    break;
  case NETWOX_SMBCMDTSCNTR_TYPE_NTNOTIFY :
    netwib_er(netwib_show_array_fmt32(pbuf, " NTNOTIFY Reply"));
    for (i = 0; i < psmbcmdtscntr->value.ntnotify.infos.size; i++) {
      netwox_smbcmdtscntr_ntnotify_item *pitem;
      pitem = (netwox_smbcmdtscntr_ntnotify_item*)psmbcmdtscntr->value.ntnotify.infos.p[i];
      netwib_er(netwox_smbcmdtscntr_ntnotify_item_show(pitem, pbuf));
    }
    break;
  case NETWOX_SMBCMDTSCNTR_TYPE_NTQUERYSECUDESC :
    netwib_er(netwib_show_array_fmt32(pbuf, " NTQUERYSECUDESC Reply"));
    netwib_er(netwox_smbcmdcmn_data_show("  secudesc", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmdtscntr->value.ntquerysecudesc.secudesc, pbuf));
    break;
  case NETWOX_SMBCMDTSCNTR_TYPE_NTSETSECUDESC :
    netwib_er(netwib_show_array_fmt32(pbuf, " NTSETSECUDESC Reply"));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtscntr_display(netwib_bufpool *pbufpool,
                                       netwox_constsmbcmdtscntr *psmbcmdtscntr)
{
  netwib_buf *pbuf;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));

  ret = netwox_smbcmdtscntr_show(psmbcmdtscntr, pbuf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_display(pbuf, NETWIB_ENCODETYPE_DATA));
  } else {
    netwib_er(netwib_fmt_display("This TransactionNT Reply could not be decoded\n"));
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(ret);
}
