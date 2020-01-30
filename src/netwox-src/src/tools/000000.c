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
netwib_conststring t000000_description[] = {
  "This tool is not designed to be directly used by users.",
  "It is only needed by netwag.",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000000_args[] = {
  NETWOX_TOOLARG_OPT_BOOL('t', "tools", NULL, NULL),
  NETWOX_TOOLARG_OPT_BOOL('h', "toolhelp", NULL, NULL),
  NETWOX_TOOLARG_OPT_BOOL('f', "formupdate", NULL, NULL),
  NETWOX_TOOLARG_OPT_BOOL('r', "run", NULL, NULL),
  NETWOX_TOOLARG_OPT_BOOL('R', "run-key", NULL, NULL),
  NETWOX_TOOLARG_OPT_BOOL('k', "kill", NULL, NULL),
  NETWOX_TOOLARG_OPT_BOOL('e', "error", NULL, NULL),
  NETWOX_TOOLARG_OPT_BOOL('c', "conf", NULL, NULL),
  NETWOX_TOOLARG_OPT_BOOL('v', "version", NULL, NULL),
  NETWOX_TOOLARG_OPT_BUF('b', "buf", NULL, NULL),
  NETWOX_TOOLARG_OPT_UINT32('u', "uint", NULL, NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000000_nodes[] = {
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000000_info = {
  "Obtain information needed by netwag",
  t000000_description,
  NULL,
  t000000_args,
  t000000_nodes,
};

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 toolcount;
  netwib_buf *pmainbuf;
  netwib_buf buf[NETWOX_TOOLTREE_DEEP_MAX];
#define T000000_NODE_MAX 3000 /* sufficient for the moment */
  netwib_bool used[T000000_NODE_MAX];
  netwox_tooltreenodetype branch[NETWOX_TOOLTREE_DEEP_MAX];
  netwox_tooltreenodetype firstfreenode;
  netwox_tooltreenodetype firstfreetool;
} t000000_tools_tree_info;

/*-------------------------------------------------------------*/
static netwib_err t000000_tools_tree_rename(t000000_tools_tree_info *pinfo,
                                            netwox_tooltreenodetype curnode,
                                            netwox_tooltreenodetype *pcurnoderen)
{
  if (curnode >= T000000_NODE_MAX) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  if (!pinfo->used[curnode]) {
    *pcurnoderen = curnode;
    pinfo->used[curnode] = NETWIB_TRUE;
  } else {
    if (curnode < NETWOX_TOOLTREE_TOOL_START) {
      *pcurnoderen = pinfo->firstfreenode;
      /* we do not use "++" because g++ does not known how to increment a
         netwox_tooltreenodetype */
      pinfo->firstfreenode = (netwox_tooltreenodetype)(pinfo->firstfreenode + 1);
    } else {
      *pcurnoderen = pinfo->firstfreetool;
      pinfo->firstfreetool = (netwox_tooltreenodetype)(pinfo->firstfreetool + 1);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000000_tools_tree(t000000_tools_tree_info *pinfo,
                                     netwox_tooltreenodetype curnode,
                                     netwox_tooltreenodetype curnoderen,
                                     netwib_uint32 deep)
{

  netwib_char deepspace[32], smallspace[2];
  netwox_tooltreenodetype subnode, subnoderen;
  netwib_uint32 i, j, toolnum;
  netwib_bool subnodesadded;

  if (deep >= sizeof(deepspace)) {
    return(NETWOX_ERR_INTERNALERROR);
  }
  netwib_c_strcpy(deepspace, "                              ");
  deepspace[deep] = '\0';
  smallspace[0] = ' ';
  smallspace[1] = '\0';

  pinfo->branch[deep] = curnode;

  netwib_er(netwib_buf_append_fmt(pinfo->pmainbuf, "set netwag_priv_toolsearch_tools_tree_expand(%{uint32}) 0\n", curnoderen));

  netwib__buf_reinit(&pinfo->buf[deep]);
  netwib_er(netwib_buf_append_fmt(&pinfo->buf[deep], "set netwag_priv_toolsearch_tools_tree_nodes(%{uint32}) {", curnoderen));

  subnodesadded = NETWIB_FALSE;
  for (j = 0; j < netwox_tooltree[curnode].subnodesset; j++) {
    subnode = netwox_tooltree[curnode].subnodes[j];
    netwib_er(t000000_tools_tree_rename(pinfo, subnode, &subnoderen));
    netwib_er(netwib_buf_append_fmt(&pinfo->buf[deep], " %{uint32}",
                                  subnoderen));
    if (subnode < NETWOX_TOOLTREE_TOOL_START) {
      netwib_er(netwib_buf_append_fmt(pinfo->pmainbuf, "set netwag_priv_toolsearch_tools_tree_text(%{uint32}) {%s+ %s}\n", subnoderen, deepspace, netwox_tooltree[subnode].description));
      netwib_er(t000000_tools_tree(pinfo, subnode, subnoderen, deep + 1));
      subnodesadded = NETWIB_TRUE;
    } else {
      toolnum = subnode - NETWOX_TOOLTREE_TOOL_START;
      netwib_er(netwib_buf_append_fmt(pinfo->pmainbuf, "set netwag_priv_toolsearch_tools_tree_text(%{uint32}) {%s%s%{uint32} : %s}\n", subnoderen, subnodesadded?"":smallspace, deepspace, toolnum, netwox_tools[toolnum].pinfo->title));
      toolnum = subnoderen - NETWOX_TOOLTREE_TOOL_START;
      if (toolnum <= pinfo->toolcount) {
        netwib_er(netwib_buf_append_fmt(pinfo->pmainbuf, "set netwag_priv_toolsearch_tools_tree_branch(%{uint32}) {", subnode));
        for (i = 0; i <= deep; i++) {
          netwib_er(netwib_buf_append_fmt(pinfo->pmainbuf, " %{uint32}",
                                        pinfo->branch[i]));
        }
        netwib_er(netwib_buf_append_string("}\n", pinfo->pmainbuf));
      }
    }
  }

  netwib_er(netwib_buf_append_fmt(&pinfo->buf[deep], "}\n"));
  netwib_er(netwib_buf_append_buf(&pinfo->buf[deep], pinfo->pmainbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000000_tools(void)
{
  t000000_tools_tree_info info;
  netwib_buf buf, buftxt;
  netwib_uint32 i, toolcount, deep;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwox_tools_count(&toolcount));

  /* max tools */
  netwib_er(netwib_buf_append_fmt(&buf, "set netwag_priv_toolsearch_tools_max %{uint32}\n", toolcount));

  /* sorted list */
  netwib_er(netwib_buf_append_fmt(&buf, "set netwag_priv_toolsearch_tools_sort_max %{uint32}\n", toolcount));
  netwib_er(netwib_buf_append_string("array set netwag_priv_toolsearch_tools_sort_lbline {", &buf));
  for (i = 1; i <= toolcount; i++) {
    if (netwox_tools[i].pinfo != NULL) {
      netwib_er(netwib_buf_append_fmt(&buf, " %{uint32} {%{uint32} : %s}",
                                      i, i, netwox_tools[i].pinfo->title));
    }
  }
  netwib_er(netwib_buf_append_string("}\n", &buf));

  /* synonyms */
  netwib_er(netwib_buf_append_string("array set netwag_priv_toolsearch_tools_synonyms {", &buf));
  for (i = 1; i <= toolcount; i++) {
    if (netwox_tools[i].pinfo != NULL) {
      if (netwox_tools[i].pinfo->synonyms != NULL) {
        netwib_er(netwib_buf_append_fmt(&buf, " %{uint32} {", i));
        netwib_er(netwib_buf_init_ext_string(netwox_tools[i].pinfo->synonyms,
                                           &buftxt));
        netwib_er(netwib_buf_encode(&buftxt, NETWIB_ENCODETYPE_LOWERCASE,
                                    &buf));
        netwib_er(netwib_buf_append_string("}", &buf));
      } else {
        netwib_er(netwib_buf_append_fmt(&buf, " %{uint32} {}", i));
      }
    }
  }
  netwib_er(netwib_buf_append_string("}\n", &buf));

  /* tree list */
  netwib_er(netwox_tooltree_init());
  info.toolcount = toolcount;
  info.pmainbuf = &buf;
  for (i = 0; i < NETWOX_TOOLTREE_DEEP_MAX; i++) {
    netwib_er(netwib_buf_init_mallocdefault(&info.buf[i]));
  }
  for (i = 0; i < T000000_NODE_MAX; i++) {
    info.used[i] = NETWIB_FALSE;
  }
  info.firstfreenode = (netwox_tooltreenodetype)(NETWOX_TOOLTREE_NODES_MAX + 1);
  info.firstfreetool = (netwox_tooltreenodetype)(NETWOX_TOOLTREE_TOOL_START + toolcount + 1);

  deep = 0;
  netwib_er(t000000_tools_tree(&info, NETWOX_TOOLTREENODETYPE_MAIN,
                               NETWOX_TOOLTREENODETYPE_MAIN, deep));
  for (i = 0; i < NETWOX_TOOLTREE_DEEP_MAX; i++) {
    netwib_er(netwib_buf_close(&info.buf[i]));
  }
  netwib_er(netwib_buf_append_fmt(&buf, "set netwag_priv_toolsearch_tools_tree_toolstart %{uint32}\n", NETWOX_TOOLTREE_TOOL_START));

  /* tools needing stdin */
  netwib_er(netwib_buf_append_fmt(&buf, "global netwag_priv_tools_stdin\n"));
  netwib_er(netwib_buf_append_fmt(&buf, "set netwag_priv_tools_stdin(7) 1\n"));
  netwib_er(netwib_buf_append_fmt(&buf,"set netwag_priv_tools_stdin(14) 1\n"));
  netwib_er(netwib_buf_append_fmt(&buf,"set netwag_priv_tools_stdin(87) 1\n"));
  netwib_er(netwib_buf_append_fmt(&buf,"set netwag_priv_tools_stdin(88) 1\n"));
  netwib_er(netwib_buf_append_fmt(&buf,"set netwag_priv_tools_stdin(89) 1\n"));
  netwib_er(netwib_buf_append_fmt(&buf,"set netwag_priv_tools_stdin(90) 1\n"));
  netwib_er(netwib_buf_append_fmt(&buf,"set netwag_priv_tools_stdin(99) 1\n"));
 netwib_er(netwib_buf_append_fmt(&buf,"set netwag_priv_tools_stdin(152) 1\n"));
 netwib_er(netwib_buf_append_fmt(&buf,"set netwag_priv_tools_stdin(171) 1\n"));

  /* tools needing backspace */
  netwib_er(netwib_buf_append_fmt(&buf, "global netwag_priv_tools_backspace\n"));
  netwib_er(netwib_buf_append_fmt(&buf, "set netwag_priv_tools_backspace(138) 1\n"));
  netwib_er(netwib_buf_append_fmt(&buf, "set netwag_priv_tools_backspace(139) 1\n"));
  netwib_er(netwib_buf_append_fmt(&buf, "set netwag_priv_tools_backspace(210) 1\n"));

  /* display */
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000000_toolhelp(netwib_uint32 toolnum)
{
  netwib_buf buf;
  netwox_tool_info *pinfo;
  const netwox_toolarg *ptoolarg;
  netwib_uint32 toolcount;
  netwib_bool hasavanced;

  netwib_er(netwox_tools_count(&toolcount));
  if (toolnum > toolcount) {
    return(NETWOX_ERR_TOOLARG_INVALID);
  }
  pinfo = netwox_tools[toolnum].pinfo;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  /* help */
  netwib_er(netwib_buf_append_fmt(&buf, "set netwag_priv_toolhelp_help(%{uint32}) {", toolnum));
  netwib_er(netwib_buf_append_fmt(&buf, "Tool number : %{uint32}\n", toolnum));
  netwib_er(netwox_tool_help(pinfo, toolnum, NETWIB_TRUE, NETWIB_FALSE, &buf));
  netwib_er(netwib_buf_append_string("}\n", &buf));

  /* example */
  netwib_er(netwib_buf_append_fmt(&buf, "set netwag_priv_toolhelp_example(%{uint32}) {%{uint32}", toolnum, toolnum));
  ptoolarg = pinfo->arg;
  while (ptoolarg->toolargclass != NETWOX_TOOLARGCLASS_END) {
    netwib_er(netwox_buf_append_toolarg(ptoolarg,
                                      NETWOX_TOOLARG_ENCODETYPE_HELPEXAMPLE2,
                                      &buf));
    ptoolarg++;
  }
  netwib_er(netwib_buf_append_string("}\n", &buf));

  /* usage */
  netwib_er(netwib_buf_append_fmt(&buf, "set netwag_priv_toolhelp_usage(%{uint32}) {%{uint32}", toolnum, toolnum));
  ptoolarg = pinfo->arg;
  while (ptoolarg->toolargclass != NETWOX_TOOLARGCLASS_END) {
    netwib_er(netwox_buf_append_toolarg(ptoolarg,
                                        NETWOX_TOOLARG_ENCODETYPE_HELPUSAGE,
                                        &buf));
    ptoolarg++;
  }
  netwib_er(netwib_buf_append_string("}\n", &buf));

  /* form */
  netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_append_fmt(&buf, "proc netwag_toolform_display_func%{uint32} { } {\n", toolnum));
  netwib_er(netwib_buf_append_fmt(&buf, "  netwag_toolform_add_text \"Parameters for tool %{uint32} (%s):\"\n", toolnum, pinfo->title));
  hasavanced = NETWIB_FALSE;
  ptoolarg = pinfo->arg;
  while (ptoolarg->toolargclass != NETWOX_TOOLARGCLASS_END) {
    if (!ptoolarg->advanced) {
      netwib_er(netwox_buf_append_toolarg(ptoolarg,
                                          NETWOX_TOOLARG_ENCODETYPE_FORM,
                                          &buf));
    } else {
      hasavanced = NETWIB_TRUE;
    }
    ptoolarg++;
  }
  if (hasavanced) {
    netwib_er(netwib_buf_append_string("  netwag_toolform_add_text \"\"\n",
                                     &buf));
    netwib_er(netwib_buf_append_string("  netwag_toolform_add_text \"Advanced parameters:\"\n", &buf));
    ptoolarg = pinfo->arg;
    while (ptoolarg->toolargclass != NETWOX_TOOLARGCLASS_END) {
      if (ptoolarg->advanced) {
        netwib_er(netwox_buf_append_toolarg(ptoolarg,
                                            NETWOX_TOOLARG_ENCODETYPE_FORM,
                                            &buf));
      }
      ptoolarg++;
    }
  }
  netwib_er(netwib_buf_append_string("}\n", &buf));
  netwib_er(netwib_buf_append_string("\n", &buf));

  /* display */
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err t000000_formupdate_quote(netwib_constbuf *pinbuf,
                                           netwib_buf *poutbuf)
{
  netwib_data datain, dataout, dataoutsave;
  netwib_uint32 dataoutneededsize, datainsize;
  netwib_byte c;

  /* obtain references */
  datainsize = netwib__buf_ref_data_size(pinbuf);
  datain = netwib__buf_ref_data_ptr(pinbuf);
  dataoutneededsize = 2*datainsize;
  netwib_er(netwib_buf_wantspace(poutbuf, dataoutneededsize, &dataout));

  /* set data */
  dataoutsave = dataout; /* needed to compute length */
  while (datainsize--) {
    c = *datain++;
    if (c == '\\' || c == '"' || c == '[' || c == '$') {
      *dataout++ = '\\';
      *dataout++ = c;
    } else {
      *dataout++ = c;
    }
  }

  /* indicate end */
  poutbuf->endoffset += dataout - dataoutsave;

  return(NETWIB_ERR_OK);
}
static netwib_err t000000_formupdate(netwib_uint32 toolnum,
                                     netwib_constbuf *pfilename)
{
  netwib_buf cmd, buf, bufquote;
  int argc;
  netwib_string *argv, filename;
  netwox_arg_item *pitem;
  netwox_arg *parg;
  netwib_uint32 i;
  netwib_err ret;

  /* read command */
  netwib_er(netwib_buf_init_mallocdefault(&cmd));
  netwib_er(netwib_buf_append_string("programnameignored ", &cmd));
  ret = netwox_conffile_read_concat(pfilename, &cmd);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_buf_close(&cmd));
    return(ret);
  }

  /* remove command file (this is specific for netwag) */
  netwib_er(netwib_filename_remove(pfilename));

  /* init this command */
  ret = netwox_cmdline_init(&cmd, &filename, &argc, &argv);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_fmt_display("Parse error in %{buf}\n", &cmd));
    netwib_er(netwib_buf_close(&cmd));
    return(ret);
  }

  /* generate update information (toolnum is checked in netwag) */
  if (argc > 1) {
    netwib_er(netwib_buf_init_mallocdefault(&buf));
    netwib_er(netwib_buf_init_mallocdefault(&bufquote));
    ret = netwox_arg_init2(argc-1, argv+1, netwox_tools[toolnum].pinfo,
                           NETWIB_TRUE, &parg);
    if (ret == NETWIB_ERR_OK) {
      for (i = 0; i < parg->argsetnum; i++) {
        pitem = &parg->arg[i];
        if (pitem->setbyuser) {
          netwib_er(netwib_buf_append_string("  netwag_toolform_update ", &buf));
          netwib_er(netwib_buf_append_byte(pitem->key, &buf));
          netwib_er(netwib_buf_append_string(" \"", &buf));
          netwib__buf_reinit(&bufquote);
          netwib_er(netwox_buf_append_toolargstore(&pitem->toolargstore,
                                                   &bufquote));
          netwib_er(t000000_formupdate_quote(&bufquote, &buf));
          netwib_er(netwib_buf_append_string("\"\n", &buf));
        }
      }
      if (parg->moreargc) {
        /* use fake key '-' */
        netwib_er(netwib_buf_append_string("  netwag_toolform_update - \"",
                                         &buf));
        for (i = 0; i < parg->moreargc; i++) {
          if (i) {
            netwib_er(netwib_buf_append_byte(' ', &buf));
          }
          netwib__buf_reinit(&bufquote);
          netwib_er(netwib_buf_append_string(parg->moreargv[i], &bufquote));
          netwib_er(t000000_formupdate_quote(&bufquote, &buf));
        }
        netwib_er(netwib_buf_append_string("\"\n", &buf));
      }
      netwib_er(netwox_arg_close(&parg));
    }
    netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
    netwib_er(netwib_buf_close(&bufquote));
    netwib_er(netwib_buf_close(&buf));
  }

  /* close */
  netwib_er(netwox_cmdline_close(&filename, &argv));
  netwib_er(netwib_buf_close(&cmd));
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err t000000_run(netwib_constbuf *pfilename)
{
  netwib_buf cmd;
  int argc;
  netwib_string *argv, filename;
  netwib_err ret;

  /* read command */
  netwib_er(netwib_buf_init_mallocdefault(&cmd));
  netwib_er(netwib_buf_append_string("programnameignored ", &cmd));
  ret = netwox_conffile_read_concat(pfilename, &cmd);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_buf_close(&cmd));
    return(ret);
  }

  /* remove command file (this is specific for netwag) */
  netwib_er(netwib_filename_remove(pfilename));

  /* execute this command */
  ret = netwox_cmdline_init(&cmd, &filename, &argc, &argv);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_fmt_display("Parse error in %{buf}\n", &cmd));
    netwib_er(netwib_buf_close(&cmd));
    return(ret);
  }

  ret = netwox_main(argc, argv);

  netwib_er(netwox_cmdline_close(&filename, &argv));
  netwib_er(netwib_buf_close(&cmd));
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err t000000_runkey(netwib_constbuf *pfilename)
{
  netwib_buf msg;

  netwib_er(t000000_run(pfilename));

  netwib_er(netwib_buf_init_ext_string("Press any key to leave", &msg));
  netwib_er(netwib_kbd_press(&msg, NULL));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000000_killtool2(netwib_uint32 pid)
{
#if defined NETWIBDEF_SYSNAME_Unix
  int reti;

  reti = kill(pid, SIGTERM);
  if (reti == -1) {
    return(NETWIB_ERR_FUKILL);
  }
#elif defined NETWIBDEF_SYSNAME_Windows
  HANDLE h;
  BOOL err;
  int reti;

  h = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
  if (h == NULL) {
    return(NETWIB_ERR_FUOPENPROCESS);
  }

  err = TerminateProcess(h, NETWIB_ERR_DATAEND);
  if (!err) {
    CloseHandle(h);
    return(NETWIB_ERR_FUTERMINATEPROCESS);
  }

  reti = CloseHandle(h);
  if (!reti) {
    return(NETWIB_ERR_FUCLOSEHANDLE);
  }
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif
  return(NETWIB_ERR_OK);
}
static netwib_err t000000_killtool(netwib_uint32 pid,
                                  netwib_constbuf *pnumber)
{
  netwib_uint32 sleepms;

  netwib_er(netwib_buf_decode_fmt(pnumber, "%{uint32}%$", &sleepms));
  netwib_er(netwib_time_sleep_msec(sleepms));

  netwib_er(t000000_killtool2(pid));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000000_error(netwib_uint32 err)
{
  netwib_er(netwox_err_display((netwib_err)err, NETWIB_ERR_ENCODETYPE_TEXT));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000000_conf(void)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_string("set netwag_priv_infol_out_devices [string trim \"\n",
                                   &buf));
  netwib_er(netwib_buf_append_conf_devices(&buf));
  netwib_er(netwib_buf_append_string("\"];\n", &buf));
  netwib_er(netwib_buf_append_string("set netwag_priv_infol_out_ip [string trim \"\n",
                                   &buf));
  netwib_er(netwib_buf_append_conf_ip(&buf));
  netwib_er(netwib_buf_append_string("\"];\n", &buf));
  netwib_er(netwib_buf_append_string("set netwag_priv_infol_out_arp [string trim \"\n",
                                   &buf));
  netwib_er(netwib_buf_append_conf_arpcache(&buf));
  netwib_er(netwib_buf_append_string("\"];\n", &buf));
  netwib_er(netwib_buf_append_string("set netwag_priv_infol_out_routes [string trim \"\n",
                                   &buf));
  netwib_er(netwib_buf_append_conf_routes(&buf));
  netwib_er(netwib_buf_append_string("\"];\n", &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000000_version(void)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_fmt_display("set netwag_netwox_versionmajor %{uint32}\n",
                               NETWOXDEF_VERSIONMAJOR));
  netwib_er(netwib_fmt_display("set netwag_netwox_versionminor %{uint32}\n",
                               NETWOXDEF_VERSIONMINOR));
  netwib_er(netwib_fmt_display("set netwag_netwox_versionmicro %{uint32}\n",
                               NETWOXDEF_VERSIONMICRO));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err t000000_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_bool tools, toolhelp, formupdate, run, runkey, killtool, error, conf;
  netwib_bool version;
  netwib_buf buf;
  netwib_uint32 ui;
  netwib_err ret;

  netwib_er(netwox_arg_init(argc, argv, &t000000_info, &parg));
  netwib_er(netwox_arg_bool(parg, 't', &tools));
  netwib_er(netwox_arg_bool(parg, 'h', &toolhelp));
  netwib_er(netwox_arg_bool(parg, 'f', &formupdate));
  netwib_er(netwox_arg_bool(parg, 'r', &run));
  netwib_er(netwox_arg_bool(parg, 'R', &runkey));
  netwib_er(netwox_arg_bool(parg, 'k', &killtool));
  netwib_er(netwox_arg_bool(parg, 'e', &error));
  netwib_er(netwox_arg_bool(parg, 'c', &conf));
  netwib_er(netwox_arg_bool(parg, 'v', &version));
  netwib_er(netwox_arg_buf(parg, 'b', &buf));
  netwib_er(netwox_arg_uint32(parg, 'u', &ui));

  ret = NETWIB_ERR_OK;
  if (tools) {
    ret = t000000_tools();
  }
  if (toolhelp) {
    ret = t000000_toolhelp(ui);
  }
  if (formupdate) {
    ret = t000000_formupdate(ui, &buf);
    if (ret != NETWIB_ERR_OK) {
      /* suppress error display */
      ret = NETWOX_ERR_TOOLARG_NOERRDISPLAY;
    }
  }
  if (run) {
    ret = t000000_run(&buf);
  }
  if (runkey) {
    ret = t000000_runkey(&buf);
  }
  if (killtool) {
    /* ignore because might be dead */ t000000_killtool(ui, &buf);
  }
  if (error) {
    ret = t000000_error(ui);
  }
  if (conf) {
    ret = t000000_conf();
  }
  if (version) {
    ret = t000000_version();
  }

  netwib_er(netwox_arg_close(&parg));
  return(ret);
}
