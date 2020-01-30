
#ifndef NETWIB_INCLUDED
#define NETWIB_INCLUDED 1
#ifdef __cplusplus
extern "C" {
#endif

#ifndef NETWIB_DEF_INCLUDED
#define NETWIB_DEF_INCLUDED 1
 /* major defines */
 #include <netwib539/def.h>
#endif

#ifdef NETWIB_DEF_ONLY
  /* in this case, we only included netwib/def.h, so we must be
     able to include netwib.h again */
 #undef NETWIB_INCLUDED
 #undef NETWIB_DEF_ONLY
#else
 /* error codes */
 #include <netwib539/err.h>
 /* data section */
 #include <netwib539/dat.h>
 /* system section */
 #include <netwib539/sys.h>
 /* network section */
 #include <netwib539/net.h>
 /* packet section */
 #include <netwib539/pkt.h>
 /* packet showing section */
 #include <netwib539/shw.h>
#endif

#ifdef __cplusplus
}
#endif
#endif /*ifndef NETWIB_INCLUDED*/

