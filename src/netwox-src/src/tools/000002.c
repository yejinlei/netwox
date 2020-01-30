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
netwib_conststring t000002_description[] = {
  "This tool displays internal information about netwox:",
  " - values for defines",
  " - how network configuration is retrieved",
  "Please, send this output when reporting a problem.",
  NETWOX_DESC_toolpriv_conf,
  NULL
};
netwox_toolarg t000002_args[] = {
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000002_nodes[] = {
  NETWOX_TOOLTREENODETYPE_INFO_NETW,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000002_info = {
  "Display debugging information",
  t000002_description,
  NULL,
  t000002_args,
  t000002_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000002_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_uint32 versionmajor, versionminor, versionmicro;
  netwib_err ret;

  netwib_er(netwox_arg_init(argc, argv, &t000002_info, &parg));
  netwib_er(netwox_arg_close(&parg));

  netwib_er(netwib_fmt_display("Netwox toolbox version %{uint32}.%{uint32}.%{uint32}.\n", NETWOXDEF_VERSIONMAJOR, NETWOXDEF_VERSIONMINOR, NETWOXDEF_VERSIONMICRO));
  netwib_er(netwib_internal_version(&versionmajor, &versionminor,&versionmicro));
  netwib_er(netwib_fmt_display("Netwib library version %{uint32}.%{uint32}.%{uint32}.\n", versionmajor, versionminor, versionmicro));

  netwib_er(netwib_fmt_display("\n\n####****####****####****####****####\n"));
  ret = netwib_internal_display();
  netwib_err_display(ret, NETWIB_ERR_ENCODETYPE_FULL);

  netwib_er(netwib_fmt_display("\n\n####****####****####****####****####\n"));
  ret = netwib_conf_display();
  netwib_err_display(ret, NETWIB_ERR_ENCODETYPE_FULL);

  netwib_er(netwib_fmt_display("\n\n####****####****####****####****####\n"));
  ret = netwib_conf_debug_display();
  netwib_err_display(ret, NETWIB_ERR_ENCODETYPE_FULL);

  netwib_er(netwib_fmt_display("\n\n####****####****####****####****####\n"));
  netwib_er(netwib_fmt_display("END\n"));

  return(NETWIB_ERR_OK);
}
