
/*-------------------------------------------------------------*/
/* This structure will store Mime-Types and their allowed extensions.
   For example:
     text/html     html,htm
     image/gif     gif
 */
/*-------------------------------------------------------------*/
typedef struct {
  netwib_hash *phash;
} netwox_mimetypes;

/*-------------------------------------------------------------*/
netwib_err netwox_mimetypes_init(netwox_mimetypes *pmimetypes);
netwib_err netwox_mimetypes_close(netwox_mimetypes *pmimetypes);

/*-------------------------------------------------------------*/
netwib_err netwox_mimetypes_add(netwox_mimetypes *pmimetypes,
                                netwib_constbuf *pmimetype,
                                netwib_constbuf *pextensions);

/*-------------------------------------------------------------*/
netwib_err netwox_mimetypes_display(netwox_mimetypes *pmimetypes);

/*-------------------------------------------------------------*/
netwib_err netwox_mimetypes_search(netwox_mimetypes *pmimetypes,
                                   netwib_constbuf *pmimetype,
                                   netwib_array **pparray);

/*-------------------------------------------------------------*/
netwib_err netwox_mimetypes_contains_extension(netwox_mimetypes *pmimetypes,
                                               netwib_constbuf *pextension,
                                               netwib_bool *pyes);

/*-------------------------------------------------------------*/
netwib_err netwox_mimetypes_test(void);

/*-------------------------------------------------------------*/
netwib_err netwox_mimetype_init_contenttype(netwib_constbuf *pcontenttype,
                                            netwib_buf *pmimetype);

/*-------------------------------------------------------------*/
#define NETWOX_MIMETYPE_HTML "text/html"
