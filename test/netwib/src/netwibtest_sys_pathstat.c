#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err netwib_pathstat_init_test2(netwib_conststring pathname,
                                                  netwib_pathstat_type wantedtype)
{
  netwib_buf buf;
  netwib_pathstat pathstat;

  netwib_er(netwib_buf_init_ext_string(pathname, &buf));
  netwib_er(netwib_pathstat_init(&buf, &pathstat));
  netwib_er(netwib_fmt_display("Pathname %s:\ntype=%{uint32} size=%{uint32} size64=%{uint64} mtime=%{uint32}:%{uint32}\n",
                               pathname, pathstat.type, pathstat.size, pathstat.size64, pathstat.mtime.sec, pathstat.mtime.nsec));
  if (pathstat.type != wantedtype) {
    netwib_er(netwib_fmt_display("Pathname %s is of type %{uint32}\n",
                                 pathname, pathstat.type));
    return(NETWIBTEST_ERR_8);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pathstat_init_test(void)
{

  puts("++++----\nTest of netwib_pathstat_init");

  netwib_er(netwib_pathstat_init_test2("./files/record",
                                       NETWIB_PATHSTAT_TYPE_REG));

#ifdef NETWIBTESTDEF_DIR_NAME_1
  if (NETWIBTESTDEF_DIR_EXISTS_1) {
    netwib_er(netwib_pathstat_init_test2(NETWIBTESTDEF_DIR_NAME_1,
                                         NETWIB_PATHSTAT_TYPE_DIR));
  }
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_2
  if (NETWIBTESTDEF_DIR_EXISTS_2) {
    netwib_er(netwib_pathstat_init_test2(NETWIBTESTDEF_DIR_NAME_2,
                                         NETWIB_PATHSTAT_TYPE_DIR));
  }
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_3
  if (NETWIBTESTDEF_DIR_EXISTS_3) {
    netwib_er(netwib_pathstat_init_test2(NETWIBTESTDEF_DIR_NAME_3,
                                         NETWIB_PATHSTAT_TYPE_DIR));
  }
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_4
  if (NETWIBTESTDEF_DIR_EXISTS_4) {
    netwib_er(netwib_pathstat_init_test2(NETWIBTESTDEF_DIR_NAME_4,
                                         NETWIB_PATHSTAT_TYPE_DIR));
  }
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_5
  if (NETWIBTESTDEF_DIR_EXISTS_5) {
    netwib_er(netwib_pathstat_init_test2(NETWIBTESTDEF_DIR_NAME_5,
                                         NETWIB_PATHSTAT_TYPE_DIR));
  }
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_6
  if (NETWIBTESTDEF_DIR_EXISTS_6) {
    netwib_er(netwib_pathstat_init_test2(NETWIBTESTDEF_DIR_NAME_6,
                                         NETWIB_PATHSTAT_TYPE_DIR));
  }
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_7
  if (NETWIBTESTDEF_DIR_EXISTS_7) {
    netwib_er(netwib_pathstat_init_test2(NETWIBTESTDEF_DIR_NAME_7,
                                         NETWIB_PATHSTAT_TYPE_DIR));
  }
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_8
  if (NETWIBTESTDEF_DIR_EXISTS_8) {
    netwib_er(netwib_pathstat_init_test2(NETWIBTESTDEF_DIR_NAME_8,
                                         NETWIB_PATHSTAT_TYPE_DIR));
  }
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_9
  if (NETWIBTESTDEF_DIR_EXISTS_9) {
    netwib_er(netwib_pathstat_init_test2(NETWIBTESTDEF_DIR_NAME_9,
                                         NETWIB_PATHSTAT_TYPE_DIR));
  }
#endif
#ifdef NETWIBTESTDEF_DIR_NAME_10
  if (NETWIBTESTDEF_DIR_EXISTS_10) {
    netwib_er(netwib_pathstat_init_test2(NETWIBTESTDEF_DIR_NAME_10,
                                         NETWIB_PATHSTAT_TYPE_DIR));
  }
#endif

#ifdef NETWIBTESTDEF_FILE_NAME_1
  if (NETWIBTESTDEF_FILE_EXISTS_1) {
    netwib_er(netwib_pathstat_init_test2(NETWIBTESTDEF_FILE_NAME_1,
                                         NETWIB_PATHSTAT_TYPE_REG));
  }
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_2
  if (NETWIBTESTDEF_FILE_EXISTS_2) {
    netwib_er(netwib_pathstat_init_test2(NETWIBTESTDEF_FILE_NAME_2,
                                         NETWIB_PATHSTAT_TYPE_REG));
  }
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_3
  if (NETWIBTESTDEF_FILE_EXISTS_3) {
    netwib_er(netwib_pathstat_init_test2(NETWIBTESTDEF_FILE_NAME_3,
                                         NETWIB_PATHSTAT_TYPE_REG));
  }
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_4
  if (NETWIBTESTDEF_FILE_EXISTS_4) {
    netwib_er(netwib_pathstat_init_test2(NETWIBTESTDEF_FILE_NAME_4,
                                         NETWIB_PATHSTAT_TYPE_REG));
  }
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_5
  if (NETWIBTESTDEF_FILE_EXISTS_5) {
    netwib_er(netwib_pathstat_init_test2(NETWIBTESTDEF_FILE_NAME_5,
                                         NETWIB_PATHSTAT_TYPE_REG));
  }
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_6
  if (NETWIBTESTDEF_FILE_EXISTS_6) {
    netwib_er(netwib_pathstat_init_test2(NETWIBTESTDEF_FILE_NAME_6,
                                         NETWIB_PATHSTAT_TYPE_REG));
  }
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_7
  if (NETWIBTESTDEF_FILE_EXISTS_7) {
    netwib_er(netwib_pathstat_init_test2(NETWIBTESTDEF_FILE_NAME_7,
                                         NETWIB_PATHSTAT_TYPE_REG));
  }
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_8
  if (NETWIBTESTDEF_FILE_EXISTS_8) {
    netwib_er(netwib_pathstat_init_test2(NETWIBTESTDEF_FILE_NAME_8,
                                             NETWIB_PATHSTAT_TYPE_REG));
  }
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_9
  if (NETWIBTESTDEF_FILE_EXISTS_9) {
    netwib_er(netwib_pathstat_init_test2(NETWIBTESTDEF_FILE_NAME_9,
                                         NETWIB_PATHSTAT_TYPE_REG));
  }
#endif
#ifdef NETWIBTESTDEF_FILE_NAME_10
  if (NETWIBTESTDEF_FILE_EXISTS_10) {
    netwib_er(netwib_pathstat_init_test2(NETWIBTESTDEF_FILE_NAME_10,
                                         NETWIB_PATHSTAT_TYPE_REG));
  }
#endif

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_pathstat(void)
{
  puts("###########################################\nTest of sys/pathstat");
  netwib_er(netwib_pathstat_init_test());

  return(NETWIB_ERR_OK);
}
