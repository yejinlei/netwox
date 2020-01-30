
/*-------------------------------------------------------------*/
/* Name : netwib_char_init_kbd
   Description :
     Ask user to enter a character
   Input parameter(s) :
     *pmessage : message to print before
     *pallowedchar : string containing allowed characters
                     For example : "aAbBrR"
                     If NULL all char are allowed
     defaultchar : default character (0 means no default)
   Input/output parameter(s) :
   Output parameter(s) :
     *pchar : character chosen
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_char_init_kbd(netwib_constbuf *pmessage,
                                netwib_constbuf *pallowedchars,
                                netwib_char defaultchar,
                                netwib_char *pchar);
#define NETWIB_CHAR_INIT_KBD_NODEF 0
