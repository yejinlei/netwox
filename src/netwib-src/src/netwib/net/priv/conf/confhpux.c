/* This file is part of Netwib. Read and honor its license.
*/

/* IPv6 not yet supported because I never had access to a computer with IPv6 */

/*-------------------------------------------------------------*/
/*
HP-UX B.11.23, B.11.11
/usr/bin/netstat -in
Name      Mtu  Network         Address         Ipkts   Ierrs Opkts   Oerrs Coll
lan0      1500 5.1.1.0         5.1.1.176       430317  0     5109438 0     0
lo0       4136 127.0.0.0       127.0.0.1       1107962 0     1107963 0     0
*/
static netwib_err netwib_priv_conf_netstatin(netwib_priv_confwork *pcw)
{
  netwib_io *piofile, *pioline;
  netwib_priv_confwork_devices *pcd;
  netwib_priv_confwork_ip *pci;
  netwib_buf buf, bufnet;
  netwib_ip net;
  netwib_bool pcdset, pciset;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string("/usr/bin/netstat -in", &buf));
  ret = netwib_io_init_exec(&buf, NETWIB_IO_WAYTYPE_READ, NETWIB_FALSE,
                            NULL, NULL, &piofile);
  if (ret != NETWIB_ERR_OK) {
    if (ret == NETWIB_ERR_LOCANTEXEC) ret = NETWIB_ERR_LONOTSUPPORTED;
    return(ret);
  }
  netwib_er(netwib_io_init_data_line(&pioline));
  netwib_er(netwib_io_plug_read(pioline, piofile));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&bufnet));

  /* prepare for error handling */
  pcdset = NETWIB_FALSE;
  pciset = NETWIB_FALSE;
  ret = NETWIB_ERR_OK;

  /* read each line */
  while (NETWIB_TRUE) {
    netwib__buf_reinit(&buf);
    netwib__buf_reinit(&bufnet);
    ret = netwib_io_read(pioline, &buf);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_eg(netwib_priv_confwork_devices_init(&pcd));
    pcdset = NETWIB_TRUE;
    netwib_eg(netwib_priv_confwork_ip_init(&pci));
    pciset = NETWIB_TRUE;
    ret = netwib_buf_decode_fmt(&buf, "%{buf} %{uint32} %{buf} %{ip} %{*;uint32}",
                                &pcd->device, &pcd->mtu, &bufnet, &pci->ip);
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      ret = NETWIB_ERR_OK;
      goto netwib_trynextentry;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    ret = netwib_priv_ip_net_init_buf(&bufnet, &net);
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      ret = NETWIB_ERR_OK;
      goto netwib_trynextentry;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* convert network to mask (i=127.0.0.1 n=127.0.0.0 -> m=255.0.0.0) */
    ret = netwib_priv_ip_maskprefix_init_ipnet(&pci->ip, &net, &pci->mask,
                                               &pci->prefix);
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      ret = NETWIB_ERR_OK;
      goto netwib_trynextentry;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* detect loopback */
    if (pci->ip.iptype == NETWIB_IPTYPE_IP4 &&
        pci->ip.ipvalue.ip4 == 0x7F000001u) {
      pcd->hwtype = NETWIB_DEVICE_HWTYPE_LOOPBACK;
    }
    /* set 2nd device */
    netwib_eg(netwib_buf_append_buf(&pcd->device, &pci->device));
    /* add it */
    netwib_eg(netwib_priv_confwork_devices_add(pcw, pcd));
    pcdset = NETWIB_FALSE;
    netwib_eg(netwib_priv_confwork_ip_add(pcw, pci));
    pciset = NETWIB_FALSE;
    /* next */
  netwib_trynextentry:
    if (pcdset) {
      netwib_er(netwib_priv_confwork_devices_close(&pcd));
      pcdset = NETWIB_FALSE;
    }
    if (pciset) {
      netwib_er(netwib_priv_confwork_ip_close(&pci));
      pciset = NETWIB_FALSE;
    }
  }

  /* clean and leave */
 netwib_gotolabel:
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_close(&bufnet));
  netwib_er(netwib_io_close(&pioline));
  if (pcdset) {
    netwib_er(netwib_priv_confwork_devices_close(&pcd));
  }
  if (pciset) {
    netwib_er(netwib_priv_confwork_ip_close(&pci));
  }
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_lanscanmai2(netwib_buf *pbuf,
                                               netwib_eth *peth)
{
  netwib_buf buf;
  netwib_data data;
  netwib_uint32 datasize;
  netwib_err ret;

  ret = NETWIB_ERR_OK;
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_eg(netwib_buf_decode(pbuf, NETWIB_DECODETYPE_HEXA, &buf));
  datasize = netwib__buf_ref_data_size(&buf);
  if (datasize != NETWIB_ETH_LEN) {
    netwib_goto(NETWIB_ERR_NOTCONVERTED);
  }
  data = netwib__buf_ref_data_ptr(&buf);
  netwib_c_memcpy(peth->b, data, NETWIB_ETH_LEN);

  /* clean and leave */
 netwib_gotolabel:
  netwib_er(netwib_buf_close(&buf));
  return(ret);
}

