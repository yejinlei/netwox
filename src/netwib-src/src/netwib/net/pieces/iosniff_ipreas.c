/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
#define SNIFF_IPREAS_TIMEOUT 30 /* in seconds */
#define SNIFF_IPREAS_NEWITEMS_CLEAN 10 /* clean hash after such new items */
#define SNIFF_IPREAS_PKT_CLEAN 1000 /* clean hash after such packets */

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bool firstfragseen;
  netwib_bool lastfragseen;
  netwib_linkhdr linkhdr;
  netwib_iphdr iphdr;
  netwib_bool ipextsallocated;
  netwib_buf ipexts; /* or ipopts, allocated only if needed */
  netwib_char ipdataset[8192 + 1]; /* +1 because use string */
  netwib_byte ipdataarray[65536];
  netwib_uint32 ipdatasize;
} sniff_ipreas_item_frag;
typedef struct {
  netwib_uint32 expireat; /* only set on creation, so we have at most
                             SNIFF_IPREAS_TIMEOUT seconds to reassemble */
  sniff_ipreas_item_frag *pfrag;
  netwib_bool sent;
} sniff_ipreas_item;

/*-------------------------------------------------------------*/
#define SNIFF_IPREAS_ITEM_KEY_MAXLEN 40
static netwib_err sniff_ipreas_item_key(netwib_constiphdr *piphdr,
                                        netwib_constip6ext *pip6extfrag,
                                        netwib_buf *pkey)
{
  netwib_byte array[SNIFF_IPREAS_ITEM_KEY_MAXLEN];
  netwib_data data;

  data = array;
  if (piphdr->iptype == NETWIB_IPTYPE_IP4) {
    netwib__data_append_uint8(data, NETWIB_IPTYPE_IP4);
    netwib__data_append_uint32(data, piphdr->src.ipvalue.ip4);
    netwib__data_append_uint32(data, piphdr->dst.ipvalue.ip4);
    netwib__data_append_uint8(data, piphdr->protocol);
    netwib__data_append_uint16(data, piphdr->header.ip4.id);
    netwib_er(netwib_buf_append_data(array, 12, pkey));
  } else {
    netwib__data_append_uint8(data, NETWIB_IPTYPE_IP6);
    netwib_c_memcpy(data, piphdr->src.ipvalue.ip6.b, NETWIB_IP6_LEN);
    data += NETWIB_IP6_LEN;
    netwib_c_memcpy(data, piphdr->dst.ipvalue.ip6.b, NETWIB_IP6_LEN);
    data += NETWIB_IP6_LEN;
    netwib__data_append_uint32(data, pip6extfrag->ext.fragment.id);
    netwib_er(netwib_buf_append_data(array, 1+2*NETWIB_IP6_LEN+4, pkey));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err sniff_ipreas_item_create(netwib_ptr *ppitem)
{
  netwib_ptr ptr;
  netwib_time t;
  sniff_ipreas_item *psii;

  netwib_er(netwib_ptr_malloc(sizeof(sniff_ipreas_item), &ptr));
  psii = (sniff_ipreas_item *)ptr;

  netwib_er(netwib_time_init_now(&t));
  psii->expireat = t.sec + SNIFF_IPREAS_TIMEOUT;
  psii->sent = NETWIB_FALSE;

  netwib_er(netwib_ptr_malloc(sizeof(sniff_ipreas_item_frag),
                              (netwib_ptr*)&psii->pfrag));
  netwib_c_memset(psii->pfrag->ipdataset, 0, sizeof(psii->pfrag->ipdataset));
  psii->pfrag->firstfragseen = NETWIB_FALSE;
  psii->pfrag->lastfragseen = NETWIB_FALSE;
  psii->pfrag->ipextsallocated = NETWIB_FALSE;

  *ppitem = (netwib_ptr *)psii;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err sniff_ipreas_item_erase(netwib_ptr pitem)
{
  sniff_ipreas_item *psii;

  psii = (sniff_ipreas_item *)pitem;

  if (psii->pfrag->ipextsallocated) {
    netwib_er(netwib_buf_close(&psii->pfrag->ipexts));
  }
  netwib_er(netwib_ptr_free((netwib_ptr*)&psii->pfrag));

  netwib_er(netwib_ptr_free((netwib_ptr *)&psii));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err sniff_ipreas_item_clean(netwib_constbuf *pkey,
                                          netwib_constptr pitem,
                                          netwib_ptr pinfos,
                                          netwib_bool *pbool)
{
  netwib_uint32 *psec = (netwib_uint32 *)pinfos;
  const sniff_ipreas_item *psii = (const sniff_ipreas_item *)pitem;

  if (*psec > psii->expireat) {
    *pbool = NETWIB_TRUE;
  } else {
    *pbool = NETWIB_FALSE;
  }
  pkey = pkey; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err sniff_ipreas_item_reas(sniff_ipreas_item *psii,
                                         netwib_constlinkhdr *plinkhdr,
                                         netwib_iphdr *piphdr,
                                         netwib_constbuf *preadpkt,
                                         netwib_constip6ext *pip6extfrag,
                                         netwib_buf *pgivepkt)
{
  netwib_buf ipdata;
  netwib_data data;
  netwib_uint32 datasize;

  if (psii->sent) {
    return(NETWIB_ERR_DATANOTAVAIL);
  }

  /* correct headers are only retrieved from first fragment */
  if (pip6extfrag->ext.fragment.fragmentoffset == 0 &&
      !psii->pfrag->firstfragseen) {
    psii->pfrag->linkhdr = *plinkhdr;
    psii->pfrag->iphdr = *piphdr;
    if (piphdr->iptype == NETWIB_IPTYPE_IP4) {
      psii->pfrag->iphdr.header.ip4.morefrag = NETWIB_FALSE;
      if (psii->pfrag->iphdr.header.ip4.ihl > 5) {
        netwib_er(netwib_buf_init_mallocdefault(&psii->pfrag->ipexts));
        psii->pfrag->ipextsallocated = NETWIB_TRUE;
        netwib_er(netwib_buf_append_buf(&psii->pfrag->iphdr.header.ip4.opts,
                                        &psii->pfrag->ipexts));
        netwib_er(netwib_buf_init_ext_buf(&psii->pfrag->ipexts,
                                        &psii->pfrag->iphdr.header.ip4.opts));
      } else {
        netwib_er(netwib_buf_init_ext_empty(&psii->pfrag->iphdr.header.ip4.opts));
      }
    } else {
      if (netwib__buf_ref_data_size(&psii->pfrag->iphdr.header.ip6.exts)) {
        netwib_er(netwib_buf_init_mallocdefault(&psii->pfrag->ipexts));
        psii->pfrag->ipextsallocated = NETWIB_TRUE;
        netwib_er(netwib_buf_append_buf(&psii->pfrag->iphdr.header.ip6.exts,
                                        &psii->pfrag->ipexts));
        netwib_er(netwib_buf_init_ext_buf(&psii->pfrag->ipexts,
                                        &psii->pfrag->iphdr.header.ip6.exts));
      } else {
        netwib_er(netwib_buf_init_ext_empty(&psii->pfrag->iphdr.header.ip6.exts));
      }
      netwib_er(netwib_iphdr_set_proto(&psii->pfrag->iphdr,
                                       pip6extfrag->nextproto));
    }
    psii->pfrag->firstfragseen = NETWIB_TRUE;
  }

  /* ipdatasize can only be computed from last fragment */
  data = netwib__buf_ref_data_ptr(preadpkt);
  datasize = netwib__buf_ref_data_size(preadpkt);
  if (!pip6extfrag->ext.fragment.morefrag && !psii->pfrag->lastfragseen) {
    psii->pfrag->ipdatasize = 8*pip6extfrag->ext.fragment.fragmentoffset
      + datasize;
    psii->pfrag->lastfragseen = NETWIB_TRUE;
  }

  /* save fragment data. We do not bother to check if fragments contains
     overlapping data. Recent data overwrites old data. */
  netwib_c_memcpy(psii->pfrag->ipdataarray + 8*pip6extfrag->ext.fragment.fragmentoffset, data, datasize);
  netwib_c_memset(psii->pfrag->ipdataset + pip6extfrag->ext.fragment.fragmentoffset, 1, (datasize + 7)/8);

  /* check if there is a complete packet now */
  if (psii->pfrag->lastfragseen) {
    if ( netwib_c_strlen(psii->pfrag->ipdataset) ==
         (psii->pfrag->ipdatasize + 7)/8 ) {
      netwib_er(netwib_buf_init_ext_arrayfilled(psii->pfrag->ipdataarray,
                                                psii->pfrag->ipdatasize,
                                                &ipdata));
      netwib_er(netwib_pkt_append_linkipdata(&psii->pfrag->linkhdr,
                                             &psii->pfrag->iphdr,
                                             &ipdata, pgivepkt));
      psii->sent = NETWIB_TRUE;
      return(NETWIB_ERR_OK);
    }
  }

  /* nothing was available */
  return(NETWIB_ERR_DATANOTAVAIL);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
static netwib_err sniff_ipreas_fraginfo(netwib_iphdr *piphdr,
                                        netwib_buf *preadpkt,
                                        netwib_ip6ext *pip6extfrag)
{
  netwib_uint32 skipsize;
  netwib_ipproto pktproto;
  netwib_buf pkt;
  netwib_err ret;

  ret = NETWIB_ERR_OK;
  switch(piphdr->iptype) {
  case NETWIB_IPTYPE_IP4 :
    pip6extfrag->ext.fragment.fragmentoffset = piphdr->header.ip4.offsetfrag;
    pip6extfrag->ext.fragment.morefrag = piphdr->header.ip4.morefrag;
    pip6extfrag->ext.fragment.id = piphdr->header.ip4.id;
    break;
  case NETWIB_IPTYPE_IP6 :
    pip6extfrag->ext.fragment.fragmentoffset = 0;
    pip6extfrag->ext.fragment.morefrag = NETWIB_FALSE;
    pip6extfrag->ext.fragment.id = 0;
    if (netwib__buf_ref_data_size(&piphdr->header.ip6.exts) != 0) {
      pktproto = piphdr->protocol;
      pkt = piphdr->header.ip6.exts;
      while (NETWIB_TRUE) {
        if (pktproto == NETWIB_IPPROTO_FRAGMENT) {
          ret = netwib_pkt_decode_ip6ext(pktproto, &pkt, pip6extfrag, NULL);
          if (ret == NETWIB_ERR_OK) {
            piphdr->header.ip6.exts.endoffset-=netwib__buf_ref_data_size(&pkt);
            preadpkt->beginoffset -= netwib__buf_ref_data_size(&pkt) - 8;
          }
          break;
        }
        ret = netwib_priv_ip6exts_skip_ip6ext(pktproto, &pkt,
                                              &pktproto, &skipsize);
        if (ret != NETWIB_ERR_OK) {
          if (ret == NETWIB_ERR_NOTCONVERTED) ret = NETWIB_ERR_OK;
          if (ret == NETWIB_ERR_DATAMISSING) ret = NETWIB_ERR_OK;
          break;
        }
        pkt.beginoffset += skipsize;
      }
    }
    break;
  default :
    return(NETWIB_ERR_PAIPTYPE);
  }

  return(ret);
}

/*-------------------------------------------------------------*/
typedef struct {
  netwib_hash *phash;
  netwib_buf readpkt;
  netwib_uint32 countnewitems; /* to purge after several new items added */
  netwib_uint32 countpkt; /* to purge after several packet received */
} netwib_priv_io_sniff_ipreas;

/*-------------------------------------------------------------*/
static netwib_err sniff_ipreas_read(netwib_priv_io_sniff_ipreas *pios,
                                    netwib_device_dlttype dlt,
                                    netwib_constbuf *preadpkt,
                                    netwib_buf *pgivepkt)
{
  netwib_byte array[SNIFF_IPREAS_ITEM_KEY_MAXLEN];
  netwib_linkhdrproto linkhdrproto;
  netwib_linkhdr linkhdr;
  netwib_iphdr iphdr;
  netwib_ip6ext ip6extfrag;
  netwib_ptr pitem;
  netwib_buf readpkt;
  sniff_ipreas_item *psii;
  netwib_buf key;
  netwib_bool isfrag;
  netwib_err ret;

  /* if readpkt is not ip, give it */
  readpkt = *preadpkt;
  ret = netwib_pkt_decode_layer_link(dlt, &readpkt, &linkhdr);
  if (ret == NETWIB_ERR_NOTCONVERTED || ret == NETWIB_ERR_DATAMISSING
      || ret == NETWIB_ERR_LONOTIMPLEMENTED) {
    netwib_er(netwib_buf_append_buf(preadpkt, pgivepkt));
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }
  netwib_er(netwib_linkhdr_get_proto(&linkhdr, &linkhdrproto));
  if (linkhdrproto != NETWIB_LINKHDRPROTO_IP4 &&
      linkhdrproto != NETWIB_LINKHDRPROTO_IP6) {
    netwib_er(netwib_buf_append_buf(preadpkt, pgivepkt));
    return(NETWIB_ERR_OK);
  }
  ret = netwib_pkt_decode_layer_ip(&readpkt, &iphdr);
  if (ret == NETWIB_ERR_NOTCONVERTED || ret == NETWIB_ERR_DATAMISSING
      || ret == NETWIB_ERR_LONOTIMPLEMENTED) {
    netwib_er(netwib_buf_append_buf(preadpkt, pgivepkt));
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }

  /* clean hash */
  if (pios->countnewitems > SNIFF_IPREAS_NEWITEMS_CLEAN ||
      pios->countpkt > SNIFF_IPREAS_PKT_CLEAN) {
    netwib_time t;
    netwib_er(netwib_time_init_now(&t));
    netwib_er(netwib_hash_del_criteria(pios->phash, &sniff_ipreas_item_clean,
                                       &t.sec, NETWIB_TRUE));
    pios->countnewitems = 0;
    pios->countpkt = 0;
  }
  pios->countpkt++;

  /* check if this packet is fragmented */
  netwib_er(sniff_ipreas_fraginfo(&iphdr, &readpkt, &ip6extfrag));
  isfrag = NETWIB_FALSE;
  if (ip6extfrag.ext.fragment.morefrag ||
      ip6extfrag.ext.fragment.fragmentoffset != 0) {
    isfrag = NETWIB_TRUE;
  }

  /* first, deal with unfragmented packets */
  if (!isfrag) {
    netwib_er(netwib_buf_append_buf(preadpkt, pgivepkt));
    return(NETWIB_ERR_OK);
  }

  /* compute hash key */
  netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &key));
  netwib_er(sniff_ipreas_item_key(&iphdr, &ip6extfrag, &key));

  /* now, deal with fragments */
  /* search this ip packet in the hash */
  ret = netwib_hash_value(pios->phash, &key, &pitem);
  if (ret == NETWIB_ERR_OK) {
    psii = (sniff_ipreas_item *)pitem;
    /* complete the packet, and eventually return it */
    ret = sniff_ipreas_item_reas(psii, &linkhdr, &iphdr, &readpkt,
                                 &ip6extfrag, pgivepkt);
  } else if (ret == NETWIB_ERR_NOTFOUND) {
    /* add it in the hash */
    netwib_er(sniff_ipreas_item_create(&pitem));
    psii = (sniff_ipreas_item *)pitem;
    netwib_er(netwib_hash_add(pios->phash, &key, pitem, NETWIB_TRUE));
    pios->countnewitems++;
    /* now set hash values */
    ret = sniff_ipreas_item_reas(psii, &linkhdr, &iphdr, &readpkt,
                                 &ip6extfrag, pgivepkt);
  }

  return(ret);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sniff_ipreas_init(netwib_bool *preadinitialized,
                                                   netwib_bool *pwriteinitialized,
                                                   netwib_priv_io_sniff_ipreas *ptr)
{

  /* hash storing packets */
  netwib_er(netwib_hash_init(&sniff_ipreas_item_erase, NULL, &ptr->phash));

  /* buffer used to retrieve packets */
  netwib_er(netwib_buf_init_mallocdefault(&ptr->readpkt));

  /* used to clean hash */
  ptr->countnewitems = 0;
  ptr->countpkt = 0;

  /* set io values */
  *preadinitialized = NETWIB_TRUE;
  *pwriteinitialized = NETWIB_FALSE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sniff_ipreas_close(netwib_priv_io_sniff_ipreas *ptr)
{

  /* close */
  netwib_er(netwib_buf_close(&ptr->readpkt));
  netwib_er(netwib_hash_close(&ptr->phash, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sniff_ipreas_read(netwib_io *pio,
                                                   netwib_buf *pbuf)
{
  netwib_priv_io_sniff_ipreas *ptr=(netwib_priv_io_sniff_ipreas *)pio->pcommon;
  netwib_device_dlttype dlt;

  netwib_er(netwib_sniff_ctl_get_dlt(pio->rd.pnext, &dlt));

  netwib__buf_reinit(&ptr->readpkt);
  netwib_er(netwib_io_read(pio->rd.pnext, &ptr->readpkt));
  /*netwib_er(netwib_pkt_link_display(dlt, &ptr->readpkt,
                                    NULL, NETWIB_ENCODETYPE_SYNTH,
                                    NETWIB_ENCODETYPE_SYNTH));*/

  netwib_er(sniff_ipreas_read(ptr, dlt, &ptr->readpkt, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_sniff_ipreas_fclose(netwib_io *pio)
{
  netwib_priv_io_sniff_ipreas *ptr=(netwib_priv_io_sniff_ipreas *)pio->pcommon;

  netwib_er(netwib_priv_io_sniff_ipreas_close(ptr));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_init_sniff_ipreas(netwib_io **ppio)
{
  netwib_bool rdinit, wrinit;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwib_priv_io_sniff_ipreas), &pcommon));
  ret = netwib_priv_io_sniff_ipreas_init(&rdinit, &wrinit,
                                       (netwib_priv_io_sniff_ipreas *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdinit, wrinit,
                           pcommon,
                           &netwib_priv_io_sniff_ipreas_read,
                           NULL, /* write */
                           NULL, /* wait */
                           NULL, /* unread */
                           NULL, /* ctl_set */
                           NULL, /* ctl_get */
                           &netwib_priv_io_sniff_ipreas_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

