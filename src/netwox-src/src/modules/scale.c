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
netwib_err netwox_scale_init(netwib_uint32 maxvalue,
                             netwox_scale *pscale)
{
  pscale->maxvalue = maxvalue;
  pscale->displayeddots = 0;
  pscale->lastchar = '.';
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_scale_close(netwox_scale *pscale)
{
  netwib_uint32 i;

  for (i = 0 ; i < pscale->displayeddots; i++) {
    netwib_er(netwib_fmt_display("\b \b"));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_scale_update(netwox_scale *pscale,
                               netwib_uint32 value)
{
  netwib_int32 i, dotstoadd;

  if (pscale->maxvalue != 0) {
    value = (value*70)/pscale->maxvalue;
  }
  if (value > 70) {
    value = 70;
  }
  dotstoadd = value - pscale->displayeddots;

  if (dotstoadd > 0) {
    if (pscale->displayeddots) {
      netwib_er(netwib_fmt_display("\b."));
    }
    for (i = 0 ; i < dotstoadd; i++) {
      netwib_er(netwib_fmt_display("."));
    }
    pscale->displayeddots += dotstoadd;
    pscale->lastchar = '.';
  } else if (dotstoadd < 0) {
    for (i = 0 ; i < -dotstoadd; i++) {
      netwib_er(netwib_fmt_display("\b \b"));
    }
    pscale->displayeddots += dotstoadd;
    pscale->lastchar = '.';
  } else {
    if (pscale->displayeddots) {
      switch(pscale->lastchar) {
      case '.' :
        netwib_er(netwib_fmt_display("\b|"));
        pscale->lastchar = '|';
        break;
      case '|' :
        netwib_er(netwib_fmt_display("\b/"));
        pscale->lastchar = '/';
        break;
      case '/' :
        netwib_er(netwib_fmt_display("\b-"));
        pscale->lastchar = '-';
        break;
      case '-' :
        netwib_er(netwib_fmt_display("\b\\"));
        pscale->lastchar = '\\';
        break;
      case '\\' :
        netwib_er(netwib_fmt_display("\b|"));
        pscale->lastchar = '|';
        break;
      default :
        return(NETWOX_ERR_INTERNALERROR);
      }
    }
  }

  return(NETWIB_ERR_OK);
}
