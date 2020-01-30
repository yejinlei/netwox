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
#include "../netwox.h"

/*-------------------------------------------------------------*/
netwib_err netwox_buf_append_err(netwib_err error,
                                 netwib_err_encodetype encodetype,
                                 netwib_buf *pbuf)
{
  netwib_conststring pc = NULL;

  switch((int)error) {
    case NETWOX_ERR_INTERNALERROR :
      pc = "internal error";
      break;
    case NETWOX_ERR_NOTIMPLEMENTED :
      pc = "not implemented";
      break;
    case NETWOX_ERR_NOTSUPPORTED :
      pc = "not supported";
      break;
    case NETWOX_ERR_NOTALLOWED :
      pc = "not allowed";
      break;
    case NETWOX_ERR_TIMEOUT :
      pc = "timeout";
      break;
    case NETWOX_ERR_PROTOCOL :
      pc = "protocol error";
      break;
    case NETWOX_ERR_AUTHNEEDED :
      pc = "authentication needed";
      break;
    case NETWOX_ERR_AUTHERROR :
      pc = "authentication error";
      break;
    case NETWOX_ERR_BADVALUE :
      pc = "bad value";
      break;
    case NETWOX_ERR_BADSIZE :
      pc = "bad size";
      break;
    case NETWOX_ERR_TOOL_NOTREGISTERED :
      pc = "this tool wasn't found";
      break;
    case NETWOX_ERR_TOOLARG_NOTDECODED :
      pc = "tool argument not decoded";
      break;
    case NETWOX_ERR_TOOLARG_HELP :
      pc = "help requested";
      break;
    case NETWOX_ERR_TOOLARG_NOERRDISPLAY :
      pc = "no error display";
      break;
    case NETWOX_ERR_TOOLARG_INVALID :
      pc = "invalid tool argument";
      break;
    case NETWOX_ERR_TOOLTREE_INIT :
      pc = "error in initialization of tooltree";
      break;
    case NETWOX_ERR_TOOLTREE_HELPMODE_END :
      pc = "end of helpmode requested";
      break;
    case NETWOX_ERR_SNIFF_INVALIDDLT :
      pc = "invalid sniff DLT";
      break;
    case NETWOX_ERR_SPOOF_INVALIDDLT :
      pc = "invalid spoof DLT";
      break;
    case NETWOX_ERR_SOCKVTCP_NOSYNACK :
      pc = "the server did not sent us a SYN-ACK";
      break;
    case NETWOX_ERR_SOCKVTCP_RST :
      pc = "the server sent us a ReSeT";
      break;
    case NETWOX_ERR_SNMP_BADREPLY :
      pc = "bad reply";
      break;
  default :
    pc = NULL;
  }

  if (pc != NULL) {
    switch (encodetype) {
    case NETWIB_ERR_ENCODETYPE_TEXT :
      netwib_er(netwib_buf_append_string(pc, pbuf));
      break;
    case NETWIB_ERR_ENCODETYPE_NUMTEXT :
      netwib_er(netwib_buf_append_fmt(pbuf, "Error %{uint32} : %s",
                                      error, pc));
      break;
    case NETWIB_ERR_ENCODETYPE_FULL :
      netwib_er(netwib_buf_append_fmt(pbuf, "Error %{uint32} : %s\n",
                                      error, pc));
      break;
    }
  } else {
    netwib_er(netwib_buf_append_err(error, encodetype, pbuf));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_err_display(netwib_err error,
                              netwib_err_encodetype encodetype)
{
  netwib_buf buf;
  netwib_char arr[4096];

  netwib_er(netwib_buf_init_ext_arraysizeofempty(arr, &buf));
  netwib_er(netwox_buf_append_err(error, encodetype, &buf));

  switch (encodetype) {
  case NETWIB_ERR_ENCODETYPE_TEXT :
  case NETWIB_ERR_ENCODETYPE_NUMTEXT :
    netwib_er(netwib_fmt_display("%{buf}\n", &buf));
    break;
  case NETWIB_ERR_ENCODETYPE_FULL :
    netwib_er(netwib_fmt_display("%{buf}", &buf));
    break;
  }

  return(NETWIB_ERR_OK);
}
