
/*-------------------------------------------------------------*/
/***************************************************************
 * A record is a file where we can save and read data.         *
 * Each data is separated by an empty newline.                 *
 ***************************************************************/

/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_RECORD_ENCODETYPE_HEXA0 = NETWIB_ENCODETYPE_HEXA0,
  NETWIB_RECORD_ENCODETYPE_HEXA1 = NETWIB_ENCODETYPE_HEXA1,
  NETWIB_RECORD_ENCODETYPE_HEXA2 = NETWIB_ENCODETYPE_HEXA2,
  NETWIB_RECORD_ENCODETYPE_HEXA4 = NETWIB_ENCODETYPE_HEXA4,
  NETWIB_RECORD_ENCODETYPE_MIXED0 = NETWIB_ENCODETYPE_MIXED0,
  NETWIB_RECORD_ENCODETYPE_MIXED1 = NETWIB_ENCODETYPE_MIXED1,
  NETWIB_RECORD_ENCODETYPE_HEXA0_WRAP = NETWIB_ENCODETYPE_HEXA0_WRAP,
  NETWIB_RECORD_ENCODETYPE_HEXA1_WRAP = NETWIB_ENCODETYPE_HEXA1_WRAP,
  NETWIB_RECORD_ENCODETYPE_HEXA2_WRAP = NETWIB_ENCODETYPE_HEXA2_WRAP,
  NETWIB_RECORD_ENCODETYPE_HEXA4_WRAP = NETWIB_ENCODETYPE_HEXA4_WRAP,
  NETWIB_RECORD_ENCODETYPE_MIXED0_WRAP = NETWIB_ENCODETYPE_MIXED0_WRAP,
  NETWIB_RECORD_ENCODETYPE_MIXED1_WRAP = NETWIB_ENCODETYPE_MIXED1_WRAP,
  NETWIB_RECORD_ENCODETYPE_DUMP = NETWIB_ENCODETYPE_DUMP,
  NETWIB_RECORD_ENCODETYPE_MIXED0H_WRAP = NETWIB_ENCODETYPE_MIXED0H_WRAP,
  NETWIB_RECORD_ENCODETYPE_MIXED1H_WRAP = NETWIB_ENCODETYPE_MIXED1H_WRAP,
  NETWIB_RECORD_ENCODETYPE_BIN = 1000,  /* binary */
  NETWIB_RECORD_ENCODETYPE_PCAP,        /* libpcap format */
  /* aliases */
  NETWIB_RECORD_ENCODETYPE_HEXA = NETWIB_RECORD_ENCODETYPE_HEXA1,
  NETWIB_RECORD_ENCODETYPE_MIXED = NETWIB_RECORD_ENCODETYPE_MIXED1,
  NETWIB_RECORD_ENCODETYPE_HEXA_WRAP = NETWIB_RECORD_ENCODETYPE_HEXA1_WRAP,
  NETWIB_RECORD_ENCODETYPE_MIXED_WRAP = NETWIB_RECORD_ENCODETYPE_MIXED1_WRAP,
  NETWIB_RECORD_ENCODETYPE_MIXEDH_WRAP = NETWIB_RECORD_ENCODETYPE_MIXED1H_WRAP
} netwib_record_encodetype;

/*-------------------------------------------------------------*/
/* Name : netwib_record_encodetype_init_kbd
   Description :
     Initialize a netwib_encodetype with data entered through keyboard.
   Input parameter(s) :
     *pmessage : message to print before
     displayonlymostuseful : only most useful values are shown
     defaultencodetype : default encodetype to use if user enters nothing
                      (if 0xFFFFFFFFu, means no default)
   Input/output parameter(s) :
   Output parameter(s) :
     *pencodetype : netwib_encodetype initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_record_encodetype_init_kbd(netwib_constbuf *pmessage,
                                             netwib_bool displayonlymostuseful,
                                             netwib_record_encodetype defaultencodetype,
                                             netwib_record_encodetype *pencodetype);
#define NETWIB_RECORD_ENCODETYPE_INIT_KBD_NODEF (netwib_record_encodetype)0xFFFFFFFFu

/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_record_encodetype
   Description :
     Append the description text of an encodetype.
   Input parameter(s) :
     encodetype : netwib_record_encodetype to append
   Input/output parameter(s) :
     *pbuf : buffer where text is appended
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_append_record_encodetype(netwib_record_encodetype encodetype,
                                               netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* Name : netwib_io_init_record
   Description :
     Open a record.
   Input parameter(s) :
     *precordname : filename of the record
   Input/output parameter(s) :
   Output parameter(s) :
     **ppio : io created
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_io_init_record(netwib_constbuf *precordname,
                                 netwib_file_inittype inittype,
                                 netwib_record_encodetype encodetype,
                                 netwib_io **ppio);
#define netwib_io_init_record_read(recordname,ppio) netwib_io_init_record(recordname,NETWIB_FILE_INITTYPE_READ,NETWIB_RECORD_ENCODETYPE_INIT_KBD_NODEF,ppio)
#define netwib_io_init_record_write(recordname,encodetype,ppio) netwib_io_init_record(recordname,NETWIB_FILE_INITTYPE_WRITE,encodetype,ppio)
#define netwib_io_init_record_append(recordname,ppio) netwib_io_init_record(recordname,NETWIB_FILE_INITTYPE_APPEND,NETWIB_RECORD_ENCODETYPE_INIT_KBD_NODEF,ppio)
