
/*-------------------------------------------------------------*/
/*                             READ                            */
/*-------------------------------------------------------------*/
/* io reading from a file, removing empty lines and
   comments (lines starting with #) */
netwib_err netwox_io_init_conffile_read(netwib_constbuf *pfilename,
                                        netwib_io **ppio);

/* read one line, and decode "var = val" */
netwib_err netwox_conffile_read_var(netwib_bufpool *pbufpool,
                                    netwib_io *pio,
                                    netwib_buf *pvar,
                                    netwib_buf *pvalbuf,
                                    netwib_bool *pvaluintdecoded,
                                    netwib_uint32 *pvaluint);

/* read all valid data in a conffile and concatenate it */
netwib_err netwox_conffile_read_concat(netwib_constbuf *pfilename,
                                       netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/*                             WRITE                           */
/*-------------------------------------------------------------*/
/* initialize the io */
#define netwox_io_init_conffile_write(pfilename,ppio) netwib_io_init_file_textwrite(pfilename,ppio)

/* help message */
netwib_err netwox_conffile_write_help(netwib_bufpool *pbufpool,
                                      netwib_io *pio);

/* # cmt */
netwib_err netwox_conffile_write_cmt(netwib_bufpool *pbufpool,
                                     netwib_io *pio,
                                     netwib_constbuf *pcmt);

/* var = "va\"lue" */
netwib_err netwox_conffile_write__var(netwib_bufpool *pbufpool,
                                      netwib_io *pio,
                                      netwib_bool cmt,
                                      netwib_bool indented,
                                      netwib_constbuf *pvar,
                                      netwib_constbuf *pval);
#define netwox_conffile_write_var(pbufpool,pio,pvar,pval) netwox_conffile_write__var(pbufpool,pio,NETWIB_FALSE,NETWIB_FALSE,pvar,pval)
#define netwox_conffile_write_varcmt(pbufpool,pio,pvar,pval) netwox_conffile_write__var(pbufpool,pio,NETWIB_TRUE,NETWIB_FALSE,pvar,pval)
#define netwox_conffile_write_varind(pbufpool,pio,pvar,pval) netwox_conffile_write__var(pbufpool,pio,NETWIB_FALSE,NETWIB_TRUE,pvar,pval)
#define netwox_conffile_write_varcmtind(pbufpool,pio,pvar,pval) netwox_conffile_write__var(pbufpool,pio,NETWIB_TRUE,NETWIB_TRUE,pvar,pval)

/* var = 123 */
netwib_err netwox_conffile_write__var_uint32(netwib_bufpool *pbufpool,
                                             netwib_io *pio,
                                             netwib_bool cmt,
                                             netwib_bool indented,
                                             netwib_constbuf *pvar,
                                             netwib_uint32 val);
#define netwox_conffile_write_var_uint32(pbufpool,pio,pvar,val) netwox_conffile_write__var_uint32(pbufpool,pio,NETWIB_FALSE,NETWIB_FALSE,pvar,val)
#define netwox_conffile_write_varcmt_uint32(pbufpool,pio,pvar,val) netwox_conffile_write__var_uint32(pbufpool,pio,NETWIB_TRUE,NETWIB_FALSE,pvar,val)
#define netwox_conffile_write_varind_uint32(pbufpool,pio,pvar,val) netwox_conffile_write__var_uint32(pbufpool,pio,NETWIB_FALSE,NETWIB_TRUE,pvar,val)
#define netwox_conffile_write_varcmtind_uint32(pbufpool,pio,pvar,val) netwox_conffile_write__var_uint32(pbufpool,pio,NETWIB_TRUE,NETWIB_TRUE,pvar,val)

/* empty line */
netwib_err netwox_conffile_write_empty(netwib_io *pio);
