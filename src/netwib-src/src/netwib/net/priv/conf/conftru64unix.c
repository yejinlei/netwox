/* This file is part of Netwib. Read and honor its license.
*/

/* IPv6 not yet supported because I never had access to a computer with IPv6 */

/*-------------------------------------------------------------*/
/*
Tru64 Unix 5.1
/usr/sbin/netstat -in
Name  Mtu   Network       Address               Ipkts Ierrs    Opkts Oerrs  Coll
alt0* 1500  <Link>        00:60:aa:aa:aa:aa      1385     0     1166     0     0
alt0* 1500  DLI           none                   1385     0     1166     0     0
ee0   1500  <Link>        00:08:aa:aa:aa:aa       443     0      385    10     0
ee0   1500  1.1.178/24    1.1.178.11              443     0      385    10     0
ee0   1500  DLI           none                    443     0      385    10     0
ee1   1500  <Link>        00:08:aa:aa:aa:aa         0     0   108433 10844     0
ee1   1500  DLI           none                      0     0   108433 10844     0
lo0   4096  <Link>        Link#5               331389     0   331389     0     0
lo0   4096  127/8         127.0.0.1            331389     0   331389     0     0
sl0*  296   <Link>        Link#4                    0     0        0     0     0
tun0* 1280  <Link>        Link#6                    0     0        0     0     0
tun1* 1280  <Link>        Link#7                    0     0        0     0     0
 */
