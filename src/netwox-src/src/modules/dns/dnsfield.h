
/*-------------------------------------------------------------*/
/*
  If pfullpkt == NULL, only convert :
     "aa.bbb.c"   to 02'aa'03'bbb'01'c'00
     ".aa..b."    to 02'aa'01'b'00
     "a.b@c.d"    to 03'a.b'01'c'01'd'00
  Else, also compress as described in RFC
*/
netwib_err netwox_dnsfield_dom_initcomp(netwib_constbuf *pfullpkt,
                                        netwib_constbuf *pdatain,
                                        netwib_bool datainisemail,
                                        netwib_buf *pdataout);
#define netwox_dnsfield_host_initcomp(pfullpkt,pdatain,pdataout) netwox_dnsfield_dom_initcomp(pfullpkt,pdatain,NETWIB_FALSE,pdataout)
#define netwox_dnsfield_email_initcomp(pfullpkt,pdatain,pdataout) netwox_dnsfield_dom_initcomp(pfullpkt,pdatain,NETWIB_TRUE,pdataout)

/*-------------------------------------------------------------*/
/*
  If pfullpkt == NULL, only convert :
     02'aa'03'bbb'01'c'00 to "aa.bbb.c"
     02'aa'01'b'00        to ".aa..b."
     03'a.b'01'c'01'd'00  to "a.b@c.d"
  Else, also uncompress as described in RFC
*/
netwib_err netwox_dnsfield_dom_uncdecode(netwib_constbuf *pfullpkt,
                                         netwib_buf *pdatain,
                                         netwib_bool datainisemail,
                                         netwib_buf *pdataout);
#define netwox_dnsfield_host_uncdecode(pfullpkt,pdatain,pdataout) netwox_dnsfield_dom_uncdecode(pfullpkt,pdatain,NETWIB_FALSE,pdataout)
#define netwox_dnsfield_email_uncdecode(pfullpkt,pdatain,pdataout) netwox_dnsfield_dom_uncdecode(pfullpkt,pdatain,NETWIB_TRUE,pdataout)

/*-------------------------------------------------------------*/
/*
  Convert :
     "aaa"   to 03'aaa'
*/
netwib_err netwox_dnsfield_text_init(netwib_constbuf *pdatain,
                                     netwib_buf *pdataout);

/*-------------------------------------------------------------*/
/*
  Convert :
     02'aa' to "aa"
*/
netwib_err netwox_dnsfield_text_decode(netwib_buf *pdatain,
                                       netwib_buf *pdataout);


