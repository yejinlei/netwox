
/*-------------------------------------------------------------*/
/***************************************************************
 * Functions herein are examples of frequently needed          *
 * netwib_io.                                                  *
 ***************************************************************/

/*-------------------------------------------------------------*/
/***************************************************************
 *                           FINAL LINKS                       *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* Name : netwib_io_init_null
   Description :
     Create an io junking data and giving nothing.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     **ppio : io created
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_io_init_null(netwib_io **ppio);

/*-------------------------------------------------------------*/
/* Name : netwib_io_init_mem
   Description :
     Create an io storing and reading data from memory.
   Input parameter(s) :
     pbufread : buffer where data is read
     pbufwrite : buffer where data is written
     *plockbufread : lock used when another thread want to
                     access pbufread
     *plockbufwrite : lock used when another thread want to
                      access pbufwrite
     closebufsatend : if the buffers are closed when the io
                      is closed (in locked version, this also
                      close rwlocks)
   Input/output parameter(s) :
   Output parameter(s) :
     **ppio : io created
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_io_init_mem(netwib_buf *pbufread,
                              netwib_buf *pbufwrite,
                              netwib_bool closebufsatend,
                              netwib_io **ppio);
netwib_err netwib_io_init_mem_lock(netwib_thread_rwlock *plockbufread,
                                   netwib_buf *pbufread,
                                   netwib_thread_rwlock *plockbufwrite,
                                   netwib_buf *pbufwrite,
                                   netwib_bool closebufsatend,
                                   netwib_io **ppio);

/*-------------------------------------------------------------*/
/* Name : netwib_io_init_tlv
   Description :
     Create an io storing and reading data from memory.
   Input parameter(s) :
     pbufread : buffer where data is read
     pbufwrite : buffer where data is written
     *plockbufread : lock used when another thread want to
                     access pbufread
     *plockbufwrite : lock used when another thread want to
                      access pbufwrite
     closebufsatend : if the buffers are closed when the io
                      is closed (in locked version, this also
                      close rwlocks)
   Input/output parameter(s) :
   Output parameter(s) :
     **ppio : io created
   Normal return values :
     NETWIB_ERR_OK : ok
   Note :
     If canslide is set on pbufread or pbufwrite, it will be
     faster.
*/
netwib_err netwib_io_init_tlv(netwib_buf *pbufread,
                              netwib_buf *pbufwrite,
                              netwib_bool closebufsatend,
                              netwib_io **ppio);
netwib_err netwib_io_init_tlv_lock(netwib_thread_rwlock *plockbufread,
                                   netwib_buf *pbufread,
                                   netwib_thread_rwlock *plockbufwrite,
                                   netwib_buf *pbufwrite,
                                   netwib_bool closebufsatend,
                                   netwib_io **ppio);

/*-------------------------------------------------------------*/
/* Name : netwib_io_init_exec
   Description :
     Create an io redirecting read and write requests to
     a sub process.
   Input parameter(s) :
     pbufcommand : command (for example "/bin/ls /")
     providedway : if user will read or write from the io
     killonclose : if true, a close kills the process
     pexitednormally : address of a boolean which will receive :
                        - NETWIB_TRUE : if program exited normally
                        - NETWIB_FALSE : else
                       if NULL, do not set it
     preturnedvalue : address of a uint32 which will receive
                      value returned by the program
                      if NULL, do not set it
   Input/output parameter(s) :
   Output parameter(s) :
     **ppio : io created
   Normal return values :
     NETWIB_ERR_OK : ok
   Example 1 :
     /bin/ls a b "c"
    corresponds to :
     filename = /bin/ls
     argv[0] = ls
     argv[1] = a
     argv[2] = b
     argv[3] = c
   Example 2 :
     ls "a \"'\t\\a\x41"
    corresponds to :
     filename = ls
     argv[0] = ls
     argv[1] = a "'_tabulation_\a_0x41_
     argv[3] = NULL
    Note : \ sequences recognized inside "" are : abtnr
*/
netwib_err netwib_io_init_exec(netwib_constbuf *pbufcommand,
                               netwib_io_waytype providedway,
                               netwib_bool killonclose,
                               netwib_bool *pexitednormally,
                               netwib_uint32 *preturnedvalue,
                               netwib_io **ppio);

