#include "netwibtest.h"

/*-------------------------------------------------------------*/
netwib_uint32 glo_v;
static netwib_err threadmut_f1(netwib_ptr infosin,
                     netwib_ptr *pinfosout)
{
  netwib_thread_mutex *pmut = (netwib_thread_mutex *)infosin;
  netwib_bool locked;

  netwib_er(netwib_thread_mutex_lock(pmut, NETWIB_TIME_INFINITE, &locked));
  if (!locked) {
    return(NETWIBTEST_ERR_7);
  }
  puts("Thread");
  netwib_er(netwib_time_sleep_msec(500));
  glo_v++;
  netwib_er(netwib_thread_mutex_unlock(pmut));

  pinfosout = pinfosout; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_thread_mutex_init_test(void)
{
  netwib_thread_mutex *pthmut;

  puts("++++----\nTest of netwib_thread_mutex_init");
  netwib_er(netwib_thread_mutex_init(&pthmut));
  netwib_er(netwib_thread_mutex_close(&pthmut));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_thread_mutex_close_test(void)
{
  puts("++++----\nTest of netwib_thread_mutex_close");
  puts("Redundant with netwib_thread_mutex_init");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_thread_mutex_lock_test(void)
{
  netwib_thread *pth1, *pth2;
  netwib_thread_mutex *pthmut;
  netwib_time t;
  netwib_bool locked;
  netwib_err ret;

  puts("++++----\nTest of netwib_thread_mutex_lock");
  netwib_er(netwib_thread_mutex_init(&pthmut));
  glo_v = 1;

  netwib_er(netwib_thread_init(&threadmut_f1, pthmut, &pth1));
  netwib_er(netwib_thread_init(&threadmut_f1, pthmut, &pth2));

  netwib_er(netwib_time_sleep_msec(50));
  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_plus_msec(&t, 10));
  netwib_er(netwib_thread_mutex_lock(pthmut, &t, &locked));
  if (locked) {
    return(NETWIBTEST_ERR_9);
  }
  netwib_er(netwib_thread_mutex_lock(pthmut, NETWIB_TIME_ZERO, &locked));
  if (locked) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_thread_mutex_lock(pthmut, NETWIB_TIME_INFINITE, &locked));
  if (!locked) {
    return(NETWIBTEST_ERR_8);
  }
  puts("Main 1");
  glo_v++;
  netwib_er(netwib_thread_mutex_unlock(pthmut));

  netwib_er(netwib_thread_wait(pth1, NETWIB_TIME_INFINITE, NULL, &ret, NULL));
  if (ret != NETWIB_ERR_OK) {
    return(NETWIBTEST_ERR_6);
  }
  netwib_er(netwib_thread_wait(pth2, NETWIB_TIME_INFINITE, NULL, &ret, NULL));
  if (ret != NETWIB_ERR_OK) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_er(netwib_thread_close(&pth1));
  netwib_er(netwib_thread_close(&pth2));
  netwib_er(netwib_thread_mutex_close(&pthmut));

  if (glo_v != 4) {
    return(NETWIBTEST_ERR_4);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_thread_mutex_unlock_test(void)
{
  puts("++++----\nTest of netwib_thread_mutex_unlock");
  puts("Redundant with netwib_thread_mutex_lock");
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_threadmut(void)
{
  puts("###########################################\nTest of sys/threadmut");
  netwib_er(netwib_thread_mutex_init_test());
  netwib_er(netwib_thread_mutex_close_test());
  netwib_er(netwib_thread_mutex_lock_test());
  netwib_er(netwib_thread_mutex_unlock_test());

  return(NETWIB_ERR_OK);
}

