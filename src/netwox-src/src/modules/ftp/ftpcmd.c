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
netwib_err netwox_ftpcmd_init_buf(netwib_constbuf *pbuf,
                                  netwox_ftpcmd *pftpcmd)
{
  netwib_data data;
  netwib_uint32 datasize;
  netwib_byte firstchar;

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);

  if (datasize == 0) {
    if (pftpcmd != NULL) *pftpcmd = NETWOX_FTPCMD_UNKNOWN;
    return(NETWIB_ERR_OK);
  }

  firstchar = data[0];
#define netwox_ftpcmd_init_buf_text(item,v) if (!netwib_c_memcasecmp(data, (netwib_constdata)item, datasize)) { if (pftpcmd != NULL) *pftpcmd = v; return(NETWIB_ERR_OK);}
  switch(firstchar) {
  case 'a' :
  case 'A' :
    if (datasize == 4) {
      netwox_ftpcmd_init_buf_text("ABOR", NETWOX_FTPCMD_ABOR);
      netwox_ftpcmd_init_buf_text("ACCT", NETWOX_FTPCMD_ACCT);
      netwox_ftpcmd_init_buf_text("ALLO", NETWOX_FTPCMD_ALLO);
      netwox_ftpcmd_init_buf_text("APPE", NETWOX_FTPCMD_APPE);
    }
    break;
  case 'c' :
  case 'C' :
    if (datasize == 3) {
      netwox_ftpcmd_init_buf_text("CWD", NETWOX_FTPCMD_CWD);
    }
    if (datasize == 4) {
      netwox_ftpcmd_init_buf_text("CDUP", NETWOX_FTPCMD_CDUP);
    }
    break;
  case 'd' :
  case 'D' :
    if (datasize == 4) {
      netwox_ftpcmd_init_buf_text("DELE", NETWOX_FTPCMD_DELE);
    }
    break;
  case 'e' :
  case 'E' :
    if (datasize == 4) {
      netwox_ftpcmd_init_buf_text("EPRT", NETWOX_FTPCMD_EPRT);
      netwox_ftpcmd_init_buf_text("EPSV", NETWOX_FTPCMD_EPSV);
    }
    break;
  case 'h' :
  case 'H' :
    if (datasize == 4) {
      netwox_ftpcmd_init_buf_text("HELP", NETWOX_FTPCMD_HELP);
    }
    break;
  case 'l' :
  case 'L' :
    if (datasize == 4) {
      netwox_ftpcmd_init_buf_text("LIST", NETWOX_FTPCMD_LIST);
    }
    break;
  case 'm' :
  case 'M' :
    if (datasize == 3) {
      netwox_ftpcmd_init_buf_text("MKD", NETWOX_FTPCMD_MKD);
    }
    if (datasize == 4) {
      netwox_ftpcmd_init_buf_text("MODE", NETWOX_FTPCMD_MODE);
    }
    break;
  case 'n' :
  case 'N' :
    if (datasize == 4) {
      netwox_ftpcmd_init_buf_text("NLST", NETWOX_FTPCMD_NLST);
      netwox_ftpcmd_init_buf_text("NOOP", NETWOX_FTPCMD_NOOP);
    }
    break;
  case 'p' :
  case 'P' :
    if (datasize == 3) {
      netwox_ftpcmd_init_buf_text("PWD", NETWOX_FTPCMD_PWD);
    }
    if (datasize == 4) {
      netwox_ftpcmd_init_buf_text("PASS", NETWOX_FTPCMD_PASS);
      netwox_ftpcmd_init_buf_text("PASV", NETWOX_FTPCMD_PASV);
      netwox_ftpcmd_init_buf_text("PORT", NETWOX_FTPCMD_PORT);
    }
    break;
  case 'q' :
  case 'Q' :
    if (datasize == 4) {
      netwox_ftpcmd_init_buf_text("QUIT", NETWOX_FTPCMD_QUIT);
    }
    break;
  case 'r' :
  case 'R' :
    if (datasize == 3) {
      netwox_ftpcmd_init_buf_text("RMD", NETWOX_FTPCMD_RMD);
    }
    if (datasize == 4) {
      netwox_ftpcmd_init_buf_text("REIN", NETWOX_FTPCMD_REIN);
      netwox_ftpcmd_init_buf_text("REST", NETWOX_FTPCMD_REST);
      netwox_ftpcmd_init_buf_text("RETR", NETWOX_FTPCMD_RETR);
      netwox_ftpcmd_init_buf_text("RNFR", NETWOX_FTPCMD_RNFR);
      netwox_ftpcmd_init_buf_text("RNTO", NETWOX_FTPCMD_RNTO);
    }
    break;
  case 's' :
  case 'S' :
    if (datasize == 4) {
      netwox_ftpcmd_init_buf_text("SITE", NETWOX_FTPCMD_SITE);
      netwox_ftpcmd_init_buf_text("SIZE", NETWOX_FTPCMD_SIZE);
      netwox_ftpcmd_init_buf_text("SMNT", NETWOX_FTPCMD_SMNT);
      netwox_ftpcmd_init_buf_text("STAT", NETWOX_FTPCMD_STAT);
      netwox_ftpcmd_init_buf_text("STOR", NETWOX_FTPCMD_STOR);
      netwox_ftpcmd_init_buf_text("STOU", NETWOX_FTPCMD_STOU);
      netwox_ftpcmd_init_buf_text("STRU", NETWOX_FTPCMD_STRU);
      netwox_ftpcmd_init_buf_text("SYST", NETWOX_FTPCMD_SYST);
    }
    break;
  case 't' :
  case 'T' :
    if (datasize == 4) {
      netwox_ftpcmd_init_buf_text("TYPE", NETWOX_FTPCMD_TYPE);
    }
    break;
  case 'u' :
  case 'U' :
    if (datasize == 4) {
      netwox_ftpcmd_init_buf_text("USER", NETWOX_FTPCMD_USER);
    }
    break;
  }

  if (pftpcmd != NULL) *pftpcmd = NETWOX_FTPCMD_UNKNOWN;
  return(NETWIB_ERR_OK);
}


