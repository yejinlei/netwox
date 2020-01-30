
/*-------------------------------------------------------------*/
/*
  Important :
    This cypher (Gilbert Vernam, also known as XOR) is strong
    only if key is random, and has the same size as buffer.
    If this is not the case, a "known cleartext" attack can
    be used to break the algorithm and to obtain the full
    clear text.
*/

/*-------------------------------------------------------------*/
typedef struct {
  netwib_buf cypherkey;
  netwib_uint32 nextoffset;
} netwox_xor_context;

/*-------------------------------------------------------------*/
netwib_err netwox_xor_init(netwib_constbuf *pcypherkey,
                           netwox_xor_context *pcontext);
netwib_err netwox_xor_close(netwox_xor_context *pcontext);

/*-------------------------------------------------------------*/
netwib_err netwox_xor_cypher(netwox_xor_context *pcontext,
                             netwib_buf *pbuf);
#define netwox_xor_decypher(pcontext,pbuf) netwox_xor_cypher(pcontext,pbuf)

/*-------------------------------------------------------------*/
netwib_err netwox_xor_gene_key(netwib_bufpool *pbufpool,
                               netwib_constbuf *pinitseed,
                               netwib_uint32 minbytesize,
                               netwib_buf *pkey);
