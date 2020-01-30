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
netwib_err netwox_ip4opts_pkt_noop(netwib_buf *ppkt)
{
  netwib_ip4opt ip4opt;

  ip4opt.type = NETWIB_IP4OPTTYPE_NOOP;
  netwib_er(netwib_pkt_append_ip4opt(&ip4opt, ppkt));
  netwib_er(netwib_pkt_append_ip4opt(&ip4opt, ppkt));
  netwib_er(netwib_pkt_append_ip4opt(&ip4opt, ppkt));
  ip4opt.type = NETWIB_IP4OPTTYPE_END;
  netwib_er(netwib_pkt_append_ip4opt(&ip4opt, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ip4opts_pkt_rr(netwib_buf *ppkt)
{
  netwib_ip4opt ip4opt;
  netwib_uint32 i;

  ip4opt.type = NETWIB_IP4OPTTYPE_RR;
  ip4opt.opt.rr.storagesize = 9;
  ip4opt.opt.rr.storedvalues = 0;
  for (i = 0; i < ip4opt.opt.rr.storagesize; i++) {
    netwib_er(netwib_ip_init_ip4(0, &ip4opt.opt.rr.ip[i]));
  }
  netwib_er(netwib_pkt_append_ip4opt(&ip4opt, ppkt));

  ip4opt.type = NETWIB_IP4OPTTYPE_END;
  netwib_er(netwib_pkt_append_ip4opt(&ip4opt, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ip4opts_pkt_lsrr(netwib_constips *pips,
                                   netwib_buf *ppkt)
{
  netwib_ip4opt ip4opt;
  netwib_ips_index *pipsindex;
  netwib_ip ipad;
  netwib_uint32 i;
  netwib_err ret=NETWIB_ERR_OK;

  ip4opt.type = NETWIB_IP4OPTTYPE_LSRR;
  ip4opt.opt.lsrr.storagesize = 0;
  ip4opt.opt.lsrr.usedvalues = 0;

  netwib_er(netwib_ips_index_init(pips, &pipsindex));
  i = 0;
  while(NETWIB_TRUE) {
    ret = netwib_ips_index_next_ip(pipsindex, &ipad);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    if (i >= NETWIB_IP4OPT_SRR_IP_LEN) {
      ret = NETWIB_ERR_PATOOHIGH;
      break;
    }
    ip4opt.opt.lsrr.ip[i] = ipad;
    ip4opt.opt.lsrr.storagesize++;
    i++;
  }
  netwib_er(netwib_ips_index_close(&pipsindex));

  netwib_er(netwib_pkt_append_ip4opt(&ip4opt, ppkt));

  ip4opt.type = NETWIB_IP4OPTTYPE_END;
  netwib_er(netwib_pkt_append_ip4opt(&ip4opt, ppkt));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ip4opts_pkt_ssrr(netwib_constips *pips,
                                   netwib_buf *ppkt)
{
  netwib_ip4opt ip4opt;
  netwib_ips_index *pipsindex;
  netwib_ip ipad;
  netwib_uint32 i;
  netwib_err ret=NETWIB_ERR_OK;

  ip4opt.type = NETWIB_IP4OPTTYPE_SSRR;
  ip4opt.opt.ssrr.storagesize = 0;
  ip4opt.opt.ssrr.usedvalues = 0;

  netwib_er(netwib_ips_index_init(pips, &pipsindex));
  i = 0;
  while(NETWIB_TRUE) {
    ret = netwib_ips_index_next_ip(pipsindex, &ipad);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    if (i >= NETWIB_IP4OPT_SRR_IP_LEN) {
      ret = NETWIB_ERR_PATOOHIGH;
      break;
    }
    ip4opt.opt.ssrr.ip[i] = ipad;
    ip4opt.opt.ssrr.storagesize++;
    i++;
  }
  netwib_er(netwib_ips_index_close(&pipsindex));

  netwib_er(netwib_pkt_append_ip4opt(&ip4opt, ppkt));

  ip4opt.type = NETWIB_IP4OPTTYPE_END;
  netwib_er(netwib_pkt_append_ip4opt(&ip4opt, ppkt));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ip4opts_pkt_ts(netwib_buf *ppkt)
{
  netwib_ip4opt ip4opt;
  netwib_uint32 i;

  ip4opt.type = NETWIB_IP4OPTTYPE_TIME;
  ip4opt.opt.time.storagesize = 9;
  ip4opt.opt.time.storedvalues = 0;
  ip4opt.opt.time.overflow = 0;
  ip4opt.opt.time.flag = NETWIB_IP4OPT_TIMEFLAG_TS;
  for (i = 0; i < ip4opt.opt.time.storagesize; i++) {
    ip4opt.opt.time.timestamp[i] = 0;
  }
  netwib_er(netwib_pkt_append_ip4opt(&ip4opt, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ip4opts_pkt_ipts(netwib_buf *ppkt)
{
  netwib_ip4opt ip4opt;
  netwib_uint32 i;

  ip4opt.type = NETWIB_IP4OPTTYPE_TIME;
  ip4opt.opt.time.storagesize = 4;
  ip4opt.opt.time.storedvalues = 0;
  ip4opt.opt.time.overflow = 0;
  ip4opt.opt.time.flag = NETWIB_IP4OPT_TIMEFLAG_IPTS;
  for (i = 9; i < ip4opt.opt.time.storagesize; i++) {
    netwib_er(netwib_ip_init_ip4(0, &ip4opt.opt.time.ip[i]));
    ip4opt.opt.time.timestamp[i] = 0;
  }
  netwib_er(netwib_pkt_append_ip4opt(&ip4opt, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ip4opts_pkt_ippts(netwib_constip *pip,
                                    netwib_buf *ppkt)
{
  netwib_ip4opt ip4opt;

  ip4opt.type = NETWIB_IP4OPTTYPE_TIME;
  ip4opt.opt.time.storagesize = 1;
  ip4opt.opt.time.storedvalues = 0;
  ip4opt.opt.time.overflow = 0;
  ip4opt.opt.time.flag = NETWIB_IP4OPT_TIMEFLAG_IPPTS;
  ip4opt.opt.time.ip[0] = *pip;
  ip4opt.opt.time.timestamp[0] = 0;
  netwib_er(netwib_pkt_append_ip4opt(&ip4opt, ppkt));

  return(NETWIB_ERR_OK);
}
