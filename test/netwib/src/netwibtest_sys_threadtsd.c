#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err threadtsd_f1(netwib_ptr infosin,
                     netwib_ptr *pinfosout)
{
  netwib_thread_tsd *ptsd = (netwib_thread_tsd *)infosin;
  netwib_ptr ptr;

  netwib_er(netwib_thread_tsd_ctl_set_value(ptsd, (netwib_ptr)0x1234));
  netwib_er(netwib_thread_tsd_ctl_get_value(ptsd, &ptr));
  if (ptr != (netwib_ptr)0x1234) {
    return(NETWIBTEST_ERR_7);
  }

  pinfosout = pinfosout; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_thread_tsd_init_test(void)
{
  netwib_thread_tsd *pthtsd;

  puts("++++----\nTest of netwib_thread_tsd_init");
  netwib_er(netwib_thread_tsd_init(&pthtsd));
  netwib_er(netwib_thread_tsd_close(&pthtsd));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_thread_tsd_close_test(void)
{
  puts("++++----\nTest of netwib_thread_tsd_close");
  puts("Redundant with netwib_thread_tsd_init");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_thread_tsd_ctl_set_test(void)
{
  netwib_thread *pth;
  netwib_thread_tsd *pthtsd;
  netwib_ptr ptr;
  netwib_err ret;

  puts("++++----\nTest of netwib_thread_tsd_ctl_set");
  netwib_er(netwib_thread_tsd_init(&pthtsd));
  netwib_er(netwib_thread_init(&threadtsd_f1, pthtsd, &pth));

  netwib_er(netwib_thread_tsd_ctl_set_value(pthtsd, (netwib_ptr)0x1235));
  netwib_er(netwib_thread_tsd_ctl_get_value(pthtsd, &ptr));
  if (ptr != (netwib_ptr)0x1235) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_thread_wait(pth, NETWIB_TIME_INFINITE, NULL, &ret, NULL));
  if (ret != NETWIB_ERR_OK) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_thread_close(&pth));
  netwib_er(netwib_thread_tsd_close(&pthtsd));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_thread_tsd_ctl_get_test(void)
{
  puts("++++----\nTest of netwib_thread_tsd_ctl_get");
  puts("Redundant with netwib_thread_tsd_ctl_set");
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_threadtsd(void)
{
  puts("###########################################\nTest of sys/threadtsd");
  netwib_er(netwib_thread_tsd_init_test());
  netwib_er(netwib_thread_tsd_close_test());
  netwib_er(netwib_thread_tsd_ctl_set_test());
  netwib_er(netwib_thread_tsd_ctl_get_test());

  return(NETWIB_ERR_OK);
}
