
/*-------------------------------------------------------------*/
/***************************************************************
 * Under Windows, '/' and '\' are recognized as directory      *
 * separator. However, under Unix, only '/' is valid.          *
 * So, if you want your code to be portable, use '/'.          *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* Name : netwib_path_canon
   Description :
     Clean a filename ("//"->"/", "/./"->"/", "/aa/../"->"/", etc.).
   Input parameter(s) :
     pathname : filename (ex : "/tmp/dir/..//./file")
   Input/output parameter(s) :
   Output parameter(s) :
     *pcanonizedpathname : cleaned filename (ex : "/tmp/file")
   Normal return values :
     NETWIB_ERR_OK : ok
   Examples of canonical paths :
     file          dir          dir/file          dir/dir
     /file         /dir         /dir/file         /dir/dir
     ../file       ../dir       ../dir/file       ../dir/dir
     /             .            ..
    Windows using a drive letter :
     c:file        c:dir        c:dir/file        c:dir/dir
     c:/file       c:/dir       c:/dir/file       c:/dir/dir
     c:../file     c:../dir     c:../dir/file     c:../dir/dir
     c:/           c:           c:..
    Windows using a smb or cifs share :
     //server/share/
     //s/sh/file   //s/sh/dir   //s/sh/dir/file   //s/sh/dir/dir
    Note : a Window share is very similar to an absolute path
           with two leading '/'. To ensure they are not mixed,
           you have to ensure an absolute path does not start
           with '//'.
*/
netwib_err netwib_path_canon(netwib_constbuf *ppathname,
                             netwib_buf *pcanonizedpathname);

/*-------------------------------------------------------------*/
/***************************************************************
 * netwib functions :                                          *
 *  - always work with :                                       *
 *     + canonized path                                        *
 *  - generally work with :                                    *
 *     + relative path with leading "./"                       *
 *     + "c:." instead of "c:" (only under Windows)            *
 *     + path with "\" instead of "/" (only under Windows)     *
 *  - might work with :                                        *
 *     + directory name ending with "/" (not under Windows)    *
 *     + path such as "/dir/../dir2/file"                      *
 *                                                             *
 * So, user should canonize paths before using netwib          *
 * functions. However, functions in this module accept         *
 * un-canonized paths and produce canonized paths. So,         *
 * functions in this module are safe for use.                  *
 ***************************************************************/

/*-------------------------------------------------------------*/
/***************************************************************
 * Empty string ("") is not a valid path. If functions of this *
 * module have an empty parameter, error                       *
 * NETWIB_ERR_PAPATHNOTCANON occurs.                           *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* Name : netwib_pathtype_init
   Description :
     Obtain type of a path.
   Input parameter(s) :
     *ppathname : pathname to obtain info for (do not need to be canonized).
   Input/output parameter(s) :
   Output parameter(s) :
     *ppathtype : type as a bitfield
   Normal return values :
     NETWIB_ERR_OK : ok
   Examples :
     pathname     absolute  root   unix  windrive  winshare
      file         0=no       0      1      0         0
      /file        1=yes      0      1      0         0
      ../file        0        0      1      0         0
      /              1        1      1      0         0
      .              0        0      1      0         0
      ..             0        0      1      0         0
      c:file         0        0      0      1         0
      c:/file        1        0      0      1         0
      c:../file      0        0      0      1         0
      c:/            1        1      0      1         0
      c:             0        0      0      1         0
      c:..           0        0      0      1         0
      //s/sh/file    1        0      0      0         1
      //s/sh/        1        1      0      0         1
*/
#define NETWIB_PATHTYPE_ABSOLUTE 0x01 /* absolute: path is fully specified */
#define NETWIB_PATHTYPE_ROOT     0x02 /* root: can't go up */
#define NETWIB_PATHTYPE_UNIX     0x04 /* unix (or windows simple path) */
#define NETWIB_PATHTYPE_WINDRIVE 0x08 /* windows drive: c:, d: */
#define NETWIB_PATHTYPE_WINSHARE 0x10 /* windows smb share: //server/share/ */
typedef netwib_uint32 netwib_pathtype;
netwib_err netwib_pathtype_init(netwib_constbuf *ppathname,
                                netwib_pathtype *ppathtype);

