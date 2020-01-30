
/*-------------------------------------------------------------*/
netwib_err netwox_io_kbd_loop(netwib_io *pio,
                              netwib_bool linebyline,
                              netwib_bool echochars,
                              netwib_decodetype decodetype,
                              netwib_encodetype encodetype);

/*-------------------------------------------------------------*/
#define NETWOX_IO_CTLTYPE_SOCK_LOCALETH NETWIB_IO_CTLTYPE_USER_BEGIN+1
#define NETWOX_IO_CTLTYPE_SOCK_REMOTEETH NETWIB_IO_CTLTYPE_USER_BEGIN+2
#define NETWOX_IO_CTLTYPE_SOCK_DEVICE NETWIB_IO_CTLTYPE_USER_BEGIN+3

/*-------------------------------------------------------------*/
netwib_err netwox_io_showecho(netwib_io *pio,
                              netwib_uint32 maxwaitms,
                              netwib_bool showscreen,
                              netwib_bool echoback);
