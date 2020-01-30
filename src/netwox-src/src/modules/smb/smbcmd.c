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
netwib_err netwox_smbcmd_init(netwox_smbcmd *psmbcmd)
{
  psmbcmd->type = NETWOX_SMBCMD_TYPE_UNKNOWN;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmd_selecttype(netwox_smbcmd *psmbcmd,
                                    netwox_smbcmd_type type)
{
  if (psmbcmd->type != NETWOX_SMBCMD_TYPE_UNKNOWN) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  psmbcmd->type = type;
  switch(type) {
  case NETWOX_SMBCMD_TYPE_CREATEDIRECTORY_Q :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.createdirectory_q.dirname));
    break;
  case NETWOX_SMBCMD_TYPE_CREATEDIRECTORY_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_DELETEDIRECTORY_Q :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.deletedirectory_q.dirname));
    break;
  case NETWOX_SMBCMD_TYPE_DELETEDIRECTORY_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_CREATE_Q :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.create_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_CREATE_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_OPEN_Q :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.open_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_OPEN_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_CLOSE_Q :
    netwox__smbcmdcmn_time1970l_initdefault(psmbcmd->value.close_q.lastwritetime);
    break;
  case NETWOX_SMBCMD_TYPE_CLOSE_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_FLUSH_Q :
  case NETWOX_SMBCMD_TYPE_FLUSH_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_DELETE_Q :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.delete_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_DELETE_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_RENAME_Q :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.rename_q.oldname));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.rename_q.newname));
    break;
  case NETWOX_SMBCMD_TYPE_RENAME_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATION_Q :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.queryinformation_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATION_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_SETINFORMATION_Q :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.setinformation_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_SETINFORMATION_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_READ_Q :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_READ_R :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.read_r.data));
    break;
  case NETWOX_SMBCMD_TYPE_WRITE_Q :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.write_q.data));
    break;
  case NETWOX_SMBCMD_TYPE_WRITE_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_CREATETEMPORARY_Q :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.createtemporary_q.dirname));
    break;
  case NETWOX_SMBCMD_TYPE_CREATETEMPORARY_R :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.createtemporary_r.filename));
    break;
  case NETWOX_SMBCMD_TYPE_CREATENEW_Q :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.createnew_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_CREATENEW_R :
    break;
  case NETWOX_SMBCMD_TYPE_CHECKDIRPATH_Q :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.checkdirpath_q.dirname));
    break;
  case NETWOX_SMBCMD_TYPE_CHECKDIRPATH_R :
  case NETWOX_SMBCMD_TYPE_SEEK_Q :
  case NETWOX_SMBCMD_TYPE_SEEK_R :
  case NETWOX_SMBCMD_TYPE_READRAW_Q :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_WRITERAW_Q :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.writeraw_q.data));
    break;
  case NETWOX_SMBCMD_TYPE_WRITERAW_R :
  case NETWOX_SMBCMD_TYPE_SETINFORMATION2_Q :
  case NETWOX_SMBCMD_TYPE_SETINFORMATION2_R :
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATION2_Q :
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATION2_R :
  case NETWOX_SMBCMD_TYPE_LOCKINGANDX_Q :
  case NETWOX_SMBCMD_TYPE_LOCKINGANDX_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_TRANSACTION_Q :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.transaction_q.name));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.transaction_q.para));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.transaction_q.data));
    break;
  case NETWOX_SMBCMD_TYPE_TRANSACTION_R :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.transaction_r.para));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.transaction_r.data));
    break;
  case NETWOX_SMBCMD_TYPE_ECHO_Q :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.echo_q.data));
    break;
  case NETWOX_SMBCMD_TYPE_ECHO_R :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.echo_r.data));
    break;
  case NETWOX_SMBCMD_TYPE_OPENANDX_Q :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.openandx_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_OPENANDX_R :
  case NETWOX_SMBCMD_TYPE_READANDX_Q :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_READANDX_R :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.readandx_r.data));
    break;
  case NETWOX_SMBCMD_TYPE_WRITEANDX_Q :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.writeandx_q.data));
    break;
  case NETWOX_SMBCMD_TYPE_WRITEANDX_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_CLOSETREEDISCONNECT_Q :
    netwox__smbcmdcmn_time1970l_initdefault(psmbcmd->value.closetreedisconnect_q.lastwritetime);
    break;
  case NETWOX_SMBCMD_TYPE_CLOSETREEDISCONNECT_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_TRANSACTION2_Q :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.transaction2_q.para));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.transaction2_q.data));
    break;
  case NETWOX_SMBCMD_TYPE_TRANSACTION2_R :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.transaction2_r.para));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.transaction2_r.data));
    break;
  case NETWOX_SMBCMD_TYPE_FINDCLOSE2_Q :
  case NETWOX_SMBCMD_TYPE_FINDCLOSE2_R :
  case NETWOX_SMBCMD_TYPE_TREEDISCONNECT_Q :
  case NETWOX_SMBCMD_TYPE_TREEDISCONNECT_R :
  case NETWOX_SMBCMD_TYPE_NEGOTIATE_Q :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_NEGOTIATE_R0 :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.negotiate_r0.challenge));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.negotiate_r0.domain));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.negotiate_r0.i1.server));
    break;
  case NETWOX_SMBCMD_TYPE_NEGOTIATE_R1 :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.negotiate_r1.serverguid));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.negotiate_r1.securityblob));
    break;
  case NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_Q0 :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.sessionsetupandx_q0.password));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.sessionsetupandx_q0.unicodepassword));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.sessionsetupandx_q0.user));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.sessionsetupandx_q0.domain));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.sessionsetupandx_q0.nativeos));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.sessionsetupandx_q0.nativelanman));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.sessionsetupandx_q0.i1.unknown));
    break;
  case NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_Q1 :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.sessionsetupandx_q1.secublob));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.sessionsetupandx_q1.nativeos));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.sessionsetupandx_q1.nativelanman));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.sessionsetupandx_q1.domain));
    break;
  case NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_R0 :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.sessionsetupandx_r0.i1.nativeos));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.sessionsetupandx_r0.i1.nativelanman));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.sessionsetupandx_r0.i1.i2.domain));
    break;
  case NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_R1 :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.sessionsetupandx_r1.secublob));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.sessionsetupandx_r1.nativeos));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.sessionsetupandx_r1.nativelanman));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.sessionsetupandx_r1.i2.domain));
    break;
  case NETWOX_SMBCMD_TYPE_LOGOFFANDX_Q :
  case NETWOX_SMBCMD_TYPE_LOGOFFANDX_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_TREECONNECTANDX_Q :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.treeconnectandx_q.password));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.treeconnectandx_q.path));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.treeconnectandx_q.service));
    break;
  case NETWOX_SMBCMD_TYPE_TREECONNECTANDX_R :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.treeconnectandx_r.service));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.treeconnectandx_r.i1.nativefs));
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATIONDISK_Q :
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATIONDISK_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_SEARCH_Q :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.search_q.searchpattern));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.search_q.resumekey));
    break;
  case NETWOX_SMBCMD_TYPE_SEARCH_R :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.search_r.entries));
    break;
  case NETWOX_SMBCMD_TYPE_NTTRANSACT_Q :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.nttransact_q.para));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.nttransact_q.data));
    break;
  case NETWOX_SMBCMD_TYPE_NTTRANSACT_R :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.nttransact_r.para));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.nttransact_r.data));
    break;
  case NETWOX_SMBCMD_TYPE_NTCREATEANDX_Q :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.ntcreateandx_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_NTCREATEANDX_R :
  case NETWOX_SMBCMD_TYPE_NTCANCEL_Q :
  case NETWOX_SMBCMD_TYPE_NTCANCEL_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_NTRENAME_Q :
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.ntrename_q.oldname));
    netwib_er(netwox_smbcmdcmn_data_initdefault(&psmbcmd->value.ntrename_q.newname));
    break;
  case NETWOX_SMBCMD_TYPE_NTRENAME_R :
    /* nothing to initialize */
    break;
  default :
    psmbcmd->type = NETWOX_SMBCMD_TYPE_UNKNOWN;
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmd_setdefault(netwox_smbcmd *psmbcmd)
{
  netwib_uint32 i;

  switch(psmbcmd->type) {
  case NETWOX_SMBCMD_TYPE_CREATEDIRECTORY_Q :
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.createdirectory_q.dirname));
    break;
  case NETWOX_SMBCMD_TYPE_CREATEDIRECTORY_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_DELETEDIRECTORY_Q :
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.deletedirectory_q.dirname));
    break;
  case NETWOX_SMBCMD_TYPE_DELETEDIRECTORY_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_CREATE_Q :
    netwox__smbcmdcmn_fileattr16_initdefault(psmbcmd->value.create_q.fileattr);
    netwox__smbcmdcmn_time1970l_initdefault(psmbcmd->value.create_q.creationtime);
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.create_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_CREATE_R :
    psmbcmd->value.create_r.fileid = 0;
    break;
  case NETWOX_SMBCMD_TYPE_OPEN_Q :
    netwox__smbcmdcmn_accessmode_initdefault(psmbcmd->value.open_q.desiredaccess);
    netwox__smbcmdcmn_fileattr16_initdefault(psmbcmd->value.open_q.searchattributes);
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.open_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_OPEN_R :
    psmbcmd->value.open_r.fileid = 0;
    netwox__smbcmdcmn_fileattr16_initdefault(psmbcmd->value.open_r.fileattributes);
    netwox__smbcmdcmn_fileattr16_initdefault(psmbcmd->value.open_r.fileattributes);
    netwox__smbcmdcmn_time1970l_initdefault(psmbcmd->value.open_r.lastwritetime);
    psmbcmd->value.open_r.filesize = 0;
    netwox__smbcmdcmn_accessmode_initdefault(psmbcmd->value.open_r.grantedaccess);
    break;
  case NETWOX_SMBCMD_TYPE_CLOSE_Q :
    psmbcmd->value.close_q.fileid = 0;
    netwox__smbcmdcmn_time1970l_initdefault(psmbcmd->value.close_q.lastwritetime);
    break;
  case NETWOX_SMBCMD_TYPE_CLOSE_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_FLUSH_Q :
    psmbcmd->value.flush_q.fileid = 0;
    break;
  case NETWOX_SMBCMD_TYPE_FLUSH_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_DELETE_Q :
    netwox__smbcmdcmn_fileattr16_initdefault(psmbcmd->value.delete_q.searchattributes);
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.delete_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_DELETE_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_RENAME_Q :
    netwox__smbcmdcmn_fileattr16_initdefault(psmbcmd->value.rename_q.searchattributes);
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.rename_q.oldname));
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.rename_q.newname));
    break;
  case NETWOX_SMBCMD_TYPE_RENAME_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATION_Q :
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.queryinformation_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATION_R :
    netwox__smbcmdcmn_fileattr16_initdefault(psmbcmd->value.queryinformation_r.fileattributes);
    netwox__smbcmdcmn_time1970l_initdefault(psmbcmd->value.queryinformation_r.lastwritetime);
    psmbcmd->value.queryinformation_r.filesize = 0;
    break;
  case NETWOX_SMBCMD_TYPE_SETINFORMATION_Q :
    netwox__smbcmdcmn_fileattr16_initdefault(psmbcmd->value.setinformation_q.fileattributes);
    netwox__smbcmdcmn_time1970l_initdefault(psmbcmd->value.setinformation_q.lastwritetime);
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.setinformation_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_SETINFORMATION_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_READ_Q :
    psmbcmd->value.read_q.fileid = 0;
    psmbcmd->value.read_q.offset = 0;
    psmbcmd->value.read_q.size = 0;
    psmbcmd->value.read_q.remaining = NETWOX_SMBCMD_READ_Q_REMAINING_SAFE;
    break;
  case NETWOX_SMBCMD_TYPE_READ_R :
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.read_r.data));
    break;
  case NETWOX_SMBCMD_TYPE_WRITE_Q :
    psmbcmd->value.write_q.fileid = 0;
    psmbcmd->value.write_q.offset = 0;
    psmbcmd->value.write_q.remaining = NETWOX_SMBCMD_WRITE_Q_REMAINING_SAFE;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.write_q.data));
    break;
  case NETWOX_SMBCMD_TYPE_WRITE_R :
    psmbcmd->value.write_r.writtensize = 0;
    break;
  case NETWOX_SMBCMD_TYPE_CREATETEMPORARY_Q :
    netwox__smbcmdcmn_fileattr16_initdefault(psmbcmd->value.createtemporary_q.fileattr);
    netwox__smbcmdcmn_time1970l_initdefault(psmbcmd->value.createtemporary_q.creationtime);
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.createtemporary_q.dirname));
    break;
  case NETWOX_SMBCMD_TYPE_CREATETEMPORARY_R :
    psmbcmd->value.createtemporary_r.fileid = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.createtemporary_r.filename));
    break;
  case NETWOX_SMBCMD_TYPE_CREATENEW_Q :
    netwox__smbcmdcmn_fileattr16_initdefault(psmbcmd->value.createnew_q.fileattributes);
    netwox__smbcmdcmn_time1970l_initdefault(psmbcmd->value.createnew_q.creationtime);
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.createnew_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_CREATENEW_R :
    psmbcmd->value.createnew_r.fileid = 0;
    break;
  case NETWOX_SMBCMD_TYPE_CHECKDIRPATH_Q :
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.checkdirpath_q.dirname));
    break;
  case NETWOX_SMBCMD_TYPE_CHECKDIRPATH_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_SEEK_Q :
    psmbcmd->value.seek_q.fileid = 0;
    netwox__smbcmdcmn_seektype_initdefault(psmbcmd->value.seek_q.seektype);
    psmbcmd->value.seek_q.offset = 0;
    break;
  case NETWOX_SMBCMD_TYPE_SEEK_R :
    psmbcmd->value.seek_r.offset = 0;
    break;
  case NETWOX_SMBCMD_TYPE_READRAW_Q :
    psmbcmd->value.readraw_q.fileid = 0;
    netwox__smbcmdcmn_uint64_initdefault(psmbcmd->value.readraw_q.offset);
    psmbcmd->value.readraw_q.maxsize = 0;
    psmbcmd->value.readraw_q.minsize = 0;
    psmbcmd->value.readraw_q.timeout = 0;
    break;
  case NETWOX_SMBCMD_TYPE_WRITERAW_Q :
    psmbcmd->value.writeraw_q.fileid = 0;
    psmbcmd->value.writeraw_q.rawdatasize = 0;
    netwox__smbcmdcmn_uint64_initdefault(psmbcmd->value.writeraw_q.offset);
    psmbcmd->value.writeraw_q.timeout = 0;
    netwox__smbcmdcmn_accessmode_initdefault(psmbcmd->value.writeraw_q.accessmode);
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.writeraw_q.data));
    break;
  case NETWOX_SMBCMD_TYPE_WRITERAW_R :
    psmbcmd->value.writeraw_r.remaining = NETWOX_SMBCMD_WRITERAW_R_REMAINING_SAFE;
    break;
  case NETWOX_SMBCMD_TYPE_SETINFORMATION2_Q :
    psmbcmd->value.setinformation2_q.fileid = 0;
    netwox__smbcmdcmn_timemsdos_initdefault(psmbcmd->value.setinformation2_q.creationtime);
    netwox__smbcmdcmn_timemsdos_initdefault(psmbcmd->value.setinformation2_q.lastaccesstime);
    netwox__smbcmdcmn_timemsdos_initdefault(psmbcmd->value.setinformation2_q.lastwritetime);
    break;
  case NETWOX_SMBCMD_TYPE_SETINFORMATION2_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATION2_Q :
    psmbcmd->value.queryinformation2_q.fileid = 0;
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATION2_R :
    netwox__smbcmdcmn_timemsdos_initdefault(psmbcmd->value.queryinformation2_r.creationtime);
    netwox__smbcmdcmn_timemsdos_initdefault(psmbcmd->value.queryinformation2_r.lastaccesstime);
    netwox__smbcmdcmn_timemsdos_initdefault(psmbcmd->value.queryinformation2_r.lastwritetime);
    psmbcmd->value.queryinformation2_r.filesize = 0;
    psmbcmd->value.queryinformation2_r.allocationsize = 0;
    netwox__smbcmdcmn_fileattr16_initdefault(psmbcmd->value.queryinformation2_r.fileattributes);
    break;
  case NETWOX_SMBCMD_TYPE_LOCKINGANDX_Q :
    psmbcmd->value.lockingandx_q.fileid = 0;
    psmbcmd->value.lockingandx_q.locktype = 0;
    netwox__smbcmdcmn_oplocklevel_initdefault(psmbcmd->value.lockingandx_q.oplocklevel);
    psmbcmd->value.lockingandx_q.timeout = 0;
    psmbcmd->value.lockingandx_q.unlockscount = 0;
    psmbcmd->value.lockingandx_q.lockscount = 0;
    for (i = 0; i < NETWOX_SMBCMD_LOCKINGANDX_RANGE_MAX; i++) {
      psmbcmd->value.lockingandx_q.locks[i].pid = 0;
      netwox__smbcmdcmn_uint64_initdefault(psmbcmd->value.lockingandx_q.locks[i].offset);
      netwox__smbcmdcmn_uint64_initdefault(psmbcmd->value.lockingandx_q.locks[i].length);
      psmbcmd->value.lockingandx_q.unlocks[i].pid = 0;
      netwox__smbcmdcmn_uint64_initdefault(psmbcmd->value.lockingandx_q.unlocks[i].offset);
      netwox__smbcmdcmn_uint64_initdefault(psmbcmd->value.lockingandx_q.unlocks[i].length);
    }
    break;
  case NETWOX_SMBCMD_TYPE_LOCKINGANDX_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_TRANSACTION_Q :
    psmbcmd->value.transaction_q.totalparasize = 0;
    psmbcmd->value.transaction_q.totaldatasize = 0;
    /* if we set too big values, server sends back an error */
    psmbcmd->value.transaction_q.maxparasize = 1024;
    psmbcmd->value.transaction_q.maxdatasize = 16384;
    psmbcmd->value.transaction_q.maxsetupcount = NETWOX_SMBCMD_TRANSACTION_SETUP_MAX;
    psmbcmd->value.transaction_q.flags = 0;
    psmbcmd->value.transaction_q.timeout = 0;
    psmbcmd->value.transaction_q.setupcount = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.transaction_q.name));
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.transaction_q.para));
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.transaction_q.data));
    psmbcmd->value.transaction_q.isunicode = NETWIB_FALSE;
    break;
  case NETWOX_SMBCMD_TYPE_TRANSACTION_R :
    psmbcmd->value.transaction_r.totalparasize = 0;
    psmbcmd->value.transaction_r.totaldatasize = 0;
    psmbcmd->value.transaction_r.paradisplacement = 0;
    psmbcmd->value.transaction_r.datadisplacement = 0;
    psmbcmd->value.transaction_r.setupcount = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.transaction_r.para));
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.transaction_r.data));
    break;
  case NETWOX_SMBCMD_TYPE_ECHO_Q :
    psmbcmd->value.echo_q.count = 1;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.echo_q.data));
    break;
  case NETWOX_SMBCMD_TYPE_ECHO_R :
    psmbcmd->value.echo_r.number = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.echo_r.data));
    break;
  case NETWOX_SMBCMD_TYPE_OPENANDX_Q :
    netwox__smbcmdcmn_openflags16_initdefault(psmbcmd->value.openandx_q.openflags);
    netwox__smbcmdcmn_accessmode_initdefault(psmbcmd->value.openandx_q.desiredaccess);
    netwox__smbcmdcmn_fileattr16_initdefault(psmbcmd->value.openandx_q.searchattributes);
    netwox__smbcmdcmn_fileattr16_initdefault(psmbcmd->value.openandx_q.fileattributes);
    netwox__smbcmdcmn_time1970l_initdefault(psmbcmd->value.openandx_q.creationtime);
    netwox__smbcmdcmn_openfunction_initdefault(psmbcmd->value.openandx_q.openfunction);
    psmbcmd->value.openandx_q.allocationsize = 0;
    psmbcmd->value.openandx_q.timeout = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.openandx_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_OPENANDX_R :
    psmbcmd->value.openandx_r.fileid = 0;
    netwox__smbcmdcmn_fileattr16_initdefault(psmbcmd->value.openandx_r.fileattributes);
    netwox__smbcmdcmn_fileattr16_initdefault(psmbcmd->value.openandx_r.fileattributes);
    netwox__smbcmdcmn_time1970l_initdefault(psmbcmd->value.openandx_r.lastwritetime);
    psmbcmd->value.openandx_r.filesize = 0;
    netwox__smbcmdcmn_accessmode_initdefault(psmbcmd->value.openandx_r.grantedaccess);
    netwox__smbcmdcmn_filetype_initdefault(psmbcmd->value.openandx_r.filetype);
    psmbcmd->value.openandx_r.ipcstate = 0;
    netwox__smbcmdcmn_createaction_initdefault(psmbcmd->value.openandx_r.action);
    psmbcmd->value.openandx_r.serverfileid = 0;
    break;
  case NETWOX_SMBCMD_TYPE_READANDX_Q :
    psmbcmd->value.readandx_q.fileid = 0;
    netwox__smbcmdcmn_uint64_initdefault(psmbcmd->value.readandx_q.offset);
    psmbcmd->value.readandx_q.maxsize = 0;
    psmbcmd->value.readandx_q.minsize = 0;
    psmbcmd->value.readandx_q.timeout = 0;
    psmbcmd->value.readandx_q.remaining = NETWOX_SMBCMD_READANDX_Q_REMAINING_SAFE(0);
    break;
  case NETWOX_SMBCMD_TYPE_READANDX_R :
    psmbcmd->value.readandx_r.remaining = NETWOX_SMBCMD_READANDX_R_REMAINING_SAFE;
    psmbcmd->value.readandx_r.datacompactionmode = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.readandx_r.data));
    break;
  case NETWOX_SMBCMD_TYPE_WRITEANDX_Q :
    psmbcmd->value.writeandx_q.fileid = 0;
    netwox__smbcmdcmn_uint64_initdefault(psmbcmd->value.writeandx_q.offset);
    psmbcmd->value.writeandx_q.timeout = 0;
    psmbcmd->value.writeandx_q.writemode = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.writeandx_q.data));
    psmbcmd->value.writeandx_q.remaining = NETWOX_SMBCMD_WRITEANDX_Q_REMAINING_SAFE(&psmbcmd->value.writeandx_q.data);
    break;
  case NETWOX_SMBCMD_TYPE_WRITEANDX_R :
    psmbcmd->value.writeandx_r.writtensize = 0;
    psmbcmd->value.writeandx_r.remaining = NETWOX_SMBCMD_WRITEANDX_R_REMAINING_SAFE;
    break;
  case NETWOX_SMBCMD_TYPE_CLOSETREEDISCONNECT_Q :
    psmbcmd->value.closetreedisconnect_q.fileid = 0;
    netwox__smbcmdcmn_time1970l_initdefault(psmbcmd->value.closetreedisconnect_q.lastwritetime);
    break;
  case NETWOX_SMBCMD_TYPE_CLOSETREEDISCONNECT_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_TRANSACTION2_Q :
    psmbcmd->value.transaction2_q.totalparasize = 0;
    psmbcmd->value.transaction2_q.totaldatasize = 0;
    /* if we set too big values, server sends back an error */
    psmbcmd->value.transaction2_q.maxparasize = 1024;
    psmbcmd->value.transaction2_q.maxdatasize = 16384;
    psmbcmd->value.transaction2_q.maxsetupcount = NETWOX_SMBCMD_TRANSACTION2_SETUP_MAX;
    psmbcmd->value.transaction2_q.flags = 0;
    psmbcmd->value.transaction2_q.timeout = 0;
    psmbcmd->value.transaction2_q.setupcount = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.transaction2_q.para));
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.transaction2_q.data));
    psmbcmd->value.transaction2_q.isunicode = NETWIB_FALSE;
    break;
  case NETWOX_SMBCMD_TYPE_TRANSACTION2_R :
    psmbcmd->value.transaction2_r.totalparasize = 0;
    psmbcmd->value.transaction2_r.totaldatasize = 0;
    psmbcmd->value.transaction2_r.paradisplacement = 0;
    psmbcmd->value.transaction2_r.datadisplacement = 0;
    psmbcmd->value.transaction2_r.setupcount = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.transaction2_r.para));
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.transaction2_r.data));
    break;
  case NETWOX_SMBCMD_TYPE_FINDCLOSE2_Q :
    psmbcmd->value.findclose2_q.sid = 0;
    break;
  case NETWOX_SMBCMD_TYPE_FINDCLOSE2_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_TREEDISCONNECT_Q :
  case NETWOX_SMBCMD_TYPE_TREEDISCONNECT_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_NEGOTIATE_Q :
    psmbcmd->value.negotiate_q.ntlm012position = 0;
    break;
  case NETWOX_SMBCMD_TYPE_NEGOTIATE_R0 :
    psmbcmd->value.negotiate_r0.dialectindex = 0;
    psmbcmd->value.negotiate_r0.secumode = 0;
    psmbcmd->value.negotiate_r0.maxmultiplex = 0;
    psmbcmd->value.negotiate_r0.maxvc = 0;
    psmbcmd->value.negotiate_r0.maxmsgsize = 0;
    psmbcmd->value.negotiate_r0.maxrawsize = 0;
    psmbcmd->value.negotiate_r0.vcid = 0;
    netwox__smbcmdcmn_negocapa_initdefault(psmbcmd->value.negotiate_r0.capabilities);
    netwox__smbcmdcmn_time1601_initdefault(psmbcmd->value.negotiate_r0.systemtime);
    psmbcmd->value.negotiate_r0.timezoneoffset = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.negotiate_r0.challenge));
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.negotiate_r0.domain));
    psmbcmd->value.negotiate_r0.i1set = NETWIB_FALSE;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.negotiate_r0.i1.server));
    break;
  case NETWOX_SMBCMD_TYPE_NEGOTIATE_R1 :
    psmbcmd->value.negotiate_r1.dialectindex = 0;
    psmbcmd->value.negotiate_r1.secumode = 0;
    psmbcmd->value.negotiate_r1.maxmultiplex = 0;
    psmbcmd->value.negotiate_r1.maxvc = 0;
    psmbcmd->value.negotiate_r1.maxmsgsize = 0;
    psmbcmd->value.negotiate_r1.maxrawsize = 0;
    psmbcmd->value.negotiate_r1.vcid = 0;
    netwox__smbcmdcmn_negocapa_initdefault(psmbcmd->value.negotiate_r1.capabilities);
    psmbcmd->value.negotiate_r1.capabilities |= NETWOX_SMBCMDCMN_NEGOCAPA_EXTENDEDSECURITY;
    netwox__smbcmdcmn_time1601_initdefault(psmbcmd->value.negotiate_r1.systemtime);
    psmbcmd->value.negotiate_r1.timezoneoffset = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.negotiate_r1.serverguid));
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.negotiate_r1.securityblob));
    break;
  case NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_Q0 :
    psmbcmd->value.sessionsetupandx_q0.maxmsgsize = 0;
    psmbcmd->value.sessionsetupandx_q0.maxmultiplex = 1;
    psmbcmd->value.sessionsetupandx_q0.vcnumber = 0;
    psmbcmd->value.sessionsetupandx_q0.vcid = 0;
    netwox__smbcmdcmn_negocapa_initdefault(psmbcmd->value.sessionsetupandx_q0.capabilities);
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.sessionsetupandx_q0.password));
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.sessionsetupandx_q0.unicodepassword));
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.sessionsetupandx_q0.user));
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.sessionsetupandx_q0.domain));
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.sessionsetupandx_q0.nativeos));
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.sessionsetupandx_q0.nativelanman));
    psmbcmd->value.sessionsetupandx_q0.i1set = NETWIB_FALSE;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.sessionsetupandx_q0.i1.unknown));
    break;
  case NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_Q1 :
    psmbcmd->value.sessionsetupandx_q1.maxmsgsize = 0;
    psmbcmd->value.sessionsetupandx_q1.maxmultiplex = 1;
    psmbcmd->value.sessionsetupandx_q1.vcnumber = 0;
    psmbcmd->value.sessionsetupandx_q1.vcid = 0;
    netwox__smbcmdcmn_negocapa_initdefault(psmbcmd->value.sessionsetupandx_q1.capabilities);
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.sessionsetupandx_q1.secublob));
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.sessionsetupandx_q1.nativeos));
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.sessionsetupandx_q1.nativelanman));
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.sessionsetupandx_q1.domain));
    break;
  case NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_R0 :
    psmbcmd->value.sessionsetupandx_r0.action = 0;
    psmbcmd->value.sessionsetupandx_r0.i1set = NETWIB_FALSE;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.sessionsetupandx_r0.i1.nativeos));
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.sessionsetupandx_r0.i1.nativelanman));
    psmbcmd->value.sessionsetupandx_r0.i1.i2set = NETWIB_FALSE;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.sessionsetupandx_r0.i1.i2.domain));
    break;
  case NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_R1 :
    psmbcmd->value.sessionsetupandx_r1.action = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.sessionsetupandx_r1.secublob));
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.sessionsetupandx_r1.nativeos));
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.sessionsetupandx_r1.nativelanman));
    psmbcmd->value.sessionsetupandx_r1.i2set = NETWIB_FALSE;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.sessionsetupandx_r1.i2.domain));
    break;
  case NETWOX_SMBCMD_TYPE_LOGOFFANDX_Q :
  case NETWOX_SMBCMD_TYPE_LOGOFFANDX_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_TREECONNECTANDX_Q :
    psmbcmd->value.treeconnectandx_q.flags = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.treeconnectandx_q.password));
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.treeconnectandx_q.path));
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.treeconnectandx_q.service));
    break;
  case NETWOX_SMBCMD_TYPE_TREECONNECTANDX_R :
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.treeconnectandx_r.service));
    psmbcmd->value.treeconnectandx_r.i1set = NETWIB_FALSE;
    psmbcmd->value.treeconnectandx_r.i1.optionalsupport = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.treeconnectandx_r.i1.nativefs));
    psmbcmd->value.treeconnectandx_r.i1.i2set = NETWIB_FALSE;
    psmbcmd->value.treeconnectandx_r.i1.i2.ui32a = 0;
    psmbcmd->value.treeconnectandx_r.i1.i2.ui32b = 0;
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATIONDISK_Q :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATIONDISK_R :
    psmbcmd->value.queryinformationdisk_r.totalunits = 0;
    psmbcmd->value.queryinformationdisk_r.sectorsperunit = 0;
    psmbcmd->value.queryinformationdisk_r.bytespersector = 0;
    psmbcmd->value.queryinformationdisk_r.freeunits = 0;
    break;
  case NETWOX_SMBCMD_TYPE_SEARCH_Q :
    psmbcmd->value.search_q.maxentries = 0;
    netwox__smbcmdcmn_fileattr16_initdefault(psmbcmd->value.search_q.searchattributes);
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.search_q.searchpattern));
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.search_q.resumekey));
    break;
  case NETWOX_SMBCMD_TYPE_SEARCH_R :
    psmbcmd->value.search_r.count = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.search_r.entries));
    break;
  case NETWOX_SMBCMD_TYPE_NTTRANSACT_Q :
    psmbcmd->value.nttransact_q.maxsetupcount = NETWOX_SMBCMD_NTTRANSACT_SETUP_MAX;
    psmbcmd->value.nttransact_q.totalparasize = 0;
    psmbcmd->value.nttransact_q.totaldatasize = 0;
    /* if we set too big values, server sends back an error */
    psmbcmd->value.nttransact_q.maxparasize = 1024;
    psmbcmd->value.nttransact_q.maxdatasize = 16384;
    psmbcmd->value.nttransact_q.subcmd = 0;
    psmbcmd->value.nttransact_q.setupcount = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.nttransact_q.para));
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.nttransact_q.data));
    psmbcmd->value.nttransact_q.isunicode = NETWIB_FALSE;
    break;
  case NETWOX_SMBCMD_TYPE_NTTRANSACT_R :
    psmbcmd->value.nttransact_r.totalparasize = 0;
    psmbcmd->value.nttransact_r.totaldatasize = 0;
    psmbcmd->value.nttransact_r.paradisplacement = 0;
    psmbcmd->value.nttransact_r.datadisplacement = 0;
    psmbcmd->value.nttransact_r.setupcount = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.nttransact_r.para));
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.nttransact_r.data));
    break;
  case NETWOX_SMBCMD_TYPE_NTCREATEANDX_Q :
    netwox__smbcmdcmn_openflags32_initdefault(psmbcmd->value.ntcreateandx_q.openflags);
    psmbcmd->value.ntcreateandx_q.rootfileid = 0;
    netwox__smbcmdcmn_accessmask_initdefault(psmbcmd->value.ntcreateandx_q.accessmask);
    netwox__smbcmdcmn_uint64_initdefault(psmbcmd->value.ntcreateandx_q.allocationsize);
    netwox__smbcmdcmn_fileattr32_initdefault(psmbcmd->value.ntcreateandx_q.fileattributes);
    netwox__smbcmdcmn_shareaccess_initdefault(psmbcmd->value.ntcreateandx_q.shareaccess);
    netwox__smbcmdcmn_createfunction_initdefault(psmbcmd->value.ntcreateandx_q.createfunction);
    netwox__smbcmdcmn_createopt_initdefault(psmbcmd->value.ntcreateandx_q.createoptions);
    netwox__smbcmdcmn_impersonation_initdefault(psmbcmd->value.ntcreateandx_q.impersonation);
    netwox__smbcmdcmn_secuflags_initdefault(psmbcmd->value.ntcreateandx_q.secuflags);
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.ntcreateandx_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_NTCREATEANDX_R :
    netwox__smbcmdcmn_oplocklevel_initdefault(psmbcmd->value.ntcreateandx_r.oplocklevel);
    psmbcmd->value.ntcreateandx_r.fileid = 0;
    netwox__smbcmdcmn_createaction_initdefault(psmbcmd->value.ntcreateandx_r.createaction);
    netwox__smbcmdcmn_time1601_initdefault(psmbcmd->value.ntcreateandx_r.creationtime);
    netwox__smbcmdcmn_time1601_initdefault(psmbcmd->value.ntcreateandx_r.lastaccesstime);
    netwox__smbcmdcmn_time1601_initdefault(psmbcmd->value.ntcreateandx_r.lastwritetime);
    netwox__smbcmdcmn_time1601_initdefault(psmbcmd->value.ntcreateandx_r.changetime);
    netwox__smbcmdcmn_fileattr32_initdefault(psmbcmd->value.ntcreateandx_r.fileattributes);
    netwox__smbcmdcmn_uint64_initdefault(psmbcmd->value.ntcreateandx_r.allocationsize);
    netwox__smbcmdcmn_uint64_initdefault(psmbcmd->value.ntcreateandx_r.filesize);
    netwox__smbcmdcmn_filetype_initdefault(psmbcmd->value.ntcreateandx_r.filetype);
    psmbcmd->value.ntcreateandx_r.ipcstate = 0;
    psmbcmd->value.ntcreateandx_r.isdirectory = NETWIB_FALSE;
    break;
  case NETWOX_SMBCMD_TYPE_NTCANCEL_Q :
  case NETWOX_SMBCMD_TYPE_NTCANCEL_R :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_NTRENAME_Q :
    netwox__smbcmdcmn_fileattr16_initdefault(psmbcmd->value.ntrename_q.searchattributes);
    psmbcmd->value.ntrename_q.informationlevel = 0;
    psmbcmd->value.ntrename_q.clustercount = 0;
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.ntrename_q.oldname));
    netwib_er(netwox_smbcmdcmn_data_setdefault(&psmbcmd->value.ntrename_q.newname));
    break;
  case NETWOX_SMBCMD_TYPE_NTRENAME_R :
    /* nothing to initialize */
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmd_close(netwox_smbcmd *psmbcmd)
{
  switch(psmbcmd->type) {
  case NETWOX_SMBCMD_TYPE_UNKNOWN :
    break;
  case NETWOX_SMBCMD_TYPE_CREATEDIRECTORY_Q :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.createdirectory_q.dirname));
    break;
  case NETWOX_SMBCMD_TYPE_CREATEDIRECTORY_R :
    /* nothing to close */
    break;
  case NETWOX_SMBCMD_TYPE_DELETEDIRECTORY_Q :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.deletedirectory_q.dirname));
    break;
  case NETWOX_SMBCMD_TYPE_DELETEDIRECTORY_R :
    /* nothing to close */
    break;
  case NETWOX_SMBCMD_TYPE_CREATE_Q :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.create_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_CREATE_R :
    /* nothing to close */
    break;
  case NETWOX_SMBCMD_TYPE_OPEN_Q :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.open_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_OPEN_R :
  case NETWOX_SMBCMD_TYPE_CLOSE_Q :
  case NETWOX_SMBCMD_TYPE_CLOSE_R :
  case NETWOX_SMBCMD_TYPE_FLUSH_Q :
  case NETWOX_SMBCMD_TYPE_FLUSH_R :
    /* nothing to close */
    break;
  case NETWOX_SMBCMD_TYPE_DELETE_Q :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.delete_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_DELETE_R :
    /* nothing to close */
    break;
  case NETWOX_SMBCMD_TYPE_RENAME_Q :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.rename_q.oldname));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.rename_q.newname));
    break;
  case NETWOX_SMBCMD_TYPE_RENAME_R :
    /* nothing to close */
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATION_Q :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.queryinformation_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATION_R :
    /* nothing to close */
    break;
  case NETWOX_SMBCMD_TYPE_SETINFORMATION_Q :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.setinformation_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_SETINFORMATION_R :
    /* nothing to close */
    break;
  case NETWOX_SMBCMD_TYPE_READ_Q :
    /* nothing to close */
    break;
  case NETWOX_SMBCMD_TYPE_READ_R :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.read_r.data));
    break;
  case NETWOX_SMBCMD_TYPE_WRITE_Q :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.write_q.data));
    break;
  case NETWOX_SMBCMD_TYPE_WRITE_R :
    /* nothing to close */
    break;
  case NETWOX_SMBCMD_TYPE_CREATETEMPORARY_Q :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.createtemporary_q.dirname));
    break;
  case NETWOX_SMBCMD_TYPE_CREATETEMPORARY_R :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.createtemporary_r.filename));
    break;
  case NETWOX_SMBCMD_TYPE_CREATENEW_Q :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.createnew_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_CREATENEW_R :
    break;
  case NETWOX_SMBCMD_TYPE_CHECKDIRPATH_Q :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.checkdirpath_q.dirname));
    break;
  case NETWOX_SMBCMD_TYPE_CHECKDIRPATH_R :
  case NETWOX_SMBCMD_TYPE_SEEK_Q :
  case NETWOX_SMBCMD_TYPE_SEEK_R :
  case NETWOX_SMBCMD_TYPE_READRAW_Q :
    /* nothing to initialize */
    break;
  case NETWOX_SMBCMD_TYPE_WRITERAW_Q :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.writeraw_q.data));
    break;
  case NETWOX_SMBCMD_TYPE_WRITERAW_R :
  case NETWOX_SMBCMD_TYPE_SETINFORMATION2_Q :
  case NETWOX_SMBCMD_TYPE_SETINFORMATION2_R :
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATION2_Q :
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATION2_R :
  case NETWOX_SMBCMD_TYPE_LOCKINGANDX_Q :
  case NETWOX_SMBCMD_TYPE_LOCKINGANDX_R :
    /* nothing to close */
    break;
  case NETWOX_SMBCMD_TYPE_TRANSACTION_Q :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.transaction_q.name));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.transaction_q.para));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.transaction_q.data));
    break;
  case NETWOX_SMBCMD_TYPE_TRANSACTION_R :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.transaction_r.para));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.transaction_r.data));
    break;
  case NETWOX_SMBCMD_TYPE_ECHO_Q :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.echo_q.data));
    break;
  case NETWOX_SMBCMD_TYPE_ECHO_R :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.echo_r.data));
    break;
  case NETWOX_SMBCMD_TYPE_OPENANDX_Q :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.openandx_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_OPENANDX_R :
  case NETWOX_SMBCMD_TYPE_READANDX_Q :
    /* nothing to close */
    break;
  case NETWOX_SMBCMD_TYPE_READANDX_R :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.readandx_r.data));
    break;
  case NETWOX_SMBCMD_TYPE_WRITEANDX_Q :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.writeandx_q.data));
    break;
  case NETWOX_SMBCMD_TYPE_WRITEANDX_R :
  case NETWOX_SMBCMD_TYPE_CLOSETREEDISCONNECT_Q :
  case NETWOX_SMBCMD_TYPE_CLOSETREEDISCONNECT_R :
    /* nothing to close */
    break;
  case NETWOX_SMBCMD_TYPE_TRANSACTION2_Q :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.transaction2_q.para));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.transaction2_q.data));
    break;
  case NETWOX_SMBCMD_TYPE_TRANSACTION2_R :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.transaction2_r.para));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.transaction2_r.data));
    break;
  case NETWOX_SMBCMD_TYPE_FINDCLOSE2_Q :
  case NETWOX_SMBCMD_TYPE_FINDCLOSE2_R :
  case NETWOX_SMBCMD_TYPE_TREEDISCONNECT_Q :
  case NETWOX_SMBCMD_TYPE_TREEDISCONNECT_R :
  case NETWOX_SMBCMD_TYPE_NEGOTIATE_Q :
    /* nothing to close */
    break;
  case NETWOX_SMBCMD_TYPE_NEGOTIATE_R0 :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.negotiate_r0.challenge));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.negotiate_r0.domain));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.negotiate_r0.i1.server));
    break;
  case NETWOX_SMBCMD_TYPE_NEGOTIATE_R1 :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.negotiate_r1.serverguid));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.negotiate_r1.securityblob));
    break;
  case NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_Q0 :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.sessionsetupandx_q0.password));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.sessionsetupandx_q0.unicodepassword));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.sessionsetupandx_q0.user));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.sessionsetupandx_q0.domain));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.sessionsetupandx_q0.nativeos));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.sessionsetupandx_q0.nativelanman));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.sessionsetupandx_q0.i1.unknown));
    break;
  case NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_Q1 :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.sessionsetupandx_q1.secublob));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.sessionsetupandx_q1.nativeos));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.sessionsetupandx_q1.nativelanman));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.sessionsetupandx_q1.domain));
    break;
  case NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_R0 :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.sessionsetupandx_r0.i1.nativeos));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.sessionsetupandx_r0.i1.nativelanman));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.sessionsetupandx_r0.i1.i2.domain));
    break;
  case NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_R1 :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.sessionsetupandx_r1.secublob));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.sessionsetupandx_r1.nativeos));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.sessionsetupandx_r1.nativelanman));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.sessionsetupandx_r1.i2.domain));
    break;
  case NETWOX_SMBCMD_TYPE_LOGOFFANDX_Q :
  case NETWOX_SMBCMD_TYPE_LOGOFFANDX_R :
    /* nothing to close */
    break;
  case NETWOX_SMBCMD_TYPE_TREECONNECTANDX_Q :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.treeconnectandx_q.password));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.treeconnectandx_q.path));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.treeconnectandx_q.service));
    break;
  case NETWOX_SMBCMD_TYPE_TREECONNECTANDX_R :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.treeconnectandx_r.service));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.treeconnectandx_r.i1.nativefs));
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATIONDISK_Q :
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATIONDISK_R :
    /* nothing to close */
    break;
  case NETWOX_SMBCMD_TYPE_SEARCH_Q :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.search_q.searchpattern));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.search_q.resumekey));
    break;
  case NETWOX_SMBCMD_TYPE_SEARCH_R :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.search_r.entries));
    break;
  case NETWOX_SMBCMD_TYPE_NTTRANSACT_Q :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.nttransact_q.para));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.nttransact_q.data));
    break;
  case NETWOX_SMBCMD_TYPE_NTTRANSACT_R :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.nttransact_r.para));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.nttransact_r.data));
    break;
  case NETWOX_SMBCMD_TYPE_NTCREATEANDX_Q :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.ntcreateandx_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_NTCREATEANDX_R :
  case NETWOX_SMBCMD_TYPE_NTCANCEL_Q :
  case NETWOX_SMBCMD_TYPE_NTCANCEL_R :
    /* nothing to close */
    break;
  case NETWOX_SMBCMD_TYPE_NTRENAME_Q :
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.ntrename_q.oldname));
    netwib_er(netwox_smbcmdcmn_data_close(&psmbcmd->value.ntrename_q.newname));
    break;
  case NETWOX_SMBCMD_TYPE_NTRENAME_R :
    /* nothing to close */
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbcmd_show(netwox_constsmbcmd *psmbcmd,
                              netwib_buf *pbuf)
{
  netwib_uint32 i;

  switch(psmbcmd->type) {
  case NETWOX_SMBCMD_TYPE_CREATEDIRECTORY_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Create Directory Query"));
    netwib_er(netwox_smbcmdcmn_data_show(" dirname", NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.createdirectory_q.dirname, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_CREATEDIRECTORY_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Create Directory Reply"));
    break;
  case NETWOX_SMBCMD_TYPE_DELETEDIRECTORY_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Delete Directory Query"));
    netwib_er(netwox_smbcmdcmn_data_show(" dirname", NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.deletedirectory_q.dirname, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_DELETEDIRECTORY_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Delete Directory Reply"));
    break;
  case NETWOX_SMBCMD_TYPE_CREATE_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Create Query"));
    netwib_er(netwox_smbcmdcmn_fileattr16_show(" fileattr", psmbcmd->value.create_q.fileattr, pbuf));
    netwib_er(netwox_smbcmdcmn_time1970l_show(" creationtime", psmbcmd->value.create_q.creationtime, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" dirname", NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.create_q.filename, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_CREATE_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Create Reply"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmd->value.create_r.fileid));
    break;
  case NETWOX_SMBCMD_TYPE_OPEN_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Open Query"));
    netwib_er(netwox_smbcmdcmn_accessmode_show(" desiredaccess", psmbcmd->value.open_q.desiredaccess, pbuf));
    netwib_er(netwox_smbcmdcmn_fileattr16_show(" searchattributes", psmbcmd->value.open_q.searchattributes, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" filename", NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.open_q.filename, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_OPEN_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Open Reply"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmd->value.open_r.fileid));
    netwib_er(netwox_smbcmdcmn_fileattr16_show(" fileattributes", psmbcmd->value.open_r.fileattributes, pbuf));
    netwib_er(netwox_smbcmdcmn_time1970l_show(" lastwritetime", psmbcmd->value.open_r.lastwritetime, pbuf));
    netwib_er(netwib_show_array_fmt32(pbuf, " filesize=%{uint32}", psmbcmd->value.open_r.filesize));
    netwib_er(netwox_smbcmdcmn_accessmode_show(" grantedaccess", psmbcmd->value.open_r.grantedaccess, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_CLOSE_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Close Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmd->value.close_q.fileid));
    netwib_er(netwox_smbcmdcmn_time1970l_show(" lastwritetime", psmbcmd->value.close_q.lastwritetime, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_CLOSE_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Close Reply"));
    break;
  case NETWOX_SMBCMD_TYPE_FLUSH_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Flush Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmd->value.flush_q.fileid));
    break;
  case NETWOX_SMBCMD_TYPE_FLUSH_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Flush Reply"));
    break;
  case NETWOX_SMBCMD_TYPE_DELETE_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Delete Query"));
    netwib_er(netwox_smbcmdcmn_fileattr16_show(" searchattributes", psmbcmd->value.delete_q.searchattributes, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" filename", NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.delete_q.filename, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_DELETE_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Delete Reply"));
    break;
  case NETWOX_SMBCMD_TYPE_RENAME_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Rename Query"));
    netwib_er(netwox_smbcmdcmn_fileattr16_show(" searchattributes", psmbcmd->value.rename_q.searchattributes, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" oldname", NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.rename_q.oldname, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" newname", NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.rename_q.newname, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_RENAME_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Rename Reply"));
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATION_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Query Information Query"));
    netwib_er(netwox_smbcmdcmn_data_show(" filename", NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.queryinformation_q.filename, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATION_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Query Information Reply"));
    netwib_er(netwox_smbcmdcmn_fileattr16_show(" fileattributes", psmbcmd->value.queryinformation_r.fileattributes, pbuf));
    netwib_er(netwox_smbcmdcmn_time1970l_show(" lastwritetime", psmbcmd->value.queryinformation_r.lastwritetime, pbuf));
    netwib_er(netwib_show_array_fmt32(pbuf, " filesize=%{uint32}", psmbcmd->value.queryinformation_r.filesize));
    break;
  case NETWOX_SMBCMD_TYPE_SETINFORMATION_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Query Information Query"));
    netwib_er(netwox_smbcmdcmn_fileattr16_show(" fileattributes", psmbcmd->value.setinformation_q.fileattributes, pbuf));
    netwib_er(netwox_smbcmdcmn_time1970l_show(" lastwritetime", psmbcmd->value.setinformation_q.lastwritetime, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" filename", NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.setinformation_q.filename, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_SETINFORMATION_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Query Information Reply"));
    break;
  case NETWOX_SMBCMD_TYPE_READ_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Read Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmd->value.read_q.fileid));
    netwib_er(netwib_show_array_fmt32(pbuf, " size=%{uint16}", psmbcmd->value.read_q.size));
    netwib_er(netwib_show_array_fmt32(pbuf, " offset=%{uint32}", psmbcmd->value.read_q.offset));
    netwib_er(netwib_show_array_fmt32(pbuf, " remaining=%{uint16}", psmbcmd->value.read_q.remaining));
    break;
  case NETWOX_SMBCMD_TYPE_READ_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Read Reply"));
    netwib_er(netwox_smbcmdcmn_data_show(" data", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.read_r.data, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_WRITE_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Write Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmd->value.write_q.fileid));
    netwib_er(netwib_show_array_fmt32(pbuf, " offset=%{uint32}", psmbcmd->value.write_q.offset));
    netwib_er(netwib_show_array_fmt32(pbuf, " remaining=%{uint16}", psmbcmd->value.write_q.remaining));
    netwib_er(netwox_smbcmdcmn_data_show(" data", NETWOX_SMBCMDCMN_DATATYPE_DATABLOCK, &psmbcmd->value.write_q.data, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_WRITE_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Write Reply"));
    netwib_er(netwib_show_array_fmt32(pbuf, " writtensize=%{uint16}", psmbcmd->value.write_r.writtensize));
    break;
  case NETWOX_SMBCMD_TYPE_CREATETEMPORARY_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Create Temporary Query"));
    netwib_er(netwox_smbcmdcmn_fileattr16_show(" fileattr", psmbcmd->value.createtemporary_q.fileattr, pbuf));
    netwib_er(netwox_smbcmdcmn_time1970l_show(" creationtime", psmbcmd->value.createtemporary_q.creationtime, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" dirname", NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.createtemporary_q.dirname, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_CREATETEMPORARY_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Create Temporary Reply"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmd->value.createtemporary_r.fileid));
    netwib_er(netwox_smbcmdcmn_data_show(" filename", NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.createtemporary_r.filename, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_CREATENEW_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Create New Query"));
    netwib_er(netwox_smbcmdcmn_fileattr16_show(" fileattributes", psmbcmd->value.createnew_q.fileattributes, pbuf));
    netwib_er(netwox_smbcmdcmn_time1970l_show(" creationtime", psmbcmd->value.createnew_q.creationtime, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" filename", NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.createnew_q.filename, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_CREATENEW_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Create New Reply"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmd->value.createnew_r.fileid));
    break;
  case NETWOX_SMBCMD_TYPE_CHECKDIRPATH_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Check Directory Path Query"));
    netwib_er(netwox_smbcmdcmn_data_show(" dirname", NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.checkdirpath_q.dirname, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_CHECKDIRPATH_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Check Directory Path Reply"));
    break;
  case NETWOX_SMBCMD_TYPE_SEEK_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Seek Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmd->value.seek_q.fileid));
    netwib_er(netwox_smbcmdcmn_seektype_show(" seektype", psmbcmd->value.seek_q.seektype, pbuf));
    netwib_er(netwib_show_array_fmt32(pbuf, " offset=%{uint32}", psmbcmd->value.seek_q.offset));
    break;
  case NETWOX_SMBCMD_TYPE_SEEK_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Seek Reply"));
    netwib_er(netwib_show_array_fmt32(pbuf, " offset=%{uint32}", psmbcmd->value.seek_r.offset));
    break;
  case NETWOX_SMBCMD_TYPE_READRAW_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Read Raw Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmd->value.readraw_q.fileid));
    netwib_er(netwox_smbcmdcmn_uint64_show(" offset", psmbcmd->value.readraw_q.offset, pbuf));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxsize=%{uint16}", psmbcmd->value.readraw_q.maxsize));
    netwib_er(netwib_show_array_fmt32(pbuf, " minsize=%{uint16}", psmbcmd->value.readraw_q.minsize));
    netwib_er(netwib_show_array_fmt32(pbuf, " timeout=%{uint32}", psmbcmd->value.readraw_q.timeout));
    break;
  case NETWOX_SMBCMD_TYPE_WRITERAW_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Write Raw Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmd->value.writeraw_q.fileid));
    netwib_er(netwib_show_array_fmt32(pbuf, " rawdatasize=%{uint32}", psmbcmd->value.writeraw_q.rawdatasize));
    netwib_er(netwox_smbcmdcmn_uint64_show(" offset", psmbcmd->value.writeraw_q.offset, pbuf));
    netwib_er(netwib_show_array_fmt32(pbuf, " timeout=%{uint32}", psmbcmd->value.writeraw_q.timeout));
    netwib_er(netwox_smbcmdcmn_accessmode_show(" accessmode", psmbcmd->value.writeraw_q.accessmode, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" data", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.writeraw_q.data, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_WRITERAW_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Write Raw Reply"));
    netwib_er(netwib_show_array_fmt32(pbuf, " remaining=%{uint16}", psmbcmd->value.writeraw_r.remaining));
    break;
  case NETWOX_SMBCMD_TYPE_SETINFORMATION2_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Set Information2 Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmd->value.setinformation2_q.fileid));
    netwib_er(netwox_smbcmdcmn_timemsdos_show(" creationtime", psmbcmd->value.setinformation2_q.creationtime, pbuf));
    netwib_er(netwox_smbcmdcmn_timemsdos_show(" lastaccesstime", psmbcmd->value.setinformation2_q.lastaccesstime, pbuf));
    netwib_er(netwox_smbcmdcmn_timemsdos_show(" lastwritetime", psmbcmd->value.setinformation2_q.lastwritetime, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_SETINFORMATION2_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Set Information2 Reply"));
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATION2_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Query Information2 Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmd->value.queryinformation2_q.fileid));
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATION2_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Query Information2 Reply"));
    netwib_er(netwox_smbcmdcmn_timemsdos_show(" creationtime", psmbcmd->value.queryinformation2_r.creationtime, pbuf));
    netwib_er(netwox_smbcmdcmn_timemsdos_show(" lastaccesstime", psmbcmd->value.queryinformation2_r.lastaccesstime, pbuf));
    netwib_er(netwox_smbcmdcmn_timemsdos_show(" lastwritetime", psmbcmd->value.queryinformation2_r.lastwritetime, pbuf));
    netwib_er(netwib_show_array_fmt32(pbuf, " filesize=%{uint32}", psmbcmd->value.queryinformation2_r.filesize));
    netwib_er(netwib_show_array_fmt32(pbuf, " allocationsize=%{uint32}", psmbcmd->value.queryinformation2_r.allocationsize));
    netwib_er(netwox_smbcmdcmn_fileattr16_show(" fileattributes", psmbcmd->value.queryinformation2_r.fileattributes, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_LOCKINGANDX_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Locking AndX Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmd->value.lockingandx_q.fileid));
    netwib_er(netwib_show_array_fmt32(pbuf, " locktype=%{uint8:8b}", psmbcmd->value.lockingandx_q.locktype));
    netwib_er(netwox_smbcmdcmn_oplocklevel_show(" oplocklevel", psmbcmd->value.lockingandx_q.oplocklevel, pbuf));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint32}", psmbcmd->value.lockingandx_q.timeout));
    if (psmbcmd->value.lockingandx_q.unlockscount > NETWOX_SMBCMD_LOCKINGANDX_RANGE_MAX ||
        psmbcmd->value.lockingandx_q.lockscount > NETWOX_SMBCMD_LOCKINGANDX_RANGE_MAX) {
      return(NETWIB_ERR_PATOOHIGH);
    }
    netwib_er(netwib_show_array_fmt32(pbuf, " Unlocks:"));
    for (i = 0; i < psmbcmd->value.lockingandx_q.unlockscount; i++) {
      netwib_er(netwib_show_array_fmt32(pbuf, "  pid=%{uint16}", psmbcmd->value.lockingandx_q.unlocks[i].pid));
      netwib_er(netwib_show_array_fmt32(pbuf, "  offset=%{uint64}", psmbcmd->value.lockingandx_q.unlocks[i].offset));
      netwib_er(netwib_show_array_fmt32(pbuf, "  length=%{uint64}", psmbcmd->value.lockingandx_q.unlocks[i].length));
    }
    netwib_er(netwib_show_array_fmt32(pbuf, " Locks:"));
    for (i = 0; i < psmbcmd->value.lockingandx_q.lockscount; i++) {
      netwib_er(netwib_show_array_fmt32(pbuf, "  pid=%{uint16}", psmbcmd->value.lockingandx_q.locks[i].pid));
      netwib_er(netwib_show_array_fmt32(pbuf, "  offset=%{uint64}", psmbcmd->value.lockingandx_q.locks[i].offset));
      netwib_er(netwib_show_array_fmt32(pbuf, "  length=%{uint64}", psmbcmd->value.lockingandx_q.locks[i].length));
    }
    break;
  case NETWOX_SMBCMD_TYPE_LOCKINGANDX_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Locking Andx Reply"));
    break;
  case NETWOX_SMBCMD_TYPE_TRANSACTION_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Transaction Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " totalparasize=%{uint16}", psmbcmd->value.transaction_q.totalparasize));
    netwib_er(netwib_show_array_fmt32(pbuf, " totaldatasize=%{uint16}", psmbcmd->value.transaction_q.totaldatasize));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxparasize=%{uint16}", psmbcmd->value.transaction_q.maxparasize));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxdatasize=%{uint16}", psmbcmd->value.transaction_q.maxdatasize));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxsetupcount=%{uint8}", psmbcmd->value.transaction_q.maxsetupcount));
    netwib_er(netwib_show_array_fmt32(pbuf, " flags=%{uint16:16b}", psmbcmd->value.transaction_q.flags));
    netwib_er(netwib_show_array_fmt32(pbuf, " timeout=%{uint32} ms", psmbcmd->value.transaction_q.timeout));
    netwib_er(netwib_show_array_fmt32(pbuf, " setupcount=%{uint8}", psmbcmd->value.transaction_q.setupcount));
    if (psmbcmd->value.transaction_q.setupcount > NETWOX_SMBCMD_TRANSACTION_SETUP_MAX) {
      netwib_er(netwib_show_array_fmt32(pbuf, "  -> too big"));
    } else {
      for (i = 0; i < psmbcmd->value.transaction_q.setupcount; i++) {
        netwib_er(netwib_show_array_fmt32(pbuf, " setup[%{uint32}]=%{uint16}", i, psmbcmd->value.transaction_q.setup[i]));
      }
    }
    netwib_er(netwox_smbcmdcmn_data_show(" name", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.transaction_q.name, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" para", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.transaction_q.para, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" data", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.transaction_q.data, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_TRANSACTION_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Transaction Reply"));
    netwib_er(netwib_show_array_fmt32(pbuf, " totalparasize=%{uint16}", psmbcmd->value.transaction_r.totalparasize));
    netwib_er(netwib_show_array_fmt32(pbuf, " totaldatasize=%{uint16}", psmbcmd->value.transaction_r.totaldatasize));
    netwib_er(netwib_show_array_fmt32(pbuf, " paradisplacement=%{uint16}", psmbcmd->value.transaction_r.paradisplacement));
    netwib_er(netwib_show_array_fmt32(pbuf, " datadisplacement=%{uint16}", psmbcmd->value.transaction_r.datadisplacement));
    netwib_er(netwib_show_array_fmt32(pbuf, " setupcount=%{uint8}", psmbcmd->value.transaction_r.setupcount));
    if (psmbcmd->value.transaction_r.setupcount > NETWOX_SMBCMD_TRANSACTION_SETUP_MAX) {
      netwib_er(netwib_show_array_fmt32(pbuf, "  -> too big"));
    } else {
      for (i = 0; i < psmbcmd->value.transaction_r.setupcount; i++) {
        netwib_er(netwib_show_array_fmt32(pbuf, " setup[%{uint32}]=%{uint16}", i, psmbcmd->value.transaction_r.setup[i]));
      }
    }
    netwib_er(netwox_smbcmdcmn_data_show(" para", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.transaction_r.para, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" data", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.transaction_r.data, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_ECHO_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Echo Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " count=%{uint16}", psmbcmd->value.echo_q.count));
    netwib_er(netwox_smbcmdcmn_data_show(" data", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.echo_q.data, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_ECHO_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Echo Reply"));
    netwib_er(netwib_show_array_fmt32(pbuf, " number=%{uint16}", psmbcmd->value.echo_r.number));
    netwib_er(netwox_smbcmdcmn_data_show(" data", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.echo_r.data, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_OPENANDX_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Open AndX Query"));
    netwib_er(netwox_smbcmdcmn_openflags16_show(" openflags", psmbcmd->value.openandx_q.openflags, pbuf));
    netwib_er(netwox_smbcmdcmn_accessmode_show(" desiredaccess", psmbcmd->value.openandx_q.desiredaccess, pbuf));
    netwib_er(netwox_smbcmdcmn_fileattr16_show(" searchattributes", psmbcmd->value.openandx_q.searchattributes, pbuf));
    netwib_er(netwox_smbcmdcmn_fileattr16_show(" fileattributes", psmbcmd->value.openandx_q.fileattributes, pbuf));
    netwib_er(netwox_smbcmdcmn_time1970l_show(" creationtime", psmbcmd->value.openandx_q.creationtime, pbuf));
    netwib_er(netwox_smbcmdcmn_openfunction_show(" openfunction", psmbcmd->value.openandx_q.openfunction, pbuf));
    netwib_er(netwib_show_array_fmt32(pbuf, " allocationsize=%{uint32}", psmbcmd->value.openandx_q.allocationsize));
    netwib_er(netwib_show_array_fmt32(pbuf, " timeout=%{uint32}", psmbcmd->value.openandx_q.timeout));
    netwib_er(netwox_smbcmdcmn_data_show(" filename", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.openandx_q.filename, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_OPENANDX_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Open AndX Reply"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmd->value.openandx_r.fileid));
    netwib_er(netwox_smbcmdcmn_fileattr16_show(" fileattributes", psmbcmd->value.openandx_r.fileattributes, pbuf));
    netwib_er(netwox_smbcmdcmn_time1970l_show(" lastwritetime", psmbcmd->value.openandx_r.lastwritetime, pbuf));
    netwib_er(netwib_show_array_fmt32(pbuf, " filesize=%{uint32}", psmbcmd->value.openandx_r.filesize));
    netwib_er(netwox_smbcmdcmn_accessmode_show(" grantedaccess", psmbcmd->value.openandx_r.grantedaccess, pbuf));
    netwib_er(netwox_smbcmdcmn_filetype_show(" filetype", psmbcmd->value.openandx_r.filetype, pbuf));
    netwib_er(netwib_show_array_fmt32(pbuf, " ipcstate=%{uint16}", psmbcmd->value.openandx_r.ipcstate));
    netwib_er(netwox_smbcmdcmn_createaction_show(" action", psmbcmd->value.openandx_r.action, pbuf));
    netwib_er(netwib_show_array_fmt32(pbuf, " serverfileid=%{uint32}", psmbcmd->value.openandx_r.serverfileid));
    break;
  case NETWOX_SMBCMD_TYPE_READANDX_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Read AndX Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmd->value.readandx_q.fileid));
    netwib_er(netwox_smbcmdcmn_uint64_show(" offset", psmbcmd->value.readandx_q.offset, pbuf));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxsize=%{uint16}", psmbcmd->value.readandx_q.maxsize));
    netwib_er(netwib_show_array_fmt32(pbuf, " minsize=%{uint16}", psmbcmd->value.readandx_q.minsize));
    netwib_er(netwib_show_array_fmt32(pbuf, " timeout=%{uint32}", psmbcmd->value.readandx_q.timeout));
    netwib_er(netwib_show_array_fmt32(pbuf, " remaining=%{uint16}", psmbcmd->value.readandx_q.remaining));
    break;
  case NETWOX_SMBCMD_TYPE_READANDX_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Read AndX Reply"));
    netwib_er(netwib_show_array_fmt32(pbuf, " remaining=%{uint16}", psmbcmd->value.readandx_r.remaining));
    netwib_er(netwib_show_array_fmt32(pbuf, " datacompactionmode=%{uint16}", psmbcmd->value.readandx_r.datacompactionmode));
    netwib_er(netwox_smbcmdcmn_data_show(" data", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.readandx_r.data, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_WRITEANDX_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Write AndX Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmd->value.writeandx_q.fileid));
    netwib_er(netwox_smbcmdcmn_uint64_show(" offset", psmbcmd->value.writeandx_q.offset, pbuf));
    netwib_er(netwib_show_array_fmt32(pbuf, " timeout=%{uint32}", psmbcmd->value.writeandx_q.timeout));
    netwib_er(netwib_show_array_fmt32(pbuf, " writemode=%{uint32}", psmbcmd->value.writeandx_q.writemode));
    netwib_er(netwib_show_array_fmt32(pbuf, " remaining=%{uint16}", psmbcmd->value.writeandx_q.remaining));
    netwib_er(netwox_smbcmdcmn_data_show(" data", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.writeandx_q.data, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_WRITEANDX_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Write AndX Reply"));
    netwib_er(netwib_show_array_fmt32(pbuf, " writtensize=%{uint16}", psmbcmd->value.writeandx_r.writtensize));
    netwib_er(netwib_show_array_fmt32(pbuf, " remaining=%{uint16}", psmbcmd->value.writeandx_r.remaining));
    break;
  case NETWOX_SMBCMD_TYPE_CLOSETREEDISCONNECT_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Close Then Tree Disconnect Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmd->value.closetreedisconnect_q.fileid));
    netwib_er(netwox_smbcmdcmn_time1970l_show(" lastwritetime", psmbcmd->value.closetreedisconnect_q.lastwritetime, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_CLOSETREEDISCONNECT_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Close Then Tree Disconnect Reply"));
    break;
  case NETWOX_SMBCMD_TYPE_TRANSACTION2_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Transaction2 Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " totalparasize=%{uint16}", psmbcmd->value.transaction2_q.totalparasize));
    netwib_er(netwib_show_array_fmt32(pbuf, " totaldatasize=%{uint16}", psmbcmd->value.transaction2_q.totaldatasize));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxparasize=%{uint16}", psmbcmd->value.transaction2_q.maxparasize));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxdatasize=%{uint16}", psmbcmd->value.transaction2_q.maxdatasize));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxsetupcount=%{uint8}", psmbcmd->value.transaction2_q.maxsetupcount));
    netwib_er(netwib_show_array_fmt32(pbuf, " flags=%{uint16:16b}", psmbcmd->value.transaction2_q.flags));
    netwib_er(netwib_show_array_fmt32(pbuf, " timeout=%{uint32} ms", psmbcmd->value.transaction2_q.timeout));
    netwib_er(netwib_show_array_fmt32(pbuf, " setupcount=%{uint8}", psmbcmd->value.transaction2_q.setupcount));
    if (psmbcmd->value.transaction2_q.setupcount > NETWOX_SMBCMD_TRANSACTION2_SETUP_MAX) {
      netwib_er(netwib_show_array_fmt32(pbuf, "  -> too big"));
    } else {
      for (i = 0; i < psmbcmd->value.transaction2_q.setupcount; i++) {
        netwib_er(netwib_show_array_fmt32(pbuf, " setup[%{uint32}]=%{uint16}", i, psmbcmd->value.transaction2_q.setup[i]));
      }
    }
    netwib_er(netwox_smbcmdcmn_data_show(" para", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.transaction2_q.para, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" data", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.transaction2_q.data, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_TRANSACTION2_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Transaction2 Reply"));
    netwib_er(netwib_show_array_fmt32(pbuf, " totalparasize=%{uint16}", psmbcmd->value.transaction2_r.totalparasize));
    netwib_er(netwib_show_array_fmt32(pbuf, " totaldatasize=%{uint16}", psmbcmd->value.transaction2_r.totaldatasize));
    netwib_er(netwib_show_array_fmt32(pbuf, " paradisplacement=%{uint16}", psmbcmd->value.transaction2_r.paradisplacement));
    netwib_er(netwib_show_array_fmt32(pbuf, " datadisplacement=%{uint16}", psmbcmd->value.transaction2_r.datadisplacement));
    netwib_er(netwib_show_array_fmt32(pbuf, " setupcount=%{uint8}", psmbcmd->value.transaction2_r.setupcount));
    if (psmbcmd->value.transaction2_r.setupcount > NETWOX_SMBCMD_TRANSACTION2_SETUP_MAX) {
      netwib_er(netwib_show_array_fmt32(pbuf, "  -> too big"));
    } else {
      for (i = 0; i < psmbcmd->value.transaction2_r.setupcount; i++) {
        netwib_er(netwib_show_array_fmt32(pbuf, " setup[%{uint32}]=%{uint16}", i, psmbcmd->value.transaction2_r.setup[i]));
      }
    }
    netwib_er(netwox_smbcmdcmn_data_show(" para", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.transaction2_r.para, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" data", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.transaction2_r.data, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_FINDCLOSE2_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Find Close2 Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " sid=%{uint16}", psmbcmd->value.findclose2_q.sid));
    break;
  case NETWOX_SMBCMD_TYPE_FINDCLOSE2_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Find Close2 Reply"));
    break;
  case NETWOX_SMBCMD_TYPE_TREEDISCONNECT_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Tree Disconnect Query"));
    break;
  case NETWOX_SMBCMD_TYPE_TREEDISCONNECT_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Tree Disconnect Reply"));
    break;
  case NETWOX_SMBCMD_TYPE_NEGOTIATE_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Negotiate Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " ntlm012position=%{uint32}", psmbcmd->value.negotiate_q.ntlm012position));
    break;
  case NETWOX_SMBCMD_TYPE_NEGOTIATE_R0 :
    netwib_er(netwib_show_array_fmt32(pbuf, " Negotiate Reply (no Extended Security)"));
    netwib_er(netwib_show_array_fmt32(pbuf, " dialectindex=%{uint32}", psmbcmd->value.negotiate_r0.dialectindex));
    netwib_er(netwib_show_array_fmt32(pbuf, " secumode=%{uint8:8b}", psmbcmd->value.negotiate_r0.secumode));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxmultiplex=%{uint16}", psmbcmd->value.negotiate_r0.maxmultiplex));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxvc=%{uint16}", psmbcmd->value.negotiate_r0.maxvc));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxmsgsize=%{uint32}", psmbcmd->value.negotiate_r0.maxmsgsize));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxrawsize=%{uint32}", psmbcmd->value.negotiate_r0.maxrawsize));
    netwib_er(netwib_show_array_fmt32(pbuf, " vcid=%{uint32}", psmbcmd->value.negotiate_r0.vcid));
    netwox_smbcmdcmn_negocapa_show(" capabilities", psmbcmd->value.negotiate_r0.capabilities, pbuf);
    netwib_er(netwox_smbcmdcmn_time1601_show(" systemtime", psmbcmd->value.negotiate_r0.systemtime, pbuf));
    netwib_er(netwib_show_array_fmt32(pbuf, " timezoneoffset=%{int16}", psmbcmd->value.negotiate_r0.timezoneoffset));
    netwib_er(netwox_smbcmdcmn_data_show(" challenge", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.negotiate_r0.challenge, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" domain", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.negotiate_r0.domain, pbuf));
    if (psmbcmd->value.negotiate_r0.i1set) {
      netwib_er(netwox_smbcmdcmn_data_show(" i1.server", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.negotiate_r0.i1.server, pbuf));
    } else {
      netwib_er(netwib_show_array_fmt32(pbuf, " i1.server unset"));
    }
    break;
  case NETWOX_SMBCMD_TYPE_NEGOTIATE_R1 :
    netwib_er(netwib_show_array_fmt32(pbuf, " Negotiate Reply (Extended Security)"));
    netwib_er(netwib_show_array_fmt32(pbuf, " dialectindex=%{uint32}", psmbcmd->value.negotiate_r1.dialectindex));
    netwib_er(netwib_show_array_fmt32(pbuf, " secumode=%{uint8:8b}", psmbcmd->value.negotiate_r1.secumode));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxmultiplex=%{uint16}", psmbcmd->value.negotiate_r1.maxmultiplex));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxvc=%{uint16}", psmbcmd->value.negotiate_r1.maxvc));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxmsgsize=%{uint32}", psmbcmd->value.negotiate_r1.maxmsgsize));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxrawsize=%{uint32}", psmbcmd->value.negotiate_r1.maxrawsize));
    netwib_er(netwib_show_array_fmt32(pbuf, " vcid=%{uint32}", psmbcmd->value.negotiate_r1.vcid));
    netwox_smbcmdcmn_negocapa_show(" capabilities", psmbcmd->value.negotiate_r1.capabilities, pbuf);
    netwib_er(netwox_smbcmdcmn_time1601_show(" systemtime", psmbcmd->value.negotiate_r1.systemtime, pbuf));
    netwib_er(netwib_show_array_fmt32(pbuf, " timezoneoffset=%{int16}", psmbcmd->value.negotiate_r1.timezoneoffset));
    netwib_er(netwox_smbcmdcmn_data_show(" serverguid", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.negotiate_r1.serverguid, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" securityblob", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.negotiate_r1.securityblob, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_Q0 :
    netwib_er(netwib_show_array_fmt32(pbuf, " Session Setup AndX Query (LM, NTLMv1/v2)"));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxmsgsize=%{uint16}", psmbcmd->value.sessionsetupandx_q0.maxmsgsize));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxmultiplex=%{uint16}", psmbcmd->value.sessionsetupandx_q0.maxmultiplex));
    netwib_er(netwib_show_array_fmt32(pbuf, " vcnumber=%{uint16}", psmbcmd->value.sessionsetupandx_q0.vcnumber));
    netwib_er(netwib_show_array_fmt32(pbuf, " vcid=%{uint32}", psmbcmd->value.sessionsetupandx_q0.vcid));
    netwox_smbcmdcmn_negocapa_show(" capabilities", psmbcmd->value.sessionsetupandx_q0.capabilities, pbuf);
    netwib_er(netwox_smbcmdcmn_data_show(" password", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.sessionsetupandx_q0.password, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" unicodepassword", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.sessionsetupandx_q0.unicodepassword, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" user", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_q0.user, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" domain", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_q0.domain, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" nativeos", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_q0.nativeos, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" nativelanman", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_q0.nativelanman, pbuf));
    if (psmbcmd->value.sessionsetupandx_q0.i1set) {
      netwib_er(netwox_smbcmdcmn_data_show(" i1.unknown", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_q0.i1.unknown, pbuf));
    } else {
      netwib_er(netwib_show_array_fmt32(pbuf, " i1.unknown unset"));
    }
    break;
  case NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_Q1 :
    netwib_er(netwib_show_array_fmt32(pbuf, " Session Setup AndX Query (Extended Security)"));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxmsgsize=%{uint16}", psmbcmd->value.sessionsetupandx_q1.maxmsgsize));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxmultiplex=%{uint16}", psmbcmd->value.sessionsetupandx_q1.maxmultiplex));
    netwib_er(netwib_show_array_fmt32(pbuf, " vcnumber=%{uint16}", psmbcmd->value.sessionsetupandx_q1.vcnumber));
    netwib_er(netwib_show_array_fmt32(pbuf, " vcid=%{uint32}", psmbcmd->value.sessionsetupandx_q1.vcid));
    netwox_smbcmdcmn_negocapa_show(" capabilities", psmbcmd->value.sessionsetupandx_q1.capabilities, pbuf);
    netwib_er(netwox_smbcmdcmn_data_show(" secublob", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.sessionsetupandx_q1.secublob, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" nativeos", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_q1.nativeos, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" nativelanman", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_q1.nativelanman, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" domain", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_q1.domain, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_R0 :
    netwib_er(netwib_show_array_fmt32(pbuf, " Session Setup AndX Reply (LM, NTLMv1/v2)"));
    netwib_er(netwib_show_array_fmt32(pbuf, " action=%{uint16:16b}", psmbcmd->value.sessionsetupandx_r0.action));
    if (psmbcmd->value.sessionsetupandx_r0.i1set) {
      netwib_er(netwox_smbcmdcmn_data_show(" i1.nativeos", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_r0.i1.nativeos, pbuf));
      netwib_er(netwox_smbcmdcmn_data_show(" i1.nativelanman", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_r0.i1.nativelanman, pbuf));
    } else {
      netwib_er(netwib_show_array_fmt32(pbuf, " i1.nativeos unset"));
      netwib_er(netwib_show_array_fmt32(pbuf, " i1.nativelanman unset"));
    }
    if (psmbcmd->value.sessionsetupandx_r0.i1set &&
        psmbcmd->value.sessionsetupandx_r0.i1.i2set) {
      netwib_er(netwox_smbcmdcmn_data_show(" i1.i2.domain", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_r0.i1.i2.domain, pbuf));
    } else {
      netwib_er(netwib_show_array_fmt32(pbuf, " i1.i2.domain unset"));
    }
    break;
  case NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_R1 :
    netwib_er(netwib_show_array_fmt32(pbuf, " Session Setup AndX Reply (Extended Security)"));
    netwib_er(netwib_show_array_fmt32(pbuf, " action=%{uint16:16b}", psmbcmd->value.sessionsetupandx_r1.action));
    netwib_er(netwox_smbcmdcmn_data_show(" secublob", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.sessionsetupandx_r1.secublob, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" nativeos", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_r1.nativeos, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" nativelanman", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_r1.nativelanman, pbuf));
    if (psmbcmd->value.sessionsetupandx_r1.i2set) {
      netwib_er(netwox_smbcmdcmn_data_show(" i2.domain", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_r1.i2.domain, pbuf));
    } else {
      netwib_er(netwib_show_array_fmt32(pbuf, " i2.domain unset"));
    }
    break;
  case NETWOX_SMBCMD_TYPE_LOGOFFANDX_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Logoff AndX Query"));
    break;
  case NETWOX_SMBCMD_TYPE_LOGOFFANDX_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Logoff AndX Reply"));
    break;
  case NETWOX_SMBCMD_TYPE_TREECONNECTANDX_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Tree Connect AndX Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " flags=%{uint16:16b}", psmbcmd->value.treeconnectandx_q.flags));
    netwib_er(netwox_smbcmdcmn_data_show(" password", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.treeconnectandx_q.password, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" path", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.treeconnectandx_q.path, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" service", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI, &psmbcmd->value.treeconnectandx_q.service, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_TREECONNECTANDX_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Tree Connect AndX Reply"));
    if (psmbcmd->value.treeconnectandx_r.i1set) {
      netwib_er(netwib_show_array_fmt32(pbuf, " i1.optionalsupport=%{uint16:16b}", psmbcmd->value.treeconnectandx_r.i1.optionalsupport));
    } else {
      netwib_er(netwib_show_array_fmt32(pbuf, " i1.optionalsupport unset"));
    }
    if (psmbcmd->value.treeconnectandx_r.i1set &&
        psmbcmd->value.treeconnectandx_r.i1.i2set) {
      netwib_er(netwib_show_array_fmt32(pbuf, " i1.i2.ui32a=%{uint32}", psmbcmd->value.treeconnectandx_r.i1.i2.ui32a));
      netwib_er(netwib_show_array_fmt32(pbuf, " i1.i2.ui32b=%{uint32}", psmbcmd->value.treeconnectandx_r.i1.i2.ui32b));
    } else {
      netwib_er(netwib_show_array_fmt32(pbuf, " i1.i2.ui32a unset"));
      netwib_er(netwib_show_array_fmt32(pbuf, " i1.i2.ui32b unset"));
    }
    netwib_er(netwox_smbcmdcmn_data_show(" service", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI, &psmbcmd->value.treeconnectandx_r.service, pbuf));
    if (psmbcmd->value.treeconnectandx_r.i1set) {
      netwib_er(netwox_smbcmdcmn_data_show(" i1.nativefs", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.treeconnectandx_r.i1.nativefs, pbuf));
    } else {
      netwib_er(netwib_show_array_fmt32(pbuf, " i1.nativefs unset"));
    }
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATIONDISK_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Query Information Disk Query"));
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATIONDISK_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Query Information Disk Reply"));
    netwib_er(netwib_show_array_fmt32(pbuf, " totalunits=%{uint16}", psmbcmd->value.queryinformationdisk_r.totalunits));
    netwib_er(netwib_show_array_fmt32(pbuf, " sectorsperunit=%{uint16}", psmbcmd->value.queryinformationdisk_r.sectorsperunit));
    netwib_er(netwib_show_array_fmt32(pbuf, " bytespersector=%{uint16}", psmbcmd->value.queryinformationdisk_r.bytespersector));
    netwib_er(netwib_show_array_fmt32(pbuf, " freeunits=%{uint16}", psmbcmd->value.queryinformationdisk_r.freeunits));
    break;
  case NETWOX_SMBCMD_TYPE_SEARCH_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " Search Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxentries=%{uint16}", psmbcmd->value.search_q.maxentries));
    netwib_er(netwox_smbcmdcmn_fileattr16_show(" searchattributes", psmbcmd->value.search_q.searchattributes, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" searchpattern", NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.search_q.searchpattern, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" resumekey", NETWOX_SMBCMDCMN_DATATYPE_VARIABLEBLOCK, &psmbcmd->value.search_q.resumekey, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_SEARCH_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " Search Reply"));
    netwib_er(netwib_show_array_fmt32(pbuf, " count=%{uint16}", psmbcmd->value.search_r.count));
    netwib_er(netwox_smbcmdcmn_data_show(" entries", NETWOX_SMBCMDCMN_DATATYPE_VARIABLEBLOCK, &psmbcmd->value.search_r.entries, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_NTTRANSACT_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " NT Transact Query"));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxsetupcount=%{uint8}", psmbcmd->value.nttransact_q.maxsetupcount));
    netwib_er(netwib_show_array_fmt32(pbuf, " totalparasize=%{uint32}", psmbcmd->value.nttransact_q.totalparasize));
    netwib_er(netwib_show_array_fmt32(pbuf, " totaldatasize=%{uint32}", psmbcmd->value.nttransact_q.totaldatasize));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxparasize=%{uint32}", psmbcmd->value.nttransact_q.maxparasize));
    netwib_er(netwib_show_array_fmt32(pbuf, " maxdatasize=%{uint32}", psmbcmd->value.nttransact_q.maxdatasize));
    netwib_er(netwib_show_array_fmt32(pbuf, " subcmd=%{uint16}", psmbcmd->value.nttransact_q.subcmd));
    netwib_er(netwib_show_array_fmt32(pbuf, " setupcount=%{uint8}", psmbcmd->value.nttransact_q.setupcount));
    if (psmbcmd->value.nttransact_q.setupcount > NETWOX_SMBCMD_NTTRANSACT_SETUP_MAX) {
      netwib_er(netwib_show_array_fmt32(pbuf, "  -> too big"));
    } else {
      for (i = 0; i < psmbcmd->value.nttransact_q.setupcount; i++) {
        netwib_er(netwib_show_array_fmt32(pbuf, " setup[%{uint32}]=%{uint16}", i, psmbcmd->value.nttransact_q.setup[i]));
      }
    }
    netwib_er(netwox_smbcmdcmn_data_show(" para", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.nttransact_q.para, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" data", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.nttransact_q.data, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_NTTRANSACT_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " NT Transact Reply"));
    netwib_er(netwib_show_array_fmt32(pbuf, " totalparasize=%{uint32}", psmbcmd->value.nttransact_r.totalparasize));
    netwib_er(netwib_show_array_fmt32(pbuf, " totaldatasize=%{uint32}", psmbcmd->value.nttransact_r.totaldatasize));
    netwib_er(netwib_show_array_fmt32(pbuf, " paradisplacement=%{uint32}", psmbcmd->value.nttransact_r.paradisplacement));
    netwib_er(netwib_show_array_fmt32(pbuf, " datadisplacement=%{uint32}", psmbcmd->value.nttransact_r.datadisplacement));
    netwib_er(netwib_show_array_fmt32(pbuf, " setupcount=%{uint8}", psmbcmd->value.nttransact_r.setupcount));
    if (psmbcmd->value.nttransact_r.setupcount > NETWOX_SMBCMD_NTTRANSACT_SETUP_MAX) {
      netwib_er(netwib_show_array_fmt32(pbuf, "  -> too big"));
    } else {
      for (i = 0; i < psmbcmd->value.nttransact_r.setupcount; i++) {
        netwib_er(netwib_show_array_fmt32(pbuf, " setup[%{uint32}]=%{uint16}", i, psmbcmd->value.nttransact_r.setup[i]));
      }
    }
    netwib_er(netwox_smbcmdcmn_data_show(" para", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.nttransact_r.para, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" data", NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.nttransact_r.data, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_NTCREATEANDX_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " NT Create AndX Query"));
    netwib_er(netwox_smbcmdcmn_openflags32_show(" openflags", psmbcmd->value.ntcreateandx_q.openflags, pbuf));
    netwib_er(netwib_show_array_fmt32(pbuf, " rootfileid=%{uint32}", psmbcmd->value.ntcreateandx_q.rootfileid));
    netwib_er(netwox_smbcmdcmn_accessmask_show(" accessmask", psmbcmd->value.ntcreateandx_q.accessmask, pbuf));
    netwib_er(netwox_smbcmdcmn_uint64_show(" allocationsize", psmbcmd->value.ntcreateandx_q.allocationsize, pbuf));
    netwib_er(netwox_smbcmdcmn_fileattr32_show(" fileattributes", psmbcmd->value.ntcreateandx_q.fileattributes, pbuf));
    netwib_er(netwox_smbcmdcmn_shareaccess_show(" shareaccess", psmbcmd->value.ntcreateandx_q.shareaccess, pbuf));
    netwib_er(netwox_smbcmdcmn_createfunction_show(" createfunction", psmbcmd->value.ntcreateandx_q.createfunction, pbuf));
    netwib_er(netwox_smbcmdcmn_createopt_show(" createoptions", psmbcmd->value.ntcreateandx_q.createoptions, pbuf));
    netwib_er(netwox_smbcmdcmn_impersonation_show(" impersonation", psmbcmd->value.ntcreateandx_q.impersonation, pbuf));
    netwib_er(netwox_smbcmdcmn_secuflags_show(" secuflags", psmbcmd->value.ntcreateandx_q.secuflags, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" filename", NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.ntcreateandx_q.filename, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_NTCREATEANDX_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " NT Create AndX Reply"));
    netwib_er(netwox_smbcmdcmn_oplocklevel_show(" oplocklevel", psmbcmd->value.ntcreateandx_r.oplocklevel, pbuf));
    netwib_er(netwib_show_array_fmt32(pbuf, " fileid=%{uint16}", psmbcmd->value.ntcreateandx_r.fileid));
    netwib_er(netwox_smbcmdcmn_createaction_show(" createaction", psmbcmd->value.ntcreateandx_r.createaction, pbuf));
    netwib_er(netwox_smbcmdcmn_time1601_show(" creationtime", psmbcmd->value.ntcreateandx_r.creationtime, pbuf));
    netwib_er(netwox_smbcmdcmn_time1601_show(" lastaccesstime", psmbcmd->value.ntcreateandx_r.lastaccesstime, pbuf));
    netwib_er(netwox_smbcmdcmn_time1601_show(" lastwritetime", psmbcmd->value.ntcreateandx_r.lastwritetime, pbuf));
    netwib_er(netwox_smbcmdcmn_time1601_show(" changetime", psmbcmd->value.ntcreateandx_r.changetime, pbuf));
    netwib_er(netwox_smbcmdcmn_fileattr32_show(" fileattributes", psmbcmd->value.ntcreateandx_r.fileattributes, pbuf));
    netwib_er(netwox_smbcmdcmn_uint64_show(" allocationsize", psmbcmd->value.ntcreateandx_r.allocationsize, pbuf));
    netwib_er(netwox_smbcmdcmn_uint64_show(" filesize", psmbcmd->value.ntcreateandx_r.filesize, pbuf));
    netwib_er(netwox_smbcmdcmn_filetype_show(" filetype", psmbcmd->value.ntcreateandx_r.filetype, pbuf));
    netwib_er(netwib_show_array_fmt32(pbuf, " ipcstate=%{uint16}", psmbcmd->value.ntcreateandx_r.ipcstate));
    netwib_er(netwib_show_array_fmt32(pbuf, " isdirectory=%{bool}", psmbcmd->value.ntcreateandx_r.isdirectory));
    break;
  case NETWOX_SMBCMD_TYPE_NTCANCEL_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " NT Cancel Query"));
    break;
  case NETWOX_SMBCMD_TYPE_NTCANCEL_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " NT Cancel Reply"));
    break;
  case NETWOX_SMBCMD_TYPE_NTRENAME_Q :
    netwib_er(netwib_show_array_fmt32(pbuf, " NT Rename Query"));
    netwib_er(netwox_smbcmdcmn_fileattr16_show(" searchattributes", psmbcmd->value.ntrename_q.searchattributes, pbuf));
    netwib_er(netwib_show_array_fmt32(pbuf, " informationlevel=%{uint16}", psmbcmd->value.ntrename_q.informationlevel));
    netwib_er(netwib_show_array_fmt32(pbuf, " clustercount=%{uint32}", psmbcmd->value.ntrename_q.clustercount));
    netwib_er(netwox_smbcmdcmn_data_show(" oldname", NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.ntrename_q.oldname, pbuf));
    netwib_er(netwox_smbcmdcmn_data_show(" newname", NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.ntrename_q.newname, pbuf));
    break;
  case NETWOX_SMBCMD_TYPE_NTRENAME_R :
    netwib_er(netwib_show_array_fmt32(pbuf, " NT Rename Reply"));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_append_smbcmd_range(netwib_uint8 locktype,
                                                    const netwox_smbcmd_lockingandx_range *prange,
                                                    netwox_smbmsg *psmbmsg)
{
  netwib_data data;
  netwib_uint32 ui32;

  if (locktype & NETWOX_SMBCMD_LOCKINGANDX_LOCKTYPE_LARGEFILES) {
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].data,
                                   20, &data));
    netwib__data_append_uint16_le(data, prange->pid);
    netwib_c_memset(data, 0, 2); data += 2; /* reserved */
    ui32 = netwib_c2_uint64_32high(prange->offset);
    netwib__data_append_uint32_le(data, ui32);
    ui32 = netwib_c2_uint64_32low(prange->offset);
    netwib__data_append_uint32_le(data, ui32);
    ui32 = netwib_c2_uint64_32high(prange->length);
    netwib__data_append_uint32_le(data, ui32);
    ui32 = netwib_c2_uint64_32low(prange->length);
    netwib__data_append_uint32_le(data, ui32);
    psmbmsg->cmds[psmbmsg->numcmds].data.endoffset += 20;
  } else {
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].data,
                                   10, &data));
    netwib__data_append_uint16_le(data, prange->pid);
    netwib__uint32_init_uint64(prange->offset, ui32);
    netwib__data_append_uint32_le(data, ui32);
    netwib__uint32_init_uint64(prange->length, ui32);
    netwib__data_append_uint32_le(data, ui32);
    psmbmsg->cmds[psmbmsg->numcmds].data.endoffset += 10;
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbmsg_append_smbcmd(netwox_constsmbcmd *psmbcmd,
                                       netwox_smbmsg *psmbmsg)
{
  netwib_data para;
  netwib_buf buf;
  netwib_uint16 ui16;
  netwib_uint32 dataoffset, parasize, paraoffset, i;

  switch(psmbcmd->type) {
  case NETWOX_SMBCMD_TYPE_CREATEDIRECTORY_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_CREATEDIRECTORY;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.createdirectory_q.dirname));
    break;
  case NETWOX_SMBCMD_TYPE_CREATEDIRECTORY_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_CREATEDIRECTORY;
    break;
  case NETWOX_SMBCMD_TYPE_DELETEDIRECTORY_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_DELETEDIRECTORY;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.deletedirectory_q.dirname));
    break;
  case NETWOX_SMBCMD_TYPE_DELETEDIRECTORY_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_DELETEDIRECTORY;
    break;
  case NETWOX_SMBCMD_TYPE_CREATE_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_CREATE;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 6,
                                   &para));
    netwox__smbcmdcmn_fileattr16_append(para, psmbcmd->value.create_q.fileattr);
    netwox__smbcmdcmn_time1970l_append(para, psmbcmd->value.create_q.creationtime);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 6;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.create_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_CREATE_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_CREATE;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 2,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.create_r.fileid);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 2;
    break;
  case NETWOX_SMBCMD_TYPE_OPEN_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_OPEN;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 4,
                                   &para));
    netwox__smbcmdcmn_accessmode_append(para, psmbcmd->value.open_q.desiredaccess);
    netwox__smbcmdcmn_fileattr16_append(para, psmbcmd->value.open_q.searchattributes);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 4;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.open_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_OPEN_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_OPEN;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 26,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.open_r.fileid);
    netwox__smbcmdcmn_fileattr16_append(para, psmbcmd->value.open_r.fileattributes);
    netwox__smbcmdcmn_time1970l_append(para, psmbcmd->value.open_r.lastwritetime);
    netwib__data_append_uint32_le(para, psmbcmd->value.open_r.filesize);
    netwox__smbcmdcmn_accessmode_append(para, psmbcmd->value.open_r.grantedaccess);
    break;
  case NETWOX_SMBCMD_TYPE_CLOSE_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_CLOSE;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 6,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.close_q.fileid);
    netwox__smbcmdcmn_time1970l_append(para, psmbcmd->value.close_q.lastwritetime);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 6;
    break;
  case NETWOX_SMBCMD_TYPE_CLOSE_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_CLOSE;
    break;
  case NETWOX_SMBCMD_TYPE_FLUSH_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_FLUSH;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 2,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.flush_q.fileid);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 2;
    break;
  case NETWOX_SMBCMD_TYPE_FLUSH_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_FLUSH;
    break;
  case NETWOX_SMBCMD_TYPE_DELETE_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_DELETE;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 2,
                                   &para));
    netwox__smbcmdcmn_fileattr16_append(para, psmbcmd->value.delete_q.searchattributes);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 2;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.delete_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_DELETE_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_DELETE;
    break;
  case NETWOX_SMBCMD_TYPE_RENAME_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_RENAME;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 2,
                                   &para));
    netwox__smbcmdcmn_fileattr16_append(para, psmbcmd->value.rename_q.searchattributes);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 2;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.rename_q.oldname));
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.rename_q.newname));
    break;
  case NETWOX_SMBCMD_TYPE_RENAME_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_RENAME;
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATION_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_QUERYINFORMATION;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.queryinformation_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATION_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_QUERYINFORMATION;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 20,
                                   &para));
    netwox__smbcmdcmn_fileattr16_append(para, psmbcmd->value.queryinformation_r.fileattributes);
    netwox__smbcmdcmn_time1970l_append(para, psmbcmd->value.queryinformation_r.lastwritetime);
    netwib__data_append_uint32_le(para, psmbcmd->value.queryinformation_r.filesize);
    netwib_c_memset(para, 0, 10); para += 10; /* reserved */
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 20;
    break;
  case NETWOX_SMBCMD_TYPE_SETINFORMATION_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_SETINFORMATION;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 16,
                                   &para));
    netwox__smbcmdcmn_fileattr16_append(para, psmbcmd->value.setinformation_q.fileattributes);
    netwox__smbcmdcmn_time1970l_append(para, psmbcmd->value.setinformation_q.lastwritetime);
    netwib_c_memset(para, 0, 10); para += 10; /* reserved */
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 16;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.setinformation_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_SETINFORMATION_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_SETINFORMATION;
    break;
  case NETWOX_SMBCMD_TYPE_READ_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_READ;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 10,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.read_q.fileid);
    netwib__data_append_uint16_le(para, psmbcmd->value.read_q.size);
    netwib__data_append_uint32_le(para, psmbcmd->value.read_q.offset);
    netwib__data_append_uint16_le(para, psmbcmd->value.read_q.remaining);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 10;
    break;
  case NETWOX_SMBCMD_TYPE_READ_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_READ;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 10,
                                   &para));
    netwib__data_append_uint16_le(para, netwib__buf_ref_data_size(&psmbcmd->value.read_r.data));
    netwib_c_memset(para, 0, 8); para += 8; /* reserved */
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 10;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_DATABLOCK, &psmbcmd->value.read_r.data));
    break;
  case NETWOX_SMBCMD_TYPE_WRITE_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_WRITE;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 10,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.write_q.fileid);
    netwib__data_append_uint16_le(para, netwib__buf_ref_data_size(&psmbcmd->value.write_q.data));
    netwib__data_append_uint32_le(para, psmbcmd->value.write_q.offset);
    netwib__data_append_uint16_le(para, psmbcmd->value.write_q.remaining);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 10;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_DATABLOCK, &psmbcmd->value.write_q.data));
    break;
  case NETWOX_SMBCMD_TYPE_WRITE_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_WRITE;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 2,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.write_r.writtensize);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 2;
    break;
  case NETWOX_SMBCMD_TYPE_CREATETEMPORARY_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_CREATETEMPORARY;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 6,
                                   &para));
    netwox__smbcmdcmn_fileattr16_append(para, psmbcmd->value.createtemporary_q.fileattr);
    netwox__smbcmdcmn_time1970l_append(para, psmbcmd->value.createtemporary_q.creationtime);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 6;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.createtemporary_q.dirname));
    break;
  case NETWOX_SMBCMD_TYPE_CREATETEMPORARY_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_CREATETEMPORARY;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 2,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.createtemporary_r.fileid);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 2;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.createtemporary_r.filename));
    break;
  case NETWOX_SMBCMD_TYPE_CREATENEW_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_CREATENEW;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 6,
                                   &para));
    netwox__smbcmdcmn_fileattr16_append(para, psmbcmd->value.createnew_q.fileattributes);
    netwox__smbcmdcmn_time1970l_append(para, psmbcmd->value.createnew_q.creationtime);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 6;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.createnew_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_CREATENEW_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_CREATENEW;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 2,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.createnew_r.fileid);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 2;
    break;
  case NETWOX_SMBCMD_TYPE_CHECKDIRPATH_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_CHECKDIRPATH;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.checkdirpath_q.dirname));
    break;
  case NETWOX_SMBCMD_TYPE_CHECKDIRPATH_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_CHECKDIRPATH;
    break;
  case NETWOX_SMBCMD_TYPE_SEEK_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_SEEK;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 8,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.seek_q.fileid);
    netwox__smbcmdcmn_seektype_append(para, psmbcmd->value.seek_q.seektype);
    netwib__data_append_uint32_le(para, psmbcmd->value.seek_q.offset);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 8;
    break;
  case NETWOX_SMBCMD_TYPE_SEEK_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_SEEK;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 4,
                                   &para));
    netwib__data_append_uint32_le(para, psmbcmd->value.seek_r.offset);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 4;
    break;
  case NETWOX_SMBCMD_TYPE_READRAW_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_READRAW;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 20,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.readraw_q.fileid);
    netwib__data_append_uint32_le(para, netwib_c2_uint64_32low(psmbcmd->value.readraw_q.offset));
    netwib__data_append_uint16_le(para, psmbcmd->value.readraw_q.maxsize);
    netwib__data_append_uint16_le(para, psmbcmd->value.readraw_q.minsize);
    netwib__data_append_uint32_le(para, psmbcmd->value.readraw_q.timeout);
    netwib_c_memset(para, 0, 2); para += 2; /* reserved */
    if (netwib_c2_uint64_32high(psmbcmd->value.readraw_q.offset)) {
      netwib__data_append_uint32_le(para, netwib_c2_uint64_32high(psmbcmd->value.readraw_q.offset));
      psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 20;
    } else {
      psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 16;
    }
    break;
  case NETWOX_SMBCMD_TYPE_WRITERAW_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_WRITERAW;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 28,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.writeraw_q.fileid);
    netwib__data_append_uint32_le(para, psmbcmd->value.writeraw_q.rawdatasize+netwib__buf_ref_data_size(&psmbcmd->value.writeraw_q.data));
    netwib__data_append_uint32_le(para, netwib_c2_uint64_32low(psmbcmd->value.writeraw_q.offset));
    netwib__data_append_uint32_le(para, psmbcmd->value.writeraw_q.timeout);
    netwox__smbcmdcmn_accessmode_append(para, psmbcmd->value.writeraw_q.accessmode);
    netwib_c_memset(para, 0, 4); para += 4; /* reserved */
    netwib__data_append_uint16_le(para, netwib__buf_ref_data_size(&psmbcmd->value.writeraw_q.data));
    if (psmbmsg->numcmds != 0) {
      /* because of offset, only know how to encode if alone */
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_append_uint16_le(para, 60); /* data offset from begin. The min
                                                is really 59, but we prepend
                                                one byte to align. */
    if (netwib_c2_uint64_32high(psmbcmd->value.writeraw_q.offset)) {
      netwib__data_append_uint32_le(para, netwib_c2_uint64_32high(psmbcmd->value.writeraw_q.offset));
      psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 28;
    } else {
      psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 24;
    }
    netwib_er(netwib_buf_append_byte(0, /* prepend one byte to align */
                                     &psmbmsg->cmds[psmbmsg->numcmds].data));
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.writeraw_q.data));
    break;
  case NETWOX_SMBCMD_TYPE_WRITERAW_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_WRITERAW;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 2,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.writeraw_r.remaining);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 2;
    break;
  case NETWOX_SMBCMD_TYPE_SETINFORMATION2_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_SETINFORMATION2;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 14,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.setinformation2_q.fileid);
    netwox__smbcmdcmn_timemsdos_append(para, psmbcmd->value.setinformation2_q.creationtime);
    netwox__smbcmdcmn_timemsdos_append(para, psmbcmd->value.setinformation2_q.lastaccesstime);
    netwox__smbcmdcmn_timemsdos_append(para, psmbcmd->value.setinformation2_q.lastwritetime);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 14;
    break;
  case NETWOX_SMBCMD_TYPE_SETINFORMATION2_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_SETINFORMATION2;
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATION2_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_QUERYINFORMATION2;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 2,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.queryinformation2_q.fileid);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 2;
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATION2_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_QUERYINFORMATION2;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 22,
                                   &para));
    netwox__smbcmdcmn_timemsdos_append(para, psmbcmd->value.queryinformation2_r.creationtime);
    netwox__smbcmdcmn_timemsdos_append(para, psmbcmd->value.queryinformation2_r.lastaccesstime);
    netwox__smbcmdcmn_timemsdos_append(para, psmbcmd->value.queryinformation2_r.lastwritetime);
    netwib__data_append_uint32_le(para, psmbcmd->value.queryinformation2_r.filesize);
    netwib__data_append_uint32_le(para, psmbcmd->value.queryinformation2_r.allocationsize);
    netwox__smbcmdcmn_fileattr16_append(para, psmbcmd->value.queryinformation2_r.fileattributes);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 22;
    break;
  case NETWOX_SMBCMD_TYPE_LOCKINGANDX_Q :
    if (psmbcmd->value.lockingandx_q.unlockscount > NETWOX_SMBCMD_LOCKINGANDX_RANGE_MAX ||
        psmbcmd->value.lockingandx_q.lockscount > NETWOX_SMBCMD_LOCKINGANDX_RANGE_MAX) {
      return(NETWIB_ERR_PATOOHIGH);
    }
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_LOCKINGANDX;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 12,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.lockingandx_q.fileid);
    netwib__data_append_uint8_le(para, psmbcmd->value.lockingandx_q.locktype);
    netwox__smbcmdcmn_oplocklevel_append(para, psmbcmd->value.lockingandx_q.oplocklevel);
    netwib__data_append_uint32_le(para, psmbcmd->value.lockingandx_q.timeout);
    netwib__data_append_uint16_le(para, psmbcmd->value.lockingandx_q.unlockscount);
    netwib__data_append_uint16_le(para, psmbcmd->value.lockingandx_q.lockscount);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 12;
    for (i = 0; i < psmbcmd->value.lockingandx_q.unlockscount; i++) {
      netwib_er(netwox_smbmsg_append_smbcmd_range(psmbcmd->value.lockingandx_q.locktype, &psmbcmd->value.lockingandx_q.unlocks[i], psmbmsg));
    }
    for (i = 0; i < psmbcmd->value.lockingandx_q.lockscount; i++) {
      netwib_er(netwox_smbmsg_append_smbcmd_range(psmbcmd->value.lockingandx_q.locktype, &psmbcmd->value.lockingandx_q.locks[i], psmbmsg));
    }
    break;
  case NETWOX_SMBCMD_TYPE_LOCKINGANDX_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_LOCKINGANDX;
    break;
  case NETWOX_SMBCMD_TYPE_TRANSACTION_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_TRANSACTION;
