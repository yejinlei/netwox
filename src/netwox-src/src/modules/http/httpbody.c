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
#include "../../netwox.h"

/*-------------------------------------------------------------*/
netwib_err netwox_httpbody_read_io_close(netwib_bufpool *pbufpool,
                                         netwib_uint32 timeoutms,
                                         netwib_io *psockio,
                                         netwib_io *pbodyio)
{
  netwib_buf *pbuf;
  netwib_bool event;
  netwib_time t;
  netwib_err ret=NETWIB_ERR_OK;

  /* reset read io type */
  netwib_er(netwib_io_ctl_set_data_type(psockio, NETWIB_IO_WAYTYPE_READ,
                                        NETWIB_IO_INIT_DATA_TYPE_TRANSPARENT));

  /* prepare wait */
  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_plus_msec(&t, timeoutms));

  /* main loop */
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
  while(NETWIB_TRUE) {
    /* read */
    netwib__buf_reinit(pbuf);
    netwib_er(netwib_io_wait_read(psockio, &t, &event));
    if (!event) {
      ret = NETWOX_ERR_TIMEOUT;
      break;
    }
    ret = netwib_io_read(psockio, pbuf);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATANOTAVAIL) {
        ret = NETWIB_ERR_OK;
        continue;
      }
      if (ret == NETWIB_ERR_DATAEND) {
        ret = NETWIB_ERR_OK;
        break;
      }
      break;
    }
    /* write */
    if (pbodyio != NULL) {
      ret = netwib_io_write(pbodyio, pbuf);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
    }
    /* reset counter */
    netwib_er(netwib_time_init_now(&t));
    netwib_er(netwib_time_plus_msec(&t, timeoutms));
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_httpbody_read_io_length(netwib_bufpool *pbufpool,
                                          netwib_uint32 timeoutms,
                                          netwib_uint32 contentlength,
                                          netwib_io *psockio,
                                          netwib_io *pbodyio)
{
  netwib_buf *pbuf;
  netwib_bool event;
  netwib_time t;
  netwib_err ret=NETWIB_ERR_OK;

  /* easy case */
  if (contentlength == 0) {
    return(NETWIB_ERR_OK);
  }

  /* reset read io type */
  netwib_er(netwib_io_ctl_set_data_type(psockio, NETWIB_IO_WAYTYPE_READ,
                                        NETWIB_IO_INIT_DATA_TYPE_CHUNK));
  netwib_er(netwib_io_ctl_set_data_chunk_minsize(psockio,
                                                 NETWIB_IO_WAYTYPE_READ, 1));
  netwib_er(netwib_io_ctl_set_data_chunk_maxsize(psockio,
                                                 NETWIB_IO_WAYTYPE_READ,
                                                 contentlength));

  /* prepare wait */
  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_plus_msec(&t, timeoutms));

  /* main loop */
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
  while(NETWIB_TRUE) {
    /* read */
    netwib__buf_reinit(pbuf);
    netwib_er(netwib_io_wait_read(psockio, &t, &event));
    if (!event) {
      ret = NETWOX_ERR_TIMEOUT;
      break;
    }
    ret = netwib_io_read(psockio, pbuf);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATANOTAVAIL) {
        ret = NETWIB_ERR_OK;
        continue;
      }
      break;
    }
    /* write */
    if (pbodyio != NULL) {
      ret = netwib_io_write(pbodyio, pbuf);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
    }
    /* update length to read */
    contentlength -= netwib__buf_ref_data_size(pbuf);
    if (contentlength == 0) {
      break;
    }
    netwib_er(netwib_io_ctl_set_data_chunk_maxsize(psockio,
                                                   NETWIB_IO_WAYTYPE_READ,
                                                   contentlength));
    /* reset counter */
    netwib_er(netwib_time_init_now(&t));
    netwib_er(netwib_time_plus_msec(&t, timeoutms));
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_httpbody_read_io(netwib_bufpool *pbufpool,
                                   netwib_uint32 timeoutms,
                                   netwib_bool contentlengthset,
                                   netwib_uint32 contentlength,
                                   netwib_io *psockio,
                                   netwib_io *pbodyio)
{

  if (contentlengthset) {
    netwib_er(netwox_httpbody_read_io_length(pbufpool, timeoutms,
                                             contentlength, psockio, pbodyio));
  } else {
    netwib_er(netwox_httpbody_read_io_close(pbufpool, timeoutms, psockio,
                                            pbodyio));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_httpbody_write_io_close(netwib_bufpool *pbufpool,
                                          netwib_io *pbodyio,
                                          netwib_io *psockio)
{
  netwib_buf *pbuf;
  netwib_err ret=NETWIB_ERR_OK;

  /* main loop */
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
  while(NETWIB_TRUE) {
    /* read */
    netwib__buf_reinit(pbuf);
    ret = netwib_io_read(pbodyio, pbuf);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATANOTAVAIL) {
        ret = NETWIB_ERR_OK;
        continue;
      }
      if (ret == NETWIB_ERR_DATAEND) {
        ret = NETWIB_ERR_OK;
        break;
      }
      break;
    }
    /* write */
    ret = netwib_io_write(psockio, pbuf);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_httpbody_read_buf_close(netwib_uint32 timeoutms,
                                          netwib_io *psockio,
                                          netwib_buf *pbodybuf)
{
  netwib_bool event;
  netwib_time t;
  netwib_err ret=NETWIB_ERR_OK;

  /* reset read io type */
  netwib_er(netwib_io_ctl_set_data_type(psockio, NETWIB_IO_WAYTYPE_READ,
                                        NETWIB_IO_INIT_DATA_TYPE_TRANSPARENT));

  /* prepare wait */
  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_plus_msec(&t, timeoutms));

  /* main loop */
  while(NETWIB_TRUE) {
    /* read */
    netwib_er(netwib_io_wait_read(psockio, &t, &event));
    if (!event) {
      ret = NETWOX_ERR_TIMEOUT;
      break;
    }
    ret = netwib_io_read(psockio, pbodybuf);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATANOTAVAIL) {
        ret = NETWIB_ERR_OK;
        continue;
      }
      if (ret == NETWIB_ERR_DATAEND) {
        ret = NETWIB_ERR_OK;
        break;
      }
      break;
    }
    /* reset counter */
    netwib_er(netwib_time_init_now(&t));
    netwib_er(netwib_time_plus_msec(&t, timeoutms));
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_httpbody_read_buf_length(netwib_uint32 timeoutms,
                                           netwib_uint32 contentlength,
                                           netwib_io *psockio,
                                           netwib_buf *pbodybuf)
{
  netwib_bool event;
  netwib_uint32 initiallength, length;
  netwib_time t;
  netwib_err ret=NETWIB_ERR_OK;

  /* easy case */
  if (contentlength == 0) {
    return(NETWIB_ERR_OK);
  }

  /* reset read io type */
  netwib_er(netwib_io_ctl_set_data_type(psockio, NETWIB_IO_WAYTYPE_READ,
                                        NETWIB_IO_INIT_DATA_TYPE_CHUNK));
  netwib_er(netwib_io_ctl_set_data_chunk_minsize(psockio,
                                                 NETWIB_IO_WAYTYPE_READ, 1));
  netwib_er(netwib_io_ctl_set_data_chunk_maxsize(psockio,
                                                 NETWIB_IO_WAYTYPE_READ,
                                                 contentlength));

  initiallength = netwib__buf_ref_data_size(pbodybuf);

  /* prepare wait */
  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_plus_msec(&t, timeoutms));

  /* main loop */
  while(NETWIB_TRUE) {
    /* read */
    netwib_er(netwib_io_wait_read(psockio, &t, &event));
    if (!event) {
      ret = NETWOX_ERR_TIMEOUT;
      break;
    }
    ret = netwib_io_read(psockio, pbodybuf);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATANOTAVAIL) {
        ret = NETWIB_ERR_OK;
        continue;
      }
      break;
    }
    /* update length to read */
    length = contentlength -
      (netwib__buf_ref_data_size(pbodybuf) - initiallength);
    if (length == 0) {
      break;
    }
    netwib_er(netwib_io_ctl_set_data_chunk_maxsize(psockio,
                                                   NETWIB_IO_WAYTYPE_READ,
                                                   length));
    /* reset counter */
    netwib_er(netwib_time_init_now(&t));
    netwib_er(netwib_time_plus_msec(&t, timeoutms));
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_httpbody_read_buf(netwib_uint32 timeoutms,
                                    netwib_bool contentlengthset,
                                    netwib_uint32 contentlength,
                                    netwib_io *psockio,
                                    netwib_buf *pbodybuf)
{

  if (contentlengthset) {
    netwib_er(netwox_httpbody_read_buf_length(timeoutms, contentlength,
                                              psockio, pbodybuf));
  } else {
    netwib_er(netwox_httpbody_read_buf_close(timeoutms, psockio, pbodybuf));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_httpbody_write_buf_close(netwib_constbuf *pbodybuf,
                                           netwib_io *psockio)
{

  netwib_er(netwib_io_write(psockio, pbodybuf));

  return(NETWIB_ERR_OK);
}
