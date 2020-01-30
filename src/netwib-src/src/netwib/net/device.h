
/*-------------------------------------------------------------*/
/* Name : netwib_device_init_kbd
   Description :
     Initialize a device by the keyboard
   Input parameter(s) :
     message : message to print before
     defaultdevice : default device to use if user enters nothing
   Input/output parameter(s) :
   Output parameter(s) :
     *pdevice : device set with user's string
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_device_init_kbd(netwib_constbuf *pmessage,
                                  netwib_constbuf *pdefaultdevice,
                                  netwib_buf *pdevice);
#define NETWIB_DEVICE_INIT_KBD_NODEF NULL

/*-------------------------------------------------------------*/
/* Hardware types */
typedef enum {
  NETWIB_DEVICE_HWTYPE_UNKNOWN = 1,
  NETWIB_DEVICE_HWTYPE_ETHER, /* or Wifi (too late now to add a new type) */
  NETWIB_DEVICE_HWTYPE_LOOPBACK,
  NETWIB_DEVICE_HWTYPE_PPP,
  NETWIB_DEVICE_HWTYPE_PLIP, /* Parallel - obsolete */
  NETWIB_DEVICE_HWTYPE_SLIP /* Serial - obsolete */
} netwib_device_hwtype;

/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_device_hwtype
   Description :
     Obtains the buf string corresponding to a hwtype.
   Input parameter(s) :
     type : type
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf set with the buf string
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_append_device_hwtype(netwib_device_hwtype type,
                                           netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* Name : netwib_device_hwtype_init_kbd
   Description :
     Initialize a netwib_device_hwtype with data entered
     through keyboard.
   Input parameter(s) :
     *pmessage : message to print before
     defaulthwtype : default hwtype to use if user enters
                      nothing (if NETWIB_DEVICE_HWTYPE_UNKNOWN,
                      means no default)
   Input/output parameter(s) :
   Output parameter(s) :
     *phwtype : netwib_hwtype initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_device_hwtype_init_kbd(netwib_constbuf *pmessage,
                                         netwib_device_hwtype defaulthwtype,
                                         netwib_device_hwtype *phwtype);
#define NETWIB_DEVICE_HWTYPE_INIT_KBD_NODEF NETWIB_DEVICE_HWTYPE_UNKNOWN

/*-------------------------------------------------------------*/
/***************************************************************
 * Sniffed data is represented as specified by the DLT         *
 * associated to packets.                                      *
 * Spoofed data (at link level) also has to be represented as  *
 * DLT expects.                                                *
 ***************************************************************/
/* Data-link level type codes (names from net/bpf.h). */
typedef enum {
  NETWIB_DEVICE_DLTTYPE_UNKNOWN = 1, /* not listed below */
  NETWIB_DEVICE_DLTTYPE_NULL,        /* no link-layer encapsulation */
  NETWIB_DEVICE_DLTTYPE_EN10MB,      /* Ethernet (10Mb) */
  NETWIB_DEVICE_DLTTYPE_ETHER = NETWIB_DEVICE_DLTTYPE_EN10MB,
  NETWIB_DEVICE_DLTTYPE_EN3MB,       /* Experimental Ethernet (3Mb) */
  NETWIB_DEVICE_DLTTYPE_AX25,        /* Amateur Radio AX.25 */
  NETWIB_DEVICE_DLTTYPE_PRONET,      /* Proteon ProNET Token Ring */
  NETWIB_DEVICE_DLTTYPE_CHAOS,       /* Chaos */
  NETWIB_DEVICE_DLTTYPE_IEEE802,     /* IEEE 802 Networks */
  NETWIB_DEVICE_DLTTYPE_ARCNET,      /* ARCNET */
  NETWIB_DEVICE_DLTTYPE_SLIP,        /* Serial Line IP */
  NETWIB_DEVICE_DLTTYPE_PPP,         /* Point-to-point Protocol */
  NETWIB_DEVICE_DLTTYPE_FDDI,        /* FDDI */
  NETWIB_DEVICE_DLTTYPE_ATM_RFC1483, /* LLC/SNAP encapsulated atm */
  NETWIB_DEVICE_DLTTYPE_RAW,         /* raw IPv4 or IPv6 */
  NETWIB_DEVICE_DLTTYPE_RAW4,        /* raw IPv4 */
  NETWIB_DEVICE_DLTTYPE_RAW6,        /* raw IPv6 */
  NETWIB_DEVICE_DLTTYPE_SLIP_BSDOS,  /* BSD/OS Serial Line IP */
  NETWIB_DEVICE_DLTTYPE_PPP_BSDOS,   /* BSD/OS Point-to-point Protocol */
  NETWIB_DEVICE_DLTTYPE_ATM_CLIP,    /* Linux Classical-IP over ATM */
  NETWIB_DEVICE_DLTTYPE_PPP_SERIAL,  /* PPP over serial with HDLC enc. */
  NETWIB_DEVICE_DLTTYPE_PPP_ETHER,   /* PPP over Ethernet */
  NETWIB_DEVICE_DLTTYPE_C_HDLC,      /* Cisco HDLC */
  NETWIB_DEVICE_DLTTYPE_IEEE802_11,  /* IEEE 802.11 wireless */
  NETWIB_DEVICE_DLTTYPE_LOOP,
  NETWIB_DEVICE_DLTTYPE_LINUX_SLL,
  NETWIB_DEVICE_DLTTYPE_LTALK,
  NETWIB_DEVICE_DLTTYPE_ECONET,
  NETWIB_DEVICE_DLTTYPE_PRISM_HEADER,
  NETWIB_DEVICE_DLTTYPE_AIRONET_HEADER,
  NETWIB_DEVICE_DLTTYPE_USER_BEGIN = NETWIB_ENUM_USER_BEGIN
} netwib_device_dlttype;

/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_device_dlttype
   Description :
     Obtains the buf string corresponding to a hwtype.
   Input parameter(s) :
     type : type
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf set with the buf string
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_append_device_dlttype(netwib_device_dlttype type,
                                            netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* Name : netwib_device_dlttype_init_kbd
   Description :
     Initialize a netwib_device_dlttype with data entered
     through keyboard.
   Input parameter(s) :
     *pmessage : message to print before
     defaultdlttype : default dlttype to use if user enters
                      nothing (if NETWIB_DEVICE_DLTTYPE_UNKNOWN,
                      means no default)
   Input/output parameter(s) :
   Output parameter(s) :
     *pdlttype : netwib_dlttype initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_device_dlttype_init_kbd(netwib_constbuf *pmessage,
                                          netwib_device_dlttype defaultdlttype,
                                          netwib_device_dlttype *pdlttype);
#define NETWIB_DEVICE_DLTTYPE_INIT_KBD_NODEF NETWIB_DEVICE_DLTTYPE_UNKNOWN
