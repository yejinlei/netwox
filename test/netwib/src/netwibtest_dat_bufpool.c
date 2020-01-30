#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_bufpool_init_test(void)
{
  netwib_bufpool *pbufpool;
  netwib_buf *pbuf1, *pbuf2, *pbuf3;

  puts("++++----\nTest of netwib_bufpool_init");

  netwib_er(netwib_bufpool_init(NETWIB_FALSE, &pbufpool));

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf1));
  netwib_er(netwib_buf_append_string("abc", pbuf1));
  netwib_er(netwibtest_buf_content2(pbuf1, "abc"));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf1));

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf1));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf2));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf3));
  netwib_er(netwib_buf_append_string("abc", pbuf1));
  netwib_er(netwib_buf_append_string("def", pbuf2));
  netwib_er(netwib_buf_append_string("ghi", pbuf3));
  netwib_er(netwibtest_buf_content2(pbuf1, "abc"));
  netwib_er(netwibtest_buf_content2(pbuf2, "def"));
  netwib_er(netwibtest_buf_content2(pbuf3, "ghi"));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf2));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf2));
  netwib_er(netwibtest_buf_content2(pbuf2, ""));
  netwib_er(netwib_buf_append_string("DEF", pbuf2));
  netwib_er(netwibtest_buf_content2(pbuf1, "abc"));
  netwib_er(netwibtest_buf_content2(pbuf2, "DEF"));
  netwib_er(netwibtest_buf_content2(pbuf3, "ghi"));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf1));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf2));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf3));

  netwib_er(netwib_bufpool_close(&pbufpool));

  netwib_er(netwib_bufpool_init(NETWIB_TRUE, &pbufpool));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf1));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf1));
  netwib_er(netwib_bufpool_close(&pbufpool));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_bufpool_close_test(void)
{
  puts("++++----\nTest of netwib_bufpool_close");
  puts("Redundant with netwib_bufpool_init");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_bufpool_buf_init_test(void)
{
  puts("++++----\nTest of netwib_bufpool_buf_init");
  puts("Redundant with netwib_bufpool_init");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_bufpool_buf_close_test(void)
{
  puts("++++----\nTest of netwib_bufpool_buf_close");
  puts("Redundant with netwib_bufpool_init");
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_dat_bufpool(void)
{
  puts("###########################################\nTest of dat/bufpool");
  netwib_er(netwib_bufpool_init_test());
  netwib_er(netwib_bufpool_close_test());
  netwib_er(netwib_bufpool_buf_init_test());
  netwib_er(netwib_bufpool_buf_close_test());

  return(NETWIB_ERR_OK);
}
