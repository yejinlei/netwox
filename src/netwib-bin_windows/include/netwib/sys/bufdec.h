
/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_decodetype
   Description :
     Append the description text of an decodetype.
   Input parameter(s) :
     decodetype : netwib_decodetype to append
   Input/output parameter(s) :
     *pbuf : buffer where text is appended
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_append_decodetype(netwib_decodetype decodetype,
                                        netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* Name : netwib_decodetype_init_kbd
   Description :
     Initialize a netwib_decodetype with data entered through keyboard.
   Input parameter(s) :
     *pmessage : message to print before
     defaultdecodetype : default decodetype to use if user enters nothing
                         (if 0xFFFFFFFFu, means no default)
   Input/output parameter(s) :
   Output parameter(s) :
     *pdecodetype : netwib_decodetype initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_decodetype_init_kbd(netwib_constbuf *pmessage,
                                      netwib_decodetype defaultdecodetype,
                                      netwib_decodetype *pdecodetype);
#define NETWIB_DECODETYPE_INIT_KBD_NODEF (netwib_decodetype)0xFFFFFFFFu
