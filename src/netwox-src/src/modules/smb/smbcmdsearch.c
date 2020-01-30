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
netwib_err netwox_smbcmdsearch_resumekey_init(netwox_smbcmdsearch_resumekey *presumekey)
{
  netwib_er(netwox_smbcmdcmn_data_initdefault(&presumekey->compactedfilename));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdsearch_resumekey_setdefault(netwox_smbcmdsearch_resumekey *presumekey)
{
  presumekey->reserved = 0;
  netwib_er(netwox_smbcmdcmn_data_setdefault(&presumekey->compactedfilename));
  presumekey->servercookie1 = 0;
  presumekey->servercookie2 = 0;
  presumekey->clientcookie = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdsearch_resumekey_close(netwox_smbcmdsearch_resumekey *presumekey)
{
  netwib_er(netwox_smbcmdcmn_data_close(&presumekey->compactedfilename));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdsearch_resumekey_decode(netwib_data *pdata,
                                                netwib_uint32 *pdatasize,
                                                netwox_smbcmdsearch_resumekey *presumekey)
{
  netwib_data data;
  netwib_uint32 datasize;

  data = *pdata;
  datasize = *pdatasize;

  if (datasize < 1) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwib__data_decode_uint8_le(data, presumekey->reserved);
  datasize -= 1;

  netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, NETWOX_SMBCMDSEARCH_RESUMEKEY_COMPACTEDFILENAME_LEN, &presumekey->compactedfilename));
  if (datasize < 9) {
    return(NETWIB_ERR_DATAMISSING);
  }
  netwib__data_decode_uint8_le(data, presumekey->servercookie1);
  netwib__data_decode_uint32_le(data, presumekey->servercookie2);
  netwib__data_decode_uint32_le(data, presumekey->clientcookie);
  datasize -= 9;

  /* no check on left datasize because this function is used to
     decode partial data followed by other data */

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdsearch_resumekey_append(const netwox_smbcmdsearch_resumekey *presumekey,
                                                netwib_buf *pbuf)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(pbuf, 1, &data));
  netwib__data_append_uint8_le(data, presumekey->reserved);
  pbuf->endoffset += 1;

  if (netwib__buf_ref_data_size(&presumekey->compactedfilename) != NETWOX_SMBCMDSEARCH_RESUMEKEY_COMPACTEDFILENAME_LEN) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  netwib_er(netwib_buf_append_buf(&presumekey->compactedfilename, pbuf));

  netwib_er(netwib_buf_wantspace(pbuf, 9, &data));
  netwib__data_append_uint8_le(data, presumekey->servercookie1);
  netwib__data_append_uint32_le(data, presumekey->servercookie2);
  netwib__data_append_uint32_le(data, presumekey->clientcookie);
  pbuf->endoffset += 9;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdsearch_resumekey_show(const netwox_smbcmdsearch_resumekey *presumekey,
                                              netwib_buf *pbuf)
{
  netwib_er(netwib_show_array_fmt32(pbuf, "  Resume Key:"));
  netwib_er(netwib_show_array_fmt32(pbuf, "   reserved=%{uint8:#8b}", presumekey->reserved));
  netwib_er(netwox_smbcmdcmn_data_show("   compactedfilename", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &presumekey->compactedfilename, pbuf));
  netwib_er(netwib_show_array_fmt32(pbuf, "   servercookie=%{uint8:#02X}%{uint32:08X}", presumekey->servercookie1, presumekey->servercookie2));
  netwib_er(netwib_show_array_fmt32(pbuf, "   clientcookie=%{uint32:#08X}", presumekey->clientcookie));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdsearch_resumekey_display(netwib_bufpool *pbufpool,
                                                 const netwox_smbcmdsearch_resumekey *presumekey)
{
  netwib_buf *pbuf;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));

  ret = netwox_smbcmdsearch_resumekey_show(presumekey, pbuf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_display(pbuf, NETWIB_ENCODETYPE_DATA));
  } else {
    netwib_er(netwib_fmt_display("This Search Resume Key could not be decoded\n"));
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdsearch_resumekey_encode_smbcmd(netwox_constsmbcmd *psmbcmd,
                                                       netwox_smbcmdsearch_resumekey *presumekey)
{
  netwib_data data;
  netwib_uint32 datasize;

  if (psmbcmd->type != NETWOX_SMBCMD_TYPE_SEARCH_Q) {
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  data = netwib__buf_ref_data_ptr(&psmbcmd->value.search_q.resumekey);
  datasize = netwib__buf_ref_data_size(&psmbcmd->value.search_q.resumekey);

  netwib_er(netwox_smbcmdsearch_resumekey_decode(&data, &datasize, presumekey));
  if (datasize != 0) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdsearch_resumekey_decode_smbcmd(const netwox_smbcmdsearch_resumekey *presumekey,
                                                       netwox_smbcmd *psmbcmd)
{
  netwib_er(netwox_smbcmdsearch_resumekey_append(presumekey, &psmbcmd->value.search_q.resumekey));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdsearch_entry_init(netwox_smbcmdsearch_entry *pentry)
{
  netwib_er(netwox_smbcmdsearch_resumekey_init(&pentry->resumekey));
  netwib_er(netwox_smbcmdcmn_data_initdefault(&pentry->filename));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdsearch_entry_setdefault(netwox_smbcmdsearch_entry *pentry)
{
  netwib_er(netwox_smbcmdsearch_resumekey_setdefault(&pentry->resumekey));

  netwox__smbcmdcmn_fileattr16_initdefault(pentry->fileattr);
  netwox__smbcmdcmn_timemsdos_initdefault(pentry->lastwritetime);
  pentry->filesize = 0;
  netwib_er(netwox_smbcmdcmn_data_setdefault(&pentry->filename));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdsearch_entry_close(netwox_smbcmdsearch_entry *pentry)
{
  netwib_er(netwox_smbcmdsearch_resumekey_close(&pentry->resumekey));
  netwib_er(netwox_smbcmdcmn_data_close(&pentry->filename));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdsearch_entry_decode(netwib_data *pdata,
                                            netwib_uint32 *pdatasize,
                                            netwox_smbcmdsearch_entry *pentry)
{
  netwib_data data, pc;
  netwib_uint32 datasize;

  data = *pdata;
  datasize = *pdatasize;

  netwib_er(netwox_smbcmdsearch_resumekey_decode(&data, &datasize, &pentry->resumekey));
  if (datasize < 9) {
    return(NETWIB_ERR_DATAMISSING);
  }
  /* fileattr is only stored on 8 bits instead of 16 bits */
  netwib__data_decode_uint8_le(data, pentry->fileattr);
  netwox__smbcmdcmn_timemsdosreversed_decode(data, pentry->lastwritetime);
  netwib__data_decode_uint32_le(data, pentry->filesize);
  datasize -= 9;

  netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, NETWOX_SMBCMDSEARCH_FILENAME_MAXLEN + 1, &pentry->filename));
  pc = netwib_c_memchr(netwib__buf_ref_data_ptr(&pentry->filename), '\0',
                       netwib__buf_ref_data_size(&pentry->filename));
  if (pc == NULL) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  pentry->filename.endoffset = pentry->filename.beginoffset + (pc - netwib__buf_ref_data_ptr(&pentry->filename));

  /* no check on left datasize because this function is used to
     decode partial data followed by other data */

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdsearch_entry_append(const netwox_smbcmdsearch_entry *pentry,
                                            netwib_buf *pbuf)
{
  netwib_data data;

  netwib_er(netwox_smbcmdsearch_resumekey_append(&pentry->resumekey, pbuf));

  netwib_er(netwib_buf_wantspace(pbuf, 9 + NETWOX_SMBCMDSEARCH_FILENAME_MAXLEN+1, &data));
  netwib__data_append_uint8_le(data, pentry->fileattr);
  netwox__smbcmdcmn_timemsdosreversed_append(data, pentry->lastwritetime);
  netwib__data_append_uint32_le(data, pentry->filesize);

  if (netwib__buf_ref_data_size(&pentry->filename) > NETWOX_SMBCMDSEARCH_FILENAME_MAXLEN) {
    return(NETWIB_ERR_PATOOHIGH);
  }
  /* fill with spaces and end with nul */
  netwib_c_memset(data, 0x20, NETWOX_SMBCMDSEARCH_FILENAME_MAXLEN);
  netwib_c_memcpy(data, netwib__buf_ref_data_ptr(&pentry->filename),
                  netwib__buf_ref_data_size(&pentry->filename));
  data[netwib__buf_ref_data_size(&pentry->filename)] = '\0';

  pbuf->endoffset += 9 + NETWOX_SMBCMDSEARCH_FILENAME_MAXLEN+1;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdsearch_entry_show(const netwox_smbcmdsearch_entry *pentry,
                                          netwib_buf *pbuf)
{

  netwib_er(netwox_smbcmdsearch_resumekey_show(&pentry->resumekey, pbuf));
  netwib_er(netwox_smbcmdcmn_fileattr16_show("  fileattr", pentry->fileattr, pbuf));
  netwib_er(netwox_smbcmdcmn_timemsdos_show("  lastwritetime", pentry->lastwritetime, pbuf));
  netwib_er(netwib_show_array_fmt32(pbuf, "  filesize=%{uint32}", pentry->filesize));
  netwib_er(netwox_smbcmdcmn_data_show("  shortfilename", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &pentry->filename, pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdsearch_entries_init(netwox_smbcmdsearch_entries *pentries)
{
  netwib_er(netwib_array_init(sizeof(netwox_smbcmdsearch_entry), 0,
                              &pentries->entries));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdsearch_entries_setdefault(netwox_smbcmdsearch_entries *pentries)
{
  netwib_uint32 i;
  netwox_smbcmdsearch_entry *pentry;

  for (i = 0; i < pentries->entries.size; i++) {
    pentry = (netwox_smbcmdsearch_entry*)pentries->entries.p[i];
    netwib_er(netwox_smbcmdsearch_entry_setdefault(pentry));
  }
  netwib_er(netwib_array_ctl_set_size(&pentries->entries, 0));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdsearch_entries_close(netwox_smbcmdsearch_entries *pentries)
{
  netwib_uint32 i;
  netwox_smbcmdsearch_entry *pentry;

  for (i = 0; i < pentries->entries.size; i++) {
    pentry = (netwox_smbcmdsearch_entry*)pentries->entries.p[i];
    netwib_er(netwox_smbcmdsearch_entry_close(pentry));
  }
  netwib_er(netwib_array_close(&pentries->entries));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdsearch_entries_show(const netwox_smbcmdsearch_entries *pentries,
                                            netwib_buf *pbuf)
{
  netwib_uint32 i;
  netwox_smbcmdsearch_entry *pentry;

  for (i = 0; i < pentries->entries.size; i++) {
    pentry = (netwox_smbcmdsearch_entry*)pentries->entries.p[i];
    netwib_er(netwox_smbcmdsearch_entry_show(pentry, pbuf));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdsearch_entries_display(netwib_bufpool *pbufpool,
                                               const netwox_smbcmdsearch_entries *pentries)
{
  netwib_buf *pbuf;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));

  ret = netwox_smbcmdsearch_entries_show(pentries, pbuf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_display(pbuf, NETWIB_ENCODETYPE_DATA));
  } else {
    netwib_er(netwib_fmt_display("These Search Entries could not be decoded\n"));
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdsearch_entries_encode_smbcmd(netwox_constsmbcmd *psmbcmd,
                                                     netwox_smbcmdsearch_entries *pentries)
{
  netwib_data data;
  netwib_uint32 datasize, i;
  netwox_smbcmdsearch_entry *pentry;

  if (psmbcmd->type != NETWOX_SMBCMD_TYPE_SEARCH_R) {
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  data = netwib__buf_ref_data_ptr(&psmbcmd->value.search_r.entries);
  datasize = netwib__buf_ref_data_size(&psmbcmd->value.search_r.entries);

  for (i = 0; i < psmbcmd->value.search_r.count; i++) {
    netwib_er(netwib_array_ctl_set_size(&pentries->entries, i+1));
    pentry = (netwox_smbcmdsearch_entry*)pentries->entries.p[i];
    netwib_er(netwox_smbcmdsearch_entry_init(pentry));
    netwib_er(netwox_smbcmdsearch_entry_decode(&data, &datasize, pentry));
  }

  if (datasize != 0) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmdsearch_entries_decode_smbcmd(const netwox_smbcmdsearch_entries *pentries,
                                                     netwox_smbcmd *psmbcmd)
{
  netwib_uint32 i;
  netwox_smbcmdsearch_entry *pentry;

  for (i = 0; i < pentries->entries.size; i++) {
    pentry = (netwox_smbcmdsearch_entry*)pentries->entries.p[i];
    netwib_er(netwox_smbcmdsearch_entry_append(pentry, &psmbcmd->value.search_r.entries));
    psmbcmd->value.search_r.count++;
  }

  return(NETWIB_ERR_OK);
}

