
/*-------------------------------------------------------------*/
/*
  Tool options have the syntax :
    --long value
    --lo value   [if there is no conflict with other arg name]
    -k value
  And only for NETWOX_TOOLARGTYPE_BOOL :
    --long
    --lo         [if there is no conflict with other arg name]
    --no-long
    -k
    -jk          [if j and k are both NETWOX_TOOLARGTYPE_BOOL]
    +k
*/

/*-------------------------------------------------------------*/
/* simple example :
netwox_toolarg t00000__args[] = {
  NETWOX_TOOLARG_REQ_IP('s', "srvip", NULL, NULL),
  NETWOX_TOOLARG_REQ_PORT('p', NULL, NULL, NULL),
  NETWOX_TOOLARG_REQ_BOOL('b', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPT_DEVICE('d', "device", "device to use", NULL),
  NETWOX_TOOLARG_OPT_IP('i', "myip", "iP", "1.5.6.4"),
  NETWOX_TOOLARG_OPT_IP('j', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPT_IP_SRC('k', NULL, NULL, "2.2.3.4"),
  NETWOX_TOOLARG_OPT_BOOL('f', NULL, NULL, "0"),
  NETWOX_TOOLARG_OPT_ETH('e', NULL, NULL, NULL),
  NETWOX_TOOLARG_MORE("file1 file2 ...", "file list"),
  NETWOX_TOOLARG_END
};
netwib_er(netwox_arg_init(argc, argv, &t00000__info, &parg));
netwib_er(netwox_arg_isset(parg, 'i', &isset));
netwib_er(netwox_arg_ip(parg, 'i', &ipad));
netwib_er(netwox_arg_more_count(parg, &count));
netwib_er(netwib_buf_init_mallocdefault(&buf));
for (i = 0; i < count; i++) {
  netwib__buf_reinit(&buf);
  netwib_er(netwox_arg_more(parg, i, &buf));
  netwib_er(netwib_fmt_display("more : %{buf}\n", &buf));
}
netwib_er(netwib_buf_close(&buf));
netwib_er(netwox_arg_close(&parg));
*/

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_TOOLARGCLASS_END = 0,
  NETWOX_TOOLARGCLASS_OPT,
  NETWOX_TOOLARGCLASS_REQ,
  NETWOX_TOOLARGCLASS_MORE
} netwox_toolargclass;

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_TOOLARGTYPE_BOOL = 1,
  NETWOX_TOOLARGTYPE_BOOL_RADIO1,
  NETWOX_TOOLARGTYPE_BOOL_RADIO2,
  NETWOX_TOOLARGTYPE_BOOL_RADIO3,
  NETWOX_TOOLARGTYPE_IP,
  NETWOX_TOOLARGTYPE_IP_SRC,
  NETWOX_TOOLARGTYPE_IP_DST,
  NETWOX_TOOLARGTYPE_IP4,
  NETWOX_TOOLARGTYPE_IP4_SRC,
  NETWOX_TOOLARGTYPE_IP4_DST,
  NETWOX_TOOLARGTYPE_IP6,
  NETWOX_TOOLARGTYPE_IP6_SRC,
  NETWOX_TOOLARGTYPE_IP6_DST,
  NETWOX_TOOLARGTYPE_ETH,
  NETWOX_TOOLARGTYPE_ETH_SRC,
  NETWOX_TOOLARGTYPE_ETH_DST,
  NETWOX_TOOLARGTYPE_PORT,
  NETWOX_TOOLARGTYPE_PORT_SRC,
  NETWOX_TOOLARGTYPE_PORT_DST,
  NETWOX_TOOLARGTYPE_IPS_SU, /* SORTUNIQ */
  NETWOX_TOOLARGTYPE_IPS_U, /* NOTSORTUNIQ */
  NETWOX_TOOLARGTYPE_IPS_N, /* NOTSORTNOTUNIQ */
  NETWOX_TOOLARGTYPE_ETHS_SU,
  NETWOX_TOOLARGTYPE_ETHS_U,
  NETWOX_TOOLARGTYPE_ETHS_N,
  NETWOX_TOOLARGTYPE_PORTS_SU,
  NETWOX_TOOLARGTYPE_PORTS_U,
  NETWOX_TOOLARGTYPE_PORTS_N,
  NETWOX_TOOLARGTYPE_UINT32,
  NETWOX_TOOLARGTYPE_INT32,
  NETWOX_TOOLARGTYPE_UINT64,
  NETWOX_TOOLARGTYPE_INT64,
  NETWOX_TOOLARGTYPE_BUF,
  NETWOX_TOOLARGTYPE_BUF_LOGIN,
  NETWOX_TOOLARGTYPE_BUF_PASSWORD,
  NETWOX_TOOLARGTYPE_BUF_DEVICE,
  NETWOX_TOOLARGTYPE_BUF_FILE,
  NETWOX_TOOLARGTYPE_BUF_FILE_RD,
  NETWOX_TOOLARGTYPE_BUF_FILE_WR,
  NETWOX_TOOLARGTYPE_BUF_DIR,
  NETWOX_TOOLARGTYPE_BUF_DIR_RD,
  NETWOX_TOOLARGTYPE_BUF_DIR_WR,
  NETWOX_TOOLARGTYPE_BUF_FILTER,
  NETWOX_TOOLARGTYPE_BUF_OID,
  NETWOX_TOOLARGTYPE_BUF_OIDTYPE,
  NETWOX_TOOLARGTYPE_BUF_MIXED,
  NETWOX_TOOLARGTYPE_BUF_IP4OPTS,
  NETWOX_TOOLARGTYPE_BUF_IP6EXTS,
  NETWOX_TOOLARGTYPE_BUF_TCPOPTS,
  NETWOX_TOOLARGTYPE_BUF_EMAIL,
  NETWOX_TOOLARGTYPE_BUF_URI,
  NETWOX_TOOLARGTYPE_BUF_NEWSGROUP,
  NETWOX_TOOLARGTYPE_BUF_HOSTNAME,
  NETWOX_TOOLARGTYPE_BUF_MD5,
  NETWOX_TOOLARGTYPE_BUF_USERAGENT,
  NETWOX_TOOLARGTYPE_BUF_NETBIOSNAME,
  NETWOX_TOOLARGTYPE_BUF_SMBSHARE,
  NETWOX_TOOLARGTYPE_ENCODE,
  NETWOX_TOOLARGTYPE_RECORDENCODE,
  NETWOX_TOOLARGTYPE_DECODE,
  NETWOX_TOOLARGTYPE_DLT,
  NETWOX_TOOLARGTYPE_SPOOFIP,
  NETWOX_TOOLARGTYPE_IPTYPE
} netwox_toolargtype;

