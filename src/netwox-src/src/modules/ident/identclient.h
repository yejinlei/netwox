
/*-------------------------------------------------------------*/
/* IDENT protocol is defined in RFC 1413
   A simple client retrieving response line is the only
   part implemented.
*/

/*-------------------------------------------------------------*/
/* use netwox_txtproto_io_rd before */
netwib_err netwox_identclient_query(netwib_io *pio,
                                    netwib_port localport,
                                    netwib_port remoteport,
                                    netwib_uint32 maxwaitms,
                                    netwib_buf *panswer);
