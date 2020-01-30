
/*-------------------------------------------------------------*/
/***************************************************************
 * Netwib supports Ethernet cards(netwib_device_hwtype=ETHER), *
 * Loopback(LOOPBACK) and Serial Modems(PPP).                  *
 * Unfortunately, these are not supported on every system as   *
 * described in this table.                                    *
 *                                                             *
 *            Device_hwtype       Supported?  Returned_DLT     *
 *  FreeBSD :                                                  *
 *            ETHER               yes         ETHER            *
 *            LOOPBACK            yes         NULL             *
 *            PPP                 yes         ?[7]             *
 *  Linux :                                                    *
 *            ETHER               yes         ETHER            *
 *            LOOPBACK            yes         NULL             *
 *            PPP                 yes         ?[7]             *
 *  OpenBSD :                                                  *
 *            ETHER               yes         ETHER            *
 *            LOOPBACK            yes         LOOP             *
 *            PPP                 yes         ?[7]             *
 *  Solaris :                                                  *
 *            ETHER               yes         ETHER            *
 *            LOOPBACK            no          -                *
 *            PPP                 yes[1]      ?[7]             *
 *  Windows 95 :                                               *
 *            ETHER               yes         ETHER            *
 *            LOOPBACK            no          -                *
 *            PPP                 yes[1][2]   ETHER            *
 *  Windows 98 :                                               *
 *            ETHER               yes         ETHER            *
 *            LOOPBACK            no          -                *
 *            PPP                 yes[1]      ETHER            *
 *  Windows Me :                                               *
 *            ETHER               yes         ETHER            *
 *            LOOPBACK            no          -                *
 *            PPP                 ?[3]        ETHER            *
 *  Windows NT4 :                                              *
 *            ETHER               yes         ETHER            *
 *            LOOPBACK            no          -                *
 *            PPP                 y/n[1][4]   ETHER            *
 *  Windows 2000 :                                             *
 *            ETHER               yes         ETHER            *
 *            LOOPBACK            no          -                *
 *            PPP                 no[5]       ETHER            *
 *  Windows XP :                                               *
 *            ETHER               yes         ETHER            *
 *            LOOPBACK            no          -                *
 *            PPP                 no[6]       -                *
 *                                                             *
 * Notes :                                                     *
 *  1 : System adds a fake Ethernet header                     *
 *  2 : Windows Sockets 2 and Dialup Networking 1.3 have to    *
 *      be installed (urls are in problemusagewindows_en.txt)  *
 *  3 : Not tested                                             *
 *  4 : As described in WinPCAP FAQ, it doesn't work (but, it  *
 *      works on my computer)                                  *
 *  5 : As described in WinPCAP FAQ, it doesn't work (on my    *
 *      computer, when packets are sniffed, we can't spoof)    *
 *  6 : As described in WinPCAP FAQ, it doesn't work (a        *
 *      contributor confirmed that it doesn't work on its      *
 *      computer)                                              *
 *  7 : I'll summarize this info later...                      *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* Name : netwib_io_init_sniff
   Description :
     Open a sniff session.
     Returned packets will start at Link header.
   Input parameter(s) :
     *pdevice : device where we sniff
     *pfilter : sniff filter (NULL to sniff everything)
   Input/output parameter(s) :
   Output parameter(s) :
     **ppio : io created
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_io_init_sniff(netwib_constbuf *pdevice,
                                netwib_constbuf *pfilter,
                                netwib_io **ppio);

/*-------------------------------------------------------------*/
/***************************************************************
 * Note about sniff filters :                                  *
 * Filters permit to select network packets we sniff.          *
 * Basic elements of a bpf filter are :                        *
 *    host 1.2.3.4                                             *
 *    net 192.168.10                                           *
 *    net 192.168.10.0 mask 255.255.255.0                      *
 *    net 192.168.10.0/24                                      *
 *    port 21                                                  *
 *    dst host 1.2.3.4                                         *
 *    src port 2345                                            *
 *    ether host a:b:c:d:e:f ("ether a:b:c:d:e:f" is not working)
 *    ether src aa:bb:cc:dd:ee:ff                              *
 *    ip                                                       *
 *    arp                                                      *
 *    rarp                                                     *
 *    tcp                                                      *
 *    icmp                                                     *
 *    udp                                                      *
 * Here are filter examples :                                  *
 *    host 1.2.3.4                                             *
 *    net 192.168 and icmp                                     *
 *    host 1.2.3.4 or dst port 80                              *
 *    (udp or tcp) and not host 1.2.3.4                        *
 ***************************************************************/

/* change the filter */
/* netwib_err f(netwib_io *pio, netwib_constbuf *pfilter); */
#define netwib_sniff_ctl_set_filter(pio,pfilter) netwib_io_ctl_set(pio,NETWIB_IO_WAYTYPE_SUPPORTED,NETWIB_IO_CTLTYPE_SNIFF_FILTER,pfilter,0)
/* netwib_err f(netwib_io *pio, netwib_buf *pfilter); */
#define netwib_sniff_ctl_get_filter(pio,pfilter) netwib_io_ctl_get(pio,NETWIB_IO_WAYTYPE_SUPPORTED,NETWIB_IO_CTLTYPE_SNIFF_FILTER,pfilter,NULL)

/*-------------------------------------------------------------*/
/* obtain the DLT of returned packets */
/* netwib_err f(netwib_io *pio, netwib_device_dlttype *pdlt); */
#define netwib_sniff_ctl_get_dlt(pio,pdlt) netwib_io_ctl_get(pio,NETWIB_IO_WAYTYPE_SUPPORTED,NETWIB_IO_CTLTYPE_SNIFF_DLT,NULL,(netwib_uint32*)pdlt)

/*-------------------------------------------------------------*/
/***************************************************************
 * This io link :                                              *
 *  - has to be plugged on netwib_io_init_sniff                *
 *  - reassemble fragmented IP packets                         *
 *  - do nothing with other packets                            *
 ***************************************************************/
netwib_err netwib_io_init_sniff_ipreas(netwib_io **ppio);

/*-------------------------------------------------------------*/
/***************************************************************
 * This io link :                                              *
 *  - has to be plugged on netwib_io_init_sniff_ipreas         *
 *  - reorder sequences of TCP packets                         *
 *  - remove duplicate TCP packets                             *
 *  - do nothing with other packets                            *
 ***************************************************************/
netwib_err netwib_io_init_sniff_tcpreord(netwib_io **ppio);

/*-------------------------------------------------------------*/
/* Name : netwib_io_init_sniff_ip
   Description :
     Open a sniff session, and return only IP packets.
     Returned packets will start at IP header (DLTTYPE_RAW).
   Input parameter(s) :
     *pdevice : device where we sniff
     *pfilter : sniff filter (NULL to sniff everything)
     ipreas : if netwib_io_init_sniff_ipreas has to
              be used
     tcpreord : if netwib_io_init_sniff_tcpreord has to
                be used
   Input/output parameter(s) :
   Output parameter(s) :
     **ppio : io created
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_io_init_sniff_ip(netwib_constbuf *pdevice,
                                   netwib_constbuf *pfilter,
                                   netwib_bool ipreas,
                                   netwib_bool tcpreord,
                                   netwib_io **ppio);
