
/*-------------------------------------------------------------*/
/* RIPEMD-128: developed by RIPE project (RACE Integrity Primitives Evaluation)
   RIPEMD-160: designed by Hans Dobbertin, Antoon Bosselaers and Bart Preneel.
   http://www.esat.kuleuven.be/~cosicart/
   http://www.esat.kuleuven.ac.be/~bosselae/ripemd160.html
*/
typedef enum {
  NETWOX_RIPEMDVERSION_128 = 1,
  NETWOX_RIPEMDVERSION_160 = 2,
} netwox_ripemdversion;

/*-------------------------------------------------------------*/
/* size of blocks (input buffer is cut in blocks which are hashed) */
#define NETWOX_RIPEMD128_BLOCK_LEN 64
#define NETWOX_RIPEMD160_BLOCK_LEN 64
#define NETWOX_RIPEMD_BLOCK_MAXLEN NETWOX_RIPEMD160_BLOCK_LEN
/* size of hash (the result) */
#define NETWOX_RIPEMD128_HASH_LEN 16
#define NETWOX_RIPEMD160_HASH_LEN 20
#define NETWOX_RIPEMD_HASH_MAXLEN NETWOX_RIPEMD160_HASH_LEN

/*-------------------------------------------------------------*/
typedef struct {
  /* version */
  netwox_ripemdversion ripemdversion;
  /* internal state */
  netwib_uint32 a;
  netwib_uint32 b;
  netwib_uint32 c;
  netwib_uint32 d;
  netwib_uint32 e;
  /* number of bytes */
  netwib_uint64 totalbytes;
  /* temporary storage */
  netwib_byte buf[NETWOX_RIPEMD_BLOCK_MAXLEN];
  netwib_uint32 bufsize;
} netwox_ripemd_context;

/*-------------------------------------------------------------*/
netwib_err netwox_ripemd_init(netwox_ripemdversion mdversion,
                              netwox_ripemd_context *pcontext);
#define netwox_ripemd128_init(pcontext) netwox_ripemd_init(NETWOX_RIPEMDVERSION_128,pcontext)
#define netwox_ripemd160_init(pcontext) netwox_ripemd_init(NETWOX_RIPEMDVERSION_160,pcontext)
netwib_err netwox_ripemd_update(netwox_ripemd_context *pcontext,
                                netwib_constbuf *pbuf);
netwib_err netwox_ripemd_final(netwox_ripemd_context *pcontext,
                               netwib_buf *pmd);

/*-------------------------------------------------------------*/
netwib_err netwox_ripemd_compute(netwox_ripemdversion mdversion,
                                 netwib_constbuf *pbuf,
                                 netwib_buf *pmd);
#define netwox_ripemd128_compute(pbuf,pmd) netwox_ripemd_compute(NETWOX_RIPEMDVERSION_128,pbuf,pmd)
#define netwox_ripemd160_compute(pbuf,pmd) netwox_ripemd_compute(NETWOX_RIPEMDVERSION_160,pbuf,pmd)

/*-------------------------------------------------------------*/
netwib_err netwox_ripemd_test(void);
