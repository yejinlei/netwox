
/*-------------------------------------------------------------*/
/*
  A rdata contains specific information about a rr.
  It can be compressed or not in a packet.
  It must not be compressed in a netwox_dnsrr.
  Following functions initialize or decode a rdata buffer. They
  can also compress/uncompress, but in order to do that, they need
  full packet starting from DNS ID field.
  So, we have 4 kind of functions (really 2 functions and 2 defines) :
   - netwox_dnsrdata_initcomp_ : init and compress (used for packet creation)
   - netwox_dnsrdata_init_ : init only (used for netwox_dnsrr creation)
   - netwox_dnsrdata_uncdecode_ : uncompress and decode (for pkt decoding)
   - netwox_dnsrdata_decode_ : decode only (used for netwox_dnsrr decoding)
  Note : some types (such as A) do not need compression, so they have only
         2 functions
*/

/*-------------------------------------------------------------*/
/* if address is IPv6, AAAA is used instead : ptype is A or AAAA
   Note : we do not use A6 here, because it is too complicated
   (need to know prefix length and prefix name) */
netwib_err netwox_dnsrdata_init_a(netwib_constip *paddress,
                                  netwox_dnsrr_type *ptype,
                                  netwib_buf *prdata);
/* this function can be used to decode A or AAAA */
netwib_err netwox_dnsrdata_decode_a(netwox_dnsrr_type type,
                                    netwib_constbuf *prdata,
                                    netwib_ip *paddress);

/*-------------------------------------------------------------*/
/* A6 is a bit complicated (rfc 2874) :
     prefixlen : size of prefix in bits
     address : ip address with first prefixlen bit zero
     prefixname : name of the prefix
   For example :
     0 fec0::10 "" =
       IP address fec0::10
     16 0:b:c:d:e:f:g:h "foo" =
       IP address 1234:b:c:d:e:f:g:h if network foo = 1234:: (16 bits)
 */
netwib_err netwox_dnsrdata_init_a6(netwib_uint32 prefixlen,
                                   netwib_constip *paddress,
                                   netwib_constbuf *pprefixname,
                                   netwib_buf *prdata);
netwib_err netwox_dnsrdata_decode_a6(netwib_constbuf *prdata,
                                     netwib_uint32 *pprefixlen,
                                     netwib_ip *paddress,
                                     netwib_buf *pprefixname);

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_initcomp_ns(netwib_constbuf *pfullpkt,
                                       netwib_constbuf *pnsdname,
                                       netwib_buf *prdata);
netwib_err netwox_dnsrdata_uncdecode_ns(netwib_constbuf *pfullpkt,
                                        netwib_constbuf *prdata,
                                        netwib_buf *pnsdname);
#define netwox_dnsrdata_init_ns(pnsdname,prdata) netwox_dnsrdata_initcomp_ns(NULL,pnsdname,prdata)
#define netwox_dnsrdata_decode_ns(prdata,pnsdname) netwox_dnsrdata_uncdecode_ns(NULL,prdata,pnsdname)

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_initcomp_cname(netwib_constbuf *pfullpkt,
                                          netwib_constbuf *pcname,
                                          netwib_buf *prdata);
netwib_err netwox_dnsrdata_uncdecode_cname(netwib_constbuf *pfullpkt,
                                           netwib_constbuf *prdata,
                                           netwib_buf *pcname);
#define netwox_dnsrdata_init_cname(pcname,prdata) netwox_dnsrdata_initcomp_cname(NULL,pcname,prdata)
#define netwox_dnsrdata_decode_cname(prdata,pcname) netwox_dnsrdata_uncdecode_cname(NULL,prdata,pcname)

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_initcomp_soa(netwib_constbuf *pfullpkt,
                                        netwib_constbuf *pdomainname,
                                        netwib_constbuf *pemail,
                                        netwib_uint32 serial,
                                        netwib_uint32 refresh,
                                        netwib_uint32 retry,
                                        netwib_uint32 expire,
                                        netwib_uint32 minimum,
                                        netwib_buf *prdata);
