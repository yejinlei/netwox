
/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_BRUTE_RESULT_GOOD = 1,
  NETWOX_BRUTE_RESULT_BAD,
  NETWOX_BRUTE_RESULT_RETRY
} netwox_brute_result;

/*-------------------------------------------------------------*/
typedef netwib_err (*netwox_brute_pf)(netwib_constbuf *plogin,
                                      netwib_constbuf *ppassword,
                                      netwib_ptr infos,
                                      netwox_brute_result *presult);

/*-------------------------------------------------------------*/
netwib_err netwox_brute(netwib_ring *pringlogin,
                        netwib_ring *pringpassword,
                        netwib_ptr infos,
                        netwib_uint32 numthread,
                        netwib_bool stopatfirstfound,
                        netwib_bool verbose,
                        netwox_brute_pf pfunc);