/*-------------------------------------------------------------*/
/* Name : netwib_path_decode_xyz
   Description :
     Separate a path.
   Input parameter(s) :
     ppathname : filename (ex : "/tmp/file") (do not need to be canonized)
   Input/output parameter(s) :
     type : type of information to extract
   Output parameter(s) :
     *pout : output buffer (canonized, even if ppathname is not)
   Normal return values :
     NETWIB_ERR_OK : ok
   Examples :
     pathname     begin   core      parent   child
      file        .       file      .        file
      d/f         .       d/f       d        f
      /file       /       /file     /        file
      /d/f        /       /d/f      /d       f
      ../file     .       ../file   ..       file
      ../d/f      .       ../d/f    ../d     f
      /           /       /         Error1   /
      .           .       .         ..       .
      ..          .       ..        ../..    ..
      c:file      c:      file      c:       file
      c:d/f       c:      d/f       c:d      f
      c:/file     c:/     /file     c:/      file
      c:/d/f      c:/     /d/f      c:/d     f
      c:../file   c:      ../file   c:..     file
      c:../d/f    c:      ../d/f    c:../d   f
      c:/         c:/     /         Error1   /
      c:          c:      .         c:..     .
      c:..        c:      ..        c:../..  ..
      //s/t/file  //s/t/  /file     //s/t/   file
      //s/t/d/f   //s/t/  /d/f      //s/t/d  f
      //s/t/      //s/t/  /         Error1   /
   Errors are :
     Error1 : NETWIB_ERR_PAPATHROOTDOTDOT
*/
typedef enum {
  NETWIB_PATH_DECODETYPE_BEGIN = 1, /* root directory or . */
  NETWIB_PATH_DECODETYPE_CORE,      /* path starting at begin */
  NETWIB_PATH_DECODETYPE_PARENT,    /* parent directory */
  NETWIB_PATH_DECODETYPE_CHILD,     /* last item */
  NETWIB_PATH_DECODETYPE_EXTENSION  /* file extension without the dot
                                       (empty if no extension) ; pout is
                                       a netwib_bufext */
  /* note : pathname == begin+core or parent+child */
} netwib_path_decodetype;
netwib_err netwib_path_decode(netwib_constbuf *ppathname,
                              netwib_path_decodetype type,
                              netwib_buf *pout);
#define netwib_path_decode_begin(pathname,pout) netwib_path_decode(pathname,NETWIB_PATH_DECODETYPE_BEGIN,pout)
#define netwib_path_decode_core(pathname,pout) netwib_path_decode(pathname,NETWIB_PATH_DECODETYPE_CORE,pout)
#define netwib_path_decode_parent(pathname,pout) netwib_path_decode(pathname,NETWIB_PATH_DECODETYPE_PARENT,pout)
#define netwib_path_decode_child(pathname,pout) netwib_path_decode(pathname,NETWIB_PATH_DECODETYPE_CHILD,pout)
#define netwib_path_decode_extension(pathname,pout) netwib_path_decode(pathname,NETWIB_PATH_DECODETYPE_EXTENSION,pout)

