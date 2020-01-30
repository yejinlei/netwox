
/*-------------------------------------------------------------*/
typedef struct {
  netwib_io *pspoofio;
  netwib_io *psniffio;
  netwib_iptype iptype;
  netwib_icmp4type type4;
  netwib_icmp6type type6;
  netwib_ip srcip; /* who sends the error */
  netwib_icmp4code code4; /* code to send */
  netwib_icmp6code code6; /* code to send */
  netwib_uint32 pointer;  /* pointer to send */
  netwib_ip gw;           /* gw to send */
} netwox_icmperr_info;

/*-------------------------------------------------------------*/
netwib_err netwox_icmperr_loop(netwox_icmperr_info *pinfo);
