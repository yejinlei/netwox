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
netwib_err netwox_dnspktex_answer_a(netwib_uint16 id,
                                    netwib_bool authanswer,
                                    netwib_bool recurdesired,
                                    netwib_bool recuravail,
                                    netwib_uint32 ttl,
                                    netwib_constbuf *phostname,
                                    netwib_constip *phostnameip,
                                    netwib_constbuf *pauthns,
                                    netwib_constip *pauthnsip,
                                    netwib_buf *ppkt)
{
  netwox_dnshdr dnsheader;
  netwib_bufpool *pbufpool;
  netwib_ptr pitem;
  netwox_dnsrr *pdnsrr;
  netwib_ring *prrring;

  /* init header */
  netwib_er(netwox_dnshdr_initdefault(&dnsheader));
  dnsheader.id = id;
  dnsheader.isaresponse = NETWIB_TRUE;
  dnsheader.authanswer = authanswer;
  dnsheader.recurdesired = recurdesired;
  dnsheader.recuravail = recuravail;

  /* init rrs */
  netwib_er(netwox_dnsrrring_init(&prrring));
  /**/
  netwib_er(netwox_dnsrrring_item_create(&pitem));
  pdnsrr = (netwox_dnsrr *)pitem;
  netwib_er(netwox_dnsrr_set_question(pdnsrr, phostname,
                                      NETWOX_DNSRR_TYPE_A,
                                      NETWOX_DNSRR_CLASS_IN));
  netwib_er(netwib_ring_add_last(prrring, pitem));
  /**/
  netwib_er(netwox_dnsrrring_item_create(&pitem));
  pdnsrr = (netwox_dnsrr *)pitem;
  netwib_er(netwox_dnsrr_set_answer(pdnsrr, phostname,
                                    NETWOX_DNSRR_TYPE_UNKNOWN, /* set below */
                                    NETWOX_DNSRR_CLASS_IN, ttl));
  netwib_er(netwox_dnsrdata_init_a(phostnameip, &pdnsrr->type,
                                   &pdnsrr->rdata));
  netwib_er(netwib_ring_add_last(prrring, pitem));
  /**/
  netwib_er(netwox_dnsrrring_item_create(&pitem));
  pdnsrr = (netwox_dnsrr *)pitem;
  netwib_er(netwox_dnsrr_set_authority(pdnsrr, pauthns,
                                       NETWOX_DNSRR_TYPE_NS,
                                       NETWOX_DNSRR_CLASS_IN, ttl));
  netwib_er(netwox_dnsrdata_init_ns(pauthns, &pdnsrr->rdata));
  netwib_er(netwib_ring_add_last(prrring, pitem));
  /**/
  netwib_er(netwox_dnsrrring_item_create(&pitem));
  pdnsrr = (netwox_dnsrr *)pitem;
  netwib_er(netwox_dnsrr_set_additional(pdnsrr, pauthns,
                                        NETWOX_DNSRR_TYPE_UNKNOWN,/*set below*/
                                        NETWOX_DNSRR_CLASS_IN, ttl));
  netwib_er(netwox_dnsrdata_init_a(pauthnsip, &pdnsrr->type,
                                   &pdnsrr->rdata));
  netwib_er(netwib_ring_add_last(prrring, pitem));

  /* init packet */
  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  netwib_er(netwox_pkt_append_dns(pbufpool, &dnsheader, prrring, ppkt));
  netwib_er(netwib_bufpool_close(&pbufpool));

  netwib_er(netwox_dnsrrring_close(&prrring));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnspktex_answer_ns(netwib_uint16 id,
                                     netwib_bool authanswer,
                                     netwib_bool recurdesired,
                                     netwib_bool recuravail,
                                     netwib_uint32 ttl,
                                     netwib_constbuf *pdomain,
                                     netwib_constbuf *pauthns,
                                     netwib_constip *pauthnsip,
                                     netwib_buf *ppkt)
{
  netwox_dnshdr dnsheader;
  netwib_bufpool *pbufpool;
  netwib_ptr pitem;
  netwox_dnsrr *pdnsrr;
  netwib_ring *prrring;

  /* init header */
  netwib_er(netwox_dnshdr_initdefault(&dnsheader));
  dnsheader.id = id;
  dnsheader.isaresponse = NETWIB_TRUE;
  dnsheader.authanswer = authanswer;
  dnsheader.recurdesired = recurdesired;
  dnsheader.recuravail = recuravail;

  /* init rrs */
  netwib_er(netwox_dnsrrring_init(&prrring));
  /**/
  netwib_er(netwox_dnsrrring_item_create(&pitem));
  pdnsrr = (netwox_dnsrr *)pitem;
  netwib_er(netwox_dnsrr_set_question(pdnsrr, pdomain,
                                      NETWOX_DNSRR_TYPE_NS,
                                      NETWOX_DNSRR_CLASS_IN));
  netwib_er(netwib_ring_add_last(prrring, pitem));
  /**/
  netwib_er(netwox_dnsrrring_item_create(&pitem));
  pdnsrr = (netwox_dnsrr *)pitem;
  netwib_er(netwox_dnsrr_set_answer(pdnsrr, pdomain,
                                    NETWOX_DNSRR_TYPE_NS,
                                    NETWOX_DNSRR_CLASS_IN, ttl));
  netwib_er(netwox_dnsrdata_init_ns(pauthns, &pdnsrr->rdata));
  netwib_er(netwib_ring_add_last(prrring, pitem));
  /**/
  netwib_er(netwox_dnsrrring_item_create(&pitem));
  pdnsrr = (netwox_dnsrr *)pitem;
  netwib_er(netwox_dnsrr_set_additional(pdnsrr, pauthns,
                                        NETWOX_DNSRR_TYPE_UNKNOWN,/*set below*/
                                        NETWOX_DNSRR_CLASS_IN, ttl));
  netwib_er(netwox_dnsrdata_init_a(pauthnsip, &pdnsrr->type,
                                   &pdnsrr->rdata));
  netwib_er(netwib_ring_add_last(prrring, pitem));

  /* init packet */
  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  netwib_er(netwox_pkt_append_dns(pbufpool, &dnsheader, prrring, ppkt));
  netwib_er(netwib_bufpool_close(&pbufpool));

  netwib_er(netwox_dnsrrring_close(&prrring));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnspktex_answer_ptr(netwib_uint16 id,
                                      netwib_bool authanswer,
                                      netwib_bool recurdesired,
                                      netwib_bool recuravail,
                                      netwib_uint32 ttl,
                                      netwib_constbuf *pip,
                                      netwib_constbuf *phostname,
                                      netwib_constbuf *pauthns,
                                      netwib_constip *pauthnsip,
                                      netwib_buf *ppkt)
{
  netwox_dnshdr dnsheader;
  netwib_bufpool *pbufpool;
  netwib_ptr pitem;
  netwox_dnsrr *pdnsrr;
  netwib_ring *prrring;

  /* init header */
  netwib_er(netwox_dnshdr_initdefault(&dnsheader));
  dnsheader.id = id;
  dnsheader.isaresponse = NETWIB_TRUE;
  dnsheader.authanswer = authanswer;
  dnsheader.recurdesired = recurdesired;
  dnsheader.recuravail = recuravail;

  /* init rrs */
  netwib_er(netwox_dnsrrring_init(&prrring));
  /**/
  netwib_er(netwox_dnsrrring_item_create(&pitem));
  pdnsrr = (netwox_dnsrr *)pitem;
  netwib_er(netwox_dnsrr_set_question(pdnsrr, pip,
                                      NETWOX_DNSRR_TYPE_PTR,
                                      NETWOX_DNSRR_CLASS_IN));
  netwib_er(netwib_ring_add_last(prrring, pitem));
  /**/
  netwib_er(netwox_dnsrrring_item_create(&pitem));
  pdnsrr = (netwox_dnsrr *)pitem;
  netwib_er(netwox_dnsrr_set_answer(pdnsrr, pip,
                                    NETWOX_DNSRR_TYPE_PTR,
                                    NETWOX_DNSRR_CLASS_IN, ttl));
  netwib_er(netwox_dnsrdata_init_ptr(phostname, &pdnsrr->rdata));
  netwib_er(netwib_ring_add_last(prrring, pitem));
  /**/
  netwib_er(netwox_dnsrrring_item_create(&pitem));
  pdnsrr = (netwox_dnsrr *)pitem;
  netwib_er(netwox_dnsrr_set_authority(pdnsrr, pauthns,
                                       NETWOX_DNSRR_TYPE_NS,
                                       NETWOX_DNSRR_CLASS_IN, ttl));
  netwib_er(netwox_dnsrdata_init_ns(pauthns, &pdnsrr->rdata));
  netwib_er(netwib_ring_add_last(prrring, pitem));
  /**/
  netwib_er(netwox_dnsrrring_item_create(&pitem));
  pdnsrr = (netwox_dnsrr *)pitem;
  netwib_er(netwox_dnsrr_set_additional(pdnsrr, pauthns,
                                        NETWOX_DNSRR_TYPE_UNKNOWN,/*set below*/
                                        NETWOX_DNSRR_CLASS_IN, ttl));
  netwib_er(netwox_dnsrdata_init_a(pauthnsip, &pdnsrr->type,
                                   &pdnsrr->rdata));
  netwib_er(netwib_ring_add_last(prrring, pitem));

  /* init packet */
  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  netwib_er(netwox_pkt_append_dns(pbufpool, &dnsheader, prrring, ppkt));
  netwib_er(netwib_bufpool_close(&pbufpool));

  netwib_er(netwox_dnsrrring_close(&prrring));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnspktex_answer_mx(netwib_uint16 id,
                                     netwib_bool authanswer,
                                     netwib_bool recurdesired,
                                     netwib_bool recuravail,
                                     netwib_uint32 ttl,
                                     netwib_constbuf *pdomain,
                                     netwib_uint16 preference,
                                     netwib_constbuf *pexchange,
                                     netwib_constbuf *pauthns,
                                     netwib_constip *pauthnsip,
                                     netwib_buf *ppkt)
{
  netwox_dnshdr dnsheader;
  netwib_bufpool *pbufpool;
  netwib_ptr pitem;
  netwox_dnsrr *pdnsrr;
  netwib_ring *prrring;

  /* init header */
  netwib_er(netwox_dnshdr_initdefault(&dnsheader));
  dnsheader.id = id;
  dnsheader.isaresponse = NETWIB_TRUE;
  dnsheader.authanswer = authanswer;
  dnsheader.recurdesired = recurdesired;
  dnsheader.recuravail = recuravail;

  /* init rrs */
  netwib_er(netwox_dnsrrring_init(&prrring));
  /**/
  netwib_er(netwox_dnsrrring_item_create(&pitem));
  pdnsrr = (netwox_dnsrr *)pitem;
  netwib_er(netwox_dnsrr_set_question(pdnsrr, pdomain,
                                      NETWOX_DNSRR_TYPE_MX,
                                      NETWOX_DNSRR_CLASS_IN));
  netwib_er(netwib_ring_add_last(prrring, pitem));
  /**/
  netwib_er(netwox_dnsrrring_item_create(&pitem));
  pdnsrr = (netwox_dnsrr *)pitem;
  netwib_er(netwox_dnsrr_set_answer(pdnsrr, pdomain,
                                    NETWOX_DNSRR_TYPE_MX,
                                    NETWOX_DNSRR_CLASS_IN, ttl));
  netwib_er(netwox_dnsrdata_init_mx(preference, pexchange, &pdnsrr->rdata));
  netwib_er(netwib_ring_add_last(prrring, pitem));
  /**/
  netwib_er(netwox_dnsrrring_item_create(&pitem));
  pdnsrr = (netwox_dnsrr *)pitem;
  netwib_er(netwox_dnsrr_set_authority(pdnsrr, pauthns,
                                       NETWOX_DNSRR_TYPE_NS,
                                       NETWOX_DNSRR_CLASS_IN, ttl));
  netwib_er(netwox_dnsrdata_init_ns(pauthns, &pdnsrr->rdata));
  netwib_er(netwib_ring_add_last(prrring, pitem));
  /**/
  netwib_er(netwox_dnsrrring_item_create(&pitem));
  pdnsrr = (netwox_dnsrr *)pitem;
  netwib_er(netwox_dnsrr_set_additional(pdnsrr, pauthns,
                                        NETWOX_DNSRR_TYPE_UNKNOWN,/*set below*/
                                        NETWOX_DNSRR_CLASS_IN, ttl));
  netwib_er(netwox_dnsrdata_init_a(pauthnsip, &pdnsrr->type,
                                   &pdnsrr->rdata));
  netwib_er(netwib_ring_add_last(prrring, pitem));

  /* init packet */
  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  netwib_er(netwox_pkt_append_dns(pbufpool, &dnsheader, prrring, ppkt));
  netwib_er(netwib_bufpool_close(&pbufpool));

  netwib_er(netwox_dnsrrring_close(&prrring));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnspktex_answer_unimplemented(netwox_constdnshdr *pdnshdr,
                                                netwib_constbuf *pname,
                                                netwox_dnsrr_type type,
                                                netwox_dnsrr_class rclass,
                                                netwib_buf *ppkt)
{
  netwox_dnshdr dnsheader;
  netwib_bufpool *pbufpool;
  netwib_ptr pitem;
  netwox_dnsrr *pdnsrr;
  netwib_ring *prrring;

  /* init header */
  dnsheader = *pdnshdr;
  dnsheader.rcode = NETWOX_DNSHDR_RCODE_UNIMPLEMENTED;
  dnsheader.isaresponse = NETWIB_TRUE;
  dnsheader.authanswer = NETWIB_TRUE;
  dnsheader.recuravail = dnsheader.recurdesired;

  /* init rr */
  netwib_er(netwox_dnsrrring_init(&prrring));
  netwib_er(netwox_dnsrrring_item_create(&pitem));
  pdnsrr = (netwox_dnsrr *)pitem;
  netwib_er(netwox_dnsrr_set_question(pdnsrr, pname, type, rclass));
  netwib_er(netwib_ring_add_last(prrring, pitem));

  /* init packet */
  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  netwib_er(netwox_pkt_append_dns(pbufpool, &dnsheader, prrring, ppkt));
  netwib_er(netwib_bufpool_close(&pbufpool));

  netwib_er(netwox_dnsrrring_close(&prrring));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_dnspktex_answer(netwib_constbuf *pquestion,
                                  netwib_bool ignoreunimplemented,
                                  netwib_constbuf *phostname,
                                  netwib_constip *phostnameip,
                                  netwib_constbuf *pauthns,
                                  netwib_constip *pauthnsip,
                                  netwib_uint32 ttl,
                                  netwib_buf *panswer)
{
  netwib_bufpool *pbufpool;
  netwib_data namearray[1024];
  netwib_buf name;
  netwox_dnshdr dnsheader;
  netwox_dnsrr_type type;
  netwox_dnsrr_class rclass;
  netwib_err ret;

  /* use an array, to not have to bother with freeing memory */
  netwib_er(netwib_buf_init_ext_arraysizeofempty(namearray, &name));

  /* check if it's a question */
  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  ret = netwox_pkt_decode_dns_question(pbufpool, pquestion, &dnsheader,
                                       &name, &type, &rclass);
  netwib_er(netwib_bufpool_close(&pbufpool));
  netwib_er(ret);

  /* reject unsupported opcodes or classes */
  if (dnsheader.opcode != NETWOX_DNSHDR_OPCODE_QUERY ||
      rclass != NETWOX_DNSRR_CLASS_IN) {
    if (ignoreunimplemented) {
      return(NETWIB_ERR_NOTCONVERTED);
    } else {
      netwib_er(netwox_dnspktex_answer_unimplemented(&dnsheader, &name,
                                                     type, rclass, panswer));
      return(NETWIB_ERR_OK);
    }
  }

  /* answer */
  switch(type) {
  case NETWOX_DNSRR_TYPE_A :
    netwib_er(netwox_dnspktex_answer_a(dnsheader.id, NETWIB_TRUE,
                                       dnsheader.recurdesired,
                                       dnsheader.recurdesired, ttl,
                                       &name, phostnameip,
                                       pauthns, pauthnsip, panswer));
    break;
  case NETWOX_DNSRR_TYPE_NS :
    netwib_er(netwox_dnspktex_answer_ns(dnsheader.id, NETWIB_TRUE,
                                        dnsheader.recurdesired,
                                        dnsheader.recurdesired, ttl,
                                        &name, pauthns, pauthnsip, panswer));
    break;
  case NETWOX_DNSRR_TYPE_PTR :
    netwib_er(netwox_dnspktex_answer_ptr(dnsheader.id, NETWIB_TRUE,
                                         dnsheader.recurdesired,
                                         dnsheader.recurdesired, ttl,
                                         &name, phostname,
                                         pauthns, pauthnsip, panswer));
    break;
  case NETWOX_DNSRR_TYPE_MX :
    netwib_er(netwox_dnspktex_answer_mx(dnsheader.id, NETWIB_TRUE,
                                        dnsheader.recurdesired,
                                        dnsheader.recurdesired, ttl,
                                        &name, 10, phostname,
                                        pauthns, pauthnsip, panswer));
    break;
  default :
    if (ignoreunimplemented) {
      return(NETWIB_ERR_NOTCONVERTED);
    } else {
      netwib_er(netwox_dnspktex_answer_unimplemented(&dnsheader, &name,
                                                     type, rclass, panswer));
      return(NETWIB_ERR_OK);
    }
    break;
  }

  return(NETWIB_ERR_OK);
}