/*-------------------------------------------------------------*/
/*
HP-UX B.11.23, B.11.11
/usr/sbin/lanscan -mai
0x0011855FAAA2 lan0 snap0     ETHER
0x0011855FAAA3 lan1 snap1     ETHER
*/
static netwib_err netwib_priv_conf_lanscanmai(netwib_priv_confwork *pcw)
{
  netwib_io *piofile, *pioline;
  netwib_priv_confwork_devices *pcd;
  netwib_buf buf, buf2;
  netwib_bool pcdset, setatleastone;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string("/usr/sbin/lanscan -mai", &buf));
  ret = netwib_io_init_exec(&buf, NETWIB_IO_WAYTYPE_READ, NETWIB_FALSE,
                            NULL, NULL, &piofile);
  if (ret != NETWIB_ERR_OK) {
    if (ret == NETWIB_ERR_LOCANTEXEC) ret = NETWIB_ERR_LONOTSUPPORTED;
    return(ret);
  }
  netwib_er(netwib_io_init_data_line(&pioline));
  netwib_er(netwib_io_plug_read(pioline, piofile));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));

  /* prepare for error handling */
  pcdset = NETWIB_FALSE;
  ret = NETWIB_ERR_OK;
  setatleastone = NETWIB_FALSE;

  /* read each line */
  while (NETWIB_TRUE) {
    netwib__buf_reinit(&buf);
    netwib__buf_reinit(&buf2);
    ret = netwib_io_read(pioline, &buf);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_eg(netwib_priv_confwork_devices_init(&pcd));
    pcdset = NETWIB_TRUE;
    ret = netwib_buf_decode_fmt(&buf, "0x%{buf} %{buf} %{*;buf} ETHER",
                                &buf2, &pcd->device);
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      ret = NETWIB_ERR_OK;
      goto netwib_trynextentry;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    pcd->hwtype = NETWIB_DEVICE_HWTYPE_ETHER;
    ret = netwib_priv_conf_lanscanmai2(&buf2, &pcd->eth);
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      ret = NETWIB_ERR_OK;
      goto netwib_trynextentry;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* add it */
    netwib_eg(netwib_priv_confwork_devices_add(pcw, pcd));
    pcdset = NETWIB_FALSE;
    setatleastone = NETWIB_TRUE;
    /* next */
  netwib_trynextentry:
    if (pcdset) {
      netwib_er(netwib_priv_confwork_devices_close(&pcd));
      pcdset = NETWIB_FALSE;
    }
  }

  if (ret == NETWIB_ERR_OK && !setatleastone) {
    /* to force use of 2nd method */
    ret = NETWIB_ERR_LONOTSUPPORTED;
  }

  /* clean and leave */
 netwib_gotolabel:
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_close(&buf2));
  netwib_er(netwib_io_close(&pioline));
  if (pcdset) {
    netwib_er(netwib_priv_confwork_devices_close(&pcd));
  }
  return(ret);
}

