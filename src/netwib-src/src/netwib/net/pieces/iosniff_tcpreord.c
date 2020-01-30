/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
#define SNIFF_TCPREORD_TIMEOUT 30 /* in seconds */
#define SNIFF_TCPREORD_NEWITEMS_CLEAN  10 /* clean hash after such new items */
#define SNIFF_TCPREORD_PKT_CLEAN  1000 /* clean hash after such packets */
#define SNIFF_TCPREORD_SEQNUM_WINDOW 100000 /* window of valid seqnum */

/*-------------------------------------------------------------*/
static netwib_err sniff_tcp_expectedseqnum(netwib_consttcphdr *ptcphdr,
                                           netwib_constbuf *ptcpdata,
                                           netwib_uint32 *pexpectedseqnum)
{

  /* note: this sum will overflow, and restart from 0. This is normal */
  *pexpectedseqnum = ptcphdr->seqnum + netwib__buf_ref_data_size(ptcpdata);
  if (ptcphdr->syn || ptcphdr->fin) {
    *pexpectedseqnum += 1;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err sniff_tcp_checksynchro(netwib_uint32 seqnum,
                                         netwib_uint32 expectedseqnum,
                                         netwib_bool *psynchro)
{
  *psynchro = NETWIB_TRUE;

  if (expectedseqnum <= 0xFFFFFFFFu - SNIFF_TCPREORD_SEQNUM_WINDOW) {
    if (seqnum > (netwib_uint32)(expectedseqnum+SNIFF_TCPREORD_SEQNUM_WINDOW)
        || /*or*/
        seqnum < expectedseqnum) {
      *psynchro = NETWIB_FALSE;
    }
  } else {
    /* this sum will overflow ; this is normal */
    if (seqnum > (netwib_uint32)(expectedseqnum+SNIFF_TCPREORD_SEQNUM_WINDOW)
        && /*and*/
        seqnum < expectedseqnum) {
      *psynchro = NETWIB_FALSE;
    }
  }

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/* Rings will contain each TCP fragment which are not yet
   synchronized.
   Following functions manage one ring item.
*/

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 seqnum; /* sequence number of first byte in pkt */
  netwib_uint32 nextexpectedseqnum; /* seqnum of last byte */
  netwib_buf pkt;
} sniff_tcp_ringitem;

/*-------------------------------------------------------------*/
static netwib_err sniff_tcp_ringitem_create(netwib_uint32 seqnum,
                                            netwib_uint32 nextexpectedseqnum,
                                            netwib_constbuf *ppkt,
                                            netwib_ptr *ppitem)
{
  netwib_ptr ptr;
  sniff_tcp_ringitem *pstri;

  netwib_er(netwib_ptr_malloc(sizeof(sniff_tcp_ringitem), &ptr));
  pstri = (sniff_tcp_ringitem *)ptr;

  pstri->seqnum = seqnum;
  pstri->nextexpectedseqnum = nextexpectedseqnum;
  netwib_er(netwib_buf_init_mallocdefault(&pstri->pkt));
  netwib_er(netwib_buf_append_buf(ppkt, &pstri->pkt));

  *ppitem = (netwib_ptr *)pstri;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err sniff_tcp_ringitem_erase(netwib_ptr pitem)
{
  sniff_tcp_ringitem *pstri;

  pstri = (sniff_tcp_ringitem *)pitem;

  netwib_er(netwib_buf_close(&pstri->pkt));

  netwib_er(netwib_ptr_free((netwib_ptr *)&pstri));
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/* Hashes will contain each ring described above.
   Following functions manage one hash item.
   Each hash item has for key: IP_src+IP_dst+PORT_src+PORT_dst.
*/

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 expireat; /* updated on each packet received for this session*/
  netwib_uint32 expectedseqnum; /* next expected seqnum for this session */
  netwib_bool ringreadytouse; /* ring is not initialized before there is
                                 one out of order TCP packet */
  netwib_ring *pring; /* ring of out of order TCP packets */
  netwib_ring_index *pringindex;
} sniff_tcp_hashitem;

/*-------------------------------------------------------------*/
#define SNIFF_TCP_HASHITEM_KEY_MAXLEN 40
static netwib_err sniff_tcp_hashitem_key(netwib_constiphdr *piphdr,
                                         netwib_consttcphdr *ptcphdr,
                                         netwib_buf *pkey)
{
  netwib_byte array[SNIFF_TCP_HASHITEM_KEY_MAXLEN];
  netwib_data data;

  data = array;
  if (piphdr->iptype == NETWIB_IPTYPE_IP4) {
    netwib__data_append_uint8(data, NETWIB_IPTYPE_IP4);
    netwib__data_append_uint32(data, piphdr->src.ipvalue.ip4);
    netwib__data_append_uint32(data, piphdr->dst.ipvalue.ip4);
    netwib__data_append_uint16(data, ptcphdr->src);
    netwib__data_append_uint16(data, ptcphdr->dst);
    netwib_er(netwib_buf_append_data(array, 1+2*NETWIB_IP4_LEN+4, pkey));
  } else {
    netwib__data_append_uint8(data, NETWIB_IPTYPE_IP6);
    netwib_c_memcpy(data, piphdr->src.ipvalue.ip6.b, NETWIB_IP6_LEN);
    data += NETWIB_IP6_LEN;
    netwib_c_memcpy(data, piphdr->dst.ipvalue.ip6.b, NETWIB_IP6_LEN);
    data += NETWIB_IP6_LEN;
    netwib__data_append_uint16(data, ptcphdr->src);
    netwib__data_append_uint16(data, ptcphdr->dst);
    netwib_er(netwib_buf_append_data(array, 1+2*NETWIB_IP6_LEN+4, pkey));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err sniff_tcp_hashitem_create(netwib_ptr *ppitem)
{
  netwib_ptr ptr;
  sniff_tcp_hashitem *psthi;

  netwib_er(netwib_ptr_malloc(sizeof(sniff_tcp_hashitem), &ptr));
  psthi = (sniff_tcp_hashitem *)ptr;

  psthi->ringreadytouse = NETWIB_FALSE;

  *ppitem = (netwib_ptr *)psthi;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err sniff_tcp_hashitem_erase(netwib_ptr pitem)
{
  sniff_tcp_hashitem *psthi;

  psthi = (sniff_tcp_hashitem *)pitem;

  if (psthi->ringreadytouse) {
    netwib_er(netwib_ring_index_close(&psthi->pringindex));
    netwib_er(netwib_ring_close(&psthi->pring, NETWIB_TRUE));
  }

  netwib_er(netwib_ptr_free((netwib_ptr *)&psthi));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* to remove items which are too old
   Instead of removing them, we could send them.
 */
static netwib_err sniff_tcp_hashitem_clean(netwib_constbuf *pkey,
                                           netwib_constptr pitem,
                                           netwib_ptr pinfos,
                                           netwib_bool *pbool)
{
  netwib_uint32 *psec = (netwib_uint32 *)pinfos;
  const sniff_tcp_hashitem *psthi = (const sniff_tcp_hashitem *)pitem;

  if (*psec > psthi->expireat) {
    *pbool = NETWIB_TRUE;
  } else {
    *pbool = NETWIB_FALSE;
  }
  pkey = pkey; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err sniff_tcp_hashitem_update(sniff_tcp_hashitem *psthi,
                                            netwib_consttcphdr *ptcphdr,
                                            netwib_constbuf *ptcpdata)
{
  netwib_time t;

  netwib_er(sniff_tcp_expectedseqnum(ptcphdr, ptcpdata,
                                     &psthi->expectedseqnum));

  netwib_er(netwib_time_init_now(&t));
  psthi->expireat = t.sec + SNIFF_TCPREORD_TIMEOUT;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err sniff_tcp_hashitem_add_packet(sniff_tcp_hashitem *psthi,
                                                netwib_consttcphdr *ptcphdr,
                                                netwib_constbuf *ptcpdata,
                                                netwib_constbuf *preadpkt)
{
  netwib_uint32 nextexpectedseqnum;
  netwib_ptr pitem;
  netwib_bool issynchro;

  /* first, check if this packet is synchronized */
  netwib_er(sniff_tcp_checksynchro(ptcphdr->seqnum, psthi->expectedseqnum,
                                   &issynchro));
  if (!issynchro) {
    /* add nothing */
    return(NETWIB_ERR_OK);
  }

  /* eventually initialize the ring */
  if (! psthi->ringreadytouse) {
    netwib_er(netwib_ring_init(&sniff_tcp_ringitem_erase, NULL,
                               &psthi->pring));
    netwib_er(netwib_ring_index_init(psthi->pring, &psthi->pringindex));
    psthi->ringreadytouse = NETWIB_TRUE;
  }

  /* add it */
  netwib_er(sniff_tcp_expectedseqnum(ptcphdr, ptcpdata, &nextexpectedseqnum));
  netwib_er(sniff_tcp_ringitem_create(ptcphdr->seqnum, nextexpectedseqnum,
                                      preadpkt, &pitem));
  netwib_er(netwib_ring_add_last(psthi->pring, pitem));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
typedef struct {
  netwib_hash *phash;
  netwib_buf readpkt;
  netwib_uint32 countnewitems; /* to purge after several new items added */
  netwib_uint32 countpkt; /* to purge after several packet seen */
  sniff_tcp_hashitem *plaststhi; /* pointer to last hash item. It will be
                                    used because after giving a fragment to
                                    user, other out of order packet, previously
                                    received, may become ordered. There are
                                    the firsts to give back to user. */
} netwib_priv_io_sniff_tcp;

/*-------------------------------------------------------------*/
static netwib_err sniff_tcp_read(netwib_priv_io_sniff_tcp *pios,
                                 netwib_device_dlttype dlt,
                                 netwib_constbuf *preadpkt,
                                 netwib_buf *pgivepkt)
{
  netwib_byte array[SNIFF_TCP_HASHITEM_KEY_MAXLEN];
  netwib_iphdr iphdr;
  netwib_tcphdr tcpheader;
  netwib_buf key, tcpdata;
  netwib_ptr pitem;
  sniff_tcp_hashitem *psthi;
  netwib_err ret;

  /* if readpkt is not ip/tcp, give it */
  ret = netwib_pkt_decode_linkiptcpdata(dlt, preadpkt,
                                        NULL, &iphdr, &tcpheader, &tcpdata);
  if (ret == NETWIB_ERR_NOTCONVERTED || ret == NETWIB_ERR_DATAMISSING
      || ret == NETWIB_ERR_LONOTIMPLEMENTED) {
    netwib_er(netwib_buf_append_buf(preadpkt, pgivepkt));
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  /* clean hash */
  if (pios->countnewitems > SNIFF_TCPREORD_NEWITEMS_CLEAN ||
      pios->countpkt > SNIFF_TCPREORD_PKT_CLEAN) {
    netwib_time t;
    netwib_er(netwib_time_init_now(&t));
    netwib_er(netwib_hash_del_criteria(pios->phash, &sniff_tcp_hashitem_clean,
                                       &t.sec, NETWIB_TRUE));
    pios->countnewitems = 0;
    pios->countpkt = 0;
  }
  pios->countpkt++;

  /* compute hash key */
  netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &key));
  netwib_er(sniff_tcp_hashitem_key(&iphdr, &tcpheader, &key));

  /* search this tcp packet in the hash */
  ret = netwib_hash_value(pios->phash, &key, &pitem);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_close(&key));
    psthi = (sniff_tcp_hashitem *)pitem;
    /* is this packet the one we want ? */
    if (tcpheader.seqnum == psthi->expectedseqnum) {
      /* give this packet to user */
      netwib_er(netwib_buf_append_buf(preadpkt, pgivepkt));
      /* update */
      netwib_er(sniff_tcp_hashitem_update(psthi, &tcpheader, &tcpdata));
      /* because now other packets can be eventually poped */
      if (psthi->ringreadytouse) {
        pios->plaststhi = psthi;
      }
      return(NETWIB_ERR_OK);
    } else {
      /* create a new entry in the ring (except if not synchronized) */
      netwib_er(sniff_tcp_hashitem_add_packet(psthi, &tcpheader, &tcpdata,
                                              preadpkt));
      return(NETWIB_ERR_DATANOTAVAIL);
    }
  } else if (ret == NETWIB_ERR_NOTFOUND) {
    /* create a new entry */
    netwib_er(sniff_tcp_hashitem_create(&pitem));
    psthi = (sniff_tcp_hashitem *)pitem;
    netwib_er(sniff_tcp_hashitem_update(psthi, &tcpheader, &tcpdata));
    /* add it in the hash */
    netwib_er(netwib_hash_add(pios->phash, &key, pitem, NETWIB_TRUE));
    pios->countnewitems++;
    /* give this packet to user */
    netwib_er(netwib_buf_append_buf(preadpkt, pgivepkt));
    return(NETWIB_ERR_OK);
  } else {
    return(ret);
  }
  /* never reached */

  return(NETWIB_ERR_LOINTERNALERROR);
}

/*-------------------------------------------------------------*/
static netwib_err sniff_tcp_read_pending2(netwib_constptr pitem,
                                          netwib_ptr pinfos,
                                          netwib_bool *pbool)
{
  const sniff_tcp_ringitem *pstri = (const sniff_tcp_ringitem *)pitem;
  netwib_uint32 *pseqnum = (netwib_uint32*)pinfos;
  netwib_bool issynchro;

  if (pstri->seqnum == *pseqnum) {
    *pbool = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  /* trick to force erase of packets no more synchronized */
  netwib_er(sniff_tcp_checksynchro(pstri->seqnum, *pseqnum, &issynchro));
  if (issynchro) {
    *pbool = NETWIB_FALSE;
  } else {
    /* will detect it and delete it */
    *pbool = NETWIB_TRUE;
  }

  return(NETWIB_ERR_OK);
}
static netwib_err sniff_tcp_read_pending(netwib_priv_io_sniff_tcp *pios,
                                         netwib_buf *pgivepkt)
{
  netwib_ptr pitem;
  sniff_tcp_ringitem *pstri;
  netwib_err ret;

  netwib_er(netwib_ring_index_ctl_set_rewind(pios->plaststhi->pringindex));

  while(NETWIB_TRUE) {
    ret = netwib_ring_index_next_criteria(pios->plaststhi->pringindex,
                                          &sniff_tcp_read_pending2,
                                          &pios->plaststhi->expectedseqnum,
                                          &pitem);
    if (ret == NETWIB_ERR_DATAEND) {
      /* nothing was found, indicate end */
      pios->plaststhi = NULL;
      return(NETWIB_ERR_DATANOTAVAIL);
    } else if (ret != NETWIB_ERR_OK) {
      return(ret);
    }
    pstri = (sniff_tcp_ringitem *)pitem;
    /* check if this packet is ok */
    if (pstri->seqnum == pios->plaststhi->expectedseqnum) {
      /* give this packet to user */
      netwib_er(netwib_buf_append_buf(&pstri->pkt, pgivepkt));
      /* update expectedseqnum */
      pios->plaststhi->expectedseqnum = pstri->nextexpectedseqnum;
      /* remove entry */
      netwib_er(netwib_ring_index_this_del(pios->plaststhi->pringindex,
                                           NETWIB_TRUE));
      return(NETWIB_ERR_OK);
    }
    /* trick (in sniff_tcp_read_pending2): this packet was not synchronized */
    netwib_er(netwib_ring_index_this_del(pios->plaststhi->pringindex,
                                         NETWIB_TRUE));
  }

  return(NETWIB_ERR_LOINTERNALERROR);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sniff_tcp_init(netwib_bool *preadinitialized,
                                                netwib_bool *pwriteinitialized,
                                                netwib_priv_io_sniff_tcp *ptr)
{

  /* hash storing packets */
  netwib_er(netwib_hash_init(&sniff_tcp_hashitem_erase, NULL, &ptr->phash));

  /* buffer used to retrieve packets */
  netwib_er(netwib_buf_init_mallocdefault(&ptr->readpkt));

  /* used to clean hash */
  ptr->countnewitems = 0;
  ptr->countpkt = 0;

  /* used for the wait, and to give back packets related to one session */
  ptr->plaststhi = NULL;

  /* set io values */
  *preadinitialized = NETWIB_TRUE;
  *pwriteinitialized = NETWIB_FALSE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sniff_tcp_close(netwib_priv_io_sniff_tcp *ptr)
{

  netwib_er(netwib_buf_close(&ptr->readpkt));
  netwib_er(netwib_hash_close(&ptr->phash, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sniff_tcp_read(netwib_io *pio,
                                                netwib_buf *pbuf)
{
  netwib_priv_io_sniff_tcp *ptr = (netwib_priv_io_sniff_tcp *)pio->pcommon;
  netwib_device_dlttype dlt;

  if (ptr->plaststhi != NULL) {
    /* there is eventually a pending packet to give */
    return(sniff_tcp_read_pending(ptr, pbuf));
  }

  netwib_er(netwib_sniff_ctl_get_dlt(pio->rd.pnext, &dlt));

  netwib__buf_reinit(&ptr->readpkt);
  netwib_er(netwib_io_read(pio->rd.pnext, &ptr->readpkt));
  /*
  netwib_er(netwib_fmt_display("RD "));
  netwib_er(netwib_pkt_link_display(dlt, &ptr->readpkt,
                                  NULL, NETWIB_ENCODETYPE_SYNTH,
                                  NETWIB_ENCODETYPE_SYNTH));
  */

  netwib_er(sniff_tcp_read(ptr, dlt, &ptr->readpkt, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sniff_tcp_wait(netwib_io *pio,
                                                netwib_io_waytype way,
                                                netwib_consttime *pabstime,
                                                netwib_bool *pevent)
{
  netwib_priv_io_sniff_tcp *ptr = (netwib_priv_io_sniff_tcp *)pio->pcommon;

  if (ptr->plaststhi != NULL) {
    /* we perhaps have some more packets to give */
    *pevent = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  way = way; /* for compiler warning */
  pabstime = pabstime; /* for compiler warning */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sniff_tcp_fclose(netwib_io *pio)
{
  netwib_priv_io_sniff_tcp *ptr = (netwib_priv_io_sniff_tcp *)pio->pcommon;

  netwib_er(netwib_priv_io_sniff_tcp_close(ptr));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_sniff_tcpreord(netwib_io **ppio)
{
  netwib_bool rdinit, wrinit;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_sniff_tcp), &pcommon));
  ret = netwib_priv_io_sniff_tcp_init(&rdinit, &wrinit,
                                      (netwib_priv_io_sniff_tcp *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           &netwib_priv_io_sniff_tcp_read,
                           NULL, /* write */
                           &netwib_priv_io_sniff_tcp_wait,
                           NULL, /* unread */
                           NULL, /* ctl_set */
                           NULL, /* ctl_get */
                           &netwib_priv_io_sniff_tcp_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

