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
netwib_err netwox_identserver_init(netwox_identserver *pidentserver)
{
  /* those can be changed by users */
  pidentserver->timeoutms = 60000;
  netwib_er(netwib_buf_init_ext_storage(&pidentserver->username));
  pidentserver->pallowedclients = NULL;

  /* internal */
  netwib_er(netwib_bufpool_init(NETWIB_TRUE, &pidentserver->pbufpool));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_identserver_close(netwox_identserver *pidentserver)
{
  netwib_er(netwib_bufpool_close(&pidentserver->pbufpool));

  netwib_er(netwib_buf_close(&pidentserver->username));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
typedef struct {
  netwox_identserver *pcommon;
  netwib_io *pio;
} netwox_priv_identserver;

/*-------------------------------------------------------------*/
static netwib_err netwox_identserver_session2(netwox_priv_identserver *pident)
{
  netwib_ip remoteip;
  netwib_buf *pline;
  netwib_bool allowed;
  netwib_err ret;

  allowed = NETWIB_TRUE;
  if (pident->pcommon->pallowedclients != NULL) {
    netwib_er(netwib_sock_ctl_get_remote(pident->pio, &remoteip, NULL));
    netwib_er(netwib_ips_contains_ip(pident->pcommon->pallowedclients,
                                     &remoteip, &allowed));
  }

  if (!allowed) {
    netwib_er(netwib_fmt_display("IP address %{ip} is not allowed\n",
                                 &remoteip));
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_bufpool_buf_init(pident->pcommon->pbufpool, &pline));

  /* read a line, but ignore its content */
  ret = netwox_txtproto_read_query(pident->pio, pident->pcommon->timeoutms,
                                   pline);

  /* reply */
  if (ret == NETWIB_ERR_OK) {
    if (netwib__buf_ref_data_size(&pident->pcommon->username) != 0) {
      netwib_er(netwib_buf_append_fmt(pline, ":USERID:UNIX:%{buf}\r\n",
                                      &pident->pcommon->username));
    } else {
      netwib_er(netwib_buf_append_string(":ERROR:HIDDEN-USER\r\n", pline));
    }
    ret = netwib_io_write(pident->pio, pline);
  }

  netwib_er(netwib_bufpool_buf_close(pident->pcommon->pbufpool, &pline));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_identserver_session(netwib_io *psockio,
                                      netwib_ptr pinfos)
{
  netwox_priv_identserver identserver;
  netwib_err ret;

  /* prepare variables */
  identserver.pcommon = (netwox_identserver *)pinfos;
  netwib_er(netwox_txtproto_io_rd(psockio, &identserver.pio));

  /* main */
  ret = netwox_identserver_session2(&identserver);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_err_display(ret, NETWIB_ERR_ENCODETYPE_TEXT));
  }

  /* close */
  netwib_er(netwib_io_unplug_next_supported(identserver.pio, NULL));
  netwib_er(netwib_io_close(&identserver.pio));

  return(NETWIB_ERR_OK);
}
