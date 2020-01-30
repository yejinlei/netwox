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
netwib_err netwox_wordlist_add_buf(netwib_ring *pring,
                                   netwib_constbuf *pbuf)
{
  netwib_ptr pitem;

  netwib_er(netwox_wordlist_item_create(pbuf, &pitem));
  netwib_er(netwib_ring_add_last(pring, pitem));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_wordlist_add_file(netwib_ring *pring,
                                    netwib_constbuf *pfilename)
{
  netwib_io *piofile, *pioline;
  netwib_buf buf;
  netwib_ptr pitem;
  netwib_err ret;

  netwib_er(netwib_io_init_file_textread(pfilename, &piofile));
  netwib_er(netwib_io_init_data_line(&pioline));
  netwib_er(netwib_io_plug_read(pioline, piofile));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  ret = NETWIB_ERR_OK;
  while(NETWIB_TRUE) {
    netwib__buf_reinit(&buf);
    ret = netwib_io_read(pioline, &buf);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_er(netwox_wordlist_item_create(&buf, &pitem));
    netwib_er(netwib_ring_add_last(pring, pitem));
  }
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_io_close(&pioline));

  return(ret);
}


