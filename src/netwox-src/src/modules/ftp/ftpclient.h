
/*-------------------------------------------------------------*/
typedef struct {
  netwib_bufpool *pbufpool;
  netwox_sockinfo *psockinfo;
  netwib_io *pio;
  netwib_uint32 maxwaitms;
  netwib_uint32 errnum;
  netwib_buf errmsg;
  netwib_bool passive;
} netwox_ftpclient;

/*-------------------------------------------------------------*/
netwib_err netwox_ftpclient_init(netwox_sockinfo *psockinfo,
                                 netwib_uint32 maxwaitms,
                                 netwox_ftpclient *pftpclient);
netwib_err netwox_ftpclient_close(netwox_ftpclient *pftpclient);

/*-------------------------------------------------------------*/
netwib_err netwox_ftpclient_data(netwox_ftpclient *pftpclient,
                                 netwib_io **ppdataio);

/*-------------------------------------------------------------*/
netwib_err netwox_ftpclient_login(netwox_ftpclient *pftpclient,
                                  netwib_constbuf *puser,
                                  netwib_constbuf *ppassword);
netwib_err netwox_ftpclient_login_anonymous(netwox_ftpclient *pftpclient);

/*-------------------------------------------------------------*/
/* default mode is active */
netwib_err netwox_ftpclient_passive(netwox_ftpclient *pftpclient);
netwib_err netwox_ftpclient_active(netwox_ftpclient *pftpclient);

/*-------------------------------------------------------------*/
/* default mode is unset (server's default) */
netwib_err netwox_ftpclient_bin(netwox_ftpclient *pftpclient);
netwib_err netwox_ftpclient_ascii(netwox_ftpclient *pftpclient);

/*-------------------------------------------------------------*/
netwib_err netwox_ftpclient_file_put(netwox_ftpclient *pftpclient,
                                     netwib_constbuf *plocalfile,
                                     netwib_constbuf *premotefile);
netwib_err netwox_ftpclient_file_get(netwox_ftpclient *pftpclient,
                                     netwib_constbuf *premotefile,
                                     netwib_constbuf *plocalfile);
netwib_err netwox_ftpclient_file_get_io(netwox_ftpclient *pftpclient,
                                        netwib_constbuf *premotefile,
                                        netwib_io *plocalio);
netwib_err netwox_ftpclient_file_delete(netwox_ftpclient *pftpclient,
                                        netwib_constbuf *pfilename);
netwib_err netwox_ftpclient_file_size(netwox_ftpclient *pftpclient,
                                      netwib_constbuf *pfilename,
                                      netwib_uint32 *psize);

/*-------------------------------------------------------------*/
netwib_err netwox_ftpclient_dir_mkdir(netwox_ftpclient *pftpclient,
                                      netwib_constbuf *pdirname);
netwib_err netwox_ftpclient_dir_rmdir(netwox_ftpclient *pftpclient,
                                      netwib_constbuf *pdirname);
netwib_err netwox_ftpclient_dir_cd(netwox_ftpclient *pftpclient,
                                   netwib_constbuf *pdirname);

/*-------------------------------------------------------------*/
typedef struct {
  netwib_buf name; /* file name */
  netwib_pathstat pathstat;
  netwib_buf linkname; /* pointed link only for symlink */
} netwox_ftp_dirlist;
netwib_err netwox_ftp_dirlist_erase(netwib_ptr pitem);
netwib_err netwox_ftp_dirlist_duplicate(netwib_constptr pitem,
                                        netwib_ptr *pdupofitem);
#define netwox_ftp_dirlist_ring_init(ppring) netwib_ring_init(&netwox_ftp_dirlist_erase,&netwox_ftp_dirlist_duplicate,ppring)
#define netwox_ftp_dirlist_ring_close(ppring) netwib_ring_close(ppring,NETWIB_TRUE)
netwib_err netwox_ftpclient_dir_ls(netwox_ftpclient *pftpclient,
                                   netwib_constbuf *pdirname,
                                   netwib_ring *pring);

/*-------------------------------------------------------------*/
/* recursive functions */
netwib_err netwox_ftpclient_dirrec_put(netwox_ftpclient *pftpclient,
                                       netwib_constbuf *plocaldir,
                                       netwib_constbuf *premotedir);
netwib_err netwox_ftpclient_dirrec_get(netwox_ftpclient *pftpclient,
                                       netwib_constbuf *premotedir,
                                       netwib_constbuf *plocaldir);
netwib_err netwox_ftpclient_dirrec_rmdir(netwox_ftpclient *pftpclient,
                                         netwib_constbuf *pdirname);
