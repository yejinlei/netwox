
/*-------------------------------------------------------------*/
netwib_err netwib_debug_string_init(netwib_string *pstringout);
netwib_err netwib_debug_string_close(netwib_string *pstringout);
netwib_err netwib_debug_string_append_string(netwib_conststring text,
                                             netwib_string *pstringout);
netwib_err netwib_debug_string_append_fmt(netwib_string *pstringout,
                                          netwib_conststring fmt,
                                          ...);
