/* This file is part of Netwib. Read and honor its license.
*/

#define NETWIB_DEBUG_STRING_ALLOCSTEP 128

/*-------------------------------------------------------------*/
netwib_err netwib_debug_string_init(netwib_string *pstringout)
{
  netwib_string stringout;

  if (pstringout == NULL)
    return(NETWIB_ERR_PANULLPTR);

  stringout = (netwib_string)malloc(NETWIB_DEBUG_STRING_ALLOCSTEP);
  if (stringout == NULL)
    return(NETWIB_ERR_FUMALLOC);

  stringout[0] = '\0';
  *pstringout = stringout;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_string_close(netwib_string *pstringout)
{
  if (pstringout == NULL)
    return(NETWIB_ERR_PANULLPTR);

  free(*pstringout);
  *pstringout = NULL;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_string_append_string(netwib_conststring text,
                                           netwib_string *pstringout)
{
  netwib_uint32 strlenstringout, strlentext;
  netwib_uint32 currentsize, neededsize;
  netwib_string stringout;

  if (text == NULL)
    return(NETWIB_ERR_PANULLPTR);
  if (pstringout == NULL)
    return(NETWIB_ERR_PANULLPTR);
  stringout = *pstringout;

  /* check if we need to realloc */
  strlenstringout = netwib_c_strlen(stringout);
  strlentext = netwib_c_strlen(text);
  currentsize = NETWIB_DEBUG_STRING_ALLOCSTEP *
                (1 + (strlenstringout + 1)/NETWIB_DEBUG_STRING_ALLOCSTEP);
  neededsize = NETWIB_DEBUG_STRING_ALLOCSTEP *
               (1 + (currentsize + strlentext + 1)/NETWIB_DEBUG_STRING_ALLOCSTEP);
  if (currentsize != neededsize) {
    stringout = (netwib_string)realloc(stringout, neededsize);
  }

  /* copy data */
  strcpy(stringout+strlenstringout, text);

  *pstringout = stringout;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_string_append_fmt(netwib_string *pstringout,
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
  if (pstringout == NULL)
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
  netwib_er(netwib_debug_string_append_string(pc, pstringout));
  return(NETWIB_ERR_OK);
}
