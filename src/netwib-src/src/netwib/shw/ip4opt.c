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
static netwib_err netwib_ip4opt_show_end(netwib_encodetype encodetype,
                                         netwib_buf *pbuf)
{
  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
      netwib_er(netwib_buf_append_string("end", pbuf));
      break;
    case NETWIB_ENCODETYPE_ARRAY :
      netwib_er(netwib_show_array_fmt32(pbuf, " end"));
      break;
    default :
      return(NETWIB_ERR_LOINTERNALERROR);
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ip4opt_show_noop(netwib_encodetype encodetype,
                                          netwib_buf *pbuf)
{
  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
      netwib_er(netwib_buf_append_string("noop", pbuf));
      break;
    case NETWIB_ENCODETYPE_ARRAY :
      netwib_er(netwib_show_array_fmt32(pbuf, " noop"));
      break;
    default :
      return(NETWIB_ERR_LOINTERNALERROR);
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ip4opt_show_rr(netwib_constip4opt_rr *pip4opt,
                                        netwib_encodetype encodetype,
                                        netwib_buf *pbuf)
{
  netwib_uint32 i;

  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
      netwib_er(netwib_buf_append_string("rr", pbuf));
      break;
    case NETWIB_ENCODETYPE_ARRAY :
      netwib_er(netwib_show_array_fmt32(pbuf, " rr (storagesize=%{uint32} storedvalues=%{uint32}) :", pip4opt->storagesize, pip4opt->storedvalues));
      for (i = 0; i < pip4opt->storedvalues; i++) {
        netwib_er(netwib_show_array_fmt32(pbuf, "   %{ip}", &pip4opt->ip[i]));
      }
      break;
    default :
      return(NETWIB_ERR_LOINTERNALERROR);
      break;
  }

  pip4opt = pip4opt; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ip4opt_show_srr(netwib_constip4opt_srr *pip4opt,
                                         netwib_bool islsrr,
                                         netwib_encodetype encodetype,
                                         netwib_buf *pbuf)
{
  netwib_uint32 i;

  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
      netwib_er(netwib_buf_append_string(islsrr?"lsrr":"ssrr", pbuf));
      break;
    case NETWIB_ENCODETYPE_ARRAY :
      netwib_er(netwib_show_array_fmt32(pbuf, " %s (storagesize=%{uint32} usedvalues=%{uint32}) :", islsrr?"lsrr":"ssrr", pip4opt->storagesize, pip4opt->usedvalues));
      for (i = 0; i < pip4opt->storagesize; i++) {
        netwib_er(netwib_show_array_fmt32(pbuf, "   %{ip}", &pip4opt->ip[i]));
      }
      break;
    default :
      return(NETWIB_ERR_LOINTERNALERROR);
      break;
  }

  pip4opt = pip4opt; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_ip4opt_show_time(netwib_constip4opt_time *pip4opt,
                                          netwib_encodetype encodetype,
                                          netwib_buf *pbuf)
{
  netwib_uint32 i;

  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
      netwib_er(netwib_buf_append_string("time", pbuf));
      break;
    case NETWIB_ENCODETYPE_ARRAY :
      netwib_er(netwib_show_array_fmt32(pbuf, " time (storagesize=%{uint32} storedvalues=%{uint32} flag=%{uint32} oflw=%{uint32}) :", pip4opt->storagesize, pip4opt->storedvalues, pip4opt->flag, pip4opt->overflow));
      if (pip4opt->flag == NETWIB_IP4OPT_TIMEFLAG_TS) {
        for (i = 0; i < pip4opt->storedvalues; i++) {
          netwib_er(netwib_show_array_fmt32(pbuf, "   %{uint32}",
                                            pip4opt->timestamp[i]));
        }
      } else if (pip4opt->flag == NETWIB_IP4OPT_TIMEFLAG_IPTS) {
        for (i = 0; i < pip4opt->storedvalues; i++) {
          netwib_er(netwib_show_array_fmt32(pbuf, "   %{ip} %{uint32}",
                                            &pip4opt->ip[i],
                                            pip4opt->timestamp[i]));
        }
      } else if (pip4opt->flag == NETWIB_IP4OPT_TIMEFLAG_IPPTS) {
        for (i = 0; i < pip4opt->storedvalues; i++) {
          netwib_er(netwib_show_array_fmt32(pbuf, "   %{ip} %{uint32}",
                                            &pip4opt->ip[i],
                                            pip4opt->timestamp[i]));
        }
        for (; i < pip4opt->storagesize; i++) {
          netwib_er(netwib_show_array_fmt32(pbuf, "   %{ip}",
                                            &pip4opt->ip[i]));
        }
      } else {
        for (i = 0; i < pip4opt->storagesize; i++) {
          netwib_er(netwib_show_array_fmt32(pbuf, "   %{ip} %{uint32}",
                                            &pip4opt->ip[i],
                                            pip4opt->timestamp[i]));
        }
      }
      break;
    default :
      return(NETWIB_ERR_LOINTERNALERROR);
      break;
  }

  pip4opt = pip4opt; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_ip4opt_show(netwib_constip4opt *pip4opt,
                              netwib_encodetype encodetype,
                              netwib_buf *pbuf)
{
  netwib_buf buf;

  switch(encodetype) {
    case NETWIB_ENCODETYPE_SYNTH :
    case NETWIB_ENCODETYPE_ARRAY :
      break;
    default:
      netwib_er(netwib_buf_init_mallocdefault(&buf));
      netwib_er(netwib_pkt_append_ip4opt(pip4opt, &buf));
      netwib_er(netwib_buf_encode(&buf, encodetype, pbuf));
      netwib_er(netwib_buf_close(&buf));
      return(NETWIB_ERR_OK);
  }

  switch(pip4opt->type) {
    case NETWIB_IP4OPTTYPE_END :
      netwib_er(netwib_ip4opt_show_end(encodetype, pbuf));
      break;
    case NETWIB_IP4OPTTYPE_NOOP :
      netwib_er(netwib_ip4opt_show_noop(encodetype, pbuf));
      break;
    case NETWIB_IP4OPTTYPE_RR :
      netwib_er(netwib_ip4opt_show_rr(&pip4opt->opt.rr, encodetype,
                                      pbuf));
      break;
    case NETWIB_IP4OPTTYPE_TIME :
      netwib_er(netwib_ip4opt_show_time(&pip4opt->opt.time,
                                        encodetype, pbuf));
      break;
    case NETWIB_IP4OPTTYPE_LSRR :
      netwib_er(netwib_ip4opt_show_srr(&pip4opt->opt.lsrr, NETWIB_TRUE,
                                       encodetype, pbuf));
      break;
    case NETWIB_IP4OPTTYPE_SSRR :
      netwib_er(netwib_ip4opt_show_srr(&pip4opt->opt.ssrr, NETWIB_FALSE,
                                       encodetype, pbuf));
      break;
    default :
      return(NETWIB_ERR_LONOTIMPLEMENTED);
      break;
  }
  return(NETWIB_ERR_OK);
}
