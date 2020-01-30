
/*-------------------------------------------------------------*/
/* should be in smbser.h, but separate transactions to reduce
   module size. */

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_transaction(netwox_smbser *psmbser,
                                        netwox_constsmbmsg *psmbmsgq,
                                        netwox_constsmbcmd *psmbcmdq,
                                        netwib_constbuf *pshare,
                                        netwib_bool *psendreply);






