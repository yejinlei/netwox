
/*-------------------------------------------------------------*/
/* Windows NTLMv2 authentication */

/*-------------------------------------------------------------*/
/* server generates a challenge */
#define NETWOX_WINAUTH_NTLMV2_CHAL_GENE_LEN NETWOX_WINAUTH_NTLMV1_CHAL_GENE_LEN
#define netwox_winauth_ntlmv2_chal_gene(pchal) netwox_winauth_ntlmv1_chal_gene(pchal)

/*-------------------------------------------------------------*/
/* client generates a "blob", from :
     - servername (ASCII, if known)
     - domain (ASCII, if known)
     - [retrieve current date/time]
 */
netwib_err netwox_winauth_ntlmv2_blob_gene(netwib_constbuf *pservername,
                                           netwib_constbuf *pdomain,
                                           netwib_buf *pblob);

/*-------------------------------------------------------------*/
/* server obtain blob stored in pntlmv2answer from client
 */
netwib_err netwox_winauth_ntlmv2_blob_get(netwib_constbuf *pntlmv2answer,
                                          netwib_bufext *pblob);

/*-------------------------------------------------------------*/
/* takes :
     - server challenge (8 bytes)
     - password (ASCII)
     - username (ASCII)
     - target (servername or domain, in ASCII)
     - blob
   then answers to the challenge (NTLMv2 and LMv2)
*/
netwib_err netwox_winauth_ntlmv2_chal_answer(netwib_constbuf *pchal,
                                             netwib_constbuf *pusername,
                                             netwib_constbuf *ppassword,
                                             netwib_constbuf *ptarget,
                                             netwib_constbuf *pblob,
                                             netwib_buf *pntlmv2answer,
                                             netwib_buf *plmv2answer);
netwib_err netwox_winauth_ntlmv2_chal_answer_test(void);