/*-------------------------------------------------------------*/
/*
HP-UX B.11.23, B.11.11
/usr/sbin/lanscan -ai
0x0011855FAAA2 lan0 snap0
0x0011855FAAA3 lan1 snap0
*/
static netwib_err netwib_priv_conf_lanscanai(netwib_priv_confwork *pcw)
{
  netwib_io *piofile, *pioline;
  netwib_priv_confwork_devices *pcd;
  netwib_buf buf, buf2;
  netwib_bool pcdset;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string("/usr/sbin/lanscan -ai", &buf));
  ret = netwib_io_init_exec(&buf, NETWIB_IO_WAYTYPE_READ, NETWIB_FALSE,
                            NULL, NULL, &piofile);
  if (ret != NETWIB_ERR_OK) {
    if (ret == NETWIB_ERR_LOCANTEXEC) ret = NETWIB_ERR_LONOTSUPPORTED;
    return(ret);
  }
  netwib_er(netwib_io_init_data_line(&pioline));
  netwib_er(netwib_io_plug_read(pioline, piofile));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));

  /* prepare for error handling */
  pcdset = NETWIB_FALSE;
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
    netwib_eg(netwib_priv_confwork_devices_init(&pcd));
    pcdset = NETWIB_TRUE;
    ret = netwib_buf_decode_fmt(&buf, "0x%{buf} %{buf}",
                               &buf2, &pcd->device);
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      ret = NETWIB_ERR_OK;
      goto netwib_trynextentry;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    pcd->hwtype = NETWIB_DEVICE_HWTYPE_ETHER;
    ret = netwib_priv_conf_lanscanmai2(&buf2, &pcd->eth);
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      ret = NETWIB_ERR_OK;
      goto netwib_trynextentry;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* add it */
    netwib_eg(netwib_priv_confwork_devices_add(pcw, pcd));
    pcdset = NETWIB_FALSE;
    /* next */
  netwib_trynextentry:
    if (pcdset) {
      netwib_er(netwib_priv_confwork_devices_close(&pcd));
      pcdset = NETWIB_FALSE;
    }
  }

  /* clean and leave */
 netwib_gotolabel:
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_buf_close(&buf2));
  netwib_er(netwib_io_close(&pioline));
  if (pcdset) {
    netwib_er(netwib_priv_confwork_devices_close(&pcd));
  }
  return(ret);
}

