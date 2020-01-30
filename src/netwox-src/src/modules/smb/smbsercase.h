
/*-------------------------------------------------------------*/
/* pathnames in queries are often caseless. So, we have to find the
   correct case if possible. This function tries to update the
   real filename, else it is left unchanged */
netwib_err netwox_smbsercase_update(netwib_bufpool *pbufpool,
                                    netwib_pathstat_type pathtype,
                                    netwib_buf *prealpathname);

/*-------------------------------------------------------------*/
netwib_err netwox_smbsercase_test(void);
