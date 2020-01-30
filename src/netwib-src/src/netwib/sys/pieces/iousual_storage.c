/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
typedef struct {
  netwib_buf buf;
  netwib_bool endreached;
} netwib_priv_io_storage2;

typedef struct {
  netwib_priv_io_storage2 rd;
  netwib_priv_io_storage2 wr;
  netwib_buf unreadbuf;
} netwib_priv_io_storage;

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_storage_init(netwib_bool *preadinitialized,
                                              netwib_bool *pwriteinitialized,
                                              netwib_priv_io_storage *ptr)
{

  netwib_er(netwib_buf_init_mallocdefault(&ptr->rd.buf));
  ptr->rd.endreached = NETWIB_FALSE;
  ptr->rd.buf.flags |= NETWIB_BUF_FLAGS_CANSLIDE;
  *preadinitialized = NETWIB_TRUE;

  netwib_er(netwib_buf_init_mallocdefault(&ptr->wr.buf));
  ptr->wr.endreached = NETWIB_FALSE;
  ptr->wr.buf.flags |= NETWIB_BUF_FLAGS_CANSLIDE;
  *pwriteinitialized = NETWIB_TRUE;

  netwib_er(netwib_buf_init_mallocdefault(&ptr->unreadbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_storage_close(netwib_priv_io_storage *ptr)
{

  /* close buffers */
  netwib_er(netwib_buf_close(&ptr->rd.buf));
  netwib_er(netwib_buf_close(&ptr->wr.buf));
  netwib_er(netwib_buf_close(&ptr->unreadbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_storage_contain(netwib_priv_io_storage2 *ptr,
                                                 netwib_bool *pyes)
{
  netwib_err ret;

  ret = netwib_tlv_entry_typelen(&ptr->buf, NULL, NULL, NULL);
  if (ret == NETWIB_ERR_DATAEND) {
    if (ptr->endreached) {
      return(NETWIB_ERR_DATAEND);
    }
    if (pyes != NULL) *pyes = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  if (pyes != NULL) *pyes = NETWIB_TRUE;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_storage_read(netwib_io *pio,
                                              netwib_buf *pbuf)
{
  netwib_priv_io_storage *ptr = (netwib_priv_io_storage *)pio->pcommon;
  netwib_buf buf;
  netwib_bool b;
  netwib_uint32 skipsize;
  netwib_err ret;

  /* check if stored data contain a storage */
  netwib_er(netwib_priv_io_storage_contain(&ptr->rd, &b));
  if (b) {
    netwib_er(netwib_tlv_decode_buf(&ptr->rd.buf, &buf, &skipsize));
    netwib_er(netwib_buf_append_buf(&buf, pbuf));
    ptr->rd.buf.beginoffset += skipsize;
    return(NETWIB_ERR_OK);
  }

  /* read more data */
  ret = netwib_io_read(pio->rd.pnext, pbuf);
  if (ret == NETWIB_ERR_DATAEND) {
    ptr->rd.endreached = NETWIB_TRUE;
  }

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_storage_flush(netwib_io *pio,
                                               netwib_bool waitinfinite)
{
  netwib_priv_io_storage *ptr = (netwib_priv_io_storage *)pio->pcommon;
  netwib_buf buf;
  netwib_bool b;
  netwib_uint32 skipsize;
  netwib_time *ptime;
  netwib_err ret;

  ptime = NETWIB_TIME_ZERO;
  if (waitinfinite) ptime = NETWIB_TIME_INFINITE;

  /* try to write every entry in the storage */
  ret = NETWIB_ERR_OK;
  while(NETWIB_TRUE) {
    /* check if we have another entry */
    ret = netwib_priv_io_storage_contain(&ptr->wr, &b);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    if (!b) {
      break;
    }
    /* check if we can write */
    netwib_er(netwib_io_wait(pio, NETWIB_IO_WAYTYPE_WRITE, ptime, &b));
    if (!b) {
      break;
    }
    /* write */
    ret = netwib_tlv_decode_buf(&ptr->wr.buf, &buf, &skipsize);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    ret = netwib_io_write(pio->wr.pnext, &buf);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    ptr->wr.buf.beginoffset += skipsize;
  }

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_storage_write(netwib_io *pio,
                                               netwib_constbuf *pbuf)
{
  netwib_priv_io_storage *ptr = (netwib_priv_io_storage *)pio->pcommon;

  netwib_er(netwib_tlv_append_buf(pbuf, &ptr->wr.buf));
  netwib_er(netwib_priv_io_storage_flush(pio, NETWIB_FALSE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_storage_wait(netwib_io *pio,
                                              netwib_io_waytype way,
                                              netwib_consttime *pabstime,
                                              netwib_bool *pevent)
{
  netwib_priv_io_storage *ptr = (netwib_priv_io_storage *)pio->pcommon;
  netwib_bool b;
  netwib_err ret;

  switch (way) {
    case NETWIB_IO_WAYTYPE_READ :
      /* check if stored data contain a storage */
      ret = netwib_priv_io_storage_contain(&ptr->rd, &b);
      if (ret == NETWIB_ERR_DATAEND) {
        if (pevent != NULL) *pevent = NETWIB_TRUE;
        return(NETWIB_ERR_OK);
      } else if (ret != NETWIB_ERR_OK) {
        return(ret);
      }
      if (b) {
        if (pevent != NULL) *pevent = NETWIB_TRUE;
        return(NETWIB_ERR_OK);
      }
      /* wait for next */
      netwib_er(netwib_io_wait(pio->rd.pnext, NETWIB_IO_WAYTYPE_READ, pabstime,
                               pevent));
      break;
    case NETWIB_IO_WAYTYPE_WRITE :
      /* always possible to write */
      if (pevent != NULL) *pevent = NETWIB_TRUE;
      break;
    case NETWIB_IO_WAYTYPE_RDWR :
    case NETWIB_IO_WAYTYPE_SUPPORTED :
      return(NETWIB_ERR_PLEASECONSTRUCT);
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_storage_unread(netwib_io *pio,
                                                netwib_constbuf *pbuf)
{
  netwib_priv_io_storage *ptr = (netwib_priv_io_storage *)pio->pcommon;

  netwib__buf_reinit(&ptr->unreadbuf);
  netwib_er(netwib_tlv_append_buf(pbuf, &ptr->unreadbuf));
  netwib_er(netwib_tlv_prepend_tlv(&ptr->unreadbuf, &ptr->rd.buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_storage_ctl_set(netwib_io *pio,
                                                 netwib_io_waytype way,
                                                 netwib_io_ctltype type,
                                                 netwib_ptr p,
                                                 netwib_uint32 ui)
{
  netwib_priv_io_storage *ptr = (netwib_priv_io_storage *)pio->pcommon;

  switch(type) {
    case NETWIB_IO_CTLTYPE_END:
      if (way != NETWIB_IO_WAYTYPE_WRITE) return(NETWIB_ERR_PAINVALIDTYPE);
      ptr->wr.endreached = NETWIB_TRUE;
      netwib_er(netwib_priv_io_storage_flush(pio, NETWIB_TRUE));
      netwib_er(netwib_io_ctl_set_support(pio, way, NETWIB_FALSE));
      netwib_er(netwib_io_ctl_set_end_write(pio->wr.pnext));
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IO_CTLTYPE_STORAGE_FLUSH:
      netwib_er(netwib_priv_io_storage_flush(pio, NETWIB_TRUE));
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
static netwib_err netwib_priv_io_storage_fclose(netwib_io *pio)
{
  netwib_priv_io_storage *ptr = (netwib_priv_io_storage *)pio->pcommon;

  netwib_er(netwib_priv_io_storage_close(ptr));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_storage(netwib_io **ppio)
{
  netwib_bool rdsup, wrsup;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_storage), &pcommon));
  ret = netwib_priv_io_storage_init(&rdsup, &wrsup,
                                    (netwib_priv_io_storage *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdsup, wrsup,
                           pcommon,
                           &netwib_priv_io_storage_read,
                           &netwib_priv_io_storage_write,
                           &netwib_priv_io_storage_wait,
                           &netwib_priv_io_storage_unread,
                           &netwib_priv_io_storage_ctl_set,
                           NULL, /* ctl_get */
                           &netwib_priv_io_storage_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

