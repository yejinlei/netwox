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
netwib_err netwox_expect(netwib_buf *pbuf,
                         netwib_conststring regularexpression1,
                         netwib_conststring regularexpression2,
                         netwib_conststring regularexpression3,
                         netwib_conststring regularexpression4,
                         netwib_bool casesensitive,
                         netwib_io *pio,
                         netwib_consttime *pabstime,
                         netwib_uint32 *pnumfound)
{
  netwib_bool event;
  netwib_buf bufre1, bufre2, bufre3, bufre4;
  netwib_err ret;

  if (pnumfound != NULL) *pnumfound = 0;

  netwib_er(netwib_buf_init_ext_string(regularexpression1, &bufre1));
  netwib_er(netwib_buf_init_ext_string(regularexpression2, &bufre2));
  netwib_er(netwib_buf_init_ext_string(regularexpression3, &bufre3));
  netwib_er(netwib_buf_init_ext_string(regularexpression4, &bufre4));
  while (NETWIB_TRUE) {
    if (regularexpression1 != NULL) {
      ret = netwib_buf_search_re(pbuf, &bufre1, casesensitive, NULL);
      if (ret == NETWIB_ERR_OK) {
        if (pnumfound != NULL) *pnumfound = 1;
        return(NETWIB_ERR_OK);
      } else if (ret != NETWIB_ERR_NOTFOUND) {
        return(ret);
      }
    }
    if (regularexpression2 != NULL) {
      ret = netwib_buf_search_re(pbuf, &bufre2, casesensitive, NULL);
      if (ret == NETWIB_ERR_OK) {
        if (pnumfound != NULL) *pnumfound = 2;
        return(NETWIB_ERR_OK);
      } else if (ret != NETWIB_ERR_NOTFOUND) {
        return(ret);
      }
    }
    if (regularexpression3 != NULL) {
      ret = netwib_buf_search_re(pbuf, &bufre3, casesensitive, NULL);
      if (ret == NETWIB_ERR_OK) {
        if (pnumfound != NULL) *pnumfound = 3;
        return(NETWIB_ERR_OK);
      } else if (ret != NETWIB_ERR_NOTFOUND) {
        return(ret);
      }
    }
    if (regularexpression4 != NULL) {
      ret = netwib_buf_search_re(pbuf, &bufre4, casesensitive, NULL);
      if (ret == NETWIB_ERR_OK) {
        if (pnumfound != NULL) *pnumfound = 4;
        return(NETWIB_ERR_OK);
      } else if (ret != NETWIB_ERR_NOTFOUND) {
        return(ret);
      }
    }
    netwib_er(netwib_io_wait_read(pio, pabstime, &event));
    if (!event) {
      return(NETWIB_ERR_OK);
    }
    ret = netwib_io_read(pio, pbuf);
    if (ret == NETWIB_ERR_DATAEND) {
      return(NETWIB_ERR_OK);
    } else if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      /* continue */
    } else if (ret != NETWIB_ERR_OK) {
      return(ret);
    }
  }

  return(NETWIB_ERR_OK);
}
