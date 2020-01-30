
/*-------------------------------------------------------------*/
/* rfc 954, 1834 and 3912
   This protocol is very simple. The problem is to find the good
   server.
*/

/*-------------------------------------------------------------*/
netwib_err netwox_whois_chooseserver(netwib_constbuf *pquery,
                                     netwib_bool verbose,
                                     netwib_ip *pserverip);

/*-------------------------------------------------------------*/
netwib_err netwox_whois_query(netwib_io *pio,
                              netwib_constbuf *pquery,
                              netwib_uint32 maxwaitms);
