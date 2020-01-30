
/*-------------------------------------------------------------*/
/*
 Note : currently, this spider does not support :
    - cookies
    - NETWOX_HTMLTAG_ELMTATTR_URIFORMAT_LISTSPACED (<object archive=...>)
    - NETWOX_HTMLTAG_ELMTATTR_URIFORMAT_LISTCOMMA (<applet archive=...>)
    - IMG ISMAP (<a href="t"><img src=u ismap></A>)
    - POST form with its variables
    - Javascript/VBscript (to search new urls)
*/

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_WEBSPIDER_LINKCONV_NONE = 0,
  NETWOX_WEBSPIDER_LINKCONV_WEB = 1
} netwox_webspider_linkconv;

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_WEBSPIDER_LOGLEVEL_NONE = 0,
  NETWOX_WEBSPIDER_LOGLEVEL_NORMAL = 1,
  NETWOX_WEBSPIDER_LOGLEVEL_DOT = 2,
  NETWOX_WEBSPIDER_LOGLEVEL_THREAD = 3,
  NETWOX_WEBSPIDER_LOGLEVEL_SCALE = 4,
  NETWOX_WEBSPIDER_LOGLEVEL_FRACTION = 5
} netwox_webspider_loglevel;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_buf localrootdir;
  netwox_httpclictx httpclictx;
  netwib_ring *pliststarturl; /* urls requested by user, added in the index */
  netwib_hash *phashurlstodownload;
  netwib_hash *phashdownloadedurls;
  netwib_hash *phashurlsinprogress; /* for mt only */
  netwib_uint32 maxrecursion;
  netwib_uint32 maxthreads;
  netwib_uint32 millisleep;
  netwib_bool aclurlset;
  netwox_rules *paclurl;
  netwib_bool getimage;
  netwib_bool getmultimedia;
  netwib_bool getcode;
  netwox_webspider_linkconv linkconv;
  netwox_webspider_loglevel loglevel;
  netwox_mimetypes mimetypes;
  netwib_bool mimetypeswarn;
  netwib_array deletedextensions;
  netwib_uint32 numtotalurls;
  netwib_uint64 numdownloadedurls;
} netwox_webspider;

/*-------------------------------------------------------------*/
netwib_err netwox_webspider_init(netwox_webspider *pwebspider);
netwib_err netwox_webspider_close(netwox_webspider *pwebspider);

/*-------------------------------------------------------------*/
netwib_err netwox_webspider_add(netwox_webspider *pwebspider,
                                netwib_constbuf *purl);
netwib_err netwox_webspider_add_canon(netwox_webspider *pwebspider,
                                      netwib_constbuf *purl);

/*-------------------------------------------------------------*/
netwib_err netwox_webspider_run(netwox_webspider *pwebspider);

/*-------------------------------------------------------------*/
netwib_err netwox_webspider_relink(netwox_webspider *pwebspider);

/*-------------------------------------------------------------*/
netwib_err netwox_webspider_index(netwox_webspider *pwebspider);

/*-------------------------------------------------------------*/
netwib_err netwox_webspider_createurllist(netwox_webspider *pwebspider);

/*-------------------------------------------------------------*/
netwib_err netwox_webspider_deltmp(netwox_webspider *pwebspider);

