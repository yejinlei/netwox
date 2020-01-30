
/*-------------------------------------------------------------*/
/* file id used for each session */
typedef enum {
  NETWOX_SMBSERFID_FIDTYPE_UNKNOWN = 0,
  NETWOX_SMBSERFID_FIDTYPE_REG,
  NETWOX_SMBSERFID_FIDTYPE_DIR
} netwox_smbserfid_fidtype;
typedef enum {
  NETWOX_SMBSERFID_REGIOTYPE_NOTOPEN = 0, /* io is not opened */
  NETWOX_SMBSERFID_REGIOTYPE_READ = NETWIB_FILE_INITTYPE_READ,
  NETWOX_SMBSERFID_REGIOTYPE_WRITE = NETWIB_FILE_INITTYPE_WRITE,
  NETWOX_SMBSERFID_REGIOTYPE_APPEND = NETWIB_FILE_INITTYPE_APPEND,
  NETWOX_SMBSERFID_REGIOTYPE_RDWR = NETWIB_FILE_INITTYPE_RDWR
} netwox_smbserfid_regiotype;
typedef struct {
  netwox_smbserfid_regiotype regiotype;
  netwib_io *pio;
  netwox_smbcmdcmn_uint64 offset; /* for read and write */
  netwib_buf readbuf;         /* to be reset after a write/seek command */
  netwib_bool readeofreached; /* to be reset after a write/seek command */
} netwox_smbserfid_itemreg;
typedef struct {
  /* common */
  netwib_buf realname;
  netwib_bool deleteonclose;
  /* type of fid (don't use before calling item_value_setdefault) */
  netwox_smbserfid_fidtype fidtype;
  /* value depends on type of fid */
  union {
    netwox_smbserfid_itemreg itemreg;
  } value;
} netwox_smbserfid_item;
netwib_err netwox_smbserfid_item_init(netwox_smbserfid_item *pitem);
netwib_err netwox_smbserfid_item_value_setdefault(netwox_smbserfid_item *pitem,
                                                  netwox_smbserfid_fidtype fidtype);
netwib_err netwox_smbserfid_item_close(netwox_smbserfid_item *pitem);

/*-------------------------------------------------------------*/
/* obtain name relative from rootdir */
netwib_err netwox_smbserfid_item_svcname(netwib_constbuf *prootdir,
                                         netwox_smbserfid_item *pitem,
                                         netwib_buf *psvcname);

/*-------------------------------------------------------------*/
/* functions needed for storage in the hash */
#define netwox_smbserfid_itemptr_malloc(pptr) netwib_ptr_malloc(sizeof(netwox_smbserfid_item), pptr)
netwib_err netwox_smbserfid_itemptr_erase(netwib_ptr ptr);
netwib_err netwox_smbserfid_itemptr_duplicate(netwib_constptr ptr,
                                              netwib_ptr *pptrdup);
#define netwox_smbserfid_item_hash_init(pphash) netwib_hash_init(&netwox_smbserfid_itemptr_erase,&netwox_smbserfid_itemptr_duplicate,pphash)
#define netwox_smbserfid_item_hash_close(pphash) netwib_hash_close(pphash,NETWIB_TRUE)

/*-------------------------------------------------------------*/
/* hash of resources */
typedef struct {
  netwib_uint16 nextnumber;
  netwib_hash *phash;
} netwox_smbserfid;
typedef const netwox_smbserfid netwox_constsmbserfid;
netwib_err netwox_smbserfid_init(netwox_smbserfid *psmbserfid);
netwib_err netwox_smbserfid_close(netwox_smbserfid *psmbserfid);

/*-------------------------------------------------------------*/
/* create an item, init it, and add it in the hash */
netwib_err netwox_smbserfid_new(netwox_smbserfid *psmbserfid,
                                netwib_uint16 *pfid,
                                netwox_smbserfid_item **ppitem);
netwib_err netwox_smbserfid_search(netwox_smbserfid *psmbserfid,
                                   netwib_uint16 fid,
                                   netwox_smbserfid_item **ppitem);
netwib_err netwox_smbserfid_del(netwox_smbserfid *psmbserfid,
                                netwib_uint16 fid);

