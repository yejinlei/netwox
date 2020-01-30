
/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_TRACERTDISC_METHOD_TCP = 1,
  NETWOX_TRACERTDISC_METHOD_UDP,
  NETWOX_TRACERTDISC_METHOD_ICMP
} netwox_tracertdisc_method;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 hopcount;
  netwib_ip hopip;
  netwib_bool isdst;
  netwib_bool portisopen; /* only know if isdst is TRUE */
  netwib_bool isfake; /* only to indicate the way to a host */
} netwox_tracertdisc_hoplist_hop;
netwib_err netwox_tracertdisc_hoplist_hop_create(netwib_ptr *ppitem);
netwib_err netwox_tracertdisc_hoplist_hop_erase(netwib_ptr pitem);
#define netwox_tracertdisc_hoplist_ring_init(ppring) netwib_ring_init(&netwox_tracertdisc_hoplist_hop_erase,NULL,ppring)
#define netwox_tracertdisc_hoplist_ring_close(ppring) netwib_ring_close(ppring,NETWIB_TRUE)
#define netwox_tracertdisc_hoplist_ring_delall(ppring) netwib_ring_del_all(ppring,NETWIB_TRUE)
netwib_err netwox_tracertdisc_hoplist_ring_add(netwib_ring *pring,
                                               netwib_uint32 hopcount,
                                               netwib_constip *phopip,
                                               netwib_bool isdst,
                                               netwib_bool portisopen,
                                               netwib_bool isfake);
netwib_err netwox_tracertdisc_hoplist_ring_display(netwib_ring *pring,
                                                   netwox_tracertdisc_method method);

/*-------------------------------------------------------------*/
typedef struct {
  netwox_tracert_addinfo addinfo;
  netwib_bool ethspoof;
  netwib_ips *pdstips;
  netwib_ports *pdsttcpports;
  netwib_ports *pdstudpports;
  netwib_bool useicmp;
  netwib_bool verbose;
} netwox_tracertdisc;

/*-------------------------------------------------------------*/
netwib_err netwox_tracertdisc_loop(netwox_tracertdisc *ptracertdisc);

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
typedef struct {
  netwib_bool resolve;
  netwib_bool verbose;
  netwox_dnscache dnscache;
#define NETWOX_TRACERTDISC_GRAPH_MAXLEVEL 256
  netwib_ring *levels[NETWOX_TRACERTDISC_GRAPH_MAXLEVEL];
  netwib_uint32 lastdisplaynumber;
  netwib_buf pad;
  netwib_ips *pipsreached;
  netwib_bool titledisplayed;
} netwox_tracertdisc_graph;
netwib_err netwox_tracertdisc_graph_init(netwox_tracertdisc *ptracertdisc,
                                         netwox_tracertdisc_graph *pgraph);
netwib_err netwox_tracertdisc_graph_close(netwox_tracertdisc_graph *pgraph);
netwib_err netwox_tracertdisc_graph_add(netwox_tracertdisc_graph *pgraph,
                                        netwib_ring *phoplist,
                                        netwox_tracertdisc_method method,
                                        netwib_port port);
netwib_err netwox_tracertdisc_graph_display(netwox_tracertdisc_graph *pgraph);
netwib_err netwox_tracertdisc_graph_test(netwox_tracertdisc *ptracertdisc);


