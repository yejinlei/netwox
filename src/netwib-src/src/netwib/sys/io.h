
/*-------------------------------------------------------------*/
/***************************************************************
 * A netwib_io is the main way to exchange data in netwib.     *
 *                                                             *
 * A concrete example first :                                  *
 *   ----.     ,-----.     ,-------.     ,----.     ,------    *
 *   file >---->readl >---( program )---->conv >---->socket    *
 *   ----'     `-----'     `-------'     `----'     `------    *
 *                                                             *
 * It works like this :                                        *
 *  - read data from a file                                    *
 *  - read only line by line                                   *
 *  - treat it in the program                                  *
 *  - write data for output                                    *
 *  - convert data, for example replacing '\' by "\\"          *
 *  - write data to socket                                     *
 *                                                             *
 * This example contains 3 types of netwib_io :                *
 *  - file and socket are "io final link"                      *
 *  - readl and conv are "io link"                             *
 *  - file and socket are also "io link" (because a            *
 *    "io final link" is a special kind of "io link")          *
 *  - conv is a "io chain" composed of conv+socket             *
 *  - readl is a "io chain" composed of readl+file             *
 *  - socket is a "io chain" composed of only socket           *
 *  - file is a "io chain" composed of only file               *
 *                                                             *
 * Using the same program, we might for example read data      *
 * from a file and write them back to a socket. This can be    *
 * represented as :                                            *
 *   ------.      ,-------.      ,---------                    *
 *    file  >----( program )-----> socket                      *
 *   ------'      `-------'      `---------                    *
 * Another variant would be to plug readl or conv to other     *
 * "io chain", such as :                                       *
 *   ----.     ,-----.     ,-------.     ,----.     ,------    *
 *   ipc  >---->readl >---( program )---->conv >---->record    *
 *   ----'     `-----'     `-------'     `----'     `------    *
 * An "io chain" can be bi-directional. For example :          *
 *   --------.       ,--.      ,-------.                       *
 *    socket  >------>l5 >----( program )-.                    *
 *           <------<   <--.   `-------'  |                    *
 *   --------'       `--'   `-------------'                    *
 *                                                             *
 * Note : you can find other examples at the end of this file  *
 *                                                             *
 * The main ideas to remember is :                             *
 *  - a "io chain" is composed of one or more "io link"        *
 *  - a "io link" might be intermediary or "io final link"     *
 *  - those 3 types use the same structure : netwib_io         *
 ***************************************************************/

/*-------------------------------------------------------------*/
typedef struct netwib_io netwib_io;

/*-------------------------------------------------------------*/
/* direction */
typedef enum {
  NETWIB_IO_WAYTYPE_READ = 1,    /* read */
  NETWIB_IO_WAYTYPE_WRITE,       /* write */
  NETWIB_IO_WAYTYPE_RDWR,        /* read and write */
  NETWIB_IO_WAYTYPE_NONE,        /* nor read nor write */
  NETWIB_IO_WAYTYPE_SUPPORTED    /* every way supported by the io */
} netwib_io_waytype;

/*-------------------------------------------------------------*/
/***************************************************************
 * Currently, system "io final link" are :                     *
 *    Name      Definition     Read/Write                      *
 *   FILE   : regular file   : read/write                      *
 *   FD     : file desc      : read/write                      *
 *   HANDLE : Windows HANDLE : read/write                      *
 *   KBD    : keyboard       : read                            *
 *   RECORD : record         : read/write                      *
 *   SOCK   : socket         : read/write                      *
 *   SNIFF  : sniff          : read                            *
 *   SPOOF  : spoof          : write                           *
 * They are initialized by :                                   *
 *  - netwib_io_init_file                                      *
 *  - netwib_io_init_fd                                        *
 *  - etc.                                                     *
 ***************************************************************/

/*-------------------------------------------------------------*/
/***************************************************************
 * Those five functions permits to create and to navigate      *
 * through a "io chain".                                       *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* Name : netwib_io_plug
   Description :
     Create a chain by pluging one link/chain to another.
   Input parameter(s) :
     *piowheretoplug : io where to plug
     typeofplug : type of plug
   Input/output parameter(s) :
     *pio : current io
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_io_plug(netwib_io *pio,
                          netwib_io_waytype typeofplug,
                          netwib_io *piowheretoplug);
/* netwib_err f(netwib_io *pio, netwib_io *piowheretoplug); */
#define netwib_io_plug_read(pio,piowheretoplug) netwib_io_plug(pio,NETWIB_IO_WAYTYPE_READ,piowheretoplug)
#define netwib_io_plug_write(pio,piowheretoplug) netwib_io_plug(pio,NETWIB_IO_WAYTYPE_WRITE,piowheretoplug)
#define netwib_io_plug_rdwr(pio,piowheretoplug) netwib_io_plug(pio,NETWIB_IO_WAYTYPE_RDWR,piowheretoplug)
#define netwib_io_plug_supported(pio,piowheretoplug) netwib_io_plug(pio,NETWIB_IO_WAYTYPE_SUPPORTED,piowheretoplug)

