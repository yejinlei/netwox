
/*-------------------------------------------------------------*/
/* To download an url :
    - HTTP : purl(http://server/page.html) is saved in local pfilename
       For statuscode NETWOX_HTTPHDR_STATUSCODE_MOVED, Location
       header is appended to the bottom of page ("Added
       by the spider : <A HREF="LocationHdr">new location</A>.").
       In this case, pcontenttype is set if servers sets it.
    - FTP : purl(ftp://server/doc.txt) is saved in local pfilename
       Value statuscode can be :
         NETWOX_HTTPHDR_STATUSCODE_OK = 200
         NETWOX_HTTPHDR_STATUSCODE_UNAUTHORIZED = 401
         NETWOX_HTTPHDR_STATUSCODE_NOTFOUND = 404
         NETWOX_HTTPHDR_STATUSCODE_SERVICEUNAVAILABLE for other errors
       If purl is a directory :
        - and ends with '/' : a HTML index is created
        - else : create a fake redirect page
    - other protocols : a fake file is created containing error.
      Return NETWOX_HTTPHDR_STATUSCODE_SERVICEUNAVAILABLE
  Those functions always return NETWIB_ERR_OK. The true error
  has to be found in pstatuscode.
*/
netwib_err netwox_url_download_file_ct(netwox_httpclictx *pctx,
                                       netwib_constbuf *purl,
                                       netwib_constbuf *pfilename,
                                       netwox_httphdr_statuscode *pstatuscode,
                                       netwib_buf *pcontenttype);
netwib_err netwox_url_download_io_ct(netwox_httpclictx *pctx,
                                     netwib_constbuf *purl,
                                     netwib_io *pfileio,
                                     netwox_httphdr_statuscode *pstatuscode,
                                     netwib_buf *pcontenttype);
#define netwox_url_download_file(pctx,purl,pfilename,pstatuscode) netwox_url_download_file_ct(pctx,purl,pfilename,pstatuscode,NULL)
#define netwox_url_download_io(pctx,purl,pfileio,pstatuscode) netwox_url_download_io_ct(pctx,purl,pfileio,pstatuscode,NULL)

/*-------------------------------------------------------------*/
/* Obtain file size. */
netwib_err netwox_url_download_size(netwox_httpclictx *pctx,
                                    netwib_constbuf *purl,
                                    netwib_uint32 *psize);
