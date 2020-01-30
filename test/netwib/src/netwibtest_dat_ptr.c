#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_ptr_malloc_test(void)
{
  netwib_ptr ptr;

  puts("++++----\nTest of netwib_ptr_malloc");
  netwib_er(netwib_ptr_malloc(10, NULL));
  netwib_er(netwib_ptr_malloc(10, &ptr));
  netwib_c_memset(ptr, 34, 10);
  netwib_er(netwib_ptr_free(&ptr));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ptr_realloc_test(void)
{
  netwib_ptr ptr;

  puts("++++----\nTest of netwib_ptr_realloc");
  netwib_er(netwib_ptr_realloc(10, NULL));
  netwib_er(netwib_ptr_malloc(10, &ptr));
  netwib_c_memset(ptr, 'a', 10);
  netwib_er(netwib_ptr_realloc(20, &ptr));
  /* check if first bytes changed */
  if ( netwib_c_strncmp((netwib_conststring)ptr, "aaaaaaaaaa", 10) ) {
    return(NETWIBTEST_ERR_0);
  }
  netwib_er(netwib_ptr_realloc(2, &ptr));
  /* check if first bytes changed */
  if ( netwib_c_strncmp((netwib_conststring)ptr, "aa", 2) ) {
    return(NETWIBTEST_ERR_0);
  }
  netwib_er(netwib_ptr_free(&ptr));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ptr_free_test(void)
{
  netwib_ptr ptr;

  puts("++++----\nTest of netwib_ptr_free");
  netwib_er(netwib_ptr_free(NULL));
  netwib_er(netwib_ptr_malloc(10, &ptr));
  netwib_er(netwib_ptr_free(&ptr));
  netwib_eir(netwib_ptr_free(&ptr), NETWIB_ERR_LOOBJCLOSEALREADYCLOSED);
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_dat_ptr(void)
{
  puts("###########################################\nTest of dat/ptr");
  netwib_er(netwib_ptr_malloc_test());
  netwib_er(netwib_ptr_realloc_test());
  netwib_er(netwib_ptr_free_test());

  return(NETWIB_ERR_OK);
}
