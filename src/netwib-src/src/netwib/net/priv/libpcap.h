
/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_PRIV_LIBPCAP_INITTYPE_SNIFF,
  NETWIB_PRIV_LIBPCAP_INITTYPE_READ,
  NETWIB_PRIV_LIBPCAP_INITTYPE_WRITE
} netwib_priv_libpcap_inittype;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_priv_libpcap_inittype inittype;
  netwib_device_dlttype dlttype;
  netwib_ptr ppcapt;       /* pcap_t* */
  netwib_ptr ppcapdumpert; /* pcap_dumper_t* */
  int fd;
  netwib_ip4 netmask;
  netwib_buf filter;
  netwib_bool lastreadreturneddata; /* trick for BSD/Win9x */
  netwib_uint32 eventonceuponatime; /* trick for BSD/Win9x */
  NETWIBHANDLE eventhandle;
} netwib_priv_libpcap;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_libpcap_init_sniff(netwib_constbuf *pdevice,
                                          netwib_priv_libpcap *plib);
netwib_err netwib_priv_libpcap_init_read(netwib_constbuf *pfilename,
                                         netwib_priv_libpcap *plib);
netwib_err netwib_priv_libpcap_init_write(netwib_constbuf *pfilename,
                                          netwib_priv_libpcap *plib);
netwib_err netwib_priv_libpcap_close(netwib_priv_libpcap *plib);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_libpcap_set_filter(netwib_priv_libpcap *plib,
                                          netwib_constbuf *pfilter);
netwib_err netwib_priv_libpcap_set_nonblock(netwib_priv_libpcap *plib);
netwib_err netwib_priv_libpcap_get_dlt(netwib_priv_libpcap *plib);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_libpcap_read(netwib_priv_libpcap *plib,
                                    netwib_buf *pbuf);
netwib_err netwib_priv_libpcap_write(netwib_priv_libpcap *plib,
                                     netwib_constbuf *pbuf);
netwib_err netwib_priv_libpcap_wait(netwib_priv_libpcap *plib,
                                    netwib_io_waytype way,
                                    netwib_consttime *pabstime,
                                    netwib_bool *pevent);

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Windows
 #define netwib_pcap_close netwib_global_dll_winpcap.pcap_close
 #define netwib_pcap_compile netwib_global_dll_winpcap.pcap_compile
 #define netwib_pcap_datalink netwib_global_dll_winpcap.pcap_datalink
 #define netwib_pcap_dump netwib_global_dll_winpcap.pcap_dump
 #define netwib_pcap_dump_close netwib_global_dll_winpcap.pcap_dump_close
 #define netwib_pcap_dump_open netwib_global_dll_winpcap.pcap_dump_open
 #define netwib_pcap_fileno netwib_global_dll_winpcap.pcap_fileno
 #define netwib_pcap_freecode netwib_global_dll_winpcap.pcap_freecode
 #define netwib_pcap_geterr netwib_global_dll_winpcap.pcap_geterr
 #define netwib_pcap_lookupnet netwib_global_dll_winpcap.pcap_lookupnet
 #define netwib_pcap_next netwib_global_dll_winpcap.pcap_next
 #define netwib_pcap_open_dead netwib_global_dll_winpcap.pcap_open_dead
 #define netwib_pcap_open_offline netwib_global_dll_winpcap.pcap_open_offline
 #define netwib_pcap_setfilter netwib_global_dll_winpcap.pcap_setfilter
#else
 #define netwib_pcap_close pcap_close
 #define netwib_pcap_compile pcap_compile
 #define netwib_pcap_datalink pcap_datalink
 #define netwib_pcap_dump pcap_dump
 #define netwib_pcap_dump_close pcap_dump_close
 #define netwib_pcap_dump_open pcap_dump_open
 #define netwib_pcap_fileno pcap_fileno
 #define netwib_pcap_freecode pcap_freecode
 #define netwib_pcap_geterr pcap_geterr
 #define netwib_pcap_lookupnet pcap_lookupnet
 #define netwib_pcap_next pcap_next
 #define netwib_pcap_open_dead pcap_open_dead
 #define netwib_pcap_open_offline pcap_open_offline
 #define netwib_pcap_setfilter pcap_setfilter
#endif
