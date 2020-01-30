/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_loopback(netwib_priv_confwork *pcw)
{
  netwib_priv_confwork_devices *pcd;
  netwib_priv_confwork_ip *pci;

  netwib_er(netwib_priv_confwork_devices_init(&pcd));
  netwib_er(netwib_buf_append_string("Loopback", &pcd->device));
  pcd->hwtype = NETWIB_DEVICE_HWTYPE_LOOPBACK;
  pcd->mtu = 0;
  netwib_er(netwib_priv_confwork_devices_add(pcw, pcd));

  netwib_er(netwib_priv_confwork_ip_init(&pci));
  netwib_er(netwib_buf_append_string("Loopback", &pci->device));
  netwib_er(netwib_ip_init_ip4(0x7F000001, &pci->ip));
  netwib_er(netwib_ip_init_ip4(0xFF000000u, &pci->mask));
  pci->ispointtopoint = NETWIB_FALSE;
  netwib_er(netwib_priv_confwork_ip_add(pcw, pci));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_pcapfindalldevs(netwib_priv_confwork *pcw);
#include "windows/pcapfindalldevs.c"

/*-------------------------------------------------------------*/
#include "windows/mib.c"
static netwib_err netwib_priv_conf_inetmib1(netwib_priv_confwork *pcw);
#include "windows/inetmib1.c"

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_iphlpapi(netwib_priv_confwork *pcw);
#include "windows/iphlpapi.c"

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_deduct(netwib_priv_confwork *pcw,
                                          netwib_priv_confwork *pcwlocal);
#include "windows/deduct.c"

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_updtdev(netwib_priv_confwork *pcw);
#include "windows/updtdev.c"

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_conf_merge(netwib_priv_confwork *pcw,
                                         netwib_priv_confwork *pcwlocal);
#include "windows/merge.c"

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confwork_obtain_sys(netwib_priv_confwork *pcw)
{
  netwib_priv_confwork cw;
  netwib_err ret;

  netwib_er(netwib_priv_conf_loopback(pcw));
  netwib_er(netwib_priv_confwork_debug(pcw, "After loopback"));

  netwib_er(netwib_priv_conf_pcapfindalldevs(pcw));
  netwib_er(netwib_priv_confwork_debug(pcw, "After pcapfindalldevs"));

  /* now, local cw will be set with a row configuration */
  netwib_er(netwib_priv_confwork_init(&cw));
  cw.debug = pcw->debug;
  cw.pdebugbuf = pcw->pdebugbuf;
  ret = netwib_priv_conf_iphlpapi(&cw);
  if (ret == NETWIB_ERR_LONOTSUPPORTED) {
    ret = netwib_priv_conf_inetmib1(&cw);
    if (ret == NETWIB_ERR_LONOTSUPPORTED) {
      /* we have no more info to add */
      netwib_er(netwib_priv_confwork_close(&cw));
      return(NETWIB_ERR_OK);
    }
  }
  netwib_er(ret);
  netwib_er(netwib_priv_confwork_debug(&cw, "local cw after iphlpapi/inetmib1"));

  /* deduct device names in cw (using devices and ip rings). Ring pdevices
     then contains correct device names associated to devnum. */
  netwib_er(netwib_priv_conf_deduct(pcw, &cw));
  netwib_er(netwib_priv_confwork_debug(&cw, "local cw after deduct"));

  /* set all devices in pip, parpcache and proutes rings of cw */
  netwib_er(netwib_priv_conf_updtdev(&cw));
  netwib_er(netwib_priv_confwork_debug(&cw, "local cw after updtdev"));

  /* copy every item of cw in pcw */
  netwib_er(netwib_priv_conf_merge(pcw, &cw));
  netwib_er(netwib_priv_confwork_debug(pcw, "After merge"));

  /* cw is not needed anymore */
  netwib_er(netwib_priv_confwork_close(&cw));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_obtain_arpcache(netwib_priv_confwork *pcw)
{
  netwib_err ret;

  ret = netwib_priv_conf_iphlpapi_arpcache(pcw);
  if (ret == NETWIB_ERR_LONOTSUPPORTED) {
    ret = netwib_priv_conf_inetmib1_arpcache(pcw);
  }
  if (ret == NETWIB_ERR_LONOTSUPPORTED) {
    ret = NETWIB_ERR_OK;
  }

  return(ret);
}
