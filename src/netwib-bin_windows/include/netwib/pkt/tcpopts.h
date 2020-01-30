
/*-------------------------------------------------------------*/
/***************************************************************
 * A "netwib_tcpopts" is a netwib_buf storing 0, 1 or more     *
 * netwib_tcpopt. There is no typedef for netwib_tcpopts.      *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* maximum length of a buffer containing a "netwib_tcpopts" */
#define NETWIB_TCPOPTS_MAXLEN 40

/*-------------------------------------------------------------*/
/***************************************************************
 * To create a "netwib_tcpopts", loop using function           *
 * netwib_pkt_append_tcpopt.                                   *
 * To decode a "netwib_tcpopts", loop using function           *
 * netwib_pkt_decode_tcpopt.                                   *
 ***************************************************************/
