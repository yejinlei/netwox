#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwibtest_checksum_content(netwib_conststring str,
                                              netwib_uint16 wantedsum)
{
  netwib_buf buf;
  netwib_uint32 tmpchecksum;
  netwib_uint16 sum;

  netwib_er(netwib_buf_init_ext_string(str, &buf));
  netwib_er(netwib_checksum_buf(&buf, &sum));
  if (sum != wantedsum) {
    netwib_er(netwib_fmt_display("%{uint16} != %{uint16}\n", sum, wantedsum));
    return(NETWIBTEST_ERR_6);
  }

  netwib_er(netwib_checksum_init(&tmpchecksum));
  if (netwib_c_strlen(str) >= 2) {
    netwib_er(netwib_checksum_update_data((netwib_constdata)str, 2,
                                          &tmpchecksum));
    netwib_er(netwib_checksum_update_data((netwib_constdata)str+2,
                                          netwib_c_strlen(str)-2,
                                          &tmpchecksum));
  } else {
    netwib_er(netwib_checksum_update_data((netwib_constdata)str,
                                          netwib_c_strlen(str),
                                          &tmpchecksum));
  }
  netwib_er(netwib_checksum_close(tmpchecksum, &sum));
  if (sum != wantedsum) {
    netwib_er(netwib_fmt_display("%{uint16} != %{uint16}\n", sum, wantedsum));
    return(NETWIBTEST_ERR_6);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_checksum_buf_test(void)
{
  puts("++++----\nTest of netwib_checksum_buf");

  netwib_er(netwibtest_checksum_content("", 65535));
  netwib_er(netwibtest_checksum_content("a", 40703));
  netwib_er(netwibtest_checksum_content("ab", 40605));
  netwib_er(netwibtest_checksum_content("abc", 15261));
  netwib_er(netwibtest_checksum_content("abcdefghij", 1280));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_checksum_init_test(void)
{
  puts("++++----\nTest of netwib_checksum_init");
  puts("Redundant with netwib_checksum_buf");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_checksum_update_buf_test(void)
{
  puts("++++----\nTest of netwib_checksum_update_buf");
  puts("Redundant with netwib_checksum_buf");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_checksum_update_data_test(void)
{
  puts("++++----\nTest of netwib_checksum_update_data");
  puts("Redundant with netwib_checksum_buf");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_checksum_close_test(void)
{
  puts("++++----\nTest of netwib_checksum_close");
  puts("Redundant with netwib_checksum_buf");
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_dat_checksum(void)
{
  puts("###########################################\nTest of dat/checksum");
  netwib_er(netwib_checksum_buf_test());
  netwib_er(netwib_checksum_init_test());
  netwib_er(netwib_checksum_update_buf_test());
  netwib_er(netwib_checksum_update_data_test());
  netwib_er(netwib_checksum_close_test());

  return(NETWIB_ERR_OK);
}

