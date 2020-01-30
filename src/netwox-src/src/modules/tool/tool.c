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
netwib_err netwox_tool_help(netwox_tool_info *pinfo,
                            netwib_uint32 toolnum,
                            netwib_bool showadvanced,
                            netwib_bool showspecial,
                            netwib_buf *pbuf)
{
  const netwox_toolarg *ptoolarg;
  netwib_uint32 countsimple, countadvanced, countreq, countmore;
  const netwib_conststring *pdesc;

  countsimple = 0;
  countadvanced = 0;
  countreq = 0;
  countmore = 0;
  ptoolarg = pinfo->arg;
  while (ptoolarg->toolargclass != NETWOX_TOOLARGCLASS_END) {
    if (ptoolarg->toolargclass == NETWOX_TOOLARGCLASS_REQ) {
      countreq++;
    }
    if (ptoolarg->advanced) {
      countadvanced++;
    } else {
      countsimple++;
    }
    if (ptoolarg->toolargclass == NETWOX_TOOLARGCLASS_MORE) {
      countmore++;
    }
    ptoolarg++;
  }

  netwib_er(netwib_buf_append_fmt(pbuf, "Title: %s\n", pinfo->title));
  if (showadvanced) {
    netwib_er(netwib_buf_append_string("+------------------------------------------------------------------------+\n", pbuf));
    pdesc = pinfo->description;
    while (*pdesc != NULL) {
      netwib_er(netwib_buf_append_fmt(pbuf, "| %{l 70;s} |\n", *pdesc));
      pdesc++;
    }
    netwib_er(netwib_buf_append_string("+------------------------------------------------------------------------+\n", pbuf));
    if (pinfo->synonyms != NULL) {
      netwib_er(netwib_buf_append_fmt(pbuf, "Synonyms: %s\n",
                                      pinfo->synonyms));
    }
  }

  netwib_er(netwib_buf_append_fmt(pbuf, "Usage: netwox %{uint32}", toolnum));
  ptoolarg = pinfo->arg;
  while (ptoolarg->toolargclass != NETWOX_TOOLARGCLASS_END) {
    if (!ptoolarg->advanced &&
        ptoolarg->toolargclass != NETWOX_TOOLARGCLASS_MORE) {
      netwib_er(netwox_buf_append_toolarg(ptoolarg,
                                          NETWOX_TOOLARG_ENCODETYPE_HELPUSAGE,
                                          pbuf));
    }
    ptoolarg++;
  }
  if (showadvanced && countadvanced) {
    ptoolarg = pinfo->arg;
    while (ptoolarg->toolargclass != NETWOX_TOOLARGCLASS_END) {
      if (ptoolarg->advanced) {
        netwib_er(netwox_buf_append_toolarg(ptoolarg,
                                          NETWOX_TOOLARG_ENCODETYPE_HELPUSAGE,
                                            pbuf));
      }
      ptoolarg++;
    }
  }
  if (countmore) {
    ptoolarg = pinfo->arg;
    while (ptoolarg->toolargclass != NETWOX_TOOLARGCLASS_END) {
      if (ptoolarg->toolargclass == NETWOX_TOOLARGCLASS_MORE) {
        netwib_er(netwox_buf_append_toolarg(ptoolarg,
                                          NETWOX_TOOLARG_ENCODETYPE_HELPUSAGE,
                                            pbuf));
      }
      ptoolarg++;
    }
  }
  netwib_er(netwib_buf_append_byte('\n', pbuf));

  if (countsimple + countadvanced == 0) {
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_buf_append_string("Parameters:\n", pbuf));

  ptoolarg = pinfo->arg;
  while (ptoolarg->toolargclass != NETWOX_TOOLARGCLASS_END) {
    if (!ptoolarg->advanced &&
        ptoolarg->toolargclass != NETWOX_TOOLARGCLASS_MORE) {
      netwib_er(netwox_buf_append_toolarg(ptoolarg,
                                          NETWOX_TOOLARG_ENCODETYPE_HELPLONG,
                                          pbuf));
      netwib_er(netwib_buf_append_byte('\n', pbuf));
    }
    ptoolarg++;
  }
  if (showadvanced && countadvanced) {
    ptoolarg = pinfo->arg;
    while (ptoolarg->toolargclass != NETWOX_TOOLARGCLASS_END) {
      if (ptoolarg->advanced) {
        netwib_er(netwox_buf_append_toolarg(ptoolarg,
                                            NETWOX_TOOLARG_ENCODETYPE_HELPLONG,
                                            pbuf));
        netwib_er(netwib_buf_append_byte('\n', pbuf));
      }
      ptoolarg++;
    }
  }
  if (countmore) {
    ptoolarg = pinfo->arg;
    while (ptoolarg->toolargclass != NETWOX_TOOLARGCLASS_END) {
      if (ptoolarg->toolargclass == NETWOX_TOOLARGCLASS_MORE) {
        netwib_er(netwox_buf_append_toolarg(ptoolarg,
                                            NETWOX_TOOLARG_ENCODETYPE_HELPLONG,
                                            pbuf));
        netwib_er(netwib_buf_append_byte('\n', pbuf));
      }
      ptoolarg++;
    }
  }

  if (showspecial) {
    if (showadvanced) {
      netwib_er(netwib_buf_append_string(" --help                         display simple help\n", pbuf));
      netwib_er(netwib_buf_append_string(" --kbd                          ask missing parameters from keyboard\n", pbuf));
      netwib_er(netwib_buf_append_string(" --kbd-k or --kbd-name          ask parameter -k|--name from keyboard\n", pbuf));
      netwib_er(netwib_buf_append_string(" --argfile file                 ask missing parameters from file\n", pbuf));
    } else {
      if (countadvanced) {
        netwib_er(netwib_buf_append_string(" --help2                        display help for advanced parameters\n", pbuf));
      } else {
        netwib_er(netwib_buf_append_string(" --help2                        display full help\n", pbuf));
      }
    }
  }

  if (countreq) {
    netwib_er(netwib_buf_append_fmt(pbuf, "Example: netwox %{uint32}",
                                    toolnum));
    ptoolarg = pinfo->arg;
    while (ptoolarg->toolargclass != NETWOX_TOOLARGCLASS_END) {
      netwib_er(netwox_buf_append_toolarg(ptoolarg,
                                        NETWOX_TOOLARG_ENCODETYPE_HELPEXAMPLE1,
                                          pbuf));
      ptoolarg++;
    }
    netwib_er(netwib_buf_append_byte('\n', pbuf));
    netwib_er(netwib_buf_append_fmt(pbuf, "Example: netwox %{uint32}",
                                    toolnum));
    ptoolarg = pinfo->arg;
    while (ptoolarg->toolargclass != NETWOX_TOOLARGCLASS_END) {
      netwib_er(netwox_buf_append_toolarg(ptoolarg,
                                        NETWOX_TOOLARG_ENCODETYPE_HELPEXAMPLE2,
                                          pbuf));
      ptoolarg++;
    }
    netwib_er(netwib_buf_append_byte('\n', pbuf));
  } else {
    netwib_er(netwib_buf_append_fmt(pbuf, "Example: netwox %{uint32}\n",
                                    toolnum));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_tool_help_display(netwox_tool_info *pinfo,
                                    netwib_uint32 toolnum,
                                    netwib_bool showadvanced,
                                    netwib_bool showspecial)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwox_tool_help(pinfo, toolnum, showadvanced, showspecial, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwox_tool netwox_tools[] =
{
#define NETWOX_TOOL_ADD(pinfo,pcore) {pinfo,pcore},
  #include "../../tools.list"
  {NULL, NULL} /* indicates end */
};

/*-------------------------------------------------------------*/
netwib_uint32 currenttoolnum;

/*-------------------------------------------------------------*/
static netwib_uint32 netwox_tools_num = 0xFFFFFFFFu;
/* tool 0 is special, so it is not counted */
netwib_err netwox_tools_count(netwib_uint32 *pcount)
{
  netwib_uint32 i;

  if (netwox_tools_num == 0xFFFFFFFFu) {
    i = 1;
    while (netwox_tools[i].pcore != NULL) i++;
    netwox_tools_num = i - 1;
  }

  if (pcount != NULL) *pcount = netwox_tools_num;
  return(NETWIB_ERR_OK);
}
