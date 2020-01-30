/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_libpcap_notcompiled(void);
netwib_err netwib_priv_libpcap_notcompiled(void)
{
#if defined NETWIBDEF_SYSNAME_Unix
  netwib_er(netwib_priv_errmsg_string("netwib wasn't compiled with libpcap support"));
#elif defined NETWIBDEF_SYSNAME_Windows
  netwib_er(netwib_priv_errmsg_string("netwib wasn't compiled with WinPCAP support"));
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif
  return(NETWIB_ERR_OK);
}
netwib_err netwib_priv_winpcap_notavail(void);
netwib_err netwib_priv_winpcap_notavail(void)
{
  netwib_er(netwib_priv_errmsg_string("WinPCAP >= 3.0 is not found"));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_libpcap_init_sniff(netwib_constbuf *pdevice,
                                          netwib_priv_libpcap *plib)
#if NETWIBDEF_LIBPCAPINSTALLED == 1
{
  netwib_buf realdevice;
  netwib_char errmsg[PCAP_ERRBUF_SIZE];
  netwib_string device;
  netwib_uint32 mtu, snifflen;
  netwib_ip4 localnet;
  netwib_device_hwtype hwtype;
  netwib_err ret;
  int reti;

#if defined NETWIBDEF_SYSNAME_Windows
  if (!netwib_global_dll_winpcap.isloaded) {
    netwib_er(netwib_priv_winpcap_notavail());
    return(NETWIB_ERR_LONOTSUPPORTED);
  }
#endif

  plib->inittype = NETWIB_PRIV_LIBPCAP_INITTYPE_SNIFF;

  /* obtain mtu and real device name */
  ret = NETWIB_ERR_OK;
  netwib_er(netwib_buf_init_mallocdefault(&realdevice));
  netwib_eg(netwib_priv_conf_device_info(pdevice, &realdevice, &mtu,
                                         &hwtype, NULL));
  netwib_eg(netwib_buf_ref_string(&realdevice, &device));

  /* sniff length is greater than mtu */
  snifflen = mtu + NETWIB_LINUXSLLHDR_LEN/*max sizeof_linkhdr*/;

  /* open sniff */
#if defined NETWIBDEF_SYSNAME_Unix
  plib->ppcapt = pcap_open_live(device, snifflen, 1, 50, errmsg);
#elif defined NETWIBDEF_SYSNAME_Windows
  if (hwtype == NETWIB_DEVICE_HWTYPE_LOOPBACK) {
    /* cannot really test because it crashes WinPCAP */
    plib->ppcapt = NULL;
    netwib_c_strcpy(errmsg, "Loopback is not supported under Windows");
  } else {
    netwib_eg(netwib_priv_cs_pcap_open_live(device, snifflen, 1, 1, errmsg,
                                            (pcap_t**)&plib->ppcapt));
  }
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  /* deal with error */
  if (plib->ppcapt == NULL) {
    netwib_eg(netwib_priv_errmsg_string(errmsg));
    netwib_goto(NETWIB_ERR_LONOTSUPPORTED);
  }

  /* set window's waiting */
#if defined NETWIBDEF_SYSNAME_Windows
  plib->eventhandle = netwib_global_dll_winpcap.pcap_getevent(plib->ppcapt);
  if (plib->eventhandle == NULL) {
    return(NETWIB_ERR_FUPCAPGETEVENT);
  }
  /* this function is not supported under Win9x. We'll use
     lastreadreturneddata trick on those platforms. */
  /*ignore*/netwib_global_dll_winpcap.pcap_setmintocopy(plib->ppcapt, 1);
#endif

  /* set fd */
  plib->fd = netwib_pcap_fileno((pcap_t *)plib->ppcapt);

  /* get the netmask */
  reti = netwib_pcap_lookupnet(device, &localnet, &plib->netmask, errmsg);
  if (reti) {
    /* may happen with WinPcap, so use a fake */
    plib->netmask = 0xFF000000u;
  }

  /* to store filter */
  netwib_eg(netwib_buf_init_mallocdefault(&plib->filter));

  /* BSD/Win9x trick */
  plib->lastreadreturneddata = NETWIB_TRUE;
  plib->eventonceuponatime = 0;

 netwib_gotolabel:
  netwib_eg(netwib_buf_close(&realdevice));
  return(ret);
}
#else
{
  netwib_er(netwib_priv_libpcap_notcompiled());
  pdevice = pdevice; /* for compiler warning */
  plib = plib; /* for compiler warning */
  return(NETWIB_ERR_LONOTSUPPORTED);
}
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_libpcap_init_read(netwib_constbuf *pfilename,
                                         netwib_priv_libpcap *plib)
#if NETWIBDEF_LIBPCAPINSTALLED == 1
{
  netwib_char errmsg[PCAP_ERRBUF_SIZE];
  netwib_string filename;

#if defined NETWIBDEF_SYSNAME_Windows
  if (!netwib_global_dll_winpcap.isloaded) {
    netwib_er(netwib_priv_winpcap_notavail());
    return(NETWIB_ERR_LONOTSUPPORTED);
  }
#endif

  netwib__constbuf_ref_string(pfilename, filename, bufstorage,
                              netwib_priv_libpcap_init_read(&bufstorage, plib));

  plib->inittype = NETWIB_PRIV_LIBPCAP_INITTYPE_READ;

  plib->ppcapt = netwib_pcap_open_offline(filename, errmsg);
  if (plib->ppcapt == NULL) {
    netwib_er(netwib_priv_errmsg_string(errmsg));
    return(NETWIB_ERR_FUPCAPOPENOFFLINE);
  }

  return(NETWIB_ERR_OK);
}
#else
{
  netwib_er(netwib_priv_libpcap_notcompiled());
  pfilename = pfilename; /* for compiler warning */
  plib = plib; /* for compiler warning */
  return(NETWIB_ERR_LONOTSUPPORTED);
}
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_libpcap_init_write(netwib_constbuf *pfilename,
                                          netwib_priv_libpcap *plib)
#if NETWIBDEF_LIBPCAPINSTALLED == 1
{
  netwib_string filename;

#if defined NETWIBDEF_SYSNAME_Windows
  if (!netwib_global_dll_winpcap.isloaded) {
    netwib_er(netwib_priv_winpcap_notavail());
    return(NETWIB_ERR_LONOTSUPPORTED);
  }
#endif

  netwib__constbuf_ref_string(pfilename, filename, bufstorage,
                              netwib_priv_libpcap_init_write(&bufstorage, plib));

  plib->inittype = NETWIB_PRIV_LIBPCAP_INITTYPE_WRITE;

  /* initialize pcap type */
#if NETWIBDEF_HAVEFUNC_PCAP_OPENDEAD == 1
  plib->ppcapt = netwib_pcap_open_dead(DLT_EN10MB, 0xFFFF);
  if (plib->ppcapt == NULL) {
    return(NETWIB_ERR_FUPCAPOPENDEAD);
  }
#else
  /* On older versions of libpcap, we have to open a live session.
     This is a problem on computers having no network cards :(
   */
  {
    netwib_conf_devices conf;
    netwib_conf_devices_index *pconfindex;
    netwib_char errmsg[PCAP_ERRBUF_SIZE];
    netwib_string device;
    netwib_err ret;
    netwib_er(netwib_conf_devices_index_init(&conf, &pconfindex));
    ret = NETWIB_ERR_OK;
    plib->ppcapt = NULL;
    while (NETWIB_TRUE) {
      ret = netwib_conf_devices_index_next(pconfindex);
      if (ret != NETWIB_ERR_OK) {
        if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
        break;
      }
      netwib_er(netwib_buf_ref_string(&conf.device, &device));
     #if defined NETWIBDEF_SYSNAME_Unix
      plib->ppcapt = pcap_open_live(device, conf.mtu, 1, 500, errmsg);
     #elif defined NETWIBDEF_SYSNAME_Windows
      if (conf.hwtype == NETWIB_DEVICE_HWTYPE_ETHER) {
        netwib_er(netwib_priv_cs_pcap_open_live(device, conf.mtu, 1, 500, errmsg,
                                                (pcap_t**)&plib->ppcapt));
      } else {
        plib->ppcapt = NULL;
      }
     #else
      #error "Unknown value for NETWIBDEF_SYSNAME"
     #endif
      if (plib->ppcapt != NULL) {
        break;
      }
    }
    netwib_er(netwib_conf_devices_index_close(&pconfindex));
    if (ret != NETWIB_ERR_OK) {
      return(ret);
    }
    if (plib->ppcapt == NULL) {
      return(NETWIB_ERR_LONOTSUPPORTED);
    }
  }
#endif

  /* open file */
  plib->ppcapdumpert = netwib_pcap_dump_open((pcap_t*)plib->ppcapt, filename);
  if (plib->ppcapdumpert == NULL) {
    netwib_pcap_close((pcap_t*)plib->ppcapt);
    return(NETWIB_ERR_FUPCAPDUMPOPEN);
  }

  return(NETWIB_ERR_OK);
}
#else
{
  netwib_er(netwib_priv_libpcap_notcompiled());
  pfilename = pfilename; /* for compiler warning */
  plib = plib; /* for compiler warning */
  return(NETWIB_ERR_LONOTSUPPORTED);
}
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_libpcap_close(netwib_priv_libpcap *plib)
#if NETWIBDEF_LIBPCAPINSTALLED == 1
{

#if defined NETWIBDEF_SYSNAME_Unix
  pcap_close((pcap_t*)plib->ppcapt);
#elif defined NETWIBDEF_SYSNAME_Windows
  netwib_er(netwib_priv_cs_pcap_close(plib->ppcapt));
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  switch(plib->inittype) {
  case NETWIB_PRIV_LIBPCAP_INITTYPE_SNIFF :
    netwib_er(netwib_buf_close(&plib->filter));
    break;
  case NETWIB_PRIV_LIBPCAP_INITTYPE_READ :
    /* nothing more to close */
    break;
  case NETWIB_PRIV_LIBPCAP_INITTYPE_WRITE :
    netwib_pcap_dump_close((pcap_dumper_t*)plib->ppcapdumpert);
    break;
  }

  return(NETWIB_ERR_OK);
}
#else
{
  netwib_er(netwib_priv_libpcap_notcompiled());
  plib = plib; /* for compiler warning */
  return(NETWIB_ERR_LONOTSUPPORTED);
}
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_libpcap_set_filter(netwib_priv_libpcap *plib,
                                          netwib_constbuf *pfilter)
#if NETWIBDEF_LIBPCAPINSTALLED == 1
{
  netwib_string filter;
  struct bpf_program fcode;
  int reti;

  if (plib->inittype != NETWIB_PRIV_LIBPCAP_INITTYPE_SNIFF) {
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  /* obtain string value for filter */
  netwib__constbuf_ref_string(pfilter, filter, bufstorage,
                              netwib_priv_libpcap_set_filter(plib, &bufstorage));

  /* compile filter */
  reti = netwib_pcap_compile((pcap_t*)plib->ppcapt, &fcode, filter, 1,
                             plib->netmask);
  if (reti) {
    netwib_er(netwib_priv_errmsg_string(netwib_pcap_geterr((pcap_t*)plib->ppcapt)));
    return(NETWIB_ERR_FUPCAPCOMPILE);
  }

  /* apply filter */
  reti = netwib_pcap_setfilter((pcap_t*)plib->ppcapt, &fcode);
  if (reti) {
    netwib_er(netwib_priv_errmsg_string(netwib_pcap_geterr((pcap_t*)plib->ppcapt)));
    return(NETWIB_ERR_FUPCAPSETFILTER);
  }

#if NETWIBDEF_HAVEFUNC_PCAP_FREECODE == 1
  /* this function only exists in libpcap > 0.6 */
  netwib_pcap_freecode(&fcode);
#endif

  return(NETWIB_ERR_OK);
}
#else
{
  netwib_er(netwib_priv_libpcap_notcompiled());
  plib = plib; /* for compiler warning */
  pfilter = pfilter; /* for compiler warning */
  return(NETWIB_ERR_LONOTSUPPORTED);
}
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_libpcap_set_nonblock(netwib_priv_libpcap *plib)
#if NETWIBDEF_LIBPCAPINSTALLED == 1
{

  if (plib->inittype != NETWIB_PRIV_LIBPCAP_INITTYPE_SNIFF) {
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

#if defined NETWIBDEF_SYSNAME_BSD
  /* under xxxBSD, a select() on fd keeps waiting, so we have
     to set BIOCIMMEDIATE. */
  {
    int reti, one;
    one = 1;
    reti = ioctl(plib->fd, BIOCIMMEDIATE, &one);
    netwib_ir(reti, NETWIB_ERR_FUIOCTL);
  }
  /* Moreover, select does not inform twice when there are two
     packets in queue. So, we use lastreadreturneddata to force
     a read. But in this case, it must be non blocking in case
     there is really non data. This is really UGLY. Any suggestion ?
     To test, run netwox 7 and netwox 7 -p. Without the trick, the second
     one (which uses select to wait on kbd and sniff), only displays
     the last packet when new data arrives.
     This error was observed on FreeBSD 4.4 */
  netwib_er(netwib_priv_fd_block_set(plib->fd, NETWIB_FALSE));
#elif defined NETWIBDEF_SYSNAME_Solaris
 #if NETWIBDEF_HAVEINCL_SYS_BUFMOD == 1
  /* under Solaris, a select() on fd keeps waiting till the next
     packet, because it is buffered, so we have to set timeout and
     chunk size to zero */
  {
    int reti, zero;
    zero = 0;
    reti = ioctl(plib->fd, SBIOCSCHUNK, &zero);
    netwib_ir(reti, NETWIB_ERR_FUIOCTL);
    reti = ioctl(plib->fd, SBIOCSTIME, &zero);
    netwib_ir(reti, NETWIB_ERR_FUIOCTL);
  }
 #endif
#elif defined NETWIBDEF_SYSNAME_HPUX
#elif defined NETWIBDEF_SYSNAME_Tru64UNIX
#elif defined NETWIBDEF_SYSNAME_Linux
#elif defined NETWIBDEF_SYSNAME_Windows
#elif defined NETWIBDEF_SYSNAME_Unsupported
  /* YOU have to take a decision... You may need to put things from
     above zones (FreeBSD/OpenBSD/MacOS or Solaris).
     Then, you can remove NETWIB_UNSUPPORTED_TAKEDECISION.
  */
  NETWIB_UNSUPPORTED_TAKEDECISION
#else
# error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  plib = plib; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
#else
{
  netwib_er(netwib_priv_libpcap_notcompiled());
  plib = plib; /* for compiler warning */
  return(NETWIB_ERR_LONOTSUPPORTED);
}
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_libpcap_get_dlt(netwib_priv_libpcap *plib)
#if NETWIBDEF_LIBPCAPINSTALLED == 1
{
  int bpfdlt;

  bpfdlt = netwib_pcap_datalink((pcap_t*)plib->ppcapt);

  plib->dlttype = NETWIB_DEVICE_DLTTYPE_UNKNOWN;
  switch(bpfdlt) {
#if defined DLT_NULL
  case DLT_NULL:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_NULL;
    break;
#endif
#if defined DLT_EN10MB
  case DLT_EN10MB:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_EN10MB;
    break;
#endif
#if defined DLT_EN3MB
  case DLT_EN3MB:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_EN3MB;
    break;
#endif
#if defined DLT_AX25
  case DLT_AX25:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_AX25;
    break;
#endif
#if defined DLT_PRONET
  case DLT_PRONET:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_PRONET;
    break;
#endif
#if defined DLT_CHAOS
  case DLT_CHAOS:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_CHAOS;
    break;
#endif
#if defined DLT_IEEE802
  case DLT_IEEE802:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_IEEE802;
    break;
#endif
#if defined DLT_ARCNET
  case DLT_ARCNET:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_ARCNET;
    break;
#endif
#if defined DLT_SLIP
  case DLT_SLIP:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_SLIP;
    break;
#endif
#if defined DLT_PPP
  case DLT_PPP:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_PPP;
    break;
#endif
#if defined DLT_FDDI
  case DLT_FDDI:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_FDDI;
    break;
#endif
#if defined DLT_ATM_RFC1483
  case DLT_ATM_RFC1483:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_ATM_RFC1483;
    break;
#endif
#if defined DLT_RAW
  case DLT_RAW:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_RAW;
    break;
#endif
#if defined DLT_SLIP_BSDOS
  case DLT_SLIP_BSDOS:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_SLIP_BSDOS;
    break;
#endif
#if defined DLT_PPP_BSDOS
  case DLT_PPP_BSDOS:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_PPP_BSDOS;
    break;
#endif
#if defined DLT_ATM_CLIP
  case DLT_ATM_CLIP:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_ATM_CLIP;
    break;
#endif
#if defined DLT_PPP_SERIAL
  case DLT_PPP_SERIAL:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_PPP_SERIAL;
    break;
#endif
#if defined DLT_PPP_ETHER
  case DLT_PPP_ETHER:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_PPP_ETHER;
    break;
#endif
#if defined DLT_C_HDLC
  case DLT_C_HDLC:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_C_HDLC;
    break;
#endif
#if defined DLT_IEEE802_11
  case DLT_IEEE802_11:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_IEEE802_11;
    break;
#endif
#if defined DLT_LOOP
  case DLT_LOOP:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_LOOP;
    break;
#endif
#if defined DLT_LINUX_SLL
  case DLT_LINUX_SLL:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_LINUX_SLL;
    break;
#endif
#if defined DLT_LTALK
  case DLT_LTALK:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_LTALK;
    break;
#endif
#if defined DLT_ECONET
  case DLT_ECONET:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_ECONET;
    break;
#endif
#if defined DLT_PRISM_HEADER
  case DLT_PRISM_HEADER:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_PRISM_HEADER;
    break;
#endif
#if defined DLT_AIRONET_HEADER
  case DLT_AIRONET_HEADER:
    plib->dlttype = NETWIB_DEVICE_DLTTYPE_AIRONET_HEADER;
    break;
#endif
  }

  return(NETWIB_ERR_OK);
}
#else
{
  netwib_er(netwib_priv_libpcap_notcompiled());
  plib = plib; /* for compiler warning */
  return(NETWIB_ERR_LONOTSUPPORTED);
}
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_libpcap_read(netwib_priv_libpcap *plib,
                                    netwib_buf *pbuf)
#if NETWIBDEF_LIBPCAPINSTALLED == 1
{
  netwib_constdata data;
  netwib_uint32 datasize, numtries;

  if (plib->inittype == NETWIB_PRIV_LIBPCAP_INITTYPE_WRITE) {
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  /* we have to use a loop, because an empty packet returns NULL,
     like if end of file was reached. So, suppose we'll never
     have more than 5 empty packets following. This is a bad trick,
     but I don't know how to solve it. We could use pcap_stat,
     but I'm sure this brings incompatibilities problems. */
  numtries = 5;
  do {
#if NETWIBDEF_LIBPCAPFROMREDHAT == 1
    {
      struct pcap_pkthdr hdr[3];
      /* In this function, we use an array for hdr, even if it
         should not be necessary. Indeed, there is an overflow
         in libpcap package modified by RedHat, so we have
         to use this trick ... (note : there is no problem
         with tcpdump because they only use pcap_loop) */
      netwib_c_memset((char *)hdr, 0, 3 * sizeof(struct pcap_pkthdr));
      data = pcap_next(plib->ppcapt, &(hdr[1]));
      datasize = hdr[1].caplen;
    }
#else
    {
      struct pcap_pkthdr hdr;
      netwib_c_memset(&hdr, 0, sizeof(struct pcap_pkthdr));
      data = netwib_pcap_next((pcap_t*)plib->ppcapt, &hdr);
      datasize = hdr.caplen;
    }
#endif
  } while (plib->inittype == NETWIB_PRIV_LIBPCAP_INITTYPE_READ &&
           data == NULL && numtries--);

  if (data == NULL) {
    if (plib->inittype == NETWIB_PRIV_LIBPCAP_INITTYPE_SNIFF) {
      /* when packet does not match filter */
      plib->lastreadreturneddata = NETWIB_FALSE;
      return(NETWIB_ERR_DATANOTAVAIL);
    }
    return(NETWIB_ERR_DATAEND);
  }

  netwib_er(netwib_buf_append_data(data, datasize, pbuf));

  plib->lastreadreturneddata = NETWIB_TRUE;
  return(NETWIB_ERR_OK);
}
#else
{
  netwib_er(netwib_priv_libpcap_notcompiled());
  plib = plib; /* for compiler warning */
  pbuf = pbuf; /* for compiler warning */
  return(NETWIB_ERR_LONOTSUPPORTED);
}
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_libpcap_write(netwib_priv_libpcap *plib,
                                     netwib_constbuf *pbuf)
#if NETWIBDEF_LIBPCAPINSTALLED == 1
{
  netwib_data data;
  netwib_uint32 datasize, sec, nsec;
  struct pcap_pkthdr ph;

  if (plib->inittype != NETWIB_PRIV_LIBPCAP_INITTYPE_WRITE) {
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);
  netwib_er(netwib_priv_time_init_now(&sec, &nsec));
  ph.ts.tv_sec = sec;
  ph.ts.tv_usec = nsec / NETWIB_PRIV_NUM_1K;
  ph.caplen = datasize;
  ph.len = datasize;
  netwib_pcap_dump((unsigned char *)plib->ppcapdumpert, &ph, data);

  return(NETWIB_ERR_OK);
}
#else
{
  netwib_er(netwib_priv_libpcap_notcompiled());
  plib = plib; /* for compiler warning */
  pbuf = pbuf; /* for compiler warning */
  return(NETWIB_ERR_LONOTSUPPORTED);
}
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_libpcap_wait(netwib_priv_libpcap *plib,
                                    netwib_io_waytype way,
                                    netwib_consttime *pabstime,
                                    netwib_bool *pevent)
#if NETWIBDEF_LIBPCAPINSTALLED == 1
{

#if defined NETWIBDEF_SYSNAME_Unix
 #if defined NETWIBDEF_SYSNAME_BSD
  if (plib->lastreadreturneddata) {
    if (pevent != NULL) *pevent=NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }
  plib->eventonceuponatime++;
  if (plib->eventonceuponatime > 10) {
    if (pevent != NULL) *pevent=NETWIB_TRUE;
    plib->eventonceuponatime = 0;
    return(NETWIB_ERR_OK);
  }
 #endif
  netwib_er(netwib_priv_fd_wait(plib->fd, way, pabstime, pevent));
#elif defined NETWIBDEF_SYSNAME_Windows
  netwib_windowstype type;
  netwib_bool elapsed;
  netwib_er(netwib_windowstype_init(&type));
  if (type == NETWIB_WINDOWSTYPE_95 ||
      type == NETWIB_WINDOWSTYPE_98 ||
      type == NETWIB_WINDOWSTYPE_ME ||
      type == NETWIB_WINDOWSTYPE_NT4) {
    /* because on those systems, pcap_setmintocopy is not supported */
    /* on NT4, it should be supported, but it is not respected */
    if (pabstime != NETWIB_TIME_ZERO && pabstime != NETWIB_TIME_INFINITE) {
      netwib_er(netwib_time_iselapsed(pabstime, &elapsed));
      if (elapsed) {
        if (pevent != NULL) *pevent = NETWIB_FALSE;
        return(NETWIB_ERR_OK);
      }
    }
    if (plib->lastreadreturneddata) {
      if (pevent != NULL) *pevent = NETWIB_TRUE;
      return(NETWIB_ERR_OK);
    }
    plib->eventonceuponatime++;
    if (plib->eventonceuponatime > 10) {
      if (pevent != NULL) *pevent = NETWIB_TRUE;
      plib->eventonceuponatime = 0;
    } else {
      if (pevent != NULL) *pevent = NETWIB_FALSE;
    }
  } else {
    netwib_er(netwib_priv_handle_waitsimple(plib->eventhandle, pabstime,
                                            pevent));
  }
  return(NETWIB_ERR_OK);
  way = way;
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  return(NETWIB_ERR_OK);
}
#else
{
  netwib_er(netwib_priv_libpcap_notcompiled());
  plib = plib; /* for compiler warning */
  way = way; /* for compiler warning */
  pabstime = pabstime; /* for compiler warning */
  pevent = pevent; /* for compiler warning */
  return(NETWIB_ERR_LONOTSUPPORTED);
}
#endif


