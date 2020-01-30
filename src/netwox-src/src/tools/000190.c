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
netwib_conststring t000190_description[] = {
  "This tool makes coffees requested by user.",
  "",
  "In case you don't understand: it's a joke/fun tool. Many users told",
  "me: 'Netwox is cool, but it does not make coffee'. Now, it does ;-)",
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000190_args[] = {
  NETWOX_TOOLARG_OPT_UINT32('n', "numcups", "number of cups", "1"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000190_nodes[] = {
  NETWOX_TOOLTREENODETYPE_NOTNET,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000190_info = {
  "Make coffee",
  t000190_description,
  NULL,
  t000190_args,
  t000190_nodes,
};

/*-------------------------------------------------------------*/
static netwib_err t000190_cup(void)
{
  netwib_buf buf;
  netwib_uint32 i;

  netwib_er(netwib_fmt_display("Work in progress..."));
  for (i = 0; i < 30; i++) {
    netwib_er(netwib_time_sleep_msec(100));
    netwib_er(netwib_fmt_display("."));
  }
  netwib_er(netwib_fmt_display("\nHere is your cup:\n"));

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_append_string("\n", &buf));
  netwib_er(netwib_buf_append_string("                             )    )    )\n", &buf));
  netwib_er(netwib_buf_append_string("                            (    (    (\n", &buf));
  netwib_er(netwib_buf_append_string("                         ____)____)____)____\n", &buf));
  netwib_er(netwib_buf_append_string("                        |                   |______\n", &buf));
  netwib_er(netwib_buf_append_string("                        |                   |-----.)\n", &buf));
  netwib_er(netwib_buf_append_string("                        |                   |    //\n", &buf));
  netwib_er(netwib_buf_append_string("                        |                   |___//\n", &buf));
  netwib_er(netwib_buf_append_string("                        |                   |---'\n", &buf));
  netwib_er(netwib_buf_append_string("                        |                   |\n", &buf));
  netwib_er(netwib_buf_append_string("                        `-.---------------.-'\n", &buf));
  netwib_er(netwib_buf_append_string("                          |_______________|\n", &buf));
  netwib_er(netwib_buf_append_string("\n", &buf));

  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err t000190_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_uint32 numcups, i;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000190_info, &parg));
  netwib_er(netwox_arg_uint32(parg, 'n', &numcups));

  /* main loop */
  for (i = 0; i < numcups; i++) {
    netwib_er(t000190_cup());
  }

  /* close */
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
