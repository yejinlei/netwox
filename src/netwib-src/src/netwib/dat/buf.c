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
#define NETWIB_PRIV_BUF_DEFAULT_APPEND1  1024
#define NETWIB_PRIV_BUF_DEFAULT_APPEND2 16384

/*-------------------------------------------------------------*/
#define NETWIB_PRIV_BUF_PTR_CLOSED ((netwib_data)1)

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_LEAK==1
 /* MSBs bitfield stores leakid */
 #define netwib__priv_buf_get_leakid(pbuf) (pbuf->flags>>NETWIB_BUF_FLAGS_USEDBITS)
 #define netwib__priv_buf_set_leakid(pbuf,leakid) { pbuf->flags = (leakid<<NETWIB_BUF_FLAGS_USEDBITS) | (pbuf->flags&((1<<NETWIB_BUF_FLAGS_USEDBITS)-1)); }
#else
 /* field inittype stores only inittype */
 #define netwib__priv_buf_get_leakid(pbuf)
 #define netwib__priv_buf_set_leakid(pbuf,leakid)
#endif

/*-------------------------------------------------------------*/
#define netwib__priv_buf_cangrow(pb) ((pb)->flags&(NETWIB_BUF_FLAGS_ALLOC|NETWIB_BUF_FLAGS_CANALLOC))

