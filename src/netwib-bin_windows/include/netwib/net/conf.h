
/*-------------------------------------------------------------*/
/* Name : netwib_conf_display
   Description :
     Print devices/arp/routes configuration.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf receiving data
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_conf_display(void);
netwib_err netwib_buf_append_conf(netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* For debugging purposes only */
netwib_err netwib_conf_debug_display(void);
netwib_err netwib_buf_append_conf_debug(netwib_buf *pbuf);
