
/*-------------------------------------------------------------*/
/* Internal SMB implementation respects Unicode characters.
   However, netwox only supports ascii filenames. Conversion
   is done in this module (and in smbcli for client). Users
   can use functions of this module without bothering, by using
   ascii filenames.
 */

/*-------------------------------------------------------------*/
/* SMB server implementation uses same fake values
   because I don't want to implement things such as "the free
   space on the disk", "the inode number", etc. All fake values
   are explained in comments. This implementation works but it
   is not very nice. I do not plan to improve it because too many
   filesystem low level feature would need to be added to netwib.
 */
/* common fake values are defined here */
#define NETWOX_SMBSER_FAKE_FS_BYTESPERSECTOR 512
#define NETWOX_SMBSER_FAKE_FS_SECTORSPERUNIT 128
#define NETWOX_SMBSER_FAKE_FS_TOTALUNITS 60000
/* 512*128*60000 = 3932Mb */
#define NETWOX_SMBSER_FAKE_FS_FREEUNITS 50000
/* 512*128*50000 = 3276Mb */
#define NETWOX_SMBSER_FAKE_FS_ID 0x1234
#define NETWOX_SMBSER_FAKE_FS_LABEL "mydisk"
#define NETWOX_SMBSER_FAKE_FS_TYPE "NTFS"

/*-------------------------------------------------------------*/
/* This implementation does not support:
    - share access authentication (only user access)
    - locking (so editing a file directly on the share is not safe)
    - Unix symlinks
    - filenames with characters other than ASCII-127 (accents)
    - when user edits a file, its size is not updated (user has
      to press F5 to see the change) (NETWOX_SMBCMDTSCNTQ_TYPE_NTNOTIFY)
   These could be implemented later.
*/

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SMBSER_SESSIONTREE_STATE_NOTUSED = 1,
  NETWOX_SMBSER_SESSIONTREE_STATE_INUSE,
  NETWOX_SMBSER_SESSIONTREE_STATE_CLOSED
} netwox_smbser_sessiontree_state;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbser_sessiontree_state state;
  netwib_uint16 treeid;
  netwib_bool isipc; /* type of share */
} netwox_smbser_tree;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_smbser_sessiontree_state state;
  netwib_uint16 userid;
  netwox_smbcmdcmn_negocapa capabilities;
  /* each tree */
  netwib_array trees; /* array of netwox_smbser_tree */
  /* current tree */
  netwib_uint32 currenttree; /* set after each received packet */
#define NETWOX_SMBSER_CURRENTTREE_NOTFOUND 0xFFFFFFFFu
} netwox_smbser_session;

/*-------------------------------------------------------------*/
/* information related to a thread */
typedef struct {
  netwox_constsmbserver *pcommon; /* shared info */
  netwib_io *pio;
  /* internal */
  netwib_bool negotiated;
  netwib_buf *pbufrecv; /* receiving buffer storage */
  netwox_smbmsg_cmd smberrcmd; /* to set when send back an error */
  netwox_smberr smberrtosend; /* to send back an error */
  netwox_smbserfid smbserfid;
  netwib_uint32 sessiontreecounter;
  netwox_smbser83 smbser83;
  netwox_smbserglob smbserglob;
  /* values defined by server and optionally negotiated with client */
  netwib_uint8 cmn_secumode;
  netwib_uint32 cmn_maxmsgsize;
  netwib_uint32 cmn_vcid;
  netwib_buf *pcmn_challenge;
  /* each session */
  netwib_array sessions; /* array of netwox_smbser_session */
  /* current session */
  netwib_uint32 currentsession; /* set after each received packet */
#define NETWOX_SMBSER_CURRENTSESSION_NOTFOUND 0xFFFFFFFFu
  netwib_uint32 currentsession_beforentbug;
} netwox_smbser;
typedef const netwox_smbser netwox_constsmbser;

netwib_err netwox_smbser_init(netwox_constsmbserver *pcommon,
                              netwib_io *pio,
                              netwox_smbser *psmbser);
netwib_err netwox_smbser_close(netwox_smbser *psmbser);

/*-------------------------------------------------------------*/
/* add, then update currentsession and currenttree */
netwib_err netwox_smbser_session_add(netwox_smbser *psmbser,
                                     netwox_smbser_session **ppsession);
netwib_err netwox_smbser_tree_add(netwox_smbser *psmbser,
                                  netwox_smbser_tree **pptree);
/* session and tree must be is INUSE state */
netwib_err netwox_smbser_sessiontree_check(netwox_smbser *psmbser,
                                           netwox_smbser_session **ppsession,
                                           netwox_smbser_tree **pptree);
/* only get session and eventually tree (if !NULL), without checking state */
netwib_err netwox_smbser_sessiontree_get(netwox_smbser *psmbser,
                                         netwox_smbser_session **ppsession,
                                         netwox_smbser_tree **pptree);
netwib_err netwox_smbser_sessiontree_display(netwox_smbser *psmbser);

/*-------------------------------------------------------------*/
netwib_err netwox_smbser_smbmsg_setdefault(netwox_smbser *psmbser,
                                           netwox_constsmbmsg *psmbmsgq,
                                           netwox_smbmsg *psmbmsgr);
netwib_err netwox_smbser_smbmsg_send(netwox_smbser *psmbser,
                                     netwox_smbmsg *psmbmsg);
netwib_err netwox_smbser_smbmsg_recv(netwox_smbser *psmbser,
                                     netwox_smbmsg *psmbmsg);

/*-------------------------------------------------------------*/
netwib_err netwox_smbser_send_begin(netwox_smbser *psmbser,
                                    netwox_constsmbmsg *psmbmsgq,
                                    netwox_smbmsg *psmbmsgr,
                                    netwox_smbcmd *psmbcmdr,
                                    netwox_smbcmd_type type);
netwib_err netwox_smbser_send_end(netwox_smbser *psmbser,
                                  netwox_smbmsg *psmbmsgr,
                                  netwox_smbcmd *psmbcmdr);

/*-------------------------------------------------------------*/
netwib_err netwox_smbser_nottested(netwox_smbser *psmbser);
netwib_err netwox_smbser_notimplemented(netwox_smbser *psmbser);
netwib_err netwox_smbser_internalerror(netwox_smbser *psmbser);

/*-------------------------------------------------------------*/
/* used for messages needing limit : all message, command and transaction
   headers should feed in 200 bytes */
#define NETWOX_SMBSER_SUFFICIENT_FOR_HEADERS 200
