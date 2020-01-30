/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_debug_disp_string(netwib_conststring msg)
{
  /* currently, we use "fprintf(stderr" on every system. On Windows,
     we could popup a new window. */
  fprintf(stderr, "%s", msg);
  fflush(stderr);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_disp_fmt(netwib_conststring fmt,
                                 ...)
{
  va_list ap;
  netwib_err ret;
  netwib_char arr[4096];
  netwib_string pc;
  netwib_buf buf;

  netwib_er(netwib_buf_init_ext_arrayempty(arr, sizeof(arr), &buf));
  va_start(ap, fmt);
  ret = netwib_priv_buf_append_vfmt(&buf, fmt, &ap);
  va_end(ap);
  if (ret != NETWIB_ERR_OK) {
    netwib__buf_reinit(&buf);
    netwib_er(netwib_buf_append_string("Error when trying to generate ", &buf));
    netwib_er(netwib_buf_append_string(fmt, &buf));
  }

  netwib_er(netwib_buf_ref_string(&buf, &pc));
  netwib_er(netwib_debug_disp_string(pc));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_disp_dump(netwib_constdata data,
                                  netwib_uint32 datasize)
{
  netwib_uint32 i;

  netwib_er(netwib_debug_disp_string("    0 :"));
  for (i = 0; i < datasize; i++) {
    netwib_er(netwib_debug_disp_fmt(" %{uint32:02X}", data[i]));
    if (!((i + 1) % 20) && i) {
      netwib_er(netwib_debug_disp_fmt("\n  %{uint32:03} :", i + 1));
    }
  }
  netwib_er(netwib_debug_disp_string("\n"));

  return(NETWIB_ERR_OK);
}

