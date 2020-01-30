
/*-------------------------------------------------------------*/
netwib_err netwox_smbsercmd_createdirectory(netwox_smbser *psmbser,
                                            netwox_constsmbmsg *psmbmsgq,
                                            netwox_constsmbcmd *psmbcmdq,
                                            netwox_smbmsg *psmbmsgr,
                                            netwox_smbcmd *psmbcmdr,
                                            netwib_constbuf *prootdir);
netwib_err netwox_smbsercmd_deletedirectory(netwox_smbser *psmbser,
                                            netwox_constsmbmsg *psmbmsgq,
                                            netwox_constsmbcmd *psmbcmdq,
                                            netwox_smbmsg *psmbmsgr,
                                            netwox_smbcmd *psmbcmdr,
                                            netwib_constbuf *prootdir);
netwib_err netwox_smbsercmd_create(netwox_smbser *psmbser,
                                   netwox_constsmbmsg *psmbmsgq,
                                   netwox_constsmbcmd *psmbcmdq,
                                   netwox_smbmsg *psmbmsgr,
                                   netwox_smbcmd *psmbcmdr,
                                   netwib_constbuf *prootdir);
netwib_err netwox_smbsercmd_open(netwox_smbser *psmbser,
                                 netwox_constsmbmsg *psmbmsgq,
                                 netwox_constsmbcmd *psmbcmdq,
                                 netwox_smbmsg *psmbmsgr,
                                 netwox_smbcmd *psmbcmdr,
                                 netwib_constbuf *prootdir);
netwib_err netwox_smbsercmd_close(netwox_smbser *psmbser,
                                  netwox_constsmbmsg *psmbmsgq,
                                  netwox_constsmbcmd *psmbcmdq,
                                  netwox_smbmsg *psmbmsgr,
                                  netwox_smbcmd *psmbcmdr);
netwib_err netwox_smbsercmd_flush(netwox_smbser *psmbser,
                                  netwox_constsmbmsg *psmbmsgq,
                                  netwox_constsmbcmd *psmbcmdq,
                                  netwox_smbmsg *psmbmsgr,
                                  netwox_smbcmd *psmbcmdr);
netwib_err netwox_smbsercmd_delete(netwox_smbser *psmbser,
                                   netwox_constsmbmsg *psmbmsgq,
                                   netwox_constsmbcmd *psmbcmdq,
                                   netwox_smbmsg *psmbmsgr,
                                   netwox_smbcmd *psmbcmdr,
                                   netwib_constbuf *prootdir);
netwib_err netwox_smbsercmd_rename(netwox_smbser *psmbser,
                                   netwox_constsmbmsg *psmbmsgq,
                                   netwox_constsmbcmd *psmbcmdq,
                                   netwox_smbmsg *psmbmsgr,
                                   netwox_smbcmd *psmbcmdr,
                                   netwib_constbuf *prootdir);
netwib_err netwox_smbsercmd_queryinformation(netwox_smbser *psmbser,
                                             netwox_constsmbmsg *psmbmsgq,
                                             netwox_constsmbcmd *psmbcmdq,
                                             netwox_smbmsg *psmbmsgr,
                                             netwox_smbcmd *psmbcmdr,
                                             netwib_constbuf *prootdir);
netwib_err netwox_smbsercmd_setinformation(netwox_smbser *psmbser,
                                           netwox_constsmbmsg *psmbmsgq,
                                           netwox_constsmbcmd *psmbcmdq,
                                           netwox_smbmsg *psmbmsgr,
                                           netwox_smbcmd *psmbcmdr,
                                           netwib_constbuf *prootdir);
netwib_err netwox_smbsercmd_read(netwox_smbser *psmbser,
                                 netwox_constsmbmsg *psmbmsgq,
                                 netwox_constsmbcmd *psmbcmdq,
                                 netwox_smbmsg *psmbmsgr,
                                 netwox_smbcmd *psmbcmdr);
