#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err threadlist_f1(netwib_ptr infosin,
                     netwib_ptr *pinfosout)
{
  netwib_uint32 *pui = (netwib_uint32 *)infosin;

  netwib_er(netwib_fmt_display("Hello. You sent me %{uint32}\n", *pui));

  *pinfosout = NULL;

  netwib_er(netwib_time_sleep_msec(*pui));
  if (*pui == 1111) {
    return(NETWIB_ERR_DATAEND);
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_threadlist_init_test(void)
{
  netwib_ring *pring;

  puts("++++----\nTest of netwib_threadlist_init");
  netwib_er(netwib_threadlist_init(&pring));
  netwib_er(netwib_threadlist_close(&pring));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_threadlist_close_test(void)
{
  netwib_ring *pring;

  puts("++++----\nTest of netwib_threadlist_close");
  netwib_er(netwib_threadlist_init(&pring));
  netwib_er(netwib_threadlist_close(&pring));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_threadlist_add_test(void)
{
  netwib_thread *pth;
  netwib_ring *pring;
  netwib_uint32 ui;

  puts("++++----\nTest of netwib_threadlist_add");

  netwib_er(netwib_threadlist_init(&pring));
  ui = 50;
  netwib_er(netwib_thread_init(&threadlist_f1, &ui, &pth));
  netwib_er(netwib_threadlist_add(pring, pth, 123));
  netwib_er(netwib_threadlist_close(&pring));

  netwib_er(netwib_threadlist_init(&pring));
  ui = 50;
  netwib_er(netwib_thread_init(&threadlist_f1, &ui, &pth));
  netwib_er(netwib_threadlist_add(pring, pth, 123));
  ui = 51;
  netwib_er(netwib_thread_init(&threadlist_f1, &ui, &pth));
  netwib_er(netwib_threadlist_add(pring, pth, 124));
  netwib_er(netwib_threadlist_close(&pring));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_threadlist_del_test(void)
{
  netwib_ring *pring;

  puts("++++----\nTest of netwib_threadlist_del");

  netwib_er(netwib_threadlist_init(&pring));
  netwib_er(netwib_threadlist_add(pring, NULL, 123));
  netwib_er(netwib_threadlist_add(pring, NULL, 124));
  netwib_er(netwib_threadlist_add(pring, NULL, 125));
  netwib_er(netwib_threadlist_add(pring, NULL, 126));
  netwib_er(netwib_threadlist_add(pring, NULL, 127));
  netwib_er(netwib_threadlist_del(pring, 127));
  netwib_er(netwib_threadlist_del(pring, 11));
  netwib_er(netwib_threadlist_del(pring, 125));
  netwib_er(netwib_threadlist_del(pring, 123));
  netwib_er(netwib_threadlist_del(pring, 124));
  netwib_er(netwib_threadlist_del(pring, 126));
  netwib_er(netwib_threadlist_close(&pring));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_threadlist_wait_test(void)
{
  netwib_thread *pth;
  netwib_ring *pring;
  netwib_time t;
  netwib_uint32 threadid, ui1, ui2, ui3;
  netwib_bool event;
  netwib_err ret;

  puts("++++----\nTest of netwib_threadlist_wait");

  netwib_er(netwib_threadlist_init(&pring));
  netwib_eir(netwib_threadlist_wait(pring, NETWIB_TIME_ZERO,
                                    &event, &threadid, &ret, NULL),
             NETWIB_ERR_DATAEND);
  netwib_eir(netwib_threadlist_wait(pring, NETWIB_TIME_INFINITE,
                                    &event, &threadid, &ret, NULL),
             NETWIB_ERR_DATAEND);

  /* note : we use big sleep because my HP-UX is very slow in debugging mode,
     so does not synchronize correctly (no time to finish task) */
  ui1 = 1000;
  netwib_er(netwib_thread_init(&threadlist_f1, &ui1, &pth));
  netwib_er(netwib_threadlist_add(pring, pth, 1));
  ui2 = 3000;
  netwib_er(netwib_thread_init(&threadlist_f1, &ui2, &pth));
  netwib_er(netwib_threadlist_add(pring, pth, 2));
  ui3 = 2000;
  netwib_er(netwib_thread_init(&threadlist_f1, &ui3, &pth));
  netwib_er(netwib_threadlist_add(pring, pth, 3));

  netwib_er(netwib_threadlist_wait(pring, NETWIB_TIME_INFINITE,
                                   &event, &threadid, &ret, NULL));
  if (threadid != 1) return(NETWIBTEST_ERR_7);
  if (ret != NETWIB_ERR_OK) return(NETWIBTEST_ERR_8);
  netwib_er(netwib_threadlist_wait(pring, NETWIB_TIME_ZERO,
                                   &event, &threadid, &ret, NULL));
  if (event) return(NETWIBTEST_ERR_5);
  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_plus_msec(&t, 50));
  netwib_er(netwib_threadlist_wait(pring, &t,
                                   &event, &threadid, &ret, NULL));
  if (event) return(NETWIBTEST_ERR_5);
  netwib_er(netwib_threadlist_wait(pring, NETWIB_TIME_INFINITE,
                                   &event, &threadid, &ret, NULL));
  if (threadid != 3) return(NETWIBTEST_ERR_7);
  if (ret != NETWIB_ERR_OK) return(NETWIBTEST_ERR_8);
  netwib_er(netwib_threadlist_wait(pring, NETWIB_TIME_INFINITE,
                                   &event, &threadid, &ret, NULL));
  if (threadid != 2) return(NETWIBTEST_ERR_7);
  if (ret != NETWIB_ERR_OK) return(NETWIBTEST_ERR_8);

  netwib_eir(netwib_threadlist_wait(pring, NETWIB_TIME_ZERO,
                                    &event, &threadid, &ret, NULL),
             NETWIB_ERR_DATAEND);
  netwib_eir(netwib_threadlist_wait(pring, NETWIB_TIME_INFINITE,
                                    &event, &threadid, &ret, NULL),
             NETWIB_ERR_DATAEND);

  netwib_er(netwib_threadlist_close(&pring));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_threadlist(void)
{
  puts("###########################################\nTest of sys/threadlist");
  netwib_er(netwib_threadlist_init_test());
  netwib_er(netwib_threadlist_close_test());
  netwib_er(netwib_threadlist_add_test());
  netwib_er(netwib_threadlist_del_test());
  netwib_er(netwib_threadlist_wait_test());

  return(NETWIB_ERR_OK);
}

