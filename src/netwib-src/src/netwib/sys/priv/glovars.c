/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_priv_glovars_t netwib_priv_glovars;


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_priv_mutex netwib_priv_glovars_other_mut;

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_glovars_other_init(void)
{
  netwib_er(netwib_priv_mutex_init(&netwib_priv_glovars_other_mut));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_glovars_other_close(void)
{
  netwib_er(netwib_priv_mutex_close(&netwib_priv_glovars_other_mut));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_glovars_other_rdlock(void)
{
  return(netwib_priv_mutex_rdlock(&netwib_priv_glovars_other_mut));
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_glovars_other_rdunlock(void)
{
  return(netwib_priv_mutex_rdunlock(&netwib_priv_glovars_other_mut));
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_glovars_other_wrlock(void)
{
  return(netwib_priv_mutex_wrlock(&netwib_priv_glovars_other_mut));
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_glovars_other_wrunlock(void)
{
  return(netwib_priv_mutex_wrunlock(&netwib_priv_glovars_other_mut));
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_priv_mutex netwib_priv_glovars_mut;

/*-------------------------------------------------------------*/
/* this define is here because I have a script checking if
   netwib contains MT-unsafe functions. Like this, they are ignored. */
#define netwib_c_localtime localtime

/*-------------------------------------------------------------*/
netwib_bool netwib_priv_glovars_initialized = NETWIB_FALSE;
netwib_err netwib_priv_glovars_init(void)
{
  struct tm *ptm;
  time_t tt;

  /* set global parameters */
  netwib_er(netwib_buf_init_mallocdefault(&netwib_priv_glovars.errmsg));
  netwib_er(netwib_priv_rand_seed(&netwib_priv_glovars.rand_seed));

  /* debug */
  netwib_priv_glovars.debug_ctrlc_pressed = NETWIB_FALSE;

  /* timezone */
  tt = time(NULL);
  ptm = netwib_c_localtime(&tt);
  netwib_priv_glovars.time_isdst = (ptm->tm_isdst>0)?NETWIB_TRUE:NETWIB_FALSE;
#if defined NETWIBDEF_SYSNAME_Unix
 #if NETWIBDEF_HAVEFIELD_TM_GMTOFF == 1
  netwib_priv_glovars.time_zoneoffset = ptm->tm_gmtoff;
 #else
  netwib_priv_glovars.time_zoneoffset = -timezone;
  if (netwib_priv_glovars.time_isdst) {
    netwib_priv_glovars.time_zoneoffset += 3600;
  }
 #endif
#elif defined NETWIBDEF_SYSNAME_Windows
  netwib_priv_glovars.time_zoneoffset = -_timezone;
  if (netwib_priv_glovars.time_isdst) {
    netwib_priv_glovars.time_zoneoffset += 3600;
  }
#endif

  /* other */
  netwib_er(netwib_priv_glovars_other_init());

  netwib_er(netwib_priv_mutex_init(&netwib_priv_glovars_mut));

  netwib_er(netwib_priv_conf_init());

  netwib_priv_glovars_initialized = NETWIB_TRUE;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_glovars_close(void)
{
  netwib_er(netwib_priv_conf_close());

  netwib_er(netwib_priv_mutex_close(&netwib_priv_glovars_mut));

  netwib_er(netwib_priv_glovars_other_close());

  netwib_er(netwib_buf_close(&netwib_priv_glovars.errmsg));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_glovars_rdlock(void)
{
  return(netwib_priv_mutex_rdlock(&netwib_priv_glovars_mut));
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_glovars_rdunlock(void)
{
  return(netwib_priv_mutex_rdunlock(&netwib_priv_glovars_mut));
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_glovars_wrlock(void)
{
  return(netwib_priv_mutex_wrlock(&netwib_priv_glovars_mut));
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_glovars_wrunlock(void)
{
  return(netwib_priv_mutex_wrunlock(&netwib_priv_glovars_mut));
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_glovars_canuse(netwib_bool *pyes)
{
  netwib_bool yes;

  yes = NETWIB_FALSE;
  if (netwib_priv_glovars_initialized) {
    yes = NETWIB_TRUE;
  }

  if (pyes != NULL) *pyes = yes;
  return(NETWIB_ERR_OK);
}
