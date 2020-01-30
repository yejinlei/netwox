
/*-------------------------------------------------------------*/
/* MD4: rfc 1320
   MD5: rfc 1321
*/
typedef enum {
  NETWOX_MDVERSION_4 = 4,
  NETWOX_MDVERSION_5 = 5,
} netwox_mdversion;

/*-------------------------------------------------------------*/
/* size of blocks (input buffer is cut in blocks which are hashed) */
#define NETWOX_MD4_BLOCK_LEN 64
#define NETWOX_MD5_BLOCK_LEN 64
#define NETWOX_MD_BLOCK_MAXLEN NETWOX_MD5_BLOCK_LEN
/* size of hash (the result) */
#define NETWOX_MD4_HASH_LEN 16
#define NETWOX_MD5_HASH_LEN 16
#define NETWOX_MD_HASH_MAXLEN NETWOX_MD5_HASH_LEN

/*-------------------------------------------------------------*/
typedef struct {
  /* version */
  netwox_mdversion mdversion;
  /* internal state */
  netwib_uint32 a;
  netwib_uint32 b;
  netwib_uint32 c;
  netwib_uint32 d;
  /* number of bytes */
  netwib_uint64 totalbytes;
  /* temporary storage */
  netwib_byte buf[NETWOX_MD_BLOCK_MAXLEN];
  netwib_uint32 bufsize;
} netwox_md_context;

/*-------------------------------------------------------------*/
netwib_err netwox_md_init(netwox_mdversion mdversion,
                          netwox_md_context *pcontext);
#define netwox_md4_init(pcontext) netwox_md_init(NETWOX_MDVERSION_4,pcontext)
#define netwox_md5_init(pcontext) netwox_md_init(NETWOX_MDVERSION_5,pcontext)
netwib_err netwox_md_update(netwox_md_context *pcontext,
                            netwib_constbuf *pbuf);
netwib_err netwox_md_final(netwox_md_context *pcontext,
                           netwib_buf *pmd);

/*-------------------------------------------------------------*/
netwib_err netwox_md_compute(netwox_mdversion mdversion,
                             netwib_constbuf *pbuf,
                             netwib_buf *pmd);
#define netwox_md4_compute(pbuf,pmd) netwox_md_compute(NETWOX_MDVERSION_4,pbuf,pmd)
#define netwox_md5_compute(pbuf,pmd) netwox_md_compute(NETWOX_MDVERSION_5,pbuf,pmd)

/*-------------------------------------------------------------*/
netwib_err netwox_md_test(void);
