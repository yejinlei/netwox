
/*-------------------------------------------------------------*/
netwib_err netwox_buf_append_pad(netwib_byte b,
                                 netwib_uint32 size,
                                 netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_buf_append_xor(netwib_constbuf *pbuf1,
                                 netwib_constbuf *pbuf2,
                                 netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_buf_lstrip(netwib_buf *pbuf);
netwib_err netwox_buf_rstrip(netwib_buf *pbuf);
netwib_err netwox_buf_strip(netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_buf_lowercase(netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* for test purposes only */
netwib_err netwox_buf_append_strmixed(netwib_conststring mixed,
                                      netwib_buf *pbuf);
netwib_err netwox_buf_check_strmixed(netwib_buf *pbuf,
                                     netwib_conststring wantedmixed);
netwib_err netwox_buf_check_strtxt(netwib_buf *pbuf,
                                   netwib_conststring wantedtxt);

/*-------------------------------------------------------------*/
/*
netwox_buf_findlist findlist = {
  {"bob", 1},
  {"cob", 2},
  NETWOX_BUF_FINDLIST_END
};
*/
typedef struct {
  netwib_conststring str;
  netwib_uint32 ui32;
} netwox_buf_findlist[];
#define NETWOX_BUF_FINDLIST_END {NULL, 0}
netwib_err netwox_buf_find(netwox_buf_findlist findlist,
                           netwib_bool findlistissorted,
                           netwib_bool casesensitive,
                           netwib_constbuf *pbuf,
                           netwib_uint32 *pui);
