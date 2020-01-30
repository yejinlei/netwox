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
netwib_err netwox_date_rfc822(netwib_consttime *pt,
                              netwib_buf *pbuf)
{
  netwib_localtime lt;

  netwib_er(netwib_time_decode_localtime(pt, &lt));

  netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_WDAY_SHORT,
                                        pbuf));
  netwib_er(netwib_buf_append_string(", ", pbuf));
  netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_MDAY_SPACE,
                                        pbuf));
  netwib_er(netwib_buf_append_string(" ", pbuf));
  netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_MON_SHORT,
                                        pbuf));
  netwib_er(netwib_buf_append_string(" ", pbuf));
  netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_YEAR_LONG,
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
  netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_ZONE_HM,
                                        pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_date_rfc822simple(netwib_consttime *pt,
                                    netwib_buf *pbuf)
{
  netwib_localtime lt;

  netwib_er(netwib_time_decode_localtime(pt, &lt));

  netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_WDAY_SHORT,
                                        pbuf));
  netwib_er(netwib_buf_append_string(", ", pbuf));
  netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_MDAY_SPACE,
                                        pbuf));
  netwib_er(netwib_buf_append_string(" ", pbuf));
  netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_MON_SHORT,
                                        pbuf));
  netwib_er(netwib_buf_append_string(" ", pbuf));
  netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_YEAR_LONG,
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

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_date_unixdate(netwib_consttime *pt,
                                netwib_buf *pbuf)
{
  netwib_localtime lt;

  netwib_er(netwib_time_decode_localtime(pt, &lt));

  netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_WDAY_SHORT,
                                        pbuf));
  netwib_er(netwib_buf_append_byte(' ', pbuf));
  netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_MON_SHORT,
                                        pbuf));
  netwib_er(netwib_buf_append_byte(' ', pbuf));
  netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_MDAY_SPACE,
                                        pbuf));
  netwib_er(netwib_buf_append_byte(' ', pbuf));
  netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_HOUR_ZERO,
                                        pbuf));
  netwib_er(netwib_buf_append_byte(':', pbuf));
  netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_MIN_ZERO,
                                        pbuf));
  netwib_er(netwib_buf_append_byte(':', pbuf));
  netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_SEC_ZERO,
                                        pbuf));
  netwib_er(netwib_buf_append_byte(' ', pbuf));
  netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_YEAR_LONG,
                                        pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_date_human(netwib_consttime *pt,
                             netwib_buf *pbuf)
{
  netwib_localtime lt;

  netwib_er(netwib_time_decode_localtime(pt, &lt));

  netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_WDAY_SHORT,
                                        pbuf));
  netwib_er(netwib_buf_append_string(", ", pbuf));
  netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_MDAY_SPACE,
                                        pbuf));
  netwib_er(netwib_buf_append_string(" ", pbuf));
  netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_MON_SHORT,
                                        pbuf));
  netwib_er(netwib_buf_append_string(" ", pbuf));
  netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_YEAR_LONG,
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
  netwib_er(netwib_buf_append_fmt(pbuf, " %{uint32}ns", lt.nsec));
  netwib_er(netwib_buf_append_string(" ", pbuf));
  netwib_er(netwib_buf_append_localtime(&lt,
                                        NETWIB_LOCALTIME_ENCODETYPE_ZONE_HM,
                                        pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_date_sec(netwib_consttime *pt,
                           netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32}", pt->sec));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_date_secnsec(netwib_consttime *pt,
                               netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32}:%{uint32}", pt->sec,
                                  pt->nsec));

  return(NETWIB_ERR_OK);
}

