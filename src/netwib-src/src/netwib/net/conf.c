/*
                                  NETWIB
                             Network library
                Copyright(c) 1999-2012 Laurent CONSTANTIN
                      http://ntwib.sourceforge.net/
                        laurentconstantin@free.fr
                                  -----
  This file is part of Netwib.

  Netwib is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3
  as published by the Free Software Foundation.

  Netwib is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details (http://www.gnu.org/).

------------------------------------------------------------------------
*/

#include <netwib/inc/maininc.h>

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_conf(netwib_buf *pbuf)
{
  netwib_er(netwib_buf_append_string("################################### Devices ###################################\n", pbuf));
  netwib_er(netwib_buf_append_conf_devices(pbuf));
  netwib_er(netwib_buf_append_string("##################################### IP ######################################\n", pbuf));
  netwib_er(netwib_buf_append_conf_ip(pbuf));
  netwib_er(netwib_buf_append_string("############################## ArpCache/Neighbor #############################\n", pbuf));
  netwib_er(netwib_buf_append_conf_arpcache(pbuf));
  netwib_er(netwib_buf_append_string("#################################### Routes ###################################\n", pbuf));
  netwib_er(netwib_buf_append_conf_routes(pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_conf_display(void)
{
  netwib_err ret;
  netwib_string pc;
  netwib_buf buf;

  /* on error, we choose to display partial configuration, because
     this helps finding origin of bug */
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  ret = netwib_buf_append_conf(&buf);
  netwib_er(netwib_buf_ref_string(&buf, &pc));
  fprintf(stdout, "%s", pc);
  fflush(stdout);
  netwib_er(netwib_buf_close(&buf));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_conf_debug(netwib_buf *pbuf)
{
  netwib_priv_confwork cw;
  netwib_err ret;

  netwib_er(netwib_priv_confwork_init(&cw));
  ret = netwib_priv_confwork_obtain(&cw, NETWIB_TRUE, pbuf);
  netwib_er(netwib_priv_confwork_close(&cw));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_conf_debug_display(void)
{
  netwib_err ret;
  netwib_string pc;
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  ret = netwib_buf_append_conf_debug(&buf);
  if (ret != NETWIB_ERR_OK) {
    netwib_buf_append_string("\nError received : ", &buf);
    netwib_buf_append_err(ret, NETWIB_ERR_ENCODETYPE_FULL, &buf);
  }
  netwib_er(netwib_buf_ref_string(&buf, &pc));
  fprintf(stdout, "%s", pc);
  fflush(stdout);
  netwib_er(netwib_buf_close(&buf));

  return(NETWIB_ERR_OK);
}