/*-------------------------------------------------------------*/
/* Name : netwib_io_init_shellserver
   Description :
     Create an io redirecting read and write requests to
     a shell.
     It should be used with netwib_io_init_shellclient.
     IMPORTANT: It is currently only implemented for Linux.
   Input parameter(s) :
     uid : requested user id
     *pbufterm : TERM environment variable
     *pbufpath : PATH environment variable
     killonclose : if true, a close kills the process
     pexitednormally : address of a boolean which will receive :
                        - NETWIB_TRUE : if program exited normally
                        - NETWIB_FALSE : else
                       if NULL, do not set it
     preturnedvalue : address of a uint32 which will receive
                      value returned by the last command executed
                      in the shell
                      if NULL, do not set it
   Input/output parameter(s) :
   Output parameter(s) :
     **ppio : io created
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_LONOTIMPLEMENTED : not on Linux
*/
netwib_err netwib_io_init_shellserver(netwib_uint32 uid,
                                      netwib_constbuf *pbufterm,
                                      netwib_bool killonclose,
                                      netwib_bool *pexitednormally,
                                      netwib_uint32 *preturnedvalue,
                                      netwib_io **ppio);
#define NETWIB_IO_INIT_SHELLSERVER_UID_NONE 0xFFFFFFFFu
#define NETWIB_IO_INIT_SHELLSERVER_TERM_NONE NULL

/*-------------------------------------------------------------*/
/* Name : netwib_io_init_shellclient
   Description :
     Create an io redirecting read and write requests to
     a console.
     It should be used with netwib_io_init_shellserver.
     IMPORTANT: It is currently only implemented for Linux.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     **ppio : io created
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_LONOTIMPLEMENTED : not on Linux
*/
netwib_err netwib_io_init_shellclient(netwib_io **ppio);
/* to obtain term for netwib_io_init_shellserver() */
netwib_err netwib_shellclient_term(netwib_bufext *pbufterm);


/*-------------------------------------------------------------*/
/***************************************************************
 *                       INTERMEDIARY LINKS                    *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* Name : netwib_io_init_data
   Description :
     Create several type of io. For example line io can be used
     like this to read line by line :
       netwib_er(netwib_io_init_file_read("/tmp/f", &pio));
       netwib_er(netwib_io_init_data_line(&pioline));
       netwib_er(netwib_io_plug_read(&pioline, &pio));
       netwib_er(netwib_io_read(&pioline, &b));
       etc.
       netwib_er(netwib_io_close_read(&pioline);
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     **ppio : io created
   Normal return values :
     NETWIB_ERR_OK : ok
*/
typedef enum {
  NETWIB_IO_INIT_DATA_TYPE_LINE = 1,
  NETWIB_IO_INIT_DATA_TYPE_CHUNK,
  NETWIB_IO_INIT_DATA_TYPE_FIXED,
  NETWIB_IO_INIT_DATA_TYPE_TRANSPARENT
} netwib_io_init_data_type;
netwib_err netwib_io_init_data(netwib_io_init_data_type rdtype,
                               netwib_io_init_data_type wrtype,
                               netwib_io **ppio);

/*-------------------------------------------------------------*/
/* read/write line by line */
#define netwib_io_init_data_line(ppio) netwib_io_init_data(NETWIB_IO_INIT_DATA_TYPE_LINE,NETWIB_IO_INIT_DATA_TYPE_LINE,ppio)
/* If write writes '\r\n'(true) or '\n'(false) */
/* netwib_err f(netwib_io *pio, netwib_bool add_r); */
#define netwib_io_ctl_set_data_line_msdos(pio,add_r) netwib_io_ctl_set(pio,NETWIB_IO_WAYTYPE_WRITE,NETWIB_IO_CTLTYPE_DATA_LINE_MSDOS,NULL,add_r)
#define netwib_io_ctl_get_data_line_msdos(pio,padd_r) netwib_io_ctl_get(pio,NETWIB_IO_WAYTYPE_WRITE,NETWIB_IO_CTLTYPE_DATA_LINE_MSDOS,NULL,padd_r)

/*-------------------------------------------------------------*/
/* read/write a chunk of data */
/* note : when end is reached, last returned chunk might be smaller */
#define netwib_io_init_data_chunk(ppio) netwib_io_init_data(NETWIB_IO_INIT_DATA_TYPE_CHUNK,NETWIB_IO_INIT_DATA_TYPE_CHUNK,ppio)
/* To change size of read/written data : between minsize and maxsize */
/* netwib_err f(netwib_io *pio, netwib_io_waytype way, netwib_uint32 size); */
#define netwib_io_ctl_set_data_chunk_minsize(pio,way,minsize) netwib_io_ctl_set(pio,way,NETWIB_IO_CTLTYPE_DATA_CHUNK_MINSIZE,NULL,minsize)
#define netwib_io_ctl_set_data_chunk_maxsize(pio,way,maxsize) netwib_io_ctl_set(pio,way,NETWIB_IO_CTLTYPE_DATA_CHUNK_MAXSIZE,NULL,maxsize)
/* netwib_err f(netwib_io *pio, netwib_io_waytype way, netwib_uint32 *psize); */
#define netwib_io_ctl_get_data_chunk_minsize(pio,way,pminsize) netwib_io_ctl_get(pio,way,NETWIB_IO_CTLTYPE_DATA_CHUNK_MINSIZE,NULL,pminsize)
#define netwib_io_ctl_get_data_chunk_maxsize(pio,way,pmaxsize) netwib_io_ctl_get(pio,way,NETWIB_IO_CTLTYPE_DATA_CHUNK_MAXSIZE,NULL,pmaxsize)

