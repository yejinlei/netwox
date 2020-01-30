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
netwib_err netwib_pathname_exists(netwib_constbuf *ppathname,
                                  netwib_bool *pyes)
{
  netwib_pathstat pathstat;
  netwib_err ret;
  netwib_bool exists;

  exists = NETWIB_FALSE;
  ret = netwib_priv_stat_init_pathname(ppathname, &pathstat);
  if (ret == NETWIB_ERR_OK) {
    exists = NETWIB_TRUE;
  } else if (ret == NETWIB_ERR_NOTFOUND) {
    exists = NETWIB_FALSE;
  } else {
    return(ret);
  }

  if (pyes != NULL) {
    *pyes = exists;
  }

  return(NETWIB_ERR_OK);
}
