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
netwox_tooltreenode netwox_tooltree[NETWOX_TOOLTREE_NODES_MAX+1];

/*-------------------------------------------------------------*/
#if 0
static netwib_err netwox_tooltree_display(void)
{
  netwox_tooltreenodetype i, subnode;
  netwib_uint32 j;

  for (i = NETWOX_TOOLTREENODETYPE_MAIN; i < NETWOX_TOOLTREE_NODES_MAX; i++) {
    netwib_er(netwib_fmt_display("%{uint32} - %s", i,
                               netwox_tooltree[i].description));
    if ( ! netwox_tooltree[i].canaddtool) {
      netwib_er(netwib_fmt_display(" (no tools)"));
    }
    netwib_er(netwib_fmt_display("\n"));
    for (j = 0; j < netwox_tooltree[i].subnodesset; j++) {
      subnode = netwox_tooltree[i].subnodes[j];
      if (subnode < NETWOX_TOOLTREE_TOOL_START) {
        netwib_er(netwib_fmt_display(" %{uint32} %s\n", subnode,
                                   netwox_tooltree[subnode].description));
      } else {
        netwib_er(netwib_fmt_display(" %{uint32}\n", subnode));
      }
    }
    netwib_er(netwib_fmt_display("\n"));
  }

  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
static netwib_err netwox_tooltree_title_buf(netwib_constbuf *pbuf)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_byte(' ', &buf));
  netwib_er(netwib_buf_append_buf(pbuf, &buf));
  netwib_er(netwib_buf_append_byte(' ', &buf));
  netwib_er(netwib_fmt_display("\n%{c#60;buf}\n", &buf));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_tooltree_title_text(netwib_conststring str)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_ext_string(str, &buf));
  netwib_er(netwox_tooltree_title_buf(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_tooltree_tool_select(netwib_uint32 *ptoolnum)
{
  netwib_buf message;
  netwib_uint32 toolcount;

  netwib_er(netwib_buf_init_ext_string("Select tool number", &message));
  netwib_er(netwox_tools_count(&toolcount));
  netwib_er(netwib_uint32_init_kbd(&message, 1, toolcount,
                                   NETWIB_UINT32_INIT_KBD_NODEF, ptoolnum));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_tooltree_tool_run(netwib_uint32 toolnum,
                                           netwib_bool usekbd,
                                           netwib_bool displayhelp)
{
  netwib_buf message, cmd;
  int argc;
  netwib_string *argv, filename;
  netwib_char key;
  netwib_err ret;

  netwib_er(netwib_buf_init_mallocdefault(&message));
  netwib_er(netwib_buf_append_fmt(&message, "running tool number %{uint32}",
                                  toolnum));
  netwib_er(netwox_tooltree_title_buf(&message));
  netwib_er(netwib_buf_close(&message));

  if (displayhelp) {
    netwib_er(netwox_tool_help_display(netwox_tools[toolnum].pinfo,
                                       toolnum, NETWIB_TRUE, NETWIB_FALSE));
  }

  netwib_er(netwib_buf_init_mallocdefault(&cmd));
  netwib_er(netwib_buf_append_fmt(&cmd, "netwox %{uint32} ",
                                  toolnum));
  if (usekbd) {
    netwib_er(netwib_fmt_display("Select tool parameters and finish with '-' key.\n"));
    netwib_er(netwib_buf_append_string("--kbd", &cmd));
  } else {
    netwib_er(netwib_fmt_display("Enter optional tool parameters and press Return key.\n"));
    netwib_er(netwib_fmt_display("netwox %{uint32} ", toolnum));
    netwib_er(netwib_buf_append_kbd(NULL, NETWIB_BUF_APPEND_KBD_NODEF, &cmd));
  }

  ret = netwox_cmdline_init(&cmd, &filename, &argc, &argv);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_fmt_display("Parse error in %{buf}\n", &cmd));
  } else {
    ret = netwox_main(argc, argv);
    netwib_er(netwox_cmdline_close(&filename, &argv));
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_fmt_display("\nCommand returned 0 (OK)\n", ret));
    } else {
      netwib_er(netwib_fmt_display("\nCommand returned %{uint32}:\n", ret));
      netwib_er(netwox_err_display(ret, NETWIB_ERR_ENCODETYPE_FULL));
    }
  }

  netwib_er(netwib_buf_close(&cmd));

  netwib_er(netwib_buf_init_ext_string("Press 'r' or 'k' to run again this tool, or any other key to continue", &message));
  netwib_er(netwib_kbd_press(&message, &key));
  if (key == 'r' || key == 'R') {
    netwib_er(netwox_tooltree_tool_run(toolnum, NETWIB_FALSE, NETWIB_TRUE));
  }
  if (key == 'k' || key == 'K') {
    netwib_er(netwox_tooltree_tool_run(toolnum, NETWIB_TRUE, NETWIB_FALSE));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_tooltree_tool_help(netwib_uint32 toolnum)
{
  netwib_buf message;
  netwib_char key;

  netwib_er(netwib_buf_init_mallocdefault(&message));
  netwib_er(netwib_buf_append_fmt(&message, "help for tool number %{uint32}",
                                  toolnum));
  netwib_er(netwox_tooltree_title_buf(&message));
  netwib_er(netwib_buf_close(&message));

  netwib_er(netwox_tool_help_display(netwox_tools[toolnum].pinfo,
                                     toolnum, NETWIB_TRUE, NETWIB_FALSE));

  netwib_er(netwib_buf_init_ext_string("Press 'r' or 'k' to run this tool, or any other key to continue", &message));
  netwib_er(netwib_kbd_press(&message, &key));
  if (key == 'r' || key == 'R') {
    netwib_er(netwox_tooltree_tool_run(toolnum, NETWIB_FALSE, NETWIB_FALSE));
  }
  if (key == 'k' || key == 'K') {
    netwib_er(netwox_tooltree_tool_run(toolnum, NETWIB_TRUE, NETWIB_FALSE));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_tooltree_init_nodes(void)
{
  netwox_tooltreenodetype cur;

#define NETWOX_TTN_CUR(c) cur = c;
#define NETWOX_TTN_INFO(d) netwox_tooltree[cur].description = d; netwox_tooltree[cur].canaddtool = NETWIB_FALSE; netwox_tooltree[cur].subnodesset = 0;
#define NETWOX_TTN_INFOa(d) netwox_tooltree[cur].description = d; netwox_tooltree[cur].canaddtool = NETWIB_TRUE; netwox_tooltree[cur].subnodesset = 0;
#define NETWOX_TTN_SN(t) netwox_tooltree[cur].subnodes[netwox_tooltree[cur].subnodesset++] = t;

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_MAIN);
  NETWOX_TTN_INFO("MAIN MENU");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_INFO);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NETPROTO);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SNIFF);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SPOOF);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_RECORD);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_PING);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_TRACEROUTE);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SCAN);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NETAUDIT);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_BRUTEFORCE);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_REMADM);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NOTNET);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_INFO);
  NETWOX_TTN_INFO("information");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_INFO_LOCAL);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_INFO_REMOTE);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_INFO_NETW);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_INFO_NETW);
  NETWOX_TTN_INFOa("information on netw");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_INFO_LOCAL);
  NETWOX_TTN_INFOa("information on local computer");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_INFO_REMOTE);
  NETWOX_TTN_INFOa("information on remote computer");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_NETPROTO);
  NETWOX_TTN_INFO("network protocol");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NETPROTO_ETH);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NETPROTO_IP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NETPROTO_UDP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NETPROTO_TCP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NETPROTO_ICMP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NETPROTO_ARP);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_NETPROTO_ETH);
  NETWOX_TTN_INFO("Ethernet");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SPOOF_ETH);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NETAUDIT_ETH);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NETPROTO_IP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NETPROTO_ARP);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_NETPROTO_IP);
  NETWOX_TTN_INFO("IP");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SPOOF_IP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NETAUDIT_IP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NETPROTO_UDP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NETPROTO_TCP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NETPROTO_ICMP);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_NETPROTO_UDP);
  NETWOX_TTN_INFO("UDP");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SPOOF_UDP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_UDP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_UDP);
  /*NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NETAUDIT_UDP);*/
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_DHCP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_DNS);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_NTP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_SNMP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_SYSLOG);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_TFTP);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_NETPROTO_TCP);
  NETWOX_TTN_INFO("TCP");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SPOOF_TCP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_TCP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_TCP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NETAUDIT_TCP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_DNS);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_FTP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_HTTP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_IDENT);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_IRC);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_NNTP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_SMB);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_SMTP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_TELNET);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_WHOIS);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_NETPROTO_ICMP);
  NETWOX_TTN_INFO("ICMP");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SPOOF_ICMP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_ICMP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NETAUDIT_ICMP);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_NETPROTO_ARP);
  NETWOX_TTN_INFO("ARP");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SPOOF_ARP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_ARP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NETAUDIT_ARP);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_APPPROTO);
  NETWOX_TTN_INFO("application protocol");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_DHCP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_DNS);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_FTP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_HTTP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_IDENT);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_IRC);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_NNTP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_NTP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_SMB);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_SMTP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_SNMP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_SYSLOG);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_TELNET);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_TFTP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_APPPROTO_WHOIS);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_APPPROTO_DHCP);
  NETWOX_TTN_INFO("dhcp");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_UDP_DHCP);
  /*NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_UDP_DHCP);*/

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_APPPROTO_DNS);
  NETWOX_TTN_INFO("dns");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_UDP_DNS);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_DNS);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_UDP_DNS);
  /*NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_TCP_DNS);*/

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_APPPROTO_FTP);
  NETWOX_TTN_INFO("ftp");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_FTP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_TCP_FTP);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_APPPROTO_HTTP);
  NETWOX_TTN_INFO("http");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_HTTP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_TCP_HTTP);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_APPPROTO_IDENT);
  NETWOX_TTN_INFO("ident");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_IDENT);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_TCP_IDENT);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_APPPROTO_IRC);
  NETWOX_TTN_INFO("irc");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_IRC);
  /*NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_TCP_IRC);*/

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_APPPROTO_NNTP);
  NETWOX_TTN_INFO("nntp");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_NNTP);
  /*NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_TCP_NNTP);*/

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_APPPROTO_NTP);
  NETWOX_TTN_INFO("ntp");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_UDP_NTP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_UDP_NTP);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_APPPROTO_SMTP);
  NETWOX_TTN_INFO("smtp");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_SMTP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_TCP_SMTP);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_APPPROTO_SMB);
  NETWOX_TTN_INFO("smb");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_SMB);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_TCP_SMB);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_APPPROTO_SYSLOG);
  NETWOX_TTN_INFO("syslog");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_UDP_SYSLOG);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_UDP_SYSLOG);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_APPPROTO_SNMP);
  NETWOX_TTN_INFO("snmp");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_UDP_SNMP);
  /*NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_UDP_SNMP);*/

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_APPPROTO_TELNET);
  NETWOX_TTN_INFO("telnet");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_TELNET);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_TCP_TELNET);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_APPPROTO_TFTP);
  NETWOX_TTN_INFO("tftp");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_UDP_TFTP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_UDP_TFTP);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_APPPROTO_WHOIS);
  NETWOX_TTN_INFO("whois");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_WHOIS);
  /*NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_TCP_WHOIS);*/

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SNIFF);
  NETWOX_TTN_INFOa("sniff (capture network packets)");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SPOOF);
  NETWOX_TTN_INFO("spoof (create and send packets)");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SPOOF_ETH);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SPOOF_IP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SPOOF_UDP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SPOOF_TCP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SPOOF_ICMP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SPOOF_ARP);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SPOOF_ETH);
  NETWOX_TTN_INFOa("Ethernet spoof");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SPOOF_IP);
  NETWOX_TTN_INFOa("IP spoof");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SPOOF_UDP);
  NETWOX_TTN_INFOa("UDP spoof");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SPOOF_TCP);
  NETWOX_TTN_INFOa("TCP spoof");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SPOOF_ICMP);
  NETWOX_TTN_INFOa("ICMP spoof");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SPOOF_ARP);
  NETWOX_TTN_INFOa("ARP spoof");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_RECORD);
  NETWOX_TTN_INFOa("record (file containing captured packets)");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_CLIENT);
  NETWOX_TTN_INFO("client");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_UDP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_TCP);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_CLIENT_UDP);
  NETWOX_TTN_INFO("UDP client");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_UDP_GENERIC);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_UDP_DHCP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_UDP_DNS);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_UDP_NTP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_UDP_SNMP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_UDP_SYSLOG);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_UDP_TFTP);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_CLIENT_UDP_GENERIC);
  NETWOX_TTN_INFOa("generic UDP client");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_CLIENT_UDP_DHCP);
  NETWOX_TTN_INFOa("DHCP client");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_CLIENT_UDP_DNS);
  NETWOX_TTN_INFOa("DNS (udp) client");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_CLIENT_UDP_NTP);
  NETWOX_TTN_INFOa("NTP client");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_CLIENT_UDP_SNMP);
  NETWOX_TTN_INFOa("SNMP client");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_CLIENT_UDP_SYSLOG);
  NETWOX_TTN_INFOa("SYSLOG client");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_CLIENT_UDP_TFTP);
  NETWOX_TTN_INFOa("TFTP client");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_CLIENT_TCP);
  NETWOX_TTN_INFO("TCP client");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_GENERIC);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_DNS);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_FTP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_HTTP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_IDENT);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_IRC);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_NNTP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_SMB);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_SMTP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_TELNET);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_WHOIS);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_GENERIC);
  NETWOX_TTN_INFOa("generic TCP client");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_DNS);
  NETWOX_TTN_INFOa("DNS (tcp) client");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_FTP);
  NETWOX_TTN_INFOa("FTP client");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_HTTP);
  NETWOX_TTN_INFOa("HTTP client");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_HTTP_SPIDER);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_HTTP_SPIDER);
  NETWOX_TTN_INFOa("web spider (website download)");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_IDENT);
  NETWOX_TTN_INFOa("IDENT client");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_IRC);
  NETWOX_TTN_INFOa("IRC client");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_NNTP);
  NETWOX_TTN_INFOa("NNTP client");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_SMB);
  NETWOX_TTN_INFOa("SMB client");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_SMTP);
  NETWOX_TTN_INFOa("SMTP client");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_TELNET);
  NETWOX_TTN_INFOa("TELNET client");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_CLIENT_TCP_WHOIS);
  NETWOX_TTN_INFOa("WHOIS client");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SERVER);
  NETWOX_TTN_INFO("server");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_UDP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_TCP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_ICMP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_ARP);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SERVER_UDP);
  NETWOX_TTN_INFO("UDP server");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_UDP_GENERIC);
  /*NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_UDP_DHCP);*/
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_UDP_DNS);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_UDP_NTP);
  /*NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_UDP_SNMP);*/
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_UDP_SYSLOG);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_UDP_TFTP);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SERVER_UDP_GENERIC);
  NETWOX_TTN_INFOa("generic UDP server");

  /*NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SERVER_UDP_DHCP);*/
  /*NETWOX_TTN_INFOa("DHCP server");*/

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SERVER_UDP_DNS);
  NETWOX_TTN_INFOa("DNS (udp) server");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SERVER_UDP_NTP);
  NETWOX_TTN_INFOa("NTP server");

  /*NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SERVER_UDP_SNMP);*/
  /*NETWOX_TTN_INFOa("SNMP server");*/

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SERVER_UDP_SYSLOG);
  NETWOX_TTN_INFOa("SYSLOG server");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SERVER_UDP_TFTP);
  NETWOX_TTN_INFOa("TFTP server");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SERVER_TCP);
  NETWOX_TTN_INFO("TCP server");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_TCP_GENERIC);
  /*NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_TCP_DNS);*/
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_TCP_FTP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_TCP_HTTP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_TCP_IDENT);
  /*NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_TCP_IRC);*/
  /*NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_TCP_NNTP);*/
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_TCP_SMB);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_TCP_SMTP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_TCP_TELNET);
  /*NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_SERVER_TCP_WHOIS);*/

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SERVER_TCP_GENERIC);
  NETWOX_TTN_INFOa("generic TCP server");

  /*NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SERVER_TCP_DNS);*/
  /*NETWOX_TTN_INFOa("DNS (tcp) server");*/

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SERVER_TCP_FTP);
  NETWOX_TTN_INFOa("FTP server");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SERVER_TCP_HTTP);
  NETWOX_TTN_INFOa("HTTP server");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SERVER_TCP_IDENT);
  NETWOX_TTN_INFOa("IDENT server");

  /*NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SERVER_TCP_IRC);*/
  /*NETWOX_TTN_INFOa("IRC server");*/

  /*NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SERVER_TCP_NNTP);*/
  /*NETWOX_TTN_INFOa("NNTP server");*/

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SERVER_TCP_SMB);
  NETWOX_TTN_INFOa("SMB server");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SERVER_TCP_SMTP);
  NETWOX_TTN_INFOa("SMTP server");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SERVER_TCP_TELNET);
  NETWOX_TTN_INFOa("TELNET server");

  /*NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SERVER_TCP_WHOIS);*/
  /*NETWOX_TTN_INFOa("WHOIS server");*/

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SERVER_ICMP);
  NETWOX_TTN_INFOa("ICMP server");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SERVER_ARP);
  NETWOX_TTN_INFOa("ARP server");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_PING);
  NETWOX_TTN_INFOa("ping (check if a computer if reachable)");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_TRACEROUTE);
  NETWOX_TTN_INFOa("traceroute (obtain list of gateways)");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_SCAN);
  NETWOX_TTN_INFOa("scan (computer and port discovery)");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_NETAUDIT);
  NETWOX_TTN_INFO("network audit");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NETAUDIT_ETH);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NETAUDIT_IP);
  /*NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NETAUDIT_UDP);*/
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NETAUDIT_TCP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NETAUDIT_ICMP);
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NETAUDIT_ARP);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_NETAUDIT_ETH);
  NETWOX_TTN_INFOa("network audit using Ethernet");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_NETAUDIT_IP);
  NETWOX_TTN_INFOa("network audit using IP");

  /*NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_NETAUDIT_UDP);*/
  /*NETWOX_TTN_INFOa("network audit using UDP");*/

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_NETAUDIT_TCP);
  NETWOX_TTN_INFOa("network audit using TCP");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_NETAUDIT_ICMP);
  NETWOX_TTN_INFOa("network audit using ICMP");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_NETAUDIT_ARP);
  NETWOX_TTN_INFOa("network audit using ARP");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_BRUTEFORCE);
  NETWOX_TTN_INFOa("brute force (check if passwords are weak)");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_REMADM);
  NETWOX_TTN_INFOa("remote administration");

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_NOTNET);
  NETWOX_TTN_INFOa("tools not related to network");
  NETWOX_TTN_SN(NETWOX_TOOLTREENODETYPE_NOTNET_FILE);

  NETWOX_TTN_CUR(NETWOX_TOOLTREENODETYPE_NOTNET_FILE);
  NETWOX_TTN_INFOa("tools working on files");

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_tooltree_update_tools(void)
{
  const netwox_tooltreenodetype *ptreenode;
  netwox_tooltreenodetype toolnode;
  netwib_uint32 i, toolcount, subnodesset;

  netwib_er(netwox_tools_count(&toolcount));
  for (i = 1; i <= toolcount; i++) {
    if (netwox_tools[i].pinfo != NULL) {
      ptreenode = netwox_tools[i].pinfo->treenode;
      while (*ptreenode != NETWOX_TOOLTREENODETYPE_END) {
        if ( ! netwox_tooltree[*ptreenode].canaddtool) {
          netwib_er(netwib_fmt_display("Node %{uint32} cannot receive tools (%{uint32})\n", *ptreenode, i));
          return(NETWOX_ERR_TOOLTREE_INIT);
        }
        subnodesset = netwox_tooltree[*ptreenode].subnodesset;
        if (subnodesset >= NETWOX_TOOLTREE_SUBNODES_MAX) {
          netwib_er(netwib_fmt_display("Too many subnodes for node %{uint32}\n",
                                       *ptreenode));
          return(NETWOX_ERR_TOOLTREE_INIT);
        }
        toolnode = (netwox_tooltreenodetype)(i + NETWOX_TOOLTREE_TOOL_START);
        netwox_tooltree[*ptreenode].subnodes[subnodesset] = toolnode;
        netwox_tooltree[*ptreenode].subnodesset++;
        ptreenode++;
      }
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_tooltree_init(void)
{
  netwib_er(netwox_tooltree_init_nodes());
  /*netwib_er(netwox_tooltree_display());*/

  netwib_er(netwox_tooltree_update_tools());
  /*netwib_er(netwox_tooltree_display());*/

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bool onefound;
  netwib_bool *tools;
} netwox_tooltree_match;

/*-------------------------------------------------------------*/
static netwib_err netwox_tooltree_helpmode_searchtool(netwox_tool_info *pinfo,
                                                      netwib_constbuf *psearch,
                                                      netwib_bool *pfound)
{
  netwib_buf buf;

  *pfound = NETWIB_FALSE;

  netwib_er(netwib_buf_init_ext_string(pinfo->title, &buf));
  netwib_er(netwox_search_exact(&buf, psearch, pfound));
  if (*pfound) {
    return(NETWIB_ERR_OK);
  }

  if (pinfo->synonyms != NULL) {
    netwib_er(netwib_buf_init_ext_string(pinfo->synonyms, &buf));
    netwib_er(netwox_search_exact(&buf, psearch, pfound));
    if (*pfound) {
      return(NETWIB_ERR_OK);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_tooltree_helpmode_searchrec(netwox_tooltreenodetype curnode,
                                                     netwib_constbuf *psearch,
                                                     netwox_tooltree_match *ptoolmatch)
{
  netwox_tooltreenodetype subnode;
  netwib_uint32 j, toolnum;
  netwib_bool found;

  for (j = 0; j < netwox_tooltree[curnode].subnodesset; j++) {
    subnode = netwox_tooltree[curnode].subnodes[j];
    if (subnode < NETWOX_TOOLTREE_TOOL_START) {
      netwib_er(netwox_tooltree_helpmode_searchrec(subnode, psearch,
                                                   ptoolmatch));
    } else {
      toolnum = subnode - NETWOX_TOOLTREE_TOOL_START;
      if (!ptoolmatch->tools[toolnum]) {
        netwib_er(netwox_tooltree_helpmode_searchtool(netwox_tools[toolnum].pinfo, psearch, &found));
        if (found) {
          ptoolmatch->tools[toolnum] = NETWIB_TRUE;
          ptoolmatch->onefound = NETWIB_TRUE;
        }
      }
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_tooltree_helpmode_searchroot(netwox_tooltreenodetype curnode,
                                                      netwib_constbuf *psearch,
                                                      netwox_tooltree_match *ptoolmatch)
{
  netwib_buf search;

  search = *psearch;
  while (NETWIB_TRUE) {
    netwib_er(netwib_fmt_display("Tools containing \"%{buf}\":\n", &search));
    netwib_er(netwox_tooltree_helpmode_searchrec(curnode, &search,
                                                 ptoolmatch));
    if (ptoolmatch->onefound) {
      break;
    }
    if (netwib__buf_ref_data_size(&search) < 4) {
      break;
    }
    netwib_er(netwib_fmt_display("  No tool matches. Try using one character less.\n"));
    search.endoffset--;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_tooltree_helpmode_search(netwox_tooltreenodetype curnode)
{
  netwib_buf message, search;
  netwib_uint32 i, toolcount;
  netwox_tooltree_match toolmatch;

  /* ask searched string */
  netwib_er(netwib_buf_init_mallocdefault(&search));
  netwib_er(netwib_buf_init_ext_string("Enter search string", &message));
  netwib_er(netwib_buf_append_kbd(&message, NETWIB_BUF_APPEND_KBD_NODEF,
                                  &search));

  /* allocate the array containing matching items */
  netwib_er(netwox_tools_count(&toolcount));
  netwib_er(netwib_ptr_malloc((toolcount+1)*sizeof(netwib_bool),
                              (netwib_ptr*)&toolmatch.tools));
  toolmatch.onefound = NETWIB_FALSE;
  for (i = 0; i <= toolcount; i++) {
    toolmatch.tools[i] = NETWIB_FALSE;
  }

  /* set array items */
  netwib_er(netwox_tooltree_title_text("list of tools containing this text"));
  netwib_er(netwox_tooltree_helpmode_searchroot(curnode, &search, &toolmatch));
  netwib_er(netwib_buf_close(&search));

  /* display patching items */
  if (toolmatch.onefound) {
    for (i = 0; i <= toolcount; i++) {
      if (toolmatch.tools[i]) {
        netwib_er(netwib_fmt_display("  %{uint32}:%s\n", i,
                                     netwox_tools[i].pinfo->title));
      }
    }
  } else {
    netwib_er(netwib_fmt_display("  No tool matches this text.\n"));
  }
  netwib_er(netwib_ptr_free((netwib_ptr*)&toolmatch.tools));

  /* key press */
  netwib_er(netwib_buf_init_ext_string("Press any key to continue", &message));
  netwib_er(netwib_kbd_press(&message, NULL));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 numset;
  netwox_tooltreenodetype chain[NETWOX_TOOLTREE_DEEP_MAX];
} netwox_tooltree_nodeschain;

/*-------------------------------------------------------------*/
static netwib_err netwox_tooltree_helpmode_node(netwox_tooltree_nodeschain *pnodeschain)
{
  netwox_tooltreenodetype curnode, subnode;
  netwib_buf allowedchars, message;
  netwib_uint32 i, toolnum;
  netwib_bool toolfound;
  netwib_char c;

  curnode = pnodeschain->chain[pnodeschain->numset-1];

  /* contain list of allowed keys */
  netwib_er(netwib_buf_init_mallocdefault(&allowedchars));

  /* display header */
  netwib_er(netwox_tooltree_title_text(netwox_tooltree[curnode].description));
  netwib_er(netwib_fmt_display(" 0 - leave netwox\n"));
  netwib_er(netwib_buf_append_byte('0', &allowedchars));
  if (curnode != NETWOX_TOOLTREENODETYPE_MAIN) {
    netwib_er(netwib_fmt_display(" 1 - go to main menu\n"));
    netwib_er(netwib_fmt_display(" 2 - go to previous menu\n"));
    netwib_er(netwib_buf_append_string("12", &allowedchars));
  }
  netwib_er(netwib_fmt_display(" 3 - search tools\n"));
  netwib_er(netwib_fmt_display(" 4 - display help of one tool\n"));
  netwib_er(netwib_fmt_display(" 5 - run a tool selecting parameters on command line\n"));
  netwib_er(netwib_fmt_display(" 6 - run a tool selecting parameters from keyboard\n"));
  netwib_er(netwib_buf_append_string("3456", &allowedchars));

  /* display sub nodes and tools */
  c = (netwib_char)('a' - 1);
  toolfound = NETWIB_FALSE;
  for (i = 0; i < netwox_tooltree[curnode].subnodesset; i++) {
    /* obtain next char */
    if (c == 'z') {
      c = (netwib_char)('A' - 1);
    } else if (c == 'Z') {
      netwib_er(netwib_fmt_display("This node (%{uint32}) contains too many tools (>52)\n", curnode));
      return(NETWOX_ERR_TOOLTREE_INIT);
    } else {
      c++;
    }
    /* display line */
    subnode = netwox_tooltree[curnode].subnodes[i];
    if (subnode < NETWOX_TOOLTREE_TOOL_START) {
      netwib_er(netwib_fmt_display(" %c + %s\n", c,
                                   netwox_tooltree[subnode].description));
    } else {
      toolnum = subnode - NETWOX_TOOLTREE_TOOL_START;
      netwib_er(netwib_fmt_display(" %c - %{uint32}:%s\n", c, toolnum,
                                   netwox_tools[toolnum].pinfo->title));
      toolfound = NETWIB_TRUE;
    }
    /* add in allowed chars */
    netwib_er(netwib_buf_append_byte(c, &allowedchars));
  }
  if (!toolfound && netwox_tooltree[curnode].canaddtool) {
    netwib_er(netwib_fmt_display("     **Currently, there is no tool in this node.**\n"));
  }

  /* ask user */
  netwib_er(netwib_buf_init_ext_string("Select a node", &message));
  netwib_er(netwib_char_init_kbd(&message, &allowedchars,
                                 NETWIB_CHAR_INIT_KBD_NODEF, &c));
  netwib_er(netwib_buf_close(&allowedchars));

  /* action depend on pressed key */
  if (c == '0') {
    pnodeschain->numset = 0;
  } else if (c == '1') {
    pnodeschain->numset = 1;
  } else if (c == '2') {
    pnodeschain->numset--;
  } else if (c == '3') {
    netwib_er(netwox_tooltree_helpmode_search(curnode));
  } else if (c == '4') {
    netwib_er(netwox_tooltree_tool_select(&toolnum));
    netwib_er(netwox_tooltree_tool_help(toolnum));
  } else if (c == '5') {
    netwib_er(netwox_tooltree_tool_select(&toolnum));
    netwib_er(netwox_tooltree_tool_run(toolnum, NETWIB_FALSE, NETWIB_TRUE));
  } else if (c == '6') {
    netwib_er(netwox_tooltree_tool_select(&toolnum));
    netwib_er(netwox_tooltree_tool_run(toolnum, NETWIB_TRUE, NETWIB_FALSE));
  } else {
    /* determine subnode from key letter */
    if (netwib_c2_islower(c)) {
      i = c - 'a';
    } else {
      i = c - 'A' - 26;
    }
    subnode = netwox_tooltree[curnode].subnodes[i];
    /* go in this node, or display help */
    if (subnode < NETWOX_TOOLTREE_TOOL_START) {
      pnodeschain->chain[pnodeschain->numset] = subnode;
      pnodeschain->numset++;
    } else {
      toolnum = subnode - NETWOX_TOOLTREE_TOOL_START;
      netwib_er(netwox_tooltree_tool_help(toolnum));
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_tooltree_helpmode(void)
{
  netwox_tooltree_nodeschain nodeschain;
  netwib_uint32 versionmajor, versionminor, versionmicro;

  netwib_er(netwox_tooltree_init());

  netwib_er(netwib_internal_version(&versionmajor, &versionminor,
                                    &versionmicro));
  netwib_er(netwib_fmt_display("Netwox toolbox version %{uint32}.%{uint32}.%{uint32}. Netwib library version %{uint32}.%{uint32}.%{uint32}.\n",
                               NETWOXDEF_VERSIONMAJOR, NETWOXDEF_VERSIONMINOR,
                               NETWOXDEF_VERSIONMICRO,
                               versionmajor, versionminor, versionmicro));

  /* initialize chain */
  nodeschain.numset = 1;
  nodeschain.chain[0] = NETWOX_TOOLTREENODETYPE_MAIN;

  /* main loop */
  while (NETWIB_TRUE) {
    netwib_er(netwox_tooltree_helpmode_node(&nodeschain));
    if (nodeschain.numset == 0) {
      return(NETWOX_ERR_TOOLTREE_HELPMODE_END);
    }
  }

  return(NETWIB_ERR_OK);
}
