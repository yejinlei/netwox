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
typedef struct {
  netwib_ptr ruleinfos;
  netwox_rules_target target;
} netwox_rules_chain_rule;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_rules_target defaulttarget;
  netwox_rules_chain_rule *rules;
  netwib_uint32 rulesallocsize;
  netwib_uint32 rulessetsize;
} netwox_rules_chain;

/*-------------------------------------------------------------*/
struct netwox_rules {
  netwox_rules_rule_match_pf pfuncmatch;
  netwox_rules_rule_erase_pf pfuncerase;
  netwox_rules_rule_display_pf pfuncdisplay;
  netwox_rules_target defaulttarget;
  netwib_bool debug;
  netwox_rules_chain *chains;
  netwib_bool *chainsinitialized;
  netwib_uint32 chainsallocsize;
};

/*-------------------------------------------------------------*/
#define NETWOX_RULES_ALLOC_STEP 10

/*-------------------------------------------------------------*/
netwib_err netwox_rules_init(netwox_rules_rule_match_pf pfuncmatch,
                             netwox_rules_rule_erase_pf pfuncerase,
                             netwox_rules_rule_display_pf pfuncdisplay,
                             const netwox_rules_target *pdefaulttarget,
                             netwib_bool debug,
                             netwox_rules **pprules)
{
  netwox_rules *prules;
  netwib_uint32 i, chainsallocsize;

  if (pdefaulttarget->targettype == NETWOX_RULES_TARGETTYPE_RETURN) {
    netwib_er(netwib_fmt_display("Target RETURN is not valid for main default\n"));
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  /* allocate needed memory to store prules */
  netwib_er(netwib_ptr_malloc(sizeof(netwox_rules), (netwib_ptr*)&prules));
  *pprules = prules;

  /* set items */
  prules->pfuncmatch = pfuncmatch;
  prules->pfuncerase = pfuncerase;
  prules->pfuncdisplay = pfuncdisplay;
  prules->defaulttarget = *pdefaulttarget;
  prules->debug = debug;

  /* allocate chains */
  chainsallocsize = NETWOX_RULES_ALLOC_STEP;
  prules->chainsallocsize = chainsallocsize;
  netwib_er(netwib_ptr_malloc(chainsallocsize*sizeof(netwox_rules_chain),
                              (netwib_ptr*)&(prules->chains)));
  netwib_er(netwib_ptr_malloc(chainsallocsize*sizeof(netwib_bool),
                              (netwib_ptr*)&(prules->chainsinitialized)));

  /* set rules to uninitialized */
  for (i = 0; i < chainsallocsize; i++) {
    prules->chainsinitialized[i] = NETWIB_FALSE;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_rules_close(netwox_rules **pprules)
{
  netwox_rules *prules;
  netwox_rules_chain *pchain;
  netwib_uint32 i, j;

  prules = *pprules;

  /* free rules in chains */
  for (i = 0; i < prules->chainsallocsize; i++) {
    if (prules->chainsinitialized[i]) {
      pchain = &prules->chains[i];
      if (prules->pfuncerase != NULL) {
        for (j = 0; j < pchain->rulessetsize; j++) {
          netwib_er((*prules->pfuncerase)(pchain->rules[j].ruleinfos));
        }
      }
      netwib_er(netwib_ptr_free((netwib_ptr*)&(pchain->rules)));
    }
  }

  /* free chains */
  netwib_er(netwib_ptr_free((netwib_ptr*)&(prules->chains)));
  netwib_er(netwib_ptr_free((netwib_ptr*)&(prules->chainsinitialized)));

  /* free prules */
  netwib_er(netwib_ptr_free((netwib_ptr*)&prules));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_rules_chain_create(netwox_rules *prules,
                                     netwox_rules_chainnumber chainnumber,
                                     const netwox_rules_target *pdefaulttarget)
{
  netwib_uint32 chainsallocsize, i;

  /* eventually alloc if number is in a zone not yet initialized */
  if (chainnumber >= prules->chainsallocsize) {
    chainsallocsize = chainnumber + NETWOX_RULES_ALLOC_STEP;
    netwib_er(netwib_ptr_realloc(chainsallocsize*sizeof(netwox_rules_chain),
                                 (netwib_ptr*)&(prules->chains)));
    netwib_er(netwib_ptr_realloc(chainsallocsize*sizeof(netwib_bool),
                                 (netwib_ptr*)&(prules->chainsinitialized)));
    for (i = prules->chainsallocsize; i < chainsallocsize; i++) {
      prules->chainsinitialized[i] = NETWIB_FALSE;
    }
    prules->chainsallocsize = chainsallocsize;
  } else {
    /* check if not already initialized */
    if (prules->chainsinitialized[chainnumber]) {
      netwib_er(netwib_fmt_display("Chain %{uint32} already created\n",
                                   chainnumber));
      return(NETWIB_ERR_PAINVALIDPOS);
    }
  }

  /* create chain */
  prules->chainsinitialized[chainnumber] = NETWIB_TRUE;
  prules->chains[chainnumber].defaulttarget = *pdefaulttarget;
  netwib_er(netwib_ptr_malloc(NETWOX_RULES_ALLOC_STEP*sizeof(netwox_rules_chain_rule), (netwib_ptr*)&(prules->chains[chainnumber].rules)));
  prules->chains[chainnumber].rulesallocsize = NETWOX_RULES_ALLOC_STEP;
  prules->chains[chainnumber].rulessetsize = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_rules_chain_rule_add(netwox_rules *prules,
                                       netwox_rules_chainnumber chainnumber,
                                       netwib_ptr ruleinfos,
                                       const netwox_rules_target *ptarget)
{
  netwox_rules_chain *pchain;
  netwox_rules_chain_rule *prule;

  /* check if chains exists */
  if (!prules->chainsinitialized[chainnumber]) {
    netwib_er(netwib_fmt_display("Chain %{uint32} wasn't created\n",
                                 chainnumber));
    return(NETWIB_ERR_PAINVALIDPOS);
  }
  pchain = &prules->chains[chainnumber];

  /* eventually alloc if rule is not yet initialized */
  if (pchain->rulessetsize == pchain->rulesallocsize) {
    pchain->rulesallocsize += NETWOX_RULES_ALLOC_STEP;
    netwib_er(netwib_ptr_realloc(pchain->rulesallocsize*sizeof(netwox_rules_chain_rule), (netwib_ptr*)&(pchain->rules)));
  }

  /* append rule */
  prule = &pchain->rules[pchain->rulessetsize];
  prule->ruleinfos = ruleinfos;
  prule->target = *ptarget;
  pchain->rulessetsize++;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_rules_defaulttarget(netwox_rules *prules,
                                    const netwox_rules_target *pdefaulttarget)
{
  prules->defaulttarget = *pdefaulttarget;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_rules_chain_defaulttarget(netwox_rules *prules,
                                    netwox_rules_chainnumber chainnumber,
                                    const netwox_rules_target *pdefaulttarget)
{
  if (!prules->chainsinitialized[chainnumber]) {
    netwib_er(netwib_fmt_display("Chain %{uint32} wasn't initialized\n",
                                 chainnumber));
    return(NETWIB_ERR_PAINVALIDPOS);
  }

  prules->chains[chainnumber].defaulttarget = *pdefaulttarget;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_rules_match(netwox_rules *prules,
                                          netwox_rules_chainnumber chainnumber,
                                          netwib_ptr userinfos,
                                          netwox_rules_targettype *ptargettype)
{
  netwib_uint32 j;
  netwox_rules_chain *pchain;
  netwox_rules_chain_rule *prule;
  netwib_bool match;

  /* check if chains exists */
  if (!prules->chainsinitialized[chainnumber]) {
    netwib_er(netwib_fmt_display("Chain %{uint32} wasn't initialized\n",
                                 chainnumber));
    return(NETWIB_ERR_PAINVALIDPOS);
  }
  if (prules->debug) {
    netwib_er(netwib_fmt_display("Entering chain %{uint32}\n", chainnumber));
  }
  pchain = &prules->chains[chainnumber];

#define netwox__priv_rules_match_leave(n) if (prules->debug) {netwib_er(netwib_fmt_display("Leaving chain %{uint32}\n", n));}

  /* use rules */
  for (j = 0; j < pchain->rulessetsize; j++) {
    prule = &pchain->rules[j];
    if (prules->debug) {
      netwib_er(netwib_fmt_display("  Rule %{uint32} : ", j));
      if (prules->pfuncdisplay != NULL) {
        netwib_er((*prules->pfuncdisplay)(prule->ruleinfos));
      } else {
        netwib_er(netwib_fmt_display("%p", prule->ruleinfos));
      }
      netwib_er(netwib_fmt_display(" : "));
      netwib_er(netwox_rules_target_display(prules, &prule->target));
      netwib_er(netwib_fmt_display("\n"));
    }
    netwib_er((*prules->pfuncmatch)(prule->ruleinfos, userinfos, &match));
    if (prules->debug) {
      if (match) {
        netwib_er(netwib_fmt_display("    Matched\n"));
      } else {
        netwib_er(netwib_fmt_display("    Not matched\n"));
      }
    }
    if (match) {
      if (prule->target.targettype != NETWOX_RULES_TARGETTYPE_JUMP) {
        *ptargettype = prule->target.targettype;
        netwox__priv_rules_match_leave(chainnumber);
        return(NETWIB_ERR_OK);
      }
      netwib_er(netwox_priv_rules_match(prules, prule->target.jumptochain,
                                        userinfos, ptargettype));
      if (prule->target.targettype != NETWOX_RULES_TARGETTYPE_RETURN) {
        netwox__priv_rules_match_leave(chainnumber);
        return(NETWIB_ERR_OK);
      }
    }
  }

  /* use default */
  if (prules->debug) {
    netwib_er(netwib_fmt_display("  Default target : "));
    netwib_er(netwox_rules_target_display(prules, &pchain->defaulttarget));
    netwib_er(netwib_fmt_display("\n    Matched\n"));
  }
  if (pchain->defaulttarget.targettype != NETWOX_RULES_TARGETTYPE_JUMP) {
    *ptargettype = pchain->defaulttarget.targettype;
    netwox__priv_rules_match_leave(chainnumber);
    return(NETWIB_ERR_OK);
  }
  netwib_er(netwox_priv_rules_match(prules, pchain->defaulttarget.jumptochain,
                                    userinfos, ptargettype));
  netwox__priv_rules_match_leave(chainnumber);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_rules_match(netwox_rules *prules,
                              netwox_rules_chainnumber startingchainnumber,
                              netwib_ptr userinfos,
                              netwib_bool *pmatch)
{
  netwox_rules_targettype targettype;
  netwib_bool match;

  /* check if chains exists */
  if (!prules->chainsinitialized[startingchainnumber]) {
    netwib_er(netwib_fmt_display("Chain %{uint32} wasn't initialized\n",
                                 startingchainnumber));
    return(NETWIB_ERR_PAINVALIDPOS);
  }

  if (prules->debug) {
    netwib_er(netwib_fmt_display("Starting match at chain %{uint32}\n",
                                 startingchainnumber));
  }

  /* loop to find solution */
  match = NETWIB_FALSE;
  while(NETWIB_TRUE) {
    /* use rules */
    netwib_er(netwox_priv_rules_match(prules, startingchainnumber, userinfos,
                                      &targettype));
    if (targettype == NETWOX_RULES_TARGETTYPE_ACCEPT) {
      match = NETWIB_TRUE;
      break;
    } else if (targettype == NETWOX_RULES_TARGETTYPE_REJECT) {
      match = NETWIB_FALSE;
      break;
    } else if (targettype != NETWOX_RULES_TARGETTYPE_RETURN) {
      return(NETWOX_ERR_INTERNALERROR);
    }
    /* use main default */
    if (prules->debug) {
      netwib_er(netwib_fmt_display("Using main default : "));
      netwib_er(netwox_rules_target_display(prules, &prules->defaulttarget));
      netwib_er(netwib_fmt_display("\n"));
    }
    if (prules->defaulttarget.targettype == NETWOX_RULES_TARGETTYPE_ACCEPT) {
      match = NETWIB_TRUE;
      break;
    } else if (prules->defaulttarget.targettype
               == NETWOX_RULES_TARGETTYPE_REJECT) {
      match = NETWIB_FALSE;
      break;
    } else if (prules->defaulttarget.targettype
               != NETWOX_RULES_TARGETTYPE_JUMP) {
      return(NETWOX_ERR_INTERNALERROR);
    }
    /* main default can be jump */
    startingchainnumber = prules->defaulttarget.jumptochain;
  }

  if (pmatch != NULL) *pmatch = match;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_rules_target_display(netwox_rules *prules,
                                       netwox_rules_target *ptarget)
{
  switch(ptarget->targettype) {
  case NETWOX_RULES_TARGETTYPE_ACCEPT :
    netwib_er(netwib_fmt_display("ACCEPT"));
    break;
  case NETWOX_RULES_TARGETTYPE_REJECT :
    netwib_er(netwib_fmt_display("REJECT"));
    break;
  case NETWOX_RULES_TARGETTYPE_RETURN :
    netwib_er(netwib_fmt_display("RETURN"));
    break;
  case NETWOX_RULES_TARGETTYPE_JUMP :
    netwib_er(netwib_fmt_display("JUMP to %{uint32}", ptarget->jumptochain));
    if (!prules->chainsinitialized[ptarget->jumptochain]) {
      netwib_er(netwib_fmt_display("[error]"));
    }
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_rules_display(netwox_rules *prules)
{
  netwib_uint32 i, j;
  netwox_rules_chain *pchain;
  netwox_rules_chain_rule *prule;

  for (i = 0; i < prules->chainsallocsize; i++) {
    if (prules->chainsinitialized[i]) {
      pchain = &prules->chains[i];
      netwib_er(netwib_fmt_display("Chain %{uint32} :\n", i));
      for (j = 0; j < pchain->rulessetsize; j++) {
        prule = &pchain->rules[j];
        netwib_er(netwib_fmt_display("  Rule %{uint32} : ", j));
        if (prules->pfuncdisplay != NULL) {
          netwib_er((*prules->pfuncdisplay)(prule->ruleinfos));
        } else {
          netwib_er(netwib_fmt_display("%p", prule->ruleinfos));
        }
        netwib_er(netwib_fmt_display(" : "));
        netwib_er(netwox_rules_target_display(prules, &prule->target));
        netwib_er(netwib_fmt_display("\n"));
      }
      netwib_er(netwib_fmt_display("  Default target : "));
      netwib_er(netwox_rules_target_display(prules, &pchain->defaulttarget));
      netwib_er(netwib_fmt_display("\n"));
    } else {
      if (prules->debug) {
        netwib_er(netwib_fmt_display("Chain %{uint32} uninitialized\n", i));
      }
    }
  }

  netwib_er(netwib_fmt_display("Default target : "));
  netwib_er(netwox_rules_target_display(prules, &prules->defaulttarget));
  netwib_er(netwib_fmt_display("\n"));

  return(NETWIB_ERR_OK);
}
