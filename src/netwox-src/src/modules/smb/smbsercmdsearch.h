
/*-------------------------------------------------------------*/
/* should be in smbsercmd.h, but separate search to reduce
   module size. */

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_search(netwox_smbser *psmbser,
                                   netwox_constsmbmsg *psmbmsgq,
                                   netwox_constsmbcmd *psmbcmdq,
                                   netwox_smbmsg *psmbmsgr,
                                   netwox_smbcmd *psmbcmdr,
                                   netwib_constbuf *prootdir);






