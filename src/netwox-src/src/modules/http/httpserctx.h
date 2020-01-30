
/*-------------------------------------------------------------*/
/* this structure can be read by two threads */
typedef struct {
  /* can be changed by user */
  netwib_uint32 timeoutms;
  netwib_ips *pallowedclients;
  netwib_buf realm;
  netwib_buf login;
  netwib_buf password;
  netwib_buf rootdir;
  /* internally needed */
  netwib_bufpool *pbufpool;
} netwox_httpserctx;

netwib_err netwox_httpserctx_init(netwox_httpserctx *pctx);
netwib_err netwox_httpserctx_close(netwox_httpserctx *pctx);