static netwib_err netwib_priv_conf_netstatin(netwib_priv_confwork *pcw)
{
  netwib_io *piofile, *pioline;
  netwib_priv_confwork_devices *pcd;
  netwib_priv_confwork_ip *pci;
  netwib_buf buf, bufnettype, bufethip;
  netwib_data data;
  netwib_uint32 datasize;
  netwib_bool pcdset, pciset;
  netwib_cmp cmp;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string("/usr/sbin/netstat -in", &buf));
  ret = netwib_io_init_exec(&buf, NETWIB_IO_WAYTYPE_READ, NETWIB_FALSE,
                            NULL, NULL, &piofile);
  if (ret != NETWIB_ERR_OK) {
    if (ret == NETWIB_ERR_LOCANTEXEC) ret = NETWIB_ERR_LONOTSUPPORTED;
    return(ret);
  }
  netwib_er(netwib_io_init_data_line(&pioline));
  netwib_er(netwib_io_plug_read(pioline, piofile));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_init_mallocdefault(&bufnettype));
  netwib_er(netwib_buf_init_mallocdefault(&bufethip));

  /* prepare for error handling */
  pcdset = NETWIB_FALSE;
  pciset = NETWIB_FALSE;
  ret = NETWIB_ERR_OK;

  /* read each line */
  while (NETWIB_TRUE) {
    netwib__buf_reinit(&buf);
    netwib__buf_reinit(&bufnettype);
    netwib__buf_reinit(&bufethip);
    ret = netwib_io_read(pioline, &buf);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_eg(netwib_priv_confwork_devices_init(&pcd));
    pcdset = NETWIB_TRUE;
    netwib_eg(netwib_priv_confwork_ip_init(&pci));
    pciset = NETWIB_TRUE;
    ret = netwib_buf_decode_fmt(&buf,
                                "%{buf} %{uint32} %{buf} %{buf} %{*;uint32}",
                                &pcd->device, &pcd->mtu, &bufnettype,
                                &bufethip);
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      ret = NETWIB_ERR_OK;
      goto netwib_trynextentry;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* device may end with a '*' */
    data = netwib__buf_ref_data_ptr(&pcd->device);
    datasize = netwib__buf_ref_data_size(&pcd->device);
    if (datasize && data[datasize-1] == '*') {
      pcd->device.endoffset--;
    }
    /* guess hardware type:
        DLI: unknown
        <Link>: may be Ethernet
        1.2.3/24: IP
     */
    pcd->hwtype = NETWIB_DEVICE_HWTYPE_UNKNOWN;
    netwib_eg(netwib_buf_cmp_string(&bufnettype, "DLI", &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      netwib_eg(netwib_priv_confwork_devices_add(pcw, pcd));
      pcdset = NETWIB_FALSE;
      goto netwib_trynextentry;
    }
    netwib_eg(netwib_buf_cmp_string(&bufnettype, "<Link>", &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      /* bufethip may contain an Ethernet address or "Link#n" */
      ret = netwib_eth_init_buf(&bufethip, &pcd->eth);
      if (ret == NETWIB_ERR_OK) {
        pcd->hwtype = NETWIB_DEVICE_HWTYPE_ETHER;
      }
      netwib_eg(netwib_priv_confwork_devices_add(pcw, pcd));
      pcdset = NETWIB_FALSE;
      goto netwib_trynextentry;
    }
    /* suppose it's an IP configuration :
       bufnettype = 1.2.3/24
       bufethip = 1.2.3.231
     */
    ret = netwib_ip_init_buf(&bufethip, NETWIB_IP_DECODETYPE_IP, &pci->ip);
    if (ret != NETWIB_ERR_OK) {
      netwib_eg(netwib_priv_confwork_devices_add(pcw, pcd));
      pcdset = NETWIB_FALSE;
      goto netwib_trynextentry;
    }
    ret = netwib_priv_ip_netmaskprefix_init_buf(&bufnettype, NULL, &pci->mask,
                                                &pci->prefix);
    if (ret != NETWIB_ERR_OK) {
      netwib_eg(netwib_priv_confwork_devices_add(pcw, pcd));
      pcdset = NETWIB_FALSE;
      goto netwib_trynextentry;
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
  netwib_er(netwib_buf_close(&bufnettype));
  netwib_er(netwib_buf_close(&bufethip));
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
/*
Tru64 UNIX 5.1
/usr/sbin/arp -anu
1.2.3.1 at 00:aa:aa:aa:aa:aa
1.2.3.2 at 00:aa:aa:aa:8a:ca stale
*/
static netwib_err netwib_priv_conf_arpaun(netwib_priv_confwork *pcw)
{
  netwib_io *piofile, *pioline;
  netwib_priv_confwork_arpcache *pca;
  netwib_buf buf;
  netwib_bool pcaset;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string("/usr/sbin/arp -aun", &buf));
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
    ret = netwib_buf_decode_fmt(&buf, "%{ip} at %{eth}", &pca->ip, &pca->eth);
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
Tru64 UNIX 5.1
/usr/sbin/netstat -rn
Routing tables
Destination      Gateway            Flags     Refs     Use  Interface

Route Tree for Protocol Family 26:
IMPORTANT: never seen with IPv6, so don't know format

Route Tree for Protocol Family 2:
default          1.2.3.129     UGS         1   902602  ee0
1.2.3/24         1.2.3.171     U          12 39662188  ee0
1.2.3.171        1.2.3.171     UHL         1     4201  ee0
127.0.0.1        127.0.0.1     UHL         5   223150  lo0
*/
static netwib_err netwib_priv_conf_netstatrn(netwib_priv_confwork *pcw)
{
  netwib_io *piofile, *pioline;
  netwib_priv_confwork_routes *pcr;
  netwib_buf buf, bufdst, bufflags;
  netwib_string strflags, pc;
  netwib_bool pcrset;
  netwib_iptype iptype;
  netwib_cmp cmp;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string("/usr/sbin/netstat -rn", &buf));
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
  iptype = NETWIB_IPTYPE_UNKNOWN;
  while (NETWIB_TRUE) {
    netwib__buf_reinit(&buf);
    ret = netwib_io_read(pioline, &buf);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    /* analyze separators */
    netwib_er(netwib_buf_cmp_string(&buf, "Route Tree for Protocol Family 2:", &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      iptype = NETWIB_IPTYPE_IP4;
      goto trynextentry;
    }
    netwib_er(netwib_buf_cmp_string(&buf, "Route Tree for Protocol Family 26:", &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      iptype = NETWIB_IPTYPE_IP6;
      goto trynextentry;
    }
    /* decode */
    netwib__buf_reinit(&bufdst);
    netwib__buf_reinit(&bufflags);
    netwib_eg(netwib_priv_confwork_routes_init(&pcr));
    pcrset = NETWIB_TRUE;
    ret = netwib_buf_decode_fmt(&buf, "%{buf} %{ip} %{buf} %{*;uint32} %{*;uint32} %{buf}%$", &bufdst, &pcr->gw, &bufflags, &pcr->device);
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
    ret = netwib_priv_ip_netmaskprefix_init_bufdefault(iptype, &bufdst,
                                                       &pcr->dst, &pcr->mask,
                                                       &pcr->prefix);
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

  ret = netwib_priv_conf_arpaun(pcw);
  if (ret != NETWIB_ERR_OK) {
    if (ret != NETWIB_ERR_LONOTSUPPORTED) {
      return(ret);
    }
  }
  netwib_er(netwib_priv_confwork_debug(pcw, "After arpaun"));

  ret = netwib_priv_conf_netstatrn(pcw);
  if (ret != NETWIB_ERR_OK) {
    if (ret != NETWIB_ERR_LONOTSUPPORTED) {
      return(ret);
    }
  }
  netwib_er(netwib_priv_confwork_debug(pcw, "After netstatrn"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_obtain_arpcache(netwib_priv_confwork *pcw)
{
  netwib_err ret;

  ret = netwib_priv_conf_arpaun(pcw);
  if (ret != NETWIB_ERR_OK) {
    if (ret != NETWIB_ERR_LONOTSUPPORTED) {
      return(ret);
    }
  }

  return(NETWIB_ERR_OK);
}
