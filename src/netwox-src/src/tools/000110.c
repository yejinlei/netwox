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
netwib_conststring t000110_description[] = {
  "This tool is an Ethernet bridge. It has to be run on a computer having",
  "two network cards. This computer cuts the network in two. It permits",
  "to limit the flow. This can be used to simulate a slow network in",
  "order to test an application.",
  "A --max12 or --max21 of 0 means no limitation.",
  NETWOX_DESC_toolpriv_snsp,
  NULL
};
netwox_toolarg t000110_args[] = {
  NETWOX_TOOLARG_REQ_BUF_DEVICE('d', "device1", "first device", NULL),
  NETWOX_TOOLARG_REQ_BUF_DEVICE('D', "device2", "second device", NULL),
  NETWOX_TOOLARG_OPT_UINT32('m', "max12", "max byte/sec from 1 to 2", NULL),
  NETWOX_TOOLARG_OPT_UINT32('M', "max21", "max byte/sec from 2 to 1", NULL),
  NETWOX_TOOLARG_OPT_BOOL('v', "verbose", "verbose", "1"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000110_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SNIFF,
  NETWOX_TOOLTREENODETYPE_SPOOF_ETH,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000110_info = {
  "Ethernet bridge limiting flow",
  t000110_description,
  NULL,
  t000110_args,
  t000110_nodes,
};

/*-------------------------------------------------------------*/
/* for array initialization */
#define T000110_DEVICE_SIZE 2
/* for array indexes */
typedef enum {
  T000110_DEVICE_1 = 0,
  T000110_DEVICE_2 = 1
} t000110_device;
/* for accessing other item */
#define t000110_device_other(i) (((i)==T000110_DEVICE_1)?T000110_DEVICE_2:T000110_DEVICE_1)

/*-------------------------------------------------------------*/
typedef struct {
  netwib_buf device[T000110_DEVICE_SIZE];
  netwib_uint32 max[T000110_DEVICE_SIZE];
  netwib_io *psniffio[T000110_DEVICE_SIZE];
  netwib_io *pspoofio[T000110_DEVICE_SIZE];
  netwib_eths *pethlan[T000110_DEVICE_SIZE];
  netwib_time canspoofat[T000110_DEVICE_SIZE];
  netwib_ring *pring[T000110_DEVICE_SIZE];
  netwib_bool verbose;
} t000110_deviceinfo;

/*-------------------------------------------------------------*/
static netwib_err t000110_spoof(t000110_deviceinfo *pdeviceinfo,
                                t000110_device devnum,
                                netwib_constbuf *ppkt)
{

  if (pdeviceinfo->verbose) {
    netwib_er(netwib_fmt_display("SPOOF%{uint32} : %{uint32} bytes\n",
                                 t000110_device_other(devnum) + 1,
                                 netwib__buf_ref_data_size(ppkt)));
  }

  /* currently, there is no rewrite of the packet before spoofing it,
     but it can be done here. */

  /* spoof packet */
  netwib_er(netwib_io_write(pdeviceinfo->pspoofio[devnum], ppkt));

  /* update next allowed time */
  if (pdeviceinfo->max[devnum] != 0) {
    netwib_uint32 msec;
    msec = (1000*netwib__buf_ref_data_size(ppkt))/pdeviceinfo->max[devnum];
    netwib_er(netwib_time_init_now(&pdeviceinfo->canspoofat[devnum]));
    netwib_er(netwib_time_plus_msec(&pdeviceinfo->canspoofat[devnum], msec));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000110_eventuallyupdate_time(netwib_time *pwaittill,
                                                netwib_consttime *preadytime)
{
  netwib_cmp cmp;

  netwib_er(netwib_time_cmp(preadytime, pwaittill, &cmp));
  if (cmp == NETWIB_CMP_LT) {
    netwib_er(netwib_time_init_time(preadytime, pwaittill));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000110_analyze(t000110_deviceinfo *pdeviceinfo,
                                  t000110_device devnum,
                                  netwib_buf *ppkt,
                                  netwib_time *pwaittill)
{
  netwib_bool yes;
  netwib_buf storedpkt;
  netwib_ptr pitem;
  netwib_ring_index *pringindex;
  netwib_uint32 count;
  netwib_linkhdr linkhdr;
  netwib_err ret;

  /* if there is a packet, check it's for us */
  if (netwib__buf_ref_data_size(ppkt) != 0) {
    netwib_buf pkt = *ppkt;
    ret = netwib_pkt_decode_layer_link(NETWIB_DEVICE_DLTTYPE_ETHER, &pkt,
                                       &linkhdr);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_eths_contains_eth(pdeviceinfo->pethlan[devnum],
                                         &linkhdr.hdr.ether.src, &yes));
      if (!yes) {
        /* src ether is not on the LAN, or is not yet registered */
        netwib_er(netwib_eths_contains_eth(pdeviceinfo->pethlan[t000110_device_other(devnum)], &linkhdr.hdr.ether.src, &yes));
        if (yes) {
          /* src ether is on the other LAN : the packet is not for us,
             so do as if no packet was sniffed */
          netwib__buf_reinit(ppkt);
        } else {
          /* add this new address in the list */
          netwib_er(netwib_eths_add_eth(pdeviceinfo->pethlan[devnum],
                                        &linkhdr.hdr.ether.src));
        }
      }
    }
  }

  /* verbose display */
  if (pdeviceinfo->verbose && netwib__buf_ref_data_size(ppkt) != 0) {
    netwib_er(netwib_fmt_display("SNIFF%{uint32} : %{uint32} bytes\n",
                                 devnum + 1, netwib__buf_ref_data_size(ppkt)));
  }

  /* first, deal with easy case when there is no delay */
  if (pdeviceinfo->max[devnum] == 0) {
    if (netwib__buf_ref_data_size(ppkt) != 0) {
      /* spoof packet */
      netwib_er(t000110_spoof(pdeviceinfo, devnum, ppkt));
    }
    /* do not change pwaittill (is infinite by default), and leave */
    return(NETWIB_ERR_OK);
  }

  /* if we are not yet allowed to spoof, add the packet in the ring */
  netwib_er(netwib_time_iselapsed(&pdeviceinfo->canspoofat[devnum], &yes));
  if (!yes) {
    if (netwib__buf_ref_data_size(ppkt) != 0) {
      netwib_er(netwox_bufstore_create(ppkt, &pitem));
      netwib_er(netwib_ring_add_last(pdeviceinfo->pring[devnum], pitem));
    }
    netwib_er(t000110_eventuallyupdate_time(pwaittill,
                                            &pdeviceinfo->canspoofat[devnum]));
    return(NETWIB_ERR_OK);
  }

  /* see if there are stored packets to send */
  netwib_er(netwib_ring_ctl_get_count(pdeviceinfo->pring[devnum], &count));
  if (count) {
    netwib_er(netwib_ring_index_init(pdeviceinfo->pring[devnum], &pringindex));
    while(NETWIB_TRUE) {
     netwib__debug_ctrlc_pressed_break();
     /* obtain stored packet */
      ret = netwib_ring_index_next(pringindex, &pitem);
      if (ret == NETWIB_ERR_DATAEND) {
        break;
      }
      netwib_er(netwox_bufstore_decode(pitem, &storedpkt));
      /* spoof it */
      netwib_er(t000110_spoof(pdeviceinfo, devnum, &storedpkt));
      /* remove it from the ring */
      netwib_er(netwib_ring_index_this_del(pringindex, NETWIB_TRUE));
      /* check if we can send more */
      netwib_er(netwib_time_iselapsed(&pdeviceinfo->canspoofat[devnum], &yes));
      if (!yes) {
        break;
      }
    }
    netwib_er(netwib_ring_index_close(&pringindex));
  }

  /* decide if the sniffed packet can be spoofed now or has to be put
     in the ring */
  if (netwib__buf_ref_data_size(ppkt) != 0) {
    netwib_er(netwib_time_iselapsed(&pdeviceinfo->canspoofat[devnum], &yes));
    if (yes) {
      netwib_er(t000110_spoof(pdeviceinfo, devnum, ppkt));
    } else {
      netwib_er(netwox_bufstore_create(ppkt, &pitem));
      netwib_er(netwib_ring_add_last(pdeviceinfo->pring[devnum], pitem));
    }
  }

  /* eventually update pwaittill time */
  netwib_er(t000110_eventuallyupdate_time(pwaittill,
                                          &pdeviceinfo->canspoofat[devnum]));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000110_loop(t000110_deviceinfo *pdeviceinfo)
{
  netwib_wait *pwait[T000110_DEVICE_SIZE];
  netwib_bool event[T000110_DEVICE_SIZE];
  netwib_buf pkt;
  netwib_time waittill;
  t000110_device i;
  netwib_err ret;

  /* initialize wait */
  netwib_er(netwib_wait_init_io_read(pdeviceinfo->psniffio[T000110_DEVICE_1],
                                     &pwait[T000110_DEVICE_1]));
  netwib_er(netwib_wait_init_io_read(pdeviceinfo->psniffio[T000110_DEVICE_2],
                                     &pwait[T000110_DEVICE_2]));

  /* initialize working buffer */
  netwib_er(netwib_buf_init_mallocdefault(&pkt));

  /* deal with each packet */
  ret=NETWIB_ERR_OK;
  netwib_er(netwib_time_init_time(NETWIB_TIME_INFINITE, &waittill));
  while (NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
    /* wait for sniff event, or for time to send packet(s) */
    netwib_er(netwib_wait_wait2(pwait[T000110_DEVICE_1],
                                pwait[T000110_DEVICE_2],
                                &waittill,
                                &event[T000110_DEVICE_1],
                                &event[T000110_DEVICE_2]));
    /* for next loop iteration, we'll need to wait, but we do not
       yet know when to stop. So set infinite time. This value
       will be adjusted in t000110_analyze functions */
    netwib_er(netwib_time_init_time(NETWIB_TIME_INFINITE, &waittill));
    /* now, for each device, see if there is a packet to sniff, and eventually
       spoof stored ones */
    for (i = T000110_DEVICE_1; i <= T000110_DEVICE_2; i++) {
      /* sniff */
      netwib__buf_reinit(&pkt);
      if (event[i]) {
        ret = netwib_io_read(pdeviceinfo->psniffio[i], &pkt);
        if (ret != NETWIB_ERR_OK && ret != NETWIB_ERR_DATANOTAVAIL) {
          break;
        }
      }
      /* analyze */
      ret = t000110_analyze(pdeviceinfo, i, &pkt, &waittill);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
    }
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }

  /* close */
  netwib_er(netwib_buf_close(&pkt));
  netwib_er(netwib_wait_close(&pwait[T000110_DEVICE_1]));
  netwib_er(netwib_wait_close(&pwait[T000110_DEVICE_2]));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000110_core(int argc, char *argv[])
{
  netwox_arg *parg;
  t000110_deviceinfo deviceinfo;
  netwib_err ret;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000110_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'd', &deviceinfo.device[T000110_DEVICE_1]));
  netwib_er(netwox_arg_buf(parg, 'D', &deviceinfo.device[T000110_DEVICE_2]));
  netwib_er(netwox_arg_uint32(parg, 'm', &deviceinfo.max[T000110_DEVICE_1]));
  netwib_er(netwox_arg_uint32(parg, 'M', &deviceinfo.max[T000110_DEVICE_2]));
  netwib_er(netwox_arg_bool(parg, 'v', &deviceinfo.verbose));

  /* initialize sniff/spoof */
  netwib_er(netwib_io_init_sniff(&deviceinfo.device[T000110_DEVICE_1], NULL,
                                 &deviceinfo.psniffio[T000110_DEVICE_1]));
  netwib_er(netwib_io_init_spoof_link(&deviceinfo.device[T000110_DEVICE_1],
                                     &deviceinfo.pspoofio[T000110_DEVICE_2]));
  netwib_er(netwib_io_init_sniff(&deviceinfo.device[T000110_DEVICE_2], NULL,
                                 &deviceinfo.psniffio[T000110_DEVICE_2]));
  netwib_er(netwib_io_init_spoof_link(&deviceinfo.device[T000110_DEVICE_2],
                                     &deviceinfo.pspoofio[T000110_DEVICE_1]));

  /* initialize list of addresses on a LAN */
  netwib_er(netwib_eths_initdefault(&deviceinfo.pethlan[T000110_DEVICE_1]));
  netwib_er(netwib_eths_initdefault(&deviceinfo.pethlan[T000110_DEVICE_2]));

  /* initialize when we are allowed to spoof */
  netwib_er(netwib_time_init_now(&deviceinfo.canspoofat[T000110_DEVICE_1]));
  netwib_er(netwib_time_init_now(&deviceinfo.canspoofat[T000110_DEVICE_2]));

  /* initialize rings containing packets waiting to be sent */
  netwib_er(netwox_bufstore_ring_init(&deviceinfo.pring[T000110_DEVICE_1]));
  netwib_er(netwox_bufstore_ring_init(&deviceinfo.pring[T000110_DEVICE_2]));

  /* main loop */
  ret = t000110_loop(&deviceinfo);

  /* close */
  netwib_er(netwox_bufstore_ring_close(&deviceinfo.pring[T000110_DEVICE_1]));
  netwib_er(netwox_bufstore_ring_close(&deviceinfo.pring[T000110_DEVICE_2]));
  netwib_er(netwib_eths_close(&deviceinfo.pethlan[T000110_DEVICE_1]));
  netwib_er(netwib_eths_close(&deviceinfo.pethlan[T000110_DEVICE_2]));
  netwib_er(netwib_io_close(&deviceinfo.psniffio[T000110_DEVICE_1]));
  netwib_er(netwib_io_close(&deviceinfo.pspoofio[T000110_DEVICE_1]));
  netwib_er(netwib_io_close(&deviceinfo.psniffio[T000110_DEVICE_2]));
  netwib_er(netwib_io_close(&deviceinfo.pspoofio[T000110_DEVICE_2]));
  netwib_er(netwox_arg_close(&parg));

  return(ret);
}
