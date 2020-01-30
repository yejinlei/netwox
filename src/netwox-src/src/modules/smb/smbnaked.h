
/*-------------------------------------------------------------*/
/* A naked SMB header is used for SMB over port 445.
   See also nbtss.h.
*/

/*-------------------------------------------------------------*/
#define NETWOX_SMBNAKEDHDR_LEN 4

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bufext smbpkt;
} netwox_smbnaked;
typedef const netwox_smbnaked netwox_constsmbnaked;

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_smbnaked(netwox_constsmbnaked *psmbnaked,
                                      netwib_buf *ppkt);
netwib_err netwox_pkt_decode_smbnaked(netwib_constbuf *ppkt,
                                      netwox_smbnaked *psmbnaked,
                                      netwib_uint32 *pskipsize);

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_smbnaked_msghdr(netwib_uint32 msgsize,
                                             netwib_buf *ppkt);


