
#ifndef NETWIB_INCLUDED
#define NETWIB_INCLUDED 1
#ifdef __cplusplus
extern "C" {
#endif

#ifndef NETWIB_DEF_INCLUDED
#define NETWIB_DEF_INCLUDED 1
 /* major defines */
 #include <netwib/def.h>
#endif

#ifdef NETWIB_DEF_ONLY
  /* in this case, we only included netwib/def.h, so we must be
     able to include netwib.h again */
 #undef NETWIB_INCLUDED
 #undef NETWIB_DEF_ONLY
#else
 /* error codes */
 #include <netwib/err.h>
 /* data section */
 #include <netwib/dat.h>
 /* system section */
 #include <netwib/sys.h>
 /* network section */
 #include <netwib/net.h>
 /* packet section */
 #include <netwib/pkt.h>
 /* packet showing section */
 #include <netwib/shw.h>
#endif

#ifdef __cplusplus
}
#endif
#endif /*ifndef NETWIB_INCLUDED*/

