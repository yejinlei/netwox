
/*-------------------------------------------------------------*/
#define NETWOX_DESC_sniff_devfilter "Parameter --device indicates on which device to sniff. Please note", "that under some systems, such as Windows, sniffing on some devices is", "not supported.", "Parameter --filter defines the sniff filter. It permits to restrict", "captured packets. This kind of filter is named a BPF or pcap filter.", "Basic elements of a filter are:", "  host 1.2.3.4", "  net 192.168.10", "  net 192.168.10.0 mask 255.255.255.0", "  net 192.168.10.0/24", "  port 21", "  dst host 1.2.3.4", "  src port 2345", "  ether host a:b:c:d:e:f ('ether a:b:c:d:e:f' is not working)", "  ether src aa:bb:cc:dd:ee:ff", "  ip", "  arp", "  rarp", "  tcp", "  icmp", "  udp", "Here are filter examples:", "  \"host 1.2.3.4\"", "  \"net 192.168 and icmp\"", "  \"host 1.2.3.4 or dst port 80\"", "  \"(udp or tcp) and not host 1.2.3.4\""

#define NETWOX_DESC_sniff_encode "Parameter --hdrencode and --dataencode defines how to display header", "and data/payload. Common useful values are: array, dump, synth,", "nothing, text. Full list is available through netwag or running tool", "12."

#define NETWOX_DESC_sniff_rawip "Parameter --rawip indicates to ignore Ethernet/link layer, and start", "displaying at IP header."

#define NETWOX_DESC_sniff_extended "Parameter --extended indicates to try to decode other protocols such", "as DNS or DHCP."

#define NETWOX_DESC_sniff_ipreas "Parameter --ipreas tries to reassemble IP packets. This might miss", "packets."

#define NETWOX_DESC_sniff_tcpreord "Parameter --tcpreord tries to reorder TCP flow (seqnum increments).", "This might miss packets."

/*-------------------------------------------------------------*/
#define NETWOX_DESC_spoofip "Parameter --spoofip indicates how to generate link layer for spoofing.", "Values 'best', 'link' or 'raw' are common choices for --spoofip. Here", "is the list of accepted values:", " - 'raw' means to spoof at IP4/IP6 level (it uses system IP stack). If", "   a firewall is installed, or on some systems, this might not work.", " - 'linkf' means to spoof at link level (currently, only Ethernet is", "   supported). The 'f' means to Fill source Ethernet address.", "   However, if source IP address is spoofed, it might be impossible", "   to Fill it. So, linkf will not work: use linkb or linkfb instead.", " - 'linkb' means to spoof at link level. The 'b' means to left a Blank", "   source Ethernet address (0:0:0:0:0:0, do not try to Fill it).", " - 'linkfb' means to spoof at link level. The 'f' means to try to Fill", "   source Ethernet address, but if it is not possible, it is left", "   Blank.", " - 'rawlinkf' means to try 'raw', then try 'linkf'", " - 'rawlinkb' means to try 'raw', then try 'linkb'", " - 'rawlinkfb' means to try 'raw', then try 'linkfb'", " - 'linkfraw' means to try 'linkf', then try 'raw'", " - 'linkbraw' means to try 'linkb', then try 'raw'", " - 'linkfbraw' means to try 'linkfb', then try 'raw'", " - 'link' is an alias for 'linkfb'", " - 'rawlink' is an alias for 'rawlinkfb'", " - 'linkraw' is an alias for 'linkfbraw'", " - 'best' is an alias for 'linkraw'. It should work in all cases."

/*-------------------------------------------------------------*/
#define NETWOX_DESC_record "A record is a capture file. It contains several packets captured", "during a sniff. It can also be created by hand. There are 7 formats", "for records: pcap (tcpdump compatible), bin (binary, unreadable by", "humans but fast) and mixed/mixed_wrap/dump/hexa/hexa_wrap (easy to", "read and edit). A record also has an associated DLT (Data Link Type),", "indicating at which level a packet start: raw (start at IP header) and", "ether (start at Ethernet header) are the 2 most common DLT. Tool 13", "displays DLT of each device."
#define NETWOX_DESC_record_file "Parameter --file indicates the record filename."
#define NETWOX_DESC_record_srcfile "Parameter --src-file indicates the input record filename."
#define NETWOX_DESC_record_dstfile "Parameter --dst-file indicates the output record filename."