/*-------------------------------------------------------------*/
/***************************************************************
 * There are three ways to unplug :                            *
 *      ,----.     ,----.     ,----.     ,----.     ,----.     *
 *   ---> io1 >----> io2 >----> io3 >----> io4 >----> io5 >--  *
 *      `----'     `----'     `----'     `----'     `----'     *
 *                                                             *
 * netwib_io_unplug_next with:                                 *
 *   pio = &io1                                                *
 *  ==> cut between io1 and io2, and put &io2 in pnextio       *
 *                                                             *
 * netwib_io_unplug_before with:                               *
 *   pio = &io1                                                *
 *   psearchedio = &io3                                        *
 *  ==> cut between io2 and io3                                *
 *                                                             *
 * netwib_io_unplug_after with:                                *
 *   pio = &io1                                                *
 *   psearchedio = &io3                                        *
 *  ==> cut between io3 and io4, and put &io4 in pafterio      *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* Name : netwib_io_unplug_xyz
   Description :
     Separate a chain by unplugging one link/chain from another.
   Input parameter(s) :
     typeofplug : type of plug
     *psearchedio : searched io
   Input/output parameter(s) :
     *pio : io chain
   Output parameter(s) :
     **ppnextio : next io or NULL if at end
     **ppafterio : next io or NULL if at end
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_NOTFOUND : psearchedio was not found
*/
netwib_err netwib_io_unplug_next(netwib_io *pio,
                                 netwib_io_waytype typeofplug,
                                 netwib_io **ppnextio);
netwib_err netwib_io_unplug_before(netwib_io *pio,
                                   netwib_io_waytype typeofplug,
                                   netwib_io *psearchedio);
netwib_err netwib_io_unplug_after(netwib_io *pio,
                                  netwib_io_waytype typeofplug,
                                  netwib_io *psearchedio,
                                  netwib_io **ppafterio);
#define netwib_io_unplug_next_read(pio,ppnextio) netwib_io_unplug_next(pio,NETWIB_IO_WAYTYPE_READ,ppnextio)
#define netwib_io_unplug_before_read(pio,psearchedio) netwib_io_unplug_before(pio,NETWIB_IO_WAYTYPE_READ,psearchedio)
#define netwib_io_unplug_after_read(pio,psearchedio,ppafterio) netwib_io_unplug_after(pio,NETWIB_IO_WAYTYPE_READ,psearchedio,ppafterio)
#define netwib_io_unplug_next_write(pio,ppnextio) netwib_io_unplug_next(pio,NETWIB_IO_WAYTYPE_WRITE,ppnextio)
#define netwib_io_unplug_before_write(pio,psearchedio) netwib_io_unplug_before(pio,NETWIB_IO_WAYTYPE_WRITE,psearchedio)
#define netwib_io_unplug_after_write(pio,psearchedio,ppafterio) netwib_io_unplug_after(pio,NETWIB_IO_WAYTYPE_WRITE,psearchedio,ppafterio)
#define netwib_io_unplug_next_rdwr(pio,ppnextio) netwib_io_unplug_next(pio,NETWIB_IO_WAYTYPE_RDWR,ppnextio)
#define netwib_io_unplug_before_rdwr(pio,psearchedio) netwib_io_unplug_before(pio,NETWIB_IO_WAYTYPE_RDWR,psearchedio)
#define netwib_io_unplug_after_rdwr(pio,psearchedio,ppafterio) netwib_io_unplug_after(pio,NETWIB_IO_WAYTYPE_RDWR,psearchedio,ppafterio)
#define netwib_io_unplug_next_supported(pio,ppnextio) netwib_io_unplug_next(pio,NETWIB_IO_WAYTYPE_SUPPORTED,ppnextio)
#define netwib_io_unplug_before_supported(pio,psearchedio) netwib_io_unplug_before(pio,NETWIB_IO_WAYTYPE_SUPPORTED,psearchedio)
#define netwib_io_unplug_after_supported(pio,psearchedio,ppafterio) netwib_io_unplug_after(pio,NETWIB_IO_WAYTYPE_SUPPORTED,psearchedio,ppafterio)

/*-------------------------------------------------------------*/
/* Name : netwib_io_next
   Description :
     Obtain the next io in the chain.
   Input parameter(s) :
     *pio : io
   Input/output parameter(s) :
     **ppionext : io after pio of type typetofollow
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_DATAEND : there is no more io after
*/
netwib_err netwib_io_next(netwib_io *pio,
                          netwib_io_waytype typetofollow,
                          netwib_io **ppionext);
