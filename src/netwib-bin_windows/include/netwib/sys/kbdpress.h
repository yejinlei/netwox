
/*-------------------------------------------------------------*/
/* Name : netwib_kbd_press
   Description :
     Ask user to press a key
   Input parameter(s) :
     *pmessage : message to print before
   Input/output parameter(s) :
   Output parameter(s) :
     *pchar : character chosen
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_kbd_press(netwib_constbuf *pmessage,
                            netwib_char *pchar);

/*-------------------------------------------------------------*/
/* Name : netwib_kbd_purge
   Description :
     Purge every pressed keys
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_kbd_purge(void);
