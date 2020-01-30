/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
#include "unix/devices_ioctl.c"

/*-------------------------------------------------------------*/
/*
Solaris 8:
netstat -pn

Net to Media Table: IPv4
Device   IP Address               Mask      Flags   Phys Addr
------ -------------------- --------------- ----- ---------------
elx1   224.0.0.1            255.255.255.255       01:00:5e:00:00:01
elx0   224.0.0.1            255.255.255.255       01:00:5e:00:00:01
elx1   192.168.1.3          255.255.255.255 SP    00:20:af:2a:aa:31
elx1   192.168.1.1          255.255.255.255 U
elx0   192.168.2.3          255.255.255.255 SP    00:20:af:2a:aa:b5
elx1   224.0.0.0            240.0.0.0       SM    01:00:5e:00:00:00
elx0   224.0.0.0            240.0.0.0       SM    01:00:5e:00:00:00

Net to Media Table: IPv6
 If   Physical Address    Type      State      Destination/Mask
----- -----------------  ------- ------------ ---------------------------
elx0  33:33:00:00:00:00  other   REACHABLE    ff00::
elx0  33:33:ff:00:00:00  other   REACHABLE    ff02::1:ff00:0
elx0  33:33:ff:2a:aa:aa  other   REACHABLE    ff02::1:ffaa:aa31
elx0  00:20:af:2a:aa:aa  local   REACHABLE    fe80::220:afff:fe2a:aab5
*/
static netwib_err netwib_priv_conf_binnetstatpn(netwib_priv_confwork *pcw)
{
  netwib_io *piofile, *pioline;
  netwib_priv_confwork_arpcache *pca;
  netwib_buf buf;
  netwib_bool pcaset;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string("/bin/netstat -pn", &buf));
  ret = netwib_io_init_exec(&buf, NETWIB_IO_WAYTYPE_READ, NETWIB_FALSE,
                            NULL, NULL, &piofile);
  if (ret != NETWIB_ERR_OK) {
    if (ret == NETWIB_ERR_NOTFOUND) {
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
    ret = netwib_buf_decode_fmt(&buf, "%{buf} %{ip} %{*;ip} %{*;buf:glob} %{eth}", &pca->device, &pca->ip, &pca->eth);
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      netwib__buf_reinit(&pca->device);
      ret = netwib_buf_decode_fmt(&buf, "%{buf} %{eth} %{*;buf} %{*;buf} %{ip}", &pca->device, &pca->eth, &pca->ip);
    }
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      /* ignore unrecognized lines */
      netwib_eg(netwib_priv_confwork_arpcache_close(&pca));
      pcaset = NETWIB_FALSE;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* ignore 224.x and eth=33:33:x */
    if ((pca->ip.iptype == NETWIB_IPTYPE_IP4 &&
         (pca->ip.ipvalue.ip4&0xFF000000u) == 0xE0000000u) ||
        (pca->ip.iptype == NETWIB_IPTYPE_IP6 &&
         pca->eth.b[0] == 0x33 && pca->eth.b[1] == 0x33)) {
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
/*
Solaris 8:
/bin/netstat -rvn

IRE Table: IPv4
  Destination             Mask           Gateway          Device Mxfrg  Rtt  Ref Flg  Out  In/Fwd
-------------------- --------------- -------------------- ------ ----- ----- --- --- ----- ------
192.168.1.0          255.255.255.0   192.168.1.3          elx1    1500*    0   1 U       83     0
192.168.2.0          255.255.255.0   192.168.2.3          elx0    1500*    0   1 U        0     0
224.0.0.0            240.0.0.0       192.168.1.3          elx1    1500*    0   1 U        0     0
1.20.3.0             255.255.255.0   192.168.1.1                  1500*    0   1 UG       0     0
default              0.0.0.0         192.168.1.254                1500*    0   1 UG       0     0
127.0.0.1            255.255.255.255 127.0.0.1            lo0     8232*    0  22 UH    4402     0

IRE Table: IPv6
  Destination/Mask            Gateway                    If    PMTU   Rtt  Ref Flags  Out   In/Fwd
--------------------------- --------------------------- ----- ------ ----- --- ----- ------ ------
fe80::/10                   fe80::220:afff:feaa:aab5    elx0   1500*     0   1 U          0      0
ff00::/8                    fe80::220:afff:feaa:aab5    elx0   1500*     0   1 U          0      0
3ffe::/16                   fec0:0:0:1::1                      1500*     0   1 UG         0      0
default                     fe80::220:afff:feaa:aab5    elx0   1500*     0   1 U          0      0
::1                         ::1                         lo0    8252*     0   1 UH         0      0
*/
static netwib_err netwib_priv_conf_binnetstatrvn(netwib_priv_confwork *pcw)
{
  netwib_io *piofile, *pioline;
  netwib_priv_confwork_routes *pcr;
  netwib_buf buf, bufdst, bufflags;
  netwib_string strflags, pc;
  netwib_bool pcrset;
  netwib_iptype iptype;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string("/bin/netstat -rvn", &buf));
  ret = netwib_io_init_exec(&buf, NETWIB_IO_WAYTYPE_READ, NETWIB_FALSE,
                            NULL, NULL, &piofile);
  if (ret != NETWIB_ERR_OK) {
    if (ret == NETWIB_ERR_NOTFOUND) {
      ret = NETWIB_ERR_LONOTSUPPORTED;
    }
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
    iptype = NETWIB_IPTYPE_IP4;
    ret = netwib_buf_decode_fmt(&buf, "%{buf} %{ip} %{ip} %{buf} %{*;uint32}* %{*;uint32} %{*;uint32} %{buf} %{*;uint32} %{*;uint32}",
                                &bufdst, &pcr->mask, &pcr->gw, &pcr->device, &bufflags);
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      netwib__buf_reinit(&bufdst);
      netwib__buf_reinit(&bufflags);
      netwib__buf_reinit(&pcr->device);
      /* try without device */
      ret = netwib_buf_decode_fmt(&buf, "%{buf} %{ip} %{ip} %{*;uint32}* %{*;uint32} %{*;uint32} %{buf} %{*;uint32} %{*;uint32}",
                                  &bufdst, &pcr->mask, &pcr->gw, &bufflags);
    }
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      /* now try both IPv6 formats, so if it works, it will be ip6 */
      iptype = NETWIB_IPTYPE_IP6;
      netwib__buf_reinit(&bufdst);
      netwib__buf_reinit(&bufflags);
      netwib__buf_reinit(&pcr->device);
      ret = netwib_buf_decode_fmt(&buf, "%{buf} %{ip} %{buf} %{*;uint32}* %{*;uint32} %{*;uint32} %{buf} %{*;uint32} %{*;uint32}",
                                  &bufdst, &pcr->gw, &pcr->device, &bufflags);
    }
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      netwib__buf_reinit(&bufdst);
      netwib__buf_reinit(&bufflags);
      netwib__buf_reinit(&pcr->device);
      ret = netwib_buf_decode_fmt(&buf, "%{buf} %{ip} %{*;uint32}* %{*;uint32} %{*;uint32} %{buf} %{*;uint32} %{*;uint32}",
                                  &bufdst, &pcr->gw, &bufflags);
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
    if (iptype == NETWIB_IPTYPE_IP4) {
      /* in this case, field does not contain mask, so don't overwrite it */
      ret = netwib_priv_ip_net_init_bufdefault(iptype, &bufdst, &pcr->dst);
    } else {
      ret = netwib_priv_ip_netmaskprefix_init_bufdefault(iptype, &bufdst,
                                                         &pcr->dst, &pcr->mask,
                                                         &pcr->prefix);
    }
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      ret = NETWIB_ERR_OK;
      goto trynextentry;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* there is no metric : every route has 1 */
    pcr->metric = 1;
    /* ignore 224.x and ff00:: */
    if ((pcr->dst.iptype == NETWIB_IPTYPE_IP4 &&
         (pcr->dst.ipvalue.ip4&0xFF000000u) == 0xE0000000u) ||
        (pcr->dst.iptype == NETWIB_IPTYPE_IP6 &&
         pcr->dst.ipvalue.ip6.b[0] == 0xFF &&
         pcr->dst.ipvalue.ip6.b[1] == 0x00 &&
         pcr->dst.ipvalue.ip6.b[2] == 0x00 /* stop here */)) {
      goto trynextentry;
    }
    /* ignore default route with fe80 source */
    if (pcr->dst.iptype == NETWIB_IPTYPE_IP6 &&
        pcr->prefix == 0 &&
        pcr->srcset && !pcr->gwset &&
        pcr->src.ipvalue.ip6.b[0] == 0xFE &&
        pcr->src.ipvalue.ip6.b[1] == 0x80 &&
        pcr->dst.ipvalue.ip6.b[2] == 0x00 /* stop here */) {
      goto trynextentry;
    }
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
  netwib_bool ip6supported;

  netwib_er(netwib_priv_ip_ip6_supported(&ip6supported));

  netwib_er(netwib_priv_conf_devices_ioctl(pcw));
  netwib_er(netwib_priv_confwork_debug(pcw, "After devices_ioctl"));

  if (ip6supported) {
    /* this function only adds IPv6 information */
    netwib_er(netwib_priv_conf_devices_ioctl6(pcw));
    netwib_er(netwib_priv_confwork_debug(pcw, "After devices_ioctl6"));
  }

  /* arp table */
  netwib_er(netwib_priv_conf_binnetstatpn(pcw));
  netwib_er(netwib_priv_confwork_debug(pcw, "After binnetstatpn"));

  /* routes */
  netwib_er(netwib_priv_conf_binnetstatrvn(pcw));
  netwib_er(netwib_priv_confwork_debug(pcw, "After binnetstatrvn"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_obtain_arpcache(netwib_priv_confwork *pcw)
{

  netwib_er(netwib_priv_conf_binnetstatpn(pcw));

  return(NETWIB_ERR_OK);
}