/* netwib_err f(netwib_io *pio, netwib_io *pionext); */
#define netwib_io_next_read(pio,pionext) netwib_io_next(pio,NETWIB_IO_WAYTYPE_READ,pionext)
#define netwib_io_next_write(pio,pionext) netwib_io_next(pio,NETWIB_IO_WAYTYPE_WRITE,pionext)
#define netwib_io_next_rdwr(pio,pionext) netwib_io_next(pio,NETWIB_IO_WAYTYPE_RDWR,pionext)
#define netwib_io_next_supported(pio,pionext) netwib_io_next(pio,NETWIB_IO_WAYTYPE_SUPPORTED,pionext)

/*-------------------------------------------------------------*/
/***************************************************************
 * Those three functions permits to exchange data through a    *
 * "io chain".                                                 *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* Name : netwib_io_write_xyz
   Description :
     Write data to a netwib_io.
   Input parameter(s) :
     *pbuf : buffer to write
   Input/output parameter(s) :
     **pio : netwib_io where to write
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_io_write(netwib_io *pio,
                           netwib_constbuf *pbuf);

/*-------------------------------------------------------------*/
/* Name : netwib_io_read_xyz
   Description :
     Read data to a netwib_io.
   Input parameter(s) :
   Input/output parameter(s) :
     **pio : netwib_io where to read
     *pbuf : buffer storing read data
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_DATAEND : end of data
*/
netwib_err netwib_io_read(netwib_io *pio,
                          netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* Name : netwib_io_unread
   Description :
     Put data back in the read io.
   Input parameter(s) :
   Input/output parameter(s) :
     **pio : netwib_io where to read
     *pbuf : buffer containing data
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_io_unread(netwib_io *pio,
                            netwib_constbuf *pbuf);

/*-------------------------------------------------------------*/
/***************************************************************
 * Those three functions permits to control an "io chain".     *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* Name : netwib_io_wait
   Description :
     Wait for data in the io.
   Input parameter(s) :
     *pabstime : end time. If *pabstime is reached, function
                 returns (*pevent set to NETWIB_FALSE).
   Input/output parameter(s) :
     **pio : netwib_io where to wait
   Output parameter(s) :
     *pevent : an event occurred
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_io_wait(netwib_io *pio,
                          netwib_io_waytype way,
                          netwib_consttime *pabstime,
                          netwib_bool *pevent);
/* netwib_err f(netwib_io *pio, netwib_consttime *pabstime, netwib_bool *pevent); */
#define netwib_io_wait_read(pio,pabstime,pevent) netwib_io_wait(pio,NETWIB_IO_WAYTYPE_READ,pabstime,pevent)
#define netwib_io_wait_write(pio,pabstime,pevent) netwib_io_wait(pio,NETWIB_IO_WAYTYPE_WRITE,pabstime,pevent)
#define netwib_io_wait_rdwr(pio,pabstime,pevent) netwib_io_wait(pio,NETWIB_IO_WAYTYPE_RDWR,pabstime,pevent)
#define netwib_io_wait_supported(pio,pabstime,pevent) netwib_io_wait(pio,NETWIB_IO_WAYTYPE_SUPPORTED,pabstime,pevent)

/*-------------------------------------------------------------*/
/* Explaining those values would be too complicated here. Please
   refer to the defines using them (which are documented).
*/
typedef enum {
  /** values working on current io (they are not recursive) **/
  NETWIB_IO_CTLTYPE_SUPPORT = 1,
  NETWIB_IO_CTLTYPE_NUMUSERS,
  NETWIB_IO_CTLTYPE_NUMUSERSINC,
  /** values working on current io (if NETWIB_ERR_OK is returned) or
      on next io (if NETWIB_ERR_PLEASETRYNEXT is returned) **/
  NETWIB_IO_CTLTYPE_RES = 100,
  NETWIB_IO_CTLTYPE_END,
  /** values which are specific **/
  /* file : 200 */
  NETWIB_IO_CTLTYPE_FILE_SEEK_BEGIN = 200,
  NETWIB_IO_CTLTYPE_FILE_SEEK_CURRENT,
  NETWIB_IO_CTLTYPE_FILE_SEEK_END,
  NETWIB_IO_CTLTYPE_FILE_TELL,
  NETWIB_IO_CTLTYPE_FILE_TRUNCATE,
  /* fd : 300 */
  /* handle : 400 */
  /* ipc : 500 */
  NETWIB_IO_CTLTYPE_IPC_SIDEREAD = 500,
  NETWIB_IO_CTLTYPE_IPC_SIDEWRITE,
  NETWIB_IO_CTLTYPE_IPC_NOTUSED,
  /* kbd : 600 */
  NETWIB_IO_CTLTYPE_KBD_ECHO = 600,
  NETWIB_IO_CTLTYPE_KBD_LINE,
  NETWIB_IO_CTLTYPE_KBD_PURGE,
  /* record : 700 */
  /* sock : 800 */
  NETWIB_IO_CTLTYPE_SOCK_IP4OPTS = 800,
  NETWIB_IO_CTLTYPE_SOCK_IP6EXTS,
  NETWIB_IO_CTLTYPE_SOCK_LOCAL,
  NETWIB_IO_CTLTYPE_SOCK_REMOTE,
  NETWIB_IO_CTLTYPE_SOCK_MULTICASTTTL,
  NETWIB_IO_CTLTYPE_SOCK_SOCKTYPE,
  /* sockv : 900 */
  NETWIB_IO_CTLTYPE_SOCKV_ANSWERALIVE = 900,
  /* sniff : 1000 */
  NETWIB_IO_CTLTYPE_SNIFF_FILTER = 1000,
  NETWIB_IO_CTLTYPE_SNIFF_DLT,
  /* spoof : 1100 */
  NETWIB_IO_CTLTYPE_SPOOF_DLT= 1100,
  /** values which are specific to iousual.h **/
  /* ioutil : 2000 */
  NETWIB_IO_CTLTYPE_DATA_LINE_MSDOS = 2000,
  NETWIB_IO_CTLTYPE_DATA_CHUNK_MINSIZE,
  NETWIB_IO_CTLTYPE_DATA_CHUNK_MAXSIZE,
  NETWIB_IO_CTLTYPE_DATA_FIXED_SIZE,
  NETWIB_IO_CTLTYPE_DATA_TYPE,
  NETWIB_IO_CTLTYPE_STORAGE_FLUSH,
  /** values which are specific to users' utilities **/
  /* user defined : start at 10000 */
  NETWIB_IO_CTLTYPE_USER_BEGIN = NETWIB_ENUM_USER_BEGIN
} netwib_io_ctltype;
/* Those functions permit to set/get parameters (pointer and
   integer) about a netwib_io. It should not be used directly,
   but by the defines.
   In function netwib_io_ctl_get, parameter p could be
   a "netwib_ptr *pp" instead of "netwib_ptr p", but it
   complicates things for nothing : a pointer can be
   used for what we want. For example, its easier to use
   ctl_get(..., &buf, &ui) instead of ("&&buf", &ui).
*/
netwib_err netwib_io_ctl_set(netwib_io *pio,
                             netwib_io_waytype way,
                             netwib_io_ctltype ctltype,
                             netwib_ptr p,
                             netwib_uint32 ui);
netwib_err netwib_io_ctl_get(netwib_io *pio,
                             netwib_io_waytype way,
                             netwib_io_ctltype ctltype,
                             netwib_ptr p,
                             netwib_uint32 *pui);

/*-------------------------------------------------------------*/
/***************************************************************
 * This function permits to close an "io chain".               *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* Name : netwib_io_close
   Description :
     Close a chain (links are closed only if nobody use them
     anymore : numusers == 0).
   Input parameter(s) :
   Input/output parameter(s) :
     **ppio : netwib_io to close
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_io_close(netwib_io **ppio);

/*-------------------------------------------------------------*/
/***************************************************************
 * Common control defines                                      *
 ***************************************************************/

/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_IO_RESTYPE_FILE = 1,
  NETWIB_IO_RESTYPE_FD,
  NETWIB_IO_RESTYPE_HANDLE,
  NETWIB_IO_RESTYPE_IPC,
  NETWIB_IO_RESTYPE_KBD,
  NETWIB_IO_RESTYPE_SCREEN,
  NETWIB_IO_RESTYPE_STREAM,
  NETWIB_IO_RESTYPE_RECORD,
  NETWIB_IO_RESTYPE_SOCK,
  NETWIB_IO_RESTYPE_SOCKV,
  NETWIB_IO_RESTYPE_SNIFF,
  NETWIB_IO_RESTYPE_SPOOF,
  NETWIB_IO_RESTYPE_NULL,
  NETWIB_IO_RESTYPE_MEM,
  NETWIB_IO_RESTYPE_TLV,
  NETWIB_IO_RESTYPE_EXEC,
  NETWIB_IO_RESTYPE_SHELLSERVER,
  NETWIB_IO_RESTYPE_SHELLCLIENT,
  NETWIB_IO_RESTYPE_USER_BEGIN = NETWIB_ENUM_USER_BEGIN
} netwib_io_restype;
/* Obtain the resource type of a netwib_io */
/* netwib_err f(netwib_io *pio, netwib_io_waytype way, netwib_io_restype *pres); */
#define netwib_io_ctl_get_res(pio,way,pres) netwib_io_ctl_get(pio,way,NETWIB_IO_CTLTYPE_RES,NULL,(netwib_uint32*)pres)

