
/*-------------------------------------------------------------*/
/* Ethernet header (NETWIB_DEVICE_DLTTYPE_EN10MB) */
typedef enum {
  NETWIB_ETHERHDRTYPE_IP4 =    0x0800,
  NETWIB_ETHERHDRTYPE_ARP =    0x0806,
  NETWIB_ETHERHDRTYPE_VINES =  0x0bad,
  NETWIB_ETHERHDRTYPE_TRAIN =  0x1984,
  NETWIB_ETHERHDRTYPE_CDP =    0x2000,
  NETWIB_ETHERHDRTYPE_RARP =   0x8035,
  NETWIB_ETHERHDRTYPE_ATALK =  0x809b,
  NETWIB_ETHERHDRTYPE_AARP =   0x80f3,
  NETWIB_ETHERHDRTYPE_VLAN =   0x8100,
  NETWIB_ETHERHDRTYPE_IPX =    0x8137,
  NETWIB_ETHERHDRTYPE_IP6 =    0x86DD,
  NETWIB_ETHERHDRTYPE_PPPOED = 0x8863,
  NETWIB_ETHERHDRTYPE_PPPOES = 0x8864,
  NETWIB_ETHERHDRTYPE_LOOP =   0x9000
} netwib_etherhdrtype;
typedef struct {
  netwib_eth dst;
  netwib_eth src;
  netwib_etherhdrtype type;
} netwib_etherhdr;
typedef const netwib_etherhdr netwib_constetherhdr;
#define NETWIB_ETHERHDR_LEN 14

/*-------------------------------------------------------------*/
/* NULL header (NETWIB_DEVICE_DLTTYPE_NULL) */
typedef struct {
  netwib_etherhdrtype type;
} netwib_nullhdr;
typedef const netwib_nullhdr netwib_constnullhdr;
#define NETWIB_NULLHDR_LEN 4

/*-------------------------------------------------------------*/
/* LOOP header (NETWIB_DEVICE_DLTTYPE_LOOP) */
typedef struct {
  netwib_etherhdrtype type;
} netwib_loophdr;
typedef const netwib_loophdr netwib_constloophdr;
#define NETWIB_LOOPHDR_LEN 4

/*-------------------------------------------------------------*/
/* RAW header (NETWIB_DEVICE_DLTTYPE_RAW) */
#define NETWIB_RAWHDR_LEN 0

/*-------------------------------------------------------------*/
/* PPP header (NETWIB_DEVICE_DLTTYPE_PPP) */
typedef enum {
  NETWIB_PPPHDRPROTO_IP4 =     0x0021, /* Internet Protocol */
  NETWIB_PPPHDRPROTO_AT =      0x0029, /* AppleTalk Protocol */
  NETWIB_PPPHDRPROTO_IPX =     0x002b, /* IPX protocol */
  NETWIB_PPPHDRPROTO_VJC_COMP =   0x002d, /* VJ compressed TCP */
  NETWIB_PPPHDRPROTO_VJC_UNCOMP = 0x002f, /* VJ uncompressed TCP */
  NETWIB_PPPHDRPROTO_IP6 =     0x0057, /* Internet Protocol Version 6 */
  NETWIB_PPPHDRPROTO_COMP =    0x00fd, /* compressed packet */
  NETWIB_PPPHDRPROTO_IPCP =    0x8021, /* IP Control Protocol */
  NETWIB_PPPHDRPROTO_ATCP =    0x8029, /* AppleTalk Control Protocol */
  NETWIB_PPPHDRPROTO_IPXCP =   0x802b, /* IPX Control Protocol */
  NETWIB_PPPHDRPROTO_IP6CP =   0x8057, /* IPv6 Control Protocol */
  NETWIB_PPPHDRPROTO_CCP =     0x80fd, /* Compression Control Protocol */
  NETWIB_PPPHDRPROTO_LCP =     0xc021, /* Link Control Protocol */
  NETWIB_PPPHDRPROTO_PAP =     0xc023, /* Password Authentication Proto */
  NETWIB_PPPHDRPROTO_LQR =     0xc025, /* Link Quality Report protocol */
  NETWIB_PPPHDRPROTO_CHAP =    0xc223, /* Crypto. Handshake Auth. P. */
  NETWIB_PPPHDRPROTO_CBCP =    0xc029  /* Callback Control Protocol */
} netwib_ppphdrproto;
typedef struct {
  netwib_byte address;
  netwib_byte control;
  netwib_ppphdrproto protocol;
} netwib_ppphdr;
typedef const netwib_ppphdr netwib_constppphdr;
#define NETWIB_PPPHDR_LEN 4

