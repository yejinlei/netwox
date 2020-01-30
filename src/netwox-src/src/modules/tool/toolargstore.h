
/*-------------------------------------------------------------*/
typedef struct {
  netwox_toolargtype type;
  union {
    netwib_bool b;
    netwib_ip ip;
    netwib_eth eth;
    netwib_port port;
    netwib_ips *pips;
    netwib_eths *peths;
    netwib_ports *pports;
    netwib_uint32 u32;
    netwib_int32 i32;
    netwib_uint64 u64;
    netwib_int64 i64;
    netwib_buf buf;
    netwib_encodetype encode;
    netwib_record_encodetype recordencode;
    netwib_decodetype decode;
    netwib_device_dlttype dlt;
    netwib_spoof_ip_inittype spoofip;
    netwib_iptype iptype;
  } value;
} netwox_toolargstore;
netwib_err netwox_toolargstore_init(const netwox_toolarg *ptoolarg,
                                    netwox_toolargstore *ptoolargstore);
netwib_err netwox_toolargstore_close(netwox_toolargstore *ptoolargstore);
netwib_err netwox_toolargstore_reset(netwox_toolargstore *ptoolargstore);
netwib_err netwox_toolargstore_set(netwib_constbuf *pbuf,
                                   netwox_toolargstore *ptoolargstore);
netwib_err netwox_toolargstore_set_string(netwib_conststring value,
                                          netwox_toolargstore *ptoolargstore);
netwib_err netwox_toolargstore_set_kbd(const netwox_toolarg *ptoolarg,
                                       netwox_toolargstore *ptoolargstore);
netwib_err netwox_buf_append_toolargstore(const netwox_toolargstore *ptoolargstore,
                                          netwib_buf *pbuf);
netwib_err netwox_toolargstore_display(const netwox_toolargstore *ptoolargstore);
