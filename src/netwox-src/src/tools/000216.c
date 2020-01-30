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
netwib_conststring t000216_description[] = {
  "This tool beeps/bips.",
  "",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000216_args[] = {
  NETWOX_TOOLARG_OPT_BOOL('a', "alarm", "use several beeps", NULL),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000216_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NOTNET,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000216_info = {
  "Beep",
  t000216_description,
  NULL,
  t000216_args,
  t000216_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000216_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_bool a;
  netwib_uint32 sleeptime;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000216_info, &parg));
  netwib_er(netwox_arg_bool(parg, 'a', &a));

  netwib_er(netwib_beep());
  if (a) {
    for (sleeptime = 800; sleeptime != 0; sleeptime -= 200) {
      netwib_er(netwib_time_sleep_msec(sleeptime));
      netwib_er(netwib_beep());
    }
  }

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
