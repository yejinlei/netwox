
/*-------------------------------------------------------------*/
netwib_err netwox_expect(netwib_buf *pbuf,
                         netwib_conststring regularexpression1,
                         netwib_conststring regularexpression2,
                         netwib_conststring regularexpression3,
                         netwib_conststring regularexpression4,
                         netwib_bool casesensitive,
                         netwib_io *pio,
                         netwib_consttime *pabstime,
                         netwib_uint32 *pnumfound); /* 0 if not found */
#define netwox_expect1(pbuf,regularexpression1,casesensitive,pio,pabstime,pnumfound) netwox_expect(pbuf,regularexpression1,NULL,NULL,NULL,casesensitive,pio,pabstime,pnumfound)
#define netwox_expect2(pbuf,regularexpression1,regularexpression2,casesensitive,pio,pabstime,pnumfound) netwox_expect(pbuf,regularexpression1,regularexpression2,NULL,NULL,casesensitive,pio,pabstime,pnumfound)
#define netwox_expect3(pbuf,regularexpression1,regularexpression2,regularexpression3,casesensitive,pio,pabstime,pnumfound) netwox_expect(pbuf,regularexpression1,regularexpression2,regularexpression3,NULL,casesensitive,pio,pabstime,pnumfound)
#define netwox_expect4(pbuf,regularexpression1,regularexpression2,regularexpression3,regularexpression4,casesensitive,pio,pabstime,pnumfound) netwox_expect(pbuf,regularexpression1,regularexpression2,regularexpression3,regularexpression4,casesensitive,pio,pabstime,pnumfound)
