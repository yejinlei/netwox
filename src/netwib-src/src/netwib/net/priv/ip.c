/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
/* those defines are here because I have a script checking if
 * netwib contains MT-unsafe functions. Like this, they are ignored. */
#define netwib_gethostbyname gethostbyname
#define netwib_gethostbyaddr gethostbyaddr

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_ip4_bad(netwib_conststring str)
{
  netwib_er(netwib_priv_errmsg_string("this is not an IPv4 address: "));
  netwib_er(netwib_priv_errmsg_append_string(str));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_ip6_bad(netwib_conststring str)
{
  netwib_er(netwib_priv_errmsg_string("this is not an IPv6 address: "));
  netwib_er(netwib_priv_errmsg_append_string(str));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ip_ip4_init_ip6(netwib_constip6 *pip6,
                                       netwib_ip4 *pip4)
{
  netwib_ip4 ip4;
  netwib_bool checkforspecial = NETWIB_FALSE;
  int i;

  /*
    ::0:1.2.3.4 -> 1.2.3.4
    ::ffff:1.2.3.4 -> 1.2.3.4
    ::1 -> 127.0.0.1
  */

  i = 10;
  while (i--) {
    if (pip6->b[i]) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  if (pip6->b[10] == 0xFF && pip6->b[11] == 0xFF) {
    checkforspecial = NETWIB_FALSE;
  } else if (pip6->b[10] == 0 && pip6->b[11] == 0) {
    checkforspecial = NETWIB_TRUE;
  } else {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  ip4 = netwib_c2_uint32_4(pip6->b[12], pip6->b[13], pip6->b[14], pip6->b[15]);
  if (checkforspecial) {
    /* RFC 3473 says (6.2 Socket Address Structure to Node Name
       and Service Name) that "The IPv6 unspecified address ("::")
       and the IPv6 loopback address ("::1") are not IPv4-compatible
       addresses." */
    if (ip4 == 0) {
      return(NETWIB_ERR_NOTCONVERTED);
    } else if (ip4 == 1) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  if (pip4 != NULL) {
    *pip4 = ip4;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ip_ip6_init_ip4(netwib_ip4 ip4,
                                       netwib_ip6 *pip6)
{

  if (pip6 != NULL) {
    netwib_c_memset(pip6->b, 0, 10);
    pip6->b[10] = 0xFF;
    pip6->b[11] = 0xFF;
    pip6->b[12] = netwib_c2_uint32_0(ip4);
    pip6->b[13] = netwib_c2_uint32_1(ip4);
    pip6->b[14] = netwib_c2_uint32_2(ip4);
    pip6->b[15] = netwib_c2_uint32_3(ip4);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* if acceptnet is set, also accept "1.2.3" for "1.2.3.0"
   and "127" for "127.0.0.0". In this case, pmask|pprefix is guessed */
static netwib_err netwib_priv_ip_netmaskprefix_init_sip4(netwib_conststring str,
                                                         netwib_bool acceptnet,
                                                         netwib_ip *pnet,
                                                         netwib_ip *pmask,
                                                         netwib_uint32 *pprefix)
{
  netwib_conststring pc;
  netwib_char c;
  netwib_uint32 numfieldsfound, numdigitfound, fieldvalue;
  netwib_ip4 ip4;

  /* first, skip spaces */
  pc = str;
  while (*pc == ' ' || *pc == '\t') {
    pc++;
  }

  /* main loop */
  numfieldsfound = 0;
  numdigitfound = 0;
  fieldvalue = 0;
  ip4 = 0;
  while (NETWIB_TRUE) {
    c = *pc;
    if (c == '\0' || c == ' ' || c == '\t' ) {
      if (numdigitfound == 0) {
        netwib_er(netwib_priv_ip4_bad(str));
        return(NETWIB_ERR_NOTCONVERTED);
      }
      ip4 = (ip4 << 8) | fieldvalue;
      numfieldsfound++;
      numdigitfound = 0;
      break;
    }
    pc++;
    if (c == '.') {
      if (numdigitfound == 0) {
        netwib_er(netwib_priv_ip4_bad(str));
        return(NETWIB_ERR_NOTCONVERTED);
      }
      if (numfieldsfound == 3) {
        netwib_er(netwib_priv_ip4_bad(str));
        return(NETWIB_ERR_NOTCONVERTED);
      }
      ip4 = (ip4 << 8) | fieldvalue;
      fieldvalue = 0;
      numfieldsfound++;
      numdigitfound = 0;
    } else if (netwib_c2_isdigit(c)) {
      if (numdigitfound == 3) {
        netwib_er(netwib_priv_ip4_bad(str));
        return(NETWIB_ERR_NOTCONVERTED);
      }
      fieldvalue = fieldvalue*10 + netwib_c2_cto9(c);
      if (fieldvalue > 255) {
        netwib_er(netwib_priv_ip4_bad(str));
        return(NETWIB_ERR_NOTCONVERTED);
      }
      numdigitfound++;
    } else {
      netwib_er(netwib_priv_ip4_bad(str));
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  /* check values */
  if (acceptnet) {
    switch(numfieldsfound) {
    case 1 :
      ip4 = ip4 << 24;
      break;
    case 2 :
      ip4 = ip4 << 16;
      break;
    case 3 :
      ip4 = ip4 << 8;
      break;
    }
  } else {
    if (numfieldsfound != 4) {
      netwib_er(netwib_priv_ip4_bad(str));
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  /* check there is only spaces */
  while (*pc == ' ' || *pc == '\t') {
    pc++;
  }
  if (*pc != '\0') {
    netwib_er(netwib_priv_ip4_bad(str));
    return(NETWIB_ERR_NOTCONVERTED);
  }

  if (pnet != NULL) {
    pnet->iptype = NETWIB_IPTYPE_IP4;
    pnet->ipvalue.ip4 = ip4;
  }
  netwib_er(netwib_priv_ip_maskprefix_init_prefix(NETWIB_IPTYPE_IP4,
                                                  8*numfieldsfound,
                                                  pmask,pprefix));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ip_init_sip4(netwib_conststring str,
                                    netwib_ip *pip)
{
  return(netwib_priv_ip_netmaskprefix_init_sip4(str, NETWIB_FALSE, pip,
                                                NULL, NULL));
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ip_init_sip6(netwib_conststring str,
                                    netwib_ip *pip)
{
  netwib_uint16 array[8]; /* 1:2:3:4:5:6:7:8 */
  netwib_conststring pc, pclaststart;
  netwib_char c;
  netwib_uint32 numfieldsfound, numdigitfound, fieldvalue, extendindex, i, j;
  netwib_bool needtocheendspaces;

  /* first, skip spaces */
  pc = str;
  while (*pc == ' ' || *pc == '\t') {
    pc++;
  }

  /* main loop */
  numfieldsfound = 0;
  numdigitfound = 0;
  fieldvalue = 0;
  extendindex = (netwib_uint32)-1;
  pclaststart = pc;
  needtocheendspaces = NETWIB_TRUE;
  while (NETWIB_TRUE) {
    c = *pc;
    if (c == '\0' || c == ' ' || c == '\t' ) {
      if (extendindex == (netwib_uint32)-1) {
        if (numdigitfound == 0 || numfieldsfound != 7) {
          netwib_er(netwib_priv_ip6_bad(str));
          return(NETWIB_ERR_NOTCONVERTED);
        }
      } else {
        if (numfieldsfound > 7) {
          netwib_er(netwib_priv_ip6_bad(str));
          return(NETWIB_ERR_NOTCONVERTED);
        }
      }
      array[numfieldsfound++] = (netwib_uint16)fieldvalue;
      break;
    }
    pc++;
    if (c == ':') {
      if (numdigitfound == 0) {
        if (numfieldsfound != 0) {
          if (extendindex != (netwib_uint32)-1) {
            netwib_er(netwib_priv_ip6_bad(str));
            return(NETWIB_ERR_NOTCONVERTED);
          }
          /* this is index for "::" */
          extendindex = numfieldsfound;
        }
      }
      if (numfieldsfound == 7) {
        netwib_er(netwib_priv_ip6_bad(str));
        return(NETWIB_ERR_NOTCONVERTED);
      }
      array[numfieldsfound] = (netwib_uint16)fieldvalue;
      fieldvalue = 0;
      numfieldsfound++;
      numdigitfound = 0;
      pclaststart = pc;
    } else if (c == '.') {
      netwib_ip ipfor4;
      /* search an IP4 from last start */
      if (numfieldsfound == 0 || numfieldsfound > 6) {
        netwib_er(netwib_priv_ip6_bad(str));
        return(NETWIB_ERR_NOTCONVERTED);
      }
      netwib_er(netwib_priv_ip_init_sip4(pclaststart, &ipfor4));
      array[numfieldsfound++] = (netwib_uint16)(ipfor4.ipvalue.ip4 >> 16);
      array[numfieldsfound++] = (netwib_uint16)(ipfor4.ipvalue.ip4 & 0xFFFF);
      needtocheendspaces = NETWIB_FALSE;
      break;
    } else if (netwib_c2_isxdigit(c)) {
      if (numdigitfound == 4) {
        netwib_er(netwib_priv_ip6_bad(str));
        return(NETWIB_ERR_NOTCONVERTED);
      }
      fieldvalue = (fieldvalue<<4) + netwib_c2_cto16(c);
      numdigitfound++;
    } else {
      netwib_er(netwib_priv_ip6_bad(str));
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }
  /* extend "::" */
  if (extendindex != (netwib_uint32)-1) {
    j = 7;
    for (i = numfieldsfound-1; i > extendindex; i--) {
      array[j--] = array[i];
    }
    while (j >= extendindex) {
      array[j--] = 0;
    }
  }

  /* check there is only spaces */
  if (needtocheendspaces) {
    while (*pc == ' ' || *pc == '\t') {
      pc++;
    }
    if (*pc != '\0') {
      netwib_er(netwib_priv_ip6_bad(str));
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }

  if (pip != NULL) {
    pip->iptype = NETWIB_IPTYPE_IP6;
    i = 0;
    j = 0;
    while (i < NETWIB_IP6_LEN) {
      pip->ipvalue.ip6.b[i++] = (netwib_byte)(array[j] >> 8);
      pip->ipvalue.ip6.b[i++] = (netwib_byte)(array[j++] & 0xFF);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ip_init_hn4(netwib_conststring hn,
                                   netwib_ip *pip)
{

#if NETWIBDEF_HAVEFUNC_GETHOSTBYNAME_R == 1
  {
    netwib_uint32 mallocsize;
    netwib_ip4 ip4;
    netwib_ptr ptr;
    struct hostent entry, *presult;
    int reti, myherrno;

    mallocsize = 1024;
    netwib_er(netwib_ptr_malloc(mallocsize, &ptr));
    while (NETWIB_TRUE) {
      reti = gethostbyname_r(hn, &entry, (netwib_string)ptr, mallocsize,
                             &presult, &myherrno);
      if (reti == ERANGE) {
        mallocsize *= 2;
        netwib_er(netwib_ptr_realloc(mallocsize, &ptr));
      } else {
        break;
      }
    }
    if (reti || presult == NULL) {
      netwib_er(netwib_ptr_free(&ptr));
      return(NETWIB_ERR_NOTCONVERTED);
    }

    ip4 = netwib_c2_uint32_4((netwib_byte)entry.h_addr_list[0][0],
                            (netwib_byte)entry.h_addr_list[0][1],
                            (netwib_byte)entry.h_addr_list[0][2],
                            (netwib_byte)entry.h_addr_list[0][3]);

    netwib_er(netwib_ptr_free(&ptr));

    if (pip != NULL) {
      pip->iptype = NETWIB_IPTYPE_IP4;
      pip->ipvalue.ip4 = ip4;
    }
  }
#else
  {
    netwib_ip4 ip4;
    struct hostent *phe;

    netwib_er(netwib_priv_glovars_other_wrlock());
    phe = netwib_gethostbyname(hn);
    if (phe == NULL) {
      netwib_er(netwib_priv_glovars_other_wrunlock());
      return(NETWIB_ERR_NOTCONVERTED);
    }

    ip4 = netwib_c2_uint32_4((netwib_byte)phe->h_addr_list[0][0],
                            (netwib_byte)phe->h_addr_list[0][1],
                            (netwib_byte)phe->h_addr_list[0][2],
                            (netwib_byte)phe->h_addr_list[0][3]);
    netwib_er(netwib_priv_glovars_other_wrunlock());

    if (pip != NULL) {
      pip->iptype = NETWIB_IPTYPE_IP4;
      pip->ipvalue.ip4 = ip4;
    }
  }
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ip_init_hn6(netwib_conststring hn,
                                   netwib_ip *pip)
{

#if NETWIBDEF_HAVEFUNC_GETADDRINFO == 1
  {
    struct addrinfo hints, *pres, *pcur;
    int reti;
    netwib_err ret;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = PF_INET6;
    reti = getaddrinfo(hn, NULL, &hints, &pres);
    if (reti == 0) {
      pcur = pres;
      while(pcur != NULL) {
        ret = netwib_priv_sa_ip_init_sa((netwib_priv_sockaddr_unalign *)pcur->ai_addr, pip);
        if (ret == NETWIB_ERR_OK) {
          freeaddrinfo(pres);
          return(NETWIB_ERR_OK);
        }
        pcur = pcur->ai_next;
      }
      freeaddrinfo(pres);
    }

    return(NETWIB_ERR_NOTCONVERTED);
  }
#elif NETWIBDEF_HAVEFUNC_GETIPNODEBYNAME == 1
  {
    struct hostent *phe;
    int error_num;

    phe = getipnodebyname(hn, AF_INET6, 0, &error_num);
    if (phe == NULL) {
      return(NETWIB_ERR_NOTCONVERTED);
    }

    if (pip != NULL) {
      pip->iptype = NETWIB_IPTYPE_IP6;
      netwib_c_memset(pip->ipvalue.ip6.b, phe->h_addr_list[0],
                      NETWIB_IP6_LEN);
    }

    freehostent(phe);
  }
#else
  return(NETWIB_ERR_NOTCONVERTED);
  pip = pip;
  hn = hn;
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ip_buf_append_ip4(netwib_constip *pip,
                                         netwib_buf *pbuf)
{
  netwib_data data, pc;
  netwib_ip4 ip4=0;
  netwib_int32 i;
  netwib_uint32 u, a;
  netwib_bool hundset;

  /* max is "255.255.255.255" == 15 chars */
  netwib_er(netwib_buf_wantspace(pbuf, 15, &data));

  switch(pip->iptype) {
    case NETWIB_IPTYPE_IP4 :
      ip4 = pip->ipvalue.ip4;
      break;
    case NETWIB_IPTYPE_IP6 :
      netwib_er(netwib_priv_ip_ip4_init_ip6(&pip->ipvalue.ip6, &ip4));
      break;
    default :
      return(NETWIB_ERR_PAIPTYPE);
  }

  i = 3;
  pc = data;
  while (NETWIB_TRUE) {
    a = 0xFF & (ip4>>(i*8));
    u = a / 100;
    hundset = NETWIB_FALSE;
    if (u) {
      *pc++ = netwib_c2_9toc(u);
      a -= u * 100;
      hundset = NETWIB_TRUE;
    }
    u = a / 10;
    if (u || hundset) {
      *pc++ = netwib_c2_9toc(u);
      a -= u * 10;
    }
    *pc++ = netwib_c2_9toc(a);
    i--;
    if (i == -1) { /* do not use "i < 0" because Visual compiler is buggy
                      (Release mode, not in debugging mode) */
      break;
    }
    *pc++ = '.';
  }

  pbuf->endoffset += pc-data;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ip_buf_append_ip6(netwib_constip *pip,
                                         netwib_buf *pbuf)
{
  netwib_data data, pc;
  netwib_ip6 ip6;
  netwib_uint32 i, quartet;
  netwib_uint32 maxzerostart, curzerostart=0, maxzerocount, curzerocount=0;
  netwib_byte b;
  netwib_bool chardisplayed, zerofound;

  /* max is "1234:1234:1234:1234:1234:1234:1234:1234" == 40 chars */
  netwib_er(netwib_buf_wantspace(pbuf, 40, &data));

  switch(pip->iptype) {
    case NETWIB_IPTYPE_IP4 :
      netwib_er(netwib_priv_ip_ip6_init_ip4(pip->ipvalue.ip4, &ip6));
      break;
    case NETWIB_IPTYPE_IP6 :
      ip6 = pip->ipvalue.ip6;
      break;
    default :
      return(NETWIB_ERR_PAIPTYPE);
  }

  /* locate which items will be compressed */
  zerofound = NETWIB_FALSE;
  maxzerostart = 0;
  maxzerocount = 0;
  for (i = 0; i < 8; i++) {
    if (ip6.b[2*i+1]+ip6.b[2*i] == 0) {
      if (!zerofound) {
        curzerostart = i;
        curzerocount = 1;
        zerofound = NETWIB_TRUE;
      } else {
        curzerocount ++;
      }
    } else {
      if (zerofound) {
        zerofound = NETWIB_FALSE;
        if (curzerocount > maxzerocount) {
          maxzerostart = curzerostart;
          maxzerocount = curzerocount;
        }
      }
    }
  }
  if (zerofound) {
    if (curzerocount > maxzerocount) {
      maxzerostart = curzerostart;
      maxzerocount = curzerocount;
    }
  }
  if (maxzerocount == 1) {
    /* no need to compress */
    maxzerocount = 0;
  }

  /* display and compress */
  i = 0;
  pc = data;
  if (maxzerocount && maxzerostart == 0) {
    *pc++ = ':';
  }
  maxzerostart *= 2;
  while (NETWIB_TRUE) {
    if (maxzerocount && i == maxzerostart) {
      i += 2 * maxzerocount;
    } else {
      chardisplayed = NETWIB_FALSE;
      b = ip6.b[i++];
      quartet = b >> 4;
      if (quartet) {
        *pc++ = netwib_c2_16toc(quartet);
        chardisplayed = NETWIB_TRUE;
      }
      quartet = b & 0xF;
      if (quartet || chardisplayed) {
        *pc++ = netwib_c2_16toc(quartet);
        chardisplayed = NETWIB_TRUE;
      }
      b = ip6.b[i++];
      quartet = b >> 4;
      if (quartet || chardisplayed) {
        *pc++ = netwib_c2_16toc(quartet);
      }
      quartet = b & 0xF;
      *pc++ = netwib_c2_16toc(quartet);
    }
    if (i == NETWIB_IP6_LEN) {
      break;
    }
    *pc++ = ':';
  }
  if (maxzerocount && (maxzerostart+2*maxzerocount == NETWIB_IP6_LEN)) {
    *pc++ = ':';
  }

  pbuf->endoffset += pc-data;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ip_buf_append_hn(netwib_constip *pip,
                                        netwib_buf *pbuf)
{
  netwib_err ret;

#if NETWIBDEF_HAVEFUNC_GETNAMEINFO == 1
  {
    netwib_priv_sockaddr_align sa;
    netwib_priv_sockaddr_len salen;
    char *host;
    int reti;

    netwib_er(netwib_priv_sa_sal_init_ipport(pip, 0, &sa, &salen));
    host = (char *)malloc(NI_MAXHOST);
    reti = getnameinfo((struct sockaddr*)&sa, salen,
                       host, NI_MAXHOST-1, NULL, 0, NI_NAMEREQD);
    if (reti) {
      free(host);
      return(NETWIB_ERR_NOTCONVERTED);
    }

    ret = netwib_buf_append_string(host, pbuf);
    free(host);
  }
#elif NETWIBDEF_HAVEFUNC_GETIPNODEBYADDR == 1
  {
    netwib_char array[4];
    struct hostent *phe;
    int error_num;

    switch(pip->iptype) {
      case NETWIB_IPTYPE_IP4 :
        array[0] = netwib_c2_uint32_0(pip->ipvalue.ip4);
        array[1] = netwib_c2_uint32_1(pip->ipvalue.ip4);
        array[2] = netwib_c2_uint32_2(pip->ipvalue.ip4);
        array[3] = netwib_c2_uint32_3(pip->ipvalue.ip4);
        phe = getipnodebyaddr(array, 4, AF_INET, &error_num);
        break;
      case NETWIB_IPTYPE_IP6 :
        phe = getipnodebyaddr(pip->ipvalue.ip6.b, NETWIB_IP6_LEN,
                              AF_INET6, &error_num);
        break;
      default :
        return(NETWIB_ERR_PAIPTYPE);
    }
    if (phe == NULL) {
      return(NETWIB_ERR_NOTCONVERTED);
    }

    ret = netwib_buf_append_string(phe->h_name, pbuf);
    freehostent(phe);
  }
#elif NETWIBDEF_HAVEFUNC_GETHOSTBYADDR_R == 1
  {
    netwib_char array[4];
    const netwib_char *pc=NULL;
    netwib_uint32 mallocsize, arraysize=0, af=0;
    netwib_ptr ptr;
    struct hostent entry, *presult;
    int reti, myherrno;

    switch(pip->iptype) {
      case NETWIB_IPTYPE_IP4 :
        array[0] = netwib_c2_uint32_0(pip->ipvalue.ip4);
        array[1] = netwib_c2_uint32_1(pip->ipvalue.ip4);
        array[2] = netwib_c2_uint32_2(pip->ipvalue.ip4);
        array[3] = netwib_c2_uint32_3(pip->ipvalue.ip4);
        pc = array;
        arraysize = 4;
        af = AF_INET;
        break;
      case NETWIB_IPTYPE_IP6 :
        pc = (const netwib_char*)pip->ipvalue.ip6.b;
        arraysize = NETWIB_IP6_LEN;
        af = AF_INET6;
        break;
      default :
        return(NETWIB_ERR_PAIPTYPE);
    }

    mallocsize = 1024;
    netwib_er(netwib_ptr_malloc(mallocsize, &ptr));
    while (NETWIB_TRUE) {
      reti = gethostbyaddr_r(pc, arraysize, af, &entry,
                             (netwib_string)ptr, mallocsize,
                             &presult, &myherrno);
      if (reti == ERANGE) {
        mallocsize *= 2;
        netwib_er(netwib_ptr_realloc(mallocsize, &ptr));
      } else {
        break;
      }
    }
    if (reti || presult == NULL) {
      netwib_er(netwib_ptr_free(&ptr));
      return(NETWIB_ERR_NOTCONVERTED);
    }
    ret = netwib_buf_append_string(entry.h_name, pbuf);
    netwib_er(netwib_ptr_free(&ptr));
   }
#else
  {
    netwib_ip4 ip4;
    netwib_char array[4];
    struct hostent *phe;

    netwib_er(netwib_ip4_init_ip(pip, &ip4));
    array[0] = netwib_c2_uint32_0(ip4);
    array[1] = netwib_c2_uint32_1(ip4);
    array[2] = netwib_c2_uint32_2(ip4);
    array[3] = netwib_c2_uint32_3(ip4);

    netwib_er(netwib_priv_glovars_other_wrlock());
    phe = netwib_gethostbyaddr(array, 4, AF_INET);
    if (phe == NULL) {
      netwib_er(netwib_priv_glovars_other_wrunlock());
      return(NETWIB_ERR_NOTCONVERTED);
    }
    ret = netwib_buf_append_string(phe->h_name, pbuf);
    netwib_er(netwib_priv_glovars_other_wrunlock());
  }
#endif

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ip_buf_append_hns(netwib_constip *pip,
                                         netwib_buf *pbuf)
{
  netwib_err ret;

  /* unfortunately, getnameinfo returns only one hostname, so we
     cannot use it */

#if NETWIBDEF_HAVEFUNC_GETIPNODEBYADDR == 1
  {
    netwib_char array[4], *pc;
    netwib_uint32 i;
    struct hostent *phe=NULL;
    int error_num;

    switch(pip->iptype) {
      case NETWIB_IPTYPE_IP4 :
        array[0] = netwib_c2_uint32_0(pip->ipvalue.ip4);
        array[1] = netwib_c2_uint32_1(pip->ipvalue.ip4);
        array[2] = netwib_c2_uint32_2(pip->ipvalue.ip4);
        array[3] = netwib_c2_uint32_3(pip->ipvalue.ip4);
        phe = getipnodebyaddr(array, 4, AF_INET, &error_num);
        break;
      case NETWIB_IPTYPE_IP6 :
        phe = getipnodebyaddr(pip->ipvalue.ip6.b, NETWIB_IP6_LEN,
                              AF_INET6, &error_num);
        break;
      default :
        return(NETWIB_ERR_PAIPTYPE);
    }
    if (phe == NULL) {
      return(NETWIB_ERR_NOTCONVERTED);
    }

    ret = netwib_buf_append_string(phe->h_name, pbuf);
    i = 0;
    while (ret == NETWIB_ERR_OK) {
      pc = phe->h_aliases[i++];
      if (pc == NULL) {
        break;
      }
      ret = netwib_buf_append_byte(',', pbuf);
      if (ret == NETWIB_ERR_OK) {
        ret = netwib_buf_append_string(pc, pbuf);
      }
    }
    freehostent(phe);
  }
#elif NETWIBDEF_HAVEFUNC_GETHOSTBYADDR_R == 1
  {
    netwib_char array[4];
    netwib_conststring pc=NULL;
    netwib_uint32 i, mallocsize, arraysize=0, af=0;
    netwib_ptr ptr;
    struct hostent entry, *presult;
    int reti, myherrno;

    switch(pip->iptype) {
      case NETWIB_IPTYPE_IP4 :
        array[0] = netwib_c2_uint32_0(pip->ipvalue.ip4);
        array[1] = netwib_c2_uint32_1(pip->ipvalue.ip4);
        array[2] = netwib_c2_uint32_2(pip->ipvalue.ip4);
        array[3] = netwib_c2_uint32_3(pip->ipvalue.ip4);
        pc = (netwib_conststring)array;
        arraysize = 4;
        af = AF_INET;
        break;
      case NETWIB_IPTYPE_IP6 :
        pc = (netwib_conststring)pip->ipvalue.ip6.b;
        arraysize = NETWIB_IP6_LEN;
        af = AF_INET6;
        break;
      default :
        return(NETWIB_ERR_PAIPTYPE);
    }

    mallocsize = 1024;
    netwib_er(netwib_ptr_malloc(mallocsize, &ptr));
    while (NETWIB_TRUE) {
      reti = gethostbyaddr_r(pc, arraysize, af, &entry,
                             (netwib_string)ptr, mallocsize,
                             &presult, &myherrno);
      if (reti == ERANGE) {
        mallocsize *= 2;
        netwib_er(netwib_ptr_realloc(mallocsize, &ptr));
      } else {
        break;
      }
    }
    if (reti || presult == NULL) {
      netwib_er(netwib_ptr_free(&ptr));
      return(NETWIB_ERR_NOTCONVERTED);
    }
    ret = netwib_buf_append_string(entry.h_name, pbuf);
    i = 0;
    while (ret == NETWIB_ERR_OK) {
      pc = entry.h_aliases[i++];
      if (pc == NULL) {
        break;
      }
      ret = netwib_buf_append_byte(',', pbuf);
      if (ret == NETWIB_ERR_OK) {
        ret = netwib_buf_append_string(pc, pbuf);
      }
    }
    netwib_er(netwib_ptr_free(&ptr));
   }
#else
  {
    netwib_ip4 ip4;
    netwib_char array[4], *pc;
    netwib_uint32 i;
    struct hostent *phe;

    netwib_er(netwib_ip4_init_ip(pip, &ip4));
    array[0] = netwib_c2_uint32_0(ip4);
    array[1] = netwib_c2_uint32_1(ip4);
    array[2] = netwib_c2_uint32_2(ip4);
    array[3] = netwib_c2_uint32_3(ip4);

    netwib_er(netwib_priv_glovars_other_wrlock());
    phe = netwib_gethostbyaddr(array, 4, AF_INET);
    if (phe == NULL) {
      netwib_er(netwib_priv_glovars_other_wrunlock());
      return(NETWIB_ERR_NOTCONVERTED);
    }
    ret = netwib_buf_append_string(phe->h_name, pbuf);
    i = 0;
    while (ret == NETWIB_ERR_OK) {
      pc = phe->h_aliases[i++];
      if (pc == NULL) {
        break;
      }
      ret = netwib_buf_append_byte(',', pbuf);
      if (ret == NETWIB_ERR_OK) {
        ret = netwib_buf_append_string(pc, pbuf);
      }
    }
    netwib_er(netwib_priv_glovars_other_wrunlock());
  }
#endif

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ip_ip6_supported(netwib_bool *pyes)
{

#if NETWIBDEF_HAVEVAR_AF_INET6 == 1
  {
    int reti;
    reti = socket(PF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (reti < 0) {
      if (pyes != NULL) *pyes = NETWIB_FALSE;
    } else {
      if (pyes != NULL) *pyes = NETWIB_TRUE;
      reti = netwib_c_closesocket(reti);
      netwib_ir(reti, NETWIB_ERR_FUCLOSE);
    }
  }
#else
  if (pyes != NULL) *pyes = NETWIB_FALSE;
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ip_net_init_ipmaskprefix(netwib_constip *pip,
                                                netwib_constip *pmask,
                                                netwib_uint32 prefix,
                                                netwib_ip *pnet)
{
  netwib_ip ipm;
  netwib_uint32 numbit, masktmp, i;

  ipm = *pip;
  switch(pip->iptype) {
    case NETWIB_IPTYPE_IP4 :
      ipm.ipvalue.ip4 &= pmask->ipvalue.ip4;
      break;
    case NETWIB_IPTYPE_IP6 :
      if (prefix > 8*NETWIB_IP6_LEN) {
        return(NETWIB_ERR_PATOOHIGH);
      }
      numbit = prefix % 8;
      i = prefix / 8;
      if (numbit) {
        masktmp = ~ ( (1 << (8-numbit)) - 1);
        ipm.ipvalue.ip6.b[i] &= masktmp;
        i++;
      }
      for (; i < NETWIB_IP6_LEN; i++) {
        ipm.ipvalue.ip6.b[i] = 0;
      }
      break;
    default :
      return(NETWIB_ERR_PAIPTYPE);
  }

  if (pnet != NULL) {
    *pnet = ipm;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ip_net_cmp_ipmaskprefix(netwib_constip *pip1,
                                               netwib_constip *pip2,
                                               netwib_constip *pmask,
                                               netwib_uint32 prefix,
                                               netwib_cmp *pcmp)
{
  netwib_ip ip1m, ip2m;

  if (pip1->iptype == NETWIB_IPTYPE_IP4 && pip2->iptype == NETWIB_IPTYPE_IP6) {
    if (pcmp != NULL) *pcmp = NETWIB_CMP_LT;
    return(NETWIB_ERR_OK);
  }
  if (pip1->iptype == NETWIB_IPTYPE_IP6 && pip2->iptype == NETWIB_IPTYPE_IP4) {
    if (pcmp != NULL) *pcmp = NETWIB_CMP_GT;
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_priv_ip_net_init_ipmaskprefix(pip1, pmask, prefix, &ip1m));
  netwib_er(netwib_priv_ip_net_init_ipmaskprefix(pip2, pmask, prefix, &ip2m));
  netwib_er(netwib_ip_cmp(&ip1m, &ip2m, pcmp));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* Convert network to mask. Unfortunately, this can not work
   correctly. We have to take decisions :
       ip           net           mask
    1.2.3.130     1.2.3.128     255.255.255.128
    1.2.3.130     1.2.3.0       255.255.255.0
    1.2.3.130     1.2.0.0       255.255.0.0
    127.0.0.1     127.0.0.0     255.255.255.0
 */
netwib_err netwib_priv_ip_maskprefix_init_ipnet(netwib_constip *pip,
                                                netwib_constip *pnet,
                                                netwib_ip *pmask,
                                                netwib_uint32 *pprefix)
{
  netwib_byte arrayip[NETWIB_IP4_LEN], arraynet[NETWIB_IP4_LEN];
  netwib_byte arraymask4[NETWIB_IP4_LEN];
  netwib_uint16 arraymask6[NETWIB_IP6_LEN/2];
  netwib_uint32 i, j, prefix, ipui, netui;
  netwib_bool copyend;

  if (pip->iptype != pnet->iptype) {
    return(NETWIB_ERR_PAIPTYPE);
  }

  prefix = 0;
  switch(pip->iptype) {
    case NETWIB_IPTYPE_IP4 :
      arrayip[0] = netwib_c2_uint32_0(pip->ipvalue.ip4);
      arrayip[1] = netwib_c2_uint32_1(pip->ipvalue.ip4);
      arrayip[2] = netwib_c2_uint32_2(pip->ipvalue.ip4);
      arrayip[3] = netwib_c2_uint32_3(pip->ipvalue.ip4);
      arraynet[0] = netwib_c2_uint32_0(pnet->ipvalue.ip4);
      arraynet[1] = netwib_c2_uint32_1(pnet->ipvalue.ip4);
      arraynet[2] = netwib_c2_uint32_2(pnet->ipvalue.ip4);
      arraynet[3] = netwib_c2_uint32_3(pnet->ipvalue.ip4);
      copyend = NETWIB_FALSE;
      for (i = 0; i < NETWIB_IP4_LEN; i++) {
        if (copyend) {
          arraymask4[i] = 0;
        } else {
          if (arrayip[i] == arraynet[i]) {
            arraymask4[i] = 255;
            prefix += 8;
          } else {
            copyend = NETWIB_TRUE;
            if (arraynet[i] == 0) {
              arraymask4[i] = 0;
            } else {
              for (j = 0; j < 8; j++) {
                if (arraynet[i] & (1<<j)) {
                  arraymask4[i] = (netwib_byte)(~((1<<j) - 1));
                  prefix += 8 - j;
                  break;
                }
              }
            }
          }
        }
      }
      if (pmask != NULL) {
        pmask->iptype = NETWIB_IPTYPE_IP4;
        pmask->ipvalue.ip4 = netwib_c2_uint32_4(arraymask4[0], arraymask4[1],
                                                arraymask4[2], arraymask4[3]);
      }
      break;
    case NETWIB_IPTYPE_IP6 :
      /* we have to use 16 bits because IPv6 addresses are separated in
         16 bit blocks */
      copyend = NETWIB_FALSE;
      for (i = 0; i < NETWIB_IP6_LEN / 2; i++) {
        ipui = netwib_c2_uint16_2(pip->ipvalue.ip6.b[2*i], pip->ipvalue.ip6.b[2*i+1]);
        netui = netwib_c2_uint16_2(pnet->ipvalue.ip6.b[2*i], pnet->ipvalue.ip6.b[2*i+1]);
        if (copyend) {
          arraymask6[i] = 0;
        } else {
          if (ipui == netui) {
            arraymask6[i] = 0xFFFF;
            prefix += 16;
          } else {
            copyend = NETWIB_TRUE;
            if (netui == 0) {
              arraymask6[i] = 0;
            } else {
              for (j = 0; j < 16; j++) {
                if (netui & (1<<j)) {
                  arraymask6[i] = (netwib_uint16)(~((1<<j) - 1));
                  prefix += 16 - j;
                  break;
                }
              }
            }
          }
        }
      }
      if (pmask != NULL) {
        pmask->iptype = NETWIB_IPTYPE_IP6;
        for (i = 0; i < NETWIB_IP6_LEN / 2; i++) {
          pmask->ipvalue.ip6.b[2*i] = netwib_c2_uint16_0(arraymask6[i]);
          pmask->ipvalue.ip6.b[2*i+1] = netwib_c2_uint16_1(arraymask6[i]);
        }
      }
      break;
    default :
      return(NETWIB_ERR_PAIPTYPE);
  }

  if (pprefix != NULL) *pprefix = prefix;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ip_maskprefix_init_sali(const netwib_priv_sockaddr_unalign *psa,
                                               netwib_priv_sockaddr_len supposedsalen,
                                               netwib_iptype supposediptype,
                                               netwib_ip *pmask,
                                               netwib_uint32 *pprefix)
{
  netwib_ip mask;

  netwib_er(netwib_priv_sa_ip_init_sali(psa, supposedsalen, supposediptype,
                                        &mask));
  if (pmask != NULL) {
    *pmask = mask;
  }
  if (pprefix != NULL) {
    netwib_er(netwib_priv_ip_prefix_init_mask(&mask, pprefix));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ip_maskprefix_init_prefix(netwib_iptype iptype,
                                                 netwib_uint32 prefix,
                                                 netwib_ip *pmask,
                                                 netwib_uint32 *pprefix)
{
  netwib_uint32 numfull, numbit, i;

  switch(iptype) {
    case NETWIB_IPTYPE_IP4 :
      if (prefix > 8*NETWIB_IP4_LEN) {
        return(NETWIB_ERR_PATOOHIGH);
      }
      netwib_er(netwib_ip_init_ip4(~ ( (1 << (32-prefix)) - 1), pmask));
      break;
    case NETWIB_IPTYPE_IP6 :
      if (prefix > 8*NETWIB_IP6_LEN) {
        return(NETWIB_ERR_PATOOHIGH);
      }
      if (pmask != NULL) {
        pmask->iptype = NETWIB_IPTYPE_IP6;
        numfull = prefix / 8;
        numbit = prefix % 8;
        for (i = 0; i < numfull; i++) {
          pmask->ipvalue.ip6.b[i] = 0xFF;
        }
        if (numbit) {
          pmask->ipvalue.ip6.b[i] = (netwib_byte)(~ ( (1 << (8-numbit)) - 1));
          i++;
        }
        for (; i < NETWIB_IP6_LEN; i++) {
          pmask->ipvalue.ip6.b[i] = 0;
        }
      }
      break;
    default :
      return(NETWIB_ERR_PAIPTYPE);
  }
  if (pprefix != NULL) {
    *pprefix = prefix;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* pbuf="1.2.3.4" => pnet=1.2.3.4, pmask=255.255.255.255
   pbuf="1.2.3" => pnet=1.2.3.0, pmask=255.255.255.0 (guess)
*/
static netwib_err netwib_priv_ip_netmaskprefix_init_bufnet(netwib_constbuf *pbuf,
                                                           netwib_ip *pnet,
                                                           netwib_ip *pmask,
                                                           netwib_uint32 *pprefix)
{
  netwib_string s;
  netwib_ip net;
  netwib_uint32 prefix;
  netwib_err ret;

  /* try with "1.2.3" */
  netwib__constbuf_ref_string(pbuf, s, bufstorage,
                              netwib_priv_ip_netmaskprefix_init_bufnet(&bufstorage, pnet, pmask, pprefix));
  ret = netwib_priv_ip_netmaskprefix_init_sip4(s, NETWIB_TRUE, pnet, pmask,
                                               pprefix);
  if (ret == NETWIB_ERR_OK) return(NETWIB_ERR_OK);
  if (ret != NETWIB_ERR_NOTCONVERTED) return(ret);

  /* try with "1.2.3.4", or an IPv6 address */
  netwib_er(netwib_ip_init_buf(pbuf, NETWIB_IP_DECODETYPE_IP, &net));
  prefix = 8* ((net.iptype==NETWIB_IPTYPE_IP4)?NETWIB_IP4_LEN:NETWIB_IP6_LEN);
  netwib_er(netwib_priv_ip_maskprefix_init_prefix(net.iptype, prefix,
                                                  pmask, pprefix));
  if (pnet != NULL) {
    *pnet = net;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ip_netmaskprefix_init_buf(netwib_constbuf *pbuf,
                                                 netwib_ip *pnet,
                                                 netwib_ip *pmask,
                                                 netwib_uint32 *pprefix)
{
  netwib_buf bufnet;
  netwib_ip net, mask;
  netwib_uint32 prefix=0;
  netwib_bool foundmask, foundprefix;
  netwib_err ret;

  netwib_er(netwib_buf_init_mallocdefault(&bufnet));

  /* obtain "net/mask", "net/prefix" or "net" */
  foundmask = NETWIB_FALSE;
  foundprefix = NETWIB_FALSE;
  ret = netwib_buf_decode_fmt(pbuf, "%{buf}/%{ip}%$", &bufnet, &mask);
  if (ret == NETWIB_ERR_OK) {
    foundmask = NETWIB_TRUE;
  } else {
    netwib__buf_reinit(&bufnet);
    ret = netwib_buf_decode_fmt(pbuf, "%{buf}/%{uint32}%$", &bufnet, &prefix);
    if (ret == NETWIB_ERR_OK) {
      foundprefix = NETWIB_TRUE;
    } else {
      netwib_er(netwib_buf_append_buf(pbuf, &bufnet));
    }
  }

  /* convert net */
  ret = netwib_priv_ip_netmaskprefix_init_bufnet(&bufnet, &net, pmask,
                                                 pprefix);
  if (ret == NETWIB_ERR_OK) {
    if (pnet != NULL) {
      *pnet = net;
    }
    if (foundmask) {
      if (pmask != NULL) {
        *pmask = mask;
      }
      if (pprefix != NULL) {
        ret = netwib_priv_ip_prefix_init_mask(&mask, pprefix);
      }
    }
    if (foundprefix) {
      ret = netwib_priv_ip_maskprefix_init_prefix(net.iptype, prefix,
                                                  pmask, pprefix);
    }
  }

  netwib_er(netwib_buf_close(&bufnet));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ip_netmaskprefix_init_bufdefault(netwib_iptype iptype,
                                                        netwib_constbuf *pbuf,
                                                        netwib_ip *pnet,
                                                        netwib_ip *pmask,
                                                       netwib_uint32 *pprefix)
{
  netwib_cmp cmp;

  netwib_er(netwib_buf_cmp_string(pbuf, "default", &cmp));
  if (cmp == NETWIB_CMP_EQ) {
    switch(iptype) {
    case NETWIB_IPTYPE_IP4 :
      netwib_er(netwib_ip_init_ip4_fields(0,0,0,0, pnet));
      netwib_er(netwib_ip_init_ip4_fields(0,0,0,0, pmask));
      if (pprefix != NULL) *pprefix = 0;
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_IPTYPE_IP6 :
      netwib_er(netwib_ip_init_ip6_fields(0,0,0,0, pnet));
      netwib_er(netwib_ip_init_ip6_fields(0,0,0,0, pmask));
      if (pprefix != NULL) *pprefix = 0;
      return(NETWIB_ERR_OK);
      break;
    default :
      return(NETWIB_ERR_PAIPTYPE);
    }
  }

  netwib_er(netwib_priv_ip_netmaskprefix_init_buf(pbuf, pnet, pmask, pprefix));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ip_prefix_init_mask(netwib_constip *pmask,
                                           netwib_uint32 *pprefix)
{
  netwib_uint32 prefix, i;

#define NETWIB_PRIV_IP_PREFIX_INIT_MASK(w,x,n) if (((w) & (x)) == (x)) {prefix += n; break;}
#define NETWIB_PRIV_IP_PREFIX_INIT_MASK2(w) { if (w != 0xFF) { NETWIB_PRIV_IP_PREFIX_INIT_MASK(w, 0xFE, 7); NETWIB_PRIV_IP_PREFIX_INIT_MASK(w, 0xFC, 6); NETWIB_PRIV_IP_PREFIX_INIT_MASK(w, 0xF8, 5); NETWIB_PRIV_IP_PREFIX_INIT_MASK(w, 0xF0, 4); NETWIB_PRIV_IP_PREFIX_INIT_MASK(w, 0xE0, 3); NETWIB_PRIV_IP_PREFIX_INIT_MASK(w, 0xC0, 2); NETWIB_PRIV_IP_PREFIX_INIT_MASK(w, 0x80, 1); NETWIB_PRIV_IP_PREFIX_INIT_MASK(w, 0x00, 0); } prefix += 8; }

  prefix = 0;
  switch(pmask->iptype) {
    case NETWIB_IPTYPE_IP4 :
      NETWIB_PRIV_IP_PREFIX_INIT_MASK2(netwib_c2_uint32_0(pmask->ipvalue.ip4));
      NETWIB_PRIV_IP_PREFIX_INIT_MASK2(netwib_c2_uint32_1(pmask->ipvalue.ip4));
      NETWIB_PRIV_IP_PREFIX_INIT_MASK2(netwib_c2_uint32_2(pmask->ipvalue.ip4));
      NETWIB_PRIV_IP_PREFIX_INIT_MASK2(netwib_c2_uint32_3(pmask->ipvalue.ip4));
      break;
    case NETWIB_IPTYPE_IP6 :
      for (i = 0; i < NETWIB_IP6_LEN; i++) {
        NETWIB_PRIV_IP_PREFIX_INIT_MASK2(pmask->ipvalue.ip6.b[i]);
      }
      break;
    default :
      return(NETWIB_ERR_PAIPTYPE);
  }

  if (pprefix != NULL) *pprefix = prefix;
  return(NETWIB_ERR_OK);
}
