
/*-------------------------------------------------------------*/
/* MD2: rfc 1319 */

/*-------------------------------------------------------------*/
/* size of blocks (input buffer is cut in blocks which are hashed) */
#define NETWOX_MD2_BLOCK_LEN 16
/* size of hash (the result) */
#define NETWOX_MD2_HASH_LEN 16

/*-------------------------------------------------------------*/
typedef struct {
  /* internal state */
  netwib_uint8 state[16];
  /* the checksum is appended at the end, so also hashed */
  netwib_uint8 checksum[16];
  /* number of bytes */
  netwib_uint32 totalbytes;
  /* temporary storage */
  netwib_byte buf[NETWOX_MD2_BLOCK_LEN];
  netwib_uint32 bufsize;
} netwox_md2_context;

/*-------------------------------------------------------------*/
netwib_err netwox_md2_init(netwox_md2_context *pcontext);
netwib_err netwox_md2_update(netwox_md2_context *pcontext,
                             netwib_constbuf *pbuf);
netwib_err netwox_md2_final(netwox_md2_context *pcontext,
                            netwib_buf *pmd);

/*-------------------------------------------------------------*/
netwib_err netwox_md2_compute(netwib_constbuf *pbuf,
                              netwib_buf *pmd);

/*-------------------------------------------------------------*/
netwib_err netwox_md2_test(void);
