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
netwib_err netwib_buf_display(netwib_constbuf *pbuf,
                              netwib_encodetype encodetype)
{
  netwib_string pc;
  netwib_buf buf;
  netwib_err ret;

  if (encodetype == NETWIB_ENCODETYPE_DATA) {
    /* try the quick solution */
    ret = netwib_constbuf_ref_string(pbuf, &pc);
    if (ret == NETWIB_ERR_OK) {
      fprintf(stdout, "%s", pc);
      fflush(stdout);
      return(NETWIB_ERR_OK);
    }
    /* it did not work, so continue below */
  }

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  ret = netwib_buf_encode(pbuf, encodetype, &buf);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_ref_string(&buf, &pc));
    fprintf(stdout, "%s", pc);
    fflush(stdout);
  }
  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_fmt_display(netwib_conststring fmt,
                              ...)
{
  va_list ap;
  netwib_err ret;
  netwib_string pc;
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  va_start(ap, fmt);
  ret = netwib_priv_buf_append_vfmt(&buf, fmt, &ap);
  va_end(ap);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_ref_string(&buf, &pc));
    fprintf(stdout, "%s", pc);
    fflush(stdout);
  }
  netwib_er(netwib_buf_close(&buf));

  return(ret);
}


