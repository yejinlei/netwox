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
#include "pieces/iousual_null.c"
#include "pieces/iousual_mem.c"
#include "pieces/iousual_tlv.c"
#if defined NETWIBDEF_SYSNAME_Unix
 #include "pieces/iousual_execcommon.c"
 #include "pieces/iousual_exec_unix.c"
#elif defined NETWIBDEF_SYSNAME_Windows
 #include "pieces/iousual_exec_windows.c"
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif
#include "pieces/iousual_shellserver.c"
#include "pieces/iousual_shellclient.c"

/*-------------------------------------------------------------*/
#include "pieces/iousual_data.c"
#include "pieces/iousual_storage.c"

/*-------------------------------------------------------------*/
#include "pieces/iousual_rdwr.c"
#include "pieces/iousual_tee.c"
#include "pieces/iousual_debug.c"
