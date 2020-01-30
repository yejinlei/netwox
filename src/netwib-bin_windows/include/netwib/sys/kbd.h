
/*-------------------------------------------------------------*/
/* Name : netwib_io_init_kbd_xyz
   Description :
     Open the keyboard. Its default state is to echo pressed
     keys and to read line by line.
   Input parameter(s) :
     fd : file descriptor to eventually use
     h : HANDLE to eventually use
   Input/output parameter(s) :
   Output parameter(s) :
     **ppio : io created
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_io_init_kbddefault(netwib_io **ppio);
netwib_err netwib_io_init_kbd_fd(int fd,
                                 netwib_io **ppio);
netwib_err netwib_io_init_kbd_handle(NETWIBHANDLE h,
                                     netwib_io **ppio);

/*-------------------------------------------------------------*/
/* decides if pressed keys have to be displayed */
/* netwib_err f(netwib_io *pio, netwib_bool b); */
#define netwib_kbd_ctl_set_echo(pio,b) netwib_io_ctl_set(pio,NETWIB_IO_WAYTYPE_READ,NETWIB_IO_CTLTYPE_KBD_ECHO,NULL,b)
/* netwib_err f(netwib_io *pio, netwib_bool *pb); */
#define netwib_kbd_ctl_get_echo(pio,pb) netwib_io_ctl_get(pio,NETWIB_IO_WAYTYPE_READ,NETWIB_IO_CTLTYPE_KBD_ECHO,NULL,pb)
/* decides if we read line by line or char by char */
/* netwib_err f(netwib_io *pio, netwib_bool b); */
#define netwib_kbd_ctl_set_line(pio,b) netwib_io_ctl_set(pio,NETWIB_IO_WAYTYPE_READ,NETWIB_IO_CTLTYPE_KBD_LINE,NULL,b)
/* netwib_err f(netwib_io *pio, netwib_bool *pb); */
#define netwib_kbd_ctl_get_line(pio,pb) netwib_io_ctl_get(pio,NETWIB_IO_WAYTYPE_READ,NETWIB_IO_CTLTYPE_KBD_LINE,NULL,pb)
/* purge pressed characters not yet read */
/* netwib_err f(netwib_io *pio); */
#define netwib_kbd_ctl_set_purge(pio) netwib_io_ctl_set(pio,NETWIB_IO_WAYTYPE_READ,NETWIB_IO_CTLTYPE_KBD_PURGE,NULL,0)

