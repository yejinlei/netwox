
/*-------------------------------------------------------------*/
/* Value stringoutmaxsize corresponds to the max size of a
   string. For example :
       netwib_char array[5];
       netwib_debug_string2_init_xyz(..., 4, array);
       netwib_debug_string2_init_xyz(..., sizeof(array)-1, array);
*/
netwib_err netwib_debug_string2_init(netwib_string stringout);
netwib_err netwib_debug_string2_append_string(netwib_conststring text,
                                              netwib_uint32 stringoutmaxsize,
                                              netwib_string stringout);
netwib_err netwib_debug_string2_append_fmt(netwib_uint32 stringoutmaxsize,
                                           netwib_string stringout,
                                           netwib_conststring fmt,
                                           ...);
