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
/*
  ___
  | |--[array of HORIZONTAL_SIZE buffers (netwib_bufpool_item)]
  | |--[array of HORIZONTAL_SIZE netwib_bufpool_item]
  | |--[array of HORIZONTAL_SIZE netwib_bufpool_item]
  . .
  . .

 Notes :
  - Horizontal array contains buffers (netwib_bufpool_horizontalitem). They
    are allocated once. When filled, a new horizontal array is
    allocated and added in vertical array.
  - Vertical array contains addresses of horizontal arrays
    (netwib_bufpool_verticalitem). It is
    reallocated to add more horizontal arrays.
*/

/*-------------------------------------------------------------*/
#define NETWIB_BUFPOOL_HORIZONTAL_SIZE 256
#define NETWIB_BUFPOOL_VERTICAL_ALLOCSTEP 2

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bool inuse;
  netwib_buf buf;
} netwib_bufpool_horizontalitem;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bufpool_horizontalitem *horizontalarray;
  netwib_uint32 numinit; /* number of buffers initialized in this array */
} netwib_bufpool_verticalitem;

/*-------------------------------------------------------------*/
struct netwib_bufpool {
  netwib_bufpool_verticalitem *verticalarray;
  netwib_uint32 verticalarraysize; /* not in bytes, but in number
                                      of horizontal arrays */
  netwib_uint32 startsearchat_vert;
  netwib_uint32 startsearchat_hori;
  netwib_bool mt;
  netwib_thread_mutex *pmtmutex;
};

