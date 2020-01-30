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
#define NETWOX_TIME1601_369YEARS_HIGH 0x2
#define NETWOX_TIME1601_369YEARS_LOW 0xB6109100u
#define NETWOX_TIME1601_100NANOSEC 10000000

/*-------------------------------------------------------------*/
netwib_err netwox_time1601_init_time(netwib_consttime *ptime,
                                     netwox_time1601 *ptime1601)
{
  netwib_uint64 tmp1, tmp2, tmp3;

  if (ptime == NETWIB_TIME_ZERO) {
    netwib__uint64_init_uint32(0, *ptime1601);
    return(NETWIB_ERR_OK);
  }
  if (ptime == NETWIB_TIME_INFINITE) {
    netwib__uint64_init_32(0xFFFFFFFFu, 0xFFFFFFFFu, *ptime1601);
    return(NETWIB_ERR_OK);
  }

  netwib__uint64_init_32(NETWOX_TIME1601_369YEARS_HIGH, NETWOX_TIME1601_369YEARS_LOW, tmp1);
  netwib__uint64_init_uint32(ptime->sec, tmp2);
  netwib__uint64_add(tmp1, tmp2, tmp3);
  netwib__uint64_init_uint32(NETWOX_TIME1601_100NANOSEC, tmp1);
  netwib__uint64_mul(tmp1, tmp3, &tmp2);
  netwib__uint64_init_uint32(ptime->nsec / 100, tmp1);
  netwib__uint64_add(tmp2, tmp1, *ptime1601);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_time_init_time1601(netwox_time1601 time1601,
                                     netwib_time *ptime)
{
  netwib_uint64 tmp1, tmp2, tmp3;

  netwib__uint64_init_uint32(NETWOX_TIME1601_100NANOSEC, tmp1);
  netwib__uint64_div(time1601, tmp1, &tmp2, &tmp3);
  if (netwib_c2_uint64_32high(tmp3) != 0) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  ptime->nsec = 100 * netwib_c2_uint64_32low(tmp3);
  netwib__uint64_init_32(NETWOX_TIME1601_369YEARS_HIGH, NETWOX_TIME1601_369YEARS_LOW, tmp1);
  netwib__uint64_sub(tmp2, tmp1, tmp3);
  if (netwib_c2_uint64_32high(tmp3) != 0) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  ptime->sec = netwib_c2_uint64_32low(tmp3);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_time1601_test(void)
{
  netwib_time t1, t2;
  netwox_time1601 t1601;

  netwib_er(netwib_time_init_now(&t1));
  netwib_er(netwib_fmt_display("%{uint32}.%{uint32}\n", t1.sec, t1.nsec));
  netwib_er(netwox_time1601_init_time(&t1, &t1601));
  netwib_er(netwib_fmt_display("%{uint64} %{uint64:16X}\n", t1601, t1601));
  netwib_er(netwox_time_init_time1601(t1601, &t2));
  netwib_er(netwib_fmt_display("%{uint32}.%{uint32}\n", t2.sec, t2.nsec));
  if (t1.sec != t2.sec) {
    return(NETWOX_ERR_INTERNALERROR);
  }
  if (t1.nsec < t2.nsec || t1.nsec > t2.nsec + 99) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_time1970l_init_time(netwib_consttime *ptime,
                                      netwox_time1970l *ptime1970l)
{
  netwib_localtime lt;

  if (ptime == NETWIB_TIME_ZERO) {
    *ptime1970l = 0;
    return(NETWIB_ERR_OK);
  }
  if (ptime == NETWIB_TIME_INFINITE) {
    *ptime1970l = 0xFFFFFFFFu;
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_time_decode_localtime(ptime, &lt));

  if (lt.zoneoffset < 0) {
    if (ptime->sec < (netwib_uint32)(-lt.zoneoffset)) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else if (lt.zoneoffset > 0) {
    if (ptime->sec > 0xFFFFFFFFu - (netwib_uint32)lt.zoneoffset) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }
  *ptime1970l = ptime->sec + lt.zoneoffset;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_time_init_time1970l(netwox_time1970l time1970l,
                                      netwib_time *ptime)
{
  netwib_time t;
  netwib_localtime lt;

  netwib_er(netwib_time_init_now(&t));
  netwib_er(netwib_time_decode_localtime(&t, &lt));

  if (lt.zoneoffset > 0) {
    if (time1970l < (netwib_uint32)lt.zoneoffset) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  } else if (lt.zoneoffset < 0) {
    if (time1970l > 0xFFFFFFFFu - (netwib_uint32)(-lt.zoneoffset)) {
      return(NETWIB_ERR_NOTCONVERTED);
    }
  }
  ptime->sec = time1970l - lt.zoneoffset;
  ptime->nsec = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_time1970l_test(void)
{
  netwib_time t1, t2;
  netwox_time1970l t1970l;

  netwib_er(netwib_time_init_now(&t1));
  netwib_er(netwib_fmt_display("%{uint32}.%{uint32}\n", t1.sec, t1.nsec));
  netwib_er(netwox_time1970l_init_time(&t1, &t1970l));
  netwib_er(netwib_fmt_display("%{uint32} %{uint32:8X}\n", t1970l, t1970l));
  netwib_er(netwox_time_init_time1970l(t1970l, &t2));
  netwib_er(netwib_fmt_display("%{uint32}.%{uint32}\n", t2.sec, t2.nsec));
  if (t1.sec != t2.sec) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  return(NETWIB_ERR_OK);

}

/*-------------------------------------------------------------*/
netwib_err netwox_timemsdos_init_time(netwib_consttime *ptime,
                                      netwox_timemsdos *ptimemsdos)
{
  netwib_localtime lt;

  netwib_er(netwib_time_decode_localtime(ptime, &lt));

  if (lt.year < 1980) {
    return(NETWIB_ERR_NOTCONVERTED);
  }
  ptimemsdos->msdosdate = (netwib_uint16)(((lt.year-1980)<<9) |
                                          (lt.mon<<5) |
                                          (lt.mday));

  ptimemsdos->msdostime = (netwib_uint16)((lt.hour<<11) |
                                          (lt.min<<5) |
                                          (lt.sec/2));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_time_init_timemsdos(netwox_timemsdos timemsdos,
                                      netwib_time *ptime)
{
  netwib_localtime lt;

  lt.year = 1980 + ((timemsdos.msdosdate>>9) & 0x7F);
  lt.mon = (timemsdos.msdosdate>>5) & 0xF;
  lt.mday = timemsdos.msdosdate & 0x1F;

  lt.hour = (timemsdos.msdostime>>11) & 0x1F;
  lt.min = (timemsdos.msdostime>>5) & 0x3F;
  lt.sec = 2 * (timemsdos.msdostime & 0x1F);

  netwib_er(netwib_time_init_localtime(&lt, ptime));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_timemsdos_test(void)
{
  netwib_time t1, t2;
  netwox_timemsdos tmsdos;

  netwib_er(netwib_time_init_now(&t1));
  netwib_er(netwib_fmt_display("%{uint32}.%{uint32}\n", t1.sec, t1.nsec));
  netwib_er(netwox_timemsdos_init_time(&t1, &tmsdos));
  netwib_er(netwib_fmt_display("%{uint16}:%{uint16}: %{uint32:8X}:%{uint32:8X}\n", tmsdos.msdosdate, tmsdos.msdostime, tmsdos.msdosdate, tmsdos.msdostime));
  netwib_er(netwox_time_init_timemsdos(tmsdos, &t2));
  netwib_er(netwib_fmt_display("%{uint32}.%{uint32}\n", t2.sec, t2.nsec));
  if (t1.sec != t2.sec && t1.sec != t2.sec+1) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  return(NETWIB_ERR_OK);

}

/*-------------------------------------------------------------*/
netwib_err netwox_time1601_init_timemsdos(netwox_timemsdos timemsdos,
                                          netwox_time1601 *ptime1601)
{
  netwib_time t;
  netwib_err ret;

  ret = netwox_time_init_timemsdos(timemsdos, &t);
  if (ret == NETWIB_ERR_OK) {
    ret = netwox_time1601_init_time(&t, ptime1601);
  }
  if (ret != NETWIB_ERR_OK) {
    netwib__uint64_init_uint32(0, *ptime1601);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_timemsdos_init_time1601(netwox_time1601 time1601,
                                          netwox_timemsdos *ptimemsdos)
{
  netwib_time t;
  netwib_err ret;

  ret = netwox_time_init_time1601(time1601, &t);
  if (ret == NETWIB_ERR_OK) {
    ret = netwox_timemsdos_init_time(&t, ptimemsdos);
  }
  if (ret != NETWIB_ERR_OK) {
    ptimemsdos->msdosdate = 0;
    ptimemsdos->msdostime = 0;
  }

  return(NETWIB_ERR_OK);
}
