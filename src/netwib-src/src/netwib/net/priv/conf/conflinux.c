/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
#include "unix/devices_ioctl.c"

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_ip6_init_bufhexa(netwib_buf *pbuf,
                                                    netwib_ip *pip)
{
  netwib_buf buf;
  netwib_data data;
  netwib_uint32 datasize;
  netwib_err ret;

  ret = NETWIB_ERR_OK;
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_eg(netwib_buf_decode(pbuf, NETWIB_DECODETYPE_HEXA, &buf));
  datasize = netwib__buf_ref_data_size(&buf);
  if (datasize != NETWIB_IP6_LEN) {
    netwib_goto(NETWIB_ERR_NOTCONVERTED);
  }
  data = netwib__buf_ref_data_ptr(&buf);
  pip->iptype = NETWIB_IPTYPE_IP6;
  netwib_c_memcpy(pip->ipvalue.ip6.b, data, NETWIB_IP6_LEN);

  /* clean and leave */
 netwib_gotolabel:
  netwib_er(netwib_buf_close(&buf));
  return(ret);
}

/*-------------------------------------------------------------*/
/*
Linux 2.4.22
/proc/net/if_inet6
fec00000000000110000000000000001 04 40 40 80     eth1
fec00000000000100000000000000001 03 40 40 80     eth0
fe80000000000000024095fffe46aaaa 03 40 20 80     eth0
00000000000000000000000000000001 01 80 10 80       lo
fe8000000000000002104bfffe44aaaa 04 40 20 80     eth1
*/
static netwib_err netwib_priv_conf_procnetifinet6(netwib_priv_confwork *pcw)
{
  netwib_io *piofile, *pioline;
  netwib_priv_confwork_ip *pci;
  netwib_buf buf, buf2;
  netwib_bool pciset;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string("/proc/net/if_inet6", &buf));
  ret = netwib_io_init_file_read(&buf, &piofile);
  if (ret != NETWIB_ERR_OK) {
    if (ret == NETWIB_ERR_NOTFOUND) {
      /* this is the case where kernel does not support rarp */
      ret = NETWIB_ERR_LONOTSUPPORTED;
    }
    return(ret);
  }
  netwib_er(netwib_io_init_data_line(&pioline));
  netwib_er(netwib_io_plug_read(pioline, piofile));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));

  /* prepare for error handling */
  pciset = NETWIB_FALSE;
  ret = NETWIB_ERR_OK;

  /* read each line */
  while (NETWIB_TRUE) {
    netwib__buf_reinit(&buf);
    netwib__buf_reinit(&buf2);
    ret = netwib_io_read(pioline, &buf);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_eg(netwib_priv_confwork_ip_init(&pci));
    pciset = NETWIB_TRUE;
    ret = netwib_buf_decode_fmt(&buf, "%{buf} %{*;uint32:X} %{uint32:X} %{*;uint32:X} %{*;uint32:X} %{buf}",
                                &buf2, &pci->prefix, &pci->device);
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      /* ignore unrecognized lines */
      netwib_eg(netwib_priv_confwork_ip_close(&pci));
      pciset = NETWIB_FALSE;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* decode IP */
    netwib_eg(netwib_priv_conf_ip6_init_bufhexa(&buf2, &pci->ip));
    /* add it */
    netwib_eg(netwib_priv_confwork_ip_add(pcw, pci));
    pciset = NETWIB_FALSE;
  }

  /* clean and leave */
 netwib_gotolabel:
  netwib_er(netwib_buf_close(&buf2));
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_io_close(&pioline));
  if (pciset) {
    netwib_er(netwib_priv_confwork_ip_close(&pci));
  }
  return(ret);
}