/*-------------------------------------------------------------*/
netwib_err netwox_buf_append_toolargtype(netwox_toolargtype toolargtype,
                                         netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_buf_toolargtype_help(void);

/*-------------------------------------------------------------*/
typedef struct {
  netwox_toolargclass toolargclass;
  netwox_toolargtype toolargtype;
  netwib_bool advanced;
  netwib_char key;
  netwib_conststring name; /* if NULL, deduct from toolargtype */
  netwib_conststring description; /* if NULL, deduct from toolargtype */
  netwib_conststring defaultvalue; /* if NULL, deduct from toolargtype.
                                      Not needed if required=true, but it is
                                      used to propose a value in netwag */
} netwox_toolarg;
#define NETWOX_TOOLARG_END {NETWOX_TOOLARGCLASS_END,NETWOX_TOOLARGTYPE_BOOL,NETWIB_FALSE,'_',NULL,NULL,NULL}
#define NETWOX_TOOLARG_OPT(toolargtype,key,name,description,defaultvalue) {NETWOX_TOOLARGCLASS_OPT,toolargtype,NETWIB_FALSE,key,name,description,defaultvalue}
#define NETWOX_TOOLARG_OPTA(toolargtype,key,name,description,defaultvalue) {NETWOX_TOOLARGCLASS_OPT,toolargtype,NETWIB_TRUE,key,name,description,defaultvalue}
#define NETWOX_TOOLARG_REQ(toolargtype,key,name,description,defaultvalue) {NETWOX_TOOLARGCLASS_REQ,toolargtype,NETWIB_FALSE,key,name,description,defaultvalue}
#define NETWOX_TOOLARG_MORE(name,description) {NETWOX_TOOLARGCLASS_MORE,NETWOX_TOOLARGTYPE_BUF,NETWIB_FALSE,'_',name,description,NULL}

/*-------------------------------------------------------------*/
#define NETWOX_TOOLARG_OPT_BOOL(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BOOL,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BOOL_RADIO1(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BOOL_RADIO1,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BOOL_RADIO2(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BOOL_RADIO2,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BOOL_RADIO3(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BOOL_RADIO3,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_IP(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_IP,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_IP_SRC(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_IP_SRC,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_IP_DST(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_IP_DST,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_IP4(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_IP4,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_IP4_SRC(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_IP4_SRC,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_IP4_DST(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_IP4_DST,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_IP6(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_IP6,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_IP6_SRC(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_IP6_SRC,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_IP6_DST(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_IP6_DST,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_ETH(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_ETH,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_ETH_SRC(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_ETH_SRC,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_ETH_DST(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_ETH_DST,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_PORT(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_PORT,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_PORT_SRC(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_PORT_SRC,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_PORT_DST(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_PORT_DST,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_IPS_SU(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_IPS_SU,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_IPS_U(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_IPS_U,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_IPS_N(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_IPS_N,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_ETHS_SU(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_ETHS_SU,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_ETHS_U(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_ETHS_U,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_ETHS_N(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_ETHS_N,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_PORTS_SU(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_PORTS_SU,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_PORTS_U(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_PORTS_U,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_PORTS_N(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_PORTS_N,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_UINT32(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_UINT32,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_INT32(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_INT32,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_UINT64(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_UINT64,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_INT64(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_INT64,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BUF(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BUF,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BUF_LOGIN(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BUF_LOGIN,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BUF_PASSWORD(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BUF_PASSWORD,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BUF_DEVICE(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BUF_DEVICE,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BUF_FILE(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BUF_FILE,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BUF_FILE_RD(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BUF_FILE_RD,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BUF_FILE_WR(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BUF_FILE_WR,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BUF_DIR(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BUF_DIR,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BUF_DIR_RD(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BUF_DIR_RD,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BUF_DIR_WR(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BUF_DIR_WR,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BUF_FILTER(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BUF_FILTER,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BUF_OID(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BUF_OID,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BUF_OIDTYPE(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BUF_OIDTYPE,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BUF_MIXED(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BUF_MIXED,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BUF_IP4OPTS(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BUF_IP4OPTS,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BUF_IP6EXTS(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BUF_IP6EXTS,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BUF_TCPOPTS(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BUF_TCPOPTS,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BUF_EMAIL(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BUF_EMAIL,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BUF_URI(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BUF_URI,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BUF_NEWSGROUP(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BUF_NEWSGROUP,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BUF_HOSTNAME(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BUF_HOSTNAME,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BUF_MD5(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BUF_MD5,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BUF_USERAGENT(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BUF_USERAGENT,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BUF_NETBIOSNAME(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BUF_NETBIOSNAME,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_BUF_SMBSHARE(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_BUF_SMBSHARE,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_ENCODE(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_ENCODE,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_RECORDENCODE(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_RECORDENCODE,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_DECODE(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_DECODE,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_DLT(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_DLT,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_SPOOFIP(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_SPOOFIP,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPT_IPTYPE(key,name,description,defaultvalue) NETWOX_TOOLARG_OPT(NETWOX_TOOLARGTYPE_IPTYPE,key,name,description,defaultvalue)

/*-------------------------------------------------------------*/
#define NETWOX_TOOLARG_OPTA_BOOL(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BOOL,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_IP(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_IP,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_IP_SRC(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_IP_SRC,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_IP_DST(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_IP_DST,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_IP4(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_IP4,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_IP4_SRC(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_IP4_SRC,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_IP4_DST(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_IP4_DST,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_IP6(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_IP6,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_IP6_SRC(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_IP6_SRC,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_IP6_DST(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_IP6_DST,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_ETH(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_ETH,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_ETH_SRC(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_ETH_SRC,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_ETH_DST(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_ETH_DST,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_PORT(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_PORT,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_PORT_SRC(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_PORT_SRC,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_PORT_DST(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_PORT_DST,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_IPS_SU(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_IPS_SU,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_IPS_U(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_IPS_U,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_IPS_N(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_IPS_N,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_ETHS_SU(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_ETHS_SU,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_ETHS_U(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_ETHS_U,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_ETHS_N(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_ETHS_N,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_PORTS_SU(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_PORTS_SU,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_PORTS_U(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_PORTS_U,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_PORTS_N(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_PORTS_N,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_UINT32(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_UINT32,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_INT32(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_INT32,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_UINT64(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_UINT64,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_INT64(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_INT64,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_BUF(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BUF,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_BUF_LOGIN(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BUF_LOGIN,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_BUF_PASSWORD(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BUF_PASSWORD,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_BUF_DEVICE(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BUF_DEVICE,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_BUF_FILE(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BUF_FILE,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_BUF_FILE_RD(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BUF_FILE_RD,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_BUF_FILE_WR(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BUF_FILE_WR,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_BUF_DIR(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BUF_DIR,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_BUF_DIR_RD(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BUF_DIR_RD,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_BUF_DIR_WR(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BUF_DIR_WR,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_BUF_FILTER(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BUF_FILTER,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_BUF_OID(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BUF_OID,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_BUF_OIDTYPE(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BUF_OIDTYPE,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_BUF_MIXED(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BUF_MIXED,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_BUF_IP4OPTS(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BUF_IP4OPTS,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_BUF_IP6EXTS(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BUF_IP6EXTS,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_BUF_TCPOPTS(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BUF_TCPOPTS,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_BUF_EMAIL(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BUF_EMAIL,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_BUF_URI(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BUF_URI,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_BUF_NEWSGROUP(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BUF_NEWSGROUP,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_BUF_HOSTNAME(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BUF_HOSTNAME,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_BUF_MD5(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BUF_MD5,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_BUF_USERAGENT(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BUF_USERAGENT,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_BUF_NETBIOSNAME(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BUF_NETBIOSNAME,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_BUF_SMBSHARE(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_BUF_SMBSHARE,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_ENCODE(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_ENCODE,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_RECORDENCODE(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_RECORDENCODE,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_DECODE(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_DECODE,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_DLT(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_DLT,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_SPOOFIP(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_SPOOFIP,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_OPTA_IPTYPE(key,name,description,defaultvalue) NETWOX_TOOLARG_OPTA(NETWOX_TOOLARGTYPE_IPTYPE,key,name,description,defaultvalue)

/*-------------------------------------------------------------*/
#define NETWOX_TOOLARG_REQ_BOOL(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BOOL,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_IP(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_IP,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_IP_SRC(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_IP_SRC,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_IP_DST(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_IP_DST,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_IP4(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_IP4,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_IP4_SRC(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_IP4_SRC,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_IP4_DST(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_IP4_DST,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_IP6(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_IP6,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_IP6_SRC(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_IP6_SRC,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_IP6_DST(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_IP6_DST,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_ETH(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_ETH,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_ETH_SRC(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_ETH_SRC,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_ETH_DST(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_ETH_DST,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_PORT(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_PORT,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_PORT_SRC(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_PORT_SRC,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_PORT_DST(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_PORT_DST,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_IPS_SU(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_IPS_SU,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_IPS_U(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_IPS_U,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_IPS_N(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_IPS_N,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_ETHS_SU(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_ETHS_SU,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_ETHS_U(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_ETHS_U,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_ETHS_N(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_ETHS_N,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_PORTS_SU(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_PORTS_SU,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_PORTS_U(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_PORTS_U,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_PORTS_N(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_PORTS_N,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_UINT32(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_UINT32,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_INT32(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_INT32,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_UINT64(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_UINT64,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_INT64(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_INT64,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_BUF(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BUF,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_BUF_LOGIN(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BUF_LOGIN,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_BUF_PASSWORD(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BUF_PASSWORD,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_BUF_DEVICE(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BUF_DEVICE,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_BUF_FILE(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BUF_FILE,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_BUF_FILE_RD(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BUF_FILE_RD,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_BUF_FILE_WR(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BUF_FILE_WR,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_BUF_DIR(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BUF_DIR,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_BUF_DIR_RD(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BUF_DIR_RD,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_BUF_DIR_WR(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BUF_DIR_WR,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_BUF_FILTER(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BUF_FILTER,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_BUF_OID(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BUF_OID,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_BUF_OIDTYPE(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BUF_OIDTYPE,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_BUF_MIXED(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BUF_MIXED,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_BUF_IP4OPTS(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BUF_IP4OPTS,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_BUF_IP6EXTS(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BUF_IP6EXTS,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_BUF_TCPOPTS(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BUF_TCPOPTS,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_BUF_EMAIL(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BUF_EMAIL,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_BUF_URI(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BUF_URI,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_BUF_NEWSGROUP(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BUF_NEWSGROUP,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_BUF_HOSTNAME(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BUF_HOSTNAME,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_BUF_MD5(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BUF_MD5,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_BUF_USERAGENT(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BUF_USERAGENT,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_BUF_NETBIOSNAME(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BUF_NETBIOSNAME,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_BUF_SMBSHARE(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_BUF_SMBSHARE,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_ENCODE(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_ENCODE,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_RECORDENCODE(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_RECORDENCODE,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_DECODE(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_DECODE,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_DLT(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_DLT,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_SPOOFIP(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_SPOOFIP,key,name,description,defaultvalue)
#define NETWOX_TOOLARG_REQ_IPTYPE(key,name,description,defaultvalue) NETWOX_TOOLARG_REQ(NETWOX_TOOLARGTYPE_IPTYPE,key,name,description,defaultvalue)

/*-------------------------------------------------------------*/
netwib_err netwox_toolarg_get_name(const netwox_toolarg *ptoolarg,
                                   netwib_buf *pbuf);
netwib_err netwox_toolarg_get_description(const netwox_toolarg *ptoolarg,
                                          netwib_buf *pbuf);
netwib_err netwox_toolarg_get_default(const netwox_toolarg *ptoolarg,
                                      netwib_buf *pbuf);

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_TOOLARG_ENCODETYPE_HELPUSAGE = 1,
  NETWOX_TOOLARG_ENCODETYPE_HELPLONG,
  NETWOX_TOOLARG_ENCODETYPE_HELPEXAMPLE1,
  NETWOX_TOOLARG_ENCODETYPE_HELPEXAMPLE2,
  NETWOX_TOOLARG_ENCODETYPE_KBD1,
  NETWOX_TOOLARG_ENCODETYPE_KBD2,
  NETWOX_TOOLARG_ENCODETYPE_FORM
} netwox_toolarg_encodetype;
netwib_err netwox_buf_append_toolarg(const netwox_toolarg *ptoolarg,
                                     netwox_toolarg_encodetype encodetype,
                                     netwib_buf *pbuf);
