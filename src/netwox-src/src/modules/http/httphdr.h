
/*-------------------------------------------------------------*/
/* rfc 2068 (however only partially implement HTTP/1.0) */

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_HTTPHDR_METHOD_GET = 1,
  NETWOX_HTTPHDR_METHOD_HEAD,
  NETWOX_HTTPHDR_METHOD_POST,
  NETWOX_HTTPHDR_METHOD_PUT,
  NETWOX_HTTPHDR_METHOD_DELETE,
  NETWOX_HTTPHDR_METHOD_TRACE,
  NETWOX_HTTPHDR_METHOD_OPTIONS
} netwox_httphdr_method;

/*-------------------------------------------------------------*/
netwib_err netwox_httphdr_methoduri(netwox_httphdr_method method,
                                    netwib_constbuf *puri,
                                    netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_httphdr_host(netwib_constbuf *phost,
                               netwib_port port,
                               netwib_buf *pbuf);
netwib_err netwox_httphdr_authbasic(netwib_constbuf *plogin,
                                    netwib_constbuf *ppassword,
                                    netwib_buf *pbuf);
netwib_err netwox_httphdr_proxyauthbasic(netwib_constbuf *plogin,
                                         netwib_constbuf *ppassword,
                                         netwib_buf *pbuf);
netwib_err netwox_httphdr_useragent(netwib_constbuf *pua,
                                    netwib_buf *pbuf);
netwib_err netwox_httphdr_endheader(netwib_buf *pbuf);


/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_HTTPHDR_STATUSCODE_OK = 200,
  NETWOX_HTTPHDR_STATUSCODE_CREATED = 201,
  NETWOX_HTTPHDR_STATUSCODE_ACCEPTED = 202,
  NETWOX_HTTPHDR_STATUSCODE_NOCONTENT = 204,
  NETWOX_HTTPHDR_STATUSCODE_MOVEDPERMANENTLY = 301,
  NETWOX_HTTPHDR_STATUSCODE_MOVEDTEMPORARILY= 302,
  NETWOX_HTTPHDR_STATUSCODE_NOTMODIFIED = 304,
  NETWOX_HTTPHDR_STATUSCODE_BADREQUEST = 400,
  NETWOX_HTTPHDR_STATUSCODE_UNAUTHORIZED = 401,
  NETWOX_HTTPHDR_STATUSCODE_FORBIDDEN = 403,
  NETWOX_HTTPHDR_STATUSCODE_NOTFOUND = 404,
  NETWOX_HTTPHDR_STATUSCODE_PROXYUNAUTHORIZED = 407,
  NETWOX_HTTPHDR_STATUSCODE_INTERNALSERVERERROR = 500,
  NETWOX_HTTPHDR_STATUSCODE_NOTIMPLEMENTED = 501,
  NETWOX_HTTPHDR_STATUSCODE_BADGATEWAY = 502,
  NETWOX_HTTPHDR_STATUSCODE_SERVICEUNAVAILABLE = 503
} netwox_httphdr_statuscode;

/*-------------------------------------------------------------*/
netwib_err netwox_httphdr_status(netwox_httphdr_statuscode statuscode,
                                 netwib_constbuf *pstatusmessage,
                                 netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_httphdr_connection(netwib_conststring type,
                                     netwib_buf *pbuf);
#define netwox_httphdr_connection_close(pbuf) netwox_httphdr_connection("close",pbuf)

/*-------------------------------------------------------------*/
netwib_err netwox_httphdr_contenttype(netwib_conststring type,
                                      netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_httphdr_contentlength(netwib_uint32 len,
                                        netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_httphdr_wwwauth(netwib_constbuf *prealm,
                                  netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_httphdr_location(netwib_constbuf *plocation,
                                   netwib_buf *pbuf);
