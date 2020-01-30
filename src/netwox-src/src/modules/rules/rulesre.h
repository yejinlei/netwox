
/*-------------------------------------------------------------*/
netwib_err netwox_rulesre_ruleinfos_create(netwib_constbuf *pregexp,
                                           netwib_ptr *pruleinfos);

/*-------------------------------------------------------------*/
netwib_err netwox_rulesre_ruleinfos_match(netwib_ptr ruleinfos,
                                          netwib_ptr userinfos,
                                          netwib_bool *pmatch);

/*-------------------------------------------------------------*/
netwib_err netwox_rulesre_ruleinfos_erase(netwib_ptr ruleinfos);

/*-------------------------------------------------------------*/
netwib_err netwox_rulesre_ruleinfos_display(netwib_ptr ruleinfos);

/*-------------------------------------------------------------*/
/* just a fake example to test and show how to use rules */
netwib_err netwox_rulesre_example(void);