#define NETWOX_SMBCMD_TRANSACTIONQ_COMMONSIZE ((netwib_uint32)28)
    if (psmbcmd->value.transaction_q.setupcount > NETWOX_SMBCMD_TRANSACTION_SETUP_MAX) {
      return(NETWIB_ERR_PATOOHIGH);
    }
    if (psmbmsg->numcmds != 0) {
      /* because of offset, only know how to decode if alone */
      return(NETWIB_ERR_NOTCONVERTED);
    }
    parasize = NETWOX_SMBCMD_TRANSACTIONQ_COMMONSIZE + 2*psmbcmd->value.transaction_q.setupcount;
    paraoffset = NETWOX_SMBMSGHDR_LEN + 1 + parasize + 2;
    if (psmbmsg->hdr.flags2 & NETWOX_SMBMSG_FLAGS2_UNICODESTRINGS) {
      paraoffset += + 1 + netwib__buf_ref_data_size(&psmbcmd->value.transaction_q.name) + 2;
    } else {
      paraoffset += netwib__buf_ref_data_size(&psmbcmd->value.transaction_q.name) + 1;
    }
    dataoffset = paraoffset + netwib__buf_ref_data_size(&psmbcmd->value.transaction_q.para);
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, parasize, &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.transaction_q.totalparasize);
    netwib__data_append_uint16_le(para, psmbcmd->value.transaction_q.totaldatasize);
    netwib__data_append_uint16_le(para, psmbcmd->value.transaction_q.maxparasize);
    netwib__data_append_uint16_le(para, psmbcmd->value.transaction_q.maxdatasize);
    netwib__data_append_uint8_le(para, psmbcmd->value.transaction_q.maxsetupcount);
    netwib_c_memset(para, 0, 1); para++; /* reserved */
    netwib__data_append_uint16_le(para, psmbcmd->value.transaction_q.flags);
    netwib__data_append_uint32_le(para, psmbcmd->value.transaction_q.timeout);
    netwib_c_memset(para, 0, 2); para += 2; /* reserved */
    netwib__data_append_uint16_le(para, netwib__buf_ref_data_size(&psmbcmd->value.transaction_q.para));
    netwib__data_append_uint16_le(para, paraoffset);
    netwib__data_append_uint16_le(para, netwib__buf_ref_data_size(&psmbcmd->value.transaction_q.data));
    netwib__data_append_uint16_le(para, dataoffset);
    netwib__data_append_uint8_le(para, psmbcmd->value.transaction_q.setupcount);
    netwib_c_memset(para, 0, 1); para++; /* reserved */
    for (i = 0; i < psmbcmd->value.transaction_q.setupcount; i++) {
      netwib__data_append_uint16_le(para, psmbcmd->value.transaction_q.setup[i]);
    }
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += parasize;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.transaction_q.name));
    netwib_er(netwib_buf_append_buf(&psmbcmd->value.transaction_q.para, &psmbmsg->cmds[psmbmsg->numcmds].data));
    netwib_er(netwib_buf_append_buf(&psmbcmd->value.transaction_q.data, &psmbmsg->cmds[psmbmsg->numcmds].data));
    break;
  case NETWOX_SMBCMD_TYPE_TRANSACTION_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_TRANSACTION;
