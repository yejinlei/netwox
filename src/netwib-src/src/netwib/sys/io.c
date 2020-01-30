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
netwib_err netwib_io_plug(netwib_io *pio,
                          netwib_io_waytype typeofplug,
                          netwib_io *piowheretoplug)
{
  netwib_io *plastio;

  /* parameter verification */
  if (pio == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_io(pio));
  netwib_er(netwib_debug_leak_valid_io(piowheretoplug));
#endif

  switch(typeofplug) {
    case NETWIB_IO_WAYTYPE_READ :
      netwib_er(netwib_priv_io_last(pio, NETWIB_IO_WAYTYPE_READ, &plastio));
      plastio->rd.pnext = piowheretoplug;
      piowheretoplug->rd.numusers++;
      break;
    case NETWIB_IO_WAYTYPE_WRITE :
      netwib_er(netwib_priv_io_last(pio, NETWIB_IO_WAYTYPE_WRITE, &plastio));
      plastio->wr.pnext = piowheretoplug;
      piowheretoplug->wr.numusers++;
      break;
    case NETWIB_IO_WAYTYPE_RDWR :
      netwib_er(netwib_priv_io_last(pio, NETWIB_IO_WAYTYPE_READ, &plastio));
      plastio->rd.pnext = piowheretoplug;
      piowheretoplug->rd.numusers++;
      netwib_er(netwib_priv_io_last(pio, NETWIB_IO_WAYTYPE_WRITE, &plastio));
      plastio->wr.pnext = piowheretoplug;
      piowheretoplug->wr.numusers++;
      break;
    case NETWIB_IO_WAYTYPE_SUPPORTED :
      if (pio->rd.supported && piowheretoplug->rd.supported) {
        netwib_er(netwib_priv_io_last(pio, NETWIB_IO_WAYTYPE_READ, &plastio));
        plastio->rd.pnext = piowheretoplug;
        piowheretoplug->rd.numusers++;
      }
      if (pio->wr.supported && piowheretoplug->wr.supported) {
        netwib_er(netwib_priv_io_last(pio, NETWIB_IO_WAYTYPE_WRITE, &plastio));
        plastio->wr.pnext = piowheretoplug;
        piowheretoplug->wr.numusers++;
      }
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_unplug_next(netwib_io *pio,
                                 netwib_io_waytype typeofplug,
                                 netwib_io **ppnextio)
{

  /* parameter verification */
  if (pio == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_io(pio));
#endif

  switch(typeofplug) {
    case NETWIB_IO_WAYTYPE_READ :
      if (ppnextio != NULL) *ppnextio = pio->rd.pnext;
      if (pio->rd.pnext != NULL) {
        pio->rd.pnext->rd.numusers--;
        pio->rd.pnext = NULL;
      }
      break;
    case NETWIB_IO_WAYTYPE_WRITE :
      if (ppnextio != NULL) *ppnextio = pio->wr.pnext;
      if (pio->wr.pnext != NULL) {
        pio->wr.pnext->wr.numusers--;
        pio->wr.pnext = NULL;
      }
      break;
    case NETWIB_IO_WAYTYPE_RDWR :
      if (ppnextio != NULL) {
        if (pio->rd.pnext != pio->wr.pnext) {
          return(NETWIB_ERR_LOOBJRDWRCONFLICT);
        }
        *ppnextio = pio->rd.pnext;
      }
      if (pio->rd.pnext != NULL) {
        pio->rd.pnext->rd.numusers--;
        pio->rd.pnext = NULL;
      }
      if (pio->wr.pnext != NULL) {
        pio->wr.pnext->wr.numusers--;
        pio->wr.pnext = NULL;
      }
      break;
    case NETWIB_IO_WAYTYPE_SUPPORTED :
      if (ppnextio != NULL) {
        if (pio->rd.supported && pio->wr.supported) {
          if (pio->rd.pnext != pio->wr.pnext) {
            return(NETWIB_ERR_LOOBJRDWRCONFLICT);
          }
          *ppnextio = pio->rd.pnext;
        } else if (pio->rd.supported) {
          *ppnextio = pio->rd.pnext;
        } else if (pio->wr.supported) {
          *ppnextio = pio->wr.pnext;
        } else {
          *ppnextio = NULL;
        }
      }
      if (pio->rd.pnext != NULL) {
        pio->rd.pnext->rd.numusers--;
        pio->rd.pnext = NULL;
      }
      if (pio->wr.pnext != NULL) {
        pio->wr.pnext->wr.numusers--;
        pio->wr.pnext = NULL;
      }
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_unplug_before(netwib_io *pio,
                                   netwib_io_waytype typeofplug,
                                   netwib_io *psearchedio)
{
  netwib_io *pprevrdio, *pprevwrio;

  /* parameter verification */
  if (pio == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_io(pio));
  netwib_er(netwib_debug_leak_valid_io(psearchedio));
#endif

  switch(typeofplug) {
    case NETWIB_IO_WAYTYPE_READ :
      netwib_er(netwib_priv_io_search(pio, NETWIB_IO_WAYTYPE_READ, psearchedio,
                                      &pprevrdio));
      if (pprevrdio != NULL) {
        pprevrdio->rd.pnext = NULL;
      }
      psearchedio->rd.numusers--;
      break;
    case NETWIB_IO_WAYTYPE_WRITE :
      netwib_er(netwib_priv_io_search(pio, NETWIB_IO_WAYTYPE_WRITE,
                                      psearchedio, &pprevwrio));
      if (pprevwrio != NULL) {
        pprevwrio->wr.pnext = NULL;
      }
      psearchedio->wr.numusers--;
      break;
    case NETWIB_IO_WAYTYPE_RDWR :
      netwib_er(netwib_priv_io_search(pio, NETWIB_IO_WAYTYPE_READ, psearchedio,
                                    &pprevrdio));
      netwib_er(netwib_priv_io_search(pio, NETWIB_IO_WAYTYPE_WRITE,
                                      psearchedio, &pprevwrio));
      if (pprevrdio != NULL) {
        pprevrdio->rd.pnext = NULL;
      }
      if (pprevwrio != NULL) {
        pprevwrio->wr.pnext = NULL;
      }
      psearchedio->rd.numusers--;
      psearchedio->wr.numusers--;
      break;
    case NETWIB_IO_WAYTYPE_SUPPORTED :
      /* we do the search before to eventually return error NOTFOUND */
      pprevrdio = NULL;
      if (pio->rd.supported) {
        netwib_er(netwib_priv_io_search(pio, NETWIB_IO_WAYTYPE_READ,
                                        psearchedio, &pprevrdio));
      }
      pprevwrio = NULL;
      if (pio->wr.supported) {
        netwib_er(netwib_priv_io_search(pio, NETWIB_IO_WAYTYPE_WRITE,
                                        psearchedio, &pprevwrio));
      }
      if (pio->rd.supported) {
        if (pprevrdio != NULL) {
          pprevrdio->rd.pnext = NULL;
        }
        psearchedio->rd.numusers--;
      }
      if (pio->wr.supported) {
        if (pprevwrio != NULL) {
          pprevwrio->wr.pnext = NULL;
        }
        psearchedio->wr.numusers--;
      }
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_unplug_after(netwib_io *pio,
                                  netwib_io_waytype typeofplug,
                                  netwib_io *psearchedio,
                                  netwib_io **ppafterio)
{

  /* parameter verification */
  if (pio == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_io(pio));
  netwib_er(netwib_debug_leak_valid_io(psearchedio));
#endif

  switch(typeofplug) {
    case NETWIB_IO_WAYTYPE_READ :
      netwib_er(netwib_priv_io_search(pio, NETWIB_IO_WAYTYPE_READ, psearchedio,
                                      NULL));
      if (ppafterio != NULL) *ppafterio = psearchedio->rd.pnext;
      if (psearchedio->rd.pnext != NULL) {
        psearchedio->rd.pnext->rd.numusers--;
        psearchedio->rd.pnext = NULL;
      }
      break;
    case NETWIB_IO_WAYTYPE_WRITE :
      netwib_er(netwib_priv_io_search(pio, NETWIB_IO_WAYTYPE_WRITE,
                                      psearchedio, NULL));
      if (ppafterio != NULL) *ppafterio = psearchedio->wr.pnext;
      if (psearchedio->wr.pnext != NULL) {
        psearchedio->wr.pnext->wr.numusers--;
        psearchedio->wr.pnext = NULL;
      }
      break;
    case NETWIB_IO_WAYTYPE_RDWR :
      netwib_er(netwib_priv_io_search(pio, NETWIB_IO_WAYTYPE_READ, psearchedio,
                                      NULL));
      netwib_er(netwib_priv_io_search(pio, NETWIB_IO_WAYTYPE_WRITE,
                                      psearchedio, NULL));
      if (ppafterio != NULL) {
        if (psearchedio->rd.pnext != psearchedio->wr.pnext ) {
          return(NETWIB_ERR_LOOBJRDWRCONFLICT);
        }
        *ppafterio = psearchedio->rd.pnext;
      }
      if (psearchedio->rd.pnext != NULL) {
        psearchedio->rd.pnext->rd.numusers--;
        psearchedio->rd.pnext = NULL;
      }
      if (psearchedio->wr.pnext != NULL) {
        psearchedio->wr.pnext->wr.numusers--;
        psearchedio->wr.pnext = NULL;
      }
      break;
    case NETWIB_IO_WAYTYPE_SUPPORTED :
      /* we do the search before to eventually return error NOTFOUND */
      if (pio->rd.supported) {
        netwib_er(netwib_priv_io_search(pio, NETWIB_IO_WAYTYPE_READ,
                                        psearchedio, NULL));
      }
      if (pio->wr.supported) {
        netwib_er(netwib_priv_io_search(pio, NETWIB_IO_WAYTYPE_WRITE,
                                        psearchedio, NULL));
      }
      if (ppafterio != NULL) {
        if (pio->rd.supported && pio->wr.supported) {
          if (psearchedio->rd.pnext != psearchedio->wr.pnext ) {
            return(NETWIB_ERR_LOOBJRDWRCONFLICT);
          }
          *ppafterio = psearchedio->rd.pnext;
        } else if (pio->rd.supported) {
          *ppafterio = psearchedio->rd.pnext;
        } else if (pio->wr.supported) {
          *ppafterio = psearchedio->wr.pnext;
        } else {
          *ppafterio = NULL;
        }
      }
      if (pio->rd.supported && psearchedio->rd.pnext != NULL) {
        psearchedio->rd.pnext->rd.numusers--;
        psearchedio->rd.pnext = NULL;
      }
      if (pio->wr.supported && psearchedio->wr.pnext != NULL) {
        psearchedio->wr.pnext->wr.numusers--;
        psearchedio->wr.pnext = NULL;
      }
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_next(netwib_io *pio,
                          netwib_io_waytype typetofollow,
                          netwib_io **ppionext)
{

  /* parameter verification */
  if (pio == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_io(pio));
#endif

  switch(typetofollow) {
    case NETWIB_IO_WAYTYPE_READ :
      if (pio->rd.pnext == NULL) {
        return(NETWIB_ERR_DATAEND);
      }
      if (ppionext != NULL) *ppionext = pio->rd.pnext;
      break;
    case NETWIB_IO_WAYTYPE_WRITE :
      if (pio->wr.pnext == NULL) {
        return(NETWIB_ERR_DATAEND);
      }
      if (ppionext != NULL) *ppionext = pio->wr.pnext;
      break;
    case NETWIB_IO_WAYTYPE_RDWR :
      if (pio->rd.pnext == pio->wr.pnext) {
        if (pio->rd.pnext == NULL) {
          return(NETWIB_ERR_DATAEND);
        }
        if (ppionext != NULL) *ppionext = pio->rd.pnext;
      } else {
        /* don't know how to choose */
        return(NETWIB_ERR_LOOBJRDWRCONFLICT);
      }
      break;
    case NETWIB_IO_WAYTYPE_SUPPORTED :
      if (pio->rd.supported && pio->wr.supported &&
          pio->rd.pnext != NULL && pio->wr.pnext != NULL) {
        if (pio->rd.pnext == pio->wr.pnext) {
          if (ppionext != NULL) *ppionext = pio->rd.pnext;
        } else {
          return(NETWIB_ERR_LOOBJRDWRCONFLICT);
        }
      } else if (pio->rd.supported && pio->rd.pnext != NULL) {
        if (ppionext != NULL) *ppionext = pio->rd.pnext;
      } else if (pio->wr.supported && pio->wr.pnext != NULL) {
        if (ppionext != NULL) *ppionext = pio->wr.pnext;
      } else {
        return(NETWIB_ERR_DATAEND);
      }
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_write(netwib_io *pio,
                           netwib_constbuf *pbuf)
{
  netwib_io *pcurrentio;
  netwib_err ret;

  /* parameter verification */
  if (pio == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_io(pio));
#endif

  /* main loop */
  pcurrentio = pio;
  while (NETWIB_TRUE) {
    /* treat current io */
    if (!pcurrentio->wr.supported) {
      return(NETWIB_ERR_LOOBJWRITENOTSUPPORTED);
    }
    if (pcurrentio->pfwrite != NULL) {
      /* write to current io */
      ret = (*pcurrentio->pfwrite)(pcurrentio, pbuf);
      if (ret == NETWIB_ERR_OK) {
        /* we can exit now */
        return(NETWIB_ERR_OK);
      } else if (ret == NETWIB_ERR_PLEASETRYNEXT) {
        /* continue to loop */
      } else {
        /* severe error */
        return(ret);
      }
    }
    /* skip io */
    if (pcurrentio->wr.pnext != NULL) {
      pcurrentio = pcurrentio->wr.pnext;
    } else {
      /* end of chain */
      break;
    }
  }

  /* if we are here, the job could not be done */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_read(netwib_io *pio,
                          netwib_buf *pbuf)
{
  netwib_io *pcurrentio;
  netwib_err ret;

  /* parameter verification */
  if (pio == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_io(pio));
#endif

  /* main loop */
  pcurrentio = pio;
  while (NETWIB_TRUE) {
    /* treat current io */
    if (!pcurrentio->rd.supported) {
      return(NETWIB_ERR_LOOBJREADNOTSUPPORTED);
    }
    if (pcurrentio->pfread != NULL) {
      /* read from current io */
      ret = (*pcurrentio->pfread)(pcurrentio, pbuf);
      if (ret == NETWIB_ERR_OK) {
        /* we can exit now */
        return(NETWIB_ERR_OK);
      } else if (ret == NETWIB_ERR_PLEASETRYNEXT) {
        /* continue to loop */
      } else {
        /* severe error */
        return(ret);
      }
    }
    /* skip io */
    if (pcurrentio->rd.pnext != NULL) {
      pcurrentio = pcurrentio->rd.pnext;
    } else {
      /* end of chain */
      break;
    }
  }

  /* if we are here, the job could not be done */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_unread(netwib_io *pio,
                            netwib_constbuf *pbuf)
{
  netwib_io *pcurrentio;
  netwib_err ret;

  /* parameter verification */
  if (pio == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_io(pio));
#endif

  /* main loop */
  pcurrentio = pio;
  while (NETWIB_TRUE) {
    /* treat current io */
    if (!pcurrentio->rd.supported) {
      return(NETWIB_ERR_LOOBJREADNOTSUPPORTED);
    }
    if (pcurrentio->pfunread != NULL) {
      /* read from current io */
      ret = (*pcurrentio->pfunread)(pcurrentio, pbuf);
      if (ret == NETWIB_ERR_OK) {
        /* we can exit now */
        return(NETWIB_ERR_OK);
      } else if (ret == NETWIB_ERR_PLEASETRYNEXT) {
        /* continue to loop */
      } else {
        /* severe error */
        return(ret);
      }
    }
    /* skip io */
    if (pcurrentio->rd.pnext != NULL) {
      pcurrentio = pcurrentio->rd.pnext;
    } else {
      /* end of chain */
      break;
    }
  }

  /* if we are here, the job could not be done */
  return(NETWIB_ERR_PLEASETRYNEXT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_wait_construct(netwib_io *pio,
                                                netwib_io_waytype way,
                                                netwib_consttime *pabstime,
                                                netwib_bool *pevent)
{
  netwib_wait *pwaitread, *pwaitwrite;
  netwib_bool eventread, evenetwrite;
  netwib_err ret;

  switch(way) {
    case NETWIB_IO_WAYTYPE_SUPPORTED :
      if (pio->rd.supported && pio->wr.supported) {
        ret = (*pio->pfwait)(pio, NETWIB_IO_WAYTYPE_RDWR, pabstime, pevent);
        if (ret == NETWIB_ERR_PLEASECONSTRUCT) {
          /* skip in _RDWR */
        } else {
          return(ret);
        }
      } else if (pio->rd.supported) {
        return((*pio->pfwait)(pio, NETWIB_IO_WAYTYPE_READ, pabstime, pevent));
      } else if (pio->wr.supported) {
        return((*pio->pfwait)(pio, NETWIB_IO_WAYTYPE_WRITE, pabstime, pevent));
      } else {
        *pevent = NETWIB_FALSE;
        return(NETWIB_ERR_OK);
      }
      /* no break */
    case NETWIB_IO_WAYTYPE_RDWR :
      netwib_er(netwib_wait_init_io_read(pio, &pwaitread));
      netwib_er(netwib_wait_init_io_write(pio, &pwaitwrite));
      netwib_er(netwib_wait_wait2(pwaitread, pwaitwrite, pabstime,
                                  &eventread, &evenetwrite));
      netwib_er(netwib_wait_close(&pwaitwrite));
      netwib_er(netwib_wait_close(&pwaitread));
      if (eventread || evenetwrite) {
        *pevent = NETWIB_TRUE;
      } else {
        *pevent = NETWIB_FALSE;
      }
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_wait2(netwib_io *pio,
                                       netwib_io_waytype way,
                                       netwib_consttime *pabstime,
                                       netwib_bool *pevent)
{
  netwib_uint32 numcalls;
  netwib_bool elapsed;
  netwib_err ret;

  if (pabstime == NETWIB_TIME_ZERO) {
    ret = (*pio->pfwait)(pio, way, NETWIB_TIME_ZERO, pevent);
    if (ret == NETWIB_ERR_PLEASECONSTRUCT) {
      ret = netwib_priv_io_wait_construct(pio, way, NETWIB_TIME_ZERO, pevent);
    }
    if (ret == NETWIB_ERR_PLEASELOOPTIME) {
      *pevent = NETWIB_FALSE;
      ret = NETWIB_ERR_OK;
    }
    return(ret);
  }

  if (pabstime == NETWIB_TIME_INFINITE) {
    numcalls = 0;
    while (NETWIB_TRUE) {
      ret = (*pio->pfwait)(pio, way, NETWIB_TIME_INFINITE, pevent);
      if (ret == NETWIB_ERR_PLEASECONSTRUCT) {
        ret = netwib_priv_io_wait_construct(pio, way, NETWIB_TIME_INFINITE,
                                            pevent);
      }
      if (ret != NETWIB_ERR_PLEASELOOPTIME) {
        /* NETWIB_ERR_OK, NETWIB_ERR_PLEASETRYNEXT or severe error */
        return(ret);
      }
      netwib_er(netwib_priv_pause2(&numcalls));
    }
    return(NETWIB_ERR_OK);
  }

  numcalls = 0;
  while (NETWIB_TRUE) {
    /* check time */
    netwib_er(netwib_time_iselapsed(pabstime, &elapsed));
    if (elapsed) {
      *pevent = NETWIB_FALSE;
      return(NETWIB_ERR_OK);
    }
    /* wait */
    ret = (*pio->pfwait)(pio, way, pabstime, pevent);
    if (ret == NETWIB_ERR_PLEASECONSTRUCT) {
      ret = netwib_priv_io_wait_construct(pio, way, pabstime, pevent);
    }
    if (ret != NETWIB_ERR_PLEASELOOPTIME) {
      /* NETWIB_ERR_OK, NETWIB_ERR_PLEASETRYNEXT or severe error */
      return(ret);
    }
    /* small pause */
    netwib_er(netwib_priv_pause2(&numcalls));
  }

  return(NETWIB_ERR_LOINTERNALERROR);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_wait(netwib_io *pio,
                          netwib_io_waytype way,
                          netwib_consttime *pabstime,
                          netwib_bool *pevent)
{
  netwib_io *pcurrentio;
  netwib_bool event, *peventtouse;
  netwib_err ret;

  /* parameter verification */
  if (pio == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_io(pio));
#endif

  /* main loop */
  peventtouse = (pevent==NULL)?&event:pevent;
  pcurrentio = pio;
  while (NETWIB_TRUE) {
    /* treat current io */
    netwib_er(netwib_priv_io_supported(pcurrentio, way));
    if (pcurrentio->pfwait != NULL) {
      /* wait on current io */
      ret = netwib_priv_io_wait2(pcurrentio, way, pabstime, peventtouse);
      if (ret != NETWIB_ERR_PLEASETRYNEXT) {
        /* severe error or OK */
        return(ret);
      }
    }
    /* skip io */
    ret = netwib_io_next(pcurrentio, way, &pcurrentio);
    if (ret == NETWIB_ERR_DATAEND) {
      /* end of chain */
      return(NETWIB_ERR_PLEASETRYNEXT);
    } else if (ret != NETWIB_ERR_OK) {
      /* severe error */
      return(ret);
    }
  }

  return(NETWIB_ERR_LOINTERNALERROR);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_ctl_set_current(netwib_io *pio,
                                                 netwib_io_waytype way,
                                                 netwib_io_ctltype ctltype,
                                                 netwib_ptr p,
                                                 netwib_uint32 ui)
{

  switch(ctltype) {
    case NETWIB_IO_CTLTYPE_SUPPORT :
      switch(way) {
        case NETWIB_IO_WAYTYPE_READ :
          pio->rd.supported = (netwib_bool)ui;
          break;
        case NETWIB_IO_WAYTYPE_WRITE :
          pio->wr.supported = (netwib_bool)ui;
          break;
        case NETWIB_IO_WAYTYPE_RDWR :
          pio->rd.supported = (netwib_bool)ui;
          pio->wr.supported = (netwib_bool)ui;
          break;
        case NETWIB_IO_WAYTYPE_SUPPORTED :
          if (pio->rd.supported) pio->rd.supported = (netwib_bool)ui;
          if (pio->wr.supported) pio->wr.supported = (netwib_bool)ui;
          break;
        default :
          return(NETWIB_ERR_PAINVALIDTYPE);
      }
      break;
    case NETWIB_IO_CTLTYPE_NUMUSERS :
      switch(way) {
        case NETWIB_IO_WAYTYPE_READ :
          pio->rd.numusers = ui;
          break;
        case NETWIB_IO_WAYTYPE_WRITE :
          pio->wr.numusers = ui;
          break;
        case NETWIB_IO_WAYTYPE_RDWR :
          pio->rd.numusers = ui;
          pio->wr.numusers = ui;
          break;
        case NETWIB_IO_WAYTYPE_SUPPORTED :
          if (pio->rd.supported) pio->rd.numusers = ui;
          if (pio->wr.supported) pio->wr.numusers = ui;
          break;
        default :
          return(NETWIB_ERR_PAINVALIDTYPE);
      }
      break;
    case NETWIB_IO_CTLTYPE_NUMUSERSINC :
      switch(way) {
        case NETWIB_IO_WAYTYPE_READ :
          pio->rd.numusers += ui;
          break;
        case NETWIB_IO_WAYTYPE_WRITE :
          pio->wr.numusers += ui;
          break;
        case NETWIB_IO_WAYTYPE_RDWR :
          pio->rd.numusers += ui;
          pio->wr.numusers += ui;
          break;
        case NETWIB_IO_WAYTYPE_SUPPORTED :
          if (pio->rd.supported) pio->rd.numusers += ui;
          if (pio->wr.supported) pio->wr.numusers += ui;
          break;
        default :
          return(NETWIB_ERR_PAINVALIDTYPE);
      }
      break;
    default :
      return(NETWIB_ERR_PLEASETRYNEXT);
  }

  p = p; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_ctl_set_construct(netwib_io *pio,
                                                   netwib_io_waytype way,
                                                   netwib_io_ctltype ctltype,
                                                   netwib_ptr p,
                                                   netwib_uint32 ui)
{
  netwib_err ret1, ret2;

  switch(way) {
    case NETWIB_IO_WAYTYPE_SUPPORTED :
      if (pio->rd.supported && pio->wr.supported) {
        ret1 = (*pio->pfctl_set)(pio, NETWIB_IO_WAYTYPE_RDWR, ctltype, p, ui);
        if (ret1 == NETWIB_ERR_PLEASECONSTRUCT) {
          /* skip in _RDWR */
        } else {
          return(ret1);
        }
      } else if (pio->rd.supported) {
        return((*pio->pfctl_set)(pio, NETWIB_IO_WAYTYPE_READ, ctltype, p, ui));
      } else if (pio->wr.supported) {
        return((*pio->pfctl_set)(pio, NETWIB_IO_WAYTYPE_WRITE,
                                 ctltype, p, ui));
      } else {
        return(NETWIB_ERR_OK);
      }
      /* no break */
    case NETWIB_IO_WAYTYPE_RDWR :
      ret1 = (*pio->pfctl_set)(pio, NETWIB_IO_WAYTYPE_READ, ctltype, p, ui);
      ret2 = (*pio->pfctl_set)(pio, NETWIB_IO_WAYTYPE_WRITE, ctltype, p, ui);
      if (ret1 == NETWIB_ERR_OK) return(ret2);
      if (ret2 == NETWIB_ERR_OK) return(ret1);
      if (ret1 == NETWIB_ERR_PLEASETRYNEXT) return(ret2);
      if (ret2 == NETWIB_ERR_PLEASETRYNEXT) return(ret1);
      if (ret1 == ret2) return(ret1);
      return(NETWIB_ERR_LOOBJRDWRCONFLICT);
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_ctl_set(netwib_io *pio,
                             netwib_io_waytype way,
                             netwib_io_ctltype ctltype,
                             netwib_ptr p,
                             netwib_uint32 ui)
{
  netwib_io *pcurrentio;
  netwib_err ret;

  /* parameter verification */
  if (pio == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_io(pio));
#endif

  /* first, try generic types */
  ret = netwib_priv_io_ctl_set_current(pio, way, ctltype, p, ui);
  if (ret != NETWIB_ERR_PLEASETRYNEXT) {
    /* OK(done) or severe error */
    return(ret);
  }

  /* main loop */
  pcurrentio = pio;
  while (NETWIB_TRUE) {
    /* treat current io */
    if (pcurrentio->pfctl_set != NULL) {
      /* read from current io */
      ret = (*pcurrentio->pfctl_set)(pcurrentio, way, ctltype, p, ui);
      if (ret == NETWIB_ERR_PLEASECONSTRUCT) {
        ret = netwib_priv_io_ctl_set_construct(pcurrentio, way, ctltype, p, ui);
      }
      if (ret != NETWIB_ERR_PLEASETRYNEXT) {
        /* OK or severe error */
        return(ret);
      }
    }
    /* skip io */
    ret = netwib_io_next(pcurrentio, way, &pcurrentio);
    if (ret == NETWIB_ERR_DATAEND) {
      /* end of chain */
      return(NETWIB_ERR_PLEASETRYNEXT);
    } else if (ret != NETWIB_ERR_OK) {
      /* severe error */
      return(ret);
    }
  }

  return(NETWIB_ERR_LOINTERNALERROR);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_ctl_get_current(netwib_io *pio,
                                                 netwib_io_waytype way,
                                                 netwib_io_ctltype ctltype,
                                                 netwib_ptr p,
                                                 netwib_uint32 *pui)
{
  netwib_uint32 ui = 0;

  switch(ctltype) {
    case NETWIB_IO_CTLTYPE_SUPPORT :
      switch(way) {
        case NETWIB_IO_WAYTYPE_READ :
          ui = pio->rd.supported;
          break;
        case NETWIB_IO_WAYTYPE_WRITE :
          ui = pio->wr.supported;
          break;
        case NETWIB_IO_WAYTYPE_RDWR :
          if (pio->rd.supported != pio->wr.supported) {
            return(NETWIB_ERR_LOOBJRDWRCONFLICT);
          }
          ui = pio->rd.supported;
          break;
        case NETWIB_IO_WAYTYPE_SUPPORTED :
          ui = NETWIB_TRUE;
          break;
        default :
          return(NETWIB_ERR_PAINVALIDTYPE);
      }
      break;
    case NETWIB_IO_CTLTYPE_NUMUSERS :
      switch(way) {
        case NETWIB_IO_WAYTYPE_READ :
          ui = pio->rd.numusers;
          break;
        case NETWIB_IO_WAYTYPE_WRITE :
          ui = pio->wr.numusers;
          break;
        case NETWIB_IO_WAYTYPE_RDWR :
          if (pio->rd.numusers != pio->wr.numusers) {
            return(NETWIB_ERR_LOOBJRDWRCONFLICT);
          }
          ui = pio->rd.numusers;
          break;
        case NETWIB_IO_WAYTYPE_SUPPORTED :
          if (pio->rd.supported && pio->wr.supported) {
            if (pio->rd.numusers != pio->wr.numusers) {
              return(NETWIB_ERR_LOOBJRDWRCONFLICT);
            }
            ui = pio->rd.numusers;
          } else if (pio->rd.supported) {
            ui = pio->rd.numusers;
          } else if (pio->wr.supported) {
            ui = pio->wr.numusers;
          } else {
            ui = 0;
          }
          break;
        default :
          return(NETWIB_ERR_PAINVALIDTYPE);
      }
      break;
    case NETWIB_IO_CTLTYPE_NUMUSERSINC :
      return(NETWIB_ERR_PAINVALIDTYPE);
    default :
      return(NETWIB_ERR_PLEASETRYNEXT);
  }

  if (pui != NULL) *pui = ui;

  p = p; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_ctl_get_construct(netwib_io *pio,
                                                   netwib_io_waytype way,
                                                   netwib_io_ctltype ctltype,
                                                   netwib_ptr p,
                                                   netwib_uint32 *pui)
{
  netwib_uint32 ui1=0, ui2=0;
  netwib_err ret1, ret2;

  switch(way) {
    case NETWIB_IO_WAYTYPE_SUPPORTED :
      if (pio->rd.supported && pio->wr.supported) {
        ret1 = (*pio->pfctl_get)(pio, NETWIB_IO_WAYTYPE_RDWR,
                                 ctltype, p, pui);
        if (ret1 == NETWIB_ERR_PLEASECONSTRUCT) {
          /* skip in _RDWR */
        } else {
          return(ret1);
        }
      } else if (pio->rd.supported) {
        return((*pio->pfctl_get)(pio, NETWIB_IO_WAYTYPE_READ,
                                 ctltype, p, pui));
      } else if (pio->wr.supported) {
        return((*pio->pfctl_get)(pio, NETWIB_IO_WAYTYPE_WRITE,
                                 ctltype, p,pui));
      } else {
        return(NETWIB_ERR_OK);
      }
      /* no break */
    case NETWIB_IO_WAYTYPE_RDWR :
      /* unfortunately, we do not know what's done with p, so we cannot
         use it to to check for a conflict */
      ret1 = (*pio->pfctl_get)(pio, NETWIB_IO_WAYTYPE_READ,
                               ctltype, p, &ui1);
      ret2 = (*pio->pfctl_get)(pio, NETWIB_IO_WAYTYPE_WRITE,
                               ctltype, p, &ui2);
      if (ret1 == NETWIB_ERR_OK && ret2 == NETWIB_ERR_OK) {
        if (ui1 != ui2) return(NETWIB_ERR_LOOBJRDWRCONFLICT);
        if (pui != NULL) *pui = ui1;
        return(NETWIB_ERR_OK);
      }
      if (ret1 == NETWIB_ERR_OK) return(ret2);
      if (ret2 == NETWIB_ERR_OK) return(ret1);
      if (ret1 == NETWIB_ERR_PLEASETRYNEXT) return(ret2);
      if (ret2 == NETWIB_ERR_PLEASETRYNEXT) return(ret1);
      if (ret1 == ret2) return(ret1);
      return(NETWIB_ERR_LOOBJRDWRCONFLICT);
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_ctl_get(netwib_io *pio,
                             netwib_io_waytype way,
                             netwib_io_ctltype ctltype,
                             netwib_ptr p,
                             netwib_uint32 *pui)
{
  netwib_io *pcurrentio;
  netwib_err ret;

  /* parameter verification */
  if (pio == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_io(pio));
#endif

  /* first, try generic types */
  ret = netwib_priv_io_ctl_get_current(pio, way, ctltype, p, pui);
  if (ret != NETWIB_ERR_PLEASETRYNEXT) {
    /* OK or severe error */
    return(ret);
  }

  /* main loop */
  pcurrentio = pio;
  while (NETWIB_TRUE) {
    /* treat current io */
    if (pcurrentio->pfctl_get != NULL) {
      /* read from current io */
      ret = (*pcurrentio->pfctl_get)(pcurrentio, way, ctltype, p, pui);
      if (ret == NETWIB_ERR_PLEASECONSTRUCT) {
        ret = netwib_priv_io_ctl_get_construct(pcurrentio, way, ctltype, p, pui);
      }
      if (ret != NETWIB_ERR_PLEASETRYNEXT) {
        /* OK or severe error */
        return(ret);
      }
    }
    /* skip io */
    ret = netwib_io_next(pcurrentio, way, &pcurrentio);
    if (ret == NETWIB_ERR_DATAEND) {
      /* end of chain */
      return(NETWIB_ERR_PLEASETRYNEXT);
    } else if (ret != NETWIB_ERR_OK) {
      /* severe error */
      return(ret);
    }
  }

  return(NETWIB_ERR_LOINTERNALERROR);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_close_current(netwib_io **ppio,
                                               netwib_bool *pclosed)
{
  netwib_io *pio;
  netwib_err ret;

  /* parameter verification */
  if (ppio == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pio = *ppio;

  /* ensure that nobody use it */
  if (pio->rd.numusers || pio->wr.numusers) {
    *pclosed = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  }

  /* call close function */
  if (pio->pfclose != NULL) {
    ret = (*pio->pfclose)(pio);
    if (ret != NETWIB_ERR_OK) {
      return(ret);
    }
  }

  /* there is one less user of next */
  if (pio->rd.pnext != NULL) {
    pio->rd.pnext->rd.numusers--;
  }
  if (pio->wr.pnext != NULL) {
    pio->wr.pnext->wr.numusers--;
  }

  /* free memory */
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_del_io(pio));
#endif
  netwib_er(netwib_ptr_free((netwib_ptr*)&pio));

  *pclosed = NETWIB_TRUE;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_io_close_cmp(netwib_constptr piteminf,
                                           netwib_constptr pitemsup,
                                           netwib_ptr pinfos,
                                           netwib_cmp *pcmp)
{
  if (piteminf == pitemsup) {
    if (pcmp != NULL) *pcmp = NETWIB_CMP_EQ;
  } else {
    if (pcmp != NULL) *pcmp = NETWIB_CMP_LT;
  }

  pinfos = pinfos; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_io_close(netwib_io **ppio)
{
  netwib_io *pio, *pcurrentio;
  netwib_ring *pring;
  netwib_ring_index *pringindex;
  netwib_uint32 count, i;
  netwib_bool closed, ioremoved;
  netwib_err ret;

  /* parameter verification */
  if (ppio == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
  pio = *ppio;
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_io(pio));
#endif

  /* init ring */
  netwib_er(netwib_ring_init(NULL, NULL, &pring));

  /* loop on read */
  pcurrentio = pio;
  while (NETWIB_TRUE) {
    netwib_er(netwib_ring_add_last(pring, pcurrentio));
    if (pcurrentio->rd.pnext == NULL) {
      /* end of chain */
      break;
    }
    pcurrentio = pcurrentio->rd.pnext;
  }

  /* loop on write */
  pcurrentio = pio;
  while (NETWIB_TRUE) {
    netwib_er(netwib_ring_add_last(pring, pcurrentio));
    if (pcurrentio->wr.pnext == NULL) {
      /* end of chain */
      break;
    }
    pcurrentio = pcurrentio->wr.pnext;
  }

  /* remove duplicates */
  netwib_er(netwib_ring_del_duplicate(pring, netwib_priv_io_close_cmp,
                                      NULL, NETWIB_FALSE));

  /* initialize ring index */
  netwib_er(netwib_ring_index_init(pring, &pringindex));

  /* remove every unused io in the ring */
  netwib_er(netwib_ring_ctl_get_count(pring, &count));
  while (count != 0) {
    netwib_er(netwib_ring_index_ctl_set_rewind(pringindex));
    ioremoved = NETWIB_FALSE;
    i = 0;
    while (i < count) {
      ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pcurrentio);
      if (ret == NETWIB_ERR_DATAEND) {
        break;
      } else if (ret != NETWIB_ERR_OK) {
        netwib_er(netwib_ring_index_close(&pringindex));
        netwib_er(netwib_ring_close(&pring, NETWIB_FALSE));
        return(ret);
      }
      ret = netwib_priv_io_close_current(&pcurrentio, &closed);
      if (ret != NETWIB_ERR_OK) {
        netwib_er(netwib_ring_index_close(&pringindex));
        netwib_er(netwib_ring_close(&pring, NETWIB_FALSE));
        return(ret);
      }
      if (closed) {
        netwib_er(netwib_ring_index_this_del(pringindex, NETWIB_FALSE));
        count--;
        ioremoved = NETWIB_TRUE;
      } else {
        i++;
      }
    }
    if (!ioremoved) {
      /* this means the io are used by someone else, so do not remove them */
      break;
    }
  }

  /* determine if passed pointer was effectively removed */
  netwib_er(netwib_ring_index_ctl_set_rewind(pringindex));
  ioremoved = NETWIB_TRUE;
  while (NETWIB_TRUE) {
    ret = netwib_ring_index_next(pringindex, (netwib_ptr*)&pcurrentio);
    if (ret == NETWIB_ERR_DATAEND) {
      break;
    } else if (ret != NETWIB_ERR_OK) {
      netwib_er(netwib_ring_index_close(&pringindex));
      netwib_er(netwib_ring_close(&pring, NETWIB_FALSE));
      return(ret);
    }
    if (pcurrentio == pio) {
      ioremoved = NETWIB_FALSE;
      break;
    }
  }
  if (ioremoved) {
    *ppio = NULL;
  }

  netwib_er(netwib_ring_index_close(&pringindex));
  netwib_er(netwib_ring_close(&pring, NETWIB_FALSE));

  return(NETWIB_ERR_OK);
}

