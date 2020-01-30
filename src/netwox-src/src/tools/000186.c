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
netwib_conststring t000186_description[] = {
  "This tool sleeps for an amount of milliseconds.",
  "Total sleep duration is slightly greater than requested value.",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000186_args[] = {
  NETWOX_TOOLARG_REQ_UINT32('m', "millisecond", "number of millisecond",
                            "1000"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000186_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NOTNET,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000186_info = {
  "Millisecond sleep",
  t000186_description,
  NULL,
  t000186_args,
  t000186_nodes,
};

/*-------------------------------------------------------------*/
netwib_err t000186_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_uint32 ms;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000186_info, &parg));
  netwib_er(netwox_arg_uint32(parg, 'm', &ms));

  netwib_er(netwib_time_sleep_msec(ms));

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
