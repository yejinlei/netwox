
/*-------------------------------------------------------------*/
/* should be in smbsercmd.h, but separate transactions to reduce
   module size. */

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_nttransact(netwox_smbser *psmbser,
                                       netwox_constsmbmsg *psmbmsgq,
                                       netwox_constsmbcmd *psmbcmdq,
                                       netwib_bool *psendreply);