/*-------------------------------------------------------------*/
/* LINUX_SLL header (NETWIB_DEVICE_DLTTYPE_LINUX_SLL) */
typedef enum {
  NETWIB_LINUXSLLHDRPKTTYPE_HOST = 0,
  NETWIB_LINUXSLLHDRPKTTYPE_BROADCAST = 1,
  NETWIB_LINUXSLLHDRPKTTYPE_MULTICAST = 2,
  NETWIB_LINUXSLLHDRPKTTYPE_OTHERHOST = 3,
  NETWIB_LINUXSLLHDRPKTTYPE_OUTGOING = 4
} netwib_linuxsllhdrpkttype;
typedef enum {
  NETWIB_LINUXSLLHDRHATYPE_NETROM = 0,
  NETWIB_LINUXSLLHDRHATYPE_ETHER = 1
} netwib_linuxsllhdrhatype;
typedef struct {
  netwib_linuxsllhdrpkttype pkttype;   /* packet type */
  netwib_linuxsllhdrhatype hatype;     /* link-layer address type */
  netwib_uint16 halen;                 /* link-layer address length */
  netwib_byte srcaddr[8];              /* link-layer address */
  netwib_etherhdrtype protocol;        /* protocol */
} netwib_linuxsllhdr;
typedef const netwib_linuxsllhdr netwib_constlinuxsllhdr;
#define NETWIB_LINUXSLLHDR_LEN 16

/*-------------------------------------------------------------*/
/* Link header */
typedef struct {
  netwib_device_dlttype type;
  union {
    netwib_etherhdr    ether;
    netwib_nullhdr     null;
    netwib_loophdr     loop;
    /* no netwib_rawhdr needed */
    netwib_ppphdr      ppp;
    netwib_linuxsllhdr linuxsll;
  } hdr;
} netwib_linkhdr;
typedef const netwib_linkhdr netwib_constlinkhdr;
#define NETWIB_LINKHDR_MAXLEN NETWIB_LINUXSLLHDR_LEN

/*-------------------------------------------------------------*/
/* Name : netwib_linkhdr_initdefault
   Description :
     Initialize a netwib_linkhdr with default values.
   Input parameter(s) :
     type : header type
   Input/output parameter(s) :
   Output parameter(s) :
     *plinkhdr : netwib_linkhdr to initialize
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_linkhdr_initdefault(netwib_device_dlttype type,
                                      netwib_linkhdr *plinkhdr);

/*-------------------------------------------------------------*/
/* Name : netwib_pkt_append_linkhdr
   Description :
     Initialize physical data from a netwib_linkhdr.
   Input parameter(s) :
     linkhdr : netwib_linkhdr
   Input/output parameter(s) :
     *ppht : buffer to initialize
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_pkt_append_linkhdr(netwib_constlinkhdr *plinkhdr,
                                     netwib_buf *ppkt);
netwib_err netwib_pkt_prepend_linkhdr(netwib_constlinkhdr *plinkhdr,
                                      netwib_buf *ppkt);

/*-------------------------------------------------------------*/
/* Name : netwib_pkt_decode_linkhdr
   Description :
     Decode a netwib_linkhdr from data coming from wire.
   Input parameter(s) :
     dlttype : DLT type of packet
     *ppkt : buffer containing physical data
   Input/output parameter(s) :
   Output parameter(s) :
     *plinkhdr : netwib_linkhdr to initialize
     *pskipsize : size to skip this header
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_pkt_decode_linkhdr(netwib_device_dlttype dlttype,
                                     netwib_constbuf *ppkt,
                                     netwib_linkhdr *plinkhdr,
                                     netwib_uint32 *pskipsize);

/*-------------------------------------------------------------*/
/***************************************************************
 * Link headers are horribly incompatible. The only field      *
 * which can be considered as compatible is the type/protocol. *
 * Following functions permit to set/get it from header.       *
 * Type netwib_linkhdrproto is the generic type.               *
 ***************************************************************/
typedef enum {
  NETWIB_LINKHDRPROTO_UNKNOWN = 0,
  NETWIB_LINKHDRPROTO_IP4,
  NETWIB_LINKHDRPROTO_IP6,
  NETWIB_LINKHDRPROTO_ARP,
  NETWIB_LINKHDRPROTO_RARP,
  NETWIB_LINKHDRPROTO_IPX
} netwib_linkhdrproto;
netwib_err netwib_linkhdr_set_proto(netwib_linkhdr *plinkhdr,
                                    netwib_linkhdrproto linkhdrproto);
netwib_err netwib_linkhdr_get_proto(netwib_constlinkhdr *plinkhdr,
                                    netwib_linkhdrproto *plinkhdrproto);
