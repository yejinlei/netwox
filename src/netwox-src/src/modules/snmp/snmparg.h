
/*-------------------------------------------------------------*/
#define NETWOX_SNMP_ARG NETWOX_TOOLARG_OPTA_UINT32('v', "version", "SNMP version (1, 2 or 3)", "1"), NETWOX_TOOLARG_OPT_BUF_PASSWORD('c', "community", "SNMPv1/v2 community", "public"), NETWOX_TOOLARG_OPTA_BOOL('m', "md5auth", "SNMPv3 auth using MD5", "1"), NETWOX_TOOLARG_OPTA_BUF_LOGIN('u', "username", "SNMPv3 username", NULL), NETWOX_TOOLARG_OPTA_BUF_PASSWORD('w', "password", "SNMPv3 password", NULL), NETWOX_TOOLARG_OPTA_BUF_MIXED('A', "authoritativeengineid", "SNMPv3 authoritativeengineid", NULL), NETWOX_TOOLARG_OPTA_UINT32('B', "authoritativeengineboots", "SNMPv3 authoritativeengineboots", NULL), NETWOX_TOOLARG_OPTA_UINT32('M', "authoritativeenginetime", "SNMPv3 authoritativeenginetime", NULL), NETWOX_TOOLARG_OPTA_BUF_MIXED('C', "contextengineid", "SNMPv3 contextengineid", NULL), NETWOX_TOOLARG_OPTA_BUF('N', "contextname", "SNMPv3 contextname", NULL), NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in ms", "10000"), NETWOX_TOOLARG_OPTA_BOOL('y', "display", "display packets", NULL)

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bufpool *pbufpool;
  netwib_uint32 maxwaitms;
  netwib_bool display;
  netwox_snmpversion version;
  union {
    /* v1 and v2 */
    struct {
      netwib_buf *pcommunity;
    } v12;
    /* v3 */
    struct {
      netwib_bool md5auth;
      netwib_buf *pusername;
      netwib_buf *ppassword;
      netwib_buf *pauthoritativeengineid;
      netwib_uint32 authoritativeengineboots;
      netwib_uint32 authoritativeenginetime;
      netwib_buf *pcontextengineid;
      netwib_buf *pcontextname;
      /* internal storage */
      netwib_buf *pku;
      netwib_uint32 id;
    } v3;
  } v;
} netwox_snmparg;
typedef netwox_snmparg netwox_constsnmparg;

/*-------------------------------------------------------------*/
netwib_err netwox_snmparg_init(netwox_arg *parg,
                               netwib_io *psockio,
                               netwox_snmparg *psnmparg);
netwib_err netwox_snmparg_close(netwox_snmparg *psnmparg);

/*-------------------------------------------------------------*/
netwib_err netwox_snmparg_display(netwox_constsnmparg *psnmparg);

/*-------------------------------------------------------------*/
netwib_err netwox_snmparg_msg_init_begin(netwox_constsnmparg *psnmparg,
                                         netwox_snmpmsg *psnmpmsg,
                                         netwox_snmp3usmsp *psnmp3usmsp,
                                         netwox_snmppdu **ppsnmppdu);
netwib_err netwox_snmparg_msg_init_end(netwox_constsnmparg *psnmparg,
                                       netwox_snmpmsg *psnmpmsg,
                                       netwox_snmp3usmsp *psnmp3usmsp);

/*-------------------------------------------------------------*/
netwib_err netwox_snmparg_msg_decode(netwox_constsnmparg *psnmparg,
                                     netwox_snmpmsg *psnmpmsg,
                                     netwox_snmp3usmsp *psnmp3usmsp,
                                     netwox_snmppdu **ppsnmppdu);
