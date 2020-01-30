
/*-------------------------------------------------------------*/
/* Create rules based on chains. Each chain has one or more rules.
   If a rule is matched, target action is done.

   For example :
     chain_1 :
       |
       +---| if match |---> ACCEPT, REJECT, RETURN or JUMP
       |
       +---| if match |---> ACCEPT, REJECT, RETURN or JUMP
       |
       `---> ACCEPT, REJECT, RETURN or JUMP
            (default target for chain 1)

   Real example :
     chain_1 :
       +---| if rule_match_pf |---> ACCEPT
       +---| if rule_match_pf |---> REJECT
       +---| if rule_match_pf |---> JUMP to chain 2
       `---> ACCEPT
     chain_2 :
       +---| if rule_match_pf |---> REJECT
       +---| if rule_match_pf |---> JUMP 3
       `---> JUMP 4
     chain_3 :
       +---| if rule_match_pf |---> RETURN (to previous chain)
       `---> JUMP 4
     chain_4 :
       +---| if rule_match_pf |---> ACCEPT
       `---> REJECT
     default : ACCEPT

   Notes :
    - chain numbers should be low : 0, 1, 2, etc. (instead of 3285)
    - there is no loop detection in netwox_rules_match
*/

/*-------------------------------------------------------------*/
typedef netwib_uint32 netwox_rules_chainnumber;

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_RULES_TARGETTYPE_ACCEPT = 1,
  NETWOX_RULES_TARGETTYPE_REJECT,
  NETWOX_RULES_TARGETTYPE_RETURN, /* to previous chain */
  NETWOX_RULES_TARGETTYPE_JUMP
} netwox_rules_targettype;

/*-------------------------------------------------------------*/
typedef struct {
  netwox_rules_targettype targettype;
  netwox_rules_chainnumber jumptochain; /* only for JUMP */
} netwox_rules_target;

/*-------------------------------------------------------------*/
typedef struct netwox_rules netwox_rules;

/*-------------------------------------------------------------*/
/* Check one rule using user info and rule specific info. Parameter
   pmatch is set if this rule matches. */
typedef netwib_err (*netwox_rules_rule_match_pf)(netwib_ptr ruleinfos,
                                                 netwib_ptr userinfos,
                                                 netwib_bool *pmatch);
/* Erase rule information */
typedef netwib_err (*netwox_rules_rule_erase_pf)(netwib_ptr ruleinfos);
/* Synthetic display of rule information (in debug mode) */
typedef netwib_err (*netwox_rules_rule_display_pf)(netwib_ptr ruleinfos);

/*-------------------------------------------------------------*/
netwib_err netwox_rules_init(netwox_rules_rule_match_pf pfuncmatch,
                             netwox_rules_rule_erase_pf pfuncerase,
                             netwox_rules_rule_display_pf pfuncdisplay,
                             const netwox_rules_target *pdefaulttarget,
                             netwib_bool debug,
                             netwox_rules **pprules);
netwib_err netwox_rules_close(netwox_rules **pprules);

/*-------------------------------------------------------------*/
netwib_err netwox_rules_chain_create(netwox_rules *prules,
                                     netwox_rules_chainnumber chainnumber,
                                    const netwox_rules_target *pdefaulttarget);
netwib_err netwox_rules_chain_rule_add(netwox_rules *prules,
                                       netwox_rules_chainnumber chainnumber,
                                       netwib_ptr ruleinfos,
                                       const netwox_rules_target *ptarget);

/*-------------------------------------------------------------*/
/* change default targets */
netwib_err netwox_rules_defaulttarget(netwox_rules *prules,
                                    const netwox_rules_target *pdefaulttarget);
netwib_err netwox_rules_chain_defaulttarget(netwox_rules *prules,
                                    netwox_rules_chainnumber chainnumber,
                                    const netwox_rules_target *pdefaulttarget);

/*-------------------------------------------------------------*/
netwib_err netwox_rules_match(netwox_rules *prules,
                              netwox_rules_chainnumber startingchainnumber,
                              netwib_ptr userinfos,
                              netwib_bool *pmatch);

/*-------------------------------------------------------------*/
netwib_err netwox_rules_target_display(netwox_rules *prules,
                                       netwox_rules_target *ptarget);
netwib_err netwox_rules_display(netwox_rules *prules);

