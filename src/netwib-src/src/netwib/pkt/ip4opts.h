
/*-------------------------------------------------------------*/
/***************************************************************
 * A "netwib_ip4opts" is a netwib_buf storing 0, 1 or more     *
 * netwib_ip4opt. There is no typedef for netwib_ip4opts.      *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* maximum length of a buffer containing a "netwib_ip4opts" */
#define NETWIB_IP4OPTS_MAXLEN 40

/*-------------------------------------------------------------*/
/***************************************************************
 * To create a "netwib_ip4opts", loop using function           *
 * netwib_pkt_append_ip4opt.                                   *
 * To decode a "netwib_ip4opts", loop using function           *
 * netwib_pkt_decode_ip4opt.                                   *
 ***************************************************************/