netwib_err netwox_dnsrdata_uncdecode_soa(netwib_constbuf *pfullpkt,
                                         netwib_constbuf *prdata,
                                         netwib_buf *pdomainname,
                                         netwib_buf *pemail,
                                         netwib_uint32 *pserial,
                                         netwib_uint32 *prefresh,
                                         netwib_uint32 *pretry,
                                         netwib_uint32 *pexpire,
                                         netwib_uint32 *pminimum);
#define netwox_dnsrdata_init_soa(pdomainname,pemail,serial,refresh,retry,expire,minimum,prdata) netwox_dnsrdata_initcomp_soa(NULL,pdomainname,pemail,serial,refresh,retry,expire,minimum,prdata)
#define netwox_dnsrdata_decode_soa(prdata,pdomainname,pemail,pserial,prefresh,pretry,pexpire,pminimum) netwox_dnsrdata_uncdecode_soa(NULL,prdata,pdomainname,pemail,pserial,prefresh,pretry,pexpire,pminimum)

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_initcomp_ptr(netwib_constbuf *pfullpkt,
                                        netwib_constbuf *pptrdname,
                                        netwib_buf *prdata);
netwib_err netwox_dnsrdata_uncdecode_ptr(netwib_constbuf *pfullpkt,
                                         netwib_constbuf *prdata,
                                         netwib_buf *pptrdname);
#define netwox_dnsrdata_init_ptr(pptrdname,prdata) netwox_dnsrdata_initcomp_ptr(NULL,pptrdname,prdata)
#define netwox_dnsrdata_decode_ptr(prdata,pptrdname) netwox_dnsrdata_uncdecode_ptr(NULL,prdata,pptrdname)

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_init_hinfo(netwib_constbuf *pcpu,
                                      netwib_constbuf *pos,
                                      netwib_buf *prdata);
netwib_err netwox_dnsrdata_decode_hinfo(netwib_constbuf *prdata,
                                        netwib_buf *pcpu,
                                        netwib_buf *pos);

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_initcomp_mx(netwib_constbuf *pfullpkt,
                                       netwib_uint16 preference,
                                       netwib_constbuf *pexchange,
                                       netwib_buf *prdata);
netwib_err netwox_dnsrdata_uncdecode_mx(netwib_constbuf *pfullpkt,
                                        netwib_constbuf *prdata,
                                        netwib_uint16 *ppreference,
                                        netwib_buf *pexchange);
#define netwox_dnsrdata_init_mx(preference,pexchange,prdata) netwox_dnsrdata_initcomp_mx(NULL,preference,pexchange,prdata)
#define netwox_dnsrdata_decode_mx(prdata,ppreference,pexchange) netwox_dnsrdata_uncdecode_mx(NULL,prdata,ppreference,pexchange)

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrdata_init_txt(netwib_constbuf *ptext,
                                    netwib_buf *prdata);
netwib_err netwox_dnsrdata_decode_txt(netwib_constbuf *prdata,
                                      netwib_buf *ptext);

/*-------------------------------------------------------------*/
/* convert from compressed packet representation to user space rdata */
netwib_err netwox_dnsrdata_init_pkt(netwib_bufpool *pbufpool,
                                    netwox_dnsrr_type type,
                                    netwib_constbuf *pfullpkt,
                                    netwib_constbuf *ppkt,
                                    netwib_buf *prdata);
/* convert from user space rdata to compressed packet representation */
netwib_err netwox_dnsrdata_decode_pkt(netwib_bufpool *pbufpool,
                                      netwox_dnsrr_type type,
                                      netwib_constbuf *pfullpkt,
                                      netwib_constbuf *prdata,
                                      netwib_buf *ppkt);

/*-------------------------------------------------------------*/
/* create a readable representation of rdata */
netwib_err netwox_buf_append_dnsrdata(netwib_bufpool *pbufpool,
                                      netwox_dnsrr_type type,
                                      netwib_constbuf *prdata,
                                      netwib_buf *pbuf);



