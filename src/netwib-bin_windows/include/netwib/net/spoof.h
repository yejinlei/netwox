
/*-------------------------------------------------------------*/
/***************************************************************
 * Netwib supports Ethernet cards(netwib_device_hwtype=ETHER), *
 * Loopback(LOOPBACK) and Serial Modems(PPP).                  *
 * Unfortunately, these are not supported on every system as   *
 * described in this table.                                    *
 *                                                             *
 *       Device    LINK       LINK_dlt   IP4        IP6        *
 *       hwtype    supported? to_use     supported? supported? *
 *  FreeBSD :                                                  *
 *       ETHER     yes        ETHER      yes        ?[3]       *
 *       LOOPBACK  yes        NULL       yes        ?[3]       *
 *       PPP       ?[3]       ?[3]       yes        ?[3]       *
 *  Linux :                                                    *
 *       ETHER     yes        ETHER      yes        yes[4]     *
 *       LOOPBACK  no         -          yes        yes[4]     *
 *       PPP       ?[3]       ?[3]       yes        yes[4]     *
 *  OpenBSD :                                                  *
 *       ETHER     yes        ETHER      yes        ?[3]       *
 *       LOOPBACK  no         -          yes        ?[3]       *
 *       PPP       ?[3]       ?[3]       yes        ?[3]       *
 *  Solaris :                                                  *
 *       ETHER     yes        ETHER      yes        ?[3]       *
 *       LOOPBACK  no         -          no[1]      no[1]      *
 *       PPP       ?[3]       ?[3]       yes        ?[3]       *
 *  Windows 95 :                                               *
 *       ETHER     yes        ETHER      no         no         *
 *       LOOPBACK  no         -          no         no         *
 *       PPP       no         no         no         no         *
 *  Windows 98 :                                               *
 *       ETHER     yes        ETHER      no         no         *
 *       LOOPBACK  no         -          no         no         *
 *       PPP       no         no         no         no         *
 *  Windows Me :                                               *
 *       ETHER     yes        ETHER      no         no         *
 *       LOOPBACK  no         -          no         no         *
 *       PPP       ?[3]       ?[3]       no         no         *
 *  Windows NT4 :                                              *
 *       ETHER     yes        ETHER      no         no         *
 *       LOOPBACK  no         -          no         no         *
 *       PPP       no         no         no         no         *
 *  Windows 2000 :                                             *
 *       ETHER     yes        ETHER      yes        ?[3]       *
 *       LOOPBACK  no         -          no         no         *
 *       PPP       ?[3]       ?[3]       yes        ?[3]       *
 *  Windows XP :                                               *
 *       ETHER     yes        ETHER      yes        yes[4]     *
 *       LOOPBACK  no         -          no         no         *
 *       PPP       ?[3]       ?[3]       ?[2]       ?[3]       *
 *                                                             *
 * Notes :                                                     *
 *  1 : not sure                                               *
 *  2 : not tested                                             *
 *  3 : I'll summarize this info later...                      *
 *  4 : on systems where IPv6 is supported                     *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* Name : netwib_io_init_spoof
   Description :
     Open a spoof session.
   Input parameter(s) :
     *pdevice : device where we spoof
   Input/output parameter(s) :
   Output parameter(s) :
     **ppio : io created
   Normal return values :
     NETWIB_ERR_OK : ok
*/
typedef enum {
  NETWIB_SPOOF_INITTYPE_LINK = 1,    /* spoof at link level */
  NETWIB_SPOOF_INITTYPE_IP4,         /* spoof at IP4 level */
  NETWIB_SPOOF_INITTYPE_IP6          /* spoof at IP6 level */
} netwib_spoof_inittype;
netwib_err netwib_io_init_spoof(netwib_spoof_inittype inittype,
                                netwib_constbuf *pdevice,
                                netwib_io **ppio);
#define netwib_io_init_spoof_link(device,ppio) netwib_io_init_spoof(NETWIB_SPOOF_INITTYPE_LINK,device,ppio)
#define netwib_io_init_spoof_ip4(ppio) netwib_io_init_spoof(NETWIB_SPOOF_INITTYPE_IP4,NULL,ppio)
#define netwib_io_init_spoof_ip6(ppio) netwib_io_init_spoof(NETWIB_SPOOF_INITTYPE_IP6,NULL,ppio)

