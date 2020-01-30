
/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_SAMPLE_IPUDPSYSLOG = 1,
  NETWOX_SAMPLE_IPTCPSYN,
  NETWOX_SAMPLE_IPTCPSYNACK,
  NETWOX_SAMPLE_IPTCPACK,
  NETWOX_SAMPLE_IPICMPPING
} netwox_sample;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_ip ipsrc;
  netwib_ip ipdst;
  netwib_bufext ip4opts;
  netwib_bufext ip6exts;
  netwib_port portdst;
  netwib_port portsrc;
} netwox_sample_info;

/*-------------------------------------------------------------*/
netwib_err netwox_sample_info_initdefault(netwox_sample_info *pinfo);

/*-------------------------------------------------------------*/
netwib_err netwox_sample_pkt(netwox_sample sample,
                             netwox_sample_info *pinfo,
                             netwib_buf *ppkt);

