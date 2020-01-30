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
typedef enum {
  NETWOX_WEBSPIDERCF_VAR_UNKNOWN = 1,
  NETWOX_WEBSPIDERCF_VAR_LOCALROOTDIR,
  NETWOX_WEBSPIDERCF_VAR_URLTODOWNLOAD,
  NETWOX_WEBSPIDERCF_VAR_PROXYHOST,
  NETWOX_WEBSPIDERCF_VAR_PROXYPORT,
  NETWOX_WEBSPIDERCF_VAR_PROXYUSER,
  NETWOX_WEBSPIDERCF_VAR_PROXYPASS,
  NETWOX_WEBSPIDERCF_VAR_MAXRECURSION,
  NETWOX_WEBSPIDERCF_VAR_MAXTHREADS,
  NETWOX_WEBSPIDERCF_VAR_MILLISLEEP,
  NETWOX_WEBSPIDERCF_VAR_ACL,
  NETWOX_WEBSPIDERCF_VAR_GETIMAGE,
  NETWOX_WEBSPIDERCF_VAR_GETMULTIMEDIA,
  NETWOX_WEBSPIDERCF_VAR_GETCODE,
  NETWOX_WEBSPIDERCF_VAR_LINKCONVERSION,
  NETWOX_WEBSPIDERCF_VAR_USERAGENT,
  NETWOX_WEBSPIDERCF_VAR_LOGLEVEL,
  NETWOX_WEBSPIDERCF_VAR_FTPPASSIVE,
  NETWOX_WEBSPIDERCF_VAR_TIMEOUTMS,
  NETWOX_WEBSPIDERCF_VAR_MIMETYPES,
  NETWOX_WEBSPIDERCF_VAR_MIMETYPESWARN,
  NETWOX_WEBSPIDERCF_VAR_DELETEDEXTENSIONS
} netwox_webspidercf_var;
#define NETWOX_WEBSPIDERCF_VARNAME_LOCALROOTDIR "localrootdir"
#define NETWOX_WEBSPIDERCF_VARNAME_URLTODOWNLOAD "urltodownload"
#define NETWOX_WEBSPIDERCF_VARNAME_PROXYHOST "proxyhost"
#define NETWOX_WEBSPIDERCF_VARNAME_PROXYPORT "proxyport"
#define NETWOX_WEBSPIDERCF_VARNAME_PROXYUSER "proxyuser"
#define NETWOX_WEBSPIDERCF_VARNAME_PROXYPASS "proxypass"
#define NETWOX_WEBSPIDERCF_VARNAME_MAXRECURSION "maxrecursion"
#define NETWOX_WEBSPIDERCF_VARNAME_MAXTHREADS "maxthreads"
#define NETWOX_WEBSPIDERCF_VARNAME_MILLISLEEP "millisleep"
#define NETWOX_WEBSPIDERCF_VARNAME_ACL "acl"
#define NETWOX_WEBSPIDERCF_VARNAME_GETIMAGE "getimage"
#define NETWOX_WEBSPIDERCF_VARNAME_GETMULTIMEDIA "getmultimedia"
#define NETWOX_WEBSPIDERCF_VARNAME_GETCODE "getcode"
#define NETWOX_WEBSPIDERCF_VARNAME_LINKCONVERSION "linkconversion"
#define NETWOX_WEBSPIDERCF_VARNAME_USERAGENT "useragent"
#define NETWOX_WEBSPIDERCF_VARNAME_LOGLEVEL "loglevel"
#define NETWOX_WEBSPIDERCF_VARNAME_FTPPASSIVE "ftppassive"
#define NETWOX_WEBSPIDERCF_VARNAME_TIMEOUTMS "timeoutms"
#define NETWOX_WEBSPIDERCF_VARNAME_MIMETYPES "mimetypes"
#define NETWOX_WEBSPIDERCF_VARNAME_MIMETYPESWARN "mimetypeswarn"
#define NETWOX_WEBSPIDERCF_VARNAME_DELETEDEXTENSIONS "deletedextensions"

/*-------------------------------------------------------------*/
static netwib_err netwox_webspidercf_var_init_buf(netwib_constbuf *pbuf,
                                                  netwox_webspidercf_var *pvar)
{
  netwib_data data;
  netwib_uint32 datasize;
  netwib_byte firstchar;

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);

  if (datasize == 0) {
    if (pvar != NULL) *pvar = NETWOX_WEBSPIDERCF_VAR_UNKNOWN;
    return(NETWIB_ERR_OK);
  }

  firstchar = data[0];
