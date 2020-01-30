/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_debug_glovars_t netwib_debug_glovars;
netwib_priv_mutex netwib_debug_glovars_mut;

/*-------------------------------------------------------------*/
netwib_err netwib_debug_glovars_init(void)
{
  return(netwib_priv_mutex_init(&netwib_debug_glovars_mut));
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_glovars_close(void)
{
  return(netwib_priv_mutex_close(&netwib_debug_glovars_mut));
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_glovars_rdlock(void)
{
  return(netwib_priv_mutex_rdlock(&netwib_debug_glovars_mut));
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_glovars_rdunlock(void)
{
  return(netwib_priv_mutex_rdunlock(&netwib_debug_glovars_mut));
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_glovars_wrlock(void)
{
  return(netwib_priv_mutex_wrlock(&netwib_debug_glovars_mut));
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_glovars_wrunlock(void)
{
  return(netwib_priv_mutex_wrunlock(&netwib_debug_glovars_mut));
}
