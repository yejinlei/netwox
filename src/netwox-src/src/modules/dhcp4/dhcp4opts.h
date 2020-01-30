
/*-------------------------------------------------------------*/
netwib_err netwox_dhcp4opts_show(netwib_constbuf *pdhcp4opts,
                                 netwib_encodetype encodetype,
                                 netwib_buf *pbuf);

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bool msgtypeset;
  netwox_dhcp4opt_msgtype msgtype;
  netwib_bool serveridset;
  netwib_ip serverid;
  netwib_bool ipadleasetimeset;
  netwib_uint32 ipadleasetime;
} netwox_dhcp4optscommon;
netwib_err netwox_dhcp4opts_decode_common(netwib_constbuf *pdhcp4opts,
                                          netwox_dhcp4optscommon *pcommon);
