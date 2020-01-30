
/*-------------------------------------------------------------*/
/* NBT Session Service is used for SMB over port 139.
   See also smbnaked.h.
*/

/*-------------------------------------------------------------*/
#define NETWOX_SMBNBTSSHDR_LEN 4

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SMBNBTSSTYPE_MSG = 0,
  NETWOX_SMBNBTSSTYPE_REQUEST = 0x81,
  NETWOX_SMBNBTSSTYPE_RESPPOSITIVE = 0x82,
  NETWOX_SMBNBTSSTYPE_RESPNEGATIVE = 0x83,
  NETWOX_SMBNBTSSTYPE_RESPRETARGET = 0x84,
  NETWOX_SMBNBTSSTYPE_KEEPALIVE = 0x85,
  NETWOX_SMBNBTSSTYPE_KEEPALIVE95 = 0xFFFFFFFF /* fake value for Windows 95
                                                  which uses a nul byte after
                                                  120 seconds of inactivity */
} netwox_smbnbtsstype;

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SMBNBTSS_RESPNEGATIVE_CALLEDNAMENOTLISTENING = 0x80,
  NETWOX_SMBNBTSS_RESPNEGATIVE_CALLINGNAMEREJECTED = 0x81,
  NETWOX_SMBNBTSS_RESPNEGATIVE_CALLEDNAMEUNREGISTERED = 0x82,
  NETWOX_SMBNBTSS_RESPNEGATIVE_INSUFFICIENTRESOURCES = 0x83,
  NETWOX_SMBNBTSS_RESPNEGATIVE_UNKNOWN = 0x8F
} netwox_smbnbtss_respnegative;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbnbtsstype smbnbtsstype;
  union {
    struct {
      netwib_bufext smbpkt;
    } msg;
    struct {
      netwib_byte callednamelabel; /* generally 0x20 */
      netwib_byte calledname[32]; /* NetBIOS encoded */
      netwib_byte callingnamelabel; /* generally 0x20 */
      netwib_byte callingname[32]; /* NetBIOS encoded */
    } request;
    /* nothing for RESPPOSITIVE */
    struct {
      netwox_smbnbtss_respnegative error;
    } respnegative;
    struct {
      netwib_ip host;
      netwib_port port;
    } respretarget;
    /* nothing for KEEPALIVE */
    /* nothing for KEEPALIVE95 */
  } payload;
} netwox_smbnbtss;
typedef const netwox_smbnbtss netwox_constsmbnbtss;

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_smbnbtss(netwox_constsmbnbtss *psmbnbtss,
                                      netwib_buf *ppkt);
netwib_err netwox_pkt_decode_smbnbtss(netwib_constbuf *ppkt,
                                      netwox_smbnbtss *psmbnbtss,
                                      netwib_uint32 *pskipsize);

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_smbnbtss_msghdr(netwib_uint32 msgsize,
                                             netwib_buf *ppkt);
