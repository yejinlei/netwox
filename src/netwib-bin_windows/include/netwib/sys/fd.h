
/*-------------------------------------------------------------*/
/* Name : netwib_io_init_fd
   Description :
     Open a file descriptor.
   Input parameter(s) :
     fd : file descriptor
   Input/output parameter(s) :
     closefdatend : if fd have to be closed when the io is
                    closed
   Output parameter(s) :
     **ppio : io created
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_io_init_fd(int fd,
                             netwib_bool closefdatend,
                             netwib_io **ppio);

