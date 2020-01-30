
/*-------------------------------------------------------------*/
netwib_err netwib_debug_exec_init(netwib_conststring command,
                                  netwib_uint32 *ppid,
                                  int *pfd);
netwib_err netwib_debug_exec_close(netwib_uint32 pid,
                                   int fd);

/* use netwib_debug_string_close to free string */
netwib_err netwib_debug_exec_line(int fd,
                                  netwib_string *pstringout);

netwib_err netwib_debug_exec_line2(int fd,
                                   netwib_uint32 stringoutmaxsize,
                                   netwib_string stringout);

