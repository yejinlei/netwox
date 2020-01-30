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
netwib_conststring t000003_description[] = {
  "This tool obtains addresses of a computer.",
  "For example, from an IP address, obtain hostname and Ethernet address.",
  "For example, from a hostname, obtain IP address and Ethernet address.",
  "",
  "Parameter --title displays title in front of each result.",
  "Parameter --ip displays IP address.",
  "Parameter --host displays main hostname.",
  "Parameter --hosts displays list of hostnames (one IP address can have",
  "several hostnames).",
  "Parameter --eth displays Ethernet address.",
  "If no option (ip, host, hosts, eth) is set, they are all displayed.",
  "Parameter --all displays information about a list of addresses (for",
  "example 'netwox 3 -a 192.168.0.0/24').",
  NETWOX_DESC_toolpriv_snsp_resolveth,
  NULL
};
netwox_toolarg t000003_args[] = {
  NETWOX_TOOLARG_OPTA_BOOL('t', "title", "display titles", NULL),
  NETWOX_TOOLARG_OPTA_BOOL('i', "ip", "obtain IP address", NULL),
  NETWOX_TOOLARG_OPTA_BOOL('h', "host", "obtain hostname", NULL),
  NETWOX_TOOLARG_OPTA_BOOL('H', "hosts", "obtain hostnames", NULL),
  NETWOX_TOOLARG_OPTA_BOOL('e', "eth", "obtain Ethernet address", NULL),
  NETWOX_TOOLARG_OPTA_BOOL('a', "all", "display all IP addresses", NULL),
  NETWOX_TOOLARG_REQ_BUF_HOSTNAME('q', "query", "IP address or hostname",
                                  NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000003_nodes[] = {
  NETWOX_TOOLTREENODETYPE_INFO_REMOTE,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000003_info = {
  "Display information about an IP address or a hostname",
  t000003_description,
  "ethernet, ip4, ip6, mac, resolve, show",
  t000003_args,
  t000003_nodes,
};

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bool wantip;
  netwib_bool wanthost;
  netwib_bool wanthosts;
  netwib_bool wanteth;
  netwib_bool wanttitle;
  netwib_bufext query;
  netwib_bool ipadvalid;
  netwib_ip ipad;
} t000003_profile;

/*-------------------------------------------------------------*/
static netwib_err t000003_disp(t000003_profile *pprofile,
                               netwib_bufpool *pbufpool)
{
  netwib_eth eth;
  netwib_buf *pbuf;
  netwib_bool testedandfailed;
  netwib_err ret=NETWIB_ERR_OK;

  /* eventually display IP address */
  if (pprofile->wantip) {
    if (pprofile->wanttitle) {
      netwib_er(netwib_fmt_display("IP address:  "));
    }
    if (pprofile->ipadvalid) {
      netwib_er(netwib_fmt_display("%{ip}\n", &pprofile->ipad));
    } else {
      netwib_er(netwib_fmt_display("unresolved\n"));
    }
  }

  /* eventually obtain and display hostname */
  testedandfailed = NETWIB_FALSE;
  if (pprofile->wanthost) {
    if (pprofile->wanttitle) {
      netwib_er(netwib_fmt_display("Hostname:    "));
    }
    if (pprofile->ipadvalid) {
      netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
      ret = netwib_buf_append_ip(&pprofile->ipad, NETWIB_IP_ENCODETYPE_HN,
                                 pbuf);
      if (ret == NETWIB_ERR_OK) {
        netwib_er(netwib_fmt_display("%{buf}\n", pbuf));
      } else {
        testedandfailed = NETWIB_TRUE;
        netwib_er(netwib_fmt_display("unresolved\n"));
      }
      netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));
    } else {
      netwib_er(netwib_fmt_display("unresolved\n"));
    }
  }
  if (pprofile->wanthosts) {
    if (pprofile->wanttitle) {
      netwib_er(netwib_fmt_display("Hostnames:   "));
    }
    if (pprofile->ipadvalid) {
      netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
      if (testedandfailed) {
        ret = NETWIB_ERR_NOTCONVERTED;
      } else {
        ret = netwib_buf_append_ip(&pprofile->ipad, NETWIB_IP_ENCODETYPE_HNS,
                                   pbuf);
      }
      if (ret == NETWIB_ERR_OK) {
        netwib_er(netwib_fmt_display("%{buf}\n", pbuf));
      } else {
        netwib_er(netwib_fmt_display("unresolved\n"));
      }
      netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));
    } else {
      netwib_er(netwib_fmt_display("unresolved\n"));
    }
  }

  /* eventually obtain and display Ethernet address */
  if (pprofile->wanteth) {
    if (pprofile->wanttitle) {
      netwib_er(netwib_fmt_display("Eth address: "));
    }
    if (pprofile->ipadvalid) {
      ret = netwib_eth_init_ip(&pprofile->ipad, &eth);
      if (ret == NETWIB_ERR_OK) {
        netwib_er(netwib_fmt_display("%{eth}\n", &eth));
      } else {
        netwib_er(netwib_fmt_display("unresolved\n"));
      }
    } else {
      netwib_er(netwib_fmt_display("unresolved\n"));
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err t000003_core(int argc, char *argv[])
{
  netwox_arg *parg;
  t000003_profile profile;
  netwib_bool displayall, addaline;
  netwib_bufpool *pbufpool;
  netwib_ips *pips;
  netwib_ips_index *pipsindex;
  netwib_err ret;

  netwib_er(netwox_arg_init(argc, argv, &t000003_info, &parg));
  netwib_er(netwox_arg_bool(parg, 't', &profile.wanttitle));
  netwib_er(netwox_arg_bool(parg, 'i', &profile.wantip));
  netwib_er(netwox_arg_bool(parg, 'h', &profile.wanthost));
  netwib_er(netwox_arg_bool(parg, 'H', &profile.wanthosts));
  netwib_er(netwox_arg_bool(parg, 'e', &profile.wanteth));
  netwib_er(netwox_arg_bool(parg, 'a', &displayall));
  netwib_er(netwox_arg_buf(parg, 'q', &profile.query));

  /* if no option is set, they are all displayed */
  if (!profile.wanttitle && !profile.wantip && !profile.wanthost &&
      !profile.wanthosts && !profile.wanteth) {
    profile.wanttitle = NETWIB_TRUE;
    profile.wantip = NETWIB_TRUE;
    profile.wanthost = NETWIB_TRUE;
    profile.wanthosts = NETWIB_TRUE;
    profile.wanteth = NETWIB_TRUE;
  }

  netwib_er(netwib_bufpool_initdefault(&pbufpool));

  profile.ipadvalid = NETWIB_FALSE;
  if (displayall) {
    /* obtain IP addresses, and display them all */
    netwib_er(netwib_ips_init(NETWIB_IPS_INITTYPE_NOTSORTUNIQ, &pips));
    ret = netwib_ips_add_buf(pips, &profile.query);
    if (ret == NETWIB_ERR_OK) {
      addaline = NETWIB_FALSE;
      profile.ipadvalid = NETWIB_TRUE;
      netwib_er(netwib_ips_index_init(pips, &pipsindex));
      while (NETWIB_TRUE) {
        ret = netwib_ips_index_next_ip(pipsindex, &profile.ipad);
        if (ret != NETWIB_ERR_OK) {
          if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
          break;
        }
        if (addaline) {
          netwib_er(netwib_fmt_display("\n"));
        }
        netwib_er(t000003_disp(&profile, pbufpool));
        addaline = NETWIB_TRUE;
      }
      netwib_er(netwib_ips_index_close(&pipsindex));
    } else {
      netwib_er(t000003_disp(&profile, pbufpool));
    }
    netwib_er(netwib_ips_close(&pips));
  } else {
    /* only obtain and display first address */
    ret = netwib_ip_init_buf_best(&profile.query, &profile.ipad);
    if (ret == NETWIB_ERR_OK) {
      profile.ipadvalid = NETWIB_TRUE;
    }
    netwib_er(t000003_disp(&profile, pbufpool));
  }

  netwib_er(netwib_bufpool_close(&pbufpool));
  netwib_er(netwox_arg_close(&parg));
  return(NETWIB_ERR_OK);
}
