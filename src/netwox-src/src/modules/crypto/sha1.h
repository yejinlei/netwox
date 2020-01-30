
/*-------------------------------------------------------------*/
/* SHA-1: FIPS-180-1 (Federal Information Processing Standards), and also
   defined in FIPS-180-2
*/

/*-------------------------------------------------------------*/
/* size of blocks (input buffer is cut in blocks which are hashed) */
#define NETWOX_SHA1_BLOCK_LEN 64
/* size of hash (the result) */
#define NETWOX_SHA1_HASH_LEN 20

/*-------------------------------------------------------------*/
typedef struct {
  /* internal state */
  netwib_uint32 a;
  netwib_uint32 b;
  netwib_uint32 c;
  netwib_uint32 d;
  netwib_uint32 e;
  /* number of bytes */
  netwib_uint64 totalbytes;
  /* temporary storage */
  netwib_byte buf[NETWOX_SHA1_BLOCK_LEN];
  netwib_uint32 bufsize;
} netwox_sha1_context;

/*-------------------------------------------------------------*/
netwib_err netwox_sha1_init(netwox_sha1_context *pcontext);
netwib_err netwox_sha1_update(netwox_sha1_context *pcontext,
                              netwib_constbuf *pbuf);
netwib_err netwox_sha1_final(netwox_sha1_context *pcontext,
                             netwib_buf *psha1);

/*-------------------------------------------------------------*/
netwib_err netwox_sha1_compute(netwib_constbuf *pbuf,
                               netwib_buf *psha1);

/*-------------------------------------------------------------*/
netwib_err netwox_sha1_test(void);
