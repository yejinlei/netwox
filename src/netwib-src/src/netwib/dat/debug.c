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
#if NETWIB_DEBUG==1
 #include "debug/string.c"
 #include "debug/string2.c"
#endif


/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_MEMPROTECT_NEEDED==1
 #include "debug/memprotect.c"
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_MEMCORRUPT_NEEDED==1
 #include "debug/memcorrupt.c"
#endif

/*-------------------------------------------------------------*/
/* we need this function otherwise gcc refuses to compile this C file
   because it does not contain any code */
netwib_err netwib_unused_dat_debug(void);
netwib_err netwib_unused_dat_debug(void)
{
  netwib_unused_dat_debug();
  return(NETWIB_ERR_OK);
}

