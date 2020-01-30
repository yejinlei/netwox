#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_handle_test(void)
{
  netwib_io *pio;

  puts("++++----\nTest of netwib_io_init_handle");

#if defined NETWIBDEF_SYSNAME_Unix
  netwib_eir(netwib_io_init_handle(NULL, NETWIB_FALSE, &pio),
             NETWIB_ERR_LONOTSUPPORTED);
#elif defined NETWIBDEF_SYSNAME_Windows
  netwib_er(netwib_io_init_handle(NULL, NETWIB_FALSE, &pio));
  netwib_er(netwib_io_close(&pio));
#else
#error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_windowstype_init_test(void)
{
  netwib_windowstype t;

  puts("++++----\nTest of netwib_windowstype_init");

#if defined NETWIBDEF_SYSNAME_Unix
  netwib_eir(netwib_windowstype_init(&t), NETWIB_ERR_LONOTSUPPORTED);
#elif defined NETWIBDEF_SYSNAME_Windows
  netwib_er(netwib_windowstype_init(&t));
  if (t == NETWIB_WINDOWSTYPE_UNKNOWN) {
    return(NETWIBTEST_ERR_7);
  }
#else
#error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_windows(void)
{
  puts("###########################################\nTest of sys/windows");
  netwib_er(netwib_io_init_handle_test());
  netwib_er(netwib_windowstype_init_test());

  return(NETWIB_ERR_OK);
}
