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
struct netwib_conf_ip_index {
  netwib_conf_ip *pitem;
  netwib_ring_index *pringindex;
};

/*-------------------------------------------------------------*/
netwib_err netwib_conf_ip_index_init(netwib_conf_ip *pitem,
                                     netwib_conf_ip_index **ppindex)
{
  netwib_conf_ip_index *pindex;

  /* parameter verification */
  if (ppindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  netwib__priv_conf_eventuallyupdate();

  /* allocate needed memory to store pindex */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_conf_ip_index),
                              (netwib_ptr*)&pindex));
  *ppindex = pindex;

  /* prepare for storage and loop */
  pindex->pitem = pitem;
  netwib_er(netwib_ring_index_init(netwib_priv_conf.pip,
                                   &pindex->pringindex));

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_add_conf_ip_index(pindex));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_conf_ip_index_close(netwib_conf_ip_index **ppindex)
{
  netwib_conf_ip_index *pindex;

  /* parameter verification */
  if (ppindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pindex = *ppindex;

  /* end storage and loop */
  netwib_er(netwib_ring_index_close(&pindex->pringindex));

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_del_conf_ip_index(pindex));
#endif

  /* free memory */
  netwib_er(netwib_ptr_free((netwib_ptr*)&pindex));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_conf_ip_index_next(netwib_conf_ip_index *pindex)
{
  netwib_conf_ip *pitem;
  netwib_err ret;

  /* parameter verification */
  if (pindex == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_conf_ip_index(pindex));
#endif


  netwib_er(netwib_priv_conf_rdlock());
  ret = netwib_ring_index_next(pindex->pringindex, (netwib_ptr*)&pitem);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_priv_conf_rdunlock());
    return(ret);
  }
  pindex->pitem->devnum = pitem->devnum;
  pindex->pitem->ip = pitem->ip;
  pindex->pitem->mask = pitem->mask;
  pindex->pitem->prefix = pitem->prefix;
  pindex->pitem->ispointtopoint = pitem->ispointtopoint;
  pindex->pitem->pointtopointip = pitem->pointtopointip;
  netwib_er(netwib_priv_conf_rdunlock());

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_conf_ip2(netwib_conf_ip *pconf,
                                             netwib_buf *pbuf)
{
  netwib_byte array[81];
  netwib_buf buf;

  netwib_er(netwib_buf_append_fmt(pbuf, "%{l 2;uint32} ", pconf->devnum));

  netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));
  switch(pconf->ip.iptype) {
    case NETWIB_IPTYPE_IP4 :
      netwib_er(netwib_buf_append_fmt(&buf, "%{l 15;ip}/%{l 15;ip}",
                                      &pconf->ip, &pconf->mask));
      break;
    case NETWIB_IPTYPE_IP6 :
      netwib_er(netwib_buf_append_fmt(&buf, "%{ip}/%{uint32}",
                                      &pconf->ip, pconf->prefix));
      break;
    default :
      return(NETWIB_ERR_PAIPTYPE);
  }
  netwib_er(netwib_buf_append_fmt(pbuf, "%{l 42;buf} ", &buf));

  netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32}",
                                  pconf->ispointtopoint));
  if (pconf->ispointtopoint) {
    netwib_er(netwib_buf_append_fmt(pbuf, " %{ip}\n", &pconf->pointtopointip));
  } else {
    netwib_er(netwib_buf_append_string("\n", pbuf));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_conf_ip(netwib_buf *pbuf)
{
  netwib_conf_ip conf;
  netwib_conf_ip_index *pconfindex;
  netwib_bool displayheader;
  netwib_err ret;

  netwib_er(netwib_conf_ip_index_init(&conf, &pconfindex));
  ret = NETWIB_ERR_OK;
  displayheader = NETWIB_TRUE;
  while (NETWIB_TRUE) {
    ret = netwib_conf_ip_index_next(pconfindex);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    if (displayheader) {
      ret = netwib_buf_append_fmt(pbuf, "nu ip             /netmask                    ppp point_to_point_with\n");
      if (ret != NETWIB_ERR_OK) {
        break;
      }
      displayheader = NETWIB_FALSE;
    }
    ret = netwib_buf_append_conf_ip2(&conf, pbuf);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_conf_ip_index_close(&pconfindex));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_conf_ip_display(void)
{
  netwib_err ret;
  netwib_string pc;
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  ret = netwib_buf_append_conf_ip(&buf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_ref_string(&buf, &pc));
    fprintf(stdout, "%s", pc);
    fflush(stdout);
  }
  netwib_er(netwib_buf_close(&buf));

  return(ret);
}
