
/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_FILE_INITTYPE_READ = 1,    /* open the file for reading */
  NETWIB_FILE_INITTYPE_WRITE,       /* open the file for writing */
  NETWIB_FILE_INITTYPE_APPEND,      /* open the file for appending */
  NETWIB_FILE_INITTYPE_RDWR         /* open the file for reading and writing */
} netwib_file_inittype;

/*-------------------------------------------------------------*/
/* Name : netwib_io_init_file
   Description :
     Open a file.
   Input parameter(s) :
     *pfilename : file name
     textmode : (useful under Windows and ignored under Unix)
                if file has to be opened in text mode ("\n" are
                converted to "\r\n" for output; "\r\n" are
                converted to "\n" for input)
   Input/output parameter(s) :
   Output parameter(s) :
     **ppio : io created
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_io_init_file(netwib_constbuf *pfilename,
                               netwib_file_inittype type,
                               netwib_bool textmode,
                               netwib_io **ppio);
#define netwib_io_init_file_read(filename,ppio) netwib_io_init_file(filename,NETWIB_FILE_INITTYPE_READ,NETWIB_FALSE,ppio)
#define netwib_io_init_file_write(filename,ppio) netwib_io_init_file(filename,NETWIB_FILE_INITTYPE_WRITE,NETWIB_FALSE,ppio)
#define netwib_io_init_file_append(filename,ppio) netwib_io_init_file(filename,NETWIB_FILE_INITTYPE_APPEND,NETWIB_FALSE,ppio)
#define netwib_io_init_file_rdwr(filename,ppio) netwib_io_init_file(filename,NETWIB_FILE_INITTYPE_RDWR,NETWIB_FALSE,ppio)
#define netwib_io_init_file_textread(filename,ppio) netwib_io_init_file(filename,NETWIB_FILE_INITTYPE_READ,NETWIB_TRUE,ppio)
#define netwib_io_init_file_textwrite(filename,ppio) netwib_io_init_file(filename,NETWIB_FILE_INITTYPE_WRITE,NETWIB_TRUE,ppio)
#define netwib_io_init_file_textappend(filename,ppio) netwib_io_init_file(filename,NETWIB_FILE_INITTYPE_APPEND,NETWIB_TRUE,ppio)
#define netwib_io_init_file_textrdwr(filename,ppio) netwib_io_init_file(filename,NETWIB_FILE_INITTYPE_RDWR,NETWIB_TRUE,ppio)

/*-------------------------------------------------------------*/
/* set current position from beginning of file */
/* netwib_err f(netwib_io *pio, netwib_int32 pos); */
#define netwib_file_ctl_set_seek_begin(pio,pos) netwib_io_ctl_set(pio,NETWIB_IO_WAYTYPE_SUPPORTED,NETWIB_IO_CTLTYPE_FILE_SEEK_BEGIN,NULL,(netwib_uint32)pos)
/* netwib_err f(netwib_io *pio, const netwib_int64 *ppos); */
#define netwib_file_ctl_set_seek64_begin(pio,ppos) netwib_io_ctl_set(pio,NETWIB_IO_WAYTYPE_SUPPORTED,NETWIB_IO_CTLTYPE_FILE_SEEK_BEGIN,ppos,0)

/*-------------------------------------------------------------*/
/* set current position from current position of file */
/* netwib_err f(netwib_io *pio, netwib_int32 pos); */
#define netwib_file_ctl_set_seek_current(pio,pos) netwib_io_ctl_set(pio,NETWIB_IO_WAYTYPE_SUPPORTED,NETWIB_IO_CTLTYPE_FILE_SEEK_CURRENT,NULL,(netwib_uint32)pos)
/* netwib_err f(netwib_io *pio, const netwib_int64 *ppos); */
#define netwib_file_ctl_set_seek64_current(pio,ppos) netwib_io_ctl_set(pio,NETWIB_IO_WAYTYPE_SUPPORTED,NETWIB_IO_CTLTYPE_FILE_SEEK_CURRENT,ppos,0)

/*-------------------------------------------------------------*/
/* set current position from end of file */
/* netwib_err f(netwib_io *pio, netwib_int32 pos); */
#define netwib_file_ctl_set_seek_end(pio,pos) netwib_io_ctl_set(pio,NETWIB_IO_WAYTYPE_SUPPORTED,NETWIB_IO_CTLTYPE_FILE_SEEK_END,NULL,(netwib_uint32)pos)
/* netwib_err f(netwib_io *pio, const netwib_int64 *ppos); */
#define netwib_file_ctl_set_seek64_end(pio,ppos) netwib_io_ctl_set(pio,NETWIB_IO_WAYTYPE_SUPPORTED,NETWIB_IO_CTLTYPE_FILE_SEEK_END,ppos,0)

/*-------------------------------------------------------------*/
/* get current position from beginning of file */
/* netwib_err f(netwib_io *pio, netwib_int32 *ppos); */
#define netwib_file_ctl_get_tell(pio,ppos) netwib_io_ctl_get(pio,NETWIB_IO_WAYTYPE_SUPPORTED,NETWIB_IO_CTLTYPE_FILE_TELL,NULL,(netwib_uint32*)ppos)
/* netwib_err f(netwib_io *pio, netwib_int64 *ppos); */
#define netwib_file_ctl_get_tell64(pio,ppos) netwib_io_ctl_get(pio,NETWIB_IO_WAYTYPE_SUPPORTED,NETWIB_IO_CTLTYPE_FILE_TELL,ppos,NULL)

/*-------------------------------------------------------------*/
/* truncate file to indicated size */
/* netwib_err f(netwib_io *pio, netwib_int32 pos); */
#define netwib_file_ctl_set_truncate(pio,pos) netwib_io_ctl_set(pio,NETWIB_IO_WAYTYPE_SUPPORTED,NETWIB_IO_CTLTYPE_FILE_TRUNCATE,NULL,(netwib_uint32)pos)
/* netwib_err f(netwib_io *pio, const netwib_int64 *ppos); */
#define netwib_file_ctl_set_truncate64(pio,ppos) netwib_io_ctl_set(pio,NETWIB_IO_WAYTYPE_SUPPORTED,NETWIB_IO_CTLTYPE_FILE_TRUNCATE,ppos,0)

/*-------------------------------------------------------------*/
/* Name : netwib_io_init_filetemp
   Description :
     Open a temporary file, and give back the chosen file name.
   Input parameter(s) :
     textmode : (useful under Windows and ignored under Unix)
                if file has to be opened in text mode ("\n" are
                converted to "\r\n")
   Input/output parameter(s) :
     *pfilename : file name
                  If its size is 0, choose a filename in
                  system's temporary directory.
                  If it contains something, append 6 random
                  characters to create the file name. So, it must
                  end with '/' to be interpreted as a directory.
                  The chosen filename is set in this variable.
   Output parameter(s) :
     **ppio : io created
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_io_init_filetemp(netwib_bool textmode,
                                   netwib_buf *pfilename,
                                   netwib_io **ppio);

