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
#define NETWOX_RELAY_IPS_STEP 10
netwib_err netwox_relay_init_arg(netwox_arg *parg,
                                 netwox_socktype socktype,
                                 netwox_relay *prelay)
{
  netwib_ips *pips;
  netwib_ip *pip;
  netwib_ips_index *pipsindex;
  netwib_uint32 arraysize;
  netwib_err ret;

  prelay->socktype = socktype;
  if (prelay->socktype == NETWOX_SOCKTYPE_TCP_MULSER) {
    netwib_er(netwox_arg_ips(parg, 'x', &pips));
    netwib_er(netwib_ips_index_init(pips, &pipsindex));
    arraysize = NETWOX_RELAY_IPS_STEP;
    netwib_er(netwib_array_init(sizeof(netwib_ip), arraysize,
                                &prelay->serverips));
    prelay->numservers = 0;
    while (NETWIB_TRUE) {
      pip = (netwib_ip*)prelay->serverips.p[prelay->numservers];
      ret = netwib_ips_index_next_ip(pipsindex, pip);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
      prelay->numservers++;
      if (prelay->numservers == arraysize) {
        arraysize += NETWOX_RELAY_IPS_STEP;
        netwib_er(netwib_array_ctl_set_size(&prelay->serverips, arraysize));
      }
    }
    if (prelay->numservers == 0) {
      netwib_er(netwib_array_close(&prelay->serverips));
      return(NETWIB_ERR_PATOOLOW);
    }
    netwib_er(netwib_ips_index_close(&pipsindex));
  } else {
    prelay->numservers = 1;
    netwib_er(netwox_arg_ip(parg, 'x', &prelay->serverip));
  }
  netwib_er(netwox_arg_port(parg, 'X', &prelay->serverport));
  netwib_er(netwox_arg_ips(parg, 'c', &prelay->pallowedclients));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_relay_close(netwox_relay *prelay)
{
  if (prelay->socktype == NETWOX_SOCKTYPE_TCP_MULSER) {
    netwib_er(netwib_array_close(&prelay->serverips));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_relay_loop_allow(netwox_relay *prelay,
                                          netwib_io *pioserver,
                                          netwib_bool *pallowed)
{
  netwib_ip remoteip;

  *pallowed = NETWIB_TRUE;
  if (prelay->pallowedclients != NULL) {
    netwib_er(netwib_sock_ctl_get_remote(pioserver, &remoteip, NULL));
    netwib_er(netwib_ips_contains_ip(prelay->pallowedclients, &remoteip,
                                     pallowed));
    if (!*pallowed) {
      netwib_er(netwib_fmt_display("IP address %{ip} is not allowed\n",
                                   &remoteip));
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_relay_loop(netwox_relay *prelay,
                             netwib_io *pioserver)
{
  netwib_io *pioclient, *pior, *piow;
  netwib_bool allowed;
  netwib_buf buf;
  netwib_uint32 i;
  netwib_bool eventserver, eventclient;
  netwib_wait *pwaitserver, *pwaitclient;
  netwib_ip *pserverip;
  netwib_err ret=NETWIB_ERR_OK;

  /* wait and check client */
  allowed = NETWIB_FALSE;
  if (prelay->socktype == NETWOX_SOCKTYPE_TCP_SER ||
      prelay->socktype == NETWOX_SOCKTYPE_TCP_MULSER) {
    /* not udp because we have to read first packet */
    if (prelay->socktype == NETWOX_SOCKTYPE_TCP_SER ) {
      /* not the tcpmulser because it had already wait */
      netwib_er(netwib_wait_init_io_read(pioserver, &pwaitserver));
      netwib_er(netwib_wait_wait1(pwaitserver, NETWIB_TIME_INFINITE, NULL));
      netwib_er(netwib_wait_close(&pwaitserver));
    }
    netwib_er(netwox_relay_loop_allow(prelay, pioserver, &allowed));
    if (!allowed) {
      return(NETWIB_ERR_OK);
    }
  }

  /* choose server (random) */
  pserverip = &prelay->serverip;
  if (prelay->numservers != 1) {
    netwib_er(netwib_uint32_init_rand(0, prelay->numservers-1, &i));
    pserverip = (netwib_ip*)prelay->serverips.p[i];
  }

  /* connect on server */
  if (prelay->socktype == NETWOX_SOCKTYPE_UDP_SER) {
    ret = netwib_io_init_sock_udp_cli_easy(pserverip, prelay->serverport,
                                           &pioclient);
  } else if (prelay->socktype == NETWOX_SOCKTYPE_TCP_SER ||
             prelay->socktype == NETWOX_SOCKTYPE_TCP_MULSER) {
    ret = netwib_io_init_sock_tcp_cli_easy(pserverip, prelay->serverport,
                                           &pioclient);
  } else {
    return(NETWOX_ERR_INTERNALERROR);
  }
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_fmt_display("Could not connect to %{ip}\n", pserverip));
    return(NETWIB_ERR_OK);
  }

  /* prepare loop */
  netwib_er(netwib_wait_init_io_read(pioserver, &pwaitserver));
  netwib_er(netwib_wait_init_io_read(pioclient, &pwaitclient));
  netwib_er(netwib_buf_init_mallocdefault(&buf));

  /* loop */
  while (NETWIB_TRUE) {
    /* wait */
    netwib_er(netwib_wait_wait2(pwaitserver, pwaitclient, NETWIB_TIME_INFINITE,
                                &eventserver, &eventclient));
    /* read */
    netwib__buf_reinit(&buf);
    pior = (eventserver)?pioserver:pioclient;
    piow = (eventserver)?pioclient:pioserver;
    ret = netwib_io_read(pior, &buf);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret == NETWIB_ERR_DATAEND) {
      ret = NETWIB_ERR_OK;
      break;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    } else {
      if (!allowed && prelay->socktype == NETWOX_SOCKTYPE_UDP_SER) {
        netwib_er(netwox_relay_loop_allow(prelay, pioserver, &allowed));
        if (!allowed) {
          break;
        }
      }
      netwib_er(netwib_io_write(piow, &buf));
    }
  }

  /* close */
  netwib_er(netwib_buf_close(&buf));
  netwib_er(netwib_wait_close(&pwaitclient));
  netwib_er(netwib_wait_close(&pwaitserver));
  netwib_er(netwib_io_close(&pioclient));

  return(ret);
}
