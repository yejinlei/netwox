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
#define NETWIB_PRIV_ARRAY_MAXBLOCKSIZE 0xFFFF

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 itemsize; /* item size (bytes) rounded to next alignment */
  netwib_uint32 itemsperblock; /* number of item per block */
  netwib_uint32 blocksize; /* itemsize*itemsperblock (bytes) */
  netwib_ptr *blockarray; /* array of blockarraysize blocks */
  netwib_uint32 blockarraysize; /* size (in ptr) of blockarray */
  netwib_uint32 initsize; /* total number of items initialized */
} netwib_array_opaque;

/*-------------------------------------------------------------*/
/* request to add *psizetoadd items. psizetoadd receives really added items */
static netwib_err netwib_priv_array_add(netwib_array *parray,
                                        netwib_uint32 *psizetoadd)
{
  netwib_array_opaque *popaque;
  netwib_ptr pitem, *p;
  netwib_uint32 initsizeinlastblock, sizetoadd;

  popaque = (netwib_array_opaque*)parray->opaque;
  initsizeinlastblock = popaque->initsize % popaque->itemsperblock;

  if (initsizeinlastblock == 0) {
    popaque->blockarraysize++;
    netwib_er(netwib_ptr_realloc(popaque->blockarraysize*sizeof(netwib_ptr),
                                 (netwib_ptr*)&popaque->blockarray));
    netwib_er(netwib_ptr_malloc(popaque->blocksize,
                             &popaque->blockarray[popaque->blockarraysize-1]));
    netwib_er(netwib_ptr_realloc(popaque->blockarraysize*popaque->itemsperblock*sizeof(netwib_ptr),
                                 (netwib_ptr*)&parray->p));
  }

  sizetoadd = *psizetoadd;
  if (sizetoadd > popaque->itemsperblock - initsizeinlastblock) {
    sizetoadd = popaque->itemsperblock - initsizeinlastblock;
  }
  *psizetoadd -= sizetoadd;

  pitem = popaque->blockarray[popaque->blockarraysize-1];
#if NETWIB_DEBUG_MEM_CORRUPT != 0
  if ((netwib_uintptr)pitem % (NETWIBDEF_ARCH_BITS/8)) {
    pitem = (netwib_ptr)(((netwib_uintptr)pitem|(NETWIBDEF_ARCH_BITS/8-1)) +1);
  }
#endif
  pitem = (netwib_data)pitem + initsizeinlastblock*popaque->itemsize;
  p = parray->p + popaque->initsize;

  popaque->initsize += sizetoadd;
  parray->size += sizetoadd;

  while (sizetoadd--) {
    *p = pitem;
    p++;
    pitem = (netwib_data)pitem + popaque->itemsize;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_array_resize(netwib_array *parray,
                                           netwib_uint32 newsize)
{
  netwib_array_opaque *popaque;
  netwib_uint32 sizetoadd;

  popaque = (netwib_array_opaque*)parray->opaque;

  if (newsize <= popaque->initsize) {
    /* shrink, but do not free resources */
    parray->size = newsize;
    return(NETWIB_ERR_OK);
  }

  sizetoadd = newsize - popaque->initsize;
  while(sizetoadd != 0) {
    netwib_er(netwib_priv_array_add(parray, &sizetoadd));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_array_init(netwib_uint32 itemsize,
                             netwib_uint32 initialsize,
                             netwib_array *parray)
{
  netwib_array_opaque *popaque;

  /* parameter verification */
  if (parray == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  if (itemsize == 0) {
    return(NETWIB_ERR_PATOOLOW);
  }

  /* initialize netwib_array */
  netwib_er(netwib_ptr_malloc(1, (netwib_ptr*)&parray->p));
  parray->size = 0;
  netwib_er(netwib_ptr_malloc(sizeof(netwib_array_opaque), &parray->opaque));
  popaque = (netwib_array_opaque*)parray->opaque;

  /* initialize netwib_array_opaque */
  if (itemsize % (NETWIBDEF_ARCH_BITS/8)) {
    itemsize = ( itemsize | (NETWIBDEF_ARCH_BITS/8-1) ) + 1;
  }
  popaque->itemsize = itemsize;
  if (itemsize >= NETWIB_PRIV_ARRAY_MAXBLOCKSIZE) {
    popaque->itemsperblock = 1;
  } else {
    popaque->itemsperblock = NETWIB_PRIV_ARRAY_MAXBLOCKSIZE / itemsize;
  }
  popaque->blocksize = itemsize * popaque->itemsperblock;
#if NETWIB_DEBUG_MEM_CORRUPT != 0
  /* because in this case, memory is not guaranteed to be aligned
     on i386(not strict) processors */
  popaque->blocksize += NETWIBDEF_ARCH_BITS/8;
#endif
  netwib_er(netwib_ptr_malloc(1, (netwib_ptr*)&popaque->blockarray));
  popaque->blockarraysize = 0;
  popaque->initsize = 0;

  /* initialize requested array */
  netwib_er(netwib_priv_array_resize(parray, initialsize));

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_add_array(parray));
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_array_close(netwib_array *parray)
{
  netwib_array_opaque *popaque;
  netwib_uint32 i;

  /* parameter verification */
  if (parray == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_del_array(parray));
#endif

  popaque = (netwib_array_opaque*)parray->opaque;
  for (i = 0; i < popaque->blockarraysize; i++) {
    netwib_er(netwib_ptr_free(&popaque->blockarray[i]));
  }
  netwib_er(netwib_ptr_free((netwib_ptr*)&popaque->blockarray));

  netwib_er(netwib_ptr_free((netwib_ptr*)&parray->p));
  netwib_er(netwib_ptr_free(&parray->opaque));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_array_ctl_set(netwib_array *parray,
                                netwib_array_ctltype type,
                                netwib_ptr p,
                                netwib_uint32 ui)
{

  /* parameter verification */
  if (parray == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_array(parray));
#endif

  switch(type) {
    case NETWIB_ARRAY_CTLTYPE_SIZE:
      netwib_er(netwib_priv_array_resize(parray, ui));
      return(NETWIB_ERR_OK);
    default:
      return(NETWIB_ERR_PAINVALIDTYPE);
      p = p;
      ui = ui;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_array_ctl_get(netwib_array *parray,
                               netwib_array_ctltype type,
                               netwib_ptr p,
                               netwib_uint32 *pui)
{

  /* parameter verification */
  if (parray == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_array(parray));
#endif

  switch(type) {
    case NETWIB_ARRAY_CTLTYPE_SIZE:
      if (pui != NULL) {
        *pui = parray->size;
      }
      break;
    default:
      return(NETWIB_ERR_PAINVALIDTYPE);
      p = p;
  }

  return(NETWIB_ERR_OK);
}
