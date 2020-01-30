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
netwib_err netwox_smberr_dos_show(netwib_uint8 cl,
                                       netwib_uint16 code,
                                       netwib_buf *pbuf)
{
  netwib_conststring pcl = "unknown";
  netwib_conststring pcode = "unknown";

  switch(cl) {
  case NETWOX_SMBERR_DOS_CLASS_OK :
    pcl = "ok";
    switch(code) {
    case NETWOX_SMBERR_DOS_CODE_OK_OK :
      pcode = "ok";
      break;
    }
    break;
  case NETWOX_SMBERR_DOS_CLASS_DOS :
    pcl = "dos_error";
    switch(code) {
    case NETWOX_SMBERR_DOS_CODE_DOS_NOTIMPLEMENTED :
      pcode = "not_implemented";
      break;
    case NETWOX_SMBERR_DOS_CODE_DOS_FILENOTFOUND :
      pcode = "file_not_found";
      break;
    case NETWOX_SMBERR_DOS_CODE_DOS_PATHNOTFOUND :
      pcode = "path_not_found";
      break;
    case NETWOX_SMBERR_DOS_CODE_DOS_TOOMANYOPENFILES :
      pcode = "too_many_open_files";
      break;
    case NETWOX_SMBERR_DOS_CODE_DOS_ACCESSDENIED :
      pcode = "access_denied/operation_not_possible";
      break;
    case NETWOX_SMBERR_DOS_CODE_DOS_BADFILEID :
      pcode = "bad_file_id";
      break;
    case NETWOX_SMBERR_DOS_CODE_DOS_NOTENOUGHMEMORY :
      pcode = "not_enough_memory";
      break;
    case NETWOX_SMBERR_DOS_CODE_DOS_BADDATA :
      pcode = "bad_data";
      break;
    case NETWOX_SMBERR_DOS_CODE_DOS_DIRNOTEMPTY :
      pcode = "directory_no_empty";
      break;
    case NETWOX_SMBERR_DOS_CODE_DOS_CROSSDEVICE :
      pcode = "crossing_device";
      break;
    case NETWOX_SMBERR_DOS_CODE_DOS_NOMOREFILES :
      pcode = "no_more_files";
      break;
    case NETWOX_SMBERR_DOS_CODE_DOS_FILEEXISTS :
      pcode = "file_exists";
      break;
    case NETWOX_SMBERR_DOS_CODE_DOS_INVALIDPARAMETER :
      pcode = "invalid_parameter";
      break;
    case NETWOX_SMBERR_DOS_CODE_DOS_INVALIDNAME :
      pcode = "invalid_name";
      break;
    case NETWOX_SMBERR_DOS_CODE_DOS_QUOTAEXCEEDED :
      pcode = "quota_exceeded";
      break;
    }
    break;
  case NETWOX_SMBERR_DOS_CLASS_SRV :
    pcl = "server_error";
    switch(code) {
    case NETWOX_SMBERR_DOS_CODE_SRV_UNKNOWN :
      pcode = "no_detail_available";
      break;
    case NETWOX_SMBERR_DOS_CODE_SRV_BADPASSWORD :
      pcode = "bad_password";
      break;
    case NETWOX_SMBERR_DOS_CODE_SRV_NOPERM :
      pcode = "no_permission";
      break;
    case NETWOX_SMBERR_DOS_CODE_SRV_BADSHARENAME :
      pcode = "bad_share_name";
      break;
    case NETWOX_SMBERR_DOS_CODE_SRV_BADUSERID :
      pcode = "bad_user_id";
      break;
    case NETWOX_SMBERR_DOS_CODE_SRV_NOTSUPPORTED :
      pcode = "not_supported";
      break;
    }
    break;
  case NETWOX_SMBERR_DOS_CLASS_HARD :
    pcl = "hardware_error";
    switch(code) {
    case NETWOX_SMBERR_DOS_CODE_HARD_DEVICEWRITEPROTECTED :
      pcode = "device_write_protected";
      break;
    case NETWOX_SMBERR_DOS_CODE_HARD_DEVICENOTREADY :
      pcode = "device_not_ready";
      break;
    }
    break;
  case NETWOX_SMBERR_DOS_CLASS_FMT :
    pcl = "smb_packet_format_error";
    break;
  }

  netwib_er(netwib_buf_append_fmt(pbuf, "class=%{uint8}=%s, code=%{uint16}=%s", cl, pcl, code, pcode));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smberr_nt_show(netwib_uint8 level,
                                      netwib_uint16 code,
                                      netwib_buf *pbuf)
{
  netwib_conststring plevel = "unknown";
  netwib_conststring pcode = "unknown";

  switch(level) {
  case NETWOX_SMBERR_NT_LEVEL_OK :
    plevel = "ok";
    switch(code) {
    case NETWOX_SMBERR_NT_CODE_OK_OK :
      pcode = "ok";
      break;
    case NETWOX_SMBERR_NT_CODE_OK_WAIT2 :
      pcode = "wait2";
      break;
    case NETWOX_SMBERR_NT_CODE_OK_NOTIFYENUMDIR :
      pcode = "notify_enum_dir";
      break;
    }
    break;
  case NETWOX_SMBERR_NT_LEVEL_INFO :
    plevel = "info";
    break;
  case NETWOX_SMBERR_NT_LEVEL_WARNING :
    plevel = "warning";
    switch(code) {
    case NETWOX_SMBERR_NT_CODE_WARNING_NOMOREFILES :
      pcode = "no_more_files";
      break;
    }
    break;
  case NETWOX_SMBERR_NT_LEVEL_ERROR :
    plevel = "error";
    switch(code) {
    case NETWOX_SMBERR_NT_CODE_ERROR_NOTIMPLEMENTED :
      pcode = "not_implemented";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_BADFILEID :
      pcode = "bad_file_id";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_NOSUCHDEVICE :
      pcode = "no_such_device";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_INVALIDPARAMETER :
      pcode = "invalid_parameter";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_FILENOTFOUND :
      pcode = "file_not_found";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_MOREPROCESSINGREQUIRED :
      pcode = "more_processing_required";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_ACCESSDENIED :
      pcode = "access_denied";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_BUFFERTOOSMALL :
      pcode = "buffer_too_small";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_NAMENOTFOUND :
      pcode = "name_not_found";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_INVALIDNAME :
      pcode = "invalid_name";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_PATHNOTFOUND :
      pcode = "path_not_found";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_BADPATHSYNTAX :
      pcode = "bad_path_syntax";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_BADDATA :
      pcode = "bad_data";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_SHARINGVIOLATION :
      pcode = "sharing_violation";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_QUOTAEXCEEDED :
      pcode = "quota_exceeded";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_PATHISADIR :
      pcode = "path_is_a_directory";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_CROSSDEVICE :
      pcode = "crossing_device";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_BADPASSWORD :
      pcode = "bad_password";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_LOGONFAILURE :
      pcode = "logon_failure";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_NOTENOUGHMEMORY :
      pcode = "not_enough_memory";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_DEVICEWRITEPROTECTED :
      pcode = "device_write_protected";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_DEVICENOTREADY :
      pcode = "device_not_ready";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_NOTSUPPORTED :
      pcode = "not_supported";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_BADSHARENAME :
      pcode = "bad_share_name";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_DIRNOTEMPTY :
      pcode = "directory_not_empty";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_NOTADIR :
      pcode = "not_a_directory";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_CANCELED :
      pcode = "canceled";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_INVALIDLEVEL :
      pcode = "invalid_level";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_TOOMANYOPENFILES :
      pcode = "too_many_open_files";
      break;
    case NETWOX_SMBERR_NT_CODE_ERROR_NOTAREPARSEPOINT :
      pcode = "not_a_reparse_point";
      break;
    }
    break;
  }

  netwib_er(netwib_buf_append_fmt(pbuf, "level=%{uint8}=%s, code=%{uint16}=%s", level, plevel, code, pcode));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smberr_init(netwox_smberr smberr,
                              netwib_bool isnt,
                              netwib_uint32 *perr)
{

  switch(smberr) {
  case NETWOX_SMBERR_INTERNALERROR :
    if (!isnt) {
      *perr = netwox_smberr_dos_classcode(NETWOX_SMBERR_DOS_CLASS_DOS,
                           NETWOX_SMBERR_DOS_CODE_DOS_NOTENOUGHMEMORY);
    } else {
      *perr = netwox_smberr_nt_levelcode(NETWOX_SMBERR_NT_LEVEL_ERROR,
                           NETWOX_SMBERR_NT_CODE_ERROR_NOTENOUGHMEMORY);
    }
    break;
  case NETWOX_SMBERR_OK :
    if (!isnt) {
      *perr = netwox_smberr_dos_classcode(NETWOX_SMBERR_DOS_CLASS_OK,
                                          NETWOX_SMBERR_DOS_CODE_OK_OK);
    } else {
      *perr = netwox_smberr_nt_levelcode(NETWOX_SMBERR_NT_LEVEL_OK,
                                         NETWOX_SMBERR_NT_CODE_OK_OK);
    }
    break;
  case NETWOX_SMBERR_NOTIMPLEMENTED :
    if (!isnt) {
      *perr = netwox_smberr_dos_classcode(NETWOX_SMBERR_DOS_CLASS_DOS,
                           NETWOX_SMBERR_DOS_CODE_DOS_NOTIMPLEMENTED);
    } else {
      *perr = netwox_smberr_nt_levelcode(NETWOX_SMBERR_NT_LEVEL_ERROR,
                           NETWOX_SMBERR_NT_CODE_ERROR_NOTIMPLEMENTED);
    }
    break;
  case NETWOX_SMBERR_NOTSUPPORTED :
    if (!isnt) {
      *perr = netwox_smberr_dos_classcode(NETWOX_SMBERR_DOS_CLASS_SRV,
                           NETWOX_SMBERR_DOS_CODE_SRV_NOTSUPPORTED);
    } else {
      *perr = netwox_smberr_nt_levelcode(NETWOX_SMBERR_NT_LEVEL_ERROR,
                           NETWOX_SMBERR_NT_CODE_ERROR_NOTSUPPORTED);
    }
    break;
  case NETWOX_SMBERR_BADSHARENAME :
    if (!isnt) {
      *perr = netwox_smberr_dos_classcode(NETWOX_SMBERR_DOS_CLASS_SRV,
                           NETWOX_SMBERR_DOS_CODE_SRV_BADSHARENAME);
    } else {
      *perr = netwox_smberr_nt_levelcode(NETWOX_SMBERR_NT_LEVEL_ERROR,
                           NETWOX_SMBERR_NT_CODE_ERROR_BADSHARENAME);
    }
    break;
  case NETWOX_SMBERR_BADPASSWORD :
    if (!isnt) {
      *perr = netwox_smberr_dos_classcode(NETWOX_SMBERR_DOS_CLASS_SRV,
                           NETWOX_SMBERR_DOS_CODE_SRV_BADPASSWORD);
    } else {
      *perr = netwox_smberr_nt_levelcode(NETWOX_SMBERR_NT_LEVEL_ERROR,
                           NETWOX_SMBERR_NT_CODE_ERROR_BADPASSWORD);
    }
    break;
  case NETWOX_SMBERR_BADPARAMETER :
    if (!isnt) {
      *perr = netwox_smberr_dos_classcode(NETWOX_SMBERR_DOS_CLASS_DOS,
                           NETWOX_SMBERR_DOS_CODE_DOS_INVALIDPARAMETER);
    } else {
      *perr = netwox_smberr_nt_levelcode(NETWOX_SMBERR_NT_LEVEL_ERROR,
                           NETWOX_SMBERR_NT_CODE_ERROR_INVALIDPARAMETER);
    }
    break;
  case NETWOX_SMBERR_BADNAME :
    if (!isnt) {
      *perr = netwox_smberr_dos_classcode(NETWOX_SMBERR_DOS_CLASS_DOS,
                           NETWOX_SMBERR_DOS_CODE_DOS_INVALIDNAME);
    } else {
      *perr = netwox_smberr_nt_levelcode(NETWOX_SMBERR_NT_LEVEL_ERROR,
                           NETWOX_SMBERR_NT_CODE_ERROR_INVALIDNAME);
    }
    break;
  case NETWOX_SMBERR_BADFILEID :
    if (!isnt) {
      *perr = netwox_smberr_dos_classcode(NETWOX_SMBERR_DOS_CLASS_DOS,
                           NETWOX_SMBERR_DOS_CODE_DOS_BADFILEID);
    } else {
      *perr = netwox_smberr_nt_levelcode(NETWOX_SMBERR_NT_LEVEL_ERROR,
                           NETWOX_SMBERR_NT_CODE_ERROR_BADFILEID);
    }
    break;
  case NETWOX_SMBERR_FILENOTFOUND :
    if (!isnt) {
      *perr = netwox_smberr_dos_classcode(NETWOX_SMBERR_DOS_CLASS_DOS,
                           NETWOX_SMBERR_DOS_CODE_DOS_FILENOTFOUND);
    } else {
      *perr = netwox_smberr_nt_levelcode(NETWOX_SMBERR_NT_LEVEL_ERROR,
                           NETWOX_SMBERR_NT_CODE_ERROR_FILENOTFOUND);
    }
    break;
  case NETWOX_SMBERR_FILEEXISTS :
    if (!isnt) {
      *perr = netwox_smberr_dos_classcode(NETWOX_SMBERR_DOS_CLASS_DOS,
                           NETWOX_SMBERR_DOS_CODE_DOS_FILEEXISTS);
    } else {
      *perr = netwox_smberr_nt_levelcode(NETWOX_SMBERR_NT_LEVEL_ERROR,
                           NETWOX_SMBERR_NT_CODE_ERROR_FILEEXISTS);
    }
    break;
  case NETWOX_SMBERR_NOTADIR :
    if (!isnt) {
      *perr = netwox_smberr_dos_classcode(NETWOX_SMBERR_DOS_CLASS_DOS,
                           NETWOX_SMBERR_DOS_CODE_DOS_FILENOTFOUND);
    } else {
      *perr = netwox_smberr_nt_levelcode(NETWOX_SMBERR_NT_LEVEL_ERROR,
                           NETWOX_SMBERR_NT_CODE_ERROR_NOTADIR);
    }
    break;
  case NETWOX_SMBERR_NOTAFILE :
    if (!isnt) {
      *perr = netwox_smberr_dos_classcode(NETWOX_SMBERR_DOS_CLASS_DOS,
                           NETWOX_SMBERR_DOS_CODE_DOS_FILENOTFOUND);
    } else {
      *perr = netwox_smberr_nt_levelcode(NETWOX_SMBERR_NT_LEVEL_ERROR,
                           NETWOX_SMBERR_NT_CODE_ERROR_PATHISADIR);
    }
    break;
  case NETWOX_SMBERR_QUOTAEXCEEDED :
    if (!isnt) {
      *perr = netwox_smberr_dos_classcode(NETWOX_SMBERR_DOS_CLASS_DOS,
                           NETWOX_SMBERR_DOS_CODE_DOS_QUOTAEXCEEDED);
    } else {
      *perr = netwox_smberr_nt_levelcode(NETWOX_SMBERR_NT_LEVEL_ERROR,
                           NETWOX_SMBERR_NT_CODE_ERROR_QUOTAEXCEEDED);
    }
    break;
  case NETWOX_SMBERR_DEVICEWRITEPROTECTED :
    if (!isnt) {
      *perr = netwox_smberr_dos_classcode(NETWOX_SMBERR_DOS_CLASS_HARD,
                           NETWOX_SMBERR_DOS_CODE_HARD_DEVICEWRITEPROTECTED);
    } else {
      *perr = netwox_smberr_nt_levelcode(NETWOX_SMBERR_NT_LEVEL_ERROR,
                           NETWOX_SMBERR_NT_CODE_ERROR_DEVICEWRITEPROTECTED);
    }
    break;
  case NETWOX_SMBERR_NOTAREPARSEPOINT :
    if (!isnt) {
      *perr = netwox_smberr_dos_classcode(NETWOX_SMBERR_DOS_CLASS_DOS,
                           NETWOX_SMBERR_DOS_CODE_DOS_NOTIMPLEMENTED);
    } else {
      *perr = netwox_smberr_nt_levelcode(NETWOX_SMBERR_NT_LEVEL_ERROR,
                           NETWOX_SMBERR_NT_CODE_ERROR_NOTAREPARSEPOINT);
    }
    break;
  case NETWOX_SMBERR_NOMOREFILES :
    if (!isnt) {
      *perr = netwox_smberr_dos_classcode(NETWOX_SMBERR_DOS_CLASS_DOS,
                           NETWOX_SMBERR_DOS_CODE_DOS_NOMOREFILES);
    } else {
      *perr = netwox_smberr_nt_levelcode(NETWOX_SMBERR_NT_LEVEL_ERROR,
                           NETWOX_SMBERR_NT_CODE_ERROR_FILENOTFOUND);
    }
    break;
  default :
    return(NETWIB_ERR_LOINTERNALERROR);
  }

  return(NETWIB_ERR_OK);
}
