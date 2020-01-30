
/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 maxvalue;
  netwib_uint32 displayeddots;
  netwib_char lastchar;
} netwox_scale;

/*-------------------------------------------------------------*/
netwib_err netwox_scale_init(netwib_uint32 maxvalue, /* 0 for none */
                             netwox_scale *pscale);
netwib_err netwox_scale_close(netwox_scale *pscale);

/*-------------------------------------------------------------*/
netwib_err netwox_scale_update(netwox_scale *pscale,
                               netwib_uint32 value);

