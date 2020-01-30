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
netwib_err netwox_smbcmdtsc2rstorage_init(netwib_bool isunicode,
                                          netwox_smbcmdtsc2rstorage *pstorage)
{

  pstorage->isunicode = isunicode;
  pstorage->setupcount = 0;

  netwib_er(netwib_buf_init_ext_storage(&pstorage->para));
  netwib_er(netwib_buf_init_ext_storage(&pstorage->data));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2rstorage_close(netwox_smbcmdtsc2rstorage *pstorage)
{
  netwib_er(netwib_buf_close(&pstorage->para));
  netwib_er(netwib_buf_close(&pstorage->data));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2rstorage_append_smbcmd(netwox_constsmbcmd *psmbcmd,
                                          netwox_smbcmdtsc2rstorage *pstorage,
                                                   netwib_bool *pneedmoremsg)
{
  netwib_uint32 i;
  netwib_bool isfirst, isalone;

  if (psmbcmd->type != NETWOX_SMBCMD_TYPE_TRANSACTION2_R) {
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  isfirst = NETWIB_TRUE;
  if (psmbcmd->value.transaction2_r.paradisplacement ||
      psmbcmd->value.transaction2_r.datadisplacement) {
    isfirst = NETWIB_FALSE;
  }
  isalone = NETWIB_TRUE;
  if ( (netwib__buf_ref_data_size(&psmbcmd->value.transaction2_r.para) !=
        psmbcmd->value.transaction2_r.totalparasize) ||
       (netwib__buf_ref_data_size(&psmbcmd->value.transaction2_r.data) !=
        psmbcmd->value.transaction2_r.totaldatasize) ) {
    isalone = NETWIB_FALSE;
  }
  if ( (netwib__buf_ref_data_size(&psmbcmd->value.transaction2_r.para) >
        psmbcmd->value.transaction2_r.totalparasize) ||
       (netwib__buf_ref_data_size(&psmbcmd->value.transaction2_r.data) >
        psmbcmd->value.transaction2_r.totaldatasize) ) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  if (isfirst) {
    if (psmbcmd->value.transaction2_r.setupcount > NETWOX_SMBCMD_TRANSACTION2_SETUP_MAX) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    pstorage->setupcount = psmbcmd->value.transaction2_r.setupcount;
    for (i = 0; i < pstorage->setupcount; i++) {
      pstorage->setup[i] = psmbcmd->value.transaction2_r.setup[i];
    }
  } else {
    if (psmbcmd->value.transaction2_r.setupcount != 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  if (isalone) {
    netwib_er(netwib_buf_init_ext_buf(&psmbcmd->value.transaction2_r.para, &pstorage->para));
    netwib_er(netwib_buf_init_ext_buf(&psmbcmd->value.transaction2_r.data, &pstorage->data));
    *pneedmoremsg = NETWIB_FALSE;
  } else {
    if (netwib__buf_ref_data_size(&psmbcmd->value.transaction2_r.para)) {
      /* if size of psmbcmd->value.transaction2_r.para is zero, we can't
         do this check under WinME, because paradisplacement is incorrectly
         set to zero. */
      if (psmbcmd->value.transaction2_r.paradisplacement !=
          netwib__buf_ref_data_size(&pstorage->para)) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
    }
    if (netwib__buf_ref_data_size(&psmbcmd->value.transaction2_r.data)) {
      /* if size of psmbcmd->value.transaction2_r.data is zero, we can't
         do this check under WinME, because datadisplacement is incorrectly
         set to zero. */
      if (psmbcmd->value.transaction2_r.datadisplacement !=
          netwib__buf_ref_data_size(&pstorage->data)) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
    }
    netwib_er(netwib_buf_append_buf(&psmbcmd->value.transaction2_r.para, &pstorage->para));
    netwib_er(netwib_buf_append_buf(&psmbcmd->value.transaction2_r.data, &pstorage->data));
    if ( (netwib__buf_ref_data_size(&pstorage->para) >
          psmbcmd->value.transaction2_r.totalparasize) ||
         (netwib__buf_ref_data_size(&pstorage->data) >
          psmbcmd->value.transaction2_r.totaldatasize) ) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    *pneedmoremsg = NETWIB_FALSE;
    if ( (netwib__buf_ref_data_size(&pstorage->para) !=
          psmbcmd->value.transaction2_r.totalparasize) ||
         (netwib__buf_ref_data_size(&pstorage->data) !=
          psmbcmd->value.transaction2_r.totaldatasize) ) {
      *pneedmoremsg = NETWIB_TRUE;
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2rstorage_decode_smbcmd(netwox_constsmbcmdtsc2rstorage *pstorage,
                                                   netwib_uint32 *poffset,
                                                   netwib_uint32 maxsize,
                                                   netwox_smbcmd *psmbcmd,
                                                   netwib_bool *pneedmoremsg)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize, i, threshold1, threshold2, storedsize;

  *pneedmoremsg = NETWIB_FALSE;

  psmbcmd->value.transaction2_r.totalparasize = (netwib_uint16)netwib__buf_ref_data_size(&pstorage->para);
  psmbcmd->value.transaction2_r.totaldatasize = (netwib_uint16)netwib__buf_ref_data_size(&pstorage->data);
  psmbcmd->value.transaction2_r.paradisplacement = 0;
  psmbcmd->value.transaction2_r.datadisplacement = 0;
  netwib_er(netwib_buf_init_ext_empty(&psmbcmd->value.transaction2_r.para));
  netwib_er(netwib_buf_init_ext_empty(&psmbcmd->value.transaction2_r.data));

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
      if (pstorage->setupcount > NETWOX_SMBCMD_TRANSACTION2_SETUP_MAX) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      if (maxsize < (netwib_uint32)(pstorage->setupcount*2)) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      psmbcmd->value.transaction2_r.setupcount = pstorage->setupcount;
      for (i = 0; i < pstorage->setupcount; i++) {
        psmbcmd->value.transaction2_r.setup[i] = pstorage->setup[i];
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
      psmbcmd->value.transaction2_r.paradisplacement = (netwib_uint16)(*poffset - threshold1);
      netwib_er(netwib_buf_init_ext_arrayfilled(para, parasize, &psmbcmd->value.transaction2_r.para));
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
    psmbcmd->value.transaction2_r.datadisplacement = (netwib_uint16)(*poffset - threshold2);
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, &psmbcmd->value.transaction2_r.data));
    *poffset += datasize;
    storedsize += datasize;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2rfmt_init_smbcmdtsc2q(netwox_constsmbcmdtsc2q *psmbcmdtsc2q,
                                                  netwox_smbcmdtsc2rfmt *psmbcmdtsc2rfmt)
{
  switch(psmbcmdtsc2q->type) {
  case NETWOX_SMBCMDTSC2Q_TYPE_FINDFIRST2 :
    psmbcmdtsc2rfmt->fmtfind = psmbcmdtsc2q->value.findfirst2.fmt;
    psmbcmdtsc2rfmt->fmtfindresumekeys = psmbcmdtsc2q->value.findfirst2.flags & NETWOX_SMBCMDTSC2Q_FIND2_FLAGS_RESUMEKEYS;
    switch(psmbcmdtsc2q->value.findfirst2.fmt) {
    case NETWOX_SMBCMDTSC2_FMT_FIND_STANDARD :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_STANDARD;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FIND_DIRECTORY :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_DIRECTORY;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FIND_FULLDIRECTORY :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_FULLDIRECTORY;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FIND_NAMESINFO :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_NAMESINFO;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FIND_BOTHDIRECTORY :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_BOTHDIRECTORY;
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    }
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_FINDNEXT2 :
    psmbcmdtsc2rfmt->fmtfind = psmbcmdtsc2q->value.findnext2.fmt;
    psmbcmdtsc2rfmt->fmtfindresumekeys = psmbcmdtsc2q->value.findnext2.flags & NETWOX_SMBCMDTSC2Q_FIND2_FLAGS_RESUMEKEYS;
    switch(psmbcmdtsc2q->value.findnext2.fmt) {
    case NETWOX_SMBCMDTSC2_FMT_FIND_STANDARD :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_STANDARD;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FIND_DIRECTORY :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_DIRECTORY;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FIND_FULLDIRECTORY :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_FULLDIRECTORY;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FIND_NAMESINFO :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_NAMESINFO;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FIND_BOTHDIRECTORY :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_BOTHDIRECTORY;
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    }
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_QUERYFSINFO :
    switch(psmbcmdtsc2q->value.queryfsinfo.fmt) {
    case NETWOX_SMBCMDTSC2_FMT_FS_OLDALLOCATION :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_OLDALLOCATION;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FS_OLDVOLUME :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_OLDVOLUME;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FS_VOLUME :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_VOLUME;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FS_SIZE :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_SIZE;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FS_DEVICE :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_DEVICE;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FS_ATTRIBUTE :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_ATTRIBUTE;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FS_EFULLSIZE :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_EFULLSIZE;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FS_EOBJECTID :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_EOBJECTID;
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    }
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_QUERYPATHINFO :
    switch(psmbcmdtsc2q->value.querypathinfo.fmt) {
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_BASIC :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_BASIC;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_STANDARD :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_STANDARD;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_EA :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EA;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_NAME :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_NAME;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_ALL :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ALL;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_ALTNAME :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ALTNAME;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_STREAM :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_STREAM;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_EBASIC :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EBASIC;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_ESTANDARD :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ESTANDARD;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_EEA :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EEA;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_ENAME :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ENAME;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_ESTREAM :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ESTREAM;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_EINTERNAL :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EINTERNAL;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_ENETWORKOPEN :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ENETWORKOPEN;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_EATTRIBUTETAG :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EATTRIBUTETAG;
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    }
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_QUERYFILEINFO :
    switch(psmbcmdtsc2q->value.queryfileinfo.fmt) {
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_BASIC :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_BASIC;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_STANDARD :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_STANDARD;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_EA :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EA;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_NAME :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_NAME;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_ALL :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ALL;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_ALTNAME :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ALTNAME;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_STREAM :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_STREAM;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_EBASIC :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EBASIC;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_ESTANDARD :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ESTANDARD;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_EEA :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EEA;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_ENAME :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ENAME;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_ESTREAM :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ESTREAM;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_EINTERNAL :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EINTERNAL;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_ENETWORKOPEN :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ENETWORKOPEN;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_EATTRIBUTETAG :
      psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EATTRIBUTETAG;
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    }
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_BASIC :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EBASIC :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_DISPOSITION :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EDISPOSITION :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_ALLOCATION :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EALLOCATION :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EOF :
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EEOF :
    /*  normally, they all should have the same answer */
    psmbcmdtsc2rfmt->type = NETWOX_SMBCMDTSC2R_TYPE_SETFILEINFO;
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2r_init(netwox_smbcmdtsc2r *psmbcmdtsc2r)
{
  psmbcmdtsc2r->type = NETWOX_SMBCMDTSC2R_TYPE_UNKNOWN;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2r_selecttype(netwox_smbcmdtsc2r *psmbcmdtsc2r,
                                         netwox_smbcmdtsc2r_type type)
{

  if (psmbcmdtsc2r->type != NETWOX_SMBCMDTSC2R_TYPE_UNKNOWN) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  psmbcmdtsc2r->type = type;
  switch(type) {
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_STANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_DIRECTORY :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_FULLDIRECTORY :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_NAMESINFO :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_BOTHDIRECTORY :
    netwib_er(netwib_array_init(sizeof(netwox_smbcmdtsc2_fmt_find_entry), 0, &psmbcmdtsc2r->value.findfirst2_common.entries));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_STANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_DIRECTORY :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_FULLDIRECTORY :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_NAMESINFO :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_BOTHDIRECTORY :
    netwib_er(netwib_array_init(sizeof(netwox_smbcmdtsc2_fmt_find_entry), 0, &psmbcmdtsc2r->value.findnext2_common.entries));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_OLDALLOCATION :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_oldallocation_init(&psmbcmdtsc2r->value.queryfsinfo_oldallocation));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_OLDVOLUME :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_oldvolume_init(&psmbcmdtsc2r->value.queryfsinfo_oldvolume));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_VOLUME :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_volume_init(&psmbcmdtsc2r->value.queryfsinfo_volume));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_SIZE :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_size_init(&psmbcmdtsc2r->value.queryfsinfo_size));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_DEVICE :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_device_init(&psmbcmdtsc2r->value.queryfsinfo_device));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_ATTRIBUTE :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_attribute_init(&psmbcmdtsc2r->value.queryfsinfo_attribute));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_EFULLSIZE :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_efullsize_init(&psmbcmdtsc2r->value.queryfsinfo_efullsize));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_EOBJECTID :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_eobjectid_init(&psmbcmdtsc2r->value.queryfsinfo_eobjectid));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_BASIC :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EBASIC :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_BASIC :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EBASIC :
    netwib_er(netwox_smbcmdtsc2_fmt_file_basic_init(&psmbcmdtsc2r->value.querypathfileinfo_basic));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_STANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ESTANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_STANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ESTANDARD :
    netwib_er(netwox_smbcmdtsc2_fmt_file_standard_init(&psmbcmdtsc2r->value.querypathfileinfo_standard));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EA :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EEA :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EA :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EEA :
    netwib_er(netwox_smbcmdtsc2_fmt_file_ea_init(&psmbcmdtsc2r->value.querypathfileinfo_ea));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_NAME :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ENAME :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_NAME :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ENAME :
    netwib_er(netwox_smbcmdtsc2_fmt_file_name_init(&psmbcmdtsc2r->value.querypathfileinfo_name));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ALL :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EALL :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ALL :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EALL :
    netwib_er(netwox_smbcmdtsc2_fmt_file_all_init(&psmbcmdtsc2r->value.querypathfileinfo_all));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ALTNAME :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ALTNAME :
    netwib_er(netwox_smbcmdtsc2_fmt_file_altname_init(&psmbcmdtsc2r->value.querypathfileinfo_altname));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_STREAM :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ESTREAM :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_STREAM :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ESTREAM :
    netwib_er(netwib_array_init(sizeof(netwox_smbcmdtsc2_fmt_file_stream), 0, &psmbcmdtsc2r->value.querypathfileinfo_stream.streams));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EINTERNAL :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EINTERNAL :
    netwib_er(netwox_smbcmdtsc2_fmt_file_einternal_init(&psmbcmdtsc2r->value.querypathfileinfo_einternal));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ENETWORKOPEN :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ENETWORKOPEN :
    netwib_er(netwox_smbcmdtsc2_fmt_file_enetworkopen_init(&psmbcmdtsc2r->value.querypathfileinfo_enetworkopen));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EATTRIBUTETAG :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EATTRIBUTETAG :
    netwib_er(netwox_smbcmdtsc2_fmt_file_eattributetag_init(&psmbcmdtsc2r->value.querypathfileinfo_eattributetag));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_SETFILEINFO :
    break;
  default :
    psmbcmdtsc2r->type = NETWOX_SMBCMDTSCQ_TYPE_UNKNOWN;
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2r_setdefault(netwox_smbcmdtsc2r *psmbcmdtsc2r)
{
  netwib_uint32 i;

  switch(psmbcmdtsc2r->type) {
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_STANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_DIRECTORY :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_FULLDIRECTORY :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_NAMESINFO :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_BOTHDIRECTORY :
    {
      netwox_smbcmdtsc2_fmt_find_entry *pitem;
      psmbcmdtsc2r->value.findfirst2_common.sid = 0;
      psmbcmdtsc2r->value.findfirst2_common.endofsearch = NETWIB_FALSE;
      psmbcmdtsc2r->value.findfirst2_common.eaerroroffset = 0;
      for (i = 0; i < psmbcmdtsc2r->value.findfirst2_common.entries.size; i++) {
        pitem = (netwox_smbcmdtsc2_fmt_find_entry*)psmbcmdtsc2r->value.findfirst2_common.entries.p[i];
        netwib_er(netwox_smbcmdtsc2_fmt_find_entry_close(pitem));
      }
      netwib_er(netwib_array_ctl_set_size(&psmbcmdtsc2r->value.findfirst2_common.entries, 0));
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_STANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_DIRECTORY :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_FULLDIRECTORY :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_NAMESINFO :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_BOTHDIRECTORY :
    {
      netwox_smbcmdtsc2_fmt_find_entry *pitem;
      psmbcmdtsc2r->value.findnext2_common.endofsearch = NETWIB_FALSE;
      psmbcmdtsc2r->value.findnext2_common.eaerroroffset = 0;
      for (i = 0; i < psmbcmdtsc2r->value.findnext2_common.entries.size; i++) {
        pitem = (netwox_smbcmdtsc2_fmt_find_entry*)psmbcmdtsc2r->value.findnext2_common.entries.p[i];
        netwib_er(netwox_smbcmdtsc2_fmt_find_entry_close(pitem));
      }
      netwib_er(netwib_array_ctl_set_size(&psmbcmdtsc2r->value.findnext2_common.entries, 0));
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_OLDALLOCATION :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_oldallocation_setdefault(&psmbcmdtsc2r->value.queryfsinfo_oldallocation));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_OLDVOLUME :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_oldvolume_setdefault(&psmbcmdtsc2r->value.queryfsinfo_oldvolume));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_VOLUME :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_volume_setdefault(&psmbcmdtsc2r->value.queryfsinfo_volume));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_SIZE :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_size_setdefault(&psmbcmdtsc2r->value.queryfsinfo_size));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_DEVICE :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_device_setdefault(&psmbcmdtsc2r->value.queryfsinfo_device));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_ATTRIBUTE :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_attribute_setdefault(&psmbcmdtsc2r->value.queryfsinfo_attribute));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_EFULLSIZE :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_efullsize_setdefault(&psmbcmdtsc2r->value.queryfsinfo_efullsize));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_EOBJECTID :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_eobjectid_setdefault(&psmbcmdtsc2r->value.queryfsinfo_eobjectid));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_BASIC :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EBASIC :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_BASIC :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EBASIC :
    netwib_er(netwox_smbcmdtsc2_fmt_file_basic_setdefault(&psmbcmdtsc2r->value.querypathfileinfo_basic));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_STANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ESTANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_STANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ESTANDARD :
    netwib_er(netwox_smbcmdtsc2_fmt_file_standard_setdefault(&psmbcmdtsc2r->value.querypathfileinfo_standard));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EA :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EEA :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EA :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EEA :
    netwib_er(netwox_smbcmdtsc2_fmt_file_ea_setdefault(&psmbcmdtsc2r->value.querypathfileinfo_ea));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_NAME :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ENAME :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_NAME :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ENAME :
    netwib_er(netwox_smbcmdtsc2_fmt_file_name_setdefault(&psmbcmdtsc2r->value.querypathfileinfo_name));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ALL :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EALL :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ALL :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EALL :
    netwib_er(netwox_smbcmdtsc2_fmt_file_all_setdefault(&psmbcmdtsc2r->value.querypathfileinfo_all));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ALTNAME :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ALTNAME :
    netwib_er(netwox_smbcmdtsc2_fmt_file_altname_setdefault(&psmbcmdtsc2r->value.querypathfileinfo_altname));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_STREAM :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ESTREAM :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_STREAM :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ESTREAM :
    psmbcmdtsc2r->value.querypathfileinfo_stream.unknown = 0;
    {
      netwox_smbcmdtsc2_fmt_file_stream *pitem;
      for (i = 0; i < psmbcmdtsc2r->value.querypathfileinfo_stream.streams.size; i++) {
        pitem = (netwox_smbcmdtsc2_fmt_file_stream*)psmbcmdtsc2r->value.querypathfileinfo_stream.streams.p[i];
        netwib_er(netwox_smbcmdtsc2_fmt_file_stream_close(pitem));
      }
    }
    netwib_er(netwib_array_ctl_set_size(&psmbcmdtsc2r->value.querypathfileinfo_stream.streams, 0));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EINTERNAL :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EINTERNAL :
    netwib_er(netwox_smbcmdtsc2_fmt_file_einternal_setdefault(&psmbcmdtsc2r->value.querypathfileinfo_einternal));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ENETWORKOPEN :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ENETWORKOPEN :
    netwib_er(netwox_smbcmdtsc2_fmt_file_enetworkopen_setdefault(&psmbcmdtsc2r->value.querypathfileinfo_enetworkopen));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EATTRIBUTETAG :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EATTRIBUTETAG :
    netwib_er(netwox_smbcmdtsc2_fmt_file_eattributetag_setdefault(&psmbcmdtsc2r->value.querypathfileinfo_eattributetag));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_SETFILEINFO :
    psmbcmdtsc2r->value.setfileinfo.unknown = 0;
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2r_close(netwox_smbcmdtsc2r *psmbcmdtsc2r)
{
  netwib_uint32 i;

  switch(psmbcmdtsc2r->type) {
  case NETWOX_SMBCMDTSC2R_TYPE_UNKNOWN :
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_STANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_DIRECTORY :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_FULLDIRECTORY :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_NAMESINFO :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_BOTHDIRECTORY :
    {
      netwox_smbcmdtsc2_fmt_find_entry *pitem;
      for (i = 0; i < psmbcmdtsc2r->value.findfirst2_common.entries.size; i++) {
        pitem = (netwox_smbcmdtsc2_fmt_find_entry*)psmbcmdtsc2r->value.findfirst2_common.entries.p[i];
        netwib_er(netwox_smbcmdtsc2_fmt_find_entry_close(pitem));
      }
      netwib_er(netwib_array_close(&psmbcmdtsc2r->value.findfirst2_common.entries));
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_STANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_DIRECTORY :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_FULLDIRECTORY :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_NAMESINFO :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_BOTHDIRECTORY :
    {
      netwox_smbcmdtsc2_fmt_find_entry *pitem;
      for (i = 0; i < psmbcmdtsc2r->value.findnext2_common.entries.size; i++) {
        pitem = (netwox_smbcmdtsc2_fmt_find_entry*)psmbcmdtsc2r->value.findnext2_common.entries.p[i];
        netwib_er(netwox_smbcmdtsc2_fmt_find_entry_close(pitem));
      }
      netwib_er(netwib_array_close(&psmbcmdtsc2r->value.findnext2_common.entries));
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_OLDALLOCATION :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_oldallocation_close(&psmbcmdtsc2r->value.queryfsinfo_oldallocation));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_OLDVOLUME :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_oldvolume_close(&psmbcmdtsc2r->value.queryfsinfo_oldvolume));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_VOLUME :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_volume_close(&psmbcmdtsc2r->value.queryfsinfo_volume));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_SIZE :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_size_close(&psmbcmdtsc2r->value.queryfsinfo_size));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_DEVICE :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_device_close(&psmbcmdtsc2r->value.queryfsinfo_device));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_ATTRIBUTE :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_attribute_close(&psmbcmdtsc2r->value.queryfsinfo_attribute));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_EFULLSIZE :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_efullsize_close(&psmbcmdtsc2r->value.queryfsinfo_efullsize));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_EOBJECTID :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_eobjectid_close(&psmbcmdtsc2r->value.queryfsinfo_eobjectid));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_BASIC :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EBASIC :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_BASIC :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EBASIC :
    netwib_er(netwox_smbcmdtsc2_fmt_file_basic_close(&psmbcmdtsc2r->value.querypathfileinfo_basic));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_STANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ESTANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_STANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ESTANDARD :
    netwib_er(netwox_smbcmdtsc2_fmt_file_standard_close(&psmbcmdtsc2r->value.querypathfileinfo_standard));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EA :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EEA :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EA :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EEA :
    netwib_er(netwox_smbcmdtsc2_fmt_file_ea_close(&psmbcmdtsc2r->value.querypathfileinfo_ea));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_NAME :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ENAME :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_NAME :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ENAME :
    netwib_er(netwox_smbcmdtsc2_fmt_file_name_close(&psmbcmdtsc2r->value.querypathfileinfo_name));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ALL :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EALL :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ALL :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EALL :
    netwib_er(netwox_smbcmdtsc2_fmt_file_all_close(&psmbcmdtsc2r->value.querypathfileinfo_all));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ALTNAME :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ALTNAME :
    netwib_er(netwox_smbcmdtsc2_fmt_file_altname_close(&psmbcmdtsc2r->value.querypathfileinfo_altname));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_STREAM :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ESTREAM :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_STREAM :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ESTREAM :
    {
      netwox_smbcmdtsc2_fmt_file_stream *pitem;
      for (i = 0; i < psmbcmdtsc2r->value.querypathfileinfo_stream.streams.size; i++) {
        pitem = (netwox_smbcmdtsc2_fmt_file_stream*)psmbcmdtsc2r->value.querypathfileinfo_stream.streams.p[i];
        netwib_er(netwox_smbcmdtsc2_fmt_file_stream_close(pitem));
      }
      netwib_er(netwib_array_close(&psmbcmdtsc2r->value.querypathfileinfo_stream.streams));
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EINTERNAL :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EINTERNAL :
    netwib_er(netwox_smbcmdtsc2_fmt_file_einternal_close(&psmbcmdtsc2r->value.querypathfileinfo_einternal));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ENETWORKOPEN :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ENETWORKOPEN :
    netwib_er(netwox_smbcmdtsc2_fmt_file_enetworkopen_close(&psmbcmdtsc2r->value.querypathfileinfo_enetworkopen));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EATTRIBUTETAG :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EATTRIBUTETAG :
    netwib_er(netwox_smbcmdtsc2_fmt_file_eattributetag_close(&psmbcmdtsc2r->value.querypathfileinfo_eattributetag));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_SETFILEINFO :
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2r_encode_storager(netwox_constsmbcmdtsc2rstorage *pstorage,
                                              netwox_constsmbcmdtsc2rfmt *psmbcmdtsc2rfmt,
                                              netwox_smbcmdtsc2r *psmbcmdtsc2r)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize;
  netwib_uint32 i, sizestd, filenameoffset;
  netwib_uint16 firstpara;

  para = netwib__buf_ref_data_ptr(&pstorage->para);
  parasize = netwib__buf_ref_data_size(&pstorage->para);
  data = netwib__buf_ref_data_ptr(&pstorage->data);
  datasize = netwib__buf_ref_data_size(&pstorage->data);

  netwib_er(netwox_smbcmdtsc2r_selecttype(psmbcmdtsc2r, psmbcmdtsc2rfmt->type));
  switch(psmbcmdtsc2rfmt->type) {
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_STANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_DIRECTORY :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_FULLDIRECTORY :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_NAMESINFO :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_BOTHDIRECTORY :
    {
      netwib_data datainfo;
      netwib_uint32 datainfosize, nextentryoffset, infoscount;
      netwib_uint32 lastnameoffset, lastnameoffsetpkt;
      if (pstorage->setupcount) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      if (parasize != 10) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib__data_decode_uint16_le(para, psmbcmdtsc2r->value.findfirst2_common.sid);
      netwib__data_decode_uint16_le(para, infoscount);
      netwib__data_decode_uint16_le(para, psmbcmdtsc2r->value.findfirst2_common.endofsearch);
      netwib__data_decode_uint16_le(para, psmbcmdtsc2r->value.findfirst2_common.eaerroroffset);
      netwib__data_decode_uint16_le(para, lastnameoffset);
      lastnameoffsetpkt = 0;
      for (i = 0; i < infoscount; i++) {
        netwox_smbcmdtsc2_fmt_find_entry *pitem;
        netwib_er(netwib_array_ctl_set_size(&psmbcmdtsc2r->value.findfirst2_common.entries, i+1));
        pitem = (netwox_smbcmdtsc2_fmt_find_entry*)psmbcmdtsc2r->value.findfirst2_common.entries.p[i];
        netwib_er(netwox_smbcmdtsc2_fmt_find_entry_init(pitem));
        if (psmbcmdtsc2r->type == NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_STANDARD) {
          netwib_er(netwox_smbcmdtsc2_fmt_find_entry_decode(psmbcmdtsc2rfmt, &data, &datasize, pitem));
        } else {
          if (datasize < 4) {
            return(NETWIB_ERR_DATAMISSING);
          }
          datainfo = data;
          netwib__data_decode_uint32_le(datainfo, nextentryoffset);
          if (i+1 == infoscount) {
            /* sometimes, nextentryoffset is 0, sometimes it points to end,
               so, we cannot check its value */
            datainfosize = datasize - 4;
          } else {
            if (nextentryoffset > datasize) {
              return(NETWIB_ERR_DATAMISSING);
            }
            datainfosize = nextentryoffset - 4;
            lastnameoffsetpkt += nextentryoffset;
          }
          netwib_er(netwox_smbcmdtsc2_fmt_find_entry_decode(psmbcmdtsc2rfmt, &datainfo, &datainfosize, pitem));
          /* next entry */
          if (i+1 == infoscount) {
            /* datainfo and datainfosize were updated, so point just after */
            data = datainfo;
            datasize = datainfosize;
          } else {
            data += nextentryoffset;
            datasize -= nextentryoffset;
            if (datainfosize > 7/* too many padding */) {
              return(NETWIB_ERR_NOTCONVERTED);
            }
          }
        }
      }
      /* lastnameoffsetpkt was not computed in loop for STANDARD, do it now */
      if (psmbcmdtsc2r->type == NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_STANDARD) {
        if (infoscount != 0) {
          netwib_er(netwox_smbcmdtsc2_fmt_find_entry_size_standard(psmbcmdtsc2rfmt, &sizestd));
          lastnameoffsetpkt = (infoscount - 1) * sizestd;
        }
      }
      if (lastnameoffset != lastnameoffsetpkt && !pstorage->isunicode) {
        /* if not unicode, sometimes use offset of filename instead of offset
           of start of structure */
        if (psmbcmdtsc2r->type != NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_STANDARD) {
          lastnameoffsetpkt += 4; /* nextentryoffset (uint32) is before item */
        }
        netwib_er(netwox_smbcmdtsc2_fmt_find_entry_offsetfilename(psmbcmdtsc2rfmt, &filenameoffset));
        lastnameoffsetpkt += filenameoffset;
      }
      /* check */
      if (lastnameoffset != lastnameoffsetpkt) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      if (datasize > 7/* too many padding */) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_STANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_DIRECTORY :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_FULLDIRECTORY :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_NAMESINFO :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_BOTHDIRECTORY :
    {
      netwib_data datainfo;
      netwib_uint32 datainfosize, nextentryoffset, infoscount;
      netwib_uint32 lastnameoffset, lastnameoffsetpkt;
      if (pstorage->setupcount) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      if (parasize != 8) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib__data_decode_uint16_le(para, infoscount);
      netwib__data_decode_uint16_le(para, psmbcmdtsc2r->value.findnext2_common.endofsearch);
      netwib__data_decode_uint16_le(para, psmbcmdtsc2r->value.findnext2_common.eaerroroffset);
      netwib__data_decode_uint16_le(para, lastnameoffset);
      lastnameoffsetpkt = 0;
      for (i = 0; i < infoscount; i++) {
        netwox_smbcmdtsc2_fmt_find_entry *pitem;
        netwib_er(netwib_array_ctl_set_size(&psmbcmdtsc2r->value.findnext2_common.entries, i+1));
        pitem = (netwox_smbcmdtsc2_fmt_find_entry*)psmbcmdtsc2r->value.findnext2_common.entries.p[i];
        netwib_er(netwox_smbcmdtsc2_fmt_find_entry_init(pitem));
        if (psmbcmdtsc2r->type == NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_STANDARD) {
          netwib_er(netwox_smbcmdtsc2_fmt_find_entry_decode(psmbcmdtsc2rfmt, &data, &datasize, pitem));
        } else {
          if (datasize < 4) {
            return(NETWIB_ERR_DATAMISSING);
          }
          datainfo = data;
          netwib__data_decode_uint32_le(datainfo, nextentryoffset);
          if (i+1 == infoscount) {
            /* sometimes, nextentryoffset is 0, sometimes it points to end,
               so, we cannot check its value */
            datainfosize = datasize - 4;
          } else {
            if (nextentryoffset > datasize) {
              return(NETWIB_ERR_DATAMISSING);
            }
            datainfosize = nextentryoffset - 4;
            lastnameoffsetpkt += nextentryoffset;
          }
          netwib_er(netwox_smbcmdtsc2_fmt_find_entry_decode(psmbcmdtsc2rfmt, &datainfo, &datainfosize, pitem));
          /* next entry */
          if (i+1 == infoscount) {
            /* datainfo and datainfosize were updated, so point just after */
            data = datainfo;
            datasize = datainfosize;
          } else {
            data += nextentryoffset;
            datasize -= nextentryoffset;
            if (datainfosize > 7/* too many padding */) {
              return(NETWIB_ERR_NOTCONVERTED);
            }
          }
        }
      }
      /* lastnameoffsetpkt was not computed in loop for STANDARD, do it now */
      if (psmbcmdtsc2r->type == NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_STANDARD) {
        if (infoscount != 0) {
          netwib_er(netwox_smbcmdtsc2_fmt_find_entry_size_standard(psmbcmdtsc2rfmt, &sizestd));
          lastnameoffsetpkt = (infoscount - 1) * sizestd;
        }
      }
      if (lastnameoffset != lastnameoffsetpkt && !pstorage->isunicode) {
        /* if not unicode, sometimes use offset of filename instead of offset
           of start of structure */
        if (psmbcmdtsc2r->type != NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_STANDARD) {
          lastnameoffsetpkt += 4; /* nextentryoffset (uint32) is before item */
        }
        netwib_er(netwox_smbcmdtsc2_fmt_find_entry_offsetfilename(psmbcmdtsc2rfmt, &filenameoffset));
        lastnameoffsetpkt += filenameoffset;
      }
      /* check */
      if (lastnameoffset != lastnameoffsetpkt) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      if (datasize > 7/* too many padding */) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_OLDALLOCATION :
    if (pstorage->setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdtsc2_fmt_fs_oldallocation_decode(&data, &datasize, &psmbcmdtsc2r->value.queryfsinfo_oldallocation));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_OLDVOLUME :
    if (pstorage->setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdtsc2_fmt_fs_oldvolume_decode(&data, &datasize, &psmbcmdtsc2r->value.queryfsinfo_oldvolume));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_VOLUME :
    if (pstorage->setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdtsc2_fmt_fs_volume_decode(&data, &datasize, &psmbcmdtsc2r->value.queryfsinfo_volume));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_SIZE :
    if (pstorage->setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdtsc2_fmt_fs_size_decode(&data, &datasize, &psmbcmdtsc2r->value.queryfsinfo_size));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_DEVICE :
    if (pstorage->setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdtsc2_fmt_fs_device_decode(&data, &datasize, &psmbcmdtsc2r->value.queryfsinfo_device));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_ATTRIBUTE :
    if (pstorage->setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdtsc2_fmt_fs_attribute_decode(&data, &datasize, &psmbcmdtsc2r->value.queryfsinfo_attribute));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_EFULLSIZE :
    if (pstorage->setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdtsc2_fmt_fs_efullsize_decode(&data, &datasize, &psmbcmdtsc2r->value.queryfsinfo_efullsize));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_EOBJECTID :
    if (pstorage->setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdtsc2_fmt_fs_eobjectid_decode(&data, &datasize, &psmbcmdtsc2r->value.queryfsinfo_eobjectid));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_BASIC :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EBASIC :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_BASIC :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EBASIC :
    if (pstorage->setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize != 2) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, firstpara);
    if (firstpara != 0) {
      /* I don't know what this. Allways seen with zero. */
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdtsc2_fmt_file_basic_decode(&data, &datasize, &psmbcmdtsc2r->value.querypathfileinfo_basic));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_STANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ESTANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_STANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ESTANDARD :
    if (pstorage->setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize != 2) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, firstpara);
    if (firstpara != 0) {
      /* I don't know what this. Allways seen with zero. */
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdtsc2_fmt_file_standard_decode(&data, &datasize, &psmbcmdtsc2r->value.querypathfileinfo_standard));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EA :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EEA :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EA :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EEA :
    if (pstorage->setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize != 2) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, firstpara);
    if (firstpara != 0) {
      /* I don't know what this. Allways seen with zero. */
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdtsc2_fmt_file_ea_decode(&data, &datasize, &psmbcmdtsc2r->value.querypathfileinfo_ea));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_NAME :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ENAME :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_NAME :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ENAME :
    if (pstorage->setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize != 2) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, firstpara);
    if (firstpara != 0) {
      /* I don't know what this. Allways seen with zero. */
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdtsc2_fmt_file_name_decode(&data, &datasize, &psmbcmdtsc2r->value.querypathfileinfo_name));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ALL :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EALL :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ALL :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EALL :
    if (pstorage->setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize != 2) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, firstpara);
    if (firstpara != 0) {
      /* I don't know what this. Allways seen with zero. */
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdtsc2_fmt_file_all_decode(&data, &datasize, &psmbcmdtsc2r->value.querypathfileinfo_all));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ALTNAME :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ALTNAME :
    if (pstorage->setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize != 2) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, firstpara);
    if (firstpara != 0) {
      /* I don't know what this. Allways seen with zero. */
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdtsc2_fmt_file_altname_decode(&data, &datasize, &psmbcmdtsc2r->value.querypathfileinfo_altname));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_STREAM :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ESTREAM :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_STREAM :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ESTREAM :
    {
      netwib_data datainfo;
      netwib_uint32 datainfosize, nextentryoffset;
      if (pstorage->setupcount) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      if (parasize != 2) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib__data_decode_uint16_le(para, psmbcmdtsc2r->value.querypathfileinfo_stream.unknown);
      if (datasize) {
        i = 0;
        while (NETWIB_TRUE) {
          netwox_smbcmdtsc2_fmt_file_stream *pitem;
          netwib_er(netwib_array_ctl_set_size(&psmbcmdtsc2r->value.querypathfileinfo_stream.streams, i+1));
          pitem = (netwox_smbcmdtsc2_fmt_file_stream*)psmbcmdtsc2r->value.querypathfileinfo_stream.streams.p[i];
          netwib_er(netwox_smbcmdtsc2_fmt_file_stream_init(pitem));
          if (datasize < 4) {
            return(NETWIB_ERR_DATAMISSING);
          }
          datainfo = data;
          netwib__data_decode_uint32_le(datainfo, nextentryoffset);
          if (nextentryoffset == 0) {
            datainfosize = datasize - 4;
          } else {
            if (nextentryoffset > datasize) {
              return(NETWIB_ERR_DATAMISSING);
            }
            datainfosize = nextentryoffset - 4;
          }
          netwib_er(netwox_smbcmdtsc2_fmt_file_stream_decode(&datainfo, &datainfosize, pitem));
          /* next entry */
          if (nextentryoffset == 0) {
            data = datainfo;
            datasize = datainfosize;
            break;
          } else {
            data += nextentryoffset;
            datasize -= nextentryoffset;
            if (datainfosize > 7/* too many padding */) {
              return(NETWIB_ERR_NOTCONVERTED);
            }
          }
          i++;
        }
        if (datasize > 7/* too many padding */) {
          return(NETWIB_ERR_NOTCONVERTED);
        }
      }
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EINTERNAL :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EINTERNAL :
    if (pstorage->setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize != 2) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, firstpara);
    if (firstpara != 0) {
      /* I don't know what this. Allways seen with zero. */
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdtsc2_fmt_file_einternal_decode(&data, &datasize, &psmbcmdtsc2r->value.querypathfileinfo_einternal));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ENETWORKOPEN :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ENETWORKOPEN :
    if (pstorage->setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize != 2) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, firstpara);
    if (firstpara != 0) {
      /* I don't know what this. Allways seen with zero. */
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdtsc2_fmt_file_enetworkopen_decode(&data, &datasize, &psmbcmdtsc2r->value.querypathfileinfo_enetworkopen));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EATTRIBUTETAG :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EATTRIBUTETAG :
    if (pstorage->setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize != 2) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, firstpara);
    if (firstpara != 0) {
      /* I don't know what this. Allways seen with zero. */
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdtsc2_fmt_file_eattributetag_decode(&data, &datasize, &psmbcmdtsc2r->value.querypathfileinfo_eattributetag));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_SETFILEINFO :
    if (pstorage->setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize != 2) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmdtsc2r->value.setfileinfo.unknown);
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    break;
  default :
    return(NETWIB_ERR_NOTCONVERTED);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2r_decode_storager(netwox_constsmbcmdtsc2r *psmbcmdtsc2r,
                                              netwox_constsmbcmdtsc2rfmt *psmbcmdtsc2rfmt,
                                              netwox_smbcmdtsc2rstorage *pstorage)
{
  netwib_data para, data;

  if (psmbcmdtsc2rfmt->type != psmbcmdtsc2r->type) {
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  switch(psmbcmdtsc2r->type) {
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_STANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_DIRECTORY :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_FULLDIRECTORY :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_NAMESINFO :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_BOTHDIRECTORY :
    {
      netwib_uint32 i, lastnameoffset, nextentryoffset, itemsize;
      netwib_er(netwib_buf_wantspace(&pstorage->para, 10, &para));
      netwib__data_append_uint16_le(para, psmbcmdtsc2r->value.findfirst2_common.sid);
      netwib__data_append_uint16_le(para, psmbcmdtsc2r->value.findfirst2_common.entries.size);
      if (psmbcmdtsc2r->value.findfirst2_common.endofsearch) {
        netwib__data_append_uint16_le(para, 1);
      } else {
        netwib__data_append_uint16_le(para, 0);
      }
      netwib__data_append_uint16_le(para, psmbcmdtsc2r->value.findfirst2_common.eaerroroffset);
      /* compute lastnameoffset */
      lastnameoffset = 0;
      for (i = 0; i < psmbcmdtsc2r->value.findfirst2_common.entries.size; i++) {
        netwox_smbcmdtsc2_fmt_find_entry *pitem;
        pitem = (netwox_smbcmdtsc2_fmt_find_entry*)psmbcmdtsc2r->value.findfirst2_common.entries.p[i];
        if (i+1 != psmbcmdtsc2r->value.findfirst2_common.entries.size) {
          if (psmbcmdtsc2r->type != NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_STANDARD) {
            lastnameoffset += 4; /* nextentryoffset (uint32) is before item */
          }
          netwib_er(netwox_smbcmdtsc2_fmt_find_entry_size(psmbcmdtsc2rfmt, pitem,
                                                           &itemsize));
          lastnameoffset += itemsize;
        }
      }
      if (!pstorage->isunicode) {
        if (psmbcmdtsc2r->type != NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_STANDARD) {
          lastnameoffset += 4; /* nextentryoffset (uint32) is before item */
        }
        netwib_er(netwox_smbcmdtsc2_fmt_find_entry_offsetfilename(psmbcmdtsc2rfmt, &itemsize));
        lastnameoffset += itemsize;
      }
      /* append lastnameoffset */
      netwib__data_append_uint16_le(para, lastnameoffset);
      pstorage->para.endoffset += 10;
      for (i = 0; i < psmbcmdtsc2r->value.findfirst2_common.entries.size; i++) {
        netwox_smbcmdtsc2_fmt_find_entry *pitem;
        pitem = (netwox_smbcmdtsc2_fmt_find_entry*)psmbcmdtsc2r->value.findfirst2_common.entries.p[i];
        if (psmbcmdtsc2r->type != NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_STANDARD) {
          netwib_er(netwib_buf_wantspace(&pstorage->data, 4, &data));
          if (i+1 == psmbcmdtsc2r->value.findfirst2_common.entries.size) {
            nextentryoffset = 0;
          } else {
            netwib_er(netwox_smbcmdtsc2_fmt_find_entry_size(psmbcmdtsc2rfmt, pitem,
                                                             &itemsize));
            nextentryoffset = 4 + itemsize;
          }
          netwib__data_append_uint32_le(data, nextentryoffset);
          pstorage->data.endoffset += 4;
        }
        netwib_er(netwox_smbcmdtsc2_fmt_find_entry_append(psmbcmdtsc2rfmt, pitem, &pstorage->data));
      }
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_STANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_DIRECTORY :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_FULLDIRECTORY :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_NAMESINFO :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_BOTHDIRECTORY :
    {
      netwib_uint32 i, lastnameoffset, nextentryoffset, itemsize;
      netwib_er(netwib_buf_wantspace(&pstorage->para, 8, &para));
      netwib__data_append_uint16_le(para, psmbcmdtsc2r->value.findnext2_common.entries.size);
      if (psmbcmdtsc2r->value.findnext2_common.endofsearch) {
        netwib__data_append_uint16_le(para, 1);
      } else {
        netwib__data_append_uint16_le(para, 0);
      }
      netwib__data_append_uint16_le(para, psmbcmdtsc2r->value.findnext2_common.eaerroroffset);
      /* compute lastnameoffset */
      lastnameoffset = 0;
      for (i = 0; i < psmbcmdtsc2r->value.findnext2_common.entries.size; i++) {
        netwox_smbcmdtsc2_fmt_find_entry *pitem;
        pitem = (netwox_smbcmdtsc2_fmt_find_entry*)psmbcmdtsc2r->value.findnext2_common.entries.p[i];
        if (i+1 != psmbcmdtsc2r->value.findnext2_common.entries.size) {
          if (psmbcmdtsc2r->type != NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_STANDARD) {
            lastnameoffset += 4; /* nextentryoffset (uint32) is before item */
          }
          netwib_er(netwox_smbcmdtsc2_fmt_find_entry_size(psmbcmdtsc2rfmt, pitem,
                                                           &itemsize));
          lastnameoffset += itemsize;
        }
      }
      if (!pstorage->isunicode) {
        if (psmbcmdtsc2r->type != NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_STANDARD) {
          lastnameoffset += 4; /* nextentryoffset (uint32) is before item */
        }
        netwib_er(netwox_smbcmdtsc2_fmt_find_entry_offsetfilename(psmbcmdtsc2rfmt, &itemsize));
        lastnameoffset += itemsize;
      }
      /* append lastnameoffset */
      netwib__data_append_uint16_le(para, lastnameoffset);
      pstorage->para.endoffset += 8;
      for (i = 0; i < psmbcmdtsc2r->value.findnext2_common.entries.size; i++) {
        netwox_smbcmdtsc2_fmt_find_entry *pitem;
        pitem = (netwox_smbcmdtsc2_fmt_find_entry*)psmbcmdtsc2r->value.findnext2_common.entries.p[i];
        if (psmbcmdtsc2r->type != NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_STANDARD) {
          netwib_er(netwib_buf_wantspace(&pstorage->data, 4, &data));
          if (i+1 == psmbcmdtsc2r->value.findfirst2_common.entries.size) {
            nextentryoffset = 0;
          } else {
            netwib_er(netwox_smbcmdtsc2_fmt_find_entry_size(psmbcmdtsc2rfmt, pitem,
                                                             &itemsize));
            nextentryoffset = 4 + itemsize;
          }
          netwib__data_append_uint32_le(data, nextentryoffset);
          pstorage->data.endoffset += 4;
        }
        netwib_er(netwox_smbcmdtsc2_fmt_find_entry_append(psmbcmdtsc2rfmt, pitem, &pstorage->data));
      }
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_OLDALLOCATION :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_oldallocation_append(&psmbcmdtsc2r->value.queryfsinfo_oldallocation, &pstorage->data));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_OLDVOLUME :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_oldvolume_append(&psmbcmdtsc2r->value.queryfsinfo_oldvolume, &pstorage->data));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_VOLUME :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_volume_append(&psmbcmdtsc2r->value.queryfsinfo_volume, &pstorage->data));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_SIZE :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_size_append(&psmbcmdtsc2r->value.queryfsinfo_size, &pstorage->data));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_DEVICE :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_device_append(&psmbcmdtsc2r->value.queryfsinfo_device, &pstorage->data));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_ATTRIBUTE :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_attribute_append(&psmbcmdtsc2r->value.queryfsinfo_attribute, &pstorage->data));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_EFULLSIZE :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_efullsize_append(&psmbcmdtsc2r->value.queryfsinfo_efullsize, &pstorage->data));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_EOBJECTID :
    netwib_er(netwox_smbcmdtsc2_fmt_fs_eobjectid_append(&psmbcmdtsc2r->value.queryfsinfo_eobjectid, &pstorage->data));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_BASIC :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EBASIC :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_BASIC :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EBASIC :
    netwib_er(netwib_buf_wantspace(&pstorage->para, 2, &para));
    netwib_c_memset(para, 0, 2); /* unknown */
    pstorage->para.endoffset += 2;
    netwib_er(netwox_smbcmdtsc2_fmt_file_basic_append(&psmbcmdtsc2r->value.querypathfileinfo_basic, &pstorage->data));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_STANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ESTANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_STANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ESTANDARD :
    netwib_er(netwib_buf_wantspace(&pstorage->para, 2, &para));
    netwib_c_memset(para, 0, 2); /* unknown */
    pstorage->para.endoffset += 2;
    netwib_er(netwox_smbcmdtsc2_fmt_file_standard_append(&psmbcmdtsc2r->value.querypathfileinfo_standard, &pstorage->data));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EA :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EEA :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EA :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EEA :
    netwib_er(netwib_buf_wantspace(&pstorage->para, 2, &para));
    netwib_c_memset(para, 0, 2); /* unknown */
    pstorage->para.endoffset += 2;
    netwib_er(netwox_smbcmdtsc2_fmt_file_ea_append(&psmbcmdtsc2r->value.querypathfileinfo_ea, &pstorage->data));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_NAME :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ENAME :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_NAME :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ENAME :
    netwib_er(netwib_buf_wantspace(&pstorage->para, 2, &para));
    netwib_c_memset(para, 0, 2); /* unknown */
    pstorage->para.endoffset += 2;
    netwib_er(netwox_smbcmdtsc2_fmt_file_name_append(&psmbcmdtsc2r->value.querypathfileinfo_name, &pstorage->data));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ALL :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EALL :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ALL :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EALL :
    netwib_er(netwib_buf_wantspace(&pstorage->para, 2, &para));
    netwib_c_memset(para, 0, 2); /* unknown */
    pstorage->para.endoffset += 2;
    netwib_er(netwox_smbcmdtsc2_fmt_file_all_append(&psmbcmdtsc2r->value.querypathfileinfo_all, &pstorage->data));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ALTNAME :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ALTNAME :
    netwib_er(netwib_buf_wantspace(&pstorage->para, 2, &para));
    netwib_c_memset(para, 0, 2); /* unknown */
    pstorage->para.endoffset += 2;
    netwib_er(netwox_smbcmdtsc2_fmt_file_altname_append(&psmbcmdtsc2r->value.querypathfileinfo_altname, &pstorage->data));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_STREAM :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ESTREAM :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_STREAM :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ESTREAM :
    {
      netwib_uint32 i, nextentryoffset, itemsize;
      netwib_er(netwib_buf_wantspace(&pstorage->para, 2, &para));
      netwib__data_append_uint16_le(para, psmbcmdtsc2r->value.querypathfileinfo_stream.unknown);
      pstorage->para.endoffset += 2;
      for (i = 0; i < psmbcmdtsc2r->value.querypathfileinfo_stream.streams.size; i++) {
        netwox_smbcmdtsc2_fmt_file_stream *pitem;
        pitem = (netwox_smbcmdtsc2_fmt_file_stream*)psmbcmdtsc2r->value.querypathfileinfo_stream.streams.p[i];
        netwib_er(netwib_buf_wantspace(&pstorage->data, 4, &data));
        if (i+1 == psmbcmdtsc2r->value.querypathfileinfo_stream.streams.size) {
          nextentryoffset = 0;
        } else {
          netwib_er(netwox_smbcmdtsc2_fmt_file_stream_size(pitem, &itemsize));
          nextentryoffset = 4 + itemsize;
        }
        netwib__data_append_uint32_le(data, nextentryoffset);
        pstorage->data.endoffset += 4;
        netwib_er(netwox_smbcmdtsc2_fmt_file_stream_append(pitem, &pstorage->data));
      }
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EINTERNAL :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EINTERNAL :
    netwib_er(netwib_buf_wantspace(&pstorage->para, 2, &para));
    netwib_c_memset(para, 0, 2); /* unknown */
    pstorage->para.endoffset += 2;
    netwib_er(netwox_smbcmdtsc2_fmt_file_einternal_append(&psmbcmdtsc2r->value.querypathfileinfo_einternal, &pstorage->data));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ENETWORKOPEN :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ENETWORKOPEN :
    netwib_er(netwib_buf_wantspace(&pstorage->para, 2, &para));
    netwib_c_memset(para, 0, 2); /* unknown */
    pstorage->para.endoffset += 2;
    netwib_er(netwox_smbcmdtsc2_fmt_file_enetworkopen_append(&psmbcmdtsc2r->value.querypathfileinfo_enetworkopen, &pstorage->data));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EATTRIBUTETAG :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EATTRIBUTETAG :
    netwib_er(netwib_buf_wantspace(&pstorage->para, 2, &para));
    netwib_c_memset(para, 0, 2); /* unknown */
    pstorage->para.endoffset += 2;
    netwib_er(netwox_smbcmdtsc2_fmt_file_eattributetag_append(&psmbcmdtsc2r->value.querypathfileinfo_eattributetag, &pstorage->data));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_SETFILEINFO :
    netwib_er(netwib_buf_wantspace(&pstorage->para, 2, &para));
    netwib__data_append_uint16_le(para, psmbcmdtsc2r->value.setfileinfo.unknown);
    pstorage->para.endoffset += 2;
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2r_show(netwox_constsmbcmdtsc2r *psmbcmdtsc2r,
                                   netwib_buf *pbuf)
{
  netwib_uint32 i;

  switch(psmbcmdtsc2r->type) {
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_STANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_DIRECTORY :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_FULLDIRECTORY :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_NAMESINFO :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_BOTHDIRECTORY :
    netwib_er(netwib_show_array_fmt32(pbuf, " FINDFIRST2 Reply"));
    netwib_er(netwib_show_array_fmt32(pbuf, " sid=%{uint16}", psmbcmdtsc2r->value.findfirst2_common.sid));
    netwib_er(netwib_show_array_fmt32(pbuf, " endofsearch=%{bool}", psmbcmdtsc2r->value.findfirst2_common.endofsearch));
    netwib_er(netwib_show_array_fmt32(pbuf, " eaerroroffset=%{uint16}", psmbcmdtsc2r->value.findfirst2_common.eaerroroffset));
    for (i = 0; i < psmbcmdtsc2r->value.findfirst2_common.entries.size; i++) {
      netwox_smbcmdtsc2_fmt_find_entry *pitem;
      pitem = (netwox_smbcmdtsc2_fmt_find_entry*)psmbcmdtsc2r->value.findfirst2_common.entries.p[i];
      netwib_er(netwox_smbcmdtsc2_fmt_find_entry_show(pitem, pbuf));
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_STANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_DIRECTORY :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_FULLDIRECTORY :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_NAMESINFO :
  case NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_BOTHDIRECTORY :
    netwib_er(netwib_show_array_fmt32(pbuf, " FINDNEXT2 Reply"));
    netwib_er(netwib_show_array_fmt32(pbuf, " endofsearch=%{bool}", psmbcmdtsc2r->value.findnext2_common.endofsearch));
    netwib_er(netwib_show_array_fmt32(pbuf, " eaerroroffset=%{uint16}", psmbcmdtsc2r->value.findnext2_common.eaerroroffset));
    for (i = 0; i < psmbcmdtsc2r->value.findnext2_common.entries.size; i++) {
      netwox_smbcmdtsc2_fmt_find_entry *pitem;
      pitem = (netwox_smbcmdtsc2_fmt_find_entry*)psmbcmdtsc2r->value.findnext2_common.entries.p[i];
      netwib_er(netwox_smbcmdtsc2_fmt_find_entry_show(pitem, pbuf));
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_OLDALLOCATION :
    netwib_er(netwib_show_array_fmt32(pbuf, " QUERYFSINFO Reply (OLDALLOCATION format)"));
    netwib_er(netwox_smbcmdtsc2_fmt_fs_oldallocation_show(&psmbcmdtsc2r->value.queryfsinfo_oldallocation, pbuf));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_OLDVOLUME :
    netwib_er(netwib_show_array_fmt32(pbuf, " QUERYFSINFO Reply (OLDVOLUME format)"));
    netwib_er(netwox_smbcmdtsc2_fmt_fs_oldvolume_show(&psmbcmdtsc2r->value.queryfsinfo_oldvolume, pbuf));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_VOLUME :
    netwib_er(netwib_show_array_fmt32(pbuf, " QUERYFSINFO Reply (VOLUME format)"));
    netwib_er(netwox_smbcmdtsc2_fmt_fs_volume_show(&psmbcmdtsc2r->value.queryfsinfo_volume, pbuf));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_SIZE :
    netwib_er(netwib_show_array_fmt32(pbuf, " QUERYFSINFO Reply (SIZE format)"));
    netwib_er(netwox_smbcmdtsc2_fmt_fs_size_show(&psmbcmdtsc2r->value.queryfsinfo_size, pbuf));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_DEVICE :
    netwib_er(netwib_show_array_fmt32(pbuf, " QUERYFSINFO Reply (DEVICE format)"));
    netwib_er(netwox_smbcmdtsc2_fmt_fs_device_show(&psmbcmdtsc2r->value.queryfsinfo_device, pbuf));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_ATTRIBUTE :
    netwib_er(netwib_show_array_fmt32(pbuf, " QUERYFSINFO Reply (ATTRIBUTE format)"));
    netwib_er(netwox_smbcmdtsc2_fmt_fs_attribute_show(&psmbcmdtsc2r->value.queryfsinfo_attribute, pbuf));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_EFULLSIZE :
    netwib_er(netwib_show_array_fmt32(pbuf, " QUERYFSINFO Reply (EFULLSIZE format)"));
    netwib_er(netwox_smbcmdtsc2_fmt_fs_efullsize_show(&psmbcmdtsc2r->value.queryfsinfo_efullsize, pbuf));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_EOBJECTID :
    netwib_er(netwib_show_array_fmt32(pbuf, " QUERYFSINFO Reply (EOBJECTID format)"));
    netwib_er(netwox_smbcmdtsc2_fmt_fs_eobjectid_show(&psmbcmdtsc2r->value.queryfsinfo_eobjectid, pbuf));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_BASIC :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EBASIC :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_BASIC :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EBASIC :
    netwib_er(netwib_show_array_fmt32(pbuf, " QUERYPATHFILEINFO Reply (BASIC format)"));
    netwib_er(netwox_smbcmdtsc2_fmt_file_basic_show(&psmbcmdtsc2r->value.querypathfileinfo_basic, pbuf));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_STANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ESTANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_STANDARD :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ESTANDARD :
    netwib_er(netwib_show_array_fmt32(pbuf, " QUERYPATHFILEINFO Reply (STANDARD format)"));
    netwib_er(netwox_smbcmdtsc2_fmt_file_standard_show(&psmbcmdtsc2r->value.querypathfileinfo_standard, pbuf));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EA :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EEA :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EA :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EEA :
    netwib_er(netwib_show_array_fmt32(pbuf, " QUERYPATHFILEINFO Reply (EA format)"));
    netwib_er(netwox_smbcmdtsc2_fmt_file_ea_show(&psmbcmdtsc2r->value.querypathfileinfo_ea, pbuf));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_NAME :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ENAME :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_NAME :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ENAME :
    netwib_er(netwib_show_array_fmt32(pbuf, " QUERYPATHFILEINFO Reply (NAME format)"));
    netwib_er(netwox_smbcmdtsc2_fmt_file_name_show(&psmbcmdtsc2r->value.querypathfileinfo_name, pbuf));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ALL :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EALL :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ALL :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EALL :
    netwib_er(netwib_show_array_fmt32(pbuf, " QUERYPATHFILEINFO Reply (ALL format)"));
    netwib_er(netwox_smbcmdtsc2_fmt_file_all_show(&psmbcmdtsc2r->value.querypathfileinfo_all, pbuf));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ALTNAME :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ALTNAME :
    netwib_er(netwib_show_array_fmt32(pbuf, " QUERYPATHFILEINFO Reply (ALTNAME format)"));
    netwib_er(netwox_smbcmdtsc2_fmt_file_altname_show(&psmbcmdtsc2r->value.querypathfileinfo_altname, pbuf));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_STREAM :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ESTREAM :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_STREAM :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ESTREAM :
    netwib_er(netwib_show_array_fmt32(pbuf, " QUERYPATHFILEINFO Reply (STREAM format)"));
    netwib_er(netwib_show_array_fmt32(pbuf, " unknown=%{uint16}", psmbcmdtsc2r->value.querypathfileinfo_stream.unknown));
    for (i = 0; i < psmbcmdtsc2r->value.querypathfileinfo_stream.streams.size; i++) {
      netwox_smbcmdtsc2_fmt_file_stream *pitem;
      pitem = (netwox_smbcmdtsc2_fmt_file_stream*)psmbcmdtsc2r->value.querypathfileinfo_stream.streams.p[i];
      netwib_er(netwox_smbcmdtsc2_fmt_file_stream_show(pitem, pbuf));
    }
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EINTERNAL :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EINTERNAL :
    netwib_er(netwib_show_array_fmt32(pbuf, " QUERYPATHFILEINFO Reply (EINTERNAL format)"));
    netwib_er(netwox_smbcmdtsc2_fmt_file_einternal_show(&psmbcmdtsc2r->value.querypathfileinfo_einternal, pbuf));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ENETWORKOPEN :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ENETWORKOPEN :
    netwib_er(netwib_show_array_fmt32(pbuf, " QUERYPATHFILEINFO Reply (ENETWORKOPEN format)"));
    netwib_er(netwox_smbcmdtsc2_fmt_file_enetworkopen_show(&psmbcmdtsc2r->value.querypathfileinfo_enetworkopen, pbuf));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EATTRIBUTETAG :
  case NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EATTRIBUTETAG :
    netwib_er(netwib_show_array_fmt32(pbuf, " QUERYPATHFILEINFO Reply (EATTRIBUTETAG format)"));
    netwib_er(netwox_smbcmdtsc2_fmt_file_eattributetag_show(&psmbcmdtsc2r->value.querypathfileinfo_eattributetag, pbuf));
    break;
  case NETWOX_SMBCMDTSC2R_TYPE_SETFILEINFO :
    netwib_er(netwib_show_array_fmt32(pbuf, " SETFILEINFO Reply"));
    netwib_er(netwib_show_array_fmt32(pbuf, " unknown=%{uint16}", psmbcmdtsc2r->value.setfileinfo.unknown));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2r_display(netwib_bufpool *pbufpool,
                                      netwox_constsmbcmdtsc2r *psmbcmdtsc2r)
{
  netwib_buf *pbuf;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));

  ret = netwox_smbcmdtsc2r_show(psmbcmdtsc2r, pbuf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_display(pbuf, NETWIB_ENCODETYPE_DATA));
  } else {
    netwib_er(netwib_fmt_display("This Transaction2 Reply could not be decoded\n"));
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(ret);
}
