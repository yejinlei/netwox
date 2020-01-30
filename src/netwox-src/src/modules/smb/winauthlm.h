
/*-------------------------------------------------------------*/
/* Windows LanManager authentication */

/*-------------------------------------------------------------*/
/* takes a password (truncated to 14 bytes), then generates a
   16 bytes LanManager hash */
#define NETWOX_WINAUTH_LM_HASH_LEN 16
netwib_err netwox_winauth_lm_hash(netwib_constbuf *ppassword,
                                  netwib_buf *phash);
netwib_err netwox_winauth_lm_hash_test(void);

/*-------------------------------------------------------------*/
/* generates a 8 bytes challenge */
#define NETWOX_WINAUTH_LM_CHAL_GENE_LEN 8
netwib_err netwox_winauth_lm_chal_gene(netwib_buf *pchal);

/*-------------------------------------------------------------*/
/* takes 8 bytes challenge and 16 bytes LM hash, and generates
   a 24 bytes answer */
#define NETWOX_WINAUTH_LM_CHAL_ANSWER_LEN 24
netwib_err netwox_winauth_lm_chal_answer(netwib_constbuf *pchal,
                                         netwib_constbuf *phash,
                                         netwib_buf *panswer);
netwib_err netwox_winauth_lm_chal_answer_test(void);
