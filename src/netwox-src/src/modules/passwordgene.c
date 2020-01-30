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
netwib_err netwox_passwordgeneword_count(netwox_passwordgeneword words[],
                                         netwib_uint32 *pcount)
{
  netwox_passwordgeneword *pword;
  netwib_uint32 count;

  count = 0;
  pword = words;
  while (*pword != NULL) {
    count++;
    pword++;
  }
  *pcount = count;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_passwordgeneword_choose(netwox_passwordgeneword words[],
                                          netwib_uint32 count,
                                          netwib_buf *pbuf)
{
  netwox_passwordgeneword *pword;
  netwib_uint32 idx;

  netwib_er(netwib_uint32_init_rand(1, count, &idx));

  pword = words;
  while (*pword != NULL && --idx) {
    pword++;
  }

  netwib_er(netwib_buf_append_string(*pword, pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_passwordgeneword_findsize(netwox_passwordgeneword words[],
                                            netwib_data data,
                                            netwib_uint32 datasize,
                                            netwib_uint32 *pfoundsize)
{
  netwox_passwordgeneword *pword;
  netwib_uint32 strlenword;

  *pfoundsize = 0;
  pword = words;
  while (*pword != NULL) {
    strlenword = netwib_c_strlen(*pword);
    if (strlenword <= datasize) {
      if (!netwib_c_memcmp(*pword, data, strlenword)) {
        *pfoundsize = strlenword;
        break;
      }
    }
    pword++;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_passwordgene(netwib_bufpool *pbufpool,
                                           netwib_uint32 numimages,
                                           netwox_passwordgeneword words[],
                                           netwib_uint32 wordscount,
                                           netwib_buf *ppassword,
                                           netwib_buf *pimages)
{
  netwib_buf *pword;
  netwib_data data;
  netwib_uint32 datasize;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pword));

  while(NETWIB_TRUE) {
    /* choose a word */
    netwib_er(netwox_passwordgeneword_choose(words, wordscount, pword));
    data = netwib__buf_ref_data_ptr(pword);
    datasize = netwib__buf_ref_data_size(pword);
    netwib_er(netwib_buf_append_data(data, 2, ppassword));
    netwib_er(netwib_buf_append_byte((netwib_byte)netwib_c2_uc(data[0]), pimages));
    netwib_er(netwib_buf_append_byte((netwib_byte)netwib_c2_uc(data[1]), pimages));
    netwib_er(netwib_buf_append_data(data+2, datasize-2, pimages));
    /* prepare next */
    numimages--;
    if (numimages == 0) {
      break;
    }
    netwib_er(netwib_buf_append_byte(' ', pimages));
    netwib__buf_reinit(pword);
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pword));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_passwordgene_variation_char(netwib_byte *pc,
                                                          netwib_constbuf *pallowedcharvar)
{
  netwib_conststring choices=NULL;
  netwib_data pallowed;
  netwib_uint32 idx;

  switch(*pc) {
  case 'a': choices = "A@4"; break;
  case 'b': choices = "B68"; break;
  case 'c': choices = "C6"; break;
  case 'd': choices = "D"; break;
  case 'e': choices = "E3"; break;
  case 'f': choices = "F"; break;
  case 'g': choices = "G9"; break;
  case 'h': choices = "Hb"; break;
  case 'i': choices = "I1|!)([]{}/"; break;
  case 'j': choices = "J"; break;
  case 'k': choices = "KX"; break;
  case 'l': choices = "L1|!)([]{}/"; break;
  case 'm': choices = "MW"; break;
  case 'n': choices = "Nr"; break;
  case 'o': choices = "O0*"; break;
  case 'p': choices = "PR"; break;
  case 'q': choices = "Q9"; break;
  case 'r': choices = "Rn"; break;
  case 's': choices = "S25$&"; break;
  case 't': choices = "T7"; break;
  case 'u': choices = "UvV"; break;
  case 'v': choices = "VuU^"; break;
  case 'w': choices = "WM"; break;
  case 'x': choices = "X%+"; break;
  case 'y': choices = "Y7"; break;
  case 'z': choices = "Z7"; break;
  default: return(NETWOX_ERR_INTERNALERROR);
  }

  netwib_er(netwib_uint32_init_rand(0, netwib_c_strlen(choices)-1, &idx));

  pallowed = netwib_c_memchr(netwib__buf_ref_data_ptr(pallowedcharvar),
                             choices[idx],
                             netwib__buf_ref_data_size(pallowedcharvar));
  if (pallowed != NULL) {
    *pc = choices[idx];
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_passwordgene_variation(netwib_buf *ppassword,
                                                     netwib_uint32 numvariations,
                                                     netwib_constbuf *pallowedcharvar,
                                                     netwib_buf *pvariation,
                                                     netwib_bool *pvarnotpossible)
{
  netwib_data data;
  netwib_uint32 datasize, idx, numtries, threshold;

  if (netwib__buf_ref_data_size(pallowedcharvar) == 0) {
    *pvarnotpossible = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  *pvarnotpossible = NETWIB_FALSE;
  numtries = 0;
  threshold = 64*numvariations;

  netwib_er(netwib_buf_append_buf(ppassword, pvariation));

  data = netwib__buf_ref_data_ptr(pvariation);
  datasize = netwib__buf_ref_data_size(pvariation);
  while(NETWIB_TRUE) {
    netwib_er(netwib_uint32_init_rand(0, datasize-1, &idx));
    if (data[idx] < 'a' || data[idx] > 'z') {
      /* this char was already changed */
      continue;
    }
    netwib_er(netwox_priv_passwordgene_variation_char(&data[idx], pallowedcharvar));
    if (data[idx] >= 'a' && data[idx] <= 'z') {
      /* this char could not be changed. If threshold exceeded, abort */
      numtries++;
      if (numtries > threshold) {
        *pvarnotpossible = NETWIB_TRUE;
        break;
      }
      continue;
    }
    numvariations--;
    if (numvariations == 0) {
      break;
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_passwordgene_pronounceable(netwib_buf *ppassword,
                                                         netwib_uint32 maxsyllables,
                                                         netwox_passwordgeneword vowel[],
                                                         netwox_passwordgeneword consonantbegin[],
                                                         netwox_passwordgeneword consonantend[],
                                                         netwib_bool *ppronounceable)
{
  netwib_data data;
  netwib_uint32 datasize, state, foundsize, foundsize2, numsyllables;

  /* An efficient algorithm would be too complicated here. We use a 3 state
     simple algo. A pronounceable word is :
       consobegin vowel consoend cs v ce ... cs v ce
       consobegin vowel consoend cs v ce ... cs v
       vowel consoend cs v ce ... cs v ce
       vowel consoend cs v ce ... cs v
     We use a 3 state machine :
       begin -> state1
       state1 and consonantbegin -> state2
       state1 and vowel -> state3
       state1 and other -> not pronounceable
       state2 and vowel -> state3
       state2 and other -> not pronounceable
       state3 and consonantend -> state1
       state3 and consonantbegin -> state2
       state3 and other -> not pronounceable
       must end in state1 or state3
  */

  data = netwib__buf_ref_data_ptr(ppassword);
  datasize = netwib__buf_ref_data_size(ppassword);

  state = 1;
  *ppronounceable = NETWIB_TRUE;
  numsyllables = 0;
  while(datasize) {
    if (state == 1) {
      netwib_er(netwox_passwordgeneword_findsize(consonantbegin, data, datasize,
                                            &foundsize));
      if (foundsize) {
        state = 2;
        data += foundsize;
        datasize -= foundsize;
      } else {
        netwib_er(netwox_passwordgeneword_findsize(vowel, data, datasize,
                                              &foundsize));
        if (foundsize) {
          state = 3;
          data += foundsize;
          datasize -= foundsize;
          numsyllables++;
        } else {
          *ppronounceable = NETWIB_FALSE;
          break;
        }
      }
    } else if (state == 2) {
      netwib_er(netwox_passwordgeneword_findsize(vowel, data, datasize,
                                            &foundsize));
      if (foundsize) {
        state = 3;
        data += foundsize;
        datasize -= foundsize;
        numsyllables++;
      } else {
        *ppronounceable = NETWIB_FALSE;
        break;
      }
    } else if (state == 3) {
      netwib_er(netwox_passwordgeneword_findsize(consonantbegin, data, datasize,
                                            &foundsize));
      netwib_er(netwox_passwordgeneword_findsize(consonantend, data, datasize,
                                            &foundsize2));
      if (foundsize == 0 && foundsize2 == 0) {
        *ppronounceable = NETWIB_FALSE;
        break;
      }
      if (foundsize > foundsize2) {
        state = 2;
        data += foundsize;
        datasize -= foundsize;
      } else {
        state = 1; /* if sizes are equal state1 is prioritary */
        data += foundsize2;
        datasize -= foundsize2;
      }
    } else {
      return(NETWOX_ERR_INTERNALERROR);
    }
  }

  if (*ppronounceable) {
    if (state == 2) { /* forbidden state at end */
      *ppronounceable = NETWIB_FALSE;
    }
  }
  if (*ppronounceable) {
    if (maxsyllables != 0) {
      if (numsyllables > maxsyllables) {
        *ppronounceable = NETWIB_FALSE;
      }
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_passwordgene(netwib_uint32 numpassword,
                               netwib_uint32 numimages,
                               netwib_bool pronounceable,
                               netwib_uint32 maxsyllables,
                               netwox_passwordgeneword words[],
                               netwib_constbuf *pallowedcharvar,
                               netwox_passwordgeneword vowel[],
                               netwox_passwordgeneword consonantbegin[],
                               netwox_passwordgeneword consonantend[])
{
  netwib_bufpool *pbufpool;
  netwib_buf *ppassword, *pimages, *pvariation;
  netwib_uint32 wordscount, i;
  netwib_bool ispronounceable, varnotpossible;

  if (numpassword == 0) {
    return(NETWIB_ERR_PATOOLOW);
  }
  if (numimages == 0) {
    return(NETWIB_ERR_PATOOLOW);
  }

  /* init */
  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &ppassword));
  ppassword->flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pimages));
  pimages->flags |= NETWIB_BUF_FLAGS_SENSITIVE;
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pvariation));
  pvariation->flags |= NETWIB_BUF_FLAGS_SENSITIVE;

  /* count the number of words */
  netwib_er(netwox_passwordgeneword_count(words, &wordscount));

  /* create passwords */
  while(NETWIB_TRUE) {
    /* generate a password */
    netwib__buf_reinit(ppassword);
    netwib__buf_reinit(pimages);
    netwib_er(netwox_priv_passwordgene(pbufpool, numimages, words, wordscount,
                                  ppassword, pimages));
    /* check if pronounceable */
    if (pronounceable) {
      netwib_er(netwox_priv_passwordgene_pronounceable(ppassword, maxsyllables,
                                                  vowel, consonantbegin,
                                                  consonantend,
                                                  &ispronounceable));
      if (!ispronounceable) {
        /* netwib_er(netwib_fmt_display("rejected: %{buf}\n", ppassword)); */
        continue;
      }
    }
    /* it's ok, so display it */
    netwib_er(netwib_fmt_display("password: %{buf}\n", ppassword));
    netwib_er(netwib_fmt_display("images: %{buf}\n", pimages));
    /* display variations */
    for (i = 1; i <= numimages; i++) {
      netwib__buf_reinit(pvariation);
      netwib_er(netwox_priv_passwordgene_variation(ppassword, i, pallowedcharvar,
                                              pvariation, &varnotpossible));
      if (varnotpossible) {
        break;
      }
      netwib_er(netwib_fmt_display("variation%{uint32}: %{buf}\n", i,
                                   pvariation));
    }
    /* prepare for next */
    numpassword--;
    if (numpassword == 0) {
      break;
    }
    netwib_er(netwib_fmt_display("\n"));
  }

  /* close */
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pvariation));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pimages));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &ppassword));
  netwib_er(netwib_bufpool_close(&pbufpool));

  return(NETWIB_ERR_OK);
}