/*-------------------------------------------------------------*/
/*
HP-UX B.11.23, B.11.11
/usr/sbin/arp -an
 (1.1.1.129) at 0:d:88:11:aa:aa ether
 (1.1.1.253) at 0:60:6d:21:aa:aa ether
*/
static netwib_err netwib_priv_conf_arpan(netwib_priv_confwork *pcw)
{
  netwib_io *piofile, *pioline;
  netwib_priv_confwork_arpcache *pca;
  netwib_buf buf;
  netwib_bool pcaset;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string("/usr/sbin/arp -an", &buf));
  ret = netwib_io_init_exec(&buf, NETWIB_IO_WAYTYPE_READ, NETWIB_FALSE,
                            NULL, NULL, &piofile);
  if (ret != NETWIB_ERR_OK) {
    if (ret == NETWIB_ERR_LOCANTEXEC) ret = NETWIB_ERR_LONOTSUPPORTED;
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
    ret = netwib_buf_decode_fmt(&buf, " (%{ip}) at %{eth} ether",
                                &pca->ip, &pca->eth);
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      ret = NETWIB_ERR_OK;
      goto netwib_trynextentry;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* add it */
    netwib_eg(netwib_priv_confwork_arpcache_add(pcw, pca));
    pcaset = NETWIB_FALSE;
    /* next */
  netwib_trynextentry:
    if (pcaset) {
      netwib_er(netwib_priv_confwork_arpcache_close(&pca));
      pcaset = NETWIB_FALSE;
    }
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
HP-UX B.11.23, B.11.11
/usr/bin/netstat -rvn
Routing tables
Dest/Netmask                    Gateway        Flags   Refs Interface  Pmtu
127.0.0.1/255.255.255.255       127.0.0.1      UH        0  lo0        4136
15.1.1.192/255.255.255.255      15.1.1.192     UH        0  lan0       4136
15.1.1.0/255.255.255.0          15.1.1.192     U         2  lan0       1500
127.0.0.0/255.0.0.0             127.0.0.1      U         0  lo0           0
default/0.0.0.0                 15.1.1.129     UG        0  lan0          0
*/
static netwib_err netwib_priv_conf_netstatrvn(netwib_priv_confwork *pcw)
{
  netwib_io *piofile, *pioline;
  netwib_priv_confwork_routes *pcr;
  netwib_buf buf, bufdst, bufflags;
  netwib_string strflags, pc;
  netwib_bool pcrset, setatleastone;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string("/usr/bin/netstat -rvn", &buf));
  ret = netwib_io_init_exec(&buf, NETWIB_IO_WAYTYPE_READ, NETWIB_FALSE,
                            NULL, NULL, &piofile);
  if (ret != NETWIB_ERR_OK) {
    if (ret == NETWIB_ERR_LOCANTEXEC) ret = NETWIB_ERR_LONOTSUPPORTED;
    return(ret);
  }
  netwib_er(netwib_io_init_data_line(&pioline));
  netwib_er(netwib_io_plug_read(pioline, piofile));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&bufdst));
  netwib_er(netwib_buf_init_mallocdefault(&bufflags));

  /* prepare for error handling */
  pcrset = NETWIB_FALSE;
  ret = NETWIB_ERR_OK;
  setatleastone = NETWIB_FALSE;

  /* read each line */
  while (NETWIB_TRUE) {
    netwib__buf_reinit(&buf);
    ret = netwib_io_read(pioline, &buf);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib__buf_reinit(&bufdst);
    netwib__buf_reinit(&bufflags);
    netwib_eg(netwib_priv_confwork_routes_init(&pcr));
    pcrset = NETWIB_TRUE;
    ret = netwib_buf_decode_fmt(&buf, "%{buf}/%{ip} %{ip} %{buf} %{*;uint32} %{buf} %{*;uint32}", &bufdst, &pcr->mask, &pcr->gw, &bufflags, &pcr->device);
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      ret = NETWIB_ERR_OK;
      goto trynextentry;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* check if flags contains 'G' for Gateway */
    netwib_eg(netwib_buf_ref_string(&bufflags, &strflags));
    pc = netwib_c_strchr(strflags, 'G');
    if (pc != NULL) {
      pcr->gwset = NETWIB_TRUE;
    } else {
      pcr->src = pcr->gw;
      pcr->srcset = NETWIB_TRUE;
    }
    /* convert bufdst */
    ret = netwib_priv_ip_net_init_bufdefault(NETWIB_IPTYPE_IP4, &bufdst,
                                             &pcr->dst);
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      ret = NETWIB_ERR_OK;
      goto trynextentry;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* there is no metric : every route has 1 */
    pcr->metric = 1;
    /* add it */
    netwib_eg(netwib_priv_confwork_routes_add(pcw, pcr));
    pcrset = NETWIB_FALSE;
    setatleastone = NETWIB_TRUE;
  trynextentry:
    if (pcrset) {
      netwib_eg(netwib_priv_confwork_routes_close(&pcr));
      pcrset = NETWIB_FALSE;
    }
  }

  if (ret == NETWIB_ERR_OK && !setatleastone) {
    /* to force use of 2nd method */
    ret = NETWIB_ERR_LONOTSUPPORTED;
  }

  /* clean and leave */
 netwib_gotolabel:
  netwib_er(netwib_buf_close(&bufflags));
  netwib_er(netwib_buf_close(&bufdst));
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_io_close(&pioline));
  if (pcrset) {
    netwib_er(netwib_priv_confwork_routes_close(&pcr));
  }
  return(ret);
}

