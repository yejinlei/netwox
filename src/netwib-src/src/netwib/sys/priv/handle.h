
/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_HANDLETYPE_UNKNOWN = 0,
  NETWIB_HANDLETYPE_FILE,
  NETWIB_HANDLETYPE_CONSOLE,
  NETWIB_HANDLETYPE_PIPE
} netwib_handletype;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_handle_type(NETWIBHANDLE h,
                                   netwib_handletype *phandletype);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_handle_write(NETWIBHANDLE h,
                                    netwib_constbuf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_handle_read(NETWIBHANDLE h,
                                   netwib_buf *pbuf);
netwib_err netwib_priv_handle_read_byte(NETWIBHANDLE h,
                                        netwib_byte *pb);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_handle_wait(NETWIBHANDLE h,
                                   netwib_handletype handletype,
                                   netwib_io_waytype way,
                                   netwib_consttime *pabstime,
                                   netwib_bool *pevent);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_handle_waitsimple(NETWIBHANDLE h,
                                         netwib_consttime *pabstime,
                                         netwib_bool *pevent);
