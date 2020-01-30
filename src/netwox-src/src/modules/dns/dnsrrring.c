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
netwib_err netwox_dnsrrring_item_create(netwib_ptr *ppitem)
{
  netwox_dnsrr *pdnsrr;
  netwib_ptr ptr;

  netwib_er(netwib_ptr_malloc(sizeof(netwox_dnsrr), &ptr));
  pdnsrr = (netwox_dnsrr *)ptr;
  *ppitem = ptr;

  netwib_er(netwox_dnsrr_init(pdnsrr));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrrring_item_erase(netwib_ptr pitem)
{
  netwox_dnsrr *pdnsrr;

  pdnsrr = (netwox_dnsrr *)pitem;

  netwib_er(netwox_dnsrr_close(pdnsrr));

  netwib_er(netwib_ptr_free(&pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnsrrring_item_duplicate(netwib_constptr pitem,
                                           netwib_ptr *pdupofitem)
{
  const netwox_dnsrr *pdnsrr = (const netwox_dnsrr *)pitem;
  netwox_dnsrr *pdnsrrdup;
  netwib_ptr ptr;

  netwib_er(netwib_ptr_malloc(sizeof(netwox_dnsrr), &ptr));
  pdnsrrdup = (netwox_dnsrr *)ptr;
  *pdupofitem = ptr;

  pdnsrrdup->kind = pdnsrr->kind;
  netwib_er(netwib_buf_append_buf(&pdnsrr->name, &pdnsrrdup->name));
  pdnsrrdup->type = pdnsrr->type;
  pdnsrrdup->rclass = pdnsrr->rclass;
  pdnsrrdup->ttl = pdnsrr->ttl;
  netwib_er(netwib_buf_append_buf(&pdnsrr->rdata, &pdnsrrdup->rdata));

  return(NETWIB_ERR_OK);
}
