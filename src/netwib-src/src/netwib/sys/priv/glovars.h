
/*-------------------------------------------------------------*/
typedef struct {
  netwib_buf errmsg; /* This variable should be thread specific
                        instead of global. Its access is correctly
                        protected by locks (netwib_priv_errmsg_...)
                        so till now, it always worked sufficiently
                        well. The day we'll encounter a problem,
                        a thread specific storage will be implemented.
                     */
  netwib_uint64 rand_seed;
  netwib_bool time_isdst; /* read only */
  netwib_int32 time_zoneoffset; /* read only */
  netwib_bool debug_ctrlc_pressed; /* read only */
} netwib_priv_glovars_t;
extern netwib_priv_glovars_t netwib_priv_glovars;
netwib_err netwib_priv_glovars_init(void);
netwib_err netwib_priv_glovars_close(void);
netwib_err netwib_priv_glovars_rdlock(void);
netwib_err netwib_priv_glovars_rdunlock(void);
netwib_err netwib_priv_glovars_wrlock(void);
netwib_err netwib_priv_glovars_wrunlock(void);
netwib_err netwib_priv_glovars_canuse(netwib_bool *pyes);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_glovars_other_rdlock(void);
netwib_err netwib_priv_glovars_other_rdunlock(void);
netwib_err netwib_priv_glovars_other_wrlock(void);
netwib_err netwib_priv_glovars_other_wrunlock(void);
