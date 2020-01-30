#include "netwibtest.h"

/*-------------------------------------------------------------*/
netwib_uint32 glo_v_rwl;
static netwib_err threadrwl_f1(netwib_ptr infosin,
                              netwib_ptr *pinfosout)
{
  netwib_thread_rwlock *prwl = (netwib_thread_rwlock *)infosin;
  netwib_bool locked;

  puts("Thread before wrlock");
  netwib_er(netwib_thread_rwlock_wrlock(prwl, NETWIB_TIME_INFINITE, &locked));
  if (!locked) {
    return(NETWIBTEST_ERR_7);
  }
  puts("Thread wrlock acquired");
  netwib_er(netwib_time_sleep_msec(500));
  glo_v_rwl++;
  netwib_er(netwib_thread_rwlock_wrunlock(prwl));
  puts("Thread wrlock released");

  puts("Thread before rdlock");
  netwib_er(netwib_thread_rwlock_rdlock(prwl, NETWIB_TIME_INFINITE, &locked));
  if (!locked) {
    return(NETWIBTEST_ERR_7);
  }
  puts("Thread rdlock acquired");
  netwib_er(netwib_fmt_display("Value : %{uint32}\n", glo_v_rwl));
  netwib_er(netwib_time_sleep_msec(500));
  netwib_er(netwib_thread_rwlock_rdunlock(prwl));
  puts("Thread rdlock released");

  pinfosout = pinfosout; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_thread_rwlock_init_test(void)
{
  netwib_thread_rwlock *pthrwl;

  puts("++++----\nTest of netwib_thread_rwlock_init");
  netwib_er(netwib_thread_rwlock_init(&pthrwl));
  netwib_er(netwib_thread_rwlock_close(&pthrwl));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_thread_rwlock_close_test(void)
{
  puts("++++----\nTest of netwib_thread_rwlock_close");
  puts("Redundant with netwib_thread_rwlock_init");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_thread_rwlock_rdlock_test(void)
{
  netwib_thread *pth1, *pth2;
  netwib_thread_rwlock *pthrwl;
  netwib_time t;
  netwib_bool locked;
  netwib_err ret;

  puts("++++----\nTest of netwib_thread_rwlock_rdlock");
  netwib_er(netwib_thread_rwlock_init(&pthrwl));
  glo_v_rwl = 1;

  netwib_er(netwib_thread_init(&threadrwl_f1, pthrwl, &pth1));
  netwib_er(netwib_thread_init(&threadrwl_f1, pthrwl, &pth2));

  netwib_er(netwib_time_sleep_msec(50));
  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_plus_msec(&t, 10));
  netwib_er(netwib_thread_rwlock_rdlock(pthrwl, &t, &locked));
  if (locked) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_thread_rwlock_wrlock(pthrwl, &t, &locked));
  if (locked) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_thread_rwlock_rdlock(pthrwl, NETWIB_TIME_ZERO, &locked));
  if (locked) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_thread_rwlock_wrlock(pthrwl, NETWIB_TIME_ZERO, &locked));
  if (locked) {
    return(NETWIBTEST_ERR_7);
  }

  puts("Main before rdlock");
  netwib_er(netwib_thread_rwlock_rdlock(pthrwl, NETWIB_TIME_INFINITE, &locked));
  if (!locked) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_thread_rwlock_rdunlock(pthrwl));
  puts("Main rdlock released");

  puts("Main before wrlock");
  netwib_er(netwib_thread_rwlock_wrlock(pthrwl, NETWIB_TIME_INFINITE, &locked));
  if (!locked) {
    return(NETWIBTEST_ERR_7);
  }
  glo_v_rwl++;
  netwib_er(netwib_thread_rwlock_wrunlock(pthrwl));
  puts("Main wrlock released");

  netwib_er(netwib_thread_wait(pth1, NETWIB_TIME_INFINITE, NULL, &ret, NULL));
  if (ret != NETWIB_ERR_OK) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_thread_wait(pth2, NETWIB_TIME_INFINITE, NULL, &ret, NULL));
  if (ret != NETWIB_ERR_OK) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_thread_close(&pth1));
  netwib_er(netwib_thread_close(&pth2));
  netwib_er(netwib_thread_rwlock_close(&pthrwl));

  if (glo_v_rwl != 4) {
    return(NETWIBTEST_ERR_8);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_thread_rwlock_rdunlock_test(void)
{
  puts("++++----\nTest of netwib_thread_rwlock_rdunlock");
  puts("Redundant with netwib_thread_rwlock_rdlock");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_thread_rwlock_wrlock_test(void)
{
  puts("++++----\nTest of netwib_thread_rwlock_wrlock");
  puts("Redundant with netwib_thread_rwlock_rdlock");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_thread_rwlock_wrunlock_test(void)
{
  puts("++++----\nTest of netwib_thread_rwlock_wrunlock");
  puts("Redundant with netwib_thread_rwlock_rdlock");
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_threadrwl(void)
{
  puts("###########################################\nTest of sys/threadrwl");
  netwib_er(netwib_thread_rwlock_init_test());
  netwib_er(netwib_thread_rwlock_close_test());
  netwib_er(netwib_thread_rwlock_rdlock_test());
  netwib_er(netwib_thread_rwlock_rdunlock_test());
  netwib_er(netwib_thread_rwlock_wrlock_test());
  netwib_er(netwib_thread_rwlock_wrunlock_test());

  return(NETWIB_ERR_OK);
}

