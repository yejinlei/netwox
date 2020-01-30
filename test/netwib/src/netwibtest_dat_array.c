#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_array_init_test(void)
{
  netwib_array array;

  puts("++++----\nTest of netwib_array_init");
  netwib_eir(netwib_array_init(0, 0, NULL), NETWIB_ERR_PANULLPTR);

  netwib_er(netwib_array_init(1, 10, &array));
  netwib_er(netwib_array_close(&array));

  netwib_er(netwib_array_init(4, 10, &array));
  netwib_er(netwib_array_close(&array));

  netwib_er(netwib_array_init(8, 10, &array));
  netwib_er(netwib_array_close(&array));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_array_close_test(void)
{
  netwib_array array;

  puts("++++----\nTest of netwib_array_close");

  netwib_er(netwib_array_init(8, 10, &array));
  netwib_er(netwib_array_close(&array));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_array_ctl_set_test(void)
{
  netwib_array array;
  netwib_uint32 size;

  puts("++++----\nTest of netwib_array_ctl_set");

  netwib_er(netwib_array_init(8, 10, &array));
  netwib_er(netwib_array_ctl_get_size(&array, &size));
  if (size != 10) {
    return(NETWIBTEST_ERR_9);
  }
  netwib_er(netwib_array_ctl_set_size(&array, 64));
  netwib_er(netwib_array_ctl_get_size(&array, &size));
  if (size != 64) {
    return(NETWIBTEST_ERR_9);
  }
  netwib_er(netwib_array_ctl_set_size(&array, 16));
  netwib_er(netwib_array_ctl_get_size(&array, &size));
  if (size != 16) {
    return(NETWIBTEST_ERR_9);
  }
  netwib_er(netwib_array_close(&array));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 ui;
  netwib_char c;
} netwibt_foobar;
static netwib_err netwib_array_ctl_get_test2(netwib_uint32 size)
{
  netwib_array array;
  netwibt_foobar *pfb;
  netwib_uint32 i;

  netwib_er(netwib_array_init(sizeof(netwibt_foobar), size, &array));
  for (i = 0; i < array.size; i++) {
    pfb = (netwibt_foobar*)array.p[i];
    pfb->ui = i;
    pfb->c = (netwib_char)i;
  }
  for (i = 0; i < array.size; i++) {
    pfb = (netwibt_foobar*)array.p[i];
    if (pfb->ui != i) return(NETWIBTEST_ERR_9);
    if (pfb->c != (netwib_char)i) return(NETWIBTEST_ERR_9);
  }
  netwib_er(netwib_array_ctl_set_size(&array, 2*size));
  for (i = size; i < array.size; i++) {
    pfb = (netwibt_foobar*)array.p[i];
    pfb->ui = i;
    pfb->c = (netwib_char)i;
  }
  for (i = 0; i < array.size; i++) {
    pfb = (netwibt_foobar*)array.p[i];
    if (pfb->ui != i) return(NETWIBTEST_ERR_9);
    if (pfb->c != (netwib_char)i) return(NETWIBTEST_ERR_9);
    netwib_er(netwib_fmt_display(" %{uint32} %p\n", i, array.p[i]));
  }
  netwib_er(netwib_array_close(&array));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_array_ctl_get_test(void)
{

  puts("++++----\nTest of netwib_array_ctl_get");
  netwib_er(netwib_array_ctl_get_test2(1));
  netwib_er(netwib_array_ctl_get_test2(2));
  netwib_er(netwib_array_ctl_get_test2(4));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_dat_array(void)
{
  puts("###########################################\nTest of dat/array");

  netwib_er(netwib_array_init_test());
  netwib_er(netwib_array_close_test());
  netwib_er(netwib_array_ctl_set_test());
  netwib_er(netwib_array_ctl_get_test());

  return(NETWIB_ERR_OK);
}