/*-------------------------------------------------------------*/
/* indicate the end of data */
/* netwib_err f(netwib_io *pio, netwib_io_waytype way); */
#define netwib_io_ctl_set_end(pio,way) netwib_io_ctl_set(pio,way,NETWIB_IO_CTLTYPE_END,NULL,0)
#define netwib_io_ctl_set_end_write(pio) netwib_io_ctl_set_end(pio,NETWIB_IO_WAYTYPE_WRITE)


/*-------------------------------------------------------------*/
/***************************************************************
 * Now, a big example ...                                      *
 ***************************************************************/

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
#if 0
/* Complete example :

  We create a module (named m) having one input (i1) and two
  outputs (o1, o2) :

            ############# m #############
            #                           #
            #         ooooooooo         #  o1
        i1  #  ,--.   o       o--------------->
      >-------->l1 >--o HEART o   ,--.  #
            #  `--'   o       o--->l2 >------->
            #         ooooooooo   `--'  #  o2
            #                           #
            #############################

  The module contains 2 ios :
   - l1 : reads line by line
   - l2 : converts data to base64

  The heart of the module reads data from l1's output. If the
  data starts by 'A', then it is sent to o1. Else, data goes
  through l2.

  *** Usage 1 :
   - i1 is plugged to a file through io l3
   - o1 is plugged to file2
   - o2 is plugged to file3 through io l4
   - l3 reads data and duplicates it (read 'qa', output 'qqaa')
   - l4 reads data, displays it to screen and sends to output
                                                   ,-------
                               #####       ,-------> file2
      -------.      ,--.       #   >------'        `-------
       file1  >----->l3 >------> m #     ,--.      ,-------
      -------'      `--'       #   >----->l4 >-----> file3
                               #####     `--'      `-------

  *** Usage 2 :
   - i1 is plugged to socket soc1
   - o1 is plugged to null (data is junked)
   - o2 is plugged back to the same socket
                               #####               ,------
      ------.                  #   >---------------> null
       soc1  >-----------------> m #               `------
            <-------.          #   >----.
      ------'        \         #####    |
                      `-----------------'

  *** Usage 3 :
   - i1 is plugged to a socket through io l3
   - o1 is plugged to null (data is junked)
   - o2 is plugged back to the same socket through io l4
                               #####               ,------
                    ,--.       #   >---------------> null
      ------.    ,-->l3 >------> m #               `------
       soc1  >--'   `--'       #   >----.
            <--.    ,--.       #####    |
      ------'   `--< l4<----------------'
                    `--'

  *** Usage 4 :
   - l5 is a bi-directional io converting string to uppercase
   - l5 is plugged to socket soc1
   - i1 is plugged to l5
   - o1 is plugged to null (data is junked)
   - o2 is plugged back to l5
                               #####               ,------
                               #   >---------------> null
      ------.       ,--.    ,--> m #               `------
       soc1  >------>l5 >--'   #   >----.
            <------<   <--.    #####    |
      ------'       `--'   `------------'

  *** Usage 5 :
   - i1 is plugged to file1 through l5
   - o1 is plugged to null (data is junked)
   - o2 is plugged to file2 through l5
                               #####               ,------
     -------.                  #   >---------------> null
      file1  >--.   ,--.    ,--> m #               `------
     -------'    `-->l5 >--'   #   >----.
     -------.   ,--<   <--.    #####    |
      file2 <--'    `--'   `------------'
     -------'

  *** Usage 6 :
   - i1 is plugged to file1
   - o1 are o2 are both plugged to file2
                               #####
      -------.                 #   >------------.  ,-------
       file1  >----------------> m #             --> file2
      -------'                 #   >------------'  `-------
                               #####

  *** Usage 7 :
   - m2 is a module similar to m except it has 2 inputs
     and one output
   - i1 is plugged to file1
   - i2 is plugged to file1
   - o1 is plugged to null (data is junked)
   - o2 is plugged to file2
                               #####
      -------.   ,------------->   #               ,------
       file1  >--              # m >---------------> null
      -------'   `------------->   #               `------
                               #####

*/

