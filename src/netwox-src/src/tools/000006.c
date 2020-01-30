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
netwib_conststring t000006_description[] = {
  "This tool displays source and destination addresses used to reach a",
  "remote computer.",
  "It shows:",
  " - associated device",
  " - local/source IP address",
  " - IP address of host to reach",
  " - local/source Ethernet address",
  " - destination Ethernet address (address of gateway/router if computer",
  "   is not on the local Ethernet LAN)",
  NETWOX_DESC_toolpriv_snsp_resolveth,
  NULL
};
netwox_toolarg t000006_args[] = {
  NETWOX_TOOLARG_OPT_BOOL('t', "title", "display titles", NULL),
  NETWOX_TOOLARG_OPT_BOOL('d', "device", "obtain device", NULL),
  NETWOX_TOOLARG_OPT_BOOL('i', "src-ip", "obtain source IP", NULL),
  NETWOX_TOOLARG_OPT_BOOL('I', "dst-ip", "display destination IP", NULL),
  NETWOX_TOOLARG_OPT_BOOL('e', "src-eth", "obtain source Eth", NULL),
  NETWOX_TOOLARG_OPT_BOOL('E', "dst-eth", "obtain destination Eth", NULL),
  NETWOX_TOOLARG_REQ_IP('q', NULL, NULL, NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000006_nodes[] = {
  NETWOX_TOOLTREENODETYPE_INFO_REMOTE,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000006_info = {
  "Display how to reach an IP address",
  t000006_description,
  "device, gateway, ip4, ip6, mac, router",
  t000006_args,
  t000006_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000006_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_bool wanttitle;
  netwib_bool wantdevice, wantsrcip, wantdstip, wantsrceth, wantdsteth;
  netwib_bool setdevice, setsrcip, setdstip, setsrceth, setdsteth;
  netwib_buf device;
  netwib_ip srcip, dstip;
  netwib_eth srceth, dsteth;
  netwib_err ret;

  netwib_er(netwox_arg_init(argc, argv, &t000006_info, &parg));
  netwib_er(netwox_arg_bool(parg, 't', &wanttitle));
  netwib_er(netwox_arg_bool(parg, 'd', &wantdevice));
  netwib_er(netwox_arg_bool(parg, 'i', &wantsrcip));
  netwib_er(netwox_arg_bool(parg, 'I', &wantdstip));
  netwib_er(netwox_arg_bool(parg, 'e', &wantsrceth));
  netwib_er(netwox_arg_bool(parg, 'E', &wantdsteth));
  netwib_er(netwox_arg_ip(parg, 'q', &dstip));
  netwib_er(netwox_arg_close(&parg));

  /* if no option is set, they are all displayed */
  if (!wanttitle && !wantdevice && !wantsrcip && !wantdstip && !wantsrceth &&
      !wantdsteth) {
    wanttitle = NETWIB_TRUE;
    wantdevice = NETWIB_TRUE;
    wantsrcip = NETWIB_TRUE;
    wantdstip = NETWIB_TRUE;
    wantsrceth = NETWIB_TRUE;
    wantdsteth = NETWIB_TRUE;
  }

  /* obtain values */
  setdevice = NETWIB_FALSE;
  setsrcip = NETWIB_FALSE;
  setdstip = NETWIB_TRUE;
  setsrceth = NETWIB_FALSE;
  setdsteth = NETWIB_FALSE;
  netwib_er(netwib_buf_init_mallocdefault(&device));
  ret = netwib_conf_routes_reach_ip(&dstip, &device, &srcip);
  switch(ret) {
    case NETWIB_ERR_OK :
      if (wantsrceth || wantdsteth) {
        netwib__buf_reinit(&device);
        ret = netwib_conf_routes_reach_eth(&dstip, &device, &srceth,
                                          &dsteth, &srcip);
        switch(ret) {
          case NETWIB_ERR_OK :
            setdevice = NETWIB_TRUE;
            setsrcip = NETWIB_TRUE;
            setsrceth = NETWIB_TRUE;
            setdsteth = NETWIB_TRUE;
            break;
          case NETWIB_ERR_ROUTELOCALDEV :
            setdevice = NETWIB_TRUE;
            break;
          case NETWIB_ERR_ROUTENOTETHER :
            netwib_er(netwib_conf_routes_reach_ip(&dstip, &device, &srcip));
            setdevice = NETWIB_TRUE;
            setsrcip = NETWIB_TRUE;
            break;
          case NETWIB_ERR_ROUTENOTFOUND :
          default :
            break;
        }
      } else {
        setdevice = NETWIB_TRUE;
        setsrcip = NETWIB_TRUE;
      }
      break;
    case NETWIB_ERR_ROUTELOCALDEV :
      setdevice = NETWIB_TRUE;
      break;
    case NETWIB_ERR_ROUTENOTFOUND :
    default :
      break;
  }

  /* display */
  if (wantdevice) {
    if (wanttitle) {
      netwib_er(netwib_fmt_display("Device:           "));
    }
    if (setdevice) {
      netwib_er(netwib_fmt_display("%{buf}\n", &device));
    } else {
      netwib_er(netwib_fmt_display("unresolved\n"));
    }
  }
  if (wantsrcip) {
    if (wanttitle) {
      netwib_er(netwib_fmt_display("Source IP:        "));
    }
    if (setsrcip) {
      netwib_er(netwib_fmt_display("%{ip}\n", &srcip));
    } else {
      netwib_er(netwib_fmt_display("unresolved\n"));
    }
  }
  if (wantdstip) {
    if (wanttitle) {
      netwib_er(netwib_fmt_display("Destination IP:   "));
    }
    if (setdstip) {
      netwib_er(netwib_fmt_display("%{ip}\n", &dstip));
    } else {
      netwib_er(netwib_fmt_display("unresolved\n"));
    }
  }
  if (wantsrceth) {
    if (wanttitle) {
      netwib_er(netwib_fmt_display("Source Eth:       "));
    }
    if (setsrceth) {
      netwib_er(netwib_fmt_display("%{eth}\n", &srceth));
    } else {
      netwib_er(netwib_fmt_display("unresolved\n"));
    }
  }
  if (wantdsteth) {
    if (wanttitle) {
      netwib_er(netwib_fmt_display("Destination Eth:  "));
    }
    if (setdsteth) {
      netwib_er(netwib_fmt_display("%{eth}\n", &dsteth));
    } else {
      netwib_er(netwib_fmt_display("unresolved\n"));
    }
  }

  netwib_er(netwib_buf_close(&device));

  return(NETWIB_ERR_OK);
}
