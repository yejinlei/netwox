#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err test_endian(void)
{
  union {
    struct {
      netwib_uint8 a;
      netwib_uint8 b;
      netwib_uint8 c;
      netwib_uint8 d;
    } one;
    netwib_uint32 two;
  } u;

  puts("++++----\nTest of endianness");

  u.two = 0x12345678;
  if (NETWIBDEF_ARCH_ENDIAN==0) {
    if (u.one.a != 0x78 || u.one.b != 0x56 ||
        u.one.c != 0x34 || u.one.d != 0x12) {
      puts("Error : this computer is not a little endian");
      return(NETWIBTEST_ERR_4);
    }
  } else if (NETWIBDEF_ARCH_ENDIAN==1) {
    if (u.one.a != 0x12 || u.one.b != 0x34 ||
        u.one.c != 0x56 || u.one.d != 0x78) {
      puts("Error : this computer is not a big endian");
      return(NETWIBTEST_ERR_4);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_sizeof(void)
{
  puts("++++----\nTest of sizeof");

  if (CHAR_BIT *sizeof(netwib_char) != 8) {
    puts("Error : netwib_char must have an 8 bit size\n");
    return(NETWIBTEST_ERR_4);
  }
  if (CHAR_BIT *sizeof(netwib_byte) != 8) {
    puts("Error : netwib_byte must have an 8 bit size\n");
    return(NETWIBTEST_ERR_4);
  }
  if (CHAR_BIT *sizeof(netwib_int8) != 8) {
    puts("Error : netwib_int8 must have an 8 bit size\n");
    return(NETWIBTEST_ERR_4);
  }
  if (CHAR_BIT *sizeof(netwib_uint8) != 8) {
    puts("Error : netwib_uint8 must have an 8 bit size\n");
    return(NETWIBTEST_ERR_4);
  }
  if (CHAR_BIT *sizeof(netwib_int16) != 16) {
    puts("Error : netwib_int16 must have a 16 bit size\n");
    return(NETWIBTEST_ERR_4);
  }
  if (CHAR_BIT *sizeof(netwib_uint16) != 16) {
    puts("Error : netwib_uint16 must have a 16 bit size\n");
    return(NETWIBTEST_ERR_4);
  }
  if (CHAR_BIT *sizeof(netwib_int32) != 32) {
    puts("Error : netwib_int32 must have a 32 bit size\n");
    return(NETWIBTEST_ERR_4);
  }
  if (CHAR_BIT *sizeof(netwib_uint32) != 32) {
    puts("Error : netwib_uint32 must have a 32 bit size\n");
    return(NETWIBTEST_ERR_4);
  }
  if (CHAR_BIT *sizeof(netwib_int64) != 64) {
    puts("Error : netwib_int64 must have a 64 bit size\n");
    return(NETWIBTEST_ERR_4);
  }
  if (CHAR_BIT *sizeof(netwib_uint64) != 64) {
    puts("Error : netwib_uint64 must have a 64 bit size\n");
    return(NETWIBTEST_ERR_4);
  }
  if (sizeof(netwib_uintptr) != sizeof(void*)) {
    puts("Error : netwib_uintptr size must be equal to pointer size\n");
    return(NETWIBTEST_ERR_4);
  }
  if (sizeof(netwib_uintmax) < sizeof(netwib_uintptr)) {
    puts("Error : netwib_uintmax size must be >= to netwib_uintptr size\n");
    return(NETWIBTEST_ERR_4);
  }
  if (CHAR_BIT*sizeof(void*) != NETWIBDEF_ARCH_BITS) {
    puts("Error : NETWIBDEF_ARCH_BITS must be equal to pointer size\n");
    return(NETWIBTEST_ERR_4);
  }

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_dat_priv(void)
{
  puts("###########################################\nTest of dat/priv");
  netwib_er(test_endian());
  netwib_er(test_sizeof());

  return(NETWIB_ERR_OK);
}