/*-------------------------------------------------------------*/
/*
Note: It's a guess. I don't have access anymore to this system :(
      netwib_priv_conf_netstatrvn() and netstatrvnskip() functions
      should be mixed to support both formats.
/usr/bin/netstat -rvn
Routing tables
Dest/Netmask          Gateway        Flags   Refs ?? Interface  Pmtu
127.0.0.1/255.255.255.255
                      127.0.0.1      UH        0  ?? lo0        4136
15.1.1.192/255.255.255.255
                      15.1.1.192     UH        0  ?? lan0       4136
15.1.1.0/255.255.255.0
                      15.1.1.192     U         2  ?? lan0       1500
127.0.0.0/255.0.0.0   127.0.0.1      U         0  ?? lo0           0
default/0.0.0.0       15.1.1.129     UG        0  ?? lan0          0
*/
static netwib_err netwib_priv_conf_netstatrvnskip(netwib_priv_confwork *pcw)
{
  netwib_io *piofile, *pioline;
  netwib_priv_confwork_routes *pcr;
  netwib_buf buf, bufdst, bufflags;
  netwib_string strflags, pc;
  netwib_bool pcrset;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string("/usr/bin/netstat -rvn", &buf));
  ret = netwib_io_init_exec(&buf, NETWIB_IO_WAYTYPE_READ, NETWIB_FALSE,
                            NULL, NULL, &piofile);
  if (ret != NETWIB_ERR_OK) {
    if (ret == NETWIB_ERR_LOCANTEXEC) ret = NETWIB_ERR_LONOTSUPPORTED;
    return(ret);
  }
  netwib_er(netwib_io_init_data_line(&pioline));
  netwib_er(netwib_io_plug_read(pioline, piofile));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&bufdst));
  netwib_er(netwib_buf_init_mallocdefault(&bufflags));

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
    netwib__buf_reinit(&bufdst);
    netwib__buf_reinit(&bufflags);
    netwib_eg(netwib_priv_confwork_routes_init(&pcr));
    pcrset = NETWIB_TRUE;
    ret = netwib_buf_decode_fmt(&buf, "%{buf}/%{ip} %{ip} %{buf} %{*;uint32} %{*;uint32} %{buf} %{*;uint32}", &bufdst, &pcr->mask, &pcr->gw, &bufflags, &pcr->device);
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      netwib__buf_reinit(&bufdst);
      netwib__buf_reinit(&bufflags);
      ret = netwib_buf_decode_fmt(&buf, "%{buf}/%{ip}", &bufdst, &pcr->mask);
      if (ret == NETWIB_ERR_OK) {
        /* it's on next line */
        netwib__buf_reinit(&buf);
        netwib_eg(netwib_io_read(pioline, &buf));
        ret = netwib_buf_decode_fmt(&buf, " %{ip} %{buf} %{*;uint32} %{*;uint32} %{buf} %{*;uint32}", &pcr->gw, &bufflags, &pcr->device);
      }
    }
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      ret = NETWIB_ERR_OK;
      goto trynextentry;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* check if flags contains 'G' for Gateway */
    netwib_eg(netwib_buf_ref_string(&bufflags, &strflags));
    pc = netwib_c_strchr(strflags, 'G');
    if (pc != NULL) {
      pcr->gwset = NETWIB_TRUE;
    } else {
      pcr->src = pcr->gw;
      pcr->srcset = NETWIB_TRUE;
    }
    /* convert bufdst */
    ret = netwib_priv_ip_net_init_bufdefault(NETWIB_IPTYPE_IP4, &bufdst,
                                             &pcr->dst);
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      ret = NETWIB_ERR_OK;
      goto trynextentry;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* there is no metric : every route has 1 */
    pcr->metric = 1;
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
  netwib_er(netwib_buf_close(&bufflags));
  netwib_er(netwib_buf_close(&bufdst));
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
  netwib_err ret;

  ret = netwib_priv_conf_netstatin(pcw);
  if (ret != NETWIB_ERR_OK) {
    if (ret != NETWIB_ERR_LONOTSUPPORTED) {
      return(ret);
    }
  }
  netwib_er(netwib_priv_confwork_debug(pcw, "After netstatin"));

  ret = netwib_priv_conf_lanscanmai(pcw);
  if (ret == NETWIB_ERR_LONOTSUPPORTED) {
    ret = netwib_priv_conf_lanscanai(pcw);
  }
  if (ret != NETWIB_ERR_OK) {
    if (ret != NETWIB_ERR_LONOTSUPPORTED) {
      return(ret);
    }
  }
  netwib_er(netwib_priv_confwork_debug(pcw, "After lanscanmai"));

  ret = netwib_priv_conf_arpan(pcw);
  if (ret != NETWIB_ERR_OK) {
    if (ret != NETWIB_ERR_LONOTSUPPORTED) {
      return(ret);
    }
  }
  netwib_er(netwib_priv_confwork_debug(pcw, "After arpan"));

  ret = netwib_priv_conf_netstatrvn(pcw);
  if (ret == NETWIB_ERR_LONOTSUPPORTED) {
    ret = netwib_priv_conf_netstatrvnskip(pcw);
  }
  if (ret != NETWIB_ERR_OK) {
    if (ret != NETWIB_ERR_LONOTSUPPORTED) {
      return(ret);
    }
  }
  netwib_er(netwib_priv_confwork_debug(pcw, "After netstatrvn"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_obtain_arpcache(netwib_priv_confwork *pcw)
{
  netwib_err ret;

  ret = netwib_priv_conf_arpan(pcw);
  if (ret != NETWIB_ERR_OK) {
    if (ret != NETWIB_ERR_LONOTSUPPORTED) {
      return(ret);
    }
  }

  return(NETWIB_ERR_OK);
}