/*-------------------------------------------------------------*/
#define netwib__priv_buf_canslide(pb) (((pb)->flags&NETWIB_BUF_FLAGS_CANSLIDE) && (pb)->beginoffset!=0 && (!netwib__priv_buf_cangrow(pb) || ((pb)->beginoffset> (pb)->totalsize/2)))
#define netwib__priv_buf_slide(pb) {netwib_c_memcpy((pb)->totalptr, netwib__buf_ref_data_ptr(pb), netwib__buf_ref_data_size(pb)); (pb)->endoffset -= (pb)->beginoffset; (pb)->beginoffset = 0;}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_buf_realloc(netwib_uint32 neededspace,
                                          netwib_buf *pbuf)
{
  netwib_data data;
  netwib_uint32 reallocsize;
  netwib_uint32 allocstep;

  if (neededspace == 0) {
    return(NETWIB_ERR_OK);
  }

  allocstep = NETWIB_PRIV_BUF_DEFAULT_APPEND1;
  if (pbuf->totalsize > NETWIB_PRIV_BUF_DEFAULT_APPEND2) {
    allocstep = NETWIB_PRIV_BUF_DEFAULT_APPEND2;
  }
  reallocsize = pbuf->totalsize;
  reallocsize += ((neededspace-1)/allocstep + 1) * allocstep;

  if (pbuf->flags & NETWIB_BUF_FLAGS_ALLOC) {
    /* realloc */
#if NETWIB_DEBUG_LEAK==1
    netwib_er(netwib_debug_leak_del_buf(pbuf->totalptr,
                                        netwib__priv_buf_get_leakid(pbuf)));
#endif
    if (pbuf->flags & NETWIB_BUF_FLAGS_SENSITIVE) {
      /* slow but safe */
      netwib_er(netwib_ptr_malloc(reallocsize, (netwib_ptr*)&data));
      netwib_c_memcpy(data, pbuf->totalptr, pbuf->totalsize);
      netwib_c_memset(pbuf->totalptr, 0, pbuf->totalsize);
      netwib_er(netwib_ptr_free((netwib_ptr*)&pbuf->totalptr));
      pbuf->totalptr = data;
    } else {
      netwib_er(netwib_ptr_realloc(reallocsize, (netwib_ptr*)&pbuf->totalptr));
    }
#if NETWIB_DEBUG_LEAK==1
    netwib_er(netwib_debug_leak_add_buf(pbuf->totalptr,
                                        netwib__priv_buf_get_leakid(pbuf)));
#endif
  } else {
    /* allocate buffer */
    netwib_er(netwib_ptr_malloc(reallocsize, (netwib_ptr*)&data));
    if (pbuf->flags & NETWIB_BUF_FLAGS_CANALLOC) {
      /* convert an array to an allocated memory */
      netwib_c_memcpy(data, pbuf->totalptr, pbuf->totalsize);
      if ( (pbuf->flags & NETWIB_BUF_FLAGS_SENSITIVE) &&
           !(pbuf->flags & NETWIB_BUF_FLAGS_SENSITIVE_READONLY)) {
        netwib_c_memset(pbuf->totalptr, 0, pbuf->totalsize);
      }
      pbuf->totalptr = data;
      /* we do not need to change beginoffset and endoffset */
      pbuf->flags &= ~NETWIB_BUF_FLAGS_CANALLOC;
      pbuf->flags |= NETWIB_BUF_FLAGS_ALLOC;
    } else {
      return(NETWIB_ERR_LOINTERNALERROR);
    }
#if NETWIB_DEBUG_LEAK==1
    {
      netwib_uint32 leakid;
      netwib_er(netwib_debug_leak_id_buf(&leakid));
      netwib__priv_buf_set_leakid(pbuf, leakid);
      netwib_er(netwib_debug_leak_add_buf(pbuf->totalptr, leakid));
    }
#endif
  }

  pbuf->totalsize = reallocsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_init_malloc(netwib_uint32 allocsize,
                                  netwib_buf *pbuf)
{
  netwib_data data;
  netwib_uint32 totalsize;

  /* parameter verification */
  if (pbuf == NULL) {
    /* nothing to alloc */
    return(NETWIB_ERR_OK);
  }

  /* better to warn user */
  if (allocsize == 0) {
    return(NETWIB_ERR_PATOOLOW);
  }

  /* compute totalsize */
  totalsize = allocsize;

  /* allocate memory */
  netwib_er(netwib_ptr_malloc(totalsize, (netwib_ptr*)&data));

  /* set values */
  pbuf->flags = NETWIB_BUF_FLAGS_ALLOC;
  pbuf->totalptr = data;
  pbuf->totalsize = totalsize;
  pbuf->beginoffset = 0;
  pbuf->endoffset = 0;

#if NETWIB_DEBUG_LEAK==1
  {
    netwib_uint32 leakid;
    netwib_er(netwib_debug_leak_id_buf(&leakid));
    netwib__priv_buf_set_leakid(pbuf, leakid);
    netwib_er(netwib_debug_leak_add_buf(pbuf->totalptr, leakid));
  }
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_init_ext_array(netwib_constptr array,
                                     netwib_uint32 arraysize,
                                     netwib_uint32 beginoffset,
                                     netwib_uint32 endoffset,
                                     netwib_bufext *pbuf)
{
  /* parameters verification */
  if (array == NULL && arraysize)
    return(NETWIB_ERR_PANULLPTRSIZE);
  if (beginoffset > arraysize)
    return(NETWIB_ERR_PATOOHIGH);
  if (endoffset > arraysize)
    return(NETWIB_ERR_PATOOHIGH);
  if (pbuf == NULL) {
    /* nothing to do */
    return(NETWIB_ERR_OK);
  }

  /* set values */
  pbuf->flags = 0;
  netwib__priv_buf_set_leakid(pbuf, NETWIB_DEBUG_LEAK_ID_BUF_IGNORE);
  pbuf->totalptr = netwib_priv_castdata(array);
  pbuf->totalsize = arraysize;
  pbuf->beginoffset = beginoffset;
  pbuf->endoffset = endoffset;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_init_ext_storagearray(netwib_constptr array,
                                            netwib_uint32 arraysize,
                                            netwib_bufext *pbuf)
{
  /* parameters verification */
  if (array == NULL && arraysize)
    return(NETWIB_ERR_PANULLPTRSIZE);
  if (pbuf == NULL) {
    /* nothing to do */
    return(NETWIB_ERR_OK);
  }

  /* set values */
  pbuf->flags = NETWIB_BUF_FLAGS_CANALLOC;
  netwib__priv_buf_set_leakid(pbuf, NETWIB_DEBUG_LEAK_ID_BUF_IGNORE);
  pbuf->totalptr = netwib_priv_castdata(array);
  pbuf->totalsize = arraysize;
  pbuf->beginoffset = 0;
  pbuf->endoffset = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_init_ext_buf(netwib_constbuf *pbufin,
                                   netwib_bufext *pbuf)
{
  if (pbuf == NULL) {
    /* nothing to do */
    return(NETWIB_ERR_OK);
  }

  /* set values */
  pbuf->flags = 0;
  netwib__priv_buf_set_leakid(pbuf, NETWIB_DEBUG_LEAK_ID_BUF_IGNORE);
  pbuf->totalptr = netwib__buf_ref_data_ptr(pbufin);
  pbuf->totalsize = netwib__buf_ref_data_size(pbufin);
  pbuf->beginoffset = 0;
  pbuf->endoffset = netwib__buf_ref_data_size(pbufin);
  netwib__buf_transfersensitive(pbufin, pbuf);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_init_ext_string(netwib_conststring str,
                                      netwib_bufext *pbuf)
{
  /* parameter verification */
  if (pbuf == NULL) {
    /* nothing to do */
    return(NETWIB_ERR_OK);
  }

  /* set values */
  pbuf->flags = 0;
  netwib__priv_buf_set_leakid(pbuf, NETWIB_DEBUG_LEAK_ID_BUF_IGNORE);
  pbuf->beginoffset = 0;
  if (str) {
    pbuf->endoffset = netwib_c_strlen(str);
    pbuf->totalptr = netwib_priv_castdata(str);
    pbuf->totalsize = pbuf->endoffset + 1;
  } else {
    pbuf->endoffset = 0;
    pbuf->totalptr = NULL;
    pbuf->totalsize = 0;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_shift(netwib_buf *pbuf,
                            netwib_int32 offset,
                            netwib_bool truncbegend)
{
  netwib_uint32 offsetpos, datasize;

  /* simple cases */
  if (pbuf == NULL) {
    /* shifting an empty buffer does not change it */
    return(NETWIB_ERR_OK);
  }
  if (pbuf->totalptr == NETWIB_PRIV_BUF_PTR_CLOSED)
    return(NETWIB_ERR_LOOBJUSECLOSED);

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_buf(pbuf->totalptr,
                                        netwib__priv_buf_get_leakid(pbuf)));
#endif

  /* simple case */
  if (offset == 0) {
    return(NETWIB_ERR_OK);
  }

  /* if truncate */
  if (truncbegend) {
    datasize = pbuf->endoffset - pbuf->beginoffset;
    if (offset < 0) {
      offsetpos = (netwib_uint32)-offset;
      if (offsetpos >= datasize) {
        pbuf->endoffset = pbuf->beginoffset;
        return(NETWIB_ERR_OK);
      }
      netwib_c_memcpy(pbuf->totalptr + pbuf->beginoffset,
                      pbuf->totalptr + pbuf->beginoffset + offsetpos,
                      datasize - offsetpos);
      pbuf->endoffset += offset;
      return(NETWIB_ERR_OK);
    }
    offsetpos = (netwib_uint32)offset;
    if (offsetpos >= datasize) {
      pbuf->beginoffset = pbuf->endoffset;
      return(NETWIB_ERR_OK);
    }
    netwib_c_memmove(pbuf->totalptr + pbuf->beginoffset + offset,
                     pbuf->totalptr + pbuf->beginoffset,
                     datasize - offsetpos);
    pbuf->beginoffset += offset;
    return(NETWIB_ERR_OK);
  }

  /* now, do not truncate */
  if (offset < 0) {
    offsetpos = (netwib_uint32)-offset;

    /* simple left shift */
    if (offsetpos <= pbuf->beginoffset) {
      netwib_c_memcpy(pbuf->totalptr + pbuf->beginoffset + offset,
                      pbuf->totalptr + pbuf->beginoffset,
                      pbuf->endoffset - pbuf->beginoffset);
      pbuf->beginoffset += offset;
      pbuf->endoffset += offset;
      return(NETWIB_ERR_OK);
    }

    /* full overwrite left shift */
    if (offsetpos >= pbuf->endoffset) {
      pbuf->beginoffset = 0;
      pbuf->endoffset = 0;
      return(NETWIB_ERR_OK);
    }

    /* half overwrite left shift */
    netwib_c_memcpy(pbuf->totalptr,
                    pbuf->totalptr - offset,
                    pbuf->endoffset + offset);
    pbuf->beginoffset = 0;
    pbuf->endoffset += offset;
    return(NETWIB_ERR_OK);
  }

  offsetpos = (netwib_uint32)offset;

  /* simple right shift */
  if (offsetpos <= pbuf->totalsize - pbuf->endoffset) {
    netwib_c_memmove(pbuf->totalptr + pbuf->beginoffset + offset,
                     pbuf->totalptr + pbuf->beginoffset,
                     pbuf->endoffset - pbuf->beginoffset);
    pbuf->beginoffset += offset;
    pbuf->endoffset += offset;
    return(NETWIB_ERR_OK);
  }

  if (!netwib__priv_buf_cangrow(pbuf)) {
    /* will truncate */
    if (offsetpos > pbuf->totalsize - pbuf->beginoffset) {
      pbuf->beginoffset = pbuf->totalsize;
    } else {
      netwib_c_memmove(pbuf->totalptr + pbuf->beginoffset + offset,
                       pbuf->totalptr + pbuf->beginoffset,
                       pbuf->totalsize - pbuf->beginoffset - offset);
      pbuf->beginoffset += offset;
    }
    pbuf->endoffset = pbuf->totalsize;
    return(NETWIB_ERR_OK);
  }

  /* right shift with realloc */
  netwib_er(netwib_priv_buf_realloc(offset - pbuf->totalsize+pbuf->endoffset,
                                    pbuf));
  netwib_c_memmove(pbuf->totalptr + pbuf->beginoffset + offset,
                   pbuf->totalptr + pbuf->beginoffset,
                   pbuf->endoffset - pbuf->beginoffset);
  pbuf->beginoffset += offset;
  pbuf->endoffset += offset;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_data(netwib_constdata data,
                                  netwib_uint32 datasize,
                                  netwib_buf *pbuf)
{
  netwib_uint32 leftsize;

  /* parameters verification */
  if (data == NULL && datasize) {
    return(NETWIB_ERR_PANULLPTRSIZE);
  }

  /* simple case */
  if (pbuf == NULL) {
    /* nothing to append to nothing */
    return(NETWIB_ERR_OK);
  }
  if (pbuf->totalptr == NETWIB_PRIV_BUF_PTR_CLOSED)
    return(NETWIB_ERR_LOOBJUSECLOSED);
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_buf(pbuf->totalptr,
                                        netwib__priv_buf_get_leakid(pbuf)));
#endif
  if (datasize == 0) {
    return(NETWIB_ERR_OK);
  }

  /* append */
  leftsize = pbuf->totalsize - pbuf->endoffset;
  if (datasize <= leftsize) {
    /* if sufficient, only append data */
    netwib_c_memcpy(pbuf->totalptr+pbuf->endoffset, data, datasize);
    pbuf->endoffset += datasize;
    return(NETWIB_ERR_OK);
  }

  /* no space for data : check if we can slide */
  if (netwib__priv_buf_canslide(pbuf)) {
    leftsize += pbuf->beginoffset;
    if (datasize <= leftsize) {
      netwib__priv_buf_slide(pbuf);
      netwib_c_memcpy(pbuf->totalptr+pbuf->endoffset, data, datasize);
      pbuf->endoffset += datasize;
      return(NETWIB_ERR_OK);
    }
  }

  /* no space for data */
  if (!netwib__priv_buf_cangrow(pbuf)) {
    return(NETWIB_ERR_DATANOSPACE);
  }

  /* we need to realloc */
  if (netwib__priv_buf_canslide(pbuf)) {
    /* decide to slide even if it is not sufficient : on next call,
       we will no more slide (it's better to slide when there is a small
       amount of memory to copy) */
    netwib__priv_buf_slide(pbuf);
  }
  netwib_er(netwib_priv_buf_realloc(datasize - leftsize, pbuf));
  /* now, copy data */
  netwib_c_memcpy(pbuf->totalptr+pbuf->endoffset, data, datasize);
  pbuf->endoffset += datasize;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_string(netwib_conststring str,
                                    netwib_buf *pbuf)
{
  if (str != NULL && pbuf != NULL) {
    netwib_er(netwib_buf_append_data((netwib_constdata)str,
                                     netwib_c_strlen(str),
                                     pbuf));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_buf(netwib_constbuf *pbuftoappend,
                                 netwib_buf *pbuf)
{

  if (pbuftoappend == NULL || pbuf == NULL) {
    /* nothing to append */
    return(NETWIB_ERR_OK);
  }

  if (pbuftoappend->totalptr == NETWIB_PRIV_BUF_PTR_CLOSED) {
    return(NETWIB_ERR_LOOBJUSECLOSED);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_buf(pbuftoappend->totalptr,
                                        netwib__priv_buf_get_leakid(pbuftoappend)));
#endif
  netwib__buf_transfersensitive(pbuftoappend, pbuf);

  if (pbuftoappend->beginoffset < pbuftoappend->endoffset) {
    netwib_er(netwib_buf_append_data(pbuftoappend->totalptr +
                                     pbuftoappend->beginoffset,
                                     pbuftoappend->endoffset -
                                     pbuftoappend->beginoffset,
                                     pbuf));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_byte(netwib_byte b,
                                  netwib_buf *pbuf)
{
  netwib_uint32 leftsize;

  /* simple case */
  if (pbuf == NULL) {
    /* nothing to append */
    return(NETWIB_ERR_OK);
  }
  if (pbuf->totalptr == NETWIB_PRIV_BUF_PTR_CLOSED)
    return(NETWIB_ERR_LOOBJUSECLOSED);
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_buf(pbuf->totalptr,
                                        netwib__priv_buf_get_leakid(pbuf)));
#endif

  /* append */
  leftsize = pbuf->totalsize - pbuf->endoffset;
  if (leftsize > 0) {
    /* if sufficient, only append data */
    pbuf->totalptr[pbuf->endoffset] = b;
    pbuf->endoffset++;
    return(NETWIB_ERR_OK);
  }

  /* no space for data : check if we can slide */
  if (netwib__priv_buf_canslide(pbuf)) {
    if (pbuf->beginoffset) {
      netwib__priv_buf_slide(pbuf);
      pbuf->totalptr[pbuf->endoffset] = b;
      pbuf->endoffset++;
      return(NETWIB_ERR_OK);
    }
  }

  if (!netwib__priv_buf_cangrow(pbuf)) {
    /* there is no space */
    return(NETWIB_ERR_DATANOSPACE);
  }

  /* we need to realloc */
  if (netwib__priv_buf_canslide(pbuf)) {
    netwib__priv_buf_slide(pbuf);
  }
  netwib_er(netwib_priv_buf_realloc(1, pbuf));

  /* now, copy data */
  pbuf->totalptr[pbuf->endoffset] = b;
  pbuf->endoffset++;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_prepend_buf(netwib_constbuf *pbuftoprepend,
                                  netwib_buf *pbuf)
{
  netwib_uint32 buftoprependsize;

  /* simple case */
  if (pbuftoprepend == NULL || pbuf == NULL) {
    /* nothing to prepend */
    return(NETWIB_ERR_OK);
  }

  if (pbuf->totalptr == NETWIB_PRIV_BUF_PTR_CLOSED)
    return(NETWIB_ERR_LOOBJUSECLOSED);
  if (pbuftoprepend->totalptr == NETWIB_PRIV_BUF_PTR_CLOSED)
    return(NETWIB_ERR_LOOBJUSECLOSED);
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_buf(pbuf->totalptr,
                                        netwib__priv_buf_get_leakid(pbuf)));
  netwib_er(netwib_debug_leak_valid_buf(pbuftoprepend->totalptr,
                                        netwib__priv_buf_get_leakid(pbuftoprepend)));
#endif
  netwib__buf_transfersensitive(pbuftoprepend, pbuf);

  buftoprependsize = netwib__buf_ref_data_size(pbuftoprepend);
  if (buftoprependsize > pbuf->beginoffset) {
    /* need some shift */
    netwib_er(netwib_buf_shift(pbuf, buftoprependsize - pbuf->beginoffset,
                               NETWIB_FALSE));
  }
  pbuf->beginoffset -= buftoprependsize;
  netwib_c_memcpy(netwib__buf_ref_data_ptr(pbuf),
                 netwib__buf_ref_data_ptr(pbuftoprepend),
                 buftoprependsize);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_ref_string(netwib_buf *pbuf,
                                 netwib_string *pstr)
{
  netwib_char *pc, *pfirst;

  /* simple case */
  if (pbuf == NULL) {
    /* it's not possible to obtain info if this pointer is NULL */
    return(NETWIB_ERR_DATANOSPACE);
  }
  if (pbuf->totalptr == NETWIB_PRIV_BUF_PTR_CLOSED) {
    return(NETWIB_ERR_LOOBJUSECLOSED);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_buf(pbuf->totalptr,
                                        netwib__priv_buf_get_leakid(pbuf)));
#endif

  /* eventually add a '\0' */
  if (pbuf->endoffset < pbuf->totalsize) {
    /* there is room to add a '\0' */
    if (pbuf->totalptr[pbuf->endoffset] != '\0') {
      /* we do this check because external array might be constant string
         (so it cannot be set) */
      pbuf->totalptr[pbuf->endoffset] = '\0';
    }
  } else if (netwib__priv_buf_canslide(pbuf)) {
    /* buffer can be slided, slide it to add a '\0' */
    netwib__priv_buf_slide(pbuf);
    pbuf->totalptr[pbuf->endoffset] = '\0';
  } else if (netwib__priv_buf_cangrow(pbuf)) {
    /* buffer can be reallocated */
    netwib_er(netwib_priv_buf_realloc(1, pbuf));
    /* now, set zero */
    pbuf->totalptr[pbuf->endoffset] = '\0';
  } else if (netwib__buf_ref_data_size(pbuf)) {
    /* our last chance is to find a zero in the middle of the buffer */
    pfirst = (netwib_string)pbuf->totalptr + pbuf->beginoffset;
    pc = (netwib_string)pbuf->totalptr + pbuf->endoffset - 1;
    while (NETWIB_TRUE) {
      if (*pc == '\0') {
        /* we are lucky, there is one */
        break;
      }
      if (pc == pfirst) {
        break;
      }
      pc--;
    }
    if (*pc != '\0') {
      return(NETWIB_ERR_DATANOSPACE);
    }
  } else {
    /* we didn't found a zero, and there is no room to put it */
    return(NETWIB_ERR_DATANOSPACE);
  }

  if (pstr != NULL) {
    /* set output */
    *pstr = (netwib_string)netwib__buf_ref_data_ptr(pbuf);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_constbuf_ref_string(netwib_constbuf *pbuf,
                                      netwib_string *pstr)
{
  netwib_char *pc, *pfirst;

  /* simple case */
  if (pbuf == NULL) {
    /* it's not possible to obtain info if this pointer is NULL */
    return(NETWIB_ERR_DATANOSPACE);
  }
  if (pbuf->totalptr == NETWIB_PRIV_BUF_PTR_CLOSED) {
    return(NETWIB_ERR_LOOBJUSECLOSED);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_buf(pbuf->totalptr,
                                        netwib__priv_buf_get_leakid(pbuf)));
#endif

  /* check if there is already a '\0' */
  if (pbuf->endoffset < pbuf->totalsize &&
      pbuf->totalptr[pbuf->endoffset] == '\0') {
    /* ok */
  } else if (netwib__buf_ref_data_size(pbuf)) {
    /* our last chance is to find a zero in the middle of the buffer */
    pfirst = (netwib_string)pbuf->totalptr + pbuf->beginoffset;
    pc = (netwib_string)pbuf->totalptr + pbuf->endoffset - 1;
    while (NETWIB_TRUE) {
      if (*pc == '\0') {
        /* we are lucky, there is one */
        break;
      }
      if (pc == pfirst) {
        break;
      }
      pc--;
    }
    if (*pc != '\0') {
      return(NETWIB_ERR_DATANOSPACE);
    }
  } else {
    /* we didn't found a zero, and there is no room to put it */
    return(NETWIB_ERR_DATANOSPACE);
  }

  if (pstr != NULL) {
    /* set output */
    *pstr = (netwib_string)netwib__buf_ref_data_ptr(pbuf);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_wantspace(netwib_buf *pbuf,
                                netwib_uint32 wantedspace,
                                netwib_data *pdata)
{
  netwib_uint32 leftsize;

  /* simple case */
  if (pbuf == NULL) {
    /* it's not possible to obtain info if this pointer is NULL */
    return(NETWIB_ERR_PANULLPTR);
  }
  if (pbuf->totalptr == NETWIB_PRIV_BUF_PTR_CLOSED) {
    return(NETWIB_ERR_LOOBJUSECLOSED);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_buf(pbuf->totalptr,
                                        netwib__priv_buf_get_leakid(pbuf)));
#endif

  leftsize = pbuf->totalsize - pbuf->endoffset;
  if (wantedspace <= leftsize) {
    /* sufficient space */
    if (pdata != NULL) {
      *pdata = pbuf->totalptr + pbuf->endoffset;
    }
    return(NETWIB_ERR_OK);
  }

  /* no space for data : check if we can slide */
  if (netwib__priv_buf_canslide(pbuf)) {
    leftsize += pbuf->beginoffset;
    if (wantedspace <= leftsize) {
      netwib__priv_buf_slide(pbuf);
      if (pdata != NULL) {
        *pdata = pbuf->totalptr + pbuf->endoffset;
      }
      return(NETWIB_ERR_OK);
    }
  }

  if (!netwib__priv_buf_cangrow(pbuf)) {
    /* cannot realloc */
    return(NETWIB_ERR_DATANOSPACE);
  }

  /* realloc */
  if (netwib__priv_buf_canslide(pbuf)) {
    netwib__priv_buf_slide(pbuf);
  }
  netwib_er(netwib_priv_buf_realloc(wantedspace - leftsize, pbuf));

  if (pdata != NULL) {
    *pdata = pbuf->totalptr + pbuf->endoffset;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_wishspace(netwib_buf *pbuf,
                                netwib_uint32 wantedspace,
                                netwib_data *pdata,
                                netwib_uint32 *pobtainedspace)
{
  netwib_uint32 leftsize;

  /* simple case */
  if (pbuf == NULL) {
    /* it's not possible to obtain info if this pointer is NULL */
    return(NETWIB_ERR_PANULLPTR);
  }
  if (pbuf->totalptr == NETWIB_PRIV_BUF_PTR_CLOSED) {
    return(NETWIB_ERR_LOOBJUSECLOSED);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_buf(pbuf->totalptr,
                                      netwib__priv_buf_get_leakid(pbuf)));
#endif

  leftsize = pbuf->totalsize - pbuf->endoffset;
  if (wantedspace <= leftsize) {
    /* sufficient space */
    if (pdata != NULL) {
      *pdata = pbuf->totalptr + pbuf->endoffset;
    }
    if (pobtainedspace != NULL) *pobtainedspace = leftsize;
    return(NETWIB_ERR_OK);
  }

  /* slide if possible */
  if (netwib__priv_buf_canslide(pbuf)) {
    netwib__priv_buf_slide(pbuf);
  }

  if (!netwib__priv_buf_cangrow(pbuf)) {
    /* cannot realloc, so return current size */
    if (pdata != NULL) {
      *pdata = pbuf->totalptr + pbuf->endoffset;
    }
    if (pobtainedspace != NULL) *pobtainedspace = leftsize;
    return(NETWIB_ERR_OK);
  }

  /* realloc */
  netwib_er(netwib_priv_buf_realloc(wantedspace - leftsize, pbuf));

  if (pdata != NULL) {
    *pdata = pbuf->totalptr + pbuf->endoffset;
  }
  if (pobtainedspace != NULL) {
    *pobtainedspace = pbuf->totalsize - pbuf->endoffset;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_cmp(netwib_constbuf *pbuf1,
                          netwib_constbuf *pbuf2,
                          netwib_cmp *pcmp)
{
  netwib_uint32 data1size, data2size, commonsize;
  netwib_cmp cmp=NETWIB_CMP_LT;
  int reti;

  /* parameter verification */
  if (pbuf1 != NULL) {
    if (pbuf1->totalptr == NETWIB_PRIV_BUF_PTR_CLOSED) {
      return(NETWIB_ERR_LOOBJUSECLOSED);
    }
#if NETWIB_DEBUG_LEAK==1
    netwib_er(netwib_debug_leak_valid_buf(pbuf1->totalptr,
                                          netwib__priv_buf_get_leakid(pbuf1)));
#endif
  }
  if (pbuf2 != NULL) {
    if (pbuf2->totalptr == NETWIB_PRIV_BUF_PTR_CLOSED) {
      return(NETWIB_ERR_LOOBJUSECLOSED);
    }
#if NETWIB_DEBUG_LEAK==1
    netwib_er(netwib_debug_leak_valid_buf(pbuf2->totalptr,
                                          netwib__priv_buf_get_leakid(pbuf2)));
#endif
  }

  data1size = netwib__buf_ref_data_sizenull(pbuf1);
  data2size = netwib__buf_ref_data_sizenull(pbuf2);
  if (data1size == 0 && data2size == 0) {
    cmp = NETWIB_CMP_EQ;
  } else if (data1size == 0) {
    cmp = NETWIB_CMP_LT;
  } else if (data2size == 0) {
    cmp = NETWIB_CMP_GT;
  } else {
    commonsize = (data1size<data2size)?data1size:data2size;
    reti = netwib_c_memcmp(pbuf1->totalptr+pbuf1->beginoffset,
                           pbuf2->totalptr+pbuf2->beginoffset, commonsize);
    if (reti == 0) {
      if (data1size == data2size) {
        cmp = NETWIB_CMP_EQ;
      } else {
        cmp = (data1size<data2size)?NETWIB_CMP_LT:NETWIB_CMP_GT;
      }
    } else if (reti < 0) {
      cmp = NETWIB_CMP_LT;
    } else {
      cmp = NETWIB_CMP_GT;
    }
  }

  if (pcmp != NULL) *pcmp = cmp;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_casecmp(netwib_constbuf *pbuf1,
                              netwib_constbuf *pbuf2,
                              netwib_cmp *pcmp)
{
  netwib_uint32 data1size, data2size, commonsize;
  netwib_cmp cmp=NETWIB_CMP_LT;
  int reti;

  /* parameter verification */
  if (pbuf1 != NULL) {
    if (pbuf1->totalptr == NETWIB_PRIV_BUF_PTR_CLOSED) {
      return(NETWIB_ERR_LOOBJUSECLOSED);
    }
#if NETWIB_DEBUG_LEAK==1
    netwib_er(netwib_debug_leak_valid_buf(pbuf1->totalptr,
                                          netwib__priv_buf_get_leakid(pbuf1)));
#endif
  }
  if (pbuf2 != NULL) {
    if (pbuf2->totalptr == NETWIB_PRIV_BUF_PTR_CLOSED) {
      return(NETWIB_ERR_LOOBJUSECLOSED);
    }
#if NETWIB_DEBUG_LEAK==1
    netwib_er(netwib_debug_leak_valid_buf(pbuf2->totalptr,
                                          netwib__priv_buf_get_leakid(pbuf2)));
#endif
  }

  data1size = netwib__buf_ref_data_sizenull(pbuf1);
  data2size = netwib__buf_ref_data_sizenull(pbuf2);
  if (data1size == 0 && data2size == 0) {
    cmp = NETWIB_CMP_EQ;
  } else if (data1size == 0) {
    cmp = NETWIB_CMP_LT;
  } else if (data2size == 0) {
    cmp = NETWIB_CMP_GT;
  } else {
    commonsize = (data1size<data2size)?data1size:data2size;
    reti = netwib_c_memcasecmp(pbuf1->totalptr+pbuf1->beginoffset,
                               pbuf2->totalptr+pbuf2->beginoffset, commonsize);
    if (reti == 0) {
      if (data1size == data2size) {
        cmp = NETWIB_CMP_EQ;
      } else {
        cmp = (data1size<data2size)?NETWIB_CMP_LT:NETWIB_CMP_GT;
      }
    } else if (reti < 0) {
      cmp = NETWIB_CMP_LT;
    } else {
      cmp = NETWIB_CMP_GT;
    }
  }

  if (pcmp != NULL) *pcmp = cmp;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_cmp_string(netwib_constbuf *pbuf1,
                                 netwib_conststring string2,
                                 netwib_cmp *pcmp)
{
  netwib_conststring ps;
  netwib_data data;
  netwib_uint32 datasize;
  netwib_char c1, c2=0;
  netwib_cmp cmp=NETWIB_CMP_LT;

  /* parameter verification */
  if (pbuf1 != NULL) {
    if (pbuf1->totalptr == NETWIB_PRIV_BUF_PTR_CLOSED) {
      return(NETWIB_ERR_LOOBJUSECLOSED);
    }
#if NETWIB_DEBUG_LEAK==1
    netwib_er(netwib_debug_leak_valid_buf(pbuf1->totalptr,
                                          netwib__priv_buf_get_leakid(pbuf1)));
#endif
  }

  datasize = netwib__buf_ref_data_sizenull(pbuf1);
  data = datasize?netwib__buf_ref_data_ptr(pbuf1):NULL;
  ps = string2;

  while (NETWIB_TRUE) {
    if (string2 == NULL) {
      c2 = '\0';
    } else {
      c2 = *ps++;
    }
    if (c2 == '\0') {
      if (datasize == 0) {
        cmp = NETWIB_CMP_EQ;
      } else {
        cmp = NETWIB_CMP_GT;
      }
      break;
    }
    if (datasize == 0) {
      cmp = NETWIB_CMP_LT;
      break;
    }
    c1 = *data++;
    datasize--;
    if (c1 > c2) {
      cmp = NETWIB_CMP_GT;
      break;
    } else if (c1 < c2) {
      cmp = NETWIB_CMP_LT;
      break;
    }
  }

  if (pcmp != NULL) *pcmp = cmp;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_casecmp_string(netwib_constbuf *pbuf1,
                                     netwib_conststring string2,
                                     netwib_cmp *pcmp)
{
  netwib_conststring ps;
  netwib_data data;
  netwib_uint32 datasize;
  netwib_char c1, c2=0;
  netwib_cmp cmp=NETWIB_CMP_LT;

  /* parameter verification */
  if (pbuf1 != NULL) {
    if (pbuf1->totalptr == NETWIB_PRIV_BUF_PTR_CLOSED) {
      return(NETWIB_ERR_LOOBJUSECLOSED);
    }
#if NETWIB_DEBUG_LEAK==1
    netwib_er(netwib_debug_leak_valid_buf(pbuf1->totalptr,
                                          netwib__priv_buf_get_leakid(pbuf1)));
#endif
  }

  datasize = netwib__buf_ref_data_sizenull(pbuf1);
  data = datasize?netwib__buf_ref_data_ptr(pbuf1):NULL;
  ps = string2;

  while (NETWIB_TRUE) {
    if (string2 == NULL) {
      c2 = '\0';
    } else {
      c2 = *ps++;
    }
    if (c2 == '\0') {
      if (datasize == 0) {
        cmp = NETWIB_CMP_EQ;
      } else {
        cmp = NETWIB_CMP_GT;
      }
      break;
    }
    if (datasize == 0) {
      cmp = NETWIB_CMP_LT;
      break;
    }
    c1 = *data++;
    datasize--;
    netwib_c2_lower(c2);
    netwib_c2_lower(c1);
    if (c1 > c2) {
      cmp = NETWIB_CMP_GT;
      break;
    } else if (c1 < c2) {
      cmp = NETWIB_CMP_LT;
      break;
    }
  }

  if (pcmp != NULL) *pcmp = cmp;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_close(netwib_buf *pbuf)
{
  /* simple case */
  if (pbuf == NULL) {
    /* nothing to close */
    return(NETWIB_ERR_OK);
  }

  if (pbuf->totalptr == NETWIB_PRIV_BUF_PTR_CLOSED) {
    return(NETWIB_ERR_LOOBJCLOSEALREADYCLOSED);
  }

  netwib_er(netwib_priv_buf_wipe(pbuf));

  if (pbuf->flags&NETWIB_BUF_FLAGS_ALLOC) {
#if NETWIB_DEBUG_LEAK==1
    netwib_er(netwib_debug_leak_del_buf(pbuf->totalptr,
                                        netwib__priv_buf_get_leakid(pbuf)));
#endif
    netwib_er(netwib_ptr_free((netwib_ptr*)&pbuf->totalptr));
  }

  pbuf->totalptr = NETWIB_PRIV_BUF_PTR_CLOSED;

  return(NETWIB_ERR_OK);
}

