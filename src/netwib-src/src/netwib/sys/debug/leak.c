/* This file is part of Netwib. Read and honor its license.
*/

/*
  The data in the list contains :
   - 0 pointer to next item
   - 1 threadid
   - 2 netwib_debug_leaktype
   - 3 infoptr
   - 4 infoint
   - 5 backtrace size
   - 6+backtrace array
*/

/*-------------------------------------------------------------*/
static netwib_err netwib_debug_leak_disp(netwib_ptr item[]);

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwib_debug_leak_init(void)
{
  netwib_er(netwib_debug_glovars_wrlock());
  netwib_debug_glovars.leak_list = NULL;
  netwib_er(netwib_debug_glovars_wrunlock());
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_debug_leak_close2(void)
{
  netwib_ptr *item, *itemtofree;
  netwib_debug_glovars_t *pglo = &netwib_debug_glovars;

  item = (netwib_ptr*)pglo->leak_list;
  while (item != NULL) {
    /* netwib_er(netwib_debug_leak_disp(item)); */
    itemtofree = item;
    /* next item */
    item = (netwib_ptr*)item[0];
    /* now free it, we do not need it */
    free(itemtofree);
  }
  pglo->leak_list = NULL;
  return(NETWIB_ERR_OK);
}
netwib_err netwib_debug_leak_close(void)
{
  netwib_err ret;
  netwib_er(netwib_debug_glovars_wrlock());
  ret = netwib_debug_leak_close2();
  netwib_er(netwib_debug_glovars_wrunlock());
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_debug_leak_add2(netwib_debug_leaktype type,
                                         netwib_constptr infoptr,
                                         netwib_uint32 infoint)
{
  netwib_ptr bt[NETWIB_DEBUG_BACKTRACE_MAXSIZE];
  netwib_uint32 btsize, i, j, threadid;
  netwib_ptr *item;
  netwib_debug_glovars_t *pglo = &netwib_debug_glovars;

  /* first, get the back trace */
  netwib_er(netwib_debug_backtrace_array(NETWIB_DEBUG_BACKTRACE_MAXSIZE,
                                         bt, &btsize));
  /*
    for ( i=0 ; i<btsize-1 ; i++ ) {
      netwib_er(netwib_debug_disp_fmt("%p,", bt[i]));
    }
    netwib_er(netwib_debug_disp_fmt("%p\n", bt[btsize-1]));
  */

  /* allocate list item */
  item = (netwib_ptr*)malloc((6 + btsize)*sizeof(netwib_ptr));
  if (item == NULL)
    return(NETWIB_ERR_FUMALLOC);

  /* store values in it */
  netwib_er(netwib_debug_thread_id(&threadid));
  item[0] = pglo->leak_list;
  item[1] = (netwib_ptr)((netwib_uintptr)threadid);
  item[2] = (netwib_ptr)((netwib_uintptr)type);
  item[3] = netwib_priv_castpptr(infoptr);
  item[4] = (netwib_ptr)((netwib_uintptr)infoint);
  item[5] = (netwib_ptr)((netwib_uintptr)btsize);
  for (i = 0, j = 6; i < btsize; i++, j++) {
    item[j] = bt[i];
  }

  /* display the item */
  /*netwib_er(netwib_debug_disp_string("ADD"));
    netwib_er(netwib_debug_leak_disp(item));*/

  /* insert the item at the beginning of the list */
  pglo->leak_list = item;

  return(NETWIB_ERR_OK);
}
netwib_err netwib_debug_leak_add(netwib_debug_leaktype type,
                                 netwib_constptr infoptr,
                                 netwib_uint32 infoint)
{
  netwib_err ret;
  netwib_er(netwib_debug_glovars_wrlock());
  ret = netwib_debug_leak_add2(type, infoptr, infoint);
  netwib_er(netwib_debug_glovars_wrunlock());
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_debug_leak_del2(netwib_debug_leaktype type,
                                         netwib_bool useptr,
                                         netwib_constptr infoptr,
                                         netwib_bool useint,
                                         netwib_uint32 infoint)
{
  netwib_ptr *item, *pprec;
  netwib_debug_glovars_t *pglo = &netwib_debug_glovars;

  item = (netwib_ptr*)pglo->leak_list;
  pprec = NULL;
  while (item != NULL) {
    if (item[2] == (netwib_ptr)type) {
      /* optimization for :
      if (!useptr || (useptr && item[3] == infoptr) ) {
        if (!useint || (useint && item[4] == infoint) ) {
       */
      if (!useptr || item[3] == infoptr) {
        if (!useint || item[4] == (netwib_ptr)((netwib_uintptr)infoint)) {
          /* found it */
          if (pprec == NULL)
            pglo->leak_list = item[0];
          else
            *pprec = item[0];
          /*netwib_er(netwib_debug_disp_string("DEL"));
            netwib_er(netwib_debug_leak_disp(item));*/
          free(item);
          return(NETWIB_ERR_OK);
        }
      }
    }
    /* next item */
    pprec = item;
    item = (netwib_ptr*)item[0];
  }

  return(NETWIB_ERR_NOTFOUND);
}
netwib_err netwib_debug_leak_del(netwib_debug_leaktype type,
                                 netwib_bool useptr,
                                 netwib_constptr infoptr,
                                 netwib_bool useint,
                                 netwib_uint32 infoint)
{
  netwib_err ret;
  netwib_er(netwib_debug_glovars_wrlock());
  ret = netwib_debug_leak_del2(type, useptr, infoptr, useint, infoint);
  netwib_er(netwib_debug_glovars_wrunlock());
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_debug_leak_valid2(netwib_debug_leaktype type,
                                           netwib_bool useptr,
                                           netwib_constptr infoptr,
                                           netwib_bool useint,
                                           netwib_uint32 infoint)
{
  netwib_ptr *item, *pprec;
  netwib_debug_glovars_t *pglo = &netwib_debug_glovars;

  item = (netwib_ptr*)pglo->leak_list;
  pprec = NULL;
  while (item != NULL) {
    if (item[2] == (netwib_ptr)type) {
      /* optimization for :
        if (!useptr || (useptr && item[3] == infoptr) ) {
          if (!useint || (useint && item[4] == infoint) ) {
       */
      if (!useptr || item[3] == infoptr) {
        if (!useint || item[4] == (netwib_ptr)((netwib_uintptr)infoint)) {
          /* found it */
          return(NETWIB_ERR_OK);
        }
      }
    }
    /* next item */
    pprec = item;
    item = (netwib_ptr*)item[0];
  }

  return(NETWIB_ERR_LOOBJUSENOTINITIALIZED);
}
netwib_err netwib_debug_leak_valid(netwib_debug_leaktype type,
                                   netwib_bool useptr,
                                   netwib_constptr infoptr,
                                   netwib_bool useint,
                                   netwib_uint32 infoint)
{
  netwib_err ret;
  netwib_er(netwib_debug_glovars_rdlock());
  ret = netwib_debug_leak_valid2(type, useptr, infoptr, useint, infoint);
  netwib_er(netwib_debug_glovars_rdunlock());
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_debug_leak_report2(void)
{
  netwib_ptr *item;
  netwib_debug_glovars_t *pglo = &netwib_debug_glovars;
  netwib_debug_leaktype type;

  /* display everything except MEM */
  for (type = (netwib_debug_leaktype)(NETWIB_DEBUG_LEAKTYPE_MEM + 2);
       type < NETWIB_DEBUG_LEAKTYPE_ENDENUM;
       type = (netwib_debug_leaktype)(type + 1)) {
    item = (netwib_ptr*)pglo->leak_list;
    while (item != NULL) {
      if (item[2] == (netwib_ptr)type) {
        netwib_er(netwib_debug_leak_disp(item));
      }
      item = (netwib_ptr*)item[0];
    }
  }

  /* display MEM */
  item = (netwib_ptr*)pglo->leak_list;
  while (item != NULL) {
    if (item[2] == (netwib_ptr)NETWIB_DEBUG_LEAKTYPE_MEM) {
      netwib_er(netwib_debug_leak_disp(item));
    }
    item = (netwib_ptr*)item[0];
  }

  return(NETWIB_ERR_OK);
}
netwib_err netwib_debug_leak_report(void)
{
  netwib_err ret;
  netwib_er(netwib_debug_glovars_rdlock());
  ret = netwib_debug_leak_report2();
  netwib_er(netwib_debug_glovars_rdunlock());
  return(ret);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwib_debug_leak_add_generic(netwib_constptr ptr,
                                         netwib_debug_leaktype leaktype)
{
  return(netwib_debug_leak_add(leaktype, ptr, 0));
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_leak_del_generic(netwib_constptr ptr,
                                         netwib_debug_leaktype leaktype)
{
  return(netwib_debug_leak_del(leaktype, NETWIB_TRUE, ptr, NETWIB_FALSE, 0));
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_leak_valid_generic(netwib_constptr ptr,
                                           netwib_debug_leaktype leaktype)
{
  return(netwib_debug_leak_valid(leaktype, NETWIB_TRUE, ptr, NETWIB_FALSE, 0));
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwib_debug_leak_add_mem(netwib_uint32 allocsize,
                                     netwib_constptr ptr)
{
  netwib_er(netwib_debug_leak_add(NETWIB_DEBUG_LEAKTYPE_MEM, ptr, allocsize));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_leak_del_mem(netwib_constptr ptr)
{
  netwib_err ret;

  ret = netwib_debug_leak_del(NETWIB_DEBUG_LEAKTYPE_MEM,
                              NETWIB_TRUE, ptr, NETWIB_FALSE, 0);
  if (ret == NETWIB_ERR_NOTFOUND) {
    return(NETWIB_ERR_LOOBJCLOSENOTINITIALIZED);
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_leak_valid_mem(netwib_constptr ptr)
{
  return(netwib_debug_leak_valid(NETWIB_DEBUG_LEAKTYPE_MEM,
                                 NETWIB_TRUE, ptr, NETWIB_FALSE, 0));
}

/*-------------------------------------------------------------*/
static netwib_err netwib_debug_leak_disp_mem(netwib_constptr infoptr,
                                             netwib_uint32 infoint)
{
  netwib_constdata infoptrdata = (netwib_constdata)infoptr;
  netwib_uint32 i, displaysize;
  netwib_byte c;

  netwib_er(netwib_debug_disp_fmt("MEM : %p of size %{uint32}\n",
                                  infoptr, infoint));

  displaysize = infoint;
  if (displaysize > 16)
    displaysize = 16;
  for (i = 0; i < displaysize; i++) {
    netwib_er(netwib_debug_disp_fmt(" %{byte:02X}", infoptrdata[i]));
  }
  netwib_er(netwib_debug_disp_string("   "));
  for (i = 0; i < displaysize; i++) {
    c = infoptrdata[i];
    if (netwib_c2_isprint(c)) {
      netwib_er(netwib_debug_disp_fmt("%c", c));
    } else {
      netwib_er(netwib_debug_disp_string("."));
    }
  }
  netwib_er(netwib_debug_disp_string("\n"));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwib_debug_leak_add_fd(netwib_uint32 fd)
{
  return(netwib_debug_leak_add(NETWIB_DEBUG_LEAKTYPE_FD, NULL, fd));
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_leak_del_fd(netwib_uint32 fd)
{
  return(netwib_debug_leak_del(NETWIB_DEBUG_LEAKTYPE_FD,
                               NETWIB_FALSE, NULL, NETWIB_TRUE, fd));
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_leak_valid_fd(netwib_uint32 fd)
{
  return(netwib_debug_leak_valid(NETWIB_DEBUG_LEAKTYPE_FD,
                                 NETWIB_FALSE, NULL, NETWIB_TRUE, fd));
}

/*-------------------------------------------------------------*/
static netwib_err netwib_debug_leak_disp_fd(netwib_uint32 infoint)
{
  netwib_er(netwib_debug_disp_fmt("FD : %{uint32}\n", infoint));

  return(NETWIB_ERR_OK);

}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/* Generate between 100 and 0x07FFFFFF :
    - 99 is NETWIB_DEBUG_LEAK_ID_BUF_IGNORE
    - stop at 0x07FFFFFF (27 bits) because stored in flags
    - leak id might round, so it's why we use both
      ptr/leakid for a buffer : probability is less important
      (it might still happen...).
*/
netwib_err netwib_debug_leak_id_buf(netwib_uint32 *pleakid)
{

  /* not MT safe, but that should not bother us */
  static netwib_uint32 leakid = 0xFFFFFFFFu >> NETWIB_BUF_FLAGS_USEDBITS;

  if (pleakid != NULL) {
    if (leakid < 100) leakid = 0xFFFFFFFFu >> NETWIB_BUF_FLAGS_USEDBITS;
    *pleakid = leakid;
    leakid--;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_leak_add_buf(netwib_constptr ptr,
                                     netwib_uint32 leakid)
{
  if (leakid != NETWIB_DEBUG_LEAK_ID_BUF_IGNORE) {
    netwib_er(netwib_debug_leak_add(NETWIB_DEBUG_LEAKTYPE_BUF, ptr, leakid));
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_leak_del_buf(netwib_constptr ptr,
                                     netwib_uint32 leakid)
{
  if (leakid != NETWIB_DEBUG_LEAK_ID_BUF_IGNORE) {
    netwib_er(netwib_debug_leak_del(NETWIB_DEBUG_LEAKTYPE_BUF,
                                  NETWIB_TRUE, ptr, NETWIB_TRUE, leakid));
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_leak_valid_buf(netwib_constptr ptr,
                                       netwib_uint32 leakid)
{
  if (leakid != NETWIB_DEBUG_LEAK_ID_BUF_IGNORE) {
    netwib_er(netwib_debug_leak_valid(NETWIB_DEBUG_LEAKTYPE_BUF,
                                    NETWIB_TRUE, ptr, NETWIB_TRUE, leakid));
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_debug_leak_disp_buf(netwib_constptr infoptr,
                                             netwib_uint32 infoint)
{
  netwib_er(netwib_debug_disp_fmt("BUF : ptr=%p leakid=%{uint32:08X}\n",
                                infoptr, infoint));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwib_debug_leak_disp(netwib_ptr item[])
{
  netwib_uint32 btsize, i, threadid;
  netwib_debug_leaktype type;
  netwib_ptr infoptr;
  netwib_uint32 infoint;

  btsize = (netwib_uint32)((netwib_uintptr)item[5]);
  if (btsize > NETWIB_DEBUG_BACKTRACE_MAXSIZE) {
    netwib_er(netwib_debug_disp_string("netwib_debug_leak_disp : btsize>NETWIB_DEBUG_BACKTRACE_MAXSIZE"));
    return(NETWIB_ERR_LOINTERNALERROR);
  }

  threadid = (netwib_uint32)((netwib_uintptr)item[1]);
  netwib_er(netwib_debug_disp_fmt("[threadid:%{uint32}] : ", threadid));

  if (btsize != 0) {
    for (i = 6; i < btsize + 5; i++) {
      netwib_er(netwib_debug_disp_fmt("%p,", item[i]));
    }
    netwib_er(netwib_debug_disp_fmt("%p\n", item[btsize + 4]));
  } else {
    netwib_er(netwib_debug_disp_string("Backtrace is empty (unsupported ?)\n"));
  }

  type = (netwib_debug_leaktype)((netwib_uintptr)item[2]);
  infoptr = item[3];
  infoint = (netwib_uint32)((netwib_uintptr)item[4]);
  switch(type) {
    case NETWIB_DEBUG_LEAKTYPE_MEM :
      netwib_er(netwib_debug_leak_disp_mem(infoptr, infoint));
      break;
    case NETWIB_DEBUG_LEAKTYPE_FD :
      netwib_er(netwib_debug_leak_disp_fd(infoint));
      break;
    case NETWIB_DEBUG_LEAKTYPE_BUF :
      netwib_er(netwib_debug_leak_disp_buf(infoptr, infoint));
      break;
    case NETWIB_DEBUG_LEAKTYPE_RING :
      netwib_er(netwib_debug_disp_fmt("RING : %p\n", infoptr));
      break;
    case NETWIB_DEBUG_LEAKTYPE_RINGI :
      netwib_er(netwib_debug_disp_fmt("RING_INDEX : %p\n", infoptr));
      break;
    case NETWIB_DEBUG_LEAKTYPE_HASH :
      netwib_er(netwib_debug_disp_fmt("HASH : %p\n", infoptr));
      break;
    case NETWIB_DEBUG_LEAKTYPE_HASHI :
      netwib_er(netwib_debug_disp_fmt("HASH_INDEX : %p\n", infoptr));
      break;
    case NETWIB_DEBUG_LEAKTYPE_ARRAY :
      netwib_er(netwib_debug_disp_fmt("ARRAY : %p\n", infoptr));
      break;
    case NETWIB_DEBUG_LEAKTYPE_DIR :
      netwib_er(netwib_debug_disp_fmt("DIR : %p\n", infoptr));
      break;
    case NETWIB_DEBUG_LEAKTYPE_IO :
      netwib_er(netwib_debug_disp_fmt("IO : %p\n", infoptr));
      break;
    case NETWIB_DEBUG_LEAKTYPE_THREAD :
      netwib_er(netwib_debug_disp_fmt("THREAD : %p\n", infoptr));
      break;
    case NETWIB_DEBUG_LEAKTYPE_THREAD_MUTEX :
      netwib_er(netwib_debug_disp_fmt("THREAD_MUTEX : %p\n", infoptr));
      break;
    case NETWIB_DEBUG_LEAKTYPE_THREAD_RWLOCK :
      netwib_er(netwib_debug_disp_fmt("THREAD_RWLOCK : %p\n", infoptr));
      break;
    case NETWIB_DEBUG_LEAKTYPE_THREAD_COND :
      netwib_er(netwib_debug_disp_fmt("THREAD_COND : %p\n", infoptr));
      break;
    case NETWIB_DEBUG_LEAKTYPE_THREAD_TSD :
      netwib_er(netwib_debug_disp_fmt("THREAD_TSD : %p\n", infoptr));
      break;
    case NETWIB_DEBUG_LEAKTYPE_WAIT :
      netwib_er(netwib_debug_disp_fmt("WAIT : %p\n", infoptr));
      break;
    case NETWIB_DEBUG_LEAKTYPE_ETHS :
      netwib_er(netwib_debug_disp_fmt("ETHS : %p\n", infoptr));
      break;
    case NETWIB_DEBUG_LEAKTYPE_IPS :
      netwib_er(netwib_debug_disp_fmt("IPS : %p\n", infoptr));
      break;
    case NETWIB_DEBUG_LEAKTYPE_PORTS :
      netwib_er(netwib_debug_disp_fmt("PORTS : %p\n", infoptr));
      break;
    case NETWIB_DEBUG_LEAKTYPE_ETHS_INDEX :
      netwib_er(netwib_debug_disp_fmt("ETHS_INDEX : %p\n", infoptr));
      break;
    case NETWIB_DEBUG_LEAKTYPE_IPS_INDEX :
      netwib_er(netwib_debug_disp_fmt("IPS_INDEX : %p\n", infoptr));
      break;
    case NETWIB_DEBUG_LEAKTYPE_PORTS_INDEX :
      netwib_er(netwib_debug_disp_fmt("PORTS_INDEX : %p\n", infoptr));
      break;
    case NETWIB_DEBUG_LEAKTYPE_CONF_DEVICES_INDEX :
      netwib_er(netwib_debug_disp_fmt("CONF_DEVICES_INDEX : %p\n", infoptr));
      break;
    case NETWIB_DEBUG_LEAKTYPE_CONF_IP_INDEX :
      netwib_er(netwib_debug_disp_fmt("CONF_IP_INDEX : %p\n", infoptr));
      break;
    case NETWIB_DEBUG_LEAKTYPE_CONF_ARPCACHE_INDEX :
      netwib_er(netwib_debug_disp_fmt("CONF_ARPCACHE_INDEX : %p\n", infoptr));
      break;
    case NETWIB_DEBUG_LEAKTYPE_CONF_ROUTES_INDEX :
      netwib_er(netwib_debug_disp_fmt("CONF_ROUTES_INDEX : %p\n", infoptr));
      break;
    case NETWIB_DEBUG_LEAKTYPE_ENDENUM :
    default :
      netwib_er(netwib_priv_errmsg_func_string("netwib_debug_leak_disp",
                                               "unknown value for type"));
      return(NETWIB_ERR_LOINTERNALERROR);
  }

  return(NETWIB_ERR_OK);
}
