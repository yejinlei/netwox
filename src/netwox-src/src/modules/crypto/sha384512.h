
/*-------------------------------------------------------------*/
/* SHA-384, SHA-512: FIPS-180-2 (Federal Information Processing Standards) */
typedef enum {
  NETWOX_SHA384512VERSION_384 = 1,
  NETWOX_SHA384512VERSION_512 = 2,
} netwox_sha384512version;

/*-------------------------------------------------------------*/
/* size of blocks (input buffer is cut in blocks which are hashed) */
#define NETWOX_SHA384_BLOCK_LEN 128
#define NETWOX_SHA512_BLOCK_LEN 128
#define NETWOX_SHA384512_BLOCK_MAXLEN NETWOX_SHA512_BLOCK_LEN
/* size of hash (the result) */
#define NETWOX_SHA384_HASH_LEN 48
#define NETWOX_SHA512_HASH_LEN 64
#define NETWOX_SHA384512_HASH_MAXLEN NETWOX_SHA512_HASH_LEN

/*-------------------------------------------------------------*/
typedef struct {
  /* version */
  netwox_sha384512version sha384512version;
  /* internal state */
  netwib_uint64 a;
  netwib_uint64 b;
  netwib_uint64 c;
  netwib_uint64 d;
  netwib_uint64 e;
  netwib_uint64 f;
  netwib_uint64 g;
  netwib_uint64 h;
  /* number of bytes */
  netwib_uint64 totalbytes;
  /* temporary storage */
  netwib_byte buf[NETWOX_SHA384512_BLOCK_MAXLEN];
  netwib_uint32 bufsize;
} netwox_sha384512_context;

/*-------------------------------------------------------------*/
netwib_err netwox_sha384512_init(netwox_sha384512version sha384512version,
                                 netwox_sha384512_context *pcontext);
#define netwox_sha384_init(pcontext) netwox_sha384512_init(NETWOX_SHA384512VERSION_384,pcontext)
#define netwox_sha512_init(pcontext) netwox_sha384512_init(NETWOX_SHA384512VERSION_512,pcontext)
netwib_err netwox_sha384512_update(netwox_sha384512_context *pcontext,
                                   netwib_constbuf *pbuf);
netwib_err netwox_sha384512_final(netwox_sha384512_context *pcontext,
                                  netwib_buf *psha384512);

/*-------------------------------------------------------------*/
netwib_err netwox_sha384512_compute(netwox_sha384512version sha384512version,
                                    netwib_constbuf *pbuf,
                                    netwib_buf *psha384512);
#define netwox_sha384_compute(pbuf,psha384512) netwox_sha384512_compute(NETWOX_SHA384512VERSION_384,pbuf,psha384512)
#define netwox_sha512_compute(pbuf,psha384512) netwox_sha384512_compute(NETWOX_SHA384512VERSION_512,pbuf,psha384512)

/*-------------------------------------------------------------*/
netwib_err netwox_sha384512_test(void);
