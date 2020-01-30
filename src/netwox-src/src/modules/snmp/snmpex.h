
/*-------------------------------------------------------------*/
netwib_err netwox_snmpex_write(netwib_io *psockio,
                               netwox_constsnmpmsg *psnmpmsg,
                               netwib_bool display);

/*-------------------------------------------------------------*/
netwib_err netwox_snmpex_read(netwib_io *psockio,
                              netwib_time *pabstime,
                              netwox_snmpmsg *psnmpmsg,
                              netwib_bool display);

/*-------------------------------------------------------------*/
typedef netwib_err (*netwox_snmpex_req_pf)(netwox_snmparg *psnmparg,
                                           netwox_snmpmsg *psnmpmsg,
                                           netwox_snmp3usmsp *psnmp3usmsp,
                                           netwox_snmppdu *psnmppdu,
                                           netwib_ptr infos);
typedef netwib_err (*netwox_snmpex_rep_pf)(netwox_snmparg *psnmparg,
                                           netwox_snmpmsg *psnmpmsg,
                                           netwox_snmp3usmsp *psnmp3usmsp,
                                           netwox_snmppdu *psnmppdu,
                                           netwib_ptr infos,
                                           netwib_bool *pstoploop);

/*-------------------------------------------------------------*/
netwib_err netwox_snmpex_session(netwox_snmparg *psnmparg,
                                 netwib_io *psockio,
                                 netwox_snmpex_req_pf pfuncreq,
                                 netwox_snmpex_rep_pf pfuncrep,
                                 netwib_ptr infos);
