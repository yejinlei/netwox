
/*-------------------------------------------------------------*/
/*
--
  rfc 2396 describes URIdentifier :
    <scheme>:<scheme-specific-part>
  URLocation : URI with scheme http/ftp/etc.
  URName : URI with scheme oid/oasis/etc.

This file only implements URI of scheme http and ftp.

However, URI specification is a bit complicated for what we need.
So, here is what is implemented :

standard_char     = alpha, number, "-", "_", "."
URI               = ( absoluteURI | relativeURI ) [ "#" fragment ]
absoluteURI       = scheme "://" authority [ abs_path ] [ "?" query ]
  scheme          = "ftp" | "http"
                     Note : for ftp, there is no query no fragment
  authority       = [ userinfo "@" ] hostport
    userinfo      = user [ ":" password ]
      user        : write : not standard_chars encoded with %xy
                    read : everything except "/", "@", ":"
      password    : write : not standard_chars encoded with %xy
                    read : everything except "/", "@"
    hostport      = host [ ":" port ]
      host        : case insensitive
      port        = *digit
  abs_path        = "/"  path_segments
    path_segments = segment *( "/" segment )
      segment     : write : not standard_chars encoded with %xy
                    read : everything except "/", "?", "#"
  query           = variable *( "&" variable )
    variable      = varname "=" varvalue
      varname     = write : not standard_chars encoded with %xy
                    read : everything except "=", "&"
      varvalue    = write : not standard_chars encoded with %xy
                    read : everything except "&", "#"
relativeURI       = ( abs_path | rel_path ) [ "?" query ]
  rel_path        = rel_segment [ abs_path ]
    rel_segment   = segment | ("./" segment) | ("../" segment)
fragment          = write : not standard_chars encoded with %xy
                    read : everything

--
  For IPv6 addresses, host is for example "[::1]" (rfc 2732).

--
  rfc 2396 also defines "param" in a segment. This is not used for HTTP.
  This is used in last segment for FTP (rfc 1738 and 1808 : ";type=i",
  ";type=a", ";type=d"), but it is ignored in netwox.

*/

/*-------------------------------------------------------------*/
/* encode a part of an url using %xy */
typedef enum {
  NETWOX_URL_PARTTYPE_CLEAN = 1, /* encode nothing */
  NETWOX_URL_PARTTYPE_UNCLEAN, /* encode everything */
  NETWOX_URL_PARTTYPE_PATH, /* 0-9a-zA-Z_.- and +/ are not encoded */
  NETWOX_URL_PARTTYPE_QUERY, /* 0-9a-zA-Z_.- and =& are not encoded */
  NETWOX_URL_PARTTYPE_FRAGMENT, /* 0-9a-zA-Z_.- are not encoded */
  NETWOX_URL_PARTTYPE_HOST, /* 0-9a-zA-Z.-_ are not encoded */
  NETWOX_URL_PARTTYPE_USRPWD /* 0-9a-zA-Z are not encoded */
} netwox_url_parttype;
netwib_err netwox_url_part_encode(netwib_constbuf *pbuf,
                                  netwox_url_parttype parttype,
                                  netwib_buf *pencodedbuf);
netwib_err netwox_url_part_decode(netwib_constbuf *pbuf,
                                  netwox_url_parttype parttype,
                                  netwib_buf *pdecodedbuf);

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_URL_SCHEME_NONE = 1, /* no scheme at all */
  NETWOX_URL_SCHEME_UNKNOWN, /* javascript:, mailto:, telnet:, etc. */
  NETWOX_URL_SCHEME_HTTP,
  NETWOX_URL_SCHEME_HTTPS,
  NETWOX_URL_SCHEME_FTP,
  NETWOX_URL_SCHEME_ANY /* used to decode any scheme */
} netwox_url_scheme;

netwib_err netwox_url_encode_scheme(netwox_url_scheme scheme,
                                    netwib_buf *pbuf);
netwib_err netwox_url_decode_scheme(netwib_constbuf *pbuf,
                                    netwox_url_scheme *pscheme,
                                    netwib_uint32 *pskipsize);
netwib_err netwox_port_init_scheme(netwox_url_scheme scheme,
                                   netwib_port *pport);