/*-------------------------------------------------------------*/
/* l1 is netwib_io_init_read_line */

/*-------------------------------------------------------------*/
/* l2 : converts data to base64 */
netwib_err l2_write(netwib_io *pio,
                    netwib_constbuf *pbuf);
netwib_err l2_write(netwib_io *pio,
                    netwib_constbuf *pbuf)
{
  netwib_buf bufbase64;
  netwib_io *pionext;
  netwib_err ret;

  /* obtain the next io in the chain */
  netwib_er(netwib_io_next_write(pio, &pionext));

  /* converts pbuf to base64 */
  netwib_er(netwib_buf_init_mallocdefault(&bufbase64));
  netwib_er(netwib_buf_encode(pbuf, NETWIB_ENCODETYPE_BASE64, &bufbase64));

  /* write data to the next io */
  ret = netwib_io_write(pionext, &bufbase64);
  netwib_er(netwib_buf_close(&bufbase64));

  return(ret);
}

netwib_err l2_init(netwib_io **ppio);
netwib_err l2_init(netwib_io **ppio)
{
  netwib_er(netwib_io_init(NETWIB_FALSE, NETWIB_TRUE, NULL,
                           NULL/*read*/, &l2_write,
                           NULL/*wait*/, NULL/*unread*/,
                           NULL/*ctlset*/, NULL/*ctlget*/,
                           NULL/*close*/, ppio));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* l3 reads data and duplicates it (read 'qa', output 'qqaa') */
netwib_err l3_dup(netwib_constbuf *pinbuf,
                  netwib_buf *poutbuf);
netwib_err l3_dup(netwib_constbuf *pinbuf,
                  netwib_buf *poutbuf)
{
  netwib_data data;
  netwib_uint32 datasize, i;

  data = netwib__buf_ref_data_ptr(pinbuf);
  datasize = netwib__buf_ref_data_size(pinbuf);
  for (i = 0; i < datasize; i++) {
    netwib_er(netwib_buf_append_byte(data[i], poutbuf));
    netwib_er(netwib_buf_append_byte(data[i], poutbuf));
  }

  return(NETWIB_ERR_OK);
}
netwib_err l3_read(netwib_io *pio,
                   netwib_buf *pbuf);
netwib_err l3_read(netwib_io *pio,
                   netwib_buf *pbuf)
{
  netwib_buf buf;
  netwib_io *pionext;

  /* obtain the next io in the chain */
  netwib_er(netwib_io_next_read(pio, &pionext));

  /* read data */
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_io_read(pionext, &buf));

  /* duplicate buf */
  netwib_er(l3_dup(&buf, pbuf));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}
