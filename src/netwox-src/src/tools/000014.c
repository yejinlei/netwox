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
netwib_conststring t000014_description[] = {
  NETWOX_DESC_record,
  "",
  "This tool reads packets stored in a record, and send them to the",
  "network.",
  "",
  NETWOX_DESC_record_file,
  NETWOX_DESC_record_dlt,
  "If dlt is not raw, parameter --device indicates the device to use.",
  "If dlt is raw, parameter --spoofip indicates how to send packets.",
  NETWOX_DESC_toolpriv_spoof,
  NULL
};
netwox_toolarg t000014_args[] = {
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('f', "file", "read from record file", NULL),
  NETWOX_TOOLARG_OPT_DLT('t', NULL, "dlt type of data stored in record", NULL),
  NETWOX_TOOLARG_OPTA_BUF_DEVICE('d', NULL, "device for spoof (if dlt!=raw)",
                                 NULL),
  NETWOX_TOOLARG_OPTA_SPOOFIP('i', NULL, "ip spoof type (if dlt==raw)", NULL),
  NETWOX_TOOLARG_OPT_BOOL('s', "screen", "display packets before sending them",
                          NULL),
  NETWOX_TOOLARG_OPT_BOOL('k', "keypress", "press a key for each packet",
                          NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000014_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SPOOF_ETH,
  NETWOX_TOOLTREENODETYPE_SPOOF_IP,
  NETWOX_TOOLTREENODETYPE_RECORD,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000014_info = {
  "Spoof a record",
  t000014_description,
  "capture, frame, send",
  t000014_args,
  t000014_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000014_init(netwib_device_dlttype recorddlt,
                               netwib_constbuf *pdevice,
                               netwib_spoof_ip_inittype spoofip,
                               netwib_io **ppio)
{
  netwib_device_dlttype spoofdlt;

  if (recorddlt == NETWIB_DEVICE_DLTTYPE_RAW) {
    netwib_er(netwib_io_init_spoof_ip(spoofip, ppio));
  } else {
    netwib_er(netwib_io_init_spoof_link(pdevice, ppio));
  }

  /* check spoof dlt */
  netwib_er(netwib_spoof_ctl_get_dlt(*ppio, &spoofdlt));
  if (spoofdlt != recorddlt) {
    netwib_er(netwib_fmt_display("Record dlt %{uint32} must the same as spoof dlt (%{uint32}\n", recorddlt, spoofdlt));
    return(NETWOX_ERR_SPOOF_INVALIDDLT);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000014_loop(netwib_io *piospoof,
                               netwib_constbuf *pfilename,
                               netwib_device_dlttype recorddlt,
                               netwib_bool dispscreen,
                               netwib_bool keypress)
{
  netwib_buf pkt;
  netwib_io *piorecord;
  netwib_encodetype_context screenctx;
  netwib_err ret=NETWIB_ERR_OK;

  /* initialize transition context */
  if (dispscreen) {
    netwib_er(netwib_buf_encode_transition_init(&screenctx));
  }

  /* initialize record */
  netwib_er(netwib_io_init_record_read(pfilename, &piorecord));

  /* main loop */
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  while (NETWIB_TRUE) {
    /* read packet */
    netwib__buf_reinit(&pkt);
    ret = netwib_io_read(piorecord, &pkt);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    /* display to screen */
    if (dispscreen) {
      netwib_er(netwib_pkt_link_display(recorddlt, &pkt, &screenctx,
                                        NETWIB_ENCODETYPE_SYNTH,
                                        NETWIB_ENCODETYPE_NOTHING));
    }
    /* key press */
    if (keypress) {
      netwib_er(netwib_kbd_press(NULL, NULL));
    }
    /* send */
    netwib_er(netwib_io_write(piospoof, &pkt));
  }

  /* close */
  netwib_er(netwib_buf_close(&pkt));
  netwib_er(netwib_io_close(&piorecord));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000014_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_io *pio;
  netwib_buf device, filename;
  netwib_device_dlttype recorddlt;
  netwib_spoof_ip_inittype spoofip;
  netwib_bool dispscreen, keypress;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000014_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'f', &filename));
  netwib_er(netwox_arg_dlt(parg, 't', &recorddlt));
  netwib_er(netwox_arg_buf(parg, 'd', &device));
  netwib_er(netwox_arg_spoofip(parg, 'i', &spoofip));
  netwib_er(netwox_arg_bool(parg, 's', &dispscreen));
  netwib_er(netwox_arg_bool(parg, 'k', &keypress));

  /* initialize io */
  netwib_er(t000014_init(recorddlt, &device, spoofip, &pio));

  /* main loop */
  netwib_er(t000014_loop(pio, &filename, recorddlt, dispscreen, keypress));

  /* close */
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
