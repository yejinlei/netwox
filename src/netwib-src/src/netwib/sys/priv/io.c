/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_io_supported(netwib_io *pio,
                                    netwib_io_waytype way)
{
  switch(way) {
    case NETWIB_IO_WAYTYPE_NONE :
      break;
    case NETWIB_IO_WAYTYPE_READ :
      if (!pio->rd.supported) {
        return(NETWIB_ERR_LOOBJREADNOTSUPPORTED);
      }
      break;
    case NETWIB_IO_WAYTYPE_WRITE :
      if (!pio->wr.supported) {
        return(NETWIB_ERR_LOOBJWRITENOTSUPPORTED);
      }
      break;
    case NETWIB_IO_WAYTYPE_RDWR :
      if (!pio->rd.supported) {
        return(NETWIB_ERR_LOOBJREADNOTSUPPORTED);
      }
      if (!pio->wr.supported) {
        return(NETWIB_ERR_LOOBJWRITENOTSUPPORTED);
      }
      break;
    case NETWIB_IO_WAYTYPE_SUPPORTED :
      /* supported is always supported :) */
      break;
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_io_last(netwib_io *pio,
                               netwib_io_waytype way,
                               netwib_io **pplastio)
{
  netwib_io *pcurrentio, *pnextrdio, *pnextwrio;

  /* parameter verification */
  if (pio == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_io(pio));
#endif

  pcurrentio = pio;
  switch(way) {
    case NETWIB_IO_WAYTYPE_READ :
      while (NETWIB_TRUE) {
        pnextrdio = pcurrentio->rd.pnext;
        if (pnextrdio == NULL) {
          /* pcurrentio is the last */
          if (pplastio != NULL) *pplastio = pcurrentio;
          return(NETWIB_ERR_OK);
        }
        pcurrentio = pnextrdio;
      }
      break;
    case NETWIB_IO_WAYTYPE_WRITE :
      while (NETWIB_TRUE) {
        pnextwrio = pcurrentio->wr.pnext;
        if (pnextwrio == NULL) {
          /* pcurrentio is the last */
          if (pplastio != NULL) *pplastio = pcurrentio;
          return(NETWIB_ERR_OK);
        }
        pcurrentio = pnextwrio;
      }
      break;
    case NETWIB_IO_WAYTYPE_RDWR :
      /* ways might diverge, so we have to call two functions */
      netwib_er(netwib_priv_io_last(pio, NETWIB_IO_WAYTYPE_READ, &pnextrdio));
      netwib_er(netwib_priv_io_last(pio, NETWIB_IO_WAYTYPE_WRITE, &pnextwrio));
      if (pnextrdio != pnextwrio) {
        /* don't know how to choose */
        return(NETWIB_ERR_LOOBJRDWRCONFLICT);
      }
      if (pplastio != NULL) *pplastio = pnextrdio;
      break;
    case NETWIB_IO_WAYTYPE_SUPPORTED :
      if (pio->rd.supported && pio->wr.supported) {
        netwib_er(netwib_priv_io_last(pio, NETWIB_IO_WAYTYPE_RDWR, pplastio));
      } else if (pio->rd.supported) {
        netwib_er(netwib_priv_io_last(pio, NETWIB_IO_WAYTYPE_READ, pplastio));
      } else if (pio->wr.supported) {
        netwib_er(netwib_priv_io_last(pio, NETWIB_IO_WAYTYPE_WRITE, pplastio));
      }
    default :
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_io_search(netwib_io *pio,
                                 netwib_io_waytype way,
                                 netwib_io *psearchedio,
                                 netwib_io **ppprevio)
{
  netwib_io *pcurrentio, *pnextio;

  /* parameter verification */
  if (pio == NULL) {
    return(NETWIB_ERR_PANULLPTR);
  }
#if NETWIB_DEBUG_LEAK==1
  netwib_er(netwib_debug_leak_valid_io(pio));
  netwib_er(netwib_debug_leak_valid_io(psearchedio));
#endif

  /* special case */
  if (psearchedio == pio) {
    if (ppprevio != NULL) *ppprevio = NULL;
    return(NETWIB_ERR_OK);
  }

  /* loop */
  pcurrentio = pio;
  while (NETWIB_TRUE) {
    switch(way) {
      case NETWIB_IO_WAYTYPE_READ :
        pnextio = pcurrentio->rd.pnext;
        break;
      case NETWIB_IO_WAYTYPE_WRITE :
        pnextio = pcurrentio->wr.pnext;
        break;
      case NETWIB_IO_WAYTYPE_RDWR :
      case NETWIB_IO_WAYTYPE_SUPPORTED :
        return(NETWIB_ERR_LONOTIMPLEMENTED);
      default :
        return(NETWIB_ERR_PAINVALIDTYPE);
    }
    if (pnextio == NULL) {
      /* pcurrentio is the last */
      return(NETWIB_ERR_NOTFOUND);
    }
    if (pnextio == psearchedio) {
      if (ppprevio != NULL) *ppprevio = pcurrentio;
      return(NETWIB_ERR_OK);
    }
    pcurrentio = pnextio;
  }

  return(NETWIB_ERR_LOINTERNALERROR);
}