/*-------------------------------------------------------------*/
/* obtain the DLT needed to spoof at link level */
/* netwib_err f(netwib_io *pio, netwib_device_dlttype *pdlt); */
#define netwib_spoof_ctl_get_dlt(pio,pdlt) netwib_io_ctl_get(pio,NETWIB_IO_WAYTYPE_SUPPORTED,NETWIB_IO_CTLTYPE_SPOOF_DLT,NULL,(netwib_uint32*)pdlt)

/*-------------------------------------------------------------*/
/***************************************************************
 * This io permits to spoof at IP4/6 level (DLTTYPE_RAW).      *
 ***************************************************************/
typedef enum {
  NETWIB_SPOOF_IP_INITTYPE_UNKNOWN = 0,
  NETWIB_SPOOF_IP_INITTYPE_RAW,   /* use only IP4/6 level */
  NETWIB_SPOOF_IP_INITTYPE_LINKF, /* add a link header and spoof
                                     at link level (source ad
                                     is filled) */
  NETWIB_SPOOF_IP_INITTYPE_LINKB, /* add a link header and spoof
                                     at link level (source ad
                                     is blank (0:0:0:0:0:0)) */
  NETWIB_SPOOF_IP_INITTYPE_LINKFB, /* add a link header and spoof
                                      at link level (source ad
                                      can be blank) */
  NETWIB_SPOOF_IP_INITTYPE_RAWLINKF, /* try IP4/6 level. If doesn't
                                        work, use link */
  NETWIB_SPOOF_IP_INITTYPE_RAWLINKB,
  NETWIB_SPOOF_IP_INITTYPE_RAWLINKFB,
  NETWIB_SPOOF_IP_INITTYPE_LINKFRAW, /* try link level. If doesn't
                                        work, use IP4/6 */
  NETWIB_SPOOF_IP_INITTYPE_LINKBRAW,
  NETWIB_SPOOF_IP_INITTYPE_LINKFBRAW,
  NETWIB_SPOOF_IP_INITTYPE_LINK = NETWIB_SPOOF_IP_INITTYPE_LINKFB,
  NETWIB_SPOOF_IP_INITTYPE_RAWLINK = NETWIB_SPOOF_IP_INITTYPE_RAWLINKFB,
  NETWIB_SPOOF_IP_INITTYPE_LINKRAW = NETWIB_SPOOF_IP_INITTYPE_LINKFBRAW,
  NETWIB_SPOOF_IP_INITTYPE_BEST = NETWIB_SPOOF_IP_INITTYPE_LINKRAW
} netwib_spoof_ip_inittype;
netwib_err netwib_io_init_spoof_ip(netwib_spoof_ip_inittype inittype,
                                   netwib_io **ppio);
#define netwib_io_init_spoof_ip_best(ppio) netwib_io_init_spoof_ip(NETWIB_SPOOF_IP_INITTYPE_BEST,NETWIB_PKT_FIELDTYPE_BEST,ppio)

/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_spoof_ip_inittype
   Description :
     Obtains the buf string corresponding to a type.
   Input parameter(s) :
     type : type
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf set with the buf string
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_append_spoof_ip_inittype(netwib_spoof_ip_inittype type,
                                               netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* Name : netwib_spoof_ip_inittype_init_kbd
   Description :
     Initialize a netwib_spoof_ip_inittype with data entered
     through keyboard.
   Input parameter(s) :
     *pmessage : message to print before
     defaulttype : default type to use if user enters
                   nothing (if NETWIB_SPOOF_IP_INITTYPE_UNKNOWN,
                   means no default)
   Input/output parameter(s) :
   Output parameter(s) :
     *phwtype : netwib_hwtype initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_spoof_ip_inittype_init_kbd(netwib_constbuf *pmessage,
                                             netwib_spoof_ip_inittype defaulttype,
                                             netwib_spoof_ip_inittype *ptype);
#define NETWIB_SPOOF_IP_INITTYPE_INIT_KBD_NODEF NETWIB_SPOOF_IP_INITTYPE_UNKNOWN