/*-------------------------------------------------------------*/
netwib_err netwib_bufpool_init(netwib_bool mt,
                               netwib_bufpool **ppbufpool)
{
  netwib_bufpool *pbufpool;
  netwib_uint32 i;

  /* allocate needed memory to store pbufpool */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_bufpool), (netwib_ptr*)&pbufpool));
  *ppbufpool = pbufpool;

  /* set values */
  pbufpool->verticalarraysize = NETWIB_BUFPOOL_VERTICAL_ALLOCSTEP;
  netwib_er(netwib_ptr_malloc(pbufpool->verticalarraysize*sizeof(netwib_bufpool_verticalitem), (netwib_ptr*)&pbufpool->verticalarray));
  pbufpool->startsearchat_vert = 0;
  pbufpool->startsearchat_hori = 0;
  for (i = 0; i < NETWIB_BUFPOOL_VERTICAL_ALLOCSTEP; i++) {
    netwib_er(netwib_ptr_malloc(NETWIB_BUFPOOL_HORIZONTAL_SIZE*sizeof(netwib_bufpool_horizontalitem), (netwib_ptr*)&pbufpool->verticalarray[i].horizontalarray));
    pbufpool->verticalarray[i].numinit = 0;
  }

  /* for multithread */
  pbufpool->mt = mt;
  if (mt) {
    netwib_er(netwib_thread_mutex_init(&pbufpool->pmtmutex));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_bufpool_close(netwib_bufpool **ppbufpool)
{
  netwib_bufpool *pbufpool;
  netwib_uint32 i, j;

  pbufpool = *ppbufpool;

  for (i = 0; i < pbufpool->verticalarraysize; i++) {
    for (j = 0; j < pbufpool->verticalarray[i].numinit; j++) {
#if NETWIB_DEBUG_LEAK==1
      if (pbufpool->verticalarray[i].horizontalarray[j].inuse) {
        netwib_er(netwib_debug_disp_fmt("/!\\/!\\ LEAK : BUFPOOL : %{uint32} %{uint32}\n", i, j));
        /* do not close to see buffer content */
      } else {
        netwib_er(netwib_buf_close(&pbufpool->verticalarray[i].horizontalarray[j].buf));
      }
#else
      netwib_er(netwib_buf_close(&pbufpool->verticalarray[i].horizontalarray[j].buf));
#endif
    }
    netwib_er(netwib_ptr_free((netwib_ptr*)&pbufpool->verticalarray[i].horizontalarray));
  }

  if (pbufpool->mt) {
    netwib_er(netwib_thread_mutex_close(&pbufpool->pmtmutex));
  }
  netwib_er(netwib_ptr_free((netwib_ptr*)&pbufpool->verticalarray));
  netwib_er(netwib_ptr_free((netwib_ptr*)&pbufpool));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_bufpool_buf_init(netwib_bufpool *pbufpool,
                                   netwib_buf **ppbuf)
{
  netwib_bool found;
  netwib_uint32 i, j, newverticalarraysize;

  if (pbufpool->mt) {
    netwib_er(netwib_thread_mutex_lock(pbufpool->pmtmutex,
                                       NETWIB_TIME_INFINITE, NULL));
  }

  /* search in available arrays */
  i = pbufpool->startsearchat_vert;
  j = pbufpool->startsearchat_hori;
  found = NETWIB_FALSE;
  for (; i < pbufpool->verticalarraysize; i++) {
    for (; j < NETWIB_BUFPOOL_HORIZONTAL_SIZE; j++) {
      if (j == pbufpool->verticalarray[i].numinit) {
        netwib_er(netwib_buf_init_mallocdefault(&pbufpool->verticalarray[i].horizontalarray[j].buf));
        pbufpool->verticalarray[i].numinit++;
        found = NETWIB_TRUE;
        break;
      }
      if (!pbufpool->verticalarray[i].horizontalarray[j].inuse) {
        found = NETWIB_TRUE;
        break;
      }
    }
    if (found) {
      break;
    }
    j = 0;
  }

  /* need more space */
  if (!found) {
    newverticalarraysize = pbufpool->verticalarraysize + NETWIB_BUFPOOL_VERTICAL_ALLOCSTEP;
    netwib_er(netwib_ptr_realloc(newverticalarraysize*sizeof(netwib_bufpool_verticalitem), (netwib_ptr*)&pbufpool->verticalarray));
    for (i = pbufpool->verticalarraysize; i < newverticalarraysize; i++) {
      netwib_er(netwib_ptr_malloc(NETWIB_BUFPOOL_HORIZONTAL_SIZE*sizeof(netwib_bufpool_horizontalitem), (netwib_ptr*)&pbufpool->verticalarray[i].horizontalarray));
      pbufpool->verticalarray[i].numinit = 0;
    }
    i = pbufpool->verticalarraysize;
    j = 0;
    netwib_er(netwib_buf_init_mallocdefault(&pbufpool->verticalarray[i].horizontalarray[j].buf));
    pbufpool->verticalarray[i].numinit++;
    pbufpool->verticalarraysize = newverticalarraysize;
  }

  /* found */
  *ppbuf = &pbufpool->verticalarray[i].horizontalarray[j].buf;
  netwib__buf_reinit(*ppbuf);
  pbufpool->verticalarray[i].horizontalarray[j].inuse = NETWIB_TRUE;
  pbufpool->startsearchat_vert = i;
  pbufpool->startsearchat_hori = j + 1;

  if (pbufpool->mt) {
    netwib_er(netwib_thread_mutex_unlock(pbufpool->pmtmutex));
  }

  return(NETWIB_ERR_OK);
}
/*-------------------------------------------------------------*/
netwib_err netwib_bufpool_buf_close(netwib_bufpool *pbufpool,
                                    netwib_buf **ppbuf)
{
  netwib_uint32 i, j;
  netwib_err ret;

  if (pbufpool->mt) {
    netwib_er(netwib_thread_mutex_lock(pbufpool->pmtmutex,
                                       NETWIB_TIME_INFINITE, NULL));
  }

  ret = NETWIB_ERR_LOOBJCLOSENOTINITIALIZED;
  for (i = 0; i < pbufpool->verticalarraysize; i++) {
    for (j = 0; j < pbufpool->verticalarray[i].numinit; j++) {
      if (pbufpool->verticalarray[i].horizontalarray[j].inuse) {
        if (*ppbuf == &pbufpool->verticalarray[i].horizontalarray[j].buf) {
          ret = netwib_priv_buf_wipe(*ppbuf);
          if (ret == NETWIB_ERR_OK) {
            netwib__buf_reinit(*ppbuf);
            pbufpool->verticalarray[i].horizontalarray[j].inuse = NETWIB_FALSE;
            *ppbuf = NULL;
            if (i < pbufpool->startsearchat_vert) {
              pbufpool->startsearchat_vert = i;
              pbufpool->startsearchat_hori = j;
            } else if (i == pbufpool->startsearchat_vert &&
                       j < pbufpool->startsearchat_hori) {
              pbufpool->startsearchat_hori = j;
            }
            break;
          }
        }
      }
    }
    if (ret == NETWIB_ERR_OK) {
      break;
    }
  }

  if (pbufpool->mt) {
    netwib_er(netwib_thread_mutex_unlock(pbufpool->pmtmutex));
  }

  return(ret);
}
