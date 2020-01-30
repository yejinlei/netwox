
/*-------------------------------------------------------------*/
/* same as netwib_path_canon, but keep the last '/' for directories */
netwib_err netwox_urlcanon_path(netwib_constbuf *ppath,
                                netwib_buf *pcanonizedpath);

/*-------------------------------------------------------------*/
/* canonize a query like "v=val&v2=val2" */
netwib_err netwox_urlcanon_query(netwib_bufpool *pbufpool,
                                 netwib_constbuf *pquery,
                                 netwib_buf *pcanonizedquery);

/*-------------------------------------------------------------*/
/* canonize an :
 - absoluteURI : "http://...", "ftp://..."
 - relativeURI : ./..., /..., etc.
*/
netwib_err netwox_urlcanon_url(netwib_bufpool *pbufpool,
                               netwib_constbuf *purl,
                               netwib_buf *pcanonizedurl);

/*-------------------------------------------------------------*/
/*
 - pfileurl is the canonized absolute url of file containing purl
 - pbasehref is <BASE HREF> tag value (NULL or empty if none). It
   should be a canonized absolute url
 - purl is an url (absolute or relative, but not canonized) found
   in the file
 - pcanonizedurl is the absolute canonized url of purl
*/
netwib_err netwox_urlcanon_file_read(netwib_bufpool *pbufpool,
                                     netwib_constbuf *pfileurl,
                                     netwib_constbuf *pbasehref,
                                     netwib_constbuf *purl,
                                     netwib_buf *pcanonizedurl);

/*-------------------------------------------------------------*/
/*
 - purl is the canonized absolute url
 - pfileurl is the canonized absolute url of the file which will
   contain pcanonizedurl
 - pcanonizedurl is the url of purl to store in the file. It is
   relative if possible, absolute otherwise
Note : this function is not needed, so it is not yet implemented.
*/
netwib_err netwox_urlcanon_file_write(netwib_bufpool *pbufpool,
                                      netwib_constbuf *pfileurl,
                                      netwib_constbuf *purl,
                                      netwib_buf *pcanonizedurl);

