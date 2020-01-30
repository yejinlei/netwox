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
netwib_conststring t000167_description[] = {
  "This tool is a TFTP server.",
  "",
  NETWOX_DESC_allowedclients,
  NETWOX_DESC_rootdir,
  NETWOX_DESC_server,
  NETWOX_DESC_toolpriv_port1024,
  NULL
};
netwox_toolarg t000167_args[] = {
  NETWOX_SOCK_ARG_UDP_MULSERPORT("69"),
  NETWOX_TOOLARG_OPTA_IPS_SU('c', "allowed-clients",
                             "clients allowed to connect", NULL),
  NETWOX_TOOLARG_OPTA_BUF_DIR_RD('r', "rootdir", "root directory", NULL),
  NETWOX_TOOLARG_OPTA_BOOL('G', "allow-get", "allow get", "1"),
  NETWOX_TOOLARG_OPTA_BOOL('U', "allow-put", "allow put", "1"),
  NETWOX_TOOLARG_OPTA_UINT32('T', "timeout", "timeout in milliseconds",
                             "10000"),
  NETWOX_TOOLARG_OPTA_UINT32('R', "retry", "number of retry", "3"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000167_nodes[] = {
  NETWOX_TOOLTREENODETYPE_SERVER_UDP_TFTP,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000167_info = {
  "TFTP server",
  t000167_description,
  NULL,
  t000167_args,
  t000167_nodes,
};

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bufpool *pbufpool;
  netwox_sockinfo sockinfo;
  netwib_ips *pallowedclients;
  netwib_buf rootdir;
  netwib_bool allowget;
  netwib_bool allowput;
  netwib_uint32 maxwaitms;
  netwib_uint32 retry;
  netwib_io *pio;
} t000167_infos;

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bufpool *pbufpool;
  netwox_tftpopcode opcode;
  netwox_tftpinfos tftpinfos;
  netwib_io *pfileio;
} t000167_threadinfos;

/*-------------------------------------------------------------*/
/* each thread executes this function */
static netwib_err t000167_thread(netwib_ptr infosin,
                                 netwib_ptr *pinfosout)
{
  t000167_threadinfos *pthreadinfos = infosin;
  netwib_err ret=NETWIB_ERR_OK;

  if (pthreadinfos->opcode == NETWOX_TFTPOPCODE_RRQ) {
    ret = netwox_tftp_write(&pthreadinfos->tftpinfos,
                            pthreadinfos->pfileio, NETWIB_TRUE);
  } else {
    ret = netwox_tftp_read(&pthreadinfos->tftpinfos,
                           pthreadinfos->pfileio, NETWIB_TRUE);
  }

  netwib_er(netwib_io_close(&pthreadinfos->tftpinfos.pio));
  netwib_er(netwib_io_close(&pthreadinfos->pfileio));

  netwib_er(netwib_ptr_free((netwib_ptr*)&pthreadinfos));

  pinfosout = pinfosout; /* for compiler warning */
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err t000167_loop(t000167_infos *pinfos)
{
  netwox_tftpinfos globaltftpinfos;
  t000167_threadinfos *pthreadinfos;
  netwox_tftppkt tftppktsend, tftppktrecv;
  netwib_buf *pfilename;
  netwib_ip remoteip;
  netwib_port remoteport;
  netwib_bool allowed, event;
  netwib_ring *pthreadlist;
  netwib_thread *pthread;
  netwib_uint32 threadid;
  netwox_sockinfo newsockinfo;
  netwib_err ret=NETWIB_ERR_OK, ret2;

  /* prepare global tftp handler */
  globaltftpinfos.pbufpool = pinfos->pbufpool;
  globaltftpinfos.maxwaitms = 1000; /* small to wait thread */
  globaltftpinfos.retry = 0;
  globaltftpinfos.pio = pinfos->pio;

  netwib_er(netwib_threadlist_init(&pthreadlist));
  threadid = 1;

  while(NETWIB_TRUE) {
    netwib__debug_ctrlc_pressed_break();
    /* check if one thread exited */
    ret = netwib_threadlist_wait(pthreadlist, NETWIB_TIME_ZERO, &event,
                                 NULL, &ret2, NULL);
    if (ret == NETWIB_ERR_OK) {
      if (event) {
        netwib_er(netwox_err_display(ret2, NETWIB_ERR_ENCODETYPE_NUMTEXT));
      }
    }
    /* To force a premature end to detect leaks
    if (threadid == 2 && ret == NETWIB_ERR_DATAEND) {
      ret = NETWIB_ERR_OK;
      break;
    }
    */
    /* read a request (wait for 1 second at most) */
    ret = netwox_tftp_recv(&globaltftpinfos, &tftppktrecv);
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) break;
      if (ret == NETWOX_ERR_TIMEOUT ||
          ret == NETWIB_ERR_DATANOTAVAIL ||
          ret == NETWIB_ERR_NOTCONVERTED ||
          ret == NETWIB_ERR_DATAMISSING) {
        ret = NETWIB_ERR_OK;
        continue;
      }
      break;
    }
    /* to ensure client have time to switch to server (netwox_tftp_cli2ser) */
    netwib_er(netwib_time_sleep_msec(10));
    /* check it */
    netwib_er(netwib_sock_ctl_get_remote(pinfos->pio, &remoteip, &remoteport));
    netwib_er(netwib_ips_contains_ip(pinfos->pallowedclients, &remoteip,
                                     &allowed));
    if (!allowed) {
      tftppktsend.opcode = NETWOX_TFTPOPCODE_ERROR;
      tftppktsend.pkt.error.code = NETWOX_TFTPERROR_ACCESSVIOLATION;
      netwib_er(netwib_buf_init_ext_string("IP not allowed",
                                         &tftppktsend.pkt.error.msg));
      /*ignore*/netwox_tftp_send(&globaltftpinfos, &tftppktsend);
      continue;
    }
    if (tftppktrecv.opcode != NETWOX_TFTPOPCODE_RRQ &&
        tftppktrecv.opcode != NETWOX_TFTPOPCODE_WRQ) {
      /* no error message because it perturbs the Linux tftp client,
         which send an unrequested ack when using loopback */
      continue;
    }
    if ((tftppktrecv.opcode == NETWOX_TFTPOPCODE_RRQ && !pinfos->allowget) ||
        (tftppktrecv.opcode == NETWOX_TFTPOPCODE_WRQ && !pinfos->allowput)) {
      tftppktsend.opcode = NETWOX_TFTPOPCODE_ERROR;
      tftppktsend.pkt.error.code = NETWOX_TFTPERROR_ILLEGALOPERATION;
      netwib_er(netwib_buf_init_ext_string("This operation is not supported",
                                         &tftppktsend.pkt.error.msg));
      /*ignore*/netwox_tftp_send(&globaltftpinfos, &tftppktsend);
      continue;
    }
    /* prepare thread information */
    netwib_er(netwib_ptr_malloc(sizeof(t000167_threadinfos),
                                (netwib_ptr*)&pthreadinfos));
    pthreadinfos->pbufpool = pinfos->pbufpool;
    pthreadinfos->tftpinfos.pbufpool = pinfos->pbufpool;
    pthreadinfos->tftpinfos.maxwaitms = pinfos->maxwaitms;
    pthreadinfos->tftpinfos.retry = pinfos->retry;
    pthreadinfos->opcode = tftppktrecv.opcode;
    /* check filename, and open it */
    netwib_er(netwib_bufpool_buf_init(pinfos->pbufpool, &pfilename));
    ret = netwib_path_init_jail(&pinfos->rootdir, &tftppktrecv.pkt.rq.filename,
                                pfilename);
    if (ret == NETWIB_ERR_OK) {
      if (tftppktrecv.opcode == NETWOX_TFTPOPCODE_RRQ) {
        ret = netwib_io_init_file_read(pfilename, &pthreadinfos->pfileio);
      } else {
        ret = netwib_io_init_file_write(pfilename, &pthreadinfos->pfileio);
      }
    }
    netwib_er(netwib_bufpool_buf_close(pinfos->pbufpool, &pfilename));
    if (ret != NETWIB_ERR_OK) {
      tftppktsend.opcode = NETWOX_TFTPOPCODE_ERROR;
      tftppktsend.pkt.error.code = NETWOX_TFTPERROR_FILENOTFOUND;
      netwib_er(netwib_buf_init_ext_string("Cannot open this file",
                                         &tftppktsend.pkt.error.msg));
      /*ignore*/netwox_tftp_send(&globaltftpinfos, &tftppktsend);
      netwib_er(netwib_ptr_free((netwib_ptr*)&pthreadinfos));
      continue;
    }
    /* do not check mode : always work in octet mode */
    /* connect back to the client */
    netwib_er(netwox_sockinfo_init_connect(&pinfos->sockinfo, pinfos->pio,
                                           remoteport, &newsockinfo));
    ret = netwox_sock_init(&newsockinfo, &pthreadinfos->tftpinfos.pio);
    netwib_er(netwox_sockinfo_close(&newsockinfo));
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwib_io_close(&pthreadinfos->pfileio));
      netwib_er(netwib_ptr_free((netwib_ptr*)&pthreadinfos));
      continue;
    }
    /* create a thread to handle this session */
    netwib_er(netwib_thread_init(&t000167_thread, (netwib_ptr)pthreadinfos,
                                 &pthread));
    netwib_er(netwib_threadlist_add(pthreadlist, pthread, threadid));
    threadid++;
  }

  netwib_er(netwib_threadlist_close(&pthreadlist));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000167_core(int argc, char *argv[])
{
  netwox_arg *parg;
  t000167_infos infos;
  netwib_bool isset;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000167_info, &parg));
  netwib_er(netwox_sockinfo_init_arg_udp_mulser(parg, &infos.sockinfo));
  netwib_er(netwox_sock_init(&infos.sockinfo, &infos.pio));
  netwib_er(netwox_arg_isset(parg, 'r', &isset));
  if (isset) {
    netwib_er(netwox_arg_buf(parg, 'r', &infos.rootdir));
  } else {
    netwib_er(netwib_buf_init_ext_string(".", &infos.rootdir));
  }
  netwib_er(netwox_arg_ips(parg, 'c', &infos.pallowedclients));
  netwib_er(netwox_arg_bool(parg, 'G', &infos.allowget));
  netwib_er(netwox_arg_bool(parg, 'U', &infos.allowput));
  netwib_er(netwox_arg_uint32(parg, 'T', &infos.maxwaitms));
  netwib_er(netwox_arg_uint32(parg, 'R', &infos.retry));

  netwib_er(netwib_bufpool_init(NETWIB_TRUE, &infos.pbufpool));

  /* loop */
  netwib_er(t000167_loop(&infos));

  /* close */
  netwib_er(netwib_io_close(&infos.pio));
  netwib_er(netwox_sockinfo_close(&infos.sockinfo));
  netwib_er(netwox_arg_close(&parg));
  netwib_er(netwib_bufpool_close(&infos.pbufpool));

  return(NETWIB_ERR_OK);
}
