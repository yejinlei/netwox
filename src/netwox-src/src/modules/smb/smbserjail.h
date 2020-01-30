
/*-------------------------------------------------------------*/
/* this function contains E because it sets psmbser->smberrtosend */
/* obtain realfilename, with correct case and 8.3 names converted */
netwib_err netwox_smbserjail_E(netwox_smbser *psmbser,
                               netwib_constbuf *prootdir,
                               netwib_pathstat_type pathtype,
                               netwib_constbuf *ppathname,
                               netwib_buf *prealpathname);
netwib_err netwox_smbserjail_rootfileid_E(netwox_smbser *psmbser,
                                          netwib_constbuf *prootdir,
                                          netwib_uint32 rootfileid,
                                          netwib_pathstat_type pathtype,
                                          netwib_constbuf *ppathname,
                                          netwib_buf *prealpathname);

/*-------------------------------------------------------------*/
netwib_err netwox_smbserjailglob_E(netwox_smbser *psmbser,
                                   netwib_constbuf *prootdir,
                                   netwox_smbcmdcmn_fileattr32 searchattributes,
                                   netwib_constbuf *psearchpattern,
                                   netwox_smbserglob_item *pitem);
