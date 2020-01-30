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
netwib_err netwox_txtproto_io_rd(netwib_io *pio,
                                 netwib_io **ppio)
{
  netwib_er(netwib_io_init_data(NETWIB_IO_INIT_DATA_TYPE_LINE,
                                NETWIB_IO_INIT_DATA_TYPE_TRANSPARENT,
                                ppio));
  netwib_er(netwib_io_plug_rdwr(*ppio, pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_txtproto_io_rdwr(netwib_io *pio,
                                   netwib_bool msdos,
                                   netwib_io **ppio)
{
  netwib_er(netwib_io_init_data(NETWIB_IO_INIT_DATA_TYPE_LINE,
                                NETWIB_IO_INIT_DATA_TYPE_LINE,
                                ppio));
  netwib_er(netwib_io_ctl_set_data_line_msdos(*ppio, msdos));
  netwib_er(netwib_io_plug_rdwr(*ppio, pio));


  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_txtproto_read_reply(netwib_constbuf *pline,
                                                  netwib_data nnn,
                                                  netwib_uint32 *perrnum,
                                                  netwib_buf *perrmsg)
{
  netwib_data data;
  netwib_uint32 datasize;

  data = netwib__buf_ref_data_ptr(pline);
  datasize = netwib__buf_ref_data_size(pline);

  /* errnum is not yet set, nor saved */
  if (nnn[0] == '\0') {
    /* check format */
    if (datasize == 3) {
      data[3] = ' ';
      datasize = 4;
    }
    if (datasize < 4) {
      return(NETWOX_ERR_PROTOCOL);
    }
    if (!netwib_c2_isdigit(data[0]) || !netwib_c2_isdigit(data[1]) ||
        !netwib_c2_isdigit(data[2])) {
      return(NETWOX_ERR_PROTOCOL);
    }
    if (data[3] != '-' && data[3] != ' ') {
      return(NETWOX_ERR_PROTOCOL);
    }
    /* convert errnum */
    if (perrnum != NULL) {
      *perrnum = netwib_c2_cto9(data[0])*100
        + netwib_c2_cto9(data[1])*10
        + netwib_c2_cto9(data[2]);
    }
    /* set errmsg */
    netwib_er(netwib_buf_append_data(data+4, datasize-4, perrmsg));
    /* check end */
    if (data[3] == ' ') {
      return(NETWIB_ERR_DATAEND);
    }
    /* save for future comparisons */
    netwib_c_memcpy(nnn, data, 3);
    nnn[3] = ' ';
    return(NETWIB_ERR_OK);
  }

  /* errnum is set : first, check for end */
  if (datasize >= 4) {
    if (!netwib_c_memcmp(nnn, data, 4)) {
      netwib_er(netwib_buf_append_data(data+4, datasize-4, perrmsg));
      return(NETWIB_ERR_DATAEND);
    }
  }

  /* now, we have an intermediary data which have to be concatenated */
  /* first, if there is "nnn-", suppress those four characters */
  if (datasize >= 4) {
    if (!netwib_c_memcmp(nnn, data, 3)) {
      if (data[3] == '-') {
        netwib_er(netwib_buf_append_data(data+4, datasize-4, perrmsg));
        return(NETWIB_ERR_OK);
      }
    }
  }

  /* now, eventually suppress leading spaces */
  while(datasize) {
    if (*data != ' ') break;
    data++;
    datasize--;
  }
  netwib_er(netwib_buf_append_byte(' ', perrmsg));
  netwib_er(netwib_buf_append_data(data, datasize, perrmsg));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_txtproto_read_reply(netwib_io *pio,
                                      netwib_uint32 maxwaitms,
                                      netwib_uint32 *perrnum,
                                      netwib_buf *perrmsg)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_time abstime;
  netwib_bool event;
  netwib_byte nnn[4];
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_time_init_now(&abstime));
  netwib_er(netwib_time_plus_msec(&abstime, maxwaitms));

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));
  nnn[0] = '\0'; /* means not set */
  while(NETWIB_TRUE) {
    /* read */
    netwib__buf_reinit(&buf);
    ret = netwib_io_wait_read(pio, &abstime, &event);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    if (!event) {
      if (perrnum != NULL) *perrnum = 0;
      netwib_er(netwib_buf_append_string("Timeout in netwox_txtproto_read",
                                       perrmsg));
      break;
    }
    ret = netwib_io_read(pio, &buf);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* analyze line */
    ret = netwox_priv_txtproto_read_reply(&buf, nnn, perrnum, perrmsg);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
  }
  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_txtproto_read_query(netwib_io *pio,
                                      netwib_uint32 maxwaitms,
                                      netwib_buf *pline)
{
  netwib_time abstime;
  netwib_bool event;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwib_time_init_now(&abstime));
  netwib_er(netwib_time_plus_msec(&abstime, maxwaitms));

  while(NETWIB_TRUE) {
    netwib_er(netwib_io_wait_read(pio, &abstime, &event));
    if (!event) {
      return(NETWOX_ERR_TIMEOUT);
    }
    ret = netwib_io_read(pio, pline);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else {
      break;
    }
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_txtproto_write_reply(netwib_io *pio,
                                       netwib_uint32 errnum,
                                       netwib_constbuf *perrmsg)
{
  netwib_byte array[512];
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &buf));

  netwib_er(netwib_buf_append_fmt(&buf, "%{uint32} %{buf}", errnum, perrmsg));

  ret = netwib_io_write(pio, &buf);

  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_txtproto_write_reply_text(netwib_io *pio,
                                            netwib_uint32 errnum,
                                            netwib_conststring errmsg)
{
  netwib_buf msg;

  netwib_er(netwib_buf_init_ext_string(errmsg, &msg));
  netwib_er(netwox_txtproto_write_reply(pio, errnum, &msg));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_txtproto_write_query(netwib_io *pio,
                                       netwib_constbuf *pline)
{
  netwib_err ret;

  ret = netwib_io_write(pio, pline);

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_txtproto_expect5(netwib_uint32 errnum,
                                   netwib_buf *perrmsg,
                                   netwib_uint32 expectederrnum1,
                                   netwib_uint32 expectederrnum2,
                                   netwib_uint32 expectederrnum3,
                                   netwib_uint32 expectederrnum4,
                                   netwib_uint32 expectederrnum5)
{
  if (errnum == expectederrnum1 || errnum == expectederrnum2 ||
      errnum == expectederrnum3 || errnum == expectederrnum4 ||
      errnum == expectederrnum5) {
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_fmt_display("We expected %{uint32}|%{uint32}|%{uint32}|%{uint32}|%{uint32} but we received %{uint32} (%{buf})\n", expectederrnum1, expectederrnum2, expectederrnum3, expectederrnum4, expectederrnum5, errnum, perrmsg));

  return(NETWOX_ERR_PROTOCOL);
}

/*-------------------------------------------------------------*/
netwib_err netwox_txtproto_expect4(netwib_uint32 errnum,
                                   netwib_buf *perrmsg,
                                   netwib_uint32 expectederrnum1,
                                   netwib_uint32 expectederrnum2,
                                   netwib_uint32 expectederrnum3,
                                   netwib_uint32 expectederrnum4)
{
  if (errnum == expectederrnum1 || errnum == expectederrnum2 ||
      errnum == expectederrnum3 || errnum == expectederrnum4) {
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_fmt_display("We expected %{uint32}|%{uint32}|%{uint32}|%{uint32} but we received %{uint32} (%{buf})\n", expectederrnum1, expectederrnum2, expectederrnum3, expectederrnum4, errnum, perrmsg));

  return(NETWOX_ERR_PROTOCOL);
}

/*-------------------------------------------------------------*/
netwib_err netwox_txtproto_expect3(netwib_uint32 errnum,
                                   netwib_buf *perrmsg,
                                   netwib_uint32 expectederrnum1,
                                   netwib_uint32 expectederrnum2,
                                   netwib_uint32 expectederrnum3)
{
  if (errnum == expectederrnum1 || errnum == expectederrnum2 ||
      errnum == expectederrnum3) {
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_fmt_display("We expected %{uint32}|%{uint32}|%{uint32} but we received %{uint32} (%{buf})\n", expectederrnum1, expectederrnum2, expectederrnum3, errnum, perrmsg));

  return(NETWOX_ERR_PROTOCOL);
}

/*-------------------------------------------------------------*/
netwib_err netwox_txtproto_expect2(netwib_uint32 errnum,
                                   netwib_buf *perrmsg,
                                   netwib_uint32 expectederrnum1,
                                   netwib_uint32 expectederrnum2)
{
  if (errnum == expectederrnum1 || errnum == expectederrnum2) {
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_fmt_display("We expected %{uint32}|%{uint32} but we received %{uint32} (%{buf})\n", expectederrnum1, expectederrnum2, errnum, perrmsg));

  return(NETWOX_ERR_PROTOCOL);
}

/*-------------------------------------------------------------*/
netwib_err netwox_txtproto_expect1(netwib_uint32 errnum,
                                   netwib_buf *perrmsg,
                                   netwib_uint32 expectederrnum1)
{
  if (errnum == expectederrnum1) {
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_fmt_display("We expected %{uint32} but we received %{uint32} (%{buf})\n", expectederrnum1, errnum, perrmsg));

  return(NETWOX_ERR_PROTOCOL);
}
