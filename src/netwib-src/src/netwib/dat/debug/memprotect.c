/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Solaris
#define NETWIB_DEBUG_CAST_MPROTECT (netwib_string)
#else
#define NETWIB_DEBUG_CAST_MPROTECT
#endif

/*-------------------------------------------------------------*/
#if NETWIBDEF_HAVEINCL_SYS_MMAN==1
 #include <sys/mman.h>
#endif
#if NETWIBDEF_HAVEINCL_SYS_PARAM==1
 #include <sys/param.h>
#endif
#if NETWIBDEF_HAVEINCL_MACHINE_PARAM==1
 #include <machine/param.h>
#endif
#if NETWIBDEF_HAVEINCL_ASM_PAGE==1
 #include <asm/page.h>
#endif
#if NETWIBDEF_HAVEINCL_MACH_MACHINE_VMPARAM==1
 #include <mach/machine/vm_param.h>
#endif

/*-------------------------------------------------------------*/
/* If PAGESIZE is not defined on the system (which might be ok
   because it is defined at running time), just use a hard coded
   value of 4096 for example. If this value is bad, it will be
   detected in netwib_debug_memprotect_alloc. */
#if NETWIBDEF_HAVEVAR_PAGE_SIZE==1
 #define NETWIB_PRIV_PAGESIZE PAGE_SIZE
#elif NETWIBDEF_HAVEVAR_PAGESIZE==1
 #define NETWIB_PRIV_PAGESIZE PAGESIZE
#else
 #define NETWIB_PRIV_PAGESIZE 4096
#endif

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
netwib_err netwib_debug_memprotect_protect(netwib_ptr ptr,
                                           netwib_uint32 ptrsize,
                                           netwib_bool allowaccess)
{
  netwib_ptr protectedptr;
  netwib_uint32 protectedpages;
  netwib_uintptr tmpvar;
  int prottype, reti;

  protectedpages = ptrsize / NETWIB_PRIV_PAGESIZE;

  tmpvar = NETWIB_PRIV_PAGESIZE - 1;
  protectedptr = (netwib_ptr)(((netwib_uintptr)ptr + tmpvar) & (~tmpvar));

  if (allowaccess) {
    prottype = PROT_READ | PROT_WRITE | PROT_EXEC;
  } else {
    prottype = PROT_NONE;
  }

  reti = mprotect(NETWIB_DEBUG_CAST_MPROTECT protectedptr,
                  protectedpages*NETWIB_PRIV_PAGESIZE, prottype);
  if (reti < 0)
    return(NETWIB_ERR_FUMPROTECT);

  return(NETWIB_ERR_OK);
}
#elif defined NETWIBDEF_SYSNAME_Windows
netwib_err netwib_debug_memprotect_protect(netwib_ptr ptr,
                                           netwib_uint32 ptrsize,
                                           netwib_bool allowaccess)
{
  DWORD oldpro, pro;
  BOOL err;

  if (allowaccess) {
    pro = PAGE_READWRITE;
  } else {
    pro = PAGE_NOACCESS;
  }

  err = VirtualProtect((netwib_ptr)ptr, ptrsize, pro, &oldpro);
  if (!err) {
    return(NETWIB_ERR_FUVIRTUALPROTECT);
  }

  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
static netwib_err netwib_debug_memprotect_checkpagesize(void);

#if defined NETWIBDEF_SYSNAME_Unix
static netwib_err netwib_debug_memprotect_checkpagesize(void)
{
  netwib_uint32 pagesize;

#if NETWIBDEF_HAVEFUNC_GETPAGESIZE==1
  pagesize = getpagesize();
#elif NETWIBDEF_HAVEFUNC_SYSCONF==1
  pagesize = sysconf(_SC_PAGE_SIZE);
#else
  pagesize = (netwib_uint32)NETWIB_PRIV_PAGESIZE;
#endif
  if (pagesize != (netwib_uint32)NETWIB_PRIV_PAGESIZE) {
    netwib_er(netwib_debug_disp_fmt("This computer has a page size of %{uint32}.\n", pagesize));
    return(NETWIB_ERR_LOINTERNALERROR);
  }
  return(NETWIB_ERR_OK);
}
#elif defined NETWIBDEF_SYSNAME_Windows
static netwib_err netwib_debug_memprotect_checkpagesize(void)
{
  SYSTEM_INFO si;
  netwib_uint32 pagesize;

  GetSystemInfo(&si);
  pagesize = si.dwPageSize;
  if (pagesize != NETWIB_PRIV_PAGESIZE) {
    netwib_er(netwib_debug_disp_fmt("This computer has a page size of %{uint32}.\n", pagesize));
    return(NETWIB_ERR_LOINTERNALERROR);
  }
  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
netwib_err netwib_debug_memprotect_alloc(netwib_uint32 allocsize,
                                         netwib_ptr *pptr)
{
  netwib_ptr ptr;
  static netwib_bool checked = NETWIB_FALSE;

  if (!checked) {
    netwib_er(netwib_debug_memprotect_checkpagesize());
    checked = NETWIB_TRUE;
  }

  ptr = (netwib_ptr) malloc(allocsize);
  if (ptr == NULL)
    return(NETWIB_ERR_FUMALLOC);

  if (pptr != NULL)
    *pptr = ptr;
  else
    free(ptr);
  return(NETWIB_ERR_OK);
}
#elif defined NETWIBDEF_SYSNAME_Windows
netwib_err netwib_debug_memprotect_alloc(netwib_uint32 allocsize,
                                         netwib_ptr *pptr)
{
  void *ptrbase, *ptr;
  static netwib_bool checked = NETWIB_FALSE;

  if (!checked) {
    netwib_er(netwib_debug_memprotect_checkpagesize());
    checked = NETWIB_TRUE;
  }

  ptrbase = VirtualAlloc(NULL, allocsize, MEM_RESERVE, PAGE_NOACCESS);
  if (ptrbase == NULL) {
    return(NETWIB_ERR_FUVIRTUALALLOC);
  }
  ptr = VirtualAlloc(ptrbase, allocsize, MEM_COMMIT, PAGE_READWRITE);
  if (ptr == NULL) {
    return(NETWIB_ERR_FUVIRTUALALLOC);
  }

  if (pptr != NULL)
    *pptr = ptrbase;
  else {
    netwib_er(netwib_debug_memprotect_free(ptrbase, allocsize));
  }

  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
netwib_err netwib_debug_memprotect_free(netwib_ptr ptr,
                                        netwib_uint32 ptrsize)
{
  free(ptr);
  ptrsize = ptrsize; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
#elif defined NETWIBDEF_SYSNAME_Windows
netwib_err netwib_debug_memprotect_free(netwib_ptr ptr,
                                        netwib_uint32 ptrsize)
{
  BOOL err;

  err = VirtualFree(ptr, ptrsize, MEM_DECOMMIT);
  if (!err) {
    return(NETWIB_ERR_FUVIRTUALFREE);
  }
  err = VirtualFree(ptr, 0, MEM_RELEASE);
  if (!err) {
    return(NETWIB_ERR_FUVIRTUALFREE);
  }

  return(NETWIB_ERR_OK);
}
#endif
