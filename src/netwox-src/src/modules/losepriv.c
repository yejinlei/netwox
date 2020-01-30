/*
                                  NETWOX
                             Network toolbox
                Copyright(c) 1999-2012 Laurent CONSTANTIN
                      http://ntwox.sourceforge.net/
                        laurentconstantin@free.fr
                                  -----

  This file is part of Netwox.

  Netwox is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3
  as published by the Free Software Foundation.

  Netwox is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details (http://www.gnu.org/).

------------------------------------------------------------------------
*/

/*-------------------------------------------------------------*/
#include "../netwox.h"

/*-------------------------------------------------------------*/
/* this define is here because I have a script checking if
 * netwib contains MT-unsafe functions. Like this, they are ignored. */
#define netwib_getpwnam getpwnam

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Linux
  #include <unistd.h>
  #include <pwd.h>
  #include <grp.h>
#endif

/*-------------------------------------------------------------*/
netwib_err netwox_losepriv(void)
{
#if defined NETWIBDEF_SYSNAME_Linux
  struct passwd *ppasswd;
  int reti;

  ppasswd = netwib_getpwnam("nobody");
  if (ppasswd == NULL) {
    return(NETWIB_ERR_FUGETPWUID);
  }

  reti = setgid(ppasswd->pw_gid);
  if (reti == -1) return(NETWIB_ERR_FUSETGID);
  reti = setgroups(0, NULL);
  if (reti == -1) return(NETWIB_ERR_FUSETGROUPS);
  reti = setuid(ppasswd->pw_uid);
  if (reti == -1) return(NETWIB_ERR_FUSETUID);

#endif
  return(NETWIB_ERR_OK);
}
