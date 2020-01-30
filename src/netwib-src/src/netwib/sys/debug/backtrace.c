/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
/* obtain list of supported methods on this system */

/* ARCH_i386 */
#define NETWIB_DEBUG_BACKTRACE_METHOD_ARCH_i386 0
#if defined NETWIBDEF_SYSARCH_i386
 #if NETWIBDEF_SYSARCH_i386==1
  #undef NETWIB_DEBUG_BACKTRACE_METHOD_ARCH_i386
  #define NETWIB_DEBUG_BACKTRACE_METHOD_ARCH_i386 1
 #endif
#endif
/* ARCH_sparc */
#define NETWIB_DEBUG_BACKTRACE_METHOD_ARCH_sparc 0
#if defined NETWIBDEF_SYSARCH_sparc
 #if NETWIBDEF_SYSARCH_sparc==1
  #undef NETWIB_DEBUG_BACKTRACE_METHOD_ARCH_sparc
  #define NETWIB_DEBUG_BACKTRACE_METHOD_ARCH_sparc 1
 #endif
#endif

/* I prefer to reject gcc which lead to several problems
  (instead of helping) : you might activate it if you know
  what you do */
/* COMPIL_gcc */
#define NETWIB_DEBUG_BACKTRACE_METHOD_COMPIL_gcc 0
#if defined __GNUC__ || defined __GCC__
 #undef NETWIB_DEBUG_BACKTRACE_METHOD_COMPIL_gcc
 #define NETWIB_DEBUG_BACKTRACE_METHOD_COMPIL_gcc 0
#endif

/* In lcrzo 4, I also used gdb and pstack, but they also lead
   to several problems each time they were used on a new system.
   So, I decided to remove this feature. */

