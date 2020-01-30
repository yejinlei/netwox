
/*-------------------------------------------------------------*/
/***************************************************************
 * A netwib_eth represents an Ethernet address.                *
 * You can init it with a netwib_eth_init_xyz function or with:*
 *   netwib_eth eth, eth2;                                     *
 *   eth.b[0] = 0x01; eth.b[1] = 0x02; eth.b[2] = 0x03; ...    *
 *   netwib_c_memcpy(eth.b, array, NETWIB_ETH_LEN);            *
 *   netwib_c_memset(eth.b, 0, NETWIB_ETH_LEN);                *
 *   eth2 = eth1;                                              *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* Name : netwib_eth_init_fields
   Description :
     Initialize an netwib_eth address.
   Input parameter(s) :
     a,b,c,d,e,f : bytes of the Ethernet address
   Input/output parameter(s) :
   Output parameter(s) :
     *peth : netwib_eth set with a:b:c:d:e:f
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_eth_init_fields(netwib_byte a,
                                  netwib_byte b,
                                  netwib_byte c,
                                  netwib_byte d,
                                  netwib_byte e,
                                  netwib_byte f,
                                  netwib_eth *peth);

/*-------------------------------------------------------------*/
/* Name : netwib_eth_init_buf
   Description :
     Convert the string "aa:bb:cc:dd:ee:ff" to a netwib_eth.
   Input parameter(s) :
     *pbuf : string
   Input/output parameter(s) :
   Output parameter(s) :
     *peth : netwib_eth set
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_NOTCONVERTED : format is not "aa:bb:cc:dd:ee:ff"
*/
netwib_err netwib_eth_init_buf(netwib_constbuf *pbuf,
                               netwib_eth *peth);

/*-------------------------------------------------------------*/
/* Name : netwib_eth_init_ip
   Description :
     Convert a netwib_ip address to a netwib_eth.
   Input parameter(s) :
     ip : IP address
   Input/output parameter(s) :
   Output parameter(s) :
     *peth : netwib_eth set with ip
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_OKUNRESOLVED : the address could not be resolved
*/
netwib_err netwib_eth_init_ip(netwib_constip *pip,
                              netwib_eth *peth);

/*-------------------------------------------------------------*/
/* Name : netwib_eth_init_kbd
   Description :
     Initialize an netwib_eth with the address entered by the
     user through the keyboard
   Input parameter(s) :
     message : message to print before
     *pdefaulteth : default eth to use if user enters nothing
                    if NULL, there is no default
   Input/output parameter(s) :
   Output parameter(s) :
     *peth : netwib_eth set with user's string
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_eth_init_kbd(netwib_constbuf *pmessage,
                               netwib_consteth *pdefaulteth,
                               netwib_eth *peth);
#define NETWIB_ETH_INIT_KBD_NODEF NULL

/*-------------------------------------------------------------*/
/* Name : netwib_eth_cmp
   Description :
     Compare two netwib_eth.
   Input parameter(s) :
     eth1 : netwib_eth to compare with eth2
     eth2 : netwib_eth to compare with eth1
   Input/output parameter(s) :
   Output parameter(s) :
     *pcmp :
       NETWIB_CMP_LT : eth1<eth2
       NETWIB_CMP_EQ : if eth1 and eth2 are equal
       NETWIB_CMP_GT : eth1>eth2
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_eth_cmp(netwib_consteth *peth1,
                          netwib_consteth *peth2,
                          netwib_cmp *pcmp);

/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_eth
   Description :
     Append a string representing a netwib_eth.
   Input parameter(s) :
     eth : netwib_eth to append
   Input/output parameter(s) :
     pbuf : buffer updated
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_append_eth(netwib_consteth *peth,
                                 netwib_buf *pbuf);

