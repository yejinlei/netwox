
/*-------------------------------------------------------------*/
/* should be in smbsercmd.h, but separate transactions to reduce
   module size. */

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_transaction2(netwox_smbser *psmbser,
                                         netwox_constsmbmsg *psmbmsgq,
                                         netwox_constsmbcmd *psmbcmdq,
                                         netwib_constbuf *prootdir,
                                         netwib_bool *psendreply);






