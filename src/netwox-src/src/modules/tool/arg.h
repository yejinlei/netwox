
/*-------------------------------------------------------------*/
netwib_err netwox_main(int argc, char *argv[]);

/*-------------------------------------------------------------*/
typedef struct {
  netwib_char key;
  netwib_char name[32]; /* length will be checked */
  netwib_bool required;
  netwib_bool setbyuser;
  netwib_bool setbydefault;
  const netwox_toolarg *ptoolarg;
  netwox_toolargstore toolargstore;
} netwox_arg_item;

typedef struct {
  netwox_tool_info *ptoolinfo;
  netwib_uint32 argtotalnum;
  netwib_uint32 argsetnum;
  netwib_uint32 keyindex[256]; /* index in the array, or -1 */
  netwox_arg_item *arg; /* array */
  netwib_bool allowmorearg;
  netwib_uint32 moreargc;
  netwib_bool moreargvisallocated;
  netwib_bool moreargvitemsareallocated;
  netwib_char **moreargv;
} netwox_arg;

/*-------------------------------------------------------------*/
netwib_err netwox_arg_init2(int argc,
                            char *argv[],
                            netwox_tool_info *ptoolinfo,
                            netwib_bool formupdate,
                            netwox_arg **pparg);
netwib_err netwox_arg_init(int argc,
                           char *argv[],
                           netwox_tool_info *ptoolinfo,
                           netwox_arg **pparg);
netwib_err netwox_arg_close(netwox_arg **pparg);

/*-------------------------------------------------------------*/
netwib_err netwox_arg_isset(netwox_arg *parg,
                            netwib_char key,
                            netwib_bool *pisset);

/*-------------------------------------------------------------*/
netwib_err netwox_arg_bool(netwox_arg *parg,
                           netwib_char key,
                           netwib_bool *pbool);
netwib_err netwox_arg_ip(netwox_arg *parg,
                         netwib_char key,
                         netwib_ip *pip);
netwib_err netwox_arg_eth(netwox_arg *parg,
                          netwib_char key,
                          netwib_eth *peth);
netwib_err netwox_arg_port(netwox_arg *parg,
                           netwib_char key,
                           netwib_port *pport);
netwib_err netwox_arg_uint64(netwox_arg *parg,
                             netwib_char key,
                             netwib_uint64 *puint64);
netwib_err netwox_arg_uint32(netwox_arg *parg,
                             netwib_char key,
                             netwib_uint32 *puint32);
netwib_err netwox_arg_uint16(netwox_arg *parg,
                             netwib_char key,
                             netwib_uint16 *puint16);
netwib_err netwox_arg_uint8(netwox_arg *parg,
                            netwib_char key,
                            netwib_uint8 *puint8);
netwib_err netwox_arg_int64(netwox_arg *parg,
                            netwib_char key,
                            netwib_int64 *pint64);
netwib_err netwox_arg_int32(netwox_arg *parg,
                            netwib_char key,
                            netwib_int32 *pint32);
netwib_err netwox_arg_encode(netwox_arg *parg,
                             netwib_char key,
                             netwib_encodetype *pencodetype);
netwib_err netwox_arg_recordencode(netwox_arg *parg,
                                   netwib_char key,
                                   netwib_record_encodetype *pencodetype);
netwib_err netwox_arg_decode(netwox_arg *parg,
                             netwib_char key,
                             netwib_decodetype *pdecodetype);
netwib_err netwox_arg_dlt(netwox_arg *parg,
                          netwib_char key,
                          netwib_device_dlttype *pdlt);
netwib_err netwox_arg_spoofip(netwox_arg *parg,
                              netwib_char key,
                              netwib_spoof_ip_inittype *ptype);
netwib_err netwox_arg_iptype(netwox_arg *parg,
                             netwib_char key,
                             netwib_iptype *ptype);
/* Warning : next 4 ones internally allocate and close
   parameter. So user must not close with ie netwib_buf_close(pbuf).
   Function netwox_arg_close() also mustn't be called before
   the end of parameter usage.
*/
netwib_err netwox_arg_ips(netwox_arg *parg,
                          netwib_char key,
                          netwib_ips **ppips);
netwib_err netwox_arg_eths(netwox_arg *parg,
                           netwib_char key,
                           netwib_eths **ppeths);
netwib_err netwox_arg_ports(netwox_arg *parg,
                            netwib_char key,
                            netwib_ports **ppports);
netwib_err netwox_arg_buf(netwox_arg *parg,
                          netwib_char key,
                          netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_arg_more_count(netwox_arg *parg,
                                 netwib_uint32 *pcount);
netwib_err netwox_arg_more(netwox_arg *parg,
                           netwib_uint32 i, /* 0 to count-1 */
                           netwib_buf *pbuf);

/*-------------------------------------------------------------*/
#define NETWOX_TOOLARG_RADIO1(key,name,description) NETWOX_TOOLARG_OPT_BOOL_RADIO1(key,name,description,NULL)
#define NETWOX_TOOLARG_RADIO2(key,name,description) NETWOX_TOOLARG_OPT_BOOL_RADIO2(key,name,description,NULL)
#define NETWOX_TOOLARG_RADIO3(key,name,description) NETWOX_TOOLARG_OPT_BOOL_RADIO3(key,name,description,NULL)
#define NETWOX_TOOLARG_RADIO1_SET(key,name,description) NETWOX_TOOLARG_OPT_BOOL_RADIO1(key,name,description,"1")
#define NETWOX_TOOLARG_RADIO2_SET(key,name,description) NETWOX_TOOLARG_OPT_BOOL_RADIO2(key,name,description,"1")
#define NETWOX_TOOLARG_RADIO3_SET(key,name,description) NETWOX_TOOLARG_OPT_BOOL_RADIO3(key,name,description,"1")
netwib_err netwox_arg_radio(netwox_arg *parg,
                            netwib_uint32 radionum,
                            netwib_char *pc);
#define netwox_arg_radio1(parg,pc) netwox_arg_radio(parg,1,pc)
#define netwox_arg_radio2(parg,pc) netwox_arg_radio(parg,2,pc)
#define netwox_arg_radio3(parg,pc) netwox_arg_radio(parg,3,pc)

