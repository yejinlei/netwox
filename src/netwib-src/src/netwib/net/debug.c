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

/*-------------------------------------------------------------*/
/* This should be in priv.c, but libnet header conflicts with
   net/if.h net/if_arp.h netinet/if_ether.h (OpenBSD, FreeBSD).
   So, we cannot mix inc/maininc.h and libnet.h.
   So libnet has to be compiled alone in order to be able to
   compile it under BSD.
   For the moment, there is nothing in this module, so I
   decided to put it here, instead of creating
   src/netwib/lib/libnet.c.
*/

/*-------------------------------------------------------------*/
#define NETWIB_DEF_ONLY 1
#include <netwib.h>

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
 #ifdef NETWIBDEF_SYSNAME_Linux
  #define _GNU_SOURCE 1
 #endif
 #include <stdlib.h>
 #if NETWIBDEF_LIBNETINSTALLED == 1
  #ifdef __cplusplus
   extern "C" {
  #endif
  /* libnet-config of libnet 1.0 under Mac OS does not define it */
  #if NETWIBDEF_ARCH_ENDIAN == 0
   #if ! defined LIBNET_LIL_ENDIAN
    #define LIBNET_LIL_ENDIAN 1
   #endif
  #else
   #if ! defined LIBNET_BIG_ENDIAN
    #define LIBNET_BIG_ENDIAN 1
   #endif
  #endif
  #include <libnet.h>
  #ifdef __cplusplus
   }
  #endif
 #endif
 #include <netwib.h>
 #define NETWIBDEF_PRIV_DONT_WANT_SYS 1
 #include <netwib/dat/priv.h>
 #include <netwib/sys/priv.h>
 #include <netwib/net/priv.h>
#elif defined NETWIBDEF_SYSNAME_Windows
 #include <netwib/inc/maininc.h>
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
/* and now the main code */
#include "priv/libnet.c"
