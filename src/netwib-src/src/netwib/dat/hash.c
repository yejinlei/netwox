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
#define NETWIB_HASHITEM_ALIGN ((sizeof(netwib_hashitem) + sizeof(void*) - 1) & ~(sizeof(void*) - 1))

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwib_priv_hash_compute(netwib_constdata data,
                                           netwib_uint32 datasize,
                                           netwib_uint32 internalrandom,
                                           netwib_uint32 *phashofkey)
{
  netwib_uint32 hashofkey, i, h;

  /* hashofkey = *data + 33*hashofkey */
  hashofkey = 0;
  for (i = 0 ; i < datasize ; i++) {
    hashofkey += (hashofkey<<5);
    hashofkey += *data;
    data++;
  }

  /* now we use some more computation to help defeat the "hash
     table collision vulnerability". Those lines are far from being
     perfect, but do not add a lot of time overload. If you know
     a fast and better solution... */
  h = (hashofkey<<31)|(hashofkey>>1);
  h += hashofkey;
  h ^= internalrandom;

  if (phashofkey != NULL) *phashofkey = h;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_hash_expand(netwib_hash *phash)
{
  netwib_hashitem **table, *phashitem, *pnext;
  netwib_uint32 pos, newpos, i, newtablesize, newtablemax;

  /* allocate table */
  newtablemax = (phash->tablemax << 1) + 1;
  newtablesize = newtablemax + 1;
  netwib_er(netwib_ptr_malloc(newtablesize * sizeof(netwib_hashitem*),
                            (netwib_ptr*)&table));
  for (i = 0 ; i < newtablesize; i++) {
    table[i] = NULL;
  }

  /* reposition each item */
  for (pos = 0; pos <= phash->tablemax; pos++) {
    phashitem = phash->table[pos];
    while (phashitem != NULL) {
      newpos = phashitem->hashofkey & newtablemax;
      pnext = phashitem->pnext;
      phashitem->pnext = table[newpos];
      table[newpos] = phashitem;
      phashitem->currenttablepos = newpos;
      phashitem = pnext;
    }
  }

  /* free old storage */
  netwib_er(netwib_ptr_free((netwib_ptr*)&(phash->table)));

  /* set parameters */
  phash->table = table;
  phash->tablemax = newtablemax;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
#if NETWIB_DEBUG_HASH_CHECK==1
static netwib_err netwib_debug_hash_check(netwib_conststring functionname,
                                          netwib_hash *phash)
{
  netwib_hashitem *phashitem;
  netwib_uint32 pos, numberofitems, hashofkey;
  netwib_buf errbuf;
  netwib_bool founderror;

  numberofitems = 0;
  founderror = NETWIB_FALSE;
  netwib_er(netwib_buf_init_mallocdefault(&errbuf));
  for (pos = 0; pos <= phash->tablemax; pos++) {
    for (phashitem = phash->table[pos];
         phashitem != NULL;
         phashitem = phashitem->pnext) {
      numberofitems++;
      netwib_er(netwib_priv_hash_compute(phashitem->key, phashitem->keysize,
                                         phash->internalrandom, &hashofkey));
      if (phashitem->hashofkey != hashofkey) {
        netwib_er(netwib_buf_append_fmt(&errbuf,
                                        "phashitem->hashofkey[%{uint32}]",
                                        phashitem->hashofkey));
        netwib_er(netwib_buf_append_fmt(&errbuf,
                                        " != hashofkey[%{uint32}]\n",
                                        hashofkey));
        netwib_er(netwib_buf_append_string("for :\n", &errbuf));
        netwib_er(netwib_buf_append_data(phashitem->key, phashitem->keysize,
                                         &errbuf));
        netwib_er(netwib_buf_append_string("\n", &errbuf));
        founderror = NETWIB_TRUE;
      }
      if (pos != (hashofkey & phash->tablemax) ) {
        netwib_er(netwib_buf_append_fmt(&errbuf, "pos[%{uint32}]", pos));
        netwib_er(netwib_buf_append_fmt(&errbuf,
                                  " != hashofkey&phash->tablemax[%{uint32}]\n",
                                        hashofkey & phash->tablemax));
        netwib_er(netwib_buf_append_string("for :\n", &errbuf));
        netwib_er(netwib_buf_append_data(phashitem->key, phashitem->keysize,
                                         &errbuf));
        netwib_er(netwib_buf_append_string("\n", &errbuf));
        founderror = NETWIB_TRUE;
      }
      if (pos != phashitem->currenttablepos) {
        netwib_er(netwib_buf_append_fmt(&errbuf, "pos[%{uint32}]", pos));
        netwib_er(netwib_buf_append_fmt(&errbuf,
                                        " != currenttablepos[%{uint32}]\n",
                                        phashitem->currenttablepos));
        netwib_er(netwib_buf_append_string("for :\n", &errbuf));
        netwib_er(netwib_buf_append_data(phashitem->key, phashitem->keysize,
                                         &errbuf));
        netwib_er(netwib_buf_append_string("\n", &errbuf));
        founderror = NETWIB_TRUE;
      }
    }
  }
  if (founderror) {
    netwib_er(netwib_priv_errmsg_func_buf(functionname, &errbuf));
    netwib_er(netwib_buf_close(&errbuf));
    return(NETWIB_ERR_LOINTERNALERROR);
  }
  netwib_er(netwib_buf_close(&errbuf));

  if (numberofitems != phash->numberofitems) {
    netwib_er(netwib_buf_init_mallocdefault(&errbuf));
    netwib_er(netwib_buf_append_fmt(&errbuf, "numberofitems[%{int32}] != ",
                                    numberofitems));
    netwib_er(netwib_buf_append_fmt(&errbuf, "phash->numberofitems[%{int32}]",
                                    phash->numberofitems));
    netwib_er(netwib_priv_errmsg_func_buf(functionname, &errbuf));
    netwib_er(netwib_buf_close(&errbuf));
    return(NETWIB_ERR_LOINTERNALERROR);
  }

  return(NETWIB_ERR_OK);
}

 #define netwib__debug_hash_check(a,b) {netwib_er(netwib_debug_hash_check(a,b));}
#else
 #define netwib__debug_hash_check(a,b) {}
#endif


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwib_hash_init(netwib_hash_erase_pf pfunc_erase,
                            netwib_hash_duplicate_pf pfunc_duplicate,
                            netwib_hash **pphash)
{
  netwib_hash *phash;
  netwib_hashitem **table;
  netwib_uint32 i, tablesize;

  /* parameter verification */
  if (pphash == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* allocate needed memory to store phash */
  netwib_er(netwib_ptr_malloc(sizeof(netwib_hash), (netwib_ptr*)&phash));
  *pphash = phash;

  /* set parameters */
  phash->numberofitems = 0;
  tablesize = 32;
  phash->tablemax = tablesize - 1;
  phash->pfunc_erase = pfunc_erase;
  phash->pfunc_duplicate = pfunc_duplicate;
  netwib_er(netwib_uint32_init_rand(0, 0xFFFFFFFFu, &phash->internalrandom));

  /* allocate table */
  netwib_er(netwib_ptr_malloc(tablesize * sizeof(netwib_hashitem*),
                            (netwib_ptr*)&table));
  for (i = 0 ; i < tablesize; i++) {
    table[i] = NULL;
  }
  phash->table = table;

#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_add_hash(phash));
#endif

  netwib__debug_hash_check("netwib_hash_init", phash);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_hash_close(netwib_hash **pphash,
                             netwib_bool eraseitems)
{
  netwib_hash *phash;

  /* parameter verification */
  if (pphash == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  phash = *pphash;
  netwib__debug_hash_check("netwib_hash_close", phash);

  /* remove items */
  netwib_er(netwib_hash_del_all(phash, eraseitems));

  /* free memory */
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_del_hash(phash));
#endif
  netwib_er(netwib_ptr_free((netwib_ptr*)&(phash->table)));
  netwib_er(netwib_ptr_free((netwib_ptr*)&phash));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_hash_ctl_set(netwib_hash *phash,
                               netwib_hash_ctltype type,
                               netwib_ptr p,
                               netwib_uint32 ui)
{
  /* parameter verification */
  if (phash == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_hash(phash));
#endif

  switch(type) {
    case NETWIB_HASH_CTLTYPE_COUNT:
      return(NETWIB_ERR_PAINVALIDTYPE);
    default:
      return(NETWIB_ERR_PAINVALIDTYPE);
      p = p;
      ui = ui;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_hash_ctl_get(netwib_hash *phash,
                               netwib_hash_ctltype type,
                               netwib_ptr p,
                               netwib_uint32 *pui)
{
  /* parameter verification */
  if (phash == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_hash(phash));
#endif

  switch(type) {
    case NETWIB_HASH_CTLTYPE_COUNT:
      if (pui != NULL) {
        *pui = phash->numberofitems;
      }
      break;
    default:
      return(NETWIB_ERR_PAINVALIDTYPE);
      p = p;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_hash_add(netwib_hash *phash,
                           netwib_constbuf *pkey,
                           netwib_constptr pitem,
                           netwib_bool erasepreviousitem)
{
  netwib_uint32 hashofkey, pos;
  netwib_hashitem *phashitem;
  netwib_data key;
  netwib_uint32 keysize;

  /* parameter verification */
  if (phash == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_hash(phash));
#endif
  if (pkey == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  netwib__debug_hash_check("netwib_hash_add 1", phash);

  /* first, check if we need to expand */
  if (phash->numberofitems > phash->tablemax) {
    netwib_er(netwib_priv_hash_expand(phash));
    netwib__debug_hash_check("netwib_hash_add 2", phash);
  }

  /* obtain pointers to key */
  key = netwib__buf_ref_data_ptr(pkey);
  keysize = netwib__buf_ref_data_size(pkey);

  /* then, compute hash of key */
  netwib_er(netwib_priv_hash_compute(key, keysize, phash->internalrandom,
                                     &hashofkey));

  /* obtain the position for this hash */
  pos = hashofkey & phash->tablemax;

  /* check if this key is already in the table */
  for (phashitem = phash->table[pos];
       phashitem != NULL;
       phashitem = phashitem->pnext) {
    if (phashitem->hashofkey == hashofkey &&
        keysize == phashitem->keysize &&
        !netwib_c_memcmp(key, phashitem->key, phashitem->keysize)) {
      /* replace */
      if (erasepreviousitem && phash->pfunc_erase != NULL) {
        netwib_er((*phash->pfunc_erase)(phashitem->pitem));
      }
      phashitem->pitem = netwib_priv_castpptr(pitem);
      phashitem->currenttablepos = pos;
      /* leave here */
      netwib__debug_hash_check("netwib_hash_add 3", phash);
      return(NETWIB_ERR_OK);
    }
  }

  /* if we are here, the key was not found, so insert it */
  netwib_er(netwib_ptr_malloc(NETWIB_HASHITEM_ALIGN+keysize+1,
                            (netwib_ptr*)&phashitem));
  phashitem->pnext = phash->table[pos];
  phash->table[pos] = phashitem;
  phashitem->currenttablepos = pos;
  phashitem->pitem = netwib_priv_castpptr(pitem);
  phashitem->hashofkey = hashofkey;
  phashitem->keysize = keysize;
  phashitem->key = (netwib_data)phashitem + NETWIB_HASHITEM_ALIGN;
  netwib_c_memcpy(phashitem->key, key, keysize);
  phashitem->key[keysize] = '\0'; /* for easy netwib_buf_ref_string */
  phash->numberofitems++;

  netwib__debug_hash_check("netwib_hash_add 4", phash);
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_hash_add_hash_criteria(netwib_hash *phash,
                                         netwib_hash *phashtoadd,
                                         netwib_hash_criteria_pf pfunc_criteria,
                                         netwib_ptr pinfos,
                                         netwib_bool erasepreviousitems)
{
  netwib_hashitem *phashtoadditem;
  netwib_uint32 pos;
  netwib_ptr pitem=NULL;
  netwib_bool b;
  netwib_buf key;
  netwib_err ret;

  /* parameter verification */
  if (phash == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_hash(phash));
#endif
  netwib__debug_hash_check("netwib_hash_add_hash_criteria 1a", phash);
  netwib__debug_hash_check("netwib_hash_add_hash_criteria 1b", phashtoadd);

  /* loop */
  b = NETWIB_TRUE;
  ret = NETWIB_ERR_OK;
  for (pos = 0; pos <= phashtoadd->tablemax; pos++) {
    phashtoadditem = phashtoadd->table[pos];
    while (phashtoadditem != NULL) {
      netwib_er(netwib_buf_init_ext_array(phashtoadditem->key,
                                          phashtoadditem->keysize+1,
                                          0, phashtoadditem->keysize, &key));
      if (pfunc_criteria != NULL) {
        b = NETWIB_FALSE;
        ret = (*pfunc_criteria)(&key, phashtoadditem->pitem, pinfos, &b);
        if (ret != NETWIB_ERR_OK) b = NETWIB_FALSE;
      }
      if (b) {
        if (phash->pfunc_duplicate != NULL) {
          ret = (*phash->pfunc_duplicate)(phashtoadditem->pitem, &pitem);
          if (ret == NETWIB_ERR_OK) {
            ret = netwib_hash_add(phash, &key, pitem, erasepreviousitems);
            if (ret != NETWIB_ERR_OK) {
              if (phash->pfunc_erase != NULL) {
                /* ignore */ (*phash->pfunc_erase)(pitem);
              }
            }
          }
        } else {
          ret = netwib_hash_add(phash, &key, phashtoadditem->pitem,
                                erasepreviousitems);
        }
      }
      if (ret != NETWIB_ERR_OK) return(ret);
      phashtoadditem = phashtoadditem->pnext;
    }
  }

  netwib__debug_hash_check("netwib_hash_add_hash_criteria 2a", phash);
  netwib__debug_hash_check("netwib_hash_add_hash_criteria 2b", phashtoadd);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_hash_contains(netwib_hash *phash,
                                netwib_constbuf *pkey,
                                netwib_bool *pyes)
{
  netwib_uint32 pos, hashofkey;
  netwib_hashitem *phashitem;
  netwib_data key;
  netwib_uint32 keysize;

  /* parameter verification */
  if (phash == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_hash(phash));
#endif
  if (pkey == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  netwib__debug_hash_check("netwib_hash_contains 1", phash);

  /* compute wanted hash */
  key = netwib__buf_ref_data_ptr(pkey);
  keysize = netwib__buf_ref_data_size(pkey);
  netwib_er(netwib_priv_hash_compute(key, keysize, phash->internalrandom,
                                     &hashofkey));

  /* find position which should contain the value */
  pos = hashofkey & phash->tablemax;

  /* loop through the table */
  for (phashitem = phash->table[pos];
       phashitem != NULL;
       phashitem = phashitem->pnext) {
    if (phashitem->hashofkey == hashofkey &&
        keysize == phashitem->keysize &&
        !netwib_c_memcmp(key, phashitem->key, phashitem->keysize)) {
      if (pyes != NULL) *pyes = NETWIB_TRUE;
      netwib__debug_hash_check("netwib_hash_contains 3", phash);
      return(NETWIB_ERR_OK);
    }
  }

  if (pyes != NULL) *pyes = NETWIB_FALSE;
  netwib__debug_hash_check("netwib_hash_contains 2", phash);
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_hash_value(netwib_hash *phash,
                             netwib_constbuf *pkey,
                             netwib_ptr *ppitem)
{
  netwib_uint32 pos, hashofkey;
  netwib_hashitem *phashitem;
  netwib_data key;
  netwib_uint32 keysize;

  /* parameter verification */
  if (phash == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_hash(phash));
#endif
  if (pkey == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  netwib__debug_hash_check("netwib_hash_value 1", phash);

  /* compute wanted hash */
  key = netwib__buf_ref_data_ptr(pkey);
  keysize = netwib__buf_ref_data_size(pkey);
  netwib_er(netwib_priv_hash_compute(key, keysize, phash->internalrandom,
                                     &hashofkey));

  /* find position which should contain the value */
  pos = hashofkey & phash->tablemax;

  /* loop through the table */
  for (phashitem = phash->table[pos];
       phashitem != NULL;
       phashitem = phashitem->pnext) {
    if (phashitem->hashofkey == hashofkey &&
        keysize == phashitem->keysize &&
        !netwib_c_memcmp(key, phashitem->key, phashitem->keysize)) {
      if (ppitem != NULL) *ppitem = phashitem->pitem;
      netwib__debug_hash_check("netwib_hash_value 3", phash);
      return(NETWIB_ERR_OK);
    }
  }

  netwib__debug_hash_check("netwib_hash_value 2", phash);
  return(NETWIB_ERR_NOTFOUND);
}

/*-------------------------------------------------------------*/
netwib_err netwib_hash_del(netwib_hash *phash,
                           netwib_constbuf *pkey,
                           netwib_bool eraseitem)
{
  netwib_uint32 pos, hashofkey;
  netwib_hashitem *phashitem, **addressprevious;
  netwib_data key;
  netwib_uint32 keysize;

  /* parameter verification */
  if (phash == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_hash(phash));
#endif
  if (pkey == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  netwib__debug_hash_check("netwib_hash_del 1", phash);

  /* compute wanted hash */
  key = netwib__buf_ref_data_ptr(pkey);
  keysize = netwib__buf_ref_data_size(pkey);
  netwib_er(netwib_priv_hash_compute(key, keysize, phash->internalrandom,
                                     &hashofkey));

  /* find position which should contain the value */
  pos = hashofkey & phash->tablemax;

  /* loop through the table */
  phashitem = phash->table[pos];
  addressprevious = &(phash->table[pos]);
  while (phashitem != NULL) {
    if (phashitem->hashofkey == hashofkey &&
        keysize == phashitem->keysize &&
        !netwib_c_memcmp(key, phashitem->key, phashitem->keysize)) {
      if (eraseitem && phash->pfunc_erase != NULL) {
        netwib_er((*phash->pfunc_erase)(phashitem->pitem));
      }
      *addressprevious = phashitem->pnext;
      netwib_er(netwib_ptr_free((netwib_ptr*)&phashitem));
      phash->numberofitems--;
      netwib__debug_hash_check("netwib_hash_del 3", phash);
      return(NETWIB_ERR_OK);
    }
    addressprevious = &(phashitem->pnext);
    phashitem = phashitem->pnext;
  }

  netwib__debug_hash_check("netwib_hash_del 2", phash);
  return(NETWIB_ERR_NOTFOUND);
}

/*-------------------------------------------------------------*/
netwib_err netwib_hash_del_criteria(netwib_hash *phash,
                                    netwib_hash_criteria_pf pfunc_criteria,
                                    netwib_ptr pinfos,
                                    netwib_bool eraseitems)
{
  netwib_hashitem *phashitem, *pnext, **addressprevious;
  netwib_uint32 pos;
  netwib_bool b;
  netwib_buf key;
  netwib_err ret;

  /* parameter verification */
  if (phash == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_hash(phash));
#endif
  netwib__debug_hash_check("netwib_hash_del_criteria 1", phash);

  /* loop */
  b = NETWIB_TRUE;
  for (pos = 0; pos <= phash->tablemax; pos++) {
    addressprevious = &(phash->table[pos]);
    phashitem = phash->table[pos];
    while (phashitem != NULL) {
      if (pfunc_criteria != NULL) {
        netwib_er(netwib_buf_init_ext_array(phashitem->key,
                                            phashitem->keysize+1,
                                            0, phashitem->keysize, &key));
        b = NETWIB_FALSE;
        ret = (*pfunc_criteria)(&key, phashitem->pitem, pinfos, &b);
        if (ret != NETWIB_ERR_OK) return(ret);
      }
      pnext = phashitem->pnext;
      if (b) {
        if (eraseitems && phash->pfunc_erase != NULL) {
          netwib_er((*phash->pfunc_erase)(phashitem->pitem));
        }
        *addressprevious = pnext;
        netwib_er(netwib_ptr_free((netwib_ptr*)&phashitem));
        phash->numberofitems--;
      } else {
        addressprevious = &(phashitem->pnext);
      }
      phashitem = pnext;
    }
  }

  netwib__debug_hash_check("netwib_hash_del_criteria 2", phash);

  return(NETWIB_ERR_OK);
}
