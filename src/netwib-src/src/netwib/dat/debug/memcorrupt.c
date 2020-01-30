/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
/* characters in uninit_pattern */
#define NETWIB_DEBUG_MEMCORRUPT_UNINITPATTERN_C0  0x10
#define NETWIB_DEBUG_MEMCORRUPT_UNINITPATTERN_C1  'E'
#define NETWIB_DEBUG_MEMCORRUPT_UNINITPATTERN_C2  0xFF
/* characters in outer_pattern */
#define NETWIB_DEBUG_MEMCORRUPT_OUTERPATTERN_C0   0xA4
#define NETWIB_DEBUG_MEMCORRUPT_OUTERPATTERN_C1   '_'
#define NETWIB_DEBUG_MEMCORRUPT_OUTERPATTERN_C2   0x03
/* characters in free_pattern */
#define NETWIB_DEBUG_MEMCORRUPT_FREEPATTERN_C0    0xD9
#define NETWIB_DEBUG_MEMCORRUPT_FREEPATTERN_C1    'Z'
#define NETWIB_DEBUG_MEMCORRUPT_FREEPATTERN_C2    0x81
/* chunk size for allocation of stored data */
#define NETWIB_DEBUG_MEMCORRUPT_ALLOCCHUNKS       1000
/* size of separator patterns */
#define MEMCORRUPT_SMALLPATTERN_SIZE 8
/* 1/x times to check outer_pattern and free_pattern */
#define NETWIB_DEBUG_MEMCORRUPT_CHECK_FREQUENCY 22
/* to speed up checks (ignore middle : just look at edges) */
#define NETWIB_DEBUG_MEMCORRUPT_SPEEDUP           1

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
#define NETWIB_DEBUG_CAST_FREE
#elif defined NETWIBDEF_SYSNAME_Windows
 /* We should not need it, but system doesn't accept "free((void*)ptr)".
    If you know a better solution, contact me. */
#define NETWIB_DEBUG_CAST_FREE (void*)
#endif

/*-------------------------------------------------------------*/
#define netwib_debug_mem_data_init_uint32(x,pc) { *(pc++)=netwib_c2_uint32_0(x); *(pc++)=netwib_c2_uint32_1(x); *(pc++)=netwib_c2_uint32_2(x); *(pc++)=netwib_c2_uint32_3(x); }
#define netwib_debug_mem_uint32_init_data(pc,x) { x=(*(pc++))<<24; x|=(*(pc++))<<16; x|=(*(pc++))<<8; x|=(*(pc++)); }

