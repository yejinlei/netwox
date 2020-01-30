
/*-------------------------------------------------------------*/
typedef struct {
  netwib_array id;
  netwib_bufpool *pbufpool;
} netwox_asn1oid;
typedef const netwox_asn1oid netwox_constasn1oid;

/*-------------------------------------------------------------*/
netwib_err netwox_asn1oid_init(netwib_bufpool *pbufpool,
                               netwox_asn1oid *pasn1oid);
netwib_err netwox_asn1oid_close(netwox_asn1oid *pasn1oid);

/*-------------------------------------------------------------*/
netwib_err netwox_asn1oid_add_id(netwox_asn1oid *pasn1oid,
                                 netwib_uint32 subid);

/*-------------------------------------------------------------*/
/* add subids of a buffer containing "1.2.3.4..." */
netwib_err netwox_buf_decode_asn1oid(netwib_constbuf *pbuf,
                                     netwox_asn1oid *pasn1oid);

/*-------------------------------------------------------------*/
netwib_err netwox_buf_append_asn1oid(netwox_constasn1oid *pasn1oid,
                                     netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* netwox_asn1oid_init must be called before on pasn1oiddst */
netwib_err netwox_asn1oid_dup(netwox_constasn1oid *pasn1oidsrc,
                              netwox_asn1oid *pasn1oiddst);

/*-------------------------------------------------------------*/
netwib_err netwox_asn1oid_cmp(netwox_constasn1oid *pasn1oid1,
                              netwox_constasn1oid *pasn1oid2,
                              netwib_cmp *pcmp);

/*-------------------------------------------------------------*/
netwib_err netwox_asn1oid_init_arg(netwib_bufpool *pbufpool,
                                   netwox_arg *parg,
                                   netwib_char key,
                                   netwox_asn1oid *pasn1oid);
