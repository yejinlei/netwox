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
typedef struct {
  netwib_io *pnextio;
  netwib_uint32 fragdatasize;
  netwib_uint32 fragtotalsize;
  netwib_bool display;
  netwib_buf bufipfrag;
} netwox_spooffrag;

/*-------------------------------------------------------------*/
static netwib_err netwox_spooffrag_init2(netwib_spoof_ip_inittype inittype,
                                         netwib_uint32 fragdatasize,
                                         netwib_uint32 fragtotalsize,
                                         netwib_bool display,
                                         netwib_bool *preadinitialized,
                                         netwib_bool *pwriteinitialized,
                                         netwox_spooffrag *ptr)
{

  /* initialize structures */
  netwib_er(netwib_io_init_spoof_ip(inittype, &ptr->pnextio));
  ptr->fragdatasize = fragdatasize;
  ptr->fragtotalsize = fragtotalsize;
  ptr->display = display;
  netwib_er(netwib_buf_init_mallocdefault(&ptr->bufipfrag));

  /* set io values */
  *preadinitialized = NETWIB_FALSE;
  *pwriteinitialized = NETWIB_TRUE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_spooffrag_close(netwox_spooffrag *ptr)
{

  /* close structures */
  netwib_er(netwib_buf_close(&ptr->bufipfrag));
  netwib_er(netwib_io_close(&ptr->pnextio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_spooffrag_write_frag(netwib_constbuf *pfrag,
                                              netwib_ptr infos)
{
  netwox_spooffrag *psf = (netwox_spooffrag *)infos;

  if (psf->display) {
    /* display to screen */
    netwib_er(netwib_pkt_ip_display(pfrag, NULL, NETWIB_ENCODETYPE_ARRAY,
                                    NETWIB_ENCODETYPE_DUMP));
  }

  netwib_er(netwib_io_write(psf->pnextio, pfrag));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_spooffrag_write(netwib_io *pio,
                                         netwib_constbuf *pbuf)
{
  netwox_spooffrag *ptr = (netwox_spooffrag *)pio->pcommon;

  netwib_er(netwib_ip_frag(pbuf, ptr->fragdatasize, ptr->fragtotalsize,
                           &netwox_spooffrag_write_frag, ptr));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_spooffrag_fclose(netwib_io *pio)
{
  netwox_spooffrag *ptr = (netwox_spooffrag *)pio->pcommon;
  netwib_er(netwox_spooffrag_close(ptr));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_spooffrag_init(netwib_spoof_ip_inittype inittype,
                                 netwib_uint32 fragdatasize,
                                 netwib_uint32 fragtotalsize,
                                 netwib_bool display,
                                 netwib_io **ppio)
{
  netwib_bool rdinit, wrinit;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwox_spooffrag), &pcommon));
  ret = netwox_spooffrag_init2(inittype, fragdatasize, fragtotalsize, display,
                               &rdinit, &wrinit,
                               (netwox_spooffrag *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           NULL, /* read */
                           &netwox_spooffrag_write,
                           NULL, /* wait */
                           NULL, /* unread */
                           NULL, /* set */
                           NULL, /* get */
                           &netwox_spooffrag_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}
