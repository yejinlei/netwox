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
static netwib_err netwox_smbsercmd_transaction2_find_add(netwox_smbser *psmbser,
                                                         netwox_constsmbmsg *psmbmsgq,
                                                         netwox_constsmbcmdtsc2rfmt *psmbcmdtsc2rfmt,
                                                         netwib_constbuf *prealfilename,
                                                         netwib_array *parray,
                                                         netwib_bool *padded,
                                                         netwib_uint32 *ptsc2datasize)
{
  netwib_pathstat pathstat;
  netwox_smbcmdtsc2_fmt_find_entry *pfe;
  netwib_buf *pbuf1, *pbuf2;
  netwib_uint32 entrysize;
  netwib_err ret;

  *padded = NETWIB_FALSE;

  ret = netwib_pathstat_init(prealfilename, &pathstat);
  if (ret != NETWIB_ERR_OK) {
    psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
    return(ret);
  }

  netwib_er(netwib_array_ctl_set_size(parray, parray->size+1));
  pfe = (netwox_smbcmdtsc2_fmt_find_entry *)parray->p[parray->size-1];
  netwib_er(netwox_smbcmdtsc2_fmt_find_entry_init(pfe));
  netwib_er(netwox_smbcmdtsc2_fmt_find_entry_setdefault(psmbcmdtsc2rfmt->fmtfind, pfe));

  netwib_er(netwox_time1601_init_time(&pathstat.mtime, &pfe->changetime));
  /* fake values for creation, lastaccess and lastwrite but should be ok */
  netwib_er(netwox_time1601_init_time(&pathstat.mtime, &pfe->creationtime));
  netwib_er(netwox_time1601_init_time(&pathstat.mtime, &pfe->lastaccesstime));
  netwib_er(netwox_time1601_init_time(&pathstat.mtime, &pfe->lastwritetime));

  if (!psmbser->pcommon->allowput) {
    pfe->fileattr16 |= NETWOX_SMBCMDCMN_FILEATTR16_READONLY;
    pfe->fileattr32 |= NETWOX_SMBCMDCMN_FILEATTR32_READONLY;
  }
  if (pathstat.type == NETWIB_PATHSTAT_TYPE_DIR) {
    pfe->fileattr16 |= NETWOX_SMBCMDCMN_FILEATTR16_DIR;
    pfe->fileattr32 |= NETWOX_SMBCMDCMN_FILEATTR32_DIR;
    netwib__uint64_init_uint32(0, pfe->allocationsize);
    netwib__uint64_init_uint32(0, pfe->filesize);
  } else {
    pfe->allocationsize = pathstat.size64;
    pfe->filesize = pathstat.size64;
  }

  /* name */
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pbuf1));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pbuf2));
  ret = netwib_path_decode_child(prealfilename, pbuf1);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwox_smbcmd_append_buf_path(psmbmsgq, pbuf1,
                                            &pfe->longfilename));
    netwib_er(netwox_smbser83_longto83_path(&psmbser->smbser83, pathstat.type,
                                            pbuf1, pbuf2));
    netwib_er(netwox_smbcmd_append_buf_path(psmbmsgq, pbuf2,
                                            &pfe->shortfilename));
  }
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pbuf2));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pbuf1));

  /* estimate the size of this entry */
  netwib_er(netwox_smbcmdtsc2_fmt_find_entry_size(psmbcmdtsc2rfmt, pfe, &entrysize));
  *ptsc2datasize += entrysize + 4/*nextentryoffset used to skip*/;

  *padded = NETWIB_TRUE;
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbsercmd_transaction2_find(netwox_smbser *psmbser,
                                                     netwox_constsmbmsg *psmbmsgq,
                                                     netwox_constsmbcmd *psmbcmdq,
                                                     netwox_constsmbcmdtsc2q *psmbcmdtsc2q,
                                                     netwox_constsmbcmdtsc2rfmt *psmbcmdtsc2rfmt,
                                                     netwib_constbuf *prootdir,
                                                     netwox_smbcmdtsc2r *psmbcmdtsc2r)
{
  netwib_buf *psearchpattern, *prealpathname;
  netwox_smbserglob_item *pitem;
  netwox_smbcmdtsc2r_type type;
  netwox_smbcmdtsc2_fmt_find fmt;
  netwox_smbcmdtsc2_fmt_find_entry *pfe;
  netwib_uint16 maxentries, flags;
  netwib_uint32 i, id, maxtsc2datasize, tsc2datasize;
  netwib_bool normalend, endofsearch, iteminit, itemdel, added, isfirst;
  netwib_array *parray;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbsercmd_transaction2_find\n"));
  }

  /* init */
  ret = NETWIB_ERR_OK;
  normalend = NETWIB_FALSE;
  endofsearch = NETWIB_FALSE;
  iteminit = NETWIB_FALSE;
  flags = 0;
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &psearchpattern));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &prealpathname));

  /* analyze request */
  /* ignore psmbcmdtsc2q->value.findfirst2.storagetype */
  isfirst = NETWIB_FALSE;
  flags = 0;
  id = 0;
  if (psmbcmdtsc2q->type == NETWOX_SMBCMDTSC2Q_TYPE_FINDFIRST2) {
    isfirst = NETWIB_TRUE;
    flags = psmbcmdtsc2q->value.findfirst2.flags;
    netwib_eg(netwox_smbcmd_decode_buf_path(psmbmsgq, &psmbcmdtsc2q->value.findfirst2.searchpattern, psearchpattern));
    if (psmbser->pcommon->verbose) {
      netwib_eg(netwib_fmt_display("Search pattern: %{buf}\n", psearchpattern));
    }
    if (netwib__buf_ref_data_size(psearchpattern) == 0) {
      netwib_eg(netwib_buf_append_string("\\*", psearchpattern));
    }
    netwib_eg(netwox_smbserglob_new(&psmbser->smbserglob, &id, &pitem));
    iteminit = NETWIB_TRUE;
    netwib_eg(netwox_smbserjailglob_E(psmbser, prootdir, psmbcmdtsc2q->value.findfirst2.searchattributes, psearchpattern, pitem));
  } else {
    isfirst = NETWIB_FALSE;
    flags = psmbcmdtsc2q->value.findnext2.flags;
    id = psmbcmdtsc2q->value.findnext2.sid;
    if (psmbser->pcommon->verbose) {
      netwib_eg(netwib_fmt_display("Search id: %{uint32}\n", id));
    }
    ret = netwox_smbserglob_search(&psmbser->smbserglob, id, &pitem);
    if (ret != NETWIB_ERR_OK) {
      psmbser->smberrtosend = NETWOX_SMBERR_BADFILEID;
      netwib_goto(NETWIB_ERR_OK);
    }
  }

  /* determine limits */
  maxentries = 0;
  if (isfirst) {
    maxentries = psmbcmdtsc2q->value.findfirst2.maxentries;
  } else {
    maxentries = psmbcmdtsc2q->value.findnext2.maxentries;
  }
  maxtsc2datasize = psmbcmdq->value.transaction2_q.maxdatasize;

  /* construct answer */
  type = 0;
  fmt = 0;
  if (isfirst) {
    fmt = psmbcmdtsc2q->value.findfirst2.fmt;
  } else {
    fmt = psmbcmdtsc2q->value.findnext2.fmt;
  }
  tsc2datasize = 2; /* lastnameoffset, see smbcmdtsc2.c */
  switch(fmt) {
  case NETWOX_SMBCMDTSC2_FMT_FIND_STANDARD :
    type = isfirst?NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_STANDARD:NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_STANDARD;
    break;
  case NETWOX_SMBCMDTSC2_FMT_FIND_DIRECTORY :
    type = isfirst?NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_DIRECTORY:NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_DIRECTORY;
    break;
  case NETWOX_SMBCMDTSC2_FMT_FIND_FULLDIRECTORY :
    type = isfirst?NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_FULLDIRECTORY:NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_FULLDIRECTORY;
    break;
  case NETWOX_SMBCMDTSC2_FMT_FIND_NAMESINFO :
    type = isfirst?NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_NAMESINFO:NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_NAMESINFO;
    break;
  case NETWOX_SMBCMDTSC2_FMT_FIND_BOTHDIRECTORY :
    type = isfirst?NETWOX_SMBCMDTSC2R_TYPE_FINDFIRST2_BOTHDIRECTORY:NETWOX_SMBCMDTSC2R_TYPE_FINDNEXT2_BOTHDIRECTORY;
    break;
  default :
    netwib_er(netwox_smbser_notimplemented(psmbser));
    psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
    netwib_goto(NETWIB_ERR_OK);
    break;
  }
  netwib_eg(netwox_smbcmdtsc2r_selecttype(psmbcmdtsc2r, type));
  netwib_eg(netwox_smbcmdtsc2r_setdefault(psmbcmdtsc2r));
  if (isfirst) {
    psmbcmdtsc2r->value.findfirst2_common.sid = (netwib_uint16)id;
  }

  /* add entries */
  i = 0;
  parray = NULL;
  if (isfirst) {
    parray = &psmbcmdtsc2r->value.findfirst2_common.entries;
  } else {
    parray = &psmbcmdtsc2r->value.findnext2_common.entries;
  }
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
    netwib_eg(netwox_smbsercmd_transaction2_find_add(psmbser, psmbmsgq, psmbcmdtsc2rfmt, prealpathname, parray, &added, &tsc2datasize));
    if (added) {
      i++;
      if (i >= maxentries) {
        break;
      }
      if (tsc2datasize > maxtsc2datasize) {
        /* save last item for next call */
        if (parray->size >= 1) {
          netwib_eg(netwox_smbserglob_item_save(pitem, prealpathname));
          pfe = (netwox_smbcmdtsc2_fmt_find_entry *)parray->p[parray->size-1];
          netwib_eg(netwox_smbcmdtsc2_fmt_find_entry_close(pfe));
          netwib_eg(netwib_array_ctl_set_size(parray, parray->size-1));
        }
        break;
      }
    }
  }
  if (isfirst) {
    psmbcmdtsc2r->value.findfirst2_common.endofsearch = endofsearch;
  } else {
    psmbcmdtsc2r->value.findnext2_common.endofsearch = endofsearch;
  }

  /* end */
  normalend = NETWIB_TRUE;
 netwib_gotolabel:
  if (iteminit) {
    itemdel = NETWIB_FALSE;
    if (!normalend) {
      itemdel = NETWIB_TRUE;
    } else {
      if (flags & NETWOX_SMBCMDTSC2Q_FIND2_FLAGS_CLOSE) {
        itemdel = NETWIB_TRUE;
      }
      if (endofsearch && (flags & NETWOX_SMBCMDTSC2Q_FIND2_FLAGS_CLOSEONEOS)) {
        itemdel = NETWIB_TRUE;
      }
    }
    if (itemdel) {
      netwib_er(netwox_smbserglob_del(&psmbser->smbserglob, id));
    }
  }
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &prealpathname));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &psearchpattern));
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbsercmd_transaction2_queryfsinfo(netwox_smbser *psmbser,
                                                         netwox_constsmbmsg *psmbmsgq,
                                                         netwox_constsmbcmdtsc2q *psmbcmdtsc2q,
                                                         netwox_smbcmdtsc2r *psmbcmdtsc2r)
{
  netwib_buf buf;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbsercmd_transaction2_queryfsinfo\n"));
  }

  /* init */
  ret = NETWIB_ERR_OK;

  /* ignore psmbcmdtsc2q->value.queryfsinfo.sharename */

  /* construct answer */
  switch(psmbcmdtsc2q->value.queryfsinfo.fmt) {
  case NETWOX_SMBCMDTSC2_FMT_FS_OLDALLOCATION :
    netwib_eg(netwox_smbcmdtsc2r_selecttype(psmbcmdtsc2r, NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_OLDALLOCATION));
    netwib_eg(netwox_smbcmdtsc2r_setdefault(psmbcmdtsc2r));
    psmbcmdtsc2r->value.queryfsinfo_oldallocation.fsid = NETWOX_SMBSER_FAKE_FS_ID;
    psmbcmdtsc2r->value.queryfsinfo_oldallocation.sectorsperunit = NETWOX_SMBSER_FAKE_FS_SECTORSPERUNIT;
    psmbcmdtsc2r->value.queryfsinfo_oldallocation.totalunits = NETWOX_SMBSER_FAKE_FS_TOTALUNITS;
    psmbcmdtsc2r->value.queryfsinfo_oldallocation.freeunits = NETWOX_SMBSER_FAKE_FS_FREEUNITS;
    psmbcmdtsc2r->value.queryfsinfo_oldallocation.bytespersector = (netwib_uint16)NETWOX_SMBSER_FAKE_FS_BYTESPERSECTOR;
    break;
  case NETWOX_SMBCMDTSC2_FMT_FS_OLDVOLUME :
    netwib_er(netwox_smbser_nottested(psmbser));
    netwib_eg(netwox_smbcmdtsc2r_selecttype(psmbcmdtsc2r, NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_OLDVOLUME));
    netwib_eg(netwox_smbcmdtsc2r_setdefault(psmbcmdtsc2r));
    psmbcmdtsc2r->value.queryfsinfo_oldvolume.serialnumber = NETWOX_SMBSER_FAKE_FS_ID;
    netwib_eg(netwib_buf_init_ext_string(NETWOX_SMBSER_FAKE_FS_LABEL, &buf));
    netwib_eg(netwox_smbcmd_append_buf(psmbmsgq, &buf, &psmbcmdtsc2r->value.queryfsinfo_oldvolume.label));
    break;
  case NETWOX_SMBCMDTSC2_FMT_FS_VOLUME :
    netwib_eg(netwox_smbcmdtsc2r_selecttype(psmbcmdtsc2r, NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_VOLUME));
    netwib_eg(netwox_smbcmdtsc2r_setdefault(psmbcmdtsc2r));
    /* fake creationtime, but should be ok */
    netwib_eg(netwox_time1601_init_time(NETWIB_TIME_ZERO, &psmbcmdtsc2r->value.queryfsinfo_volume.creationtime));
    psmbcmdtsc2r->value.queryfsinfo_volume.serialnumber = NETWOX_SMBSER_FAKE_FS_ID;
    netwib_eg(netwib_buf_init_ext_string(NETWOX_SMBSER_FAKE_FS_LABEL, &buf));
    netwib_eg(netwox_smbcmd_append_buf(psmbmsgq, &buf, &psmbcmdtsc2r->value.queryfsinfo_volume.label));
    break;
  case NETWOX_SMBCMDTSC2_FMT_FS_SIZE :
    netwib_eg(netwox_smbcmdtsc2r_selecttype(psmbcmdtsc2r, NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_SIZE));
    netwib_eg(netwox_smbcmdtsc2r_setdefault(psmbcmdtsc2r));
    netwib__uint64_init_uint32(NETWOX_SMBSER_FAKE_FS_TOTALUNITS, psmbcmdtsc2r->value.queryfsinfo_size.totalunits);
    netwib__uint64_init_uint32(NETWOX_SMBSER_FAKE_FS_FREEUNITS, psmbcmdtsc2r->value.queryfsinfo_size.freeunits);
    psmbcmdtsc2r->value.queryfsinfo_size.sectorsperunit = NETWOX_SMBSER_FAKE_FS_SECTORSPERUNIT;
    psmbcmdtsc2r->value.queryfsinfo_size.bytespersector = NETWOX_SMBSER_FAKE_FS_BYTESPERSECTOR;
    break;
  case NETWOX_SMBCMDTSC2_FMT_FS_DEVICE :
    netwib_er(netwox_smbser_nottested(psmbser));
    netwib_eg(netwox_smbcmdtsc2r_selecttype(psmbcmdtsc2r, NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_DEVICE));
    netwib_eg(netwox_smbcmdtsc2r_setdefault(psmbcmdtsc2r));
    psmbcmdtsc2r->value.queryfsinfo_device.devicetype = NETWOX_SMBCMDTSC2_FMT_FS_DEVICETYPE_DISK;
    if (!psmbser->pcommon->allowput) {
      psmbcmdtsc2r->value.queryfsinfo_device.devicecharacteristics |= NETWOX_SMBCMDTSC2_FMT_FS_DEVICECHARACTERISTICS_READONLY;
    }
    break;
  case NETWOX_SMBCMDTSC2_FMT_FS_ATTRIBUTE :
    netwib_eg(netwox_smbcmdtsc2r_selecttype(psmbcmdtsc2r, NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_ATTRIBUTE));
    netwib_eg(netwox_smbcmdtsc2r_setdefault(psmbcmdtsc2r));
    psmbcmdtsc2r->value.queryfsinfo_attribute.fsattributes = NETWOX_SMBCMDTSC2_FS_ATTRIBUTE_CASESENSSEARCH | NETWOX_SMBCMDTSC2_FS_ATTRIBUTE_CASEPRESERVING;
    psmbcmdtsc2r->value.queryfsinfo_attribute.maxnamesize = 255;
    netwib_eg(netwib_buf_init_ext_string(NETWOX_SMBSER_FAKE_FS_TYPE, &buf));
    netwib_eg(netwox_smbcmd_append_buf(psmbmsgq, &buf, &psmbcmdtsc2r->value.queryfsinfo_attribute.fsname));
    break;
  case NETWOX_SMBCMDTSC2_FMT_FS_EFULLSIZE :
    netwib_er(netwox_smbser_nottested(psmbser));
    netwib_eg(netwox_smbcmdtsc2r_selecttype(psmbcmdtsc2r, NETWOX_SMBCMDTSC2R_TYPE_QUERYFSINFO_SIZE));
    netwib_eg(netwox_smbcmdtsc2r_setdefault(psmbcmdtsc2r));
    netwib__uint64_init_uint32(NETWOX_SMBSER_FAKE_FS_TOTALUNITS, psmbcmdtsc2r->value.queryfsinfo_efullsize.totalunits);
    netwib__uint64_init_uint32(NETWOX_SMBSER_FAKE_FS_FREEUNITS, psmbcmdtsc2r->value.queryfsinfo_efullsize.callerfreeunits);
    netwib__uint64_init_uint32(NETWOX_SMBSER_FAKE_FS_FREEUNITS, psmbcmdtsc2r->value.queryfsinfo_efullsize.actualfreeunits);
    psmbcmdtsc2r->value.queryfsinfo_efullsize.sectorsperunit = NETWOX_SMBSER_FAKE_FS_SECTORSPERUNIT;
    psmbcmdtsc2r->value.queryfsinfo_efullsize.bytespersector = NETWOX_SMBSER_FAKE_FS_BYTESPERSECTOR;
    break;
  case NETWOX_SMBCMDTSC2_FMT_FS_EOBJECTID :
    /* I don't know what to put in this case */
    netwib_er(netwox_smbser_notimplemented(psmbser));
    psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
    netwib_goto(NETWIB_ERR_OK);
    break;
  default :
    netwib_er(netwox_smbser_notimplemented(psmbser));
    psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
    netwib_goto(NETWIB_ERR_OK);
    break;
  }

  /* end */
 netwib_gotolabel:
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcmdtsc2_fmt_file_basic_set(netwox_smbser *psmbser,
                                                       netwib_constbuf *prealfilename,
                                                       netwox_smbcmdtsc2_fmt_file_basic *pbasic)
{
  netwib_pathstat pathstat;

  netwib_er(netwib_pathstat_init(prealfilename, &pathstat));

  netwib_er(netwox_time1601_init_time(&pathstat.mtime, &pbasic->changetime));
  /* fake values for creation, lastaccess and lastwrite but should be ok */
  netwib_er(netwox_time1601_init_time(&pathstat.mtime, &pbasic->creationtime));
  netwib_er(netwox_time1601_init_time(&pathstat.mtime, &pbasic->lastaccesstime));
  netwib_er(netwox_time1601_init_time(&pathstat.mtime, &pbasic->lastwritetime));
  if (!psmbser->pcommon->allowput) {
    pbasic->fileattributes |= NETWOX_SMBCMDCMN_FILEATTR16_READONLY;
  }
  if (pathstat.type == NETWIB_PATHSTAT_TYPE_DIR) {
    pbasic->fileattributes |= NETWOX_SMBCMDCMN_FILEATTR16_DIR;
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcmdtsc2_fmt_file_standard_set(netwib_constbuf *prealfilename,
                                                          netwox_smbcmdtsc2_fmt_file_standard *pstandard)
{
  netwib_pathstat pathstat;

  netwib_er(netwib_pathstat_init(prealfilename, &pathstat));

  if (pathstat.type == NETWIB_PATHSTAT_TYPE_DIR) {
    pstandard->isdirectory = NETWIB_TRUE;
    netwib__uint64_init_uint32(0, pstandard->allocationsize);
    netwib__uint64_init_uint32(0, pstandard->filesize);
  } else {
    pstandard->isdirectory = NETWIB_FALSE;
    pstandard->allocationsize = pathstat.size64;
    pstandard->filesize = pathstat.size64;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcmdtsc2_fmt_file_all_set(netwox_smbser *psmbser,
                                                     netwib_constbuf *prealfilename,
                                                     netwox_smbcmdtsc2_fmt_file_all *pall)
{
  netwib_pathstat pathstat;

  netwib_er(netwib_pathstat_init(prealfilename, &pathstat));

  netwib_er(netwox_time1601_init_time(&pathstat.mtime, &pall->changetime));
  /* fake values for creation, lastaccess and lastwrite but should be ok */
  netwib_er(netwox_time1601_init_time(&pathstat.mtime, &pall->creationtime));
  netwib_er(netwox_time1601_init_time(&pathstat.mtime, &pall->lastaccesstime));
  netwib_er(netwox_time1601_init_time(&pathstat.mtime, &pall->lastwritetime));
  if (!psmbser->pcommon->allowput) {
    pall->fileattributes |= NETWOX_SMBCMDCMN_FILEATTR32_READONLY;
  }
  if (pathstat.type == NETWIB_PATHSTAT_TYPE_DIR) {
    pall->fileattributes |= NETWOX_SMBCMDCMN_FILEATTR32_DIR;
    pall->isdirectory = NETWIB_TRUE;
    netwib__uint64_init_uint32(0, pall->allocationsize);
    netwib__uint64_init_uint32(0, pall->filesize);
  } else {
    pall->isdirectory = NETWIB_FALSE;
    pall->allocationsize = pathstat.size64;
    pall->filesize = pathstat.size64;
  }
  /* filename set after */

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcmdtsc2_fmt_file_enetworkopen_set(netwox_smbser *psmbser,
                                                     netwib_constbuf *prealfilename,
                                                     netwox_smbcmdtsc2_fmt_file_enetworkopen *penetworkopen)
{
  netwib_pathstat pathstat;

  netwib_er(netwib_pathstat_init(prealfilename, &pathstat));

  netwib_er(netwox_time1601_init_time(&pathstat.mtime, &penetworkopen->changetime));
  /* fake values for creation, lastaccess and lastwrite but should be ok */
  netwib_er(netwox_time1601_init_time(&pathstat.mtime, &penetworkopen->creationtime));
  netwib_er(netwox_time1601_init_time(&pathstat.mtime, &penetworkopen->lastaccesstime));
  netwib_er(netwox_time1601_init_time(&pathstat.mtime, &penetworkopen->lastwritetime));
  if (!psmbser->pcommon->allowput) {
    penetworkopen->fileattributes |= NETWOX_SMBCMDCMN_FILEATTR32_READONLY;
  }
  if (pathstat.type == NETWIB_PATHSTAT_TYPE_DIR) {
    penetworkopen->fileattributes |= NETWOX_SMBCMDCMN_FILEATTR32_DIR;
    netwib__uint64_init_uint32(0, penetworkopen->allocationsize);
    netwib__uint64_init_uint32(0, penetworkopen->filesize);
  } else {
    penetworkopen->allocationsize = pathstat.size64;
    penetworkopen->filesize = pathstat.size64;
  }
  /* filename set after */

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbcmdtsc2_fmt_file_einternal_set(netwox_smbcmdtsc2_fmt_file_einternal *peinternal)
{
  /* fake values, but should be ok */
  peinternal->dev = 0;
  peinternal->ino = 0;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbsercmd_transaction2_querypathfileinfo(netwox_smbser *psmbser,
                                                                   netwox_constsmbmsg *psmbmsgq,
                                                                   netwox_constsmbcmdtsc2q *psmbcmdtsc2q,
                                                                   netwib_constbuf *prootdir,
                                                                   netwox_smbcmdtsc2r *psmbcmdtsc2r)
{
  netwib_buf *pfilename, *prealfilename, *p83name, *pbuftmp;
  netwox_smbserfid_item *pitem;
  netwox_smbcmdtsc2_fmt_fileq fmt=0;
  netwox_smbcmdtsc2r_type type=0;
  netwib_pathstat pathstat;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbsercmd_transaction2_querypathfileinfo\n"));
  }

  /* init */
  ret = NETWIB_ERR_OK;
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pfilename));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &prealfilename));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &p83name));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pbuftmp));

  /* obtain filename */
  if (psmbcmdtsc2q->type == NETWOX_SMBCMDTSC2Q_TYPE_QUERYPATHINFO) {
    netwib_eg(netwox_smbcmd_decode_buf_path(psmbmsgq, &psmbcmdtsc2q->value.querypathinfo.filename, pfilename));
    if (psmbser->pcommon->verbose) {
      netwib_eg(netwib_fmt_display("Filename: %{buf}\n", pfilename));
    }
    netwib_eg(netwox_smbserjail_E(psmbser, prootdir, NETWIB_PATHSTAT_TYPE_UNKNOWN, pfilename, prealfilename));
    switch(psmbcmdtsc2q->value.querypathinfo.fmt) {
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_BASIC :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_BASIC;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_EBASIC :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EBASIC;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_STANDARD :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_STANDARD;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_ESTANDARD :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ESTANDARD;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_EA :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EA;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_EEA :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EEA;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_NAME :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_NAME;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_ENAME :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ENAME;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_ALL :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ALL;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_EALL :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EALL;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_ALTNAME :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ALTNAME;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_STREAM :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_STREAM;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_ESTREAM :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ESTREAM;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_EINTERNAL :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EINTERNAL;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_ENETWORKOPEN :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_ENETWORKOPEN;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_EATTRIBUTETAG :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYPATHINFO_EATTRIBUTETAG;
      break;
    default :
      netwib_er(netwox_smbser_notimplemented(psmbser));
      psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
      netwib_goto(NETWIB_ERR_OK);
      break;
    }
    fmt = psmbcmdtsc2q->value.querypathinfo.fmt;
  } else {
    if (psmbser->pcommon->verbose) {
      netwib_eg(netwib_fmt_display("Fileid: %{uint16}\n", psmbcmdtsc2q->value.queryfileinfo.fileid));
    }
    ret = netwox_smbserfid_search(&psmbser->smbserfid, psmbcmdtsc2q->value.queryfileinfo.fileid, &pitem);
    if (ret != NETWIB_ERR_OK) {
      psmbser->smberrtosend = NETWOX_SMBERR_BADFILEID;
      netwib_goto(NETWIB_ERR_OK);
    }
    netwib_eg(netwib_buf_append_buf(&pitem->realname, prealfilename));
    if (psmbser->pcommon->verbose) {
      netwib_eg(netwib_fmt_display("Real filename: %{buf}\n", prealfilename));
    }
    netwib_eg(netwox_smbserfid_item_svcname(prootdir, pitem, pfilename));
    if (psmbser->pcommon->verbose) {
      netwib_eg(netwib_fmt_display("Svc filename: %{buf}\n", pfilename));
    }
    switch(psmbcmdtsc2q->value.queryfileinfo.fmt) {
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_BASIC :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_BASIC;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_EBASIC :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EBASIC;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_STANDARD :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_STANDARD;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_ESTANDARD :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ESTANDARD;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_EA :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EA;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_EEA :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EEA;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_NAME :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_NAME;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_ENAME :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ENAME;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_ALL :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ALL;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_EALL :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EALL;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_ALTNAME :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ALTNAME;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_STREAM :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_STREAM;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_ESTREAM :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ESTREAM;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_EINTERNAL :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EINTERNAL;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_ENETWORKOPEN :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_ENETWORKOPEN;
      break;
    case NETWOX_SMBCMDTSC2_FMT_FILEQ_EATTRIBUTETAG :
      type = NETWOX_SMBCMDTSC2R_TYPE_QUERYFILEINFO_EATTRIBUTETAG;
      break;
    default :
      netwib_er(netwox_smbser_notimplemented(psmbser));
      psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
      netwib_goto(NETWIB_ERR_OK);
      break;
    }
    fmt = psmbcmdtsc2q->value.queryfileinfo.fmt;
  }

  /* construct answer */
  switch(fmt) {
  case NETWOX_SMBCMDTSC2_FMT_FILEQ_BASIC :
  case NETWOX_SMBCMDTSC2_FMT_FILEQ_EBASIC :
    netwib_eg(netwox_smbcmdtsc2r_selecttype(psmbcmdtsc2r, type));
    netwib_eg(netwox_smbcmdtsc2r_setdefault(psmbcmdtsc2r));
    ret = netwox_smbcmdtsc2_fmt_file_basic_set(psmbser, prealfilename, &psmbcmdtsc2r->value.querypathfileinfo_basic);
    if (ret != NETWIB_ERR_OK) {
      psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
      netwib_goto(NETWIB_ERR_OK);
    }
    break;
  case NETWOX_SMBCMDTSC2_FMT_FILEQ_STANDARD :
  case NETWOX_SMBCMDTSC2_FMT_FILEQ_ESTANDARD :
    netwib_eg(netwox_smbcmdtsc2r_selecttype(psmbcmdtsc2r, type));
    netwib_eg(netwox_smbcmdtsc2r_setdefault(psmbcmdtsc2r));
    ret = netwox_smbcmdtsc2_fmt_file_standard_set(prealfilename, &psmbcmdtsc2r->value.querypathfileinfo_standard);
    if (ret != NETWIB_ERR_OK) {
      psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
      netwib_goto(NETWIB_ERR_OK);
    }
    break;
  case NETWOX_SMBCMDTSC2_FMT_FILEQ_EA :
  case NETWOX_SMBCMDTSC2_FMT_FILEQ_EEA :
    netwib_eg(netwox_smbcmdtsc2r_selecttype(psmbcmdtsc2r, type));
    netwib_eg(netwox_smbcmdtsc2r_setdefault(psmbcmdtsc2r));
    psmbcmdtsc2r->value.querypathfileinfo_ea.easize = 0;
    break;
  case NETWOX_SMBCMDTSC2_FMT_FILEQ_NAME :
  case NETWOX_SMBCMDTSC2_FMT_FILEQ_ENAME :
    netwib_eg(netwox_smbcmdtsc2r_selecttype(psmbcmdtsc2r, type));
    netwib_eg(netwox_smbcmdtsc2r_setdefault(psmbcmdtsc2r));
    netwib_eg(netwib_path_decode_child(pfilename, pbuftmp));
    netwib_eg(netwox_smbcmd_append_buf_path(psmbmsgq, pbuftmp, &psmbcmdtsc2r->value.querypathfileinfo_name.filename));
    break;
  case NETWOX_SMBCMDTSC2_FMT_FILEQ_ALL :
  case NETWOX_SMBCMDTSC2_FMT_FILEQ_EALL :
    netwib_eg(netwox_smbcmdtsc2r_selecttype(psmbcmdtsc2r, type));
    netwib_eg(netwox_smbcmdtsc2r_setdefault(psmbcmdtsc2r));
    ret = netwox_smbcmdtsc2_fmt_file_all_set(psmbser, prealfilename, &psmbcmdtsc2r->value.querypathfileinfo_all);
    if (ret != NETWIB_ERR_OK) {
      psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
      netwib_goto(NETWIB_ERR_OK);
    }
    netwib_eg(netwox_smbcmd_append_buf_path(psmbmsgq, pfilename, &psmbcmdtsc2r->value.querypathfileinfo_all.filename));
    break;
  case NETWOX_SMBCMDTSC2_FMT_FILEQ_ALTNAME :
    netwib_eg(netwox_smbcmdtsc2r_selecttype(psmbcmdtsc2r, type));
    netwib_eg(netwox_smbcmdtsc2r_setdefault(psmbcmdtsc2r));
    ret = netwib_pathstat_init(prealfilename, &pathstat);
    if (ret != NETWIB_ERR_OK) {
      psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
      netwib_goto(NETWIB_ERR_OK);
    }
    netwib_eg(netwib_path_decode_child(pfilename, pbuftmp));
    netwib_eg(netwox_smbser83_longto83_path(&psmbser->smbser83, pathstat.type,
                                            pbuftmp, p83name));
    netwib_eg(netwox_smbcmd_append_buf_path(psmbmsgq, p83name, &psmbcmdtsc2r->value.querypathfileinfo_altname.filename));
    break;
  case NETWOX_SMBCMDTSC2_FMT_FILEQ_STREAM :
  case NETWOX_SMBCMDTSC2_FMT_FILEQ_ESTREAM :
    /* it is better to answer unsupported, otherwise Windows thinks we support
       data streams and uses "file:information:$data" filenames. */
    psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
    netwib_goto(NETWIB_ERR_OK);
#if 0
    /* to fully test this, I should have created a program using
       OpenStream function. However, it seems to work. */
    netwib_eg(netwox_smbcmdtsc2r_selecttype(psmbcmdtsc2r, type));
    netwib_eg(netwox_smbcmdtsc2r_setdefault(psmbcmdtsc2r));
    netwib_er(netwib_array_ctl_set_size(&psmbcmdtsc2r->value.querypathfileinfo_stream.infos, 1));
    {
      netwox_smbcmdtsc2_fmt_file_stream *pstream;
      netwib_buf buf;
      pstream = (netwox_smbcmdtsc2_fmt_file_stream *)psmbcmdtsc2r->value.querypathfileinfo_stream.infos.p[0];
      netwib_er(netwox_smbcmdtsc2_fmt_file_stream_init(pstream));
      netwib_er(netwox_smbcmdtsc2_fmt_file_stream_setdefault(pstream));
      netwib_eg(netwib_buf_init_ext_string("::$DATA", &buf));
      netwib_eg(netwox_smbcmd_append_buf_path(psmbmsgq, &buf, &pstream->name));
    }
#endif
    break;
  case NETWOX_SMBCMDTSC2_FMT_FILEQ_EINTERNAL :
    netwib_er(netwox_smbser_nottested(psmbser));
    netwib_eg(netwox_smbcmdtsc2r_selecttype(psmbcmdtsc2r, type));
    netwib_eg(netwox_smbcmdtsc2r_setdefault(psmbcmdtsc2r));
    ret = netwox_smbcmdtsc2_fmt_file_einternal_set(&psmbcmdtsc2r->value.querypathfileinfo_einternal);
    if (ret != NETWIB_ERR_OK) {
      psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
      netwib_goto(NETWIB_ERR_OK);
    }
    break;
  case NETWOX_SMBCMDTSC2_FMT_FILEQ_ENETWORKOPEN :
    netwib_er(netwox_smbser_nottested(psmbser));
    netwib_eg(netwox_smbcmdtsc2r_selecttype(psmbcmdtsc2r, type));
    netwib_eg(netwox_smbcmdtsc2r_setdefault(psmbcmdtsc2r));
    ret = netwox_smbcmdtsc2_fmt_file_enetworkopen_set(psmbser, prealfilename, &psmbcmdtsc2r->value.querypathfileinfo_enetworkopen);
    if (ret != NETWIB_ERR_OK) {
      psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
      netwib_goto(NETWIB_ERR_OK);
    }
    break;
  case NETWOX_SMBCMDTSC2_FMT_FILEQ_EATTRIBUTETAG :
    psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
    netwib_goto(NETWIB_ERR_OK);
    break;
  default :
    netwib_er(netwox_smbser_notimplemented(psmbser));
    psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
    netwib_goto(NETWIB_ERR_OK);
    break;
  }

  /* end */
 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pbuftmp));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &p83name));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &prealfilename));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pfilename));
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbsercmd_transaction2_setfileinfo(netwox_smbser *psmbser,
                                                             netwox_constsmbmsg *psmbmsgq,
                                                             netwox_constsmbcmdtsc2q *psmbcmdtsc2q,
                                                             netwox_smbcmdtsc2r *psmbcmdtsc2r)
{
  netwib_buf *prealfilename;
  netwox_smbserfid_item *pitem;
  netwib_uint64 offset;
  netwib_uint16 fileid;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbsercmd_transaction2_setfileinfo\n"));
  }

  /* init */
  ret = NETWIB_ERR_OK;
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &prealfilename));

  /* obtain filename */
  fileid = 0;
  switch(psmbcmdtsc2q->type) {
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_BASIC:
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EBASIC:
    fileid = psmbcmdtsc2q->value.setfileinfo_basic.fileid;
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_DISPOSITION:
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EDISPOSITION:
    fileid = psmbcmdtsc2q->value.setfileinfo_disposition.fileid;
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_ALLOCATION:
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EALLOCATION:
    fileid = psmbcmdtsc2q->value.setfileinfo_allocation.fileid;
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EOF:
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EEOF:
    fileid = psmbcmdtsc2q->value.setfileinfo_eof.fileid;
    break;
  default :
    netwib_er(netwox_smbser_notimplemented(psmbser));
    psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
    netwib_goto(NETWIB_ERR_OK);
    break;
  }
  /* obtain real filename from fileid */
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Fileid: %{uint16}\n", fileid));
  }
  ret = netwox_smbserfid_search(&psmbser->smbserfid, fileid, &pitem);
  if (ret != NETWIB_ERR_OK) {
    psmbser->smberrtosend = NETWOX_SMBERR_BADFILEID;
    netwib_goto(NETWIB_ERR_OK);
  }
  netwib_eg(netwib_buf_append_buf(&pitem->realname, prealfilename));
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Real filename: %{buf}\n", prealfilename));
  }

  /* construct answer */
  netwib_eg(netwox_smbcmdtsc2r_selecttype(psmbcmdtsc2r, NETWOX_SMBCMDTSC2R_TYPE_SETFILEINFO));
  netwib_eg(netwox_smbcmdtsc2r_setdefault(psmbcmdtsc2r));
  switch(psmbcmdtsc2q->type) {
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_BASIC:
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EBASIC:
    /* do nothing (don't change time or attributes) and return ok */
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_DISPOSITION:
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EDISPOSITION:
    pitem->deleteonclose = psmbcmdtsc2q->value.setfileinfo_disposition.disposition.deleteonclose;
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_ALLOCATION:
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EALLOCATION:
    if (pitem->fidtype != NETWOX_SMBSERFID_FIDTYPE_REG) {
      psmbser->smberrtosend = NETWOX_SMBERR_BADFILEID;
      netwib_goto(NETWIB_ERR_OK);
    }
    offset = psmbcmdtsc2q->value.setfileinfo_allocation.allocation.allocationsize;
    ret = netwib_file_ctl_set_truncate64(pitem->value.itemreg.pio, &offset);
    if (ret != NETWIB_ERR_OK) {
      psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
      netwib_goto(NETWIB_ERR_OK);
    }
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EOF:
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EEOF:
    if (pitem->fidtype != NETWOX_SMBSERFID_FIDTYPE_REG) {
      psmbser->smberrtosend = NETWOX_SMBERR_BADFILEID;
      netwib_goto(NETWIB_ERR_OK);
    }
    offset = psmbcmdtsc2q->value.setfileinfo_eof.eof.filesize;
    ret = netwib_file_ctl_set_truncate64(pitem->value.itemreg.pio, &offset);
    if (ret != NETWIB_ERR_OK) {
      psmbser->smberrtosend = NETWOX_SMBERR_FILENOTFOUND;
      netwib_goto(NETWIB_ERR_OK);
    }
    break;
  default :
    netwib_er(netwox_smbser_notimplemented(psmbser));
    psmbser->smberrtosend = NETWOX_SMBERR_NOTIMPLEMENTED;
    netwib_goto(NETWIB_ERR_OK);
    break;
  }

  /* end */
 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &prealfilename));
  psmbmsgq = psmbmsgq; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_transaction2(netwox_smbser *psmbser,
                                         netwox_constsmbmsg *psmbmsgq,
                                         netwox_constsmbcmd *psmbcmdq,
                                         netwib_constbuf *prootdir,
                                         netwib_bool *psendreply)
{
  netwox_smbser_session *psession;
  netwox_smbser_tree *ptree;
  netwib_buf *pbuf;
  netwox_smbmsg smbmsgr;
  netwox_smbcmd smbcmdr;
  netwox_smbcmdtsc2q smbcmdtsc2q;
  netwox_smbcmdtsc2rstorage smbcmdtsc2rstorage;
  netwox_smbcmdtsc2rfmt smbcmdtsc2rfmt;
  netwox_smbcmdtsc2r smbcmdtsc2r;
  netwib_uint32 offset, maxsize;
  netwib_bool needmoremsg;
  netwib_err ret;

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Function netwox_smbsercmd_transaction2\n"));
  }
  netwib_er(netwox_smbser_sessiontree_check(psmbser, &psession, &ptree));

  /* init */
  ret = NETWIB_ERR_OK;
  netwib_eg(netwox_smbcmdtsc2q_init(&smbcmdtsc2q));
  netwib_eg(netwox_smbcmdtsc2rstorage_init(psession->capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_UNICODE, &smbcmdtsc2rstorage));
  netwib_eg(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &pbuf));
  netwib_eg(netwox_smbcmdtsc2r_init(&smbcmdtsc2r));

  /* check query */
  if (psmbcmdq->type != NETWOX_SMBCMD_TYPE_TRANSACTION2_Q) {
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  /* ignore psmbcmdq->value.transaction2_q.maxparasize, maxdatasize and
     maxsetupcount because our reply will not contain a lot of data, so it
     should be ok */
  /* ignore psmbcmdq->value.transaction2_q.flags because its meaning is
     unknown */
  /* ignore psmbcmdq->value.transaction2_q.timeout because we do not have
     delay in our answer */

  /* decode transaction2 and prepare answer */
  netwib_eg(netwox_smbcmdtsc2q_encode_smbcmd(psmbcmdq, &smbcmdtsc2q));
  if (psmbser->pcommon->debug) {
    netwib_eg(netwib_fmt_display("Received transaction2 query:\n"));
    netwib_eg(netwox_smbcmdtsc2q_display(psmbser->pcommon->pbufpool,
                                         &smbcmdtsc2q));
  }
  netwib_eg(netwox_smbcmdtsc2rfmt_init_smbcmdtsc2q(&smbcmdtsc2q, &smbcmdtsc2rfmt));
  switch(smbcmdtsc2q.type) {
  case NETWOX_SMBCMDTSC2Q_TYPE_FINDFIRST2:
  case NETWOX_SMBCMDTSC2Q_TYPE_FINDNEXT2:
    netwib_eg(netwox_smbsercmd_transaction2_find(psmbser, psmbmsgq, psmbcmdq, &smbcmdtsc2q, &smbcmdtsc2rfmt, prootdir, &smbcmdtsc2r));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_QUERYFSINFO:
    netwib_eg(netwox_smbsercmd_transaction2_queryfsinfo(psmbser, psmbmsgq, &smbcmdtsc2q, &smbcmdtsc2r));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_QUERYPATHINFO:
  case NETWOX_SMBCMDTSC2Q_TYPE_QUERYFILEINFO:
    netwib_eg(netwox_smbsercmd_transaction2_querypathfileinfo(psmbser, psmbmsgq, &smbcmdtsc2q, prootdir, &smbcmdtsc2r));
    break;
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_BASIC:
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EBASIC:
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_DISPOSITION:
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EDISPOSITION:
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_ALLOCATION:
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EALLOCATION:
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EOF:
  case NETWOX_SMBCMDTSC2Q_TYPE_SETFILEINFO_EEOF:
    netwib_eg(netwox_smbsercmd_transaction2_setfileinfo(psmbser, psmbmsgq, &smbcmdtsc2q, &smbcmdtsc2r));
    break;
  default :
    netwib_eg(NETWOX_ERR_PROTOCOL);
  }
  if (psmbser->smberrtosend != NETWOX_SMBERR_OK) {
    netwib_goto(NETWIB_ERR_OK);
  }
  if (psmbser->pcommon->debug) {
    netwib_eg(netwib_fmt_display("Sent transaction2 reply:\n"));
    netwib_eg(netwox_smbcmdtsc2r_display(psmbser->pcommon->pbufpool,
                                         &smbcmdtsc2r));
  }
  netwib_eg(netwox_smbcmdtsc2r_decode_storager(&smbcmdtsc2r, &smbcmdtsc2rfmt,
                                               &smbcmdtsc2rstorage));

  /* normally, none of our function should have created a transaction
     too big. But check in case. */
  if (netwib__buf_ref_data_size(&smbcmdtsc2rstorage.para) > psmbcmdq->value.transaction2_q.maxparasize) {
    netwib_er(netwox_smbser_internalerror(psmbser));
    netwib_eg(NETWOX_ERR_INTERNALERROR);
  }
  if (netwib__buf_ref_data_size(&smbcmdtsc2rstorage.data) > psmbcmdq->value.transaction2_q.maxdatasize) {
    netwib_er(netwox_smbser_internalerror(psmbser));
    netwib_eg(NETWOX_ERR_INTERNALERROR);
  }

  /* send replies */
  offset = 0;
  /* difficult to compute, so use a value which should be ok */
  maxsize = psmbser->cmn_maxmsgsize - NETWOX_SMBSER_SUFFICIENT_FOR_HEADERS;
  while(NETWIB_TRUE) {
    netwib_eg(netwox_smbser_send_begin(psmbser, psmbmsgq, &smbmsgr, &smbcmdr,
                                       NETWOX_SMBCMD_TYPE_TRANSACTION2_R));
    netwib_eg(netwox_smbcmdtsc2rstorage_decode_smbcmd(&smbcmdtsc2rstorage,
                                                      &offset, maxsize,
                                                      &smbcmdr, &needmoremsg));
    netwib_eg(netwox_smbser_send_end(psmbser, &smbmsgr, &smbcmdr));
    if (!needmoremsg) {
      break;
    }
  }
  *psendreply = NETWIB_FALSE;

  /* end */
 netwib_gotolabel:
  netwib_er(netwox_smbcmdtsc2r_close(&smbcmdtsc2r));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &pbuf));
  netwib_er(netwox_smbcmdtsc2rstorage_close(&smbcmdtsc2rstorage));
  netwib_er(netwox_smbcmdtsc2q_close(&smbcmdtsc2q));
  return(ret);
}
