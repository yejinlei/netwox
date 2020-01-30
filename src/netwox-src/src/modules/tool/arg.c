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
#if 0
static netwib_err netwox_arg_display(netwox_arg *parg)
{
  netwox_arg_item *pitem;
  netwib_uint32 i;

  netwib_er(netwib_fmt_display("tot=%{uint32} set=%{uint32}\n",
                               parg->argtotalnum, parg->argsetnum));

  netwib_er(netwib_fmt_display("keyindex :"));
  for (i = 0; i < 256; i++) {
    if (parg->keyindex[i] != 0xFFFFFFFFu) {
      netwib_er(netwib_fmt_display(" %c->%{uint32}", i, parg->keyindex[i]));
    }
  }
  netwib_er(netwib_fmt_display("\n"));

  for (i = 0; i < parg->argsetnum; i++) {
    pitem = &parg->arg[i];
    netwib_er(netwib_fmt_display("%{uint32} : k=%c name=%s r=%{bool} s=%{bool} ",
                                 i, pitem->key, pitem->name,
                                 pitem->required, pitem->setbyuser));
    netwib_er(netwib_fmt_display("sd=%{bool} p=%p ", pitem->setbydefault,
                                 pitem->ptoolarg));
    if (pitem->setbyuser || pitem->setbydefault) {
      netwib_er(netwox_toolargstore_display(&pitem->toolargstore));
    }
    netwib_er(netwib_fmt_display("\n"));
  }

  netwib_er(netwib_fmt_display("ma=%{bool} argc=%{uint32} ava=%{bool} avia=%{bool}\n", parg->allowmorearg, parg->moreargc, parg->moreargvisallocated, parg->moreargvitemsareallocated));
  for (i = 0; i < parg->moreargc; i++) {
    netwib_er(netwib_fmt_display("%{uint32} : %s\n", i, parg->moreargv[i]));
  }

  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
static netwib_err netwox_arg_check(netwox_arg *parg)
{
  netwox_arg_item *pitem;
  netwib_uint32 i;

  for (i = 0; i < parg->argsetnum; i++) {
    pitem = &parg->arg[i];
    if (pitem->required && !pitem->setbyuser) {
      netwib_er(netwib_fmt_display("Option '-%c|--%s' is required\n",
                                   pitem->key, pitem->name));
      return(NETWOX_ERR_TOOLARG_NOTDECODED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_arg_key_isbool(netwox_arg *parg,
                                        netwib_char key,
                                        netwib_bool *pyes)
{
  netwox_arg_item *pitem;

  if (parg->keyindex[(netwib_uint32)key] == 0xFFFFFFFFu) {
    netwib_er(netwib_fmt_display("Option '-%c' is not supported\n", key));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }
  pitem = &parg->arg[parg->keyindex[(netwib_uint32)key]];

  if (pitem->toolargstore.type == NETWOX_TOOLARGTYPE_BOOL ||
      pitem->toolargstore.type == NETWOX_TOOLARGTYPE_BOOL_RADIO1 ||
      pitem->toolargstore.type == NETWOX_TOOLARGTYPE_BOOL_RADIO2 ||
      pitem->toolargstore.type == NETWOX_TOOLARGTYPE_BOOL_RADIO3) {
    *pyes = NETWIB_TRUE;
  } else {
    *pyes = NETWIB_FALSE;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_arg_add(netwox_arg *parg,
                                 netwib_char key,
                                 netwib_constbuf *value)
{
  netwox_arg_item *pitem;
  netwib_err ret;

  if (parg->keyindex[(netwib_uint32)key] == 0xFFFFFFFFu) {
    netwib_er(netwib_fmt_display("Option '-%c' is not supported\n", key));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }
  pitem = &parg->arg[parg->keyindex[(netwib_uint32)key]];

  if (pitem->setbyuser) {
    netwib_er(netwox_toolargstore_reset(&pitem->toolargstore));
    pitem->setbyuser = NETWIB_FALSE;
  }

  ret = netwox_toolargstore_set(value, &pitem->toolargstore);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_fmt_display("Option '-%c|--%s' could not be set\n", key,
                                 pitem->name));
    return(ret);
  }
  pitem->setbyuser = NETWIB_TRUE;

  return(NETWIB_ERR_OK);
}
static netwib_err netwox_arg_add_string(netwox_arg *parg,
                                        netwib_char key,
                                        netwib_conststring value)
{
  netwib_buf buf;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_string(value, &buf));
  ret = netwox_arg_add(parg, key, &buf);

  return(ret);
}
static netwib_err netwox_arg_add_bool(netwox_arg *parg,
                                      netwib_char key,
                                      netwib_bool b)
{
  return(netwox_arg_add_string(parg, key, b?"1":"0"));
}

/*-------------------------------------------------------------*/
static netwib_err netwox_arg_key_init_name_string(netwox_arg *parg,
                                                  netwib_conststring name,
                                                  netwib_char *pkey)
{
  netwox_arg_item *pitem;
  netwib_bool partialfound;
  netwib_uint32 i, partialpos=0, cmpsize;

  partialfound = NETWIB_FALSE;
  for (i = 0; i < parg->argsetnum; i++) {
    pitem = &parg->arg[i];
    if (! netwib_c_strcmp(name, pitem->name)) {
      if (pkey != NULL) *pkey = pitem->key;
      return(NETWIB_ERR_OK);
    }
    cmpsize = netwib_c_strlen(name);
    if (! netwib_c_strncmp(name, pitem->name, cmpsize)) {
      if (partialfound) {
        netwib_er(netwib_fmt_display("Option '--%s' conflicts with --%s and --%s\n",
                                     name, pitem->name,
                                     parg->arg[partialpos].name));
        return(NETWOX_ERR_TOOLARG_NOTDECODED);
      } else {
        partialfound = NETWIB_TRUE;
        partialpos = i;
      }
    }
  }

  if (!partialfound) {
    netwib_er(netwib_fmt_display("Option '--%s' is not supported\n", name));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  if (pkey != NULL) *pkey = parg->arg[partialpos].key;
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
static netwib_err netwox_arg_update_toolarg(netwox_arg *parg,
                                            const netwox_toolarg *ptoolarg)
{
  netwib_buf name;
  netwib_string namestr;
  netwox_arg_item *pitem;
  netwib_uint32 i;

  netwib_er(netwib_buf_init_mallocdefault(&name));
  i = 0;
  while (ptoolarg[i].toolargclass != NETWOX_TOOLARGCLASS_END) {
    if (parg->argsetnum > parg->argtotalnum) return(NETWOX_ERR_INTERNALERROR);
    pitem = &parg->arg[parg->argsetnum];
    /* check class */
    switch(ptoolarg[i].toolargclass) {
      case NETWOX_TOOLARGCLASS_OPT :
        pitem->required = NETWIB_FALSE;
        break;
      case NETWOX_TOOLARGCLASS_REQ :
        pitem->required = NETWIB_TRUE;
        break;
      case NETWOX_TOOLARGCLASS_MORE :
        parg->allowmorearg = NETWIB_TRUE;
        /* skip directly to next entry */
        i++;
        continue;
      default :
        netwib_er(netwib_fmt_display("Option '-%c' has an unknown class (%{uint32})\n", ptoolarg[i].key, ptoolarg[i].toolargclass));
        netwib_er(netwib_buf_close(&name));
        return(NETWOX_ERR_TOOLARG_NOTDECODED);
        break;
    }
    /* option '-' is not valid */
    if (ptoolarg[i].key == '-') {
      netwib_er(netwib_fmt_display("Option '-' is invalid\n"));
      return(NETWOX_ERR_TOOLARG_NOTDECODED);
    }
    /* set key index */
    if (parg->keyindex[(int)ptoolarg[i].key] != 0xFFFFFFFFu) {
      netwib_er(netwib_fmt_display("Option '-%c' has several meanings\n",
                                 ptoolarg[i].key));
      netwib_er(netwib_buf_close(&name));
      return(NETWOX_ERR_TOOLARG_NOTDECODED);
    }
    parg->keyindex[(int)ptoolarg[i].key] = parg->argsetnum;
    /* set easy values in the item */
    pitem->key = ptoolarg[i].key;
    pitem->ptoolarg = &ptoolarg[i];
    pitem->setbyuser = NETWIB_FALSE;
    pitem->setbydefault = NETWIB_FALSE;
    /* now, init toolargstore */
    netwib_er(netwox_toolargstore_init(pitem->ptoolarg, &pitem->toolargstore));
    /* set name (check size) */
    netwib__buf_reinit(&name);
    netwib_er(netwox_toolarg_get_name(&ptoolarg[i], &name));
    netwib_er(netwib_buf_ref_string(&name, &namestr));
    if (!netwib_c_strcmp(namestr, "help") ||
        !netwib_c_strcmp(namestr, "kbd") ||
        !netwib_c_strcmp(namestr, "argfile") ) {
      netwib_er(netwib_fmt_display("Option '-%c' has a reserved name (--%s)\n",
                                 ptoolarg[i].key, namestr));
      netwib_er(netwib_buf_close(&name));
      return(NETWOX_ERR_TOOLARG_NOTDECODED);
    }
    if (netwib_c_strlen(namestr) > sizeof(pitem->name)-1) {
      netwib_er(netwib_fmt_display("Option '-%c' has a name too long (--%s)\n",
                                 ptoolarg[i].key, namestr));
      netwib_er(netwib_buf_close(&name));
      return(NETWOX_ERR_TOOLARG_NOTDECODED);
    }
    netwib_c_strcpy(pitem->name, namestr);
    /* one more is set */
    parg->argsetnum++;
    i++;
  }
  netwib_er(netwib_buf_close(&name));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_arg_update_argcargv_req(netwox_arg *parg,
                                                 netwib_conststring argv,
                                                 netwib_bool *pskiptomore)
{
  netwox_arg_item *pitem=NULL;
  netwib_uint32 i;
  netwib_err ret;

  /* find first unset arg */
  *pskiptomore = NETWIB_TRUE;
  for (i = 0; i < parg->argsetnum; i++) {
    pitem = &parg->arg[i];
    if (pitem->required && !pitem->setbyuser) {
      *pskiptomore = NETWIB_FALSE;
      break;
    }
  }
  if (*pskiptomore) {
    return(NETWIB_ERR_OK);
  }

  /* try to set it */
  ret = netwox_toolargstore_set_string(argv, &pitem->toolargstore);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_fmt_display("Option '-%c|--%s' could not be set from %s\n", pitem->key, pitem->name, argv));
    return(ret);
  }
  pitem->setbyuser = NETWIB_TRUE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_arg_update_argcargv_opt_short(netwox_arg *parg,
                                                       int argc,
                                                       char *argv[],
                                                       int *pi)
{
  netwib_char *pc, key;
  netwib_bool isbool;

  /* "-bbbb" */
  if (netwib_c_strlen(argv[*pi]) > 2) {
    /* they all must be booleans */
    pc = argv[*pi] + 1;
    while (*pc != '\0') {
      if (parg->keyindex[(netwib_uint32)*pc] == 0xFFFFFFFFu) {
        netwib_er(netwib_fmt_display("Option '%c' is not supported (found in %s)\n", *pc, argv[*pi]));
        return(NETWOX_ERR_TOOLARG_NOTDECODED);
      }
      netwib_er(netwox_arg_key_isbool(parg, *pc, &isbool));
      if (!isbool) {
        netwib_er(netwib_fmt_display("Option '%c' is not boolean (found in %s)\n", *pc, argv[*pi]));
        return(NETWOX_ERR_TOOLARG_NOTDECODED);
      }
      netwib_er(netwox_arg_add_bool(parg, *pc, NETWIB_TRUE));
      pc++;
    }
    return(NETWIB_ERR_OK);
  }

  key = argv[*pi][1];

  /* "-b" */
  netwib_er(netwox_arg_key_isbool(parg, key, &isbool));
  if (isbool) {
    netwib_er(netwox_arg_add_bool(parg, key, NETWIB_TRUE));
    return(NETWIB_ERR_OK);
  }

  /* "-k value" */
  (*pi)++;
  if (*pi >= argc){
    netwox_arg_item *pitem;
    pitem = &parg->arg[parg->keyindex[(netwib_uint32)key]];
    netwib_er(netwib_fmt_display("Option '-%c|--%s' must be followed by a value\n", key, pitem->name));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }
  netwib_er(netwox_arg_add_string(parg, key, argv[*pi]));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_arg_update_argcargv_opt_short_plus(netwox_arg *parg,
                                                            netwib_conststring argv)
{
  netwib_conststring pc;
  netwib_char key;
  netwib_bool isbool;

  /* "+bbbb" */
  if (netwib_c_strlen(argv) > 2) {
    /* they all must be booleans */
    pc = argv + 1;
    while (*pc != '\0') {
      if (parg->keyindex[(netwib_uint32)*pc] == 0xFFFFFFFFu) {
        netwib_er(netwib_fmt_display("Option '%c' is not supported (found in %s)\n", *pc, argv));
        return(NETWOX_ERR_TOOLARG_NOTDECODED);
      }
      netwib_er(netwox_arg_key_isbool(parg, *pc, &isbool));
      if (!isbool) {
        netwib_er(netwib_fmt_display("Option '%c' is not boolean (found in %s)\n", *pc, argv));
        return(NETWOX_ERR_TOOLARG_NOTDECODED);
      }
      netwib_er(netwox_arg_add_bool(parg, *pc, NETWIB_FALSE));
      pc++;
    }
    return(NETWIB_ERR_OK);
  }

  key = argv[1];

  /* "+b" */
  netwib_er(netwox_arg_key_isbool(parg, key, &isbool));
  if (!isbool) {
    netwib_er(netwib_fmt_display("Option '%c' is not boolean\n", key));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  netwib_er(netwox_arg_add_bool(parg, key, NETWIB_FALSE));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_arg_update_argcargv(netwox_arg *parg,
                                             int argc,
                                             char *argv[],
                                             netwib_bool formupdate);
static netwib_err netwox_arg_update_argfile(netwox_arg *parg,
                                            netwib_conststring argfile)
{
  netwib_buf buf, argfilename;
  netwib_string *argv, filename;
  int argc;
  netwib_err ret;

  /* from now on, moreargs will be allocated (even if we do not
     find any morearg in argfile) */
  parg->moreargvitemsareallocated = NETWIB_TRUE;

  /* read each line of the file, and concatenate them */
  netwib_er(netwib_buf_init_ext_string(argfile, &argfilename));
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_string("programnameignored ", &buf));
  ret = netwox_conffile_read_concat(&argfilename, &buf);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_buf_close(&buf));
    return(ret);
  }

  /* buf now contains the whole command line, so we create an
     argc/argv array and parse it */
  ret = netwox_cmdline_init(&buf, &filename, &argc, &argv);
  if (ret == NETWIB_ERR_OK) {
    ret = netwox_arg_update_argcargv(parg, argc, argv, NETWIB_FALSE);
    netwib_er(netwox_cmdline_close(&filename, &argv));
  }

  netwib_er(netwib_buf_close(&buf));
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_arg_update_kbd(netwox_arg *parg)
{
  netwox_arg_item *pitem=NULL;
  netwib_uint32 i;
  netwib_bool asktocontinue, somethingunset;
  netwib_buf message, allowedchars;
  netwib_char c, endkey;

  /* ask required args */
  asktocontinue = NETWIB_FALSE;
  somethingunset = NETWIB_FALSE;
  for (i = 0; i < parg->argsetnum; i++) {
    pitem = &parg->arg[i];
    if (pitem->required && !pitem->setbyuser) {
      asktocontinue = NETWIB_TRUE;
      netwib_er(netwox_toolargstore_set_kbd(pitem->ptoolarg,
                                            &pitem->toolargstore));
      pitem->setbyuser = NETWIB_TRUE;
    }
    if (!pitem->setbyuser) {
      somethingunset = NETWIB_TRUE;
    }
  }

  /* if everything is set, stop here */
  if (!somethingunset) {
    return(NETWIB_ERR_OK);
  }

  if (asktocontinue) {
    netwib_er(netwib_buf_init_ext_string("Do you want to set optional arguments",
                                       &message));
    netwib_er(netwib_buf_init_ext_string("ynYN", &allowedchars));
    netwib_er(netwib_char_init_kbd(&message, &allowedchars, 'n', &c));
    if (c == 'n' || c == 'N') {
      return(NETWIB_ERR_OK);
    }
  }

  /* ask optional arguments */
  endkey = '-';
  netwib_er(netwib_buf_init_mallocdefault(&message));
  netwib_er(netwib_buf_init_mallocdefault(&allowedchars));
  while (NETWIB_TRUE) {
    netwib__buf_reinit(&message);
    netwib__buf_reinit(&allowedchars);
    netwib_er(netwib_buf_append_string("List of available option:\n", &message));
    for (i = 0; i < parg->argsetnum; i++) {
      pitem = &parg->arg[i];
      netwib_er(netwib_buf_append_byte(pitem->key, &allowedchars));
      netwib_er(netwib_buf_append_fmt(&message, " %c : ", pitem->key));
      netwib_er(netwox_buf_append_toolarg(pitem->ptoolarg,
                                          NETWOX_TOOLARG_ENCODETYPE_KBD1,
                                          &message));
      if (pitem->ptoolarg->toolargtype == NETWOX_TOOLARGTYPE_BUF_PASSWORD) {
        netwib_er(netwib_buf_append_string("\n", &message));
      } else {
        if (pitem->setbyuser) {
          netwib_er(netwib_buf_append_string(" (set with ", &message));
          netwib_er(netwox_buf_append_toolargstore(&pitem->toolargstore,
                                                   &message));
          netwib_er(netwib_buf_append_string(")\n", &message));
        } else {
          netwib_er(netwib_buf_append_string(" (default is ", &message));
          netwib_er(netwox_toolarg_get_default(pitem->ptoolarg, &message));
          netwib_er(netwib_buf_append_string(")\n", &message));
        }
      }
    }
    netwib_er(netwib_buf_append_byte(endkey, &allowedchars));
    netwib_er(netwib_buf_append_fmt(&message, " %c : end\n", endkey));
    netwib_er(netwib_buf_append_string("Select argument key", &message));
    netwib_er(netwib_char_init_kbd(&message, &allowedchars, endkey, &c));
    if (c == endkey) {
      break;
    }
    pitem = &parg->arg[parg->keyindex[(netwib_uint32)c]];
    netwib_er(netwox_toolargstore_set_kbd(pitem->ptoolarg,
                                          &pitem->toolargstore));
    pitem->setbyuser = NETWIB_TRUE;
  }
  netwib_er(netwib_buf_close(&allowedchars));
  netwib_er(netwib_buf_close(&message));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_arg_update_argcargv_opt_long(netwox_arg *parg,
                                                      int argc,
                                                      char *argv[],
                                                      netwib_bool formupdate,
                                                      int *pi)
{
  netwib_char *pc, key;
  netwox_arg_item *pitem=NULL;
  netwib_bool isbool;

  /* "--help" */
  if (!formupdate) {
    if (!netwib_c_strcmp(argv[*pi], "--help") ||
        !netwib_c_strcmp(argv[*pi], "--?")) {
      netwib_er(netwox_tool_help_display(parg->ptoolinfo, currenttoolnum,
                                         NETWIB_FALSE, NETWIB_TRUE));
      return(NETWOX_ERR_TOOLARG_HELP);
    }
    if (!netwib_c_strcmp(argv[*pi], "--help2") ||
        !netwib_c_strcmp(argv[*pi], "--??")) {
      netwib_er(netwox_tool_help_display(parg->ptoolinfo, currenttoolnum,
                                         NETWIB_TRUE, NETWIB_TRUE));
      return(NETWOX_ERR_TOOLARG_HELP);
    }
  }

  /* "--argfile" */
  if (!formupdate) {
    if (!netwib_c_strcmp(argv[*pi], "--argfile")) {
      (*pi)++;
      if (*pi >= argc){
        netwib_er(netwib_fmt_display("Option '--argfile' must be followed by a filename\n"));
        return(NETWOX_ERR_TOOLARG_NOTDECODED);
      }
      netwib_er(netwox_arg_update_argfile(parg, argv[*pi]));
      return(NETWIB_ERR_OK);
    }
  }

  /* "--kbd" */
  if (!formupdate) {
    if (!netwib_c_strcmp(argv[*pi], "--kbd")) {
      netwib_er(netwox_arg_update_kbd(parg));
      return(NETWIB_ERR_OK);
    }
  }

  /* "--kbd-opt" */
  if (!formupdate) {
    if (netwib_c_strlen(argv[*pi]) > 6 &&
        !netwib_c_strncmp(argv[*pi], "--kbd-", 6)) {
      pc = argv[*pi] + 6;
      if (pc[1] == '\0') {
        key = pc[0];
        if (parg->keyindex[(netwib_uint32)key] == 0xFFFFFFFFu) {
          netwib_er(netwib_fmt_display("Option '%c' in '--kbd-%c' is not a valid option\n", key, key));
          return(NETWOX_ERR_TOOLARG_NOTDECODED);
        }
      } else {
        netwib_er(netwox_arg_key_init_name_string(parg, pc, &key));
      }
      pitem = &parg->arg[parg->keyindex[(netwib_uint32)key]];
      netwib_er(netwox_toolargstore_set_kbd(pitem->ptoolarg,
                                            &pitem->toolargstore));
      pitem->setbyuser = NETWIB_TRUE;
      return(NETWIB_ERR_OK);
    }
  }

  /* "--no-opt" */
  if (netwib_c_strlen(argv[*pi]) > 5 &&
      !netwib_c_strncmp(argv[*pi], "--no-", 5)) {
    /* opt must be boolean */
    pc = argv[*pi] + 5;
    netwib_er(netwox_arg_key_init_name_string(parg, pc, &key));
    netwib_er(netwox_arg_key_isbool(parg, key, &isbool));
    if (!isbool) {
      netwib_er(netwib_fmt_display("Option '%s' is not boolean\n", argv[*pi]));
      return(NETWOX_ERR_TOOLARG_NOTDECODED);
    }
    netwib_er(netwox_arg_add_bool(parg, key, NETWIB_FALSE));
    return(NETWIB_ERR_OK);
  }

  /* "--opt" */
  pc = argv[*pi] + 2;
  netwib_er(netwox_arg_key_init_name_string(parg, pc, &key));
  netwib_er(netwox_arg_key_isbool(parg, key, &isbool));
  if (isbool) {
    netwib_er(netwox_arg_add_bool(parg, key, NETWIB_TRUE));
    return(NETWIB_ERR_OK);
  }

  /* "--opt value" */
  (*pi)++;
  if (*pi >= argc){
    netwib_er(netwib_fmt_display("Option '%s' must be followed by a value\n",
                                 argv[*pi-1]));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }
  netwib_er(netwox_arg_add_string(parg, key, argv[*pi]));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_arg_update_argcargv(netwox_arg *parg,
                                             int argc,
                                             char *argv[],
                                             netwib_bool formupdate)
{
  netwib_uint32 moreargc;
  netwib_string pc;
  netwib_bool skiptomore;
  int i;

  /* first, analyze opt/req arguments */
  for (i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (argv[i][1] == '\0') {
        netwib_er(netwib_fmt_display("Option '-' is not recognized\n"));
        return(NETWOX_ERR_TOOLARG_NOTDECODED);
      } else if (argv[i][1] == '-') {
        if (argv[i][2] == '\0') {
          /* "--" skip to more */
          i++;
          break;
        }
        netwib_er(netwox_arg_update_argcargv_opt_long(parg, argc, argv,
                                                      formupdate, &i));
      } else {
        netwib_er(netwox_arg_update_argcargv_opt_short(parg, argc, argv, &i));
      }
    } else if (argv[i][0] == '+') {
      if (argv[i][1] == '\0') {
        netwib_er(netwib_fmt_display("Option '+' is not recognized\n"));
        return(NETWOX_ERR_TOOLARG_NOTDECODED);
      }
      netwib_er(netwox_arg_update_argcargv_opt_short_plus(parg, argv[i]));
    } else {
      netwib_er(netwox_arg_update_argcargv_req(parg, argv[i], &skiptomore));
      if (skiptomore) {
        /* if all required options are set, skip to more */
        break;
      }
    }
  }

  /* now, add more options */
  moreargc = argc - i;
  if (moreargc == 0) {
    return(NETWIB_ERR_OK);
  }
  if (!parg->allowmorearg) {
    netwib_er(netwib_fmt_display("There are too many options (%s ...)\n",
                                 argv[i]));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }
  if (parg->moreargvisallocated) {
    netwib_er(netwib_ptr_realloc((parg->moreargc+moreargc) * sizeof(netwib_char*),
                                 (netwib_ptr*)&parg->moreargv));
  } else {
    netwib_er(netwib_ptr_malloc(moreargc * sizeof(netwib_char*),
                             (netwib_ptr*)&parg->moreargv));
    parg->moreargvisallocated = NETWIB_TRUE;
  }
  /* by default, we do not need to moreargvitemsareallocated, but if an
     argfile was previously read, we have to continue allocating */
  while (i < argc) {
    if (parg->moreargvitemsareallocated) {
      netwib_er(netwib_ptr_malloc(netwib_c_strlen(argv[i])+1,
                                  (netwib_ptr*)&pc));
      netwib_c_strcpy(pc, argv[i]);
      parg->moreargv[parg->moreargc++] = pc;
    } else {
      parg->moreargv[parg->moreargc++] = argv[i];
    }
    i++;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_arg_init2(int argc,
                            char *argv[],
                            netwox_tool_info *ptoolinfo,
                            netwib_bool formupdate,
                            netwox_arg **pparg)
{
  const netwox_toolarg *ptoolarg;
  netwox_arg *parg;
  netwib_uint32 argtotalnum, i;
  netwib_err ret;

  /* first, count number of option (should be quite fast) */
  argtotalnum = 0;
  ptoolarg = ptoolinfo->arg;
  while (ptoolarg[argtotalnum].toolargclass != NETWOX_TOOLARGCLASS_END) {
    argtotalnum++;
  }

  /* allocate resources to store them */
  netwib_er(netwib_ptr_malloc(sizeof(netwox_arg), (netwib_ptr*)&parg));
  netwib_er(netwib_ptr_malloc(argtotalnum * sizeof(netwox_arg_item),
                              (netwib_ptr*)&parg->arg));

  /* initialize structure */
  parg->argtotalnum = argtotalnum;
  parg->argsetnum = 0;
  for (i = 0; i < 256; i++) parg->keyindex[i] = 0xFFFFFFFFu;
  parg->ptoolinfo = ptoolinfo;
  parg->allowmorearg = NETWIB_FALSE;
  parg->moreargc = 0;
  parg->moreargvisallocated = NETWIB_FALSE;
  parg->moreargvitemsareallocated = NETWIB_FALSE;

  /* parse opt info and store it in structure */
  ret = netwox_arg_update_toolarg(parg, ptoolinfo->arg);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_arg_close(&parg));
    return(ret);
  }
  /*netwib_er(netwox_arg_display(parg));*/

  /* parse argc/argv and store it in structure */
  ret = netwox_arg_update_argcargv(parg, argc, argv, formupdate);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwox_arg_close(&parg));
    return(ret);
  }
  /*netwib_er(netwox_arg_display(parg));*/

  if (!formupdate) {
    /* check if structure is ok (ie required opt are setbyuser) */
    ret = netwox_arg_check(parg);
    if (ret != NETWIB_ERR_OK) {
      netwib_er(netwox_arg_close(&parg));
      return(ret);
    }
    /*netwib_er(netwox_arg_display(parg));*/
  }

  /* set output value */
  *pparg = parg;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_arg_init(int argc,
                           char *argv[],
                           netwox_tool_info *ptoolinfo,
                           netwox_arg **pparg)
{
  return(netwox_arg_init2(argc, argv, ptoolinfo, NETWIB_FALSE, pparg));
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
netwib_err netwox_arg_close(netwox_arg **pparg)
{
  netwox_arg *parg;
  netwox_arg_item *pitem;
  netwib_string pc;
  netwib_uint32 i;

  parg = *pparg;

  for (i = 0; i < parg->argsetnum; i++) {
    pitem = &parg->arg[i];
    netwib_er(netwox_toolargstore_close(&pitem->toolargstore));
  }

  netwib_er(netwib_ptr_free((netwib_ptr*)&parg->arg));

  if (parg->allowmorearg) {
    if (parg->moreargvisallocated) {
      if (parg->moreargvitemsareallocated) {
        for (i = 0; i < parg->moreargc; i++) {
          pc = parg->moreargv[i];
          netwib_er(netwib_ptr_free((netwib_ptr*)&pc));
        }
      }
      netwib_er(netwib_ptr_free((netwib_ptr*)&parg->moreargv));
    }
  }

  netwib_er(netwib_ptr_free((netwib_ptr*)&parg));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_arg_isset(netwox_arg *parg,
                            netwib_char key,
                            netwib_bool *pisset)
{
  netwox_arg_item *pitem;

  if (parg->keyindex[(netwib_uint32)key] == 0xFFFFFFFFu) {
    netwib_er(netwib_fmt_display("Option '-%c' is not supported\n", key));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }
  pitem = &parg->arg[parg->keyindex[(netwib_uint32)key]];

  if (pisset != NULL) *pisset = pitem->setbyuser;

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
static netwib_err netwox_arg_get_prepare(netwox_arg *parg,
                                         netwib_char key,
                                         netwox_toolargstore **pptoolargstore)
{
  netwox_arg_item *pitem;
  netwib_buf buf;
  netwib_err ret;

  if (parg->keyindex[(netwib_uint32)key] == 0xFFFFFFFFu) {
    netwib_er(netwib_fmt_display("Option '-%c' is not supported\n", key));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }
  pitem = &parg->arg[parg->keyindex[(netwib_uint32)key]];

  if (!pitem->setbyuser && !pitem->setbydefault) {
    netwib_er(netwib_buf_init_mallocdefault(&buf));
    ret = netwox_toolarg_get_default(pitem->ptoolarg, &buf);
    if (ret == NETWIB_ERR_OK) {
      ret = netwox_toolargstore_set(&buf, &pitem->toolargstore);
    }
    if (ret != NETWIB_ERR_OK) {
      return(ret);
    }
    netwib_er(netwib_buf_close(&buf));
    pitem->setbydefault = NETWIB_TRUE;
  }

  if (pptoolargstore != NULL) *pptoolargstore = &pitem->toolargstore;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_arg_bool(netwox_arg *parg,
                           netwib_char key,
                           netwib_bool *pbool)
{
  netwox_toolargstore *ptoolargstore;

  netwib_er(netwox_arg_get_prepare(parg, key, &ptoolargstore));

  if (ptoolargstore->type != NETWOX_TOOLARGTYPE_BOOL &&
      ptoolargstore->type != NETWOX_TOOLARGTYPE_BOOL_RADIO1 &&
      ptoolargstore->type != NETWOX_TOOLARGTYPE_BOOL_RADIO2 &&
      ptoolargstore->type != NETWOX_TOOLARGTYPE_BOOL_RADIO3) {
    netwib_er(netwib_fmt_display("Option '-%c' is not of type BOOL\n", key));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  if (pbool != NULL) *pbool = ptoolargstore->value.b;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_arg_ip(netwox_arg *parg,
                         netwib_char key,
                         netwib_ip *pip)
{
  netwox_toolargstore *ptoolargstore;

  netwib_er(netwox_arg_get_prepare(parg, key, &ptoolargstore));

  if (ptoolargstore->type != NETWOX_TOOLARGTYPE_IP &&
      ptoolargstore->type != NETWOX_TOOLARGTYPE_IP4 &&
      ptoolargstore->type != NETWOX_TOOLARGTYPE_IP6) {
    netwib_er(netwib_fmt_display("Option '-%c' is not of type IP\n", key));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  if (pip != NULL) *pip = ptoolargstore->value.ip;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_arg_eth(netwox_arg *parg,
                          netwib_char key,
                          netwib_eth *peth)
{
  netwox_toolargstore *ptoolargstore;

  netwib_er(netwox_arg_get_prepare(parg, key, &ptoolargstore));

  if (ptoolargstore->type != NETWOX_TOOLARGTYPE_ETH) {
    netwib_er(netwib_fmt_display("Option '-%c' is not of type ETH\n", key));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  if (peth != NULL) *peth = ptoolargstore->value.eth;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_arg_port(netwox_arg *parg,
                           netwib_char key,
                           netwib_port *pport)
{
  netwox_toolargstore *ptoolargstore;

  netwib_er(netwox_arg_get_prepare(parg, key, &ptoolargstore));

  if (ptoolargstore->type != NETWOX_TOOLARGTYPE_PORT) {
    netwib_er(netwib_fmt_display("Option '-%c' is not of type PORT\n", key));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  if (pport != NULL) *pport = ptoolargstore->value.port;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_arg_uint64(netwox_arg *parg,
                             netwib_char key,
                             netwib_uint64 *puint64)
{
  netwox_toolargstore *ptoolargstore;

  netwib_er(netwox_arg_get_prepare(parg, key, &ptoolargstore));

  if (ptoolargstore->type != NETWOX_TOOLARGTYPE_UINT64) {
    netwib_er(netwib_fmt_display("Option '-%c' is not of type UINT64\n", key));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  if (puint64 != NULL) *puint64 = ptoolargstore->value.u64;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_arg_uint32(netwox_arg *parg,
                             netwib_char key,
                             netwib_uint32 *puint32)
{
  netwox_toolargstore *ptoolargstore;

  netwib_er(netwox_arg_get_prepare(parg, key, &ptoolargstore));

  if (ptoolargstore->type != NETWOX_TOOLARGTYPE_UINT32) {
    netwib_er(netwib_fmt_display("Option '-%c' is not of type UINT32\n", key));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  if (puint32 != NULL) *puint32 = ptoolargstore->value.u32;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_arg_uint16(netwox_arg *parg,
                             netwib_char key,
                             netwib_uint16 *puint16)
{
  netwox_toolargstore *ptoolargstore;

  netwib_er(netwox_arg_get_prepare(parg, key, &ptoolargstore));

  if (ptoolargstore->type != NETWOX_TOOLARGTYPE_UINT32) {
    netwib_er(netwib_fmt_display("Option '-%c' is not of type UINT32\n", key));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  if (puint16 != NULL) *puint16 = (netwib_uint16)ptoolargstore->value.u32;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_arg_uint8(netwox_arg *parg,
                            netwib_char key,
                            netwib_uint8 *puint8)
{
  netwox_toolargstore *ptoolargstore;

  netwib_er(netwox_arg_get_prepare(parg, key, &ptoolargstore));

  if (ptoolargstore->type != NETWOX_TOOLARGTYPE_UINT32) {
    netwib_er(netwib_fmt_display("Option '-%c' is not of type UINT32\n", key));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  if (puint8 != NULL) *puint8 = (netwib_uint8)ptoolargstore->value.u32;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_arg_int64(netwox_arg *parg,
                            netwib_char key,
                            netwib_int64 *pint64)
{
  netwox_toolargstore *ptoolargstore;

  netwib_er(netwox_arg_get_prepare(parg, key, &ptoolargstore));

  if (ptoolargstore->type != NETWOX_TOOLARGTYPE_INT64) {
    netwib_er(netwib_fmt_display("Option '-%c' is not of type INT64\n", key));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  if (pint64 != NULL) *pint64 = ptoolargstore->value.i64;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_arg_int32(netwox_arg *parg,
                            netwib_char key,
                            netwib_int32 *pint32)
{
  netwox_toolargstore *ptoolargstore;

  netwib_er(netwox_arg_get_prepare(parg, key, &ptoolargstore));

  if (ptoolargstore->type != NETWOX_TOOLARGTYPE_INT32) {
    netwib_er(netwib_fmt_display("Option '-%c' is not of type INT32\n", key));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  if (pint32 != NULL) *pint32 = ptoolargstore->value.i32;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_arg_encode(netwox_arg *parg,
                             netwib_char key,
                             netwib_encodetype *pencodetype)
{
  netwox_toolargstore *ptoolargstore;

  netwib_er(netwox_arg_get_prepare(parg, key, &ptoolargstore));

  if (ptoolargstore->type != NETWOX_TOOLARGTYPE_ENCODE) {
    netwib_er(netwib_fmt_display("Option '-%c' is not of type ENCODE\n", key));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  if (pencodetype != NULL) *pencodetype = ptoolargstore->value.encode;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_arg_recordencode(netwox_arg *parg,
                                   netwib_char key,
                                   netwib_record_encodetype *pencodetype)
{
  netwox_toolargstore *ptoolargstore;

  netwib_er(netwox_arg_get_prepare(parg, key, &ptoolargstore));

  if (ptoolargstore->type != NETWOX_TOOLARGTYPE_RECORDENCODE) {
    netwib_er(netwib_fmt_display("Option '-%c' is not of type RECORDENCODE\n",
                               key));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  if (pencodetype != NULL) *pencodetype = ptoolargstore->value.recordencode;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_arg_decode(netwox_arg *parg,
                             netwib_char key,
                             netwib_decodetype *pdecodetype)
{
  netwox_toolargstore *ptoolargstore;

  netwib_er(netwox_arg_get_prepare(parg, key, &ptoolargstore));

  if (ptoolargstore->type != NETWOX_TOOLARGTYPE_DECODE) {
    netwib_er(netwib_fmt_display("Option '-%c' is not of type DECODE\n", key));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  if (pdecodetype != NULL) *pdecodetype = ptoolargstore->value.decode;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_arg_dlt(netwox_arg *parg,
                          netwib_char key,
                          netwib_device_dlttype *pdlt)
{
  netwox_toolargstore *ptoolargstore;

  netwib_er(netwox_arg_get_prepare(parg, key, &ptoolargstore));

  if (ptoolargstore->type != NETWOX_TOOLARGTYPE_DLT) {
    netwib_er(netwib_fmt_display("Option '-%c' is not of type DLT\n", key));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  if (pdlt != NULL) *pdlt = ptoolargstore->value.dlt;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_arg_spoofip(netwox_arg *parg,
                              netwib_char key,
                              netwib_spoof_ip_inittype *ptype)
{
  netwox_toolargstore *ptoolargstore;

  netwib_er(netwox_arg_get_prepare(parg, key, &ptoolargstore));

  if (ptoolargstore->type != NETWOX_TOOLARGTYPE_SPOOFIP) {
    netwib_er(netwib_fmt_display("Option '-%c' is not of type SPOOFIP\n", key));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  if (ptype != NULL) *ptype = ptoolargstore->value.spoofip;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_arg_iptype(netwox_arg *parg,
                             netwib_char key,
                             netwib_iptype *ptype)
{
  netwox_toolargstore *ptoolargstore;

  netwib_er(netwox_arg_get_prepare(parg, key, &ptoolargstore));

  if (ptoolargstore->type != NETWOX_TOOLARGTYPE_IPTYPE) {
    netwib_er(netwib_fmt_display("Option '-%c' is not of type IPTYPE\n", key));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  if (ptype != NULL) *ptype = ptoolargstore->value.iptype;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwox_arg_ips(netwox_arg *parg,
                          netwib_char key,
                          netwib_ips **ppips)
{
  netwox_toolargstore *ptoolargstore;

  netwib_er(netwox_arg_get_prepare(parg, key, &ptoolargstore));

  if (ptoolargstore->type != NETWOX_TOOLARGTYPE_IPS_SU &&
      ptoolargstore->type != NETWOX_TOOLARGTYPE_IPS_U &&
      ptoolargstore->type != NETWOX_TOOLARGTYPE_IPS_N) {
    netwib_er(netwib_fmt_display("Option '-%c' is not of type IPS\n", key));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  if (ppips != NULL) *ppips = ptoolargstore->value.pips;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_arg_eths(netwox_arg *parg,
                           netwib_char key,
                           netwib_eths **ppeths)
{
  netwox_toolargstore *ptoolargstore;

  netwib_er(netwox_arg_get_prepare(parg, key, &ptoolargstore));

  if (ptoolargstore->type != NETWOX_TOOLARGTYPE_ETHS_SU &&
      ptoolargstore->type != NETWOX_TOOLARGTYPE_ETHS_U &&
      ptoolargstore->type != NETWOX_TOOLARGTYPE_ETHS_N) {
    netwib_er(netwib_fmt_display("Option '-%c' is not of type ETHS\n", key));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  if (ppeths != NULL) *ppeths = ptoolargstore->value.peths;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_arg_ports(netwox_arg *parg,
                            netwib_char key,
                            netwib_ports **ppports)
{
  netwox_toolargstore *ptoolargstore;

  netwib_er(netwox_arg_get_prepare(parg, key, &ptoolargstore));

  if (ptoolargstore->type != NETWOX_TOOLARGTYPE_PORTS_SU &&
      ptoolargstore->type != NETWOX_TOOLARGTYPE_PORTS_U &&
      ptoolargstore->type != NETWOX_TOOLARGTYPE_PORTS_N) {
    netwib_er(netwib_fmt_display("Option '-%c' is not of type PORTS\n", key));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  if (ppports != NULL) *ppports = ptoolargstore->value.pports;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_arg_buf(netwox_arg *parg,
                          netwib_char key,
                          netwib_buf *pbuf)
{
  netwox_toolargstore *ptoolargstore;

  netwib_er(netwox_arg_get_prepare(parg, key, &ptoolargstore));

  if (ptoolargstore->type != NETWOX_TOOLARGTYPE_BUF &&
      ptoolargstore->type != NETWOX_TOOLARGTYPE_BUF_MIXED) {
    netwib_er(netwib_fmt_display("Option '-%c' is not of type BUF\n", key));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  if (pbuf != NULL) *pbuf = ptoolargstore->value.buf;
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
netwib_err netwox_arg_more_count(netwox_arg *parg,
                                 netwib_uint32 *pcount)
{

  if (!parg->allowmorearg) {
    netwib_er(netwib_fmt_display("NETWOX_TOOLARGCLASS_MORE is not supported\n"));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  if (pcount != NULL) *pcount = parg->moreargc;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_arg_more(netwox_arg *parg,
                           netwib_uint32 i,
                           netwib_buf *pbuf)
{

  if (!parg->allowmorearg) {
    netwib_er(netwib_fmt_display("NETWOX_TOOLARGCLASS_MORE is not supported\n"));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  if (i+1 > parg->moreargc) {
    return(NETWIB_ERR_PATOOHIGH);
  }

  netwib_er(netwib_buf_append_string(parg->moreargv[i], pbuf));
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
static netwib_err netwox_arg_radio_error(netwox_arg *parg,
                                         netwib_uint32 numkeys,
                                         netwib_char keys[])
{
  netwox_arg_item *pitem;
  netwib_uint32 i, c;

  for (i = 0; i < numkeys; i++) {
    c = keys[i];
    pitem = &parg->arg[parg->keyindex[c]];
    netwib_er(netwib_fmt_display("  -%c|--%s\n", pitem->key, pitem->name));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_arg_radio(netwox_arg *parg,
                            netwib_uint32 radionum,
                            netwib_char *pc)
{
#define NETWOX_ARG_RADIO_MAXKEY 9
  netwib_char keys[NETWOX_ARG_RADIO_MAXKEY], keydefault, keyset;
  netwib_bool set, value, doublekeydefault, doublekeyset;
  netwib_uint32 i, numkeys;
  netwox_arg_item *pitem;
  netwox_toolargtype wantedtoolargtype=0;

  switch(radionum) {
  case 1 : wantedtoolargtype = NETWOX_TOOLARGTYPE_BOOL_RADIO1; break;
  case 2 : wantedtoolargtype = NETWOX_TOOLARGTYPE_BOOL_RADIO2; break;
  case 3 : wantedtoolargtype = NETWOX_TOOLARGTYPE_BOOL_RADIO3; break;
  default :
    netwib_er(netwib_fmt_display("Could not set wantedtoolargtype.\n"));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  /* set keys array (contains keys of this radio) */
  numkeys = 0;
  for (i = 0; i < parg->argsetnum; i++) {
    pitem = &parg->arg[i];
    if (pitem->ptoolarg->toolargtype == wantedtoolargtype) {
      if (numkeys >= NETWOX_ARG_RADIO_MAXKEY) {
        netwib_er(netwib_fmt_display("Too many radio keys.\n"));
        return(NETWOX_ERR_TOOLARG_NOTDECODED);
      }
      keys[numkeys++] = pitem->key;
    }
  }
  if (numkeys == 0) {
    netwib_er(netwib_fmt_display("This radio does not have any key.\n"));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  keydefault = ' ';
  keyset = ' ';
  doublekeydefault = NETWIB_FALSE;
  doublekeyset = NETWIB_FALSE;
  for (i = 0; i < numkeys; i++) {
    netwib_er(netwox_arg_isset(parg, keys[i], &set));
    netwib_er(netwox_arg_bool(parg, keys[i], &value));
    if (set) {
      if (value) {
        /* --key */
        if (keyset != ' ') {
          /* one key was already set */
          doublekeyset = NETWIB_TRUE;
          break;
        }
        keyset = keys[i];
      } else {
        /* --no-key, so ignore */
      }
    } else {
      if (value) {
        /* this key is a default */
        if (keydefault != ' ') {
          /* one key was already set */
          doublekeydefault = NETWIB_TRUE;
          break;
        }
        keydefault = keys[i];
      } else {
        /* not set, not default, so nothing to do */
      }
    }
  }

  if (keyset == ' ' && keydefault != ' ') {
    /* use default */
    keyset = keydefault;
  }

  /* treat errors */
  if (doublekeydefault) {
    netwib_er(netwib_fmt_display("This tool has several defaults set in radio.\n"));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }
  if (doublekeyset) {
    netwib_er(netwib_fmt_display("Only one of following parameters must be set:\n"));
    netwib_er(netwox_arg_radio_error(parg, numkeys, keys));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }
  if (keyset == ' ') {
    netwib_er(netwib_fmt_display("One of following parameters must be set:\n"));
    netwib_er(netwox_arg_radio_error(parg, numkeys, keys));
    return(NETWOX_ERR_TOOLARG_NOTDECODED);
  }

  *pc = keyset;
  return(NETWIB_ERR_OK);
}
