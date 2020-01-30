
/*-------------------------------------------------------------*/
/* SHA-256: FIPS-180-2 (Federal Information Processing Standards)
   SHA-224: FIPS-180-2 with change notice (added in 2004)
*/
typedef enum {
  NETWOX_SHA224256VERSION_224 = 1,
  NETWOX_SHA224256VERSION_256 = 2,
} netwox_sha224256version;

/*-------------------------------------------------------------*/
/* size of blocks (input buffer is cut in blocks which are hashed) */
#define NETWOX_SHA224_BLOCK_LEN 64
#define NETWOX_SHA256_BLOCK_LEN 64
#define NETWOX_SHA224256_BLOCK_MAXLEN NETWOX_SHA256_BLOCK_LEN
/* size of hash (the result) */
#define NETWOX_SHA224_HASH_LEN 28
#define NETWOX_SHA256_HASH_LEN 32
#define NETWOX_SHA224256_HASH_MAXLEN NETWOX_SHA256_HASH_LEN

/*-------------------------------------------------------------*/
typedef struct {
  /* version */
  netwox_sha224256version sha224256version;
  /* internal state */
  netwib_uint32 a;
  netwib_uint32 b;
  netwib_uint32 c;
  netwib_uint32 d;
  netwib_uint32 e;
  netwib_uint32 f;
  netwib_uint32 g;
  netwib_uint32 h;
  /* number of bytes */
  netwib_uint64 totalbytes;
  /* temporary storage */
  netwib_byte buf[NETWOX_SHA224256_BLOCK_MAXLEN];
  netwib_uint32 bufsize;
} netwox_sha224256_context;

/*-------------------------------------------------------------*/
netwib_err netwox_sha224256_init(netwox_sha224256version sha224256version,
                                 netwox_sha224256_context *pcontext);
#define netwox_sha224_init(pcontext) netwox_sha224256_init(NETWOX_SHA224256VERSION_224,pcontext)
#define netwox_sha256_init(pcontext) netwox_sha224256_init(NETWOX_SHA224256VERSION_256,pcontext)
netwib_err netwox_sha224256_update(netwox_sha224256_context *pcontext,
                                   netwib_constbuf *pbuf);
netwib_err netwox_sha224256_final(netwox_sha224256_context *pcontext,
                                  netwib_buf *psha224256);

/*-------------------------------------------------------------*/
netwib_err netwox_sha224256_compute(netwox_sha224256version sha224256version,
                                    netwib_constbuf *pbuf,
                                    netwib_buf *psha224256);
#define netwox_sha224_compute(pbuf,psha224256) netwox_sha224256_compute(NETWOX_SHA224256VERSION_224,pbuf,psha224256)
#define netwox_sha256_compute(pbuf,psha224256) netwox_sha224256_compute(NETWOX_SHA224256VERSION_256,pbuf,psha224256)

/*-------------------------------------------------------------*/
netwib_err netwox_sha224256_test(void);
