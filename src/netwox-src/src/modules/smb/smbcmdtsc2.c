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
#define NETWOX_SMBCMDTSC2_SHORTFILENAME_ULEN 24
#define NETWOX_SMBCMDTSC2_VOLUME_LEN 12

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_find_entry_init(netwox_smbcmdtsc2_fmt_find_entry *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_initdefault(&pitem->shortfilename));
  netwib_er(netwox_smbcmdcmn_data_initdefault(&pitem->longfilename));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_find_entry_setdefault(netwox_smbcmdtsc2_fmt_find fmtfind,
                                                        netwox_smbcmdtsc2_fmt_find_entry *pitem)
{
  pitem->fmtfind = fmtfind;
  pitem->resumekey = 0;
  netwox__smbcmdcmn_time1601_initdefault(pitem->creationtime);
  netwox__smbcmdcmn_time1601_initdefault(pitem->lastaccesstime);
  netwox__smbcmdcmn_time1601_initdefault(pitem->lastwritetime);
  netwox__smbcmdcmn_time1601_initdefault(pitem->changetime);
  netwox__smbcmdcmn_uint64_initdefault(pitem->filesize);
  netwox__smbcmdcmn_uint64_initdefault(pitem->allocationsize);
  netwox__smbcmdcmn_fileattr16_initdefault(pitem->fileattr16);
  netwox__smbcmdcmn_fileattr32_initdefault(pitem->fileattr32);
  pitem->easize = 0;

  netwib_er(netwox_smbcmdcmn_data_setdefault(&pitem->shortfilename));
  netwib_er(netwox_smbcmdcmn_data_setdefault(&pitem->longfilename));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_find_entry_close(netwox_smbcmdtsc2_fmt_find_entry *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_close(&pitem->shortfilename));
  netwib_er(netwox_smbcmdcmn_data_close(&pitem->longfilename));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcmdtsc2_fmt_find_entry_decode_old(netwox_constsmbcmdtsc2rfmt *prfmt,
                                                               netwib_data *pdata,
                                                               netwib_uint32 *pdatasize,
                                                               netwox_smbcmdtsc2_fmt_find_entry *pitem)
{
  netwib_data data;
  netwib_uint32 datasize, shortfilenamesize, ui32;
  netwox_smbcmdcmn_timemsdos timemsdos;

  data = *pdata;
  datasize = *pdatasize;

  if (prfmt->fmtfindresumekeys) {
    if (datasize < 4) {
      return(NETWIB_ERR_DATAMISSING);
    }
    netwib__data_decode_uint32_le(data, pitem->resumekey);
    datasize -= 4;
  } else {
    pitem->resumekey= 0;
  }

  if (datasize < 24) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwox__smbcmdcmn_timemsdos_decode(data, timemsdos);
  netwib_er(netwox_time1601_init_timemsdos(timemsdos, &pitem->creationtime));
  netwox__smbcmdcmn_timemsdos_decode(data, timemsdos);
  netwib_er(netwox_time1601_init_timemsdos(timemsdos, &pitem->lastaccesstime));
  netwox__smbcmdcmn_timemsdos_decode(data, timemsdos);
  netwib_er(netwox_time1601_init_timemsdos(timemsdos, &pitem->lastwritetime));
  netwib__data_decode_uint32_le(data, ui32);
  netwib__uint64_init_uint32(ui32, pitem->filesize);
  netwib__data_decode_uint32_le(data, ui32);
  netwib__uint64_init_uint32(ui32, pitem->allocationsize);
  netwox__smbcmdcmn_fileattr16_decode(data, pitem->fileattr16);
  netwib__data_decode_uint8_le(data, shortfilenamesize);
  data++; /* reserved */
  datasize -= 24;

  /* short file name */
  if (shortfilenamesize > NETWOX_SMBCMDTSC2_SHORTFILENAME_ULEN) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize,
                                            NETWOX_SMBCMDTSC2_SHORTFILENAME_ULEN, &pitem->shortfilename));
  pitem->shortfilename.endoffset = pitem->shortfilename.beginoffset + shortfilenamesize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_find_entry_decode(netwox_constsmbcmdtsc2rfmt *prfmt,
                                                    netwib_data *pdata,
                                                    netwib_uint32 *pdatasize,
                                                    netwox_smbcmdtsc2_fmt_find_entry *pitem)
{
  netwib_data data;
  netwib_uint32 datasize, longfilenamesize, shortfilenamesize, minsize;

  pitem->fmtfind = prfmt->fmtfind;
  if (prfmt->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_STANDARD) {
    return(netwox_smbcmdtsc2_fmt_find_entry_decode_old(prfmt, pdata, pdatasize, pitem));
  }

  data = *pdata;
  datasize = *pdatasize;

  switch(prfmt->fmtfind) {
  case NETWOX_SMBCMDTSC2_FMT_FIND_DIRECTORY :     minsize = 4+52+4+0+0; break;
  case NETWOX_SMBCMDTSC2_FMT_FIND_FULLDIRECTORY : minsize = 4+52+4+4+0; break;
  case NETWOX_SMBCMDTSC2_FMT_FIND_NAMESINFO :     minsize = 4+ 0+4+0+0; break;
  case NETWOX_SMBCMDTSC2_FMT_FIND_BOTHDIRECTORY : minsize = 4+52+4+4+2; break;
  default : return(NETWIB_ERR_NOTCONVERTED);
  }

  if (datasize < minsize) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwib__data_decode_uint32_le(data, pitem->resumekey);
  if (prfmt->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_DIRECTORY ||
      prfmt->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_FULLDIRECTORY ||
      prfmt->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_BOTHDIRECTORY) {
    netwox__smbcmdcmn_time1601_decode(data, pitem->creationtime);
    netwox__smbcmdcmn_time1601_decode(data, pitem->lastaccesstime);
    netwox__smbcmdcmn_time1601_decode(data, pitem->lastwritetime);
    netwox__smbcmdcmn_time1601_decode(data, pitem->changetime);
    netwox__smbcmdcmn_uint64_decode(data, pitem->filesize);
    netwox__smbcmdcmn_uint64_decode(data, pitem->allocationsize);
    netwox__smbcmdcmn_fileattr32_decode(data, pitem->fileattr32);
  }
  netwib__data_decode_uint32_le(data, longfilenamesize);
  if (prfmt->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_FULLDIRECTORY ||
      prfmt->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_BOTHDIRECTORY) {
    netwib__data_decode_uint32_le(data, pitem->easize);
  }
  shortfilenamesize = 0;
  if (prfmt->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_BOTHDIRECTORY) {
    netwib__data_decode_uint8_le(data, shortfilenamesize);
    data++; /* reserved */
  }
  datasize -= minsize;

  /* short file name */
  if (prfmt->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_BOTHDIRECTORY) {
    if (shortfilenamesize > NETWOX_SMBCMDTSC2_SHORTFILENAME_ULEN) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize,
                                              NETWOX_SMBCMDTSC2_SHORTFILENAME_ULEN, &pitem->shortfilename));
    pitem->shortfilename.endoffset = pitem->shortfilename.beginoffset + shortfilenamesize;
  }

  /* long file name */
  netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, longfilenamesize,
                                            &pitem->longfilename));

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_find_entry_offsetfilename(netwox_constsmbcmdtsc2rfmt *prfmt,
                                                            netwib_uint32 *psize)
{

  switch(prfmt->fmtfind) {
  case NETWOX_SMBCMDTSC2_FMT_FIND_STANDARD :
    if (prfmt->fmtfindresumekeys) {
      *psize = 4 + 24;
    } else {
      *psize = 24;
    }
    break;
  case NETWOX_SMBCMDTSC2_FMT_FIND_DIRECTORY :
    *psize = 4+52+4+0+0 + 0;
    break;
  case NETWOX_SMBCMDTSC2_FMT_FIND_FULLDIRECTORY :
    *psize = 4+52+4+4+0 + 0;
    break;
  case NETWOX_SMBCMDTSC2_FMT_FIND_NAMESINFO :
    *psize = 4+ 0+4+0+0 + 0;
    break;
  case NETWOX_SMBCMDTSC2_FMT_FIND_BOTHDIRECTORY :
    *psize = 4+52+4+4+2 + NETWOX_SMBCMDTSC2_SHORTFILENAME_ULEN;
    break;
  default :
    return(NETWIB_ERR_NOTCONVERTED);
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_find_entry_size(netwox_constsmbcmdtsc2rfmt *prfmt,
                                                  netwox_smbcmdtsc2_fmt_find_entry *pitem,
                                                  netwib_uint32 *psize)
{

  switch(prfmt->fmtfind) {
  case NETWOX_SMBCMDTSC2_FMT_FIND_STANDARD :
    netwib_er(netwox_smbcmdtsc2_fmt_find_entry_size_standard(prfmt, psize));
    break;
  case NETWOX_SMBCMDTSC2_FMT_FIND_DIRECTORY :
  case NETWOX_SMBCMDTSC2_FMT_FIND_FULLDIRECTORY :
  case NETWOX_SMBCMDTSC2_FMT_FIND_NAMESINFO :
  case NETWOX_SMBCMDTSC2_FMT_FIND_BOTHDIRECTORY :
    netwib_er(netwox_smbcmdtsc2_fmt_find_entry_offsetfilename(prfmt, psize));
    *psize += netwib__buf_ref_data_size(&pitem->longfilename);
    break;
    break;
  default :
    return(NETWIB_ERR_NOTCONVERTED);
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_find_entry_size_standard(netwox_constsmbcmdtsc2rfmt *prfmt,
                                                           netwib_uint32 *psize)
{
  netwib_er(netwox_smbcmdtsc2_fmt_find_entry_offsetfilename(prfmt, psize));
  *psize += NETWOX_SMBCMDTSC2_SHORTFILENAME_ULEN;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcmdtsc2_fmt_find_entry_append_old(netwox_constsmbcmdtsc2rfmt *prfmt,
                                                               const netwox_smbcmdtsc2_fmt_find_entry *pitem,
                                                               netwib_buf *pbuf)
{
  netwib_data data;
  netwox_timemsdos timemsdos;

  if (prfmt->fmtfindresumekeys) {
    netwib_er(netwib_buf_wantspace(pbuf, 4, &data));
    netwib__data_append_uint32_le(data, pitem->resumekey);
    pbuf->endoffset += 4;
  }

  netwib_er(netwib_buf_wantspace(pbuf, 24 + NETWOX_SMBCMDTSC2_SHORTFILENAME_ULEN, &data));
  netwib_er(netwox_timemsdos_init_time1601(pitem->creationtime, &timemsdos));
  netwox__smbcmdcmn_timemsdos_append(data, timemsdos);
  netwib_er(netwox_timemsdos_init_time1601(pitem->lastaccesstime, &timemsdos));
  netwox__smbcmdcmn_timemsdos_append(data, timemsdos);
  netwib_er(netwox_timemsdos_init_time1601(pitem->lastwritetime, &timemsdos));
  netwox__smbcmdcmn_timemsdos_append(data, timemsdos);
  netwib__data_append_uint32_le(data, netwib_c2_uint64_32low(pitem->filesize));
  netwib__data_append_uint32_le(data, netwib_c2_uint64_32low(pitem->allocationsize));
  netwox__smbcmdcmn_fileattr16_append(data, pitem->fileattr16);
  netwib__data_append_uint8_le(data, netwib__buf_ref_data_size(&pitem->shortfilename));
  netwib_c_memset(data, 0, 1); data++; /* reserved */

  netwib_c_memset(data, 0, NETWOX_SMBCMDTSC2_SHORTFILENAME_ULEN);
  if (netwib__buf_ref_data_size(&pitem->shortfilename) > NETWOX_SMBCMDTSC2_SHORTFILENAME_ULEN) {
    return(NETWIB_ERR_PATOOHIGH);
  }
  netwib_c_memcpy(data, netwib__buf_ref_data_ptr(&pitem->shortfilename),
                  netwib__buf_ref_data_size(&pitem->shortfilename));
  pbuf->endoffset += 24 + NETWOX_SMBCMDTSC2_SHORTFILENAME_ULEN;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_find_entry_append(netwox_constsmbcmdtsc2rfmt *prfmt,
                                                    const netwox_smbcmdtsc2_fmt_find_entry *pitem,
                                                    netwib_buf *pbuf)
{
  netwib_data data;
  netwib_uint32 minsize;

  if (prfmt->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_STANDARD) {
    return(netwox_smbcmdtsc2_fmt_find_entry_append_old(prfmt, pitem, pbuf));
  }

  switch(prfmt->fmtfind) {
  case NETWOX_SMBCMDTSC2_FMT_FIND_DIRECTORY :     minsize = 4+52+4+0+0; break;
  case NETWOX_SMBCMDTSC2_FMT_FIND_FULLDIRECTORY : minsize = 4+52+4+4+0; break;
  case NETWOX_SMBCMDTSC2_FMT_FIND_NAMESINFO :     minsize = 4+ 0+4+0+0; break;
  case NETWOX_SMBCMDTSC2_FMT_FIND_BOTHDIRECTORY : minsize = 4+52+4+4+2; break;
  default : return(NETWIB_ERR_NOTCONVERTED);
  }

  netwib_er(netwib_buf_wantspace(pbuf, minsize, &data));
  netwib__data_append_uint32_le(data, pitem->resumekey);
  if (prfmt->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_DIRECTORY ||
      prfmt->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_FULLDIRECTORY ||
      prfmt->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_BOTHDIRECTORY) {
    netwox__smbcmdcmn_time1601_append(data, pitem->creationtime);
    netwox__smbcmdcmn_time1601_append(data, pitem->lastaccesstime);
    netwox__smbcmdcmn_time1601_append(data, pitem->lastwritetime);
    netwox__smbcmdcmn_time1601_append(data, pitem->changetime);
    netwox__smbcmdcmn_uint64_append(data, pitem->filesize);
    netwox__smbcmdcmn_uint64_append(data, pitem->allocationsize);
    netwox__smbcmdcmn_fileattr32_append(data, pitem->fileattr32);
  }
  netwib__data_append_uint32_le(data, netwib__buf_ref_data_size(&pitem->longfilename));
  if (prfmt->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_FULLDIRECTORY ||
      prfmt->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_BOTHDIRECTORY) {
    netwib__data_append_uint32_le(data, pitem->easize);
  }
  if (prfmt->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_BOTHDIRECTORY) {
    netwib__data_append_uint8_le(data, netwib__buf_ref_data_size(&pitem->shortfilename));
    netwib_c_memset(data, 0, 1); data++; /* reserved */
  }
  pbuf->endoffset += minsize;

  if (prfmt->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_BOTHDIRECTORY) {
  netwib_er(netwib_buf_wantspace(pbuf, NETWOX_SMBCMDTSC2_SHORTFILENAME_ULEN, &data));
    netwib_c_memset(data, 0, NETWOX_SMBCMDTSC2_SHORTFILENAME_ULEN);
    if (netwib__buf_ref_data_size(&pitem->shortfilename) > NETWOX_SMBCMDTSC2_SHORTFILENAME_ULEN) {
      return(NETWIB_ERR_PATOOHIGH);
    }
    netwib_c_memcpy(data, netwib__buf_ref_data_ptr(&pitem->shortfilename),
                    netwib__buf_ref_data_size(&pitem->shortfilename));
    pbuf->endoffset += NETWOX_SMBCMDTSC2_SHORTFILENAME_ULEN;
  }

  netwib_er(netwib_buf_append_buf(&pitem->longfilename, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_find_entry_show(const netwox_smbcmdtsc2_fmt_find_entry *pitem,
                                                  netwib_buf *pbuf)
{

  netwib_er(netwib_show_array_fmt32(pbuf, "  Find type:%{uint32}", pitem->fmtfind));
  netwib_er(netwib_show_array_fmt32(pbuf, "  resumekey=%{uint32}", pitem->resumekey));
  if (pitem->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_STANDARD ||
      pitem->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_DIRECTORY ||
      pitem->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_FULLDIRECTORY ||
      pitem->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_BOTHDIRECTORY) {
    netwib_er(netwox_smbcmdcmn_time1601_show("  creationtime", pitem->creationtime, pbuf));
    netwib_er(netwox_smbcmdcmn_time1601_show("  lastaccesstime", pitem->lastaccesstime, pbuf));
    netwib_er(netwox_smbcmdcmn_time1601_show("  lastwritetime", pitem->lastwritetime, pbuf));
  }
  if (pitem->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_DIRECTORY ||
      pitem->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_FULLDIRECTORY ||
      pitem->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_BOTHDIRECTORY) {
    netwib_er(netwox_smbcmdcmn_time1601_show("  changetime", pitem->changetime, pbuf));
  }
  if (pitem->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_STANDARD ||
      pitem->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_DIRECTORY ||
      pitem->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_FULLDIRECTORY ||
      pitem->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_BOTHDIRECTORY) {
    netwib_er(netwox_smbcmdcmn_uint64_show("  filesize", pitem->filesize, pbuf));
    netwib_er(netwox_smbcmdcmn_uint64_show("  allocationsize", pitem->allocationsize, pbuf));
  }
  if (pitem->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_STANDARD) {
    netwib_er(netwox_smbcmdcmn_fileattr16_show("  fileattr16", pitem->fileattr16, pbuf));
  }
  if (pitem->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_DIRECTORY ||
      pitem->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_FULLDIRECTORY ||
      pitem->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_BOTHDIRECTORY) {
    netwib_er(netwox_smbcmdcmn_fileattr32_show("  fileattr32", pitem->fileattr32, pbuf));
  }
  if (pitem->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_FULLDIRECTORY ||
      pitem->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_BOTHDIRECTORY) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  easize=%{uint32}", pitem->easize));
  }
  if (pitem->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_STANDARD ||
      pitem->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_BOTHDIRECTORY) {
    netwib_er(netwox_smbcmdcmn_data_show("  shortfilename", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &pitem->shortfilename, pbuf));
  }
  if (pitem->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_DIRECTORY ||
      pitem->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_FULLDIRECTORY ||
      pitem->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_NAMESINFO ||
      pitem->fmtfind == NETWOX_SMBCMDTSC2_FMT_FIND_BOTHDIRECTORY) {
    netwib_er(netwox_smbcmdcmn_data_show("  longfilename", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &pitem->longfilename, pbuf));
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_oldallocation_init(netwox_smbcmdtsc2_fmt_fs_oldallocation *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_oldallocation_setdefault(netwox_smbcmdtsc2_fmt_fs_oldallocation *pitem)
{
  pitem->fsid = 0;
  pitem->sectorsperunit = 0;
  pitem->totalunits = 0;
  pitem->freeunits = 0;
  pitem->bytespersector = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_oldallocation_close(netwox_smbcmdtsc2_fmt_fs_oldallocation *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_oldallocation_decode(netwib_data *pdata,
                                       netwib_uint32 *pdatasize,
                                       netwox_smbcmdtsc2_fmt_fs_oldallocation *pitem)
{
  netwib_data data;
  netwib_uint32 datasize;

  data = *pdata;
  datasize = *pdatasize;

  if (datasize < 18) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwib__data_decode_uint32_le(data, pitem->fsid);
  netwib__data_decode_uint32_le(data, pitem->sectorsperunit);
  netwib__data_decode_uint32_le(data, pitem->totalunits);
  netwib__data_decode_uint32_le(data, pitem->freeunits);
  netwib__data_decode_uint16_le(data, pitem->bytespersector);
  datasize -= 18;

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_oldallocation_append(const netwox_smbcmdtsc2_fmt_fs_oldallocation *pitem,
                                                           netwib_buf *pbuf)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(pbuf, 18, &data));
  netwib__data_append_uint32_le(data, pitem->fsid);
  netwib__data_append_uint32_le(data, pitem->sectorsperunit);
  netwib__data_append_uint32_le(data, pitem->totalunits);
  netwib__data_append_uint32_le(data, pitem->freeunits);
  netwib__data_append_uint16_le(data, pitem->bytespersector);
  pbuf->endoffset += 18;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_oldallocation_show(const netwox_smbcmdtsc2_fmt_fs_oldallocation *pitem,
                                                         netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_fmt32(pbuf, "  fsid=%{uint32}", pitem->fsid));
  netwib_er(netwib_show_array_fmt32(pbuf, "  sectorsperunit=%{uint32}", pitem->sectorsperunit));
  netwib_er(netwib_show_array_fmt32(pbuf, "  totalunits=%{uint32}", pitem->totalunits));
  netwib_er(netwib_show_array_fmt32(pbuf, "  freeunits=%{uint32}", pitem->freeunits));
  netwib_er(netwib_show_array_fmt32(pbuf, "  bytespersector=%{uint16}", pitem->bytespersector));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_oldvolume_init(netwox_smbcmdtsc2_fmt_fs_oldvolume *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_initdefault(&pitem->label));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_oldvolume_setdefault(netwox_smbcmdtsc2_fmt_fs_oldvolume *pitem)
{
  pitem->serialnumber = 0;
  netwib_er(netwox_smbcmdcmn_data_setdefault(&pitem->label));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_oldvolume_close(netwox_smbcmdtsc2_fmt_fs_oldvolume *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_close(&pitem->label));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_oldvolume_decode(netwib_data *pdata,
                                                  netwib_uint32 *pdatasize,
                                       netwox_smbcmdtsc2_fmt_fs_oldvolume *pitem)
{
  netwib_data data;
  netwib_uint32 datasize, labelsize, guessedstoragesize;

  data = *pdata;
  datasize = *pdatasize;

  if (datasize < 5) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwib__data_decode_uint32_le(data, pitem->serialnumber);
  netwib__data_decode_uint8_le(data, labelsize);
  datasize -= 5;

  if (labelsize > NETWOX_SMBCMDTSC2_VOLUME_LEN) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  /* Windows stores label in a buffer of length NETWOX_SMBCMDTSC2_VOLUME_LEN
     Samba 2 stores label in a buffer of length labelsize+1 */
  guessedstoragesize = NETWOX_SMBCMDTSC2_VOLUME_LEN;
  if (datasize < NETWOX_SMBCMDTSC2_VOLUME_LEN) {
    guessedstoragesize = labelsize + 1;
  }
  netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize,
                                            guessedstoragesize,
                                            &pitem->label));
  pitem->label.endoffset = pitem->label.beginoffset + labelsize;

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_oldvolume_append(const netwox_smbcmdtsc2_fmt_fs_oldvolume *pitem,
                                                           netwib_buf *pbuf)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(pbuf, 5+NETWOX_SMBCMDTSC2_VOLUME_LEN, &data));
  netwib__data_append_uint32_le(data, pitem->serialnumber);
  netwib__data_append_uint8_le(data, netwib__buf_ref_data_size(&pitem->label));
  netwib_c_memset(data, 0, NETWOX_SMBCMDTSC2_VOLUME_LEN);
  if (netwib__buf_ref_data_size(&pitem->label) >NETWOX_SMBCMDTSC2_VOLUME_LEN) {
    return(NETWIB_ERR_PATOOHIGH);
  }
  netwib_c_memcpy(data, netwib__buf_ref_data_ptr(&pitem->label),
                  netwib__buf_ref_data_size(&pitem->label));
  pbuf->endoffset += 5 + NETWOX_SMBCMDTSC2_VOLUME_LEN;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_oldvolume_show(const netwox_smbcmdtsc2_fmt_fs_oldvolume *pitem,
                                                         netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_fmt32(pbuf, "  serialnumber=%{uint32:#08X}", pitem->serialnumber));
  netwib_er(netwox_smbcmdcmn_data_show("  label", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &pitem->label, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_volume_init(netwox_smbcmdtsc2_fmt_fs_volume *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_initdefault(&pitem->label));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_volume_setdefault(netwox_smbcmdtsc2_fmt_fs_volume *pitem)
{
  netwox__smbcmdcmn_time1601_initdefault(pitem->creationtime);
  pitem->serialnumber = 0;

  netwib_er(netwox_smbcmdcmn_data_setdefault(&pitem->label));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_volume_close(netwox_smbcmdtsc2_fmt_fs_volume *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_close(&pitem->label));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_volume_decode(netwib_data *pdata,
                                                  netwib_uint32 *pdatasize,
                                       netwox_smbcmdtsc2_fmt_fs_volume *pitem)
{
  netwib_data data;
  netwib_uint32 datasize, labelsize;

  data = *pdata;
  datasize = *pdatasize;

  if (datasize < 18) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwox__smbcmdcmn_time1601_decode(data, pitem->creationtime);
  netwib__data_decode_uint32_le(data, pitem->serialnumber);
  netwib__data_decode_uint32_le(data, labelsize);
  data += 2; /* reserved */
  datasize -= 18;

  netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, labelsize,
                                            &pitem->label));

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_volume_append(const netwox_smbcmdtsc2_fmt_fs_volume *pitem,
                                                           netwib_buf *pbuf)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(pbuf, 18, &data));
  netwox__smbcmdcmn_time1601_append(data, pitem->creationtime);
  netwib__data_append_uint32_le(data, pitem->serialnumber);
  netwib__data_append_uint32_le(data, netwib__buf_ref_data_size(&pitem->label));
  netwib_c_memset(data, 0, 2); data += 2; /* reserved */
  pbuf->endoffset += 18;
  netwib_er(netwib_buf_append_buf(&pitem->label, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_volume_show(const netwox_smbcmdtsc2_fmt_fs_volume *pitem,
                                                         netwib_buf *pbuf)
{
  netwib_er(netwox_smbcmdcmn_time1601_show("  creationtime", pitem->creationtime, pbuf));
  netwib_er(netwib_show_array_fmt32(pbuf, "  serialnumber=%{uint32:#08X}", pitem->serialnumber));
  netwib_er(netwox_smbcmdcmn_data_show("  label", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &pitem->label, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_size_init(netwox_smbcmdtsc2_fmt_fs_size *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_size_setdefault(netwox_smbcmdtsc2_fmt_fs_size *pitem)
{
  netwox__smbcmdcmn_uint64_initdefault(pitem->totalunits);
  netwox__smbcmdcmn_uint64_initdefault(pitem->freeunits);
  pitem->sectorsperunit = 0;
  pitem->bytespersector = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_size_close(netwox_smbcmdtsc2_fmt_fs_size *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_size_decode(netwib_data *pdata,
                                                netwib_uint32 *pdatasize,
                                       netwox_smbcmdtsc2_fmt_fs_size *pitem)
{
  netwib_data data;
  netwib_uint32 datasize;

  data = *pdata;
  datasize = *pdatasize;

  if (datasize < 24) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwox__smbcmdcmn_uint64_decode(data, pitem->totalunits);
  netwox__smbcmdcmn_uint64_decode(data, pitem->freeunits);
  netwib__data_decode_uint32_le(data, pitem->sectorsperunit);
  netwib__data_decode_uint32_le(data, pitem->bytespersector);
  datasize -= 24;

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_size_append(const netwox_smbcmdtsc2_fmt_fs_size *pitem,
                                                           netwib_buf *pbuf)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(pbuf, 24, &data));
  netwox__smbcmdcmn_uint64_append(data, pitem->totalunits);
  netwox__smbcmdcmn_uint64_append(data, pitem->freeunits);
  netwib__data_append_uint32_le(data, pitem->sectorsperunit);
  netwib__data_append_uint32_le(data, pitem->bytespersector);
  pbuf->endoffset += 24;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_size_show(const netwox_smbcmdtsc2_fmt_fs_size *pitem,
                                                         netwib_buf *pbuf)
{
  netwib_er(netwox_smbcmdcmn_uint64_show("  totalunits", pitem->totalunits, pbuf));
  netwib_er(netwox_smbcmdcmn_uint64_show("  freeunits", pitem->freeunits, pbuf));
  netwib_er(netwib_show_array_fmt32(pbuf, "  sectorsperunit=%{uint32}", pitem->sectorsperunit));
  netwib_er(netwib_show_array_fmt32(pbuf, "  bytespersector=%{uint32}", pitem->bytespersector));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_device_init(netwox_smbcmdtsc2_fmt_fs_device *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_device_setdefault(netwox_smbcmdtsc2_fmt_fs_device *pitem)
{
  pitem->devicetype = NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_DISK;
  pitem->devicecharacteristics = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_device_close(netwox_smbcmdtsc2_fmt_fs_device *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_device_decode(netwib_data *pdata,
                                                  netwib_uint32 *pdatasize,
                                       netwox_smbcmdtsc2_fmt_fs_device *pitem)
{
  netwib_data data;
  netwib_uint32 datasize;

  data = *pdata;
  datasize = *pdatasize;

  if (datasize < 8) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwib__data_decode_uint32_le(data, pitem->devicetype);
  netwib__data_decode_uint32_le(data, pitem->devicecharacteristics);
  datasize -= 8;

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_device_append(const netwox_smbcmdtsc2_fmt_fs_device *pitem,
                                                           netwib_buf *pbuf)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(pbuf, 8, &data));
  netwib__data_append_uint32_le(data, pitem->devicetype);
  netwib__data_append_uint32_le(data, pitem->devicecharacteristics);
  pbuf->endoffset += 8;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_device_show(const netwox_smbcmdtsc2_fmt_fs_device *pitem,
                                                         netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_fmt32(pbuf, "  devicetype=%{uint32}", pitem->devicetype));
  netwib_er(netwib_show_array_fmt32(pbuf, "  devicecharacteristics=%{uint32:#32b}", pitem->devicecharacteristics));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_attribute_init(netwox_smbcmdtsc2_fmt_fs_attribute *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_initdefault(&pitem->fsname));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_attribute_setdefault(netwox_smbcmdtsc2_fmt_fs_attribute *pitem)
{
  pitem->fsattributes = 0;
  pitem->maxnamesize = 0;

  netwib_er(netwox_smbcmdcmn_data_setdefault(&pitem->fsname));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_attribute_close(netwox_smbcmdtsc2_fmt_fs_attribute *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_close(&pitem->fsname));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_attribute_decode(netwib_data *pdata,
                             netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_fs_attribute *pitem)
{
  netwib_data data;
  netwib_uint32 datasize, fsnamesize;

  data = *pdata;
  datasize = *pdatasize;

  if (datasize < 12) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwib__data_decode_uint32_le(data, pitem->fsattributes);
  netwib__data_decode_uint32_le(data, pitem->maxnamesize);
  netwib__data_decode_uint32_le(data, fsnamesize);
  datasize -= 12;

  netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, fsnamesize,
                                            &pitem->fsname));

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_attribute_append(const netwox_smbcmdtsc2_fmt_fs_attribute *pitem,
                                                           netwib_buf *pbuf)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(pbuf, 12, &data));
  netwib__data_append_uint32_le(data, pitem->fsattributes);
  netwib__data_append_uint32_le(data, pitem->maxnamesize);
  netwib__data_append_uint32_le(data, netwib__buf_ref_data_size(&pitem->fsname));
  pbuf->endoffset += 12;
  netwib_er(netwib_buf_append_buf(&pitem->fsname, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_attribute_show(const netwox_smbcmdtsc2_fmt_fs_attribute *pitem,
                                                         netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_fmt32(pbuf, "  fsattributes=%{uint32:#32b}", pitem->fsattributes));
  netwib_er(netwib_show_array_fmt32(pbuf, "  maxnamesize=%{uint32}", pitem->maxnamesize));
  netwib_er(netwox_smbcmdcmn_data_show("  fsname", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &pitem->fsname, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_efullsize_init(netwox_smbcmdtsc2_fmt_fs_efullsize *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_efullsize_setdefault(netwox_smbcmdtsc2_fmt_fs_efullsize *pitem)
{
  netwox__smbcmdcmn_uint64_initdefault(pitem->totalunits);
  netwox__smbcmdcmn_uint64_initdefault(pitem->callerfreeunits);
  netwox__smbcmdcmn_uint64_initdefault(pitem->actualfreeunits);
  pitem->sectorsperunit = 0;
  pitem->bytespersector = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_efullsize_close(netwox_smbcmdtsc2_fmt_fs_efullsize *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_efullsize_decode(netwib_data *pdata,
                                                netwib_uint32 *pdatasize,
                                       netwox_smbcmdtsc2_fmt_fs_efullsize *pitem)
{
  netwib_data data;
  netwib_uint32 datasize;

  data = *pdata;
  datasize = *pdatasize;

  if (datasize < 32) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwox__smbcmdcmn_uint64_decode(data, pitem->totalunits);
  netwox__smbcmdcmn_uint64_decode(data, pitem->callerfreeunits);
  netwox__smbcmdcmn_uint64_decode(data, pitem->actualfreeunits);
  netwib__data_decode_uint32_le(data, pitem->sectorsperunit);
  netwib__data_decode_uint32_le(data, pitem->bytespersector);
  datasize -= 32;

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_efullsize_append(const netwox_smbcmdtsc2_fmt_fs_efullsize *pitem,
                                                           netwib_buf *pbuf)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(pbuf, 32, &data));
  netwox__smbcmdcmn_uint64_append(data, pitem->totalunits);
  netwox__smbcmdcmn_uint64_append(data, pitem->callerfreeunits);
  netwox__smbcmdcmn_uint64_append(data, pitem->actualfreeunits);
  netwib__data_append_uint32_le(data, pitem->sectorsperunit);
  netwib__data_append_uint32_le(data, pitem->bytespersector);
  pbuf->endoffset += 32;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_efullsize_show(const netwox_smbcmdtsc2_fmt_fs_efullsize *pitem,
                                                         netwib_buf *pbuf)
{
  netwib_er(netwox_smbcmdcmn_uint64_show("  totalunits", pitem->totalunits, pbuf));
  netwib_er(netwox_smbcmdcmn_uint64_show("  callerfreeunits", pitem->callerfreeunits, pbuf));
  netwib_er(netwox_smbcmdcmn_uint64_show("  actualfreeunits", pitem->actualfreeunits, pbuf));
  netwib_er(netwib_show_array_fmt32(pbuf, "  sectorsperunit=%{uint32}", pitem->sectorsperunit));
  netwib_er(netwib_show_array_fmt32(pbuf, "  bytespersector=%{uint32}", pitem->bytespersector));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_eobjectid_init(netwox_smbcmdtsc2_fmt_fs_eobjectid *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_initdefault(&pitem->guid));
  netwib_er(netwox_smbcmdcmn_data_initdefault(&pitem->unknown));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_eobjectid_setdefault(netwox_smbcmdtsc2_fmt_fs_eobjectid *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_setdefault(&pitem->guid));
  netwib_er(netwox_smbcmdcmn_data_setdefault(&pitem->unknown));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_eobjectid_close(netwox_smbcmdtsc2_fmt_fs_eobjectid *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_close(&pitem->guid));
  netwib_er(netwox_smbcmdcmn_data_close(&pitem->unknown));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_eobjectid_decode(netwib_data *pdata,
                                                     netwib_uint32 *pdatasize,
                                       netwox_smbcmdtsc2_fmt_fs_eobjectid *pitem)
{
  netwib_data data;
  netwib_uint32 datasize;

  data = *pdata;
  datasize = *pdatasize;

  netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, 16,
                                            &pitem->guid));
  netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, datasize,
                                            &pitem->unknown));

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_eobjectid_append(const netwox_smbcmdtsc2_fmt_fs_eobjectid *pitem,
                                                           netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_buf(&pitem->guid, pbuf));
  netwib_er(netwib_buf_append_buf(&pitem->unknown, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_fs_eobjectid_show(const netwox_smbcmdtsc2_fmt_fs_eobjectid *pitem,
                                                         netwib_buf *pbuf)
{
  netwib_er(netwox_smbcmdcmn_data_show("  guid", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &pitem->guid, pbuf));
  netwib_er(netwox_smbcmdcmn_data_show("  unknown", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &pitem->unknown, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_basic_init(netwox_smbcmdtsc2_fmt_file_basic *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_basic_setdefault(netwox_smbcmdtsc2_fmt_file_basic *pitem)
{
  netwox__smbcmdcmn_time1601_initdefault(pitem->creationtime);
  netwox__smbcmdcmn_time1601_initdefault(pitem->lastaccesstime);
  netwox__smbcmdcmn_time1601_initdefault(pitem->lastwritetime);
  netwox__smbcmdcmn_time1601_initdefault(pitem->changetime);
  netwox__smbcmdcmn_fileattr32_initdefault(pitem->fileattributes);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_basic_close(netwox_smbcmdtsc2_fmt_file_basic *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_basic_decode(netwib_data *pdata,
                                                   netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_file_basic *pitem)
{
  netwib_data data;
  netwib_uint32 datasize, unused;

  data = *pdata;
  datasize = *pdatasize;

  if (datasize < 36) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwox__smbcmdcmn_time1601_decode(data, pitem->creationtime);
  netwox__smbcmdcmn_time1601_decode(data, pitem->lastaccesstime);
  netwox__smbcmdcmn_time1601_decode(data, pitem->lastwritetime);
  netwox__smbcmdcmn_time1601_decode(data, pitem->changetime);
  netwox__smbcmdcmn_fileattr32_decode(data, pitem->fileattributes);
  datasize -= 36;

  /* there is an optional unused field */
  if (datasize >= 4) {
    netwib__data_decode_uint32_le(data, unused);
    datasize -= 4;
  }

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_basic_append(const netwox_smbcmdtsc2_fmt_file_basic *pitem,
                                                   netwib_buf *pbuf)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(pbuf, 40, &data));
  netwox__smbcmdcmn_time1601_append(data, pitem->creationtime);
  netwox__smbcmdcmn_time1601_append(data, pitem->lastaccesstime);
  netwox__smbcmdcmn_time1601_append(data, pitem->lastwritetime);
  netwox__smbcmdcmn_time1601_append(data, pitem->changetime);
  netwox__smbcmdcmn_fileattr32_append(data, pitem->fileattributes);
  /* unused value */
  netwib__data_append_uint32_le(data, 0);
  pbuf->endoffset += 40;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_basic_show(const netwox_smbcmdtsc2_fmt_file_basic *pitem,
                                                 netwib_buf *pbuf)
{
  netwib_er(netwox_smbcmdcmn_time1601_show("  creationtime", pitem->creationtime, pbuf));
  netwib_er(netwox_smbcmdcmn_time1601_show("  lastaccesstime", pitem->lastaccesstime, pbuf));
  netwib_er(netwox_smbcmdcmn_time1601_show("  lastwritetime", pitem->lastwritetime, pbuf));
  netwib_er(netwox_smbcmdcmn_time1601_show("  changetime", pitem->changetime, pbuf));
  netwib_er(netwox_smbcmdcmn_fileattr32_show("  fileattributes", pitem->fileattributes, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_standard_init(netwox_smbcmdtsc2_fmt_file_standard *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_standard_setdefault(netwox_smbcmdtsc2_fmt_file_standard *pitem)
{
  netwox__smbcmdcmn_uint64_initdefault(pitem->allocationsize);
  netwox__smbcmdcmn_uint64_initdefault(pitem->filesize);
  pitem->linkcount = 1;
  pitem->deletepending = 0;
  pitem->isdirectory = NETWIB_FALSE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_standard_close(netwox_smbcmdtsc2_fmt_file_standard *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_standard_decode(netwib_data *pdata,
                                                   netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_file_standard *pitem)
{
  netwib_data data;
  netwib_uint32 datasize;

  data = *pdata;
  datasize = *pdatasize;

  if (datasize < 24) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwox__smbcmdcmn_uint64_decode(data, pitem->allocationsize);
  netwox__smbcmdcmn_uint64_decode(data, pitem->filesize);
  netwib__data_decode_uint32_le(data, pitem->linkcount);
  netwib__data_decode_uint16_le(data, pitem->deletepending);
  netwib__data_decode_uint8_le(data, pitem->isdirectory);
  data++; /* reserved */
  datasize -= 24;

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_standard_append(const netwox_smbcmdtsc2_fmt_file_standard *pitem,
                                                   netwib_buf *pbuf)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(pbuf, 24, &data));
  netwox__smbcmdcmn_uint64_append(data, pitem->allocationsize);
  netwox__smbcmdcmn_uint64_append(data, pitem->filesize);
  netwib__data_append_uint32_le(data, pitem->linkcount);
  netwib__data_append_uint16_le(data, pitem->deletepending);
  if (pitem->isdirectory) {
    netwib__data_append_uint8_le(data, 1);
  } else {
    netwib__data_append_uint8_le(data, 0);
  }
  netwib_c_memset(data, 0, 1); /* reserved */
  pbuf->endoffset += 24;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_standard_show(const netwox_smbcmdtsc2_fmt_file_standard *pitem,
                                                 netwib_buf *pbuf)
{
  netwib_er(netwox_smbcmdcmn_uint64_show("  allocationsize", pitem->allocationsize, pbuf));
  netwib_er(netwox_smbcmdcmn_uint64_show("  filesize", pitem->filesize, pbuf));
  netwib_er(netwib_show_array_fmt32(pbuf, "  linkcount=%{uint32}", pitem->linkcount));
  netwib_er(netwib_show_array_fmt32(pbuf, "  deletepending=%{uint16}", pitem->deletepending));
  netwib_er(netwib_show_array_fmt32(pbuf, "  isdirectory=%{bool}", pitem->isdirectory));


  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_ea_init(netwox_smbcmdtsc2_fmt_file_ea *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_ea_setdefault(netwox_smbcmdtsc2_fmt_file_ea *pitem)
{
  pitem->easize = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_ea_close(netwox_smbcmdtsc2_fmt_file_ea *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_ea_decode(netwib_data *pdata,
                                                   netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_file_ea *pitem)
{
  netwib_data data;
  netwib_uint32 datasize;

  data = *pdata;
  datasize = *pdatasize;

  if (datasize < 4) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwib__data_decode_uint32_le(data, pitem->easize);
  datasize -= 4;

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_ea_append(const netwox_smbcmdtsc2_fmt_file_ea *pitem,
                                                   netwib_buf *pbuf)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(pbuf, 4, &data));
  netwib__data_append_uint32_le(data, pitem->easize);
  pbuf->endoffset += 4;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_ea_show(const netwox_smbcmdtsc2_fmt_file_ea *pitem,
                                                 netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_fmt32(pbuf, "  easize=%{uint32}", pitem->easize));


  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_name_init(netwox_smbcmdtsc2_fmt_file_name *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_initdefault(&pitem->filename));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_name_setdefault(netwox_smbcmdtsc2_fmt_file_name *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_setdefault(&pitem->filename));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_name_close(netwox_smbcmdtsc2_fmt_file_name *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_close(&pitem->filename));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_name_decode(netwib_data *pdata,
                                                     netwib_uint32 *pdatasize,
                                       netwox_smbcmdtsc2_fmt_file_name *pitem)
{
  netwib_data data;
  netwib_uint32 datasize, filenamesize;

  data = *pdata;
  datasize = *pdatasize;

  if (datasize < 4) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwib__data_decode_uint32_le(data, filenamesize);
  datasize -= 4;

  if (datasize == 4 && filenamesize == 1089) {
    /* SETFILEINFO sometimes uses an empty name. Correct it. */
    data += 2;
    datasize -= 2;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&pitem->filename));
  } else {
    netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, filenamesize,
                                              &pitem->filename));
  }

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_name_append(const netwox_smbcmdtsc2_fmt_file_name *pitem,
                                                     netwib_buf *pbuf)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(pbuf, 4, &data));
  netwib__data_append_uint32_le(data, netwib__buf_ref_data_size(&pitem->filename));
  pbuf->endoffset += 4;
  netwib_er(netwib_buf_append_buf(&pitem->filename, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_name_show(const netwox_smbcmdtsc2_fmt_file_name *pitem,
                                                         netwib_buf *pbuf)
{
  netwib_er(netwox_smbcmdcmn_data_show("  filename", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &pitem->filename, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_allocation_init(netwox_smbcmdtsc2_fmt_file_allocation *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_allocation_setdefault(netwox_smbcmdtsc2_fmt_file_allocation *pitem)
{
  netwox__smbcmdcmn_uint64_initdefault(pitem->allocationsize);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_allocation_close(netwox_smbcmdtsc2_fmt_file_allocation *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_allocation_decode(netwib_data *pdata,
                                                   netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_file_allocation *pitem)
{
  netwib_data data;
  netwib_uint32 datasize;

  data = *pdata;
  datasize = *pdatasize;

  if (datasize < 8) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwox__smbcmdcmn_uint64_decode(data, pitem->allocationsize);
  datasize -= 8;

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_allocation_append(const netwox_smbcmdtsc2_fmt_file_allocation *pitem,
                                                        netwib_buf *pbuf)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(pbuf, 8, &data));
  netwox__smbcmdcmn_uint64_append(data, pitem->allocationsize);
  pbuf->endoffset += 8;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_allocation_show(const netwox_smbcmdtsc2_fmt_file_allocation *pitem,
                                                      netwib_buf *pbuf)
{
  netwib_er(netwox_smbcmdcmn_uint64_show("  allocationsize", pitem->allocationsize, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_eof_init(netwox_smbcmdtsc2_fmt_file_eof *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_eof_setdefault(netwox_smbcmdtsc2_fmt_file_eof *pitem)
{
  netwox__smbcmdcmn_uint64_initdefault(pitem->filesize);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_eof_close(netwox_smbcmdtsc2_fmt_file_eof *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_eof_decode(netwib_data *pdata,
                                                   netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_file_eof *pitem)
{
  netwib_data data;
  netwib_uint32 datasize;

  data = *pdata;
  datasize = *pdatasize;

  if (datasize < 8) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwox__smbcmdcmn_uint64_decode(data, pitem->filesize);
  datasize -= 8;

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_eof_append(const netwox_smbcmdtsc2_fmt_file_eof *pitem,
                                                        netwib_buf *pbuf)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(pbuf, 8, &data));
  netwox__smbcmdcmn_uint64_append(data, pitem->filesize);
  pbuf->endoffset += 8;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_eof_show(const netwox_smbcmdtsc2_fmt_file_eof *pitem,
                                                      netwib_buf *pbuf)
{
  netwib_er(netwox_smbcmdcmn_uint64_show("  filesize", pitem->filesize, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_all_init(netwox_smbcmdtsc2_fmt_file_all *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_initdefault(&pitem->filename));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_all_setdefault(netwox_smbcmdtsc2_fmt_file_all *pitem)
{
  netwox__smbcmdcmn_time1601_initdefault(pitem->creationtime);
  netwox__smbcmdcmn_time1601_initdefault(pitem->lastaccesstime);
  netwox__smbcmdcmn_time1601_initdefault(pitem->lastwritetime);
  netwox__smbcmdcmn_time1601_initdefault(pitem->changetime);
  netwox__smbcmdcmn_fileattr32_initdefault(pitem->fileattributes);
  pitem->unknown1 = 0;
  netwox__smbcmdcmn_uint64_initdefault(pitem->allocationsize);
  netwox__smbcmdcmn_uint64_initdefault(pitem->filesize);
  pitem->linkcount = 1;
  pitem->deletepending = NETWIB_FALSE;
  pitem->isdirectory = NETWIB_FALSE;
  pitem->unknown2 = 0;
  pitem->easize = 0;

  netwib_er(netwox_smbcmdcmn_data_setdefault(&pitem->filename));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_all_close(netwox_smbcmdtsc2_fmt_file_all *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_close(&pitem->filename));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_all_decode(netwib_data *pdata,
                                                   netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_file_all *pitem)
{
  netwib_data data;
  netwib_uint32 datasize, filenamesize;

  data = *pdata;
  datasize = *pdatasize;

  if (datasize < 72) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwox__smbcmdcmn_time1601_decode(data, pitem->creationtime);
  netwox__smbcmdcmn_time1601_decode(data, pitem->lastaccesstime);
  netwox__smbcmdcmn_time1601_decode(data, pitem->lastwritetime);
  netwox__smbcmdcmn_time1601_decode(data, pitem->changetime);
  netwox__smbcmdcmn_fileattr32_decode(data, pitem->fileattributes);
  netwib__data_decode_uint32_le(data, pitem->unknown1);
  netwox__smbcmdcmn_uint64_decode(data, pitem->allocationsize);
  netwox__smbcmdcmn_uint64_decode(data, pitem->filesize);
  netwib__data_decode_uint32_le(data, pitem->linkcount);
  netwib__data_decode_uint8_le(data, pitem->deletepending);
  netwib__data_decode_uint8_le(data, pitem->isdirectory);
  netwib__data_decode_uint16_le(data, pitem->unknown2);
  netwib__data_decode_uint32_le(data, pitem->easize);
  netwib__data_decode_uint32_le(data, filenamesize);
  datasize -= 72;

  netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, filenamesize,
                                            &pitem->filename));

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_all_append(const netwox_smbcmdtsc2_fmt_file_all *pitem,
                                                        netwib_buf *pbuf)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(pbuf, 72, &data));
  netwox__smbcmdcmn_time1601_append(data, pitem->creationtime);
  netwox__smbcmdcmn_time1601_append(data, pitem->lastaccesstime);
  netwox__smbcmdcmn_time1601_append(data, pitem->lastwritetime);
  netwox__smbcmdcmn_time1601_append(data, pitem->changetime);
  netwox__smbcmdcmn_fileattr32_append(data, pitem->fileattributes);
  netwib__data_append_uint32_le(data, pitem->unknown1);
  netwox__smbcmdcmn_uint64_append(data, pitem->allocationsize);
  netwox__smbcmdcmn_uint64_append(data, pitem->filesize);
  netwib__data_append_uint32_le(data, pitem->linkcount);
  netwib__data_append_uint8_le(data, pitem->deletepending?1:0);
  netwib__data_append_uint8_le(data, pitem->isdirectory?1:0);
  netwib__data_append_uint16_le(data, pitem->unknown2);
  netwib__data_append_uint32_le(data, pitem->easize);
  netwib__data_append_uint32_le(data, netwib__buf_ref_data_size(&pitem->filename));
  pbuf->endoffset += 72;

  netwib_er(netwib_buf_append_buf(&pitem->filename, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_all_show(const netwox_smbcmdtsc2_fmt_file_all *pitem,
                                                      netwib_buf *pbuf)
{
  netwib_er(netwox_smbcmdcmn_time1601_show("  creationtime", pitem->creationtime, pbuf));
  netwib_er(netwox_smbcmdcmn_time1601_show("  lastaccesstime", pitem->lastaccesstime, pbuf));
  netwib_er(netwox_smbcmdcmn_time1601_show("  lastwritetime", pitem->lastwritetime, pbuf));
  netwib_er(netwox_smbcmdcmn_time1601_show("  changetime", pitem->changetime, pbuf));
  netwib_er(netwox_smbcmdcmn_fileattr32_show("  fileattributes", pitem->fileattributes, pbuf));
  netwib_er(netwib_show_array_fmt32(pbuf, "  unknown1=%{uint32}", pitem->unknown1));
  netwib_er(netwox_smbcmdcmn_uint64_show("  allocationsize", pitem->allocationsize, pbuf));
  netwib_er(netwox_smbcmdcmn_uint64_show("  filesize", pitem->filesize, pbuf));
  netwib_er(netwib_show_array_fmt32(pbuf, "  linkcount=%{uint32}", pitem->linkcount));
  netwib_er(netwib_show_array_fmt32(pbuf, "  deletepending=%{bool}", pitem->deletepending));
  netwib_er(netwib_show_array_fmt32(pbuf, "  isdirectory=%{bool}", pitem->isdirectory));
  netwib_er(netwib_show_array_fmt32(pbuf, "  unknown2=%{uint16}", pitem->unknown2));
  netwib_er(netwib_show_array_fmt32(pbuf, "  easize=%{uint32}", pitem->easize));

  netwib_er(netwox_smbcmdcmn_data_show("  filename", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &pitem->filename, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_altname_init(netwox_smbcmdtsc2_fmt_file_altname *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_initdefault(&pitem->filename));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_altname_setdefault(netwox_smbcmdtsc2_fmt_file_altname *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_setdefault(&pitem->filename));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_altname_close(netwox_smbcmdtsc2_fmt_file_altname *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_close(&pitem->filename));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_altname_decode(netwib_data *pdata,
                                                     netwib_uint32 *pdatasize,
                                       netwox_smbcmdtsc2_fmt_file_altname *pitem)
{
  netwib_data data;
  netwib_uint32 datasize, filenamesize;

  data = *pdata;
  datasize = *pdatasize;

  if (datasize < 4) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwib__data_decode_uint32_le(data, filenamesize);
  datasize -= 4;

  netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, filenamesize,
                                            &pitem->filename));

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_altname_append(const netwox_smbcmdtsc2_fmt_file_altname *pitem,
                                                     netwib_buf *pbuf)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(pbuf, 4, &data));
  netwib__data_append_uint32_le(data, netwib__buf_ref_data_size(&pitem->filename));
  pbuf->endoffset += 4;
  netwib_er(netwib_buf_append_buf(&pitem->filename, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_altname_show(const netwox_smbcmdtsc2_fmt_file_altname *pitem,
                                                         netwib_buf *pbuf)
{
  netwib_er(netwox_smbcmdcmn_data_show("  filename", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &pitem->filename, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_stream_init(netwox_smbcmdtsc2_fmt_file_stream *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_initdefault(&pitem->name));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_stream_setdefault(netwox_smbcmdtsc2_fmt_file_stream *pitem)
{
  netwox__smbcmdcmn_uint64_initdefault(pitem->streamsize);
  netwox__smbcmdcmn_uint64_initdefault(pitem->allocationsize);

  netwib_er(netwox_smbcmdcmn_data_setdefault(&pitem->name));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_stream_close(netwox_smbcmdtsc2_fmt_file_stream *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_close(&pitem->name));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_stream_decode(netwib_data *pdata,
                             netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_file_stream *pitem)
{
  netwib_data data;
  netwib_uint32 datasize, namesize;

  data = *pdata;
  datasize = *pdatasize;

  if (datasize < 20) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwib__data_decode_uint32_le(data, namesize);
  netwox__smbcmdcmn_uint64_decode(data, pitem->streamsize);
  netwox__smbcmdcmn_uint64_decode(data, pitem->allocationsize);
  datasize -= 20;

  netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, namesize,
                                            &pitem->name));

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_stream_size(netwox_smbcmdtsc2_fmt_file_stream *pitem,
                                                         netwib_uint32 *psize)
{
  *psize = 20 + netwib__buf_ref_data_size(&pitem->name);
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_stream_append(const netwox_smbcmdtsc2_fmt_file_stream *pitem,
                                                     netwib_buf *pbuf)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(pbuf, 20, &data));
  netwib__data_append_uint32_le(data, netwib__buf_ref_data_size(&pitem->name));
  netwox__smbcmdcmn_uint64_append(data, pitem->streamsize);
  netwox__smbcmdcmn_uint64_append(data, pitem->allocationsize);
  pbuf->endoffset += 20;
  netwib_er(netwib_buf_append_buf(&pitem->name, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_stream_show(const netwox_smbcmdtsc2_fmt_file_stream *pitem,
                                                   netwib_buf *pbuf)
{
  netwib_er(netwox_smbcmdcmn_uint64_show("  streamsize", pitem->streamsize, pbuf));
  netwib_er(netwox_smbcmdcmn_uint64_show("  allocationsize", pitem->allocationsize, pbuf));
  netwib_er(netwox_smbcmdcmn_data_show("  name", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &pitem->name, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_disposition_init(netwox_smbcmdtsc2_fmt_file_disposition *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_disposition_setdefault(netwox_smbcmdtsc2_fmt_file_disposition *pitem)
{
  pitem->deleteonclose = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_disposition_close(netwox_smbcmdtsc2_fmt_file_disposition *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_disposition_decode(netwib_data *pdata,
                                                   netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_file_disposition *pitem)
{
  netwib_data data;
  netwib_uint32 datasize;
  netwib_byte b;

  data = *pdata;
  datasize = *pdatasize;

  if (datasize < 1) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwib__data_decode_uint8_le(data, b);
  pitem->deleteonclose = b?NETWIB_TRUE:NETWIB_FALSE;
  datasize -= 1;

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_disposition_append(const netwox_smbcmdtsc2_fmt_file_disposition *pitem,
                                                   netwib_buf *pbuf)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(pbuf, 1, &data));
  netwib__data_append_uint8_le(data, (pitem->deleteonclose?0xFF:0));
  pbuf->endoffset += 1;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_disposition_show(const netwox_smbcmdtsc2_fmt_file_disposition *pitem,
                                                 netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_fmt32(pbuf, "  deleteonclose=%{bool}", pitem->deleteonclose));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_einternal_init(netwox_smbcmdtsc2_fmt_file_einternal *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_einternal_setdefault(netwox_smbcmdtsc2_fmt_file_einternal *pitem)
{
  pitem->dev = 0;
  pitem->ino = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_einternal_close(netwox_smbcmdtsc2_fmt_file_einternal *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_einternal_decode(netwib_data *pdata,
                                                   netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_file_einternal *pitem)
{
  netwib_data data;
  netwib_uint32 datasize;

  data = *pdata;
  datasize = *pdatasize;

  if (datasize < 8) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwib__data_decode_uint32_le(data, pitem->dev);
  netwib__data_decode_uint32_le(data, pitem->ino);
  datasize -= 8;

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_einternal_append(const netwox_smbcmdtsc2_fmt_file_einternal *pitem,
                                                   netwib_buf *pbuf)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(pbuf, 8, &data));
  netwib__data_append_uint32_le(data, pitem->dev);
  netwib__data_append_uint32_le(data, pitem->ino);
  pbuf->endoffset += 8;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_einternal_show(const netwox_smbcmdtsc2_fmt_file_einternal *pitem,
                                                 netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_fmt32(pbuf, "  dev=%{uint32}", pitem->dev));
  netwib_er(netwib_show_array_fmt32(pbuf, "  ino=%{uint32}", pitem->ino));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_enetworkopen_init(netwox_smbcmdtsc2_fmt_file_enetworkopen *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_enetworkopen_setdefault(netwox_smbcmdtsc2_fmt_file_enetworkopen *pitem)
{
  netwox__smbcmdcmn_time1601_initdefault(pitem->creationtime);
  netwox__smbcmdcmn_time1601_initdefault(pitem->lastaccesstime);
  netwox__smbcmdcmn_time1601_initdefault(pitem->lastwritetime);
  netwox__smbcmdcmn_time1601_initdefault(pitem->changetime);
  netwox__smbcmdcmn_uint64_initdefault(pitem->allocationsize);
  netwox__smbcmdcmn_uint64_initdefault(pitem->filesize);
  netwox__smbcmdcmn_fileattr32_initdefault(pitem->fileattributes);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_enetworkopen_close(netwox_smbcmdtsc2_fmt_file_enetworkopen *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_enetworkopen_decode(netwib_data *pdata,
                                                     netwib_uint32 *pdatasize,
                                       netwox_smbcmdtsc2_fmt_file_enetworkopen *pitem)
{
  netwib_data data;
  netwib_uint32 datasize, unused;

  data = *pdata;
  datasize = *pdatasize;

  if (datasize < 56) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwox__smbcmdcmn_time1601_decode(data, pitem->creationtime);
  netwox__smbcmdcmn_time1601_decode(data, pitem->lastaccesstime);
  netwox__smbcmdcmn_time1601_decode(data, pitem->lastwritetime);
  netwox__smbcmdcmn_time1601_decode(data, pitem->changetime);
  netwox__smbcmdcmn_uint64_decode(data, pitem->allocationsize);
  netwox__smbcmdcmn_uint64_decode(data, pitem->filesize);
  netwox__smbcmdcmn_fileattr32_decode(data, pitem->fileattributes);
  netwib__data_decode_uint32_le(data, unused);
  datasize -= 56;

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_enetworkopen_append(const netwox_smbcmdtsc2_fmt_file_enetworkopen *pitem,
                                                          netwib_buf *pbuf)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(pbuf, 56, &data));
  netwox__smbcmdcmn_time1601_append(data, pitem->creationtime);
  netwox__smbcmdcmn_time1601_append(data, pitem->lastaccesstime);
  netwox__smbcmdcmn_time1601_append(data, pitem->lastwritetime);
  netwox__smbcmdcmn_time1601_append(data, pitem->changetime);
  netwox__smbcmdcmn_uint64_append(data, pitem->allocationsize);
  netwox__smbcmdcmn_uint64_append(data, pitem->filesize);
  netwox__smbcmdcmn_fileattr32_append(data, pitem->fileattributes);
  /* unused value */
  netwib__data_append_uint32_le(data, 0);
  pbuf->endoffset += 56;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_enetworkopen_show(const netwox_smbcmdtsc2_fmt_file_enetworkopen *pitem,
                                                         netwib_buf *pbuf)
{
  netwib_er(netwox_smbcmdcmn_time1601_show("  creationtime", pitem->creationtime, pbuf));
  netwib_er(netwox_smbcmdcmn_time1601_show("  lastaccesstime", pitem->lastaccesstime, pbuf));
  netwib_er(netwox_smbcmdcmn_time1601_show("  lastwritetime", pitem->lastwritetime, pbuf));
  netwib_er(netwox_smbcmdcmn_time1601_show("  changetime", pitem->changetime, pbuf));
  netwib_er(netwox_smbcmdcmn_uint64_show("  allocationsize", pitem->allocationsize, pbuf));
  netwib_er(netwox_smbcmdcmn_uint64_show("  filesize", pitem->filesize, pbuf));
  netwib_er(netwox_smbcmdcmn_fileattr32_show("  fileattributes", pitem->fileattributes, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_eattributetag_init(netwox_smbcmdtsc2_fmt_file_eattributetag *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_eattributetag_setdefault(netwox_smbcmdtsc2_fmt_file_eattributetag *pitem)
{
  pitem->mode = 0;
  pitem->unknown = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_eattributetag_close(netwox_smbcmdtsc2_fmt_file_eattributetag *pitem)
{
  pitem = pitem; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_eattributetag_decode(netwib_data *pdata,
                                                   netwib_uint32 *pdatasize,
                             netwox_smbcmdtsc2_fmt_file_eattributetag *pitem)
{
  netwib_data data;
  netwib_uint32 datasize;

  data = *pdata;
  datasize = *pdatasize;

  if (datasize < 8) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwib__data_decode_uint32_le(data, pitem->mode);
  netwib__data_decode_uint32_le(data, pitem->unknown);
  datasize -= 8;

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_eattributetag_append(const netwox_smbcmdtsc2_fmt_file_eattributetag *pitem,
                                                   netwib_buf *pbuf)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(pbuf, 8, &data));
  netwib__data_append_uint32_le(data, pitem->mode);
  netwib__data_append_uint32_le(data, pitem->unknown);
  pbuf->endoffset += 8;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_eattributetag_show(const netwox_smbcmdtsc2_fmt_file_eattributetag *pitem,
                                                 netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_fmt32(pbuf, "  mode=%{uint32}", pitem->mode));
  netwib_er(netwib_show_array_fmt32(pbuf, "  unknown=%{uint32}", pitem->unknown));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_unknown_init(netwox_smbcmdtsc2_fmt_file_unknown *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_initdefault(&pitem->unknown));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_unknown_setdefault(netwox_smbcmdtsc2_fmt_file_unknown *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_setdefault(&pitem->unknown));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_unknown_close(netwox_smbcmdtsc2_fmt_file_unknown *pitem)
{
  netwib_er(netwox_smbcmdcmn_data_close(&pitem->unknown));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_unknown_decode(netwib_data *pdata,
                                                     netwib_uint32 *pdatasize,
                                       netwox_smbcmdtsc2_fmt_file_unknown *pitem)
{
  netwib_data data;
  netwib_uint32 datasize;

  data = *pdata;
  datasize = *pdatasize;

  netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, datasize,
                                            &pitem->unknown));

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_unknown_append(const netwox_smbcmdtsc2_fmt_file_unknown *pitem,
                                                     netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_buf(&pitem->unknown, pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdtsc2_fmt_file_unknown_show(const netwox_smbcmdtsc2_fmt_file_unknown *pitem,
                                                   netwib_buf *pbuf)
{
  netwib_er(netwox_smbcmdcmn_data_show("  unknown", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &pitem->unknown, pbuf));

  return(NETWIB_ERR_OK);
}
