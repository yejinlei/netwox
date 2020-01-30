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
struct netwib_conf_routes_index {
  netwib_conf_routes *pitem;
  netwib_ring_index *pringindex;
};

/*-------------------------------------------------------------*/
netwib_err netwib_conf_routes_index_init(netwib_conf_routes *pitem,
                                         netwib_conf_routes_index **ppindex)
{
  netwib_conf_routes_index *pindex;

  /* parameter verification */
  if (ppindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  netwib__priv_conf_eventuallyupdate();

  /* allocate needed memory to store pindex */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_conf_routes_index),
                              (netwib_ptr*)&pindex));
  *ppindex = pindex;

  /* prepare for storage and loop */
  pindex->pitem = pitem;
  netwib_er(netwib_ring_index_init(netwib_priv_conf.proutes,
                                   &pindex->pringindex));

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_add_conf_routes_index(pindex));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_conf_routes_index_close(netwib_conf_routes_index **ppindex)
{
  netwib_conf_routes_index *pindex;

  /* parameter verification */
  if (ppindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pindex = *ppindex;

  /* end storage and loop */
  netwib_er(netwib_ring_index_close(&pindex->pringindex));

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_del_conf_routes_index(pindex));
#endif

  /* free memory */
  netwib_er(netwib_ptr_free((netwib_ptr*)&pindex));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_conf_routes_index_next(netwib_conf_routes_index *pindex)
{
  netwib_conf_routes *pitem;
  netwib_err ret;

  /* parameter verification */
  if (pindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_conf_routes_index(pindex));
#endif

  netwib_er(netwib_priv_conf_rdlock());
  ret = netwib_ring_index_next(pindex->pringindex, (netwib_ptr*)&pitem);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_priv_conf_rdunlock());
    return(ret);
  }
  pindex->pitem->devnum = pitem->devnum;
  pindex->pitem->dst = pitem->dst;
  pindex->pitem->mask = pitem->mask;
  pindex->pitem->prefix = pitem->prefix;
  pindex->pitem->srcset = pitem->srcset;
  pindex->pitem->src = pitem->src;
  pindex->pitem->gwset = pitem->gwset;
  pindex->pitem->gw = pitem->gw;
  pindex->pitem->metric = pitem->metric;
  netwib_er(netwib_priv_conf_rdunlock());

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_conf_routes2(netwib_conf_routes *pconf,
                                                 netwib_buf *pbuf)
{
  netwib_byte array[81];
  netwib_buf buf;

  netwib_er(netwib_buf_append_fmt(pbuf, "%{l 2;uint32} ", pconf->devnum));

  switch(pconf->dst.iptype) {
    case NETWIB_IPTYPE_IP4 :
      netwib_er(netwib_buf_append_fmt(pbuf, "%{l 15;ip}/%{l 15;ip} ",
                                      &pconf->dst, &pconf->mask));
      break;
    case NETWIB_IPTYPE_IP6 :
      netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));
      netwib_er(netwib_buf_append_fmt(&buf, "%{ip}/%{uint32}", &pconf->dst,
                                      pconf->prefix));
      netwib_er(netwib_buf_append_fmt(pbuf, "%{l 31;buf} ", &buf));
      break;
    default :
      return(NETWIB_ERR_PAIPTYPE);
  }
  if (pconf->srcset && pconf->gwset) {
    netwib_er(netwib_buf_append_fmt(pbuf, "%{l 19;ip} %{l 20;ip}",
                                    &pconf->src, &pconf->gw));
  } else if (pconf->srcset) {
    netwib_er(netwib_buf_append_fmt(pbuf, "%{l 40;ip}", &pconf->src));
  } else if (pconf->gwset) {
    netwib_er(netwib_buf_append_fmt(pbuf, "unknown             %{l 20;ip}",
                                    &pconf->gw));
  } else {
    netwib_er(netwib_buf_append_string("local                                   ",
                                     pbuf));
  }

  netwib_er(netwib_buf_append_fmt(pbuf, " %{r 3;uint32}\n", pconf->metric));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_conf_routes(netwib_buf *poutbuf)
{
  netwib_conf_routes conf;
  netwib_conf_routes_index *pconfindex;
  netwib_bool displayheader;
  netwib_err ret;

  netwib_er(netwib_conf_routes_index_init(&conf, &pconfindex));
  ret = NETWIB_ERR_OK;
  displayheader = NETWIB_TRUE;
  while (NETWIB_TRUE) {
    ret = netwib_conf_routes_index_next(pconfindex);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    if (displayheader) {
      ret = netwib_buf_append_fmt(poutbuf, "nu destination    /netmask         source              gateway           metric\n");
      if (ret != NETWIB_ERR_OK) {
        break;
      }
      displayheader = NETWIB_FALSE;
    }
    ret = netwib_buf_append_conf_routes2(&conf, poutbuf);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_conf_routes_index_close(&pconfindex));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_conf_routes_display(void)
{
  netwib_err ret;
  netwib_string pc;
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  ret = netwib_buf_append_conf_routes(&buf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_ref_string(&buf, &pc));
    fprintf(stdout, "%s", pc);
    fflush(stdout);
  }
  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_conf_routes_reach_eth2(netwib_uint32 devnum,
                                                netwib_buf *pdevice,
                                                netwib_bool *pisethernet,
                                                netwib_eth *psrceth)
{
  netwib_conf_devices conf;
  netwib_conf_devices_index *pconfindex;
  netwib_err ret;

  netwib_er(netwib_conf_devices_index_init(&conf, &pconfindex));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    ret = netwib_conf_devices_index_next(pconfindex);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_NOTFOUND;
      break;
    }
    if (conf.devnum != devnum) {
      continue;
    }
    ret = netwib_buf_append_buf(&conf.deviceeasy, pdevice);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (conf.hwtype == NETWIB_DEVICE_HWTYPE_ETHER) {
      if (pisethernet != NULL) *pisethernet = NETWIB_TRUE;
      if (psrceth != NULL) *psrceth = conf.eth;
    } else {
      if (pisethernet != NULL) *pisethernet = NETWIB_FALSE;
    }
    break;
  }
  netwib_er(netwib_conf_devices_index_close(&pconfindex));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_conf_routes_reach_eth(netwib_constip *pdstip,
                                        netwib_buf *pdevice,
                                        netwib_eth *psrceth,
                                        netwib_eth *pdsteth,
                                        netwib_ip *psrcip)
{
  netwib_conf_routes conf;
  netwib_conf_routes_index *pconfindex;
  netwib_bool routenotetherfound, isethernet;
  netwib_buf device;
  netwib_cmp cmp;
  netwib_err ret;

  netwib_er(netwib_buf_init_mallocdefault(&device));
  netwib_er(netwib_conf_routes_index_init(&conf, &pconfindex));
  ret = NETWIB_ERR_OK;
  routenotetherfound = NETWIB_FALSE;
  while (NETWIB_TRUE) {
    /* obtain next entry */
    ret = netwib_conf_routes_index_next(pconfindex);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) {
        if (routenotetherfound) {
          ret = NETWIB_ERR_ROUTENOTETHER;
        } else {
          ret = NETWIB_ERR_ROUTENOTFOUND;
        }
      }
      break;
    }
    /* if this route does not match, continue */
    ret = netwib_priv_ip_net_cmp_ipmaskprefix(pdstip, &conf.dst,
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
      ret = netwib_conf_routes_reach_eth2(conf.devnum, pdevice, NULL, NULL);
      if (ret == NETWIB_ERR_OK) {
        ret = NETWIB_ERR_ROUTELOCALDEV;
      }
      break;
    }
    /* check device is Ethernet and obtain its address */
    ret = netwib_conf_routes_reach_eth2(conf.devnum, &device,
                                        &isethernet, psrceth);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (!isethernet) {
      /* try to find another route */
      routenotetherfound = NETWIB_TRUE;
      continue;
    }
    /* obtain dst Ethernet */
    if (conf.gwset) {
      ret = netwib_eth_init_ip(&conf.gw, pdsteth);
    } else {
      ret = netwib_eth_init_ip(pdstip, pdsteth);
    }
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* set other values */
    ret = netwib_buf_append_buf(&device, pdevice);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (psrcip != NULL) {
      *psrcip = conf.src;
    }
    /* found, so finish loop */
    break;
  }
  netwib_er(netwib_conf_routes_index_close(&pconfindex));
  netwib_er(netwib_buf_close(&device));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_conf_routes_reach_ip(netwib_constip *pdstip,
                                       netwib_buf *pdevice,
                                       netwib_ip *psrcip)
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
    ret = netwib_priv_ip_net_cmp_ipmaskprefix(pdstip, &conf.dst,
                                              &conf.mask, conf.prefix,
                                              &cmp);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (cmp != NETWIB_CMP_EQ) {
      continue;
    }
    /* set values */
    ret = netwib_conf_routes_reach_eth2(conf.devnum, pdevice, NULL, NULL);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (!conf.srcset && !conf.gwset) {
      /* case of local device */
      ret = NETWIB_ERR_ROUTELOCALDEV;
      break;
    }
    if (psrcip != NULL) {
      *psrcip = conf.src;
    }
    /* found, so finish loop */
    break;
  }
  netwib_er(netwib_conf_routes_index_close(&pconfindex));

  return(ret);
}
