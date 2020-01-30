
/*-------------------------------------------------------------*/
/* this structure can be read by two threads */
typedef struct {
  netwox_dnscache dnscache;
  netwib_bufpool *pbufpool;
  netwib_uint32 timeoutms;
  netwib_bool useproxy;
  netwib_ip proxy_ip;
  netwib_port proxy_port;
  netwib_buf proxy_login;
  netwib_buf proxy_password;
  netwib_buf useragent;
  netwib_bool ftp_passive; /* for FTP only */
} netwox_httpclictx;

netwib_err netwox_httpclictx_init(netwox_httpclictx *pctx);
netwib_err netwox_httpclictx_close(netwox_httpclictx *pctx);
