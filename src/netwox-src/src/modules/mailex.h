
/*-------------------------------------------------------------*/
/* create an 8 bit body from a text file read line by line */
netwib_err netwox_mailex_io_part_8bit(netwib_io *piobody,
                                      netwib_buf *pemail);

/*-------------------------------------------------------------*/
/* create an base64 body from an attachment file */
netwib_err netwox_mailex_io_part_base64(netwib_io *pioatt,
                                        netwib_buf *pemail);

/*-------------------------------------------------------------*/
/* sample mail construction */
netwib_err netwox_mailex_sub(netwib_constbuf *pfrom,
                             netwib_constbuf *pfromname,
                             netwib_bool toisanewgroup,
                             netwib_constbuf *pto,
                             netwib_constbuf *psubject,
                             netwib_buf *pemail);
netwib_err netwox_mailex_subbody(netwib_constbuf *pfrom,
                                 netwib_constbuf *pfromname,
                                 netwib_bool toisanewgroup,
                                 netwib_constbuf *pto,
                                 netwib_constbuf *psubject,
                                 netwib_io *piobody,
                                 netwib_buf *pemail);
netwib_err netwox_mailex_subatt(netwib_constbuf *pfrom,
                                netwib_constbuf *pfromname,
                                netwib_bool toisanewgroup,
                                netwib_constbuf *pto,
                                netwib_constbuf *psubject,
                                netwib_constbuf *pattfilename,
                                netwib_io *pioatt,
                                netwib_buf *pemail);
netwib_err netwox_mailex_subfwd(netwib_constbuf *pfrom,
                                netwib_constbuf *pfromname,
                                netwib_bool toisanewgroup,
                                netwib_constbuf *pto,
                                netwib_constbuf *psubject,
                                netwib_constbuf *pfwdfilename,
                                netwib_io *piofwd,
                                netwib_buf *pemail);
netwib_err netwox_mailex_subbodyattfwd(netwib_constbuf *pfrom,
                                       netwib_constbuf *pfromname,
                                       netwib_bool toisanewgroup,
                                       netwib_constbuf *pto,
                                       netwib_constbuf *psubject,
                                       netwib_io *piobody,
                                       netwib_constbuf *pattfilename,
                                       netwib_io *pioatt,
                                       netwib_constbuf *pfwdfilename,
                                       netwib_io *piofwd,
                                       netwib_buf *pemail);
#define netwox_mailex_subbodyatt(pfrom,pfromname,toisanewgroup,pto,psubject,piobody,pattfilename,pioatt,pemail) netwox_mailex_subbodyattfwd(pfrom,pfromname,toisanewgroup,pto,psubject,piobody,pattfilename,pioatt,NULL,NULL,pemail)
#define netwox_mailex_subbodyfwd(pfrom,pfromname,toisanewgroup,pto,psubject,piobody,pfwdfilename,piofwd,pemail) netwox_mailex_subbodyattfwd(pfrom,pfromname,toisanewgroup,pto,psubject,piobody,NULL,NULL,pfwdfilename,piofwd,pemail)
