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
netwib_conststring t000024_description[] = {
  "This tool converts a range of IP addresses. For example:",
  "  netwox 24 --ips 192.168.1.128/27 --iprange --ipnumber --ipmask",
  "returns:",
  "  192.168.1.128-192.168.1.159",
  "  192.168.1.128/27",
  "  192.168.1.128/255.255.255.224",
  "It permits to easily compute netmask.",
  "To obtain information about each address in a list of IP addresses,",
  "use tool 3 with option -a.",
  "See also tool 213.",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000024_args[] = {
  NETWOX_TOOLARG_REQ_IPS_SU('i', NULL, NULL, "192.168.1.0/24"),
  NETWOX_TOOLARG_OPTA_BOOL('h', "hnrange", "display hostnames", NULL),
  NETWOX_TOOLARG_OPTA_BOOL('r', "iprange", "display ip-ip", NULL),
  NETWOX_TOOLARG_OPTA_BOOL('n', "ipnumber", "display ip/num", NULL),
  NETWOX_TOOLARG_OPTA_BOOL('m', "ipmask", "display ip/mask", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000024_nodes[] = {
  NETWOX_TOOLTREENODETYPE_INFO_REMOTE,
  NETWOX_TOOLTREENODETYPE_NOTNET,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000024_info = {
  "Convert IP addresses ranges",
  t000024_description,
  "ipcalc, netmask",
  t000024_args,
  t000024_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000024_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_ips *pips;
  netwib_buf buf;
  netwib_bool disphnrange, dispiprange, dispipnumber, dispipmask;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000024_info, &parg));
  netwib_er(netwox_arg_ips(parg, 'i', &pips));
  netwib_er(netwox_arg_bool(parg, 'h', &disphnrange));
  netwib_er(netwox_arg_bool(parg, 'r', &dispiprange));
  netwib_er(netwox_arg_bool(parg, 'n', &dispipnumber));
  netwib_er(netwox_arg_bool(parg, 'm', &dispipmask));

  /* if no option is set, they are all displayed */
  if (!disphnrange && !dispiprange && !dispipnumber && !dispipmask) {
    disphnrange = NETWIB_TRUE;
    dispiprange = NETWIB_TRUE;
    dispipnumber = NETWIB_TRUE;
    dispipmask = NETWIB_TRUE;
  }

  /* display IP */
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  if (dispiprange) {
    netwib__buf_reinit(&buf);
    netwib_er(netwib_buf_append_ips(pips, NETWIB_IPS_ENCODETYPE_IPRANGE,
                                    &buf));
    netwib_er(netwib_fmt_display("%{buf}\n", &buf));
  }
  if (dispipnumber) {
    netwib__buf_reinit(&buf);
    netwib_er(netwib_buf_append_ips(pips, NETWIB_IPS_ENCODETYPE_IPNUMBER,
                                    &buf));
    netwib_er(netwib_fmt_display("%{buf}\n", &buf));
  }
  if (dispipmask) {
    netwib__buf_reinit(&buf);
    netwib_er(netwib_buf_append_ips(pips, NETWIB_IPS_ENCODETYPE_IPMASK, &buf));
    netwib_er(netwib_fmt_display("%{buf}\n", &buf));
  }
  if (disphnrange) {
    netwib__buf_reinit(&buf);
    netwib_er(netwib_buf_append_ips(pips, NETWIB_IPS_ENCODETYPE_HNRANGE,
                                    &buf));
    netwib_er(netwib_fmt_display("%{buf}\n", &buf));
  }

  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
