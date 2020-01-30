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
netwib_err netwox_search_exact(netwib_constbuf *ptext,
                               netwib_constbuf *ppattern,
                               netwib_bool *pfound)
{
  netwib_data pc;

  if (netwib__buf_ref_data_size(ppattern) == 0) {
    *pfound = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  *pfound = NETWIB_FALSE;
  pc = netwib_c_memcasemem(netwib__buf_ref_data_ptr(ptext),
                           netwib__buf_ref_data_size(ptext),
                           netwib__buf_ref_data_ptr(ppattern),
                           netwib__buf_ref_data_size(ppattern));
  if (pc != NULL) {
    *pfound = NETWIB_TRUE;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_search_root(netwib_constbuf *ptext,
                              netwib_constbuf *ppattern,
                              netwib_uint32 percentage,
                              netwib_bool *pfound)
{
  netwib_data pc;
  netwib_uint32 patternsize;

  if (percentage > 100) {
    return(NETWIB_ERR_PATOOHIGH);
  }

  if (netwib__buf_ref_data_size(ppattern) == 0) {
    *pfound = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  *pfound = NETWIB_FALSE;
  patternsize = (percentage * netwib__buf_ref_data_size(ppattern)) / 100;
  pc = netwib_c_memcasemem(netwib__buf_ref_data_ptr(ptext),
                           netwib__buf_ref_data_size(ptext),
                           netwib__buf_ref_data_ptr(ppattern), patternsize);
  if (pc != NULL) {
    *pfound = NETWIB_TRUE;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_search_approx(netwib_constbuf *ptext,
                                netwib_constbuf *ppattern,
                                netwib_uint32 percentage,
                                netwib_bool *pfound)
{
  netwib_data T, P, Aold, Anew, Atmp;
  netwib_uint32 t, p, i, j, allowederrors;

  if (percentage > 100) {
    return(NETWIB_ERR_PATOOHIGH);
  }

  *pfound = NETWIB_FALSE;
  T = netwib__buf_ref_data_ptr(ptext) - 1;
  t = netwib__buf_ref_data_size(ptext);
  P = netwib__buf_ref_data_ptr(ppattern) - 1;
  p = netwib__buf_ref_data_size(ppattern);
  allowederrors = (percentage *p) / 100;

  if (p == 0) {
    *pfound = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  /* allocate 2 columns */
  netwib_er(netwib_ptr_malloc((p+1)*sizeof(netwib_uint32),
                              (netwib_ptr*)&Aold));
  netwib_er(netwib_ptr_malloc((p+1)*sizeof(netwib_uint32),
                              (netwib_ptr*)&Anew));

  /* initialize old column */
  for (j = 0; j <= p; j++) {
    Aold[j] = (netwib_byte)j;
  }

  /* main loop */
  for (i = 0; i < t; i++) {
    /* compute Anew column */
    Anew[0] = 0;
    for (j = 1; j <= p; j++) {
      if (netwib_c2_lc(T[i]) == netwib_c2_lc(P[j])) {
        Anew[j] = Aold[j-1];
      } else {
        Anew[j] = Anew[j-1];
        if (Anew[j] > Aold[j-1]) {
          Anew[j] = Aold[j-1];
        }
        if (Anew[j] > Aold[j]) {
          Anew[j] = Aold[j];
        }
        Anew[j]++;
      }
    }
    /* check end */
    if (Anew[p] <= allowederrors) {
      *pfound = NETWIB_TRUE;
      break;
    }
    /* swap both columns */
    Atmp = Aold;
    Aold = Anew;
    Anew = Atmp;
  }

  /* free columns */
  netwib_er(netwib_ptr_free((netwib_ptr*)&Aold));
  netwib_er(netwib_ptr_free((netwib_ptr*)&Anew));

  return(NETWIB_ERR_OK);
}
