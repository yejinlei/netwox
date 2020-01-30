/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ips_array_init_ip(netwib_constip *pip,
                                         netwib_byte array[])
{
  /* note : IPv4 address storage start with 0, and IPv6 start
     with 1 */

  switch(pip->iptype) {
    case NETWIB_IPTYPE_IP4 :
      netwib_c_memset(array, 0, 13);
      array[13] = netwib_c2_uint32_0(pip->ipvalue.ip4);
      array[14] = netwib_c2_uint32_1(pip->ipvalue.ip4);
      array[15] = netwib_c2_uint32_2(pip->ipvalue.ip4);
      array[16] = netwib_c2_uint32_3(pip->ipvalue.ip4);
      break;
    case NETWIB_IPTYPE_IP6 :
      array[0] = 1;
      netwib_c_memcpy(array+1, pip->ipvalue.ip6.b, NETWIB_IP6_LEN);
      break;
    default :
      return(NETWIB_ERR_PAIPTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ips_ip_init_array(netwib_byte array[],
                                         netwib_ip *pip)
{
  switch(array[0]) {
    case 0 :
      if (pip != NULL) {
        pip->iptype = NETWIB_IPTYPE_IP4;
        pip->ipvalue.ip4 = netwib_c2_uint32_4(array[13], array[14],
                                             array[15], array[16]);
      }
      break;
    case 1 :
      if (pip != NULL) {
        pip->iptype = NETWIB_IPTYPE_IP6;
        netwib_c_memcpy(pip->ipvalue.ip6.b, array+1, NETWIB_IP6_LEN);
      }
      break;
    default :
      return(NETWIB_ERR_LOINTERNALERROR);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_ips_add_string_all(netwib_priv_ranges *pr,
                                                 netwib_conststring str,
                                                 netwib_bool usedel)
{
  netwib_byte infarray[NETWIB_IPS_LEN], suparray[NETWIB_IPS_LEN];
  netwib_ip ipa;
  netwib_err ret = NETWIB_ERR_OK;

  if (!netwib_c_strcmp(str, "all") || !netwib_c_strcmp(str, "any")) {
    netwib_er(netwib_ip_init_ip4_fields(0, 0, 0, 0, &ipa));
    netwib_er(netwib_priv_ips_array_init_ip(&ipa, infarray));
    netwib_er(netwib_ip_init_ip4_fields(0xFF, 0xFF, 0xFF, 0xFF, &ipa));
    netwib_er(netwib_priv_ips_array_init_ip(&ipa, suparray));
    if (usedel) {
      ret = netwib_priv_ranges_del_range(pr, infarray, suparray);
    } else {
      ret = netwib_priv_ranges_add_range(pr, infarray, suparray);
    }
    if (ret != NETWIB_ERR_OK) return(ret);
    netwib_er(netwib_ip_init_ip6_fields(0, 0, 0, 0, &ipa));
    netwib_er(netwib_priv_ips_array_init_ip(&ipa, infarray));
    netwib_er(netwib_ip_init_ip6_fields(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu,
                                        0xFFFFFFFFu, &ipa));
    netwib_er(netwib_priv_ips_array_init_ip(&ipa, suparray));
    if (usedel) {
      ret = netwib_priv_ranges_del_range(pr, infarray, suparray);
    } else {
      ret = netwib_priv_ranges_add_range(pr, infarray, suparray);
    }
    return(ret);
  }

  return(NETWIB_ERR_NOTCONVERTED);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_ips_add_string_ip4(netwib_priv_ranges *pr,
                                                 netwib_conststring str,
                                                 netwib_bool usedel)
{
  netwib_byte array[2*NETWIB_IP4_LEN];
  netwib_byte infarray[NETWIB_IPS_LEN], suparray[NETWIB_IPS_LEN];
  netwib_ip ipa;
  netwib_ip4 infip4, supip4, mask=0;
  netwib_conststring pc;
  netwib_uint32 i, separator, numdigitfound, fieldvalue, num;
  netwib_char c;
  netwib_err ret = NETWIB_ERR_OK;

  /* a range is of the form :
       1.2.3.4
       1.2.3.4-5.6.7.8
       1.2.3.0/255.255.255.0
       1.2.3.0/24
       1.2.3/255.255.255.0
       1.2.3/24
       1.2.3.0%255.255.255.O
       1.2.3.0%24
       1.2.3%255.255.255.O
       1.2.3%24
   */

  /* first set default (not needed for mask) */
  netwib_c_memset(array, 0, NETWIB_IP4_LEN);

  /* main loop */
  i = 0;
  separator = NETWIB_PRIV_RANGES_SEPARATOR_NONE;
  numdigitfound = 0;
  fieldvalue = 0;
  pc = str;
  while(NETWIB_TRUE) {
    c = *pc;
    if (c == '.' || c == '\0' || c == '-' || c == '/' || c == '%') {
      /* generic error checking */
      if (i >= 2*NETWIB_IP4_LEN || numdigitfound == 0) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      array[i++] = (netwib_byte)fieldvalue;
      fieldvalue = 0;
      numdigitfound = 0;
      /* specific error checking */
      if (c == '\0') {
        break;
      } else if (c == '.') {
        if (i == NETWIB_IP4_LEN) return(NETWIB_ERR_NOTCONVERTED);
      } else if (c == '-') {
        if (i != NETWIB_IP4_LEN) return(NETWIB_ERR_NOTCONVERTED);
        if (separator != NETWIB_PRIV_RANGES_SEPARATOR_NONE) {
          return(NETWIB_ERR_NOTCONVERTED); /* forbid two separators */
        }
        separator = NETWIB_PRIV_RANGES_SEPARATOR_DASH;
      } else if (c == '/') {
        if (i > NETWIB_IP4_LEN) return(NETWIB_ERR_NOTCONVERTED);
        if (separator != NETWIB_PRIV_RANGES_SEPARATOR_NONE) {
          return(NETWIB_ERR_NOTCONVERTED); /* forbid two separators */
        }
        separator = NETWIB_PRIV_RANGES_SEPARATOR_SLASH;
        i = NETWIB_IP4_LEN;
      } else if (c == '%') {
        if (i > NETWIB_IP4_LEN) return(NETWIB_ERR_NOTCONVERTED);
        if (separator != NETWIB_PRIV_RANGES_SEPARATOR_NONE) {
          return(NETWIB_ERR_NOTCONVERTED); /* forbid two separators */
        }
        separator = NETWIB_PRIV_RANGES_SEPARATOR_PERCENT;
        i = NETWIB_IP4_LEN;
      }
    } else if (netwib_c2_isdigit(c)) {
      if (numdigitfound == 3) return(NETWIB_ERR_NOTCONVERTED);
      fieldvalue = fieldvalue*10 + netwib_c2_cto9(c);
      if (fieldvalue > 255) return(NETWIB_ERR_NOTCONVERTED);
      numdigitfound++;
    } else {
      return(NETWIB_ERR_NOTCONVERTED);
    }
    pc++;
  }

  /* final error checking */
  switch(separator) {
    case NETWIB_PRIV_RANGES_SEPARATOR_NONE :
      if (i != NETWIB_IP4_LEN) return(NETWIB_ERR_NOTCONVERTED);
      break;
    case NETWIB_PRIV_RANGES_SEPARATOR_DASH :
      if (i != 2*NETWIB_IP4_LEN) return(NETWIB_ERR_NOTCONVERTED);
      break;
    case NETWIB_PRIV_RANGES_SEPARATOR_SLASH :
      if (i == NETWIB_IP4_LEN+1) {
        separator = NETWIB_PRIV_RANGES_SEPARATOR_SLASH_NUM;
      } else if (i != 2*NETWIB_IP4_LEN) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      break;
    case NETWIB_PRIV_RANGES_SEPARATOR_PERCENT :
      if (i == NETWIB_IP4_LEN+1) {
        separator = NETWIB_PRIV_RANGES_SEPARATOR_PERCENT_NUM;
      } else if (i != 2*NETWIB_IP4_LEN) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      break;
    default :
      return(NETWIB_ERR_LOINTERNALERROR);
  }

  /* convert array to ip4 */
  infip4 = netwib_c2_uint32_4(array[0], array[1], array[2], array[3]);
  supip4 = netwib_c2_uint32_4(array[4], array[5], array[6], array[7]);

  /* eventually reset ip4 values */
  switch(separator) {
    case NETWIB_PRIV_RANGES_SEPARATOR_SLASH :
    case NETWIB_PRIV_RANGES_SEPARATOR_PERCENT :
      mask = supip4;
      break;
    case NETWIB_PRIV_RANGES_SEPARATOR_SLASH_NUM :
    case NETWIB_PRIV_RANGES_SEPARATOR_PERCENT_NUM :
      num = (array+NETWIB_IP4_LEN)[0];
      if (num > 32) {
        return(NETWIB_ERR_NOTCONVERTED);
      }
      if (num == 0) {
        mask = 0;
      } else {
        mask = ~ ( (1 << (32-num)) - 1);
      }
      break;
  }
  switch(separator) {
    case NETWIB_PRIV_RANGES_SEPARATOR_NONE :
      supip4 = infip4;
      break;
    case NETWIB_PRIV_RANGES_SEPARATOR_DASH :
      /* nothing to change */
      break;
    case NETWIB_PRIV_RANGES_SEPARATOR_SLASH :
    case NETWIB_PRIV_RANGES_SEPARATOR_PERCENT :
    case NETWIB_PRIV_RANGES_SEPARATOR_SLASH_NUM :
    case NETWIB_PRIV_RANGES_SEPARATOR_PERCENT_NUM :
      infip4 = infip4 & mask;
      supip4 = infip4 | (~mask);
      break;
  }
  switch(separator) {
    case NETWIB_PRIV_RANGES_SEPARATOR_PERCENT :
    case NETWIB_PRIV_RANGES_SEPARATOR_PERCENT_NUM :
      infip4++;
      supip4--;
      break;
  }

  /* add item */
  netwib_er(netwib_ip_init_ip4(infip4, &ipa));
  netwib_er(netwib_priv_ips_array_init_ip(&ipa, infarray));
  netwib_er(netwib_ip_init_ip4(supip4, &ipa));
  netwib_er(netwib_priv_ips_array_init_ip(&ipa, suparray));
  if (usedel) {
    ret = netwib_priv_ranges_del_range(pr, infarray, suparray);
  } else {
    ret = netwib_priv_ranges_add_range(pr, infarray, suparray);
  }

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_ips_add_string_ip6(netwib_priv_ranges *pr,
                                                 netwib_conststring str,
                                                 netwib_bool usedel)
{
  netwib_byte infarray[NETWIB_IPS_LEN], suparray[NETWIB_IPS_LEN];
  netwib_ip infip, supip;
  netwib_buf buf;
  netwib_string pc, pcbuf;
  netwib_err ret;

  /* special case of range */
  pc = netwib_c_strchr(str, '-');
  if (pc != NULL) {
    supip.iptype = NETWIB_IPTYPE_IP6; /* for compiler warning */
    netwib_er(netwib_buf_init_mallocdefault(&buf));
    netwib_er(netwib_buf_append_string(str, &buf));
    netwib_er(netwib_buf_ref_string(&buf, &pcbuf));
    pc = netwib_c_strchr(pcbuf, '-');
    *pc++ = '\0';
    ret = netwib_priv_ip_init_sip6(pcbuf, &infip);
    if (ret == NETWIB_ERR_OK) {
      ret = netwib_priv_ip_init_sip6(pc, &supip);
    }
    netwib_er(netwib_buf_close(&buf));
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_priv_ips_array_init_ip(&infip, infarray));
      netwib_er(netwib_priv_ips_array_init_ip(&supip, suparray));
      if (usedel) {
        ret = netwib_priv_ranges_del_range(pr, infarray, suparray);
      } else {
        ret = netwib_priv_ranges_add_range(pr, infarray, suparray);
      }
    }
    return(ret);
  }

  /* address alone */
  ret = netwib_priv_ip_init_sip6(str, &infip);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_priv_ips_array_init_ip(&infip, infarray));
    if (usedel) {
      ret = netwib_priv_ranges_del(pr, infarray);
    } else {
      ret = netwib_priv_ranges_add(pr, infarray);
    }
  }

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_ips_add_string_hns(netwib_priv_ranges *pr,
                                                 netwib_conststring hn,
                                                 netwib_bool usedel)
{
  netwib_byte array[NETWIB_IPS_LEN];
  netwib_ip ipa;
  netwib_err ret;

  ret = NETWIB_ERR_OK;
#if NETWIBDEF_HAVEFUNC_GETADDRINFO == 1
  {
    struct addrinfo hints, *pres, *pcur;
    struct sockaddr *psa;
    int reti;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = PF_UNSPEC;
    reti = getaddrinfo(hn, NULL, &hints, &pres);
    if (reti) {
      return(NETWIB_ERR_NOTCONVERTED);
    }

    pcur = pres;
    while(pcur != NULL) {
      psa = pcur->ai_addr;
      ret = netwib_priv_sa_ip_init_sa((const netwib_priv_sockaddr_unalign *)psa, &ipa);
      if (ret == NETWIB_ERR_OK) {
        ret = netwib_priv_ips_array_init_ip(&ipa, array);
        if (ret == NETWIB_ERR_OK) {
          if (usedel) {
            ret = netwib_priv_ranges_del(pr, array);
          } else {
            ret = netwib_priv_ranges_add(pr, array);
          }
        }
        if (ret != NETWIB_ERR_OK) {
          break;
        }
      } else if (ret == NETWIB_ERR_PAINVALIDTYPE ||
                 ret == NETWIB_ERR_PAIPTYPE) {
        /* note : we can have PAINVALIDTYPE, because PF_UNSPEC can return
           Unix sockets */
        ret = NETWIB_ERR_OK;
      } else {
        break;
      }
      pcur = pcur->ai_next;
    }
    freeaddrinfo(pres);
  }
#elif NETWIBDEF_HAVEFUNC_GETIPNODEBYNAME == 1
  {
    netwib_uint32 i;
    struct hostent *phe;
    int error_num;

    phe = getipnodebyname(hn, AF_INET6, AI_V4MAPPED|AI_ALL, &error_num);
    if (phe == NULL) {
      return(NETWIB_ERR_NOTCONVERTED);
    }

    i = 0;
    while (phe->h_addr_list[i] != NULL) {
      ip.iptype = NETWIB_IPTYPE_IP6;
      netwib_c_memset(ipa.ipvalue.ip6.b, phe->h_addr_list[i], NETWIB_IP6_LEN);
      ret = netwib_priv_ips_array_init_ip(&ipa, array);
      if (ret == NETWIB_ERR_OK) {
        if (usedel) {
          ret = netwib_priv_ranges_del(pr, array);
        } else {
          ret = netwib_priv_ranges_add(pr, array);
        }
      }
      if (ret != NETWIB_ERR_OK) {
        break;
      }
      i++;
    }

    freehostent(phe);
  }
#elif NETWIBDEF_HAVEFUNC_GETHOSTBYNAME_R == 1
  {
    netwib_uint32 mallocsize, i;
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

    i = 0;
    while (entry.h_addr_list[i] != NULL) {
      ipa.iptype = NETWIB_IPTYPE_IP4;
      ipa.ipvalue.ip4 = netwib_c2_uint32_4((netwib_byte)entry.h_addr_list[i][0], (netwib_byte)entry.h_addr_list[i][1], (netwib_byte)entry.h_addr_list[i][2], (netwib_byte)entry.h_addr_list[i][3]);
      ret = netwib_priv_ips_array_init_ip(&ipa, array);
      if (ret == NETWIB_ERR_OK) {
        if (usedel) {
          ret = netwib_priv_ranges_del(pr, array);
        } else {
          ret = netwib_priv_ranges_add(pr, array);
        }
      }
      if (ret != NETWIB_ERR_OK) {
        break;
      }
      i++;
    }

    netwib_er(netwib_ptr_free(&ptr));
  }
#else
  {
    netwib_uint32 i;
    struct hostent *phe;

    netwib_er(netwib_priv_glovars_other_wrlock());
    phe = netwib_gethostbyname(hn);
    if (phe == NULL) {
      netwib_er(netwib_priv_glovars_other_wrunlock());
      return(NETWIB_ERR_NOTCONVERTED);
    }

    i = 0;
    while (phe->h_addr_list[i] != NULL) {
      ipa.iptype = NETWIB_IPTYPE_IP4;
      ipa.ipvalue.ip4 = netwib_c2_uint32_4((netwib_byte)phe->h_addr_list[i][0],
                                          (netwib_byte)phe->h_addr_list[i][1],
                                          (netwib_byte)phe->h_addr_list[i][2],
                                          (netwib_byte)phe->h_addr_list[i][3]);
      ret = netwib_priv_ips_array_init_ip(&ipa, array);
      if (ret == NETWIB_ERR_OK) {
        if (usedel) {
          ret = netwib_priv_ranges_del(pr, array);
        } else {
          ret = netwib_priv_ranges_add(pr, array);
        }
      }
      if (ret != NETWIB_ERR_OK) {
        break;
      }
      i++;
    }

    netwib_er(netwib_priv_glovars_other_wrunlock());
  }
#endif

   return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_ips_add_string_hn(netwib_priv_ranges *pr,
                                                netwib_conststring str,
                                                netwib_bool usedel)
{
  netwib_byte infarray[NETWIB_IPS_LEN], suparray[NETWIB_IPS_LEN];
  netwib_ip infip, supip;
  netwib_buf buf;
  netwib_string pc, pcbuf;
  netwib_err ret;

  pc = netwib_c_strchr(str, '=');
  if (pc == NULL) {
    netwib_er(netwib_priv_ips_add_string_hns(pr, str, usedel));
    return(NETWIB_ERR_OK);
  }

  supip.iptype = NETWIB_IPTYPE_IP4; /* for compiler warning */
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_string(str, &buf));
  netwib_er(netwib_buf_ref_string(&buf, &pcbuf));
  pc = netwib_c_strchr(pcbuf, '=');
  *pc++ = '\0';
  ret = netwib_priv_ip_init_hn4(pcbuf, &infip);
  if (ret != NETWIB_ERR_OK) {
    ret = netwib_priv_ip_init_hn6(pcbuf, &infip);
  }
  if (ret == NETWIB_ERR_OK) {
    ret = netwib_priv_ip_init_hn4(pc, &supip);
    if (ret != NETWIB_ERR_OK) {
      ret = netwib_priv_ip_init_hn6(pc, &supip);
    }
  }
  netwib_er(netwib_buf_close(&buf));

  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_priv_ips_array_init_ip(&infip, infarray));
    netwib_er(netwib_priv_ips_array_init_ip(&supip, suparray));
    if (usedel) {
      ret = netwib_priv_ranges_del_range(pr, infarray, suparray);
    } else {
      ret = netwib_priv_ranges_add_range(pr, infarray, suparray);
    }
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_ips_add_string(netwib_priv_ranges *pr,
                                      netwib_conststring str,
                                      netwib_bool usedel)
{
  netwib_err ret;

  ret = netwib_priv_ips_add_string_all(pr, str, usedel);
  if (ret == NETWIB_ERR_OK) return(NETWIB_ERR_OK);
  if (ret != NETWIB_ERR_NOTCONVERTED) return(ret);

  ret = netwib_priv_ips_add_string_ip4(pr, str, usedel);
  if (ret == NETWIB_ERR_OK) return(NETWIB_ERR_OK);
  if (ret != NETWIB_ERR_NOTCONVERTED) return(ret);

  ret = netwib_priv_ips_add_string_ip6(pr, str, usedel);
  if (ret == NETWIB_ERR_OK) return(NETWIB_ERR_OK);
  if (ret != NETWIB_ERR_NOTCONVERTED) return(ret);

  ret = netwib_priv_ips_add_string_hn(pr, str, usedel);
  if (ret == NETWIB_ERR_OK) return(NETWIB_ERR_OK);
  if (ret != NETWIB_ERR_NOTCONVERTED) return(ret);

  return(ret);
}
