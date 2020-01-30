/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_routes_init(netwib_priv_confwork_routes **ppitem)
{
  netwib_priv_confwork_routes *pitem;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_confwork_routes),
                              (netwib_ptr*)ppitem));
  pitem = *ppitem;
  pitem->devnum = 0;
  netwib_er(netwib_buf_init_mallocdefault(&pitem->device));
  pitem->srcset = NETWIB_FALSE;
  pitem->gwset = NETWIB_FALSE;
  pitem->metric = 999;

  /* otherwise if forgot, they'll crash debug function */
  netwib_er(netwib_ip_init_ip4_fields(0xFF,0xFF,0xFF,0xFF, &pitem->dst));
  netwib_er(netwib_ip_init_ip4_fields(0xFF,0xFF,0xFF,0xFF, &pitem->mask));
  netwib_er(netwib_ip_init_ip4_fields(0xFF,0xFF,0xFF,0xFF, &pitem->src));
  netwib_er(netwib_ip_init_ip4_fields(0xFF,0xFF,0xFF,0xFF, &pitem->gw));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_routes_close(netwib_priv_confwork_routes **ppitem)
{
  netwib_priv_confwork_routes *pitem;

  pitem = *ppitem;
  netwib_er(netwib_buf_close(&pitem->device));
  netwib_er(netwib_ptr_free((netwib_ptr*)ppitem));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_routes_add(netwib_priv_confwork *pcw,
                                           netwib_priv_confwork_routes *pitem)
{
  netwib_er(netwib_priv_confwork_dev_noalias(&pitem->device));
  netwib_er(netwib_ring_add_last(pcw->proutes, pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_routes_erase(netwib_ptr pitem)
{
  netwib_er(netwib_priv_confwork_routes_close((netwib_priv_confwork_routes **)&pitem));
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
netwib_err netwib_conf_routes_init(netwib_conf_routes **ppitem)
{
  netwib_er(netwib_ptr_malloc(sizeof(netwib_conf_routes),
                              (netwib_ptr*)ppitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_conf_routes_close(netwib_conf_routes **ppitem)
{
  netwib_er(netwib_ptr_free((netwib_ptr*)ppitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_conf_routes_add(netwib_priv_conf_t *pc,
                                  netwib_conf_routes *pitem)
{
  netwib_er(netwib_ring_add_last(pc->proutes, pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_conf_routes_erase(netwib_ptr pitem)
{
  netwib_er(netwib_conf_routes_close((netwib_conf_routes **)&pitem));
  return(NETWIB_ERR_OK);
}
