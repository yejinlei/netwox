
/*-------------------------------------------------------------*/
/* Name : netwib_uint32_init_rand
   Description :
     Generate a random number
   Input parameter(s) :
     min : minimum value
     max : maximum value
   Input/output parameter(s) :
   Output parameter(s) :
     *pnumber : random number generated
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_uint32_init_rand(netwib_uint32 min,
                                   netwib_uint32 max,
                                   netwib_uint32 *pnumber);
#define netwib_uint32_init_rand_all(pnumber) netwib_uint32_init_rand(0,0xFFFFFFFFu,pnumber)
netwib_err netwib_uint64_init_rand(netwib_uint64 min,
                                   netwib_uint64 max,
                                   netwib_uint64 *pnumber);

/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_rand
   Description :
     Generate a random buffer
   Input parameter(s) :
     size : wanted buffer size
     min : minimal value
     max : maximal value
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : random buffer generated
   Normal return values :
     NETWIB_ERR_OK : ok
   This function sets NETWIB_BUF_FLAGS_SENSITIVE.
*/
netwib_err netwib_buf_append_rand(netwib_uint32 size,
                                  netwib_byte min,
                                  netwib_byte max,
                                  netwib_buf *pbuf);
#define netwib_buf_append_rand_all(size,pbuf) netwib_buf_append_rand(size,0,255,pbuf)
