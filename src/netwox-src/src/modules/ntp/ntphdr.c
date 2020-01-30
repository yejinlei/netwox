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
netwib_err netwox_ntphdr_initdefault(netwox_ntphdr *pntphdr)
{
  pntphdr->ntpleapindic = NETWOX_NTPLEAPINDIC_NORMAL;
  pntphdr->ntpversion = NETWOX_NTPVERSION_3;
  pntphdr->ntpmode = NETWOX_NTPMODE_RESERVED;
  pntphdr->stratum = 0;
  pntphdr->pool = 0;
  pntphdr->precision = 0;
  pntphdr->rootdelay = 0;
  pntphdr->rootdispersion = 0;
  pntphdr->refid[0] = 0;
  pntphdr->refid[1] = 0;
  pntphdr->refid[2] = 0;
  pntphdr->refid[3] = 0;
  pntphdr->refts.sec = 0;
  pntphdr->refts.fraction = 0;
  pntphdr->origts.sec = 0;
  pntphdr->origts.fraction = 0;
  pntphdr->recvts.sec = 0;
  pntphdr->recvts.fraction = 0;
  pntphdr->transts.sec = 0;
  pntphdr->transts.fraction = 0;
  netwib_er(netwib_buf_init_ext_empty(&pntphdr->extsauth));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_append_ntphdr(netwox_constntphdr *pntphdr,
                                    netwib_buf *ppkt)
{
  netwib_data data;

  netwib_er(netwib_buf_wantspace(ppkt, 48, &data));

  if (pntphdr->ntpleapindic > 3) {
    return(NETWIB_ERR_PATOOHIGH);
  }
  if (pntphdr->ntpversion > 7) {
    return(NETWIB_ERR_PATOOHIGH);
  }
  if (pntphdr->ntpmode > 7) {
    return(NETWIB_ERR_PATOOHIGH);
  }
  netwib__data_append_uint8(data, (pntphdr->ntpleapindic<<6) |
                            (pntphdr->ntpversion<<3) |
                            (pntphdr->ntpmode));
  netwib__data_append_uint8(data, pntphdr->stratum);
  netwib__data_append_uint8(data, pntphdr->pool);
  netwib__data_append_uint8(data, pntphdr->precision);

  netwib__data_append_uint32(data, pntphdr->rootdelay);

  netwib__data_append_uint32(data, pntphdr->rootdispersion);

  *data++ = pntphdr->refid[0];
  *data++ = pntphdr->refid[1];
  *data++ = pntphdr->refid[2];
  *data++ = pntphdr->refid[3];

  netwib__data_append_uint32(data, pntphdr->refts.sec);
  netwib__data_append_uint32(data, pntphdr->refts.fraction);

  netwib__data_append_uint32(data, pntphdr->origts.sec);
  netwib__data_append_uint32(data, pntphdr->origts.fraction);

  netwib__data_append_uint32(data, pntphdr->recvts.sec);
  netwib__data_append_uint32(data, pntphdr->recvts.fraction);

  netwib__data_append_uint32(data, pntphdr->transts.sec);
  netwib__data_append_uint32(data, pntphdr->transts.fraction);

  ppkt->endoffset += 48;

  netwib_er(netwib_buf_append_buf(&pntphdr->extsauth, ppkt));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_pkt_decode_ntphdr(netwib_constbuf *ppkt,
                                    netwox_ntphdr *pntphdr)
{
  netwib_data data;
  netwib_uint32 datasize;

  datasize = netwib__buf_ref_data_size(ppkt);
  if (datasize < 48) {
    return(NETWIB_ERR_DATAMISSING);
  }
  data = netwib__buf_ref_data_ptr(ppkt);

  pntphdr->ntpleapindic = data[0] >> 6;
  pntphdr->ntpversion = (data[0] >> 3) & 0x7;
  pntphdr->ntpmode = data[0] & 0x7;

  switch(pntphdr->ntpversion) {
  case NETWOX_NTPVERSION_1 :
  case NETWOX_NTPVERSION_2 :
  case NETWOX_NTPVERSION_3 :
  case NETWOX_NTPVERSION_4 :
    break;
  default :
    return(NETWIB_ERR_NOTCONVERTED);
  }
  switch(pntphdr->ntpmode) {
  case NETWOX_NTPMODE_RESERVED :
  case NETWOX_NTPMODE_SYMACTIVE :
  case NETWOX_NTPMODE_SYMPASSIVE :
  case NETWOX_NTPMODE_CLIENT :
  case NETWOX_NTPMODE_SERVER :
  case NETWOX_NTPMODE_BROADCAST :
    break;
  default :
    return(NETWIB_ERR_NOTCONVERTED);
  }

  data++;
  netwib__data_decode_uint8(data, pntphdr->stratum);
  netwib__data_decode_uint8t(data, pntphdr->pool, netwib_int8);
  netwib__data_decode_uint8t(data, pntphdr->precision, netwib_int8);
  netwib__data_decode_uint32(data, pntphdr->rootdelay);
  netwib__data_decode_uint32(data, pntphdr->rootdispersion);
  pntphdr->refid[0] = *data++;
  pntphdr->refid[1] = *data++;
  pntphdr->refid[2] = *data++;
  pntphdr->refid[3] = *data++;
  netwib__data_decode_uint32(data, pntphdr->refts.sec);
  netwib__data_decode_uint32(data, pntphdr->refts.fraction);
  netwib__data_decode_uint32(data, pntphdr->origts.sec);
  netwib__data_decode_uint32(data, pntphdr->origts.fraction);
  netwib__data_decode_uint32(data, pntphdr->recvts.sec);
  netwib__data_decode_uint32(data, pntphdr->recvts.fraction);
  netwib__data_decode_uint32(data, pntphdr->transts.sec);
  netwib__data_decode_uint32(data, pntphdr->transts.fraction);
  datasize -= 48;

  if (datasize == 0) {
    netwib_er(netwib_buf_init_ext_empty(&pntphdr->extsauth));
  } else {
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize,
                                              &pntphdr->extsauth));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_ntphdr_show_fraction(float f,
                                                   netwib_buf *pbuf)
{
  if (f < 0.001) {
    f = (float)(f*1000000000000.0);
    netwib_er(netwib_buf_append_fmt(pbuf, "~%{uint32}ps",
                                    (netwib_uint32)f));
  } else {
    f = (float)(f*1000000000.0);
    netwib_er(netwib_buf_append_fmt(pbuf, "~%{uint32}ns", (netwib_uint32)f));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_ntphdr_show_int8(netwib_int8 i,
                                               netwib_buf *pbuf)
{
  float f;

  netwib_er(netwib_buf_append_fmt(pbuf, "%{int8} (2^%{int8} = ", i, i));

  if (i > 0) {
    netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32}s)",
                                    (netwib_uint32)(1<<i)));
  } else if (i == 0) {
    netwib_er(netwib_buf_append_string("1s)", pbuf));
  } else {
    f = 0;
    i = (netwib_int8)-i;
    if (i < 32) {
      f = (float)(1.0 / (1 << i));
    }
    netwib_er(netwox_priv_ntphdr_show_fraction(f, pbuf));
    netwib_er(netwib_buf_append_byte(')', pbuf));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_ntphdr_show_int32(netwib_int32 i,
                                                netwib_buf *pbuf)
{
  netwib_int32 high;
  netwib_uint32 low;
  float f;

  high = (netwib_int32)(i >> 16);
  low = (netwib_uint32)(i & 0xFFFF);

  netwib_er(netwib_buf_append_fmt(pbuf, "%{int32}:%{uint32:#04X} (%{int32}s",
                                  high, low, high));

  if (low) {
    f = (float)low / (float)0x10000;
    netwib_er(netwib_buf_append_byte(':', pbuf));
    netwib_er(netwox_priv_ntphdr_show_fraction(f, pbuf));
  }

  netwib_er(netwib_buf_append_byte(')', pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_ntphdr_show_uint32(netwib_uint32 i,
                                                 netwib_buf *pbuf)
{
  netwib_uint32 high, low;
  float f;

  high = (netwib_int32)(i >> 16);
  low = (netwib_uint32)(i & 0xFFFF);

  netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32}:%{uint32:#04X} (%{uint32}s",
                                  high, low, high));

  if (low) {
    f = (float)low / (float)0x10000;
    netwib_er(netwib_buf_append_byte(':', pbuf));
    netwib_er(netwox_priv_ntphdr_show_fraction(f, pbuf));
  }

  netwib_er(netwib_buf_append_byte(')', pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_ntphdr_show_ts(const netwox_ntptimestamp *pts,
                                             netwib_buf *pbuf)
{
  float f;

  netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32}:%{uint32:#08X} (%{uint32}s",
                                  pts->sec, pts->fraction, pts->sec));

  if (pts->fraction) {
    f = (float)((float)pts->fraction / ((float)0xFFFFFFFFu + 1.0));
    netwib_er(netwib_buf_append_byte(':', pbuf));
    netwib_er(netwox_priv_ntphdr_show_fraction(f, pbuf));
  }

  netwib_er(netwib_buf_append_byte(')', pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ntphdr_show(netwox_constntphdr *pntphdr,
                              netwib_encodetype encodetype,
                              netwib_buf *pbuf)
{
  netwib_byte array[80];
  netwib_buf bufarray;

  if (encodetype == NETWIB_ENCODETYPE_SYNTH) {
    netwib_er(netwib_buf_append_fmt(pbuf, "NTP%{uint32}",
                                    pntphdr->ntpversion));
    return(NETWIB_ERR_OK);
  } else if (encodetype != NETWIB_ENCODETYPE_ARRAY) {
    return(NETWOX_ERR_NOTIMPLEMENTED);
  }

  netwib_er(netwib_show_array_head("NTP", pbuf));

  netwib_er(netwib_show_array_fmt32(pbuf, " version=%{uint32}",
                                    pntphdr->ntpversion));

  switch(pntphdr->ntpleapindic) {
  case NETWOX_NTPLEAPINDIC_NORMAL :
    netwib_er(netwib_buf_init_ext_string("normal", &bufarray));
    break;
  case NETWOX_NTPLEAPINDIC_LAST61 :
    netwib_er(netwib_buf_init_ext_string("last_61s", &bufarray));
    break;
  case NETWOX_NTPLEAPINDIC_LAST59 :
    netwib_er(netwib_buf_init_ext_string("last_59s", &bufarray));
    break;
  case NETWOX_NTPLEAPINDIC_NOTSYNC :
    netwib_er(netwib_buf_init_ext_string("not_synchronized", &bufarray));
    break;
  }
  netwib_er(netwib_show_array_fmt32(pbuf, " leap_indicator=%{buf}",
                                    &bufarray));

  netwib_er(netwib_buf_init_ext_string("mode=", &bufarray));
  switch(pntphdr->ntpmode) {
  case NETWOX_NTPMODE_RESERVED :
    netwib_er(netwib_buf_init_ext_string("reserved", &bufarray));
    break;
  case NETWOX_NTPMODE_SYMACTIVE :
    netwib_er(netwib_buf_init_ext_string("symmetric active", &bufarray));
    break;
  case NETWOX_NTPMODE_SYMPASSIVE :
    netwib_er(netwib_buf_init_ext_string("symmetric passive", &bufarray));
    break;
  case NETWOX_NTPMODE_CLIENT :
    netwib_er(netwib_buf_init_ext_string("client", &bufarray));
    break;
  case NETWOX_NTPMODE_SERVER :
    netwib_er(netwib_buf_init_ext_string("server", &bufarray));
    break;
  case NETWOX_NTPMODE_BROADCAST :
    netwib_er(netwib_buf_init_ext_string("broadcast", &bufarray));
    break;
  case NETWOX_NTPMODE_CTL :
    netwib_er(netwib_buf_init_ext_string("control", &bufarray));
    break;
  case NETWOX_NTPMODE_PRIV :
    netwib_er(netwib_buf_init_ext_string("private", &bufarray));
    break;
  }
  netwib_er(netwib_show_array_fmt32(pbuf, " mode=%{buf}", &bufarray));

  netwib_er(netwib_show_array_fmt32(pbuf, " stratum=%{uint32}",
                                    pntphdr->stratum));

  netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &bufarray));
  netwib_er(netwox_priv_ntphdr_show_int8(pntphdr->pool, &bufarray));
  netwib_er(netwib_show_array_fmt32(pbuf, " pool=%{buf}", &bufarray));

  netwib__buf_reinit(&bufarray);
  netwib_er(netwox_priv_ntphdr_show_int8(pntphdr->precision, &bufarray));
  netwib_er(netwib_show_array_fmt32(pbuf, " precision=%{buf}", &bufarray));

  netwib__buf_reinit(&bufarray);
  netwib_er(netwox_priv_ntphdr_show_int32(pntphdr->rootdelay, &bufarray));
  netwib_er(netwib_show_array_fmt32(pbuf, " rootdelay=%{buf}", &bufarray));

  netwib__buf_reinit(&bufarray);
  netwib_er(netwox_priv_ntphdr_show_uint32(pntphdr->rootdispersion,
                                           &bufarray));
  netwib_er(netwib_show_array_fmt32(pbuf, " rootdispersion=%{buf}",
                                    &bufarray));

  netwib_er(netwib_show_array_fmt32(pbuf,
                                  " refid=%{byte:#02X}%{byte:02X}%{byte:02X}%{byte:02X} (\"%c%c%c%c\", %{byte}.%{byte}.%{byte}.%{byte})",
                                  pntphdr->refid[0], pntphdr->refid[1], pntphdr->refid[2], pntphdr->refid[3],
                                  netwib_c2_isprint(pntphdr->refid[0])?pntphdr->refid[0]:'.', netwib_c2_isprint(pntphdr->refid[1])?pntphdr->refid[1]:'.', netwib_c2_isprint(pntphdr->refid[2])?pntphdr->refid[2]:'.', netwib_c2_isprint(pntphdr->refid[3])?pntphdr->refid[3]:'.',
                                  pntphdr->refid[0], pntphdr->refid[1], pntphdr->refid[2], pntphdr->refid[3]));

  netwib__buf_reinit(&bufarray);
  netwib_er(netwox_priv_ntphdr_show_ts(&pntphdr->refts, &bufarray));
  netwib_er(netwib_show_array_fmt32(pbuf, " refts=%{buf}", &bufarray));
  netwib__buf_reinit(&bufarray);
  netwib_er(netwox_priv_ntphdr_show_ts(&pntphdr->origts, &bufarray));
  netwib_er(netwib_show_array_fmt32(pbuf, " origts=%{buf}", &bufarray));
  netwib__buf_reinit(&bufarray);
  netwib_er(netwox_priv_ntphdr_show_ts(&pntphdr->recvts, &bufarray));
  netwib_er(netwib_show_array_fmt32(pbuf, " recvts=%{buf}", &bufarray));
  netwib__buf_reinit(&bufarray);
  netwib_er(netwox_priv_ntphdr_show_ts(&pntphdr->transts, &bufarray));
  netwib_er(netwib_show_array_fmt32(pbuf, " transts=%{buf}", &bufarray));

  if (netwib__buf_ref_data_size(&pntphdr->extsauth)) {
    netwib_er(netwib_show_array_data(" extsauth", &pntphdr->extsauth,
                                     NETWIB_ENCODETYPE_HEXA0, ' ', pbuf));
  }

  netwib_er(netwib_show_array_tail(pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ntphdr_test(void)
{
  netwib_bufpool *pbufpool;
  netwib_buf pkt, pktmix;

  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_buf_init_mallocdefault(&pktmix));
  netwib_er(netwib_buf_append_string("0B0106F0", &pktmix));
  netwib_er(netwib_buf_append_string("02345678", &pktmix));
  netwib_er(netwib_buf_append_string("1122FFFF", &pktmix));
  netwib_er(netwib_buf_append_string("0A0B410D", &pktmix));
  netwib_er(netwib_buf_append_string("00000002 00000003", &pktmix));
  netwib_er(netwib_buf_append_string("00000004 00000005", &pktmix));
  netwib_er(netwib_buf_append_string("00000006 FFFFFFFF", &pktmix));
  netwib_er(netwib_buf_append_string("00000008 00001009", &pktmix));
  netwib_er(netwib_buf_append_string("'az' 00 01", &pktmix));
  netwib_er(netwib_buf_decode(&pktmix, NETWIB_DECODETYPE_MIXED, &pkt));
  netwib_er(netwox_pkt_ntp_display(pbufpool, &pkt, NULL,
                                   NETWIB_ENCODETYPE_ARRAY,
                                   NETWIB_ENCODETYPE_DUMP));

  netwib_er(netwib_buf_close(&pkt));
  netwib_er(netwib_buf_close(&pktmix));
  netwib_er(netwib_bufpool_close(&pbufpool));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#define NETWOX_NTP_OFFSET 2208988800u
netwib_err netwox_time_init_ntptimestamp(netwox_constntptimestamp *pts,
                                         netwib_time *pt)
{
  float f;

  if (pts->sec < NETWOX_NTP_OFFSET) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  pt->sec = pts->sec - NETWOX_NTP_OFFSET;

  f = (float)((float)pts->fraction / 4294967296.0);
  f = (float)(f*1000000000.0);
  pt->nsec = (netwib_uint32)f;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_ntptimestamp_init_time(netwib_consttime *pt,
                                         netwox_ntptimestamp *pts)
{
  float f;

  pts->sec = pt->sec + NETWOX_NTP_OFFSET;
  f = (float)((pt->nsec / 1000000000.0) * 4294967296.0);
  pts->fraction = (netwib_uint32)f;

  return(NETWIB_ERR_OK);
}
