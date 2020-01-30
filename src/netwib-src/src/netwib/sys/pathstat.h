
/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_PATHSTAT_TYPE_UNKNOWN = 0, /* unknown type */
  NETWIB_PATHSTAT_TYPE_REG,         /* regular file */
  NETWIB_PATHSTAT_TYPE_DIR,         /* directory */
  NETWIB_PATHSTAT_TYPE_LINK,        /* link */
  NETWIB_PATHSTAT_TYPE_SOCK,        /* socket */
  NETWIB_PATHSTAT_TYPE_BLOCK,       /* block file */
  NETWIB_PATHSTAT_TYPE_CHAR,        /* char file */
  NETWIB_PATHSTAT_TYPE_FIFO         /* fifo */
} netwib_pathstat_type;

/*-------------------------------------------------------------*/
/***************************************************************
 * On conventional filesystems, maximum file size is           *
 * 0x7FFFFFFF (2G) because it is stored as signed (as netwib   *
 * defines size as unsigned, we could extend this limit to 4G, *
 * but this poses some other problems because all underlying   *
 * system calls have a limit of 2G).                           *
 * On large filesystems, maximum file size can be greater and  *
 * might not feet in an netwib_uint32 size. If file size is    *
 * > 0x7FFFFFFF, size is set to NETWIB_PATHSTAT_SIZE_GT2G.     *
 * In both cases, size64 variable contains the real value.     *
 ***************************************************************/
#define NETWIB_PATHSTAT_SIZE_GT2G 0x80000000u
typedef struct {
  netwib_pathstat_type type;
  netwib_uint32 size; /* set to NETWIB_PATHSTAT_SIZE_GT2G
                         if netwib_uint32 variable is too
                         small to contain real size */
  netwib_uint64 size64; /* real size, not truncated */
  netwib_time mtime; /* last modification time */
} netwib_pathstat;

/*-------------------------------------------------------------*/
/* Name : netwib_pathstat_init
   Description :
     Get information of a path.
   Input parameter(s) :
     *ppathstat : pathstat
   Input/output parameter(s) :
   Output parameter(s) :
     *ptype : type of the path
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_OKFILENOTFOUND : file was not found
*/
netwib_err netwib_pathstat_init(netwib_constbuf *ppath,
                                netwib_pathstat *pstat);
