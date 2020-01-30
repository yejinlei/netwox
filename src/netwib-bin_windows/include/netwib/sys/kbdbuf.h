
/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_kbd
   Description :
     Ask user to enter a text on keyboard.
   Input parameter(s) :
     *pmessage : message to print before asking user
     *pdefaultext : text string to use if user enters nothing
                    if NULL, there is no default
   Input/output parameter(s) :
   Output parameter(s) :
     pbuf : output buffer set with the text
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_append_kbd(netwib_constbuf *pmessage,
                                 netwib_constbuf *pdefaulttext,
                                 netwib_buf *pbuf);
#define NETWIB_BUF_APPEND_KBD_NODEF NULL

/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_password
   Description :
     Ask user to enter a password on keyboard.
   Input parameter(s) :
     *pmessage : message to print before asking user
     *pdefaultext : text string to use if user enters nothing
   Input/output parameter(s) :
   Output parameter(s) :
     pbuf : output buffer set with the text
   Normal return values :
     NETWIB_ERR_OK : ok
   This function sets NETWIB_BUF_FLAGS_SENSITIVE.
*/
netwib_err netwib_buf_append_password(netwib_constbuf *pmessage,
                                      netwib_constbuf *pdefaulttext,
                                      netwib_buf *pbuf);
