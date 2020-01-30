
/*-------------------------------------------------------------*/
netwib_err netwib_priv_cs_init(void);
netwib_err netwib_priv_cs_close(void);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_cs_PacketOpenAdapter(LPTSTR AdapterName,
                                            LPADAPTER *plp);
netwib_err netwib_priv_cs_PacketCloseAdapter(LPADAPTER lpAdapter);

netwib_err netwib_priv_cs_pcap_open_live(char *dev,
                                         int mtu,
                                         int promisc,
                                         int toms,
                                         char *err,
                                         pcap_t* *ppc);
netwib_err netwib_priv_cs_pcap_close(pcap_t *pt);
