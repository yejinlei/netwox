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
netwib_err netwox_smbcmdcmn_data_initdefault(netwox_smbcmdcmn_data *pdata)
{
  netwib_er(netwib_buf_init_ext_storage(pdata));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_data_setdefault(netwox_smbcmdcmn_data *pdata)
{
  netwib__buf_reinit(pdata);
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_data_close(netwox_smbcmdcmn_data *pdata)
{
  netwib_er(netwib_buf_close(pdata));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_data_show(netwib_conststring title,
                                      netwox_smbcmdcmn_datatype datatype,
                                      netwox_constsmbcmdcmn_data *pdata,
                                      netwib_buf *pbuf)
{
  switch(datatype) {
  case NETWOX_SMBCMDCMN_DATATYPE_DATABLOCK :
  case NETWOX_SMBCMDCMN_DATATYPE_VARIABLEBLOCK :
  case NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE :
    netwib_er(netwib_show_array_data(title, pdata, NETWIB_ENCODETYPE_HEXA0,
                                     ' ', pbuf));
    break;
  case NETWOX_SMBCMDCMN_DATATYPE_DIALECT :
  case NETWOX_SMBCMDCMN_DATATYPE_PATHNAME :
  case NETWOX_SMBCMDCMN_DATATYPE_ASCII :
  case NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY :
  case NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI :
  case NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_UNINOPAD :
    netwib_er(netwib_show_array_data(title, pdata, NETWIB_ENCODETYPE_MIXED,
                                     ' ', pbuf));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_data_append(netwox_smbmsg *psmbmsg,
                                        netwib_uint32 idxcmd,
                                        netwox_smbcmdcmn_datatype datatype,
                                        netwox_constsmbcmdcmn_data *pbuf)
{
  netwib_data data;
  netwib_bool sizeisaligned;

  switch(datatype) {
  case NETWOX_SMBCMDCMN_DATATYPE_DATABLOCK :
  case NETWOX_SMBCMDCMN_DATATYPE_VARIABLEBLOCK :
    netwib_er(netwib_buf_append_byte((netwib_byte)datatype, &psmbmsg->cmds[idxcmd].data));
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[idxcmd].data, 2, &data));
    netwib__data_append_uint16_le(data, netwib__buf_ref_data_size(pbuf));
    psmbmsg->cmds[idxcmd].data.endoffset += 2;
    netwib_er(netwib_buf_append_buf(pbuf, &psmbmsg->cmds[idxcmd].data));
    break;
  case NETWOX_SMBCMDCMN_DATATYPE_DIALECT :
    netwib_er(netwib_buf_append_byte((netwib_byte)datatype, &psmbmsg->cmds[idxcmd].data));
    netwib_er(netwib_buf_append_buf(pbuf, &psmbmsg->cmds[idxcmd].data));
    netwib_er(netwib_buf_append_byte(0, &psmbmsg->cmds[idxcmd].data));
    break;
  case NETWOX_SMBCMDCMN_DATATYPE_PATHNAME :
  case NETWOX_SMBCMDCMN_DATATYPE_ASCII :
    netwib_er(netwib_buf_append_byte((netwib_byte)datatype, &psmbmsg->cmds[idxcmd].data));
    /* no break */
  case NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY :
    /* append pad char for Unicode only */
    if (psmbmsg->hdr.flags2 & NETWOX_SMBMSG_FLAGS2_UNICODESTRINGS) {
      sizeisaligned = (netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data) % 2)?NETWIB_FALSE:NETWIB_TRUE;
      if ((psmbmsg->cmds[idxcmd].dataisaligned && !sizeisaligned) ||
          (!psmbmsg->cmds[idxcmd].dataisaligned && sizeisaligned)) {
        netwib_er(netwib_buf_append_byte(0, &psmbmsg->cmds[idxcmd].data));
      }
    }
    /* append string */
    netwib_er(netwib_buf_append_buf(pbuf, &psmbmsg->cmds[idxcmd].data));
    /* end with '\0' */
    netwib_er(netwib_buf_append_byte(0, &psmbmsg->cmds[idxcmd].data));
    if (psmbmsg->hdr.flags2 & NETWOX_SMBMSG_FLAGS2_UNICODESTRINGS) {
      netwib_er(netwib_buf_append_byte(0, &psmbmsg->cmds[idxcmd].data));
    }
    break;
  case NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_UNINOPAD :
    /* append string */
    netwib_er(netwib_buf_append_buf(pbuf, &psmbmsg->cmds[idxcmd].data));
    /* end with double '\0' */
    netwib_er(netwib_buf_append_byte(0, &psmbmsg->cmds[idxcmd].data));
    netwib_er(netwib_buf_append_byte(0, &psmbmsg->cmds[idxcmd].data));
    break;
  case NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI :
    /* append string */
    netwib_er(netwib_buf_append_buf(pbuf, &psmbmsg->cmds[idxcmd].data));
    /* end with '\0' */
    netwib_er(netwib_buf_append_byte(0, &psmbmsg->cmds[idxcmd].data));
    break;
  case NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE :
    netwib_er(netwib_buf_append_buf(pbuf, &psmbmsg->cmds[idxcmd].data));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_data_decode(netwox_constsmbmsg *psmbmsg,
                                        netwib_uint32 idxcmd,
                                        netwox_smbcmdcmn_datatype expecteddatatype,
                                        netwib_data *pdata,
                                        netwib_uint32 *pdatasize,
                                        netwox_smbcmdcmn_data *pbuf)
{
  netwib_data data, datastart;
  netwib_uint32 datasize, size;
  netwox_smbcmdcmn_datatype datatype=NETWOX_SMBCMDCMN_DATATYPE_UNKNOWN;
  netwib_bool isunicode, sizeisaligned;

  data = *pdata;
  datasize = *pdatasize;

  if (expecteddatatype == NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE) {
    /* use other function */
    return(NETWOX_ERR_INTERNALERROR);
  } else if (expecteddatatype == NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY) {
    /* there is no datatype */
    datatype = NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY;
  } else if (expecteddatatype == NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI) {
    /* there is no datatype */
    datatype = NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI;
  } else if (expecteddatatype ==
             NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_UNINOPAD) {
    /* there is no datatype */
    datatype = NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_UNINOPAD;
  } else {
    /* first byte is datatype */
    if (datasize == 0) {
      return(NETWIB_ERR_DATAMISSING);
    }
    netwib__data_decode_uint8_le(data, datatype);
    datasize--;
    if (expecteddatatype != NETWOX_SMBCMDCMN_DATATYPE_UNKNOWN) {
      if (datatype != expecteddatatype) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
    }
  }

  switch(datatype) {
  case NETWOX_SMBCMDCMN_DATATYPE_DATABLOCK :
  case NETWOX_SMBCMDCMN_DATATYPE_VARIABLEBLOCK :
    if (datasize < 2) {
      return(NETWIB_ERR_DATAMISSING);
    }
    netwib__data_decode_uint16_le(data, size);
    datasize -= 2;
    if (datasize < size) {
      return(NETWIB_ERR_DATAMISSING);
    }
    netwib_er(netwib_buf_init_ext_arrayfilled(data, size, pbuf));
    data += size;
    datasize -= size;
    break;
  case NETWOX_SMBCMDCMN_DATATYPE_DIALECT :
    /* dialect is never unicode */
    datastart = data;
    while(NETWIB_TRUE) {
      if (datasize == 0) {
        return(NETWIB_ERR_DATAMISSING);
      }
      if (*data == '\0') {
        break;
      }
      data++;
      datasize--;
    }
    netwib_er(netwib_buf_init_ext_arrayfilled(datastart, data-datastart,
                                              pbuf));
    data++;
    datasize--;
    break;
  case NETWOX_SMBCMDCMN_DATATYPE_PATHNAME :
  case NETWOX_SMBCMDCMN_DATATYPE_ASCII :
  case NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY :
  case NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI :
  case NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_UNINOPAD :
    /* optionally skip padding */
    isunicode = NETWIB_FALSE;
    if (datatype == NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_UNINOPAD) {
      /* Unicode, but not padded */
      isunicode = NETWIB_TRUE;
    } else if (datatype == NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI) {
      /* not Unicode, so not padded */
    } else if (psmbmsg->hdr.flags2 & NETWOX_SMBMSG_FLAGS2_UNICODESTRINGS) {
      /* Unicode, and padded */
      isunicode = NETWIB_TRUE;
      sizeisaligned = ((data-netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data)) % 2)?NETWIB_FALSE:NETWIB_TRUE;
      if ((psmbmsg->cmds[idxcmd].dataisaligned && !sizeisaligned) ||
          (!psmbmsg->cmds[idxcmd].dataisaligned && sizeisaligned)) {
        if (datasize < 1) {
          return(NETWIB_ERR_DATAMISSING);
        }
        data++;
        datasize--;
      }
    }
    /* copy */
    netwib_er(netwox_smbcmdcmn_data_decodestr(&data, &datasize, isunicode,
                                              pbuf));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_data_decodeext(netwib_data *pdata,
                                           netwib_uint32 *pdatasize,
                                           netwib_uint32 buffersize,
                                           netwox_smbcmdcmn_data *pbuf)
{
  netwib_data data;
  netwib_uint32 datasize;

  if (buffersize == 0) {
    netwib_er(netwib_buf_init_ext_empty(pbuf));
    return(NETWIB_ERR_OK);
  }

  data = *pdata;
  datasize = *pdatasize;

  if (datasize < buffersize) {
    return(NETWIB_ERR_DATAMISSING);
  }

  netwib_er(netwib_buf_init_ext_arrayfilled(data, buffersize, pbuf));

  *pdata = data + buffersize;
  *pdatasize = datasize - buffersize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_data_appendstr(netwox_constsmbcmdcmn_data *pdata,
                                           netwib_bool isunicode,
                                           netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_buf(pdata, pbuf));
  netwib_er(netwib_buf_append_byte(0, pbuf));
  if (isunicode) {
    netwib_er(netwib_buf_append_byte(0, pbuf));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_data_decodestr(netwib_data *pdata,
                                           netwib_uint32 *pdatasize,
                                           netwib_bool isunicode,
                                           netwox_smbcmdcmn_data *pbuf)
{
  netwib_data data, datastart;
  netwib_uint32 datasize;
  netwib_bool unicodebug;

  data = *pdata;
  datasize = *pdatasize;

  datastart = data;
  unicodebug = NETWIB_FALSE;
  if (isunicode) {
    while(NETWIB_TRUE) {
      if (datasize == 1 && data[0] == '\0') {
        /* Session Setup AndX commands are bugged: they end with only one
           '\0'. This is a workaround against it and other potential cases.*/
        unicodebug = NETWIB_TRUE;
        break;
      }
      if (datasize < 2) {
        return(NETWIB_ERR_DATAMISSING);
      }
      if (data[0] == '\0' && data[1] == '\0') {
        break;
      }
      data += 2;
      datasize -= 2;
    }
  } else {
    while(NETWIB_TRUE) {
      if (datasize == 0) {
        return(NETWIB_ERR_DATAMISSING);
      }
      if (*data == '\0') {
        break;
      }
      data++;
      datasize--;
    }
  }
  netwib_er(netwib_buf_init_ext_arrayfilled(datastart, data-datastart, pbuf));
  if (isunicode && !unicodebug) {
    data += 2;
    datasize -= 2;
  } else {
    data++;
    datasize--;
  }

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_uint64_show(netwib_conststring title,
                                        netwox_constsmbcmdcmn_uint64 ui64,
                                        netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_fmt32(pbuf, "%s=%{uint64}", title, ui64));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_time1601_show(netwib_conststring title,
                                   netwox_constsmbcmdcmn_time1601 time1601,
                                   netwib_buf *pbuf)
{
  netwib_byte array[80];
  netwib_buf buf;
  netwib_time t;
  netwib_err ret;

  ret = netwox_time_init_time1601(time1601, &t);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));
    netwib_er(netwox_date_rfc822simple(&t, &buf));
    netwib_er(netwib_show_array_fmt32(pbuf, "%s=%{buf}", title, &buf));
  } else {
    netwib_er(netwib_show_array_fmt32(pbuf, "%s=unknown (%{uint64})", title,
                                      time1601));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_time1970l_show(netwib_conststring title,
                                   netwox_constsmbcmdcmn_time1970l time1970l,
                                   netwib_buf *pbuf)
{
  netwib_byte array[80];
  netwib_buf buf;
  netwib_time t;
  netwib_err ret;

  if (time1970l == 0xFFFFFFFFu) {
    netwib_er(netwib_show_array_fmt32(pbuf, "%s=Time is unknown", title));
  } else {
    ret = netwox_time_init_time1970l(time1970l, &t);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));
      netwib_er(netwox_date_rfc822simple(&t, &buf));
      netwib_er(netwib_show_array_fmt32(pbuf, "%s=%{buf}", title, &buf));
    } else {
      netwib_er(netwib_show_array_fmt32(pbuf, "%s=unknown (%{uint32})",
                                        title, time1970l));
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_timemsdos_show(netwib_conststring title,
                                   netwox_constsmbcmdcmn_timemsdos timemsdos,
                                   netwib_buf *pbuf)
{
  netwib_byte array[80];
  netwib_buf buf;
  netwib_time t;
  netwib_err ret;

  if (timemsdos.msdosdate == 0 && timemsdos.msdostime == 0 ) {
    netwib_er(netwib_show_array_fmt32(pbuf, "%s=Time is unknown", title));
  } else {
    ret = netwox_time_init_timemsdos(timemsdos, &t);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));
      netwib_er(netwox_date_rfc822simple(&t, &buf));
      netwib_er(netwib_show_array_fmt32(pbuf, "%s=%{buf}", title, &buf));
    } else {
      netwib_er(netwib_show_array_fmt32(pbuf, "%s=unknown (%{uint16:16b}:%{uint16:16b})", title, timemsdos.msdosdate, timemsdos.msdostime));
    }
  }

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/* Flag bits */
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_openflags16_show(netwib_conststring title,
                                        netwox_smbcmdcmn_openflags16 openflags,
                                        netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_fmt32(pbuf, "%s=%{uint16:16b}", title,
                                    openflags));
  if (openflags & NETWOX_SMBCMDCMN_OPENFLAGS16_ADDITIONALINFO) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> additional information requested"));
  }
  if (openflags & NETWOX_SMBCMDCMN_OPENFLAGS16_EXCLUSIVEOPLOCK) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> exclusive oplock requested"));
  }
  if (openflags & NETWOX_SMBCMDCMN_OPENFLAGS16_BATCHOPLOCK) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> batch oplock requested"));
  }
  if (openflags & NETWOX_SMBCMDCMN_OPENFLAGS16_TARGETISADIR) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> target for open is a directory"));
  }
  if (openflags & NETWOX_SMBCMDCMN_OPENFLAGS16_EXTENDEDRESPONSE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> extended response requested"));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_openflags32_show(netwib_conststring title,
                                        netwox_smbcmdcmn_openflags32 openflags,
                                        netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_fmt32(pbuf, "%s=%{uint32:32b}", title,
                                    openflags));
  if (openflags & NETWOX_SMBCMDCMN_OPENFLAGS32_ADDITIONALINFO) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> additional information requested"));
  }
  if (openflags & NETWOX_SMBCMDCMN_OPENFLAGS32_EXCLUSIVEOPLOCK) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> exclusive oplock requested"));
  }
  if (openflags & NETWOX_SMBCMDCMN_OPENFLAGS32_BATCHOPLOCK) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> batch oplock requested"));
  }
  if (openflags & NETWOX_SMBCMDCMN_OPENFLAGS32_TARGETISADIR) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> target for open is a directory"));
  }
  if (openflags & NETWOX_SMBCMDCMN_OPENFLAGS32_EXTENDEDRESPONSE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> extended response requested"));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_fileattr16_show(netwib_conststring title,
                                        netwox_smbcmdcmn_fileattr16 fileattr16,
                                        netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_fmt32(pbuf, "%s=%{uint16:16b}", title,
                                    fileattr16));
  if (fileattr16 & NETWOX_SMBCMDCMN_FILEATTR16_READONLY) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> read only"));
  }
  if (fileattr16 & NETWOX_SMBCMDCMN_FILEATTR16_HIDDEN) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> hidden"));
  }
  if (fileattr16 & NETWOX_SMBCMDCMN_FILEATTR16_SYSTEM) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> system"));
  }
  if (fileattr16 & NETWOX_SMBCMDCMN_FILEATTR16_VOLUME) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> volume"));
  }
  if (fileattr16 & NETWOX_SMBCMDCMN_FILEATTR16_DIR) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> directory"));
  }
  if (fileattr16 & NETWOX_SMBCMDCMN_FILEATTR16_ARCHIVE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> archive"));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_fileattr32_show(netwib_conststring title,
                                        netwox_smbcmdcmn_fileattr32 fileattr32,
                                        netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_fmt32(pbuf, "%s=%{uint32:32b}", title,
                                    fileattr32));
  if (fileattr32 & NETWOX_SMBCMDCMN_FILEATTR32_READONLY) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> read only"));
  }
  if (fileattr32 & NETWOX_SMBCMDCMN_FILEATTR32_HIDDEN) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> hidden"));
  }
  if (fileattr32 & NETWOX_SMBCMDCMN_FILEATTR32_SYSTEM) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> system"));
  }
  if (fileattr32 & NETWOX_SMBCMDCMN_FILEATTR32_VOLUME) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> volume"));
  }
  if (fileattr32 & NETWOX_SMBCMDCMN_FILEATTR32_DIR) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> directory"));
  }
  if (fileattr32 & NETWOX_SMBCMDCMN_FILEATTR32_ARCHIVE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> archive"));
  }
  if (fileattr32 & NETWOX_SMBCMDCMN_FILEATTR32_DEVICE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> device"));
  }
  if (fileattr32 & NETWOX_SMBCMDCMN_FILEATTR32_NORMAL) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> normal"));
  }
  if (fileattr32 & NETWOX_SMBCMDCMN_FILEATTR32_TEMP) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> temporary"));
  }
  if (fileattr32 & NETWOX_SMBCMDCMN_FILEATTR32_SPARSE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> sparse file"));
  }
  if (fileattr32 & NETWOX_SMBCMDCMN_FILEATTR32_REPARSE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> have an associated sparse point"));
  }
  if (fileattr32 & NETWOX_SMBCMDCMN_FILEATTR32_COMPRESSED) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> compressed"));
  }
  if (fileattr32 & NETWOX_SMBCMDCMN_FILEATTR32_OFFLINE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> offline"));
  }
  if (fileattr32 & NETWOX_SMBCMDCMN_FILEATTR32_NONINDEXED) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> content may not be indexed"));
  }
  if (fileattr32 & NETWOX_SMBCMDCMN_FILEATTR32_ENCRYPTED) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> encrypted"));
  }
  if (fileattr32 & NETWOX_SMBCMDCMN_FILEATTR32_POSIXSEMANTICS) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> POSIX semantics"));
  }
  if (fileattr32 & NETWOX_SMBCMDCMN_FILEATTR32_BACKUPSEMANTICS) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> backup semantics"));
  }
  if (fileattr32 & NETWOX_SMBCMDCMN_FILEATTR32_DELETEONCLOSE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> delete on close"));
  }
  if (fileattr32 & NETWOX_SMBCMDCMN_FILEATTR32_SEQUENTIALSCAN) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> sequential scan"));
  }
  if (fileattr32 & NETWOX_SMBCMDCMN_FILEATTR32_RANDOMACCESS) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> random access"));
  }
  if (fileattr32 & NETWOX_SMBCMDCMN_FILEATTR32_NOBUFFERING) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> no buffering"));
  }
  if (fileattr32 & NETWOX_SMBCMDCMN_FILEATTR32_WRITETHROUGH) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> write through"));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_negocapa_show(netwib_conststring title,
                                          netwox_smbcmdcmn_negocapa negocapa,
                                          netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_fmt32(pbuf, "%s=%{uint32:32b}", title,
                                    negocapa));
  if (negocapa & NETWOX_SMBCMDCMN_NEGOCAPA_RAWMODE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> READ_RAW and WRITE_RAW commands"));
  }
  if (negocapa & NETWOX_SMBCMDCMN_NEGOCAPA_MPXMODE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> READ_MPX and WRITE_MPX commands"));
  }
  if (negocapa & NETWOX_SMBCMDCMN_NEGOCAPA_UNICODE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> unicode strings"));
  }
  if (negocapa & NETWOX_SMBCMDCMN_NEGOCAPA_LARGEFILES) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> large files (4GB)"));
  }
  if (negocapa & NETWOX_SMBCMDCMN_NEGOCAPA_NTSMBS) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> NT SMBs (advanced features)"));
  }
  if (negocapa & NETWOX_SMBCMDCMN_NEGOCAPA_RPCREMOTEAPIS) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> RPC remote APIs"));
  }
  if (negocapa & NETWOX_SMBCMDCMN_NEGOCAPA_NTERROR) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> NT error code"));
  }
  if (negocapa & NETWOX_SMBCMDCMN_NEGOCAPA_LEVEL2OPLOCKS) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> level 2 oplocks"));
  }
  if (negocapa & NETWOX_SMBCMDCMN_NEGOCAPA_LOCKANDREAD) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> LOCK_AND_READ command supported"));
  }
  if (negocapa & NETWOX_SMBCMDCMN_NEGOCAPA_NTFIND) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> NT find (advanced features)"));
  }
  if (negocapa & NETWOX_SMBCMDCMN_NEGOCAPA_DFS) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> Distributed File System"));
  }
  if (negocapa & NETWOX_SMBCMDCMN_NEGOCAPA_INFOLEVELPASSTHROUGH) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> infolevel passthrough (?)"));
  }
  if (negocapa & NETWOX_SMBCMDCMN_NEGOCAPA_LARGEREADANDX) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> READ_ANDX command can transfer large data"));
  }
  if (negocapa & NETWOX_SMBCMDCMN_NEGOCAPA_LARGEWRITEANDX) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> WRITE_ANDX command can transfer large data"));
  }
  if (negocapa & NETWOX_SMBCMDCMN_NEGOCAPA_UNIX) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> Unix extensions (HP)"));
  }
  if (negocapa & NETWOX_SMBCMDCMN_NEGOCAPA_BULKTRANSFER) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> READ_BULK and WRITE_BULK commands"));
  }
  if (negocapa & NETWOX_SMBCMDCMN_NEGOCAPA_COMPRESSEDDATA) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> compressed data"));
  }
  if (negocapa & NETWOX_SMBCMDCMN_NEGOCAPA_EXTENDEDSECURITY) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> extended security"));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_accessmask_show(netwib_conststring title,
                                       netwox_smbcmdcmn_accessmask accessmask,
                                       netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_fmt32(pbuf, "%s=%{uint32:32b}", title,
                                    accessmask));
  if (accessmask & NETWOX_SMBCMDCMN_ACCESSMASK_READ) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> read"));
  }
  if (accessmask & NETWOX_SMBCMDCMN_ACCESSMASK_WRITE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> write"));
  }
  if (accessmask & NETWOX_SMBCMDCMN_ACCESSMASK_APPEND) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> append"));
  }
  if (accessmask & NETWOX_SMBCMDCMN_ACCESSMASK_READEA) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> read Extended Attributes"));
  }
  if (accessmask & NETWOX_SMBCMDCMN_ACCESSMASK_WRITEEA) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> write Extended Attributes"));
  }
  if (accessmask & NETWOX_SMBCMDCMN_ACCESSMASK_EXECUTE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> execute"));
  }
  if (accessmask & NETWOX_SMBCMDCMN_ACCESSMASK_DELCHILD) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> delete child"));
  }
  if (accessmask & NETWOX_SMBCMDCMN_ACCESSMASK_READATTR) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> read attributes"));
  }
  if (accessmask & NETWOX_SMBCMDCMN_ACCESSMASK_WRITEATTR) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> write attributes"));
  }
  if (accessmask & NETWOX_SMBCMDCMN_ACCESSMASK_DELETE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> delete"));
  }
  if (accessmask & NETWOX_SMBCMDCMN_ACCESSMASK_READCONTROL) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> read control"));
  }
  if (accessmask & NETWOX_SMBCMDCMN_ACCESSMASK_WRITEDAC) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> write DAC"));
  }
  if (accessmask & NETWOX_SMBCMDCMN_ACCESSMASK_WRITEOWNER) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> write owner"));
  }
  if (accessmask & NETWOX_SMBCMDCMN_ACCESSMASK_SYNCHRO) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> synchronize"));
  }
  if (accessmask & NETWOX_SMBCMDCMN_ACCESSMASK_SYSSECU) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> system security"));
  }
  if (accessmask & NETWOX_SMBCMDCMN_ACCESSMASK_MAXALLOWED) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> maximu allowed"));
  }
  if (accessmask & NETWOX_SMBCMDCMN_ACCESSMASK_GENERICALL) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> generic all"));
  }
  if (accessmask & NETWOX_SMBCMDCMN_ACCESSMASK_GENERICEXECUTE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> generic execute"));
  }
  if (accessmask & NETWOX_SMBCMDCMN_ACCESSMASK_GENERICWRITE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> generic write"));
  }
  if (accessmask & NETWOX_SMBCMDCMN_ACCESSMASK_GENERICREAD) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> generic read"));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_shareaccess_show(netwib_conststring title,
                                    netwox_smbcmdcmn_shareaccess shareaccess,
                                    netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_fmt32(pbuf, "%s=%{uint32:32b}", title,
                                    shareaccess));
  if (shareaccess & NETWOX_SMBCMDCMN_SHAREACCESS_READ) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> read"));
  }
  if (shareaccess & NETWOX_SMBCMDCMN_SHAREACCESS_WRITE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> write"));
  }
  if (shareaccess & NETWOX_SMBCMDCMN_SHAREACCESS_DELETE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> delete"));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_secuflags_show(netwib_conststring title,
                                    netwox_smbcmdcmn_secuflags secuflags,
                                    netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_fmt32(pbuf, "%s=%{uint8:8b}", title,
                                    secuflags));
  if (secuflags & NETWOX_SMBCMDCMN_SECUFLAGS_CONTEXTTRACKING) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> security tracking mode is static"));
  }
  if (secuflags & NETWOX_SMBCMDCMN_SECUFLAGS_EFFECTIVEONLY) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> all aspects of the client's security context are available"));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_createopt_show(netwib_conststring title,
                                    netwox_smbcmdcmn_createopt createopt,
                                    netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_fmt32(pbuf, "%s=%{uint32:32b}", title,
                                    createopt));
  if (createopt & NETWOX_SMBCMDCMN_CREATEOPT_DIRECTORY) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> directory"));
  }
  if (createopt & NETWOX_SMBCMDCMN_CREATEOPT_WRITETHROUGH) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> write through"));
  }
  if (createopt & NETWOX_SMBCMDCMN_CREATEOPT_SEQUENTIALONLY) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> sequential only"));
  }
  if (createopt & NETWOX_SMBCMDCMN_CREATEOPT_SYNCIOALERT) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> sync I/O alert"));
  }
  if (createopt & NETWOX_SMBCMDCMN_CREATEOPT_SYNCIONONALERT) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> sync I/O non alert"));
  }
  if (createopt & NETWOX_SMBCMDCMN_CREATEOPT_NONDIRECTORY) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> non directory"));
  }
  if (createopt & NETWOX_SMBCMDCMN_CREATEOPT_NOEAKNOWLEDGE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> no Extended Attribute knowledge"));
  }
  if (createopt & NETWOX_SMBCMDCMN_CREATEOPT_83ONLY) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> 8.3 only"));
  }
  if (createopt & NETWOX_SMBCMDCMN_CREATEOPT_RANDOMACCESS) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> random access"));
  }
  if (createopt & NETWOX_SMBCMDCMN_CREATEOPT_DELETEONCLOSE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> delete on close"));
  }
  if (createopt & NETWOX_SMBCMDCMN_CREATEOPT_OPENBYFILEID) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> open by file id"));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_servertype_show(netwib_conststring title,
                                    netwox_smbcmdcmn_servertype servertype,
                                    netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_fmt32(pbuf, "%s=%{uint32:32b}", title,
                                    servertype));
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_WORKSTATION) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> Workstation"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_SERVER) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> Server"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_SQLSERVER) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> SQL Server"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_DOMAINCONTROLER) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> Domain Controler"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_DOMAINBACKUPCONTROLER) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> Domain Backup Controler"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_TIMESOURCE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> Time source"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_APPLESERVER) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> Apple server"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_NOVELLSERVER) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> Novell server"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_DOMAINMEMBER) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> Domain member"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_PRINTQUEUESERVER) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> Print Queue Server"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_DIALINSERVER) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> Dial-in server"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_XENIXSERVER) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> Xenix Server"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_NTWORKSTATION) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> NT Workstation"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_WINFORWORKGROUP) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> Windows for Workgroup"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_MFPNSERVER) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> MFPN Server"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_NTSERVER) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> NT Server"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_POTENTIALBROWSER) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> Potential browser"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_BACKUPBROWSER) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> Backup browser"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_MASTERBROWSER) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> Master browser"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_DOMAINMASTERBROWSER) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> Domain master browser"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_OSFSERVER) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> OSF server"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_VMSSERVER) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> VMS server"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_WIN95ANDMORE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> Windows 95 and more"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_DFS) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> DFS"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_NTCLUSTER) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> NT Cluster"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_DCE) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> DCE"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_ALTERNATETRANSPORT) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> Alternate transport"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_LOCALENUM) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> Local enumeration"));
  }
  if (servertype & NETWOX_SMBCMDCMN_SERVERTYPE_DOMAINENUM) {
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> Domain enumeration"));
  }

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/* Flag masks */
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_accessmode_show(netwib_conststring title,
                                        netwox_smbcmdcmn_accessmode accessmode,
                                        netwib_buf *pbuf)
{
  netwib_buf family;

  netwib_er(netwib_show_array_fmt32(pbuf, "%s=%{uint16:16b}", title,
                                    accessmode));
  netwib_er(netwib_buf_init_ext_string("write through mode", &family));
  switch(accessmode & NETWOX_SMBCMDCMN_ACCESSMODE_WRITETHROUGH) {
  case NETWOX_SMBCMDCMN_ACCESSMODE_WRITETHROUGH_NO :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: disabled", &family));
    break;
  case NETWOX_SMBCMDCMN_ACCESSMODE_WRITETHROUGH_YES :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: enabled", &family));
    break;
  default :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: unknown", &family));
    break;
  }

  netwib_er(netwib_buf_init_ext_string("cache mode", &family));
  switch(accessmode & NETWOX_SMBCMDCMN_ACCESSMODE_CACHE) {
  case NETWOX_SMBCMDCMN_ACCESSMODE_CACHE_NORMAL :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: cache this file", &family));
    break;
  case NETWOX_SMBCMDCMN_ACCESSMODE_CACHE_NONE :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: do not cache this file", &family));
    break;
  default :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: unknown", &family));
    break;
  }

  netwib_er(netwib_buf_init_ext_string("locality of reference", &family));
  switch(accessmode & NETWOX_SMBCMDCMN_ACCESSMODE_LOCALITY) {
  case NETWOX_SMBCMDCMN_ACCESSMODE_LOCALITY_UNKNOWN :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: unknown", &family));
    break;
  case NETWOX_SMBCMDCMN_ACCESSMODE_LOCALITY_SEQUENTIAL :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: mainly sequential access", &family));
    break;
  case NETWOX_SMBCMDCMN_ACCESSMODE_LOCALITY_RANDOM :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: mainly random access", &family));
    break;
  case NETWOX_SMBCMDCMN_ACCESSMODE_LOCALITY_RANDOMLOC :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: random access with some locality", &family));
    break;
  default :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: unknown", &family));
    break;
  }

  netwib_er(netwib_buf_init_ext_string("sharing mode", &family));
  switch(accessmode & NETWOX_SMBCMDCMN_ACCESSMODE_SHARING) {
  case NETWOX_SMBCMDCMN_ACCESSMODE_SHARING_COMPATIBILITY :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: compatibility mode", &family));
    break;
  case NETWOX_SMBCMDCMN_ACCESSMODE_SHARING_DENYRWX :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: deny read/write/execute", &family));
    break;
  case NETWOX_SMBCMDCMN_ACCESSMODE_SHARING_DENYW :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: deny write", &family));
    break;
  case NETWOX_SMBCMDCMN_ACCESSMODE_SHARING_DENYRX :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: deny read/execute", &family));
    break;
  case NETWOX_SMBCMDCMN_ACCESSMODE_SHARING_DENYNONE :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: deny none", &family));
    break;
  default :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: unknown", &family));
    break;
  }

  netwib_er(netwib_buf_init_ext_string("access mode", &family));
  switch(accessmode & NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS) {
  case NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_R :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: open for reading", &family));
    break;
  case NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_W :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: open for writing", &family));
    break;
  case NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_RW :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: open for reading and writing", &family));
    break;
  case NETWOX_SMBCMDCMN_ACCESSMODE_ACCESS_X :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: open for execute", &family));
    break;
  default :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: unknown", &family));
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_openfunction_show(netwib_conststring title,
                              netwox_smbcmdcmn_openfunction openfunction,
                              netwib_buf *pbuf)
{
  netwib_buf family;

  netwib_er(netwib_show_array_fmt32(pbuf, "%s=%{uint16:16b}", title,
                                    openfunction));

  netwib_er(netwib_buf_init_ext_string("if file doesn't exist, creating it will", &family));
  switch(openfunction & NETWOX_SMBCMDCMN_OPENFUNCTION_CREATEFILENOEXIST) {
  case NETWOX_SMBCMDCMN_OPENFUNCTION_CREATEFILENOEXIST_ERROR :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: generate an error", &family));
    break;
  case NETWOX_SMBCMDCMN_OPENFUNCTION_CREATEFILENOEXIST_OVERWRITE :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: overwrite", &family));
    break;
  default :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: unknown", &family));
    break;
  }

  netwib_er(netwib_buf_init_ext_string("if file exists, opening it will", &family));
  switch(openfunction & NETWOX_SMBCMDCMN_OPENFUNCTION_OPENFILEEXIST) {
  case NETWOX_SMBCMDCMN_OPENFUNCTION_OPENFILEEXIST_ERROR :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: generate an error", &family));
    break;
  case NETWOX_SMBCMDCMN_OPENFUNCTION_OPENFILEEXIST_OPEN :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: open", &family));
    break;
  case NETWOX_SMBCMDCMN_OPENFUNCTION_OPENFILEEXIST_TRUNCATE :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: truncate", &family));
    break;
  default :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: unknown", &family));
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_createaction_show(netwib_conststring title,
                              netwox_smbcmdcmn_createaction createaction,
                              netwib_buf *pbuf)
{
  netwib_buf family;

  netwib_er(netwib_show_array_fmt32(pbuf, "%s=%{uint32:32b}", title,
                                    createaction));

  netwib_er(netwib_buf_init_ext_string("oplock", &family));
  switch(createaction & NETWOX_SMBCMDCMN_CREATEACTION_OPLOCK) {
  case NETWOX_SMBCMDCMN_CREATEACTION_OPLOCK_NO :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: none", &family));
    break;
  case NETWOX_SMBCMDCMN_CREATEACTION_OPLOCK_YES :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: granted", &family));
    break;
  default :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: unknown", &family));
    break;
  }

  netwib_er(netwib_buf_init_ext_string("action", &family));
  switch(createaction & NETWOX_SMBCMDCMN_CREATEACTION_ACTION) {
  case NETWOX_SMBCMDCMN_CREATEACTION_ACTION_NONE :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: no action taken", &family));
    break;
  case NETWOX_SMBCMDCMN_CREATEACTION_ACTION_EXISTOPENED :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: file existed and was opened", &family));
    break;
  case NETWOX_SMBCMDCMN_CREATEACTION_ACTION_NOEXISTCREATED :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: file did not exist but was created", &family));
    break;
  case NETWOX_SMBCMDCMN_CREATEACTION_ACTION_EXISTTRUNCATED :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: file existed and was truncated", &family));
    break;
  default :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> %{buf}: unknown", &family));
    break;
  }

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/* Enums */
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_impersonation_show(netwib_conststring title,
                                 netwox_smbcmdcmn_impersonation impersonation,
                                 netwib_buf *pbuf)
{

  netwib_er(netwib_show_array_fmt32(pbuf, "%s=%{uint32}", title,
                                    impersonation));
  switch(impersonation) {
  case NETWOX_SMBCMDCMN_IMPERSONATION_ANONYMOUS :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> anonymous"));
    break;
  case NETWOX_SMBCMDCMN_IMPERSONATION_IDENTIFICATION :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> identification"));
    break;
  case NETWOX_SMBCMDCMN_IMPERSONATION_IMPERSONATION :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> impersonation"));
    break;
  case NETWOX_SMBCMDCMN_IMPERSONATION_DELEGATION :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> delegation"));
    break;
  default :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> unknown"));
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_oplocklevel_show(netwib_conststring title,
                                 netwox_smbcmdcmn_oplocklevel oplocklevel,
                                 netwib_buf *pbuf)
{

  netwib_er(netwib_show_array_fmt32(pbuf, "%s=%{uint32}", title,
                                    oplocklevel));
  switch(oplocklevel) {
  case NETWOX_SMBCMDCMN_OPLOCKLEVEL_NONE :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> no oplock"));
    break;
  case NETWOX_SMBCMDCMN_OPLOCKLEVEL_EXCLUSIVEOPLOCK :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> exclusive oplock"));
    break;
  case NETWOX_SMBCMDCMN_OPLOCKLEVEL_BATCHOPLOCK :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> batch oplock"));
    break;
  case NETWOX_SMBCMDCMN_OPLOCKLEVEL_LEVEL2OPLOCK :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> level 2 oplock"));
    break;
  default :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> unknown"));
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_createfunction_show(netwib_conststring title,
                               netwox_smbcmdcmn_createfunction createfunction,
                               netwib_buf *pbuf)
{

  netwib_er(netwib_show_array_fmt32(pbuf, "%s=%{uint32}", title,
                                    createfunction));
  switch(createfunction) {
  case NETWOX_SMBCMDCMN_CREATEFUNCTION_SUPERSEDE :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> supersede (supersede existing file, if it exists)"));
    break;
  case NETWOX_SMBCMDCMN_CREATEFUNCTION_OPEN :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> open (if file exists open it, else fail)"));
    break;
  case NETWOX_SMBCMDCMN_CREATEFUNCTION_CREATE :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> create (if file exists fail, else create it)"));
    break;
  case NETWOX_SMBCMDCMN_CREATEFUNCTION_OPENIF :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> open If (if file exists open it, else create it)"));
    break;
  case NETWOX_SMBCMDCMN_CREATEFUNCTION_OVERWRITE :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> overwrite (if file exists overwrite, else fail)"));
    break;
  case NETWOX_SMBCMDCMN_CREATEFUNCTION_OVERWRITEIF :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> overwrite If (if file exists overwrite, else create it)"));
    break;
  default :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> unknown"));
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_filetype_show(netwib_conststring title,
                                          netwox_smbcmdcmn_filetype filetype,
                                          netwib_buf *pbuf)
{

  netwib_er(netwib_show_array_fmt32(pbuf, "%s=%{uint16}", title,
                                    filetype));
  switch(filetype) {
  case NETWOX_SMBCMDCMN_FILETYPE_FILEDIR :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> file or directory"));
    break;
  case NETWOX_SMBCMDCMN_FILETYPE_PIPEBYTE :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> named pipe in byte mode"));
    break;
  case NETWOX_SMBCMDCMN_FILETYPE_PIPEPMSG :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> named pipe in message mode"));
    break;
  case NETWOX_SMBCMDCMN_FILETYPE_LPT :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> printer"));
    break;
  case NETWOX_SMBCMDCMN_FILETYPE_COM :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> com"));
    break;
  default :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> unknown"));
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_sharetype_show(netwib_conststring title,
                                          netwox_smbcmdcmn_sharetype sharetype,
                                          netwib_buf *pbuf)
{

  netwib_er(netwib_show_array_fmt32(pbuf, "%s=%{uint16}", title,
                                    sharetype));
  switch(sharetype) {
  case NETWOX_SMBCMDCMN_SHARETYPE_DIR :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> directory"));
    break;
  case NETWOX_SMBCMDCMN_SHARETYPE_PRINTER :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> printer"));
    break;
  case NETWOX_SMBCMDCMN_SHARETYPE_COM :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> communication device"));
    break;
  case NETWOX_SMBCMDCMN_SHARETYPE_IPC :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> IPC"));
    break;
  default :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> unknown"));
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_seektype_show(netwib_conststring title,
                                          netwox_smbcmdcmn_seektype seektype,
                                          netwib_buf *pbuf)
{

  netwib_er(netwib_show_array_fmt32(pbuf, "%s=%{uint16}", title, seektype));
  switch(seektype) {
  case NETWOX_SMBCMDCMN_SEEKTYPE_BEGIN :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> begin"));
    break;
  case NETWOX_SMBCMDCMN_SEEKTYPE_CURRENT :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> current"));
    break;
  case NETWOX_SMBCMDCMN_SEEKTYPE_END :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> end"));
    break;
  default :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> unknown"));
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdcmn_alignment_show(netwib_conststring title,
                                          netwox_smbcmdcmn_alignment alignment,
                                          netwib_buf *pbuf)
{

  netwib_er(netwib_show_array_fmt32(pbuf, "%s=%{uint32}", title, alignment));
  switch(alignment) {
  case NETWOX_SMBCMDCMN_ALIGNMENT_1 :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> 1 bit"));
    break;
  case NETWOX_SMBCMDCMN_ALIGNMENT_2 :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> 2 bit"));
    break;
  case NETWOX_SMBCMDCMN_ALIGNMENT_4 :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> 4 bit"));
    break;
  case NETWOX_SMBCMDCMN_ALIGNMENT_8 :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> 8 bit"));
    break;
  case NETWOX_SMBCMDCMN_ALIGNMENT_16 :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> 16 bit"));
    break;
  case NETWOX_SMBCMDCMN_ALIGNMENT_32 :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> 32 bit"));
    break;
  case NETWOX_SMBCMDCMN_ALIGNMENT_64 :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> 64 bit"));
    break;
  case NETWOX_SMBCMDCMN_ALIGNMENT_128 :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> 128 bit"));
    break;
  case NETWOX_SMBCMDCMN_ALIGNMENT_256 :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> 256 bit"));
    break;
  case NETWOX_SMBCMDCMN_ALIGNMENT_512 :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> 512 bit"));
    break;
  default :
    netwib_er(netwib_show_array_fmt32(pbuf, "  -> unknown"));
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmd_append_buf(netwox_constsmbmsg *psmbmsg,
                                    netwib_constbuf *pbuf,
                                    netwib_buf *pbufcmd)
{

  if (psmbmsg->hdr.flags2 & NETWOX_SMBMSG_FLAGS2_UNICODESTRINGS) {
    netwib_er(netwox_buf_decode_ucs2le(pbuf, pbufcmd));
  } else {
    netwib_er(netwib_buf_append_buf(pbuf, pbufcmd));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmd_append_buf_uc(netwox_constsmbmsg *psmbmsg,
                                       netwib_constbuf *pbuf,
                                       netwib_buf *pbufcmd)
{

  if (psmbmsg->hdr.flags2 & NETWOX_SMBMSG_FLAGS2_UNICODESTRINGS) {
    netwib_er(netwox_buf_decode_ucs2le_uc(pbuf, pbufcmd));
  } else {
    netwib_er(netwib_buf_encode(pbuf, NETWIB_ENCODETYPE_UPPERCASE, pbufcmd));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmd_append_buf_path(netwox_constsmbmsg *psmbmsg,
                                         netwib_constbuf *pbuf,
                                         netwib_buf *pbufcmd)
{
  netwib_buf buf;
  netwib_data data;
  netwib_uint32 datasize;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_buf(pbuf, &buf));
  data = netwib__buf_ref_data_ptr(&buf);
  datasize = netwib__buf_ref_data_size(&buf);
  while(datasize--) {
    if (*data == '/') {
      *data = '\\';
    }
    data++;
  }

  ret = netwox_smbcmd_append_buf(psmbmsg, &buf, pbufcmd);

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmd_decode_buf(netwox_constsmbmsg *psmbmsg,
                                    netwib_constbuf *pbufcmd,
                                    netwib_buf *pbuf)
{

  if (psmbmsg->hdr.flags2 & NETWOX_SMBMSG_FLAGS2_UNICODESTRINGS) {
    netwib_er(netwox_buf_append_ucs2le(pbufcmd, pbuf));
  } else {
    netwib_er(netwib_buf_append_buf(pbufcmd, pbuf));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmd_decode_buf_path(netwox_constsmbmsg *psmbmsg,
                                         netwib_constbuf *pbufcmd,
                                         netwib_buf *pbuf)
{
  netwib_buf buf;
  netwib_data data;
  netwib_uint32 datasize;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  ret = netwox_smbcmd_decode_buf(psmbmsg, pbufcmd, &buf);
  if (ret == NETWIB_ERR_OK) {
    data = netwib__buf_ref_data_ptr(&buf);
    datasize = netwib__buf_ref_data_size(&buf);
    while(datasize--) {
      if (*data == '\\') {
        *data = '/';
      }
      data++;
    }
    netwib_er(netwib_buf_append_buf(&buf, pbuf));
  }

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}
