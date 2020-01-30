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
netwib_err netwib_beep(void)
{
#if defined NETWIBDEF_SYSNAME_Unix
  printf("%c", 7);
  fflush(stdout);
#elif defined NETWIBDEF_SYSNAME_Windows
  Beep(400, 150);
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif
  return(NETWIB_ERR_OK);
}
