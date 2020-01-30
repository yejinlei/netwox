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
netwib_err netwib_global_ctl_set(netwib_global_ctltype type,
                                 netwib_ptr p,
                                 netwib_uint32 ui)
{

  switch(type) {
    case NETWIB_GLOBAL_CTLTYPE_ERR_PURGE:
      netwib_er(netwib_priv_err_purge());
      break;
    case NETWIB_GLOBAL_CTLTYPE_CONF_UPDATE:
      netwib_er(netwib_priv_conf_update());
      break;
    case NETWIB_GLOBAL_CTLTYPE_DEBUG_CTRLC_PRESSED:
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
      ui = ui;
  }

  p = p; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_global_ctl_get(netwib_global_ctltype type,
                                 netwib_ptr p,
                                 netwib_uint32 *pui)
{

  switch(type) {
    case NETWIB_GLOBAL_CTLTYPE_ERR_PURGE:
    case NETWIB_GLOBAL_CTLTYPE_CONF_UPDATE:
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    case NETWIB_GLOBAL_CTLTYPE_DEBUG_CTRLC_PRESSED:
      *pui = (netwib_uint32)netwib_priv_glovars.debug_ctrlc_pressed;
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
      pui = pui;
  }

  p = p; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