/*-------------------------------------------------------------*/
/* http://authority/path?query#fragment (SCHEME_HTTP)
   https://authority/path?query#fragment (SCHEME_HTTPS)
     authority is not encoded/decoded
     path is encoded/decoded, but not canonized:
                             url              ppath
                 for HTTP/S, http://s/dir  -> /dir
                 for HTTP/S, http://s//dir -> //dir
                 for HTTP/S, http://s/     -> /
     query is not encoded/decoded
     fragment is encoded/decoded
   ftp://authority/path (SCHEME_FTP)
     authority is not encoded/decoded
     path is encoded/decoded, but not canonized:
                             url              ppath
                 for FTP,    ftp://s/dir   -> ./dir
                 for FTP,    ftp://s//dir  -> /dir
                 for FTP,    ftp://s/      -> ./
     query is not used
     fragment is not used
   /path?query#fragment (SCHEME_NONE)
   ../path?query#fragment (SCHEME_NONE)
   ./path?query#fragment (SCHEME_NONE)
     authority is not used
     path is encoded/decoded, but not canonized
     query is not encoded/decoded
     fragment is encoded/decoded
   SCHEME_UNKNOWN
     unsupported
*/
netwib_err netwox_url_encode(netwox_url_scheme scheme,
                             netwib_constbuf *pauthority,
                             netwib_constbuf *ppath,
                             netwib_constbuf *pquery,
                             netwib_constbuf *pfragment,
                             netwib_buf *pbuf);
netwib_err netwox_url_decode_expected(netwib_constbuf *pbuf,
                                      netwox_url_scheme expectedscheme,
                                      netwox_url_scheme *pscheme,
                                      netwib_bufext *pauthority,
                                      netwib_buf *ppath,
                                      netwib_bufext *pquery,
                                      netwib_buf *pfragment);
#define netwox_url_decode(pbuf,pscheme,pauthority,ppath,pquery,pfragment) netwox_url_decode_expected(pbuf,NETWOX_URL_SCHEME_ANY,pscheme,pauthority,ppath,pquery,pfragment)

#define netwox_url_encode_http(pauthority,ppath,pquery,pfragment,pbuf) netwox_url_encode(NETWOX_URL_SCHEME_HTTP,pauthority,ppath,pquery,pfragment,pbuf)
#define netwox_url_decode_http(pbuf,pauthority,ppath,pquery,pfragment) netwox_url_decode_expected(pbuf,NETWOX_URL_SCHEME_HTTP,NULL,pauthority,ppath,pquery,pfragment)
#define netwox_url_encode_ftp(pauthority,ppath,pbuf) netwox_url_encode(NETWOX_URL_SCHEME_FTP,pauthority,ppath,NULL,NULL,pbuf)
#define netwox_url_decode_ftp(pbuf,pauthority,ppath) netwox_url_decode_expected(pbuf,NETWOX_URL_SCHEME_FTP,NULL,pauthority,ppath,NULL,NULL)

/*-------------------------------------------------------------*/
/* path?query#fragment
   path is encoded/decoded, but not canonized
   query is not encoded/decoded
   fragment is encoded/decoded
*/
netwib_err netwox_url_pqf_encode(netwib_constbuf *ppath,
                                 netwib_constbuf *pquery,
                                 netwib_constbuf *pfragment,
                                 netwib_buf *pbuf);
netwib_err netwox_url_pqf_decode(netwib_constbuf *pbuf,
                                 netwib_buf *ppath,
                                 netwib_bufext *pquery,
                                 netwib_buf *pfragment);

/*-------------------------------------------------------------*/
/* user:password@host:port
   user, password and host are encoded/decoded
*/
netwib_err netwox_url_authority_encode(netwox_url_scheme scheme,
                                       netwib_constbuf *puser,
                                       netwib_constbuf *ppassword,
                                       netwib_constbuf *phost,
                                       netwib_port port,
                                       netwib_buf *pbuf);
netwib_err netwox_url_authority_decode(netwox_url_scheme scheme,
                                       netwib_constbuf *pbuf,
                                       netwib_buf *puser,
                                       netwib_buf *ppassword,
                                       netwib_buf *phost,
                                       netwib_port *pport);

/*-------------------------------------------------------------*/
/* name=value
   name and value are encoded/decoded
*/
netwib_err netwox_url_queryvar_encode(netwib_constbuf *pname,
                                      netwib_bool valueset,
                                      netwib_constbuf *pvalue,
                                      netwib_buf *pbuf);
netwib_err netwox_url_queryvar_decode(netwib_buf *pbuf,/*updated to point end*/
                                      netwib_buf *pname,
                                      netwib_bool *pvalueset,
                                      netwib_buf *pvalue);

/*-------------------------------------------------------------*/
/* search name in query "name=value&name2=value"
*/
netwib_err netwox_url_query_decode(netwib_constbuf *pquery,
                                   netwib_constbuf *pname,
                                   netwib_bool *pvalueset,
                                   netwib_buf *pvalue);
