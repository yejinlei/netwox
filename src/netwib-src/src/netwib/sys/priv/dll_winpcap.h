
/*-------------------------------------------------------------*/
typedef int bpf_int32;
typedef u_int bpf_u_int32;
typedef struct pcap pcap_t;
typedef struct pcap_dumper pcap_dumper_t;
typedef struct pcap_if pcap_if_t;
typedef struct pcap_addr pcap_addr_t;

struct pcap_pkthdr {
  struct timeval ts;
  bpf_u_int32 caplen;
  bpf_u_int32 len;
};

struct pcap_if {
  struct pcap_if *next;
  char *name;
  char *description;
  struct pcap_addr *addresses;
  bpf_u_int32 flags;
};
#define PCAP_IF_LOOPBACK 0x00000001

struct pcap_addr {
  struct pcap_addr *next;
  struct sockaddr *addr;
  struct sockaddr *netmask;
  struct sockaddr *broadaddr;
  struct sockaddr *dstaddr;
};

#define PCAP_ERRBUF_SIZE 256

struct bpf_program {
  u_int bf_len;
  void *bf_insns;
};

#define DLT_NULL        0       /* no link-layer encapsulation */
#define DLT_EN10MB      1       /* Ethernet (10Mb) */
#define DLT_EN3MB       2       /* Experimental Ethernet (3Mb) */
#define DLT_AX25        3       /* Amateur Radio AX.25 */
#define DLT_PRONET      4       /* Proteon ProNET Token Ring */
#define DLT_CHAOS       5       /* Chaos */
#define DLT_IEEE802     6       /* IEEE 802 Networks */
#define DLT_ARCNET      7       /* ARCNET, with BSD-style header */
#define DLT_SLIP        8       /* Serial Line IP */
#define DLT_PPP         9       /* Point-to-point Protocol */
#define DLT_FDDI        10      /* FDDI */

/*-------------------------------------------------------------*/
typedef void (*ppcap_close) (pcap_t *);
typedef int (*ppcap_compile) (pcap_t *, struct bpf_program *, char *, int, bpf_u_int32);
typedef int (*ppcap_datalink) (pcap_t *);
typedef void (*ppcap_dump) (u_char *, const struct pcap_pkthdr *, const u_char *);
typedef void (*ppcap_dump_close) (pcap_dumper_t *);
typedef pcap_dumper_t* (*ppcap_dump_open) (pcap_t *, const char *);
typedef int (*ppcap_fileno) (pcap_t *);
typedef int (*ppcap_findalldevs) (pcap_if_t **, char *);
typedef void (*ppcap_freealldevs) (pcap_if_t *);
typedef void (*ppcap_freecode) (struct bpf_program *);
typedef int (*ppcap_lookupnet) (const char *, bpf_u_int32 *, bpf_u_int32 *, char *);
typedef char* (*ppcap_geterr) (pcap_t *);
typedef HANDLE (*ppcap_getevent) (pcap_t *p);
typedef const u_char* (*ppcap_next) (pcap_t *, struct pcap_pkthdr *);
typedef pcap_t* (*ppcap_open_dead) (int p1, int p2);
typedef pcap_t* (*ppcap_open_live) (const char *, int, int, int, char *);
typedef pcap_t* (*ppcap_open_offline) (const char *, char *);
typedef int (*ppcap_setfilter) (pcap_t *, struct bpf_program *);
typedef int (*ppcap_setmintocopy) (pcap_t *p, int size);

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bool isloaded;
  HINSTANCE hinstance;
  ppcap_close pcap_close;
  ppcap_compile pcap_compile;
  ppcap_datalink pcap_datalink;
  ppcap_dump pcap_dump;
  ppcap_dump_close pcap_dump_close;
  ppcap_dump_open pcap_dump_open;
  ppcap_fileno pcap_fileno;
  ppcap_findalldevs pcap_findalldevs;
  ppcap_freealldevs pcap_freealldevs;
  ppcap_freecode pcap_freecode;
  ppcap_lookupnet pcap_lookupnet;
  ppcap_geterr pcap_geterr;
  ppcap_getevent pcap_getevent;
  ppcap_next pcap_next;
  ppcap_open_dead pcap_open_dead;
  ppcap_open_live pcap_open_live;
  ppcap_open_offline pcap_open_offline;
  ppcap_setfilter pcap_setfilter;
  ppcap_setmintocopy pcap_setmintocopy;
} netwib_priv_dll_winpcap;
extern netwib_priv_dll_winpcap netwib_global_dll_winpcap;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_winpcap_init(netwib_priv_dll_winpcap *pdll_winpcap);
netwib_err netwib_priv_dll_winpcap_close(netwib_priv_dll_winpcap *pdll_winpcap);
