
/*-------------------------------------------------------------*/
/* Windows NTLMv1 authentication */

/*-------------------------------------------------------------*/
/* takes a password (truncated to 14 bytes), then generates a
   16 bytes NTLMv1 hash */
#define NETWOX_WINAUTH_NTLMV1_HASH_LEN 16
netwib_err netwox_winauth_ntlmv1_hash(netwib_constbuf *ppassword,
                                      netwib_buf *phash);
netwib_err netwox_winauth_ntlmv1_hash_test(void);

/*-------------------------------------------------------------*/
/* NTLMv1 challenge is like LM */
#define NETWOX_WINAUTH_NTLMV1_CHAL_GENE_LEN NETWOX_WINAUTH_LM_CHAL_GENE_LEN
#define NETWOX_WINAUTH_NTLMV1_CHAL_ANSWER_LEN NETWOX_WINAUTH_LM_CHAL_ANSWER_LEN
#define netwox_winauth_ntlmv1_chal_gene(pchal) netwox_winauth_lm_chal_gene(pchal)
#define netwox_winauth_ntlmv1_chal_answer(pchal,phash,panswer) netwox_winauth_lm_chal_answer(pchal,phash,panswer)