/*-------------------------------------------------------------*/
/*
Linux 2.4.22
/proc/net/arp
IP address       HW type     Flags       HW address            Mask     Device
192.168.1.3      0x1         0x0         00:00:00:00:00:00     *        eth0
192.168.1.2      0x1         0x6         01:01:01:01:01:01     *        eth0
*/
static netwib_err netwib_priv_conf_procnetarp(netwib_priv_confwork *pcw)
{
  netwib_io *piofile, *pioline;
  netwib_priv_confwork_arpcache *pca;
  netwib_buf buf;
  netwib_bool pcaset;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string("/proc/net/arp", &buf));
  ret = netwib_io_init_file_read(&buf, &piofile);
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }
  netwib_er(netwib_io_init_data_line(&pioline));
  netwib_er(netwib_io_plug_read(pioline, piofile));

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  /* prepare for error handling */
  pcaset = NETWIB_FALSE;
  ret = NETWIB_ERR_OK;

  /* read each line */
  while (NETWIB_TRUE) {
    netwib__buf_reinit(&buf);
    ret = netwib_io_read(pioline, &buf);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_eg(netwib_priv_confwork_arpcache_init(&pca));
    pcaset = NETWIB_TRUE;
    ret = netwib_buf_decode_fmt(&buf, "%{ip} 0x%{*;uint32:X} 0x%{*;uint32:X} %{eth} %{*;s} %{buf}", &pca->ip, &pca->eth, &pca->device);
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      /* ignore unrecognized lines */
      netwib_eg(netwib_priv_confwork_arpcache_close(&pca));
      pcaset = NETWIB_FALSE;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (pca->eth.b[0] == 0 && pca->eth.b[1] == 0 && pca->eth.b[2] == 0 &&
        pca->eth.b[3] == 0 && pca->eth.b[4] == 0 && pca->eth.b[5] == 0) {
      /* Linux kernel sets 00:00:00:00:00:00 for unresolved entries */
      netwib_eg(netwib_priv_confwork_arpcache_close(&pca));
      pcaset = NETWIB_FALSE;
      continue;
    }
    netwib_eg(netwib_priv_confwork_arpcache_add(pcw, pca));
    pcaset = NETWIB_FALSE;
  }

  /* clean and leave */
 netwib_gotolabel:
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_io_close(&pioline));
  if (pcaset) {
    netwib_er(netwib_priv_confwork_arpcache_close(&pca));
  }
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_procnetrarp(netwib_priv_confwork *pcw)
{
  netwib_io *piofile, *pioline;
  netwib_priv_confwork_arpcache *pca;
  netwib_buf buf;
  netwib_bool pcaset;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string("/proc/net/rarp", &buf));
  ret = netwib_io_init_file_read(&buf, &piofile);
  if (ret != NETWIB_ERR_OK) {
    if (ret == NETWIB_ERR_NOTFOUND) {
      /* this this the case where kernel does not support rarp */
      ret = NETWIB_ERR_LONOTSUPPORTED;
    }
    return(ret);
  }
  netwib_er(netwib_io_init_data_line(&pioline));
  netwib_er(netwib_io_plug_read(pioline, piofile));

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  /* prepare for error handling */
  pcaset = NETWIB_FALSE;
  ret = NETWIB_ERR_OK;

  /* read each line */
  while (NETWIB_TRUE) {
    netwib__buf_reinit(&buf);
    ret = netwib_io_read(pioline, &buf);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_eg(netwib_priv_confwork_arpcache_init(&pca));
    pcaset = NETWIB_TRUE;
    ret = netwib_buf_decode_fmt(&buf, "%{ip} %{*;s:glob} %{eth}",
                                &pca->ip, &pca->eth);
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      /* ignore unrecognized lines */
      netwib_eg(netwib_priv_confwork_arpcache_close(&pca));
      pcaset = NETWIB_FALSE;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    netwib_eg(netwib_priv_confwork_arpcache_add(pcw, pca));
    pcaset = NETWIB_FALSE;
  }

  /* clean and leave */
 netwib_gotolabel:
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_io_close(&pioline));
  if (pcaset) {
    netwib_er(netwib_priv_confwork_arpcache_close(&pca));
  }
  return(ret);
}