#define NETWOX_SMBCMD_TRANSACTIONR_COMMONSIZE ((netwib_uint32)20)
    if (psmbcmd->value.transaction_r.setupcount > NETWOX_SMBCMD_TRANSACTION_SETUP_MAX) {
      return(NETWIB_ERR_PATOOHIGH);
    }
    if (psmbmsg->numcmds != 0) {
      /* because of offset, only know how to decode if alone */
      return(NETWIB_ERR_NOTCONVERTED);
    }
    parasize = NETWOX_SMBCMD_TRANSACTIONR_COMMONSIZE + 2*psmbcmd->value.transaction_r.setupcount;
    paraoffset = NETWOX_SMBMSGHDR_LEN + 1 + parasize + 2;
    paraoffset += 1; /* always padded, to align transaction_r.para
                        and transaction_r.data if unicode */
    dataoffset = paraoffset + netwib__buf_ref_data_size(&psmbcmd->value.transaction_r.para);
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, parasize, &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.transaction_r.totalparasize);
    netwib__data_append_uint16_le(para, psmbcmd->value.transaction_r.totaldatasize);
    netwib_c_memset(para, 0, 2); para += 2; /* reserved */
    netwib__data_append_uint16_le(para, netwib__buf_ref_data_size(&psmbcmd->value.transaction_r.para));
    netwib__data_append_uint16_le(para, paraoffset);
    netwib__data_append_uint16_le(para, psmbcmd->value.transaction_r.paradisplacement);
    netwib__data_append_uint16_le(para, netwib__buf_ref_data_size(&psmbcmd->value.transaction_r.data));
    netwib__data_append_uint16_le(para, dataoffset);
    netwib__data_append_uint16_le(para, psmbcmd->value.transaction_r.datadisplacement);
    netwib__data_append_uint8_le(para, psmbcmd->value.transaction_r.setupcount);
    netwib_c_memset(para, 0, 1); para++; /* reserved */
    for (i = 0; i < psmbcmd->value.transaction_r.setupcount; i++) {
      netwib__data_append_uint16_le(para, psmbcmd->value.transaction_r.setup[i]);
    }
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += parasize;
    netwib_er(netwib_buf_append_byte(0, /* always padded */
                                     &psmbmsg->cmds[psmbmsg->numcmds].data));
    netwib_er(netwib_buf_append_buf(&psmbcmd->value.transaction_r.para, &psmbmsg->cmds[psmbmsg->numcmds].data));
    netwib_er(netwib_buf_append_buf(&psmbcmd->value.transaction_r.data, &psmbmsg->cmds[psmbmsg->numcmds].data));
    break;
  case NETWOX_SMBCMD_TYPE_ECHO_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_ECHO;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 2,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.echo_q.count);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 2;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.echo_q.data));
    break;
  case NETWOX_SMBCMD_TYPE_ECHO_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_ECHO;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 2,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.echo_r.number);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 2;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.echo_r.data));
    break;
  case NETWOX_SMBCMD_TYPE_OPENANDX_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_OPENANDX;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 26,
                                   &para));
    netwox__smbcmdcmn_openflags16_append(para, psmbcmd->value.openandx_q.openflags);
    netwox__smbcmdcmn_accessmode_append(para, psmbcmd->value.openandx_q.desiredaccess);
    netwox__smbcmdcmn_fileattr16_append(para, psmbcmd->value.openandx_q.searchattributes);
    netwox__smbcmdcmn_fileattr16_append(para, psmbcmd->value.openandx_q.fileattributes);
    netwox__smbcmdcmn_time1970l_append(para, psmbcmd->value.openandx_q.creationtime);
    netwox__smbcmdcmn_openfunction_append(para, psmbcmd->value.openandx_q.openfunction);
    netwib__data_append_uint32_le(para, psmbcmd->value.openandx_q.allocationsize);
    netwib__data_append_uint32_le(para, psmbcmd->value.openandx_q.timeout);
    netwib_c_memset(para, 0, 4); para += 4; /* reserved */
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 26;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.openandx_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_OPENANDX_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_OPENANDX;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 26,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.openandx_r.fileid);
    netwox__smbcmdcmn_fileattr16_append(para, psmbcmd->value.openandx_r.fileattributes);
    netwox__smbcmdcmn_time1970l_append(para, psmbcmd->value.openandx_r.lastwritetime);
    netwib__data_append_uint32_le(para, psmbcmd->value.openandx_r.filesize);
    netwox__smbcmdcmn_accessmode_append(para, psmbcmd->value.openandx_r.grantedaccess);
    netwox__smbcmdcmn_filetype_append(para, psmbcmd->value.openandx_r.filetype);
    netwib__data_append_uint16_le(para, psmbcmd->value.openandx_r.ipcstate);
    netwox__smbcmdcmn_createaction_append(para, psmbcmd->value.openandx_r.action);
    netwib__data_append_uint32_le(para, psmbcmd->value.openandx_r.serverfileid);
    netwib_c_memset(para, 0, 2); para += 2; /* reserved */
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 26;
    break;
  case NETWOX_SMBCMD_TYPE_READANDX_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_READANDX;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 20,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.readandx_q.fileid);
    netwib__data_append_uint32_le(para, netwib_c2_uint64_32low(psmbcmd->value.readandx_q.offset));
    netwib__data_append_uint16_le(para, psmbcmd->value.readandx_q.maxsize);
    netwib__data_append_uint16_le(para, psmbcmd->value.readandx_q.minsize);
    netwib__data_append_uint32_le(para, psmbcmd->value.readandx_q.timeout);
    netwib__data_append_uint16_le(para, psmbcmd->value.readandx_q.remaining);
    if (netwib_c2_uint64_32high(psmbcmd->value.readandx_q.offset)) {
      netwib__data_append_uint32_le(para, netwib_c2_uint64_32high(psmbcmd->value.readandx_q.offset));
      psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 20;
    } else {
      psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 16;
    }
    break;
  case NETWOX_SMBCMD_TYPE_READANDX_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_READANDX;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 20,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.readandx_r.remaining);
    netwib__data_append_uint16_le(para, psmbcmd->value.readandx_r.datacompactionmode);
    netwib_c_memset(para, 0, 2); para += 2; /* reserved */
    netwib__data_append_uint16_le(para, netwib__buf_ref_data_size(&psmbcmd->value.readandx_r.data));
    /* compute offset (only first packet in AndX is supported) */
    if (psmbmsg->numcmds != 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    dataoffset = NETWOX_SMBMSGHDR_LEN + 1 + NETWOX_SMBMSGANDX_LEN + 20 +2;
    dataoffset++; /* ensure data will be aligned: add 1 padding byte */
    netwib__data_append_uint16_le(para, dataoffset);
    netwib_c_memset(para, 0, 10); para += 10; /* reserved */
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 20;
    /* we always add one padding byte */
    netwib_er(netwib_buf_append_byte(0, &psmbmsg->cmds[psmbmsg->numcmds].data));
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.readandx_r.data));
    break;
  case NETWOX_SMBCMD_TYPE_WRITEANDX_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_WRITEANDX;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 24,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.writeandx_q.fileid);
    netwib__data_append_uint32_le(para, netwib_c2_uint64_32low(psmbcmd->value.writeandx_q.offset));
    netwib__data_append_uint32_le(para, psmbcmd->value.writeandx_q.timeout);
    netwib__data_append_uint16_le(para, psmbcmd->value.writeandx_q.writemode);
    netwib__data_append_uint16_le(para, psmbcmd->value.writeandx_q.remaining);
    netwib_c_memset(para, 0, 2); para += 2; /* reserved */
    netwib__data_append_uint16_le(para, netwib__buf_ref_data_size(&psmbcmd->value.writeandx_q.data));
    /* compute offset (only first packet in AndX is supported) */
    if (psmbmsg->numcmds != 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    dataoffset = NETWOX_SMBMSGHDR_LEN + 1 + NETWOX_SMBMSGANDX_LEN + 20 +2;
    if (netwib_c2_uint64_32high(psmbcmd->value.writeandx_q.offset)) {
      dataoffset += 4;
    }
    dataoffset++; /* ensure data will be aligned: add 1 padding byte */
    netwib__data_append_uint16_le(para, dataoffset);
    if (netwib_c2_uint64_32high(psmbcmd->value.writeandx_q.offset)) {
      netwib__data_append_uint32_le(para, netwib_c2_uint64_32high(psmbcmd->value.writeandx_q.offset));
      psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 24;
    } else {
      psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 20;
    }
    /* we always add one padding byte */
    netwib_er(netwib_buf_append_byte(0, &psmbmsg->cmds[psmbmsg->numcmds].data));
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.writeandx_q.data));
    break;
  case NETWOX_SMBCMD_TYPE_WRITEANDX_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_WRITEANDX;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 8,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.writeandx_r.writtensize);
    netwib__data_append_uint16_le(para, psmbcmd->value.writeandx_r.remaining);
    netwib_c_memset(para, 0, 4); para += 4; /* reserved */
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 8;
    break;
  case NETWOX_SMBCMD_TYPE_CLOSETREEDISCONNECT_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_CLOSETREEDISCONNECT;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 6,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.closetreedisconnect_q.fileid);
    netwox__smbcmdcmn_time1970l_append(para, psmbcmd->value.closetreedisconnect_q.lastwritetime);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 6;
    break;
  case NETWOX_SMBCMD_TYPE_CLOSETREEDISCONNECT_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_CLOSETREEDISCONNECT;
    break;
  case NETWOX_SMBCMD_TYPE_TRANSACTION2_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_TRANSACTION2;
