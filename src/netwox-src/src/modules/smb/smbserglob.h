
/*-------------------------------------------------------------*/
/*
   Search pattern is of the form:
    - \directory\glob
    - \glob
    - glob (equivalent to \glob)
   The glob part can be for example:
    - *
    - *.txt *.c
    - <.txt <.c
    - >"txt
    - >>>>>>>>">>>
    - >>>>>>>>"TXT
    - >>>>>>>>
    - ?*.TXT
    - ?.TXT
    - file*
    - file.???
    - file"*
    - file
    - file.ext
   The meaning of the glob part varies:
    - Search pattern is cut in two parts analyzed separately:
       + file.ext : file part and ext part
       + file.    : file with an optional extension
       + file     : file with an optional extension
       + .ext     : any file with extension
    - Each part has the same meaning:
       + [empty]  : zero or more characters
       + ABC      : only ABC
       + *        : zero or more characters
       + ?ABC     : one character, then ABC
       + ABC?     : ABC, then one character
       + ABC??    : ABC, then one or two characters
       + ABC????? : ABC, then one to five characters
       + ???????? : one or more characters
    - special characters have equivalents:
       + > = ?  (however, >>>>>>>>.>>> has to be considered as *.*)
       + " = .
       + < = *
*/

/*-------------------------------------------------------------*/
/* convert a globbing pattern to a regular expression */
netwib_err netwox_smbserglob_re(netwib_constbuf *pglob,
                                netwib_buf *pre);
netwib_err netwox_smbserglob_re_test(void);

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bufpool *pbufpool;
  netwox_smbcmdcmn_fileattr32 searchattributes;
  netwib_buf *prealdirname;
  netwib_buf *pfilere;
  netwib_dir *pdir;
  netwib_buf *psaved; /* for netwox_smbserglob_item_save() */
  netwib_bool volumegiven;
} netwox_smbserglob_item;
netwib_err netwox_smbserglob_item_init(netwib_bufpool *pbufpool,
                                       netwox_smbserglob_item *pitem);
netwib_err netwox_smbserglob_item_set(netwox_smbserglob_item *pitem,
                                      netwox_smbcmdcmn_fileattr32 searchattributes,
                                      netwib_constbuf *prealdirname,
                                      netwib_constbuf *pfileglob);
netwib_err netwox_smbserglob_item_close(netwox_smbserglob_item *pitem);

/*-------------------------------------------------------------*/
netwib_err netwox_smbserglob_item_next(netwox_smbser83 *psmbser83,
                                       netwox_smbserglob_item *pitem,
                                       netwib_buf *prealpathname);
/* sometimes, we need to save back for next call */
netwib_err netwox_smbserglob_item_save(netwox_smbserglob_item *pitem,
                                       netwib_constbuf *prealpathname);

/*-------------------------------------------------------------*/
/* functions needed for storage in the hash */
#define netwox_smbserglob_itemptr_malloc(pptr) netwib_ptr_malloc(sizeof(netwox_smbserglob_item), pptr)
netwib_err netwox_smbserglob_itemptr_erase(netwib_ptr ptr);
netwib_err netwox_smbserglob_itemptr_duplicate(netwib_constptr ptr,
                                               netwib_ptr *pptrdup);
#define netwox_smbserglob_item_hash_init(pphash) netwib_hash_init(&netwox_smbserglob_itemptr_erase,&netwox_smbserglob_itemptr_duplicate,pphash)
#define netwox_smbserglob_item_hash_close(pphash) netwib_hash_close(pphash,NETWIB_TRUE)

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bufpool *pbufpool;
  netwib_uint32 nextnumber;
  netwib_hash *phash;
} netwox_smbserglob;
netwib_err netwox_smbserglob_init(netwib_bufpool *pbufpool,
                                  netwox_smbserglob *psmbserglob);
netwib_err netwox_smbserglob_close(netwox_smbserglob *psmbserglob);

/*-------------------------------------------------------------*/
netwib_err netwox_smbserglob_new(netwox_smbserglob *psmbserglob,
                                 netwib_uint32 *pid,
                                 netwox_smbserglob_item **ppitem);
netwib_err netwox_smbserglob_search(netwox_smbserglob *psmbserglob,
                                    netwib_uint32 id,
                                    netwox_smbserglob_item **ppitem);
netwib_err netwox_smbserglob_del(netwox_smbserglob *psmbserglob,
                                 netwib_uint32 id);
