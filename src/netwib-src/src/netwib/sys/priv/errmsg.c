/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_errmsg_reinit(void)
{
  netwib_er(netwib_priv_glovars_wrlock());
  netwib__buf_reinit(&netwib_priv_glovars.errmsg);
  netwib_er(netwib_priv_glovars_wrunlock());

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_errmsg_append_buf(netwib_constbuf *pbuf)
{
  netwib_err ret;

  netwib_er(netwib_priv_glovars_wrlock());
  ret = netwib_buf_append_buf(pbuf, &netwib_priv_glovars.errmsg);
  netwib_er(netwib_priv_glovars_wrunlock());

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_errmsg_append_string(netwib_conststring msg)
{
  netwib_err ret;

  netwib_er(netwib_priv_glovars_wrlock());
  ret = netwib_buf_append_string(msg, &netwib_priv_glovars.errmsg);
  netwib_er(netwib_priv_glovars_wrunlock());

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_errmsg_buf(netwib_constbuf *pbuf)
{
  netwib_err ret;

  netwib_er(netwib_priv_glovars_wrlock());
  netwib__buf_reinit(&netwib_priv_glovars.errmsg);
  ret = netwib_buf_append_buf(pbuf, &netwib_priv_glovars.errmsg);
  netwib_er(netwib_priv_glovars_wrunlock());

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_errmsg_string(netwib_conststring msg)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_ext_string(msg, &buf));
  netwib_er(netwib_priv_errmsg_buf(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_errmsg_func_buf(netwib_conststring functionname,
                                       netwib_constbuf *pexplain)
{
  netwib_byte array[256];
  netwib_buf buf;

  netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));
  netwib_er(netwib_buf_append_string(functionname, &buf));
  netwib_er(netwib_buf_append_string(": ", &buf));
  netwib_er(netwib_buf_append_buf(pexplain, &buf));

  netwib_er(netwib_priv_errmsg_buf(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_errmsg_func_string(netwib_conststring functionname,
                                          netwib_conststring explain)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_ext_string(explain, &buf));
  netwib_er(netwib_priv_errmsg_func_buf(functionname, &buf));

  return(NETWIB_ERR_OK);
}
