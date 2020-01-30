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
netwib_bool netwox_pcapfilt_ip6_set = NETWIB_FALSE;
netwib_bool netwox_pcapfilt_ip6 = NETWIB_FALSE;

/*-------------------------------------------------------------*/
netwib_err netwox_pcapfilt_supports_ip6(netwib_constbuf *pdevice,
                                        netwib_bool *psupported)
{
  netwib_buf filter;
  netwib_io *pio;
  netwib_err ret;

#if NETWIBDEF_HAVEFUNC_PCAP_FREECODE==0 || NETWIBDEF_HAVEFUNC_PCAP_OPENDEAD==0
  /* Old versions of libpcap (<0.6) have a bug : if sniff is called once
     with a bad filter, it can no more work. So, we cannot detect
     if IPv6 is supported on those versions. So, on old versions,
     say IPv6 is unsupported.
     Libpcap's changelog says :
        Fixed bug that could cause subsequent "pcap_compile()"s to fail
        erroneously after one compile failed.
 */
  if (psupported != NULL) *psupported = NETWIB_FALSE;
  return(NETWIB_ERR_OK);
#endif

  if (netwox_pcapfilt_ip6_set) {
    if (psupported != NULL) *psupported = netwox_pcapfilt_ip6;
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_buf_init_ext_string("ip6 or icmp6 or host fec0::1", &filter));

  netwox_pcapfilt_ip6_set = NETWIB_TRUE;
  netwox_pcapfilt_ip6 = NETWIB_FALSE;
  ret = netwib_io_init_sniff(pdevice, &filter, &pio);
  if (ret == NETWIB_ERR_OK) {
    netwox_pcapfilt_ip6 = NETWIB_TRUE;
    netwib_er(netwib_io_close(&pio));
  }

  if (psupported != NULL) *psupported = netwox_pcapfilt_ip6;
  return(NETWIB_ERR_OK);
}
