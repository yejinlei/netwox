#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwibtest_time_content(netwib_consttime *ptime,
                                          netwib_uint32 sec,
                                          netwib_uint32 nsec)
{
  netwib_uint32 obtainsec, obtainmsec, obtainusec, obtainnsec;

  netwib_er(netwib_time_decode_fields(ptime, &obtainsec, &obtainmsec,
                                      &obtainusec, &obtainnsec));
  obtainnsec += 1000*obtainusec + 1000000*obtainmsec;
  if (obtainsec != sec) {
    netwib_er(netwib_fmt_display("obtainsec(%{uint32})!=sec(%{uint32})\n",
                                 obtainsec, sec));
    return(NETWIBTEST_ERR_9);
  }
  if (obtainnsec != nsec) {
    netwib_er(netwib_fmt_display("obtainnsec(%{uint32})!=nsec(%{uint32})\n",
                                 obtainnsec, nsec));
    return(NETWIBTEST_ERR_9);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwibtest_time_content_zero(netwib_consttime *ptime)
{
  if (ptime->sec != 0 || ptime->nsec != 0) {
    netwib_er(netwib_fmt_display("We want a ZERO time\n"));
    return(NETWIBTEST_ERR_9);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwibtest_time_content_infinite(netwib_consttime *ptime)
{
  if (ptime->sec != 0xFFFFFFFFu || ptime->nsec != 999999999) {
    netwib_er(netwib_fmt_display("We want a INFINITE time\n"));
    return(NETWIBTEST_ERR_9);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_time_init_fields_test(void)
{
  netwib_time t;

  puts("++++----\nTest of netwib_time_init_fields");
  netwib_er(netwib_time_init_fields(1, 2, 3, 4, &t));
  netwib_er(netwibtest_time_content(&t, 1, 2003004));

  netwib_er(netwib_time_init_fields(0, 0, 0, 0, &t));
  netwib_er(netwibtest_time_content_zero(&t));
  netwib_er(netwib_time_init_fields(0xFFFFFFFFu, 0xFFFFFFFFu, 0, 0, &t));
  netwib_er(netwibtest_time_content_infinite(&t));

  netwib_er(netwib_time_init_sec(23, &t));
  netwib_er(netwibtest_time_content(&t, 23, 0));
  netwib_er(netwib_time_init_sec(0xFFFFFFFFu, &t));
  netwib_er(netwibtest_time_content(&t, 0xFFFFFFFFu, 0));

  netwib_er(netwib_time_init_msec(23, &t));
  netwib_er(netwibtest_time_content(&t, 0, 23000000));
  netwib_er(netwib_time_init_msec(999, &t));
  netwib_er(netwibtest_time_content(&t, 0, 999000000));
  netwib_er(netwib_time_init_msec(1000, &t));
  netwib_er(netwibtest_time_content(&t, 1, 0));
  netwib_er(netwib_time_init_msec(1001, &t));
  netwib_er(netwibtest_time_content(&t, 1, 1000000));
  netwib_er(netwib_time_init_msec(0xFFFFFFFFu, &t));
  netwib_er(netwibtest_time_content(&t, 4294967, 295000000));

  netwib_er(netwib_time_init_usec(23, &t));
  netwib_er(netwibtest_time_content(&t, 0, 23000));
  netwib_er(netwib_time_init_usec(999999, &t));
  netwib_er(netwibtest_time_content(&t, 0, 999999000));
  netwib_er(netwib_time_init_usec(1000000, &t));
  netwib_er(netwibtest_time_content(&t, 1, 0));
  netwib_er(netwib_time_init_usec(1000001, &t));
  netwib_er(netwibtest_time_content(&t, 1, 1000));
  netwib_er(netwib_time_init_usec(0xFFFFFFFFu, &t));
  netwib_er(netwibtest_time_content(&t, 4294, 967295000));

  netwib_er(netwib_time_init_nsec(23, &t));
  netwib_er(netwibtest_time_content(&t, 0, 23));
  netwib_er(netwib_time_init_nsec(999999999, &t));
  netwib_er(netwibtest_time_content(&t, 0, 999999999));
  netwib_er(netwib_time_init_nsec(1000000000, &t));
  netwib_er(netwibtest_time_content(&t, 1, 0));
  netwib_er(netwib_time_init_nsec(1000000001, &t));
  netwib_er(netwibtest_time_content(&t, 1, 1));
  netwib_er(netwib_time_init_nsec(0xFFFFFFFFu, &t));
  netwib_er(netwibtest_time_content(&t, 4, 294967295));

  netwib_er(netwib_time_init_fields(23, 45, 67, 89, &t));
  netwib_er(netwibtest_time_content(&t, 23, 45067089));
  netwib_er(netwib_time_init_fields(23, 4500, 6700, 8900, &t));
  netwib_er(netwibtest_time_content(&t, 27, 506708900));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_time_init_time_test(void)
{
  netwib_time t, t2;

  puts("++++----\nTest of netwib_time_init_time");
  netwib_er(netwib_time_init_fields(1, 2, 3, 4, &t));
  netwib_er(netwibtest_time_content(&t, 1, 2003004));
  netwib_er(netwib_time_init_time(&t, &t2));
  netwib_er(netwibtest_time_content(&t2, 1, 2003004));

  netwib_er(netwib_time_init_time(NETWIB_TIME_ZERO, &t2));
  netwib_er(netwibtest_time_content_zero(&t2));

  netwib_er(netwib_time_init_time(NETWIB_TIME_INFINITE, &t2));
  netwib_er(netwibtest_time_content_infinite(&t2));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_time_decode_sec_test(void)
{
  netwib_time t;
  netwib_uint32 sec;

  puts("++++----\nTest of netwib_time_decode_sec");
  netwib_er(netwib_time_init_fields(1, 2, 3, 4, &t));
  netwib_er(netwibtest_time_content(&t, 1, 2003004));
  netwib_er(netwib_time_decode_sec(&t, &sec));
  if (sec != 1) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_time_decode_sec(NETWIB_TIME_ZERO, &sec));
  if (sec != 0) {
    return(NETWIBTEST_ERR_9);
  }
  netwib_eir(netwib_time_decode_sec(NETWIB_TIME_INFINITE, &sec),
             NETWIB_ERR_NOTCONVERTED);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_time_decode_msec_test(void)
{
  netwib_time t;
  netwib_uint32 msec;

  puts("++++----\nTest of netwib_time_decode_msec");
  netwib_er(netwib_time_init_fields(1, 2, 3, 4, &t));
  netwib_er(netwib_time_decode_msec(&t, &msec));
  if (msec != 1002) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_time_init_fields(4294967, 295, 3, 4, &t));
  netwib_er(netwib_time_decode_msec(&t, &msec));
  if (msec != 0xFFFFFFFFu) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_time_init_fields(4294967, 295, 1000, 0, &t));
  netwib_eir(netwib_time_decode_msec(&t, &msec), NETWIB_ERR_NOTCONVERTED);

  netwib_er(netwib_time_init_fields(4294968, 295, 1000, 0, &t));
  netwib_eir(netwib_time_decode_msec(&t, &msec), NETWIB_ERR_NOTCONVERTED);

  netwib_er(netwib_time_decode_msec(NETWIB_TIME_ZERO, &msec));
  if (msec != 0) {
    return(NETWIBTEST_ERR_9);
  }
  netwib_eir(netwib_time_decode_msec(NETWIB_TIME_INFINITE, &msec),
             NETWIB_ERR_NOTCONVERTED);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_time_decode_usec_test(void)
{
  netwib_time t;
  netwib_uint32 usec;

  puts("++++----\nTest of netwib_time_decode_usec");
  netwib_er(netwib_time_init_fields(1, 2, 3, 4, &t));
  netwib_er(netwib_time_decode_usec(&t, &usec));
  if (usec != 1002003) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_time_init_fields(4294, 967, 295, 0, &t));
  netwib_er(netwib_time_decode_usec(&t, &usec));
  if (usec != 0xFFFFFFFFu) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_time_init_fields(4294, 967, 296, 0, &t));
  netwib_eir(netwib_time_decode_usec(&t, &usec), NETWIB_ERR_NOTCONVERTED);

  netwib_er(netwib_time_init_fields(4294968, 295, 1000, 0, &t));
  netwib_eir(netwib_time_decode_usec(&t, &usec), NETWIB_ERR_NOTCONVERTED);

  netwib_er(netwib_time_decode_usec(NETWIB_TIME_ZERO, &usec));
  if (usec != 0) {
    return(NETWIBTEST_ERR_9);
  }
  netwib_eir(netwib_time_decode_usec(NETWIB_TIME_INFINITE, &usec),
             NETWIB_ERR_NOTCONVERTED);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_time_decode_nsec_test(void)
{
  netwib_time t;
  netwib_uint32 nsec;

  puts("++++----\nTest of netwib_time_decode_nsec");
  netwib_er(netwib_time_init_fields(1, 2, 3, 4, &t));
  netwib_er(netwib_time_decode_nsec(&t, &nsec));
  if (nsec != 1002003004) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_time_init_fields(4, 294, 967, 295, &t));
  netwib_er(netwib_time_decode_nsec(&t, &nsec));
  if (nsec != 0xFFFFFFFFu) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_time_init_fields(4, 294, 967, 296, &t));
  netwib_eir(netwib_time_decode_nsec(&t, &nsec), NETWIB_ERR_NOTCONVERTED);

  netwib_er(netwib_time_init_fields(4294, 968, 295, 1000, &t));
  netwib_eir(netwib_time_decode_nsec(&t, &nsec), NETWIB_ERR_NOTCONVERTED);

  netwib_er(netwib_time_decode_nsec(NETWIB_TIME_ZERO, &nsec));
  if (nsec != 0) {
    return(NETWIBTEST_ERR_9);
  }
  netwib_eir(netwib_time_decode_nsec(NETWIB_TIME_INFINITE, &nsec),
             NETWIB_ERR_NOTCONVERTED);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_time_decode_fields_test(void)
{
  netwib_time t;
  netwib_uint32 sec, msec, usec, nsec;

  puts("++++----\nTest of netwib_time_decode_fields");
  netwib_er(netwib_time_init_fields(1, 2, 3, 4, &t));
  netwib_er(netwib_time_decode_fields(&t, &sec, &msec, &usec, &nsec));
  if (sec != 1) {
    return(NETWIBTEST_ERR_2);
  }
  if (msec != 2) {
    return(NETWIBTEST_ERR_3);
  }
  if (usec != 3) {
    return(NETWIBTEST_ERR_4);
  }
  if (nsec != 4) {
    return(NETWIBTEST_ERR_5);
  }

  netwib_er(netwib_time_decode_fields(NETWIB_TIME_ZERO, &sec, &msec,
                                      &usec, &nsec));
  if (sec != 0 || msec != 0 || usec != 0 || nsec != 0) {
    return(NETWIBTEST_ERR_9);
  }
  netwib_eir(netwib_time_decode_fields(NETWIB_TIME_INFINITE, &sec, &msec,
                                       &usec, &nsec),
             NETWIB_ERR_NOTCONVERTED);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_time_plus_time_test(void)
{
  netwib_time t1, t2;

  puts("++++----\nTest of netwib_time_plus_time");
  netwib_er(netwib_time_init_fields(12, 345, 678, 123, &t1));
  netwib_er(netwib_time_init_fields(23, 456, 789, 345, &t2));
  netwib_er(netwib_time_plus_time(&t1, &t2));
  netwib_er(netwibtest_time_content(&t1, 35, 802467468));

  netwib_er(netwib_time_init_fields(0xFFFFFFF3u, 345, 678, 34, &t1));
  netwib_er(netwib_time_init_fields(20, 456, 789, 4, &t2));
  netwib_er(netwib_time_plus_time(&t1, &t2));
  netwib_er(netwibtest_time_content_infinite(&t1));

  netwib_er(netwib_time_init_fields(1, 2, 3, 4, &t1));
  netwib_er(netwib_time_plus_time(&t1, NETWIB_TIME_ZERO));
  netwib_er(netwibtest_time_content(&t1, 1, 2003004));
  netwib_er(netwib_time_init_fields(1, 2, 3, 4, &t1));
  netwib_er(netwib_time_plus_time(&t1, NETWIB_TIME_INFINITE));
  netwib_er(netwibtest_time_content_infinite(&t1));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_time_plus_fields_test(void)
{
  netwib_time t;

  puts("++++----\nTest of netwib_time_plus_fields");
  netwib_er(netwib_time_init_fields(12, 345, 678, 9, &t));
  netwib_er(netwib_time_plus_fields(&t, 1, 2, 3, 4));
  netwib_er(netwibtest_time_content(&t, 13, 347681013));

  netwib_er(netwib_time_plus_sec(&t, 1));
  netwib_er(netwibtest_time_content(&t, 14, 347681013));

  netwib_er(netwib_time_plus_msec(&t, 1));
  netwib_er(netwibtest_time_content(&t, 14, 348681013));

  netwib_er(netwib_time_plus_usec(&t, 1));
  netwib_er(netwibtest_time_content(&t, 14, 348682013));

  netwib_er(netwib_time_plus_nsec(&t, 1));
  netwib_er(netwibtest_time_content(&t, 14, 348682014));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_time_minus_time_test(void)
{
  netwib_time t1, t2;

  puts("++++----\nTest of netwib_time_minus_time");
  netwib_er(netwib_time_init_fields(120, 345, 678, 34, &t1));
  netwib_er(netwib_time_init_fields(23, 456, 789, 23, &t2));
  netwib_er(netwib_time_minus_time(&t1, &t2));
  netwib_er(netwibtest_time_content(&t1, 96, 888889011));

  netwib_er(netwib_time_init_fields(19, 345, 678, 34, &t1));
  netwib_er(netwib_time_init_fields(20, 456, 789, 23, &t2));
  netwib_eir(netwib_time_minus_time(&t1, &t2), NETWIB_ERR_PATIMEDIFFNEG);
  netwib_er(netwibtest_time_content(&t1, 19, 345678034));

  netwib_er(netwib_time_init_fields(1, 2, 3, 4, &t1));
  netwib_er(netwib_time_minus_time(&t1, NETWIB_TIME_ZERO));
  netwib_er(netwibtest_time_content(&t1, 1, 2003004));
  netwib_er(netwib_time_init_fields(1, 2, 3, 4, &t1));
  netwib_eir(netwib_time_minus_time(&t1, NETWIB_TIME_INFINITE),
             NETWIB_ERR_PATIMEDIFFNEG);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_time_minus_fields_test(void)
{
  netwib_time t;

  puts("++++----\nTest of netwib_time_minus_fields");
  netwib_er(netwib_time_init_fields(12, 345, 678, 9, &t));
  netwib_er(netwib_time_minus_fields(&t, 1, 2, 3, 1));
  netwib_er(netwibtest_time_content(&t, 11, 343675008));

  netwib_er(netwib_time_minus_sec(&t, 1));
  netwib_er(netwibtest_time_content(&t, 10, 343675008));

  netwib_er(netwib_time_minus_msec(&t, 1));
  netwib_er(netwibtest_time_content(&t, 10, 342675008));

  netwib_er(netwib_time_minus_usec(&t, 1));
  netwib_er(netwibtest_time_content(&t, 10, 342674008));

  netwib_er(netwib_time_minus_nsec(&t, 1));
  netwib_er(netwibtest_time_content(&t, 10, 342674007));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_time_cmp_test(void)
{
  netwib_time t1, t2;
  netwib_cmp cmp;

  puts("++++----\nTest of netwib_time_cmp");
  netwib_er(netwib_time_init_sec(3, &t1));
  netwib_er(netwib_time_init_sec(3, &t2));
  netwib_er(netwib_time_cmp(&t1, &t2, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    return(NETWIBTEST_ERR_9);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_time_sleep_time_test(void)
{
  netwib_time t;

  puts("++++----\nTest of netwib_time_sleep_time");
  netwib_er(netwib_time_init_fields(1, 2, 3, 4, &t));
  netwib_er(netwib_time_sleep_time(&t));

  netwib_er(netwib_time_sleep_time(NETWIB_TIME_ZERO));

  /*netwib_er(netwib_time_sleep_time(NETWIB_TIME_INFINITE));*/

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_time_sleep_fields_test(void)
{

  puts("++++----\nTest of netwib_time_sleep_fields");
  netwib_er(netwib_time_sleep_fields(1, 2, 3, 4));

  netwib_er(netwib_time_sleep_sec(1));

  netwib_er(netwib_time_sleep_msec(1));

  netwib_er(netwib_time_sleep_usec(1));

  netwib_er(netwib_time_sleep_nsec(1));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_time_wait_time_test(void)
{
  netwib_time t;

  puts("++++----\nTest of netwib_time_wait_time");
  netwib_er(netwib_time_wait_time(NETWIB_TIME_ZERO));

  /*netwib_er(netwib_time_wait_time(NETWIB_TIME_INFINITE));*/

  netwib_er(netwib_time_init_fields(1, 2, 3, 4, &t));
  netwib_er(netwib_time_wait_time(&t));

  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_wait_time(&t));

  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_plus_msec(&t, 200));
  netwib_er(netwib_time_wait_time(&t));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_time_test(void)
{
  netwib_time t;
  netwib_buf buf;

  puts("++++----\nTest of netwib_buf_append_time");
  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_time(&t, NETWIB_TIME_ENCODETYPE_BEST, &buf));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_time_init_now_test(void)
{
  netwib_time t1, t2;
  netwib_cmp cmp;

  puts("++++----\nTest of netwib_time_init_now");
  netwib_er(netwib_time_init_now(&t1));
  netwib_er(netwib_time_init_now(&t2));
  netwib_er(netwib_time_cmp(&t1, &t2, &cmp));
  if (cmp != NETWIB_CMP_LT && cmp != NETWIB_CMP_EQ) {
    return(NETWIBTEST_ERR_9);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_time_decode_localtime_test(void)
{
  netwib_time t;
  netwib_localtime dt;

  puts("++++----\nTest of netwib_time_decode_localtime");
  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_decode_localtime(&t, &dt));

  netwib_er(netwib_fmt_display("%{uint32}/%{uint32}/%{uint32} %{uint32}:%{uint32}:%{uint32} (%{int32})\n", dt.mday, dt.mon, dt.year, dt.hour, dt.min, dt.sec, dt.zoneoffset));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_time_init_localtime_test(void)
{
  netwib_time t1, t2;
  netwib_localtime dt;
  netwib_cmp cmp;

  puts("++++----\nTest of netwib_time_init_localtime");
  netwib_er(netwib_time_init_now(&t1));
  t1.nsec = 0;
  netwib_er(netwib_time_decode_localtime(&t1, &dt));
  netwib_er(netwib_time_init_localtime(&dt, &t2));
  netwib_er(netwib_time_cmp(&t1, &t2, &cmp));

  netwib_er(netwib_fmt_display("%{uint32}.%{uint32}\n", t1.sec, t1.nsec));
  netwib_er(netwib_fmt_display("%{uint32}/%{uint32}/%{uint32} %{uint32}:%{uint32}:%{uint32}\n", dt.mday, dt.mon, dt.year, dt.hour, dt.min, dt.sec));
  netwib_er(netwib_fmt_display("%{uint32}.%{uint32}\n", t2.sec, t2.nsec));

  if (cmp != NETWIB_CMP_EQ) {
    return(NETWIBTEST_ERR_9);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_localtime_test(void)
{
  netwib_time t;
  netwib_localtime lt;
  netwib_buf buf;

  puts("++++----\nTest of netwib_buf_append_localtime");
  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_decode_localtime(&t, &lt));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_localtime(&lt, NETWIB_LOCALTIME_ENCODETYPE_NOTHING, &buf)); netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_append_localtime(&lt, NETWIB_LOCALTIME_ENCODETYPE_NSEC_ZERO, &buf)); netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_append_localtime(&lt, NETWIB_LOCALTIME_ENCODETYPE_NSEC_SPACE, &buf)); netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_append_localtime(&lt, NETWIB_LOCALTIME_ENCODETYPE_SEC_ZERO, &buf)); netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_append_localtime(&lt, NETWIB_LOCALTIME_ENCODETYPE_SEC_SPACE, &buf)); netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_append_localtime(&lt, NETWIB_LOCALTIME_ENCODETYPE_MIN_ZERO, &buf)); netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_append_localtime(&lt, NETWIB_LOCALTIME_ENCODETYPE_MIN_SPACE, &buf)); netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_append_localtime(&lt, NETWIB_LOCALTIME_ENCODETYPE_HOUR_ZERO, &buf)); netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_append_localtime(&lt, NETWIB_LOCALTIME_ENCODETYPE_HOUR_SPACE, &buf)); netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_append_localtime(&lt, NETWIB_LOCALTIME_ENCODETYPE_MDAY_ZERO, &buf)); netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_append_localtime(&lt, NETWIB_LOCALTIME_ENCODETYPE_MDAY_SPACE, &buf)); netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_append_localtime(&lt, NETWIB_LOCALTIME_ENCODETYPE_MON_ZERO, &buf)); netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_append_localtime(&lt, NETWIB_LOCALTIME_ENCODETYPE_MON_SPACE, &buf)); netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_append_localtime(&lt, NETWIB_LOCALTIME_ENCODETYPE_MON_SHORT, &buf)); netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_append_localtime(&lt, NETWIB_LOCALTIME_ENCODETYPE_MON_LONG, &buf)); netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_append_localtime(&lt, NETWIB_LOCALTIME_ENCODETYPE_YEAR_SHORT, &buf)); netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_append_localtime(&lt, NETWIB_LOCALTIME_ENCODETYPE_YEAR_LONG, &buf)); netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_append_localtime(&lt, NETWIB_LOCALTIME_ENCODETYPE_WDAY_SHORT, &buf)); netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_append_localtime(&lt, NETWIB_LOCALTIME_ENCODETYPE_WDAY_LONG, &buf)); netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_append_localtime(&lt, NETWIB_LOCALTIME_ENCODETYPE_YDAY_ZERO, &buf)); netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_append_localtime(&lt, NETWIB_LOCALTIME_ENCODETYPE_YDAY_SPACE, &buf)); netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_append_localtime(&lt, NETWIB_LOCALTIME_ENCODETYPE_ZONE_SEC, &buf)); netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_append_localtime(&lt, NETWIB_LOCALTIME_ENCODETYPE_ZONE_GMT, &buf)); netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_append_localtime(&lt, NETWIB_LOCALTIME_ENCODETYPE_ZONE_HM, &buf)); netwib_er(netwib_buf_append_string("\n", &buf));

  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_time_iselapsed_test(void)
{
  netwib_time t;
  netwib_bool b;

  puts("++++----\nTest of netwib_time_iselapsed");
  netwib_er(netwib_time_iselapsed(NETWIB_TIME_ZERO, &b));
  if (!b) {
    return(NETWIBTEST_ERR_9);
  }
  netwib_er(netwib_time_iselapsed(NETWIB_TIME_INFINITE, &b));
  if (b) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_time_init_fields(1, 2, 3, 4, &t));
  netwib_er(netwib_time_iselapsed(&t, &b));
  if (!b) {
    return(NETWIBTEST_ERR_9);
  }

  netwib_er(netwib_time_init_fields(0xFFFFFFFFu, 2, 3, 4, &t));
  netwib_er(netwib_time_iselapsed(&t, &b));
  if (b) {
    return(NETWIBTEST_ERR_9);
  }

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_time(void)
{
  puts("###########################################\nTest of sys/time");
  netwib_er(netwib_time_init_fields_test());
  netwib_er(netwib_time_init_time_test());

  netwib_er(netwib_time_decode_sec_test());
  netwib_er(netwib_time_decode_msec_test());
  netwib_er(netwib_time_decode_usec_test());
  netwib_er(netwib_time_decode_nsec_test());
  netwib_er(netwib_time_decode_fields_test());

  netwib_er(netwib_time_plus_time_test());
  netwib_er(netwib_time_plus_fields_test());
  netwib_er(netwib_time_minus_time_test());
  netwib_er(netwib_time_minus_fields_test());

  netwib_er(netwib_time_cmp_test());

  netwib_er(netwib_time_sleep_time_test());
  netwib_er(netwib_time_sleep_fields_test());
  netwib_er(netwib_time_wait_time_test());

  netwib_er(netwib_buf_append_time_test());

  netwib_er(netwib_time_init_now_test());
  netwib_er(netwib_time_decode_localtime_test());
  netwib_er(netwib_time_init_localtime_test());
  netwib_er(netwib_buf_append_localtime_test());

  netwib_er(netwib_time_iselapsed_test());

  return(NETWIB_ERR_OK);
}

