#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err f1(netwib_ptr infosin,
                     netwib_ptr *pinfosout)
{
  netwib_uint32 *pui = (netwib_uint32 *)infosin;

  netwib_er(netwib_fmt_display("Hello. You sent me %{uint32}\n", *pui));

  *pinfosout = (void*)0x12345678;
  if (*pui == 55) {
    netwib_er(netwib_time_sleep_msec(600));
    return(NETWIB_ERR_DATAEND);
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_thread_init_test(void)
{
  netwib_thread *pth;
  netwib_time t;
  netwib_uint32 ui;
  netwib_ptr ptr;
  netwib_bool event;
  netwib_err ret;

  puts("++++----\nTest of netwib_thread_init");
  ui = 56;
  netwib_er(netwib_thread_init(&f1, &ui, &pth));
  netwib_er(netwib_thread_wait(pth, NETWIB_TIME_INFINITE, &event, &ret, &ptr));
  if (!event) {
    return(NETWIBTEST_ERR_7);
  }
  if (ret != NETWIB_ERR_OK) {
    return(NETWIBTEST_ERR_7);
  }
  if (ptr != (void*)0x12345678) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_thread_close(&pth));

  ui = 55;
  netwib_er(netwib_thread_init(&f1, &ui, &pth));
  netwib_er(netwib_thread_wait(pth, NETWIB_TIME_ZERO, &event, &ret, &ptr));
  if (event) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_plus_msec(&t, 100));
  netwib_er(netwib_thread_wait(pth, &t, &event, &ret, &ptr));
  if (event) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_thread_wait(pth, NETWIB_TIME_INFINITE, &event, &ret, &ptr));
  if (!event) {
    return(NETWIBTEST_ERR_7);
  }
  if (ret != NETWIB_ERR_DATAEND) {
    return(NETWIBTEST_ERR_7);
  }
  if (ptr != (void*)0x12345678) {
    return(NETWIBTEST_ERR_7);
  }
  /* it must work if we call it twice */
  netwib_er(netwib_thread_wait(pth, NETWIB_TIME_INFINITE, &event, &ret, &ptr));
  if (!event) {
    return(NETWIBTEST_ERR_7);
  }
  if (ret != NETWIB_ERR_DATAEND) {
    return(NETWIBTEST_ERR_7);
  }
  if (ptr != (void*)0x12345678) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_thread_close(&pth));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_thread_close_test(void)
{
  puts("++++----\nTest of netwib_thread_close");
  puts("Redundant with netwib_thread_init");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_thread_wait_test(void)
{
  puts("++++----\nTest of netwib_thread_wait");
  puts("Redundant with netwib_thread_init");
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_thread(void)
{
  puts("###########################################\nTest of sys/thread");
  netwib_er(netwib_thread_init_test());
  netwib_er(netwib_thread_close_test());
  netwib_er(netwib_thread_wait_test());


  return(NETWIB_ERR_OK);
}
