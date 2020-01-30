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
#define netwib_priv_time_init_zero(ptime) {ptime->sec=0;ptime->nsec=0;}
#define netwib_priv_time_init_infinite(ptime) {ptime->sec=0xFFFFFFFFu;ptime->nsec=999999999u;}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_time_norm(netwib_uint32 sec,
                                        netwib_uint32 msec,
                                        netwib_uint32 usec,
                                        netwib_uint32 nsec,
                                        netwib_uint32 *psec,
                                        netwib_uint32 *pnsec)
{
  netwib_uint32 secsum, nsecsum, tmpval;

  /* simple case */
  if (msec == 0 && usec == 0 && nsec < NETWIB_PRIV_NUM_1G) {
    *psec = sec;
    *pnsec = nsec;
    return(NETWIB_ERR_OK);
  }

  /* complex case */
  /* convert nsec */
  tmpval = nsec / NETWIB_PRIV_NUM_1G;
  if (nsec < NETWIB_PRIV_NUM_1G) {
    nsecsum = nsec;
    secsum = 0;
  } else {
    tmpval = nsec / NETWIB_PRIV_NUM_1G;
    nsecsum = nsec - tmpval * NETWIB_PRIV_NUM_1G;
    secsum = tmpval;
  }

  /* convert usec */
  if (usec < NETWIB_PRIV_NUM_1M) {
    nsecsum += NETWIB_PRIV_NUM_1K * usec;
  } else {
    tmpval = usec / NETWIB_PRIV_NUM_1M;
    nsecsum += NETWIB_PRIV_NUM_1K * (usec - tmpval*NETWIB_PRIV_NUM_1M);
    secsum += tmpval;
  }
  /* can have an overflow of nsecsum */
  if (nsecsum >= NETWIB_PRIV_NUM_1G) {
    tmpval = nsecsum / NETWIB_PRIV_NUM_1G;
    nsecsum -= tmpval * NETWIB_PRIV_NUM_1G;
    secsum += tmpval;
  }

  /* convert msec */
  if (msec < NETWIB_PRIV_NUM_1K) {
    nsecsum += NETWIB_PRIV_NUM_1M * msec;
  } else {
    tmpval = msec / NETWIB_PRIV_NUM_1K;
    nsecsum += NETWIB_PRIV_NUM_1M * (msec - tmpval*NETWIB_PRIV_NUM_1K);
    secsum += tmpval;
  }
  /* can have an overflow of nsecsum */
  if (nsecsum >= NETWIB_PRIV_NUM_1G) {
    tmpval = nsecsum / NETWIB_PRIV_NUM_1G;
    nsecsum -= tmpval * NETWIB_PRIV_NUM_1G;
    secsum += tmpval;
  }

  if (0xFFFFFFFFu - sec < secsum) {
    /* overflow */
    return(NETWIB_ERR_PATOOHIGH);
  }
  secsum += sec;

  *psec = secsum;
  *pnsec = nsecsum;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_time_init_time(netwib_consttime *ptimein,
                                 netwib_time *ptimeout)
{

  /* parameter verification */
  if (ptimein == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  /* parameter verification */
  if (ptimeout == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* set parameters */
  if (ptimein == NETWIB_TIME_ZERO) {
    netwib_priv_time_init_zero(ptimeout);
  } else if (ptimein == NETWIB_TIME_INFINITE) {
    netwib_priv_time_init_infinite(ptimeout);
    return(NETWIB_ERR_OK);
  } else {
    ptimeout->sec = ptimein->sec;
    ptimeout->nsec = ptimein->nsec;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_time_init_fields(netwib_uint32 sec,
                                   netwib_uint32 msec,
                                   netwib_uint32 usec,
                                   netwib_uint32 nsec,
                                   netwib_time *ptime)
{
  netwib_err ret;

  /* parameter verification */
  if (ptime == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  ret = netwib_priv_time_norm(sec, msec, usec, nsec,
                             &ptime->sec, &ptime->nsec);
  if (ret == NETWIB_ERR_PATOOHIGH) {
    netwib_priv_time_init_infinite(ptime);
    ret = NETWIB_ERR_OK;
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_time_decode_sec(netwib_consttime *ptime,
                                  netwib_uint32 *psec)
{
  /* parameters verification */
  if (ptime == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* special cases of ZERO and INFINITE */
  if (ptime == NETWIB_TIME_ZERO) {
    if (psec != NULL) *psec = 0;
    return(NETWIB_ERR_OK);
  }
  if (ptime == NETWIB_TIME_INFINITE) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  /* store values */
  if (psec != NULL) {
    *psec = ptime->sec;
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_time_decode_msec(netwib_consttime *ptime,
                                   netwib_uint32 *pmsec)
{
  /* parameters verification */
  if (ptime == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* special cases of ZERO and INFINITE */
  if (ptime == NETWIB_TIME_ZERO) {
    if (pmsec != NULL) *pmsec = 0;
    return(NETWIB_ERR_OK);
  }
  if (ptime == NETWIB_TIME_INFINITE) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  if (ptime->sec > 4294967)
    return(NETWIB_ERR_NOTCONVERTED);
  if (ptime->sec == 4294967 && ptime->nsec > 295999999)
    return(NETWIB_ERR_NOTCONVERTED);

  /* store values */
  if (pmsec != NULL) {
    *pmsec = NETWIB_PRIV_NUM_1K * ptime->sec + ptime->nsec / NETWIB_PRIV_NUM_1M;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_time_decode_usec(netwib_consttime *ptime,
                                   netwib_uint32 *pusec)
{
  /* parameters verification */
  if (ptime == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* special cases of ZERO and INFINITE */
  if (ptime == NETWIB_TIME_ZERO) {
    if (pusec != NULL) *pusec = 0;
    return(NETWIB_ERR_OK);
  }
  if (ptime == NETWIB_TIME_INFINITE) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  if (ptime->sec > 4294)
    return(NETWIB_ERR_NOTCONVERTED);
  if (ptime->sec == 4294 && ptime->nsec > 967295999)
    return(NETWIB_ERR_NOTCONVERTED);

  /* store values */
  if (pusec != NULL) {
    *pusec = NETWIB_PRIV_NUM_1M * ptime->sec + ptime->nsec / NETWIB_PRIV_NUM_1K;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_time_decode_nsec(netwib_consttime *ptime,
                                   netwib_uint32 *pnsec)
{
  /* parameters verification */
  if (ptime == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* special cases of ZERO and INFINITE */
  if (ptime == NETWIB_TIME_ZERO) {
    if (pnsec != NULL) *pnsec = 0;
    return(NETWIB_ERR_OK);
  }
  if (ptime == NETWIB_TIME_INFINITE) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  if (ptime->sec > 4)
    return(NETWIB_ERR_NOTCONVERTED);
  if (ptime->sec == 4 && ptime->nsec > 294967295)
    return(NETWIB_ERR_NOTCONVERTED);

  /* store values */
  if (pnsec != NULL) {
    *pnsec = NETWIB_PRIV_NUM_1G * ptime->sec + ptime->nsec;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_time_decode_fields(netwib_consttime *ptime,
                                     netwib_uint32 *psec,
                                     netwib_uint32 *pmsec,
                                     netwib_uint32 *pusec,
                                     netwib_uint32 *pnsec)
{
  netwib_uint32 msec, usec, nsec;

  /* parameters verification */
  if (ptime == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* special cases of ZERO and INFINITE */
  if (ptime == NETWIB_TIME_ZERO) {
    if (psec != NULL) *psec = 0;
    if (pmsec != NULL) *pmsec = 0;
    if (pusec != NULL) *pusec = 0;
    if (pnsec != NULL) *pnsec = 0;
    return(NETWIB_ERR_OK);
  }
  if (ptime == NETWIB_TIME_INFINITE) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  /* compute values */
  nsec = ptime->nsec;
  msec = nsec / NETWIB_PRIV_NUM_1M;
  if (msec) {
    nsec -= msec * NETWIB_PRIV_NUM_1M;
  }
  usec = nsec / NETWIB_PRIV_NUM_1K;
  if (usec) {
    nsec -= usec * NETWIB_PRIV_NUM_1K;
  }

  /* store values */
  if (psec != NULL)
    *psec = ptime->sec;
  if (pmsec != NULL)
    *pmsec = msec;
  if (pusec != NULL)
    *pusec = usec;
  if (pnsec != NULL)
    *pnsec = nsec;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_time_plus_time(netwib_time *ptime,
                                 netwib_consttime *ptimetoadd)
{
  netwib_uint32 sec, nsec;

  if (ptime == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  if (ptimetoadd == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* if zero, change nothing */
  if (ptimetoadd == NETWIB_TIME_ZERO) {
    return(NETWIB_ERR_OK);
  }

  /* if infinite, set max */
  if (ptimetoadd == NETWIB_TIME_INFINITE) {
    netwib_priv_time_init_infinite(ptime);
    return(NETWIB_ERR_OK);
  }

  /* compute sum */
  nsec = ptime->nsec + ptimetoadd->nsec;
  sec = ptime->sec;
  if (nsec >= NETWIB_PRIV_NUM_1G) {
    nsec -= NETWIB_PRIV_NUM_1G;
    if (ptime->sec == 0xFFFFFFFFu) {
      netwib_priv_time_init_infinite(ptime);
      return(NETWIB_ERR_OK);
    }
    sec ++;
  }
  if (ptimetoadd->sec > 0xFFFFFFFFu - sec) {
    netwib_priv_time_init_infinite(ptime);
    return(NETWIB_ERR_OK);
  }
  sec += ptimetoadd->sec;

  /* update time */
  ptime->sec = sec;
  ptime->nsec = nsec;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_time_plus_fields(netwib_time *ptime,
                                   netwib_uint32 sec,
                                   netwib_uint32 msec,
                                   netwib_uint32 usec,
                                   netwib_uint32 nsec)
{
  netwib_time timefields;

  netwib_er(netwib_time_init_fields(sec, msec, usec, nsec, &timefields));
  netwib_er(netwib_time_plus_time(ptime, &timefields));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_time_minus_time(netwib_time *ptime,
                                  netwib_consttime *ptimetosub)
{

  if (ptime == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  if (ptimetosub == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* if zero, change nothing */
  if (ptimetosub == NETWIB_TIME_ZERO) {
    return(NETWIB_ERR_OK);
  }

  /* if infinite, negative difference */
  if (ptimetosub == NETWIB_TIME_INFINITE) {
    return(NETWIB_ERR_PATIMEDIFFNEG);
  }

  /* negative difference */
  if (ptime->sec < ptimetosub->sec) {
    return(NETWIB_ERR_PATIMEDIFFNEG);
  }
  if (ptime->sec == ptimetosub->sec && ptime->nsec <ptimetosub->nsec) {
    return(NETWIB_ERR_PATIMEDIFFNEG);
  }

  /* compute */
  if (ptime->nsec >= ptimetosub->nsec) {
    ptime->sec -= ptimetosub->sec;
    ptime->nsec -= ptimetosub->nsec;
  } else {
    ptime->sec = ptime->sec - ptimetosub->sec - 1;
    ptime->nsec = ptime->nsec - ptimetosub->nsec + NETWIB_PRIV_NUM_1G;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_time_minus_fields(netwib_time *ptime,
                                    netwib_uint32 sec,
                                    netwib_uint32 msec,
                                    netwib_uint32 usec,
                                    netwib_uint32 nsec)
{
  netwib_time timefields;

  netwib_er(netwib_time_init_fields(sec, msec, usec, nsec, &timefields));
  netwib_er(netwib_time_minus_time(ptime, &timefields));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_time_cmp(netwib_consttime *ptime1,
                           netwib_consttime *ptime2,
                           netwib_cmp *pcmp)
{
  netwib_cmp cmp;

  if (ptime1 == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  if (ptime2 == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  if (ptime1 == NETWIB_TIME_ZERO) {
    if (ptime2 == NETWIB_TIME_ZERO) {
      cmp = NETWIB_CMP_EQ;
    } else {
      cmp = NETWIB_CMP_LT;
    }
  } else if (ptime1 == NETWIB_TIME_INFINITE) {
    if (ptime2 == NETWIB_TIME_INFINITE) {
      cmp = NETWIB_CMP_EQ;
    } else {
      cmp = NETWIB_CMP_GT;
    }
  } else if (ptime2 == NETWIB_TIME_ZERO) {
    if (ptime1 == NETWIB_TIME_ZERO) {
      cmp = NETWIB_CMP_EQ;
    } else {
      cmp = NETWIB_CMP_GT;
    }
  } else if (ptime2 == NETWIB_TIME_INFINITE) {
    if (ptime1 == NETWIB_TIME_INFINITE) {
      cmp = NETWIB_CMP_EQ;
    } else {
      cmp = NETWIB_CMP_LT;
    }
  } else {
    if (ptime1->sec < ptime2->sec) {
      cmp = NETWIB_CMP_LT;
    } else if (ptime1->sec > ptime2->sec) {
      cmp = NETWIB_CMP_GT;
    } else {
      if (ptime1->nsec < ptime2->nsec) {
        cmp = NETWIB_CMP_LT;
      } else if (ptime1->nsec > ptime2->nsec) {
        cmp = NETWIB_CMP_GT;
      } else {
        cmp = NETWIB_CMP_EQ;
      }
    }
  }

  if (pcmp != NULL) *pcmp = cmp;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_time_sleep_time(netwib_consttime *preltime)
{

  /* parameter verification */
  if (preltime == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* no need to sleep */
  if (preltime == NETWIB_TIME_ZERO) {
    return(NETWIB_ERR_OK);
  }

  /* infinite sleep */
  if (preltime == NETWIB_TIME_INFINITE) {
    netwib_er(netwib_priv_time_sleep(0xFFFFFFFFu, 0));
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_priv_time_sleep(preltime->sec, preltime->nsec));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_time_wait_time(netwib_consttime *pabstime)
{
  netwib_time now, diff;
  netwib_err ret;

  /* parameter verification */
  if (pabstime == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  /* no need to wait */
  if (pabstime == NETWIB_TIME_ZERO) {
    return(NETWIB_ERR_OK);
  }

  /* infinite wait */
  if (pabstime == NETWIB_TIME_INFINITE) {
    netwib_er(netwib_priv_time_sleep(0xFFFFFFFFu, 0));
    return(NETWIB_ERR_OK);
  }

  /* wait */
  netwib_er(netwib_priv_time_init_now(&now.sec, &now.nsec));
  diff = *pabstime;
  ret = netwib_time_minus_time(&diff, &now);
  if (ret == NETWIB_ERR_PATIMEDIFFNEG) {
    /* elapsed */
    ret = NETWIB_ERR_OK;
  } else if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_priv_time_sleep(diff.sec, diff.nsec));
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_time_sleep_fields(netwib_uint32 sec,
                                    netwib_uint32 msec,
                                    netwib_uint32 usec,
                                    netwib_uint32 nsec)
{
  netwib_uint32 secsum, nsecsum;
  netwib_err ret;

  ret = netwib_priv_time_norm(sec, msec, usec, nsec, &secsum, &nsecsum);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_priv_time_sleep(secsum, nsecsum));
  } else if (ret == NETWIB_ERR_PATOOHIGH) {
    netwib_er(netwib_priv_time_sleep(0xFFFFFFFFu, 0));
  } else {
    return(ret);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_buf_append_time2(netwib_consttime *preltime,
                                          netwib_time_encodetype encodetype,
                                          netwib_buf *pbuf)
{
  netwib_uint32 sec, msec, usec, nsec, xday, xhour, xmin, ui;
  netwib_bool addsep;

  /* special case of infinite */
  if (preltime == NETWIB_TIME_INFINITE) {
    switch(encodetype) {
      case NETWIB_TIME_ENCODETYPE_NOTHING :
        break;
      case NETWIB_TIME_ENCODETYPE_BEST :
      case NETWIB_TIME_ENCODETYPE_BEST2 :
      case NETWIB_TIME_ENCODETYPE_SEC :
      case NETWIB_TIME_ENCODETYPE_SEC2 :
      case NETWIB_TIME_ENCODETYPE_MSEC :
      case NETWIB_TIME_ENCODETYPE_MSEC2 :
      case NETWIB_TIME_ENCODETYPE_USEC :
      case NETWIB_TIME_ENCODETYPE_USEC2 :
      case NETWIB_TIME_ENCODETYPE_NSEC :
      case NETWIB_TIME_ENCODETYPE_NSEC2 :
        netwib_er(netwib_buf_append_string("infinite", pbuf));
        break;
      default :
        return(NETWIB_ERR_PAINVALIDTYPE);
        break;
    }
    return(NETWIB_ERR_OK);
  }

  switch(encodetype) {
    case NETWIB_TIME_ENCODETYPE_NOTHING :
      break;
    case NETWIB_TIME_ENCODETYPE_BEST :
    case NETWIB_TIME_ENCODETYPE_BEST2 :
      netwib_er(netwib_time_decode_fields(preltime, &ui, &msec, &usec, &nsec));
      sec = ui % 60;
      ui = (ui - sec) / 60;
      xmin = ui % 60;
      ui = (ui - xmin) / 60;
      xhour = ui % 24;
      xday = (ui - xhour) / 24;
      addsep = NETWIB_FALSE;
      if (xday) {
        netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32}", xday));
        if (encodetype == NETWIB_TIME_ENCODETYPE_BEST) {
          netwib_er(netwib_buf_append_byte('d', pbuf));
        } else {
          netwib_er(netwib_buf_append_string(" day", pbuf));
          if (xday != 1) {
            netwib_er(netwib_buf_append_byte('s', pbuf));
          }
        }
        addsep = NETWIB_TRUE;
      }
      if (xmin) {
        if (addsep) {
          if (encodetype == NETWIB_TIME_ENCODETYPE_BEST) {
            netwib_er(netwib_buf_append_byte(':', pbuf));
          } else {
            netwib_er(netwib_buf_append_byte(' ', pbuf));
          }
        }
        netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32}", xmin));
        if (encodetype == NETWIB_TIME_ENCODETYPE_BEST) {
          netwib_er(netwib_buf_append_byte('m', pbuf));
        } else {
          netwib_er(netwib_buf_append_string(" minute", pbuf));
          if (xmin != 1) {
            netwib_er(netwib_buf_append_byte('s', pbuf));
          }
        }
        addsep = NETWIB_TRUE;
      }
      if (xhour) {
        if (addsep) {
          if (encodetype == NETWIB_TIME_ENCODETYPE_BEST) {
            netwib_er(netwib_buf_append_byte(':', pbuf));
          } else {
            netwib_er(netwib_buf_append_byte(' ', pbuf));
          }
        }
        netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32}", xhour));
        if (encodetype == NETWIB_TIME_ENCODETYPE_BEST) {
          netwib_er(netwib_buf_append_byte('h', pbuf));
        } else {
          netwib_er(netwib_buf_append_string(" hour", pbuf));
          if (xhour != 1) {
            netwib_er(netwib_buf_append_byte('s', pbuf));
          }
        }
        addsep = NETWIB_TRUE;
      }
      if (sec) {
        if (addsep) {
          if (encodetype == NETWIB_TIME_ENCODETYPE_BEST) {
            netwib_er(netwib_buf_append_byte(':', pbuf));
          } else {
            netwib_er(netwib_buf_append_byte(' ', pbuf));
          }
        }
        netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32}", sec));
        if (encodetype == NETWIB_TIME_ENCODETYPE_BEST) {
          netwib_er(netwib_buf_append_byte('s', pbuf));
        } else {
          netwib_er(netwib_buf_append_string(" second", pbuf));
          if (sec != 1) {
            netwib_er(netwib_buf_append_byte('s', pbuf));
          }
        }
        addsep = NETWIB_TRUE;
      }
      if (msec) {
        if (addsep) {
          if (encodetype == NETWIB_TIME_ENCODETYPE_BEST) {
            netwib_er(netwib_buf_append_byte(':', pbuf));
          } else {
            netwib_er(netwib_buf_append_byte(' ', pbuf));
          }
        }
        netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32}", msec));
        if (encodetype == NETWIB_TIME_ENCODETYPE_BEST) {
          netwib_er(netwib_buf_append_string("ms", pbuf));
        } else {
          netwib_er(netwib_buf_append_string(" millisecond", pbuf));
          if (sec != 1) {
            netwib_er(netwib_buf_append_byte('s', pbuf));
          }
        }
        addsep = NETWIB_TRUE;
      }
      if (usec) {
        if (addsep) {
          if (encodetype == NETWIB_TIME_ENCODETYPE_BEST) {
            netwib_er(netwib_buf_append_byte(':', pbuf));
          } else {
            netwib_er(netwib_buf_append_byte(' ', pbuf));
          }
        }
        netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32}", usec));
        if (encodetype == NETWIB_TIME_ENCODETYPE_BEST) {
          netwib_er(netwib_buf_append_string("us", pbuf));
        } else {
          netwib_er(netwib_buf_append_string(" microsecond", pbuf));
          if (usec != 1) {
            netwib_er(netwib_buf_append_byte('s', pbuf));
          }
        }
        addsep = NETWIB_TRUE;
      }
      if (nsec) {
        if (addsep) {
          if (encodetype == NETWIB_TIME_ENCODETYPE_BEST) {
            netwib_er(netwib_buf_append_byte(':', pbuf));
          } else {
            netwib_er(netwib_buf_append_byte(' ', pbuf));
          }
        }
        netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32}", nsec));
        if (encodetype == NETWIB_TIME_ENCODETYPE_BEST) {
          netwib_er(netwib_buf_append_string("ns", pbuf));
        } else {
          netwib_er(netwib_buf_append_string(" nanosecond", pbuf));
          if (nsec != 1) {
            netwib_er(netwib_buf_append_byte('s', pbuf));
          }
        }
        addsep = NETWIB_TRUE;
      }
      break;
    case NETWIB_TIME_ENCODETYPE_SEC :
      netwib_er(netwib_time_decode_sec(preltime, &sec));
      netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32}s", sec));
      break;
    case NETWIB_TIME_ENCODETYPE_SEC2 :
      netwib_er(netwib_time_decode_sec(preltime, &sec));
      netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32} second", sec));
      if (sec > 1) {
        netwib_er(netwib_buf_append_byte('s', pbuf));
      }
      break;
    case NETWIB_TIME_ENCODETYPE_MSEC :
      netwib_er(netwib_time_decode_msec(preltime, &msec));
      netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32}ms", msec));
      break;
    case NETWIB_TIME_ENCODETYPE_MSEC2 :
      netwib_er(netwib_time_decode_msec(preltime, &msec));
      netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32} millisecond", msec));
      if (msec > 1) {
        netwib_er(netwib_buf_append_byte('s', pbuf));
      }
      break;
    case NETWIB_TIME_ENCODETYPE_USEC :
      netwib_er(netwib_time_decode_usec(preltime, &usec));
      netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32}us", usec));
      break;
    case NETWIB_TIME_ENCODETYPE_USEC2 :
      netwib_er(netwib_time_decode_usec(preltime, &usec));
      netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32} microsecond", usec));
      if (usec > 1) {
        netwib_er(netwib_buf_append_byte('s', pbuf));
      }
      break;
    case NETWIB_TIME_ENCODETYPE_NSEC :
      netwib_er(netwib_time_decode_nsec(preltime, &nsec));
      netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32}ns", nsec));
      break;
    case NETWIB_TIME_ENCODETYPE_NSEC2 :
      netwib_er(netwib_time_decode_nsec(preltime, &nsec));
      netwib_er(netwib_buf_append_fmt(pbuf, "%{uint32} nanosecond", nsec));
      if (nsec > 1) {
        netwib_er(netwib_buf_append_byte('s', pbuf));
      }
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_time(netwib_consttime *preltime,
                                  netwib_time_encodetype encodetype,
                                  netwib_buf *pbuf)
{
  netwib_uint32 savedsize;
  netwib_err ret;

  if (pbuf == NULL) {
    return(NETWIB_ERR_OK);
  }

  /* save position in case of error */
  savedsize = netwib__buf_ref_data_size(pbuf);

  ret = netwib_buf_append_time2(preltime, encodetype, pbuf);

  /* on space error, restore first pos */
  if (ret == NETWIB_ERR_DATANOSPACE) {
    pbuf->endoffset = pbuf->beginoffset + savedsize;
  }
  return(ret);
}


/*-------------------------------------------------------------*/
netwib_err netwib_time_init_now(netwib_time *pabstime)
{
  if (pabstime == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  netwib_er(netwib_priv_time_init_now(&pabstime->sec, &pabstime->nsec));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_time_init_localtime(netwib_constlocaltime *plocaltime,
                                      netwib_time *ptime)
{
  struct tm t;
  time_t tt;

  if (plocaltime == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  if (ptime == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  t.tm_sec = plocaltime->sec;
  t.tm_min = plocaltime->min;
  t.tm_hour = plocaltime->hour;
  t.tm_mday = plocaltime->mday;
  if (plocaltime->mon == 0) return(NETWIB_ERR_PATOOLOW);
  t.tm_mon = plocaltime->mon - 1;
  if (plocaltime->year < 1900) return(NETWIB_ERR_PATOOLOW);
  t.tm_year = plocaltime->year - 1900;
  t.tm_isdst = netwib_priv_glovars.time_isdst?1:0;

  tt = mktime(&t);
  if (tt == -1) {
    return(NETWIB_ERR_NOTCONVERTED);
  }

  ptime->sec = (netwib_uint32)tt;
  ptime->nsec = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/* this define is here because I have a script checking if
   netwib contains MT-unsafe functions. Like this, they are ignored. */
#define netwib_c_localtime localtime

/*-------------------------------------------------------------*/
netwib_err netwib_time_decode_localtime(netwib_consttime *ptime,
                                        netwib_localtime *plocaltime)
{
  struct tm *ptm;
  time_t tt;

  if (plocaltime == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  if (ptime == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  tt = ptime->sec;

#if NETWIBDEF_HAVEFUNC_LOCALTIME_R == 1
  {
    struct tm tm2;
 #if NETWIBDEF_LIBPTHREADFROMDCE == 1
    {
      int reti;
      reti = localtime_r(&tt, &tm2);
      if (reti) {
        return(NETWIB_ERR_FULOCALTIMER);
      }
      ptm = &tm2;
    }
 #else
    ptm = localtime_r(&tt, &tm2);
    if (ptm == NULL) {
      return(NETWIB_ERR_FULOCALTIMER);
    }
 #endif
  }
#else
  netwib_er(netwib_priv_glovars_other_wrlock());
  ptm = netwib_c_localtime(&tt);
#endif

  plocaltime->nsec = 0;
  plocaltime->sec = ptm->tm_sec;
  plocaltime->min = ptm->tm_min;
  plocaltime->hour = ptm->tm_hour;
  plocaltime->mday = ptm->tm_mday;
  plocaltime->mon = ptm->tm_mon + 1;
  plocaltime->year = ptm->tm_year + 1900;
  plocaltime->wday = ptm->tm_wday;
  plocaltime->yday = ptm->tm_yday + 1;
  plocaltime->zoneoffset = netwib_priv_glovars.time_zoneoffset;

#if NETWIBDEF_HAVEFUNC_LOCALTIME_R == 0
  netwib_er(netwib_priv_glovars_other_wrunlock());
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_localtime(netwib_constlocaltime *plocaltime,
                                       netwib_localtime_encodetype encodetype,
                                       netwib_buf *pbuf)
{
  netwib_conststring pc=NULL;
  netwib_uint32 ui;
  netwib_int32 i;

  switch(encodetype) {
  case NETWIB_LOCALTIME_ENCODETYPE_NOTHING :
    break;
  case NETWIB_LOCALTIME_ENCODETYPE_NSEC_ZERO :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{r010;uint32}", plocaltime->nsec));
    break;
  case NETWIB_LOCALTIME_ENCODETYPE_NSEC_SPACE :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{r 10;uint32}", plocaltime->nsec));
    break;
  case NETWIB_LOCALTIME_ENCODETYPE_SEC_ZERO :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{r02;uint32}", plocaltime->sec));
    break;
  case NETWIB_LOCALTIME_ENCODETYPE_SEC_SPACE :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{r 2;uint32}", plocaltime->sec));
    break;
  case NETWIB_LOCALTIME_ENCODETYPE_MIN_ZERO :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{r02;uint32}", plocaltime->min));
    break;
  case NETWIB_LOCALTIME_ENCODETYPE_MIN_SPACE :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{r_2;uint32}", plocaltime->min));
    break;
  case NETWIB_LOCALTIME_ENCODETYPE_HOUR_ZERO :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{r02;uint32}", plocaltime->hour));
    break;
  case NETWIB_LOCALTIME_ENCODETYPE_HOUR_SPACE :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{r 2;uint32}", plocaltime->hour));
    break;
  case NETWIB_LOCALTIME_ENCODETYPE_MDAY_ZERO :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{r02;uint32}", plocaltime->mday));
    break;
  case NETWIB_LOCALTIME_ENCODETYPE_MDAY_SPACE :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{r 2;uint32}", plocaltime->mday));
    break;
  case NETWIB_LOCALTIME_ENCODETYPE_MON_ZERO :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{r02;uint32}", plocaltime->mon));
    break;
  case NETWIB_LOCALTIME_ENCODETYPE_MON_SPACE :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{r 2;uint32}", plocaltime->mon));
    break;
  case NETWIB_LOCALTIME_ENCODETYPE_MON_SHORT :
    switch(plocaltime->mon) {
    case 1: pc = "Jan"; break;
    case 2: pc = "Feb"; break;
    case 3: pc = "Mar"; break;
    case 4: pc = "Apr"; break;
    case 5: pc = "May"; break;
    case 6: pc = "Jun"; break;
    case 7: pc = "Jul"; break;
    case 8: pc = "Aug"; break;
    case 9: pc = "Sep"; break;
    case 10: pc = "Oct"; break;
    case 11: pc = "Nov"; break;
    case 12: pc = "Dec"; break;
    default: pc = "???"; break;
    }
    netwib_er(netwib_buf_append_string(pc, pbuf));
    break;
  case NETWIB_LOCALTIME_ENCODETYPE_MON_LONG :
    switch(plocaltime->mon) {
    case 1: pc = "January"; break;
    case 2: pc = "February"; break;
    case 3: pc = "March"; break;
    case 4: pc = "April"; break;
    case 5: pc = "May"; break;
    case 6: pc = "June"; break;
    case 7: pc = "July"; break;
    case 8: pc = "August"; break;
    case 9: pc = "September"; break;
    case 10: pc = "October"; break;
    case 11: pc = "November"; break;
    case 12: pc = "December"; break;
    default: pc = "???"; break;
    }
    netwib_er(netwib_buf_append_string(pc, pbuf));
    break;
  case NETWIB_LOCALTIME_ENCODETYPE_YEAR_SHORT :
    ui = plocaltime->year / 100;
    ui = plocaltime->year - ui*100;
    netwib_er(netwib_buf_append_fmt(pbuf, "%{r02;uint32}", ui));
    break;
  case NETWIB_LOCALTIME_ENCODETYPE_YEAR_LONG :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{r04;uint32}", plocaltime->year));
    break;
  case NETWIB_LOCALTIME_ENCODETYPE_WDAY_SHORT :
    switch(plocaltime->wday) {
    case 0: pc = "Sun"; break;
    case 1: pc = "Mon"; break;
    case 2: pc = "Tue"; break;
    case 3: pc = "Wed"; break;
    case 4: pc = "Thu"; break;
    case 5: pc = "Fri"; break;
    case 6: pc = "Sat"; break;
    default: pc = "???"; break;
    }
    netwib_er(netwib_buf_append_string(pc, pbuf));
    break;
  case NETWIB_LOCALTIME_ENCODETYPE_WDAY_LONG :
    switch(plocaltime->wday) {
    case 0: pc = "Sunday"; break;
    case 1: pc = "Monday"; break;
    case 2: pc = "Tuesday"; break;
    case 3: pc = "Wednesday"; break;
    case 4: pc = "Thursday"; break;
    case 5: pc = "Friday"; break;
    case 6: pc = "Saturday"; break;
    default: pc = "???"; break;
    }
    netwib_er(netwib_buf_append_string(pc, pbuf));
    break;
  case NETWIB_LOCALTIME_ENCODETYPE_YDAY_ZERO :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{r03;uint32}", plocaltime->yday));
    break;
  case NETWIB_LOCALTIME_ENCODETYPE_YDAY_SPACE :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{r 3;uint32}", plocaltime->yday));
    break;
  case NETWIB_LOCALTIME_ENCODETYPE_ZONE_SEC :
    netwib_er(netwib_buf_append_fmt(pbuf, "%{int32:+}",
                                    plocaltime->zoneoffset));
    break;
  case NETWIB_LOCALTIME_ENCODETYPE_ZONE_GMT :
    i = plocaltime->zoneoffset / 3600;
    netwib_er(netwib_buf_append_string("GMT", pbuf));
    if (i != 0) {
      netwib_er(netwib_buf_append_fmt(pbuf, "%{int32:+}", i));
    }
    break;
  case NETWIB_LOCALTIME_ENCODETYPE_ZONE_HM :
    ui = (netwib_uint32) plocaltime->zoneoffset;
    if (plocaltime->zoneoffset >= 0) {
      netwib_er(netwib_buf_append_string("+", pbuf));
    } else {
      netwib_er(netwib_buf_append_string("-", pbuf));
      ui = (netwib_uint32) - plocaltime->zoneoffset;
    }
    netwib_er(netwib_buf_append_fmt(pbuf, "%{r02;uint32}", ui/3600));
    ui = ui - 3600*(ui/3600);
    netwib_er(netwib_buf_append_fmt(pbuf, "%{r02;uint32}", ui/60));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_time_iselapsed(netwib_consttime *pabstime,
                                 netwib_bool *pyes)
{
  netwib_time t;
  netwib_cmp cmp;

  if (pabstime == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }

  if (pabstime == NETWIB_TIME_ZERO) {
    if (pyes != NULL) *pyes = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  if (pabstime == NETWIB_TIME_INFINITE) {
    if (pyes != NULL) *pyes = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_priv_time_init_now(&t.sec, &t.nsec));
  netwib_er(netwib_time_cmp(pabstime, &t, &cmp));
  if (pyes != NULL) {
    *pyes = (cmp==NETWIB_CMP_GT)?NETWIB_FALSE:NETWIB_TRUE;
  }

  return(NETWIB_ERR_OK);
}
