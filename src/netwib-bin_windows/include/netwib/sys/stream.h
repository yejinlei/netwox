
/*-------------------------------------------------------------*/
/* Name : netwib_io_init_stream
   Description :
     Open a file descriptor.
   Input parameter(s) :
     pfile : stream
   Input/output parameter(s) :
     closefileatend : if pfile have to be closed when the io is
                      closed
   Output parameter(s) :
     **ppio : io created
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_io_init_stream(NETWIBFILE *pfile,
                                 netwib_bool closefileatend,
                                 netwib_io **ppio);
