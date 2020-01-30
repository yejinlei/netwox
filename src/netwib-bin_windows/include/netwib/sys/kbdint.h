
/*-------------------------------------------------------------*/
/* Name : netwib_uint32_init_kbd
   Description :
     Ask user to enter an integer.
   Input parameter(s) :
     *pmessage : message to print before
     min : minvalue which can be entered (if 0 no min)
     max : maxvalue which can be entered (if 0xFFFFFFFFu no max)
     defaultnumber : default number (if 0xFFFFFFFFu no default)
   Input/output parameter(s) :
   Output parameter(s) :
     *pnumber : number chosen
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_uint32_init_kbd(netwib_constbuf *pmessage,
                                  netwib_uint32 min,
                                  netwib_uint32 max,
                                  netwib_uint32 defaultnumber,
                                  netwib_uint32 *pnumber);
#define NETWIB_UINT32_INIT_KBD_NOMIN 0
#define NETWIB_UINT32_INIT_KBD_NOMAX 0xFFFFFFFFu
#define NETWIB_UINT32_INIT_KBD_NODEF 0xFFFFFFFFu
netwib_err netwib_uint64_init_kbd(netwib_constbuf *pmessage,
                                  netwib_uint64 defaultnumber,
                                  netwib_uint64 *pnumber);

/*-------------------------------------------------------------*/
/* Name : netwib_int32_init_kbd
   Description :
     Ask user to enter an integer.
   Input parameter(s) :
     *pmessage : message to print before
     min : minvalue which can be entered (if -0x80000000 no min)
     max : maxvalue which can be entered (if 0x7FFFFFFF no max)
     defaultnumber : default number (if 0x7FFFFFFF no default)
   Input/output parameter(s) :
   Output parameter(s) :
     *pnumber : number chosen
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_int32_init_kbd(netwib_constbuf *pmessage,
                                 netwib_int32 min,
                                 netwib_int32 max,
                                 netwib_int32 defaultnumber,
                                 netwib_int32 *pnumber);
#define NETWIB_INT32_INIT_KBD_NOMIN (-0x7FFFFFFF-1)
#define NETWIB_INT32_INIT_KBD_NOMAX 0x7FFFFFFF
#define NETWIB_INT32_INIT_KBD_NODEF 0x7FFFFFFF
netwib_err netwib_int64_init_kbd(netwib_constbuf *pmessage,
                                 netwib_int64 defaultnumber,
                                 netwib_int64 *pnumber);
