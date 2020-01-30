
/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_decode_line(netwib_constbuf *pbuf,
                                     netwib_bufext *pcmd,
                                     netwib_bufext *ppara);

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_path(netwib_bufpool *pbufpool,
                              netwib_constbuf *prootdir,
                              netwib_constbuf *pcwd,
                              netwib_constbuf *pwantedpath,
                              netwib_buf *pvirtpath,
                              netwib_buf *prealpath);

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_user(netwib_io *pctlio,
                              netwib_constbuf *pwanteduser,
                              netwib_bool passwordok,
                              netwib_constbuf *puser,
                              netwib_bool *puserok);
netwib_err netwox_ftpser_pass(netwib_io *pctlio,
                              netwib_bool userok,
                              netwib_constbuf *pwantedpassword,
                              netwib_constbuf *ppassword,
                              netwib_bool *ppasswordok);

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_quit(netwib_io *pctlio);

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_syst(netwib_io *pctlio);

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_cwd(netwib_bufpool *pbufpool,
                             netwib_constbuf *prootdir,
                             netwib_buf *pcwd,
                             netwib_io *pctlio,
                             netwib_constbuf *pwanteddir);
netwib_err netwox_ftpser_cdup(netwib_bufpool *pbufpool,
                              netwib_constbuf *prootdir,
                              netwib_buf *pcwd,
                              netwib_io *pctlio);
netwib_err netwox_ftpser_pwd(netwib_bufpool *pbufpool,
                             netwib_constbuf *prootdir,
                             netwib_buf *pcwd,
                             netwib_io *pctlio);

/*-------------------------------------------------------------*/
/* only ascii (only non-print; telnet and asa are not supported)
   and image/binary */
netwib_err netwox_ftpser_type(netwib_io *pctlio,
                              netwib_constbuf *ptype,
                              netwib_bool *pascii);

/*-------------------------------------------------------------*/
/* only File is supported, even if it's against RFC 959 (5.1) */
netwib_err netwox_ftpser_stru(netwib_io *pctlio,
                              netwib_constbuf *pstru);

/*-------------------------------------------------------------*/
/* only stream is supported */
netwib_err netwox_ftpser_mode(netwib_io *pctlio,
                              netwib_constbuf *pmode);

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_rnfr(netwib_bufpool *pbufpool,
                              netwib_constbuf *prootdir,
                              netwib_buf *pcwd,
                              netwib_buf *prnfr,
                              netwib_io *pctlio,
                              netwib_constbuf *ppara);
netwib_err netwox_ftpser_rnto(netwib_bufpool *pbufpool,
                              netwib_constbuf *prootdir,
                              netwib_buf *pcwd,
                              netwib_buf *prnfr,
                              netwib_io *pctlio,
                              netwib_constbuf *prnto);

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_dele(netwib_bufpool *pbufpool,
                              netwib_constbuf *prootdir,
                              netwib_buf *pcwd,
                              netwib_io *pctlio,
                              netwib_constbuf *pfile);

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_size(netwib_bufpool *pbufpool,
                              netwib_constbuf *prootdir,
                              netwib_buf *pcwd,
                              netwib_io *pctlio,
                              netwib_constbuf *pfile);

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_rmd(netwib_bufpool *pbufpool,
                             netwib_constbuf *prootdir,
                             netwib_buf *pcwd,
                             netwib_io *pctlio,
                             netwib_constbuf *pdir);
netwib_err netwox_ftpser_mkd(netwib_bufpool *pbufpool,
                             netwib_constbuf *prootdir,
                             netwib_buf *pcwd,
                             netwib_io *pctlio,
                             netwib_constbuf *pdir);

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_noop(netwib_io *pctlio);

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_port(netwox_sockinfo *pmainsockinfo,
                              netwib_ips *pallowedclients,
                              netwib_constbuf *phostport,
                              netwib_io *pctlio,
                              netwib_io **ppdataio);
netwib_err netwox_ftpser_eprt(netwox_sockinfo *pmainsockinfo,
                              netwib_ips *pallowedclients,
                              netwib_constbuf *phostport,
                              netwib_io *pctlio,
                              netwib_io **ppdataio);

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_pasv(netwib_bufpool *pbufpool,
                              netwox_sockinfo *pmainsockinfo,
                              netwib_io *pctlio,
                              netwib_io **ppdataio);
netwib_err netwox_ftpser_epsv(netwib_bufpool *pbufpool,
                              netwox_sockinfo *pmainsockinfo,
                              netwib_io *pctlio,
                              netwib_io **ppdataio);

/*-------------------------------------------------------------*/
netwib_err netwox_ftpser_list(netwib_bufpool *pbufpool,
                              netwib_constbuf *prootdir,
                              netwib_buf *pcwd,
                              netwib_io *pctlio,
                              netwib_io **ppdataio,
                              netwib_constbuf *pfile,
                              netwib_bool longdisplay);

/*-------------------------------------------------------------*/
/* does not support ABORT */
netwib_err netwox_ftpser_retr(netwib_bufpool *pbufpool,
                              netwib_constbuf *prootdir,
                              netwib_buf *pcwd,
                              netwib_io *pctlio,
                              netwib_io **ppdataio,
                              netwib_constbuf *pfile,
                              netwib_bool ascii);
netwib_err netwox_ftpser_stor(netwib_bufpool *pbufpool,
                              netwib_constbuf *prootdir,
                              netwib_buf *pcwd,
                              netwib_io *pctlio,
                              netwib_io **ppdataio,
                              netwib_constbuf *pfile,
                              netwib_bool ascii);
