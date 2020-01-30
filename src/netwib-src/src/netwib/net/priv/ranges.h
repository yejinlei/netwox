
/*-------------------------------------------------------------*/
#define NETWIB_IPS_LEN 17
#define NETWIB_PORT_LEN 2
#define NETWIB_IP4_LEN 4

/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_PRIV_RANGES_INITTYPE_SORTUNIQ = 1,
  NETWIB_PRIV_RANGES_INITTYPE_NOTSORTUNIQ = 2,
  NETWIB_PRIV_RANGES_INITTYPE_NOTSORTNOTUNIQ = 3
} netwib_priv_ranges_inittype;

/*-------------------------------------------------------------*/
#define NETWIB_PRIV_RANGES_MAXITEMSIZE NETWIB_IPS_LEN
typedef struct {
  netwib_priv_ranges_inittype inittype;
  netwib_uint32 itemsize; /* ETH==6, IP==16+1, port==2 */
  netwib_uint32 rangesize; /* == 2*itemsize, but used frequently */
  netwib_uint32 ptrallocsize;
  netwib_data ptr;
  netwib_uint32 numranges;
} netwib_priv_ranges;
typedef const netwib_priv_ranges netwib_priv_constranges;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ranges_init(netwib_priv_ranges_inittype inittype,
                                   netwib_uint32 itemsize,
                                   netwib_priv_ranges *pr);
netwib_err netwib_priv_ranges_close(netwib_priv_ranges *pr);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ranges_add(netwib_priv_ranges *pr,
                                  netwib_constdata ptr);
netwib_err netwib_priv_ranges_add_range(netwib_priv_ranges *pr,
                                        netwib_constdata ptrinf,
                                        netwib_constdata ptrsup);
netwib_err netwib_priv_ranges_add_all(netwib_priv_ranges *pr);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ranges_del(netwib_priv_ranges *pr,
                                  netwib_constdata ptr);
netwib_err netwib_priv_ranges_del_range(netwib_priv_ranges *pr,
                                        netwib_constdata ptrinf,
                                        netwib_constdata ptrsup);
netwib_err netwib_priv_ranges_del_all(netwib_priv_ranges *pr);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ranges_contains(netwib_priv_constranges *pr,
                                       netwib_constdata ptr,
                                       netwib_bool *pyes);
netwib_err netwib_priv_ranges_contains_range(netwib_priv_constranges *pr,
                                             netwib_constdata ptrinf,
                                             netwib_constdata ptrsup,
                                             netwib_bool *pyes);

/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_PRIV_RANGES_SEPARATOR_NONE = 1,
  NETWIB_PRIV_RANGES_SEPARATOR_DASH,
  NETWIB_PRIV_RANGES_SEPARATOR_SLASH,
  NETWIB_PRIV_RANGES_SEPARATOR_SLASH_NUM,
  NETWIB_PRIV_RANGES_SEPARATOR_PERCENT,
  NETWIB_PRIV_RANGES_SEPARATOR_PERCENT_NUM
} netwib_priv_ranges_separator;
typedef netwib_err (*netwib_priv_ranges_add_buf_pf)(netwib_conststring str,
                                                    netwib_data array,
                                                    netwib_priv_ranges_separator
                                                    *pseparator);
netwib_err netwib_priv_ranges_add_buf(netwib_priv_ranges *pr,
                                      netwib_constbuf *pbuf,
                                      netwib_priv_ranges_add_buf_pf pfunc);

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
typedef struct {
  netwib_priv_ranges *pr;
  netwib_bool lastset;
  netwib_uint32 lastindex;
  netwib_byte lastinfitem[NETWIB_PRIV_RANGES_MAXITEMSIZE];
  netwib_byte lastsupitem[NETWIB_PRIV_RANGES_MAXITEMSIZE];
} netwib_priv_ranges_index;

/*-------------------------------------------------------------*/
/* pr is constant, but _del function will change it */
netwib_err netwib_priv_ranges_index_init(netwib_priv_constranges *pr,
                                         netwib_priv_ranges_index *pri);
netwib_err netwib_priv_ranges_index_close(netwib_priv_ranges_index *pri);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ranges_index_rewind(netwib_priv_ranges_index *pri);
netwib_err netwib_priv_ranges_index_index(netwib_priv_ranges_index *pri,
                                          netwib_priv_ranges_index *priref);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ranges_index_next(netwib_priv_ranges_index *pri,
                                         netwib_data ptr);
netwib_err netwib_priv_ranges_index_next_range(netwib_priv_ranges_index *pri,
                                               netwib_data ptrinf,
                                               netwib_data ptrsup);
netwib_err netwib_priv_ranges_index_this(netwib_priv_ranges_index *pri,
                                         netwib_data ptr);
netwib_err netwib_priv_ranges_index_this_range(netwib_priv_ranges_index *pri,
                                               netwib_data ptrinf,
                                               netwib_data ptrsup);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ranges_index_this_del(netwib_priv_ranges_index *pri);

/*-------------------------------------------------------------*/
struct netwib_ips {
  netwib_priv_ranges ranges;
};
struct netwib_eths {
  netwib_priv_ranges ranges;
};
struct netwib_ports {
  netwib_priv_ranges ranges;
};