#define NETWOX_DESC_record_dlt "Parameter --dlt defines the DLT of packets in record (it depends on", "how it was sniffed, generally 'ether' (for Ethernet) or 'raw' (if", "packet starts at IP header)). Full list is available through netwag or", "running tool 12."
#define NETWOX_DESC_record_indlt "Parameter --input-dlt defines the DLT of packets in input record (it", "depends on how it was sniffed, generally 'ether' (for Ethernet) or", "'raw' (if packet starts at IP header)). Full list is available through", "netwag or running tool 12."
#define NETWOX_DESC_record_outdlt "Parameter --output-dlt defines the DLT of packets to write in output", "record."

#define NETWOX_DESC_record_encode "Parameter --recordencode defines how to encode data in this record", "(suggested values: bin, pcap and mixed_wrap)."

/*-------------------------------------------------------------*/
#define NETWOX_DESC_spoof_packet "This tool sends a fake packet on the network.", "Each parameter name should be self explaining."

#define NETWOX_DESC_spoof_sample "This tool sends hardcoded packet samples. Samples are (--sample", "defines number): 1=udp_syslog, 2=tcp_syn, 3=tcpsynack, 4=tcpack,", "5=ping."
#define NETWOX_DESC_spoof_sampleopt "Packets contain IP4 options and can be fragmented before been sent."

/*-------------------------------------------------------------*/
#define NETWOX_DESC_ping "This tool checks if a host is up. It's done sending a probe packet and", "expecting a reply."
#define NETWOX_DESC_traceroute "This tool lists routers on the path to a computer.", "How traceroute works: IP header contains a field named TTL indicating", "the number of hops a packet can cross. Each router decreases TTL. When", "it reaches 0, the router sends an ICMP Time Exceeded error back to the", "IP source address. Traceroute works by slowly increasing TTL. So, we", "obtain the list of successive routers because each one sends an ICMP", "Time Exceeded."
#define NETWOX_DESC_scan_host "This tool checks if several hosts are up."
#define NETWOX_DESC_scan_udp "This tool checks if several UDP ports are open on host."
#define NETWOX_DESC_scan_tcp "This tool checks if several TCP ports are open on host."
#define NETWOX_DESC_pts_ether "This tool also permits to use fake Ethernet and IP addresses."
#define NETWOX_DESC_pts_icmp "This tool sends an ICMP Echo Request to a computer. If host is up and", "permitting ping, it will send back an ICMP Echo Reply."
#define NETWOX_DESC_pts_tcp "This tool sends a TCP SYN to a computer. If host permits TCP, it will", "send back a TCP SYN-ACK (if port is open), or a TCP RST (if port is", "closed)."
#define NETWOX_DESC_pts_udp "This tool sends an UDP packet to a computer. If host permits UDP, it", "will send back an ICMP error (if port is closed), or send back nothing", "(if port is open). So, it's important to understand UDP port has to be", "closed for computer to return something."
#define NETWOX_DESC_pts_arp "This tool sends an ARP request. If host is up, it will send back an", "ARP Reply."
#define NETWOX_DESC_pts_icmp6nd "This tool sends an ICMP6 Neighbor Discovery request. If host is up, it", "will send back a Neighbor Discovery Reply."
#define NETWOX_DESC_pts_proto "This tool sends an IP only packet. There is no way to detect end (it", "depends on protocol), so user has to interrupt tool at end."
#define NETWOX_DESC_tracertdisc "This tool sends traceroute queries to a remote network, and creates", "a graph of remote computers. It traceroutes common TCP/UDP ports,", "and ICMP. Setting min-ttl and max-ttl correctly will speed up", "the scan."

/*-------------------------------------------------------------*/
#define NETWOX_DESC_icmp_error "This tool sends an ICMP error message when a packet matching filter", "is sniffed."

