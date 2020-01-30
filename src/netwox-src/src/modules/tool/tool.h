
/*-------------------------------------------------------------*/
typedef netwib_err (*netwox_tool_core_pf)(int argc, char *argv[]);

/*-------------------------------------------------------------*/
typedef struct {
  netwib_conststring title;
  const netwib_conststring *description; /* arrays ends with NULL. Each
                                            line will be terminated by a \n */
  const netwib_conststring synonyms;
  const netwox_toolarg *arg; /* array ends with NETWOX_TOOLARG_END */
  const netwox_tooltreenodetype *treenode; /*end NETWOX_TOOLTREENODETYPE_END */
} netwox_tool_info;

/*-------------------------------------------------------------*/
netwib_err netwox_tool_help(netwox_tool_info *pinfo,
                            netwib_uint32 toolnum,
                            netwib_bool showadvanced,
                            netwib_bool showspecial,
                            netwib_buf *pbuf);
netwib_err netwox_tool_help_display(netwox_tool_info *pinfo,
                                    netwib_uint32 toolnum,
                                    netwib_bool showadvanced,
                                    netwib_bool showspecial);

/*-------------------------------------------------------------*/
typedef struct {
  netwox_tool_info *pinfo;
  netwox_tool_core_pf pcore;
} netwox_tool;

/*-------------------------------------------------------------*/
extern netwox_tool netwox_tools[];
extern netwib_uint32 currenttoolnum;

/*-------------------------------------------------------------*/
netwib_err netwox_tools_count(netwib_uint32 *pcount);
