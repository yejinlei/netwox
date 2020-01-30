
/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
netwib_err netwib_priv_fd_block_set(int fd,
                                    netwib_bool beblocking);
netwib_err netwib_priv_fd_block_get(int fd,
                                    netwib_bool *pbeblocking);
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_fd_write(int fd,
                                netwib_constbuf *pbuf);
netwib_err netwib_priv_fd_write_uint32(int fd,
                                       netwib_uint32 ui);
#if defined NETWIBDEF_SYSNAME_Unix
netwib_err netwib_priv_fdpipe_write(int fd,
                                    netwib_constbuf *pbuf);
netwib_err netwib_priv_fdpipe_write_uint32(int fd,
                                           netwib_uint32 ui);
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_fd_wait(int fd,
                               netwib_io_waytype way,
                               netwib_consttime *pabstime,
                               netwib_bool *pevent);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_fd_read(int fd,
                               netwib_buf *pbuf);
netwib_err netwib_priv_fd_read_byte(int fd,
                                    netwib_byte *pb);
netwib_err netwib_priv_fd_read_uint32(int fd,
                                      netwib_uint32 *pui);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_fd_flush(int fd);