/*-------------------------------------------------------------*/
#define NETWOX_DESC_client "Parameter --dst-ip is the address of remote host/server.", "Parameter --dst-port is the port number where to connect.", "Optional parameter --src-ip defines the local IP address to use.", "Optional parameter --src-port defines the local port number to use.", "Optional parameter --ip4opts permits to add IP4 options encoded as", "mixed.", "Optional parameter --ip6exts permits to add IP6 options encoded as", "mixed (first byte is the next header number).", "Optional parameters --device, --src-eth and --dst-eth permits to", "create a virtual client using fake addresses (in this case, tool have", "to be run with admin privilege in order to sniff and spoof at", "Ethernet level)."
#define NETWOX_DESC_server_common "Parameter --src-port defines the local port number to listen on.", "Optional parameter --src-ip defines the local IP address to use.", "Optional parameter --iptype defines which kind of IP address to use", "when --src-ip is unset (accepted values: ip4 and ip6).", "Optional parameter --ip4opts permits to add IP4 options encoded as", "mixed.", "Optional parameter --ip6exts permits to add IP6 options encoded as", "mixed (first byte is the next header number)."
#define NETWOX_DESC_server NETWOX_DESC_server_common, "Optional parameters --device and --src-eth permits to create", "a virtual server using fake addresses (in this case, tool have to be", "run with admin privilege in order to sniff and spoof at Ethernet", "level)."
#define NETWOX_DESC_servertcpmul NETWOX_DESC_server_common, "Optional parameters --device and --src-eth are not yet implemented."

/*-------------------------------------------------------------*/
#define NETWOX_DESC_mulser "This tool listens for several clients connections. It can show", "received data to screen (--showscreen), and echo it back to", "client (--echoback)."

/*-------------------------------------------------------------*/
#define NETWOX_DESC_cliser_encdec "Parameter --encode defines how to display data (common values: data,", "dump, mixed).", "Parameter --decode defines how keyboard data is entered (common", "values: data, mixed)."

/*-------------------------------------------------------------*/
#define NETWOX_DESC_remadm_passcyp "Parameter --password defines the password (same as server's password).", "Parameter --cypher defines if data has to be encrypted. Please note", "this is not very strong. If known text is used, this algorithm can be", "cracked."

/*-------------------------------------------------------------*/
#define NETWOX_DESC_dns_tcp "Parameter --tcp uses TCP instead of UDP. This is generally not", "allowed."
#define NETWOX_DESC_dns_hostns "Parameter --hostname defines the hostname to tell (www.example.com).", "Parameter --hostnameip defines IP address of this hostname (1.2.3.4).", "Parameter --authns is the authoritative name server (ns.example.com).", "Parameter --authnsip defines IP address of name server (1.2.3.5).", "Parameter --ttl defines the Time To Live of this reply."

/*-------------------------------------------------------------*/
#define NETWOX_DESC_email "Parameter --from sets From header.", "Parameter --from-name sets the name of user in From header.", "Parameter --subject sets Subject header", "Parameter --file-body is the file containing the body of message.", "Parameter --file-att is the file containing the attachment."

/*-------------------------------------------------------------*/
#define NETWOX_DESC_userpass "Parameters --user and --pass defines the username and password."
#define NETWOX_DESC_loginpassword "Parameters --login and --password defines needed login and password."
#define NETWOX_DESC_allowedclients "Parameter --allowed-clients lists the IP addresses or hosts allowed", "to connect."
#define NETWOX_DESC_rootdir "Parameter --rootdir defines the root directory for put and get file", "operations (cannot exit this root jail)."

/*-------------------------------------------------------------*/
#define NETWOX_DESC_newsgroup "Parameter --newsgroup sets the newsgroup name."

/*-------------------------------------------------------------*/
#define NETWOX_DESC_ftp_passive "FTP protocol can use Active or Passive transfers. Parameter --passive", "activates a passive transfer (by default, it's active)."
/*-------------------------------------------------------------*/
#define NETWOX_DESC_http_uri "Parameter --uri is the uri/url requested."
#define NETWOX_DESC_http_status "Parameter --display-status indicates to display status code of reply", "to screen."
#define NETWOX_DESC_http_headers "Parameter --display-headers indicates to display reply headers.", "Parameter --file-headers indicates the filename where to store", "headers."
#define NETWOX_DESC_http_body "Parameter --display-body indicates to display reply body.", "Parameter --file-body indicates the filename where to store body."
#define NETWOX_DESC_http_proxy "Parameters --proxy-ip, --proxy-port, --proxy-login and", "--proxy-password defines the web proxy and credentials."
#define NETWOX_DESC_http_useragent "Parameter --user-agent defines the client name to send. For example", "'Mozilla/5.0'."

