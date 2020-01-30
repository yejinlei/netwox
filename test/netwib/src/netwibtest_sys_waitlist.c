#include "netwibtest.h"


/*-------------------------------------------------------------*/
static netwib_err netwib_waitlist_init_test(void)
{
  netwib_ring *pring;

  puts("++++----\nTest of netwib_waitlist_init");
  netwib_er(netwib_waitlist_init(&pring));
  netwib_er(netwib_waitlist_close(&pring));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_waitlist_add_test(void)
{
  netwib_ring *pring;

  puts("++++----\nTest of netwib_waitlist_add");

  netwib_er(netwib_waitlist_init(&pring));
  netwib_er(netwib_waitlist_add(pring, NULL, 123));
  netwib_er(netwib_waitlist_add(pring, NULL, 124));
  netwib_er(netwib_waitlist_add(pring, NULL, 125));
  netwib_er(netwib_waitlist_add(pring, NULL, 126));
  netwib_er(netwib_waitlist_add(pring, NULL, 127));
  netwib_er(netwib_waitlist_del(pring, 127));
  netwib_er(netwib_waitlist_del(pring, 11));
  netwib_er(netwib_waitlist_del(pring, 125));
  netwib_er(netwib_waitlist_del(pring, 123));
  netwib_er(netwib_waitlist_del(pring, 124));
  netwib_er(netwib_waitlist_del(pring, 126));
  netwib_er(netwib_waitlist_close(&pring));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_waitlist_del_test(void)
{
  puts("++++----\nTest of netwib_waitlist_del");
  puts("Redundant with netwib_waitlist_add");

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err fthreadwl(netwib_ptr infosin,
                            netwib_ptr *pinfosout)
{
  netwib_uint32 ui = *(netwib_uint32*)infosin;

  netwib_er(netwib_time_sleep_msec(ui));
  infosin = infosin; /* for compiler warning */
  pinfosout = pinfosout; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_waitlist_wait_test(void)
{
  netwib_ring *pring, *pringid;
  netwib_ring_index *pringindex;
  netwib_wait *pwait[3];
  netwib_thread *pthread[3];
  netwib_ptr ptr;
  netwib_uint32 ui[3], count;
  netwib_err ret[3];
  netwib_bool b;

  puts("++++----\nTest of netwib_waitlist_wait");

  /* note : we use big sleep because my HP-UX is very slow in debugging mode,
     so does not synchronize correctly (no time to finish task) */
  ui[0] = 2000;
  netwib_er(netwib_thread_init(&fthreadwl, &ui[0], &pthread[0]));
  ui[1] = 1000;
  netwib_er(netwib_thread_init(&fthreadwl, &ui[1], &pthread[1]));
  ui[2] = 3000;
  netwib_er(netwib_thread_init(&fthreadwl, &ui[2], &pthread[2]));

  netwib_er(netwib_wait_init_thread_end(pthread[0], &ret[0], NULL, &pwait[0]));
  netwib_er(netwib_wait_init_thread_end(pthread[1], &ret[1], NULL, &pwait[1]));
  netwib_er(netwib_wait_init_thread_end(pthread[2], &ret[2], NULL, &pwait[2]));

  netwib_er(netwib_waitlist_init(&pring));
  netwib_er(netwib_waitlist_add(pring, pwait[0], 0));
  netwib_er(netwib_waitlist_add(pring, pwait[1], 1));
  netwib_er(netwib_waitlist_add(pring, pwait[2], 2));

  netwib_er(netwib_waitlist_wait(pring, NETWIB_TIME_INFINITE, &b, &pringid));
  if (!b) {
    return(NETWIBTEST_ERR_6);
  }
  netwib_er(netwib_ring_ctl_get_count(pringid, &count));
  if (count != 1) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_ring_index_init(pringid, &pringindex));
  netwib_er(netwib_ring_index_next(pringindex, &ptr));
  if (netwib_waitlist_waitident_init_ptr(ptr) != 1) {
    return(NETWIBTEST_ERR_9);
  }
  if (ret[1] != NETWIB_ERR_OK) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  netwib_er(netwib_ring_close(&pringid, NETWIB_FALSE));
  netwib_er(netwib_waitlist_del(pring, 1));
  netwib_er(netwib_wait_close(&pwait[1]));
  netwib_er(netwib_thread_close(&pthread[1]));

  netwib_er(netwib_waitlist_wait(pring, NETWIB_TIME_INFINITE, &b, &pringid));
  if (!b) {
    return(NETWIBTEST_ERR_6);
  }
  netwib_er(netwib_ring_ctl_get_count(pringid, &count));
  if (count != 1) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_ring_index_init(pringid, &pringindex));
  netwib_er(netwib_ring_index_next(pringindex, &ptr));
  if (netwib_waitlist_waitident_init_ptr(ptr) != 0) {
    return(NETWIBTEST_ERR_9);
  }
  if (ret[0] != NETWIB_ERR_OK) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  netwib_er(netwib_ring_close(&pringid, NETWIB_FALSE));
  netwib_er(netwib_waitlist_del(pring, 0));
  netwib_er(netwib_wait_close(&pwait[0]));
  netwib_er(netwib_thread_close(&pthread[0]));

  netwib_er(netwib_waitlist_wait(pring, NETWIB_TIME_INFINITE, &b, &pringid));
  if (!b) {
    return(NETWIBTEST_ERR_6);
  }
  netwib_er(netwib_ring_ctl_get_count(pringid, &count));
  if (count != 1) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_ring_index_init(pringid, &pringindex));
  netwib_er(netwib_ring_index_next(pringindex, &ptr));
  if (netwib_waitlist_waitident_init_ptr(ptr) != 2) {
    return(NETWIBTEST_ERR_9);
  }
  if (ret[2] != NETWIB_ERR_OK) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_ring_index_close(&pringindex));
  netwib_er(netwib_ring_close(&pringid, NETWIB_FALSE));
  netwib_er(netwib_waitlist_del(pring, 2));
  netwib_er(netwib_wait_close(&pwait[2]));
  netwib_er(netwib_thread_close(&pthread[2]));

  netwib_er(netwib_waitlist_close(&pring));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_waitlist(void)
{
  puts("###########################################\nTest of sys/waitlist");
  netwib_er(netwib_waitlist_init_test());
  netwib_er(netwib_waitlist_add_test());
  netwib_er(netwib_waitlist_del_test());
  netwib_er(netwib_waitlist_wait_test());

  return(NETWIB_ERR_OK);
}

