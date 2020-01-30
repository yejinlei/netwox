/*
                                  NETWIB
                             Network library
                Copyright(c) 1999-2012 Laurent CONSTANTIN
                      http://ntwib.sourceforge.net/
                        laurentconstantin@free.fr
                                  -----
  This file is part of Netwib.

  Netwib is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3
  as published by the Free Software Foundation.

  Netwib is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details (http://www.gnu.org/).

------------------------------------------------------------------------
*/

#include <netwib/inc/maininc.h>

/*-------------------------------------------------------------*/
#include "priv/conf.c"
#include "priv/confwork.c"
#include "priv/confdev.c"
#include "priv/confip.c"
#include "priv/confarp.c"
#include "priv/confrout.c"

/*-------------------------------------------------------------*/
#include "priv/sa.c"
#include "priv/ip.c"
#include "priv/ranges.c"
#include "priv/ips.c"

/*-------------------------------------------------------------*/
#include "priv/libpcap.c"

/*-------------------------------------------------------------*/
netwib_err netwib_priv_conf_routes_gw(netwib_constip *pdst,
                                      netwib_ip *pgw,
                                      netwib_uint32 *pdevnum)
{
  netwib_conf_routes conf;
  netwib_conf_routes_index *pconfindex;
  netwib_cmp cmp;
  netwib_err ret;

  netwib_er(netwib_conf_routes_index_init(&conf, &pconfindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    /* obtain next entry */
    ret = netwib_conf_routes_index_next(pconfindex);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) {
        ret = NETWIB_ERR_ROUTENOTFOUND;
      }
      break;
    }
    /* if this route does not match, continue */
    ret = netwib_priv_ip_net_cmp_ipmaskprefix(pdst, &conf.dst,
                                              &conf.mask, conf.prefix,
                                              &cmp);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (cmp != NETWIB_CMP_EQ) {
      continue;
    }
    /* case of local device */
    if (!conf.srcset && !conf.gwset) {
      ret = NETWIB_ERR_ROUTELOCALDEV;
      break;
    }
    /* set output */
    if (conf.gwset) {
      *pgw = conf.gw;
    } else {
      *pgw = *pdst;
    }
    /* found, so finish loop */
    if (pdevnum != NULL) *pdevnum = conf.devnum;
    break;
  }
  netwib_er(netwib_conf_routes_index_close(&pconfindex));

  return(ret);
}

