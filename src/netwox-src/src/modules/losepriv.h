
/*-------------------------------------------------------------*/
/* This function can not be used for spoofing, even if the netwib_io
   is open. Indeed, the internal fd is automagically reopened
   and the IP->Eth resolution (to fill the Ethernet addresses) also
   needs to spoof. In order to correct this, a global netwib_io
   must be opened for each device before loosing privileges. This
   is too complicated for now.
*/
netwib_err netwox_losepriv(void);

/*-------------------------------------------------------------*/
#define NETWOX_LOSEPRIV_TOOLARG_COMMON(q) NETWOX_TOOLARG_OPTA_BOOL(q, "losepriv", "lose privileges to nobody user under Linux", "0")
