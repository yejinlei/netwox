
/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_CRYPTOHASH_TYPE_MD2 =             1,
  NETWOX_CRYPTOHASH_TYPE_MD4 =             2,
  NETWOX_CRYPTOHASH_TYPE_MD5 =             3,
  NETWOX_CRYPTOHASH_TYPE_RIPEMD128 =       4,
  NETWOX_CRYPTOHASH_TYPE_RIPEMD160 =       5,
  NETWOX_CRYPTOHASH_TYPE_SHA1 =            6,
  NETWOX_CRYPTOHASH_TYPE_SHA224 =          7,
  NETWOX_CRYPTOHASH_TYPE_SHA256 =          8,
  NETWOX_CRYPTOHASH_TYPE_SHA384 =          9,
  NETWOX_CRYPTOHASH_TYPE_SHA512 =         10,
  NETWOX_CRYPTOHASH_TYPE__FIRST = NETWOX_CRYPTOHASH_TYPE_MD2,
  NETWOX_CRYPTOHASH_TYPE__LAST =  NETWOX_CRYPTOHASH_TYPE_SHA512
} netwox_cryptohash_type;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_cryptohash_type hashtype;
  union {
    netwox_md2_context md2;
    netwox_md_context md;
    netwox_ripemd_context ripemd;
    netwox_sha1_context sha1;
    netwox_sha224256_context sha224256;
    netwox_sha384512_context sha384512;
  } value;
} netwox_cryptohash_context;

/*-------------------------------------------------------------*/
/* the maximum length of blocks (currently SHA512) */
#define NETWOX_CRYPTOHASH_BLOCK_MAXLEN NETWOX_SHA512_BLOCK_LEN
/* the maximum length of a hash (currently SHA512) */
#define NETWOX_CRYPTOHASH_HASH_MAXLEN NETWOX_SHA512_HASH_LEN
/* obtain the real size of blocks and hash */
netwib_err netwox_cryptohash_size(netwox_cryptohash_type hashtype,
                                  netwib_uint32 *pblocksize,
                                  netwib_uint32 *phashsize);
netwib_err netwox_buf_append_cryptohash_type(netwox_cryptohash_type hashtype,
                                             netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_cryptohash_init(netwox_cryptohash_type hashtype,
                                  netwox_cryptohash_context *pcontext);
netwib_err netwox_cryptohash_update(netwox_cryptohash_context *pcontext,
                                    netwib_constbuf *pbuf);
netwib_err netwox_cryptohash_final(netwox_cryptohash_context *pcontext,
                                   netwib_buf *phash);

/*-------------------------------------------------------------*/
netwib_err netwox_cryptohash_final_hexa(netwox_cryptohash_context *pcontext,
                                        netwib_buf *phexahash);

/*-------------------------------------------------------------*/
netwib_err netwox_cryptohash_compute(netwox_cryptohash_type hashtype,
                                     netwib_constbuf *pbuf,
                                     netwib_buf *phash);
netwib_err netwox_cryptohash_compute_hexa(netwox_cryptohash_type hashtype,
                                          netwib_constbuf *pbuf,
                                          netwib_buf *phexahash);

/*-------------------------------------------------------------*/
netwib_err netwox_cryptohash_compute_file(netwox_cryptohash_type hashtype,
                                          netwib_constbuf *pfilename,
                                          netwib_buf *phexahash);
