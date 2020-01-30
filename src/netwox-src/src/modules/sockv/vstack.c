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
#include "../../netwox.h"

/*-------------------------------------------------------------*/
/*

Some ideas:

netwox_vstack_init create a manager thread. This thread :
 - listens for sniff. When a packet arrives, it is decoded
   step by step :
     - if ARP for us, answer
     - if IP for us
        - if ping, eventually answer
        - if icmp6nd, eventually answer
        - if tcp:
           - if closed, eventually reset
           - else, send info (seqnum,etc.) and data to pio[port]
        - if udp:
           - if closed, eventually icmp port unreach
           - else, send info (client,etc.) and data to pio[port]
 - creates a new thread when netwox_vstack_udp/tcp_cli/ser_full is
   called. So a computer having two TCP clients will have one manager
   thread and two TCP threads.

Manager thread is connected to main thread using a TLV to transmit :
 - ping configuration
 - tcprst configuration
 - udpunreach configuration
 - new sub-thread creation request
 - close request (netwox_vstack_close to close all, and netwib_io_close
   to close only one sub-thread)
Each of this command is acknowledged with OK, or an error code. If OK,
some data (client address, etc.) can be added. A mutex
has to be used to ensure only one command is submitted at a time.

Sub-threads are connected to the manager to receive sniffed data through
pio[port].
They are also connected to user thread to read and write its data.

Each thread can spoof. A special spoof function has to be used :
 - a mutex ensures only one thread at a time sends data
 - a boolean indicates if the spoof function has to fragment
   or to send directly.

*/

/*-------------------------------------------------------------*/
/* we need this function otherwise gcc refuses to compile this C file
   because it does not contain any code */
netwib_err netwox_unused_vstack(void);
netwib_err netwox_unused_vstack(void)
{
  netwox_unused_vstack();
  return(NETWIB_ERR_OK);
}
