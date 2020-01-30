/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
typedef struct {
  netwib_buf *pbufread;
  netwib_buf *pbufwrite;
  netwib_thread_rwlock *plockbufread;
  netwib_thread_rwlock *plockbufwrite;
  netwib_bool closebufsatend;
} netwib_priv_io_mem;

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_mem_init(netwib_thread_rwlock *plockbufread,
                                          netwib_buf *pbufread,
                                          netwib_thread_rwlock *plockbufwrite,
                                          netwib_buf *pbufwrite,
                                          netwib_bool closebufsatend,
                                          netwib_bool *preadinitialized,
                                          netwib_bool *pwriteinitialized,
                                          netwib_priv_io_mem *ptr)
{
  ptr->pbufread = pbufread;
  ptr->pbufwrite = pbufwrite;
  ptr->plockbufread = plockbufread;
  ptr->plockbufwrite = plockbufwrite;
  ptr->closebufsatend = closebufsatend;
  *preadinitialized = (pbufread != NULL)?NETWIB_TRUE:NETWIB_FALSE;
  *pwriteinitialized = (pbufwrite != NULL)?NETWIB_TRUE:NETWIB_FALSE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_mem_close(netwib_priv_io_mem *ptr)
{

  /* close buffers */
  if (ptr->closebufsatend) {
    if (ptr->pbufread != NULL) {
      netwib_er(netwib_buf_close(ptr->pbufread));
    }
    if (ptr->pbufwrite != NULL) {
      netwib_er(netwib_buf_close(ptr->pbufwrite));
    }
    if (ptr->plockbufread != NULL) {
      netwib_er(netwib_thread_rwlock_close(&ptr->plockbufread));
    }
    if (ptr->plockbufwrite != NULL) {
      netwib_er(netwib_thread_rwlock_close(&ptr->plockbufwrite));
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_mem_read(netwib_io *pio,
                                          netwib_buf *pbuf)
{
  netwib_priv_io_mem *ptr = (netwib_priv_io_mem *)pio->pcommon;
  netwib_uint32 datasize;

  datasize = netwib__buf_ref_data_size(ptr->pbufread);
  if (datasize == 0) {
    return(NETWIB_ERR_DATANOTAVAIL);
  }

  netwib_er(netwib_buf_append_buf(ptr->pbufread, pbuf));
  netwib__buf_reinit(ptr->pbufread);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_mem_read_lock(netwib_io *pio,
                                               netwib_buf *pbuf)
{
  netwib_priv_io_mem *ptr = (netwib_priv_io_mem *)pio->pcommon;
  netwib_err ret;

  if (ptr->plockbufread != NULL) {
    netwib_er(netwib_thread_rwlock_rdlock(ptr->plockbufread,
                                          NETWIB_TIME_INFINITE, NULL));
  }
  ret = netwib_priv_io_mem_read(pio, pbuf);
  if (ptr->plockbufread != NULL) {
    netwib_er(netwib_thread_rwlock_rdunlock(ptr->plockbufread));
  }

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_mem_write(netwib_io *pio,
                                           netwib_constbuf *pbuf)
{
  netwib_priv_io_mem *ptr = (netwib_priv_io_mem *)pio->pcommon;

  netwib_er(netwib_buf_append_buf(pbuf, ptr->pbufwrite));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_mem_write_lock(netwib_io *pio,
                                                netwib_constbuf *pbuf)
{
  netwib_priv_io_mem *ptr = (netwib_priv_io_mem *)pio->pcommon;
  netwib_err ret;

  if (ptr->plockbufread != NULL) {
    netwib_er(netwib_thread_rwlock_wrlock(ptr->plockbufwrite,
                                          NETWIB_TIME_INFINITE, NULL));
  }
  ret = netwib_priv_io_mem_write(pio, pbuf);
  if (ptr->plockbufread != NULL) {
    netwib_er(netwib_thread_rwlock_wrunlock(ptr->plockbufwrite));
  }

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_mem_wait(netwib_io *pio,
                                          netwib_io_waytype way,
                                          netwib_consttime *pabstime,
                                          netwib_bool *pevent)
{
  netwib_priv_io_mem *ptr = (netwib_priv_io_mem *)pio->pcommon;
  netwib_uint32 datasize;
  netwib_err ret;

  switch (way) {
    case NETWIB_IO_WAYTYPE_READ :
      datasize = netwib__buf_ref_data_size(ptr->pbufread);
      if (datasize) {
        if (pevent != NULL) *pevent = NETWIB_TRUE;
      } else {
        if (pevent != NULL) *pevent = NETWIB_FALSE;
      }
      break;
    case NETWIB_IO_WAYTYPE_WRITE :
      ret = netwib_buf_wantspace(ptr->pbufwrite, 1, NULL);
      if (ret == NETWIB_ERR_DATANOSPACE) {
        if (pevent != NULL) *pevent = NETWIB_FALSE;
      } else if (ret == NETWIB_ERR_OK) {
        if (pevent != NULL) *pevent = NETWIB_TRUE;
      } else {
        return(ret);
      }
      break;
    case NETWIB_IO_WAYTYPE_RDWR :
      datasize = netwib__buf_ref_data_size(ptr->pbufread);
      if (datasize) {
        if (pevent != NULL) *pevent = NETWIB_TRUE;
      } else {
        ret = netwib_buf_wantspace(ptr->pbufwrite, 1, NULL);
        if (ret == NETWIB_ERR_DATANOSPACE) {
          if (pevent != NULL) *pevent = NETWIB_FALSE;
        } else if (ret == NETWIB_ERR_OK) {
          if (pevent != NULL) *pevent = NETWIB_TRUE;
        } else {
          return(ret);
        }
      }
      break;
    case NETWIB_IO_WAYTYPE_SUPPORTED :
      return(NETWIB_ERR_PLEASECONSTRUCT);
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  pabstime = pabstime; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_mem_wait_lock(netwib_io *pio,
                                               netwib_io_waytype way,
                                               netwib_consttime *pabstime,
                                               netwib_bool *pevent)
{
  netwib_priv_io_mem *ptr = (netwib_priv_io_mem *)pio->pcommon;
  netwib_bool event;
  netwib_err ret;

  switch (way) {
    case NETWIB_IO_WAYTYPE_READ :
      if (ptr->plockbufread != NULL) {
        netwib_er(netwib_thread_rwlock_rdlock(ptr->plockbufread,
                                            NETWIB_TIME_INFINITE, NULL));
      }
      ret = netwib_priv_io_mem_wait(pio, way, NETWIB_TIME_ZERO, &event);
      if (ptr->plockbufread != NULL) {
        netwib_er(netwib_thread_rwlock_rdunlock(ptr->plockbufread));
      }
      if (ret != NETWIB_ERR_OK) return(ret);
      if (!event) return(NETWIB_ERR_PLEASELOOPTIME);
      if (pevent != NULL) *pevent = event;
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IO_WAYTYPE_WRITE :
      if (ptr->plockbufwrite != NULL) {
        netwib_er(netwib_thread_rwlock_wrlock(ptr->plockbufwrite,
                                            NETWIB_TIME_INFINITE, NULL));
      }
      ret = netwib_priv_io_mem_wait(pio, way, NETWIB_TIME_ZERO, &event);
      if (ptr->plockbufwrite != NULL) {
        netwib_er(netwib_thread_rwlock_wrunlock(ptr->plockbufwrite));
      }
      if (ret != NETWIB_ERR_OK) return(ret);
      if (!event) return(NETWIB_ERR_PLEASELOOPTIME);
      if (pevent != NULL) *pevent = event;
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IO_WAYTYPE_RDWR :
      if (ptr->plockbufread != NULL) {
        netwib_er(netwib_thread_rwlock_rdlock(ptr->plockbufread,
                                            NETWIB_TIME_INFINITE, NULL));
      }
      if (ptr->plockbufwrite != NULL) {
        netwib_er(netwib_thread_rwlock_wrlock(ptr->plockbufwrite,
                                            NETWIB_TIME_INFINITE, NULL));
      }
      ret = netwib_priv_io_mem_wait(pio, way, NETWIB_TIME_ZERO, &event);
      if (ptr->plockbufread != NULL) {
        netwib_er(netwib_thread_rwlock_rdunlock(ptr->plockbufread));
      }
      if (ptr->plockbufwrite != NULL) {
        netwib_er(netwib_thread_rwlock_wrunlock(ptr->plockbufwrite));
      }
      if (ret != NETWIB_ERR_OK) return(ret);
      if (!event) return(NETWIB_ERR_PLEASELOOPTIME);
      if (pevent != NULL) *pevent = event;
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IO_WAYTYPE_SUPPORTED :
      return(NETWIB_ERR_PLEASECONSTRUCT);
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  pabstime = pabstime; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_mem_unread(netwib_io *pio,
                                            netwib_constbuf *pbuf)
{
  netwib_priv_io_mem *ptr = (netwib_priv_io_mem *)pio->pcommon;

  netwib_er(netwib_buf_prepend_buf(pbuf, ptr->pbufread));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_mem_unread_lock(netwib_io *pio,
                                                 netwib_constbuf *pbuf)
{
  netwib_priv_io_mem *ptr = (netwib_priv_io_mem *)pio->pcommon;
  netwib_err ret;

  if (ptr->plockbufread != NULL) {
    netwib_er(netwib_thread_rwlock_rdlock(ptr->plockbufread,
                                        NETWIB_TIME_INFINITE, NULL));
  }
  ret = netwib_priv_io_mem_unread(pio, pbuf);
  if (ptr->plockbufread != NULL) {
    netwib_er(netwib_thread_rwlock_rdunlock(ptr->plockbufread));
  }

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_mem_ctl_set(netwib_io *pio,
                                             netwib_io_waytype way,
                                             netwib_io_ctltype type,
                                             netwib_ptr p,
                                             netwib_uint32 ui)
{
  switch(type) {
    case NETWIB_IO_CTLTYPE_RES:
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
    case NETWIB_IO_CTLTYPE_END:
      if (way != NETWIB_IO_WAYTYPE_WRITE) return(NETWIB_ERR_PAINVALIDTYPE);
      netwib_er(netwib_io_ctl_set_support(pio, way, NETWIB_FALSE));
      return(NETWIB_ERR_OK);
      break;
    default:
      return(NETWIB_ERR_PLEASETRYNEXT);
  }

  p = p; /* for compiler warning */
  ui = ui; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_mem_ctl_set_lock(netwib_io *pio,
                                                  netwib_io_waytype way,
                                                  netwib_io_ctltype type,
                                                  netwib_ptr p,
                                                  netwib_uint32 ui)
{
  netwib_priv_io_mem *ptr = (netwib_priv_io_mem *)pio->pcommon;
  netwib_err ret;

  if (ptr->plockbufread != NULL && ptr->plockbufread != NULL) {
    netwib_er(netwib_thread_rwlock_wrlock(ptr->plockbufwrite,
                                          NETWIB_TIME_INFINITE, NULL));
    netwib_er(netwib_thread_rwlock_rdlock(ptr->plockbufread,
                                          NETWIB_TIME_INFINITE,
                                          NULL));
  } else if (ptr->plockbufread != NULL) {
    netwib_er(netwib_thread_rwlock_rdlock(ptr->plockbufread,
                                          NETWIB_TIME_INFINITE, NULL));
  } else if (ptr->plockbufwrite != NULL) {
    netwib_er(netwib_thread_rwlock_wrlock(ptr->plockbufwrite,
                                          NETWIB_TIME_INFINITE, NULL));
  }

  ret = netwib_priv_io_mem_ctl_set(pio, way, type, p, ui);

  if (ptr->plockbufread != NULL) {
    netwib_er(netwib_thread_rwlock_rdunlock(ptr->plockbufread));
  }
  if (ptr->plockbufwrite != NULL) {
    netwib_er(netwib_thread_rwlock_wrunlock(ptr->plockbufwrite));
  }

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_mem_ctl_get(netwib_io *pio,
                                             netwib_io_waytype way,
                                             netwib_io_ctltype type,
                                             netwib_ptr p,
                                             netwib_uint32 *pui)
{
  switch(type) {
    case NETWIB_IO_CTLTYPE_RES:
      if (pui != NULL) *pui = NETWIB_IO_RESTYPE_MEM;
      return(NETWIB_ERR_OK);
    case NETWIB_IO_CTLTYPE_END:
      return(NETWIB_ERR_PAINVALIDTYPE);
    default:
      return(NETWIB_ERR_PLEASETRYNEXT);
  }

  pio = pio; /* for compiler warning */
  way = way; /* for compiler warning */
  p = p; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_mem_ctl_get_lock(netwib_io *pio,
                                                  netwib_io_waytype way,
                                                  netwib_io_ctltype type,
                                                  netwib_ptr p,
                                                  netwib_uint32 *pui)
{
  netwib_priv_io_mem *ptr = (netwib_priv_io_mem *)pio->pcommon;
  netwib_err ret;

  if (ptr->plockbufread != NULL && ptr->plockbufread != NULL) {
    netwib_er(netwib_thread_rwlock_wrlock(ptr->plockbufwrite,
                                          NETWIB_TIME_INFINITE, NULL));
    netwib_er(netwib_thread_rwlock_rdlock(ptr->plockbufread,
                                          NETWIB_TIME_INFINITE, NULL));
  } else if (ptr->plockbufread != NULL) {
    netwib_er(netwib_thread_rwlock_rdlock(ptr->plockbufread,
                                          NETWIB_TIME_INFINITE, NULL));
  } else if (ptr->plockbufwrite != NULL) {
    netwib_er(netwib_thread_rwlock_wrlock(ptr->plockbufwrite,
                                          NETWIB_TIME_INFINITE, NULL));
  }

  ret = netwib_priv_io_mem_ctl_get(pio, way, type, p, pui);

  if (ptr->plockbufread != NULL) {
    netwib_er(netwib_thread_rwlock_rdunlock(ptr->plockbufread));
  }
  if (ptr->plockbufwrite != NULL) {
    netwib_er(netwib_thread_rwlock_wrunlock(ptr->plockbufwrite));
  }

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_mem_fclose(netwib_io *pio)
{
  netwib_priv_io_mem *ptr = (netwib_priv_io_mem *)pio->pcommon;

  netwib_er(netwib_priv_io_mem_close(ptr));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_mem(netwib_buf *pbufread,
                              netwib_buf *pbufwrite,
                              netwib_bool closebufsatend,
                              netwib_io **ppio)
{
  netwib_bool rdsup, wrsup;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_mem), &pcommon));
  ret = netwib_priv_io_mem_init(NULL, pbufread, NULL, pbufwrite,
                                closebufsatend, &rdsup, &wrsup,
                                (netwib_priv_io_mem *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdsup, wrsup,
                           pcommon,
                           &netwib_priv_io_mem_read,
                           &netwib_priv_io_mem_write,
                           &netwib_priv_io_mem_wait,
                           &netwib_priv_io_mem_unread,
                           &netwib_priv_io_mem_ctl_set,
                           &netwib_priv_io_mem_ctl_get,
                           &netwib_priv_io_mem_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_mem_lock(netwib_thread_rwlock *plockbufread,
                                   netwib_buf *pbufread,
                                   netwib_thread_rwlock *plockbufwrite,
                                   netwib_buf *pbufwrite,
                                   netwib_bool closebufsatend,
                                   netwib_io **ppio)
{
  netwib_bool rdsup, wrsup;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_mem), &pcommon));
  ret = netwib_priv_io_mem_init(plockbufread, pbufread,
                                plockbufwrite, pbufwrite,
                                closebufsatend, &rdsup, &wrsup,
                                (netwib_priv_io_mem *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdsup, wrsup,
                           pcommon,
                           &netwib_priv_io_mem_read_lock,
                           &netwib_priv_io_mem_write_lock,
                           &netwib_priv_io_mem_wait_lock,
                           &netwib_priv_io_mem_unread_lock,
                           &netwib_priv_io_mem_ctl_set_lock,
                           &netwib_priv_io_mem_ctl_get_lock,
                           &netwib_priv_io_mem_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