/*-------------------------------------------------------------*/
#define NETWIB_DEBUG_MEMCORRUPT_ALIGNPAD(s) ( ( sizeof(void*) - ( s & (sizeof(void*) - 1) ) ) & (sizeof(void*) - 1) )
#define NETWIB_DEBUG_MEMCORRUPT_ALIGN(s) ((s + sizeof(void*) - 1) & ~((netwib_uintptr)(sizeof(void*) - 1)))


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/* Those do not start with "netwib_debug" because it's too long */
typedef enum {
  MEMCORRUPT_PATTERNTYPE_UNINIT = 1,
  MEMCORRUPT_PATTERNTYPE_OUTER,
  MEMCORRUPT_PATTERNTYPE_FREE,
  MEMCORRUPT_PATTERNTYPE_PROTECTED
} memcorrupt_patterntype;
static netwib_err memcorrupt_pattern_init(netwib_data *ppc,
                                          netwib_uint32 patternsize,
                                          memcorrupt_patterntype patterntype)
{
  netwib_byte c0, c1, c2;
  netwib_uint32 i;
  netwib_int32 j;
  netwib_data pc;

#if NETWIB_DEBUG_MEMCORRUPT_SPEEDUP == 1
  if (patternsize > 64) {
    netwib_uint32 skipsize, lastsize;
    skipsize = patternsize - 64;
    skipsize = 3*(skipsize/3);
    netwib_er(memcorrupt_pattern_init(ppc, 30, patterntype));
    *ppc += skipsize;
    lastsize = patternsize - 30 - skipsize;
    netwib_er(memcorrupt_pattern_init(ppc, lastsize, patterntype));
    return(NETWIB_ERR_OK);
  }
#endif

  c0 = c1 = c2 = 0;
  switch (patterntype) {
    case MEMCORRUPT_PATTERNTYPE_UNINIT :
      c0 = NETWIB_DEBUG_MEMCORRUPT_UNINITPATTERN_C0;
      c1 = NETWIB_DEBUG_MEMCORRUPT_UNINITPATTERN_C1;
      c2 = NETWIB_DEBUG_MEMCORRUPT_UNINITPATTERN_C2;
      break;
    case MEMCORRUPT_PATTERNTYPE_OUTER :
      c0 = NETWIB_DEBUG_MEMCORRUPT_OUTERPATTERN_C0;
      c1 = NETWIB_DEBUG_MEMCORRUPT_OUTERPATTERN_C1;
      c2 = NETWIB_DEBUG_MEMCORRUPT_OUTERPATTERN_C2;
      break;
    case MEMCORRUPT_PATTERNTYPE_FREE :
      c0 = NETWIB_DEBUG_MEMCORRUPT_FREEPATTERN_C0;
      c1 = NETWIB_DEBUG_MEMCORRUPT_FREEPATTERN_C1;
      c2 = NETWIB_DEBUG_MEMCORRUPT_FREEPATTERN_C2;
      break;
    case MEMCORRUPT_PATTERNTYPE_PROTECTED :
      c0 = 0xCC;
      c1 = 0xCC;
      c2 = 0xCC;
      break;
  }

  pc = *ppc;
  for (i = 0, j = 0; i < patternsize; i++, j++) {
    if (j == 0)
      *(pc++) = c0;
    else if (j == 1)
      *(pc++) = c1;
    else if (j == 2) {
      *(pc++) = c2;
      j = -1;
    }
  }
  *ppc = pc;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err memcorrupt_pattern_check(netwib_conststring patternname,
                                           netwib_constdata *ppc,
                                           netwib_uint32 patternsize,
                                           memcorrupt_patterntype patterntype)
{
  netwib_byte c0, c1, c2, c;
  netwib_uint32 i;
  netwib_int32 j;
  netwib_constdata pc;

#if NETWIB_DEBUG_MEMCORRUPT_SPEEDUP == 1
  if (patternsize > 64) {
    netwib_uint32 skipsize, lastsize;
    skipsize = patternsize - 64;
    skipsize = 3*(skipsize/3);
    netwib_er(memcorrupt_pattern_check(patternname, ppc, 30, patterntype));
    *ppc += skipsize;
    lastsize = patternsize - 30 - skipsize;
    netwib_er(memcorrupt_pattern_check(patternname, ppc, lastsize,patterntype));
    return(NETWIB_ERR_OK);
  }
#endif

  c0 = c1 = c2 = 0;
  switch (patterntype) {
    case MEMCORRUPT_PATTERNTYPE_UNINIT:
      c0 = NETWIB_DEBUG_MEMCORRUPT_UNINITPATTERN_C0;
      c1 = NETWIB_DEBUG_MEMCORRUPT_UNINITPATTERN_C1;
      c2 = NETWIB_DEBUG_MEMCORRUPT_UNINITPATTERN_C2;
      break;
    case MEMCORRUPT_PATTERNTYPE_OUTER:
      c0 = NETWIB_DEBUG_MEMCORRUPT_OUTERPATTERN_C0;
      c1 = NETWIB_DEBUG_MEMCORRUPT_OUTERPATTERN_C1;
      c2 = NETWIB_DEBUG_MEMCORRUPT_OUTERPATTERN_C2;
      break;
    case MEMCORRUPT_PATTERNTYPE_FREE:
      c0 = NETWIB_DEBUG_MEMCORRUPT_FREEPATTERN_C0;
      c1 = NETWIB_DEBUG_MEMCORRUPT_FREEPATTERN_C1;
      c2 = NETWIB_DEBUG_MEMCORRUPT_FREEPATTERN_C2;
      break;
    case MEMCORRUPT_PATTERNTYPE_PROTECTED :
      netwib_er(netwib_priv_errmsg_func_string("memcorrupt_pattern_check",
                                             "unknown value for patterntype"));
      return(NETWIB_ERR_LOINTERNALERROR);
  }

  c = 0; /* for the compiler warning */
  pc = *ppc;
  for (i = 0, j = 0; i < patternsize; i++, j++) {
    if (j == 0) {
      if (*pc++ != c0) {
        break;
      }
    } else if (j == 1) {
      if (*pc++ != c1) {
        break;
      }
    } else if (j == 2) {
      if (*pc++ != c2) {
        break;
      }
      j = -1;
    }
  }
  if (i != patternsize) {
    pc--;
    netwib_er(netwib_debug_disp_fmt("%s : byte %{uint32} of pattern ",
                                  patternname, i));
    netwib_er(netwib_debug_disp_fmt("at %p has changed.\n", *ppc));
    netwib_er(netwib_debug_disp_fmt("We found %{byte:02X} instead of ", *pc));
    netwib_er(netwib_debug_disp_fmt("%{byte:02X} (pattern", c));
    netwib_er(netwib_debug_disp_fmt("%{byte:02X}%{byte:02X}%{byte:02X}).\n",
                                  c0, c1, c2));
    netwib_er(netwib_debug_disp_string("The bad pattern is :\n"));
    netwib_er(netwib_debug_disp_dump(*ppc, patternsize));
    return(NETWIB_ERR_LOOBJUSECORRUPT);
  }
  *ppc = pc;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_debug_memcorrupt_al_add2(netwib_constdata ptr)
{
  netwib_uintptr alloclistsize;
  netwib_debug_glovars_t *pglo = &netwib_debug_glovars;

  /* add in list */
  alloclistsize = (netwib_uintptr) pglo->corrupt_alloclist[0];
  if (alloclistsize == pglo->corrupt_alloclist_maxitems) {
    pglo->corrupt_alloclist_maxitems += NETWIB_DEBUG_MEMCORRUPT_ALLOCCHUNKS;
    pglo->corrupt_alloclist =
      (const void **)realloc(NETWIB_DEBUG_CAST_FREE(pglo->corrupt_alloclist),
                             (pglo->corrupt_alloclist_maxitems + 1) *
                             sizeof(void *));
    if (pglo->corrupt_alloclist == NULL) {
      return(NETWIB_ERR_FUREALLOC);
    }
  }
  alloclistsize++;
  pglo->corrupt_alloclist[alloclistsize] = (const void *)ptr;
  pglo->corrupt_alloclist[0] = (const void *)alloclistsize;
  /*netwib_er(netwib_debug_disp_fmt("Set %p\n", ptr)); */
  return(NETWIB_ERR_OK);
}
static netwib_err netwib_debug_memcorrupt_al_add(netwib_constdata ptr)
{
  netwib_err ret;
  netwib_er(netwib_debug_glovars_wrlock());
  ret = netwib_debug_memcorrupt_al_add2(ptr);
  netwib_er(netwib_debug_glovars_wrunlock());
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_debug_memcorrupt_al_del2(netwib_constdata ptr)
{
  netwib_uintptr alloclistsize, i;
  netwib_debug_glovars_t *pglo = &netwib_debug_glovars;

  alloclistsize = (netwib_uintptr) pglo->corrupt_alloclist[0];
  if (alloclistsize == 0) {
    return(NETWIB_ERR_LOOBJCLOSENOTINITIALIZED);
  }
  /*netwib_er(netwib_debug_disp_fmt("Del %p\n", ptr)); */

  for (i = 1; i <= alloclistsize; i++) {
    if (ptr == pglo->corrupt_alloclist[i]) {
      if (alloclistsize != 1) {
        pglo->corrupt_alloclist[i] = pglo->corrupt_alloclist[alloclistsize];
        pglo->corrupt_alloclist[alloclistsize] = NULL;
      }
      alloclistsize--;
      pglo->corrupt_alloclist[0] = (const void *)alloclistsize;
      return(NETWIB_ERR_OK);
    }
  }

  return(NETWIB_ERR_LOOBJCLOSENOTINITIALIZED);
}
/*
Not needed for the moment
static netwib_err netwib_debug_memcorrupt_al_del(netwib_constdata ptr);
static netwib_err netwib_debug_memcorrupt_al_del(netwib_constdata ptr)
{
  netwib_err ret;
  netwib_er(netwib_debug_glovars_wrlock());
  ret = netwib_debug_memcorrupt_al_del2(ptr);
  netwib_er(netwib_debug_glovars_wrunlock());
  return(ret);
}
*/

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwib_debug_memcorrupt_init(void)
{
  int i;
  netwib_debug_glovars_t *pglo = &netwib_debug_glovars;

  netwib_er(netwib_debug_glovars_wrlock());

  /* initialize for alloc */
  pglo->corrupt_alloclist_maxitems = NETWIB_DEBUG_MEMCORRUPT_ALLOCCHUNKS;
  pglo->corrupt_alloclist =
    (const void **)malloc((pglo->corrupt_alloclist_maxitems + 1) *
                          sizeof(void *));
  if (pglo->corrupt_alloclist == NULL) {
    netwib_er(netwib_debug_glovars_wrunlock());
    return(NETWIB_ERR_FUMALLOC);
  }
  pglo->corrupt_alloclist[0] = (const void *)0;

  /* initialize for free */
  pglo->corrupt_freelist_nextpost = 0;
  for (i = 0; i < NETWIB_DEBUG_MEMCORRUPT_FREELIST_SIZE; i++) {
    pglo->corrupt_freelist_ptr[i] = NULL;
  }

  netwib_er(netwib_debug_glovars_wrunlock());
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_memcorrupt_close(void)
{
  netwib_data ptr;
  netwib_uint32 ptrsize;
  int i;
  netwib_debug_glovars_t *pglo = &netwib_debug_glovars;

  netwib_er(netwib_debug_glovars_wrlock());

  /* close alloc */
  free(NETWIB_DEBUG_CAST_FREE(pglo->corrupt_alloclist));
  pglo->corrupt_alloclist = NULL;
  pglo->corrupt_alloclist_maxitems = 0;

  /* close free */
  for (i = 0; i < NETWIB_DEBUG_MEMCORRUPT_FREELIST_SIZE; i++) {
    ptr = (netwib_data) pglo->corrupt_freelist_ptr[i];
    ptrsize = pglo->corrupt_freelist_size[i];
    if (ptr != NULL) {
      /*netwib_er(netwib_debug_disp_fmt("Free %p\n", ptr)); */
#if NETWIB_DEBUG_MEM_CORRUPT==2 || NETWIB_DEBUG_MEM_CORRUPT==3
      {
        netwib_err ret;
        ret = netwib_debug_memprotect_protect(ptr, ptrsize, NETWIB_TRUE);
        if (ret != NETWIB_ERR_OK) {
          netwib_er(netwib_debug_glovars_wrunlock());
          return(ret);
        }
        ret = netwib_debug_memprotect_free(ptr, ptrsize);
        if (ret != NETWIB_ERR_OK) {
          netwib_er(netwib_debug_glovars_wrunlock());
          return(ret);
        }
      }
#else
      free(ptr);
#endif
      pglo->corrupt_freelist_ptr[i] = NULL;
    }
  }
  pglo->corrupt_freelist_nextpost = 0;

  netwib_er(netwib_debug_glovars_wrunlock());
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_memcorrupt_check(netwib_constptr ptr)
{
  return(netwib_debug_memcorrupt_alloc_check(ptr, NULL, NULL, NULL, NULL));
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_memcorrupt_check_all(void)
{
  netwib_uintptr alloclistsize, i;
  netwib_uint32 size;
  const void *ptr;
  netwib_debug_glovars_t *pglo = &netwib_debug_glovars;
  netwib_err ret;

  netwib_er(netwib_debug_glovars_rdlock());

  /* check allocated data */
  alloclistsize = (netwib_uintptr) pglo->corrupt_alloclist[0];
  for (i = 1; i <= alloclistsize; i++) {
    ptr = pglo->corrupt_alloclist[i];
    /*netwib_er(netwib_debug_disp_fmt("CheckA %p\n", ptr)); */
    ret = netwib_debug_memcorrupt_check((netwib_constdata) ptr);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwib_debug_glovars_rdunlock());
      return(ret);
    }
  }

  /* check freed data */
  for (i = 0; i < NETWIB_DEBUG_MEMCORRUPT_FREELIST_SIZE; i++) {
    ptr = pglo->corrupt_freelist_ptr[i];
    size = pglo->corrupt_freelist_size[i];
    /*netwib_er(netwib_debug_disp_fmt("CheckF %p %{uint32}\n", ptr, size)); */
    if (ptr != NULL) {
      ret = netwib_debug_memcorrupt_free_check((netwib_constdata)ptr, size);
      if (ret != NETWIB_ERR_OK) {
        netwib_er(netwib_debug_glovars_rdunlock());
        return(ret);
      }
    }
  }

  netwib_er(netwib_debug_glovars_rdunlock());
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_memcorrupt_check_periodic(void)
{
  static int i = 1;

  if (NETWIB_DEBUG_MEMCORRUPT_CHECK_FREQUENCY == 0)
    return(NETWIB_ERR_OK);

  i++;
  if (i > NETWIB_DEBUG_MEMCORRUPT_CHECK_FREQUENCY) {
    i = 1;
    return(netwib_debug_memcorrupt_check_all());
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_memcorrupt_realloc(netwib_uint32 newallocsize,
                                           netwib_ptr *pptr)
{
  netwib_ptr ptr;
  netwib_uint32 allocsize;

  if (*pptr == NULL) {
    return(netwib_debug_memcorrupt_alloc(newallocsize, pptr));
  }

  netwib_er(netwib_debug_memcorrupt_alloc_check(*pptr, &allocsize,
                                                NULL, NULL, NULL));
  netwib_er(netwib_debug_memcorrupt_alloc(newallocsize, &ptr));
  if (newallocsize <= allocsize) {
    netwib_c_memcpy(ptr, *pptr, newallocsize);
  } else {
    netwib_c_memcpy(ptr, *pptr, allocsize);
    netwib_c_memset((netwib_data) ptr + allocsize, 0,
                    newallocsize - allocsize);
  }
  netwib_er(netwib_debug_memcorrupt_free(pptr));

  if (pptr != NULL)
    *pptr = ptr;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_memcorrupt_add(netwib_ptr ptr)
{
  netwib_er(netwib_debug_memcorrupt_al_add((netwib_constdata)ptr));
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_MEM_CORRUPT==1
/* ptroffset
   pattern
   allocsize smallpattern
   allocsizexor smallpattern
   data
   smallpattern allocsize
   pattern
 Which is :
   sizeof(netwib_uint32)+
   patternsize+
   sizeof(netwib_uint32)+MEMCORRUPT_SMALLPATTERN_SIZE+
   sizeof(netwib_uint32)+MEMCORRUPT_SMALLPATTERN_SIZE+
   allocsize+
   MEMCORRUPT_SMALLPATTERN_SIZE+sizeof(netwib_uint32)+
   patternsize;
 */
netwib_err netwib_debug_memcorrupt_alloc(netwib_uint32 allocsize,
                                         netwib_ptr *pptr)
{
  netwib_uint32 patternsize, ptroffset;
  netwib_uint32 trueallocsize;
  netwib_data trueptr, ptr, pc;

  /* compute ptroffset */
  patternsize = allocsize + 2; /* this value migth increase if aligned */
  ptroffset = patternsize + 3*sizeof(netwib_uint32) +
    2*MEMCORRUPT_SMALLPATTERN_SIZE;
#if NETWIBDEF_ARCH_ALIGN==1
  ptroffset = NETWIB_DEBUG_MEMCORRUPT_ALIGN(ptroffset);
  patternsize = ptroffset -
    (3*sizeof(netwib_uint32) + 2*MEMCORRUPT_SMALLPATTERN_SIZE);
#endif

  /* compute trueallocsize */
  trueallocsize = ptroffset + allocsize + sizeof(netwib_uint32) +
    MEMCORRUPT_SMALLPATTERN_SIZE + patternsize;

  /* allocate data */
  trueptr = (netwib_data) malloc(trueallocsize);
  if (trueptr == NULL)
    return(NETWIB_ERR_FUMALLOC);

  /* set ptr */
  ptr = trueptr + ptroffset;

  /* now, initialize trueptr */
  pc = trueptr;
  netwib_debug_mem_data_init_uint32(ptroffset, pc);
  netwib_er(memcorrupt_pattern_init(&pc, patternsize,
                                    MEMCORRUPT_PATTERNTYPE_OUTER));
  netwib_debug_mem_data_init_uint32(allocsize, pc);
  netwib_er(memcorrupt_pattern_init(&pc, MEMCORRUPT_SMALLPATTERN_SIZE,
                                    MEMCORRUPT_PATTERNTYPE_OUTER));
  netwib_debug_mem_data_init_uint32(0xFFFFFFFFu ^ allocsize, pc);
  netwib_er(memcorrupt_pattern_init(&pc, MEMCORRUPT_SMALLPATTERN_SIZE,
                                    MEMCORRUPT_PATTERNTYPE_OUTER));
  netwib_er(memcorrupt_pattern_init(&pc, allocsize,
                                    MEMCORRUPT_PATTERNTYPE_UNINIT));
  netwib_er(memcorrupt_pattern_init(&pc, MEMCORRUPT_SMALLPATTERN_SIZE,
                                    MEMCORRUPT_PATTERNTYPE_OUTER));
  netwib_debug_mem_data_init_uint32(allocsize, pc);
  netwib_er(memcorrupt_pattern_init(&pc, patternsize,
                                    MEMCORRUPT_PATTERNTYPE_OUTER));

  /* display */
  /*netwib_er(netwib_debug_disp_dump(trueptr, trueallocsize)); */

  /* add in list */
  netwib_er(netwib_debug_memcorrupt_al_add(ptr));

  /* set output */
  *pptr = ptr;
  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_MEM_CORRUPT==1
static netwib_err netwib_debug_memcorrupt_free2(netwib_ptr *pptr)
{
  netwib_data trueptr, pc, ptr;
  netwib_uint32 trueallocsize, ptrsize, nextpost;
  netwib_debug_glovars_t *pglo = &netwib_debug_glovars;

  /* check it was not corrupted */
  netwib_er(netwib_debug_memcorrupt_alloc_check(*pptr, NULL, NULL,
                                                (netwib_ptr *) &trueptr,
                                                &trueallocsize));

  /* remove it from the list */
  netwib_er(netwib_debug_memcorrupt_al_del2((netwib_data)*pptr));

  /* empty data zone */
  pc = trueptr;
  netwib_er(memcorrupt_pattern_init(&pc, trueallocsize,
                                    MEMCORRUPT_PATTERNTYPE_FREE));

  /* deal with saved freed data */
  nextpost = pglo->corrupt_freelist_nextpost;
  ptr = (netwib_data) pglo->corrupt_freelist_ptr[nextpost];
  if (ptr != NULL) {
    ptrsize = pglo->corrupt_freelist_size[nextpost];
    netwib_er(netwib_debug_memcorrupt_free_check(ptr, ptrsize));
    /*netwib_er(netwib_debug_disp_fmt("Free_ %p\n", ptr)); */
    free(ptr);
  }
  pglo->corrupt_freelist_ptr[nextpost] = trueptr;
  pglo->corrupt_freelist_size[nextpost] = trueallocsize;
  nextpost++;
  if (nextpost >= NETWIB_DEBUG_MEMCORRUPT_FREELIST_SIZE)
    nextpost = 0;
  pglo->corrupt_freelist_nextpost = nextpost;

  return(NETWIB_ERR_OK);
}
netwib_err netwib_debug_memcorrupt_free(netwib_ptr *pptr)
{
  netwib_err ret;

  netwib_er(netwib_debug_glovars_wrlock());
  ret = netwib_debug_memcorrupt_free2(pptr);
  netwib_er(netwib_debug_glovars_wrunlock());

  return(ret);
}
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_MEM_CORRUPT==1
netwib_err netwib_debug_memcorrupt_alloc_check(netwib_constptr ptr,
                                               netwib_uint32 *pallocsize,
                                               netwib_uint32 *pptroffset,
                                               netwib_ptr *ptrueptr,
                                               netwib_uint32 *ptrueallocsize)
{
  netwib_uint32 patternsize, offset, readuint32;
  netwib_uint32 ptroffset, allocsize;
  netwib_constdata pc;

  /* obtain allocsize from bottom data */
  offset = 2*sizeof(netwib_uint32) + 2*MEMCORRUPT_SMALLPATTERN_SIZE;
  pc = (netwib_constdata) ptr - offset;
  netwib_debug_mem_uint32_init_data(pc, allocsize);
  netwib_er(memcorrupt_pattern_check("Outer_pattern -2", &pc,
                                     MEMCORRUPT_SMALLPATTERN_SIZE,
                                     MEMCORRUPT_PATTERNTYPE_OUTER));
  netwib_debug_mem_uint32_init_data(pc, readuint32);
  netwib_er(memcorrupt_pattern_check("Outer_pattern -1", &pc,
                                     MEMCORRUPT_SMALLPATTERN_SIZE,
                                     MEMCORRUPT_PATTERNTYPE_OUTER));
  if (allocsize != (readuint32 ^ 0xFFFFFFFFu)) {
    netwib_er(netwib_debug_disp_fmt("Allocsize(%{uint32:08X}) and allocsize",
                                    allocsize));
    netwib_er(netwib_debug_disp_fmt("(xor(%{uint32:08X})=%{uint32:08X}) ",
                                    readuint32, readuint32 ^ 0xFFFFFFFFu));
    netwib_er(netwib_debug_disp_fmt("are different.\nHere is allocsize+outer"));
    netwib_er(netwib_debug_disp_fmt("pattern-2+allocsizexor+"));
    netwib_er(netwib_debug_disp_fmt("outerpattern-1 :\n"));
    netwib_er(netwib_debug_disp_dump((netwib_constdata) ptr - offset, offset));
    netwib_er(netwib_debug_disp_fmt("Here is concerned memory (ptr=%p ", ptr));
    netwib_er(netwib_debug_disp_fmt("of size %{uint32}) :\n", allocsize));
    netwib_er(netwib_debug_disp_dump((netwib_constdata)ptr, allocsize));
    return(NETWIB_ERR_LOOBJUSECORRUPT);
  }

  /* check it corresponds with top data */
  pc = (netwib_constdata) ptr + allocsize;
  netwib_er(memcorrupt_pattern_check("Outer_pattern +1", &pc,
                                     MEMCORRUPT_SMALLPATTERN_SIZE,
                                     MEMCORRUPT_PATTERNTYPE_OUTER));
  netwib_debug_mem_uint32_init_data(pc, readuint32);
  if (allocsize != readuint32) {
    netwib_er(netwib_debug_disp_fmt("Bottom allocsize(%{uint32:08X}) and top ",
                                    allocsize));
    netwib_er(netwib_debug_disp_fmt("allocsize(%{uint32:08X}) are different.\n",
                                    readuint32));
    netwib_er(netwib_debug_disp_fmt("Here is outerpattern+1+allocsize :\n"));
    netwib_er(netwib_debug_disp_dump((netwib_constdata) ptr + allocsize +
                                     MEMCORRUPT_SMALLPATTERN_SIZE, offset));
    netwib_er(netwib_debug_disp_fmt("Here is concerned memory (ptr=%p ", ptr));
    netwib_er(netwib_debug_disp_fmt("of size %{uint32}) :\n", allocsize));
    netwib_er(netwib_debug_disp_dump((netwib_constdata)ptr, allocsize));
    return(NETWIB_ERR_LOOBJUSECORRUPT);
  }

  /* now check bottom patternsize */
  patternsize = allocsize + 2;
  offset = patternsize + 3*sizeof(netwib_uint32) +
    2*MEMCORRUPT_SMALLPATTERN_SIZE;
#if NETWIBDEF_ARCH_ALIGN==1
  offset = NETWIB_DEBUG_MEMCORRUPT_ALIGN(offset);
  patternsize = offset - 28;
#endif
  pc = (netwib_constdata) ptr - offset;
  netwib_debug_mem_uint32_init_data(pc, ptroffset);
  netwib_er(memcorrupt_pattern_check("Outer_pattern -3", &pc,
                                     patternsize,
                                     MEMCORRUPT_PATTERNTYPE_OUTER));
  if (ptroffset != offset) {
    netwib_er(netwib_debug_disp_fmt("Ptroffset(%{uint32:08X}) and computer ",
                                    ptroffset));
    netwib_er(netwib_debug_disp_fmt("offset(%{uint32:08X}) are different.\n",
                                    offset));
    netwib_er(netwib_debug_disp_fmt("Here is concerned memory (ptr=%p ", ptr));
    netwib_er(netwib_debug_disp_fmt("of size %{uint32}) :\n", allocsize));
    netwib_er(netwib_debug_disp_dump((netwib_constdata)ptr, allocsize));
    return(NETWIB_ERR_LOOBJUSECORRUPT);
  }

  /* now check top patternsize */
  pc = (netwib_constdata) ptr + allocsize + sizeof(netwib_uint32) +
    MEMCORRUPT_SMALLPATTERN_SIZE;
  netwib_er(memcorrupt_pattern_check("Outer_pattern +2", &pc,
                                     patternsize,
                                     MEMCORRUPT_PATTERNTYPE_OUTER));

  /* set output */
  if (pallocsize != NULL)
    *pallocsize = allocsize;
  if (pptroffset != NULL)
    *pptroffset = ptroffset;
  if (ptrueptr != NULL) {
    *ptrueptr = (netwib_data) ((netwib_uintptr)ptr - ptroffset);
  }
  if (ptrueallocsize != NULL) {
    *ptrueallocsize = ptroffset + allocsize + sizeof(netwib_uint32) +
      MEMCORRUPT_SMALLPATTERN_SIZE + patternsize;
  }
  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_MEM_CORRUPT==1
netwib_err netwib_debug_memcorrupt_free_check(netwib_constptr trueptr,
                                              netwib_uint32 trueallocsize)
{
  netwib_constdata pc;

  pc = (netwib_constdata)trueptr;
  netwib_er(memcorrupt_pattern_check("Free", &pc, trueallocsize,
                                     MEMCORRUPT_PATTERNTYPE_FREE));

  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_MEM_CORRUPT==2 || NETWIB_DEBUG_MEM_CORRUPT==3
/* bottom_pattern
   protected_memory
   middle_pattern
   bottom_pattern_size
   protected_memory_size
   middle_pattern_size
   top_pattern_size
   allocsize smallpattern
   allocsizexor smallpattern
   data
   protected_memory
   top_pattern
*/
/* bottom_pattern
   protected_memory
   allocsize smallpattern
   allocsizexor smallpattern
   data
   smallpattern
   bottom_pattern_size
   protected_memory_size
   middle_pattern_size
   top_pattern_size
   middle_pattern
   protected_memory
   top_pattern
*/
netwib_err netwib_debug_memcorrupt_alloc(netwib_uint32 allocsize,
                                         netwib_ptr *pptr)
{
  netwib_uint32 protectedpages, protectedsize, checkzonesize;
  netwib_uint32 middlepages, middlesize, middlepatternsize, middlefixedsize;
  netwib_uint32 bottompatternsize, toppatternsize, tmpvar, trueallocsize;
  netwib_uint32 alignpad;
  netwib_uintptr tmpvarptr;
  netwib_data trueptr, ptr, pc;

  /* compute protectedsize */
  protectedsize = allocsize + 2;
  /* number of memory pages is a multiple of NETWIB_PRIV_PAGESIZE */
  protectedpages = 1 + (protectedsize - 1) / NETWIB_PRIV_PAGESIZE;
  protectedsize = protectedpages * NETWIB_PRIV_PAGESIZE;

  /* compute the necessary alignment pad */
#if NETWIB_DEBUG_MEM_CORRUPT==2 && NETWIBDEF_ARCH_ALIGN==1
  /* note : if NETWIB_DEBUG_MEM_CORRUPT==3, it is aligned, so alignpad=0 */
  alignpad = NETWIB_DEBUG_MEMCORRUPT_ALIGNPAD(allocsize);
#else
  alignpad = 0;
#endif

  /* middle pages must be aligned on a NETWIB_PRIV_PAGESIZE boundary */
  checkzonesize = 4 * sizeof(netwib_uint32) +
    2*sizeof(netwib_uint32) + 2*MEMCORRUPT_SMALLPATTERN_SIZE;
#if NETWIB_DEBUG_MEM_CORRUPT==3
  checkzonesize += MEMCORRUPT_SMALLPATTERN_SIZE;
#endif
  middlefixedsize = allocsize + alignpad + checkzonesize;
  tmpvar = middlefixedsize % NETWIB_PRIV_PAGESIZE;
  middlepatternsize = (tmpvar == 0) ? 0 : NETWIB_PRIV_PAGESIZE - tmpvar;
  middlesize = middlepatternsize + middlefixedsize;
  middlepages = middlesize / NETWIB_PRIV_PAGESIZE;

  /* compute trueallocsize */
  tmpvar = 2 * protectedpages + middlepages;
#if defined NETWIBDEF_SYSNAME_Unix
  trueallocsize = tmpvar * NETWIB_PRIV_PAGESIZE + NETWIB_PRIV_PAGESIZE - 1;
#elif defined NETWIBDEF_SYSNAME_Windows
  trueallocsize = tmpvar * NETWIB_PRIV_PAGESIZE;
#endif

  /* allocate data */
  netwib_er(netwib_debug_memprotect_alloc(trueallocsize,
                                          (netwib_ptr *)&trueptr));

  /* compute bottom pattern size (aligned) */
  tmpvarptr = NETWIB_PRIV_PAGESIZE - 1;
  pc = (netwib_data) (((netwib_uintptr)trueptr + tmpvarptr) & (~tmpvarptr));
  bottompatternsize = pc - trueptr;

  /* compute top pattern size */
  tmpvar = trueallocsize % NETWIB_PRIV_PAGESIZE;
  toppatternsize = tmpvar - bottompatternsize;

  /* set ptr */
#if NETWIB_DEBUG_MEM_CORRUPT==2
  ptr = pc + protectedsize + middlepatternsize + checkzonesize;
#elif NETWIB_DEBUG_MEM_CORRUPT==3
  ptr = pc + protectedsize +
    2 * (sizeof(netwib_uint32) + MEMCORRUPT_SMALLPATTERN_SIZE);
#endif

  /* now, initialize trueptr */
  pc = trueptr;
  netwib_er(memcorrupt_pattern_init(&pc, bottompatternsize,
                                    MEMCORRUPT_PATTERNTYPE_OUTER));
  netwib_er(netwib_debug_memprotect_protect(pc,
                                            protectedpages*NETWIB_PRIV_PAGESIZE,
                                            NETWIB_FALSE));
  pc += protectedpages * NETWIB_PRIV_PAGESIZE;
#if NETWIB_DEBUG_MEM_CORRUPT==2
  netwib_er(memcorrupt_pattern_init(&pc, middlepatternsize,
                                    MEMCORRUPT_PATTERNTYPE_OUTER));
  netwib_debug_mem_data_init_uint32(bottompatternsize, pc);
  netwib_debug_mem_data_init_uint32(protectedsize, pc);
  netwib_debug_mem_data_init_uint32(middlepatternsize, pc);
  netwib_debug_mem_data_init_uint32(toppatternsize, pc);
  netwib_debug_mem_data_init_uint32(allocsize, pc);
  netwib_er(memcorrupt_pattern_init(&pc, MEMCORRUPT_SMALLPATTERN_SIZE,
                                    MEMCORRUPT_PATTERNTYPE_OUTER));
  netwib_debug_mem_data_init_uint32(0xFFFFFFFFu ^ allocsize, pc);
  netwib_er(memcorrupt_pattern_init(&pc, MEMCORRUPT_SMALLPATTERN_SIZE,
                                    MEMCORRUPT_PATTERNTYPE_OUTER));
  netwib_er(memcorrupt_pattern_init(&pc, allocsize,
                                    MEMCORRUPT_PATTERNTYPE_UNINIT));
  netwib_er(memcorrupt_pattern_init(&pc, alignpad,
                                    MEMCORRUPT_PATTERNTYPE_OUTER));
#elif NETWIB_DEBUG_MEM_CORRUPT==3
  netwib_debug_mem_data_init_uint32(allocsize, pc);
  netwib_er(memcorrupt_pattern_init(&pc, MEMCORRUPT_SMALLPATTERN_SIZE,
                                    MEMCORRUPT_PATTERNTYPE_OUTER));
  netwib_debug_mem_data_init_uint32(0xFFFFFFFFu ^ allocsize, pc);
  netwib_er(memcorrupt_pattern_init(&pc, MEMCORRUPT_SMALLPATTERN_SIZE,
                                    MEMCORRUPT_PATTERNTYPE_OUTER));
  netwib_er(memcorrupt_pattern_init(&pc, allocsize,
                                    MEMCORRUPT_PATTERNTYPE_UNINIT));
  netwib_er(memcorrupt_pattern_init(&pc, MEMCORRUPT_SMALLPATTERN_SIZE,
                                    MEMCORRUPT_PATTERNTYPE_OUTER));
  netwib_debug_mem_data_init_uint32(bottompatternsize, pc);
  netwib_debug_mem_data_init_uint32(protectedsize, pc);
  netwib_debug_mem_data_init_uint32(middlepatternsize, pc);
  netwib_debug_mem_data_init_uint32(toppatternsize, pc);
  netwib_er(memcorrupt_pattern_init(&pc, middlepatternsize,
                                    MEMCORRUPT_PATTERNTYPE_OUTER));
#endif
  netwib_er(netwib_debug_memprotect_protect(pc,
                                            protectedpages*NETWIB_PRIV_PAGESIZE,
                                            NETWIB_FALSE));
  pc += protectedpages * NETWIB_PRIV_PAGESIZE;
  netwib_er(memcorrupt_pattern_init(&pc, toppatternsize,
                                    MEMCORRUPT_PATTERNTYPE_OUTER));

  /* display */
  /*netwib_er(netwib_debug_disp_dump(trueptr, trueallocsize)); */

  /* add in list */
  netwib_er(netwib_debug_memcorrupt_al_add(ptr));

  /* set output */
  *pptr = ptr;
  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_MEM_CORRUPT==2 || NETWIB_DEBUG_MEM_CORRUPT==3
static netwib_err netwib_debug_memcorrupt_free2(netwib_ptr *pptr)
{
  netwib_ptr trueptr, ptr;
  netwib_uint32 trueallocsize, ptrsize, nextpost;
  netwib_debug_glovars_t *pglo = &netwib_debug_glovars;

  /* check it was not corrupted */
  netwib_er(netwib_debug_memcorrupt_alloc_check(*pptr, NULL, NULL,
                                                &trueptr, &trueallocsize));

  /* remove it from the list */
  netwib_er(netwib_debug_memcorrupt_al_del2((netwib_constdata)*pptr));

  /* set all pages as protected */
  netwib_er(netwib_debug_memprotect_protect(trueptr, trueallocsize,
                                            NETWIB_FALSE));

  /* deal with saved freed data */
  nextpost = pglo->corrupt_freelist_nextpost;
  ptr = (netwib_data) pglo->corrupt_freelist_ptr[nextpost];
  if (ptr != NULL) {
    ptrsize = pglo->corrupt_freelist_size[nextpost];
    netwib_er(netwib_debug_memcorrupt_free_check(ptr, ptrsize));
    netwib_er(netwib_debug_memprotect_protect(ptr, ptrsize, NETWIB_TRUE));
    netwib_er(netwib_debug_memprotect_free(ptr, ptrsize));
  }
  pglo->corrupt_freelist_ptr[nextpost] = trueptr;
  pglo->corrupt_freelist_size[nextpost] = trueallocsize;
  nextpost++;
  if (nextpost >= NETWIB_DEBUG_MEMCORRUPT_FREELIST_SIZE)
    nextpost = 0;
  pglo->corrupt_freelist_nextpost = nextpost;

  return(NETWIB_ERR_OK);
}
netwib_err netwib_debug_memcorrupt_free(netwib_ptr *pptr)
{
  netwib_err ret;

  netwib_er(netwib_debug_glovars_wrlock());
  ret = netwib_debug_memcorrupt_free2(pptr);
  netwib_er(netwib_debug_glovars_wrunlock());

  return(ret);
}
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_MEM_CORRUPT==2 || NETWIB_DEBUG_MEM_CORRUPT==3
netwib_err netwib_debug_memcorrupt_alloc_check(netwib_constptr ptr,
                                               netwib_uint32 *pallocsize,
                                               netwib_uint32 *pptroffset,
                                               netwib_ptr *ptrueptr,
                                               netwib_uint32 *ptrueallocsize)
{
  netwib_uint32 readuint32, checkzonesize2, checkzonesize4;
  netwib_uint32 ptroffset, allocsize, bottompatternsize, trueallocsize;
  netwib_uint32 protectedsize, middlepatternsize, toppatternsize;
  netwib_uint32 alignpad;
  netwib_constdata pc;

  /* obtain allocsize from bottom data */
  checkzonesize2 = 2 * (sizeof(netwib_uint32) +
                        MEMCORRUPT_SMALLPATTERN_SIZE);
  checkzonesize4 = 4 * sizeof(netwib_uint32);
#if NETWIB_DEBUG_MEM_CORRUPT==3
  checkzonesize4 += MEMCORRUPT_SMALLPATTERN_SIZE;
#endif
  pc = (netwib_constdata) ptr - checkzonesize2;
  netwib_debug_mem_uint32_init_data(pc, allocsize);
  netwib_er(memcorrupt_pattern_check("Outer_pattern -2", &pc,
                                     MEMCORRUPT_SMALLPATTERN_SIZE,
                                     MEMCORRUPT_PATTERNTYPE_OUTER));
  netwib_debug_mem_uint32_init_data(pc, readuint32);
  netwib_er(memcorrupt_pattern_check("Outer_pattern -1", &pc,
                                     MEMCORRUPT_SMALLPATTERN_SIZE,
                                     MEMCORRUPT_PATTERNTYPE_OUTER));
  if (allocsize != (readuint32 ^ 0xFFFFFFFFu)) {
    netwib_er(netwib_debug_disp_fmt("Allocsize(%{uint32:08X}) and allocsize(",
                                    allocsize));
    netwib_er(netwib_debug_disp_fmt("xor(%{uint32:08X})=%{uint32:08X}) are ",
                                    readuint32));
    netwib_er(netwib_debug_disp_fmt("different.\n", readuint32 ^ 0xFFFFFFFFu));
    netwib_er(netwib_debug_disp_fmt("Here is allocsize+outerpattern-2+alloc"));
    netwib_er(netwib_debug_disp_fmt("sizexor+outerpattern-1 :\n"));
    netwib_er(netwib_debug_disp_dump((netwib_constdata) ptr - checkzonesize2,
                                     checkzonesize2));
    return(NETWIB_ERR_LOOBJUSECORRUPT);
  }

  /* compute the necessary alignment pad */
#if NETWIB_DEBUG_MEM_CORRUPT==2 && NETWIBDEF_ARCH_ALIGN==1
  /* note : if NETWIB_DEBUG_MEM_CORRUPT==3, it is aligned, so alignpad=0 */
  alignpad = NETWIB_DEBUG_MEMCORRUPT_ALIGNPAD(allocsize);
#else
  alignpad = 0;
#endif

  /* obtain other values */
#if NETWIB_DEBUG_MEM_CORRUPT==2
  pc = (netwib_constdata) ptr - checkzonesize2 - checkzonesize4;
#elif NETWIB_DEBUG_MEM_CORRUPT==3
  pc = (netwib_constdata) ptr + allocsize;
  netwib_er(memcorrupt_pattern_check("Outer_pattern +1", &pc,
                                     MEMCORRUPT_SMALLPATTERN_SIZE,
                                     MEMCORRUPT_PATTERNTYPE_OUTER));
#endif
  netwib_debug_mem_uint32_init_data(pc, bottompatternsize);
  netwib_debug_mem_uint32_init_data(pc, protectedsize);
  netwib_debug_mem_uint32_init_data(pc, middlepatternsize);
  netwib_debug_mem_uint32_init_data(pc, toppatternsize);
  /* compute those values */
#if NETWIB_DEBUG_MEM_CORRUPT==2
  ptroffset = bottompatternsize + protectedsize + middlepatternsize +
              checkzonesize4 + checkzonesize2;
  trueallocsize = ptroffset + allocsize + alignpad + protectedsize +
                  toppatternsize;
#elif NETWIB_DEBUG_MEM_CORRUPT==3
  ptroffset = bottompatternsize + protectedsize + checkzonesize2;
  trueallocsize = ptroffset + allocsize + checkzonesize4 +
                  middlepatternsize + protectedsize + toppatternsize;
#endif

  /* check memory */
  pc = (netwib_constdata) ptr - ptroffset;
#if NETWIB_DEBUG_MEM_CORRUPT==2
  netwib_er(memcorrupt_pattern_check("Outer_pattern -4", &pc,
                                     bottompatternsize,
                                     MEMCORRUPT_PATTERNTYPE_OUTER));
  pc += protectedsize;
  netwib_er(memcorrupt_pattern_check("Outer_pattern -3", &pc,
                                     middlepatternsize,
                                     MEMCORRUPT_PATTERNTYPE_OUTER));
  pc += checkzonesize4 + checkzonesize2 + allocsize;
  netwib_er(memcorrupt_pattern_check("Outer_pattern +1b", &pc,
                                     alignpad,
                                     MEMCORRUPT_PATTERNTYPE_OUTER));
  pc += protectedsize;
  netwib_er(memcorrupt_pattern_check("Outer_pattern +1", &pc,
                                     toppatternsize,
                                     MEMCORRUPT_PATTERNTYPE_OUTER));
#elif NETWIB_DEBUG_MEM_CORRUPT==3
  netwib_er(memcorrupt_pattern_check("Outer_pattern -3", &pc,
                                     bottompatternsize,
                                     MEMCORRUPT_PATTERNTYPE_OUTER));
  pc += protectedsize + checkzonesize2 + allocsize + checkzonesize4;
  netwib_er(memcorrupt_pattern_check("Outer_pattern +2", &pc,
                                     middlepatternsize,
                                     MEMCORRUPT_PATTERNTYPE_OUTER));
  pc += protectedsize;
  netwib_er(memcorrupt_pattern_check("Outer_pattern +3", &pc,
                                     toppatternsize,
                                     MEMCORRUPT_PATTERNTYPE_OUTER));
#endif

  /* set output */
  if (pallocsize != NULL)
    *pallocsize = allocsize;
  if (pptroffset != NULL)
    *pptroffset = ptroffset;
  if (ptrueptr != NULL)
    *ptrueptr = (netwib_data) ((netwib_uintptr) ptr - ptroffset);
  if (ptrueallocsize != NULL)
    *ptrueallocsize = trueallocsize;
  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_MEM_CORRUPT==2 || NETWIB_DEBUG_MEM_CORRUPT==3
netwib_err netwib_debug_memcorrupt_free_check(netwib_constptr trueptr,
                                              netwib_uint32 trueallocsize)
{
  /* there is no check in this case */
  trueptr = trueptr; /* for compiler warning */
  trueallocsize = trueallocsize; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
#endif
