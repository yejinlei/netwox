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
netwib_err netwox_syslog_msg_init(netwox_syslog_priority priority,
                                  netwib_time *ptime,
                                  netwib_buf *phostname,
                                  netwib_ip *pip,
                                  netwib_constbuf *pmessage,
                                  netwib_buf *pbuf)
{
  netwib_localtime lt;

  netwib_er(netwib_buf_append_fmt(pbuf, "<%{uint32}>", priority));

  if (ptime != NULL && (phostname != NULL || pip != NULL) ) {
    netwib_er(netwib_time_decode_localtime(ptime, &lt));
    netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_MON_SHORT,
                                          pbuf));
    netwib_er(netwib_buf_append_string(" ", pbuf));
    netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_MDAY_SPACE,
                                          pbuf));
    netwib_er(netwib_buf_append_string(" ", pbuf));
    netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_HOUR_ZERO,
                                          pbuf));
    netwib_er(netwib_buf_append_string(":", pbuf));
    netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_MIN_ZERO,
                                          pbuf));
    netwib_er(netwib_buf_append_string(":", pbuf));
    netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_SEC_ZERO,
                                          pbuf));
    netwib_er(netwib_buf_append_string(" ", pbuf));
    if (phostname != NULL) {
      netwib_er(netwib_buf_append_buf(phostname, pbuf));
    } else {
      netwib_er(netwib_buf_append_ip(pip, NETWIB_IP_ENCODETYPE_IP, pbuf));
    }
    netwib_er(netwib_buf_append_string(" ", pbuf));
  }

  netwib_er(netwib_buf_append_buf(pmessage, pbuf));
  return(NETWIB_ERR_OK);
}
