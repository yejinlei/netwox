
/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_encodetype
   Description :
     Append the description text of an encodetype.
   Input parameter(s) :
     encodetype : netwib_encodetype to append
   Input/output parameter(s) :
     *pbuf : buffer where text is appended
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_append_encodetype(netwib_encodetype encodetype,
                                        netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* Name : netwib_encodetype_init_kbd
   Description :
     Initialize a netwib_encodetype with data entered through keyboard.
   Input parameter(s) :
     *pmessage : message to print before
     displayonlymostuseful : only most useful values are shown
     defaultencodetype : default encodetype to use if user enters nothing
                         (if 0xFFFFFFFFu, means no default)
   Input/output parameter(s) :
   Output parameter(s) :
     *pencodetype : netwib_encodetype initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_encodetype_init_kbd(netwib_constbuf *pmessage,
                                      netwib_bool displayonlymostuseful,
                                      netwib_encodetype defaultencodetype,
                                      netwib_encodetype *pencodetype);
#define NETWIB_ENCODETYPE_INIT_KBD_NODEF (netwib_encodetype)0xFFFFFFFFu