#define netwox_webspidercf_var_init_buf_text(item,v) if (!netwib_c_memcmp(data, (netwib_constdata)item, datasize)) { if (pvar != NULL) *pvar = v; return(NETWIB_ERR_OK);}
  switch(firstchar) {
  case 'a' :
    if (datasize == 3) {
      netwox_webspidercf_var_init_buf_text(NETWOX_WEBSPIDERCF_VARNAME_ACL, NETWOX_WEBSPIDERCF_VAR_ACL);
    }
    break;
  case 'd' :
    if (datasize == 17) {
      netwox_webspidercf_var_init_buf_text(NETWOX_WEBSPIDERCF_VARNAME_DELETEDEXTENSIONS, NETWOX_WEBSPIDERCF_VAR_DELETEDEXTENSIONS);
    }
    break;
  case 'f' :
    if (datasize == 10) {
      netwox_webspidercf_var_init_buf_text(NETWOX_WEBSPIDERCF_VARNAME_FTPPASSIVE, NETWOX_WEBSPIDERCF_VAR_FTPPASSIVE);
    }
    break;
  case 'g' :
    switch(datasize) {
    case 7 :
      netwox_webspidercf_var_init_buf_text(NETWOX_WEBSPIDERCF_VARNAME_GETCODE, NETWOX_WEBSPIDERCF_VAR_GETCODE);
      break;
    case 8 :
      netwox_webspidercf_var_init_buf_text(NETWOX_WEBSPIDERCF_VARNAME_GETIMAGE, NETWOX_WEBSPIDERCF_VAR_GETIMAGE);
      break;
    case 13 :
      netwox_webspidercf_var_init_buf_text(NETWOX_WEBSPIDERCF_VARNAME_GETMULTIMEDIA, NETWOX_WEBSPIDERCF_VAR_GETMULTIMEDIA);
      break;
    }
    break;
  case 'l' :
    switch(datasize) {
    case 8 :
      netwox_webspidercf_var_init_buf_text(NETWOX_WEBSPIDERCF_VARNAME_LOGLEVEL, NETWOX_WEBSPIDERCF_VAR_LOGLEVEL);
      break;
    case 12 :
      netwox_webspidercf_var_init_buf_text(NETWOX_WEBSPIDERCF_VARNAME_LOCALROOTDIR, NETWOX_WEBSPIDERCF_VAR_LOCALROOTDIR);
      break;
    case 14 :
      netwox_webspidercf_var_init_buf_text(NETWOX_WEBSPIDERCF_VARNAME_LINKCONVERSION, NETWOX_WEBSPIDERCF_VAR_LINKCONVERSION);
      break;
    }
    break;
  case 'm' :
    switch(datasize) {
    case 9 :
      netwox_webspidercf_var_init_buf_text(NETWOX_WEBSPIDERCF_VARNAME_MIMETYPES, NETWOX_WEBSPIDERCF_VAR_MIMETYPES);
      break;
    case 10 :
      netwox_webspidercf_var_init_buf_text(NETWOX_WEBSPIDERCF_VARNAME_MAXTHREADS, NETWOX_WEBSPIDERCF_VAR_MAXTHREADS);
      netwox_webspidercf_var_init_buf_text(NETWOX_WEBSPIDERCF_VARNAME_MILLISLEEP, NETWOX_WEBSPIDERCF_VAR_MILLISLEEP);
      break;
    case 12 :
      netwox_webspidercf_var_init_buf_text(NETWOX_WEBSPIDERCF_VARNAME_MAXRECURSION, NETWOX_WEBSPIDERCF_VAR_MAXRECURSION);
      break;
    case 13 :
      netwox_webspidercf_var_init_buf_text(NETWOX_WEBSPIDERCF_VARNAME_MIMETYPESWARN, NETWOX_WEBSPIDERCF_VAR_MIMETYPESWARN);
      break;
    }
    break;
  case 'p' :
    if (datasize == 9) {
      netwox_webspidercf_var_init_buf_text(NETWOX_WEBSPIDERCF_VARNAME_PROXYHOST, NETWOX_WEBSPIDERCF_VAR_PROXYHOST);
      netwox_webspidercf_var_init_buf_text(NETWOX_WEBSPIDERCF_VARNAME_PROXYPORT, NETWOX_WEBSPIDERCF_VAR_PROXYPORT);
      netwox_webspidercf_var_init_buf_text(NETWOX_WEBSPIDERCF_VARNAME_PROXYUSER, NETWOX_WEBSPIDERCF_VAR_PROXYUSER);
      netwox_webspidercf_var_init_buf_text(NETWOX_WEBSPIDERCF_VARNAME_PROXYPASS, NETWOX_WEBSPIDERCF_VAR_PROXYPASS);
    }
    break;
  case 't' :
    if (datasize == 9) {
      netwox_webspidercf_var_init_buf_text(NETWOX_WEBSPIDERCF_VARNAME_TIMEOUTMS, NETWOX_WEBSPIDERCF_VAR_TIMEOUTMS);
    }
    break;
  case 'u' :
    switch(datasize) {
    case 9 :
      netwox_webspidercf_var_init_buf_text(NETWOX_WEBSPIDERCF_VARNAME_USERAGENT, NETWOX_WEBSPIDERCF_VAR_USERAGENT);
      break;
    case 13 :
      netwox_webspidercf_var_init_buf_text(NETWOX_WEBSPIDERCF_VARNAME_URLTODOWNLOAD, NETWOX_WEBSPIDERCF_VAR_URLTODOWNLOAD);
      break;
    }
    break;
  }

  if (pvar != NULL) *pvar = NETWOX_WEBSPIDERCF_VAR_UNKNOWN;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_webspidercf_write_sample(netwib_constbuf *pfilename)
{
  netwib_io *pio;
  netwib_bufpool *pbufpool;
  netwib_buf cmt, var, val;

  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  netwib_er(netwox_io_init_conffile_write(pfilename, &pio));

  netwib_er(netwox_conffile_write_help(pbufpool, pio));

  netwib_er(netwib_buf_init_ext_string("Variable localrootdir contains directory name where files have to be saved", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string(NETWOX_WEBSPIDERCF_VARNAME_LOCALROOTDIR, &var));
  netwib_er(netwib_buf_init_ext_string(NETWOX_DIREX, &val));
  netwib_er(netwox_conffile_write_var(pbufpool, pio, &var, &val));
  netwib_er(netwox_conffile_write_empty(pio));

  netwib_er(netwib_buf_init_ext_string("Variable urltodownload contains url to download", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string(NETWOX_WEBSPIDERCF_VARNAME_URLTODOWNLOAD, &var));
  netwib_er(netwib_buf_init_ext_string("http://www.example.com/dir/page.html", &val));
  netwib_er(netwox_conffile_write_var(pbufpool, pio, &var, &val));
  netwib_er(netwox_conffile_write_empty(pio));

  netwib_er(netwib_buf_init_ext_string("Variables proxyuser, proxypass, proxyhost, proxyport contains proxy", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string("information (comment it if you do not have a proxy)", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string(NETWOX_WEBSPIDERCF_VARNAME_PROXYHOST, &var));
  netwib_er(netwib_buf_init_ext_string("proxy.example.com", &val));
  netwib_er(netwox_conffile_write_varcmt(pbufpool, pio, &var, &val));
  netwib_er(netwib_buf_init_ext_string(NETWOX_WEBSPIDERCF_VARNAME_PROXYPORT, &var));
  netwib_er(netwox_conffile_write_varcmt_uint32(pbufpool, pio, &var, 8080));
  netwib_er(netwib_buf_init_ext_string(NETWOX_WEBSPIDERCF_VARNAME_PROXYUSER, &var));
  netwib_er(netwib_buf_init_ext_string("joe", &val));
  netwib_er(netwox_conffile_write_varcmt(pbufpool, pio, &var, &val));
  netwib_er(netwib_buf_init_ext_string(NETWOX_WEBSPIDERCF_VARNAME_PROXYPASS, &var));
  netwib_er(netwib_buf_init_ext_string("mypwd", &val));
  netwib_er(netwox_conffile_write_varcmt(pbufpool, pio, &var, &val));
  netwib_er(netwox_conffile_write_empty(pio));

  netwib_er(netwib_buf_init_ext_string("Variable maxrecursion contains maximum recursion level", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string("(0 means no limit)", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string(NETWOX_WEBSPIDERCF_VARNAME_MAXRECURSION, &var));
  netwib_er(netwox_conffile_write_var_uint32(pbufpool, pio, &var, 0));
  netwib_er(netwox_conffile_write_empty(pio));

  netwib_er(netwib_buf_init_ext_string("Variable maxthreads contains maximum number of threads to use", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string("(recommended : forlocalsite=1, forremotesite=5)", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string(NETWOX_WEBSPIDERCF_VARNAME_MAXTHREADS, &var));
  netwib_er(netwox_conffile_write_var_uint32(pbufpool, pio, &var, 5));
  netwib_er(netwox_conffile_write_empty(pio));

  netwib_er(netwib_buf_init_ext_string("Variable millisleep contains time to wait between each download", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string("(recommended : 1000)", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string(NETWOX_WEBSPIDERCF_VARNAME_MILLISLEEP, &var));
  netwib_er(netwox_conffile_write_var_uint32(pbufpool, pio, &var, 0));
  netwib_er(netwox_conffile_write_empty(pio));

  netwib_er(netwib_buf_init_ext_string("This block defines ACL for urls, expressed as regular expressions. Syntax", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string("can be very complex (read rulesfile.h). Starting chain is 1. Comment this", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string("part to allow everything. ACL definition is enclosed between acl=begin", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string("and acl=end", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));

  netwib_er(netwib_buf_init_ext_string(NETWOX_WEBSPIDERCF_VARNAME_ACL, &var));
  netwib_er(netwib_buf_init_ext_string("begin", &val));
  netwib_er(netwox_conffile_write_var(pbufpool, pio, &var, &val));
  /**/
  netwib_er(netwib_buf_init_ext_string("First example (only accept /dir subdirectory):", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string(NETWOX_RULESFILE_VARNAME_DEFAULTTARGET, &var));
  netwib_er(netwib_buf_init_ext_string("reject", &val));
  netwib_er(netwox_conffile_write_varcmtind(pbufpool, pio, &var, &val));
  netwib_er(netwib_buf_init_ext_string(NETWOX_RULESFILE_VARNAME_CHAINNUMBER, &var));
  netwib_er(netwox_conffile_write_varcmtind_uint32(pbufpool, pio, &var, 1));
  netwib_er(netwib_buf_init_ext_string(NETWOX_RULESFILE_VARNAME_CHAINDEFAULTTARGET, &var));
  netwib_er(netwib_buf_init_ext_string("reject", &val));
  netwib_er(netwox_conffile_write_varcmtind(pbufpool, pio, &var, &val));
  netwib_er(netwib_buf_init_ext_string(NETWOX_RULESFILE_VARNAME_RULETARGET, &var));
  netwib_er(netwib_buf_init_ext_string("accept", &val));
  netwib_er(netwox_conffile_write_varcmtind(pbufpool, pio, &var, &val));
  netwib_er(netwib_buf_init_ext_string(NETWOX_RULESFILE_VARNAME_RULE, &var));
  netwib_er(netwib_buf_init_ext_string("^http://www\\.example\\.com/dir/", &val));
  netwib_er(netwox_conffile_write_varcmtind(pbufpool, pio, &var, &val));
  /**/
  netwib_er(netwib_buf_init_ext_string("Second example (only accept /dir and /mydir subdirectories):", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string(NETWOX_RULESFILE_VARNAME_DEFAULTTARGET, &var));
  netwib_er(netwib_buf_init_ext_string("reject", &val));
  netwib_er(netwox_conffile_write_varcmtind(pbufpool, pio, &var, &val));
  netwib_er(netwib_buf_init_ext_string(NETWOX_RULESFILE_VARNAME_CHAINNUMBER, &var));
  netwib_er(netwox_conffile_write_varcmtind_uint32(pbufpool, pio, &var, 1));
  netwib_er(netwib_buf_init_ext_string(NETWOX_RULESFILE_VARNAME_CHAINDEFAULTTARGET, &var));
  netwib_er(netwib_buf_init_ext_string("reject", &val));
  netwib_er(netwox_conffile_write_varcmtind(pbufpool, pio, &var, &val));
  netwib_er(netwib_buf_init_ext_string(NETWOX_RULESFILE_VARNAME_RULETARGET, &var));
  netwib_er(netwib_buf_init_ext_string("accept", &val));
  netwib_er(netwox_conffile_write_varcmtind(pbufpool, pio, &var, &val));
  netwib_er(netwib_buf_init_ext_string(NETWOX_RULESFILE_VARNAME_RULE, &var));
  netwib_er(netwib_buf_init_ext_string("^http://www\\.example\\.com/dir/", &val));
  netwib_er(netwox_conffile_write_varcmtind(pbufpool, pio, &var, &val));
  netwib_er(netwib_buf_init_ext_string(NETWOX_RULESFILE_VARNAME_RULETARGET, &var));
  netwib_er(netwib_buf_init_ext_string("accept", &val));
  netwib_er(netwox_conffile_write_varcmtind(pbufpool, pio, &var, &val));
  netwib_er(netwib_buf_init_ext_string(NETWOX_RULESFILE_VARNAME_RULE, &var));
  netwib_er(netwib_buf_init_ext_string("^http://www\\.example\\.com/mydir/", &val));
  netwib_er(netwox_conffile_write_varcmtind(pbufpool, pio, &var, &val));
  /**/
  netwib_er(netwib_buf_init_ext_string("Third example (subdirectory /dir except privatedir and mp3):", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string(NETWOX_RULESFILE_VARNAME_DEFAULTTARGET, &var));
  netwib_er(netwib_buf_init_ext_string("reject", &val));
  netwib_er(netwox_conffile_write_varcmtind(pbufpool, pio, &var, &val));
  netwib_er(netwib_buf_init_ext_string(NETWOX_RULESFILE_VARNAME_CHAINNUMBER, &var));
  netwib_er(netwox_conffile_write_varcmtind_uint32(pbufpool, pio, &var, 1));
  netwib_er(netwib_buf_init_ext_string(NETWOX_RULESFILE_VARNAME_CHAINDEFAULTTARGET, &var));
  netwib_er(netwib_buf_init_ext_string("return", &val));
  netwib_er(netwox_conffile_write_varcmtind(pbufpool, pio, &var, &val));
  netwib_er(netwib_buf_init_ext_string(NETWOX_RULESFILE_VARNAME_RULETARGET, &var));
  netwib_er(netwib_buf_init_ext_string("jump 2", &val));
  netwib_er(netwox_conffile_write_varcmtind(pbufpool, pio, &var, &val));
  netwib_er(netwib_buf_init_ext_string(NETWOX_RULESFILE_VARNAME_RULE, &var));
  netwib_er(netwib_buf_init_ext_string("^http://www\\.example\\.com/dir/", &val));
  netwib_er(netwox_conffile_write_varcmtind(pbufpool, pio, &var, &val));
  netwib_er(netwib_buf_init_ext_string(NETWOX_RULESFILE_VARNAME_RULETARGET, &var));
  netwib_er(netwib_buf_init_ext_string("jump 2", &val));
  netwib_er(netwox_conffile_write_varcmtind(pbufpool, pio, &var, &val));
  netwib_er(netwib_buf_init_ext_string(NETWOX_RULESFILE_VARNAME_RULE, &var));
  netwib_er(netwib_buf_init_ext_string("^http://1.2.3.4/dir/", &val));
  netwib_er(netwox_conffile_write_varcmtind(pbufpool, pio, &var, &val));
  netwib_er(netwib_buf_init_ext_string(NETWOX_RULESFILE_VARNAME_CHAINNUMBER, &var));
  netwib_er(netwox_conffile_write_varcmtind_uint32(pbufpool, pio, &var, 2));
  netwib_er(netwib_buf_init_ext_string(NETWOX_RULESFILE_VARNAME_CHAINDEFAULTTARGET, &var));
  netwib_er(netwib_buf_init_ext_string("accept", &val));
  netwib_er(netwox_conffile_write_varcmtind(pbufpool, pio, &var, &val));
  netwib_er(netwib_buf_init_ext_string(NETWOX_RULESFILE_VARNAME_RULETARGET, &var));
  netwib_er(netwib_buf_init_ext_string("reject", &val));
  netwib_er(netwox_conffile_write_varcmtind(pbufpool, pio, &var, &val));
  netwib_er(netwib_buf_init_ext_string(NETWOX_RULESFILE_VARNAME_RULE, &var));
  netwib_er(netwib_buf_init_ext_string("/privatedir/", &val));
  netwib_er(netwox_conffile_write_varcmtind(pbufpool, pio, &var, &val));
  netwib_er(netwib_buf_init_ext_string(NETWOX_RULESFILE_VARNAME_RULETARGET, &var));
  netwib_er(netwib_buf_init_ext_string("reject", &val));
  netwib_er(netwox_conffile_write_varcmtind(pbufpool, pio, &var, &val));
  netwib_er(netwib_buf_init_ext_string(NETWOX_RULESFILE_VARNAME_RULE, &var));
  netwib_er(netwib_buf_init_ext_string("\\.mp3$", &val));
  netwib_er(netwox_conffile_write_varcmtind(pbufpool, pio, &var, &val));
  netwib_er(netwib_buf_init_ext_string(NETWOX_WEBSPIDERCF_VARNAME_ACL, &var));
  netwib_er(netwib_buf_init_ext_string("end", &val));
  netwib_er(netwox_conffile_write_var(pbufpool, pio, &var, &val));
  netwib_er(netwox_conffile_write_empty(pio));

  netwib_er(netwib_buf_init_ext_string("Variable getimage indicates if we want to download", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string("images (0=no, 1=yes)", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string(NETWOX_WEBSPIDERCF_VARNAME_GETIMAGE, &var));
  netwib_er(netwox_conffile_write_var_uint32(pbufpool, pio, &var, 1));
  netwib_er(netwox_conffile_write_empty(pio));

  netwib_er(netwib_buf_init_ext_string("Variable getmultimedia indicates if we want to download", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string("multimedia documents (0=no, 1=yes)", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string(NETWOX_WEBSPIDERCF_VARNAME_GETMULTIMEDIA, &var));
  netwib_er(netwox_conffile_write_var_uint32(pbufpool, pio, &var, 1));
  netwib_er(netwox_conffile_write_empty(pio));

  netwib_er(netwib_buf_init_ext_string("Variable getcode indicates if we want to download", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string("script code (0=no, 1=yes)", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string(NETWOX_WEBSPIDERCF_VARNAME_GETCODE, &var));
  netwib_er(netwox_conffile_write_var_uint32(pbufpool, pio, &var, 1));
  netwib_er(netwox_conffile_write_empty(pio));

  netwib_er(netwib_buf_init_ext_string("Variable linkconversion indicates how to convert links in files after", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string("downloading (0: no conversion, 1: point links not downloaded to the", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string("web location)", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string(NETWOX_WEBSPIDERCF_VARNAME_LINKCONVERSION, &var));
  netwib_er(netwox_conffile_write_var_uint32(pbufpool, pio, &var, 1));
  netwib_er(netwox_conffile_write_empty(pio));

  netwib_er(netwib_buf_init_ext_string("Variable useragent contains the name of User-Agent", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string(NETWOX_WEBSPIDERCF_VARNAME_USERAGENT, &var));
  netwib_er(netwib_buf_init_ext_string("Mozilla/4.79", &val));
  netwib_er(netwox_conffile_write_var(pbufpool, pio, &var, &val));
  netwib_er(netwox_conffile_write_empty(pio));

  netwib_er(netwib_buf_init_ext_string("Variable loglevel indicates how to log events on", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string("stdout (0:none ; 1:normal ; 2:'...' ; 3:threadnumber%10 ; 4:scale ;", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string("5:fraction)", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string(NETWOX_WEBSPIDERCF_VARNAME_LOGLEVEL, &var));
  netwib_er(netwox_conffile_write_var_uint32(pbufpool, pio, &var, 1));
  netwib_er(netwox_conffile_write_empty(pio));

  netwib_er(netwib_buf_init_ext_string("Variable ftppassive indicates if use passive FTP (instead of active FTP)", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string(NETWOX_WEBSPIDERCF_VARNAME_FTPPASSIVE,
                                     &var));
  netwib_er(netwox_conffile_write_var_uint32(pbufpool, pio, &var, 1));
  netwib_er(netwox_conffile_write_empty(pio));

  netwib_er(netwib_buf_init_ext_string("Variable timeoutms indicates timeout in milli-seconds", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string(NETWOX_WEBSPIDERCF_VARNAME_TIMEOUTMS, &var));
  netwib_er(netwox_conffile_write_var_uint32(pbufpool, pio, &var, 30000));
  netwib_er(netwox_conffile_write_empty(pio));

  netwib_er(netwib_buf_init_ext_string("This block, between begin and end, defines MIME types and their", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string("allowed extensions.", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string(NETWOX_WEBSPIDERCF_VARNAME_MIMETYPES, &var));
  netwib_er(netwib_buf_init_ext_string("begin", &val));
  netwib_er(netwox_conffile_write_var(pbufpool, pio, &var, &val));
  /**/
#define netwox_webspidercf_mime_add(mimetype,extensions) netwib_er(netwib_buf_init_ext_string(mimetype, &var)); netwib_er(netwib_buf_init_ext_string(extensions, &val)); netwib_er(netwox_conffile_write_varind(pbufpool, pio, &var, &val));
  netwox_webspidercf_mime_add("application/mac-binhex40", "hqx");
  netwox_webspidercf_mime_add("application/macbinary", "bin");
  netwox_webspidercf_mime_add("application/msexcel", "xls");
  netwox_webspidercf_mime_add("application/msword", "doc");
  netwox_webspidercf_mime_add("application/octet-stream", "dat,bin,cab,core,class,com,dll,dms,exe,jar,lha,lzh,o,rar,so,z,gz");
  netwox_webspidercf_mime_add("application/ogg", "ogg");
  netwox_webspidercf_mime_add("application/pdf", "pdf");
  netwox_webspidercf_mime_add("application/postscript", "ai,eps,ps");
  netwox_webspidercf_mime_add("application/rdf+xml", "rdf");
  netwox_webspidercf_mime_add("application/rtf", "rtf");
  netwox_webspidercf_mime_add("application/stuffit", "sit");
  netwox_webspidercf_mime_add("application/vnd.mozilla.xul+xml", "xul");
  netwox_webspidercf_mime_add("application/vnd.ms-excel", "xls");
  netwox_webspidercf_mime_add("application/vnd.ms-powerpoint", "ppt");
  netwox_webspidercf_mime_add("application/x-arj-compressed", "arj");
  netwox_webspidercf_mime_add("application/x-compress", "z");
  netwox_webspidercf_mime_add("application/x-csh", "csh");
  netwox_webspidercf_mime_add("application/x-deflate", "zlib");
  netwox_webspidercf_mime_add("application/x-dvi", "dvi");
  netwox_webspidercf_mime_add("application/x-hdf", "hdf");
  netwox_webspidercf_mime_add("application/x-gzip", "gz,tgz,tar.gz");
  netwox_webspidercf_mime_add("application/x-javascript", "js");
  netwox_webspidercf_mime_add("application/x-latex", "latex");
  netwox_webspidercf_mime_add("application/x-lha-compressed", "lha,lzh");
  netwox_webspidercf_mime_add("application/x-mif", "mif");
  netwox_webspidercf_mime_add("application/x-rpm", "rpm");
  netwox_webspidercf_mime_add("application/x-rtf", "rtf");
  netwox_webspidercf_mime_add("application/x-sh", "sh");
  netwox_webspidercf_mime_add("application/x-shar", "shar");
  netwox_webspidercf_mime_add("application/x-shockwave-flash", "swf");
  netwox_webspidercf_mime_add("application/x-tar", "tar,tgz");
  netwox_webspidercf_mime_add("application/x-tcl", "tcl");
  netwox_webspidercf_mime_add("application/x-tex", "tex");
  netwox_webspidercf_mime_add("application/x-texinfo", "texinfo,texi");
  netwox_webspidercf_mime_add("application/x-troff", "t,tr,roff");
  netwox_webspidercf_mime_add("application/x-troff-man", "man");
  netwox_webspidercf_mime_add("application/x-troff-me", "me");
  netwox_webspidercf_mime_add("application/x-troff-ms", "ms");
  netwox_webspidercf_mime_add("application/x-ustar", "ustar");
  netwox_webspidercf_mime_add("application/x-zip-compressed", "zip");
  netwox_webspidercf_mime_add("application/xhtml+xml", "xhtml,xht");
  netwox_webspidercf_mime_add("application/xml", "xml,xsl");
  netwox_webspidercf_mime_add("application/xml-dtd", "dtd");
  netwox_webspidercf_mime_add("application/xslt+xml", "xslt");
  netwox_webspidercf_mime_add("application/zip", "zip");
  netwox_webspidercf_mime_add("audio/basic", "au,snd,mp2,mp3");
  netwox_webspidercf_mime_add("audio/midi", "mid,midi,kar");
  netwox_webspidercf_mime_add("audio/mpeg", "mpga,mp2,mp3");
  netwox_webspidercf_mime_add("audio/x-aiff", "aif,aiff,aifc");
  netwox_webspidercf_mime_add("audio/x-mpegurl", "m3u");
  netwox_webspidercf_mime_add("audio/x-pn-realaudio", "ram,rm");
  netwox_webspidercf_mime_add("audio/x-pn-realaudio-plugin", "rpm");
  netwox_webspidercf_mime_add("audio/x-realaudio", "ra");
  netwox_webspidercf_mime_add("audio/x-wav", "wav");
  netwox_webspidercf_mime_add("image/bmp", "bmp");
  netwox_webspidercf_mime_add("image/cmu-raster", "ras");
  netwox_webspidercf_mime_add("image/gif", "gif");
  netwox_webspidercf_mime_add("image/jpeg", "jpeg,jpg,jpe");
  netwox_webspidercf_mime_add("image/png", "png");
  netwox_webspidercf_mime_add("image/svg+xml", "svg");
  netwox_webspidercf_mime_add("image/tiff", "tiff,tif");
  netwox_webspidercf_mime_add("image/x-icon", "ico");
  netwox_webspidercf_mime_add("image/x-portable-anymap", "pnm");
  netwox_webspidercf_mime_add("image/x-portable-bitmap", "pbm");
  netwox_webspidercf_mime_add("image/x-portable-graymap", "pgm");
  netwox_webspidercf_mime_add("image/x-portable-pixmap", "ppm");
  netwox_webspidercf_mime_add("image/x-rgb", "rgb");
  netwox_webspidercf_mime_add("image/x-xbitmap", "xbm");
  netwox_webspidercf_mime_add("image/x-xpixmap", "xpm");
  netwox_webspidercf_mime_add("image/x-xwindowdump", "xwd");
  netwox_webspidercf_mime_add("model/vrml", "wrl,vrml");
  netwox_webspidercf_mime_add("text/css", "css");
  netwox_webspidercf_mime_add("text/html", "html,htm");
  netwox_webspidercf_mime_add("text/plain", "txt,asc,asm,bas,bat,c,cc,conf,cpp,diff,eml,expect,f90,h,hh,java,log,make,nasl,patch,pas,pl,pm,py,ruby,s,sql,vb,vbs");
  netwox_webspidercf_mime_add("text/rtf", "rtf");
  netwox_webspidercf_mime_add("text/sgml", "sgml,sgm");
  netwox_webspidercf_mime_add("text/tab-separated-values", "tsv");
  netwox_webspidercf_mime_add("text/xml", "xml");
  netwox_webspidercf_mime_add("video/mpeg", "mpeg,mpg,mpe");
  netwox_webspidercf_mime_add("video/quicktime", "qt,mov");
  netwox_webspidercf_mime_add("video/x-msvideo", "avi");
  netwox_webspidercf_mime_add("video/x-sgi-movie", "movie");
  netwox_webspidercf_mime_add("www/mime", "mime");
  /**/
  netwib_er(netwib_buf_init_ext_string("The 'none' value is special: it defines a list of allowed extensions if", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string("the HTTP server does not set a Content-Type header.", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string("none", &var));
  netwib_er(netwib_buf_init_ext_string("dat,zip,tar.gz", &val));
  netwib_er(netwox_conffile_write_varind(pbufpool, pio, &var, &val));
  netwib_er(netwib_buf_init_ext_string(NETWOX_WEBSPIDERCF_VARNAME_MIMETYPES, &var));
  netwib_er(netwib_buf_init_ext_string("end", &val));
  netwib_er(netwox_conffile_write_var(pbufpool, pio, &var, &val));
  netwib_er(netwox_conffile_write_empty(pio));

  netwib_er(netwib_buf_init_ext_string("Variable mimetypeswarn indicates if a warning is displayed when a", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string("MIME type error occurs.", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string(NETWOX_WEBSPIDERCF_VARNAME_MIMETYPESWARN,
                                       &var));
  netwib_er(netwox_conffile_write_var_uint32(pbufpool, pio, &var, 1));
  netwib_er(netwox_conffile_write_empty(pio));

  netwib_er(netwib_buf_init_ext_string("Variable deletedextensions contains a list a extensions without a real", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string("meaning. They are suggested when mimetypeswarn is on.", &cmt));
  netwib_er(netwox_conffile_write_cmt(pbufpool, pio, &cmt));
  netwib_er(netwib_buf_init_ext_string(NETWOX_WEBSPIDERCF_VARNAME_DELETEDEXTENSIONS, &var));
  netwib_er(netwib_buf_init_ext_string("asp,cgi,php,php3,phtml", &val));
  netwib_er(netwox_conffile_write_var(pbufpool, pio, &var, &val));
  netwib_er(netwox_conffile_write_empty(pio));

  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_bufpool_close(&pbufpool));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_webspidercf_update(netwox_webspider *pwebspider,
                                            netwib_constbuf *pvar,
                                            netwib_constbuf *pvalbuf,
                                            netwib_bool valuintdecoded,
                                            netwib_uint32 valuint,
                                            netwib_bool *pinacl,
                                            netwib_bool *pinmimetypes,
                                           netwox_rulesfile_state *prulesstate)
{
  netwox_webspidercf_var var;

  netwib_er(netwox_webspidercf_var_init_buf(pvar, &var));

  /* simple toggle for ACL and MIMETYPES */
  if (var == NETWOX_WEBSPIDERCF_VAR_ACL) {
    *pinacl = ! *pinacl;
    return(NETWIB_ERR_OK);
  }
  if (var == NETWOX_WEBSPIDERCF_VAR_MIMETYPES) {
    *pinmimetypes = ! *pinmimetypes;
    return(NETWIB_ERR_OK);
  }

  /* if in ACL, give info to specific function */
  if (*pinacl) {
    pwebspider->aclurlset = NETWIB_TRUE;
    return(netwox_rulesfile_state_addvar(pwebspider->paclurl, prulesstate,
                                         pvar, pvalbuf));
  }

  /* if in MIMETYPES, add to the hash table */
  if (*pinmimetypes) {
    return(netwox_mimetypes_add(&pwebspider->mimetypes, pvar, pvalbuf));
  }

  /* check if variable name is known */
  if (var == NETWOX_WEBSPIDERCF_VAR_UNKNOWN) {
    netwib_er(netwib_fmt_display("Error : variable %{buf} is not recognized\n",
                                 pvar));
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  /* check integers */
  switch(var) {
  case NETWOX_WEBSPIDERCF_VAR_PROXYPORT :
  case NETWOX_WEBSPIDERCF_VAR_MAXRECURSION :
  case NETWOX_WEBSPIDERCF_VAR_MAXTHREADS :
  case NETWOX_WEBSPIDERCF_VAR_MILLISLEEP :
  case NETWOX_WEBSPIDERCF_VAR_GETIMAGE :
  case NETWOX_WEBSPIDERCF_VAR_GETMULTIMEDIA :
  case NETWOX_WEBSPIDERCF_VAR_GETCODE :
  case NETWOX_WEBSPIDERCF_VAR_LINKCONVERSION :
  case NETWOX_WEBSPIDERCF_VAR_LOGLEVEL :
  case NETWOX_WEBSPIDERCF_VAR_FTPPASSIVE :
  case NETWOX_WEBSPIDERCF_VAR_TIMEOUTMS :
  case NETWOX_WEBSPIDERCF_VAR_MIMETYPESWARN :
    if (!valuintdecoded) {
      netwib_er(netwib_fmt_display("Error : variable %{buf} must be set to an integer ('%{buf}' is not an integer)\n", pvar, pvalbuf));
      return(NETWIB_ERR_PAINVALIDTYPE);
    }
    break;
  default:
    break;
  }

  /* set variable */
  switch(var) {
  case NETWOX_WEBSPIDERCF_VAR_LOCALROOTDIR :
    netwib__buf_reinit(&pwebspider->localrootdir);
    netwib_er(netwib_path_canon(pvalbuf, &pwebspider->localrootdir));
    break;
  case NETWOX_WEBSPIDERCF_VAR_URLTODOWNLOAD :
    netwib_er(netwox_webspider_add_canon(pwebspider, pvalbuf));
    break;
  case NETWOX_WEBSPIDERCF_VAR_PROXYHOST :
    pwebspider->httpclictx.useproxy = NETWIB_TRUE;
    netwib_er(netwox_dnscache_ip(&pwebspider->httpclictx.dnscache, pvalbuf,
                                 &pwebspider->httpclictx.proxy_ip));
    break;
  case NETWOX_WEBSPIDERCF_VAR_PROXYPORT :
    pwebspider->httpclictx.proxy_port = valuint;
    break;
  case NETWOX_WEBSPIDERCF_VAR_PROXYUSER :
    netwib__buf_reinit(&pwebspider->httpclictx.proxy_login);
    netwib_er(netwib_buf_append_buf(pvalbuf,
                                    &pwebspider->httpclictx.proxy_login));
    break;
  case NETWOX_WEBSPIDERCF_VAR_PROXYPASS :
    netwib__buf_reinit(&pwebspider->httpclictx.proxy_password);
    netwib_er(netwib_buf_append_buf(pvalbuf,
                                    &pwebspider->httpclictx.proxy_password));
    break;
  case NETWOX_WEBSPIDERCF_VAR_MAXRECURSION :
    pwebspider->maxrecursion = valuint;
    break;
  case NETWOX_WEBSPIDERCF_VAR_MAXTHREADS :
    if (valuint == 0) {
      pwebspider->maxthreads = 100;
    } else {
      pwebspider->maxthreads = valuint;
    }
    break;
  case NETWOX_WEBSPIDERCF_VAR_MILLISLEEP :
    pwebspider->millisleep = valuint;
    break;
  case NETWOX_WEBSPIDERCF_VAR_GETIMAGE :
    pwebspider->getimage = valuint;
    break;
  case NETWOX_WEBSPIDERCF_VAR_GETMULTIMEDIA :
    pwebspider->getmultimedia = valuint;
    break;
  case NETWOX_WEBSPIDERCF_VAR_GETCODE :
    pwebspider->getcode = valuint;
    break;
  case NETWOX_WEBSPIDERCF_VAR_LINKCONVERSION :
    pwebspider->linkconv = valuint;
    break;
  case NETWOX_WEBSPIDERCF_VAR_USERAGENT :
    netwib__buf_reinit(&pwebspider->httpclictx.useragent);
    netwib_er(netwib_buf_append_buf(pvalbuf,
                                    &pwebspider->httpclictx.useragent));
    break;
  case NETWOX_WEBSPIDERCF_VAR_LOGLEVEL :
    pwebspider->loglevel = valuint;
    break;
  case NETWOX_WEBSPIDERCF_VAR_FTPPASSIVE :
    pwebspider->httpclictx.ftp_passive = valuint;
    break;
  case NETWOX_WEBSPIDERCF_VAR_TIMEOUTMS :
    pwebspider->httpclictx.timeoutms = valuint;
    break;
  case NETWOX_WEBSPIDERCF_VAR_MIMETYPESWARN :
    pwebspider->mimetypeswarn = valuint;
    break;
  case NETWOX_WEBSPIDERCF_VAR_DELETEDEXTENSIONS :
    netwib_er(netwox_bufarray_add_extensions(&pwebspider->deletedextensions, pvalbuf));
    break;
  default:
    return(NETWOX_ERR_INTERNALERROR);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_webspidercf_read(netwib_constbuf *pfilename,
                                   netwox_webspider *pwebspider)
{
  netwib_io *pio;
  netwib_buf *pvar, *pval;
  netwib_bufpool *pbufpool;
  netwib_bool uidec, inacl, inmimetypes;
  netwox_rulesfile_state rulesstate;
  netwib_uint32 ui;
  netwib_err ret;

  netwib_er(netwib_bufpool_initdefault(&pbufpool));

  netwib_er(netwox_io_init_conffile_read(pfilename, &pio));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pvar));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pval));
  netwib_er(netwox_rulesfile_state_init(&rulesstate));

  ret = NETWIB_ERR_OK;
  inacl = NETWIB_FALSE;
  inmimetypes = NETWIB_FALSE;
  while (NETWIB_TRUE) {
    /* read */
    netwib__buf_reinit(pvar);
    netwib__buf_reinit(pval);
    ret = netwox_conffile_read_var(pbufpool, pio, pvar, pval, &uidec, &ui);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    /*
    if (uidec) {
      netwib_er(netwib_fmt_display("%{buf} =u %{uint32}\n", pvar, ui));
    } else {
      netwib_er(netwib_fmt_display("%{buf} = %{buf}\n", pvar, pval));
    }
    */
    /* update configuration */
    ret = netwox_webspidercf_update(pwebspider, pvar, pval, uidec, ui,
                                    &inacl, &inmimetypes, &rulesstate);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }

  netwib_er(netwox_rulesfile_state_close(&rulesstate));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pvar));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pval));
  netwib_er(netwib_io_close(&pio));

  netwib_er(netwib_bufpool_close(&pbufpool));

  return(ret);
}
