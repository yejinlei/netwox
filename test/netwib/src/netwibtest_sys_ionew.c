#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err test_read(netwib_io *pio,
                            netwib_buf *pbuf)
{
  pio = pio; /* for compiler warning */
  pbuf = pbuf; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_write(netwib_io *pio,
                             netwib_constbuf *pbuf)
{
  pio = pio; /* for compiler warning */
  pbuf = pbuf; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_wait(netwib_io *pio,
                            netwib_io_waytype way,
                            netwib_consttime *pabstime,
                            netwib_bool *pevent)
{
  pio = pio; /* for compiler warning */
  way = way; /* for compiler warning */
  pabstime = pabstime; /* for compiler warning */
  pevent = pevent; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_unread(netwib_io *pio,
                              netwib_constbuf *pbuf)
{
  pio = pio; /* for compiler warning */
  pbuf = pbuf; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_ctl_set(netwib_io *pio,
                               netwib_io_waytype way,
                               netwib_io_ctltype ctltype,
                               netwib_ptr p,
                               netwib_uint32 ui)
{
  pio = pio; /* for compiler warning */
  way = way; /* for compiler warning */
  ctltype = ctltype; /* for compiler warning */
  p = p; /* for compiler warning */
  ui = ui; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_ctl_get(netwib_io *pio,
                               netwib_io_waytype way,
                               netwib_io_ctltype ctltype,
                               netwib_ptr p,
                               netwib_uint32 *pui)
{
  pio = pio; /* for compiler warning */
  way = way; /* for compiler warning */
  ctltype = ctltype; /* for compiler warning */
  p = p; /* for compiler warning */
  pui = pui; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_close(netwib_io *pio)
{
  pio = pio; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_test(void)
{
  netwib_io *pio;

  puts("++++----\nTest of netwib_io_init");
  netwib_er(netwib_io_init(NETWIB_TRUE, NETWIB_TRUE, NULL,
                           test_read, test_write, test_wait, test_unread,
                           test_ctl_set, test_ctl_get, test_close,
                           &pio));
  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_ionew(void)
{
  puts("###########################################\nTest of sys/ionew");
  netwib_er(netwib_io_init_test());

  return(NETWIB_ERR_OK);
}
