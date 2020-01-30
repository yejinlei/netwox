
This file describes how to run tests, or how to compile with
debugging activated.

--------------------------------- netwib ----------------------------------

* Unix :
To run test, do :
 - optionally edit ../src/netwib-src/src/config.dat [useless in most cases]
 - cd netwib
 - ./genemaketest
 - make
 - ./netwibtest 0 0 Ka  (quick test)
 - ./netwibtest 0       (full test)

* Windows :
To run test, do :
 - go up of one directory ("cd ..")
 - rename "src/netwib-src/src/netwib/def_windows.h" to
   "src/netwib-src/src/netwib/def.h" [as if you were compiling netwib]
 - with Visual C++ :
    - open test/netwib/compil/windows/netwibtest/netwibtest.dsw
    - press F7
 - with Dev-C++ :
    - open test/netwib/compil/devcpp/netwibtest.dev
    - press F9
 - open a MSDOS console and run :
     cd test/netwib
     netwibtest 0 0 Ka  (quick test)
     netwibtest 0       (full test)

**
If everything works, you can activate more debugging by editing
src/netwib-src/src/netwib/inc/debuga.h and replacing :
 #define NETWIB_DEBUG_ERROR_PID               0
 #define NETWIB_DEBUG_ERROR_BT                0
 #define NETWIB_DEBUG_TYPE_NOTSET             0
 #define NETWIB_DEBUG_OBSOLETE_REJECT         0
 #define NETWIB_DEBUG_MEM_CORRUPT             0
 #define NETWIB_DEBUG_LEAK                    0
 #define NETWIB_DEBUG_FD_OPEN                 0
 #define NETWIB_DEBUG_TASKTERM_WARN           0
 #define NETWIB_DEBUG_LIST_CHECK              0
 #define NETWIB_DEBUG_HASH_CHECK              0
with :
 #define NETWIB_DEBUG_ERROR_PID               1
 #define NETWIB_DEBUG_ERROR_BT                1
 #define NETWIB_DEBUG_TYPE_NOTSET             1
 #define NETWIB_DEBUG_OBSOLETE_REJECT         1
 #define NETWIB_DEBUG_MEM_CORRUPT             2
 #define NETWIB_DEBUG_LEAK                    1
 #define NETWIB_DEBUG_FD_OPEN                 1
 #define NETWIB_DEBUG_TASKTERM_WARN           1
 #define NETWIB_DEBUG_LIST_CHECK              1
 #define NETWIB_DEBUG_HASH_CHECK              1

--------------------------------- netwox ----------------------------------

* Unix :
To run test, do :
 - optionally edit ../src/netwib-src/src/config.dat [useless in most cases]
 - cd netwox
 - ./genemaketest
 - make
 - ./netwox

* Windows :
To run test, do :
 - go up of one directory ("cd ..")
 - rename "src/netwib-src/src/netwib/def_windows.h" to
   "src/netwib-src/src/netwib/def.h" [as if you were compiling netwib]
 - rename "src/netwox-src/src/def_windows.h" to
   "src/netwox-src/src/def.h"
 - rename "src/netwox-src/src/netwib_windows.h" to
   "src/netwox-src/src/netwib.h"
 - with Visual C++ :
    - open test/netwox/compil/windows/netwoxtest/netwoxtest.dsw
    - press F7
    - select netwox, click on right button, select Set Active Project
    - press F7
 - with Dev-C++ :
    - open test/netwox/compil/devcpp/netwoxtest.dev
    - press F9
 - open a MSDOS console and run :
     cd test/netwox
     netwox
 - to run it from Visual, add ../../../../../../test/netwox in
   netwox539 - Project - Setting - Debug - General - Working
   Directory.

--------------------------------------------------------------------------

If you :
 - encounter a problem
 - have a new idea
 - have different ideas
 - etc.
contact me.

Thanks.
Laurent CONSTANTIN
laurentconstantin@free.fr

