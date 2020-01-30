#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_internal_version_test(void)
{
  netwib_uint32 major, minor, micro;

  puts("++++----\nTest of netwib_internal_version");
  netwib_er(netwib_internal_version(&major, &minor, &micro));
  if (major != NETWIBDEF_VERSIONMAJOR) {
    return(NETWIBTEST_ERR_7);
  }
  if (minor != NETWIBDEF_VERSIONMINOR) {
    return(NETWIBTEST_ERR_7);
  }
  if (micro != NETWIBDEF_VERSIONMICRO) {
    return(NETWIBTEST_ERR_7);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_internal_display_test(void)
{
  puts("++++----\nTest of netwib_internal_display");
  netwib_er(netwib_internal_display());

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_internal_test(void)
{
  netwib_buf buf;

  puts("++++----\nTest of netwib_buf_append_internal");
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_internal(&buf));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_internal(void)
{
  puts("###########################################\nTest of sys/internal");
  netwib_er(netwib_internal_version_test());
  netwib_er(netwib_internal_display_test());
  netwib_er(netwib_buf_append_internal_test());

  return(NETWIB_ERR_OK);
}