netwib_err l3_init(netwib_io **ppio);
netwib_err l3_init(netwib_io **ppio)
{
  netwib_er(netwib_io_init(NETWIB_FALSE, NETWIB_TRUE, NULL,
                           &l3_read/*read*/, NULL/*write*/,
                           NULL/*wait*/, NULL/*unread*/,
                           NULL/*ctlset*/, NULL/*ctlget*/,
                           NULL/*close*/, ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* l4 reads data, displays it to screen and sends to output */
netwib_err l4_write(netwib_io *pio,
                    netwib_constbuf *pbuf);
netwib_err l4_write(netwib_io *pio,
                    netwib_constbuf *pbuf)
{
  netwib_buf bufbase64;
  netwib_io *pionext;
  netwib_err ret;

  /* obtain the next io in the chain */
  netwib_er(netwib_io_next_write(pio, &pionext));

  /* display it */
  netwib_er(netwib_buf_display(pbuf, NETWIB_ENCODETYPE_DUMP));

  /* write data to the next io */
  ret = netwib_io_write(pionext, pbuf);

  return(ret);
}

netwib_err l4_init(netwib_io **ppio);
netwib_err l4_init(netwib_io **ppio)
{
  netwib_er(netwib_io_init(NETWIB_FALSE, NETWIB_TRUE, NULL,
                           NULL/*read*/, &l4_write,
                           NULL/*wait*/, NULL/*unread*/,
                           NULL/*ctlset*/, NULL/*ctlget*/,
                           NULL/*close*/, ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* l5 convert data to uppercase */
netwib_err l5_up(netwib_constbuf *pinbuf,
                 netwib_buf *poutbuf);
netwib_err l5_up(netwib_constbuf *pinbuf,
                 netwib_buf *poutbuf)
{
  netwib_data data;
  netwib_uint32 datasize, i;
  netwib_byte c;

  data = netwib__buf_ref_data_ptr(pinbuf);
  datasize = netwib__buf_ref_data_size(pinbuf);
  for (i = 0; i < datasize; i++) {
    c = data[i];
    netwib_c2_upper(c);
    netwib_er(netwib_buf_append_byte(c, poutbuf));
  }

  return(NETWIB_ERR_OK);
}
netwib_err l5_read(netwib_io *pio,
                   netwib_buf *pbuf);
netwib_err l5_read(netwib_io *pio,
                   netwib_buf *pbuf)
{
  netwib_buf buf;
  netwib_io *pionext;

  netwib_er(netwib_io_next_read(pio, &pionext));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_io_read(pionext, &buf));
  netwib_er(l5_up(&buf, pbuf));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}
netwib_err l5_write(netwib_io *pio,
                    netwib_constbuf *pbuf);
netwib_err l5_write(netwib_io *pio,
                    netwib_constbuf *pbuf)
{
  netwib_buf buf;
  netwib_io *pionext;
  netwib_err ret;

  netwib_er(netwib_io_next_write(pio, &pionext));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(l5_up(pbuf, &buf));
  ret = netwib_io_write(pionext, &buf);
  netwib_er(netwib_buf_close(&buf));

  return(ret);
}
netwib_err l5_init(netwib_io **ppio);
netwib_err l5_init(netwib_io **ppio)
{
  netwib_er(netwib_io_init(NETWIB_FALSE, NETWIB_TRUE, NULL,
                           &l5_read, &l5_write,
                           NULL/*wait*/, NULL/*unread*/,
                           NULL/*ctlset*/, NULL/*ctlget*/,
                           NULL/*close*/, ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* module m */
netwib_err m(netwib_io *pi1,
             netwib_io *po1,
             netwib_io *po2);
netwib_err m(netwib_io *pi1,
             netwib_io *po1,
             netwib_io *po2)
{
  netwib_buf buf;
  netwib_err ret;
  netwib_io *pl1, *pl2;
  netwib_data data;
  netwib_uint32 datasize;

  /* create l1 */
  netwib_er(netwib_io_init_line(&pl1));

  /* create l2 */
  netwib_er(l2_init(&pl2));

  /* add l1 to i1 */
  netwib_er(netwib_io_plug_read(pl1, pi1));

  /* add l2 to o2 */
  netwib_er(netwib_io_plug_read(pl2, po2));

  /* main loop */
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  while (NETWIB_TRUE) {
    netwib__buf_reinit(&buf);
    ret = netwib_io_read(pl1, &buf);
    if (ret == NETWIB_ERR_DATAEND) {
      ret = NETWIB_ERR_OK;
      break;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    data = netwib__buf_ref_data_ptr(&buf);
    datasize = netwib__buf_ref_data_size(&buf);
    if (datasize && data[0] == 'A') {
      ret = netwib_io_write(po1, &buf);
    } else {
      ret = netwib_io_write(pl2, &buf);
    }
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_buf_close(&buf));

  /* close only l1 and l2 (not the complete chain) */
  netwib_er(netwib_io_close(&pl1));
  netwib_er(netwib_io_close(&pl2));

  return(ret);
}

/*-------------------------------------------------------------*/
/* usage 1 */
netwib_err usage1(void);
netwib_err usage1(void)
{
  netwib_err ret;
  netwib_buf filename;
  netwib_io *pfile1, *pfile2, *pfile3, *pl3 , *pl4;

  /* open files */
  netwib_er(netwib_buf_init_ext_string("file1", &filename));
  netwib_er(netwib_io_init_file_read(&filename, &pfile1));
  netwib_er(netwib_buf_init_ext_string("file2", &filename));
  netwib_er(netwib_io_init_file_write(&filename, &pfile2));
  netwib_er(netwib_buf_init_ext_string("file3", &filename));
  netwib_er(netwib_io_init_file_write(&filename, &pfile3));

  /* initialize l3 and l4 */
  netwib_er(l3_init(&pl3));
  netwib_er(l4_init(&pl4));

  /* create chains */
  netwib_er(netwib_io_plug_read(pl3, pfile1));
  netwib_er(netwib_io_plug_write(pl4, pfile3));

  /* core loop */
  ret = m(pl3, pfile2, pl4);

  /* close chains */
  netwib_er(netwib_io_close(&pl3)); /* or close_read */
  netwib_er(netwib_io_close(&pfile2)); /* or close_write */
  netwib_er(netwib_io_close(&pl4)); /* or close_write */

  return(ret);
}

/*-------------------------------------------------------------*/
/* usage 2 */
netwib_err usage2(void);
netwib_err usage2(void)
{
  netwib_err ret;
  netwib_io *psock, *pnull;
  netwib_ip ipad;

  /* open socket */
  netwib_er(netwib_ip_init_ip4(0x7F000001, &ipad));
  netwib_er(netwib_io_init_sock_tcp_cli_easy(&ipad, 1234, &psock));

  /* open null */
  netwib_er(netwib_io_init_null(&pnull));

  /* core loop */
  ret = m(psock, pnull, psock);

  /* close chains */
  netwib_er(netwib_io_close(&psock));
  netwib_er(netwib_io_close(&pnull)); /* or close_write */

  return(ret);
}

/*-------------------------------------------------------------*/
/* usage 3 */
netwib_err usage3(void);
netwib_err usage3(void)
{
  netwib_err ret;
  netwib_io *psock, *pnull, *pl3 , *pl4;
  netwib_ip ipad;

  /* open socket */
  netwib_er(netwib_ip_init_ip4(0x7F000001, &ipad));
  netwib_er(netwib_io_init_sock_tcp_cli_easy(&ipad, 1234, &psock));

  /* open null */
  netwib_er(netwib_io_init_null(&pnull));

  /* initialize l3 and l4 */
  netwib_er(l3_init(&pl3));
  netwib_er(l4_init(&pl4));

  /* create chains */
  netwib_er(netwib_io_plug_read(pl3, psock));
  netwib_er(netwib_io_plug_write(pl4, psock));

  /* core loop */
  ret = m(pl3, pnull, pl4);

  /* close chains */
  netwib_er(netwib_io_close(&pl3)); /* or close_read */
  netwib_er(netwib_io_close(&pnull)); /* or close_write */
  netwib_er(netwib_io_close(&pl4)); /* or close_write */

  return(ret);
}

/*-------------------------------------------------------------*/
/* usage 4 */
netwib_err usage4(void);
netwib_err usage4(void)
{
  netwib_err ret;
  netwib_io *psock, *pnull, *pl5;
  netwib_ip ipad;

  /* open socket */
  netwib_er(netwib_ip_init_ip4(0x7F000001, &ipad));
  netwib_er(netwib_io_init_sock_tcp_cli_easy(&ipad, 1234, &psock));

  /* open null */
  netwib_er(netwib_io_init_null(&pnull));

  /* initialize l5 */
  netwib_er(l5_init(&pl5));

  /* create chains */
  netwib_er(netwib_io_plug_read(pl5, psock));
  netwib_er(netwib_io_plug_write(pl5, psock));

  /* core loop */
  ret = m(pl5, pnull, pl5);

  /* close chains */
  netwib_er(netwib_io_close(&pl5));
  netwib_er(netwib_io_close(&pnull)); /* or close_write */

  return(ret);
}

/*-------------------------------------------------------------*/
/* usage 5 */
netwib_err usage5(void);
netwib_err usage5(void)
{
  netwib_err ret;
  netwib_buf filename;
  netwib_io *pfile1, *pfile2, *pnull, *pl5;

  /* open files */
  netwib_er(netwib_buf_init_ext_string("file1", &filename));
  netwib_er(netwib_io_init_file_read(&filename, &pfile1));
  netwib_er(netwib_buf_init_ext_string("file2", &filename));
  netwib_er(netwib_io_init_file_write(&filename, &pfile2));

  /* open null */
  netwib_er(netwib_io_init_null(&pnull));

  /* initialize l5 */
  netwib_er(l5_init(&pl5));

  /* create chains */
  netwib_er(netwib_io_plug_read(pl5, pfile1));
  netwib_er(netwib_io_plug_write(pl5, pfile2));

  /* core loop */
  ret = m(pl5, pnull, pl5);

  /* close chains */
  netwib_er(netwib_io_close(&pl5));
  netwib_er(netwib_io_close(&pnull)); /* or close_write */

  return(ret);
}

/*-------------------------------------------------------------*/
/* usage 6 */
netwib_err usage6(void);
netwib_err usage6(void)
{
  netwib_err ret;
  netwib_buf filename;
  netwib_io *pfile1, *pfile2;

  /* open files */
  netwib_er(netwib_buf_init_ext_string("file1", &filename));
  netwib_er(netwib_io_init_file_read(&filename, &pfile1));
  netwib_er(netwib_buf_init_ext_string("file2", &filename));
  netwib_er(netwib_io_init_file_write(&filename, &pfile2));

  /* core loop */
  ret = m(pfile1, pfile2, pfile2);

  /* close chains */
  netwib_er(netwib_io_close(&pfile1));
  netwib_er(netwib_io_close(&pfile2));

  return(ret);
}

/*-------------------------------------------------------------*/
/* usage 7 */
netwib_err usage7(void);
netwib_err m2(netwib_io *pi1,
              netwib_io *pi2,
              netwib_io *po1);
netwib_err usage7(void)
{
  netwib_err ret;
  netwib_buf filename;
  netwib_io *pfile1, *pnull;

  /* open files */
  netwib_er(netwib_buf_init_ext_string("file1", &filename));
  netwib_er(netwib_io_init_file_read(&filename, &pfile1));

  /* open null */
  netwib_er(netwib_io_init_null(&pnull));

  /* core loop */
  ret = m2(pfile1, pfile1, pnull);

  /* close chains */
  netwib_er(netwib_io_close(&pfile1));
  netwib_er(netwib_io_close(&pnull));

  return(ret);
}
#endif
