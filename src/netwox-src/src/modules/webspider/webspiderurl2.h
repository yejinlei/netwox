
/*-------------------------------------------------------------*/
/* downloaded urls */

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bool parsehtml;
  netwib_buf localrelativefilename; /* to save memory do not save rootdir */
} netwox_webspiderurl2_item;

/*-------------------------------------------------------------*/
netwib_err netwox_webspiderurl2_item_create(netwib_bool parsehtml,
                                            netwib_constbuf *plocalrelativefilename,
                                            netwib_ptr *ppitem);
netwib_err netwox_webspiderurl2_item_erase(netwib_ptr pitem);

/*-------------------------------------------------------------*/
#define netwox_webspiderurl2_hash_init(pphash) netwib_hash_init(&netwox_webspiderurl2_item_erase,NULL,pphash)
#define netwox_webspiderurl2_hash_close(pphash) netwib_hash_close(pphash,NETWIB_TRUE)

/*-------------------------------------------------------------*/
netwib_err netwox_webspiderurl2_add(netwox_webspider *pwebspider,
                                    netwib_constbuf *purl,
                                    netwib_bool parsehtml,
                                    netwib_constbuf *plocalfilename);

/*-------------------------------------------------------------*/
netwib_err netwox_webspiderurl2_downloaded(netwox_webspider *pwebspider,
                                           netwib_constbuf *purl,
                                           netwib_bool *pdownloaded,
                                           netwib_buf *plocalfilename);