/*-------------------------------------------------------------*/
/* read/write fixed size of data */
/* note : when end is reached, last returned data might be smaller */
#define netwib_io_init_data_fixed(ppio) netwib_io_init_data(NETWIB_IO_INIT_DATA_TYPE_FIXED,NETWIB_IO_INIT_DATA_TYPE_FIXED,ppio)
/* To change size of read data. */
/* netwib_err f(netwib_io *pio, netwib_io_waytype way, netwib_uint32 size); */
#define netwib_io_ctl_set_data_fixed_size(pio,way,size) netwib_io_ctl_set(pio,way,NETWIB_IO_CTLTYPE_DATA_FIXED_SIZE,NULL,size)
/* netwib_err f(netwib_io *pio, netwib_io_waytype way, netwib_uint32 *psize); */
#define netwib_io_ctl_get_data_fixed_size(pio,way,psize) netwib_io_ctl_get(pio,way,NETWIB_IO_CTLTYPE_DATA_FIXED_SIZE,NULL,psize)

/*-------------------------------------------------------------*/
/* transparent : does nothing */
#define netwib_io_init_data_transparent(ppio) netwib_io_init_data(NETWIB_IO_INIT_DATA_TYPE_TRANSPARENT,NETWIB_IO_INIT_DATA_TYPE_TRANSPARENT,ppio)

/*-------------------------------------------------------------*/
/* To change io type */
/* netwib_err f(netwib_io *pio, netwib_io_init_data_type type); */
#define netwib_io_ctl_set_data_type(pio,way,type) netwib_io_ctl_set(pio,way,NETWIB_IO_CTLTYPE_DATA_TYPE,NULL,type)
/* netwib_err f(netwib_io *pio, netwib_io_init_data_type *ptype); */
#define netwib_io_ctl_get_data_type(pio,way,ptype) netwib_io_ctl_get(pio,way,NETWIB_IO_CTLTYPE_DATA_TYPE,NULL,ptype)

/*-------------------------------------------------------------*/
/* Name : netwib_io_init_storage
   Description :
     Create an io buffering data. It can be plugged in front
     of low level io not supporting unread for example.
   Input parameter(s) :
   Input/output parameter(s) :
   Output parameter(s) :
     **ppio : io created
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_io_init_storage(netwib_io **ppio);
#define netwib_io_ctl_set_storage_flush(pio) netwib_io_ctl_set(pio,NETWIB_IO_WAYTYPE_SUPPORTED,NETWIB_IO_CTLTYPE_STORAGE_FLUSH,NULL,0)


/*-------------------------------------------------------------*/
/***************************************************************
 *                         MULTIPLEX LINKS                     *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* Name : netwib_io_init_rdwr
   Description :
     Create an io redirecting read an write requests to two
     distinct io.
   Input parameter(s) :
     preadio : io where data is read
     pwriteio : io where data is written
     closeiosatend : if the io are closed when the io
                     is closed
   Input/output parameter(s) :
   Output parameter(s) :
     **ppio : io created
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_io_init_rdwr(netwib_io *preadio,
                               netwib_io *pwriteio,
                               netwib_bool closeiosatend,
                               netwib_io **ppio);

/*-------------------------------------------------------------*/
/* Name : netwib_io_init_tee
   Description :
     Write data to 2 io.
     Read data from 2 io.
   Input parameter(s) :
     pio1 : first io where data is read/written
     pio2 : second io where data is read/written
     closeiosatend : if pio1/pio2 are closed when the io
                     is closed
   Input/output parameter(s) :
   Output parameter(s) :
     **ppio : io created
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_io_init_tee(netwib_io *pio1,
                              netwib_io *pio2,
                              netwib_bool closeiosatend,
                              netwib_io **ppio);

/*-------------------------------------------------------------*/
/* Name : netwib_io_init_debug
   Description :
     Display information for each request.
   Input parameter(s) :
     pnormalio : io where normal data is read/written
     pdebugio : io where debugging data is written
     closeiosatend : if io are closed when the io
                     is closed
   Input/output parameter(s) :
   Output parameter(s) :
     **ppio : io created
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_io_init_debug(netwib_io *pnormalio,
                                netwib_io *pdebugio,
                                netwib_bool closeiosatend,
                                netwib_io **ppio);

