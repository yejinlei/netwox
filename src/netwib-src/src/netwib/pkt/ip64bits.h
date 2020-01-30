
/*-------------------------------------------------------------*/
/* Name : netwib_ip64bits_init_ippkt
   Description :
     Initialize a buffer containing an IP header and
     64 bits of data.
     This kind of packets are needed for errors (cf. rfc 792).
   Input parameter(s) :
     *piphdr : IP header
     *pipdata : IP data
     *phdrudp : udp header
     *phdrtcp : tcp header
   Input/output parameter(s) :
   Output parameter(s) :
     *pip64bits : buffer containing an IP header and
                  64 bits of data.
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ip64bits_init_ippkt(netwib_constbuf *pippkt,
                                      netwib_bufext *pip64bits);

/*-------------------------------------------------------------*/
/* Name : netwib_ip64bits_decode_ipxyz
   Description :
     Try to decode ip packet contained in ip64bits.
   Input parameter(s) :
     *pip64bits : ip header and at least 64 bits of ip data
   Input/output parameter(s) :
   Output parameter(s) :
     *piphdr : decoded IP header
     *pipdata : buffer containing ip data
     *pudpsrc : udp source port
     *pudpdst : udp destination port
     *pudplen : udp len
     *pudpcheck : udp checksum
     *ptcpsrc : tcp source port
     *ptcpdst : tcp destination port
     *pseqnum : tcp sequence number
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_NOTCONVERTED : packet not decoded
     NETWIB_ERR_DATAMISSING : pkt was not big enough
*/
netwib_err netwib_ip64bits_decode_ipudp(netwib_constbuf *pip64bits,
                                        netwib_iphdr *piphdr,
                                        netwib_port *pudpsrc,
                                        netwib_port *pudpdst);
netwib_err netwib_ip64bits_decode_iptcp(netwib_constbuf *pip64bits,
                                        netwib_iphdr *piphdr,
                                        netwib_port *ptcpsrc,
                                        netwib_port *ptcpdst);
#define netwib_ip64bits_decode_ipdata(pip64bits,piphdr,pipdata) netwib_pkt_decode_ipdata(pip64bits,piphdr,pipdata)

