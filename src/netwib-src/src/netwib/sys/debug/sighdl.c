/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
static void netwib_debug_sighdl_report(int num)
{
  netwib_err ret;

#if NETWIB_DEBUG_SIGHDL_REPORT_LEAKMEMFD == 1
  if (!netwib_priv_glovars.debug_ctrlc_pressed) {
    puts("Signal received. The program may stop on next event.");
    puts("Press Control-C now, to stop the program immediately.");
    /* this variable will be read by netwib_global_ctl_get_debug_ctrlc_
       pressed(pbool) */
    netwib_priv_glovars.debug_ctrlc_pressed = NETWIB_TRUE;
    return;
  }

  puts("Signal received twice. Checking leaks (might cause deadlock...)");
  ret = netwib_debug_leak_report();
  if (ret != NETWIB_ERR_OK) {
    netwib_priv_notify_err(NETWIB_PRIV_NOTIFYTYPE_WARNING, ret);
  }
  ret = netwib_debug_memcorrupt_check_all();
  if (ret != NETWIB_ERR_OK) {
    netwib_priv_notify_err(NETWIB_PRIV_NOTIFYTYPE_WARNING, ret);
  }
  ret = netwib_debug_fdopen_check();
  if (ret != NETWIB_ERR_OK) {
    netwib_priv_notify_err(NETWIB_PRIV_NOTIFYTYPE_WARNING, ret);
  }
#else
  /* Well, this message is never displayed, because for now
     netwib_debug_sighdl is only used when
     NETWIB_DEBUG_SIGHDL_REPORT_LEAKMEMFD is set. */
  puts("Signal received.");
  puts("Leak report currently disabled.");
#endif

  if (num == SIGINT || num == SIGTERM) {
    raise(SIGKILL);
  }
  ret = NETWIB_ERR_OK; /* for compiler warning */
  return;
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_sighdl_init(void)
{

  /* set signal handlers */
  signal(SIGINT, netwib_debug_sighdl_report);
  signal(SIGTERM, netwib_debug_sighdl_report);
  /* we could also set SIGUSR1/USR2, but LinuxThread implementation
     on kernels < 2.2 use them (documented in LinuxThread FAQ). */

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_sighdl_close(void)
{

  /* set signal handlers */
  signal(SIGINT, SIG_DFL);
  signal(SIGTERM, SIG_DFL);

  return(NETWIB_ERR_OK);
}
