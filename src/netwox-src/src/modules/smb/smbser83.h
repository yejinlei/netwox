
/*-------------------------------------------------------------*/
typedef struct {
  netwib_bufpool *pbufpool;
  netwib_uint32 count;
  netwib_hash *p83tolong;
  netwib_hash *plongto83;
} netwox_smbser83;
typedef const netwox_smbser83 netwox_constsmbser83;
netwib_err netwox_smbser83_init(netwib_bufpool *pbufpool,
                                netwox_smbser83 *psmbser83);
netwib_err netwox_smbser83_close(netwox_smbser83 *psmbser83);

/*-------------------------------------------------------------*/
/* convert a filename to 8.3 names, and dirnames to 8 names:
     abcd            => ABCD         idem   (don't add in hash table)
     abcd.efg        => ABCD.EFG     ABCDEF~1
     abcd.efgh       => ABCD~1.EFG   ABCDEF~1
     ab.cd.ef        => ABCD~1.EF    ABCDEF~1
     ab cd           => ABCD~1       idem
   if already in hash table, obtain the corresponding name instead
*/
netwib_err netwox_smbser83_longto83(netwox_smbser83 *psmbser83,
                                    netwib_pathstat_type pathtype,
                                    netwib_constbuf *plongpathname,
                                    netwib_buf *p83pathname);

/*-------------------------------------------------------------*/
/* convert a path to 8.3 names:
     /abcde ghi klm\no pqrs.t/ => /ABCDEGH~1\NOPRST~2/
*/
netwib_err netwox_smbser83_longto83_path(netwox_smbser83 *psmbser83,
                                         netwib_pathstat_type pathtype,
                                         netwib_constbuf *plongpathname,
                                         netwib_buf *p83pathname);

/*-------------------------------------------------------------*/
/* convert a 8.3 name to filename:
     ABCD~1.EFG ==(hash_table)==> abcd.efgh
     abcd~1.efg ==(uppercase)==> ABCD~1.EFG ==(hash_table)==> abcd.efgh
   if not found in hash table, keep the name:
     ABCD ====> ABCD, later converted with netwox_smbsercase_update
 */
netwib_err netwox_smbser83_83tolong(netwox_smbser83 *psmbser83,
                                    netwib_constbuf *p83pathname,
                                    netwib_buf *plongpathname);

/*-------------------------------------------------------------*/
/* convert a path of 8.3 names to path:
     /ABCDEGH~1\NOPRST~2/ => /abcde ghi klm\no pqrs.t/
*/
netwib_err netwox_smbser83_83tolong_path(netwox_smbser83 *psmbser83,
                                         netwib_constbuf *p83pathname,
                                         netwib_buf *plongpathname);

/*-------------------------------------------------------------*/
netwib_err netwox_smbser83_test(void);
