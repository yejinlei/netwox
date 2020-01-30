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
netwib_err netwox_rulesfile_state_init(netwox_rulesfile_state *pstate)
{
  pstate->defaulttargetset = NETWIB_FALSE;
  pstate->chainnumberset = NETWIB_FALSE;
  pstate->chaindefaulttargetset = NETWIB_FALSE;
  pstate->ruletargetset = NETWIB_FALSE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_rulesfile_state_close(netwox_rulesfile_state *pstate)
{
  pstate->defaulttargetset = NETWIB_FALSE;
  pstate->chainnumberset = NETWIB_FALSE;
  pstate->chaindefaulttargetset = NETWIB_FALSE;
  pstate->ruletargetset = NETWIB_FALSE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_RULESFILE_VAR_DEFAULTTARGET = 1,
  NETWOX_RULESFILE_VAR_CHAINNUMBER,
  NETWOX_RULESFILE_VAR_CHAINDEFAULTTARGET,
  NETWOX_RULESFILE_VAR_RULETARGET,
  NETWOX_RULESFILE_VAR_RULE
} netwox_rulesfile_var;

/*-------------------------------------------------------------*/
static netwib_err netwox_rulesfile_state_var(netwib_constbuf *pvar,
                                             netwox_rulesfile_var *pvarset)
{
  netwib_string str;

  netwib__constbuf_ref_string(pvar, str, bufstorage,
                              netwox_rulesfile_state_var(&bufstorage, pvarset));

  if (!netwib_c_strcmp(str, NETWOX_RULESFILE_VARNAME_DEFAULTTARGET)) {
    *pvarset = NETWOX_RULESFILE_VAR_DEFAULTTARGET;
  } else if (!netwib_c_strcmp(str, NETWOX_RULESFILE_VARNAME_CHAINNUMBER)) {
    *pvarset = NETWOX_RULESFILE_VAR_CHAINNUMBER;
  } else if (!netwib_c_strcmp(str,
                              NETWOX_RULESFILE_VARNAME_CHAINDEFAULTTARGET)) {
    *pvarset = NETWOX_RULESFILE_VAR_CHAINDEFAULTTARGET;
  } else if (!netwib_c_strcmp(str, NETWOX_RULESFILE_VARNAME_RULETARGET)) {
    *pvarset = NETWOX_RULESFILE_VAR_RULETARGET;
  } else if (!netwib_c_strcmp(str, NETWOX_RULESFILE_VARNAME_RULE)) {
    *pvarset = NETWOX_RULESFILE_VAR_RULE;
  } else {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_rulesfile_state_update_target(netwib_constbuf *pval,
                                                  netwox_rules_target *ptarget)
{
  netwib_string str;

  netwib__constbuf_ref_string(pval, str, bufstorage,
                              netwox_rulesfile_state_update_target(&bufstorage, ptarget));

  if (!netwib_c_strcasecmp(str, "accept")) {
    ptarget->targettype = NETWOX_RULES_TARGETTYPE_ACCEPT;
  } else if (!netwib_c_strcasecmp(str, "reject")) {
    ptarget->targettype = NETWOX_RULES_TARGETTYPE_REJECT;
  } else if (!netwib_c_strcasecmp(str, "return")) {
    ptarget->targettype = NETWOX_RULES_TARGETTYPE_RETURN;
  } else if (!netwib_c_strncasecmp(str, "jump", 4)) {
    netwib_er(netwib_buf_decode_fmt(pval,
                                    "%{*;c}%{*;c}%{*;c}%{*;c} %{uint32}%$",
                                    &ptarget->jumptochain));
    ptarget->targettype = NETWOX_RULES_TARGETTYPE_JUMP;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_rulesfile_state_update(netwox_rulesfile_state *pstate,
                                                netwox_rulesfile_var var,
                                                netwib_constbuf *pval)
{
  switch(var) {
  case NETWOX_RULESFILE_VAR_DEFAULTTARGET :
    netwib_er(netwox_rulesfile_state_update_target(pval,
                                                   &pstate->defaulttarget));
    pstate->defaulttargetset = NETWIB_TRUE;
    break;
  case NETWOX_RULESFILE_VAR_CHAINNUMBER :
    netwib_er(netwib_buf_decode_fmt(pval, "%{uint32}%$",
                                    &pstate->chainnumber));
    pstate->chainnumberset = NETWIB_TRUE;
    break;
  case NETWOX_RULESFILE_VAR_CHAINDEFAULTTARGET :
    netwib_er(netwox_rulesfile_state_update_target(pval,
                                                 &pstate->chaindefaulttarget));
    pstate->chaindefaulttargetset = NETWIB_TRUE;
    break;
  case NETWOX_RULESFILE_VAR_RULETARGET :
    netwib_er(netwox_rulesfile_state_update_target(pval,
                                                   &pstate->ruletarget));
    pstate->ruletargetset = NETWIB_TRUE;
    break;
  case NETWOX_RULESFILE_VAR_RULE :
    /* nothing to do here */
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_rulesfile_state_addvar(netwox_rules *prules,
                                         netwox_rulesfile_state *pstate,
                                         netwib_constbuf *pvar,
                                         netwib_constbuf *pval)
{
  netwox_rulesfile_var var;
  netwib_ptr ruleinfos;

  /* obtain variable */
  netwib_er(netwox_rulesfile_state_var(pvar, &var));

  /* check if every needed thing is set */
  switch(var) {
  case NETWOX_RULESFILE_VAR_DEFAULTTARGET :
    if (pstate->defaulttargetset) {
      netwib_er(netwib_fmt_display("Error : variable %s is set twice\n",
                                   NETWOX_RULESFILE_VARNAME_DEFAULTTARGET));
      return(NETWOX_ERR_PROTOCOL);
    }
    break;
  case NETWOX_RULESFILE_VAR_CHAINNUMBER :
    if (!pstate->defaulttargetset) {
      netwib_er(netwib_fmt_display("Error : variable %s is not set\n",
                                   NETWOX_RULESFILE_VARNAME_DEFAULTTARGET));
      return(NETWOX_ERR_PROTOCOL);
    }
    pstate->chaindefaulttargetset = NETWIB_FALSE;
    pstate->ruletargetset = NETWIB_FALSE;
    break;
  case NETWOX_RULESFILE_VAR_CHAINDEFAULTTARGET :
    if (!pstate->defaulttargetset) {
      netwib_er(netwib_fmt_display("Error : variable %s is not set\n",
                                   NETWOX_RULESFILE_VARNAME_DEFAULTTARGET));
      return(NETWOX_ERR_PROTOCOL);
    }
    if (!pstate->chainnumberset) {
      netwib_er(netwib_fmt_display("Error : variable %s is not set\n",
                                   NETWOX_RULESFILE_VARNAME_CHAINNUMBER));
      return(NETWOX_ERR_PROTOCOL);
    }
    break;
  case NETWOX_RULESFILE_VAR_RULETARGET :
    if (!pstate->defaulttargetset) {
      netwib_er(netwib_fmt_display("Error : variable %s is not set\n",
                                   NETWOX_RULESFILE_VARNAME_DEFAULTTARGET));
      return(NETWOX_ERR_PROTOCOL);
    }
    if (!pstate->chainnumberset) {
      netwib_er(netwib_fmt_display("Error : variable %s is not set\n",
                                   NETWOX_RULESFILE_VARNAME_CHAINNUMBER));
      return(NETWOX_ERR_PROTOCOL);
    }
    break;
  case NETWOX_RULESFILE_VAR_RULE :
    if (!pstate->defaulttargetset) {
      netwib_er(netwib_fmt_display("Error : variable %s is not set\n",
                                   NETWOX_RULESFILE_VARNAME_DEFAULTTARGET));
      return(NETWOX_ERR_PROTOCOL);
    }
    if (!pstate->chainnumberset) {
      netwib_er(netwib_fmt_display("Error : variable %s is not set\n",
                                   NETWOX_RULESFILE_VARNAME_CHAINNUMBER));
      return(NETWOX_ERR_PROTOCOL);
    }
    if (!pstate->ruletargetset) {
      netwib_er(netwib_fmt_display("Error : variable %s is not set\n",
                                   NETWOX_RULESFILE_VARNAME_RULETARGET));
      return(NETWOX_ERR_PROTOCOL);
    }
    break;
  }

  /* update state */
  netwib_er(netwox_rulesfile_state_update(pstate, var, pval));

  /* add chains or rules */
  switch(var) {
  case NETWOX_RULESFILE_VAR_DEFAULTTARGET :
    netwib_er(netwox_rules_defaulttarget(prules, &pstate->defaulttarget));
    break;
  case NETWOX_RULESFILE_VAR_CHAINNUMBER :
    /* need more info before adding */
    break;
  case NETWOX_RULESFILE_VAR_CHAINDEFAULTTARGET :
    netwib_er(netwox_rules_chain_create(prules, pstate->chainnumber,
                                        &pstate->chaindefaulttarget));
    break;
  case NETWOX_RULESFILE_VAR_RULETARGET :
    /* need more info before adding */
    break;
  case NETWOX_RULESFILE_VAR_RULE :
    netwib_er(netwox_rulesre_ruleinfos_create(pval, &ruleinfos));
    netwib_er(netwox_rules_chain_rule_add(prules, pstate->chainnumber,
                                          ruleinfos,
                                          &pstate->ruletarget));
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_rulesfile_example_text(netwox_rules *prules,
                                                netwox_rulesfile_state *pstate,
                                                netwib_conststring var,
                                                netwib_conststring val)
{
  netwib_buf bufvar, bufval;

  netwib_er(netwib_buf_init_ext_string(var, &bufvar));
  netwib_er(netwib_buf_init_ext_string(val, &bufval));

  netwib_er(netwox_rulesfile_state_addvar(prules, pstate, &bufvar, &bufval));

  return(NETWIB_ERR_OK);
}
netwib_err netwox_rulesfile_example(void)
{
  netwox_rules *prules;
  netwox_rules_target defaulttarget;
  netwox_rulesfile_state state;

  /* init */
  defaulttarget.targettype = NETWOX_RULES_TARGETTYPE_ACCEPT;
  netwib_er(netwox_rules_init(&netwox_rulesre_ruleinfos_match,
                              &netwox_rulesre_ruleinfos_erase,
                              &netwox_rulesre_ruleinfos_display,
                              &defaulttarget, NETWIB_FALSE, &prules));
  netwib_er(netwox_rulesfile_state_init(&state));

  /* update */
  netwib_er(netwox_rulesfile_example_text(prules, &state, NETWOX_RULESFILE_VARNAME_DEFAULTTARGET, "accept"));
  netwib_er(netwox_rulesfile_example_text(prules, &state, NETWOX_RULESFILE_VARNAME_CHAINNUMBER, "1"));
  netwib_er(netwox_rulesfile_example_text(prules, &state, NETWOX_RULESFILE_VARNAME_CHAINDEFAULTTARGET, "return"));
  netwib_er(netwox_rulesfile_example_text(prules, &state, NETWOX_RULESFILE_VARNAME_CHAINNUMBER, "2"));
  netwib_er(netwox_rulesfile_example_text(prules, &state, NETWOX_RULESFILE_VARNAME_CHAINDEFAULTTARGET, "jump 1"));
  netwib_er(netwox_rulesfile_example_text(prules, &state, NETWOX_RULESFILE_VARNAME_CHAINNUMBER, "3"));
  netwib_er(netwox_rulesfile_example_text(prules, &state, NETWOX_RULESFILE_VARNAME_CHAINDEFAULTTARGET, "reject"));
  netwib_er(netwox_rulesfile_example_text(prules, &state, NETWOX_RULESFILE_VARNAME_RULETARGET, "reject"));
  netwib_er(netwox_rulesfile_example_text(prules, &state, NETWOX_RULESFILE_VARNAME_RULE, "a"));
  netwib_er(netwox_rulesfile_example_text(prules, &state, NETWOX_RULESFILE_VARNAME_RULE, "b"));
  netwib_er(netwox_rulesfile_example_text(prules, &state, NETWOX_RULESFILE_VARNAME_RULETARGET, "accept"));
  netwib_er(netwox_rulesfile_example_text(prules, &state, NETWOX_RULESFILE_VARNAME_RULE, "c"));

  /* show */
  netwib_er(netwox_rules_display(prules));

  /* close */
  netwib_er(netwox_rulesfile_state_close(&state));
  netwib_er(netwox_rules_close(&prules));

  return(NETWIB_ERR_OK);
}
