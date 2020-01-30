/* This file is part of Netwib. Read and honor its license.
*/


/*-------------------------------------------------------------*/
netwib_err netwib_debug_string2_init(netwib_string stringout)
{
  if (stringout == NULL)
    return(NETWIB_ERR_PANULLPTR);
  stringout[0] = '\0';
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_string2_append_string(netwib_conststring text,
                                              netwib_uint32 stringoutmaxsize,
                                              netwib_string stringout)
{
  netwib_uint32 strlenstringout, strlentext;

  if (text == NULL)
    return(NETWIB_ERR_PANULLPTR);
  if (stringout == NULL)
    return(NETWIB_ERR_PANULLPTR);

  strlenstringout = netwib_c_strlen(stringout);
  strlentext = netwib_c_strlen(text);

  if (strlenstringout + strlentext > stringoutmaxsize)
    return(NETWIB_ERR_DATANOSPACE);

  strcpy(stringout+strlenstringout, text);
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_string2_append_fmt(netwib_uint32 stringoutmaxsize,
                                           netwib_string stringout,
                                           netwib_conststring fmt,
                                           ...)
{
  va_list ap;
  netwib_err ret;
  netwib_char arr[4096];
  netwib_string pc;
  netwib_buf buf;

  if (fmt == NULL)
    return(NETWIB_ERR_PANULLPTR);
  if (stringout == NULL)
    return(NETWIB_ERR_PANULLPTR);

  netwib_er(netwib_buf_init_ext_arraysizeofempty(arr, &buf));
  va_start(ap, fmt);
  ret = netwib_priv_buf_append_vfmt(&buf, fmt, &ap);
  va_end(ap);
  if (ret != NETWIB_ERR_OK) {
    netwib__buf_reinit(&buf);
    netwib_er(netwib_buf_append_string("Error when trying to generate ", &buf));
    netwib_er(netwib_buf_append_string(fmt, &buf));
  }

  netwib_er(netwib_buf_ref_string(&buf, &pc));
  netwib_er(netwib_debug_string2_append_string(pc, stringoutmaxsize, stringout));

  return(NETWIB_ERR_OK);
}
