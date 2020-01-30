#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_null_test(void)
{
  netwib_io *pio;

  puts("++++----\nTest of netwib_io_init_null");

  netwib_er(netwib_io_init_null(&pio));
  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_null_waitwrite(void)
{
  netwib_buf bufdata;
  netwib_io *pio;
  netwib_bool b;

  puts("++++----\nTest of wait, write [null]");

  netwib_er(netwib_io_init_null(&pio));

  netwib_er(netwib_io_wait_write(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_buf_init_ext_string("hello1", &bufdata));
  netwib_er(netwib_io_write(pio, &bufdata));

  netwib_er(netwib_io_wait_write(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_ctl_set_end_write(pio));
  netwib_eir(netwib_io_wait_write(pio, NETWIB_TIME_INFINITE, &b),
             NETWIB_ERR_LOOBJWRITENOTSUPPORTED);

  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_null_waitreadunread(void)
{
  netwib_buf bufdata;
  netwib_io *pio;
  netwib_bool b;

  puts("++++----\nTest of wait, read, unread [null]");

  netwib_er(netwib_buf_init_mallocdefault(&bufdata));

  netwib_er(netwib_io_init_null(&pio));
  netwib_er(netwib_io_wait_read(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_read(pio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, ""));
  netwib_er(netwib_io_close(&pio));

  netwib_er(netwib_buf_close(&bufdata));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_null_ctl(void)
{
  netwib_io_restype res;
  netwib_io *pio;

  puts("++++----\nTest of ctl [null]");

  netwib_er(netwib_io_init_null(&pio));

  netwib_er(netwib_io_ctl_get_res(pio, NETWIB_IO_WAYTYPE_RDWR, &res));
  if (res != NETWIB_IO_RESTYPE_NULL) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_mem_test(void)
{
  netwib_io *pio;
  netwib_buf buf1, buf2;

  puts("++++----\nTest of netwib_io_init_mem");

  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  netwib_er(netwib_io_init_mem(&buf1, &buf2, NETWIB_FALSE, &pio));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_buf_close(&buf1));
  netwib_er(netwib_buf_close(&buf2));

  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  netwib_er(netwib_io_init_mem(&buf1, &buf2, NETWIB_TRUE, &pio));
  netwib_er(netwib_io_close(&pio));

  netwib_er(netwib_io_init_mem(NULL, NULL, NETWIB_TRUE, &pio));
  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_mem_waitwrite(void)
{
  netwib_buf bufdata;
  netwib_io *pio;
  netwib_buf buf1, buf2;
  netwib_bool b;

  puts("++++----\nTest of wait, write [mem]");
  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  netwib_er(netwib_io_init_mem(&buf1, &buf2, NETWIB_TRUE, &pio));

  netwib_er(netwib_io_wait_write(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_buf_init_ext_string("hello1", &bufdata));
  netwib_er(netwib_io_write(pio, &bufdata));
  netwib_er(netwibtest_buf_content3(&buf1, ""));
  netwib_er(netwibtest_buf_content3(&buf2, "'hello1'"));
  netwib_er(netwib_buf_init_ext_string("hello2", &bufdata));
  netwib_er(netwib_io_write(pio, &bufdata));
  netwib_er(netwibtest_buf_content3(&buf1, ""));
  netwib_er(netwibtest_buf_content3(&buf2, "'hello1hello2'"));

  netwib_er(netwib_io_wait_write(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_ctl_set_end_write(pio));
  netwib_eir(netwib_io_wait_write(pio, NETWIB_TIME_INFINITE, &b),
             NETWIB_ERR_LOOBJWRITENOTSUPPORTED);

  netwib_er(netwib_io_close(&pio));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_mem_waitreadunread(void)
{
  netwib_buf bufdata;
  netwib_io *pio;
  netwib_buf buf1, buf2;
  netwib_bool b;

  puts("++++----\nTest of wait, read, unread [mem]");
  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  netwib_er(netwib_io_init_mem(&buf1, &buf2, NETWIB_TRUE, &pio));

  netwib_er(netwib_buf_init_mallocdefault(&bufdata));
  netwib_er(netwib_io_wait_read(pio, NETWIB_TIME_INFINITE, &b));
  if (b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_buf_append_string("hello1", &buf1));
  netwib_er(netwib_io_read(pio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'hello1'"));
  netwib_er(netwibtest_buf_content3(&buf1, ""));
  netwib_eir(netwib_io_read(pio, &bufdata), NETWIB_ERR_DATANOTAVAIL);
  netwib_er(netwib_io_unread(pio, &bufdata));
  netwib_er(netwibtest_buf_content3(&buf1, "'hello1'"));

  netwib_er(netwib_buf_close(&bufdata));

  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_mem_ctl(void)
{
  netwib_io_restype res;
  netwib_io *pio;

  puts("++++----\nTest of ctl [mem]");

  netwib_er(netwib_io_init_mem(NULL, NULL, NETWIB_TRUE, &pio));

  netwib_er(netwib_io_ctl_get_res(pio, NETWIB_IO_WAYTYPE_SUPPORTED, &res));
  if (res != NETWIB_IO_RESTYPE_MEM) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
typedef struct {
  netwib_buf bufrd;
  netwib_buf bufwr;
  netwib_thread_rwlock *plockrd;
  netwib_thread_rwlock *plockwr;
} test_mem_thread_st;
static netwib_err test_mem_thread_f(netwib_ptr infosin,
                                    netwib_ptr *pinfosout)
{
  test_mem_thread_st *pst = (test_mem_thread_st *)infosin;

  netwib_er(netwib_time_sleep_msec(100));
  netwib_er(netwib_thread_rwlock_wrlock(pst->plockrd, NETWIB_TIME_INFINITE,NULL));
  netwib_er(netwib_buf_append_string("hello1", &pst->bufrd));
  netwib_er(netwib_thread_rwlock_wrunlock(pst->plockrd));
  netwib_er(netwib_time_sleep_msec(100));
  netwib_er(netwib_thread_rwlock_wrlock(pst->plockrd, NETWIB_TIME_INFINITE,NULL));
  netwib_er(netwib_buf_append_string("hello2", &pst->bufrd));
  netwib_er(netwib_thread_rwlock_wrunlock(pst->plockrd));
  netwib_er(netwib_time_sleep_msec(100));
  netwib_er(netwib_thread_rwlock_rdlock(pst->plockwr, NETWIB_TIME_INFINITE,NULL));
  netwib_er(netwibtest_buf_content3(&pst->bufwr, "'hello1hello2'"));
  netwib_er(netwib_thread_rwlock_rdunlock(pst->plockwr));

  pinfosout = pinfosout; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_io_init_mem_lock_test(void)
{
  test_mem_thread_st st;
  netwib_thread *pthread;
  netwib_buf bufdata;
  netwib_bool b;
  netwib_io *pio;
  netwib_err ret;

  puts("++++----\nTest of netwib_io_init_mem_lock");

  netwib_er(netwib_buf_init_mallocdefault(&st.bufrd));
  netwib_er(netwib_buf_init_mallocdefault(&st.bufwr));
  netwib_er(netwib_thread_rwlock_init(&st.plockrd));
  netwib_er(netwib_thread_rwlock_init(&st.plockwr));

  netwib_er(netwib_io_init_mem_lock(st.plockrd, &st.bufrd,
                                    st.plockwr, &st.bufwr, NETWIB_TRUE, &pio));
  netwib_er(netwib_thread_init(&test_mem_thread_f, &st, &pthread));

  netwib_er(netwib_buf_init_mallocdefault(&bufdata));
  netwib_er(netwib_io_wait_read(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_read(pio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'hello1'"));
  netwib_er(netwib_io_wait_read(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_read(pio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'hello1hello2'"));
  netwib_er(netwib_io_write(pio, &bufdata));
  netwib_er(netwib_buf_close(&bufdata));

  netwib_er(netwib_thread_wait(pthread, NETWIB_TIME_INFINITE, NULL, &ret, NULL));
  if (ret != NETWIB_ERR_OK) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_thread_close(&pthread));

  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_tlv_test(void)
{
  netwib_io *pio;
  netwib_buf buf1, buf2;

  puts("++++----\nTest of netwib_io_init_tlv");

  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  netwib_er(netwib_io_init_tlv(&buf1, &buf2, NETWIB_FALSE, &pio));
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwib_buf_close(&buf1));
  netwib_er(netwib_buf_close(&buf2));

  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  netwib_er(netwib_io_init_tlv(&buf1, &buf2, NETWIB_TRUE, &pio));
  netwib_er(netwib_io_close(&pio));

  netwib_er(netwib_io_init_tlv(NULL, NULL, NETWIB_TRUE, &pio));
  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_tlv_waitwrite(void)
{
  netwib_buf bufdata;
  netwib_io *pio;
  netwib_buf buf1, buf2;
  netwib_bool b;

  puts("++++----\nTest of wait, write [tlv]");
  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  netwib_er(netwib_io_init_tlv(&buf1, &buf2, NETWIB_TRUE, &pio));

  netwib_er(netwib_io_wait_write(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_buf_init_ext_string("hello", &bufdata));
  netwib_er(netwib_io_write(pio, &bufdata));
  netwib_er(netwibtest_buf_content3(&buf1, ""));
  netwib_er(netwibtest_buf_content3(&buf2, "0000000100000005'hello'"));
  netwib_er(netwib_buf_init_ext_string("bye", &bufdata));
  netwib_er(netwib_io_write(pio, &bufdata));
  netwib_er(netwibtest_buf_content3(&buf1, ""));
  netwib_er(netwibtest_buf_content3(&buf2, "0000000100000005'hello' 0000000100000003'bye'"));

  netwib_er(netwib_io_wait_write(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_ctl_set_end_write(pio));
  netwib_eir(netwib_io_wait_write(pio, NETWIB_TIME_INFINITE, &b),
             NETWIB_ERR_LOOBJWRITENOTSUPPORTED);
  netwib_er(netwibtest_buf_content3(&buf2, "0000000100000005'hello' 0000000100000003'bye' 0000006400000000"));

  netwib_er(netwib_io_close(&pio));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_tlv_waitreadunread(void)
{
  netwib_buf bufdata1, bufdata2;
  netwib_io *pio;
  netwib_buf buf1, buf2;
  netwib_bool b;

  puts("++++----\nTest of wait, read, unread [tlv]");
  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  netwib_er(netwib_io_init_tlv(&buf1, &buf2, NETWIB_TRUE, &pio));

  netwib_er(netwib_buf_init_mallocdefault(&bufdata1));
  netwib_er(netwib_buf_init_mallocdefault(&bufdata2));

  netwib_er(netwib_io_wait_read(pio, NETWIB_TIME_INFINITE, &b));
  if (b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_buf_append_string("hello", &bufdata1));
  netwib_er(netwib_tlv_append_buf(&bufdata1, &buf1));
  netwib__buf_reinit(&bufdata1);

  netwib_er(netwib_io_read(pio, &bufdata2));
  netwib_er(netwibtest_buf_content3(&bufdata2, "'hello'"));
  netwib__buf_reinit(&bufdata2);
  netwib_er(netwibtest_buf_content3(&buf1, ""));

  netwib_eir(netwib_io_read(pio, &bufdata2), NETWIB_ERR_DATANOTAVAIL);
  netwib__buf_reinit(&bufdata2);

  netwib_er(netwib_buf_append_string("bye", &bufdata1));
  netwib_er(netwib_io_unread(pio, &bufdata1));
  netwib__buf_reinit(&bufdata1);
  netwib_er(netwibtest_buf_content3(&buf1, "0000000100000003'bye'"));

  netwib_er(netwib_tlv_append_end(&buf1));
  netwib_er(netwib_io_read(pio, &bufdata2));
  netwib_eir(netwib_io_read(pio, &bufdata2), NETWIB_ERR_DATAEND);

  netwib_er(netwib_buf_close(&bufdata1));
  netwib_er(netwib_buf_close(&bufdata2));

  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_tlv_ctl(void)
{
  netwib_io_restype res;
  netwib_io *pio;

  puts("++++----\nTest of ctl [tlv]");

  netwib_er(netwib_io_init_tlv(NULL, NULL, NETWIB_TRUE, &pio));

  netwib_er(netwib_io_ctl_get_res(pio, NETWIB_IO_WAYTYPE_SUPPORTED, &res));
  if (res != NETWIB_IO_RESTYPE_TLV) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
typedef struct {
  netwib_buf bufrd;
  netwib_buf bufwr;
  netwib_thread_rwlock *plockrd;
  netwib_thread_rwlock *plockwr;
} test_tlv_thread_st;
static netwib_err test_tlv_thread_f(netwib_ptr infosin,
                                    netwib_ptr *pinfosout)
{
  test_tlv_thread_st *pst = (test_tlv_thread_st *)infosin;
  netwib_buf tlvbuf;

  netwib_er(netwib_buf_init_mallocdefault(&tlvbuf));
  netwib_er(netwib_time_sleep_msec(100));
  netwib_er(netwib_thread_rwlock_wrlock(pst->plockrd, NETWIB_TIME_INFINITE,NULL));
  netwib_er(netwib_buf_append_string("hello1", &tlvbuf));
  netwib_er(netwib_tlv_append_buf(&tlvbuf, &pst->bufrd));
  netwib_er(netwib_thread_rwlock_wrunlock(pst->plockrd));
  netwib_er(netwib_time_sleep_msec(100));
  netwib_er(netwib_thread_rwlock_wrlock(pst->plockrd, NETWIB_TIME_INFINITE,NULL));
  netwib__buf_reinit(&tlvbuf);
  netwib_er(netwib_buf_append_string("hello2", &tlvbuf));
  netwib_er(netwib_tlv_append_buf(&tlvbuf, &pst->bufrd));
  netwib_er(netwib_thread_rwlock_wrunlock(pst->plockrd));
  netwib_er(netwib_time_sleep_msec(100));
  netwib_er(netwib_thread_rwlock_rdlock(pst->plockwr, NETWIB_TIME_INFINITE,NULL));
  netwib_er(netwibtest_buf_content3(&pst->bufwr, "000000010000000C'hello1hello2'"));
  netwib_er(netwib_thread_rwlock_rdunlock(pst->plockwr));
  netwib_er(netwib_buf_close(&tlvbuf));

  pinfosout = pinfosout; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_io_init_tlv_lock_test(void)
{
  test_tlv_thread_st st;
  netwib_thread *pthread;
  netwib_buf bufdata, tlvbuf;
  netwib_bool b;
  netwib_io *pio;
  netwib_err ret;

  puts("++++----\nTest of netwib_io_init_tlv_lock");

  netwib_er(netwib_buf_init_mallocdefault(&st.bufrd));
  netwib_er(netwib_buf_init_mallocdefault(&st.bufwr));
  netwib_er(netwib_thread_rwlock_init(&st.plockrd));
  netwib_er(netwib_thread_rwlock_init(&st.plockwr));

  netwib_er(netwib_io_init_tlv_lock(st.plockrd, &st.bufrd,
                                    st.plockwr, &st.bufwr, NETWIB_TRUE, &pio));
  netwib_er(netwib_thread_init(&test_tlv_thread_f, &st, &pthread));

  netwib_er(netwib_buf_init_mallocdefault(&bufdata));
  netwib_er(netwib_buf_init_mallocdefault(&tlvbuf));
  netwib_er(netwib_io_wait_read(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_read(pio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'hello1'"));
  netwib_er(netwib_io_wait_read(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_read(pio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'hello1hello2'"));
  netwib_er(netwib_io_write(pio, &bufdata));
  netwib_er(netwib_buf_close(&bufdata));
  netwib_er(netwib_buf_close(&tlvbuf));

  netwib_er(netwib_thread_wait(pthread, NETWIB_TIME_INFINITE, NULL, &ret, NULL));
  if (ret != NETWIB_ERR_OK) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_thread_close(&pthread));

  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_data_test(void)
{
  puts("++++----\nTest of netwib_io_init_data");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_netwib_io_init_data_line(void)
{
  netwib_io *pio;

  puts("++++----\nTest of define netwib_io_init_data_line");

  netwib_er(netwib_io_init_data_line(&pio));
  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_data_line_waitwrite(void)
{
  netwib_buf bufdata;
  netwib_io *ptlvio, *plineio;
  netwib_buf buf1, buf2;
  netwib_bool b;

  puts("++++----\nTest of wait, write [line]");
  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  netwib_er(netwib_io_init_tlv(&buf1, &buf2, NETWIB_TRUE, &ptlvio));
  netwib_er(netwib_io_init_data_line(&plineio));
  netwib_er(netwib_io_plug_write(plineio, ptlvio));

  netwib_er(netwib_io_wait_write(plineio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_buf_init_mallocdefault(&bufdata));
  netwib_er(netwib_buf_append_string("hello", &bufdata));
  netwib_er(netwib_io_write(plineio, &bufdata));
  netwib_er(netwib_buf_close(&bufdata));
  netwib_er(netwibtest_buf_content3(&buf1, ""));
  netwib_er(netwibtest_buf_content3(&buf2, "0000000100000007'hello'0D0A"));

  netwib_er(netwib_io_close(&plineio));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_data_line_waitreadunread(void)
{
  netwib_buf bufdata;
  netwib_io *ptlvio, *plineio;
  netwib_buf buf, buftxt;
  netwib_bool b;

  puts("++++----\nTest of wait, read, unread [line]");
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_io_init_tlv(&buf, NULL, NETWIB_TRUE, &ptlvio));
  netwib_er(netwib_io_init_data_line(&plineio));
  netwib_er(netwib_io_plug_read(plineio, ptlvio));

  netwib_er(netwib_buf_init_mallocdefault(&bufdata));

  netwib_er(netwib_io_wait_read(plineio, NETWIB_TIME_ZERO, &b));
  if (b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_buf_init_ext_string("hello\n", &buftxt));
  netwib_er(netwib_tlv_append_buf(&buftxt, &buf));
  netwib_er(netwib_io_read(plineio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'hello'"));
  netwib_er(netwibtest_buf_content3(&buf, ""));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_init_ext_string("hel\nlo", &buftxt));
  netwib_er(netwib_tlv_append_buf(&buftxt, &buf));
  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_io_read(plineio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'hel'"));
  netwib__buf_reinit(&bufdata);
  netwib_eir(netwib_io_read(plineio, &bufdata), NETWIB_ERR_DATANOTAVAIL);
  netwib_er(netwib_tlv_append_end(&buf));
  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_io_read(plineio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'lo'"));
  netwib_er(netwib_io_close(&plineio));

  /**/
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_io_init_tlv(&buf, NULL, NETWIB_TRUE, &ptlvio));
  netwib_er(netwib_io_init_data_line(&plineio));
  netwib_er(netwib_io_plug_read(plineio, ptlvio));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_init_ext_string("a\r\nb\rc\nd\r\r\n\n", &buftxt));
  netwib_er(netwib_tlv_append_buf(&buftxt, &buf));
  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_io_read(plineio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'a'"));
  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_io_read(plineio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'b'"));
  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_io_read(plineio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'c'"));
  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_io_read(plineio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'d'"));
  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_io_read(plineio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, ""));
  netwib_er(netwib_io_close(&plineio));

  netwib_er(netwib_buf_close(&bufdata));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_data_line_ctl(void)
{
  netwib_io_restype res;
  netwib_io *ptlvio, *plineio;
  netwib_buf buf;

  puts("++++----\nTest of ctl [line]");

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_io_init_tlv(&buf, NULL, NETWIB_TRUE, &ptlvio));
  netwib_er(netwib_io_init_data_line(&plineio));
  netwib_er(netwib_io_plug_read(plineio, ptlvio));

  netwib_er(netwib_io_ctl_get_res(plineio, NETWIB_IO_WAYTYPE_SUPPORTED, &res));
  if (res != NETWIB_IO_RESTYPE_TLV) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_io_close(&plineio));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err test_netwib_io_init_data_chunk(void)
{
  netwib_io *pio;

  puts("++++----\nTest of define netwib_io_init_data_chunk");

  netwib_er(netwib_io_init_data_chunk(&pio));
  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_data_chunk_waitwrite(void)
{
  netwib_buf bufdata;
  netwib_io *ptlvio, *pchunkio;
  netwib_buf buf1, buf2;
  netwib_bool b;

  puts("++++----\nTest of wait, write [chunk]");
  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  netwib_er(netwib_io_init_tlv(&buf1, &buf2, NETWIB_TRUE, &ptlvio));
  netwib_er(netwib_io_init_data_chunk(&pchunkio));
  netwib_er(netwib_io_plug_write(pchunkio, ptlvio));

  netwib_er(netwib_io_wait_write(pchunkio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_ctl_set_data_chunk_minsize(pchunkio,
                                                 NETWIB_IO_WAYTYPE_WRITE, 2));
  netwib_er(netwib_io_ctl_set_data_chunk_maxsize(pchunkio,
                                                 NETWIB_IO_WAYTYPE_WRITE, 4));
  netwib_er(netwib_buf_init_mallocdefault(&bufdata));

  netwib_er(netwib_buf_append_string("hello", &bufdata));
  netwib_er(netwib_io_write(pchunkio, &bufdata));
  netwib_er(netwibtest_buf_content3(&buf1, ""));
  netwib_er(netwibtest_buf_content3(&buf2, "0000000100000004'hell'"));

  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_buf_append_string("bigworld", &bufdata));
  netwib_er(netwib_io_write(pchunkio, &bufdata));
  netwib_er(netwibtest_buf_content3(&buf2, "0000000100000004'hell' 0000000100000004'obig' 0000000100000004'worl'"));

  netwib_er(netwib_io_ctl_set_end_write(pchunkio));
  netwib_er(netwibtest_buf_content3(&buf2, "0000000100000004'hell' 0000000100000004'obig' 0000000100000004'worl' 0000000100000001'd' 0000006400000000"));

  netwib_er(netwib_buf_close(&bufdata));
  netwib_er(netwib_io_close(&pchunkio));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_data_chunk_waitreadunread(void)
{
  netwib_buf bufdata;
  netwib_io *ptlvio, *pchunkio;
  netwib_buf buf, buftxt;
  netwib_bool b;

  puts("++++----\nTest of wait, read, unread [chunk]");
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_io_init_tlv(&buf, NULL, NETWIB_TRUE, &ptlvio));
  netwib_er(netwib_io_init_data_chunk(&pchunkio));
  netwib_er(netwib_io_plug_read(pchunkio, ptlvio));

  netwib_er(netwib_buf_init_mallocdefault(&bufdata));

  netwib_er(netwib_io_wait_read(pchunkio, NETWIB_TIME_ZERO, &b));
  if (b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_buf_init_ext_string("hello", &buftxt));
  netwib_er(netwib_tlv_append_buf(&buftxt, &buf));
  netwib_er(netwib_io_read(pchunkio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'hello'"));
  netwib_er(netwibtest_buf_content3(&buf, ""));

  netwib_er(netwib_io_ctl_set_data_chunk_minsize(pchunkio,
                                                 NETWIB_IO_WAYTYPE_READ, 3));
  netwib_er(netwib_io_ctl_set_data_chunk_maxsize(pchunkio,
                                                 NETWIB_IO_WAYTYPE_READ, 3));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_init_ext_string("hello", &buftxt));
  netwib_er(netwib_tlv_append_buf(&buftxt, &buf));
  netwib_er(netwib_buf_init_ext_string("my", &buftxt));
  netwib_er(netwib_tlv_append_buf(&buftxt, &buf));
  netwib_er(netwib_buf_init_ext_string("name", &buftxt));
  netwib_er(netwib_tlv_append_buf(&buftxt, &buf));
  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_io_read(pchunkio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'hel'"));
  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_io_read(pchunkio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'lom'"));
  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_io_read(pchunkio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'yna'"));
  netwib__buf_reinit(&bufdata);
  netwib_eir(netwib_io_read(pchunkio, &bufdata), NETWIB_ERR_DATANOTAVAIL);
  netwib_er(netwib_tlv_append_end(&buf));
  netwib_er(netwib_io_read(pchunkio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'me'"));
  netwib_eir(netwib_io_read(pchunkio, &bufdata), NETWIB_ERR_DATAEND);

  netwib_er(netwib_io_close(&pchunkio));
  netwib_er(netwib_buf_close(&bufdata));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_data_chunk_ctl(void)
{
  netwib_io_restype res;
  netwib_io *ptlvio, *pchunkio;
  netwib_buf buf;
  netwib_uint32 ui;

  puts("++++----\nTest of ctl [chunk]");

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_io_init_tlv(&buf, NULL, NETWIB_TRUE, &ptlvio));
  netwib_er(netwib_io_init_data_chunk(&pchunkio));
  netwib_er(netwib_io_plug_read(pchunkio, ptlvio));

  netwib_er(netwib_io_ctl_get_res(pchunkio, NETWIB_IO_WAYTYPE_SUPPORTED, &res));
  if (res != NETWIB_IO_RESTYPE_TLV) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_io_ctl_set_data_chunk_minsize(pchunkio,
                                                 NETWIB_IO_WAYTYPE_SUPPORTED,
                                                 10));
  netwib_er(netwib_io_ctl_get_data_chunk_minsize(pchunkio,
                                                 NETWIB_IO_WAYTYPE_SUPPORTED,
                                                 &ui));
  if (ui != 10) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_io_ctl_set_data_chunk_maxsize(pchunkio,
                                                 NETWIB_IO_WAYTYPE_SUPPORTED,
                                                 11));
  netwib_er(netwib_io_ctl_get_data_chunk_maxsize(pchunkio,
                                                 NETWIB_IO_WAYTYPE_SUPPORTED,
                                                 &ui));
  if (ui != 11) {
    return(NETWIBTEST_ERR_6);
  }

  netwib_er(netwib_io_close(&pchunkio));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err test_netwib_io_init_data_fixed(void)
{
  netwib_io *pio;

  puts("++++----\nTest of define netwib_io_init_data_fixed");

  netwib_er(netwib_io_init_data_fixed(&pio));
  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_data_fixed_waitwrite(void)
{
  netwib_buf bufdata;
  netwib_io *ptlvio, *pfixedio;
  netwib_buf buf1, buf2;
  netwib_bool b;

  puts("++++----\nTest of wait, write [fixed]");
  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  netwib_er(netwib_io_init_tlv(&buf1, &buf2, NETWIB_TRUE, &ptlvio));
  netwib_er(netwib_io_init_data_fixed(&pfixedio));
  netwib_er(netwib_io_plug_write(pfixedio, ptlvio));

  netwib_er(netwib_io_wait_write(pfixedio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_ctl_set_data_fixed_size(pfixedio, NETWIB_IO_WAYTYPE_WRITE,
                                              4));
  netwib_er(netwib_buf_init_mallocdefault(&bufdata));

  netwib_er(netwib_buf_append_string("hello", &bufdata));
  netwib_er(netwib_io_write(pfixedio, &bufdata));
  netwib_er(netwibtest_buf_content3(&buf1, ""));
  netwib_er(netwibtest_buf_content3(&buf2, "0000000100000004'hell'"));

  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_buf_append_string("bigworld", &bufdata));
  netwib_er(netwib_io_write(pfixedio, &bufdata));
  netwib_er(netwibtest_buf_content3(&buf2, "0000000100000004'hell' 0000000100000004'obig' 0000000100000004'worl'"));

  netwib_er(netwib_io_ctl_set_end_write(pfixedio));
  netwib_er(netwibtest_buf_content3(&buf2, "0000000100000004'hell' 0000000100000004'obig' 0000000100000004'worl' 0000000100000001'd' 0000006400000000"));

  netwib_er(netwib_buf_close(&bufdata));
  netwib_er(netwib_io_close(&pfixedio));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_data_fixed_waitreadunread(void)
{
  netwib_buf bufdata;
  netwib_io *ptlvio, *pfixedio;
  netwib_buf buf, buftxt;
  netwib_bool b;

  puts("++++----\nTest of wait, read, unread [fixed]");
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_io_init_tlv(&buf, NULL, NETWIB_TRUE, &ptlvio));
  netwib_er(netwib_io_init_data_fixed(&pfixedio));
  netwib_er(netwib_io_plug_read(pfixedio, ptlvio));

  netwib_er(netwib_buf_init_mallocdefault(&bufdata));

  netwib_er(netwib_io_wait_read(pfixedio, NETWIB_TIME_ZERO, &b));
  if (b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_buf_init_ext_string("hello", &buftxt));
  netwib_er(netwib_tlv_append_buf(&buftxt, &buf));
  netwib_er(netwib_io_ctl_set_data_fixed_size(pfixedio,
                                              NETWIB_IO_WAYTYPE_READ, 3));
  netwib_er(netwib_io_read(pfixedio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'hel'"));
  netwib_er(netwibtest_buf_content3(&buf, ""));
  netwib_er(netwib_io_ctl_set_data_fixed_size(pfixedio,
                                              NETWIB_IO_WAYTYPE_READ, 2));
  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_io_read(pfixedio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'lo'"));

  netwib_er(netwib_io_ctl_set_data_fixed_size(pfixedio,
                                              NETWIB_IO_WAYTYPE_READ, 3));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_init_ext_string("hello", &buftxt));
  netwib_er(netwib_tlv_append_buf(&buftxt, &buf));
  netwib_er(netwib_buf_init_ext_string("my", &buftxt));
  netwib_er(netwib_tlv_append_buf(&buftxt, &buf));
  netwib_er(netwib_buf_init_ext_string("name", &buftxt));
  netwib_er(netwib_tlv_append_buf(&buftxt, &buf));
  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_io_read(pfixedio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'hel'"));
  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_io_read(pfixedio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'lom'"));
  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_io_read(pfixedio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'yna'"));
  netwib__buf_reinit(&bufdata);
  netwib_eir(netwib_io_read(pfixedio, &bufdata), NETWIB_ERR_DATANOTAVAIL);
  netwib_er(netwib_tlv_append_end(&buf));
  netwib_er(netwib_io_read(pfixedio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'me'"));
  netwib_eir(netwib_io_read(pfixedio, &bufdata), NETWIB_ERR_DATAEND);

  netwib_er(netwib_io_close(&pfixedio));
  netwib_er(netwib_buf_close(&bufdata));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_data_fixed_ctl(void)
{
  netwib_io_restype res;
  netwib_io *ptlvio, *pfixedio;
  netwib_buf buf;
  netwib_uint32 ui;

  puts("++++----\nTest of ctl [fixed]");

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_io_init_tlv(&buf, NULL, NETWIB_TRUE, &ptlvio));
  netwib_er(netwib_io_init_data_fixed(&pfixedio));
  netwib_er(netwib_io_plug_read(pfixedio, ptlvio));

  netwib_er(netwib_io_ctl_get_res(pfixedio, NETWIB_IO_WAYTYPE_SUPPORTED, &res));
  if (res != NETWIB_IO_RESTYPE_TLV) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_io_ctl_set_data_fixed_size(pfixedio,
                                              NETWIB_IO_WAYTYPE_SUPPORTED,
                                              10));
  netwib_er(netwib_io_ctl_get_data_fixed_size(pfixedio,
                                              NETWIB_IO_WAYTYPE_SUPPORTED,
                                              &ui));
  if (ui != 10) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_io_close(&pfixedio));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err test_netwib_io_init_data_transparent(void)
{
  netwib_io *pio;

  puts("++++----\nTest of define netwib_io_init_data_transparent");

  netwib_er(netwib_io_init_data_transparent(&pio));
  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_data_transparent_waitwrite(void)
{
  netwib_buf bufdata;
  netwib_io *ptlvio, *ptransparentio;
  netwib_buf buf1, buf2;
  netwib_bool b;

  puts("++++----\nTest of wait, write [transparent]");
  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  netwib_er(netwib_io_init_tlv(&buf1, &buf2, NETWIB_TRUE, &ptlvio));
  netwib_er(netwib_io_init_data_transparent(&ptransparentio));
  netwib_er(netwib_io_plug_write(ptransparentio, ptlvio));

  netwib_er(netwib_io_wait_write(ptransparentio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_buf_init_mallocdefault(&bufdata));

  netwib_er(netwib_buf_append_string("hello", &bufdata));
  netwib_er(netwib_io_write(ptransparentio, &bufdata));
  netwib_er(netwibtest_buf_content3(&buf1, ""));
  netwib_er(netwibtest_buf_content3(&buf2, "0000000100000005'hello'"));

  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_buf_append_string("bigworld", &bufdata));
  netwib_er(netwib_io_write(ptransparentio, &bufdata));
  netwib_er(netwibtest_buf_content3(&buf2, "0000000100000005'hello' 0000000100000008'bigworld'"));

  netwib_er(netwib_io_ctl_set_end_write(ptransparentio));
  netwib_er(netwibtest_buf_content3(&buf2, "0000000100000005'hello' 0000000100000008'bigworld' 0000006400000000"));

  netwib_er(netwib_buf_close(&bufdata));
  netwib_er(netwib_io_close(&ptransparentio));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_data_transparent_waitreadunread(void)
{
  netwib_buf bufdata;
  netwib_io *ptlvio, *ptransparentio;
  netwib_buf buf, buftxt;
  netwib_bool b;

  puts("++++----\nTest of wait, read, unread [transparent]");
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_io_init_tlv(&buf, NULL, NETWIB_TRUE, &ptlvio));
  netwib_er(netwib_io_init_data_transparent(&ptransparentio));
  netwib_er(netwib_io_plug_read(ptransparentio, ptlvio));

  netwib_er(netwib_buf_init_mallocdefault(&bufdata));

  netwib_er(netwib_io_wait_read(ptransparentio, NETWIB_TIME_ZERO, &b));
  if (b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_buf_init_ext_string("hello", &buftxt));
  netwib_er(netwib_tlv_append_buf(&buftxt, &buf));
  netwib_er(netwib_io_read(ptransparentio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'hello'"));
  netwib_er(netwibtest_buf_content3(&buf, ""));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_init_ext_string("hello", &buftxt));
  netwib_er(netwib_tlv_append_buf(&buftxt, &buf));
  netwib_er(netwib_buf_init_ext_string("my", &buftxt));
  netwib_er(netwib_tlv_append_buf(&buftxt, &buf));
  netwib_er(netwib_buf_init_ext_string("name", &buftxt));
  netwib_er(netwib_tlv_append_buf(&buftxt, &buf));
  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_io_read(ptransparentio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'hello'"));
  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_io_read(ptransparentio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'my'"));
  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_tlv_append_end(&buf));
  netwib_er(netwib_io_read(ptransparentio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'name'"));
  netwib_eir(netwib_io_read(ptransparentio, &bufdata), NETWIB_ERR_DATAEND);

  netwib_er(netwib_io_close(&ptransparentio));
  netwib_er(netwib_buf_close(&bufdata));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_data_transparent_ctl(void)
{
  netwib_io_restype res;
  netwib_io *ptlvio, *ptransparentio;
  netwib_buf buf;

  puts("++++----\nTest of ctl [transparent]");

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_io_init_tlv(&buf, NULL, NETWIB_TRUE, &ptlvio));
  netwib_er(netwib_io_init_data_transparent(&ptransparentio));
  netwib_er(netwib_io_plug_read(ptransparentio, ptlvio));

  netwib_er(netwib_io_ctl_get_res(ptransparentio, NETWIB_IO_WAYTYPE_SUPPORTED, &res));
  if (res != NETWIB_IO_RESTYPE_TLV) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_io_close(&ptransparentio));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_storage_test(void)
{
  netwib_io *pio;

  puts("++++----\nTest of netwib_io_init_storage");

  netwib_er(netwib_io_init_storage(&pio));
  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_storage_waitwrite(void)
{
  netwib_buf bufdata;
  netwib_io *ptlvio, *pstorageio;
  netwib_buf buf1, buf2;
  netwib_bool b;

  puts("++++----\nTest of wait, write [storage]");
  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  netwib_er(netwib_io_init_tlv(&buf1, &buf2, NETWIB_TRUE, &ptlvio));
  netwib_er(netwib_io_init_storage(&pstorageio));
  netwib_er(netwib_io_plug_write(pstorageio, ptlvio));

  netwib_er(netwib_io_wait_write(pstorageio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_buf_init_mallocdefault(&bufdata));

  netwib_er(netwib_buf_append_string("hello", &bufdata));
  netwib_er(netwib_io_write(pstorageio, &bufdata));
  netwib_er(netwibtest_buf_content3(&buf1, ""));
  netwib_er(netwibtest_buf_content3(&buf2, "0000000100000005'hello'"));

  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_buf_append_string("bye", &bufdata));
  netwib_er(netwib_io_write(pstorageio, &bufdata));
  netwib_er(netwib_io_ctl_set_storage_flush(pstorageio));
  netwib_er(netwibtest_buf_content3(&buf2, "0000000100000005'hello' 0000000100000003'bye'"));

  netwib_er(netwib_io_ctl_set_end_write(pstorageio));
  netwib_er(netwibtest_buf_content3(&buf2, "0000000100000005'hello' 0000000100000003'bye' 0000006400000000"));

  netwib_er(netwib_buf_close(&bufdata));
  netwib_er(netwib_io_close(&pstorageio));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_storage_waitreadunread(void)
{
  netwib_buf bufdata;
  netwib_io *ptlvio, *pstorageio;
  netwib_buf buf, buftxt;
  netwib_bool b;

  puts("++++----\nTest of wait, read, unread [storage]");
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_io_init_tlv(&buf, NULL, NETWIB_TRUE, &ptlvio));
  netwib_er(netwib_io_init_storage(&pstorageio));
  netwib_er(netwib_io_plug_read(pstorageio, ptlvio));

  netwib_er(netwib_buf_init_mallocdefault(&bufdata));

  netwib_er(netwib_io_wait_read(pstorageio, NETWIB_TIME_ZERO, &b));
  if (b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_buf_init_ext_string("hello", &buftxt));
  netwib_er(netwib_tlv_append_buf(&buftxt, &buf));
  netwib_er(netwib_io_read(pstorageio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'hello'"));
  netwib_er(netwibtest_buf_content3(&buf, ""));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_init_ext_string("hello", &buftxt));
  netwib_er(netwib_tlv_append_buf(&buftxt, &buf));
  netwib_er(netwib_buf_init_ext_string("my", &buftxt));
  netwib_er(netwib_tlv_append_buf(&buftxt, &buf));
  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_io_read(pstorageio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'hello'"));
  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_io_read(pstorageio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'my'"));
  netwib__buf_reinit(&bufdata);
  netwib_eir(netwib_io_read(pstorageio, &bufdata), NETWIB_ERR_DATANOTAVAIL);
  netwib_er(netwib_tlv_append_end(&buf));
  netwib_eir(netwib_io_read(pstorageio, &bufdata), NETWIB_ERR_DATAEND);

  netwib_er(netwib_io_close(&pstorageio));
  netwib_er(netwib_buf_close(&bufdata));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_storage_ctl(void)
{
  netwib_io_restype res;
  netwib_io *ptlvio, *pstorageio;
  netwib_buf buf;

  puts("++++----\nTest of ctl [storage]");

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_io_init_tlv(&buf, NULL, NETWIB_TRUE, &ptlvio));
  netwib_er(netwib_io_init_storage(&pstorageio));
  netwib_er(netwib_io_plug_write(pstorageio, ptlvio));

  netwib_er(netwib_io_ctl_get_res(pstorageio, NETWIB_IO_WAYTYPE_SUPPORTED, &res));
  if (res != NETWIB_IO_RESTYPE_TLV) {
    return(NETWIBTEST_ERR_7);
  }

  netwib_er(netwib_io_ctl_set_storage_flush(pstorageio));

  netwib_er(netwib_io_close(&pstorageio));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_rdwr_test(void)
{
  netwib_io *pio, *pio1, *pio2;
  netwib_buf buf1, buf2;

  puts("++++----\nTest of netwib_io_init_rdwr");

  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_io_init_mem(&buf1, NULL, NETWIB_TRUE, &pio1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  netwib_er(netwib_io_init_mem(NULL, &buf2, NETWIB_TRUE, &pio2));
  netwib_er(netwib_io_init_rdwr(pio1, pio2, NETWIB_TRUE, &pio));
  netwib_er(netwib_io_close(&pio));

  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_io_init_mem(&buf1, NULL, NETWIB_TRUE, &pio1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  netwib_er(netwib_io_init_mem(NULL, &buf2, NETWIB_TRUE, &pio2));
  netwib_er(netwib_io_init_rdwr(pio1, pio2, NETWIB_TRUE, &pio));
  netwib_er(netwib_io_close(&pio1)); /* will not close because it is used */
  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_rdwr_waitwrite(void)
{
  netwib_buf bufdata;
  netwib_io *pio, *pio1, *pio2;
  netwib_buf buf1, buf2;
  netwib_bool b;

  puts("++++----\nTest of wait, write [rdwr]");
  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_io_init_mem(&buf1, NULL, NETWIB_TRUE, &pio1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  netwib_er(netwib_io_init_mem(NULL, &buf2, NETWIB_TRUE, &pio2));
  netwib_er(netwib_io_init_rdwr(pio1, pio2, NETWIB_TRUE, &pio));

  netwib_er(netwib_io_wait_write(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_buf_init_ext_string("hello1", &bufdata));
  netwib_er(netwib_io_write(pio, &bufdata));
  netwib_er(netwibtest_buf_content3(&buf1, ""));
  netwib_er(netwibtest_buf_content3(&buf2, "'hello1'"));
  netwib_er(netwib_buf_init_ext_string("hello2", &bufdata));
  netwib_er(netwib_io_write(pio, &bufdata));
  netwib_er(netwibtest_buf_content3(&buf1, ""));
  netwib_er(netwibtest_buf_content3(&buf2, "'hello1hello2'"));

  netwib_er(netwib_io_wait_write(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_ctl_set_end_write(pio));
  netwib_eir(netwib_io_wait_write(pio, NETWIB_TIME_INFINITE, &b),
             NETWIB_ERR_LOOBJWRITENOTSUPPORTED);

  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_rdwr_waitreadunread(void)
{
  netwib_buf bufdata;
  netwib_io *pio, *pio1, *pio2;
  netwib_buf buf1, buf2;
  netwib_bool b;

  puts("++++----\nTest of wait, read, unread [rdwr]");
  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_io_init_mem(&buf1, NULL, NETWIB_TRUE, &pio1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  netwib_er(netwib_io_init_mem(NULL, &buf2, NETWIB_TRUE, &pio2));
  netwib_er(netwib_io_init_rdwr(pio1, pio2, NETWIB_TRUE, &pio));

  netwib_er(netwib_buf_init_mallocdefault(&bufdata));

  netwib_er(netwib_io_wait_read(pio, NETWIB_TIME_INFINITE, &b));
  if (b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_buf_append_string("hello1", &buf1));
  netwib_er(netwib_io_read(pio, &bufdata));
  netwib_er(netwibtest_buf_content3(&bufdata, "'hello1'"));
  netwib_er(netwibtest_buf_content3(&buf1, ""));
  netwib_eir(netwib_io_read(pio, &bufdata), NETWIB_ERR_DATANOTAVAIL);
  netwib_er(netwib_io_unread(pio, &bufdata));
  netwib_er(netwibtest_buf_content3(&buf1, "'hello1'"));

  netwib_er(netwib_buf_close(&bufdata));

  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_rdwr_ctl(void)
{
  netwib_io_restype res;
  netwib_io *pio, *pio1, *pio2;
  netwib_buf buf1, buf2;

  puts("++++----\nTest of ctl [rdwr]");
  netwib_er(netwib_buf_init_mallocdefault(&buf1));
  netwib_er(netwib_io_init_mem(&buf1, NULL, NETWIB_TRUE, &pio1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  netwib_er(netwib_io_init_mem(NULL, &buf2, NETWIB_TRUE, &pio2));
  netwib_er(netwib_io_init_rdwr(pio1, pio2, NETWIB_TRUE, &pio));

  netwib_er(netwib_io_ctl_get_res(pio, NETWIB_IO_WAYTYPE_READ, &res));
  if (res != NETWIB_IO_RESTYPE_MEM) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_ctl_get_res(pio, NETWIB_IO_WAYTYPE_WRITE, &res));
  if (res != NETWIB_IO_RESTYPE_MEM) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_io_ctl_get_res(pio, NETWIB_IO_WAYTYPE_RDWR, &res));
  if (res != NETWIB_IO_RESTYPE_MEM) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
#define NETWIBTESTVAR_EXEC "./exec/execu.sh"
#define NETWIBTESTVAR_WANT1 "'Hello' 0A"
#define NETWIBTESTVAR_WANT2 "'Read hello1' 0A"
#elif defined NETWIBDEF_SYSNAME_Windows
#define NETWIBTESTVAR_EXEC "./exec/execw.exe"
#define NETWIBTESTVAR_WANT1 "'Hello' 0D0A"
#define NETWIBTESTVAR_WANT2 "'Read hello1' 0D0A"
#else
#error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_exec_test(void)
{
  netwib_io *pio;
  netwib_buf bufcmd;
  netwib_bool exitednormally;
  netwib_uint32 returnedvalue;

  puts("++++----\nTest of netwib_io_init_exec");

  netwib_er(netwib_buf_init_ext_string(NETWIBTESTVAR_EXEC, &bufcmd));
  netwib_er(netwib_io_init_exec(&bufcmd, NETWIB_IO_WAYTYPE_RDWR, NETWIB_FALSE,
                                &exitednormally, &returnedvalue, &pio));
  netwib_er(netwib_io_close(&pio));
  if (!exitednormally) {
    return(NETWIBTEST_ERR_9);
  }
  if (returnedvalue != 255) {
    return(NETWIBTEST_ERR_8);
  }

  netwib_er(netwib_buf_init_ext_string("./exec/donoexists", &bufcmd));
  netwib_eir(netwib_io_init_exec(&bufcmd, NETWIB_IO_WAYTYPE_RDWR, NETWIB_FALSE,
                                 &exitednormally, &returnedvalue, &pio),
             NETWIB_ERR_LOCANTEXEC);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_exec_waitwritereadunread(void)
{
  netwib_io *pio;
  netwib_buf bufcmd, bufdatar, bufdataw;
  netwib_bool exitednormally;
  netwib_uint32 returnedvalue;
  netwib_bool b;

  puts("++++----\nTest of wait, write, read, unread [exec]");
  netwib_er(netwib_buf_init_ext_string("hello1\n", &bufdataw));
  netwib_er(netwib_buf_init_mallocdefault(&bufdatar));

  /**/
  netwib_er(netwib_buf_init_mallocdefault(&bufcmd));
  netwib_er(netwib_buf_append_fmt(&bufcmd, "%s 1", NETWIBTESTVAR_EXEC));
  netwib_er(netwib_io_init_exec(&bufcmd, NETWIB_IO_WAYTYPE_NONE, NETWIB_FALSE,
                                &exitednormally, &returnedvalue, &pio));
  netwib_er(netwib_buf_close(&bufcmd));

  netwib_eir(netwib_io_wait_write(pio, NETWIB_TIME_INFINITE, &b),
             NETWIB_ERR_LOOBJWRITENOTSUPPORTED);
  netwib_eir(netwib_io_write(pio, &bufdataw),
             NETWIB_ERR_LOOBJWRITENOTSUPPORTED);
  netwib_eir(netwib_io_wait_read(pio, NETWIB_TIME_INFINITE, &b),
             NETWIB_ERR_LOOBJREADNOTSUPPORTED);
  netwib_eir(netwib_io_read(pio, &bufdatar),
             NETWIB_ERR_LOOBJREADNOTSUPPORTED);

  netwib_er(netwib_io_close(&pio));
  if (!exitednormally) {
    return(NETWIBTEST_ERR_9);
  }
  if (returnedvalue != 0) {
    return(NETWIBTEST_ERR_8);
  }

  /**/
  netwib_er(netwib_buf_init_mallocdefault(&bufcmd));
  netwib_er(netwib_buf_append_fmt(&bufcmd, "%s 2", NETWIBTESTVAR_EXEC));
  netwib_er(netwib_io_init_exec(&bufcmd, NETWIB_IO_WAYTYPE_NONE, NETWIB_FALSE,
                                &exitednormally, &returnedvalue, &pio));
  netwib_er(netwib_buf_close(&bufcmd));
  netwib_er(netwib_io_close(&pio));
  if (!exitednormally) {
    return(NETWIBTEST_ERR_9);
  }
  if (returnedvalue != 123) {
    return(NETWIBTEST_ERR_8);
  }

  /**/
  netwib_er(netwib_buf_init_mallocdefault(&bufcmd));
  netwib_er(netwib_buf_append_fmt(&bufcmd, "%s 3", NETWIBTESTVAR_EXEC));
  netwib_er(netwib_io_init_exec(&bufcmd, NETWIB_IO_WAYTYPE_READ, NETWIB_FALSE,
                                &exitednormally, &returnedvalue, &pio));
  netwib_er(netwib_buf_close(&bufcmd));

  netwib_eir(netwib_io_wait_write(pio, NETWIB_TIME_INFINITE, &b),
             NETWIB_ERR_LOOBJWRITENOTSUPPORTED);
  netwib_eir(netwib_io_write(pio, &bufdataw),
             NETWIB_ERR_LOOBJWRITENOTSUPPORTED);
  netwib_er(netwib_io_wait_read(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_read(pio, &bufdatar));
  netwib_er(netwibtest_buf_content3(&bufdatar, NETWIBTESTVAR_WANT1));

  netwib_er(netwib_io_close(&pio));
  if (!exitednormally) {
    return(NETWIBTEST_ERR_9);
  }
  if (returnedvalue != 0) {
    return(NETWIBTEST_ERR_8);
  }

  /**/
  netwib_er(netwib_buf_init_mallocdefault(&bufcmd));
  netwib_er(netwib_buf_append_fmt(&bufcmd, "%s 4", NETWIBTESTVAR_EXEC));
  netwib_er(netwib_io_init_exec(&bufcmd, NETWIB_IO_WAYTYPE_WRITE, NETWIB_FALSE,
                                &exitednormally, &returnedvalue, &pio));
  netwib_er(netwib_buf_close(&bufcmd));

  netwib_er(netwib_io_wait_write(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_write(pio, &bufdataw));
  netwib_eir(netwib_io_wait_read(pio, NETWIB_TIME_INFINITE, &b),
             NETWIB_ERR_LOOBJREADNOTSUPPORTED);
  netwib_eir(netwib_io_read(pio, &bufdatar),
             NETWIB_ERR_LOOBJREADNOTSUPPORTED);

  netwib_er(netwib_io_close(&pio));
  if (!exitednormally) {
    return(NETWIBTEST_ERR_9);
  }
  if (returnedvalue != 0) {
    return(NETWIBTEST_ERR_8);
  }

  /**/
  netwib_er(netwib_buf_init_mallocdefault(&bufcmd));
  netwib_er(netwib_buf_append_fmt(&bufcmd, "%s 5", NETWIBTESTVAR_EXEC));
  netwib_er(netwib_io_init_exec(&bufcmd, NETWIB_IO_WAYTYPE_RDWR, NETWIB_FALSE,
                                &exitednormally, &returnedvalue, &pio));
  netwib_er(netwib_buf_close(&bufcmd));

  netwib_er(netwib_io_wait_write(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_write(pio, &bufdataw));
  netwib_er(netwib_io_wait_read(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_7);
  }
  netwib__buf_reinit(&bufdatar);
  netwib_er(netwib_io_read(pio, &bufdatar));
  netwib_er(netwibtest_buf_content3(&bufdatar, NETWIBTESTVAR_WANT2));

  netwib_er(netwib_io_close(&pio));
  if (!exitednormally) {
    return(NETWIBTEST_ERR_9);
  }
  if (returnedvalue != 0) {
    return(NETWIBTEST_ERR_8);
  }

  /**/
  netwib_er(netwib_buf_init_mallocdefault(&bufcmd));
  netwib_er(netwib_buf_append_fmt(&bufcmd, "%s 6", NETWIBTESTVAR_EXEC));
  netwib_er(netwib_io_init_exec(&bufcmd, NETWIB_IO_WAYTYPE_RDWR, NETWIB_FALSE,
                                &exitednormally, &returnedvalue, &pio));
  netwib_er(netwib_buf_close(&bufcmd));

  netwib__buf_reinit(&bufdatar);
  netwib_er(netwib_io_wait_read(pio, NETWIB_TIME_INFINITE, &b));
  if (!b) {
    return(NETWIBTEST_ERR_5);
  }
  netwib_er(netwib_io_read(pio, &bufdatar));
  netwib_er(netwibtest_buf_content3(&bufdatar, NETWIBTESTVAR_WANT1));
  netwib_er(netwib_io_write(pio, &bufdataw));
  netwib__buf_reinit(&bufdatar);
  netwib_er(netwib_io_read(pio, &bufdatar));
  netwib_er(netwibtest_buf_content3(&bufdatar, NETWIBTESTVAR_WANT2));

  netwib_er(netwib_io_close(&pio));
  if (!exitednormally) {
    return(NETWIBTEST_ERR_9);
  }
  if (returnedvalue != 0) {
    return(NETWIBTEST_ERR_8);
  }

  /**/
  netwib_er(netwib_buf_init_mallocdefault(&bufcmd));
  netwib_er(netwib_buf_append_fmt(&bufcmd, "%s 7", NETWIBTESTVAR_EXEC));
  netwib_er(netwib_io_init_exec(&bufcmd, NETWIB_IO_WAYTYPE_RDWR, NETWIB_FALSE,
                                &exitednormally, &returnedvalue, &pio));
  netwib_er(netwib_buf_close(&bufcmd));

  netwib__buf_reinit(&bufdatar);
  netwib_er(netwib_io_read(pio, &bufdatar));
  netwib_er(netwibtest_buf_content3(&bufdatar, NETWIBTESTVAR_WANT1));
  netwib_er(netwib_io_write(pio, &bufdataw));
  netwib__buf_reinit(&bufdatar);
  netwib_er(netwib_io_read(pio, &bufdatar));
  netwib_er(netwibtest_buf_content3(&bufdatar, NETWIBTESTVAR_WANT2));

  netwib_er(netwib_io_close(&pio));
  if (!exitednormally) {
    return(NETWIBTEST_ERR_9);
  }
  if (returnedvalue != 0) {
    return(NETWIBTEST_ERR_8);
  }

  /**/
  netwib_er(netwib_buf_init_mallocdefault(&bufcmd));
  netwib_er(netwib_buf_append_fmt(&bufcmd, "%s 8", NETWIBTESTVAR_EXEC));
  netwib_er(netwib_io_init_exec(&bufcmd, NETWIB_IO_WAYTYPE_RDWR, NETWIB_FALSE,
                                &exitednormally, &returnedvalue, &pio));
  netwib_er(netwib_buf_close(&bufcmd));

  netwib_er(netwib_io_write(pio, &bufdataw));
  netwib__buf_reinit(&bufdatar);
  netwib_er(netwib_io_read(pio, &bufdatar));
  netwib_er(netwibtest_buf_content3(&bufdatar, NETWIBTESTVAR_WANT2));
  netwib_er(netwib_io_write(pio, &bufdataw));
  netwib__buf_reinit(&bufdatar);
  netwib_er(netwib_io_read(pio, &bufdatar));
  netwib_er(netwibtest_buf_content3(&bufdatar, NETWIBTESTVAR_WANT2));
  netwib_er(netwib_io_ctl_set_end_write(pio));

  netwib_er(netwib_io_close(&pio));
  if (!exitednormally) {
    return(NETWIBTEST_ERR_9);
  }
  if (returnedvalue != 0) {
    return(NETWIBTEST_ERR_8);
  }

  /**/
  netwib_er(netwib_buf_init_mallocdefault(&bufcmd));
  netwib_er(netwib_buf_append_fmt(&bufcmd, "%s 8", NETWIBTESTVAR_EXEC));
  netwib_er(netwib_io_init_exec(&bufcmd, NETWIB_IO_WAYTYPE_RDWR, NETWIB_TRUE,
                                &exitednormally, &returnedvalue, &pio));
  netwib_er(netwib_buf_close(&bufcmd));

  netwib_er(netwib_io_write(pio, &bufdataw));
  netwib__buf_reinit(&bufdatar);
  netwib_er(netwib_io_read(pio, &bufdatar));
  netwib_er(netwibtest_buf_content3(&bufdatar, NETWIBTESTVAR_WANT2));

  netwib_er(netwib_io_close(&pio));
  if (!exitednormally) {
    /* normal value */
  } else if (exitednormally && returnedvalue==0xD0) {
    /* Under Solaris, when bourne shell "read" command receives a signal,
       it returns 2000 = 0x7D0. So this case is also normal. */
  } else {
    return(NETWIBTEST_ERR_9);
  }

  /**/
  netwib_er(netwib_buf_init_mallocdefault(&bufcmd));
  netwib_er(netwib_buf_append_fmt(&bufcmd, "%s 9", NETWIBTESTVAR_EXEC));
  netwib_er(netwib_io_init_exec(&bufcmd, NETWIB_IO_WAYTYPE_NONE, NETWIB_FALSE,
                                NULL, NULL, &pio));
  netwib_er(netwib_buf_close(&bufcmd));
  netwib_er(netwib_io_close(&pio));

  netwib_er(netwib_buf_close(&bufdatar));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_exec_ctl(void)
{
  netwib_io_restype res;
  netwib_io *pio;
  netwib_buf bufcmd;
  netwib_bool exitednormally;
  netwib_uint32 returnedvalue;

  puts("++++----\nTest of ctl [exec]");
  netwib_er(netwib_buf_init_mallocdefault(&bufcmd));
  netwib_er(netwib_buf_append_fmt(&bufcmd, "%s 1", NETWIBTESTVAR_EXEC));
  netwib_er(netwib_io_init_exec(&bufcmd, NETWIB_IO_WAYTYPE_RDWR, NETWIB_FALSE,
                                &exitednormally, &returnedvalue, &pio));
  netwib_er(netwib_buf_close(&bufcmd));

  netwib_er(netwib_io_ctl_get_res(pio, NETWIB_IO_WAYTYPE_READ, &res));
  if (res != NETWIB_IO_RESTYPE_EXEC) {
    return(NETWIBTEST_ERR_7);
  }
  netwib_er(netwib_io_ctl_get_res(pio, NETWIB_IO_WAYTYPE_WRITE, &res));
  if (res != NETWIB_IO_RESTYPE_EXEC) {
    return(NETWIBTEST_ERR_8);
  }
  netwib_er(netwib_io_ctl_get_res(pio, NETWIB_IO_WAYTYPE_RDWR, &res));
  if (res != NETWIB_IO_RESTYPE_EXEC) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwib_shellclient_term_test(void)
{
  netwib_buf term;

  puts("++++----\nTest of netwib_shellclient_term");
  netwib_er(netwib_shellclient_term(&term));
  netwib_er(netwib_fmt_display("TERM=%{buf}\n", &term));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test__read_write(netwib_io *pinio,
                                   netwib_io *poutio,
                                   netwib_buf *pbuf)
{
  netwib_err ret;

  netwib__buf_reinit(pbuf);
  ret = netwib_io_read(pinio, pbuf);
  if (ret != NETWIB_ERR_OK) {
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
    }
    return(ret);
  }

  netwib_er(netwib_io_write(poutio, pbuf));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_io_init_shellserver_test(void)
{
  netwib_io *pshellserverio, *pshellclientio;
  netwib_wait *pshellserverwait, *pshellclientwait;
  netwib_buf buf, term;
  netwib_bool shellserverevent, shellclientevent;
  netwib_bool exitednormally;
  netwib_uint32 returnedvalue;
  netwib_err ret;

  puts("++++----\nTest of netwib_io_init_shellserver");

  netwib_er(netwibtest_something_to_do("Enter exit to quit this shell."));
  ret = netwib_io_init_shellclient(&pshellclientio);
  if (ret == NETWIB_ERR_LONOTIMPLEMENTED) {
    return(NETWIB_ERR_OK);
  }
  netwib_er(ret);
  netwib_er(netwib_wait_init_io_read(pshellclientio, &pshellclientwait));

  netwib_er(netwib_shellclient_term(&term));
  netwib_er(netwib_io_init_shellserver(NETWIB_IO_INIT_SHELLSERVER_UID_NONE, &term, NETWIB_TRUE,
                                       &exitednormally, &returnedvalue,
                                       &pshellserverio));
  netwib_er(netwib_wait_init_io_read(pshellserverio, &pshellserverwait));

  /* main loop */
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  while(NETWIB_TRUE) {
    /* wait */
    netwib_er(netwib_wait_wait2(pshellserverwait, pshellclientwait,
                                NETWIB_TIME_INFINITE,
                                &shellserverevent, &shellclientevent));
    /* read and write */
    if (shellserverevent) {
      ret = test__read_write(pshellserverio, pshellclientio, &buf);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
    }
    if (shellclientevent) {
      ret = test__read_write(pshellclientio, pshellserverio, &buf);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
    }
  }
  if (ret == NETWIB_ERR_DATAEND) {
    ret = NETWIB_ERR_OK;
  }
  netwib_er(netwib_buf_close(&buf));

  netwib_er(netwib_wait_close(&pshellserverwait));
  netwib_er(netwib_wait_close(&pshellclientwait));
  netwib_er(netwib_io_close(&pshellclientio));
  netwib_er(netwib_io_close(&pshellserverio));

  netwib_er(ret);
  if (!exitednormally) {
    return(NETWIBTEST_ERR_0);
  }
  if (returnedvalue != 0) {
    netwib_er(netwib_fmt_display("Return value of last command executed in shell : %{uint32}\n", returnedvalue));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_shellclient_test(void)
{

  puts("++++----\nTest of netwib_io_init_shellclient");
  puts("Done in netwib_io_init_shellserver_test");

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_debug_test(void)
{
  netwib_buf bufdata;
  netwib_io *pscreenio, *pnullio, *pdebugio;

  puts("++++----\nTest of netwib_io_init_debug");

  netwib_er(netwib_io_init_null(&pnullio));
  netwib_er(netwib_io_init_screen(&pscreenio));
  netwib_er(netwib_io_init_debug(pnullio, pscreenio, NETWIB_TRUE, &pdebugio));

  netwib_er(netwib_buf_init_mallocdefault(&bufdata));

  netwib_er(netwib_io_read(pdebugio, &bufdata));
  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_buf_append_string("hello1", &bufdata));
  netwib_er(netwib_io_write(pdebugio, &bufdata));
  netwib_er(netwib_io_wait_read(pdebugio, NETWIB_TIME_INFINITE, NULL));
  netwib__buf_reinit(&bufdata);
  netwib_er(netwib_buf_append_string("hello1", &bufdata));
  netwib_er(netwib_io_unread(pdebugio, &bufdata));
  netwib_er(netwib_io_ctl_get_res(pdebugio, NETWIB_IO_WAYTYPE_READ, NULL));
  netwib_er(netwib_io_ctl_set_end(pdebugio, NETWIB_IO_WAYTYPE_WRITE));

  netwib_er(netwib_io_close(&pdebugio));
  netwib_er(netwib_buf_close(&bufdata));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_tee_test(void)
{
  netwib_buf bufdata;
  netwib_io *pscreen1io, *pscreen2io, *pteeio;

  puts("++++----\nTest of netwib_io_init_tee");

  netwib_er(netwib_io_init_screen(&pscreen1io));
  netwib_er(netwib_io_init_screen(&pscreen2io));
  netwib_er(netwib_io_init_tee(pscreen1io, pscreen2io, NETWIB_TRUE, &pteeio));

  netwib_er(netwib_buf_init_mallocdefault(&bufdata));

  netwib_er(netwib_buf_append_string("hello1\n", &bufdata));
  netwib_er(netwib_io_write(pteeio, &bufdata));
  netwib_er(netwib_io_wait_write(pteeio, NETWIB_TIME_INFINITE, NULL));
  netwib_er(netwib_io_ctl_get_res(pteeio, NETWIB_IO_WAYTYPE_WRITE, NULL));
  netwib_er(netwib_io_ctl_set_end(pteeio, NETWIB_IO_WAYTYPE_WRITE));

  netwib_er(netwib_io_close(&pteeio));
  netwib_er(netwib_buf_close(&bufdata));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_iousual(void)
{
  puts("###########################################\nTest of sys/iousual");

  netwib_er(netwib_io_init_null_test());
  netwib_er(test_null_waitwrite());
  netwib_er(test_null_waitreadunread());
  netwib_er(test_null_ctl());

  netwib_er(netwib_io_init_mem_test());
  netwib_er(test_mem_waitwrite());
  netwib_er(test_mem_waitreadunread());
  netwib_er(test_mem_ctl());
  netwib_er(netwib_io_init_mem_lock_test());

  netwib_er(netwib_io_init_tlv_test());
  netwib_er(test_tlv_waitwrite());
  netwib_er(test_tlv_waitreadunread());
  netwib_er(test_tlv_ctl());
  netwib_er(netwib_io_init_tlv_lock_test());

  netwib_er(netwib_io_init_data_test());
  netwib_er(test_netwib_io_init_data_line());
  netwib_er(test_data_line_waitwrite());
  netwib_er(test_data_line_waitreadunread());
  netwib_er(test_data_line_ctl());
  netwib_er(test_netwib_io_init_data_chunk());
  netwib_er(test_data_chunk_waitwrite());
  netwib_er(test_data_chunk_waitreadunread());
  netwib_er(test_data_chunk_ctl());
  netwib_er(test_netwib_io_init_data_fixed());
  netwib_er(test_data_fixed_waitwrite());
  netwib_er(test_data_fixed_waitreadunread());
  netwib_er(test_data_fixed_ctl());
  netwib_er(test_netwib_io_init_data_transparent());
  netwib_er(test_data_transparent_waitwrite());
  netwib_er(test_data_transparent_waitreadunread());
  netwib_er(test_data_transparent_ctl());

  netwib_er(netwib_io_init_storage_test());
  netwib_er(test_storage_waitwrite());
  netwib_er(test_storage_waitreadunread());
  netwib_er(test_storage_ctl());

  netwib_er(netwib_io_init_rdwr_test());
  netwib_er(test_rdwr_waitwrite());
  netwib_er(test_rdwr_waitreadunread());
  netwib_er(test_rdwr_ctl());

  netwib_er(netwib_io_init_exec_test());
  netwib_er(test_exec_waitwritereadunread());
  netwib_er(test_exec_ctl());

  netwib_er(netwib_shellclient_term_test());
  if (netwibtest_testkbdlevel) {
    netwib_er(netwib_io_init_shellserver_test());
    netwib_er(netwib_io_init_shellclient_test());
  }

  netwib_er(netwib_io_init_debug_test());

  netwib_er(netwib_io_init_tee_test());

  return(NETWIB_ERR_OK);
}
