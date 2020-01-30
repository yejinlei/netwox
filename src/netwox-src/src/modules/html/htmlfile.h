
/*-------------------------------------------------------------*/
/*
   <element attribute="value">
   ^tagbegin                 ^tagend
            ^attributebegin ^attributeend
                      ^valb ^valueend
*/
typedef struct {
  netwox_htmltag_elmtattr elmtattr;
  netwib_uint32 tagbegin;
  netwib_uint32 tagend;
  netwib_uint32 attributebegin;
  netwib_uint32 attributeend;
  netwib_uint32 valuebegin;
  netwib_uint32 valueend;
  netwib_buf value; /* real value (after a call of netwox_html_data_decode) */
  /* following is only set if used for replacing */
  netwib_bool supportreplacing;
  netwib_bool suppresstag; /* from tagbegin to tagend */
  netwib_bool suppressattribute; /* from attributebegin to attributeend */
  netwib_buf replacingvalue; /* this value will be encoded with
                                netwox_html_data_decode, and surrounded
                                by quotes. It will replace from valuebegin to
                                valueend */
} netwox_htmlfile_urllist_item;

/*-------------------------------------------------------------*/
netwib_err netwox_htmlfile_urllist_item_create(netwib_bool supportreplacing,
                                               netwib_ptr *ppitem);
netwib_err netwox_htmlfile_urllist_item_erase(netwib_ptr pitem);

/*-------------------------------------------------------------*/
/* a ring of netwox_htmlfile_urllist_item */
#define netwox_htmlfile_urllist_ring_init(ppring) netwib_ring_init(&netwox_htmlfile_urllist_item_erase,NULL,ppring)
#define netwox_htmlfile_urllist_ring_close(ppring) netwib_ring_close(ppring,NETWIB_TRUE)
netwib_err netwox_htmlfile_urllist_ring_display(netwib_ring *pring);

/*-------------------------------------------------------------*/
/* read an HTML file and store urls in the ring. Note : we only
   look for urls (instead of trying to properly decode the HTML
   file) : this function does not implement a compliant parser,
   but is specialized in retrieving urls. */
netwib_err netwox_htmlfile_urllist_file_read(netwib_constbuf *pfilename,
                                             netwib_bool supportreplacing,
                                             netwib_ring *pring);

/*-------------------------------------------------------------*/
/* update the urls of an HTML file accordingly to items in the ring */
netwib_err netwox_htmlfile_urllist_file_write(netwib_constbuf *pinfilename,
                                              netwib_ring *pring,
                                              netwib_constbuf *poutfilename);

/*-------------------------------------------------------------*/
/* canonize all values in the ring */
netwib_err netwox_htmlfile_urllist_canon(netwib_bufpool *pbufpool,
                                         netwib_constbuf *pfileurl,
                                         netwib_bool onlysupported,
                                         netwib_ring *pring);
