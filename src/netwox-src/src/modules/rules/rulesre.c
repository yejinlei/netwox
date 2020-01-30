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
netwib_err netwox_rulesre_ruleinfos_create(netwib_constbuf *pregexp,
                                           netwib_ptr *pruleinfos)
{
  netwib_er(netwox_bufstore_create(pregexp, pruleinfos));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_rulesre_ruleinfos_match(netwib_ptr ruleinfos,
                                          netwib_ptr userinfos,
                                          netwib_bool *pmatch)
{
  netwox_bufstore *pbufstore = (netwox_bufstore *)ruleinfos;
  netwib_buf buf, *puserinfos;
  netwib_err ret;

  netwib_er(netwib_buf_init_ext_arrayfilled(pbufstore->data,
                                            pbufstore->datasize, &buf));
  puserinfos = (netwib_buf *)userinfos;

  ret = netwib_buf_search_re(puserinfos, &buf, NETWIB_TRUE, NULL);
  if (ret == NETWIB_ERR_OK) {
    *pmatch = NETWIB_TRUE;
  } else {
    *pmatch = NETWIB_FALSE;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_rulesre_ruleinfos_erase(netwib_ptr ruleinfos)
{
  netwib_er(netwox_bufstore_erase(ruleinfos));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_rulesre_ruleinfos_display(netwib_ptr ruleinfos)
{
  netwox_bufstore *pbufstore = (netwox_bufstore *)ruleinfos;
  netwib_buf buf;

  netwib_er(netwib_buf_init_ext_arrayfilled(pbufstore->data,
                                            pbufstore->datasize, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
static netwib_err netwox_rulesre_example_test(netwox_rules *prules,
                                              netwox_rules_chainnumber startingchainnumber,
                                              netwib_conststring usertext,
                                              netwib_bool wantedmatch)
{
  netwib_buf userbuf;
  netwib_bool match;

  netwib_er(netwib_fmt_display("\nWanted result for '%s' starting at %{uint32} : %{bool:t}\n", usertext, startingchainnumber, wantedmatch));
  netwib_er(netwib_buf_init_ext_string(usertext, &userbuf));
  netwib_er(netwox_rules_match(prules, startingchainnumber, &userbuf, &match));

  if (match != wantedmatch) {
    return(NETWOX_ERR_PROTOCOL);
  }

  return(NETWIB_ERR_OK);
}
netwib_err netwox_rulesre_example(void)
{
  netwox_rules *prules;
  netwib_ptr ruleinfos;
  netwox_rules_target defaulttarget, target;
  netwib_buf buf;

  /* init */
  defaulttarget.targettype = NETWOX_RULES_TARGETTYPE_ACCEPT;
  netwib_er(netwox_rules_init(&netwox_rulesre_ruleinfos_match,
                              &netwox_rulesre_ruleinfos_erase,
                              &netwox_rulesre_ruleinfos_display,
                              &defaulttarget, NETWIB_TRUE, &prules));

  /* create chain */
  target.targettype = NETWOX_RULES_TARGETTYPE_JUMP;
  target.jumptochain = 2;
  netwib_er(netwox_rules_chain_create(prules, 1, &target));

  /* create chain */
  target.targettype = NETWOX_RULES_TARGETTYPE_REJECT;
  netwib_er(netwox_rules_chain_create(prules, 2, &target));

  /* add rule */
  target.targettype = NETWOX_RULES_TARGETTYPE_ACCEPT;
  netwib_er(netwib_buf_init_ext_string("z", &buf));
  netwib_er(netwox_rulesre_ruleinfos_create(&buf, &ruleinfos));
  netwib_er(netwox_rules_chain_rule_add(prules, 1, ruleinfos, &target));

  /* add rule */
  target.targettype = NETWOX_RULES_TARGETTYPE_REJECT;
  netwib_er(netwib_buf_init_ext_string("y", &buf));
  netwib_er(netwox_rulesre_ruleinfos_create(&buf, &ruleinfos));
  netwib_er(netwox_rules_chain_rule_add(prules, 1, ruleinfos, &target));

  /* add rule */
  target.targettype = NETWOX_RULES_TARGETTYPE_JUMP;
  target.jumptochain = 2;
  netwib_er(netwib_buf_init_ext_string("a", &buf));
  netwib_er(netwox_rulesre_ruleinfos_create(&buf, &ruleinfos));
  netwib_er(netwox_rules_chain_rule_add(prules, 1, ruleinfos, &target));

  /* add rule */
  target.targettype = NETWOX_RULES_TARGETTYPE_JUMP;
  target.jumptochain = 3;
  netwib_er(netwib_buf_init_ext_string("b", &buf));
  netwib_er(netwox_rulesre_ruleinfos_create(&buf, &ruleinfos));
  netwib_er(netwox_rules_chain_rule_add(prules, 1, ruleinfos, &target));

  /* create chain */
  target.targettype = NETWOX_RULES_TARGETTYPE_RETURN;
  netwib_er(netwox_rules_chain_create(prules, 3, &target));

  /* add rule */
  target.targettype = NETWOX_RULES_TARGETTYPE_REJECT;
  netwib_er(netwib_buf_init_ext_string("t", &buf));
  netwib_er(netwox_rulesre_ruleinfos_create(&buf, &ruleinfos));
  netwib_er(netwox_rules_chain_rule_add(prules, 3, ruleinfos, &target));

  /* add rule */
  target.targettype = NETWOX_RULES_TARGETTYPE_RETURN;
  netwib_er(netwib_buf_init_ext_string("x", &buf));
  netwib_er(netwox_rulesre_ruleinfos_create(&buf, &ruleinfos));
  netwib_er(netwox_rules_chain_rule_add(prules, 2, ruleinfos, &target));

  /* show */
  netwib_er(netwox_rules_display(prules));

  /* test */
  netwib_er(netwox_rulesre_example_test(prules, 1, "z", NETWIB_TRUE));
  netwib_er(netwox_rulesre_example_test(prules, 1, "qse", NETWIB_FALSE));
  netwib_er(netwox_rulesre_example_test(prules, 1, "y", NETWIB_FALSE));
  netwib_er(netwox_rulesre_example_test(prules, 1, "a", NETWIB_FALSE));
  netwib_er(netwox_rulesre_example_test(prules, 1, "ax", NETWIB_TRUE));
  netwib_er(netwox_rulesre_example_test(prules, 1, "b", NETWIB_TRUE));
  netwib_er(netwox_rulesre_example_test(prules, 1, "bt", NETWIB_FALSE));
  netwib_er(netwox_rulesre_example_test(prules, 3, "t", NETWIB_FALSE));
  netwib_er(netwox_rulesre_example_test(prules, 3, "u", NETWIB_TRUE));

  /* close */
  netwib_er(netwox_rules_close(&prules));

  /*******/

  /* init */
  defaulttarget.targettype = NETWOX_RULES_TARGETTYPE_JUMP;
  defaulttarget.jumptochain = 2;
  netwib_er(netwox_rules_init(&netwox_rulesre_ruleinfos_match,
                              &netwox_rulesre_ruleinfos_erase,
                              &netwox_rulesre_ruleinfos_display,
                              &defaulttarget, NETWIB_TRUE, &prules));

  /* create chain */
  target.targettype = NETWOX_RULES_TARGETTYPE_ACCEPT;
  netwib_er(netwox_rules_chain_create(prules, 1, &target));

  /* create chain */
  target.targettype = NETWOX_RULES_TARGETTYPE_REJECT;
  netwib_er(netwox_rules_chain_create(prules, 2, &target));

  /* add rule */
  target.targettype = NETWOX_RULES_TARGETTYPE_RETURN;
  netwib_er(netwib_buf_init_ext_string("z", &buf));
  netwib_er(netwox_rulesre_ruleinfos_create(&buf, &ruleinfos));
  netwib_er(netwox_rules_chain_rule_add(prules, 1, ruleinfos, &target));

  /* show */
  netwib_er(netwox_rules_display(prules));

  /* test */
  netwib_er(netwox_rulesre_example_test(prules, 1, "z", NETWIB_FALSE));
  netwib_er(netwox_rulesre_example_test(prules, 1, "a", NETWIB_TRUE));

  /* close */
  netwib_er(netwox_rules_close(&prules));


  return(NETWIB_ERR_OK);
}
