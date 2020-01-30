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
netwib_conststring t000004_description[] = {
  "This tool obtains addresses related to an Ethernet address.",
  "",
  "Parameter --title displays title in front of each result.",
  "Parameter --ip displays IP address.",
  "Parameter --host displays main hostname.",
  "Parameter --hosts displays list of hostnames (one IP address can have",
  "several hostnames).",
  "If no option (ip, host, hosts) is set, they are all displayed.",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000004_args[] = {
  NETWOX_TOOLARG_OPTA_BOOL('t', "title", "display titles", NULL),
  NETWOX_TOOLARG_OPTA_BOOL('i', "ip", "obtain IP address", NULL),
  NETWOX_TOOLARG_OPTA_BOOL('h', "host", "obtain hostname", NULL),
  NETWOX_TOOLARG_OPTA_BOOL('H', "hosts", "obtain hostnames", NULL),
  NETWOX_TOOLARG_REQ_ETH('e', NULL, NULL, NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000004_nodes[] = {
  NETWOX_TOOLTREENODETYPE_INFO_REMOTE,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000004_info = {
  "Display information about an Ethernet address",
  t000004_description,
  "computer, hostname, mac, resolve, show",
  t000004_args,
  t000004_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000004_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_bool wantip, wanthost, wanthosts, wanttitle, ipadvalid;
  netwib_buf buf;
  netwib_ip ipad;
  netwib_eth eth;
  netwib_err ret;

  netwib_er(netwox_arg_init(argc, argv, &t000004_info, &parg));
  netwib_er(netwox_arg_bool(parg, 't', &wanttitle));
  netwib_er(netwox_arg_bool(parg, 'i', &wantip));
  netwib_er(netwox_arg_bool(parg, 'h', &wanthost));
  netwib_er(netwox_arg_bool(parg, 'H', &wanthosts));
  netwib_er(netwox_arg_eth(parg, 'e', &eth));
  netwib_er(netwox_arg_close(&parg));

  /* if no option is set, they are all displayed */
  if (!wanttitle && !wantip && !wanthost && !wanthosts) {
    wanttitle = NETWIB_TRUE;
    wantip = NETWIB_TRUE;
    wanthost = NETWIB_TRUE;
    wanthosts = NETWIB_TRUE;
  }

  /* obtain IP address */
  ipadvalid = NETWIB_FALSE;
  ret = netwib_ip_init_eth(&eth, &ipad);
  if (ret == NETWIB_ERR_OK) {
    ipadvalid = NETWIB_TRUE;
  }

  /* eventually display IP address */
  if (wantip) {
    if (wanttitle) {
      netwib_er(netwib_fmt_display("IP address:  "));
    }
    if (ipadvalid) {
      netwib_er(netwib_fmt_display("%{ip}\n", &ipad));
    } else {
      netwib_er(netwib_fmt_display("unresolved\n"));
    }
  }

  /* eventually obtain and display hostname */
  if (wanthost) {
    if (wanttitle) {
      netwib_er(netwib_fmt_display("Hostname:    "));
    }
    if (ipadvalid) {
      netwib_er(netwib_buf_init_mallocdefault(&buf));
      ret = netwib_buf_append_ip(&ipad, NETWIB_IP_ENCODETYPE_HN, &buf);
      if (ret == NETWIB_ERR_OK) {
        netwib_er(netwib_fmt_display("%{buf}\n", &buf));
      } else {
        netwib_er(netwib_fmt_display("unresolved\n"));
      }
      netwib_er(netwib_buf_close(&buf));
    } else {
      netwib_er(netwib_fmt_display("unresolved\n"));
    }
  }
  if (wanthosts) {
    if (wanttitle) {
      netwib_er(netwib_fmt_display("Hostnames:   "));
    }
    if (ipadvalid) {
      netwib_er(netwib_buf_init_mallocdefault(&buf));
      ret = netwib_buf_append_ip(&ipad, NETWIB_IP_ENCODETYPE_HNS, &buf);
      if (ret == NETWIB_ERR_OK) {
        netwib_er(netwib_fmt_display("%{buf}\n", &buf));
      } else {
        netwib_er(netwib_fmt_display("unresolved\n"));
      }
      netwib_er(netwib_buf_close(&buf));
    } else {
      netwib_er(netwib_fmt_display("unresolved\n"));
    }
  }

  return(NETWIB_ERR_OK);
}








