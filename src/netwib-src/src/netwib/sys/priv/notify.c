/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_notify_string(netwib_priv_notifytype type,
                                   netwib_conststring msg)
{
  /* currently, we use "fprintf(stderr" on every system. On Windows,
     we could popup a new window. */
  if (type == NETWIB_PRIV_NOTIFYTYPE_EMERG ||
      type == NETWIB_PRIV_NOTIFYTYPE_ALERT) {
    fprintf(stderr, "\n");
    fprintf(stderr, "%s\n", "           _          _          _");
    fprintf(stderr, "%s\n", "          / \\        / \\        / \\");
    fprintf(stderr, "%s\n", "         / I \\      / I \\      / I \\");
    fprintf(stderr, "%s\n", "        /  o  \\    /  o  \\    /  o  \\");
    fprintf(stderr, "%s\n", "       /_______\\  /_______\\  /_______\\");
    fprintf(stderr, "\n");
  }

  fprintf(stderr, "%s\n", msg);

  if (type == NETWIB_PRIV_NOTIFYTYPE_EMERG) {
    fprintf(stderr, "%s\n", "This is a fatal error.");
    fprintf(stderr, "%s\n", "Please contact Laurent.");
  } else if (type == NETWIB_PRIV_NOTIFYTYPE_ALERT) {
    fprintf(stderr, "%s\n", "This is a fatal error.");
    fprintf(stderr, "%s\n", "You must change your program.");
  } else if (type == NETWIB_PRIV_NOTIFYTYPE_WARNING) {
    fprintf(stderr, "%s\n", "This is a warning.");
    fprintf(stderr, "%s\n", "You should correct your program.");
  }

  fflush(stderr);

#if NETWIB_DEBUG_ERROR_BT==1
  if (type != NETWIB_PRIV_NOTIFYTYPE_DEBUG) {
    netwib_string str;
    fprintf(stderr, "Backtrace : ");
    netwib_er(netwib_debug_backtrace_string(&str));
    fprintf(stderr, "%s\n", str);
    netwib_er(netwib_debug_string_close(&str));
    fflush(stderr);
  }
#endif

  if (type == NETWIB_PRIV_NOTIFYTYPE_EMERG ||
      type == NETWIB_PRIV_NOTIFYTYPE_ALERT) {
    netwib_er(netwib_priv_program_exit());
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_notify_fmt(netwib_priv_notifytype type,
                                  netwib_conststring fmt,
                                  ...)
{
  va_list ap;
  netwib_err ret;
  netwib_char arr[4096];
  netwib_string pc;
  netwib_buf buf;

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
  netwib_er(netwib_priv_notify_string(type, pc));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_notify_err(netwib_priv_notifytype type,
                                  netwib_err error)
{
  netwib_buf buf;
  netwib_string pc;
  netwib_uint32 varerrno, varherrno, vargetlasterror;
  netwib_char arr[1024];

  netwib_er(netwib_priv_err_syserrors(&varerrno, &varherrno,
                                      &vargetlasterror));

  netwib_er(netwib_buf_init_ext_arraysizeofempty(arr, &buf));
  netwib_er(netwib_priv_err_append_err(error, varerrno, varherrno,
                                     vargetlasterror,
                                     NETWIB_ERR_ENCODETYPE_FULL, &buf));
  netwib_er(netwib_buf_ref_string(&buf, &pc));
  netwib_er(netwib_priv_notify_string(type, pc));
  return(NETWIB_ERR_OK);
}

