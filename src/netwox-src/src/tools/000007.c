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
netwib_conststring t000007_description[] = {
  "This tool captures network packets. It can display them, or save them",
  "in a file (named 'record' in netwox).",
  "",
  NETWOX_DESC_sniff_devfilter,
  "Parameter --pause permits to press P (pause) or Q (quit) keys",
  "to pause or stop capture.",
  NETWOX_DESC_sniff_encode,
  NETWOX_DESC_sniff_rawip,
  NETWOX_DESC_sniff_extended,
  NETWOX_DESC_sniff_ipreas,
  NETWOX_DESC_sniff_tcpreord,
  "",
  NETWOX_DESC_record,
  "Parameter --outfile indicates the name of file where to store captured",
  "packets. Parameter --recordencode defines how to encode data in this",
  "file (suggested values: bin, pcap and mixed_wrap). Capture can",
  "automatically swap file using parameters --split-size or --split-age.",
  "The DLT (Data Link Type) of packets in this record will be 'raw' if",
  "--rawip is set, otherwise the sniff DLT obtained by tool 13.",
  NETWOX_DESC_toolpriv_sniff,
  NULL
};
netwox_toolarg t000007_args[] = {
  NETWOX_TOOLARG_OPT_BUF_DEVICE('d', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPT_BUF_FILTER('f', NULL, NULL, NULL),
  NETWOX_TOOLARG_OPTA_BOOL('r', "rawip", "sniff at IP level", NULL),
  NETWOX_TOOLARG_OPT_BOOL('p', "pause", "can pause", NULL),
  NETWOX_TOOLARG_OPTA_BOOL('x', "extended", "display other protocols", "1"),
  NETWOX_TOOLARG_OPTA_BOOL('i', "ipreas", "reassemble IP packets", NULL),
  NETWOX_TOOLARG_OPTA_BOOL('t', "tcpreord", "reorder TCP packets", NULL),
  NETWOX_TOOLARG_OPTA_BOOL('s', "screen", "display to screen", "1"),
  NETWOX_TOOLARG_OPT_ENCODE('H', "hdrencode",
                            "header encoding type for screen", "array"),
  NETWOX_TOOLARG_OPT_ENCODE('D', "dataencode", "data encoding type for screen",
                            "dump"),
  NETWOX_TOOLARG_OPTA_BUF_FILE_WR('o', "outfile", "save in record file", NULL),
  NETWOX_TOOLARG_OPTA_RECORDENCODE('R', "recordencode",
                                   "encoding type for record file", NULL),
  NETWOX_TOOLARG_OPTA_UINT32('c', "split-size", "maximum size of record in kb",
                             NULL),
  NETWOX_TOOLARG_OPTA_UINT32('C', "split-age",
                             "maximum age of record in seconds", NULL),
  NETWOX_LOSEPRIV_TOOLARG_COMMON('Q'),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000007_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SNIFF,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000007_info = {
  "Sniff",
  t000007_description,
  "capture, ethereal, frame, pcap, snoop, tcpdump",
  t000007_args,
  t000007_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000007_init(netwib_constbuf *pdevice,
                               netwib_constbuf *pfilter,
                               netwib_bool rawip,
                               netwib_bool ipreas,
                               netwib_bool tcpreord,
                               netwib_io **ppio)
{
  netwib_io *pio1, *pio2;

  if (rawip) {
    netwib_er(netwib_io_init_sniff_ip(pdevice, pfilter, ipreas, tcpreord,
                                      ppio));
  } else {
    netwib_er(netwib_io_init_sniff(pdevice, pfilter, &pio1));
    if (ipreas || tcpreord) {
      netwib_er(netwib_io_init_sniff_ipreas(&pio2));
      netwib_er(netwib_io_plug_read(pio2, pio1));
      pio1 = pio2;
    }
    if (tcpreord) {
      netwib_er(netwib_io_init_sniff_tcpreord(&pio2));
      netwib_er(netwib_io_plug_read(pio2, pio1));
      pio1 = pio2;
    }
    if (ppio != NULL) *ppio = pio1;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* This structure is used to not have to give more than 5 parameters
   to functions. They are all stored here.
*/
typedef struct {
  netwib_uint32 num;
  netwib_buf basefilename;
  netwib_buf filename;
  netwib_record_encodetype encodetype;
  netwib_uint32 size;
  netwib_uint32 splitage;
  netwib_uint32 timeexpire;
} t000007_split;

/*-------------------------------------------------------------*/
static netwib_err t000007_split_init(netwib_constbuf *pfilename,
                                     netwib_record_encodetype encodetype,
                                     netwib_uint32 splitage,
                                     t000007_split *psplit)
{
  psplit->num = 0;
  netwib_er(netwib_buf_init_mallocdefault(&psplit->basefilename));
  netwib_er(netwib_buf_append_buf(pfilename, &psplit->basefilename));
  netwib_er(netwib_buf_init_mallocdefault(&psplit->filename));
  psplit->encodetype = encodetype;
  psplit->splitage = splitage;

  psplit->size = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000007_split_close(t000007_split *psplit)
{
  netwib_er(netwib_buf_close(&psplit->basefilename));
  netwib_er(netwib_buf_close(&psplit->filename));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
static netwib_err t000007_split_reopen(t000007_split *psplit,
                                       netwib_io **ppio)
{
  netwib_time t;
  netwib_localtime lt;

  /* first, close previous (except on first call) */
  if (psplit->num) {
    netwib_er(netwib_io_close(ppio));
  }
  psplit->num++;

  /* create the new name */
  netwib__buf_reinit(&psplit->filename);
  netwib_er(netwib_buf_append_buf(&psplit->basefilename, &psplit->filename));
  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_decode_localtime(&t, &lt));
  netwib_er(netwib_buf_append_fmt(&psplit->filename, ".%{uint32}%{r02;uint32}%{r02;uint32}_%{r02;uint32}%{r02;uint32}%{r02;uint32}.%{uint32}", lt.year, lt.mon, lt.mday, lt.hour, lt.min, lt.sec, psplit->num));

  /* open the new file (if file already exist, we leave) */
  netwib_er(netwib_io_init_record_write(&psplit->filename, psplit->encodetype,
                                      ppio));

  psplit->size = 0;
  psplit->timeexpire = t.sec + psplit->splitage;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000007_loop(netwib_io *piosniff,
                               netwib_bool pause,
                               netwib_bool extended,
                               netwib_bool dispscreen,
                               netwib_encodetype hdrencodetype,
                               netwib_encodetype dataencodetype,
                               netwib_bool disprecord,
                               netwib_constbuf *pfilename,
                               netwib_record_encodetype recordencodetype,
                               netwib_uint32 splitsize,
                               netwib_uint32 splitage)
{
  netwib_buf pkt, kbd;
  netwib_io *piorecord=NULL, *piokbd=NULL;
  netwib_wait *pwaitsniff=NULL, *pwaitkbd=NULL;
  netwib_bufpool *pbufpool;
  netwib_device_dlttype dlttype;
  netwib_encodetype_context screenctx;
  netwib_bool eventsniff, eventkbd;
  netwib_bool usesplit, needtosplit;
  netwib_string kbdstr;
  netwib_time t;
  t000007_split split;
  netwib_err ret=NETWIB_ERR_OK;

  /* for warning */
  split.size = 0;
  split.timeexpire = 0;

  /* initialize transition context */
  if (dispscreen || pause) {
    netwib_er(netwib_buf_encode_transition_init(&screenctx));
  }

  /* initialize record */
  usesplit = NETWIB_FALSE;
  if (disprecord) {
    if (splitsize || splitage) {
      usesplit = NETWIB_TRUE;
      netwib_er(t000007_split_init(pfilename, recordencodetype, splitage,
                                   &split));
      netwib_er(t000007_split_reopen(&split, &piorecord));
    } else {
      netwib_er(netwib_io_init_record_write(pfilename, recordencodetype,
                                            &piorecord));
    }
  }

  /* initialize kbd */
  if (pause) {
    netwib_er(netwib_io_init_kbddefault(&piokbd));
    netwib_er(netwib_kbd_ctl_set_line(piokbd, NETWIB_FALSE));
    netwib_er(netwib_kbd_ctl_set_echo(piokbd, NETWIB_FALSE));
    netwib_er(netwib_wait_init_io_read(piosniff, &pwaitsniff));
    netwib_er(netwib_wait_init_io_read(piokbd, &pwaitkbd));
  }
  /* should be in the "if pause", but generates a "use without
     initialization" warning */
  netwib_er(netwib_buf_init_mallocdefault(&kbd));

  /* main loop */
  netwib_er(netwib_sniff_ctl_get_dlt(piosniff, &dlttype));
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  while (NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
    /* read packet */
    if (pause) {
      netwib_er(netwib_wait_wait2(pwaitsniff, pwaitkbd, NETWIB_TIME_INFINITE,
                                  &eventsniff, &eventkbd));
      if (eventkbd) {
        netwib__buf_reinit(&kbd);
        ret = netwib_io_read(piokbd, &kbd);
        if (ret == NETWIB_ERR_DATANOTAVAIL) {
          ret = NETWIB_ERR_OK;
          continue;
        } else if (ret != NETWIB_ERR_OK) {
          break;
        }
        netwib_er(netwib_buf_ref_string(&kbd, &kbdstr));
        if (kbdstr[0] == 'q' || kbdstr[0] == 'Q') {
          break;
        } else if (kbdstr[0] == 'p' || kbdstr[0] == 'P') {
          dispscreen = (netwib_bool)! dispscreen;
          if (! dispscreen) {
            netwib_er(netwib_fmt_display("Paused. Press p to restart.\n"));
          }
        } else {
          netwib_er(netwib_fmt_display("Press q to quit. Press p to pause.\n"));
        }
      }
      if (!eventsniff) {
        continue;
      }
    }
    netwib__buf_reinit(&pkt);
    ret = netwib_io_read(piosniff, &pkt);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* display to screen */
    if (dispscreen) {
      if (extended) {
        netwib_er(netwox_pkt_link_display(pbufpool, dlttype, &pkt, &screenctx,
                                          hdrencodetype, dataencodetype));
      } else {
        netwib_er(netwib_pkt_link_display(dlttype, &pkt, &screenctx,
                                          hdrencodetype, dataencodetype));
      }
    }
    /* display to record */
    if (disprecord) {
      /* check if we need to reopen the record */
      if (usesplit) {
        needtosplit = NETWIB_FALSE;
        if (splitsize) {
          split.size += pkt.endoffset - pkt.beginoffset;
          if (split.size >= splitsize) {
            needtosplit = NETWIB_TRUE;
          }
          /* note : if pkt size > splitsize, we do not respect the limit */
        }
        if (splitage) {
          netwib_er(netwib_time_init_now(&t));
          if (t.sec >= split.timeexpire) {
            needtosplit = NETWIB_TRUE;
          }
        }
        if (needtosplit) {
          netwib_er(t000007_split_reopen(&split, &piorecord));
        }
      }
      /* write data */
      netwib_er(netwib_io_write(piorecord, &pkt));
    }
  }

  /* close */
  netwib_er(netwib_bufpool_close(&pbufpool));
  netwib_er(netwib_buf_close(&pkt));
  if (disprecord) {
    netwib_er(netwib_io_close(&piorecord));
    if (usesplit) {
      netwib_er(t000007_split_close(&split));
    }
  }
  if (pause) {
    netwib_er(netwib_io_close(&piokbd));
    netwib_er(netwib_wait_close(&pwaitsniff));
    netwib_er(netwib_wait_close(&pwaitkbd));
  }
  netwib_er(netwib_buf_close(&kbd));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000007_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_io *pio;
  netwib_buf device, filter, filename;
  netwib_bool rawip, pause, extended, ipreas, tcpreord, dispscreen, disprecord;
  netwib_bool losepriv;
  netwib_encodetype hdrencodetype, dataencodetype;
  netwib_record_encodetype recordencodetype;
  netwib_uint32 splitsize, splitage;
  netwib_err ret;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000007_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'd', &device));
  netwib_er(netwox_arg_buf(parg, 'f', &filter));
  netwib_er(netwox_arg_bool(parg, 'r', &rawip));
  netwib_er(netwox_arg_bool(parg, 'p', &pause));
  netwib_er(netwox_arg_bool(parg, 'x', &extended));
  netwib_er(netwox_arg_bool(parg, 'i', &ipreas));
  netwib_er(netwox_arg_bool(parg, 't', &tcpreord));
  netwib_er(netwox_arg_bool(parg, 's', &dispscreen));
  /* for dispscreen || pause */
  netwib_er(netwox_arg_encode(parg, 'H', &hdrencodetype));
  netwib_er(netwox_arg_encode(parg, 'D', &dataencodetype));
  netwib_er(netwox_arg_isset(parg, 'o', &disprecord));
  /* for disprecord */
  netwib_er(netwox_arg_buf(parg, 'o', &filename));
  netwib_er(netwox_arg_recordencode(parg, 'R', &recordencodetype));
  netwib_er(netwox_arg_uint32(parg, 'c', &splitsize));
  netwib_er(netwox_arg_uint32(parg, 'C', &splitage));
  /**/
  netwib_er(netwox_arg_bool(parg, 'Q', &losepriv));

  /* initialize io */
  netwib_er(t000007_init(&device, &filter, rawip, ipreas, tcpreord, &pio));
  if (losepriv) {
    netwib_er(netwox_losepriv());
  }

  /* main loop */
  if (pause) {
    netwib_er(netwib_fmt_display("Press q to quit. Press p to pause.\n"));
  }
  ret = t000007_loop(pio, pause, extended,
                     dispscreen, hdrencodetype, dataencodetype,
                     disprecord, &filename, recordencodetype,
                     splitsize, splitage);

  /* close */
  netwib_er(netwib_io_close(&pio));
  netwib_er(netwox_arg_close(&parg));

  return(ret);
}
