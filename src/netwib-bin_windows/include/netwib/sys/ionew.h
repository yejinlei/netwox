
/*-------------------------------------------------------------*/
/***************************************************************
 * Functions herein allow to create a new netwib_io.           *
 ***************************************************************/

/*-------------------------------------------------------------*/
/***************************************************************
 * Following functions can return :                            *
 *  - NETWIB_ERR_OK : the job was done (by the io or a next in *
 *                    the chain)                               *
 *  - NETWIB_ERR_PLEASETRYNEXT : the io doesn't know how to do *
 *                               what was requested, so please *
 *                               try next                      *
 *  - NETWIB_ERR_PLEASECONSTRUCT : if way is                   *
 *                                 NETWIB_IO_WAYTYPE_RDWR or   *
 *                                _SUPPORTED, the library has  *
 *                                to do the task using _READ   *
 *                                and _WRITE                   *
 *  - NETWIB_ERR_PLEASELOOPTIME : there is no event, so please *
 *                                loop to reach abstime        *
 ***************************************************************/

/* Function called when netwib_io_read is called on the io
   This function should return :
     NETWIB_ERR_OK
     NETWIB_ERR_PLEASETRYNEXT
*/
typedef netwib_err (*netwib_io_read_pf)(netwib_io *pio,
                                        netwib_buf *pbuf);

/* Function called when netwib_io_write is called on the io
   This function should return :
     NETWIB_ERR_OK
     NETWIB_ERR_PLEASETRYNEXT
*/
typedef netwib_err (*netwib_io_write_pf)(netwib_io *pio,
                                         netwib_constbuf *pbuf);

/* Function called when netwib_io_wait is called on the io
   This function should return :
     NETWIB_ERR_OK
     NETWIB_ERR_PLEASETRYNEXT
     NETWIB_ERR_PLEASECONSTRUCT
     NETWIB_ERR_PLEASELOOPTIME
*/
typedef netwib_err (*netwib_io_wait_pf)(netwib_io *pio,
                                        netwib_io_waytype way,
                                        netwib_consttime *pabstime,
                                        netwib_bool *pevent);

/* Function called when netwib_io_unread is called on the io
   This function should return :
     NETWIB_ERR_OK
     NETWIB_ERR_PLEASETRYNEXT
*/
typedef netwib_err (*netwib_io_unread_pf)(netwib_io *pio,
                                          netwib_constbuf *pbuf);

/* Function called when netwib_io_ctl_set is called on the io
   This function should return :
     NETWIB_ERR_OK
     NETWIB_ERR_PLEASETRYNEXT
     NETWIB_ERR_PLEASECONSTRUCT
*/
typedef netwib_err (*netwib_io_ctl_set_pf)(netwib_io *pio,
                                           netwib_io_waytype way,
                                           netwib_io_ctltype ctltype,
                                           netwib_ptr p,
                                           netwib_uint32 ui);
/* Function called when netwib_io_ctl_get is called on the io
   This function should return :
     NETWIB_ERR_OK
     NETWIB_ERR_PLEASETRYNEXT
     NETWIB_ERR_PLEASECONSTRUCT
*/
typedef netwib_err (*netwib_io_ctl_get_pf)(netwib_io *pio,
                                           netwib_io_waytype way,
                                           netwib_io_ctltype ctltype,
                                           netwib_ptr p,
                                           netwib_uint32 *pui);

/* Function called when netwib_io_close is called on the io
   This function should return :
     NETWIB_ERR_OK
*/
typedef netwib_err (*netwib_io_close_pf)(netwib_io *pio);

/*-------------------------------------------------------------*/
/* Name : netwib_io_init
   Description :
     Create a user defined netwib_io.
   Input parameter(s) :
     readsupported : read is supported
     writesupported : write is supported
     pcommon : common data which can be shared between functions
     pfx : functions or NULL if not needed
   Input/output parameter(s) :
   Output parameter(s) :
     **ppio : io created
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_io_init(netwib_bool readsupported,
                          netwib_bool writesupported,
                          netwib_ptr pcommon,
                          netwib_io_read_pf pfread,
                          netwib_io_write_pf pfwrite,
                          netwib_io_wait_pf pfwait,
                          netwib_io_unread_pf pfunread,
                          netwib_io_ctl_set_pf pfctl_set,
                          netwib_io_ctl_get_pf pfctl_get,
                          netwib_io_close_pf pfclose,
                          netwib_io **ppio);

/*-------------------------------------------------------------*/
/***************************************************************
 * Those structure definitions should only be used in functions*
 * for netwib_io_init.                                         *
 ***************************************************************/
typedef struct {
  netwib_io *pnext; /* next io in the chain */
  netwib_bool supported; /* true if way is supported */
  netwib_uint32 numusers; /* number of io using this one */
} netwib_io_way_t;
struct netwib_io {
  netwib_io_way_t rd; /* read information */
  netwib_io_way_t wr; /* write information */
  netwib_ptr pcommon; /* pointer used in netwib_io_init */
  netwib_io_write_pf pfwrite;
  netwib_io_read_pf pfread;
  netwib_io_unread_pf pfunread;
  netwib_io_wait_pf pfwait;
  netwib_io_ctl_set_pf pfctl_set;
  netwib_io_ctl_get_pf pfctl_get;
  netwib_io_close_pf pfclose;
};

/*-------------------------------------------------------------*/
/***************************************************************
 * Previous structure is useful to do simple things. But,      *
 * it's complicated to deal with several netwib_io_waytype.    *
 * Those defines can be used :                                 *
 *  - to work on "pnext", use netwib_io_next (in io.h)         *
 *  - to work on "supported", use netwib_io_ctl_s/get_support  *
 *  - to work on "numusers", use netwib_io_ctl_s/get_numusers  *
 ***************************************************************/

/* netwib_err f(netwib_io *pio, netwib_io_waytype way, netwib_bool yes); */
#define netwib_io_ctl_set_support(pio,way,yes) netwib_io_ctl_set(pio,way,NETWIB_IO_CTLTYPE_SUPPORT,NULL,(netwib_uint32)yes)
#define netwib_io_ctl_get_support(pio,way,pyes) netwib_io_ctl_get(pio,way,NETWIB_IO_CTLTYPE_SUPPORT,NULL,(netwib_uint32*)pyes)

/* netwib_err f(netwib_io *pio,netwib_io_waytype way,netwib_uint32 numusers);*/
#define netwib_io_ctl_set_numusers(pio,way,numusers) netwib_io_ctl_set(pio,way,NETWIB_IO_CTLTYPE_NUMUSERS,NULL,numusers)
#define netwib_io_ctl_get_numusers(pio,way,pnumusers) netwib_io_ctl_get(pio,way,NETWIB_IO_CTLTYPE_NUMUSERS,NULL,pnumusers)
/* only increment or decrement */
#define netwib_io_ctl_set_numusers_inc(pio,way) netwib_io_ctl_set(pio,way,NETWIB_IO_CTLTYPE_NUMUSERSINC,NULL,+1)
#define netwib_io_ctl_set_numusers_dec(pio,way) netwib_io_ctl_set(pio,way,NETWIB_IO_CTLTYPE_NUMUSERSINC,NULL,(netwib_uint32)-1)
