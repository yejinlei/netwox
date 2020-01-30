
/*-------------------------------------------------------------*/
typedef struct {
  netwox_httphdr_statuscode statuscode;
  netwib_buf statusmessage;
  netwib_bool contentlengthset;
  netwib_uint32 contentlength;
  netwib_bool locationset;
  netwib_buf location;
  netwib_bool contenttypeset;
  netwib_buf contenttype;
  netwib_bool saveallheaders; /* to be set by user */
  netwib_buf allheaders;
} netwox_httpcliresphdrs;

netwib_err netwox_httpcliresphdrs_init(netwox_httpcliresphdrs *phdrs);
netwib_err netwox_httpcliresphdrs_close(netwox_httpcliresphdrs *phdrs);

/*-------------------------------------------------------------*/
/* the io must read line by line */
netwib_err netwox_httpcliresphdrs_decode(netwox_httpclictx *pctx,
                                         netwib_io *pio,
                                         netwox_httpcliresphdrs *phdrs);
