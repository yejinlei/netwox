#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err fevent(netwib_ptr infos,
                         netwib_consttime *pabstime,
                         netwib_bool *pevent)
{
  if (pevent != NULL) *pevent = NETWIB_TRUE;
  infos = infos; /* for compiler warning */
  pabstime = pabstime; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_wait_init_test(void)
{
  netwib_wait *pwait;

  puts("++++----\nTest of netwib_wait_init");

  netwib_er(netwib_wait_init(&fevent, NULL, NULL, &pwait));
  netwib_er(netwib_wait_close(&pwait));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_wait_close_test(void)
{
  puts("++++----\nTest of netwib_wait_close");
  puts("Redundant with netwib_wait_init");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_wait_init_io_test(void)
{
  netwib_wait *pwait;
  netwib_io *pio;
  netwib_buf buf1, buf2, bufdata;
  netwib_bool b;

  puts("++++----\nTest of netwib_wait_init_io");
  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  netwib_er(netwib_io_init_tlv(&buf1, &buf2, NETWIB_TRUE, &pio));

  netwib_er(netwib_wait_init_io(pio, NETWIB_IO_WAYTYPE_READ, &pwait));
  netwib_er(netwib_wait_wait1(pwait, NETWIB_TIME_ZERO, &b));
  if (b) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_buf_init_mallocdefault(&bufdata));
  netwib_er(netwib_buf_append_string("hello", &bufdata));
  netwib_er(netwib_tlv_append_buf(&bufdata, &buf1));
  netwib_er(netwib_buf_close(&bufdata));
  netwib_er(netwib_wait_wait1(pwait, NETWIB_TIME_ZERO, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_wait_close(&pwait));
  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err fthreadw(netwib_ptr infosin,
                          netwib_ptr *pinfosout)
{
  netwib_uint32 ui = *(netwib_uint32*)infosin;

  netwib_er(netwib_time_sleep_msec(ui));
  infosin = infosin; /* for compiler warning */
  pinfosout = pinfosout; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_wait_init_thread_end_test(void)
{
  netwib_wait *pwait;
  netwib_thread *pthread;
  netwib_uint32 ui;
  netwib_bool b;
  netwib_err ret;

  puts("++++----\nTest of netwib_wait_init_thread_end");

  ui = 200;
  netwib_er(netwib_thread_init(&fthreadw, &ui, &pthread));

  netwib_er(netwib_wait_init_thread_end(pthread, &ret, NULL, &pwait));
  netwib_er(netwib_wait_wait1(pwait, NETWIB_TIME_ZERO, &b));
  if (b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_wait_wait1(pwait, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_6);
  }
  if (ret != NETWIB_ERR_OK) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_er(netwib_thread_wait(pthread, NETWIB_TIME_INFINITE, &b, &ret, NULL));
  if (!b) {
    return(NETWIBTEST_ERR_4);
  }
  if (ret != NETWIB_ERR_OK) {
    return(NETWIBTEST_ERR_3);
  }

  netwib_er(netwib_wait_close(&pwait));
  netwib_er(netwib_thread_close(&pthread));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_wait_init_thread_cond_test(void)
{
  puts("++++----\nTest of netwib_wait_init_thread_cond");
  puts("Tested in netwox (modules/sockv.c)");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_wait_wait1_test(void)
{
  puts("++++----\nTest of netwib_wait_wait1");
  puts("Redundant with netwib_wait_init_thread");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_wait_wait5_test(void)
{
  netwib_wait *pwait[3];
  netwib_thread *pthread[3];
  netwib_uint32 ui[3];
  netwib_err ret[3];
  netwib_bool b[3];

  puts("++++----\nTest of netwib_wait_wait5");

  /* note : we use big sleep because my HP-UX is very slow in debugging mode,
     so does not synchronize correctly (no time to finish task) */
  ui[0] = 2000;
  netwib_er(netwib_thread_init(&fthreadw, &ui[0], &pthread[0]));
  ui[1] = 1000;
  netwib_er(netwib_thread_init(&fthreadw, &ui[1], &pthread[1]));
  ui[2] = 3000;
  netwib_er(netwib_thread_init(&fthreadw, &ui[2], &pthread[2]));

  netwib_er(netwib_wait_init_thread_end(pthread[0], &ret[0], NULL, &pwait[0]));
  netwib_er(netwib_wait_init_thread_end(pthread[1], &ret[1], NULL, &pwait[1]));
  netwib_er(netwib_wait_init_thread_end(pthread[2], &ret[2], NULL, &pwait[2]));

  netwib_er(netwib_wait_wait3(pwait[0], pwait[1], pwait[2],
                              NETWIB_TIME_INFINITE, &b[0], &b[1], &b[2]));
  if (!b[1]) {
    return(NETWIBTEST_ERR_6);
  }
  if (ret[1] != NETWIB_ERR_OK) {
    return(NETWIBTEST_ERR_7);
  }
  if (b[0] || b[2]) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_wait_close(&pwait[1]));
  netwib_er(netwib_thread_close(&pthread[1]));

  netwib_er(netwib_wait_wait2(pwait[0], pwait[2],
                              NETWIB_TIME_INFINITE, &b[0], &b[2]));
  if (!b[0]) {
    return(NETWIBTEST_ERR_6);
  }
  if (ret[0] != NETWIB_ERR_OK) {
    return(NETWIBTEST_ERR_7);
  }
  if (b[2]) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_wait_close(&pwait[0]));
  netwib_er(netwib_thread_close(&pthread[0]));

  netwib_er(netwib_wait_wait2(NULL, pwait[2],
                              NETWIB_TIME_INFINITE, NULL, &b[2]));
  if (!b[2]) {
    return(NETWIBTEST_ERR_6);
  }
  if (ret[2] != NETWIB_ERR_OK) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_wait_close(&pwait[2]));
  netwib_er(netwib_thread_close(&pthread[2]));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_wait(void)
{
  puts("###########################################\nTest of sys/wait");
  netwib_er(netwib_wait_init_test());
  netwib_er(netwib_wait_close_test());
  netwib_er(netwib_wait_init_io_test());
  netwib_er(netwib_wait_init_thread_end_test());
  netwib_er(netwib_wait_init_thread_cond_test());
  netwib_er(netwib_wait_wait1_test());
  netwib_er(netwib_wait_wait5_test());

  return(NETWIB_ERR_OK);
}
