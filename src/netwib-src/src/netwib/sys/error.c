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
netwib_err netwib_buf_append_err(netwib_err error,
                                 netwib_err_encodetype encodetype,
                                 netwib_buf *pbuf)
{
  netwib_buf buf;
  netwib_uint32 varerrno, varherrno, vargetlasterror;
  netwib_uint32 savedsize;
  netwib_err ret;

  netwib_er(netwib_priv_err_syserrors(&varerrno, &varherrno,
                                      &vargetlasterror));

  if (pbuf == NULL) {
    /* we work on a fake buf, just to test error */
    netwib_er(netwib_buf_init_mallocdefault(&buf));
    ret = netwib_priv_err_append_err(error, varerrno, varherrno,
                                     vargetlasterror, encodetype, &buf);
    netwib_er(netwib_buf_close(&buf));
    return(ret);
  }

  /* save position in case of error */
  savedsize = netwib__buf_ref_data_size(pbuf);

  ret = netwib_priv_err_append_err(error, varerrno, varherrno,
                                   vargetlasterror, encodetype, pbuf);

  /* on error, restore first pos */
  if (ret != NETWIB_ERR_OK) {
    pbuf->endoffset = pbuf->beginoffset + savedsize;
  }
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_err_display(netwib_err error,
                              netwib_err_encodetype encodetype)
{
  netwib_buf buf;
  netwib_string pc;
  netwib_uint32 varerrno, varherrno, vargetlasterror;
  netwib_char arr[4096];

  netwib_er(netwib_priv_err_syserrors(&varerrno, &varherrno,
                                      &vargetlasterror));

  netwib_er(netwib_buf_init_ext_arraysizeofempty(arr, &buf));
  netwib_er(netwib_priv_err_append_err(error, varerrno, varherrno,
                                       vargetlasterror, encodetype, &buf));
  netwib_er(netwib_buf_ref_string(&buf, &pc));
  printf("%s", pc);
  fflush(stdout);

  return(NETWIB_ERR_OK);
}
