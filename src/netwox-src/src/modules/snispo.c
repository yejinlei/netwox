/*
                                  NETWOX
                             Network toolbox
                Copyright(c) 1999-2012 Laurent CONSTANTIN
                      http://ntwox.sourceforge.net/
                        laurentconstantin@free.fr
                                  -----

  This file is part of Netwox.

  Netwox is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3
  as published by the Free Software Foundation.

  Netwox is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details (http://www.gnu.org/).

------------------------------------------------------------------------
*/

/*-------------------------------------------------------------*/
#include "../netwox.h"

/*-------------------------------------------------------------*/
netwib_err netwox_snispo_init(netwib_constbuf *pdevice,
                              netwib_constbuf *psnifffilter,
                              netwib_spoof_inittype spoofinittype,
                              netwib_io **ppio)
{
  netwib_io *psniffio, *pspoofio;
  netwib_err ret;

  netwib_er(netwib_io_init_sniff(pdevice, psnifffilter, &psniffio));

  ret = netwib_io_init_spoof(spoofinittype, pdevice, &pspoofio);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_io_close(&psniffio));
    return(ret);
  }

  netwib_er(netwib_io_init_rdwr(psniffio, pspoofio, NETWIB_TRUE, ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snispo_init_eth(netwib_constbuf *pdevice,
                                  netwib_constbuf *psnifffilter,
                                  netwib_io **ppio)
{
  netwib_io *psniffio, *pspoofio;
  netwib_device_dlttype dlttype;
  netwib_err ret;

  netwib_er(netwib_io_init_sniff(pdevice, psnifffilter, &psniffio));
  netwib_er(netwib_sniff_ctl_get_dlt(psniffio, &dlttype));
  if (dlttype != NETWIB_DEVICE_DLTTYPE_ETHER) {
    netwib_er(netwib_fmt_display("Sniffing on %{buf} is not of DLT ETHER (%{uint32})\n", pdevice, dlttype));
    netwib_er(netwib_io_close(&psniffio));
    return(NETWOX_ERR_SNIFF_INVALIDDLT);
  }

  ret = netwib_io_init_spoof_link(pdevice, &pspoofio);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_io_close(&psniffio));
    return(ret);
  }
  netwib_er(netwib_spoof_ctl_get_dlt(pspoofio, &dlttype));
  if (dlttype != NETWIB_DEVICE_DLTTYPE_ETHER) {
    netwib_er(netwib_fmt_display("Spoofing on %{buf} is not of DLT ETHER (%{uint32})\n", pdevice, dlttype));
    netwib_er(netwib_io_close(&psniffio));
    netwib_er(netwib_io_close(&pspoofio));
    return(NETWOX_ERR_SPOOF_INVALIDDLT);
  }

  netwib_er(netwib_io_init_rdwr(psniffio, pspoofio, NETWIB_TRUE, ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_snispo_init_ip(netwib_constbuf *pdevice,
                                 netwib_constbuf *psnifffilter,
                                 netwib_bool sniffipreas,
                                 netwib_bool snifftcpreord,
                                 netwib_spoof_ip_inittype spoofinittype,
                                 netwib_io **ppio)
{
  netwib_io *psniffio, *pspoofio;
  netwib_err ret;

  netwib_er(netwib_io_init_sniff_ip(pdevice, psnifffilter, sniffipreas,
                                    snifftcpreord, &psniffio));

  ret = netwib_io_init_spoof_ip(spoofinittype, &pspoofio);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_io_close(&psniffio));
    return(ret);
  }

  netwib_er(netwib_io_init_rdwr(psniffio, pspoofio, NETWIB_TRUE, ppio));

  return(NETWIB_ERR_OK);
}
