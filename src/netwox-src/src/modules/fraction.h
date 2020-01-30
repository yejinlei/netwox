
/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 displayedchars;
} netwox_fraction;

/*-------------------------------------------------------------*/
netwib_err netwox_fraction_init(netwox_fraction *pfraction);
netwib_err netwox_fraction_close(netwox_fraction *pfraction);

/*-------------------------------------------------------------*/
netwib_err netwox_fraction_update(netwox_fraction *pfraction,
                                  netwib_uint32 value,
                                  netwib_uint32 maxvalue);

