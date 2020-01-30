/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
/* Notes:
    - 0 is a valid salen because it corresponds to 0.0.0.0:0
    - supposedsalen == 0xFFFFFFFFu if user does not know salen. In
      this case, we'll try to guess it.
    - supposedsalen != 0xFFFFFFFFu if user knows salen. In this
      case, this value is supposed to be good (no guess).
    - if header safamily == 0, it is guessed from salen.
    - in some cases (ie FreeBSD 4.0) safamily field in packet
      is invalid. So, we need to set supposediptype from previous
      converted entries (ie: ip address is of type IPv4, so netmask
      is also IPv4).
*/
static netwib_err netwib_priv_sa_info(const netwib_priv_sockaddr_unalign *psa,
                                      netwib_priv_sockaddr_len supposedsalen,
                                      netwib_iptype supposediptype,
                                      netwib_uint32 *pguessedsafamily,
                                      netwib_priv_sockaddr_len *pguessedsalen)
{
  struct sockaddr ssa;
  netwib_uint32 guessedsafamily, sizetocopy;
  netwib_priv_sockaddr_len guessedsalen;

  /* copy only beginning (only used here) */
  sizetocopy = sizeof(ssa.sa_family);
#if NETWIBDEF_HAVEFIELD_SOCKADDR_SALEN == 1
  sizetocopy += sizeof(ssa.sa_len);
#endif
  netwib_c_memcpy(&ssa, psa, sizetocopy);

  /* first step of salen guessing: user value is priority over
     BSD field value */
  guessedsalen = supposedsalen;
#if NETWIBDEF_HAVEFIELD_SOCKADDR_SALEN == 1
  if (supposedsalen == 0xFFFFFFFFu) {
    /* user does not know length, so take header value */
    guessedsalen = ssa.sa_len;
  }
#endif

  /* first step of safamily guessing: user value is priority over
     field value */
  guessedsafamily = ssa.sa_family;
  switch (supposediptype) {
  case NETWIB_IPTYPE_IP4 :
    guessedsafamily = AF_INET;
    break;
 #if NETWIBDEF_HAVEVAR_AF_INET6 == 1
  case NETWIB_IPTYPE_IP6 :
    guessedsafamily = AF_INET6;
    break;
 #endif
  default :
    break;
  }

  /* if family is valid, ensure salen is valid */
  switch(guessedsafamily) {
  case AF_INET :
    if ((netwib_uint32)guessedsalen == 0xFFFFFFFFu) {
      guessedsalen = sizeof(struct sockaddr_in);
    } else {
      if ((netwib_uint32)guessedsalen > (netwib_uint32)sizeof(struct sockaddr_in)) {
        guessedsalen = sizeof(struct sockaddr_in);
      }
    }
    *pguessedsafamily = AF_INET;
    *pguessedsalen = guessedsalen;
    return(NETWIB_ERR_OK);
#if NETWIBDEF_HAVETYPE_SOCKADDR_IN6 == 1
 #if NETWIBDEF_HAVEVAR_AF_INET6 == 1
  case AF_INET6 :
    if ((netwib_uint32)guessedsalen == 0xFFFFFFFFu) {
      guessedsalen = sizeof(struct sockaddr_in6);
    } else {
      if ((netwib_uint32)guessedsalen > sizeof(struct sockaddr_in6)) {
        guessedsalen = sizeof(struct sockaddr_in6);
      }
    }
    *pguessedsafamily = AF_INET6;
    *pguessedsalen = guessedsalen;
    return(NETWIB_ERR_OK);
 #endif
#endif
  case 0 :
    /* try to guess family from salen */
    if ((netwib_uint32)guessedsalen == 0xFFFFFFFFu) {
      return(NETWIB_ERR_PAIPTYPE);
    } else {
      if ((netwib_uint32)guessedsalen <= sizeof(struct sockaddr_in)) {
        *pguessedsafamily = AF_INET;
        *pguessedsalen = guessedsalen;
#if NETWIBDEF_HAVETYPE_SOCKADDR_IN6 == 1
 #if NETWIBDEF_HAVEVAR_AF_INET6 == 1
      } else if ((netwib_uint32)guessedsalen <= sizeof(struct sockaddr_in6)) {
        *pguessedsafamily = AF_INET6;
        *pguessedsalen = guessedsalen;
      } else if (guessedsalen == AF_INET6) {
        /* FreeBSD 4.0 sets AF_INET6 instead of size */
        *pguessedsafamily = AF_INET6;
        *pguessedsalen = sizeof(struct sockaddr_in6);
 #endif
#endif
      } else {
        return(NETWIB_ERR_PAIPTYPE);
      }
    }
    break;
  default :
    /* other protocols (AppleTalk, etc.) are not supported */
    return(NETWIB_ERR_PAIPTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_sa_ipport_init_sali(const netwib_priv_sockaddr_unalign *psa,
                                           netwib_priv_sockaddr_len supposedsalen,
                                           netwib_iptype supposediptype,
                                           netwib_ip *pip,
                                           netwib_port *pport)
{
  struct sockaddr_in sain;
#if NETWIBDEF_HAVETYPE_SOCKADDR_IN6 == 1
  struct sockaddr_in6 sain6;
#endif
  netwib_uint32 safamily;
  netwib_priv_sockaddr_len salen;

  netwib_er(netwib_priv_sa_info(psa, supposedsalen, supposediptype,
                                &safamily, &salen));

  /* we copy data for 2 reasons :
      - problems with strict aligned processors
      - if salen<sizeof_sockaddr, it means address ends with zeroes
  */

  switch(safamily) {
  case AF_INET :
    netwib_c_memset(&sain, 0, sizeof(struct sockaddr_in));
    netwib_c_memcpy(&sain, psa, salen);
    if (pip != NULL) {
      pip->iptype = NETWIB_IPTYPE_IP4;
      pip->ipvalue.ip4 = netwib_priv_ntohl(sain.sin_addr.s_addr);
    }
    if (pport != NULL) {
      *pport = netwib_priv_ntohs(sain.sin_port);
    }
    break;
#if NETWIBDEF_HAVETYPE_SOCKADDR_IN6 == 1
 #if NETWIBDEF_HAVEVAR_AF_INET6 == 1
  case AF_INET6 :
    netwib_c_memset(&sain6, 0, sizeof(struct sockaddr_in6));
    netwib_c_memcpy(&sain6, psa, salen);
    if (pip != NULL) {
      pip->iptype = NETWIB_IPTYPE_IP6;
      netwib_c_memcpy(pip->ipvalue.ip6.b, sain6.sin6_addr.s6_addr,
                      NETWIB_IP6_LEN);
  #if defined NETWIBDEF_SYSNAME_BSD
      if (pip->ipvalue.ip6.b[0] == 0xFE && pip->ipvalue.ip6.b[1] == 0x80) {
        /* under FreeBSD, this field contains device number : reset it */
        pip->ipvalue.ip6.b[3] = 0;
      }
  #endif
    }
    if (pport != NULL) {
      *pport = netwib_priv_ntohs(sain6.sin6_port);
    }
    break;
 #endif
#endif
  default :
    return(NETWIB_ERR_PAIPTYPE);
    break;
  }

#if NETWIBDEF_HAVETYPE_SOCKADDR_IN6 == 1
  /* in rare cases where AF_INET6 doesn't exist */
  sain6.sin6_port = 0; /* for compiler warning */
#endif
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_sa_sal_init_iptport(netwib_constip *pip,
                                           netwib_iptype iptype,
                                           netwib_port port,
                                           netwib_priv_sockaddr_align *psa,
                                           netwib_priv_sockaddr_len *psalen)
{
  struct sockaddr_in sain;
#if NETWIBDEF_HAVETYPE_SOCKADDR_IN6 == 1
  struct sockaddr_in6 sain6;
#endif

  if (pip != NULL) {
    iptype = pip->iptype;
  }

  switch(iptype) {
  case NETWIB_IPTYPE_IP4 :
    if (psa != NULL) {
      netwib_c_memset(&sain, 0, sizeof(struct sockaddr_in));
 #if NETWIBDEF_HAVEFIELD_SOCKADDRIN_SINLEN == 1
      sain.sin_len = sizeof(struct sockaddr_in);
 #endif
      sain.sin_family = AF_INET;
      sain.sin_port = netwib_priv_htons(port);
      if (pip != NULL) {
        sain.sin_addr.s_addr = netwib_priv_htonl(pip->ipvalue.ip4);
      } else {
        sain.sin_addr.s_addr = netwib_priv_htonl(INADDR_ANY);
      }
      netwib_c_memcpy(psa, &sain, sizeof(struct sockaddr_in));
    }
    if (psalen != NULL) {
      *psalen = sizeof(struct sockaddr_in);
    }
    break;
#if NETWIBDEF_HAVETYPE_SOCKADDR_IN6 == 1
 #if NETWIBDEF_HAVEVAR_AF_INET6 == 1
  case NETWIB_IPTYPE_IP6 :
    if (psa != NULL) {
      netwib_c_memset(&sain6, 0, sizeof(struct sockaddr_in6));
 #if NETWIBDEF_HAVEFIELD_SOCKADDRIN6_SIN6LEN == 1
      sain6.sin6_len = sizeof(struct sockaddr_in6);
 #endif
      sain6.sin6_family = AF_INET6;
      sain6.sin6_port = netwib_priv_htons(port);
      if (pip != NULL) {
        netwib_c_memcpy(sain6.sin6_addr.s6_addr, pip->ipvalue.ip6.b,
                        NETWIB_IP6_LEN);
      }
      netwib_c_memcpy(psa, &sain6, sizeof(struct sockaddr_in6));
    }
    if (psalen != NULL) {
      *psalen = sizeof(struct sockaddr_in6);
    }
    break;
 #endif
#endif
  default :
    return(NETWIB_ERR_PAIPTYPE);
    break;
  }

#if NETWIBDEF_HAVETYPE_SOCKADDR_IN6 == 1
  sain6.sin6_port = 0; /* for compiler warning */
#endif
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_sa_send(int fd,
                               netwib_constbuf *pbuf)
{
  netwib_data data;
  netwib_uint32 datasize;
  int reti, sendopt;

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);

#if defined MSG_NOSIGNAL
  sendopt = MSG_NOSIGNAL;
#else
  sendopt = 0;
#endif

  reti = netwib_c_send(fd, data, datasize, sendopt);
  if (reti == -1) {
    if (errno == EBADF) {
      errno = 0;
      return(NETWIB_ERR_LOOBJWRITENOTSUPPORTED);
    }
    return(NETWIB_ERR_FUSEND);
  }

  if ((netwib_uint32)reti != datasize) {
    return(NETWIB_ERR_FUSEND);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_sa_sendto(int fd,
                                 netwib_constbuf *pbuf,
                                 const netwib_priv_sockaddr_unalign *psa,
                                 netwib_priv_sockaddr_len salen)
{
  netwib_priv_sockaddr_align saalign;
  netwib_data data;
  netwib_uint32 datasize;
  int reti;

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);

  if ((netwib_uint32)salen > sizeof(saalign)) {
    return(NETWIB_ERR_LOINTERNALERROR);
  }
  netwib_c_memcpy(&saalign, psa, salen);
  reti = netwib_c_sendto(fd, data, datasize, 0,
                         (const struct sockaddr*)&saalign, salen);
  if (reti == -1) {
    if (errno == EBADF) {
      errno = 0;
      return(NETWIB_ERR_LOOBJWRITENOTSUPPORTED);
    }
    return(NETWIB_ERR_FUSENDTO);
  }

  if ((netwib_uint32)reti != datasize) {
    return(NETWIB_ERR_FUSENDTO);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_sa_recv(int fd,
                               netwib_buf *pbuf)
{
  netwib_data data;
  netwib_uint32 maxmsgsize;
  int readsize;

  netwib_er(netwib_buf_wishspace(pbuf, NETWIB_PRIV_MINMSGSIZE_READ,
                                 &data, &maxmsgsize));
  if (maxmsgsize == 0) {
    return(NETWIB_ERR_DATANOSPACE);
  }

  /* read */
  readsize = netwib_c_recv(fd, data, maxmsgsize, 0);
  /* an error occurred */
#if defined NETWIBDEF_SYSNAME_Unix
  if (readsize == -1) {
    if (errno == EAGAIN || errno == EINTR) {
      /* nothing was available */
      errno = 0;
      return(NETWIB_ERR_DATANOTAVAIL);
    } else if (errno == EBADF || errno == ECONNRESET) {
      /* reached end */
      errno = 0;
      return(NETWIB_ERR_DATAEND);
    } else {
      /* fatal error */
      return(NETWIB_ERR_FURECV);
    }
  }
#elif defined NETWIBDEF_SYSNAME_Windows
  if (readsize == SOCKET_ERROR) {
    switch(h_errno) {
    case WSAESHUTDOWN :
    case WSAECONNRESET :
      return(NETWIB_ERR_DATAEND);
    case WSAEWOULDBLOCK :
      return(NETWIB_ERR_DATANOTAVAIL);
    default :
      return(NETWIB_ERR_FURECV);
    }
  }
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  /* if we read 0, we are at end of file */
  if (readsize == 0) {
    return(NETWIB_ERR_DATAEND);
  }

  /* store read data */
  pbuf->endoffset += readsize;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_sa_recvfrom(int fd,
                                   netwib_buf *pbuf,
                                   netwib_priv_sockaddr_align *psa,
                                   netwib_priv_sockaddr_len *psalen)
{
  netwib_data data;
  netwib_uint32 maxmsgsize;
  netwib_priv_sockaddr_len salen;
  int readsize;

  netwib_er(netwib_buf_wishspace(pbuf, NETWIB_PRIV_MINMSGSIZE_READ,
                                 &data, &maxmsgsize));
  if (maxmsgsize == 0) {
    return(NETWIB_ERR_DATANOSPACE);
  }

  /* read */
  salen = sizeof(netwib_priv_sockaddr_align);
  readsize = netwib_c_recvfrom(fd, data, maxmsgsize, 0,
                               (struct sockaddr*)psa, &salen);
  /* an error occurred */
#if defined NETWIBDEF_SYSNAME_Unix
  if (readsize == -1) {
    if (errno == EAGAIN || errno == EINTR) {
      /* nothing was available */
      errno = 0;
      return(NETWIB_ERR_DATANOTAVAIL);
    } else if (errno == EBADF || errno == ECONNRESET) {
      /* reached end */
      errno = 0;
      return(NETWIB_ERR_DATAEND);
    } else {
      /* fatal error */
      return(NETWIB_ERR_FURECVFROM);
    }
  }
#elif defined NETWIBDEF_SYSNAME_Windows
  if (readsize == SOCKET_ERROR) {
    switch(h_errno) {
    case WSAESHUTDOWN :
    case WSAECONNRESET :
      return(NETWIB_ERR_DATAEND);
    case WSAEWOULDBLOCK :
      return(NETWIB_ERR_DATANOTAVAIL);
    default :
      return(NETWIB_ERR_FURECVFROM);
    }
  }
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  /* if we read 0, we are at end of file */
  if (readsize == 0) {
    return(NETWIB_ERR_DATAEND);
  }

  if (psalen != NULL) {
    *psalen = salen;
  }

  /* store read data */
  pbuf->endoffset += readsize;

  return(NETWIB_ERR_OK);
}
