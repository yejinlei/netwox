/*
                                  NETWIB
                             Network library
                Copyright(c) 1999-2012 Laurent CONSTANTIN
                      http://ntwib.sourceforge.net/
                        laurentconstantin@free.fr
                                  -----
  This file is part of Netwib.

  Netwib is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3
  as published by the Free Software Foundation.

  Netwib is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details (http://www.gnu.org/).

------------------------------------------------------------------------
*/

#include <netwib/inc/maininc.h>

/*-------------------------------------------------------------*/
netwib_err netwib_ptr_malloc(netwib_uint32 allocsize,
                             netwib_ptr *pptr)
{
  netwib_ptr ptr;

#if NETWIB_DEBUG_MEM_CORRUPT==0
  ptr = (netwib_ptr) malloc(allocsize);
  if (ptr == NULL)
    return(NETWIB_ERR_FUMALLOC);
#else
  netwib_er(netwib_debug_memcorrupt_alloc(allocsize, &ptr));
  netwib_er(netwib_debug_memcorrupt_check_periodic());
#endif

#if NETWIB_DEBUG_LEAK!=0
  netwib_er(netwib_debug_leak_add_mem(allocsize, ptr));
#endif

  if (pptr != NULL) {
    *pptr = ptr;
  } else {
    /* our check to see if memory can really be allocated succeeded */
#if NETWIB_DEBUG_LEAK!=0
    netwib_er(netwib_debug_leak_del_mem(ptr));
#endif

#if NETWIB_DEBUG_MEM_CORRUPT==0
    free(ptr);
#else
    netwib_er(netwib_debug_memcorrupt_free(&ptr));
    netwib_er(netwib_debug_memcorrupt_check_periodic());
#endif
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ptr_realloc(netwib_uint32 newallocsize,
                              netwib_ptr *pptr)
{
  if (pptr == NULL) {
    /* we have no sufficient info, but we suppose it will be ok if
       a correct parameter is given*/
    return(NETWIB_ERR_OK);
  }

#if NETWIB_DEBUG_LEAK!=0
  netwib_er(netwib_debug_leak_del_mem(*pptr));
#endif

#if NETWIB_DEBUG_MEM_CORRUPT==0
  *pptr = (netwib_ptr) realloc(*pptr, newallocsize);
  if (*pptr == NULL)
    return(NETWIB_ERR_FUREALLOC);
#else
  netwib_er(netwib_debug_memcorrupt_realloc(newallocsize, pptr));
  netwib_er(netwib_debug_memcorrupt_check_periodic());
#endif

#if NETWIB_DEBUG_LEAK!=0
  netwib_er(netwib_debug_leak_add_mem(newallocsize, *pptr));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ptr_free(netwib_ptr *pptr)
{
  if (pptr == NULL) {
    /* we have no sufficient info, but we suppose it will be ok if
       a correct parameter is given*/
    return(NETWIB_ERR_OK);
  }
  if (*pptr == NULL)
    return(NETWIB_ERR_LOOBJCLOSEALREADYCLOSED);

#if NETWIB_DEBUG_LEAK!=0
  netwib_er(netwib_debug_leak_del_mem(*pptr));
#endif

#if NETWIB_DEBUG_MEM_CORRUPT==0
  free(*pptr);
#else
  netwib_er(netwib_debug_memcorrupt_free(pptr));
  netwib_er(netwib_debug_memcorrupt_check_periodic());
#endif

  *pptr = NULL;
  return(NETWIB_ERR_OK);
}
