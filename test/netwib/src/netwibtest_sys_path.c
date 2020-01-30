#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err t1(netwib_conststring path,
                     netwib_conststring canonpath)
{
  netwib_buf buf1, buf2;
  netwib_err ret;

  netwib_er(netwib_fmt_display("%s\n", path));

  netwib_er(netwib_buf_init_ext_string(path, &buf1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  ret = netwib_path_canon(&buf1, &buf2);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwibtest_buf_content2(&buf2, canonpath));
  }
  netwib_er(netwib_buf_close(&buf2));

  netwib_er(netwib_buf_init_ext_string(path, &buf1));
  netwib_er(netwib_buf_init_mallocdefault(&buf2));
  netwib_er(netwib_buf_append_string("a/b/../c/d/", &buf2));
  ret = netwib_path_canon(&buf1, &buf2);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_init_mallocdefault(&buf1));
    netwib_er(netwib_buf_append_string("a/b/../c/d/", &buf1));
    netwib_er(netwib_buf_append_string(canonpath, &buf1));
    netwib_er(netwib_buf_append_byte('\0', &buf1));
    netwib_er(netwibtest_buf_content2(&buf2, (netwib_conststring)netwib__buf_ref_data_ptr(&buf1)));
    netwib_er(netwib_buf_close(&buf1));
  }
  netwib_er(netwib_buf_close(&buf2));

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_path_canon_test(void)
{
  puts("++++----\nTest of netwib_path_canon");

  /* test suite */
  netwib_er(t1("file", "file"));
  netwib_er(t1("./file", "file"));
  netwib_er(t1(".\\file", "file"));
  netwib_er(t1("dir/", "dir"));
  netwib_er(t1("dir\\", "dir"));
  netwib_er(t1("./dir/", "dir"));
  netwib_er(t1(".\\dir\\", "dir"));
  netwib_er(t1("dir/file", "dir/file"));
  netwib_er(t1("dir\\file", "dir/file"));
  netwib_er(t1("./dir/file", "dir/file"));
  netwib_er(t1(".\\dir\\file", "dir/file"));
  netwib_er(t1("/file", "/file"));
  netwib_er(t1("\\file", "/file"));
  netwib_er(t1("/dir/", "/dir"));
  netwib_er(t1("\\dir\\", "/dir"));
  netwib_er(t1("/dir/file", "/dir/file"));
  netwib_er(t1("\\dir\\file", "/dir/file"));
  netwib_er(t1("../file", "../file"));
  netwib_er(t1("..\\file", "../file"));
  netwib_er(t1("../dir/", "../dir"));
  netwib_er(t1("..\\dir\\", "../dir"));
  netwib_er(t1("../dir/file", "../dir/file"));
  netwib_er(t1("..\\dir\\file", "../dir/file"));
  netwib_er(t1("/", "/"));
  netwib_er(t1("\\", "/"));
  netwib_er(t1(".", "."));
  netwib_er(t1("..", ".."));
  netwib_er(t1("c:file", "c:file"));
  netwib_er(t1("c:./file", "c:file"));
  netwib_er(t1("c:.\\file", "c:file"));
  netwib_er(t1("C:file", "c:file"));
  netwib_er(t1("c:dir/", "c:dir"));
  netwib_er(t1("c:dir\\", "c:dir"));
  netwib_er(t1("c:./dir/", "c:dir"));
  netwib_er(t1("c:.\\dir\\", "c:dir"));
  netwib_er(t1("c:dir/file", "c:dir/file"));
  netwib_er(t1("c:dir\\file", "c:dir/file"));
  netwib_er(t1("c:./dir/file", "c:dir/file"));
  netwib_er(t1("c:.\\dir\\file", "c:dir/file"));
  netwib_er(t1("c:/file", "c:/file"));
  netwib_er(t1("c:\\file", "c:/file"));
  netwib_er(t1("c:/dir/", "c:/dir"));
  netwib_er(t1("c:\\dir\\", "c:/dir"));
  netwib_er(t1("c:/dir/file", "c:/dir/file"));
  netwib_er(t1("c:\\dir\\file", "c:/dir/file"));
  netwib_er(t1("c:../file", "c:../file"));
  netwib_er(t1("c:..\\file", "c:../file"));
  netwib_er(t1("c:../dir/", "c:../dir"));
  netwib_er(t1("c:..\\dir\\", "c:../dir"));
  netwib_er(t1("c:../dir/file", "c:../dir/file"));
  netwib_er(t1("c:..\\dir\\file", "c:../dir/file"));
  netwib_er(t1("c:/", "c:/"));
  netwib_er(t1("c:\\", "c:/"));
  netwib_er(t1("c:", "c:"));
  netwib_er(t1("c:.", "c:"));
  netwib_er(t1("c:..", "c:.."));
  netwib_er(t1("//h/sh/file", "//h/sh/file"));
  netwib_er(t1("\\\\h\\sh\\file", "//h/sh/file"));
  netwib_er(t1("//H/SH/file", "//h/sh/file"));
  netwib_er(t1("//h/sh/dir/", "//h/sh/dir"));
  netwib_er(t1("\\\\h\\sh\\dir\\", "//h/sh/dir"));
  netwib_er(t1("//h/sh/dir/file", "//h/sh/dir/file"));
  netwib_er(t1("\\\\h\\sh\\dir\\file", "//h/sh/dir/file"));
  netwib_er(t1("//h/sh/", "//h/sh/"));
  netwib_er(t1("\\\\h\\sh\\", "//h/sh/"));

  /* other tests */
  netwib_er(t1("/tmp////tutu", "/tmp/tutu"));
  netwib_er(t1("////tmp////tutu", "/tmp/tutu"));
  netwib_er(t1("////tmp/tutu", "/tmp/tutu"));
  netwib_er(t1("//", "/"));
  netwib_er(t1("/./", "/"));
  netwib_er(t1("aa/..", "."));
  netwib_er(t1("aa/../aa/..", "."));
  netwib_er(t1("../", ".."));
  netwib_er(t1("aa/./bb", "aa/bb"));
  netwib_er(t1("aa/././bb", "aa/bb"));
  netwib_er(t1("a/././b", "a/b"));
  netwib_er(t1("/././b", "/b"));
  netwib_er(t1("aa/././", "aa"));
  netwib_er(t1("aa/./.", "aa"));
  netwib_er(t1("/./.", "/"));
  netwib_er(t1("aa/.", "aa"));
  netwib_er(t1("aa/bb/..", "aa"));
  netwib_er(t1("aa/bb/cc/..", "aa/bb"));
  netwib_er(t1("aa/bb/cc/../dd", "aa/bb/dd"));
  netwib_er(t1("aa/bb/cc/../dd/../ee", "aa/bb/ee"));
  netwib_er(t1("aa/bb/cc/../..", "aa"));
  netwib_er(t1("aa/bb/cc/../../", "aa"));
  netwib_er(t1("aa/bb/cc/../../dd/../ee", "aa/ee"));
  netwib_er(t1("aa/bb/../../dd/../ee", "ee"));
  netwib_er(t1("./aa/.", "aa"));
  netwib_er(t1("./aa/bb/..", "aa"));
  netwib_er(t1("./aa/bb/cc/..", "aa/bb"));
  netwib_er(t1("./aa/bb/cc/../dd", "aa/bb/dd"));
  netwib_er(t1("./aa/bb/cc/../dd/../ee", "aa/bb/ee"));
  netwib_er(t1("./aa/bb/cc/../..", "aa"));
  netwib_er(t1("./aa/bb/cc/../../", "aa"));
  netwib_er(t1("./aa/bb/cc/../../dd/../ee", "aa/ee"));
  netwib_er(t1("./aa/bb/../../dd/../ee", "ee"));
  netwib_er(t1("aa/..", "."));
  netwib_er(t1("aa/../..", ".."));
  netwib_er(t1("aa/../../", ".."));
  netwib_er(t1("aa/../../b", "../b"));
  netwib_er(t1("aa/../bb/../../c/d/e/../", "../c/d"));
  netwib_er(t1("../bb/../cc/aa", "../cc/aa"));
  netwib_er(t1("../bb/dd/../cc/aa/../ee", "../bb/cc/ee"));
  netwib_er(t1("aa/../../bb/../../../cc", "../../../cc"));
  netwib_er(t1("aa/../../bb/../../../cc/../../dd", "../../../../dd"));
  netwib_er(t1("aa/../../bb/../../../cc/../ee/dd", "../../../ee/dd"));
  netwib_er(t1("/aa/..", "/"));
  netwib_er(t1("/aa/../", "/"));
  netwib_eir(t1("/..", ""), NETWIB_ERR_PAPATHROOTDOTDOT);
  netwib_eir(t1("/../../", ""), NETWIB_ERR_PAPATHROOTDOTDOT);
  netwib_eir(t1("/aa/../../", ""), NETWIB_ERR_PAPATHROOTDOTDOT);
  netwib_er(t1("aa///////bb/.././././///./../dd///../ee", "ee"));
  netwib_er(t1("aa/..\\../bb/../..\\../cc/../ee/dd", "../../../ee/dd"));
  netwib_er(t1("d:\\dir\\..\\", "d:/"));
  netwib_er(t1("d:\\dir\\..\\file", "d:/file"));
  netwib_er(t1("\\dir\\file", "/dir/file"));
  netwib_er(t1("\\\\comp", "/comp"));
  netwib_er(t1("\\\\comp\\", "/comp"));
  netwib_er(t1("\\\\comp\\sh/dir/..\\file", "//comp/sh/file"));
  netwib_er(t1("//comp", "/comp"));
  netwib_er(t1("//comp/", "/comp"));
  netwib_er(t1("d:dir/../file", "d:file"));
  netwib_er(t1("d:dir/../../file", "d:../file"));
  netwib_eir(t1("c:\\..", ""), NETWIB_ERR_PAPATHROOTDOTDOT);
  netwib_eir(t1("c:\\..\\", ""), NETWIB_ERR_PAPATHROOTDOTDOT);
  netwib_eir(t1("\\\\comp\\sh\\..\\", ""), NETWIB_ERR_PAPATHROOTDOTDOT);
  netwib_eir(t1("//comp/sh/../", ""), NETWIB_ERR_PAPATHROOTDOTDOT);
  netwib_eir(t1("", ""), NETWIB_ERR_PAPATHNOTCANON);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t2(netwib_conststring path,
                     netwib_pathtype wantedpathtype)
{
  netwib_buf buf;
  netwib_pathtype pathtype;

  netwib_er(netwib_fmt_display("%s\n", path));

  netwib_er(netwib_buf_init_ext_string(path, &buf));
  netwib_er(netwib_pathtype_init(&buf, &pathtype));
  if (pathtype != wantedpathtype) {
    return(NETWIBTEST_ERR_9);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_pathtype_init_test(void)
{

  puts("++++----\nTest of netwib_pathtype_init");

  /* test suite */
  netwib_er(t2("file", NETWIB_PATHTYPE_UNIX));
  netwib_er(t2("./file", NETWIB_PATHTYPE_UNIX));
  netwib_er(t2(".\\file", NETWIB_PATHTYPE_UNIX));
  netwib_er(t2("dir/", NETWIB_PATHTYPE_UNIX));
  netwib_er(t2("dir\\", NETWIB_PATHTYPE_UNIX));
  netwib_er(t2("./dir/", NETWIB_PATHTYPE_UNIX));
  netwib_er(t2(".\\dir\\", NETWIB_PATHTYPE_UNIX));
  netwib_er(t2("dir/file", NETWIB_PATHTYPE_UNIX));
  netwib_er(t2("dir\\file", NETWIB_PATHTYPE_UNIX));
  netwib_er(t2("./dir/file", NETWIB_PATHTYPE_UNIX));
  netwib_er(t2(".\\dir\\file", NETWIB_PATHTYPE_UNIX));
  netwib_er(t2("/file", NETWIB_PATHTYPE_ABSOLUTE | NETWIB_PATHTYPE_UNIX));
  netwib_er(t2("\\file", NETWIB_PATHTYPE_ABSOLUTE | NETWIB_PATHTYPE_UNIX));
  netwib_er(t2("/dir/", NETWIB_PATHTYPE_ABSOLUTE | NETWIB_PATHTYPE_UNIX));
  netwib_er(t2("\\dir\\", NETWIB_PATHTYPE_ABSOLUTE | NETWIB_PATHTYPE_UNIX));
  netwib_er(t2("/dir/file", NETWIB_PATHTYPE_ABSOLUTE | NETWIB_PATHTYPE_UNIX));
  netwib_er(t2("\\dir\\file", NETWIB_PATHTYPE_ABSOLUTE |
               NETWIB_PATHTYPE_UNIX));
  netwib_er(t2("../file", NETWIB_PATHTYPE_UNIX));
  netwib_er(t2("..\\file", NETWIB_PATHTYPE_UNIX));
  netwib_er(t2("../dir/", NETWIB_PATHTYPE_UNIX));
  netwib_er(t2("..\\dir\\", NETWIB_PATHTYPE_UNIX));
  netwib_er(t2("../dir/file", NETWIB_PATHTYPE_UNIX));
  netwib_er(t2("..\\dir\\file", NETWIB_PATHTYPE_UNIX));
  netwib_er(t2("/", NETWIB_PATHTYPE_ABSOLUTE | NETWIB_PATHTYPE_ROOT |
               NETWIB_PATHTYPE_UNIX));
  netwib_er(t2("\\", NETWIB_PATHTYPE_ABSOLUTE | NETWIB_PATHTYPE_ROOT |
               NETWIB_PATHTYPE_UNIX));
  netwib_er(t2(".", NETWIB_PATHTYPE_UNIX));
  netwib_er(t2("..", NETWIB_PATHTYPE_UNIX));
  netwib_er(t2("c:file", NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:./file", NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:.\\file", NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("C:file", NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:dir/", NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:dir\\", NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:./dir/", NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:.\\dir\\", NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:dir/file", NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:dir\\file", NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:./dir/file", NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:.\\dir\\file", NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:/file", NETWIB_PATHTYPE_ABSOLUTE |
               NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:\\file", NETWIB_PATHTYPE_ABSOLUTE |
               NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:/dir/", NETWIB_PATHTYPE_ABSOLUTE |
               NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:\\dir\\", NETWIB_PATHTYPE_ABSOLUTE |
               NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:/dir/file", NETWIB_PATHTYPE_ABSOLUTE |
               NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:\\dir\\file", NETWIB_PATHTYPE_ABSOLUTE |
               NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:../file", NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:..\\file", NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:../dir/", NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:..\\dir\\", NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:../dir/file", NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:..\\dir\\file", NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:/", NETWIB_PATHTYPE_ABSOLUTE | NETWIB_PATHTYPE_ROOT |
               NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:\\", NETWIB_PATHTYPE_ABSOLUTE | NETWIB_PATHTYPE_ROOT |
               NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:", NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:.", NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("c:..", NETWIB_PATHTYPE_WINDRIVE));
  netwib_er(t2("//h/sh/file", NETWIB_PATHTYPE_ABSOLUTE |
               NETWIB_PATHTYPE_WINSHARE));
  netwib_er(t2("\\\\h\\sh\\file", NETWIB_PATHTYPE_ABSOLUTE |
               NETWIB_PATHTYPE_WINSHARE));
  netwib_er(t2("//H/SH/file", NETWIB_PATHTYPE_ABSOLUTE |
               NETWIB_PATHTYPE_WINSHARE));
  netwib_er(t2("//h/sh/dir/", NETWIB_PATHTYPE_ABSOLUTE |
               NETWIB_PATHTYPE_WINSHARE));
  netwib_er(t2("\\\\h\\sh\\dir\\", NETWIB_PATHTYPE_ABSOLUTE |
               NETWIB_PATHTYPE_WINSHARE));
  netwib_er(t2("//h/sh/dir/file", NETWIB_PATHTYPE_ABSOLUTE |
               NETWIB_PATHTYPE_WINSHARE));
  netwib_er(t2("\\\\h\\sh\\dir\\file", NETWIB_PATHTYPE_ABSOLUTE |
               NETWIB_PATHTYPE_WINSHARE));
  netwib_er(t2("//h/sh/", NETWIB_PATHTYPE_ABSOLUTE | NETWIB_PATHTYPE_ROOT |
               NETWIB_PATHTYPE_WINSHARE));
  netwib_er(t2("\\\\h\\sh\\", NETWIB_PATHTYPE_ABSOLUTE | NETWIB_PATHTYPE_ROOT |
               NETWIB_PATHTYPE_WINSHARE));

  /* other tests */

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t3(netwib_conststring path,
                     netwib_bool alloc,
                     netwib_path_decodetype type,
                     netwib_conststring wanted)
{
  netwib_buf buf1, buf2;
  netwib_err ret;

  if (alloc) {
    netwib_er(netwib_buf_init_mallocdefault(&buf2));
  }

  netwib_er(netwib_buf_init_ext_string(path, &buf1));
  ret = netwib_path_decode(&buf1, type, &buf2);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwibtest_buf_content2(&buf2, wanted));
  } else if (ret == NETWIB_ERR_PAPATHROOTDOTDOT) {
    netwib__buf_reinit(&buf2);
    netwib_er(netwib_buf_append_string("PATHROOTDOTDOT", &buf2));
    netwib_er(netwibtest_buf_content2(&buf2, wanted));
  } else {
    return(ret);
  }
  if (alloc) {
    netwib_er(netwib_buf_close(&buf2));
  }

  return(NETWIB_ERR_OK);
}
static netwib_err t3a(netwib_conststring path,
                      netwib_conststring begin,
                      netwib_conststring core,
                      netwib_conststring parent,
                      netwib_conststring child)
{

  netwib_er(netwib_fmt_display("%s\n", path));

  netwib_er(t3(path, NETWIB_TRUE, NETWIB_PATH_DECODETYPE_BEGIN, begin));
  netwib_er(t3(path, NETWIB_TRUE, NETWIB_PATH_DECODETYPE_CORE, core));
  netwib_er(t3(path, NETWIB_TRUE, NETWIB_PATH_DECODETYPE_PARENT, parent));
  netwib_er(t3(path, NETWIB_TRUE, NETWIB_PATH_DECODETYPE_CHILD, child));

  return(NETWIB_ERR_OK);
}
static netwib_err t3b(netwib_conststring path,
                      netwib_conststring extension)
{

  netwib_er(netwib_fmt_display("%s\n", path));

  netwib_er(t3(path, NETWIB_FALSE, NETWIB_PATH_DECODETYPE_EXTENSION,
               extension));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_path_decode_test(void)
{

  puts("++++----\nTest of netwib_path_decode");

  /* test suite */
  netwib_er(t3a("file",    ".", "file", ".", "file"));
  netwib_er(t3a("./file",  ".", "file", ".", "file"));
  netwib_er(t3a(".\\file", ".", "file", ".", "file"));
  netwib_er(t3a("dir/",     ".", "dir", ".", "dir"));
  netwib_er(t3a("dir\\",    ".", "dir", ".", "dir"));
  netwib_er(t3a("./dir/",   ".", "dir", ".", "dir"));
  netwib_er(t3a(".\\dir\\", ".", "dir", ".", "dir"));
  netwib_er(t3a("dir/file",     ".", "dir/file", "dir", "file"));
  netwib_er(t3a("dir\\file",    ".", "dir/file", "dir", "file"));
  netwib_er(t3a("./dir/file",   ".", "dir/file", "dir", "file"));
  netwib_er(t3a(".\\dir\\file", ".", "dir/file", "dir", "file"));
  netwib_er(t3a("/file",  "/", "/file", "/", "file"));
  netwib_er(t3a("\\file", "/", "/file", "/", "file"));
  netwib_er(t3a("/dir",  "/", "/dir", "/", "dir"));
  netwib_er(t3a("\\dir", "/", "/dir", "/", "dir"));
  netwib_er(t3a("/dir/file",   "/", "/dir/file", "/dir", "file"));
  netwib_er(t3a("\\dir\\file", "/", "/dir/file", "/dir", "file"));
  netwib_er(t3a("../file",  ".", "../file", "..", "file"));
  netwib_er(t3a("..\\file", ".", "../file", "..", "file"));
  netwib_er(t3a("../dir/",   ".", "../dir", "..", "dir"));
  netwib_er(t3a("..\\dir\\", ".", "../dir", "..", "dir"));
  netwib_er(t3a("../dir/file",   ".", "../dir/file", "../dir", "file"));
  netwib_er(t3a("..\\dir\\file", ".", "../dir/file", "../dir", "file"));
  netwib_er(t3a("/",  "/", "/", "PATHROOTDOTDOT", "/"));
  netwib_er(t3a("\\", "/", "/", "PATHROOTDOTDOT", "/"));
  netwib_er(t3a(".", ".", ".", "..", "."));
  netwib_er(t3a("..", ".", "..", "../..", ".."));
  netwib_er(t3a("c:file",    "c:", "file", "c:", "file"));
  netwib_er(t3a("c:./file",  "c:", "file", "c:", "file"));
  netwib_er(t3a("c:.\\file", "c:", "file", "c:", "file"));
  netwib_er(t3a("C:file",    "c:", "file", "c:", "file"));
  netwib_er(t3a("c:dir/",     "c:", "dir", "c:", "dir"));
  netwib_er(t3a("c:dir\\",    "c:", "dir", "c:", "dir"));
  netwib_er(t3a("c:./dir/",   "c:", "dir", "c:", "dir"));
  netwib_er(t3a("c:.\\dir\\", "c:", "dir", "c:", "dir"));
  netwib_er(t3a("c:dir/file",     "c:", "dir/file", "c:dir", "file"));
  netwib_er(t3a("c:dir\\file",    "c:", "dir/file", "c:dir", "file"));
  netwib_er(t3a("c:./dir/file",   "c:", "dir/file", "c:dir", "file"));
  netwib_er(t3a("c:.\\dir\\file", "c:", "dir/file", "c:dir", "file"));
  netwib_er(t3a("c:/file",  "c:/", "/file", "c:/", "file"));
  netwib_er(t3a("c:\\file", "c:/", "/file", "c:/", "file"));
  netwib_er(t3a("c:/dir",  "c:/", "/dir", "c:/", "dir"));
  netwib_er(t3a("c:\\dir", "c:/", "/dir", "c:/", "dir"));
  netwib_er(t3a("c:/dir/file",   "c:/", "/dir/file", "c:/dir", "file"));
  netwib_er(t3a("c:\\dir\\file", "c:/", "/dir/file", "c:/dir", "file"));
  netwib_er(t3a("c:../file",  "c:", "../file", "c:..", "file"));
  netwib_er(t3a("c:..\\file", "c:", "../file", "c:..", "file"));
  netwib_er(t3a("c:../dir/",   "c:", "../dir", "c:..", "dir"));
  netwib_er(t3a("c:..\\dir\\", "c:", "../dir", "c:..", "dir"));
  netwib_er(t3a("c:../dir/file",   "c:", "../dir/file", "c:../dir", "file"));
  netwib_er(t3a("c:..\\dir\\file", "c:", "../dir/file", "c:../dir", "file"));
  netwib_er(t3a("c:/",  "c:/", "/", "PATHROOTDOTDOT", "/"));
  netwib_er(t3a("c:\\", "c:/", "/", "PATHROOTDOTDOT", "/"));
  netwib_er(t3a("c:.", "c:", ".", "c:..", "."));
  netwib_er(t3a("c:..", "c:", "..", "c:../..", ".."));
  netwib_er(t3a("//h/sh/file",     "//h/sh/", "/file", "//h/sh/", "file"));
  netwib_er(t3a("\\\\h\\sh\\file", "//h/sh/", "/file", "//h/sh/", "file"));
  netwib_er(t3a("//H/SH/file",     "//h/sh/", "/file", "//h/sh/", "file"));
  netwib_er(t3a("//h/sh/dir/",      "//h/sh/", "/dir", "//h/sh/", "dir"));
  netwib_er(t3a("\\\\h\\sh\\dir\\", "//h/sh/", "/dir", "//h/sh/", "dir"));
  netwib_er(t3a("//h/sh/dir/file",      "//h/sh/", "/dir/file", "//h/sh/dir",
                "file"));
  netwib_er(t3a("\\\\h\\sh\\dir\\file", "//h/sh/", "/dir/file", "//h/sh/dir",
                "file"));
  netwib_er(t3a("//h/sh/",     "//h/sh/", "/", "PATHROOTDOTDOT", "/"));
  netwib_er(t3a("\\\\h\\sh\\", "//h/sh/", "/", "PATHROOTDOTDOT", "/"));

  /* other tests */
  netwib_er(t3b("t.txt", "txt"));
  netwib_er(t3b("t.", ""));
  netwib_er(t3b(".", ""));
  netwib_er(t3b("t", ""));
  netwib_er(t3b("t.u.txt", "txt"));
  netwib_er(t3b("/t.txt", "txt"));
  netwib_er(t3b("/t.", ""));
  netwib_er(t3b("/.", ""));
  netwib_er(t3b("/t", ""));
  netwib_er(t3b("/t.u.txt", "txt"));
  netwib_er(t3b("/", ""));
  netwib_er(t3b("\\t.txt", "txt"));
  netwib_er(t3b("\\t.", ""));
  netwib_er(t3b("\\.", ""));
  netwib_er(t3b("\\t", ""));
  netwib_er(t3b("\\t.u.txt", "txt"));
  netwib_er(t3b("\\", ""));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t4(netwib_path_inittype inittype,
                     netwib_conststring d1,
                     netwib_conststring p2,
                     netwib_conststring w)
{
  netwib_buf buf1, buf2, bufo;
  netwib_err ret;

  netwib_er(netwib_fmt_display("%s %s\n", d1, p2));

  netwib_er(netwib_buf_init_ext_string(d1, &buf1));
  netwib_er(netwib_buf_init_ext_string(p2, &buf2));
  netwib_er(netwib_buf_init_mallocdefault(&bufo));

  ret = NETWIB_ERR_LOINTERNALERROR;
  switch (inittype) {
  case NETWIB_PATH_INITTYPE_CONCAT :
    ret = netwib_path_init_concat(&buf1, &buf2, &bufo);
    break;
  case NETWIB_PATH_INITTYPE_JAIL :
    ret = netwib_path_init_jail(&buf1, &buf2, &bufo);
    break;
  case NETWIB_PATH_INITTYPE_ABS :
    ret = netwib_path_init_abs(&buf1, &buf2, &bufo);
    break;
  case NETWIB_PATH_INITTYPE_RELA :
    ret = netwib_path_init_rela(&buf1, &buf2, &bufo);
    break;
  case NETWIB_PATH_INITTYPE_RELB :
    ret = netwib_path_init_relb(&buf1, &buf2, &bufo);
    break;
  default :
    return(NETWIBTEST_ERR_1);
  }

  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwibtest_buf_content2(&bufo, w));
  } else if (ret == NETWIB_ERR_PAPATHCANTINIT) {
    netwib__buf_reinit(&bufo);
    netwib_er(netwib_buf_append_string("PAPATHCANTINIT", &bufo));
    netwib_er(netwibtest_buf_content2(&bufo, w));
  } else {
    return(ret);
  }

  netwib_er(netwib_buf_close(&bufo));

  return(NETWIB_ERR_OK);
}
#define t4_concat(d1,p2,w) t4(NETWIB_PATH_INITTYPE_CONCAT,d1,p2,w)
#define t4_jail(d1,p2,w) t4(NETWIB_PATH_INITTYPE_JAIL,d1,p2,w)
#define t4_abs(d1,p2,w) t4(NETWIB_PATH_INITTYPE_ABS,d1,p2,w)
#define t4_rela(d1,p2,w) t4(NETWIB_PATH_INITTYPE_RELA,d1,p2,w)
#define t4_relb(d1,p2,w) t4(NETWIB_PATH_INITTYPE_RELB,d1,p2,w)
static netwib_err netwib_path_init_test(void)
{
  puts("++++----\nTest of netwib_path_init");

  netwib_er(t4_concat("d1", "d2/f2", "d1/d2/f2"));
  netwib_er(t4_concat("d1", "../d2/f2", "d2/f2"));
  netwib_er(t4_concat("d1", "/d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_concat("../d1", "d2/f2", "../d1/d2/f2"));
  netwib_er(t4_concat("../d1", "../d2/f2", "../d2/f2"));
  netwib_er(t4_concat("../d1", "/d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_concat("/d1", "d2/f2", "/d1/d2/f2"));
  netwib_er(t4_concat("/d1", "../d2/f2", "/d2/f2"));
  netwib_er(t4_concat("/d1", "/d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_concat("/", "/d2/f2", "/d2/f2"));
  /**/
  netwib_er(t4_concat("/d1", "c:d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_concat("/d1", "c:/d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_concat("/d1", "c:../d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_concat("/d1", "\\\\s\\s\\d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_concat("c:/", "\\\\s\\s\\d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_concat("c:", "d2/f2", "c:d2/f2"));
  netwib_er(t4_concat("c:", "c:d2/f2", "c:d2/f2"));
  netwib_er(t4_concat("c:", "D:d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_concat("/", "D:d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_concat("c:/", "d2/f2", "c:/d2/f2"));
  netwib_er(t4_concat("c:", "/d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_concat("c:/", "/d2/f2", "c:/d2/f2"));
  netwib_er(t4_concat("c:/", "C:/d2/f2", "c:/d2/f2"));
  netwib_er(t4_concat("\\\\s\\s\\", "/d2/f2", "//s/s/d2/f2"));
  netwib_er(t4_concat("\\\\s\\s\\", "d2/f2", "//s/s/d2/f2"));

  netwib_er(t4_jail("d1", "d2/f2", "d1/d2/f2"));
  netwib_er(t4_jail("d1", "../d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_jail("d1", "/d2/f2", "d1/d2/f2"));
  netwib_er(t4_jail("../d1", "d2/f2", "../d1/d2/f2"));
  netwib_er(t4_jail("../d1", "../d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_jail("../d1", "/d2/f2", "../d1/d2/f2"));
  netwib_er(t4_jail("/d1", "d2/f2", "/d1/d2/f2"));
  netwib_er(t4_jail("/d1", "../d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_jail("/d1", "/d2/f2", "/d1/d2/f2"));
  netwib_er(t4_jail("/", "/d2/f2", "/d2/f2"));
  /**/
  netwib_er(t4_jail("c:", "d2/f2", "c:d2/f2"));
  netwib_er(t4_jail("c:/", "d2/f2", "c:/d2/f2"));
  netwib_er(t4_jail("//s/s/", "d2/f2", "//s/s/d2/f2"));
  netwib_er(t4_jail("c:", "c:d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_jail("//s/s/", "//s/s/", "PAPATHCANTINIT"));
  netwib_er(t4_jail("/d1", "/../d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_jail("/d1", "/b/../../d2/f2", "PAPATHCANTINIT"));

  netwib_er(t4_abs("d1", "d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_abs("d1", "../d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_abs("d1", "/d2/f2", "/d2/f2"));
  netwib_er(t4_abs("../d1", "d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_abs("../d1", "../d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_abs("../d1", "/d2/f2", "/d2/f2"));
  netwib_er(t4_abs("/d1", "d2/f2", "/d1/d2/f2"));
  netwib_er(t4_abs("/d1", "../d2/f2", "/d2/f2"));
  netwib_er(t4_abs("/d1", "/d2/f2", "/d2/f2"));
  netwib_er(t4_abs("/", "/d2/f2", "/d2/f2"));
  /**/
  netwib_er(t4_abs("/d1", "c:d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_abs("/d1", "c:/d2/f2", "c:/d2/f2"));
  netwib_er(t4_abs("/d1", "c:../d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_abs("./d1", "c:d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_abs("./d1", "c:/d2/f2", "c:/d2/f2"));
  netwib_er(t4_abs("./d1", "c:../d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_abs("\\\\s\\s\\", "d2/f2", "//s/s/d2/f2"));
  netwib_er(t4_abs("c:/", "d2/f2", "c:/d2/f2"));
  netwib_er(t4_abs("\\\\s\\s\\", "/d2/f2", "/d2/f2"));

  netwib_er(t4_rela("./d1", "d2/f2", "d2/f2"));
  netwib_er(t4_rela("./d1", "../d2/f2", "../d2/f2"));
  netwib_er(t4_rela("./d1", "/d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_rela("../d1", "d2/f2", "d2/f2"));
  netwib_er(t4_rela("../d1", "../d2/f2", "../d2/f2"));
  netwib_er(t4_rela("../d1", "/d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_rela("/d1", "d2/f2", "d2/f2"));
  netwib_er(t4_rela("/d1", "../d2/f2", "../d2/f2"));
  netwib_er(t4_rela("/d1", "/d2/f2", "../d2/f2"));
  netwib_er(t4_rela("/", "/d2/f2", "d2/f2"));
  netwib_er(t4_rela("d1", "d1", "d1"));
  netwib_er(t4_rela("/d1", "/d1", "."));
  /**/
  netwib_er(t4_rela("/d1", "c:d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_rela("/d1", "c:/d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_rela("/d1", "c:../d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_rela("./d1", "c:d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_rela("./d1", "c:/d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_rela("./d1", "c:../d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_rela("./d1", "\\\\s\\s\\d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_rela("/d1", "\\\\s\\s\\d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_rela("../d1", "\\\\s\\s\\d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_rela("c:", "/d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_rela("c:/d1", "c:/d2/f2", "../d2/f2"));
  netwib_er(t4_rela("c:/d1", "d:/d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_rela("\\\\s\\sh\\d1", "\\\\s\\sh\\d2/f2", "../d2/f2"));
  netwib_er(t4_rela("\\\\s\\sh\\d1", "\\\\s\\sh1\\d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_rela("/d1", "/d1/f2", "f2"));
  netwib_er(t4_rela("/d", "/d1/f2", "../d1/f2"));
  netwib_er(t4_rela("/d11", "/d1/f2", "../d1/f2"));
  netwib_er(t4_rela("/d1/f", "/d1/f2", "../f2"));
  netwib_er(t4_rela("/d1/f2", "/d1/f2", "."));
  netwib_er(t4_rela("/d1/f22", "/d1/f2", "../f2"));
  netwib_er(t4_rela("/d1/f2/f", "/d1/f2", ".."));
  netwib_er(t4_rela("/d1/f2/f", "/", "../../.."));
  netwib_er(t4_rela("/d1/f2/f", "/d", "../../../d"));
  netwib_er(t4_rela("/d1/f2/f", "/d11", "../../../d11"));
  netwib_er(t4_rela("/d1/f2/f", "/d1", "../.."));
  netwib_er(t4_rela("/", "/", "."));

  netwib_er(t4_relb("d1", "d2/f2", "../d2/f2"));
  netwib_er(t4_relb("d1", "../d2/f2", "../../d2/f2"));
  netwib_er(t4_relb("d1", "/d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_relb("../d1", "./d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_relb("../d1", "../d1/f2", "f2"));
  netwib_er(t4_relb("../d1", "/d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_relb("/d1", "./d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_relb("/d1", "../d2/f2", "PAPATHCANTINIT"));
  netwib_er(t4_relb("/d1", "/d2/f2", "../d2/f2"));
  netwib_er(t4_relb("d1", "d1", "."));
  netwib_er(t4_relb("/d1", "/d1", "."));
  /**/
  netwib_er(t4_relb("/d1", "/", ".."));
  netwib_er(t4_relb("/d1", "/d2", "../d2"));
  netwib_er(t4_relb("/d1", "/d1/d2", "d2"));
  netwib_er(t4_relb("/d1/d2", "/d3", "../../d3"));
  netwib_er(t4_relb("/d1/d2", "/d1/d3", "../d3"));
  netwib_er(t4_relb("/", "/", "."));
  netwib_er(t4_relb("d1", "d1", "."));
  netwib_er(t4_relb("/", "/d1", "d1"));
  netwib_er(t4_relb("z", "z/f", "f"));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_sys_path(void)
{
  puts("###########################################\nTest of sys/path");
  netwib_er(netwib_path_canon_test());
  netwib_er(netwib_pathtype_init_test());
  netwib_er(netwib_path_decode_test());
  netwib_er(netwib_path_init_test());

  return(NETWIB_ERR_OK);
}