#define NETWOX_SMBCMD_TRANSACTION2Q_COMMONSIZE ((netwib_uint32)28)
    if (psmbcmd->value.transaction2_q.setupcount > NETWOX_SMBCMD_TRANSACTION2_SETUP_MAX) {
      return(NETWIB_ERR_PATOOHIGH);
    }
    if (psmbmsg->numcmds != 0) {
      /* because of offset, only know how to decode if alone */
      return(NETWIB_ERR_NOTCONVERTED);
    }
    parasize = NETWOX_SMBCMD_TRANSACTION2Q_COMMONSIZE + 2*psmbcmd->value.transaction2_q.setupcount;
    paraoffset = NETWOX_SMBMSGHDR_LEN + 1 + parasize + 2;
    paraoffset += 1; /* always padded, to align transaction2_q.para
                        and transaction2_q.data if unicode */
    dataoffset = paraoffset + netwib__buf_ref_data_size(&psmbcmd->value.transaction2_q.para);
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, parasize, &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.transaction2_q.totalparasize);
    netwib__data_append_uint16_le(para, psmbcmd->value.transaction2_q.totaldatasize);
    netwib__data_append_uint16_le(para, psmbcmd->value.transaction2_q.maxparasize);
    netwib__data_append_uint16_le(para, psmbcmd->value.transaction2_q.maxdatasize);
    netwib__data_append_uint8_le(para, psmbcmd->value.transaction2_q.maxsetupcount);
    netwib_c_memset(para, 0, 1); para++; /* reserved */
    netwib__data_append_uint16_le(para, psmbcmd->value.transaction2_q.flags);
    netwib__data_append_uint32_le(para, psmbcmd->value.transaction2_q.timeout);
    netwib_c_memset(para, 0, 2); para += 2; /* reserved */
    netwib__data_append_uint16_le(para, netwib__buf_ref_data_size(&psmbcmd->value.transaction2_q.para));
    netwib__data_append_uint16_le(para, paraoffset);
    netwib__data_append_uint16_le(para, netwib__buf_ref_data_size(&psmbcmd->value.transaction2_q.data));
    netwib__data_append_uint16_le(para, dataoffset);
    netwib__data_append_uint8_le(para, psmbcmd->value.transaction2_q.setupcount);
    netwib_c_memset(para, 0, 1); para++; /* reserved */
    for (i = 0; i < psmbcmd->value.transaction2_q.setupcount; i++) {
      netwib__data_append_uint16_le(para, psmbcmd->value.transaction2_q.setup[i]);
    }
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += parasize;
    netwib_er(netwib_buf_append_byte(0, /* always padded */
                                     &psmbmsg->cmds[psmbmsg->numcmds].data));
    netwib_er(netwib_buf_append_buf(&psmbcmd->value.transaction2_q.para, &psmbmsg->cmds[psmbmsg->numcmds].data));
    netwib_er(netwib_buf_append_buf(&psmbcmd->value.transaction2_q.data, &psmbmsg->cmds[psmbmsg->numcmds].data));
    break;
  case NETWOX_SMBCMD_TYPE_TRANSACTION2_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_TRANSACTION2;
