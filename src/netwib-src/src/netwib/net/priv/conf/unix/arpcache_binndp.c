/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
/***
  Used by :
    BSD
 ***/

/*-------------------------------------------------------------*/
/*
OpenBSD 2.7
/usr/sbin/ndp -an
Neighbor                        Linklayer Address  Netif Expire    St Flgs Prbs
fe80::220:afff:fe2a:aa31%ep0    0:20:af:aa:aa:aa     ep0 permanent R
fe80::240:95ff:fe46:aacb%ep0    0:40:95:aa:aa:aa     ep0 28s       R
fec0:0:0:1::1                   0:10:4b:aa:aa:aa     ep0 18s       R
*/
static netwib_err netwib_priv_conf_arpcache_binndp(netwib_priv_confwork *pcw)
{
  netwib_io *piofile, *pioline;
  netwib_priv_confwork_arpcache *pca;
  netwib_buf buf;
  netwib_bool pcaset;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string("/usr/sbin/ndp -an", &buf));
  ret = netwib_io_init_exec(&buf, NETWIB_IO_WAYTYPE_READ, NETWIB_FALSE,
                            NULL, NULL, &piofile);
  if (ret != NETWIB_ERR_OK) {
    if (ret == NETWIB_ERR_NOTFOUND) {
      ret = NETWIB_ERR_LONOTSUPPORTED;
    }
    return(ret);
  }
  netwib_er(netwib_io_init_data_line(&pioline));
  netwib_er(netwib_io_plug_read(pioline, piofile));

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  /* prepare for error handling */
  pcaset = NETWIB_FALSE;
  ret = NETWIB_ERR_OK;

  /* read each line */
  while (NETWIB_TRUE) {
    netwib__buf_reinit(&buf);
    ret = netwib_io_read(pioline, &buf);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    netwib_eg(netwib_priv_confwork_arpcache_init(&pca));
    pcaset = NETWIB_TRUE;
    ret = netwib_buf_decode_fmt(&buf, "%{ip} %{eth} %{buf}", &pca->ip,
                                &pca->eth, &pca->device);
    if (ret == NETWIB_ERR_NOTCONVERTED) {
      /* ignore unrecognized lines */
      netwib_eg(netwib_priv_confwork_arpcache_close(&pca));
      pcaset = NETWIB_FALSE;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    netwib_eg(netwib_priv_confwork_arpcache_add(pcw, pca));
    pcaset = NETWIB_FALSE;
  }

  /* clean and leave */
 netwib_gotolabel:
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_io_close(&pioline));
  if (pcaset) {
    netwib_er(netwib_priv_confwork_arpcache_close(&pca));
  }
  return(ret);
}