/*-------------------------------------------------------------*/
/* empty backtrace */
static netwib_err netwib_debug_backtrace_array_unsup(netwib_uint32 btmaxsize,
                                                     void **bt,
                                                     netwib_uint32 *pbtsize)
{
  if (pbtsize != NULL) {
    *pbtsize = 0;
  }

  bt = bt; /* for compiler warning */
  btmaxsize = btmaxsize; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_BACKTRACE_METHOD_ARCH_i386==1
/* backtrace using i386 processor */
static netwib_err netwib_debug_backtrace_array_arch_i386_fake(void)
{
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_debug_backtrace_array_arch_i386(netwib_uint32 btmaxsize,
                                                         void **bt,
                                                         netwib_uint32 *pbtsize)
{
  void *ebpp, *espp, *storedframemin, *ra, *nextframe;
  void **storedframe;
  netwib_uint32 i;

#if defined NETWIBDEF_SYSNAME_Unix
  ebpp = NULL;                  /* just for compiler warning */
  espp = NULL;                  /* just for compiler warning */
  __asm__("mov %%ebp, %0": "=r"(ebpp):);
  __asm__("mov %%esp, %0": "=r"(espp):);
#elif defined NETWIBDEF_SYSNAME_Windows
 #ifdef NETWIBDEF_PROGCC_MINGW32
  __asm__("mov %%ebp, %0": "=r"(ebpp):);
  __asm__("mov %%esp, %0": "=r"(espp):);
 #else
  __asm {mov ebpp, ebp}
  __asm {mov espp, esp}
 #endif
#else
#error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  /* we have to call a fake function, otherwise optimization (-O2) of
     gcc 3.0.4 does some optimizations which cause "ebpp <= espp" */
  netwib_er(netwib_debug_backtrace_array_arch_i386_fake());

  /* this defines detects bad pointers */
#define netwib_debug_backtrace_array_badptr(p) (p==NULL||((netwib_uintptr)p&0x3))

  /* check if esp is valid (esp is only needed to check ebp) */
  if (netwib_debug_backtrace_array_badptr(espp)) {
    if (pbtsize != NULL)
      *pbtsize = 0;
    return(NETWIB_ERR_OK);
  }
  /* check if ebp is valid */
  if (netwib_debug_backtrace_array_badptr(ebpp)) {
    if (pbtsize != NULL)
      *pbtsize = 0;
    return(NETWIB_ERR_OK);
  }
  if (ebpp <= espp) {
    if (pbtsize != NULL)
      *pbtsize = 0;
    return(NETWIB_ERR_OK);
  }

  /* retrieve stack info */
  storedframe = (void **)ebpp;
  storedframemin = storedframe;
  for (i = 0; i < btmaxsize; i++) {
    /* get the return address and next frame */
    ra = storedframe[1];
    nextframe = storedframe[0];
    /* check ra */
    if (ra == NULL) {
      /* this is the normal ending condition for programs created with gcc */
      break;
    }
    /* do not do netwib_debug_backtrace_array_badptr(ra) because ra does not
       have to be aligned */
    /* check nextframe */
    if (nextframe == NULL) {
      /* this is the normal ending condition for programs created with gcc */
      break;
    }
    if (netwib_debug_backtrace_array_badptr(nextframe)) {
      break;
    }
    if (nextframe <= storedframemin) {
      break;
    }
    if (((netwib_uintptr) nextframe - (netwib_uintptr) storedframemin) >
        0x00100000u) {
      /* this is the normal ending condition for programs created with
         Visual C++ */
      break;
    }
    /* set info, and skip the frame */
    bt[i] = ra;
    storedframemin = storedframe;
    storedframe = (void **)(nextframe);
  }
  if (pbtsize != NULL)
    *pbtsize = i;

  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_BACKTRACE_METHOD_ARCH_sparc==1
/* backtrace using Sparc processor */
static netwib_err netwib_debug_backtrace_array_badptr(netwib_ptr ptr,
                                                      netwib_bool allowunaligned,
                                                      netwib_bool *pisvalid)
{
  netwib_bool isvalid;

#if defined NETWIBDEF_SYSNAME_Solaris
  static netwib_uint32 pagemask = (netwib_uint32) - 1;
  netwib_ptr ptr2;
  char c;
#endif

  if (ptr == NULL) {
    if (pisvalid != NULL)
      *pisvalid = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  }

  if (!allowunaligned && ((netwib_uintptr) ptr & 0x3)) {
    if (pisvalid != NULL)
      *pisvalid = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  }
#if defined NETWIBDEF_SYSNAME_Solaris
  if (pagemask == (netwib_uint32) - 1) {
    /* need to set pagemask */
    pagemask = ~(sysconf(_SC_PAGESIZE) - 1);
  }
  ptr2 = (netwib_ptr) ((netwib_uintptr) ptr & pagemask);
  if (mincore(ptr2, 1, &c) == -1 && errno == ENOMEM)
    isvalid = NETWIB_FALSE;
  else
    isvalid = NETWIB_TRUE;
#else
  isvalid = NETWIB_TRUE;
#endif

  if (pisvalid != NULL)
    *pisvalid = isvalid;
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_debug_backtrace_array_arch_sparc(netwib_uint32 btmaxsize,
                                                          void **bt,
                                                          netwib_uint32 *pbtsize)
{
  void *fpp, *spp, *storedframemin, *ra, *nextframe;
  void **storedframe;
  netwib_uint32 i;
  netwib_bool isvalid;

  /* Flush registers (ST_FLUSH_WINDOWS in /usr/include/sys/trap.h).
     With Sparcv9, should we use "flushw" ? */
  __asm__("ta 3");

  fpp = NULL;                   /* just for compiler warning */
  __asm__("mov %%fp, %0": "=r"(fpp):);
  spp = NULL;                   /* just for compiler warning */
  __asm__("mov %%sp, %0": "=r"(spp):);

  /* check if esp is valid (esp is only needed to check ebp) */
  netwib_er(netwib_debug_backtrace_array_badptr(spp, NETWIB_FALSE, &isvalid));
  if (!isvalid) {
    if (pbtsize != NULL)
      *pbtsize = 0;
    return(NETWIB_ERR_OK);
  }
  /* check if ebp is valid */
  netwib_er(netwib_debug_backtrace_array_badptr(fpp, NETWIB_FALSE, &isvalid));
  if (!isvalid) {
    if (pbtsize != NULL)
      *pbtsize = 0;
    return(NETWIB_ERR_OK);
  }
  if (fpp <= spp) {
    if (pbtsize != NULL)
      *pbtsize = 0;
    return(NETWIB_ERR_OK);
  }
  /* retrieve stack info */
  storedframe = (void **)fpp;
  storedframemin = storedframe;
  for (i = 0; i < btmaxsize; i++) {
    /* get the return address and next frame */
    ra = storedframe[15];
    nextframe = storedframe[14];
    /* check ra */
    if (ra == NULL) {
      /* this is the normal ending condition for programs created with gcc */
      break;
    }
    netwib_er(netwib_debug_backtrace_array_badptr(ra, NETWIB_TRUE, &isvalid));
    if (!isvalid) {
      break;
    }
    /* check nextframe */
    if (nextframe == NULL) {
      /* this is the normal ending condition for programs created with gcc */
      break;
    }
    netwib_er(netwib_debug_backtrace_array_badptr(nextframe, NETWIB_FALSE,
                                               &isvalid));
    if (!isvalid) {
      break;
    }
    if (nextframe <= storedframemin) {
      break;
    }
    /* set info, and skip the frame */
    bt[i] = ra;
    storedframemin = storedframe;
    storedframe = (void **)(nextframe);
  }
  if (pbtsize != NULL)
    *pbtsize = i;

  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_BACKTRACE_METHOD_COMPIL_gcc==1
/* backtrace using gcc extension */
static netwib_err netwib_debug_backtrace_array_compil_gcc(netwib_uint32 btmaxsize,
                                                          void **bt,
                                                          netwib_uint32 *pbtsize)
{
  void *p;
  netwib_bool getmoread;
  netwib_uint32 i, j;

#define btgc(i) case i: p = (j == 0) ? (__builtin_frame_address(i)) : (__builtin_return_address(i)); break;

  i = 0;
  getmoread = NETWIB_TRUE;
  for (i = 0; i < btmaxsize && getmoread; i++) {
    for (j = 0; j < 2 && getmoread; j++) {
      switch ((int)i) {
        btgc(0); btgc(1); btgc(2); btgc(3); btgc(4);
        btgc(5); btgc(6); btgc(7); btgc(8); btgc(9);
        btgc(10); btgc(11); btgc(12); btgc(13); btgc(14);
        btgc(15); btgc(16); btgc(17); btgc(18); btgc(19);
        btgc(20); btgc(21); btgc(22); btgc(23); btgc(24);
        btgc(25); btgc(26); btgc(27); btgc(28); btgc(29);
        btgc(30); btgc(31); btgc(32); btgc(33); btgc(34);
        btgc(35); btgc(36); btgc(37); btgc(38); btgc(39);
        btgc(40); btgc(41); btgc(42); btgc(43); btgc(44);
        btgc(45); btgc(46); btgc(47); btgc(48); btgc(49);
        default :
          p = NULL;
      }
      if (p == NULL) {
        getmoread = NETWIB_FALSE;
      } else {
        if (j == 1) {
          bt[i] = p;
        }
      }
    }
  }
  if (pbtsize != NULL)
    *pbtsize = i;
  return(NETWIB_ERR_OK);
}
#endif


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwib_debug_backtrace_array(netwib_uint32 btmaxsize,
                                        void **bt,
                                        netwib_uint32 *pbtsize)
{
  netwib_err ret;

#if NETWIB_DEBUG_BACKTRACE_METHOD_ARCH_i386==1
  ret = netwib_debug_backtrace_array_arch_i386(btmaxsize, bt, pbtsize);
  if (ret == NETWIB_ERR_OK) return(NETWIB_ERR_OK);
#endif

#if NETWIB_DEBUG_BACKTRACE_METHOD_ARCH_sparc==1
  ret = netwib_debug_backtrace_array_arch_sparc(btmaxsize, bt, pbtsize);
  if (ret == NETWIB_ERR_OK) return(NETWIB_ERR_OK);
#endif

#if NETWIB_DEBUG_BACKTRACE_METHOD_COMPIL_gcc==1
  ret = netwib_debug_backtrace_array_compil_gcc(btmaxsize, bt, pbtsize);
  if (ret == NETWIB_ERR_OK) return(NETWIB_ERR_OK);
#endif

  ret = netwib_debug_backtrace_array_unsup(btmaxsize, bt, pbtsize);
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_backtrace_string(netwib_string *pstringout)
{
  void *bt[NETWIB_DEBUG_BACKTRACE_MAXSIZE];
  netwib_uint32 btsize, i;

  netwib_er(netwib_debug_backtrace_array(NETWIB_DEBUG_BACKTRACE_MAXSIZE,
                                         bt, &btsize));
  netwib_er(netwib_debug_string_init(pstringout));
  if (btsize == 0) {
    netwib_er(netwib_debug_string_append_string("empty", pstringout));
  } else {
    for (i = 0; i < btsize - 1; i++) {
      netwib_er(netwib_debug_string_append_fmt(pstringout, "%p,", bt[i]));
    }
    netwib_er(netwib_debug_string_append_fmt(pstringout, "%p", bt[btsize - 1]));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_backtrace_disp(void)
{
  netwib_string str;

  netwib_er(netwib_debug_backtrace_string(&str));
  netwib_er(netwib_debug_string_append_string("\n", &str));
  netwib_er(netwib_debug_disp_string(str));
  netwib_er(netwib_debug_string_close(&str));

  return(NETWIB_ERR_OK);
}
