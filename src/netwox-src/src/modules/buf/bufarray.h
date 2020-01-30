
/*
  This module manages an array of buffers.
*/

/*-------------------------------------------------------------*/
netwib_err netwox_bufarray_init(netwib_array *parray);
netwib_err netwox_bufarray_close(netwib_array *parray);

/*-------------------------------------------------------------*/
/* add a buf at the end of the array */
netwib_err netwox_bufarray_add_buf(netwib_array *parray,
                                   netwib_constbuf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_bufarray_contains(netwib_constarray *parray,
                                    netwib_constbuf *pbuf,
                                    netwib_bool *pyes);

/*-------------------------------------------------------------*/
/* for test purposes only */
netwib_err netwox_bufarray_check_strtxt(netwib_constarray *parray,
                                        netwib_conststring wantedtxt);

/*-------------------------------------------------------------*/
/* this function is specific: it adds extensions to an array */
netwib_err netwox_bufarray_add_extensions(netwib_array *parray,
                                          netwib_constbuf *pextensions);

