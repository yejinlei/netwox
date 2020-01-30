
/*-------------------------------------------------------------*/
/* this structure can be read by two threads */
typedef struct {
  /* can be changed by user */
  netwib_buf domain; /* ASCII (not Unicode) */
  netwib_buf share; /* ASCII (not Unicode) */
  netwib_buf rootdir; /* ASCII (not Unicode) */
  netwib_uint32 maxwaitms;
  netwib_ips *pallowedclients;
  netwib_buf user; /* ASCII (not Unicode) */
  netwib_buf password; /* ASCII (not Unicode) */
  netwib_bool allowput;
  netwib_bool debug;
  netwib_bool verbose;
  /* internally needed */
  netwib_bufpool *pbufpool;
  netwib_bool isnaked; /* port 139 or 445 */
} netwox_smbserver;
typedef const netwox_smbserver netwox_constsmbserver;

netwib_err netwox_smbserver_init(netwox_sockinfo *psockinfo,
                                 netwox_smbserver *psmbserver);
netwib_err netwox_smbserver_close(netwox_smbserver *psmbserver);

/*-------------------------------------------------------------*/
netwib_err netwox_smbserver_session(netwib_io *psockio,
                                    netwib_ptr pinfos);