/*-------------------------------------------------------------*/
/*
Linux 2.4.22
/bin/ip neigh show
192.168.1.3 dev eth0  nud failed
192.168.1.2 dev eth0 lladdr 01:01:01:01:01:01 nud permanent
*/
static netwib_err netwib_priv_conf_binip(netwib_priv_confwork *pcw)
{
  netwib_io *piofile, *pioline;
  netwib_priv_confwork_arpcache *pca;
  netwib_buf buf;
  netwib_bool pcaset, yes;
  netwib_err ret;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_string("/bin/ip", &buf));
  netwib_er(netwib_filename_exists(&buf, &yes));
  if (!yes) {
    netwib__buf_reinit(&buf);
    netwib_er(netwib_buf_append_string("/sbin/ip", &buf));
    netwib_er(netwib_filename_exists(&buf, &yes));
    if (!yes) {
      netwib_er(netwib_buf_close(&buf));
      return(NETWIB_ERR_LONOTSUPPORTED);
    }
  }
  netwib_er(netwib_buf_append_string(" neigh show", &buf));
  ret = netwib_io_init_exec(&buf, NETWIB_IO_WAYTYPE_READ, NETWIB_FALSE,
                            NULL, NULL, &piofile);
  netwib_er(netwib_buf_close(&buf));
  if (ret != NETWIB_ERR_OK) {
    if (ret == NETWIB_ERR_NOTFOUND) {
      /* this this the case where kernel does not support rarp */
      ret = NETWIB_ERR_LONOTSUPPORTED;
    }
    return(ret);
  }
  netwib_er(netwib_io_init_data_line(&pioline));
  netwib_er(netwib_io_plug_read(pioline, piofile));

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  /* prepare for error handling */
  pcaset = NETWIB_FALSE;
  ret = NETWIB_ERR_OK;

  /* read each line */
  while (NETWIB_TRUE) {
    netwib__buf_reinit(&buf);
    ret = netwib_io_read(pioline, &buf);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_eg(netwib_priv_confwork_arpcache_init(&pca));
    pcaset = NETWIB_TRUE;
    ret = netwib_buf_decode_fmt(&buf, "%{ip} dev %{buf} lladdr %{eth} ",
                                &pca->ip, &pca->device, &pca->eth);
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      /* ignore unrecognized lines */
      netwib_eg(netwib_priv_confwork_arpcache_close(&pca));
      pcaset = NETWIB_FALSE;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    netwib_eg(netwib_priv_confwork_arpcache_add(pcw, pca));
    pcaset = NETWIB_FALSE;
  }

  /* clean and leave */
 netwib_gotolabel:
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_io_close(&pioline));
  if (pcaset) {
    netwib_er(netwib_priv_confwork_arpcache_close(&pca));
  }
  return(ret);
}

/*-------------------------------------------------------------*/
/*
Linux 2.4.22
/proc/net/route
Iface   Destination     Gateway         Flags   RefCnt  Use     Metric  Mask            MTU     Window  IRTT
eth0    00AAAAC0        00000000        0001    0       0       0       00FFFFFF        0       0       0
eth1    000AAAC0        00000000        0001    0       0       0       00FFFFFF        0       0       0
eth0    000AAAC0        00000000        0001    0       0       0       00FFFFFF        0       0       0
eth0    00000051        00000000        0001    0       0       0       000000FF        0       0       0
eth0    00000000        FE0AAAC0        0003    0       0       0       00000000        0       0       0
*/
static netwib_err netwib_priv_conf_procnetroute(netwib_priv_confwork *pcw)
{
  netwib_io *piofile, *pioline;
  netwib_priv_confwork_routes *pcr;
  netwib_buf buf;
  netwib_uint32 dst, gw, flags, mask;
  netwib_bool pcrset;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string("/proc/net/route", &buf));
  ret = netwib_io_init_file_read(&buf, &piofile);
  if (ret != NETWIB_ERR_OK) {
    return(ret);
  }
  netwib_er(netwib_io_init_data_line(&pioline));
  netwib_er(netwib_io_plug_read(pioline, piofile));

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  /* prepare for error handling */
  pcrset = NETWIB_FALSE;
  ret = NETWIB_ERR_OK;

  /* read each line */
  while (NETWIB_TRUE) {
    netwib__buf_reinit(&buf);
    ret = netwib_io_read(pioline, &buf);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_eg(netwib_priv_confwork_routes_init(&pcr));
    pcrset = NETWIB_TRUE;
    ret = netwib_buf_decode_fmt(&buf, "%{buf} %{uint32:X} %{uint32:X} %{uint32:X} %{*;uint32} %{*;uint32} %{uint32} %{uint32:X} %{*;uint32} %{*;uint32} %{*;uint32}",
                                &pcr->device, &dst, &gw, &flags, &pcr->metric,
                                &mask);
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      goto trynextentry;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    if ( ! (flags & RTF_UP) ) {
      goto trynextentry;
    }
    netwib_eg(netwib_ip_init_ip4(netwib_priv_ntohl(dst), &pcr->dst));
    netwib_eg(netwib_ip_init_ip4(netwib_priv_ntohl(mask), &pcr->mask));
    if (flags & RTF_GATEWAY) {
      pcr->gwset = NETWIB_TRUE;
      netwib_eg(netwib_ip_init_ip4(netwib_priv_ntohl(gw), &pcr->gw));
    }
    netwib_eg(netwib_priv_confwork_routes_add(pcw, pcr));
    pcrset = NETWIB_FALSE;
   trynextentry:
    if (pcrset) {
      netwib_eg(netwib_priv_confwork_routes_close(&pcr));
      pcrset = NETWIB_FALSE;
    }
  }

  /* clean and leave */
 netwib_gotolabel:
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_io_close(&pioline));
  if (pcrset) {
    netwib_er(netwib_priv_confwork_routes_close(&pcr));
  }
  return(ret);
}

