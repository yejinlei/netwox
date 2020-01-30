
/*-------------------------------------------------------------*/
typedef struct {
  netwox_httphdr_method method;
  netwib_buf uri;
  netwib_bool hostset;
  netwib_buf hosthost;
  netwib_port hostport;
  netwib_bool authbasicset;
  netwib_buf authbasiclogin;
  netwib_buf authbasicpassword;
  netwib_bool proxyauthbasicset;
  netwib_buf proxyauthbasiclogin;
  netwib_buf proxyauthbasicpassword;
  netwib_bool useragentset;
  netwib_buf useragent;
  netwib_bool contentlengthset;
  netwib_uint32 contentlength;
  netwib_bool saveallheaders; /* to be set by user */
  netwib_buf allheaders;
} netwox_httpserreqhdrs;

netwib_err netwox_httpserreqhdrs_init(netwox_httpserreqhdrs *phdrs);
netwib_err netwox_httpserreqhdrs_close(netwox_httpserreqhdrs *phdrs);

/*-------------------------------------------------------------*/
/* the io must read line by line */
netwib_err netwox_httpserreqhdrs_decode(netwox_httpserctx *pctx,
                                        netwib_io *pio,
                                        netwox_httpserreqhdrs *phdrs);
