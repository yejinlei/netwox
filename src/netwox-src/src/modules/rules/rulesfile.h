
/*-------------------------------------------------------------*/
/* Rules can be expressed in a linear way. For example :
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
   can be written :
     defaulttarget = "accept"
     chainnumber = 1
     chaindefaulttarget = ACCEPT
     ruletarget = accept
     rule = "match data"
     ruletarget = reject
     rule = "match data2"
     ruletarget = "jump 2"
     rule = "match data3"
     chainnumber = 2
     chaindefaulttarget = "jump 4"
     ruletarget = reject
     rule = "match data hello"
     ruletarget = "jump 3"
     rule = "match data hello2"
     etc.

   Please note order is important. All fields are required. The only
   exception is for ruletarget which can be omitted if several rules
   in the same chain have the same target :
     ruletarget = reject
     rule = "match"
     [ruletarget = reject] (not needed)
     rule = "match2"

   Functions in this file analyze those variables and initialize
   a netwox_rules.
*/

/*-------------------------------------------------------------*/
#define NETWOX_RULESFILE_VARNAME_DEFAULTTARGET "defaulttarget"
#define NETWOX_RULESFILE_VARNAME_CHAINNUMBER "chainnumber"
#define NETWOX_RULESFILE_VARNAME_CHAINDEFAULTTARGET "chaindefaulttarget"
#define NETWOX_RULESFILE_VARNAME_RULETARGET "ruletarget"
#define NETWOX_RULESFILE_VARNAME_RULE "rule"

/*-------------------------------------------------------------*/
typedef struct {
  netwox_rules_target defaulttarget;
  netwib_bool defaulttargetset;
  netwox_rules_chainnumber chainnumber;
  netwib_bool chainnumberset;
  netwox_rules_target chaindefaulttarget;
  netwib_bool chaindefaulttargetset;
  netwox_rules_target ruletarget;
  netwib_bool ruletargetset;
} netwox_rulesfile_state;

/*-------------------------------------------------------------*/
netwib_err netwox_rulesfile_state_init(netwox_rulesfile_state *pstate);
netwib_err netwox_rulesfile_state_close(netwox_rulesfile_state *pstate);

/*-------------------------------------------------------------*/
/* add chains and rules */
netwib_err netwox_rulesfile_state_addvar(netwox_rules *prules,
                                         netwox_rulesfile_state *pstate,
                                         netwib_constbuf *pvar,
                                         netwib_constbuf *pval);

/*-------------------------------------------------------------*/
/* just a fake example to test and show how to use rules */
netwib_err netwox_rulesfile_example(void);
