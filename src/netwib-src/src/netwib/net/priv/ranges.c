/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_ranges_item_cmp(netwib_priv_constranges *pr,
                                              netwib_constdata data1,
                                              netwib_constdata data2,
                                              netwib_cmp *pcmp)
{
  int reti;

  reti = netwib_c_memcmp(data1, data2, pr->itemsize);
  if (reti == 0) {
    *pcmp = NETWIB_CMP_EQ;
  } else if (reti > 0) {
    *pcmp = NETWIB_CMP_GT;
  } else {
    *pcmp = NETWIB_CMP_LT;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_ranges_item_inc(netwib_priv_constranges *pr,
                                              netwib_data data)
{
  netwib_uint32 i;

  i = pr->itemsize - 1;
  while (NETWIB_TRUE) {
    if (data[i] == 0xFF) {
      data[i] = 0;
    } else {
      data[i]++;
      break;
    }
    if (i == 0) {
      return(NETWIB_ERR_LOINTERNALERROR);
    }
    i--;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_ranges_item_dec(netwib_priv_constranges *pr,
                                              netwib_data data)
{
  netwib_uint32 i;

  i = pr->itemsize - 1;
  while (NETWIB_TRUE) {
    if (data[i] == 0) {
      data[i] = 0xFF;
    } else {
      data[i]--;
      break;
    }
    if (i == 0) {
      return(NETWIB_ERR_LOINTERNALERROR);
    }
    i--;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_ranges_item_not(netwib_priv_constranges *pr,
                                              netwib_data data)
{
  netwib_uint32 i;

  for (i = 0; i < pr->itemsize; i++) {
    data[i] = (netwib_byte)~data[i];
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_ranges_item_and(netwib_priv_constranges *pr,
                                              netwib_data datamod,
                                              netwib_constdata data)
{
  netwib_uint32 i;

  for (i = 0; i < pr->itemsize; i++) {
    datamod[i] &= data[i];
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_ranges_item_or(netwib_priv_constranges *pr,
                                             netwib_data datamod,
                                             netwib_constdata data)
{
  netwib_uint32 i;

  for (i = 0; i < pr->itemsize; i++) {
    datamod[i] |= data[i];
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_ranges_item_mask(netwib_priv_constranges *pr,
                                               netwib_uint32 numberbit,
                                               netwib_data data)
{
  netwib_uint32 setbytes, leftbit;
  netwib_byte b = 0;

  if (numberbit > 8*pr->itemsize) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  netwib_c_memset(data, 0, pr->itemsize);
  setbytes = numberbit / 8;
  netwib_c_memset(data, 0xFF, setbytes);
  leftbit = numberbit - 8*setbytes;
  switch(leftbit) {
    case 0 : b = 0x00; break;
    case 1 : b = 0x80; break;
    case 2 : b = 0xC0; break;
    case 3 : b = 0xE0; break;
    case 4 : b = 0xF0; break;
    case 5 : b = 0xF8; break;
    case 6 : b = 0xFC; break;
    case 7 : b = 0xFE; break;
    case 8 : b = 0xFF; break;
  }
  data[setbytes] = b;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* true if "data1+1 == data2" */
static netwib_err netwib_priv_ranges_item_edge(netwib_priv_constranges *pr,
                                               netwib_constdata data1,
                                               netwib_constdata data2,
                                               netwib_bool *pyes)
{
  netwib_byte array[NETWIB_PRIV_RANGES_MAXITEMSIZE];
  netwib_cmp cmp;

  netwib_c_memcpy(array, data1, pr->itemsize);
  netwib_er(netwib_priv_ranges_item_inc(pr, array));
  netwib_er(netwib_priv_ranges_item_cmp(pr, array, data2, &cmp));
  if (cmp == NETWIB_CMP_EQ) {
    *pyes = NETWIB_TRUE;
  } else {
    *pyes = NETWIB_FALSE;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_RANGES_CHECK==1
static netwib_err netwib_priv_ranges_dump(netwib_priv_constranges *pr)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_ext_arrayfilled(pr->ptr,
                                            pr->numranges*pr->rangesize,
                                            &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_HEXA0));
  netwib_er(netwib_fmt_display("\n"));

  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_RANGES_CHECK==1
static netwib_err netwib_priv_ranges_display(netwib_priv_constranges *pr,
                                             netwib_data prange)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_ext_arrayfilled(prange, pr->rangesize, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_HEXA0));
  netwib_er(netwib_fmt_display("\n"));

  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_RANGES_CHECK==1
static netwib_err netwib_debug_ranges_check(netwib_conststring functionname,
                                            netwib_priv_constranges *pr)
{
  netwib_byte array[NETWIB_PRIV_RANGES_MAXITEMSIZE];
  netwib_data pitem;
  netwib_uint32 i;
  netwib_bool arrayset;
  netwib_cmp cmp;

  if (0) {
    netwib_er(netwib_priv_ranges_dump(pr));
    netwib_er(netwib_priv_ranges_display(pr, NULL));
  }

  if (pr->itemsize > NETWIB_PRIV_RANGES_MAXITEMSIZE) {
    netwib_er(netwib_priv_errmsg_func_string(functionname,
                                           "pranges->itemsize too big"));
    return(NETWIB_ERR_LOINTERNALERROR);
  }
  if (pr->rangesize != 2*pr->itemsize) {
    netwib_er(netwib_priv_errmsg_func_string(functionname,
                                           "rangesize != 2*itemsize"));
    return(NETWIB_ERR_LOINTERNALERROR);
  }

  pitem = (netwib_data)pr->ptr;
  arrayset = NETWIB_FALSE;
  for (i = 0 ; i < pr->numranges ; i++ ) {
    netwib_er(netwib_priv_ranges_item_cmp(pr, pitem, pitem+pr->itemsize,
                                          &cmp));
    if (cmp == NETWIB_CMP_GT) {
      netwib_er(netwib_priv_errmsg_func_string(functionname,
                                             "iteminf > itemsup"));
      return(NETWIB_ERR_LOINTERNALERROR);
    }
    if (pr->inittype == NETWIB_PRIV_RANGES_INITTYPE_SORTUNIQ) {
      if (arrayset) {
        netwib_er(netwib_priv_ranges_item_cmp(pr, array, pitem, &cmp));
        if (cmp != NETWIB_CMP_LT) {
          netwib_er(netwib_priv_errmsg_func_string(functionname,
                                              "lastitemtop >= currentbottom"));
          return(NETWIB_ERR_LOINTERNALERROR);
        }
      }
      netwib_c_memcpy(array, pitem+pr->itemsize, pr->itemsize);
      arrayset = NETWIB_TRUE;
    }
    pitem += pr->rangesize;
  }

  return(NETWIB_ERR_OK);
}
 #define netwib__debug_ranges_check(a,b) netwib_er(netwib_debug_ranges_check(a,b))
#else
 #define netwib__debug_ranges_check(a,b) {}
#endif

/*-------------------------------------------------------------*/
/* Search an item in a sorted list, stopping before found range.
   We start searching at pstartrange range.
   We stop before range containing item (pinthisrange set), or
   before superior range (pinthisrange unset).
   If item is not in list, point at the end.
*/
static netwib_err netwib_priv_ranges_search_linear_sort(netwib_priv_constranges *pr,
                                                 netwib_constdata pstartrange,
                                                 netwib_constdata searcheditem,
                                                 netwib_uint32 *pfoundindex,
                                                 netwib_data *pfoundrange,
                                                 netwib_bool *pinthisrange)
{
  netwib_data pitem;
  netwib_uint32 i, startindex;
  netwib_cmp cmp;

  startindex = (pstartrange - (netwib_data)pr->ptr) / pr->rangesize;
  pitem = (netwib_data)pr->ptr + startindex * pr->rangesize;
  for (i = startindex ; i < pr->numranges ; i++ ) {
    netwib_er(netwib_priv_ranges_item_cmp(pr, searcheditem, pitem, &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      *pfoundindex = i;
      *pfoundrange = pitem;
      *pinthisrange = NETWIB_TRUE;
      return(NETWIB_ERR_OK);
    } else if (cmp == NETWIB_CMP_LT) {
      *pfoundindex = i;
      *pfoundrange = pitem;
      *pinthisrange = NETWIB_FALSE;
      return(NETWIB_ERR_OK);
    }
    pitem += pr->itemsize;
    netwib_er(netwib_priv_ranges_item_cmp(pr, searcheditem, pitem, &cmp));
    if (cmp != NETWIB_CMP_GT) {
      *pfoundindex = i;
      *pfoundrange = pitem - pr->itemsize;
      *pinthisrange = NETWIB_TRUE;
      return(NETWIB_ERR_OK);
    }
    pitem += pr->itemsize;
  }

  *pfoundindex = pr->numranges;
  *pfoundrange = pitem;
  *pinthisrange = NETWIB_FALSE;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* Search an item in an unsorted list, stopping before found range.
   We start searching at pstartrange range.
   We stop before range containing item (pinthisrange set), or
   before end of list (pinthisrange unset = item is not in list).
*/
static netwib_err netwib_priv_ranges_search_linear_notsort(netwib_priv_constranges *pr,
                                                 netwib_constdata pstartrange,
                                                 netwib_constdata searcheditem,
                                                 netwib_uint32 *pfoundindex,
                                                 netwib_data *pfoundrange,
                                                 netwib_bool *pinthisrange)
{
  netwib_data pitem;
  netwib_uint32 i, startindex;
  netwib_cmp cmp;

  startindex = (pstartrange - (netwib_data)pr->ptr) / pr->rangesize;
  pitem = (netwib_data)pr->ptr + startindex * pr->rangesize;
  for (i = startindex ; i < pr->numranges ; i++ ) {
    netwib_er(netwib_priv_ranges_item_cmp(pr, searcheditem, pitem, &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      *pfoundindex = i;
      *pfoundrange = pitem;
      *pinthisrange = NETWIB_TRUE;
      return(NETWIB_ERR_OK);
    } else if (cmp == NETWIB_CMP_LT) {
      pitem += pr->rangesize;
      continue;
    }
    pitem += pr->itemsize;
    netwib_er(netwib_priv_ranges_item_cmp(pr, searcheditem, pitem, &cmp));
    if (cmp != NETWIB_CMP_GT) {
      *pfoundindex = i;
      *pfoundrange = pitem - pr->itemsize;
      *pinthisrange = NETWIB_TRUE;
      return(NETWIB_ERR_OK);
    }
    pitem += pr->itemsize;
  }

  *pfoundindex = pr->numranges;
  *pfoundrange = pitem;
  *pinthisrange = NETWIB_FALSE;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* Search an item in a sorted list, stopping before found range.
   We start searching at pstartrange range.
   We stop before range containing item (pinthisrange set), or
   before superior range (pinthisrange unset).
   If item is not in list, point at the end.
*/
static netwib_err netwib_priv_ranges_search_dichotomy(netwib_priv_constranges *pr,
                                                netwib_constdata psearcheditem,
                                                netwib_uint32 *pfoundindex,
                                                netwib_data *pfoundrange,
                                                netwib_bool *pinthisrange)
{
  netwib_data pitem;
  netwib_uint32 indexlow, indexhigh, indextry;
  netwib_cmp cmp1, cmp2;

  /* equivalent to following, but faster
     netwib_er(netwib_priv_ranges_search_linear_sort(pr, pr->ptr,
                                                     psearcheditem,
                                                     pfoundindex,
                                                     pfoundrange,
                                                     pinthisrange));
  */
  indexlow = 0;
  indexhigh = pr->numranges;
  while (NETWIB_TRUE) {
    if (indexlow == indexhigh) {
      /* just before indexhigh */
      *pfoundindex = indexhigh;
      *pfoundrange = (netwib_data)pr->ptr + *pfoundindex * pr->rangesize;
      *pinthisrange = NETWIB_FALSE;
      return(NETWIB_ERR_OK);
    }
    /* decide at wich index to look */
    indextry = indexlow + (indexhigh - indexlow) / 2;
    /* look */
    pitem = (netwib_data)pr->ptr + indextry * pr->rangesize;
    netwib_er(netwib_priv_ranges_item_cmp(pr, psearcheditem, pitem, &cmp1));
    if (cmp1 == NETWIB_CMP_EQ) {
      /* in the range */
      *pfoundindex = indextry;
      *pfoundrange = (netwib_data)pr->ptr + *pfoundindex * pr->rangesize;
      *pinthisrange = NETWIB_TRUE;
      return(NETWIB_ERR_OK);
    }
    netwib_er(netwib_priv_ranges_item_cmp(pr, psearcheditem, pitem+pr->itemsize, &cmp2));
    if (cmp1 != NETWIB_CMP_LT && cmp2 != NETWIB_CMP_GT) {
      /* in the range */
      *pfoundindex = indextry;
      *pfoundrange = (netwib_data)pr->ptr + *pfoundindex * pr->rangesize;
      *pinthisrange = NETWIB_TRUE;
      return(NETWIB_ERR_OK);
    }
    /* shrink for next try */
    if (cmp1 == NETWIB_CMP_LT) {
      indexhigh = indextry;
    } else {
      indexlow = indextry + 1;
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* Warning : this function has to be called before searching for items,
   otherwise, if memory is reallocated, the found addresses are not valid
   anymore */
#define NETWIB_PRIV_RANGE_ALLOC_STEP 1024
static netwib_err netwib_priv_ranges_eventuallyrealloc(netwib_priv_ranges *pr)
{
  if (pr->numranges*pr->rangesize + 2*NETWIB_PRIV_RANGES_MAXITEMSIZE >
      pr->ptrallocsize) {
    pr->ptrallocsize += NETWIB_PRIV_RANGE_ALLOC_STEP;
    netwib_er(netwib_ptr_realloc(pr->ptrallocsize, (netwib_ptr*)&pr->ptr));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ranges_init(netwib_priv_ranges_inittype inittype,
                                   netwib_uint32 itemsize,
                                   netwib_priv_ranges *pr)
{
  pr->inittype = inittype;
  pr->itemsize = itemsize;
  pr->rangesize = 2 * itemsize;
  pr->ptrallocsize = NETWIB_PRIV_RANGE_ALLOC_STEP;
  pr->numranges = 0;
  netwib_er(netwib_ptr_malloc(pr->ptrallocsize, (netwib_ptr*)&pr->ptr));

  netwib__debug_ranges_check("netwib_priv_ranges_init", pr);
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ranges_close(netwib_priv_ranges *pr)
{
  netwib__debug_ranges_check("netwib_priv_ranges_close", pr);
  netwib_er(netwib_ptr_free((netwib_ptr*)&pr->ptr));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_ranges_add_sort(netwib_priv_ranges *pr,
                                              netwib_constdata ptr)
{
  netwib_uint32 foundindex;
  netwib_data foundrange;
  netwib_bool inthisrange, edge1, edge2;

  netwib_er(netwib_priv_ranges_search_dichotomy(pr, ptr, &foundindex,
                                                &foundrange, &inthisrange));

  if (inthisrange) {
    /* already inside */
    return(NETWIB_ERR_OK);
  }

  /* first, check if it corresponds to edges */
  edge1 = NETWIB_FALSE;
  edge2 = NETWIB_FALSE;
  if (foundindex != 0) {
    netwib_er(netwib_priv_ranges_item_edge(pr, foundrange-pr->itemsize, ptr,
                                           &edge1));
  }
  if (foundindex != pr->numranges) {
    netwib_er(netwib_priv_ranges_item_edge(pr, ptr, foundrange, &edge2));
  }

  /* treat each case */
  if (edge1 && edge2) {
    /* we have to merge two ranges */
    netwib_c_memmove(foundrange-pr->itemsize, foundrange+pr->itemsize,
                     (pr->numranges-foundindex)*pr->rangesize-pr->itemsize);
    pr->numranges--;
  } else if (edge1) {
    /* extend previous range */
    netwib_c_memcpy(foundrange-pr->itemsize, ptr, pr->itemsize);
  } else if (edge2) {
    /* extend current range */
    netwib_c_memcpy(foundrange, ptr, pr->itemsize);
  } else {
    /* prepend it */
    /* note : memory has been reallocated in netwib_priv_ranges_add() */
    netwib_c_memmove(foundrange+pr->rangesize, foundrange,
                    (pr->numranges-foundindex)*pr->rangesize);
    netwib_c_memcpy(foundrange, ptr, pr->itemsize);
    netwib_c_memcpy(foundrange+pr->itemsize, ptr, pr->itemsize);
    pr->numranges++;
  }

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_ranges_add_notsort(netwib_priv_ranges *pr,
                                                 netwib_constdata ptr)
{
  netwib_uint32 foundindex;
  netwib_data foundrange, pitem;
  netwib_bool inthisrange;

  if (pr->inittype == NETWIB_PRIV_RANGES_INITTYPE_NOTSORTUNIQ) {
    netwib_er(netwib_priv_ranges_search_linear_notsort(pr, pr->ptr, ptr,
                                                       &foundindex,
                                                       &foundrange,
                                                       &inthisrange));
    if (inthisrange) {
      /* already inside */
      return(NETWIB_ERR_OK);
    }
    /* do not try to extend a previous range (at edge) */
  }

  /* append at end */
  pitem = (netwib_data)pr->ptr + pr->numranges * pr->rangesize;
  /* note : memory has been reallocated in netwib_priv_ranges_add() */
  netwib_c_memcpy(pitem, ptr, pr->itemsize);
  netwib_c_memcpy(pitem+pr->itemsize, ptr, pr->itemsize);
  pr->numranges++;

  return(NETWIB_ERR_OK);
}
netwib_err netwib_priv_ranges_add(netwib_priv_ranges *pr,
                                  netwib_constdata ptr)
{

  netwib__debug_ranges_check("netwib_priv_ranges_add", pr);

  /* always start by reallocating: see comment before eventuallyrealloc */
  netwib_er(netwib_priv_ranges_eventuallyrealloc(pr));

  if (pr->inittype == NETWIB_PRIV_RANGES_INITTYPE_SORTUNIQ) {
    netwib_er(netwib_priv_ranges_add_sort(pr, ptr));
  } else {
    netwib_er(netwib_priv_ranges_add_notsort(pr, ptr));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_ranges_add_range_sort(netwib_priv_ranges *pr,
                                                    netwib_constdata ptrinf,
                                                    netwib_constdata ptrsup)
{
  netwib_uint32 inffoundindex, supfoundindex;
  netwib_data inffoundrange, supfoundrange, srcpos, dstpos;
  netwib_bool infinthisrange, supinthisrange, edge;
  netwib_uint32 numrangestoremove;
  netwib_cmp cmp;

  netwib_er(netwib_priv_ranges_item_cmp(pr, ptrinf, ptrsup, &cmp));
  if (cmp == NETWIB_CMP_GT) {
    return(NETWIB_ERR_PAINVALIDRANGE);
  }

  netwib_er(netwib_priv_ranges_search_dichotomy(pr, ptrinf,
                                                &inffoundindex,
                                                &inffoundrange,
                                                &infinthisrange));
  netwib_er(netwib_priv_ranges_search_linear_sort(pr, inffoundrange, ptrsup,
                                                  &supfoundindex,
                                                  &supfoundrange,
                                                  &supinthisrange));

  /* entries at edge can be considered as inside the range */
  if (!infinthisrange) {
    if (inffoundindex) {
      netwib_er(netwib_priv_ranges_item_edge(pr, inffoundrange-pr->itemsize,
                                             ptrinf, &edge));
      if (edge) {
        inffoundindex--;
        inffoundrange -= pr->rangesize;
        infinthisrange = NETWIB_TRUE;
      }
    }
  }
  if (!supinthisrange) {
    if (supfoundindex < pr->numranges) {
      netwib_er(netwib_priv_ranges_item_edge(pr, ptrsup, supfoundrange,
                                             &edge));
      if (edge) {
        supinthisrange = NETWIB_TRUE;
      }
    }
  }

  /* treat various cases */
  if (infinthisrange && supinthisrange) {
    /* items are already inside, so only have to remove intermediary items */
    srcpos = supfoundrange + pr->itemsize;
    dstpos = inffoundrange + pr->itemsize;
  } else if (infinthisrange) {
    netwib_c_memcpy(supfoundrange-pr->itemsize, ptrsup, pr->itemsize);
    srcpos = supfoundrange - pr->itemsize;
    dstpos = inffoundrange + pr->itemsize;
  } else if (supinthisrange) {
    netwib_c_memcpy(inffoundrange, ptrinf, pr->itemsize);
    dstpos = inffoundrange + pr->itemsize;
    srcpos = supfoundrange + pr->itemsize;
  } else {
    if (inffoundindex == supfoundindex) {
      /* we need to insert one range */
      /* note : memory has been reallocated in netwib_priv_ranges_add_range()*/
      netwib_c_memmove(inffoundrange+pr->rangesize, inffoundrange,
                      (pr->numranges-inffoundindex)*pr->rangesize);
      netwib_c_memcpy(inffoundrange, ptrinf, pr->itemsize);
      netwib_c_memcpy(inffoundrange+pr->itemsize, ptrsup, pr->itemsize);
      pr->numranges++;
      srcpos = NULL; /* not needed */
      dstpos = NULL; /* not needed */
    } else {
      netwib_c_memcpy(inffoundrange, ptrinf, pr->itemsize);
      dstpos = inffoundrange + pr->itemsize;
      netwib_c_memcpy(supfoundrange-pr->itemsize, ptrsup, pr->itemsize);
      srcpos = supfoundrange - pr->itemsize;
    }
  }

  /* now, move memory */
  if (srcpos < dstpos) {
    return(NETWIB_ERR_LOINTERNALERROR);
  }
  if (srcpos != dstpos) {
    numrangestoremove = (srcpos - dstpos) / pr->rangesize;
    netwib_c_memmove(dstpos, srcpos,
                    (netwib_data)pr->ptr+pr->numranges*pr->rangesize - srcpos);
    pr->numranges -= numrangestoremove;
  }

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_ranges_add_range_notsort(netwib_priv_ranges *pr,
                                                       netwib_constdata ptrinf,
                                                       netwib_constdata ptrsup)
{
  netwib_data pitem;

  if (pr->inittype == NETWIB_PRIV_RANGES_INITTYPE_NOTSORTUNIQ) {
    /* remove previous if any */
    netwib_er(netwib_priv_ranges_del_range(pr, ptrinf, ptrsup));
  }

  /* append at end */
  pitem = (netwib_data)pr->ptr + pr->numranges * pr->rangesize;
  /* note : memory has been reallocated in netwib_priv_ranges_add_range()*/
  netwib_c_memcpy(pitem, ptrinf, pr->itemsize);
  netwib_c_memcpy(pitem+pr->itemsize, ptrsup, pr->itemsize);
  pr->numranges++;

  return(NETWIB_ERR_OK);
}
netwib_err netwib_priv_ranges_add_range(netwib_priv_ranges *pr,
                                        netwib_constdata ptrinf,
                                        netwib_constdata ptrsup)
{

  netwib__debug_ranges_check("netwib_priv_ranges_add_range", pr);

  /* always start by reallocating: see comment before eventuallyrealloc */
  netwib_er(netwib_priv_ranges_eventuallyrealloc(pr));

  if (pr->inittype == NETWIB_PRIV_RANGES_INITTYPE_SORTUNIQ) {
    netwib_er(netwib_priv_ranges_add_range_sort(pr, ptrinf, ptrsup));
  } else {
    netwib_er(netwib_priv_ranges_add_range_notsort(pr, ptrinf, ptrsup));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ranges_add_all(netwib_priv_ranges *pr)
{

  netwib__debug_ranges_check("netwib_priv_ranges_add_all", pr)
  pr->numranges = 1;
  netwib_c_memset(pr->ptr, 0, pr->itemsize);
  netwib_c_memset((netwib_data)pr->ptr+pr->itemsize, 0xFF, pr->itemsize);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_ranges_del_item(netwib_priv_ranges *pr,
                                              netwib_constdata itemtodel,
                                              netwib_uint32 itemindex,
                                              netwib_data itemrange)
{
  netwib_bool edgeinf, edgesup;
  netwib_cmp cmp;

  /* check if item is at the edge */
  edgeinf = NETWIB_FALSE;
  edgesup = NETWIB_FALSE;
  netwib_er(netwib_priv_ranges_item_cmp(pr, itemrange, itemtodel, &cmp));
  if (cmp == NETWIB_CMP_EQ) {
    edgeinf = NETWIB_TRUE;
  }
  netwib_er(netwib_priv_ranges_item_cmp(pr, itemrange+pr->itemsize, itemtodel,
                                        &cmp));
  if (cmp == NETWIB_CMP_EQ) {
    edgesup = NETWIB_TRUE;
  }

  /* treat each case */
  if (edgeinf && edgesup) {
    /* remove the range */
    netwib_c_memmove(itemrange, itemrange+pr->rangesize,
                     (pr->numranges-itemindex+1)*pr->rangesize);
    pr->numranges--;
    return(NETWIB_ERR_OK);
  } else if (edgeinf) {
    netwib_er(netwib_priv_ranges_item_inc(pr, itemrange));
    return(NETWIB_ERR_OK);
  } else if (edgesup) {
    netwib_er(netwib_priv_ranges_item_dec(pr, itemrange+pr->itemsize));
    return(NETWIB_ERR_OK);
  } else {
    /* cut the range in two */
    /* note : memory has been reallocated in netwib_priv_ranges_del() */
    netwib_c_memmove(itemrange+pr->rangesize, itemrange,
                     (pr->numranges-itemindex)*pr->rangesize);
    netwib_c_memcpy(itemrange, itemrange+pr->rangesize, pr->itemsize);
    netwib_c_memcpy(itemrange+pr->itemsize, itemtodel, pr->itemsize);
    netwib_c_memcpy(itemrange+pr->rangesize, itemtodel, pr->itemsize);
    netwib_er(netwib_priv_ranges_item_dec(pr, itemrange+pr->itemsize));
    netwib_er(netwib_priv_ranges_item_inc(pr, itemrange+pr->rangesize));
    pr->numranges++;
  }

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_ranges_del_sort(netwib_priv_ranges *pr,
                                              netwib_constdata ptr)
{
  netwib_uint32 foundindex;
  netwib_data foundrange;
  netwib_bool inthisrange;

  netwib_er(netwib_priv_ranges_search_dichotomy(pr, ptr, &foundindex,
                                                &foundrange, &inthisrange));

  if (!inthisrange) {
    /* already outside */
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_priv_ranges_del_item(pr, ptr, foundindex, foundrange));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_ranges_del_notsort(netwib_priv_ranges *pr,
                                                 netwib_constdata ptr)
{
  netwib_uint32 foundindex;
  netwib_data foundrange, prange;
  netwib_bool inthisrange;

  prange = pr->ptr;
  while (NETWIB_TRUE) {
    netwib_er(netwib_priv_ranges_search_linear_notsort(pr, prange, ptr,
                                                       &foundindex,
                                                       &foundrange,
                                                       &inthisrange));
    if (!inthisrange) {
      break;
    }
    netwib_er(netwib_priv_ranges_del_item(pr, ptr, foundindex, foundrange));
    prange = (netwib_data)pr->ptr + foundindex*pr->rangesize;
  }

  return(NETWIB_ERR_OK);
}
netwib_err netwib_priv_ranges_del(netwib_priv_ranges *pr,
                                  netwib_constdata ptr)
{

  netwib__debug_ranges_check("netwib_priv_ranges_del", pr);

  /* always start by reallocating: see comment before eventuallyrealloc */
  netwib_er(netwib_priv_ranges_eventuallyrealloc(pr));

  if (pr->inittype == NETWIB_PRIV_RANGES_INITTYPE_SORTUNIQ) {
    netwib_er(netwib_priv_ranges_del_sort(pr, ptr));
  } else {
    netwib_er(netwib_priv_ranges_del_notsort(pr, ptr));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_ranges_del_items(netwib_priv_ranges *pr,
                                               netwib_constdata iteminftodel,
                                               netwib_uint32 iteminfindex,
                                               netwib_data iteminfrange,
                                               netwib_bool infinthisrange,
                                               netwib_constdata itemsuptodel,
                                               netwib_uint32 itemsupindex,
                                               netwib_data itemsuprange,
                                               netwib_bool supinthisrange)
{
  netwib_data srcpos, dstpos;
  netwib_uint32 numrangestoremove;
  netwib_cmp cmp;

  /* if inf is on lower edge, it's the same as suppressing range */
  if (infinthisrange) {
    netwib_er(netwib_priv_ranges_item_cmp(pr, iteminftodel, iteminfrange,
                                          &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      infinthisrange = NETWIB_FALSE;
    }
  }
  /* if sup is on upper edge, it's the same as suppressing range */
  if (supinthisrange) {
    netwib_er(netwib_priv_ranges_item_cmp(pr, itemsuptodel,
                                          itemsuprange+pr->itemsize, &cmp));
    if (cmp == NETWIB_CMP_EQ) {
      supinthisrange = NETWIB_FALSE;
      itemsupindex++;
      itemsuprange += pr->rangesize;
    }
  }

  /* treat various cases */
  if (infinthisrange && supinthisrange) {
    if (iteminfindex == itemsupindex) {
      /* we need to insert one range */
      /* note : memory has been reallocated in netwib_priv_ranges_del_range()*/
      netwib_c_memmove(iteminfrange+pr->rangesize+pr->itemsize,
                       iteminfrange+pr->itemsize,
                       (pr->numranges-iteminfindex)*pr->rangesize);
      netwib_c_memcpy(iteminfrange+pr->itemsize, iteminftodel, pr->itemsize);
      netwib_er(netwib_priv_ranges_item_dec(pr, iteminfrange+pr->itemsize));
      netwib_c_memcpy(iteminfrange+pr->rangesize, itemsuptodel, pr->itemsize);
      netwib_er(netwib_priv_ranges_item_inc(pr, iteminfrange+pr->rangesize));
      pr->numranges++;
      srcpos = NULL; /* not needed */
      dstpos = NULL; /* not needed */
    } else {
      netwib_c_memcpy(iteminfrange+pr->itemsize, iteminftodel, pr->itemsize);
      netwib_er(netwib_priv_ranges_item_dec(pr, iteminfrange+pr->itemsize));
      netwib_c_memcpy(itemsuprange, itemsuptodel, pr->itemsize);
      netwib_er(netwib_priv_ranges_item_inc(pr, itemsuprange));
      srcpos = itemsuprange;
      dstpos = iteminfrange + pr->rangesize;
    }
  } else if (infinthisrange) {
    netwib_c_memcpy(iteminfrange+pr->itemsize, iteminftodel, pr->itemsize);
    netwib_er(netwib_priv_ranges_item_dec(pr, iteminfrange+pr->itemsize));
    srcpos = itemsuprange;
    dstpos = iteminfrange + pr->rangesize;
  } else if (supinthisrange) {
    netwib_c_memcpy(itemsuprange, itemsuptodel, pr->itemsize);
    netwib_er(netwib_priv_ranges_item_inc(pr, itemsuprange));
    srcpos = itemsuprange;
    dstpos = iteminfrange;
  } else {
    srcpos = itemsuprange;
    dstpos = iteminfrange;
  }

  /* now, move memory */
  if (srcpos < dstpos) {
    return(NETWIB_ERR_LOINTERNALERROR);
  }
  if (srcpos != dstpos) {
    numrangestoremove = (srcpos - dstpos) / pr->rangesize;
    netwib_c_memmove(dstpos, srcpos,
                    (netwib_data)pr->ptr+pr->numranges*pr->rangesize - srcpos);
    pr->numranges -= numrangestoremove;
  }

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_ranges_del_range_sort(netwib_priv_ranges *pr,
                                                    netwib_constdata ptrinf,
                                                    netwib_constdata ptrsup)
{
  netwib_uint32 inffoundindex, supfoundindex;
  netwib_data inffoundrange, supfoundrange;
  netwib_bool infinthisrange, supinthisrange;

  netwib_er(netwib_priv_ranges_search_dichotomy(pr, ptrinf,
                                                &inffoundindex,
                                                &inffoundrange,
                                                &infinthisrange));
  netwib_er(netwib_priv_ranges_search_linear_sort(pr, inffoundrange, ptrsup,
                                                  &supfoundindex,
                                                  &supfoundrange,
                                                  &supinthisrange));

  netwib_er(netwib_priv_ranges_del_items(pr, ptrinf, inffoundindex,
                                         inffoundrange, infinthisrange,
                                         ptrsup, supfoundindex,
                                         supfoundrange, supinthisrange));

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_ranges_del_range_notsort(netwib_priv_ranges *pr,
                                                       netwib_constdata ptrinf,
                                                       netwib_constdata ptrsup)
{
  netwib_data prange, prangesup;
  netwib_bool infinthisrange, supinthisrange;
  netwib_uint32 i;
  netwib_cmp cmp;

  prange = (netwib_data)pr->ptr;
  i = 0;
  while (i < pr->numranges) {
    /* if inf is above rangeend, skip it */
    netwib_er(netwib_priv_ranges_item_cmp(pr, ptrinf, prange+pr->itemsize,
                                          &cmp));
    if (cmp == NETWIB_CMP_GT) {
      prange += pr->rangesize;
      i++;
      continue;
    }
    /* if sup is below rangebegin, skip it */
    netwib_er(netwib_priv_ranges_item_cmp(pr, ptrsup, prange, &cmp));
    if (cmp == NETWIB_CMP_LT) {
      prange += pr->rangesize;
      i++;
      continue;
    }
    /* check if inf/sup is in range */
    infinthisrange = NETWIB_TRUE;
    netwib_er(netwib_priv_ranges_item_cmp(pr, ptrinf, prange, &cmp));
    if (cmp == NETWIB_CMP_LT) {
      infinthisrange = NETWIB_FALSE;
    }
    supinthisrange = NETWIB_TRUE;
    prangesup = prange;
    netwib_er(netwib_priv_ranges_item_cmp(pr, ptrsup, prange+pr->itemsize,
                                          &cmp));
    if (cmp == NETWIB_CMP_GT) {
      supinthisrange = NETWIB_FALSE;
      /* del_items function is shared with del_range_sort, which expect
         prangesup to be after range to suppress */
      prangesup += pr->rangesize;
    }
    /* del this part */
    netwib_er(netwib_priv_ranges_del_items(pr, ptrinf, i, prange,
                                           infinthisrange, ptrsup, i,
                                           prangesup, supinthisrange));
    prange = (netwib_data)pr->ptr + i*pr->rangesize;
    /* don't increment i or prange, because del_items might have deleted
       a range and shifted to left */
  }

  return(NETWIB_ERR_OK);
}
netwib_err netwib_priv_ranges_del_range(netwib_priv_ranges *pr,
                                        netwib_constdata ptrinf,
                                        netwib_constdata ptrsup)
{
  netwib_cmp cmp;

  netwib__debug_ranges_check("netwib_priv_ranges_del_range", pr);

  /* always start by reallocating: see comment before eventuallyrealloc */
  netwib_er(netwib_priv_ranges_eventuallyrealloc(pr));

  netwib_er(netwib_priv_ranges_item_cmp(pr, ptrinf, ptrsup, &cmp));
  if (cmp == NETWIB_CMP_GT) {
    return(NETWIB_ERR_PAINVALIDRANGE);
  }

  if (pr->inittype == NETWIB_PRIV_RANGES_INITTYPE_SORTUNIQ) {
    netwib_er(netwib_priv_ranges_del_range_sort(pr, ptrinf, ptrsup));
  } else {
    netwib_er(netwib_priv_ranges_del_range_notsort(pr, ptrinf, ptrsup));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ranges_del_all(netwib_priv_ranges *pr)
{
  netwib__debug_ranges_check("netwib_priv_ranges_del_all", pr)
  pr->numranges = 0;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ranges_contains(netwib_priv_constranges *pr,
                                       netwib_constdata ptr,
                                       netwib_bool *pyes)
{
  netwib_uint32 foundindex;
  netwib_data foundrange;

  netwib__debug_ranges_check("netwib_priv_ranges_contains", pr)

  if (pr->inittype == NETWIB_PRIV_RANGES_INITTYPE_SORTUNIQ) {
    netwib_er(netwib_priv_ranges_search_dichotomy(pr, ptr, &foundindex,
                                                  &foundrange, pyes));
  } else {
    netwib_er(netwib_priv_ranges_search_linear_notsort(pr, pr->ptr, ptr,
                                                      &foundindex, &foundrange,
                                                      pyes));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_ranges_contains_range_sort(netwib_priv_constranges *pr,
                                                      netwib_constdata ptrinf,
                                                      netwib_constdata ptrsup,
                                                      netwib_bool *pyes)
{
  netwib_uint32 inffoundindex, supfoundindex;
  netwib_data inffoundrange, supfoundrange;
  netwib_bool infinthisrange, supinthisrange;

  netwib_er(netwib_priv_ranges_search_dichotomy(pr, ptrinf,
                                                &inffoundindex,
                                                &inffoundrange,
                                                &infinthisrange));
  if (!infinthisrange) {
    *pyes = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_priv_ranges_search_linear_sort(pr, inffoundrange, ptrsup,
                                                  &supfoundindex,
                                                  &supfoundrange,
                                                  &supinthisrange));
  if (supinthisrange &&
      inffoundindex == supfoundindex) {
    *pyes = NETWIB_TRUE;
  } else {
    *pyes = NETWIB_FALSE;
  }

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_ranges_contains_range_notsort(netwib_priv_constranges *pr,
                                                    netwib_constdata ptrinf,
                                                    netwib_constdata ptrsup,
                                                    netwib_bool *pyes)
{
  netwib_byte searched[NETWIB_PRIV_RANGES_MAXITEMSIZE];
  netwib_data foundrange;
  netwib_uint32 foundindex;
  netwib_bool found;
  netwib_cmp cmp;

  netwib_c_memcpy(searched, ptrinf, pr->itemsize);
  while(NETWIB_TRUE) {
    /* search bottom */
    netwib_er(netwib_priv_ranges_search_linear_notsort(pr, pr->ptr, searched,
                                                       &foundindex,
                                                       &foundrange,
                                                       &found));
    if (!found) {
      *pyes = NETWIB_FALSE;
      break;
    }
    /* decide which one needs to be searched next */
    netwib_c_memcpy(searched, foundrange+pr->itemsize, pr->itemsize);
    netwib_er(netwib_priv_ranges_item_inc(pr, searched));
    /* check if end has been reached */
    netwib_er(netwib_priv_ranges_item_cmp(pr, searched, ptrsup, &cmp));
    if (cmp == NETWIB_CMP_GT) {
      *pyes = NETWIB_TRUE;
      break;
    }
  }

  return(NETWIB_ERR_OK);
}
netwib_err netwib_priv_ranges_contains_range(netwib_priv_constranges *pr,
                                             netwib_constdata ptrinf,
                                             netwib_constdata ptrsup,
                                             netwib_bool *pyes)
{
  netwib_cmp cmp;

  netwib__debug_ranges_check("netwib_priv_ranges_contains_range", pr)

  netwib_er(netwib_priv_ranges_item_cmp(pr, ptrinf, ptrsup, &cmp));
  if (cmp == NETWIB_CMP_GT) {
    return(NETWIB_ERR_PAINVALIDRANGE);
  }

  if (pr->inittype == NETWIB_PRIV_RANGES_INITTYPE_SORTUNIQ) {
    netwib_er(netwib_priv_ranges_contains_range_sort(pr, ptrinf, ptrsup,
                                                     pyes));
  } else {
    netwib_er(netwib_priv_ranges_contains_range_notsort(pr, ptrinf, ptrsup,
                                                       pyes));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_ranges_add_buf2(netwib_priv_ranges *pr,
                                              netwib_data array,
                                              netwib_priv_ranges_separator
                                              separator,
                                              netwib_data ptrinf,
                                              netwib_data ptrsup)
{
  netwib_byte tmparray[NETWIB_PRIV_RANGES_MAXITEMSIZE];
  netwib_uint32 num;

  switch(separator) {
    case NETWIB_PRIV_RANGES_SEPARATOR_NONE :
      netwib_c_memcpy(ptrinf, array, pr->itemsize);
      netwib_c_memcpy(ptrsup, array, pr->itemsize);
      break;
    case NETWIB_PRIV_RANGES_SEPARATOR_DASH :
      netwib_c_memcpy(ptrinf, array, pr->itemsize);
      netwib_c_memcpy(ptrsup, array+pr->itemsize, pr->itemsize);
      break;
    case NETWIB_PRIV_RANGES_SEPARATOR_SLASH :
      netwib_c_memcpy(ptrinf, array, pr->itemsize);
      netwib_c_memcpy(ptrsup, array, pr->itemsize);
      netwib_c_memcpy(tmparray, array+pr->itemsize, pr->itemsize);
      netwib_er(netwib_priv_ranges_item_and(pr, ptrinf, tmparray));
      netwib_er(netwib_priv_ranges_item_not(pr, tmparray));
      netwib_er(netwib_priv_ranges_item_or(pr, ptrsup, tmparray));
      break;
    case NETWIB_PRIV_RANGES_SEPARATOR_PERCENT :
      netwib_c_memcpy(ptrinf, array, pr->itemsize);
      netwib_c_memcpy(ptrsup, array, pr->itemsize);
      netwib_c_memcpy(tmparray, array+pr->itemsize, pr->itemsize);
      netwib_er(netwib_priv_ranges_item_and(pr, ptrinf, tmparray));
      netwib_er(netwib_priv_ranges_item_not(pr, tmparray));
      netwib_er(netwib_priv_ranges_item_or(pr, ptrsup, tmparray));
      netwib_er(netwib_priv_ranges_item_inc(pr, ptrinf));
      netwib_er(netwib_priv_ranges_item_dec(pr, ptrsup));
      break;
    case NETWIB_PRIV_RANGES_SEPARATOR_SLASH_NUM :
      num = (array+pr->itemsize)[0];
      netwib_er(netwib_priv_ranges_item_mask(pr, num, tmparray));
      netwib_c_memcpy(ptrinf, array, pr->itemsize);
      netwib_c_memcpy(ptrsup, array, pr->itemsize);
      netwib_er(netwib_priv_ranges_item_and(pr, ptrinf, tmparray));
      netwib_er(netwib_priv_ranges_item_not(pr, tmparray));
      netwib_er(netwib_priv_ranges_item_or(pr, ptrsup, tmparray));
      break;
    case NETWIB_PRIV_RANGES_SEPARATOR_PERCENT_NUM :
      num = (array+pr->itemsize)[0];
      netwib_er(netwib_priv_ranges_item_mask(pr, num, tmparray));
      netwib_c_memcpy(ptrinf, array, pr->itemsize);
      netwib_c_memcpy(ptrsup, array, pr->itemsize);
      netwib_er(netwib_priv_ranges_item_and(pr, ptrinf, tmparray));
      netwib_er(netwib_priv_ranges_item_not(pr, tmparray));
      netwib_er(netwib_priv_ranges_item_or(pr, ptrsup, tmparray));
      netwib_er(netwib_priv_ranges_item_inc(pr, ptrinf));
      netwib_er(netwib_priv_ranges_item_dec(pr, ptrsup));
      break;
  }

  return(NETWIB_ERR_OK);
}
netwib_err netwib_priv_ranges_add_buf(netwib_priv_ranges *pr,
                                      netwib_constbuf *pbuf,
                                      netwib_priv_ranges_add_buf_pf pfunc)
{
  netwib_byte infarray[NETWIB_PRIV_RANGES_MAXITEMSIZE];
  netwib_byte suparray[NETWIB_PRIV_RANGES_MAXITEMSIZE];
  netwib_byte array[2*NETWIB_PRIV_RANGES_MAXITEMSIZE];
  netwib_priv_ranges_separator separator;
  netwib_uint32 itemsize;
  netwib_buf buf;
  netwib_string pc, pcstart, pcend, pstr;
  netwib_bool usedel;
  netwib_err ret;

  netwib__constbuf_ref_string(pbuf, pc, bufstorage,
                              netwib_priv_ranges_add_buf(pr, &bufstorage, pfunc));

  netwib__debug_ranges_check("netwib_priv_ranges_add_buf", pr)

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    /* skip spaces */
    while (*pc == ' ' || *pc == '\t') {
      pc++;
    }
    /* check if there is '!' which indicate to del instead of add */
    usedel = NETWIB_FALSE;
    if (*pc == '!') {
      pc++;
      usedel = NETWIB_TRUE;
    }
    /* skip spaces */
    while (*pc == ' ' || *pc == '\t') {
      pc++;
    }
    /* now the real start */
    pcstart = pc;
    /* find end */
    while (*pc != '\0' && *pc != ',' && *pc != ' ' && *pc != '\t') {
      pc++;
    }
    pcend = pc;
    /* put this data in the buffer */
    itemsize = pcend - pcstart;
    if (itemsize) {
      netwib_er(netwib_buf_wantspace(&buf, itemsize+1, (netwib_data*)&pstr));
      netwib_c_memcpy(pstr, pcstart, itemsize);
      pstr[itemsize] = '\0';
      if (!netwib_c_strcmp(pstr, "all") || !netwib_c_strcmp(pstr, "any")) {
        if (usedel) {
          netwib_er(netwib_priv_ranges_del_all(pr));
        } else {
          netwib_er(netwib_priv_ranges_add_all(pr));
        }
      } else {
        /* decode the string */
        ret = (*pfunc)(pstr, array, &separator);
        if (ret != NETWIB_ERR_OK) {
          break;
        }
        ret = netwib_priv_ranges_add_buf2(pr, array, separator,
                                         infarray, suparray);
        if (ret != NETWIB_ERR_OK) {
          break;
        }
        if (usedel) {
          ret = netwib_priv_ranges_del_range(pr, infarray, suparray);
        } else {
          ret = netwib_priv_ranges_add_range(pr, infarray, suparray);
        }
        if (ret != NETWIB_ERR_OK) {
          break;
        }
      }
    }
    /* end */
    if (*pc == '\0') {
      break;
    }
    pc++;
    /* reinit for next call */
    netwib__buf_reinit(&buf);
  }
  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwib_priv_ranges_index_init(netwib_priv_constranges *pr,
                                         netwib_priv_ranges_index *pri)
{
  netwib__debug_ranges_check("netwib_priv_ranges_next_range", pr)
  pri->pr = netwib_priv_castptr(pr);
  pri->lastset = NETWIB_FALSE;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ranges_index_close(netwib_priv_ranges_index *pri)
{
  pri->pr = NULL;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ranges_index_rewind(netwib_priv_ranges_index *pri)
{
  pri->lastset = NETWIB_FALSE;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ranges_index_index(netwib_priv_ranges_index *pri,
                                          netwib_priv_ranges_index *priref)
{

  if (priref->pr->itemsize != pri->pr->itemsize) {
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  pri->pr = priref->pr;
  pri->lastset = priref->lastset;
  if (priref->lastset) {
    pri->lastindex = priref->lastindex;
    netwib_c_memcpy(pri->lastinfitem, priref->lastinfitem,
                    priref->pr->itemsize);
    netwib_c_memcpy(pri->lastsupitem, priref->lastsupitem,
                    priref->pr->itemsize);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_ranges_index_adjust(netwib_priv_ranges_index *pri,
                                                  netwib_uint32 *pfoundindex,
                                                  netwib_data *pfoundrange,
                                                  netwib_bool *pinthisrange)
{
  netwib_priv_ranges *pr;
  netwib_data pitem;
  netwib_cmp cmp1, cmp2;

  pr = pri->pr;

  /* check if last item is really inside lastindex */
  if (pri->lastindex < pr->numranges) {
    pitem = (netwib_data)pr->ptr + pri->lastindex * pr->rangesize;
    netwib_er(netwib_priv_ranges_item_cmp(pr, pitem,
                                          pri->lastsupitem, &cmp1));
    netwib_er(netwib_priv_ranges_item_cmp(pr, pitem + pr->itemsize,
                                          pri->lastsupitem, &cmp2));
    if (cmp1 != NETWIB_CMP_GT && cmp2 != NETWIB_CMP_LT) {
      *pfoundindex = pri->lastindex;
      *pfoundrange = pitem;
      *pinthisrange = NETWIB_TRUE;
      return(NETWIB_ERR_OK);
    }
  }

  /* otherwise, something changed, and research index position */
  if (pr->inittype == NETWIB_PRIV_RANGES_INITTYPE_SORTUNIQ) {
    netwib_er(netwib_priv_ranges_search_dichotomy(pr, pri->lastsupitem,
                                                  pfoundindex,
                                                  pfoundrange, pinthisrange));
  } else {
    netwib_er(netwib_priv_ranges_search_linear_notsort(pr, pr->ptr,
                                                       pri->lastsupitem,
                                                       pfoundindex,
                                                       pfoundrange,
                                                       pinthisrange));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ranges_index_next(netwib_priv_ranges_index *pri,
                                         netwib_data ptr)
{
  netwib_priv_ranges *pr;
  netwib_uint32 foundindex;
  netwib_data foundrange;
  netwib_bool inthisrange;
  netwib_cmp cmp;

  pr = pri->pr;
  netwib__debug_ranges_check("netwib_priv_ranges_index_next", pr)

  /* special case of first call */
  if (!pri->lastset) {
    if (pr->numranges == 0) {
      return(NETWIB_ERR_DATAEND);
    }
    netwib_c_memcpy(ptr, pr->ptr, pr->itemsize);
    netwib_c_memcpy(pri->lastinfitem, pr->ptr, pr->itemsize);
    netwib_c_memcpy(pri->lastsupitem, pr->ptr, pr->itemsize);
    pri->lastindex = 0;
    pri->lastset = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  /* obtain next range, eventually dealing with removed values */
  netwib_er(netwib_priv_ranges_index_adjust(pri, &foundindex,
                                            &foundrange, &inthisrange));
  if (!inthisrange) {
    if (foundindex == pr->numranges) {
      return(NETWIB_ERR_DATAEND);
    }
    if (pr->inittype != NETWIB_PRIV_RANGES_INITTYPE_SORTUNIQ) {
      /* item is no more in the list, so say it's end */
      return(NETWIB_ERR_DATAEND);
    }
    /* item was removed, so use next low value */
    netwib_c_memcpy(ptr, foundrange, pr->itemsize);
    netwib_c_memcpy(pri->lastinfitem, foundrange, pr->itemsize);
    netwib_c_memcpy(pri->lastsupitem, foundrange, pr->itemsize);
    pri->lastindex = foundindex;
    return(NETWIB_ERR_OK);
  }

  /* item is in foundrange */
  netwib_er(netwib_priv_ranges_item_cmp(pr, foundrange+pr->itemsize,
                                        pri->lastsupitem, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    /* not last position */
    netwib_er(netwib_priv_ranges_item_inc(pr, pri->lastsupitem));
    netwib_c_memcpy(ptr, pri->lastsupitem, pr->itemsize);
    netwib_c_memcpy(pri->lastinfitem, pri->lastsupitem, pr->itemsize);
    pri->lastindex = foundindex;
    return(NETWIB_ERR_OK);
  }

  /* we have to use next ranges */
  if (foundindex == pr->numranges-1) {
    /* it was the last */
    return(NETWIB_ERR_DATAEND);
  }
  netwib_c_memcpy(ptr, foundrange+pr->rangesize, pr->itemsize);
  netwib_c_memcpy(pri->lastinfitem, ptr, pr->itemsize);
  netwib_c_memcpy(pri->lastsupitem, ptr, pr->itemsize);
  pri->lastindex = foundindex + 1;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ranges_index_next_range(netwib_priv_ranges_index *pri,
                                               netwib_data ptrinf,
                                               netwib_data ptrsup)
{
  netwib_priv_ranges *pr;
  netwib_uint32 foundindex;
  netwib_data foundrange;
  netwib_bool inthisrange;
  netwib_cmp cmp;

  pr = pri->pr;
  netwib__debug_ranges_check("netwib_priv_ranges_index_next_range", pr)

  /* special case of first call */
  if (!pri->lastset) {
    if (pr->numranges == 0) {
      return(NETWIB_ERR_DATAEND);
    }
    netwib_c_memcpy(ptrinf, pr->ptr, pr->itemsize);
    netwib_c_memcpy(ptrsup, (netwib_data)pr->ptr+pr->itemsize,
                    pr->itemsize);
    netwib_c_memcpy(pri->lastinfitem, ptrinf, pr->itemsize);
    netwib_c_memcpy(pri->lastsupitem, ptrsup, pr->itemsize);
    pri->lastindex = 0;
    pri->lastset = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  /* obtain next range, eventually dealing with removed values */
  netwib_er(netwib_priv_ranges_index_adjust(pri, &foundindex,
                                            &foundrange, &inthisrange));
  if (!inthisrange) {
    if (foundindex == pr->numranges) {
      return(NETWIB_ERR_DATAEND);
    }
    if (pr->inittype != NETWIB_PRIV_RANGES_INITTYPE_SORTUNIQ) {
      /* item is no more in the list, so say it's end */
      return(NETWIB_ERR_DATAEND);
    }
    /* item was removed, so use low value */
    netwib_c_memcpy(ptrinf, foundrange, pr->itemsize);
    netwib_c_memcpy(ptrsup, foundrange+pr->itemsize, pr->itemsize);
    netwib_c_memcpy(pri->lastinfitem, ptrinf, pr->itemsize);
    netwib_c_memcpy(pri->lastsupitem, ptrsup, pr->itemsize);
    pri->lastindex = foundindex;
    return(NETWIB_ERR_OK);
  }

  /* item is in foundrange */
  netwib_er(netwib_priv_ranges_item_cmp(pr, foundrange+pr->itemsize,
                                        pri->lastsupitem, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    /* not last position */
    netwib_er(netwib_priv_ranges_item_inc(pr, pri->lastsupitem));
    netwib_c_memcpy(ptrinf, pri->lastsupitem, pr->itemsize);
    netwib_c_memcpy(ptrsup, foundrange+pr->itemsize, pr->itemsize);
    netwib_c_memcpy(pri->lastinfitem, ptrinf, pr->itemsize);
    netwib_c_memcpy(pri->lastsupitem, ptrsup, pr->itemsize);
    pri->lastindex = foundindex;
    return(NETWIB_ERR_OK);
  }

  /* we have to use next ranges */
  if (foundindex == pr->numranges-1) {
    /* it was the last */
    return(NETWIB_ERR_DATAEND);
  }
  netwib_c_memcpy(ptrinf, foundrange+pr->rangesize, pr->itemsize);
  netwib_c_memcpy(ptrsup, foundrange+pr->rangesize+pr->itemsize, pr->itemsize);
  netwib_c_memcpy(pri->lastinfitem, ptrinf, pr->itemsize);
  netwib_c_memcpy(pri->lastsupitem, ptrsup, pr->itemsize);
  pri->lastindex = foundindex + 1;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ranges_index_this(netwib_priv_ranges_index *pri,
                                         netwib_data ptr)
{
  netwib_priv_ranges *pr;

  pr = pri->pr;
  netwib__debug_ranges_check("netwib_priv_ranges_index_this", pr)

  if (!pri->lastset) {
    return(NETWIB_ERR_PAINDEXNODATA);
  }

  netwib_c_memcpy(ptr, pri->lastsupitem, pr->itemsize);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ranges_index_this_range(netwib_priv_ranges_index *pri,
                                               netwib_data ptrinf,
                                               netwib_data ptrsup)
{
  netwib_priv_ranges *pr;

  pr = pri->pr;
  netwib__debug_ranges_check("netwib_priv_ranges_index_this", pr)

  if (!pri->lastset) {
    return(NETWIB_ERR_PAINDEXNODATA);
  }

  netwib_c_memcpy(ptrinf, pri->lastinfitem, pr->itemsize);
  netwib_c_memcpy(ptrsup, pri->lastsupitem, pr->itemsize);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ranges_index_this_del(netwib_priv_ranges_index *pri)
{
  netwib_priv_ranges *pr;

  pr = pri->pr;
  netwib__debug_ranges_check("netwib_priv_ranges_index_this", pr)

  if (!pri->lastset) {
    return(NETWIB_ERR_PAINDEXNODATA);
  }

  netwib_er(netwib_priv_ranges_del_range(pr,
                                         pri->lastinfitem, pri->lastsupitem));

  return(NETWIB_ERR_OK);
}