netwib_err netwox_smbsercmd_write(netwox_smbser *psmbser,
                                  netwox_constsmbmsg *psmbmsgq,
                                  netwox_constsmbcmd *psmbcmdq,
                                  netwox_smbmsg *psmbmsgr,
                                  netwox_smbcmd *psmbcmdr);
netwib_err netwox_smbsercmd_createtemporary(netwox_smbser *psmbser,
                                            netwox_constsmbmsg *psmbmsgq,
                                            netwox_constsmbcmd *psmbcmdq,
                                            netwox_smbmsg *psmbmsgr,
                                            netwox_smbcmd *psmbcmdr,
                                            netwib_constbuf *prootdir);
netwib_err netwox_smbsercmd_createnew(netwox_smbser *psmbser,
                                      netwox_constsmbmsg *psmbmsgq,
                                      netwox_constsmbcmd *psmbcmdq,
                                      netwox_smbmsg *psmbmsgr,
                                      netwox_smbcmd *psmbcmdr,
                                      netwib_constbuf *prootdir);
netwib_err netwox_smbsercmd_checkdirpath(netwox_smbser *psmbser,
                                         netwox_constsmbmsg *psmbmsgq,
                                         netwox_constsmbcmd *psmbcmdq,
                                         netwox_smbmsg *psmbmsgr,
                                         netwox_smbcmd *psmbcmdr,
                                         netwib_constbuf *prootdir);
netwib_err netwox_smbsercmd_seek(netwox_smbser *psmbser,
                                 netwox_constsmbmsg *psmbmsgq,
                                 netwox_constsmbcmd *psmbcmdq,
                                 netwox_smbmsg *psmbmsgr,
                                 netwox_smbcmd *psmbcmdr);
netwib_err netwox_smbsercmd_setinformation2(netwox_smbser *psmbser,
                                            netwox_constsmbmsg *psmbmsgq,
                                            netwox_constsmbcmd *psmbcmdq,
                                            netwox_smbmsg *psmbmsgr,
                                            netwox_smbcmd *psmbcmdr);
netwib_err netwox_smbsercmd_queryinformation2(netwox_smbser *psmbser,
                                              netwox_constsmbmsg *psmbmsgq,
                                              netwox_constsmbcmd *psmbcmdq,
                                              netwox_smbmsg *psmbmsgr,
                                              netwox_smbcmd *psmbcmdr);
netwib_err netwox_smbsercmd_lockingandx(netwox_smbser *psmbser,
                                        netwox_constsmbmsg *psmbmsgq,
                                        netwox_constsmbcmd *psmbcmdq,
                                        netwox_smbmsg *psmbmsgr,
                                        netwox_smbcmd *psmbcmdr);
netwib_err netwox_smbsercmd_echo(netwox_smbser *psmbser,
                                 netwox_constsmbmsg *psmbmsgq,
                                 netwox_constsmbcmd *psmbcmdq,
                                 netwib_bool *psendreply);
netwib_err netwox_smbsercmd_openandx(netwox_smbser *psmbser,
                                     netwox_constsmbmsg *psmbmsgq,
                                     netwox_constsmbcmd *psmbcmdq,
                                     netwox_smbmsg *psmbmsgr,
                                     netwox_smbcmd *psmbcmdr,
                                     netwib_constbuf *prootdir);
netwib_err netwox_smbsercmd_readandx(netwox_smbser *psmbser,
                                     netwox_constsmbmsg *psmbmsgq,
                                     netwox_constsmbcmd *psmbcmdq,
                                     netwox_smbmsg *psmbmsgr,
                                     netwox_smbcmd *psmbcmdr);
netwib_err netwox_smbsercmd_writeandx(netwox_smbser *psmbser,
                                      netwox_constsmbmsg *psmbmsgq,
                                      netwox_constsmbcmd *psmbcmdq,
                                      netwox_smbmsg *psmbmsgr,
                                      netwox_smbcmd *psmbcmdr);