/*-------------------------------------------------------------*/
/*
Linux 2.4.22
/proc/net/ipv6_route
00000000000000000000000000000001 80 00000000000000000000000000000000 00 00000000000000000000000000000000 00000000 00000000 00000000 00200001       lo
fe8000000000000002104bfffeAAAA97 80 00000000000000000000000000000000 00 00000000000000000000000000000000 00000000 00000000 00000000 00200001       lo
fe80000000000000024095fffeAAAAcb 80 00000000000000000000000000000000 00 00000000000000000000000000000000 00000000 00000000 00000000 00200001       lo
fe800000000000000000000000000000 40 00000000000000000000000000000000 00 00000000000000000000000000000000 00000100 00000000 00000000 00040001     eth0
fe800000000000000000000000000000 40 00000000000000000000000000000000 00 00000000000000000000000000000000 00000100 00000000 00000000 00040001     eth1
fec00000000000100000000000000001 80 00000000000000000000000000000000 00 00000000000000000000000000000000 00000000 00000000 00000000 00200001       lo
fec00000000000100000000000000000 40 00000000000000000000000000000000 00 00000000000000000000000000000000 00000100 00000000 00000000 00040001     eth0
fec00000000000110000000000000001 80 00000000000000000000000000000000 00 00000000000000000000000000000000 00000000 00000000 00000000 00200001       lo
fec00000000000110000000000000000 40 00000000000000000000000000000000 00 00000000000000000000000000000000 00000100 00000000 00000000 00040001     eth1
ff000000000000000000000000000000 08 00000000000000000000000000000000 00 00000000000000000000000000000000 00000100 00000000 00000000 00040001     eth0
ff000000000000000000000000000000 08 00000000000000000000000000000000 00 00000000000000000000000000000000 00000100 00000000 00000000 00040001     eth1
00000000000000000000000000000000 00 00000000000000000000000000000000 00 00000000000000000000000000000000 00000100 00000000 00000000 00070001     eth0
00000000000000000000000000000000 00 00000000000000000000000000000000 00 00000000000000000000000000000000 00000100 00000000 00000000 00070001     eth1
00000000000000000000000000000000 00 00000000000000000000000000000000 00 00000000000000000000000000000000 ffffffff 00000001 00000001 00200200       lo
*/
static netwib_err netwib_priv_conf_procnetipv6route(netwib_priv_confwork *pcw)
{
  netwib_io *piofile, *pioline;
  netwib_priv_confwork_routes *pcr;
  netwib_buf buf, bufa, bufb;
  netwib_uint32 flags;
  netwib_bool pcrset;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string("/proc/net/ipv6_route", &buf));
  ret = netwib_io_init_file_read(&buf, &piofile);
  if (ret != NETWIB_ERR_OK) {
    if (ret == NETWIB_ERR_NOTFOUND) {
      /* this this the case where kernel does not support rarp */
      ret = NETWIB_ERR_LONOTSUPPORTED;
    }
    return(ret);
  }
  netwib_er(netwib_io_init_data_line(&pioline));
  netwib_er(netwib_io_plug_read(pioline, piofile));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&bufa));
  netwib_er(netwib_buf_init_mallocdefault(&bufb));

  /* prepare for error handling */
  pcrset = NETWIB_FALSE;
  ret = NETWIB_ERR_OK;

  /* read each line */
  while (NETWIB_TRUE) {
    netwib__buf_reinit(&buf);
    netwib__buf_reinit(&bufa);
    netwib__buf_reinit(&bufb);
    ret = netwib_io_read(pioline, &buf);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_eg(netwib_priv_confwork_routes_init(&pcr));
    pcrset = NETWIB_TRUE;
    ret = netwib_buf_decode_fmt(&buf, "%{buf} %{uint32:X} %{*;buf} %{*;uint32:X} %{buf} %{uint32:X} %{*;uint32:X} %{*;uint32:X} %{uint32:X} %{buf}",
                                &bufa, &pcr->prefix, &bufb, &pcr->metric,
                                &flags, &pcr->device);
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      goto trynextentry;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    if ( ! (flags & RTF_UP) ) {
      goto trynextentry;
    }
    if ( ! (flags & RTF_GATEWAY) ) {
      goto trynextentry;
    }
    /* decode IP */
    netwib_eg(netwib_priv_conf_ip6_init_bufhexa(&bufa, &pcr->dst));
    netwib_eg(netwib_priv_conf_ip6_init_bufhexa(&bufb, &pcr->gw));
    pcr->gwset = NETWIB_TRUE;
    /* add it */
    netwib_eg(netwib_priv_confwork_routes_add(pcw, pcr));
    pcrset = NETWIB_FALSE;
   trynextentry:
    if (pcrset) {
      netwib_eg(netwib_priv_confwork_routes_close(&pcr));
      pcrset = NETWIB_FALSE;
    }
  }

  /* clean and leave */
 netwib_gotolabel:
  netwib_er(netwib_buf_close(&bufb));
  netwib_er(netwib_buf_close(&bufa));
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_io_close(&pioline));
  if (pcrset) {
    netwib_er(netwib_priv_confwork_routes_close(&pcr));
  }
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confwork_obtain_sys(netwib_priv_confwork *pcw)
{
  netwib_bool ip6supported, needtocompletearp;
  netwib_err ret;

  netwib_er(netwib_priv_ip_ip6_supported(&ip6supported));

  netwib_er(netwib_priv_conf_devices_ioctl(pcw));
  netwib_er(netwib_priv_confwork_debug(pcw, "After devices_ioctl"));
  if (ip6supported) {
    /* this function only complete (does not return IPv4 info) */
    ret = netwib_priv_conf_procnetifinet6(pcw);
    if (ret != NETWIB_ERR_OK) {
      if (ret != NETWIB_ERR_LONOTSUPPORTED) {
        return(ret);
      }
    }
    netwib_er(netwib_priv_confwork_debug(pcw, "After procnetifinet6"));
  }

  needtocompletearp = NETWIB_TRUE;
  if (ip6supported) {
    ret = netwib_priv_conf_binip(pcw);
    if (ret == NETWIB_ERR_OK) {
      needtocompletearp = NETWIB_FALSE;
    } else if (ret != NETWIB_ERR_LONOTSUPPORTED) {
      return(ret);
    }
  }
  if (needtocompletearp) {
    netwib_er(netwib_priv_conf_procnetarp(pcw));
    ret = netwib_priv_conf_procnetrarp(pcw);
    if (ret != NETWIB_ERR_OK) {
      if (ret != NETWIB_ERR_LONOTSUPPORTED) {
        return(ret);
      }
    }
  }
  netwib_er(netwib_priv_confwork_debug(pcw, "After procnetarp"));

  netwib_er(netwib_priv_conf_procnetroute(pcw));
  netwib_er(netwib_priv_confwork_debug(pcw, "After procnetroute"));
  if (ip6supported) {
    /* this function only complete (does not return IPv4 info) */
    ret = netwib_priv_conf_procnetipv6route(pcw);
    if (ret != NETWIB_ERR_OK) {
      if (ret != NETWIB_ERR_LONOTSUPPORTED) {
        return(ret);
      }
    }
    netwib_er(netwib_priv_confwork_debug(pcw, "After procnetipv6route"));
  }


  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_obtain_arpcache(netwib_priv_confwork *pcw)
{
  netwib_bool ip6supported, needtocompletearp;
  netwib_err ret;

  netwib_er(netwib_priv_ip_ip6_supported(&ip6supported));

  needtocompletearp = NETWIB_TRUE;
  if (ip6supported) {
    ret = netwib_priv_conf_binip(pcw);
    if (ret == NETWIB_ERR_OK) {
      needtocompletearp = NETWIB_FALSE;
    } else if (ret != NETWIB_ERR_LONOTSUPPORTED) {
      return(ret);
    }
  }
  if (needtocompletearp) {
    netwib_er(netwib_priv_conf_procnetarp(pcw));
    ret = netwib_priv_conf_procnetrarp(pcw);
    if (ret != NETWIB_ERR_OK) {
      if (ret != NETWIB_ERR_LONOTSUPPORTED) {
        return(ret);
      }
    }
  }

  return(NETWIB_ERR_OK);
}