#define NETWOX_SMBCMD_TRANSACTION2R_COMMONSIZE ((netwib_uint32)20)
    if (psmbcmd->value.transaction2_r.setupcount > NETWOX_SMBCMD_TRANSACTION2_SETUP_MAX) {
      return(NETWIB_ERR_PATOOHIGH);
    }
    if (psmbmsg->numcmds != 0) {
      /* because of offset, only know how to decode if alone */
      return(NETWIB_ERR_NOTCONVERTED);
    }
    parasize = NETWOX_SMBCMD_TRANSACTION2R_COMMONSIZE + 2*psmbcmd->value.transaction2_r.setupcount;
    paraoffset = NETWOX_SMBMSGHDR_LEN + 1 + parasize + 2;
    paraoffset += 1; /* always padded, to align transaction2_r.para
                        and transaction2_r.data if unicode */
    dataoffset = paraoffset + netwib__buf_ref_data_size(&psmbcmd->value.transaction2_r.para);
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, parasize, &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.transaction2_r.totalparasize);
    netwib__data_append_uint16_le(para, psmbcmd->value.transaction2_r.totaldatasize);
    netwib_c_memset(para, 0, 2); para += 2; /* reserved */
    netwib__data_append_uint16_le(para, netwib__buf_ref_data_size(&psmbcmd->value.transaction2_r.para));
    netwib__data_append_uint16_le(para, paraoffset);
    netwib__data_append_uint16_le(para, psmbcmd->value.transaction2_r.paradisplacement);
    netwib__data_append_uint16_le(para, netwib__buf_ref_data_size(&psmbcmd->value.transaction2_r.data));
    netwib__data_append_uint16_le(para, dataoffset);
    netwib__data_append_uint16_le(para, psmbcmd->value.transaction2_r.datadisplacement);
    netwib__data_append_uint8_le(para, psmbcmd->value.transaction2_r.setupcount);
    netwib_c_memset(para, 0, 1); para++; /* reserved */
    for (i = 0; i < psmbcmd->value.transaction2_r.setupcount; i++) {
      netwib__data_append_uint16_le(para, psmbcmd->value.transaction2_r.setup[i]);
    }
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += parasize;
    netwib_er(netwib_buf_append_byte(0, /* always padded */
                                     &psmbmsg->cmds[psmbmsg->numcmds].data));
    netwib_er(netwib_buf_append_buf(&psmbcmd->value.transaction2_r.para, &psmbmsg->cmds[psmbmsg->numcmds].data));
    netwib_er(netwib_buf_append_buf(&psmbcmd->value.transaction2_r.data, &psmbmsg->cmds[psmbmsg->numcmds].data));
    break;
  case NETWOX_SMBCMD_TYPE_FINDCLOSE2_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_FINDCLOSE2;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 2,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.findclose2_q.sid);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 2;
    break;
  case NETWOX_SMBCMD_TYPE_FINDCLOSE2_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_FINDCLOSE2;
    break;
  case NETWOX_SMBCMD_TYPE_TREEDISCONNECT_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_TREEDISCONNECT;
    break;
  case NETWOX_SMBCMD_TYPE_TREEDISCONNECT_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_TREEDISCONNECT;
    break;
  case NETWOX_SMBCMD_TYPE_NEGOTIATE_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_NEGOTIATE;
    /* only support this dialect */
    netwib_er(netwib_buf_init_ext_string(NETWOX_SMBCMD_NEGOTIATE_DIALECT_NTLM012,
                                       &buf));
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_DIALECT, &buf));
    break;
  case NETWOX_SMBCMD_TYPE_NEGOTIATE_R0 :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_NEGOTIATE;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 34,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.negotiate_r0.dialectindex);
    netwib__data_append_uint8_le(para, psmbcmd->value.negotiate_r0.secumode);
    netwib__data_append_uint16_le(para, psmbcmd->value.negotiate_r0.maxmultiplex);
    netwib__data_append_uint16_le(para, psmbcmd->value.negotiate_r0.maxvc);
    netwib__data_append_uint32_le(para, psmbcmd->value.negotiate_r0.maxmsgsize);
    netwib__data_append_uint32_le(para, psmbcmd->value.negotiate_r0.maxrawsize);
    netwib__data_append_uint32_le(para, psmbcmd->value.negotiate_r0.vcid);
    netwox__smbcmdcmn_negocapa_append(para, psmbcmd->value.negotiate_r0.capabilities);
    netwox__smbcmdcmn_time1601_append(para, psmbcmd->value.negotiate_r0.systemtime);
    ui16 = (netwib_uint16)psmbcmd->value.negotiate_r0.timezoneoffset;
    netwib__data_append_uint16_le(para, ui16);
    netwib__data_append_uint8_le(para, netwib__buf_ref_data_size(&psmbcmd->value.negotiate_r0.challenge));
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 34;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.negotiate_r0.challenge));
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_UNINOPAD, &psmbcmd->value.negotiate_r0.domain));
    if (psmbcmd->value.negotiate_r0.i1set) {
      netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_UNINOPAD, &psmbcmd->value.negotiate_r0.i1.server));
    }
    break;
  case NETWOX_SMBCMD_TYPE_NEGOTIATE_R1 :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_NEGOTIATE;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 34,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.negotiate_r1.dialectindex);
    netwib__data_append_uint8_le(para, psmbcmd->value.negotiate_r1.secumode);
    netwib__data_append_uint16_le(para, psmbcmd->value.negotiate_r1.maxmultiplex);
    netwib__data_append_uint16_le(para, psmbcmd->value.negotiate_r1.maxvc);
    netwib__data_append_uint32_le(para, psmbcmd->value.negotiate_r1.maxmsgsize);
    netwib__data_append_uint32_le(para, psmbcmd->value.negotiate_r1.maxrawsize);
    netwib__data_append_uint32_le(para, psmbcmd->value.negotiate_r1.vcid);
    netwox__smbcmdcmn_negocapa_append(para, psmbcmd->value.negotiate_r1.capabilities);
    netwox__smbcmdcmn_time1601_append(para, psmbcmd->value.negotiate_r1.systemtime);
    ui16 = (netwib_uint16)psmbcmd->value.negotiate_r1.timezoneoffset;
    netwib__data_append_uint16_le(para, ui16);
    netwib_c_memset(para, 0, 1); para += 1; /* reserved */
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 34;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.negotiate_r1.serverguid));
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.negotiate_r1.securityblob));
    break;
  case NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_Q0 :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_SESSIONSETUPANDX;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 22,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.sessionsetupandx_q0.maxmsgsize);
    netwib__data_append_uint16_le(para, psmbcmd->value.sessionsetupandx_q0.maxmultiplex);
    netwib__data_append_uint16_le(para, psmbcmd->value.sessionsetupandx_q0.vcnumber);
    netwib__data_append_uint32_le(para, psmbcmd->value.sessionsetupandx_q0.vcid);
    netwib__data_append_uint16_le(para, netwib__buf_ref_data_size(&psmbcmd->value.sessionsetupandx_q0.password));
    netwib__data_append_uint16_le(para, netwib__buf_ref_data_size(&psmbcmd->value.sessionsetupandx_q0.unicodepassword));
    netwib_c_memset(para, 0, 4); para += 4; /* reserved */
    netwox__smbcmdcmn_negocapa_append(para, psmbcmd->value.sessionsetupandx_q0.capabilities);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 22;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.sessionsetupandx_q0.password));
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.sessionsetupandx_q0.unicodepassword));
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_q0.user));
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_q0.domain));
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_q0.nativeos));
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_q0.nativelanman));
    if (psmbcmd->value.sessionsetupandx_q0.i1set) {
      netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_q0.i1.unknown));
    }
    break;
  case NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_Q1 :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_SESSIONSETUPANDX;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 20,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.sessionsetupandx_q1.maxmsgsize);
    netwib__data_append_uint16_le(para, psmbcmd->value.sessionsetupandx_q1.maxmultiplex);
    netwib__data_append_uint16_le(para, psmbcmd->value.sessionsetupandx_q1.vcnumber);
    netwib__data_append_uint32_le(para, psmbcmd->value.sessionsetupandx_q1.vcid);
    netwib__data_append_uint16_le(para, netwib__buf_ref_data_size(&psmbcmd->value.sessionsetupandx_q1.secublob));
    netwib_c_memset(para, 0, 4); para += 4; /* reserved */
    netwox__smbcmdcmn_negocapa_append(para, psmbcmd->value.sessionsetupandx_q1.capabilities);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 20;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.sessionsetupandx_q1.secublob));
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_q1.nativeos));
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_q1.nativelanman));
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_q1.domain));
    break;
  case NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_R0 :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_SESSIONSETUPANDX;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 2,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.sessionsetupandx_r0.action);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 2;
    if (psmbcmd->value.sessionsetupandx_r0.i1set) {
      netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_r0.i1.nativeos));
      netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_r0.i1.nativelanman));
      if (psmbcmd->value.sessionsetupandx_r0.i1.i2set) {
        netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_r0.i1.i2.domain));
      }
    }
    break;
  case NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_R1 :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_SESSIONSETUPANDX;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 4,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.sessionsetupandx_r1.action);
    netwib__data_append_uint16_le(para, netwib__buf_ref_data_size(&psmbcmd->value.sessionsetupandx_r1.secublob));
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 4;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.sessionsetupandx_r1.secublob));
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_r1.nativeos));
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_r1.nativelanman));
    if (psmbcmd->value.sessionsetupandx_r1.i2set) {
      netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.sessionsetupandx_r1.i2.domain));
    }
    break;
  case NETWOX_SMBCMD_TYPE_LOGOFFANDX_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_LOGOFFANDX;
    break;
  case NETWOX_SMBCMD_TYPE_LOGOFFANDX_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_LOGOFFANDX;
    break;
  case NETWOX_SMBCMD_TYPE_TREECONNECTANDX_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_TREECONNECTANDX;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 4,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.treeconnectandx_q.flags);
    netwib__data_append_uint16_le(para, netwib__buf_ref_data_size(&psmbcmd->value.treeconnectandx_q.password));
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 4;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_BUFEXTSIZE, &psmbcmd->value.treeconnectandx_q.password));
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.treeconnectandx_q.path));
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI, &psmbcmd->value.treeconnectandx_q.service));
    break;
  case NETWOX_SMBCMD_TYPE_TREECONNECTANDX_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_TREECONNECTANDX;
    if (psmbcmd->value.treeconnectandx_r.i1set) {
      netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 10,
                                     &para));
      netwib__data_append_uint16_le(para, psmbcmd->value.treeconnectandx_r.i1.optionalsupport);
      if (psmbcmd->value.treeconnectandx_r.i1.i2set) {
        netwib__data_append_uint32_le(para, psmbcmd->value.treeconnectandx_r.i1.i2.ui32a);
        netwib__data_append_uint32_le(para, psmbcmd->value.treeconnectandx_r.i1.i2.ui32b);
        psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 10;
      } else {
        psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 2;
      }
    }
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI, &psmbcmd->value.treeconnectandx_r.service));
    if (psmbcmd->value.treeconnectandx_r.i1set) {
      netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.treeconnectandx_r.i1.nativefs));
    }
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATIONDISK_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_QUERYINFORMATIONDISK;
    break;
  case NETWOX_SMBCMD_TYPE_QUERYINFORMATIONDISK_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_QUERYINFORMATIONDISK;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 10,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.queryinformationdisk_r.totalunits);
    netwib__data_append_uint16_le(para, psmbcmd->value.queryinformationdisk_r.sectorsperunit);
    netwib__data_append_uint16_le(para, psmbcmd->value.queryinformationdisk_r.bytespersector);
    netwib__data_append_uint16_le(para, psmbcmd->value.queryinformationdisk_r.freeunits);
    netwib_c_memset(para, 0, 2); para += 2; /* reserved bytes */
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 10;
    break;
  case NETWOX_SMBCMD_TYPE_SEARCH_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_SEARCH;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 4,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.search_q.maxentries);
    netwox__smbcmdcmn_fileattr16_append(para, psmbcmd->value.search_q.searchattributes);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 4;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.search_q.searchpattern));
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_VARIABLEBLOCK, &psmbcmd->value.search_q.resumekey));
    break;
  case NETWOX_SMBCMD_TYPE_SEARCH_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_SEARCH;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 2,
                                   &para));
    netwib__data_append_uint16_le(para, psmbcmd->value.search_r.count);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 2;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_VARIABLEBLOCK, &psmbcmd->value.search_r.entries));
    break;
  case NETWOX_SMBCMD_TYPE_NTTRANSACT_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_NTTRANSACT;
#define NETWOX_SMBCMD_NTTRANSACTQ_COMMONSIZE ((netwib_uint32)38)
    if (psmbcmd->value.nttransact_q.setupcount > NETWOX_SMBCMD_NTTRANSACT_SETUP_MAX) {
      return(NETWIB_ERR_PATOOHIGH);
    }
    if (psmbmsg->numcmds != 0) {
      /* because of offset, only know how to decode if alone */
      return(NETWIB_ERR_NOTCONVERTED);
    }
    parasize = NETWOX_SMBCMD_NTTRANSACTQ_COMMONSIZE + 2*psmbcmd->value.nttransact_q.setupcount;
    paraoffset = NETWOX_SMBMSGHDR_LEN + 1 + parasize + 2;
    paraoffset += 1; /* always padded, to align nttransact_q.para
                        and nttransact_q.data if unicode */
    dataoffset = paraoffset + netwib__buf_ref_data_size(&psmbcmd->value.nttransact_q.para);
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, parasize, &para));
    netwib__data_append_uint8_le(para, psmbcmd->value.nttransact_q.maxsetupcount);
    netwib_c_memset(para, 0, 2); para += 2; /* reserved */
    netwib__data_append_uint32_le(para, psmbcmd->value.nttransact_q.totalparasize);
    netwib__data_append_uint32_le(para, psmbcmd->value.nttransact_q.totaldatasize);
    netwib__data_append_uint32_le(para, psmbcmd->value.nttransact_q.maxparasize);
    netwib__data_append_uint32_le(para, psmbcmd->value.nttransact_q.maxdatasize);
    netwib__data_append_uint32_le(para, netwib__buf_ref_data_size(&psmbcmd->value.nttransact_q.para));
    netwib__data_append_uint32_le(para, paraoffset);
    netwib__data_append_uint32_le(para, netwib__buf_ref_data_size(&psmbcmd->value.nttransact_q.data));
    netwib__data_append_uint32_le(para, dataoffset);
    netwib__data_append_uint8_le(para, psmbcmd->value.nttransact_q.setupcount);
    netwib__data_append_uint16_le(para, psmbcmd->value.nttransact_q.subcmd);
    for (i = 0; i < psmbcmd->value.nttransact_q.setupcount; i++) {
      netwib__data_append_uint16_le(para, psmbcmd->value.nttransact_q.setup[i]);
    }
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += parasize;
    netwib_er(netwib_buf_append_byte(0, /* always padded */
                                     &psmbmsg->cmds[psmbmsg->numcmds].data));
    netwib_er(netwib_buf_append_buf(&psmbcmd->value.nttransact_q.para, &psmbmsg->cmds[psmbmsg->numcmds].data));
    netwib_er(netwib_buf_append_buf(&psmbcmd->value.nttransact_q.data, &psmbmsg->cmds[psmbmsg->numcmds].data));
    break;
  case NETWOX_SMBCMD_TYPE_NTTRANSACT_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_NTTRANSACT;
