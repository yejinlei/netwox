
/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 encrypt_subkeys[32];
  netwib_uint32 decrypt_subkeys[32];
} netwox_des_context;

/* a key must have a 8 byte size (ignore lsb which is parity) */
netwib_err netwox_des_context_init8(netwib_constbuf *pkey,
                                    netwib_bool forbidweakkeys,
                                    netwox_des_context *pcontext);
/* a key must have a 7 byte size (all bits are used) */
netwib_err netwox_des_context_init7(netwib_constbuf *pkey,
                                    netwib_bool forbidweakkeys,
                                    netwox_des_context *pcontext);

/*-------------------------------------------------------------*/
/* encrypt one block of 8 bytes (64 bits converted to 64 bits) */
#define NETWOX_DES_BLOCK_LEN 8
netwib_err netwox_des_encrypt(netwox_des_context *pcontext,
                              netwib_constbuf *pfrom,
                              netwib_buf *pto);
netwib_err netwox_des_decrypt(netwox_des_context *pcontext,
                              netwib_constbuf *pfrom,
                              netwib_buf *pto);

/*-------------------------------------------------------------*/
netwib_err netwox_des_test(void);
