#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_uint32_init_rand_stat(netwib_uint32 max)
{
  netwib_uint32 r, i, sum;

  sum = 0;
  for (i = 1; i <= max; i++) {
    netwib_er(netwib_uint32_init_rand(0, 100, &r));
    sum += r;
  }
  netwib_er(netwib_fmt_display("average(%{uint32})=%{uint32} (50 is good)\n",
                               max, sum/max));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_uint32_init_rand_test(void)
{
  netwib_uint32 r, i;
  netwib_uint32 array[11];

  puts("++++----\nTest of netwib_uint32_init_rand");

  for (i = 0; i < 5; i++) {
    netwib_er(netwib_uint32_init_rand(10, 20, &r));
    netwib_er(netwib_fmt_display("r=%{uint32}\t", r));
  }
  netwib_er(netwib_fmt_display("\n"));

  for (i = 0; i < 5; i++) {
    netwib_er(netwib_uint32_init_rand_all(&r));
    netwib_er(netwib_fmt_display("r=%{uint32}\t", r));
  }
  netwib_er(netwib_fmt_display("\n"));

  /**/
  netwib_er(netwib_fmt_display("Stat:\n"));
  netwib_er(netwib_uint32_init_rand_stat(100));
  netwib_er(netwib_uint32_init_rand_stat(1000));

  /**/
  for (i = 0; i < 35; i++) {
    netwib_er(netwib_uint32_init_rand(0, 1, &r));
    netwib_er(netwib_fmt_display("%{uint32} ", r));
  }
  netwib_er(netwib_fmt_display("\n"));

  /**/
  for (i = 0; i < 11; i++) {
    array[i] = 0;
  }
  for (i = 0; i < 1000; i++) {
    netwib_er(netwib_uint32_init_rand(0, 10, &r));
    array[r]++;
  }
  for (i = 0; i < 11; i++) {
    netwib_er(netwib_fmt_display("%{uint32}[%{uint32}] ", array[i], i));
  }
  netwib_er(netwib_fmt_display("\n"));

  /**/
  for (i = 0; i < 50; i++) {
    netwib_er(netwib_uint32_init_rand_all(&r));
    netwib_er(netwib_fmt_display("r=%{r032;uint32:b} %{r08;uint32:X}\n", r,
                                 r));
  }
  netwib_er(netwib_fmt_display("\n"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_uint64_init_rand_test(void)
{
  netwib_uint64 r, min, max;
  netwib_uint32 i;

  puts("++++----\nTest of netwib_uint64_init_rand");

  netwib__uint64_init_uint32(10, min);
  netwib__uint64_init_uint32(20, max);
  for (i = 0; i < 5; i++) {
    netwib_er(netwib_uint64_init_rand(min, max, &r));
    netwib_er(netwib_fmt_display("r=%{uint64}\n", r));
  }
  netwib_er(netwib_fmt_display("\n"));

  netwib__uint64_init_uint32(0xFFFFFFF, min);
  netwib__uint64_init_32(0xFFFFFFFFu, 0xFFFFFFFFu, max);
  for (i = 0; i < 50; i++) {
    netwib_er(netwib_uint64_init_rand(min, max, &r));
    netwib_er(netwib_fmt_display("r=%{r064;uint64:b}\n", r));
  }
  netwib_er(netwib_fmt_display("\n"));
  for (i = 0; i < 50; i++) {
    netwib_er(netwib_uint64_init_rand(min, max, &r));
    netwib_er(netwib_fmt_display("r=%{r016;uint64:X}\n", r));
  }
  netwib_er(netwib_fmt_display("\n"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_rand_test(void)
{
  netwib_buf buf;

  puts("++++----\nTest of netwib_buf_append_rand");

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_buf_append_rand(10, 'a', 'z', &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib__buf_reinit(&buf);

  netwib_er(netwib_buf_append_rand_all(10, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
  netwib__buf_reinit(&buf);

  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_rand(void)
{
  puts("###########################################\nTest of sys/rand");
  netwib_er(netwib_uint32_init_rand_test());
  netwib_er(netwib_uint64_init_rand_test());
  netwib_er(netwib_buf_append_rand_test());

  return(NETWIB_ERR_OK);
}
