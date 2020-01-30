/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_ip_init(netwib_priv_confwork_ip **ppitem)
{
  netwib_priv_confwork_ip *pitem;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_confwork_ip),
                            (netwib_ptr*)ppitem));
  pitem = *ppitem;
  pitem->devnum = 0;
  netwib_er(netwib_buf_init_mallocdefault(&pitem->device));
  pitem->ispointtopoint = NETWIB_FALSE;

  /* otherwise if forgot, they'll crash debug function */
  netwib_er(netwib_ip_init_ip4_fields(0xFF,0xFF,0xFF,0xFF, &pitem->ip));
  netwib_er(netwib_ip_init_ip4_fields(0xFF,0xFF,0xFF,0xFF, &pitem->mask));
  netwib_er(netwib_ip_init_ip4_fields(0xFF,0xFF,0xFF,0xFF,
                                      &pitem->pointtopointip));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_ip_close(netwib_priv_confwork_ip **ppitem)
{
  netwib_priv_confwork_ip *pitem;

  pitem = *ppitem;
  netwib_er(netwib_buf_close(&pitem->device));
  netwib_er(netwib_ptr_free((netwib_ptr*)ppitem));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_ip_add(netwib_priv_confwork *pcw,
                                       netwib_priv_confwork_ip *pitem)
{
  netwib_er(netwib_priv_confwork_dev_noalias(&pitem->device));
  netwib_er(netwib_ring_add_last(pcw->pip, pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_ip_erase(netwib_ptr pitem)
{
  netwib_er(netwib_priv_confwork_ip_close((netwib_priv_confwork_ip **)&pitem));
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
netwib_err netwib_conf_ip_init(netwib_conf_ip **ppitem)
{
  netwib_er(netwib_ptr_malloc(sizeof(netwib_conf_ip),
                            (netwib_ptr*)ppitem));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_conf_ip_close(netwib_conf_ip **ppitem)
{
  netwib_er(netwib_ptr_free((netwib_ptr*)ppitem));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_conf_ip_add(netwib_priv_conf_t *pc,
                                 netwib_conf_ip *pitem)
{
  netwib_er(netwib_ring_add_last(pc->pip, pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_conf_ip_erase(netwib_ptr pitem)
{
  netwib_er(netwib_conf_ip_close((netwib_conf_ip **)&pitem));
  return(NETWIB_ERR_OK);
}
