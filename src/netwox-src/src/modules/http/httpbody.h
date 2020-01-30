
/*-------------------------------------------------------------*/
netwib_err netwox_httpbody_read_io_close(netwib_bufpool *pbufpool,
                                         netwib_uint32 timeoutms,
                                         netwib_io *psockio,
                                         netwib_io *pbodyio);
netwib_err netwox_httpbody_read_io_length(netwib_bufpool *pbufpool,
                                          netwib_uint32 timeoutms,
                                          netwib_uint32 contentlength,
                                          netwib_io *psockio,
                                          netwib_io *pbodyio);
netwib_err netwox_httpbody_read_io(netwib_bufpool *pbufpool,
                                   netwib_uint32 timeoutms,
                                   netwib_bool contentlengthset,
                                   netwib_uint32 contentlength,
                                   netwib_io *psockio,
                                   netwib_io *pbodyio);

/*-------------------------------------------------------------*/
netwib_err netwox_httpbody_write_io_close(netwib_bufpool *pbufpool,
                                          netwib_io *pbodyio,
                                          netwib_io *psockio);

/*-------------------------------------------------------------*/
netwib_err netwox_httpbody_read_buf_close(netwib_uint32 timeoutms,
                                          netwib_io *psockio,
                                          netwib_buf *pbodybuf);
netwib_err netwox_httpbody_read_buf_length(netwib_uint32 timeoutms,
                                           netwib_uint32 contentlength,
                                           netwib_io *psockio,
                                           netwib_buf *pbodybuf);
netwib_err netwox_httpbody_read_buf(netwib_uint32 timeoutms,
                                    netwib_bool contentlengthset,
                                    netwib_uint32 contentlength,
                                    netwib_io *psockio,
                                    netwib_buf *pbodybuf);

/*-------------------------------------------------------------*/
netwib_err netwox_httpbody_write_buf_close(netwib_constbuf *pbodybuf,
                                           netwib_io *psockio);


