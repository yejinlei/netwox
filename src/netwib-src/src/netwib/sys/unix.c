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
netwib_err netwib_unix_symlink(netwib_constbuf *ppathname,
                               netwib_constbuf *plinkname)
#if defined NETWIBDEF_SYSNAME_Unix
{
  netwib_string pathname, linkname;
  int reti;

  netwib__constbuf_ref_string(ppathname, pathname, bufstorage,
                              netwib_unix_symlink(&bufstorage, plinkname));
  netwib__constbuf_ref_string(plinkname, linkname, bufstorage,
                              netwib_unix_symlink(ppathname, &bufstorage));

  reti = symlink(pathname, linkname);
  if (reti == -1) {
    return(NETWIB_ERR_FUSYMLINK);
  }

  return(NETWIB_ERR_OK);
}
#elif defined NETWIBDEF_SYSNAME_Windows
{
  ppathname = ppathname; /* for compiler warning */
  plinkname = plinkname; /* for compiler warning */
  return(NETWIB_ERR_LONOTSUPPORTED);
}
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_unix_readlink(netwib_constbuf *plinkname,
                                netwib_buf *ppathname)
#if defined NETWIBDEF_SYSNAME_Unix
{
  netwib_byte array[1024];
  netwib_buf buf;
  netwib_string linkname;
  netwib_uint32 savedsize;
  netwib_err ret;
  int reti;

  netwib__constbuf_ref_string(plinkname, linkname, bufstorage,
                              netwib_unix_readlink(&bufstorage, ppathname));

  reti = readlink(linkname, (netwib_string)array, sizeof(array));
  if (reti == -1) {
    return(NETWIB_ERR_FUREADLINK);
  }

  /* save position in case of error */
  savedsize = netwib__buf_ref_data_size(ppathname);

  netwib_er(netwib_buf_init_ext_arrayfilled(array, reti, &buf));
  ret = netwib_path_canon(&buf, ppathname);
  if (ret != NETWIB_ERR_OK) {
    ppathname->endoffset = ppathname->beginoffset + savedsize;
    netwib_er(netwib_buf_append_data(array, reti, ppathname));
  }

  return(NETWIB_ERR_OK);
}
#elif defined NETWIBDEF_SYSNAME_Windows
{
  plinkname = plinkname; /* for compiler warning */
  ppathname = ppathname; /* for compiler warning */
  return(NETWIB_ERR_LONOTSUPPORTED);
}
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif
