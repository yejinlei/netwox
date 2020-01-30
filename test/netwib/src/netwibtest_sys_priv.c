#include "netwibtest.h"


/*-------------------------------------------------------------*/
static netwib_err test_priv_cmdline_check(netwib_conststring command,
                                          netwib_conststring wantedmixed)
{
  netwib_buf commandbuf, obtainedbuf;
  netwib_string filename, *argv;
  int i, argc;

  netwib_er(netwib_buf_init_ext_string(command, &commandbuf));
  netwib_er(netwib_priv_cmdline_init(&commandbuf, &filename, &argc, &argv));

  netwib_er(netwib_buf_init_mallocdefault(&obtainedbuf));
  netwib_er(netwib_buf_append_string(filename, &obtainedbuf));
  i = 0;
  while (argv[i] != NULL) {
    netwib_er(netwib_buf_append_fmt(&obtainedbuf, " %{uint32}=", i));
    netwib_er(netwib_buf_append_string(argv[i], &obtainedbuf));
    i++;
  }
  netwib_er(netwib_priv_cmdline_close(&filename, &argv));
  if (argc != i) return(NETWIBTEST_ERR_3);

  netwib_er(netwibtest_buf_content3(&obtainedbuf, wantedmixed));
  netwib_er(netwib_buf_close(&obtainedbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err test_priv_cmdline(void)
{
  puts("++++----\nTest of netwib_priv_cmdline");

  netwib_er(test_priv_cmdline_check("ls", "'ls 0=ls'"));
  netwib_er(test_priv_cmdline_check("  ls  ", "'ls 0=ls'"));
  netwib_er(test_priv_cmdline_check("/bin/ls", "'/bin/ls 0=ls'"));
  netwib_er(test_priv_cmdline_check("/bin\\ls", "'/bin\\ls 0=ls'"));
  netwib_er(test_priv_cmdline_check("/bin/ls a ", "'/bin/ls 0=ls 1=a'"));
  netwib_er(test_priv_cmdline_check("/bin/ls a b c d e f g h i j k l m n o p q r s t u v w x y z", "'/bin/ls 0=ls 1=a 2=b 3=c 4=d 5=e 6=f 7=g 8=h 9=i 10=j 11=k 12=l 13=m 14=n 15=o 16=p 17=q 18=r 19=s 20=t 21=u 22=v 23=w 24=x 25=y 26=z'"));
  netwib_er(test_priv_cmdline_check("/bin/ls a  b \"c\"",
                                    "'/bin/ls 0=ls 1=a 2=b 3=c'"));
  netwib_er(test_priv_cmdline_check("ls \"a \\\"'\\t\\\\a\\x41\"",
                                    "'ls 0=ls 1=a \"''\t\\aA'"));
  netwib_er(test_priv_cmdline_check("\"/bin/ls\"", "'/bin/ls 0=ls'"));
  netwib_er(test_priv_cmdline_check("\"/bin l/s\"", "'/bin l/s 0=s'"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
#elif defined NETWIBDEF_SYSNAME_Windows
static netwib_err test_priv_cmdline_win(void)
{
  netwib_buf buf1, buf2;

  puts("++++----\nTest of netwib_priv_cmdline_win");
  netwib_er(netwib_buf_init_mallocdefault(&buf2));

  netwib_er(netwib_buf_init_ext_string("dir", &buf1));
  netwib_er(netwib_priv_cmdline_win(&buf1, &buf2));
  netwib_er(netwibtest_buf_content3(&buf2, "'dir'"));

  netwib_er(netwib_buf_init_ext_string("dir c:", &buf1));
  netwib__buf_reinit(&buf2);
  netwib_er(netwib_priv_cmdline_win(&buf1, &buf2));
  netwib_er(netwibtest_buf_content3(&buf2, "'dir c:'"));

  netwib_er(netwib_buf_init_ext_string("dir \"c:prog file\"", &buf1));
  netwib__buf_reinit(&buf2);
  netwib_er(netwib_priv_cmdline_win(&buf1, &buf2));
  netwib_er(netwibtest_buf_content3(&buf2, "'dir \"c:prog file\"'"));

  netwib_er(netwib_buf_init_ext_string("d/i\\r \"a/b\\\\c\"", &buf1));
  netwib__buf_reinit(&buf2);
  netwib_er(netwib_priv_cmdline_win(&buf1, &buf2));
  netwib_er(netwibtest_buf_content3(&buf2, "'d\\i\\r a/b\\c'"));

  netwib_er(netwib_buf_close(&buf2));
  return(NETWIB_ERR_OK);
}

#else
#error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_priv(void)
{
  puts("###########################################\nTest of sys/priv");
  netwib_er(test_priv_cmdline());

#if defined NETWIBDEF_SYSNAME_Unix
#elif defined NETWIBDEF_SYSNAME_Windows
  netwib_er(test_priv_cmdline_win());
#else
#error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  return(NETWIB_ERR_OK);
}
