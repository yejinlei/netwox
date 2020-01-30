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
netwib_err netwib_io_init(netwib_bool readsupported,
                          netwib_bool writesupported,
                          netwib_ptr pcommon,
                          netwib_io_read_pf pfread,
                          netwib_io_write_pf pfwrite,
                          netwib_io_wait_pf pfwait,
                          netwib_io_unread_pf pfunread,
                          netwib_io_ctl_set_pf pfctl_set,
                          netwib_io_ctl_get_pf pfctl_get,
                          netwib_io_close_pf pfclose,
                          netwib_io **ppio)
{
  netwib_io *pio;

  /* parameter verification */
  if (ppio == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* allocate needed memory to store pio */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_io), (netwib_ptr*)&pio));
  *ppio = pio;

  /* set parameters */
  pio->rd.pnext = NULL;
  pio->rd.supported = readsupported;
  pio->rd.numusers = 0;
  pio->wr.pnext = NULL;
  pio->wr.supported = writesupported;
  pio->wr.numusers = 0;

  pio->pcommon = pcommon;

  pio->pfread = pfread;
  pio->pfwrite = pfwrite;
  pio->pfwait = pfwait;
  pio->pfunread = pfunread;
  pio->pfctl_set = pfctl_set;
  pio->pfctl_get = pfctl_get;
  pio->pfclose = pfclose;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_add_io(pio));
#endif

  return(NETWIB_ERR_OK);
}

