
/*-------------------------------------------------------------*/
/* HMAC : RFC 2104 (add a key/password to a hash function: Keyed-Hashing) */

/*-------------------------------------------------------------*/
#define NETWOX_HMAC_HASH_MAXLEN NETWOX_CRYPTOHASH_HASH_MAXLEN

/*-------------------------------------------------------------*/
typedef struct {
  /* hash used */
  netwox_cryptohash_type hashtype;
  /* precomputed keys */
  netwib_uint32 blocksize; /* real size used in ki and ko */
  netwib_byte ki[NETWOX_CRYPTOHASH_BLOCK_MAXLEN];
  netwib_byte ko[NETWOX_CRYPTOHASH_BLOCK_MAXLEN];
} netwox_hmac_keys;

netwib_err netwox_hmac_keys_init(netwox_cryptohash_type hashtype,
                                 netwib_constbuf *pkey, /* password */
                                 netwox_hmac_keys *pkeys);

/*-------------------------------------------------------------*/
netwib_err netwox_hmac_compute(netwox_hmac_keys *pkeys,
                               netwib_constbuf *pbuf,
                               netwib_buf *phmac);

/*-------------------------------------------------------------*/
netwib_err netwox_hmac_test(void);