#define NETWOX_SMBCMD_NTTRANSACTR_COMMONSIZE ((netwib_uint32)36)
    if (psmbcmd->value.nttransact_r.setupcount > NETWOX_SMBCMD_NTTRANSACT_SETUP_MAX) {
      return(NETWIB_ERR_PATOOHIGH);
    }
    if (psmbmsg->numcmds != 0) {
      /* because of offset, only know how to decode if alone */
      return(NETWIB_ERR_NOTCONVERTED);
    }
    parasize = NETWOX_SMBCMD_NTTRANSACTR_COMMONSIZE + 2*psmbcmd->value.nttransact_r.setupcount;
    paraoffset = NETWOX_SMBMSGHDR_LEN + 1 + parasize + 2;
    paraoffset += 1; /* always padded, to align nttransact_r.para
                        and nttransact_r.data if unicode */
    dataoffset = paraoffset + netwib__buf_ref_data_size(&psmbcmd->value.nttransact_r.para);
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, parasize, &para));
    netwib_c_memset(para, 0, 3); para += 3; /* reserved */
    netwib__data_append_uint32_le(para, psmbcmd->value.nttransact_r.totalparasize);
    netwib__data_append_uint32_le(para, psmbcmd->value.nttransact_r.totaldatasize);
    netwib__data_append_uint32_le(para, netwib__buf_ref_data_size(&psmbcmd->value.nttransact_r.para));
    netwib__data_append_uint32_le(para, paraoffset);
    netwib__data_append_uint32_le(para, psmbcmd->value.nttransact_r.paradisplacement);
    netwib__data_append_uint32_le(para, netwib__buf_ref_data_size(&psmbcmd->value.nttransact_r.data));
    netwib__data_append_uint32_le(para, dataoffset);
    netwib__data_append_uint32_le(para, psmbcmd->value.nttransact_r.datadisplacement);
    netwib__data_append_uint8_le(para, psmbcmd->value.nttransact_r.setupcount);
    for (i = 0; i < psmbcmd->value.nttransact_r.setupcount; i++) {
      netwib__data_append_uint16_le(para, psmbcmd->value.nttransact_r.setup[i]);
    }
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += parasize;
    netwib_er(netwib_buf_append_byte(0, /* always padded */
                                     &psmbmsg->cmds[psmbmsg->numcmds].data));
    netwib_er(netwib_buf_append_buf(&psmbcmd->value.nttransact_r.para, &psmbmsg->cmds[psmbmsg->numcmds].data));
    netwib_er(netwib_buf_append_buf(&psmbcmd->value.nttransact_r.data, &psmbmsg->cmds[psmbmsg->numcmds].data));
    break;
  case NETWOX_SMBCMD_TYPE_NTCREATEANDX_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_NTCREATEANDX;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 44,
                                   &para));
    netwib_c_memset(para, 0, 1); para++; /* reserved byte */
    netwib__data_append_uint16_le(para, netwib__buf_ref_data_size(&psmbcmd->value.ntcreateandx_q.filename));
    netwox__smbcmdcmn_openflags32_append(para, psmbcmd->value.ntcreateandx_q.openflags);
    netwib__data_append_uint32_le(para, psmbcmd->value.ntcreateandx_q.rootfileid);
    netwox__smbcmdcmn_accessmask_append(para, psmbcmd->value.ntcreateandx_q.accessmask);
    netwox__smbcmdcmn_uint64_append(para, psmbcmd->value.ntcreateandx_q.allocationsize);
    netwox__smbcmdcmn_fileattr32_append(para, psmbcmd->value.ntcreateandx_q.fileattributes);
    netwox__smbcmdcmn_shareaccess_append(para, psmbcmd->value.ntcreateandx_q.shareaccess);
    netwox__smbcmdcmn_createfunction_append(para, psmbcmd->value.ntcreateandx_q.createfunction);
    netwox__smbcmdcmn_createopt_append(para, psmbcmd->value.ntcreateandx_q.createoptions);
    netwib__data_append_uint32_le(para, psmbcmd->value.ntcreateandx_q.impersonation);
    netwox__smbcmdcmn_secuflags_append(para, psmbcmd->value.ntcreateandx_q.secuflags);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 44;
    /* should not use STRINGONLY, but see decoding */
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &psmbcmd->value.ntcreateandx_q.filename));
    break;
  case NETWOX_SMBCMD_TYPE_NTCREATEANDX_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_NTCREATEANDX;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 64,
                                   &para));
    netwox__smbcmdcmn_oplocklevel_append(para, psmbcmd->value.ntcreateandx_r.oplocklevel);
    netwib__data_append_uint16_le(para, psmbcmd->value.ntcreateandx_r.fileid);
    netwox__smbcmdcmn_createaction_append(para, psmbcmd->value.ntcreateandx_r.createaction);
    netwox__smbcmdcmn_time1601_append(para, psmbcmd->value.ntcreateandx_r.creationtime);
    netwox__smbcmdcmn_time1601_append(para, psmbcmd->value.ntcreateandx_r.lastaccesstime);
    netwox__smbcmdcmn_time1601_append(para, psmbcmd->value.ntcreateandx_r.lastwritetime);
    netwox__smbcmdcmn_time1601_append(para, psmbcmd->value.ntcreateandx_r.changetime);
    netwox__smbcmdcmn_fileattr32_append(para, psmbcmd->value.ntcreateandx_r.fileattributes);
    netwox__smbcmdcmn_uint64_append(para, psmbcmd->value.ntcreateandx_r.allocationsize);
    netwox__smbcmdcmn_uint64_append(para, psmbcmd->value.ntcreateandx_r.filesize);
    netwox__smbcmdcmn_filetype_append(para, psmbcmd->value.ntcreateandx_r.filetype);
    netwib__data_append_uint16_le(para, psmbcmd->value.ntcreateandx_r.ipcstate);
    netwib__data_append_uint8_le(para, psmbcmd->value.ntcreateandx_r.isdirectory);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 64;
    break;
  case NETWOX_SMBCMD_TYPE_NTCANCEL_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_NTCANCEL;
    break;
  case NETWOX_SMBCMD_TYPE_NTCANCEL_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_NTCANCEL;
    break;
  case NETWOX_SMBCMD_TYPE_NTRENAME_Q :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_NTRENAME;
    netwib_er(netwib_buf_wantspace(&psmbmsg->cmds[psmbmsg->numcmds].para, 8,
                                   &para));
    netwox__smbcmdcmn_fileattr16_append(para, psmbcmd->value.ntrename_q.searchattributes);
    netwib__data_append_uint16_le(para, psmbcmd->value.ntrename_q.informationlevel);
    netwib__data_append_uint32_le(para, psmbcmd->value.ntrename_q.clustercount);
    psmbmsg->cmds[psmbmsg->numcmds].para.endoffset += 8;
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.ntrename_q.oldname));
    netwib_er(netwox_smbcmdcmn_data_append(psmbmsg, psmbmsg->numcmds, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &psmbcmd->value.ntrename_q.newname));
    break;
  case NETWOX_SMBCMD_TYPE_NTRENAME_R :
    psmbmsg->cmds[psmbmsg->numcmds].cmd = NETWOX_SMBMSG_CMD_NTRENAME;
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  psmbmsg->numcmds++;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_createdirectory(netwox_constsmbmsg *psmbmsg,
                                                       netwib_uint32 idxcmd,
                                                       netwox_smbcmd *psmbcmd)
{
  netwib_data data;
  netwib_uint32 parasize, datasize;

  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_CREATEDIRECTORY_R));
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_CREATEDIRECTORY_Q));
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &data, &datasize, &psmbcmd->value.createdirectory_q.dirname));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_deletedirectory(netwox_constsmbmsg *psmbmsg,
                                                       netwib_uint32 idxcmd,
                                                       netwox_smbcmd *psmbcmd)
{
  netwib_data data;
  netwib_uint32 parasize, datasize;

  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_DELETEDIRECTORY_R));
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_DELETEDIRECTORY_Q));
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &data, &datasize, &psmbcmd->value.deletedirectory_q.dirname));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_create(netwox_constsmbmsg *psmbmsg,
                                                     netwib_uint32 idxcmd,
                                                     netwox_smbcmd *psmbcmd)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_CREATE_R));
    if (parasize != 2) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.create_r.fileid);
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_CREATE_Q));
    if (parasize != 6) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwox__smbcmdcmn_fileattr16_decode(para, psmbcmd->value.create_q.fileattr);
    netwox__smbcmdcmn_time1970l_decode(para, psmbcmd->value.create_q.creationtime);
    netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &data, &datasize, &psmbcmd->value.create_q.filename));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_open(netwox_constsmbmsg *psmbmsg,
                                                   netwib_uint32 idxcmd,
                                                   netwox_smbcmd *psmbcmd)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_OPEN_R));
    /* I didn't have such packet, so this decoding is a guess. It might
       be wrong. */
    if (parasize != 14) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.open_r.fileid);
    netwox__smbcmdcmn_fileattr16_decode(para, psmbcmd->value.open_r.fileattributes);
    netwox__smbcmdcmn_time1970l_decode(para, psmbcmd->value.open_r.lastwritetime);
    netwib__data_decode_uint32_le(para, psmbcmd->value.open_r.filesize);
    netwox__smbcmdcmn_accessmode_decode(para, psmbcmd->value.open_r.grantedaccess);
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_OPEN_Q));
    if (parasize != 4) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwox__smbcmdcmn_accessmode_decode(para, psmbcmd->value.open_q.desiredaccess);
    netwox__smbcmdcmn_fileattr16_decode(para, psmbcmd->value.open_q.searchattributes);
    netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &data, &datasize, &psmbcmd->value.open_q.filename));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_close(netwox_constsmbmsg *psmbmsg,
                                                    netwib_uint32 idxcmd,
                                                    netwox_smbcmd *psmbcmd)
{
  netwib_data para;
  netwib_uint32 parasize, datasize;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_CLOSE_R));
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_CLOSE_Q));
    if (parasize != 6) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.close_q.fileid);
    netwox__smbcmdcmn_time1970l_decode(para, psmbcmd->value.close_q.lastwritetime);
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_flush(netwox_constsmbmsg *psmbmsg,
                                                    netwib_uint32 idxcmd,
                                                    netwox_smbcmd *psmbcmd)
{
  netwib_data para;
  netwib_uint32 parasize, datasize;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_FLUSH_R));
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_FLUSH_Q));
    if (parasize != 2) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.flush_q.fileid);
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_delete(netwox_constsmbmsg *psmbmsg,
                                                       netwib_uint32 idxcmd,
                                                       netwox_smbcmd *psmbcmd)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_DELETE_R));
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_DELETE_Q));
    if (parasize != 2) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwox__smbcmdcmn_fileattr16_decode(para, psmbcmd->value.delete_q.searchattributes);
    netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &data, &datasize, &psmbcmd->value.delete_q.filename));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_rename(netwox_constsmbmsg *psmbmsg,
                                                       netwib_uint32 idxcmd,
                                                       netwox_smbcmd *psmbcmd)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_RENAME_R));
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_RENAME_Q));
    if (parasize != 2) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwox__smbcmdcmn_fileattr16_decode(para, psmbcmd->value.rename_q.searchattributes);
    netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &data, &datasize, &psmbcmd->value.rename_q.oldname));
    netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &data, &datasize, &psmbcmd->value.rename_q.newname));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_queryinformation(netwox_constsmbmsg *psmbmsg,
                                                       netwib_uint32 idxcmd,
                                                       netwox_smbcmd *psmbcmd)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_QUERYINFORMATION_R));
    if (parasize != 20) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwox__smbcmdcmn_fileattr16_decode(para, psmbcmd->value.queryinformation_r.fileattributes);
    netwox__smbcmdcmn_time1970l_decode(para, psmbcmd->value.queryinformation_r.lastwritetime);
    netwib__data_decode_uint32_le(para, psmbcmd->value.queryinformation_r.filesize);
    para += 10; /* reserved */
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_QUERYINFORMATION_Q));
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &data, &datasize, &psmbcmd->value.queryinformation_q.filename));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_setinformation(netwox_constsmbmsg *psmbmsg,
                                                       netwib_uint32 idxcmd,
                                                       netwox_smbcmd *psmbcmd)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_SETINFORMATION_R));
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_SETINFORMATION_Q));
    if (parasize != 16) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwox__smbcmdcmn_fileattr16_decode(para, psmbcmd->value.setinformation_q.fileattributes);
    netwox__smbcmdcmn_time1970l_decode(para, psmbcmd->value.setinformation_q.lastwritetime);
    para += 10; /* reserved */
    netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &data, &datasize, &psmbcmd->value.setinformation_q.filename));
    if (datasize && datasize != 4) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_read(netwox_constsmbmsg *psmbmsg,
                                                       netwib_uint32 idxcmd,
                                                       netwox_smbcmd *psmbcmd)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize, pktdatasize;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_READ_R));
    if (parasize != 10) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, pktdatasize);
    para += 8; /* reserved */
    netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_DATABLOCK, &data, &datasize, &psmbcmd->value.read_r.data));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (pktdatasize != netwib__buf_ref_data_size(&psmbcmd->value.read_r.data)) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_READ_Q));
    if (parasize != 10) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.read_q.fileid);
    netwib__data_decode_uint16_le(para, psmbcmd->value.read_q.size);
    netwib__data_decode_uint32_le(para, psmbcmd->value.read_q.offset);
    netwib__data_decode_uint16_le(para, psmbcmd->value.read_q.remaining);
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_write(netwox_constsmbmsg *psmbmsg,
                                                    netwib_uint32 idxcmd,
                                                    netwox_smbcmd *psmbcmd)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize;
  netwib_uint16 datasizepkt;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_WRITE_R));
    if (parasize != 2) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.write_r.writtensize);
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_WRITE_Q));
    if (parasize != 10) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.write_q.fileid);
    netwib__data_decode_uint16_le(para, datasizepkt);
    netwib__data_decode_uint32_le(para, psmbcmd->value.write_q.offset);
    netwib__data_decode_uint16_le(para, psmbcmd->value.write_q.remaining);
    netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_DATABLOCK, &data, &datasize, &psmbcmd->value.write_q.data));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasizepkt !=
        netwib__buf_ref_data_size(&psmbcmd->value.write_q.data)) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_createtemporary(netwox_constsmbmsg *psmbmsg,
                                                              netwib_uint32 idxcmd,
                                                              netwox_smbcmd *psmbcmd)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_CREATETEMPORARY_R));
    if (parasize != 2) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.createtemporary_r.fileid);
    netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &data, &datasize, &psmbcmd->value.createtemporary_r.filename));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_CREATETEMPORARY_Q));
    if (parasize != 6) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwox__smbcmdcmn_fileattr16_decode(para, psmbcmd->value.createtemporary_q.fileattr);
    netwox__smbcmdcmn_time1970l_decode(para, psmbcmd->value.createtemporary_q.creationtime);
    netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &data, &datasize, &psmbcmd->value.createtemporary_q.dirname));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_createnew(netwox_constsmbmsg *psmbmsg,
                                                              netwib_uint32 idxcmd,
                                                              netwox_smbcmd *psmbcmd)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_CREATENEW_R));
    if (parasize != 2) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.createnew_r.fileid);
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_CREATENEW_Q));
    if (parasize != 6) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwox__smbcmdcmn_fileattr16_decode(para, psmbcmd->value.createnew_q.fileattributes);
    netwox__smbcmdcmn_time1970l_decode(para, psmbcmd->value.createnew_q.creationtime);
    netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &data, &datasize, &psmbcmd->value.createnew_q.filename));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_checkdirpath(netwox_constsmbmsg *psmbmsg,
                                                       netwib_uint32 idxcmd,
                                                       netwox_smbcmd *psmbcmd)
{
  netwib_data data;
  netwib_uint32 parasize, datasize;

  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_CHECKDIRPATH_R));
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_CHECKDIRPATH_Q));
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &data, &datasize, &psmbcmd->value.checkdirpath_q.dirname));
    if (datasize == 2) {
      /* Samba 2 ends with 2 zero bytes if not Unicode */
      if (! (psmbmsg->hdr.flags2 & NETWOX_SMBMSG_FLAGS2_UNICODESTRINGS)) {
        if (data[0] == 0 && data[1] == 0) {
          data += 2;
          datasize -= 2;
        }
      }
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_seek(netwox_constsmbmsg *psmbmsg,
                                                   netwib_uint32 idxcmd,
                                                   netwox_smbcmd *psmbcmd)
{
  netwib_data para;
  netwib_uint32 parasize, datasize;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_SEEK_R));
    if (parasize != 4) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint32_le(para, psmbcmd->value.seek_r.offset);
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_SEEK_Q));
    if (parasize != 8) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.seek_q.fileid);
    netwox__smbcmdcmn_seektype_decode(para, psmbcmd->value.seek_q.seektype);
    netwib__data_decode_uint32_le(para, psmbcmd->value.seek_q.offset);
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_readraw(netwox_constsmbmsg *psmbmsg,
                                                      netwib_uint32 idxcmd,
                                                      netwox_smbcmd *psmbcmd)
{
  netwib_data para;
  netwib_uint32 parasize, datasize;
  netwib_uint32 offsetlow, offsethigh;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    /* normally, there is no reply */
    return(NETWIB_ERR_NOTCONVERTED);
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_READRAW_Q));
    if (parasize != 20 && parasize != 16) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.readraw_q.fileid);
    netwib__data_decode_uint32_le(para, offsetlow);
    netwib__data_decode_uint16_le(para, psmbcmd->value.readraw_q.maxsize);
    netwib__data_decode_uint16_le(para, psmbcmd->value.readraw_q.minsize);
    netwib__data_decode_uint32_le(para, psmbcmd->value.readraw_q.timeout);
    para += 2; /* reserved */
    offsethigh = 0;
    if (parasize == 20) {
      netwib__data_decode_uint32_le(para, offsethigh);
    }
    netwib__uint64_init_32(offsethigh, offsetlow, psmbcmd->value.readraw_q.offset);
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_writeraw(netwox_constsmbmsg *psmbmsg,
                                                       netwib_uint32 idxcmd,
                                                       netwox_smbcmd *psmbcmd)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize;
  netwib_uint32 totaldatasize, pktdatasize, pktdataoffset;
  netwib_uint32 offsetlow, offsethigh;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_WRITERAW_R));
    if (parasize != 2) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.writeraw_r.remaining);
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    if (idxcmd != 0) {
      /* because of offset (59), only know how to decode if alone */
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_WRITERAW_Q));
    if (parasize != 24 && parasize != 28) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.writeraw_q.fileid);
    netwib__data_decode_uint32_le(para, totaldatasize);
    netwib__data_decode_uint32_le(para, offsetlow);
    netwib__data_decode_uint32_le(para, psmbcmd->value.writeraw_q.timeout);
    netwox__smbcmdcmn_accessmode_decode(para, psmbcmd->value.writeraw_q.accessmode);
    para += 4; /* reserved */
    netwib__data_decode_uint16_le(para, pktdatasize);
    netwib__data_decode_uint16_le(para, pktdataoffset);
    if (pktdataoffset < 59) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    pktdataoffset -= 59;
    if (pktdataoffset > datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    offsethigh = 0;
    if (parasize == 28) {
      netwib__data_decode_uint32_le(para, offsethigh);
    }
    netwib__uint64_init_32(offsethigh, offsetlow, psmbcmd->value.writeraw_q.offset);
    data += pktdataoffset;
    datasize -= pktdataoffset;
    netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, pktdatasize, &psmbcmd->value.writeraw_q.data));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (totaldatasize < netwib__buf_ref_data_size(&psmbcmd->value.writeraw_q.data)) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    psmbcmd->value.writeraw_q.rawdatasize = totaldatasize - netwib__buf_ref_data_size(&psmbcmd->value.writeraw_q.data);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_setinformation2(netwox_constsmbmsg *psmbmsg,
                                                       netwib_uint32 idxcmd,
                                                       netwox_smbcmd *psmbcmd)
{
  netwib_data para;
  netwib_uint32 parasize, datasize;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_SETINFORMATION2_R));
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_SETINFORMATION2_Q));
    if (parasize != 14) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.setinformation2_q.fileid);
    netwox__smbcmdcmn_timemsdos_decode(para, psmbcmd->value.setinformation2_q.creationtime);
    netwox__smbcmdcmn_timemsdos_decode(para, psmbcmd->value.setinformation2_q.lastaccesstime);
    netwox__smbcmdcmn_timemsdos_decode(para, psmbcmd->value.setinformation2_q.lastwritetime);
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_queryinformation2(netwox_constsmbmsg *psmbmsg,
                                                       netwib_uint32 idxcmd,
                                                       netwox_smbcmd *psmbcmd)
{
  netwib_data para;
  netwib_uint32 parasize, datasize;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_QUERYINFORMATION2_R));
    if (parasize != 22) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwox__smbcmdcmn_timemsdos_decode(para, psmbcmd->value.queryinformation2_r.creationtime);
    netwox__smbcmdcmn_timemsdos_decode(para, psmbcmd->value.queryinformation2_r.lastaccesstime);
    netwox__smbcmdcmn_timemsdos_decode(para, psmbcmd->value.queryinformation2_r.lastwritetime);
    netwib__data_decode_uint32_le(para, psmbcmd->value.queryinformation2_r.filesize);
    netwib__data_decode_uint32_le(para, psmbcmd->value.queryinformation2_r.allocationsize);
    netwox__smbcmdcmn_fileattr16_decode(para, psmbcmd->value.queryinformation2_r.fileattributes);
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_QUERYINFORMATION2_Q));
    if (parasize != 2) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.queryinformation2_q.fileid);
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_lockingandx_range(netwib_data *pdata,
                                                     netwib_uint32 *pdatasize,
                                                     netwib_uint8 locktype,
                                                                netwox_smbcmd_lockingandx_range *prange)
{
  netwib_data data;
  netwib_uint32 datasize, ui32a, ui32b;

  data = *pdata;
  datasize = *pdatasize;

  if (locktype & NETWOX_SMBCMD_LOCKINGANDX_LOCKTYPE_LARGEFILES) {
    if (datasize < 20) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(data, prange->pid);
    data += 2; /* reserved */
    netwib__data_decode_uint32_le(data, ui32a);
    netwib__data_decode_uint32_le(data, ui32b);
    netwib__uint64_init_32(ui32a, ui32b, prange->offset);
    netwib__data_decode_uint32_le(data, ui32a);
    netwib__data_decode_uint32_le(data, ui32b);
    netwib__uint64_init_32(ui32a, ui32b, prange->length);
    datasize -= 20;
  } else {
    if (datasize < 10) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(data, prange->pid);
    netwib__data_decode_uint32_le(data, ui32a);
    netwib__uint64_init_32(0, ui32a, prange->offset);
    netwib__data_decode_uint32_le(data, ui32a);
    netwib__uint64_init_32(0, ui32a, prange->length);
    datasize -= 10;
  }

  *pdata = data;
  *pdatasize = datasize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_lockingandx(netwox_constsmbmsg *psmbmsg,
                                                       netwib_uint32 idxcmd,
                                                       netwox_smbcmd *psmbcmd)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize, i;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_LOCKINGANDX_R));
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_LOCKINGANDX_Q));
    if (parasize != 12) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.lockingandx_q.fileid);
    netwib__data_decode_uint8_le(para, psmbcmd->value.lockingandx_q.locktype);
    netwox__smbcmdcmn_oplocklevel_decode(para, psmbcmd->value.lockingandx_q.oplocklevel);
    netwib__data_decode_uint32_le(para, psmbcmd->value.lockingandx_q.timeout);
    netwib__data_decode_uint16_le(para, psmbcmd->value.lockingandx_q.unlockscount);
    if (psmbcmd->value.lockingandx_q.unlockscount > NETWOX_SMBCMD_LOCKINGANDX_RANGE_MAX) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.lockingandx_q.lockscount);
    if (psmbcmd->value.lockingandx_q.lockscount > NETWOX_SMBCMD_LOCKINGANDX_RANGE_MAX) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    for (i = 0 ; i < psmbcmd->value.lockingandx_q.unlockscount; i++) {
      netwib_er(netwox_smbmsg_decode_smbcmd_lockingandx_range(&data, &datasize, psmbcmd->value.lockingandx_q.locktype, &psmbcmd->value.lockingandx_q.unlocks[i]));
    }
    for (i = 0 ; i < psmbcmd->value.lockingandx_q.lockscount; i++) {
      netwib_er(netwox_smbmsg_decode_smbcmd_lockingandx_range(&data, &datasize, psmbcmd->value.lockingandx_q.locktype, &psmbcmd->value.lockingandx_q.locks[i]));
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_transaction(netwox_constsmbmsg *psmbmsg,
                                                   netwib_uint32 idxcmd,
                                                   netwox_smbcmd *psmbcmd)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize, i, datasizeori, skipsize, dataoffset;
  netwib_uint16 parasizepkt, paraoffsetpkt, datasizepkt, dataoffsetpkt;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_TRANSACTION_R));
    if (parasize < NETWOX_SMBCMD_TRANSACTIONR_COMMONSIZE) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.transaction_r.totalparasize);
    netwib__data_decode_uint16_le(para, psmbcmd->value.transaction_r.totaldatasize);
    para += 2; /* reserved */
    netwib__data_decode_uint16_le(para, parasizepkt);
    netwib__data_decode_uint16_le(para, paraoffsetpkt);
    netwib__data_decode_uint16_le(para, psmbcmd->value.transaction_r.paradisplacement);
    netwib__data_decode_uint16_le(para, datasizepkt);
    netwib__data_decode_uint16_le(para, dataoffsetpkt);
    netwib__data_decode_uint16_le(para, psmbcmd->value.transaction_r.datadisplacement);
    netwib__data_decode_uint8_le(para, psmbcmd->value.transaction_r.setupcount);
    para++; /* reserved */
    if (psmbcmd->value.transaction_r.setupcount > NETWOX_SMBCMD_TRANSACTION_SETUP_MAX) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize != NETWOX_SMBCMD_TRANSACTIONR_COMMONSIZE + 2*psmbcmd->value.transaction_r.setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    for (i = 0; i < psmbcmd->value.transaction_r.setupcount; i++) {
      netwib__data_decode_uint16_le(para, psmbcmd->value.transaction_r.setup[i]);
    }
    if (idxcmd != 0) {
      /* because of offset, only know how to decode if alone */
      return(NETWIB_ERR_NOTCONVERTED);
    }
    dataoffset = NETWOX_SMBMSGHDR_LEN + 1 + NETWOX_SMBCMD_TRANSACTIONR_COMMONSIZE + 2*psmbcmd->value.transaction_r.setupcount + 2;
    if (parasizepkt) {
      if (paraoffsetpkt < dataoffset) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      paraoffsetpkt = (netwib_uint16)(paraoffsetpkt - dataoffset);
      if ((netwib_uint32)paraoffsetpkt + parasizepkt > datasize) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib_er(netwib_buf_init_ext_arrayfilled(data+paraoffsetpkt, parasizepkt, &psmbcmd->value.transaction_r.para));
      skipsize = paraoffsetpkt + parasizepkt;
      data += skipsize;
      datasize -= skipsize;
      dataoffset += skipsize;
    } else {
      netwib_er(netwib_buf_init_ext_empty(&psmbcmd->value.transaction_r.para));
    }
    if (datasizepkt) {
      if (dataoffsetpkt < dataoffset) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      dataoffsetpkt = (netwib_uint16)(dataoffsetpkt - dataoffset);
      if ((netwib_uint32)dataoffsetpkt + datasizepkt > datasize) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib_er(netwib_buf_init_ext_arrayfilled(data+dataoffsetpkt, datasizepkt, &psmbcmd->value.transaction_r.data));
      skipsize = dataoffsetpkt + datasizepkt;
      data += skipsize;
      datasize -= skipsize;
      dataoffset += skipsize;
    } else {
      netwib_er(netwib_buf_init_ext_empty(&psmbcmd->value.transaction_r.data));
    }
    if (datasize != 0 && datasize != 1/* sometimes ends with a pad */) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_TRANSACTION_Q));
    if (parasize < NETWOX_SMBCMD_TRANSACTIONQ_COMMONSIZE) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.transaction_q.totalparasize);
    netwib__data_decode_uint16_le(para, psmbcmd->value.transaction_q.totaldatasize);
    netwib__data_decode_uint16_le(para, psmbcmd->value.transaction_q.maxparasize);
    netwib__data_decode_uint16_le(para, psmbcmd->value.transaction_q.maxdatasize);
    netwib__data_decode_uint8_le(para, psmbcmd->value.transaction_q.maxsetupcount);
    para++; /* reserved */
    netwib__data_decode_uint16_le(para, psmbcmd->value.transaction_q.flags);
    netwib__data_decode_uint32_le(para, psmbcmd->value.transaction_q.timeout);
    para += 2; /* reserved */
    netwib__data_decode_uint16_le(para, parasizepkt);
    netwib__data_decode_uint16_le(para, paraoffsetpkt);
    netwib__data_decode_uint16_le(para, datasizepkt);
    netwib__data_decode_uint16_le(para, dataoffsetpkt);
    netwib__data_decode_uint8_le(para, psmbcmd->value.transaction_q.setupcount);
    para++; /* reserved */
    if (psmbcmd->value.transaction_q.setupcount > NETWOX_SMBCMD_TRANSACTION_SETUP_MAX) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize != NETWOX_SMBCMD_TRANSACTIONQ_COMMONSIZE + 2*psmbcmd->value.transaction_q.setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    for (i = 0; i < psmbcmd->value.transaction_q.setupcount; i++) {
      netwib__data_decode_uint16_le(para, psmbcmd->value.transaction_q.setup[i]);
    }
    if (idxcmd != 0) {
      /* because of offset, only know how to decode if alone */
      return(NETWIB_ERR_NOTCONVERTED);
    }
    datasizeori = datasize;
    dataoffset = NETWOX_SMBMSGHDR_LEN + 1 + NETWOX_SMBCMD_TRANSACTIONQ_COMMONSIZE + 2*psmbcmd->value.transaction_q.setupcount + 2;
    netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &data, &datasize, &psmbcmd->value.transaction_q.name));
    skipsize = datasizeori - datasize;
    dataoffset += skipsize; /* data and datasize have already been updated */
    if (parasizepkt) {
      if (paraoffsetpkt < dataoffset) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      paraoffsetpkt = (netwib_uint16)(paraoffsetpkt - dataoffset);
      if ((netwib_uint32)paraoffsetpkt + parasizepkt > datasize) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib_er(netwib_buf_init_ext_arrayfilled(data+paraoffsetpkt, parasizepkt, &psmbcmd->value.transaction_q.para));
      skipsize = paraoffsetpkt + parasizepkt;
      data += skipsize;
      datasize -= skipsize;
      dataoffset += skipsize;
    } else {
      netwib_er(netwib_buf_init_ext_empty(&psmbcmd->value.transaction_q.para));
    }
    if (datasizepkt) {
      if (dataoffsetpkt < dataoffset) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      dataoffsetpkt = (netwib_uint16)(dataoffsetpkt - dataoffset);
      if ((netwib_uint32)dataoffsetpkt + datasizepkt > datasize) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib_er(netwib_buf_init_ext_arrayfilled(data+dataoffsetpkt, datasizepkt, &psmbcmd->value.transaction_q.data));
      skipsize = dataoffsetpkt + datasizepkt;
      data += skipsize;
      datasize -= skipsize;
      dataoffset += skipsize;
    } else {
      netwib_er(netwib_buf_init_ext_empty(&psmbcmd->value.transaction_q.data));
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    psmbcmd->value.transaction_q.isunicode = psmbmsg->hdr.flags2 & NETWOX_SMBMSG_FLAGS2_UNICODESTRINGS;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_echo(netwox_constsmbmsg *psmbmsg,
                                                   netwib_uint32 idxcmd,
                                                   netwox_smbcmd *psmbcmd)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_ECHO_R));
    if (parasize != 2) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.echo_r.number);
    netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, datasize, &psmbcmd->value.echo_r.data));
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_ECHO_Q));
    if (parasize != 2) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.echo_q.count);
    netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, datasize, &psmbcmd->value.echo_q.data));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_openandx(netwox_constsmbmsg *psmbmsg,
                                                       netwib_uint32 idxcmd,
                                                       netwox_smbcmd *psmbcmd)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_OPENANDX_R));
    if (parasize != 26) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.openandx_r.fileid);
    netwox__smbcmdcmn_fileattr16_decode(para, psmbcmd->value.openandx_r.fileattributes);
    netwox__smbcmdcmn_time1970l_decode(para, psmbcmd->value.openandx_r.lastwritetime);
    netwib__data_decode_uint32_le(para, psmbcmd->value.openandx_r.filesize);
    netwox__smbcmdcmn_accessmode_decode(para, psmbcmd->value.openandx_r.grantedaccess);
    netwox__smbcmdcmn_filetype_decode(para, psmbcmd->value.openandx_r.filetype);
    netwib__data_decode_uint16_le(para, psmbcmd->value.openandx_r.ipcstate);
    netwox__smbcmdcmn_createaction_decode(para, psmbcmd->value.openandx_r.action);
    netwib__data_decode_uint32_le(para, psmbcmd->value.openandx_r.serverfileid);
    para += 2; /* reserved */
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_OPENANDX_Q));
    if (parasize != 26) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwox__smbcmdcmn_openflags16_decode(para, psmbcmd->value.openandx_q.openflags);
    netwox__smbcmdcmn_accessmode_decode(para, psmbcmd->value.openandx_q.desiredaccess);
    netwox__smbcmdcmn_fileattr16_decode(para, psmbcmd->value.openandx_q.searchattributes);
    netwox__smbcmdcmn_fileattr16_decode(para, psmbcmd->value.openandx_q.fileattributes);
    netwox__smbcmdcmn_time1970l_decode(para, psmbcmd->value.openandx_q.creationtime);
    netwox__smbcmdcmn_openfunction_decode(para, psmbcmd->value.openandx_q.openfunction);
    netwib__data_decode_uint32_le(para, psmbcmd->value.openandx_q.allocationsize);
    netwib__data_decode_uint32_le(para, psmbcmd->value.openandx_q.timeout);
    netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &data, &datasize, &psmbcmd->value.openandx_q.filename));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_readandx(netwox_constsmbmsg *psmbmsg,
                                                       netwib_uint32 idxcmd,
                                                       netwox_smbcmd *psmbcmd)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize;
  netwib_uint32 offsetlow, offsethigh, datasizepkt, dataoffset, minoffset;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_READANDX_R));
    if (parasize != 20) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.readandx_r.remaining);
    netwib__data_decode_uint16_le(para, psmbcmd->value.readandx_r.datacompactionmode);
    para += 2; /* reserved */
    netwib__data_decode_uint16_le(para, datasizepkt);
    netwib__data_decode_uint16_le(para, dataoffset);
    para += 10; /* reserved */
    /* We have to skip padding. For the moment, only implement offset for
       first command in the AndX. */
    if (idxcmd != 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasizepkt == 0 && dataoffset == 0) {
      netwib_er(netwib_buf_init_ext_empty(&psmbcmd->value.readandx_r.data));
    } else {
      minoffset = NETWOX_SMBMSGHDR_LEN + 1 + NETWOX_SMBMSGANDX_LEN + parasize +2;
      if (dataoffset < minoffset) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      dataoffset -= minoffset;
      if (dataoffset > datasize) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      data += dataoffset;
      datasize -= dataoffset;
      netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, datasizepkt, &psmbcmd->value.readandx_r.data));
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_READANDX_Q));
    if (parasize != 20 && parasize != 16) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.readandx_q.fileid);
    netwib__data_decode_uint32_le(para, offsetlow);
    netwib__data_decode_uint16_le(para, psmbcmd->value.readandx_q.maxsize);
    netwib__data_decode_uint16_le(para, psmbcmd->value.readandx_q.minsize);
    netwib__data_decode_uint32_le(para, psmbcmd->value.readandx_q.timeout);
    netwib__data_decode_uint16_le(para, psmbcmd->value.readandx_q.remaining);
    offsethigh = 0;
    if (parasize == 20) {
      netwib__data_decode_uint32_le(para, offsethigh);
    }
    netwib__uint64_init_32(offsethigh, offsetlow, psmbcmd->value.readandx_q.offset);
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_writeandx(netwox_constsmbmsg *psmbmsg,
                                                        netwib_uint32 idxcmd,
                                                        netwox_smbcmd *psmbcmd)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize;
  netwib_uint32 offsetlow, offsethigh, datasizepkt, dataoffset, minoffset;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_WRITEANDX_R));
    if (parasize != 8) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.writeandx_r.writtensize);
    netwib__data_decode_uint16_le(para, psmbcmd->value.writeandx_r.remaining);
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_WRITEANDX_Q));
    if (parasize != 24 && parasize != 20) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.writeandx_q.fileid);
    netwib__data_decode_uint32_le(para, offsetlow);
    netwib__data_decode_uint32_le(para, psmbcmd->value.writeandx_q.timeout);
    netwib__data_decode_uint16_le(para, psmbcmd->value.writeandx_q.writemode);
    netwib__data_decode_uint16_le(para, psmbcmd->value.writeandx_q.remaining);
    para += 2; /* reserved */
    netwib__data_decode_uint16_le(para, datasizepkt);
    netwib__data_decode_uint16_le(para, dataoffset);
    offsethigh = 0;
    if (parasize == 24) {
      netwib__data_decode_uint32_le(para, offsethigh);
    }
    netwib__uint64_init_32(offsethigh, offsetlow, psmbcmd->value.writeandx_q.offset);
    /* We have to skip padding. For the moment, only implement offset for
       first command in the AndX. */
    if (idxcmd != 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    minoffset = NETWOX_SMBMSGHDR_LEN + 1 + NETWOX_SMBMSGANDX_LEN + parasize +2;
    if (dataoffset < minoffset) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    dataoffset -= minoffset;
    if (dataoffset > datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    data += dataoffset;
    datasize -= dataoffset;
    netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, datasizepkt, &psmbcmd->value.writeandx_q.data));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_closetreedisconnect(netwox_constsmbmsg *psmbmsg,
                                                    netwib_uint32 idxcmd,
                                                    netwox_smbcmd *psmbcmd)
{
  netwib_data para;
  netwib_uint32 parasize, datasize;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_CLOSETREEDISCONNECT_R));
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_CLOSETREEDISCONNECT_Q));
    if (parasize != 6) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.closetreedisconnect_q.fileid);
    netwox__smbcmdcmn_time1970l_decode(para, psmbcmd->value.closetreedisconnect_q.lastwritetime);
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_transaction2(netwox_constsmbmsg *psmbmsg,
                                                   netwib_uint32 idxcmd,
                                                   netwox_smbcmd *psmbcmd)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize, i, skipsize, dataoffset;
  netwib_uint16 parasizepkt, paraoffsetpkt, datasizepkt, dataoffsetpkt;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_TRANSACTION2_R));
    if (parasize < NETWOX_SMBCMD_TRANSACTION2R_COMMONSIZE) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.transaction2_r.totalparasize);
    netwib__data_decode_uint16_le(para, psmbcmd->value.transaction2_r.totaldatasize);
    para += 2; /* reserved */
    netwib__data_decode_uint16_le(para, parasizepkt);
    netwib__data_decode_uint16_le(para, paraoffsetpkt);
    netwib__data_decode_uint16_le(para, psmbcmd->value.transaction2_r.paradisplacement);
    netwib__data_decode_uint16_le(para, datasizepkt);
    netwib__data_decode_uint16_le(para, dataoffsetpkt);
    netwib__data_decode_uint16_le(para, psmbcmd->value.transaction2_r.datadisplacement);
    netwib__data_decode_uint8_le(para, psmbcmd->value.transaction2_r.setupcount);
    para++; /* reserved */
    if (psmbcmd->value.transaction2_r.setupcount > NETWOX_SMBCMD_TRANSACTION2_SETUP_MAX) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize != NETWOX_SMBCMD_TRANSACTION2R_COMMONSIZE + 2*psmbcmd->value.transaction2_r.setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    for (i = 0; i < psmbcmd->value.transaction2_r.setupcount; i++) {
      netwib__data_decode_uint16_le(para, psmbcmd->value.transaction2_r.setup[i]);
    }
    if (idxcmd != 0) {
      /* because of offset, only know how to decode if alone */
      return(NETWIB_ERR_NOTCONVERTED);
    }
    dataoffset = NETWOX_SMBMSGHDR_LEN + 1 + NETWOX_SMBCMD_TRANSACTION2R_COMMONSIZE + 2*psmbcmd->value.transaction2_r.setupcount + 2;
    if (parasizepkt) {
      if (paraoffsetpkt < dataoffset) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      paraoffsetpkt = (netwib_uint16)(paraoffsetpkt - dataoffset);
      if ((netwib_uint32)paraoffsetpkt + parasizepkt > datasize) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib_er(netwib_buf_init_ext_arrayfilled(data+paraoffsetpkt, parasizepkt, &psmbcmd->value.transaction2_r.para));
      skipsize = paraoffsetpkt + parasizepkt;
      data += skipsize;
      datasize -= skipsize;
      dataoffset += skipsize;
    } else {
      netwib_er(netwib_buf_init_ext_empty(&psmbcmd->value.transaction2_r.para));
    }
    if (datasizepkt) {
      if (dataoffsetpkt < dataoffset) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      dataoffsetpkt = (netwib_uint16)(dataoffsetpkt - dataoffset);
      if ((netwib_uint32)dataoffsetpkt + datasizepkt > datasize) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib_er(netwib_buf_init_ext_arrayfilled(data+dataoffsetpkt, datasizepkt, &psmbcmd->value.transaction2_r.data));
      skipsize = dataoffsetpkt + datasizepkt;
      data += skipsize;
      datasize -= skipsize;
      dataoffset += skipsize;
    } else {
      netwib_er(netwib_buf_init_ext_empty(&psmbcmd->value.transaction2_r.data));
    }
    if (datasize > 2/* sometimes ends with one or two pad */) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_TRANSACTION2_Q));
    if (parasize < NETWOX_SMBCMD_TRANSACTION2Q_COMMONSIZE) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.transaction2_q.totalparasize);
    netwib__data_decode_uint16_le(para, psmbcmd->value.transaction2_q.totaldatasize);
    netwib__data_decode_uint16_le(para, psmbcmd->value.transaction2_q.maxparasize);
    netwib__data_decode_uint16_le(para, psmbcmd->value.transaction2_q.maxdatasize);
    netwib__data_decode_uint8_le(para, psmbcmd->value.transaction2_q.maxsetupcount);
    para++; /* reserved */
    netwib__data_decode_uint16_le(para, psmbcmd->value.transaction2_q.flags);
    netwib__data_decode_uint32_le(para, psmbcmd->value.transaction2_q.timeout);
    para += 2; /* reserved */
    netwib__data_decode_uint16_le(para, parasizepkt);
    netwib__data_decode_uint16_le(para, paraoffsetpkt);
    netwib__data_decode_uint16_le(para, datasizepkt);
    netwib__data_decode_uint16_le(para, dataoffsetpkt);
    netwib__data_decode_uint8_le(para, psmbcmd->value.transaction2_q.setupcount);
    para++; /* reserved */
    if (psmbcmd->value.transaction2_q.setupcount > NETWOX_SMBCMD_TRANSACTION2_SETUP_MAX) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize != NETWOX_SMBCMD_TRANSACTION2Q_COMMONSIZE + 2*psmbcmd->value.transaction2_q.setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    for (i = 0; i < psmbcmd->value.transaction2_q.setupcount; i++) {
      netwib__data_decode_uint16_le(para, psmbcmd->value.transaction2_q.setup[i]);
    }
    if (idxcmd != 0) {
      /* because of offset, only know how to decode if alone */
      return(NETWIB_ERR_NOTCONVERTED);
    }
    dataoffset = NETWOX_SMBMSGHDR_LEN + 1 + NETWOX_SMBCMD_TRANSACTION2Q_COMMONSIZE + 2*psmbcmd->value.transaction2_q.setupcount + 2;
    if (parasizepkt) {
      if (paraoffsetpkt < dataoffset) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      paraoffsetpkt = (netwib_uint16)(paraoffsetpkt - dataoffset);
      if ((netwib_uint32)paraoffsetpkt + parasizepkt > datasize) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib_er(netwib_buf_init_ext_arrayfilled(data+paraoffsetpkt, parasizepkt, &psmbcmd->value.transaction2_q.para));
      skipsize = paraoffsetpkt + parasizepkt;
      data += skipsize;
      datasize -= skipsize;
      dataoffset += skipsize;
    } else {
      netwib_er(netwib_buf_init_ext_empty(&psmbcmd->value.transaction2_q.para));
    }
    if (datasizepkt) {
      if (dataoffsetpkt < dataoffset) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      dataoffsetpkt = (netwib_uint16)(dataoffsetpkt - dataoffset);
      if ((netwib_uint32)dataoffsetpkt + datasizepkt > datasize) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib_er(netwib_buf_init_ext_arrayfilled(data+dataoffsetpkt, datasizepkt, &psmbcmd->value.transaction2_q.data));
      skipsize = dataoffsetpkt + datasizepkt;
      data += skipsize;
      datasize -= skipsize;
      dataoffset += skipsize;
    } else {
      netwib_er(netwib_buf_init_ext_empty(&psmbcmd->value.transaction2_q.data));
    }
    if (datasize != 0 && datasize != 4 && datasize != 6) {
      /* depending on subcommand, we can have a 0, 4 or 6 byte size */
      return(NETWIB_ERR_NOTCONVERTED);
    }
    psmbcmd->value.transaction2_q.isunicode = psmbmsg->hdr.flags2 & NETWOX_SMBMSG_FLAGS2_UNICODESTRINGS;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_findclose2(netwox_constsmbmsg *psmbmsg,
                                                       netwib_uint32 idxcmd,
                                                       netwox_smbcmd *psmbcmd)
{
  netwib_data para;
  netwib_uint32 parasize, datasize;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_FINDCLOSE2_R));
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_FINDCLOSE2_Q));
    if (parasize != 2) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.findclose2_q.sid);
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_treedisconnect(netwox_constsmbmsg *psmbmsg,
                                                       netwib_uint32 idxcmd,
                                                       netwox_smbcmd *psmbcmd)
{
  netwib_uint32 parasize, datasize;

  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_TREEDISCONNECT_R));
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_TREEDISCONNECT_Q));
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_negotiate(netwox_constsmbmsg *psmbmsg,
                                                        netwib_uint32 idxcmd,
                                                        netwox_smbcmd *psmbcmd)
{
  netwib_data para, data, paracapa;
  netwib_uint32 parasize, datasize, i, challengesize, capabilities;
  netwib_uint16 ui16;
  netwib_buf buf, ntlm012;
  netwib_cmp cmp;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    if (parasize != 34) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    paracapa = para + 19;
    netwox__smbcmdcmn_negocapa_decode(paracapa, capabilities);
    if (capabilities & NETWOX_SMBCMDCMN_NEGOCAPA_EXTENDEDSECURITY) {
      netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_NEGOTIATE_R1));
      netwib__data_decode_uint16_le(para, psmbcmd->value.negotiate_r1.dialectindex);
      netwib__data_decode_uint8_le(para, psmbcmd->value.negotiate_r1.secumode);
      netwib__data_decode_uint16_le(para, psmbcmd->value.negotiate_r1.maxmultiplex);
      netwib__data_decode_uint16_le(para, psmbcmd->value.negotiate_r1.maxvc);
      netwib__data_decode_uint32_le(para, psmbcmd->value.negotiate_r1.maxmsgsize);
      netwib__data_decode_uint32_le(para, psmbcmd->value.negotiate_r1.maxrawsize);
      netwib__data_decode_uint32_le(para, psmbcmd->value.negotiate_r1.vcid);
      netwox__smbcmdcmn_negocapa_decode(para, psmbcmd->value.negotiate_r1.capabilities);
      netwox__smbcmdcmn_time1601_decode(para, psmbcmd->value.negotiate_r1.systemtime);
      netwib__data_decode_uint16_le(para, ui16);
      psmbcmd->value.negotiate_r1.timezoneoffset = (netwib_int16)ui16;
      para++; /* reserved */
      netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, 16, &psmbcmd->value.negotiate_r1.serverguid));
      /* security blob is till the end */
      netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, &psmbcmd->value.negotiate_r1.securityblob));
    } else {
      netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_NEGOTIATE_R0));
      netwib__data_decode_uint16_le(para, psmbcmd->value.negotiate_r0.dialectindex);
      netwib__data_decode_uint8_le(para, psmbcmd->value.negotiate_r0.secumode);
      netwib__data_decode_uint16_le(para, psmbcmd->value.negotiate_r0.maxmultiplex);
      netwib__data_decode_uint16_le(para, psmbcmd->value.negotiate_r0.maxvc);
      netwib__data_decode_uint32_le(para, psmbcmd->value.negotiate_r0.maxmsgsize);
      netwib__data_decode_uint32_le(para, psmbcmd->value.negotiate_r0.maxrawsize);
      netwib__data_decode_uint32_le(para, psmbcmd->value.negotiate_r0.vcid);
      netwox__smbcmdcmn_negocapa_decode(para, psmbcmd->value.negotiate_r0.capabilities);
      netwox__smbcmdcmn_time1601_decode(para, psmbcmd->value.negotiate_r0.systemtime);
      netwib__data_decode_uint16_le(para, ui16);
      psmbcmd->value.negotiate_r0.timezoneoffset = (netwib_int16)ui16;
      netwib__data_decode_uint8_le(para, challengesize);
      netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, challengesize, &psmbcmd->value.negotiate_r0.challenge));
      psmbcmd->value.negotiate_r0.i1set = NETWIB_FALSE;
      if (datasize) {
        /* sometimes, domain is not present */
        netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_UNINOPAD, &data, &datasize, &psmbcmd->value.negotiate_r0.domain));
        if (datasize) {
          /* generally, server is not present */
          psmbcmd->value.negotiate_r0.i1set = NETWIB_TRUE;
          netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_UNINOPAD, &data, &datasize, &psmbcmd->value.negotiate_r0.i1.server));
          if (datasize != 0) {
            return(NETWIB_ERR_NOTCONVERTED);
          }
        }
      } else {
        netwib_er(netwib_buf_init_ext_empty(&psmbcmd->value.negotiate_r0.domain));
      }
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_NEGOTIATE_Q));
    if (parasize != 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwib_buf_init_ext_string(NETWOX_SMBCMD_NEGOTIATE_DIALECT_NTLM012,
                                       &ntlm012));
    psmbcmd->value.negotiate_q.ntlm012position = 0;
    i = 0;
    while(datasize) {
      netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_DIALECT, &data, &datasize, &buf));
      i++;
      netwib_er(netwib_buf_cmp(&buf, &ntlm012, &cmp));
      if (cmp == NETWIB_CMP_EQ) {
        psmbcmd->value.negotiate_q.ntlm012position = i;
        break;
      }
    }
    if (psmbcmd->value.negotiate_q.ntlm012position == 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_sessionsetupandx(netwox_constsmbmsg *psmbmsg,
                                                       netwib_uint32 idxcmd,
                                                       netwox_smbcmd *psmbcmd)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize;
  netwib_uint16 passwordsize, unicodepasswordsize=0, secublobsize;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    if (parasize == 2) {
      /* R0 */
      netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_R0));
      netwib__data_decode_uint16_le(para, psmbcmd->value.sessionsetupandx_r0.action);
      psmbcmd->value.sessionsetupandx_r0.i1set = NETWIB_FALSE;
      if (datasize != 0) {
        psmbcmd->value.sessionsetupandx_r0.i1set = NETWIB_TRUE;
        netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &data, &datasize, &psmbcmd->value.sessionsetupandx_r0.i1.nativeos));
        netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &data, &datasize, &psmbcmd->value.sessionsetupandx_r0.i1.nativelanman));
        psmbcmd->value.sessionsetupandx_r0.i1.i2set = NETWIB_FALSE;
        if (datasize != 0) {
          psmbcmd->value.sessionsetupandx_r0.i1.i2set = NETWIB_TRUE;
          netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &data, &datasize, &psmbcmd->value.sessionsetupandx_r0.i1.i2.domain));
          if (datasize != 0) {
            return(NETWIB_ERR_NOTCONVERTED);
          }
        }
      }
    } else if (parasize == 4) {
      /* R1 */
      netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_R1));
      netwib__data_decode_uint16_le(para, psmbcmd->value.sessionsetupandx_r1.action);
      netwib__data_decode_uint16_le(para, secublobsize);
      netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, secublobsize, &psmbcmd->value.sessionsetupandx_r1.secublob));
      netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &data, &datasize, &psmbcmd->value.sessionsetupandx_r1.nativeos));
      netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &data, &datasize, &psmbcmd->value.sessionsetupandx_r1.nativelanman));
      psmbcmd->value.sessionsetupandx_r1.i2set = NETWIB_FALSE;
      if (datasize != 0) {
        psmbcmd->value.sessionsetupandx_r1.i2set = NETWIB_TRUE;
        netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &data, &datasize, &psmbcmd->value.sessionsetupandx_r1.i2.domain));
        if (datasize != 0) {
          return(NETWIB_ERR_NOTCONVERTED);
        }
      }
    } else {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    if (parasize == 22) {
      /* Q0 */
      netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_Q0));
      netwib__data_decode_uint16_le(para, psmbcmd->value.sessionsetupandx_q0.maxmsgsize);
      netwib__data_decode_uint16_le(para, psmbcmd->value.sessionsetupandx_q0.maxmultiplex);
      netwib__data_decode_uint16_le(para, psmbcmd->value.sessionsetupandx_q0.vcnumber);
      netwib__data_decode_uint32_le(para, psmbcmd->value.sessionsetupandx_q0.vcid);
      netwib__data_decode_uint16_le(para, passwordsize);
      netwib__data_decode_uint16_le(para, unicodepasswordsize);
      para += 4; /* reserved */
      netwox__smbcmdcmn_negocapa_decode(para, psmbcmd->value.sessionsetupandx_q0.capabilities);
      netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, passwordsize, &psmbcmd->value.sessionsetupandx_q0.password));
      netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, unicodepasswordsize, &psmbcmd->value.sessionsetupandx_q0.unicodepassword));
      netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &data, &datasize, &psmbcmd->value.sessionsetupandx_q0.user));
      netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &data, &datasize, &psmbcmd->value.sessionsetupandx_q0.domain));
      netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &data, &datasize, &psmbcmd->value.sessionsetupandx_q0.nativeos));
      netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &data, &datasize, &psmbcmd->value.sessionsetupandx_q0.nativelanman));
      psmbcmd->value.sessionsetupandx_q0.i1set = NETWIB_FALSE;
      if (datasize != 0) {
        psmbcmd->value.sessionsetupandx_q0.i1set = NETWIB_TRUE;
        netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &data, &datasize, &psmbcmd->value.sessionsetupandx_q0.i1.unknown));
        if (datasize != 0) {
          /* Some packets are wrong : nativelanman is empty, and stored after.
             Correct the bug if possible. */
          if (netwib__buf_ref_data_size(&psmbcmd->value.sessionsetupandx_q0.nativelanman) == 0) {
            psmbcmd->value.sessionsetupandx_q0.nativelanman = psmbcmd->value.sessionsetupandx_q0.i1.unknown;
            netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &data, &datasize, &psmbcmd->value.sessionsetupandx_q0.i1.unknown));
          } else {
            /* this is not the bug, so I don't know */
            return(NETWIB_ERR_NOTCONVERTED);
          }
          if (datasize != 0) {
            return(NETWIB_ERR_NOTCONVERTED);
          }
        }
      }
    } else if (parasize == 20) {
      /* Q1 */
      netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_SESSIONSETUPANDX_Q1));
      netwib__data_decode_uint16_le(para, psmbcmd->value.sessionsetupandx_q1.maxmsgsize);
      netwib__data_decode_uint16_le(para, psmbcmd->value.sessionsetupandx_q1.maxmultiplex);
      netwib__data_decode_uint16_le(para, psmbcmd->value.sessionsetupandx_q1.vcnumber);
      netwib__data_decode_uint32_le(para, psmbcmd->value.sessionsetupandx_q1.vcid);
      netwib__data_decode_uint16_le(para, secublobsize);
      para += 4; /* reserved */
      netwox__smbcmdcmn_negocapa_decode(para, psmbcmd->value.sessionsetupandx_q1.capabilities);
      netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, secublobsize, &psmbcmd->value.sessionsetupandx_q1.secublob));
      netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &data, &datasize, &psmbcmd->value.sessionsetupandx_q1.nativeos));
      netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &data, &datasize, &psmbcmd->value.sessionsetupandx_q1.nativelanman));
      netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &data, &datasize, &psmbcmd->value.sessionsetupandx_q1.domain));
      if (datasize != 0) {
          return(NETWIB_ERR_NOTCONVERTED);
      }
    } else {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_logoffandx(netwox_constsmbmsg *psmbmsg,
                                                       netwib_uint32 idxcmd,
                                                       netwox_smbcmd *psmbcmd)
{
  netwib_uint32 parasize, datasize;

  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_LOGOFFANDX_R));
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_LOGOFFANDX_Q));
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_treeconnectandx(netwox_constsmbmsg *psmbmsg,
                                                       netwib_uint32 idxcmd,
                                                       netwox_smbcmd *psmbcmd)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize;
  netwib_uint16 passwordsize;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_TREECONNECTANDX_R));
    psmbcmd->value.treeconnectandx_r.i1set = NETWIB_FALSE;
    psmbcmd->value.treeconnectandx_r.i1.i2set = NETWIB_FALSE;
    if (parasize == 0) {
    } else if (parasize == 2) {
      psmbcmd->value.treeconnectandx_r.i1set = NETWIB_TRUE;
      netwib__data_decode_uint16_le(para, psmbcmd->value.treeconnectandx_r.i1.optionalsupport);
    } else if (parasize == 10) {
      psmbcmd->value.treeconnectandx_r.i1set = NETWIB_TRUE;
      psmbcmd->value.treeconnectandx_r.i1.i2set = NETWIB_TRUE;
      netwib__data_decode_uint16_le(para, psmbcmd->value.treeconnectandx_r.i1.optionalsupport);
      netwib__data_decode_uint32_le(para, psmbcmd->value.treeconnectandx_r.i1.i2.ui32a);
      netwib__data_decode_uint32_le(para, psmbcmd->value.treeconnectandx_r.i1.i2.ui32b);
    } else {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI, &data, &datasize, &psmbcmd->value.treeconnectandx_r.service));
    if (psmbcmd->value.treeconnectandx_r.i1set) {
      netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &data, &datasize, &psmbcmd->value.treeconnectandx_r.i1.nativefs));
    }
    if (datasize && parasize != 10) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_TREECONNECTANDX_Q));
    if (parasize != 4) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.treeconnectandx_q.flags);
    netwib__data_decode_uint16_le(para, passwordsize);
    netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, passwordsize, &psmbcmd->value.treeconnectandx_q.password));
    netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY, &data, &datasize, &psmbcmd->value.treeconnectandx_q.path));
    netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_STRINGONLY_NOUNI, &data, &datasize, &psmbcmd->value.treeconnectandx_q.service));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_queryinformationdisk(netwox_constsmbmsg *psmbmsg,
                                                       netwib_uint32 idxcmd,
                                                       netwox_smbcmd *psmbcmd)
{
  netwib_data para;
  netwib_uint32 parasize, datasize;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_QUERYINFORMATIONDISK_R));
    if (parasize != 10) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.queryinformationdisk_r.totalunits);
    netwib__data_decode_uint16_le(para, psmbcmd->value.queryinformationdisk_r.sectorsperunit);
    netwib__data_decode_uint16_le(para, psmbcmd->value.queryinformationdisk_r.bytespersector);
    netwib__data_decode_uint16_le(para, psmbcmd->value.queryinformationdisk_r.freeunits);
    para += 2;  /* reserved bytes */
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_QUERYINFORMATIONDISK_Q));
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_search(netwox_constsmbmsg *psmbmsg,
                                                     netwib_uint32 idxcmd,
                                                     netwox_smbcmd *psmbcmd)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_SEARCH_R));
    if (parasize != 2) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.search_r.count);
    netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_VARIABLEBLOCK, &data, &datasize, &psmbcmd->value.search_r.entries));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_SEARCH_Q));
    if (parasize != 4) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.search_q.maxentries);
    netwox__smbcmdcmn_fileattr16_decode(para, psmbcmd->value.search_q.searchattributes);
    netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &data, &datasize, &psmbcmd->value.search_q.searchpattern));
    netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_VARIABLEBLOCK, &data, &datasize, &psmbcmd->value.search_q.resumekey));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_nttransact(netwox_constsmbmsg *psmbmsg,
                                                   netwib_uint32 idxcmd,
                                                   netwox_smbcmd *psmbcmd)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize, i, skipsize, dataoffset;
  netwib_uint32 parasizepkt, paraoffsetpkt, datasizepkt, dataoffsetpkt;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_NTTRANSACT_R));
    if (parasize < NETWOX_SMBCMD_NTTRANSACTR_COMMONSIZE) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    para += 3; /* reserved */
    netwib__data_decode_uint32_le(para, psmbcmd->value.nttransact_r.totalparasize);
    netwib__data_decode_uint32_le(para, psmbcmd->value.nttransact_r.totaldatasize);
    netwib__data_decode_uint32_le(para, parasizepkt);
    netwib__data_decode_uint32_le(para, paraoffsetpkt);
    netwib__data_decode_uint32_le(para, psmbcmd->value.nttransact_r.paradisplacement);
    netwib__data_decode_uint32_le(para, datasizepkt);
    netwib__data_decode_uint32_le(para, dataoffsetpkt);
    netwib__data_decode_uint32_le(para, psmbcmd->value.nttransact_r.datadisplacement);
    netwib__data_decode_uint8_le(para, psmbcmd->value.nttransact_r.setupcount);
    if (psmbcmd->value.nttransact_r.setupcount > NETWOX_SMBCMD_NTTRANSACT_SETUP_MAX) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (parasize != NETWOX_SMBCMD_NTTRANSACTR_COMMONSIZE + 2*psmbcmd->value.nttransact_r.setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    for (i = 0; i < psmbcmd->value.nttransact_r.setupcount; i++) {
      netwib__data_decode_uint16_le(para, psmbcmd->value.nttransact_r.setup[i]);
    }
    if (idxcmd != 0) {
      /* because of offset, only know how to decode if alone */
      return(NETWIB_ERR_NOTCONVERTED);
    }
    dataoffset = NETWOX_SMBMSGHDR_LEN + 1 + NETWOX_SMBCMD_NTTRANSACTR_COMMONSIZE + 2*psmbcmd->value.nttransact_r.setupcount + 2;
    if (parasizepkt) {
      if (paraoffsetpkt < dataoffset) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      paraoffsetpkt -= dataoffset;
      if ((netwib_uint32)paraoffsetpkt + parasizepkt > datasize) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib_er(netwib_buf_init_ext_arrayfilled(data+paraoffsetpkt, parasizepkt, &psmbcmd->value.nttransact_r.para));
      skipsize = paraoffsetpkt + parasizepkt;
      data += skipsize;
      datasize -= skipsize;
      dataoffset += skipsize;
    } else {
      netwib_er(netwib_buf_init_ext_empty(&psmbcmd->value.nttransact_r.para));
    }
    if (datasizepkt) {
      if (dataoffsetpkt < dataoffset) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      dataoffsetpkt -= dataoffset;
      if ((netwib_uint32)dataoffsetpkt + datasizepkt > datasize) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib_er(netwib_buf_init_ext_arrayfilled(data+dataoffsetpkt, datasizepkt, &psmbcmd->value.nttransact_r.data));
      skipsize = dataoffsetpkt + datasizepkt;
      data += skipsize;
      datasize -= skipsize;
      dataoffset += skipsize;
    } else {
      netwib_er(netwib_buf_init_ext_empty(&psmbcmd->value.nttransact_r.data));
    }
    if (datasize > 3/* sometimes ends with padding to be aligned */) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_NTTRANSACT_Q));
    if (parasize < NETWOX_SMBCMD_NTTRANSACTQ_COMMONSIZE) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint8_le(para, psmbcmd->value.nttransact_q.maxsetupcount);
    para += 2; /* reserved */
    netwib__data_decode_uint32_le(para, psmbcmd->value.nttransact_q.totalparasize);
    netwib__data_decode_uint32_le(para, psmbcmd->value.nttransact_q.totaldatasize);
    netwib__data_decode_uint32_le(para, psmbcmd->value.nttransact_q.maxparasize);
    netwib__data_decode_uint32_le(para, psmbcmd->value.nttransact_q.maxdatasize);
    netwib__data_decode_uint32_le(para, parasizepkt);
    netwib__data_decode_uint32_le(para, paraoffsetpkt);
    netwib__data_decode_uint32_le(para, datasizepkt);
    netwib__data_decode_uint32_le(para, dataoffsetpkt);
    netwib__data_decode_uint8_le(para, psmbcmd->value.nttransact_q.setupcount);
    if (psmbcmd->value.nttransact_q.setupcount > NETWOX_SMBCMD_NTTRANSACT_SETUP_MAX) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwib__data_decode_uint16_le(para, psmbcmd->value.nttransact_q.subcmd);
    if (parasize != NETWOX_SMBCMD_NTTRANSACTQ_COMMONSIZE + 2*psmbcmd->value.nttransact_q.setupcount) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    for (i = 0; i < psmbcmd->value.nttransact_q.setupcount; i++) {
      netwib__data_decode_uint16_le(para, psmbcmd->value.nttransact_q.setup[i]);
    }
    if (idxcmd != 0) {
      /* because of offset, only know how to decode if alone */
      return(NETWIB_ERR_NOTCONVERTED);
    }
    dataoffset = NETWOX_SMBMSGHDR_LEN + 1 + NETWOX_SMBCMD_NTTRANSACTQ_COMMONSIZE + 2*psmbcmd->value.nttransact_q.setupcount + 2;
    if (parasizepkt) {
      if (paraoffsetpkt < dataoffset) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      paraoffsetpkt -= dataoffset;
      if ((netwib_uint32)paraoffsetpkt + parasizepkt > datasize) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib_er(netwib_buf_init_ext_arrayfilled(data+paraoffsetpkt, parasizepkt, &psmbcmd->value.nttransact_q.para));
      skipsize = paraoffsetpkt + parasizepkt;
      data += skipsize;
      datasize -= skipsize;
      dataoffset += skipsize;
    } else {
      netwib_er(netwib_buf_init_ext_empty(&psmbcmd->value.nttransact_q.para));
    }
    if (datasizepkt) {
      if (dataoffsetpkt < dataoffset) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      dataoffsetpkt -= dataoffset;
      if ((netwib_uint32)dataoffsetpkt + datasizepkt > datasize) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib_er(netwib_buf_init_ext_arrayfilled(data+dataoffsetpkt, datasizepkt, &psmbcmd->value.nttransact_q.data));
      skipsize = dataoffsetpkt + datasizepkt;
      data += skipsize;
      datasize -= skipsize;
      dataoffset += skipsize;
    } else {
      netwib_er(netwib_buf_init_ext_empty(&psmbcmd->value.nttransact_q.data));
    }
    if (datasize > 3/* sometimes ends with padding to be aligned */) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    psmbcmd->value.nttransact_q.isunicode = psmbmsg->hdr.flags2 & NETWOX_SMBMSG_FLAGS2_UNICODESTRINGS;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_ntcreateandx(netwox_constsmbmsg *psmbmsg,
                                                       netwib_uint32 idxcmd,
                                                       netwox_smbcmd *psmbcmd)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize;
  netwib_uint16 filenamesize;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_NTCREATEANDX_R));
    if (parasize != 64) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwox__smbcmdcmn_oplocklevel_decode(para, psmbcmd->value.ntcreateandx_r.oplocklevel);
    netwib__data_decode_uint16_le(para, psmbcmd->value.ntcreateandx_r.fileid);
    netwox__smbcmdcmn_createaction_decode(para, psmbcmd->value.ntcreateandx_r.createaction);
    netwox__smbcmdcmn_time1601_decode(para, psmbcmd->value.ntcreateandx_r.creationtime);
    netwox__smbcmdcmn_time1601_decode(para, psmbcmd->value.ntcreateandx_r.lastaccesstime);
    netwox__smbcmdcmn_time1601_decode(para, psmbcmd->value.ntcreateandx_r.lastwritetime);
    netwox__smbcmdcmn_time1601_decode(para, psmbcmd->value.ntcreateandx_r.changetime);
    netwox__smbcmdcmn_fileattr32_decode(para, psmbcmd->value.ntcreateandx_r.fileattributes);
    netwox__smbcmdcmn_uint64_decode(para, psmbcmd->value.ntcreateandx_r.allocationsize);
    netwox__smbcmdcmn_uint64_decode(para, psmbcmd->value.ntcreateandx_r.filesize);
    netwox__smbcmdcmn_filetype_decode(para, psmbcmd->value.ntcreateandx_r.filetype);
    netwib__data_decode_uint16_le(para, psmbcmd->value.ntcreateandx_r.ipcstate);
    netwib__data_decode_uint8_le(para, psmbcmd->value.ntcreateandx_r.isdirectory);
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_NTCREATEANDX_Q));
    if (parasize != 44) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    para++; /* reserved byte */
    netwib__data_decode_uint16_le(para, filenamesize);
    netwox__smbcmdcmn_openflags32_decode(para, psmbcmd->value.ntcreateandx_q.openflags);
    netwib__data_decode_uint32_le(para, psmbcmd->value.ntcreateandx_q.rootfileid);
    netwox__smbcmdcmn_accessmask_decode(para, psmbcmd->value.ntcreateandx_q.accessmask);
    netwox__smbcmdcmn_uint64_decode(para, psmbcmd->value.ntcreateandx_q.allocationsize);
    netwox__smbcmdcmn_fileattr32_decode(para, psmbcmd->value.ntcreateandx_q.fileattributes);
    netwox__smbcmdcmn_shareaccess_decode(para, psmbcmd->value.ntcreateandx_q.shareaccess);
    netwox__smbcmdcmn_createfunction_decode(para, psmbcmd->value.ntcreateandx_q.createfunction);
    netwox__smbcmdcmn_createopt_decode(para, psmbcmd->value.ntcreateandx_q.createoptions);
    netwib__data_decode_uint32_le(para, psmbcmd->value.ntcreateandx_q.impersonation);
    netwox__smbcmdcmn_secuflags_decode(para, psmbcmd->value.ntcreateandx_q.secuflags);
    /* if Unicode, filename begins with a padding alignment byte */
    if (psmbmsg->hdr.flags2 & NETWOX_SMBMSG_FLAGS2_UNICODESTRINGS) {
      if (datasize == 0) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      data++;
      datasize--;
    }
    netwib_er(netwox_smbcmdcmn_data_decodeext(&data, &datasize, filenamesize, &psmbcmd->value.ntcreateandx_q.filename));
    /* filename ends with an optional \0, or \0\0 for Unicode */
    if (psmbmsg->hdr.flags2 & NETWOX_SMBMSG_FLAGS2_UNICODESTRINGS) {
      if (datasize >= 2 && data[0] == '\0' && data[1] == '\0') {
        data += 2;
        datasize -= 2;
      }
    } else {
      if (datasize >= 1 && data[0] == '\0') {
        data++;
        datasize--;
      }
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_ntcancel(netwox_constsmbmsg *psmbmsg,
                                                       netwib_uint32 idxcmd,
                                                       netwox_smbcmd *psmbcmd)
{
  netwib_uint32 parasize, datasize;

  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_NTCANCEL_R));
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_NTCANCEL_Q));
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbmsg_decode_smbcmd_ntrename(netwox_constsmbmsg *psmbmsg,
                                                       netwib_uint32 idxcmd,
                                                       netwox_smbcmd *psmbcmd)
{
  netwib_data para, data;
  netwib_uint32 parasize, datasize;

  para = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].para);
  parasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].para);
  data = netwib__buf_ref_data_ptr(&psmbmsg->cmds[idxcmd].data);
  datasize = netwib__buf_ref_data_size(&psmbmsg->cmds[idxcmd].data);

  if (psmbmsg->hdr.flags & NETWOX_SMBMSG_FLAGS_REPLY) {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_NTRENAME_R));
    if (parasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else {
    netwib_er(netwox_smbcmd_selecttype(psmbcmd, NETWOX_SMBCMD_TYPE_NTRENAME_Q));
    if (parasize != 8) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    netwox__smbcmdcmn_fileattr16_decode(para, psmbcmd->value.ntrename_q.searchattributes);
    netwib__data_decode_uint16_le(para, psmbcmd->value.ntrename_q.informationlevel);
    netwib__data_decode_uint32_le(para, psmbcmd->value.ntrename_q.clustercount);
    netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &data, &datasize, &psmbcmd->value.ntrename_q.oldname));
    netwib_er(netwox_smbcmdcmn_data_decode(psmbmsg, idxcmd, NETWOX_SMBCMDCMN_DATATYPE_ASCII, &data, &datasize, &psmbcmd->value.ntrename_q.newname));
    if (datasize) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbmsg_decode_smbcmd(netwox_constsmbmsg *psmbmsg,
                                       netwib_uint32 idxcmd,
                                       netwox_smbcmd *psmbcmd)
{

  if (idxcmd >= NETWOX_SMBMSG_ANDX_MAX) {
    return(NETWIB_ERR_PATOOHIGH);
  }
  if (idxcmd >= psmbmsg->numcmds) {
    return(NETWIB_ERR_PATOOHIGH);
  }

  switch(psmbmsg->cmds[idxcmd].cmd) {
  case NETWOX_SMBMSG_CMD_CREATEDIRECTORY :
    netwib_er(netwox_smbmsg_decode_smbcmd_createdirectory(psmbmsg, idxcmd,
                                                          psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_DELETEDIRECTORY :
    netwib_er(netwox_smbmsg_decode_smbcmd_deletedirectory(psmbmsg, idxcmd,
                                                          psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_CREATE :
    netwib_er(netwox_smbmsg_decode_smbcmd_create(psmbmsg, idxcmd, psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_OPEN :
    netwib_er(netwox_smbmsg_decode_smbcmd_open(psmbmsg, idxcmd, psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_CLOSE :
    netwib_er(netwox_smbmsg_decode_smbcmd_close(psmbmsg, idxcmd, psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_FLUSH :
    netwib_er(netwox_smbmsg_decode_smbcmd_flush(psmbmsg, idxcmd, psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_DELETE :
    netwib_er(netwox_smbmsg_decode_smbcmd_delete(psmbmsg, idxcmd, psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_RENAME :
    netwib_er(netwox_smbmsg_decode_smbcmd_rename(psmbmsg, idxcmd, psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_QUERYINFORMATION :
    netwib_er(netwox_smbmsg_decode_smbcmd_queryinformation(psmbmsg, idxcmd,
                                                           psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_SETINFORMATION :
    netwib_er(netwox_smbmsg_decode_smbcmd_setinformation(psmbmsg, idxcmd,
                                                         psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_READ :
    netwib_er(netwox_smbmsg_decode_smbcmd_read(psmbmsg, idxcmd, psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_WRITE :
    netwib_er(netwox_smbmsg_decode_smbcmd_write(psmbmsg, idxcmd, psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_CREATETEMPORARY :
    netwib_er(netwox_smbmsg_decode_smbcmd_createtemporary(psmbmsg, idxcmd,
                                                          psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_CREATENEW :
    netwib_er(netwox_smbmsg_decode_smbcmd_createnew(psmbmsg, idxcmd,
                                                    psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_CHECKDIRPATH :
    netwib_er(netwox_smbmsg_decode_smbcmd_checkdirpath(psmbmsg, idxcmd,
                                                       psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_SEEK :
    netwib_er(netwox_smbmsg_decode_smbcmd_seek(psmbmsg, idxcmd, psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_READRAW :
    netwib_er(netwox_smbmsg_decode_smbcmd_readraw(psmbmsg, idxcmd, psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_WRITERAW :
    netwib_er(netwox_smbmsg_decode_smbcmd_writeraw(psmbmsg, idxcmd, psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_SETINFORMATION2 :
    netwib_er(netwox_smbmsg_decode_smbcmd_setinformation2(psmbmsg, idxcmd,
                                                          psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_QUERYINFORMATION2 :
    netwib_er(netwox_smbmsg_decode_smbcmd_queryinformation2(psmbmsg, idxcmd,
                                                            psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_LOCKINGANDX :
    netwib_er(netwox_smbmsg_decode_smbcmd_lockingandx(psmbmsg, idxcmd,
                                                      psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_TRANSACTION :
    netwib_er(netwox_smbmsg_decode_smbcmd_transaction(psmbmsg, idxcmd,
                                                      psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_ECHO :
    netwib_er(netwox_smbmsg_decode_smbcmd_echo(psmbmsg, idxcmd, psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_OPENANDX :
    netwib_er(netwox_smbmsg_decode_smbcmd_openandx(psmbmsg, idxcmd, psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_READANDX :
    netwib_er(netwox_smbmsg_decode_smbcmd_readandx(psmbmsg, idxcmd, psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_WRITEANDX :
    netwib_er(netwox_smbmsg_decode_smbcmd_writeandx(psmbmsg, idxcmd, psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_CLOSETREEDISCONNECT :
    netwib_er(netwox_smbmsg_decode_smbcmd_closetreedisconnect(psmbmsg, idxcmd, psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_TRANSACTION2 :
    netwib_er(netwox_smbmsg_decode_smbcmd_transaction2(psmbmsg, idxcmd,
                                                       psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_FINDCLOSE2 :
    netwib_er(netwox_smbmsg_decode_smbcmd_findclose2(psmbmsg, idxcmd,
                                                     psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_TREEDISCONNECT :
    netwib_er(netwox_smbmsg_decode_smbcmd_treedisconnect(psmbmsg, idxcmd,
                                                         psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_NEGOTIATE :
    netwib_er(netwox_smbmsg_decode_smbcmd_negotiate(psmbmsg, idxcmd, psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_SESSIONSETUPANDX :
    netwib_er(netwox_smbmsg_decode_smbcmd_sessionsetupandx(psmbmsg, idxcmd,
                                                           psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_LOGOFFANDX :
    netwib_er(netwox_smbmsg_decode_smbcmd_logoffandx(psmbmsg, idxcmd,
                                                     psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_TREECONNECTANDX :
    netwib_er(netwox_smbmsg_decode_smbcmd_treeconnectandx(psmbmsg, idxcmd,
                                                          psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_NTTRANSACT :
    netwib_er(netwox_smbmsg_decode_smbcmd_nttransact(psmbmsg, idxcmd,
                                                     psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_QUERYINFORMATIONDISK :
    netwib_er(netwox_smbmsg_decode_smbcmd_queryinformationdisk(psmbmsg, idxcmd,
                                                               psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_SEARCH :
    netwib_er(netwox_smbmsg_decode_smbcmd_search(psmbmsg, idxcmd, psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_NTCREATEANDX :
    netwib_er(netwox_smbmsg_decode_smbcmd_ntcreateandx(psmbmsg, idxcmd,
                                                       psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_NTCANCEL :
    netwib_er(netwox_smbmsg_decode_smbcmd_ntcancel(psmbmsg, idxcmd, psmbcmd));
    break;
  case NETWOX_SMBMSG_CMD_NTRENAME :
    netwib_er(netwox_smbmsg_decode_smbcmd_ntrename(psmbmsg, idxcmd, psmbcmd));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

