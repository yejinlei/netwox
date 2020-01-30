
/*-------------------------------------------------------------*/
/* Name : netwib_port_init_kbd
   Description :
     Initialize a netwib_port with data entered through keyboard.
   Input parameter(s) :
     *pmessage : message to print before
     min : minvalue which can be entered (if 0 no min)
     max : maxvalue which can be entered (if 0xFFFFFFFFu no max)
     defaultport : default port to use if user enters nothing
                   if 0xFFFFFFFFu, there is no default
   Input/output parameter(s) :
   Output parameter(s) :
     *pport : netwib_port initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_port_init_kbd(netwib_constbuf *pmessage,
                                netwib_port min,
                                netwib_port max,
                                netwib_port defaultport,
                                netwib_port *pport);
#define NETWIB_PORT_INIT_KBD_NOMIN 0
#define NETWIB_PORT_INIT_KBD_NOMAX 0xFFFFFFFFu
#define NETWIB_PORT_INIT_KBD_NODEF 0xFFFFFFFFu


/*-------------------------------------------------------------*/
/* Name : netwib_port_init_buf
   Description :
     Convert a string "123" to a netwib_port.
   Input parameter(s) :
     *pbuf : string to convert
   Input/output parameter(s) :
   Output parameter(s) :
     *pport : netwib_port set
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_port_init_buf(netwib_constbuf *pbuf,
                                netwib_port *pport);

/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_port
   Description :
     Append a string representing a netwib_port.
   Input parameter(s) :
     port : netwib_port to append
   Input/output parameter(s) :
     pbuf : buffer updated
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_append_port(netwib_port port,
                                  netwib_buf *pbuf);
