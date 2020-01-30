/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
typedef struct {
  netwib_io_init_data_type type;
  netwib_buf buf;
  netwib_bool endreached;
  netwib_bool line_msdos;
  netwib_uint32 chunk_minsize;
  netwib_uint32 chunk_maxsize;
  netwib_uint32 fixed_size;
} netwib_priv_io_data2;

typedef struct {
  netwib_priv_io_data2 rd;
  netwib_priv_io_data2 wr;
} netwib_priv_io_data;

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_data_init(netwib_io_init_data_type rdtype,
                                           netwib_io_init_data_type wrtype,
                                           netwib_bool *preadinitialized,
                                           netwib_bool *pwriteinitialized,
                                           netwib_priv_io_data *ptr)
{

  netwib_er(netwib_buf_init_mallocdefault(&ptr->rd.buf));
  ptr->rd.type = rdtype;
  ptr->rd.buf.flags |= NETWIB_BUF_FLAGS_CANSLIDE;
  ptr->rd.endreached = NETWIB_FALSE;
  ptr->rd.line_msdos = NETWIB_TRUE;
  ptr->rd.chunk_minsize = 1;
  ptr->rd.chunk_maxsize = 0;
  ptr->rd.fixed_size = 1;
  *preadinitialized = NETWIB_TRUE;

  netwib_er(netwib_buf_init_mallocdefault(&ptr->wr.buf));
  ptr->wr.type = wrtype;
  ptr->wr.buf.flags |= NETWIB_BUF_FLAGS_CANSLIDE;
  ptr->wr.endreached = NETWIB_FALSE;
  ptr->wr.line_msdos = NETWIB_TRUE;
  ptr->wr.chunk_minsize = 1;
  ptr->wr.chunk_maxsize = 0;
  ptr->wr.fixed_size = 1;
  *pwriteinitialized = NETWIB_TRUE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_data_close(netwib_priv_io_data *ptr)
{

  /* close buffers */
  netwib_er(netwib_buf_close(&ptr->rd.buf));
  netwib_er(netwib_buf_close(&ptr->wr.buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_data_contain(netwib_priv_io_data2 *ptr,
                                              netwib_bool *pyes,
                                              netwib_uint32 *pdatasize,
                                              netwib_uint32 *poffsetnext)
{
  netwib_data data;
  netwib_uint32 datasize, i, offset0D;
  netwib_bool found0D;
  netwib_byte c;

  if (pyes != NULL) *pyes = NETWIB_FALSE;

  switch(ptr->type) {
  case NETWIB_IO_INIT_DATA_TYPE_LINE :
    /* We treat those cases :
       abc            : nodata
       abc\r          : nodata
       abc\n          : abc (next: empty)
       abc\ndef       : abc (next: def)
       abc\r\ndef     : abc (next: def)
       abc\r\r\ndef   : abc (next: def)
       abc\rdef       : abc (next: def)
       abc\rd\r\nef   : abc (next: d\r\nef)
       abc[end]       : abc (next: empty)
       abc\r[end]     : abc (next: empty)
       abc\n[end]     : abc (next: empty)
       [end]          : return NETWIB_ERR_DATAEND
    */
    data = netwib__buf_ref_data_ptr(&ptr->buf);
    datasize = netwib__buf_ref_data_size(&ptr->buf);
    found0D = NETWIB_FALSE;
    offset0D = 0;
    for (i = 0 ; i < datasize ; i++) {
      c = data[i];
      if (c == '\n') {
        if (pdatasize != NULL) *pdatasize = found0D?offset0D:i;
        if (poffsetnext != NULL) *poffsetnext = i+1;
        if (pyes != NULL) *pyes = NETWIB_TRUE;
        return(NETWIB_ERR_OK);
      } else if (c == '\r') {
        if (!found0D) {
          offset0D = i;
          found0D = NETWIB_TRUE;
        }
      } else {
        if (found0D) {
          if (pdatasize != NULL) *pdatasize = offset0D;
          if (poffsetnext != NULL) *poffsetnext = i;
          if (pyes != NULL) *pyes = NETWIB_TRUE;
          return(NETWIB_ERR_OK);
        }
      }
    }
    /* if we are here, we did not found a data. We check special case
       of end of data */
    if (ptr->endreached) {
      if (datasize == 0) {
        return(NETWIB_ERR_DATAEND);
      }
      i = datasize;
      while (i) {
        i--;
      if (data[i] != '\r') break;
      }
      if (pdatasize != NULL) *pdatasize = i+1;
      if (poffsetnext != NULL) *poffsetnext = datasize;
      if (pyes != NULL) *pyes = NETWIB_TRUE;
      return(NETWIB_ERR_OK);
    }
    break;
  case NETWIB_IO_INIT_DATA_TYPE_CHUNK :
    /* We treat those cases (minsize==3, maxsize==4) :
       abc            : abc (next: empty)
       ab             : nochunk
       abc[end]       : abc (next: empty)
       ab[end]        : ab (this is an exception, but we have to return
                        this data)
       [end]          : return NETWIB_ERR_DATAEND
    */
    datasize = netwib__buf_ref_data_size(&ptr->buf);
    if (datasize >= ptr->chunk_minsize) {
      if (pyes != NULL) *pyes = NETWIB_TRUE;
      if (ptr->chunk_maxsize != 0) {
        if (datasize > ptr->chunk_maxsize) {
          datasize = ptr->chunk_maxsize;
        }
      }
      if (pdatasize != NULL) *pdatasize = datasize;
      if (poffsetnext != NULL) *poffsetnext = datasize;
      return(NETWIB_ERR_OK);
    }
    if (ptr->endreached) {
      if (datasize) {
        if (pyes != NULL) *pyes = NETWIB_TRUE;
        if (pdatasize != NULL) *pdatasize = datasize;
        if (poffsetnext != NULL) *poffsetnext = datasize;
        return(NETWIB_ERR_OK);
      }
      return(NETWIB_ERR_DATAEND);
    }
    break;
  case NETWIB_IO_INIT_DATA_TYPE_FIXED :
    /* We treat those cases (size==3) :
       abc            : abc (next: empty)
       ab             : nodata
       abc[end]       : abc (next: empty)
       ab[end]        : ab (this is an exception, but we have to return
                        this data)
       [end]          : return NETWIB_ERR_DATAEND
    */
    datasize = netwib__buf_ref_data_size(&ptr->buf);
    if (datasize >= ptr->fixed_size) {
      if (pyes != NULL) *pyes = NETWIB_TRUE;
      if (pdatasize != NULL) *pdatasize = ptr->fixed_size;
      if (poffsetnext != NULL) *poffsetnext = ptr->fixed_size;
      return(NETWIB_ERR_OK);
    }
    if (ptr->endreached) {
      if (datasize) {
        if (pyes != NULL) *pyes = NETWIB_TRUE;
        if (pdatasize != NULL) *pdatasize = datasize;
        if (poffsetnext != NULL) *poffsetnext = datasize;
        return(NETWIB_ERR_OK);
      }
      return(NETWIB_ERR_DATAEND);
    }
    break;
  case NETWIB_IO_INIT_DATA_TYPE_TRANSPARENT :
    /* We treat those cases :
       abc            : abc (next: empty)
       abc[end]       : abc (next: empty)
       [end]          : return NETWIB_ERR_DATAEND
    */
    datasize = netwib__buf_ref_data_size(&ptr->buf);
    if (datasize) {
      if (pyes != NULL) *pyes = NETWIB_TRUE;
      if (pdatasize != NULL) *pdatasize = datasize;
      if (poffsetnext != NULL) *poffsetnext = datasize;
      return(NETWIB_ERR_OK);
    }
    if (ptr->endreached) {
      return(NETWIB_ERR_DATAEND);
    }
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_data_read(netwib_io *pio,
                                           netwib_buf *pbuf)
{
  netwib_priv_io_data *ptr = (netwib_priv_io_data *)pio->pcommon;
  netwib_data data;
  netwib_uint32 datasize, offsetnext;
  netwib_bool b;
  netwib_err ret;

  /* check if stored data contain expected data */
  netwib_er(netwib_priv_io_data_contain(&ptr->rd, &b, &datasize, &offsetnext));
  if (b) {
    data = netwib__buf_ref_data_ptr(&ptr->rd.buf);
    netwib_er(netwib_buf_append_data(data, datasize, pbuf));
    ptr->rd.buf.beginoffset += offsetnext;
    return(NETWIB_ERR_OK);
  }

  /* optimization for transparent : if we are here, there is nothing in
     rd.buf, so we can directly store in user pbuf */
  if (ptr->rd.type == NETWIB_IO_INIT_DATA_TYPE_TRANSPARENT) {
    return(netwib_io_read(pio->rd.pnext, pbuf));
  }

  /* read more data */
  ret = netwib_io_read(pio->rd.pnext, &ptr->rd.buf);
  if (ret == NETWIB_ERR_DATAEND) {
    ptr->rd.endreached = NETWIB_TRUE;
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  /* check if it's ok now */
  netwib_er(netwib_priv_io_data_contain(&ptr->rd, &b, &datasize, &offsetnext));
  if (b) {
    data = netwib__buf_ref_data_ptr(&ptr->rd.buf);
    netwib_er(netwib_buf_append_data(data, datasize, pbuf));
    ptr->rd.buf.beginoffset += offsetnext;
    return(NETWIB_ERR_OK);
  }

  return(NETWIB_ERR_DATANOTAVAIL);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_data_write(netwib_io *pio,
                                            netwib_constbuf *pbuf)
{
  netwib_priv_io_data *ptr = (netwib_priv_io_data *)pio->pcommon;
  netwib_uint32 datasize, offsetnext, realsize;
  netwib_bool b;
  netwib_err ret;

  /* optimization for transparent : if there is nothing in
     wr.buf, so we can directly write user pbuf */
  if (ptr->wr.type == NETWIB_IO_INIT_DATA_TYPE_TRANSPARENT &&
      netwib__buf_ref_data_size(&ptr->wr.buf) == 0) {
    return(netwib_io_write(pio->wr.pnext, pbuf));
  }

  /* store user data in buffer */
  netwib_er(netwib_buf_append_buf(pbuf, &ptr->wr.buf));

  /* for line, instead of rechecking if buffer contains a '\n' (using
     data_contain), we directly write (because we will add rn\n). */
  if (ptr->wr.type == NETWIB_IO_INIT_DATA_TYPE_LINE) {
    if (ptr->wr.line_msdos) {
      netwib_er(netwib_buf_append_string("\r\n", &ptr->wr.buf));
    } else {
      netwib_er(netwib_buf_append_string("\n", &ptr->wr.buf));
    }
    netwib_er(netwib_io_write(pio->wr.pnext, &ptr->wr.buf));
    netwib__buf_reinit(&ptr->wr.buf);
    return(NETWIB_ERR_OK);
  }

  /* write 0, 1, or more chunks */
  while(NETWIB_TRUE) {
    /* check if stored data contain expected data */
    netwib_er(netwib_priv_io_data_contain(&ptr->wr, &b, &datasize,
                                          &offsetnext));
    if (!b) {
      return(NETWIB_ERR_OK);
    }
    /* write this chunk */
    realsize = netwib__buf_ref_data_size(&ptr->wr.buf);
    ptr->wr.buf.endoffset = ptr->wr.buf.beginoffset + datasize;
    ret = netwib_io_write(pio->wr.pnext, &ptr->wr.buf);
    ptr->wr.buf.endoffset = ptr->wr.buf.beginoffset + realsize;
    if (ret != NETWIB_ERR_OK) {
      return(ret);
    }
    /* skip */
    ptr->wr.buf.beginoffset += offsetnext;
  }

  return(NETWIB_ERR_LOINTERNALERROR);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_data_wait(netwib_io *pio,
                                           netwib_io_waytype way,
                                           netwib_consttime *pabstime,
                                           netwib_bool *pevent)
{
  netwib_priv_io_data *ptr = (netwib_priv_io_data *)pio->pcommon;
  netwib_bool b;
  netwib_err ret;

  switch (way) {
    case NETWIB_IO_WAYTYPE_READ :
      /* check if stored data contain expected data */
      ret = netwib_priv_io_data_contain(&ptr->rd, &b, NULL, NULL);
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
      /* check if stored data contain expected data */
      ret = netwib_priv_io_data_contain(&ptr->wr, &b, NULL, NULL);
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
      netwib_er(netwib_io_wait(pio->wr.pnext, NETWIB_IO_WAYTYPE_WRITE,
                               pabstime, pevent));
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
static netwib_err netwib_priv_io_data_unread(netwib_io *pio,
                                             netwib_constbuf *pbuf)
{
  netwib_priv_io_data *ptr = (netwib_priv_io_data *)pio->pcommon;
  netwib_buf eolbuf;

  switch(ptr->rd.type) {
  case NETWIB_IO_INIT_DATA_TYPE_LINE :
    if (ptr->rd.line_msdos) {
      netwib_er(netwib_buf_init_ext_string("\r\n", &eolbuf));
    } else {
      netwib_er(netwib_buf_init_ext_string("\n", &eolbuf));
    }
    netwib_er(netwib_buf_prepend_buf(&eolbuf, &ptr->rd.buf));
    netwib_er(netwib_buf_prepend_buf(pbuf, &ptr->rd.buf));
    break;
  case NETWIB_IO_INIT_DATA_TYPE_CHUNK :
  case NETWIB_IO_INIT_DATA_TYPE_FIXED :
  case NETWIB_IO_INIT_DATA_TYPE_TRANSPARENT :
    netwib_er(netwib_buf_prepend_buf(pbuf, &ptr->rd.buf));
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_data_ctl_set(netwib_io *pio,
                                              netwib_io_waytype way,
                                              netwib_io_ctltype type,
                                              netwib_ptr p,
                                              netwib_uint32 ui)
{
  netwib_priv_io_data *ptr = (netwib_priv_io_data *)pio->pcommon;

#define netwib__io_data_ctl_set(name,type,value) switch(way){case NETWIB_IO_WAYTYPE_READ:ptr->rd.name=(type)value;break; case NETWIB_IO_WAYTYPE_WRITE:ptr->wr.name=(type)value;break; case NETWIB_IO_WAYTYPE_RDWR:ptr->rd.name=(type)value;ptr->wr.name=(type)value;break; case NETWIB_IO_WAYTYPE_NONE:break; case NETWIB_IO_WAYTYPE_SUPPORTED:return(NETWIB_ERR_PLEASECONSTRUCT);break; default:return(NETWIB_ERR_PAINVALIDTYPE);}

  switch(type) {
  case NETWIB_IO_CTLTYPE_END:
    if (way != NETWIB_IO_WAYTYPE_WRITE) return(NETWIB_ERR_PAINVALIDTYPE);
    if (netwib__buf_ref_data_size(&ptr->wr.buf)) {
      /* note : for line, we do not put an ending '\r\n' or '\n' */
      netwib_er(netwib_io_write(pio->wr.pnext, &ptr->wr.buf));
      netwib__buf_reinit(&ptr->wr.buf);
    }
    ptr->wr.endreached = NETWIB_TRUE;
    netwib_er(netwib_io_ctl_set_support(pio, way, NETWIB_FALSE));
    netwib_er(netwib_io_ctl_set_end_write(pio->wr.pnext));
    break;
  case NETWIB_IO_CTLTYPE_DATA_LINE_MSDOS :
    netwib__io_data_ctl_set(line_msdos, netwib_bool, ui);
    break;
  case NETWIB_IO_CTLTYPE_DATA_CHUNK_MINSIZE :
    netwib__io_data_ctl_set(chunk_minsize, netwib_uint32, ui);
    break;
  case NETWIB_IO_CTLTYPE_DATA_CHUNK_MAXSIZE :
    netwib__io_data_ctl_set(chunk_maxsize, netwib_uint32, ui);
    break;
  case NETWIB_IO_CTLTYPE_DATA_FIXED_SIZE :
    netwib__io_data_ctl_set(fixed_size, netwib_uint32, ui);
    break;
  case NETWIB_IO_CTLTYPE_DATA_TYPE :
    netwib__io_data_ctl_set(type, netwib_io_init_data_type, ui);
    break;
  default:
    return(NETWIB_ERR_PLEASETRYNEXT);
  }

  p = p; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_data_ctl_get(netwib_io *pio,
                                              netwib_io_waytype way,
                                              netwib_io_ctltype type,
                                              netwib_ptr p,
                                              netwib_uint32 *pui)
{
  netwib_priv_io_data *ptr = (netwib_priv_io_data *)pio->pcommon;

#define netwib__io_data_ctl_get(name,type,stor) switch(way){case NETWIB_IO_WAYTYPE_READ:if(stor!=NULL)*stor=(type)ptr->rd.name;break; case NETWIB_IO_WAYTYPE_WRITE:if(stor!=NULL)*stor=(type)ptr->wr.name;break; case NETWIB_IO_WAYTYPE_RDWR:case NETWIB_IO_WAYTYPE_SUPPORTED:return(NETWIB_ERR_PLEASECONSTRUCT);break; default:return(NETWIB_ERR_PAINVALIDTYPE);}

  switch(type) {
  case NETWIB_IO_CTLTYPE_DATA_LINE_MSDOS :
    netwib__io_data_ctl_get(line_msdos, netwib_uint32, pui);
    break;
  case NETWIB_IO_CTLTYPE_DATA_CHUNK_MINSIZE :
    netwib__io_data_ctl_get(chunk_minsize, netwib_uint32, pui);
    break;
  case NETWIB_IO_CTLTYPE_DATA_CHUNK_MAXSIZE :
    netwib__io_data_ctl_get(chunk_maxsize, netwib_uint32, pui);
    break;
  case NETWIB_IO_CTLTYPE_DATA_FIXED_SIZE :
    netwib__io_data_ctl_get(fixed_size, netwib_uint32, pui);
    break;
  case NETWIB_IO_CTLTYPE_DATA_TYPE :
    netwib__io_data_ctl_get(type, netwib_uint32, pui);
    break;
  default:
    return(NETWIB_ERR_PLEASETRYNEXT);
  }

  p = p; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_data_fclose(netwib_io *pio)
{
  netwib_priv_io_data *ptr = (netwib_priv_io_data *)pio->pcommon;

  netwib_er(netwib_priv_io_data_close(ptr));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_data(netwib_io_init_data_type rdtype,
                               netwib_io_init_data_type wrtype,
                               netwib_io **ppio)
{
  netwib_bool rdsup, wrsup;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_data), &pcommon));
  ret = netwib_priv_io_data_init(rdtype, wrtype, &rdsup, &wrsup,
                                 (netwib_priv_io_data *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdsup, wrsup,
                           pcommon,
                           &netwib_priv_io_data_read,
                           &netwib_priv_io_data_write,
                           &netwib_priv_io_data_wait,
                           &netwib_priv_io_data_unread,
                           &netwib_priv_io_data_ctl_set,
                           &netwib_priv_io_data_ctl_get,
                           &netwib_priv_io_data_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}


