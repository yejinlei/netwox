
/*-------------------------------------------------------------*/
netwib_err netwib_priv_ips_array_init_ip(netwib_constip *pip,
                                         netwib_byte array[]);
netwib_err netwib_priv_ips_ip_init_array(netwib_byte array[],
                                         netwib_ip *pip);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ips_add_string(netwib_priv_ranges *pr,
                                      netwib_conststring str,
                                      netwib_bool usedel);
