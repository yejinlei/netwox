
/*-------------------------------------------------------------*/
/* urls to download */

/* as we only need to store two small integers, we store them on 16 bits,
   and do not allocate memory for the ring item */
#define NETWOX_WEBSPIDERURL1_RECURSIONLEVEL(pitem) (((netwib_uintptr)pitem)>>16)
#define NETWOX_WEBSPIDERURL1_ELMTATTR(pitem) (((netwib_uintptr)pitem)&0xFFFF)

/*-------------------------------------------------------------*/
#define netwox_webspiderurl1_hash_init(pphash) netwib_hash_init(NULL,NULL,pphash)
#define netwox_webspiderurl1_hash_close(pphash) netwib_hash_close(pphash,NETWIB_FALSE)

/*-------------------------------------------------------------*/
netwib_err netwox_webspiderurl1_add(netwox_webspider *pwebspider,
                                    netwib_constbuf *purl,
                                    netwib_uint32 recursionlevel,/*start at 1*/
                                    netwox_htmltag_elmtattr elmtattr);
