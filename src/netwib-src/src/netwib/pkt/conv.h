
/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_PKT_NEWFIELD_BLANK = 1, /* do not try to compute field */
  NETWIB_PKT_NEWFIELD_FILL,      /* compute field */
  NETWIB_PKT_NEWFIELD_FILLBLANK, /* if can't compute left blank */
  NETWIB_PKT_NEWFIELD_BEST = NETWIB_PKT_NEWFIELD_FILLBLANK
} netwib_pkt_newfield;

/*-------------------------------------------------------------*/
/* Name : netwib_pkt_convert
   Description :
     Convert a packet from one type to another
   Input parameter(s) :
     dlttype : DLT type of input packet
     *ppktin : buffer containing physical input packet
     newfieldopt : type to use for optional fields
     newfieldreq : type to use for mandatory fields
     dlttypeout : wanted outtype
   Input/output parameter(s) :
     *plinkhdrout : new link header
     *ppktout : packet to put after the link header
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_pkt_convert(netwib_device_dlttype dlttypein,
                              netwib_constbuf *ppktin,
                              netwib_pkt_newfield newfieldopt,
                              netwib_pkt_newfield newfieldreq,
                              netwib_device_dlttype dlttypeout,
                              netwib_linkhdr *plinkhdrout,
                              netwib_bufext *ppktout);

