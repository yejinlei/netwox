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
#include "netwox.h"

/*-------------------------------------------------------------*/
netwib_err netwox_main(int argc, char *argv[])
{
  netwib_buf buf;
  netwib_uint32 toolnum, toolcount;
  netwib_err ret;

  if (argc == 1) {
    netwib_er(netwox_tooltree_helpmode());
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_buf_init_ext_string(argv[1], &buf));
  ret = netwib_buf_decode_fmt(&buf, "%{uint32}%$", &toolnum);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_fmt_display("The first parameter ('%s') must be a number\n", argv[1]));
    return(NETWOX_ERR_TOOL_NOTREGISTERED);
  }
  netwib_er(netwox_tools_count(&toolcount));
  if (toolnum > toolcount) {
    netwib_er(netwib_fmt_display("Tool number (%{uint32}) is superior to max number (%{uint32})\n", toolnum, toolcount));
    return(NETWOX_ERR_TOOL_NOTREGISTERED);
  }

  currenttoolnum = toolnum;
  netwib_er((netwox_tools[toolnum].pcore)(argc-1, argv+1));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
int main(int argc, char *argv[])
{
  netwib_err ret;

  netwib_init();
  ret = netwox_main(argc, argv);
  if (ret == NETWOX_ERR_TOOLARG_HELP) {
    /* user called help, so return no error */
    ret = NETWIB_ERR_OK;
  }
  if (ret == NETWOX_ERR_TOOLTREE_HELPMODE_END) {
    /* user called helpmode */
    ret = NETWIB_ERR_OK;
  }
  if (ret != NETWIB_ERR_OK && ret != NETWOX_ERR_TOOLARG_NOERRDISPLAY) {
    /* no error, or user does not want to display them */
    netwox_err_display(ret, NETWIB_ERR_ENCODETYPE_FULL);
  }
  netwib_close();

  if (ret != NETWIB_ERR_OK) {
    return(1);
  }
  return(0);
}

