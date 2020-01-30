#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err threadcond_f1(netwib_ptr infosin,
                               netwib_ptr *pinfosout)
{
  netwib_thread_cond *pcond = (netwib_thread_cond *)infosin;

  netwib_er(netwib_time_sleep_msec(500));

  netwib_er(netwib_thread_cond_broadcast(pcond, 34));
  netwib_er(netwib_time_sleep_msec(500));
  netwib_er(netwib_thread_cond_broadcast(pcond, 56));

  pinfosout = pinfosout; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_thread_cond_init_test(void)
{
  netwib_thread_cond *pthcond;

  puts("++++----\nTest of netwib_thread_cond_init");
  netwib_er(netwib_thread_cond_init(&pthcond));
  netwib_er(netwib_thread_cond_close(&pthcond));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_thread_cond_close_test(void)
{
  puts("++++----\nTest of netwib_thread_cond_close");
  puts("Redundant with netwib_thread_cond_init");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_thread_cond_broadcast_test(void)
{
  netwib_thread *pth;
  netwib_thread_cond *pthcond;
  netwib_time t;
  netwib_bool event;
  netwib_uint32 value;
  netwib_err ret;

  puts("++++----\nTest of netwib_thread_cond_broadcast");
  netwib_er(netwib_thread_cond_init(&pthcond));

  netwib_er(netwib_thread_init(&threadcond_f1, pthcond, &pth));

  netwib_er(netwib_thread_cond_wait(pthcond, NETWIB_TIME_ZERO, &event, NULL));
  if (event) {
    return(NETWIBTEST_ERR_2);
  }
  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_plus_msec(&t, 10));
  netwib_er(netwib_thread_cond_wait(pthcond, &t, &event, NULL));
  if (event) {
    return(NETWIBTEST_ERR_3);
  }

  netwib_er(netwib_thread_cond_wait(pthcond, NETWIB_TIME_INFINITE,
                                    &event, &value));
  if (!event) {
    return(NETWIBTEST_ERR_4);
  }
  if (value != 34) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_er(netwib_thread_cond_reinit(pthcond));
  netwib_er(netwib_thread_cond_wait(pthcond, NETWIB_TIME_INFINITE,
                                    &event, &value));
  if (!event) {
    return(NETWIBTEST_ERR_4);
  }
  if (value != 56) {
    return(NETWIBTEST_ERR_5);
  }

  netwib_er(netwib_thread_wait(pth, NETWIB_TIME_INFINITE, NULL, &ret, NULL));
  if (ret != NETWIB_ERR_OK) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_thread_close(&pth));
  netwib_er(netwib_thread_cond_close(&pthcond));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_thread_cond_wait_test(void)
{
  puts("++++----\nTest of netwib_thread_cond_wait");
  puts("Redundant with netwib_thread_cond_broadcast");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_thread_cond_reinit_test(void)
{
  puts("++++----\nTest of netwib_thread_cond_reinit");
  puts("Redundant with netwib_thread_cond_broadcast");
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_threadcond(void)
{
  puts("###########################################\nTest of sys/threadcond");
  netwib_er(netwib_thread_cond_init_test());
  netwib_er(netwib_thread_cond_close_test());
  netwib_er(netwib_thread_cond_broadcast_test());
  netwib_er(netwib_thread_cond_wait_test());
  netwib_er(netwib_thread_cond_reinit_test());

  return(NETWIB_ERR_OK);
}

