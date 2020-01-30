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
static netwib_err netwox_smbsercmdsearch_add_file(netwib_constbuf *pfilename,
                                                  netwox_smbcmdsearch_entry *pse)
{
  netwib_data datain, dataout, pc;
  netwib_uint32 datainsize, dataoutsize, sizetocopy;

  if (netwib__buf_ref_data_size(pfilename) > NETWOX_SMBCMDSEARCH_FILENAME_MAXLEN) {
    return(NETWOX_ERR_INTERNALERROR);
  } else {
    datain = netwib__buf_ref_data_ptr(pfilename);
    datainsize = netwib__buf_ref_data_size(pfilename);
    /* pse->filename contains the 8.3 filename */
    netwib_er(netwib_buf_wantspace(&pse->filename, NETWOX_SMBCMDSEARCH_FILENAME_MAXLEN, &dataout));
    dataoutsize = 0;
    /*   append filename */
    netwib_c_memcpy(dataout, datain, datainsize); /* size checked above */
    dataout += datainsize;
    dataoutsize += datainsize;
    /*   end with '\0' */
    if (dataoutsize < NETWOX_SMBCMDSEARCH_FILENAME_MAXLEN) {
      *dataout++ = '\0';
      dataoutsize++;
    }
    /*   pad with spaces */
    netwib_c_memset(dataout, ' ', NETWOX_SMBCMDSEARCH_FILENAME_MAXLEN-dataoutsize);
    /*   indicate new size */
    pse->filename.endoffset = pse->filename.beginoffset + NETWOX_SMBCMDSEARCH_FILENAME_MAXLEN;
    /* we have to remove the dot of pfilename to set compactedfilename */
    netwib_er(netwib_buf_wantspace(&pse->resumekey.compactedfilename, NETWOX_SMBCMDSEARCH_RESUMEKEY_COMPACTEDFILENAME_LEN, &dataout));
    dataoutsize = 0;
    pc = netwib_c_memchr(datain, '.', datainsize);
    if (pc == NULL) {
      /*   filename without dot: ensure not too long, then copy */
      sizetocopy = datainsize;
      if (sizetocopy > NETWOX_SMBCMDSEARCH_RESUMEKEY_COMPACTEDFILENAME_LEN) {
        sizetocopy = NETWOX_SMBCMDSEARCH_RESUMEKEY_COMPACTEDFILENAME_LEN;
      }
      netwib_c_memcpy(dataout, datain, sizetocopy);
      dataout += sizetocopy;
      dataoutsize += sizetocopy;
    } else {
      /*   name before extension */
      sizetocopy = pc - datain;
      netwib_c_memcpy(dataout, datain, sizetocopy);
      dataout += sizetocopy;
      dataoutsize += sizetocopy;
      /*   extension */
      sizetocopy = datainsize - (pc+1 - datain);
      netwib_c_memcpy(dataout, pc+1, sizetocopy);
      dataout += sizetocopy;
      dataoutsize += sizetocopy;
    }
    /*   pad with spaces (there is no final '\0' before) */
    netwib_c_memset(dataout, ' ', NETWOX_SMBCMDSEARCH_RESUMEKEY_COMPACTEDFILENAME_LEN - dataoutsize);
    /*   indicate new size */
    pse->resumekey.compactedfilename.endoffset = pse->resumekey.compactedfilename.beginoffset + NETWOX_SMBCMDSEARCH_RESUMEKEY_COMPACTEDFILENAME_LEN;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbsercmdsearch_add(netwox_smbser *psmbser,
                                             netwox_smbcmdsearch_resumekey *presumekeyq,
                                             netwib_uint32 id,
                                             netwib_bool isvolume,
                                             netwib_constbuf *prealfilename,
                                             netwib_array *parray,
                                             netwib_bool *padded)
{
  netwib_pathstat pathstat;
  netwox_smbcmdsearch_entry *pse;
  netwib_buf *pbuf1, *pbuf2;
  netwib_err ret;

  *padded = NETWIB_FALSE;

  pathstat.size = 0; pathstat.type = 0; /* for compiler warning */
  if (!isvolume) {
    ret = netwib_pathstat_init(prealfilename, &pathstat);
    if (ret != NETWIB_ERR_OK) {
      psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
      return(ret);
    }
  }

  netwib_er(netwib_array_ctl_set_size(parray, parray->size+1));
  pse = (netwox_smbcmdsearch_entry *)parray->p[parray->size-1];
  netwib_er(netwox_smbcmdsearch_entry_init(pse));
  netwib_er(netwox_smbcmdsearch_entry_setdefault(pse));

  /* resumekey (compactedfilename set after) */
  pse->resumekey.reserved = presumekeyq->reserved;
  pse->resumekey.servercookie1 = 0x01;
  pse->resumekey.servercookie2 = id;
  pse->resumekey.clientcookie = presumekeyq->clientcookie;

  /* fileattr and filesize */
  if (isvolume) {
     pse->filesize = 0;
     pse->fileattr = NETWOX_SMBCMDCMN_FILEATTR16_HIDDEN | NETWOX_SMBCMDCMN_FILEATTR16_SYSTEM | NETWOX_SMBCMDCMN_FILEATTR16_VOLUME | NETWOX_SMBCMDCMN_FILEATTR16_DIR | NETWOX_SMBCMDCMN_FILEATTR16_ARCHIVE;
  } else {
    if (!psmbser->pcommon->allowput) {
      pse->fileattr |= NETWOX_SMBCMDCMN_FILEATTR16_READONLY;
    }
    if (pathstat.type == NETWIB_PATHSTAT_TYPE_DIR) {
      pse->fileattr |= NETWOX_SMBCMDCMN_FILEATTR16_DIR;
      pse->filesize = 0;
    } else {
      pse->filesize = pathstat.size;
    }
  }

  /* lastwritetime */
  if (isvolume) {
    pse->lastwritetime.msdosdate = 0;
    pse->lastwritetime.msdostime = 0;
  } else {
    netwib_er(netwox_timemsdos_init_time(&pathstat.mtime, &pse->lastwritetime));
  }

  /* filename */
  if (isvolume) {
    ret = netwox_smbsercmdsearch_add_file(prealfilename, pse);
  } else {
    netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pbuf1));
    netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pbuf2));
    ret = netwib_path_decode_child(prealfilename, pbuf1);
    if (ret == NETWIB_ERR_OK) {
      ret = netwox_smbser83_longto83_path(&psmbser->smbser83, pathstat.type,
                                          pbuf1, pbuf2);
      if (ret == NETWIB_ERR_OK) {
        ret = netwox_smbsercmdsearch_add_file(pbuf2, pse);
      }
    }
    netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pbuf2));
    netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pbuf1));
  }

  *padded = NETWIB_TRUE;
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_search(netwox_smbser *psmbser,
                                   netwox_constsmbmsg *psmbmsgq,
                                   netwox_constsmbcmd *psmbcmdq,
                                   netwox_smbmsg *psmbmsgr,
                                   netwox_smbcmd *psmbcmdr,
                                   netwib_constbuf *prootdir)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_buf *psearchpattern, *prealpathname;
  netwox_smbserglob_item *pitem;
  netwox_smbcmdsearch_resumekey resumekeyq;
  netwox_smbcmdsearch_entries entriesr;
  netwib_uint16 maxentries;
  netwib_uint32 i, id;
  netwib_bool normalend, idinit=NETWIB_FALSE, added, isvolume, endofsearch;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbsercmd_search\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_SEARCH_Q) {
    netwib_er(NETWOX_ERR_PROTOCOL);
  }
  maxentries = psmbcmdq->value.search_q.maxentries;

  /* init */
  ret = NETWIB_ERR_OK;
  normalend = NETWIB_FALSE;
  idinit = NETWIB_FALSE;
  endofsearch = NETWIB_FALSE;
  id = 0;
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &psearchpattern));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &prealpathname));
  netwib_eg(netwox_smbcmdsearch_resumekey_init(&resumekeyq));
  netwib_eg(netwox_smbcmdsearch_entries_init(&entriesr));
  netwib_eg(netwox_smbcmdsearch_entries_setdefault(&entriesr));

  /* analyze request */
  if (netwib__buf_ref_data_size(&psmbcmdq->value.search_q.resumekey) == 0) {
    netwib_eg(netwox_smbcmd_decode_buf_path(psmbmsgq, &psmbcmdq->value.search_q.searchpattern, psearchpattern));
    if (psmbser->pcommon->verbose) {
      netwib_eg(netwib_fmt_display("Search pattern: %{buf}\n", psearchpattern));
    }
    netwib_eg(netwox_smbserglob_new(&psmbser->smbserglob, &id, &pitem));
    idinit = NETWIB_TRUE;
    netwib_eg(netwox_smbserjailglob_E(psmbser, prootdir, psmbcmdq->value.search_q.searchattributes, psearchpattern, pitem));
    netwib_eg(netwox_smbcmdsearch_resumekey_setdefault(&resumekeyq));
  } else {
    netwib_eg(netwox_smbcmdsearch_resumekey_encode_smbcmd(psmbcmdq,
                                                          &resumekeyq));
    id = resumekeyq.servercookie2;
    if (psmbser->pcommon->verbose) {
      netwib_eg(netwib_fmt_display("Search id: %{uint32}\n", id));
    }
    ret = netwox_smbserglob_search(&psmbser->smbserglob, id, &pitem);
    if (ret != NETWIB_ERR_OK) {
      psmbser->smberrtosend = NETWOX_SMBERR_BADFILEID;
      netwib_goto(NETWIB_ERR_OK);
    }
    idinit = NETWIB_TRUE;
  }

  /* construct answer */
  netwib_eg(netwox_smbcmd_selecttype(psmbcmdr, NETWOX_SMBCMD_TYPE_SEARCH_R));
  netwib_eg(netwox_smbcmd_setdefault(psmbcmdr));

  /* ensure maxentries is not too big */
  if (((netwib_uint32)maxentries * NETWOX_SMBCMDSEARCH_ENTRY_PKT_LEN) > (psmbser->cmn_maxmsgsize - NETWOX_SMBSER_SUFFICIENT_FOR_HEADERS)) {
    maxentries = (netwib_uint16)((psmbser->cmn_maxmsgsize - NETWOX_SMBSER_SUFFICIENT_FOR_HEADERS) / NETWOX_SMBCMDSEARCH_ENTRY_PKT_LEN);
  }

  /* special case of volume */
  isvolume = (pitem->searchattributes & NETWOX_SMBCMDCMN_FILEATTR32_VOLUME)?NETWIB_TRUE:NETWIB_FALSE;

  /* add entries */
  i = 0;
  while(NETWIB_TRUE) {
    netwib__buf_reinit(prealpathname);
    ret = netwox_smbserglob_item_next(&psmbser->smbser83, pitem, prealpathname);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) {
        endofsearch = NETWIB_TRUE;
        ret = NETWIB_ERR_OK;
        break;
      }
      netwib_eg(ret);
    }
    if (psmbser->pcommon->verbose) {
      netwib_eg(netwib_fmt_display("File: %{buf}\n", prealpathname));
    }
    netwib_eg(netwox_smbsercmdsearch_add(psmbser, &resumekeyq, id, isvolume, prealpathname, &entriesr.entries, &added));
    if (added) {
      i++;
      if (i >= maxentries) {
        break;
      }
    }
  }
  netwib_eg(netwox_smbcmdsearch_entries_decode_smbcmd(&entriesr, psmbcmdr));
  psmbcmdr->value.search_r.count = (netwib_uint16)entriesr.entries.size;
  if (endofsearch && (i == 0)) {
    psmbser->smberrtosend = NETWOX_SMBERR_NOMOREFILES;
    netwib_goto(NETWIB_ERR_OK);
  }

  /* end */
  normalend = NETWIB_TRUE;
 netwib_gotolabel:
  if (idinit && !normalend) {
    netwib_er(netwox_smbserglob_del(&psmbser->smbserglob, id));
  }
  netwib_er(netwox_smbcmdsearch_entries_close(&entriesr));
  netwib_er(netwox_smbcmdsearch_resumekey_close(&resumekeyq));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &prealpathname));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &psearchpattern));
  psmbmsgr = psmbmsgr; /* for compiler warning */
  return(ret);
}

