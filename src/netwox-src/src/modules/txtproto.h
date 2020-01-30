
/*-------------------------------------------------------------*/
/* This function prepares the io for reading line by line (writing
   is transparent). */
netwib_err netwox_txtproto_io_rd(netwib_io *pio,
                                 netwib_io **ppio);
/* This function prepares the io for reading and writing line by line. */
netwib_err netwox_txtproto_io_rdwr(netwib_io *pio,
                                   netwib_bool msdos,
                                   netwib_io **ppio);

/*-------------------------------------------------------------*/
/* SMTP protocol use error messages like this :
    ^200 OK
    ^201-foo
    ^201-whatever
    ^201 bar
   FTP protocol use error messages like this :
    ^200 OK
    ^201-foo
    ^whatever
    ^    whatever
    ^ 33 whatever
    ^201 bar  ["201 " is the end mark, because it corresponds to first line]
   NNTP protocol use error messages like this :
    ^200 OK
    ^200[nospace] -> converted to "200 "
   This function supports those 3 protocols using these rules :
    - must start with "nnn " or "nnn-" : set errnum
    - if "nnn " is found, this is the last line
    - all comments are concatenated (removing leading spaces or "nnn-")
*/
netwib_err netwox_txtproto_read_reply(netwib_io *pio,
                                      netwib_uint32 maxwaitms,
                                      netwib_uint32 *perrnum,
                                      netwib_buf *perrmsg);

/*-------------------------------------------------------------*/
/* Read a query. Unfortunately, queries are not compatible
   between text protocols "USER password, RETR_space_filename,
   MAIL FROM: "username" <email>".
   So, this function only read a line. Specific functions
   are needed for each protocol. */
netwib_err netwox_txtproto_read_query(netwib_io *pio,
                                      netwib_uint32 maxwaitms,
                                      netwib_buf *pline);

/*-------------------------------------------------------------*/
/* Write a reply (init with netwox_txtproto_io_rdwr) */
netwib_err netwox_txtproto_write_reply(netwib_io *pio,
                                       netwib_uint32 errnum,
                                       netwib_constbuf *perrmsg);
netwib_err netwox_txtproto_write_reply_text(netwib_io *pio,
                                            netwib_uint32 errnum,
                                            netwib_conststring errmsg);

/*-------------------------------------------------------------*/
/* Write a query (init with netwox_txtproto_io_rdwr) */
netwib_err netwox_txtproto_write_query(netwib_io *pio,
                                       netwib_constbuf *pline);

/*-------------------------------------------------------------*/
/* Expect a reply code */
netwib_err netwox_txtproto_expect5(netwib_uint32 errnum,
                                   netwib_buf *perrmsg,
                                   netwib_uint32 expectederrnum1,
                                   netwib_uint32 expectederrnum2,
                                   netwib_uint32 expectederrnum3,
                                   netwib_uint32 expectederrnum4,
                                   netwib_uint32 expectederrnum5);
netwib_err netwox_txtproto_expect4(netwib_uint32 errnum,
                                   netwib_buf *perrmsg,
                                   netwib_uint32 expectederrnum1,
                                   netwib_uint32 expectederrnum2,
                                   netwib_uint32 expectederrnum3,
                                   netwib_uint32 expectederrnum4);
netwib_err netwox_txtproto_expect3(netwib_uint32 errnum,
                                   netwib_buf *perrmsg,
                                   netwib_uint32 expectederrnum1,
                                   netwib_uint32 expectederrnum2,
                                   netwib_uint32 expectederrnum3);
netwib_err netwox_txtproto_expect2(netwib_uint32 errnum,
                                   netwib_buf *perrmsg,
                                   netwib_uint32 expectederrnum1,
                                   netwib_uint32 expectederrnum2);
netwib_err netwox_txtproto_expect1(netwib_uint32 errnum,
                                   netwib_buf *perrmsg,
                                   netwib_uint32 expectederrnum1);