/*-------------------------------------------------------------*/
#define NETWOX_DESC_brute_title "This tool successively tries couples of login and passwords in order", "to find matching authentication credentials."
#define NETWOX_DESC_brute "Parameter --login defines the logins to try. To check several logins,", "parameter --login-file is the file containing logins (one per line).", "Parameter --password-file is the file containing passwords (1/line).", "Parameter --stopatfirst indicates to stop when first match is found.", "Parameter --numthread defines the number of threads (simultaneous", "running checks) to use."

/*-------------------------------------------------------------*/
#define NETWOX_DESC_spider "This tool spiders a HTTP or FTP url. 'Spidering' an url means to", "download the url, read the new urls it contains, download them, and so", "on recursively."

/*-------------------------------------------------------------*/
#define NETWOX_DESC_perf_chunkdur "Parameter --chunksize changes the size of data used for each step.", "Parameter --duration defines the duration of test in milliseconds."

/*-------------------------------------------------------------*/
#define NETWOX_DESC_snmp_common "Parameter --version defines SNMP version to use (from 1 to 3).", "Versions 1 and 2 need --community parameter to set community name.", "Version 3 requires --username and --password. Parameter --authmd5", "indicates if md5 authentication is used. Parameters", "--authoritativeengineid, --authoritativeengineboots,", "--authoritativeenginetime, --contextengineid and --contextname are", "optional version 3 parameters.", "Parameter --display shows packets."
#define NETWOX_DESC_snmp_oid "Parameter --oid is the oid to query. An oid is for example", "'.1.3.6.1.2.1.1.4.0'."

/*-------------------------------------------------------------*/
#define NETWOX_DESC_check_md5 "Parameter --md5 is the expected md5sum."
#define NETWOX_DESC_check_work "It ensures the server is working properly."

/*-------------------------------------------------------------*/
#define NETWOX_DESC_relay "This tool is a relay: a client will connect to it, then it will", "connect to server. All data received from server is sent back to", "client. All data received from client is sent to server. From server's", "view point, it only sees relay as source IP address. From client's", "viewpoint, it only sees relay as destination IP address."

#define NETWOX_DESC_relay_para "Parameter --server-ip is the address of remote host/server.", "Parameter --server-port is the port number where to connect.", NETWOX_DESC_allowedclients

/*-------------------------------------------------------------*/
#define NETWOX_DESC_toolpriv_port1024 "", "This tool may need to be run with admin privilege in order to listen", "on a small port number (port<1024)."
#define NETWOX_DESC_toolpriv_sniff "", "This tool may need to be run with admin privilege in order to sniff."
#define NETWOX_DESC_toolpriv_spoof "", "This tool may need to be run with admin privilege in order to spoof."
#define NETWOX_DESC_toolpriv_snsp "", "This tool may need to be run with admin privilege in order to sniff", "and spoof."
#define NETWOX_DESC_toolpriv_snsp_resolveth "", "This tool may need to be run with admin privilege in order to sniff", "and spoof (for Ethernet addresses resolving)."
#define NETWOX_DESC_toolpriv_conf "", "This tool may need to be run with admin privilege in order to obtain", "full network configuration."
#define NETWOX_DESC_toolpriv_none NULL

/*-------------------------------------------------------------*/
#define NETWOX_DESC_smbcli_common "Parameter --user indicates the optional username.", "Parameter --password indicates the optional password.", "Parameter --authversion defines the authentication protocol to use", "(0=Lanman, 1=NTLMv1, 2=NTLMv2, unset=guess(try NTLMv2, then NTLMv1,", "and finally Lanman)).", "Parameter --netbiosname defines the NetBIOS name of server. It is", "generally the same as its DNS name. Most of the time, it is not", "needed.", "Parameter --verbose displays activity (set it if a problem occurs).", "Parameter --debug displays packets to screen."
#define NETWOX_DESC_smbcli_ipc NETWOX_DESC_smbcli_common
#define NETWOX_DESC_smbcli_share "Parameter --share indicates the share name.", NETWOX_DESC_smbcli_common, "Example, to connect on //server/myshare/, use:",  "--dst-ip myserver --share myshare --netbiosname myserver --user bob", "  --password hello"
