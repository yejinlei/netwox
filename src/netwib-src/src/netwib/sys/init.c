/*
                                  NETWIB
                             Network library
                Copyright(c) 1999-2012 Laurent CONSTANTIN
                      http://ntwib.sourceforge.net/
                        laurentconstantin@free.fr
                                  -----
  This file is part of Netwib.

  Netwib is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3
  as published by the Free Software Foundation.

  Netwib is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details (http://www.gnu.org/).

------------------------------------------------------------------------
*/

#include <netwib/inc/maininc.h>


/*-------------------------------------------------------------*/
netwib_bool netwib_priv_netwibwasinitialized = NETWIB_FALSE;
static netwib_err netwib_init2(void)
{
  /* ignore double initialization */
  if (netwib_priv_netwibwasinitialized) {
    return(NETWIB_ERR_OK);
  }
  netwib_priv_netwibwasinitialized = NETWIB_TRUE;

#if NETWIB_DEBUG == 1
  netwib_er(netwib_debug_glovars_init());
  netwib_er(netwib_debug_fdopen_init());
  netwib_er(netwib_debug_leak_init());
  netwib_er(netwib_debug_memcorrupt_init());
  netwib_er(netwib_debug_sighdl_init());
#endif

  netwib_er(netwib_priv_glovars_init());

#if defined NETWIBDEF_SYSNAME_Unix
#elif defined NETWIBDEF_SYSNAME_Windows
  netwib_er(netwib_priv_cs_init());
  /* init winsock 1.1 */
  netwib_er(netwib_priv_winsock_init());
  /* load dlls */
  netwib_er(netwib_priv_dll_init());
#else
#error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_init(void)
{
  netwib_err ret;

  ret = netwib_init2();
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_priv_notify_err(NETWIB_PRIV_NOTIFYTYPE_EMERG, ret));
  }

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_close2(void)
{
  /* ignore double close */
  if (!netwib_priv_netwibwasinitialized) {
    return(NETWIB_ERR_OK);
  }
  netwib_priv_netwibwasinitialized = NETWIB_FALSE;

#if defined NETWIBDEF_SYSNAME_Unix
#elif defined NETWIBDEF_SYSNAME_Windows
  netwib_er(netwib_priv_dll_close());
  netwib_er(netwib_priv_winsock_close());
  netwib_er(netwib_priv_cs_close());
#else
#error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  netwib_er(netwib_priv_glovars_close());

#if NETWIB_DEBUG == 1
  netwib_er(netwib_debug_sighdl_close());
  netwib_er(netwib_debug_leak_report());
  netwib_er(netwib_debug_leak_close());
  netwib_er(netwib_debug_memcorrupt_check_all());
  netwib_er(netwib_debug_memcorrupt_close());
  netwib_er(netwib_debug_fdopen_check());
  netwib_er(netwib_debug_glovars_close());
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_close(void)
{
  netwib_err ret;

  ret = netwib_close2();
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_priv_notify_err(NETWIB_PRIV_NOTIFYTYPE_EMERG, ret));
  }

  return(ret);
}
