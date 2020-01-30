/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_program_exit(void)
{
#if defined NETWIBDEF_SYSNAME_Unix
  _exit(NETWIB_ERR_DATAEND);
#elif defined NETWIBDEF_SYSNAME_Windows
  exit(NETWIB_ERR_DATAEND);
#else
#error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  netwib_er(netwib_priv_errmsg_func_string("netwib_priv_program_exit",
                                           "should have exited"));
  return(NETWIB_ERR_LOINTERNALERROR);
}
