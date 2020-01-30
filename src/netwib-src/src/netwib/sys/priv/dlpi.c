/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dlpi_putmsg(int fd,
                                   netwib_ptr ptr,
                                   int len,
                                   int flags)
{
  struct strbuf ctl;
  int reti;

  ctl.maxlen = 0;
  ctl.len = len;
  ctl.buf = ptr;

  reti = putmsg(fd, &ctl, (struct strbuf *)NULL, flags);
  netwib_ir(reti, NETWIB_ERR_FUPUTMSG);
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dlpi_getmsg(int fd,
                                   int len,
                                   netwib_ptr ptr)
{
  union DL_primitives dlp;
  struct strbuf ctl;
  int reti, flags;

  ctl.maxlen = 8192;
  ctl.len = 0;
  ctl.buf = ptr;

  flags = 0;
  reti = getmsg(fd, &ctl, (struct strbuf *)NULL, &flags);
  netwib_ir(reti, NETWIB_ERR_FUGETMSG);

  memcpy(&dlp, ctl.buf, sizeof(union DL_primitives));
  if (dlp.dl_primitive == DL_ERROR_ACK) {
    return(NETWIB_ERR_FUGETMSG);
  }
  if (ctl.len < len) {
    return(NETWIB_ERR_FUGETMSG);
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dlpi_open(netwib_conststring device,
                                 int *pfd)
{
  netwib_char devdevice[20], devdevicenumber[20];
  netwib_string pc;
  dl_info_req_t req;
  dl_bind_req_t bindreq;
  struct strioctl str;
  register dl_info_ack_t *infop;
  unsigned long buf[8192];
  int reti, ppa, fd;
  netwib_err ret;

  /* init "/dev/namexyz" */
  if (netwib_c_strlen(device) > sizeof(devdevice)-6) {
    return(NETWIB_ERR_PATOOHIGH);
  }
  netwib_c_strcpy(devdevicenumber, "/dev/");
  netwib_c_strcat(devdevicenumber, device);

  /* init "/dev/name" and ppa=xyz */
  netwib_c_strcpy(devdevice, devdevicenumber);
  pc = netwib_c_strpbrk(devdevice, "0123456789");
  if (pc == NULL) {
    ppa = 0;
  } else {
    ppa = atoi(pc);
    *pc = '\0';
  }

  /* try to open those devices */
  fd = open(devdevicenumber, O_RDWR);
  if (fd < 0) {
    fd = open(devdevice, O_RDWR);
    if (fd < 0) {
      return(NETWIB_ERR_FUOPEN);
    }
  }

  /* prepare for error */
  ret = NETWIB_ERR_OK;

  /* attach only if it's a style2 */
  req.dl_primitive = DL_INFO_REQ;
  netwib_eg(netwib_priv_dlpi_putmsg(fd, &req, sizeof(req), RS_HIPRI));
  netwib_eg(netwib_priv_dlpi_getmsg(fd, DL_INFO_ACK_SIZE, buf));
  infop = &((union DL_primitives *)buf)->info_ack;
  if (infop->dl_provider_style == DL_STYLE2) {
    dl_attach_req_t attachreq;
    attachreq.dl_primitive = DL_ATTACH_REQ;
    attachreq.dl_ppa = ppa;
    netwib_eg(netwib_priv_dlpi_putmsg(fd, &attachreq, sizeof(attachreq), 0));
    netwib_eg(netwib_priv_dlpi_getmsg(fd, DL_OK_ACK_SIZE, buf));
  }

  /* bind */
  netwib_c_memset((char *)&bindreq, 0, sizeof(bindreq));
  bindreq.dl_primitive = DL_BIND_REQ;
  bindreq.dl_sap = 0;
  bindreq.dl_service_mode = DL_CLDLS;
  netwib_eg(netwib_priv_dlpi_putmsg(fd, &bindreq, sizeof(bindreq), 0));
  netwib_eg(netwib_priv_dlpi_getmsg(fd, DL_BIND_ACK_SIZE, buf));

  /* ioctl to say we send in raw */
  str.ic_cmd = DLIOCRAW;
  str.ic_timout = -1;
  str.ic_len = 0;
  str.ic_dp = NULL;
  reti = ioctl(fd, I_STR, &str);
  netwib_ig(reti, NETWIB_ERR_FUIOCTL);

  *pfd = fd;
  return(NETWIB_ERR_OK);

 netwib_gotolabel :
  close(fd);
  return(ret);
}