/*-------------------------------------------------------------*/
/* Name : netwib_path_init_xyz
   Description :
     Initialize a path.
   Input parameter(s) :
     See below (do not need to be canonized)
   Input/output parameter(s) :
     pout : path initialized (canonized, even if input is not)
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
typedef enum {
  NETWIB_PATH_INITTYPE_CONCAT = 1,
  NETWIB_PATH_INITTYPE_JAIL,
  NETWIB_PATH_INITTYPE_ABS,
  NETWIB_PATH_INITTYPE_RELA,
  NETWIB_PATH_INITTYPE_RELB,
} netwib_path_inittype;
/*
   Definitions :
     concat : path from the beginning of dirname1 to pathname2
                 [dirname1]-[pathname2]
                 X-------------------->
              If pathname2 is absolute, an error occurs (except if
              dirname1 is a root).
     jail : same as concat except pathname2 is considered as an
            absolute path inside dirname1. It's a kind of chroot
            or jail with a rootdir of dirname1. Resulting filename
            cannot escape from dirname1.
     abs : absolute path to pathname2 (pathname2 is in a
           file located in dirname1 directory)
              root
                [dirname1]-[pathname2]
              X---------------------->
           If pathname2 is absolute, result is pathname2.
           If dirname1 is not absolute, an error occurs.
     rela : relative path to go to pathname2 from a file
            in dirname1 directory (pathname2 is in a
            file located in dirname1 directory)
               [dirname1]-[pathname2]
                        X----------->
            If pathname2 is relative, result is pathname2.
            If pathname2 is absolute, dirname1 must be absolute.
     relb : relative path to go to pathname2 from a file
            in dirname1 directory (pathname1 and pathname2
            are located in the same directory)
               [dirname1]
               [  pathname2   ]
                        X----->
            If pathname2 is absolute, dirname1 must be absolute.
            If pathname2 is relative, dirname1 must be relative.
   Examples of concat, jail and abs :
     dirname1   pathname2  concat       jail         abs
     d1         d2/f2      d1/d2/f2     d1/d2/f2     Error
     d1         ../d2/f2   d2/f2        Error        Error
     d1         /d2/f2     Error        d1/d2/f2     /d2/f2
     ../d1      d2/f2      ../d1/d2/f2  ../d1/d2/f2  Error
     ../d1      ../d2/f2   ../d2/f2     Error        Error
     ../d1      /d2/f2     Error        ../d1/d2/f2  /d2/f2
     /d1        d2/f2      /d1/d2/f2    /d1/d2/f2    /d1/d2/f2
     /d1        ../d2/f2   /d2/f2       Error        /d2/f2
     /d1        /d2/f2     Error        /d1/d2/f2    /d2/f2
     isroot     /d2/f2     x/d2/f2      x/d2/f2      /d2/f2
   Examples of rela and relb :
     dirname1   pathname2  rela         relb
     d1         d2/f2      d2/f2        ../d2/f2
     d1         ../d2/f2   ../d2/f2     ../../d2/f2
     d1         /d2/f2     Error        Error
     ../d1      d2/f2      d2/f2        Error
     ../d1      ../d2/f2   ../d2/f2     ../d2/f2
     ../d1      /d2/f2     Error        Error
     /d1        d2/f2      d2/f2        Error
     /d1        ../d2/f2   ../d2/f2     Error
     /d1        /d2/f2     ../d2/f2     ../d2/f2
     d1         d1         d1           . (because pathname2 is like ...)
     /d1        /d1        .            . (dirname1, so it's a directory)
   The errors have the code : NETWIB_ERR_PAPATHCANTINIT.
*/
netwib_err netwib_path_init(netwib_constbuf *pdirname1,
                            netwib_constbuf *ppathname2,
                            netwib_path_inittype type,
                            netwib_buf *pout);
#define netwib_path_init_concat(dirname1,pathname2,pout) netwib_path_init(dirname1,pathname2,NETWIB_PATH_INITTYPE_CONCAT,pout)
#define netwib_path_init_jail(dirname1,pathname2,pout) netwib_path_init(dirname1,pathname2,NETWIB_PATH_INITTYPE_JAIL,pout)
#define netwib_path_init_abs(dirname1,pathname2,pout) netwib_path_init(dirname1,pathname2,NETWIB_PATH_INITTYPE_ABS,pout)
#define netwib_path_init_rela(dirname1,pathname2,pout) netwib_path_init(dirname1,pathname2,NETWIB_PATH_INITTYPE_RELA,pout)
#define netwib_path_init_relb(dirname1,pathname2,pout) netwib_path_init(dirname1,pathname2,NETWIB_PATH_INITTYPE_RELB,pout)
