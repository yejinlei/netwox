/* This file is part of Netwib. Read and honor its license.
*/

/* IMPORTANT : NOT YET IMPLEMENTED */

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confwork_obtain_sys(netwib_priv_confwork *pcw)
{
  netwib_priv_confwork_devices *pcd;
  netwib_priv_confwork_ip *pci;

  netwib_er(netwib_priv_confwork_devices_init(&pcd));
  netwib_er(netwib_buf_append_string("lo", &pcd->device));
  pcd->hwtype = NETWIB_DEVICE_HWTYPE_LOOPBACK;
  pcd->mtu = 15000;
  netwib_er(netwib_priv_confwork_devices_add(pcw, pcd));

  netwib_er(netwib_priv_confwork_ip_init(&pci));
  netwib_er(netwib_buf_append_string("lo", &pci->device));
  netwib_er(netwib_ip_init_ip4(0x7F000001, &pci->ip));
  netwib_er(netwib_ip_init_ip4(0xFF000000u, &pci->mask));
  pci->ispointtopoint = NETWIB_FALSE;
  netwib_er(netwib_priv_confwork_ip_add(pcw, pci));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_obtain_arpcache(netwib_priv_confwork *pcw)
{
  pcw = pcw; /* for compiler warning */
  return(NETWIB_ERR_OK);
}
