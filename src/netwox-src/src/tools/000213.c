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
netwib_conststring t000213_description[] = {
  "This tool displays a list of IP addresses. For example:",
  "  --ips 1.2.3.4-1.2.3.6 --separator \",\"",
  "displays:",
  "  1.2.3.4,1.2.3.5,1.2.3.6",
  "",
  "This tool is useful to generate IP addresses lists for other tools.",
  "",
  "See also tool 24 for other IP address conversions.",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000213_args[] = {
  NETWOX_TOOLARG_REQ_IPS_SU('i', "ips",
                             "list of addresses to display", NULL),
  NETWOX_TOOLARG_OPTA_BUF('s', "separator",
                          "separator to use", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000213_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NOTNET,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000213_info = {
  "Display a list of IP addresses",
  t000213_description,
  NULL,
  t000213_args,
  t000213_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000213_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_ips_index *pipsindex;
  netwib_ips *pips;
  netwib_ip ipad;
  netwib_buf buf;
  netwib_bool isset, addsep;
  netwib_err ret=NETWIB_ERR_OK;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000213_info, &parg));
  netwib_er(netwox_arg_ips(parg, 'i', &pips));
  netwib_er(netwox_arg_isset(parg, 's', &isset));
  if (isset) {
    netwib_er(netwox_arg_buf(parg, 's', &buf));
  }

  /* display */
  netwib_er(netwib_ips_index_init(pips, &pipsindex));
  addsep = NETWIB_FALSE;
  while (NETWIB_TRUE) {
    ret = netwib_ips_index_next_ip(pipsindex, &ipad);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    if (isset) {
      if (addsep) {
        netwib_er(netwib_fmt_display("%{buf}%{ip}", &buf, &ipad));
      } else {
        netwib_er(netwib_fmt_display("%{ip}", &ipad));
      }
    } else {
      netwib_er(netwib_fmt_display("%{ip}\n", &ipad));
    }
    addsep = NETWIB_TRUE;
  }
  netwib_er(netwib_ips_index_close(&pipsindex));

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(ret);
}
