
/*
  This module is adapted from netwib/sys/priv/cmdline.c.
  Cross changes are checked using a private script to ensure they
  are both updated when one changes.
*/

/*-------------------------------------------------------------*/
netwib_err netwox_cmdline_init(netwib_constbuf *pbufcommand,
                               netwib_string *pfilename,
                               int *pargc,
                               netwib_string *pargv[]);

/*-------------------------------------------------------------*/
netwib_err netwox_cmdline_close(netwib_string *pfilename,
                                netwib_string *pargv[]);
