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
netwib_err netwox_fraction_init(netwox_fraction *pfraction)
{
  pfraction->displayedchars = 0;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_fraction_close(netwox_fraction *pfraction)
{
  netwib_uint32 i;

  for (i = 0 ; i < pfraction->displayedchars; i++) {
    netwib_er(netwib_fmt_display("\b \b"));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_fraction_update(netwox_fraction *pfraction,
                                  netwib_uint32 value,
                                  netwib_uint32 maxvalue)
{
  netwib_byte array[80];
  netwib_buf buf;
  netwib_uint32 i;

  netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));

  for (i = 0 ; i < pfraction->displayedchars; i++) {
    netwib_er(netwib_fmt_display("\b \b"));
  }

  netwib_er(netwib_buf_append_fmt(&buf, "%{uint32}/%{uint32}", value, maxvalue));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  pfraction->displayedchars = netwib__buf_ref_data_size(&buf);

  return(NETWIB_ERR_OK);
}
