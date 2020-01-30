
/*-------------------------------------------------------------*/
/* Only main options are implemented */

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_DHCP4OPTTYPE_UNKNOWN = 256,
  NETWOX_DHCP4OPTTYPE_PAD = 0,
  NETWOX_DHCP4OPTTYPE_END = 255,
  NETWOX_DHCP4OPTTYPE_SUBNETMASK = 1,
  NETWOX_DHCP4OPTTYPE_GATEWAYS = 3,
  NETWOX_DHCP4OPTTYPE_TIMESERVERS = 4,
  NETWOX_DHCP4OPTTYPE_NAMESERVERS = 5,
  NETWOX_DHCP4OPTTYPE_DNSSERVERS = 6,
  NETWOX_DHCP4OPTTYPE_LOGSERVERS = 7,
  NETWOX_DHCP4OPTTYPE_LPRSERVERS = 9,
  NETWOX_DHCP4OPTTYPE_HOSTNAME = 12,
  NETWOX_DHCP4OPTTYPE_DOMAINNAME = 15,
  NETWOX_DHCP4OPTTYPE_BROADCASTAD = 28,
  NETWOX_DHCP4OPTTYPE_PERFORMROUTDISC = 31,
  NETWOX_DHCP4OPTTYPE_STATICROUTES = 33,
  NETWOX_DHCP4OPTTYPE_NISDOMAIN = 40,
  NETWOX_DHCP4OPTTYPE_NISSERVERS = 41,
  NETWOX_DHCP4OPTTYPE_NTPSERVERS = 42,
  NETWOX_DHCP4OPTTYPE_VENDORINFO = 43,
  NETWOX_DHCP4OPTTYPE_NETBIOSNS = 44,
  NETWOX_DHCP4OPTTYPE_NETBIOSDD = 45,
  NETWOX_DHCP4OPTTYPE_NETBIOSNODETYPE = 46,
  NETWOX_DHCP4OPTTYPE_NETBIOSSCOPE = 47,
  NETWOX_DHCP4OPTTYPE_REQUESTEDIPAD = 50,
  NETWOX_DHCP4OPTTYPE_IPADLEASETIME = 51,
  NETWOX_DHCP4OPTTYPE_MSGTYPE = 53,
  NETWOX_DHCP4OPTTYPE_SERVERID = 54,
  NETWOX_DHCP4OPTTYPE_REQLIST = 55,
  NETWOX_DHCP4OPTTYPE_ERRORMSG = 56,
  NETWOX_DHCP4OPTTYPE_RENEWALTIME = 58,
  NETWOX_DHCP4OPTTYPE_REBINDINGTIME = 59,
  NETWOX_DHCP4OPTTYPE_VENDORID = 60,
  NETWOX_DHCP4OPTTYPE_CLIENTID = 61,
  NETWOX_DHCP4OPTTYPE_NISPDOMAIN = 64,
  NETWOX_DHCP4OPTTYPE_NISPSERVERS = 65,
  NETWOX_DHCP4OPTTYPE_TFTPSERVERS = 66,
  NETWOX_DHCP4OPTTYPE_SMTPSERVERS = 69,
  NETWOX_DHCP4OPTTYPE_POP3SERVERS = 70,
  NETWOX_DHCP4OPTTYPE_NNTPSERVERS = 71,
  NETWOX_DHCP4OPTTYPE_WWWSERVERS = 72,
  NETWOX_DHCP4OPTTYPE_IRCSERVERS = 74
} netwox_dhcp4opttype;

/*-------------------------------------------------------------*/
typedef struct {
#define NETWOX_DHCP4OPT_IPARRAY_LEN 10
  netwib_uint32 storedvalues;
  netwib_ip ip[NETWOX_DHCP4OPT_IPARRAY_LEN];
} netwox_dhcp4opt_iparray;

/*-------------------------------------------------------------*/
typedef struct {
#define NETWOX_DHCP4OPT_STATICROUTE_LEN 10
  netwib_uint32 storedvalues;
  netwib_ip dst[NETWOX_DHCP4OPT_STATICROUTE_LEN];
  netwib_ip gw[NETWOX_DHCP4OPT_STATICROUTE_LEN];
} netwox_dhcp4opt_staticroutes;

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_DHCP4OPT_MSGTYPE_DISCOVER = 1,
  NETWOX_DHCP4OPT_MSGTYPE_OFFER = 2,
  NETWOX_DHCP4OPT_MSGTYPE_REQUEST = 3,
  NETWOX_DHCP4OPT_MSGTYPE_DECLINE = 4,
  NETWOX_DHCP4OPT_MSGTYPE_ACK = 5,
  NETWOX_DHCP4OPT_MSGTYPE_NAK = 6,
  NETWOX_DHCP4OPT_MSGTYPE_RELEASE = 7,
  NETWOX_DHCP4OPT_MSGTYPE_INFORM = 8
} netwox_dhcp4opt_msgtype;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint8 type;
  netwib_bufext id;
} netwox_dhcp4opt_clientid;

/*-------------------------------------------------------------*/
typedef struct {
#define NETWOX_DHCP4OPT_REQLIST_LEN 50
  netwib_uint32 storedvalues;
  netwox_dhcp4opttype type[NETWOX_DHCP4OPT_REQLIST_LEN];
} netwox_dhcp4opt_reqlist;

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_DHCP4OPT_NETBIOSNODETYPE_B = 1,
  NETWOX_DHCP4OPT_NETBIOSNODETYPE_P = 2,
  NETWOX_DHCP4OPT_NETBIOSNODETYPE_M = 4,
  NETWOX_DHCP4OPT_NETBIOSNODETYPE_H = 8
} netwox_dhcp4opt_netbiosnodetype;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_dhcp4opttype type;
  union {
    netwib_bufext unknown;
    /* type pad : no value */
    /* type end : no value */
    netwib_ip subnetmask;
    netwox_dhcp4opt_iparray gateways;
    netwox_dhcp4opt_iparray timeservers;
    netwox_dhcp4opt_iparray nameservers;
    netwox_dhcp4opt_iparray dnsservers;
    netwox_dhcp4opt_iparray logservers;
    netwox_dhcp4opt_iparray lprservers;
    netwib_bufext hostname;
    netwib_bufext domainname;
    netwib_ip broadcastad;
    netwib_bool performroutdisc;
    netwox_dhcp4opt_staticroutes staticroutes;
    netwib_bufext nisdomain;
    netwox_dhcp4opt_iparray nisservers;
    netwox_dhcp4opt_iparray ntpservers;
    netwib_bufext vendorinfo;
    netwox_dhcp4opt_iparray netbiosns;
    netwox_dhcp4opt_iparray netbiosdd;
    netwox_dhcp4opt_netbiosnodetype netbiosnodetype;
    netwib_bufext netbiosscope;
    netwib_ip requestedipad;
    netwib_uint32 ipadleasetime;
    netwox_dhcp4opt_msgtype msgtype;
    netwib_ip serverid;
    netwox_dhcp4opt_reqlist reqlist;
    netwib_bufext errormsg;
    netwib_uint32 renewaltime;
    netwib_uint32 rebindingtime;
    netwib_bufext vendorid;
    netwox_dhcp4opt_clientid clientid;
    netwib_bufext nispdomain;
    netwox_dhcp4opt_iparray nispservers;
    netwox_dhcp4opt_iparray tftpservers;
    netwox_dhcp4opt_iparray smtpservers;
    netwox_dhcp4opt_iparray pop3servers;
    netwox_dhcp4opt_iparray nntpservers;
    netwox_dhcp4opt_iparray wwwservers;
    netwox_dhcp4opt_iparray ircservers;
  } opt;
} netwox_dhcp4opt;
typedef const netwox_dhcp4opt netwox_constdhcp4opt;

/*-------------------------------------------------------------*/
netwib_err netwox_dhcp4opt_initdefault(netwox_dhcp4opttype dhcp4opttype,
                                       netwox_dhcp4opt *pdhcp4opt);
netwib_err netwox_pkt_append_dhcp4opt(netwox_constdhcp4opt *pdhcp4opt,
                                      netwib_buf *ppkt);
netwib_err netwox_pkt_decode_dhcp4opt(netwib_constbuf *ppkt,
                                      netwox_dhcp4opt *pdhcp4opt,
                                      netwib_uint32 *pskipsize);

/*-------------------------------------------------------------*/
netwib_err netwox_dhcp4opt_show(netwox_constdhcp4opt *pdhcp4opt,
                                netwib_encodetype encodetype,
                                netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* request common fields */
netwib_err netwox_dhcp4opt_reqlist_initall(netwox_dhcp4opt *pdhcp4opt);
