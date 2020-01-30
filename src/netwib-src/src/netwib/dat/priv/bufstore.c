/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_bufstore_create(netwib_constbuf *pbuf,
                                       netwib_ptr *ppitem)
{
  netwib_priv_bufstore *pbufstore;
  netwib_data data;
  netwib_uint32 datasize;
  netwib_ptr ptr;

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);

  netwib_er(netwib_ptr_malloc(NETWIB_PRIV_BUFSTORE_ALIGN+datasize+1, &ptr));
  pbufstore = (netwib_priv_bufstore *)ptr;
  *ppitem = ptr;

  pbufstore->data = (netwib_data)ptr + NETWIB_PRIV_BUFSTORE_ALIGN;
  pbufstore->datasize = datasize;
  netwib_c_memcpy(pbufstore->data, data, datasize);
  pbufstore->data[datasize] = '\0'; /* for easy netwib_buf_ref_string */

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_bufstore_decode(netwib_constptr pitem,
                                       netwib_bufext *pbuf)
{
  const netwib_priv_bufstore *pbufstore = (const netwib_priv_bufstore *)pitem;

  netwib_er(netwib_buf_init_ext_array(pbufstore->data,
                                      pbufstore->datasize+1,
                                      0, pbufstore->datasize, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_bufstore_erase(netwib_ptr pitem)
{
  netwib_er(netwib_ptr_free(&pitem));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_bufstore_duplicate(netwib_constptr pitem,
                                          netwib_ptr *pdupofitem)
{
  const netwib_priv_bufstore *pbufstore = (const netwib_priv_bufstore *)pitem;
  netwib_priv_bufstore *pbufstoredup;
  netwib_ptr ptr;

  netwib_er(netwib_ptr_malloc(NETWIB_PRIV_BUFSTORE_ALIGN+pbufstore->datasize+1, &ptr));
  pbufstoredup = (netwib_priv_bufstore *)ptr;
  *pdupofitem = ptr;

  pbufstoredup->data = (netwib_data)ptr + NETWIB_PRIV_BUFSTORE_ALIGN;
  pbufstoredup->datasize = pbufstore->datasize;
  netwib_c_memcpy(pbufstoredup->data, pbufstore->data, pbufstore->datasize);
  pbufstoredup->data[pbufstoredup->datasize] = '\0';

  return(NETWIB_ERR_OK);
}