netwib_err netwox_smbsercmd_closetreedisconnect(netwox_smbser *psmbser,
                                                netwox_constsmbmsg *psmbmsgq,
                                                netwox_constsmbcmd *psmbcmdq,
                                                netwox_smbmsg *psmbmsgr,
                                                netwox_smbcmd *psmbcmdr);
netwib_err netwox_smbsercmd_findclose2(netwox_smbser *psmbser,
                                       netwox_constsmbmsg *psmbmsgq,
                                       netwox_constsmbcmd *psmbcmdq,
                                       netwox_smbmsg *psmbmsgr,
                                       netwox_smbcmd *psmbcmdr);
netwib_err netwox_smbsercmd_treedisconnect(netwox_smbser *psmbser,
                                           netwox_constsmbmsg *psmbmsgq,
                                           netwox_constsmbcmd *psmbcmdq,
                                           netwox_smbmsg *psmbmsgr,
                                           netwox_smbcmd *psmbcmdr);
netwib_err netwox_smbsercmd_negotiate(netwox_smbser *psmbser,
                                      netwox_constsmbmsg *psmbmsgq,
                                      netwox_constsmbcmd *psmbcmdq,
                                      netwox_smbmsg *psmbmsgr,
                                      netwox_smbcmd *psmbcmdr,
                                      netwib_constbuf *pdomain);
netwib_err netwox_smbsercmd_sessionsetupandx(netwox_smbser *psmbser,
                                             netwox_constsmbmsg *psmbmsgq,
                                             netwox_constsmbcmd *psmbcmdq,
                                             netwox_smbmsg *psmbmsgr,
                                             netwox_smbcmd *psmbcmdr,
                                             netwib_constbuf *pdomain,
                                             netwib_constbuf *puser,
                                             netwib_constbuf *ppassword);
netwib_err netwox_smbsercmd_logoffandx(netwox_smbser *psmbser,
                                       netwox_constsmbmsg *psmbmsgq,
                                       netwox_constsmbcmd *psmbcmdq,
                                       netwox_smbmsg *psmbmsgr,
                                       netwox_smbcmd *psmbcmdr);
netwib_err netwox_smbsercmd_treeconnectandx(netwox_smbser *psmbser,
                                            netwox_constsmbmsg *psmbmsgq,
                                            netwox_constsmbcmd *psmbcmdq,
                                            netwox_smbmsg *psmbmsgr,
                                            netwox_smbcmd *psmbcmdr,
                                            netwib_constbuf *pshare);
netwib_err netwox_smbsercmd_queryinformationdisk(netwox_smbser *psmbser,
                                                 netwox_constsmbmsg *psmbmsgq,
                                                 netwox_constsmbcmd *psmbcmdq,
                                                 netwox_smbmsg *psmbmsgr,
                                                 netwox_smbcmd *psmbcmdr);
netwib_err netwox_smbsercmd_ntcreateandx(netwox_smbser *psmbser,
                                         netwox_constsmbmsg *psmbmsgq,
                                         netwox_constsmbcmd *psmbcmdq,
                                         netwox_smbmsg *psmbmsgr,
                                         netwox_smbcmd *psmbcmdr,
                                         netwib_constbuf *prootdir);
netwib_err netwox_smbsercmd_ntcancel(netwox_smbser *psmbser,
                                     netwox_constsmbmsg *psmbmsgq,
                                     netwox_constsmbcmd *psmbcmdq,
                                     netwox_smbmsg *psmbmsgr,
                                     netwox_smbcmd *psmbcmdr);
netwib_err netwox_smbsercmd_ntrename(netwox_smbser *psmbser,
                                     netwox_constsmbmsg *psmbmsgq,
                                     netwox_constsmbcmd *psmbcmdq,
                                     netwox_smbmsg *psmbmsgr,
                                     netwox_smbcmd *psmbcmdr,
                                     netwib_constbuf *prootdir);
