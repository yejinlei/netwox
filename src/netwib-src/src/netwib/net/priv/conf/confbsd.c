/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
#include "unix/devices_sysctl.c"
#include "unix/arpcache_sysctl.c"
#include "unix/arpcache_binndp.c"
#include "unix/routes_sysctl.c"

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_confwork_obtain_sys(netwib_priv_confwork *pcw)
{
  netwib_bool ip6supported;
  netwib_err ret;

  netwib_er(netwib_priv_ip_ip6_supported(&ip6supported));

  netwib_er(netwib_priv_conf_devices_sysctl(pcw));
  netwib_er(netwib_priv_confwork_debug(pcw, "After devices_sysctl"));

  netwib_er(netwib_priv_conf_arpcache_sysctl(pcw));
  netwib_er(netwib_priv_confwork_debug(pcw, "After arpcache_sysctl"));
  if (ip6supported) {
    ret = netwib_priv_conf_arpcache_binndp(pcw);
    if (ret != NETWIB_ERR_OK) {
      if (ret != NETWIB_ERR_LONOTSUPPORTED) {
        return(ret);
      }
    }
    netwib_er(netwib_priv_confwork_debug(pcw, "After arpcache_binndp"));
  }

  netwib_er(netwib_priv_conf_routes_sysctl(pcw));
  netwib_er(netwib_priv_confwork_debug(pcw, "After routes_sysctl"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_confwork_obtain_arpcache(netwib_priv_confwork *pcw)
{
  netwib_bool ip6supported;
  netwib_err ret;

  netwib_er(netwib_priv_ip_ip6_supported(&ip6supported));

  netwib_er(netwib_priv_conf_arpcache_sysctl(pcw));
  if (ip6supported) {
    ret = netwib_priv_conf_arpcache_binndp(pcw);
    if (ret != NETWIB_ERR_OK) {
      if (ret != NETWIB_ERR_LONOTSUPPORTED) {
        return(ret);
      }
    }
  }

  return(NETWIB_ERR_OK);
}
