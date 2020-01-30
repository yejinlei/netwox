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
netwib_conststring t000187_description[] = {
  "Display current time (or --time) using several formats.",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000187_args[] = {
  NETWOX_TOOLARG_OPT_UINT32('t', "time", "time to display", NULL),
  NETWOX_TOOLARG_RADIO1_SET('r', "disp-rfc822", "display rfc822 date"),
  NETWOX_TOOLARG_RADIO1('u', "disp-unixdate", "display unix date(1) format"),
  NETWOX_TOOLARG_RADIO1('h', "disp-human", "display human readable time"),
  NETWOX_TOOLARG_RADIO1('s', "disp-sec", "display seconds"),
  NETWOX_TOOLARG_RADIO1('n', "disp-secnsec",
                        "display seconds and nanoseconds"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000187_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NOTNET,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000187_info = {
  "Display date and time",
  t000187_description,
  NULL,
  t000187_args,
  t000187_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000187_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_bool isset;
  netwib_time t;
  netwib_buf buf;
  netwib_char c;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000187_info, &parg));
  netwib_er(netwox_arg_isset(parg, 't', &isset));
  t.nsec = 0;
  if (isset) {
    netwib_er(netwox_arg_uint32(parg, 't', &t.sec));
  } else {
    netwib_er(netwib_time_init_now(&t));
  }
  netwib_er(netwox_arg_radio1(parg, &c));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  switch(c) {
  case 'r' :
    netwib_er(netwox_date_rfc822(&t, &buf));
    break;
  case 'u' :
    netwib_er(netwox_date_unixdate(&t, &buf));
    break;
  case 'h' :
    netwib_er(netwox_date_human(&t, &buf));
    break;
  case 's' :
    netwib_er(netwox_date_sec(&t, &buf));
    break;
  case 'n' :
    netwib_er(netwox_date_secnsec(&t, &buf));
    break;
  }
  netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_buf_close(&buf));

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
