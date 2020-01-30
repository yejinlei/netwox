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
typedef enum {
  NETWOX_SMBSERGLOB_RE_STATE_BEGIN =     1,
  NETWOX_SMBSERGLOB_RE_STATE_END =       2,
  NETWOX_SMBSERGLOB_RE_STATE_BEFOREDOT = 3,
  NETWOX_SMBSERGLOB_RE_STATE_AFTERDOT =  4,
  NETWOX_SMBSERGLOB_RE_STATE_CHAR =      5,
  NETWOX_SMBSERGLOB_RE_STATE_QUESTION =  6,
  NETWOX_SMBSERGLOB_RE_STATE_SUP =       7,
  NETWOX_SMBSERGLOB_RE_STATE_STAR =      8
} netwox_smbserglob_re_state;

/*-------------------------------------------------------------*/
netwib_err netwox_smbserglob_re(netwib_constbuf *pglob,
                                netwib_buf *pre)
{
  netwib_data datain, dataout, dataoutori, pc;
  netwib_uint32 datainsize;
  netwox_smbserglob_re_state state, nextstate;
  netwib_uint32 statequestioncount;
  netwib_char c;
  netwib_bool dotseen, specialcase, seenotherchars;

  datain = netwib__buf_ref_data_ptr(pglob);
  datainsize = netwib__buf_ref_data_size(pglob);

  /* in the worse case, dataoutsize is 4 times greater */
  netwib_er(netwib_buf_wantspace(pre, 4*datainsize+5, &dataout));
  dataoutori = dataout;

  /* create the regular expression:
      > or ? : "." for first
               ".*" for next, until end (or dot for is83)
               "...." for next, until another character
      " or . : "[.]"
      < or * : ".*" for first
               nothing for next
      empty  : ".*"
  */
  state = NETWOX_SMBSERGLOB_RE_STATE_BEGIN;
  statequestioncount = 0;
  c = '\0';
  dotseen = NETWIB_FALSE;
  seenotherchars = NETWIB_FALSE;
  while(NETWIB_TRUE) {
    /* obtain nextstate (current character) */
    nextstate = NETWOX_SMBSERGLOB_RE_STATE_END;
    if (datainsize != 0) {
      nextstate = NETWOX_SMBSERGLOB_RE_STATE_CHAR;
      c = *datain++;
      datainsize--;
      if (c == '.' || c == '"') {
        nextstate = NETWOX_SMBSERGLOB_RE_STATE_BEFOREDOT;
        dotseen = NETWIB_TRUE;
      } else if (c == '*' || c == '<') {
        nextstate = NETWOX_SMBSERGLOB_RE_STATE_STAR;
      } else if (c == '?') {
        nextstate = NETWOX_SMBSERGLOB_RE_STATE_QUESTION;
      } else if (c == '>') {
        nextstate = NETWOX_SMBSERGLOB_RE_STATE_SUP;
      }
    }
    /* a change of state from QUESTION implies to add the dots or .* */
    if ((state == NETWOX_SMBSERGLOB_RE_STATE_QUESTION &&
         nextstate != NETWOX_SMBSERGLOB_RE_STATE_QUESTION) ||
        (state == NETWOX_SMBSERGLOB_RE_STATE_SUP &&
         nextstate != NETWOX_SMBSERGLOB_RE_STATE_SUP)) {
      if (nextstate == NETWOX_SMBSERGLOB_RE_STATE_END ||
          nextstate == NETWOX_SMBSERGLOB_RE_STATE_BEFOREDOT) {
        /* first, try to detect the >>>>>>>> or >>> special case */
        specialcase = NETWIB_FALSE;
        if ((state == NETWOX_SMBSERGLOB_RE_STATE_SUP) && !seenotherchars) {
          if ((statequestioncount == 7) || (nextstate == NETWOX_SMBSERGLOB_RE_STATE_END && statequestioncount == 2)) {
            specialcase = NETWIB_TRUE;
            *dataout++ = '*'; /* there is already the dot of ".*" */
          }
        }
        /* now, do the normal case if the special case was not done */
        if (!specialcase) {
          if (statequestioncount > 9) {
            /* we should add the range, but overflow so this should be ok */
            *dataout++ = '.';
            *dataout++ = '*';
          } else if (statequestioncount) {
            /* add the range */
            *dataout++ = '.';
            *dataout++ = '{';
            *dataout++ = '0';
            *dataout++ = ',';
            *dataout++ = netwib_c2_9toc(statequestioncount);
            *dataout++ = '}';
          }
        }
      } else {
        /* simply add the exact number of dots */
        while (statequestioncount--) {
          *dataout++ = '.';
        }
      }
    }
    /* a change of state from BEGIN or AFTERDOT to END or BEFOREDOT
       implies to add ".*" */
    if ((state == NETWOX_SMBSERGLOB_RE_STATE_BEGIN ||
         state == NETWOX_SMBSERGLOB_RE_STATE_AFTERDOT) &&
        (nextstate == NETWOX_SMBSERGLOB_RE_STATE_END ||
         nextstate == NETWOX_SMBSERGLOB_RE_STATE_BEFOREDOT)) {
      *dataout++ = '.';
      *dataout++ = '*';
    }
    /* check for end of string */
    if (nextstate == NETWOX_SMBSERGLOB_RE_STATE_END) {
      if (!dotseen) {
        *dataout++ = '[';
        *dataout++ = '.';
        *dataout++ = ']';
        *dataout++ = '?';
        *dataout++ = '.';
        *dataout++ = '*';
      }
      break;
    }
    /* manage states */
    switch(nextstate) {
    case NETWOX_SMBSERGLOB_RE_STATE_CHAR :
      seenotherchars = NETWIB_TRUE;
      if (netwib_c2_isdigit(c)) {
        *dataout++ = c;
      } else if (netwib_c2_isalpha(c)) {
        *dataout++ = '[';
        *dataout++ = netwib_c2_lc(c);
        *dataout++ = netwib_c2_uc(c);
        *dataout++ = ']';
      } else {
        /* protect reserved characters */
        *dataout++ = '[';
        *dataout++ = c;
        *dataout++ = ']';
      }
      state = NETWOX_SMBSERGLOB_RE_STATE_CHAR;
      break;
    case NETWOX_SMBSERGLOB_RE_STATE_QUESTION :
      seenotherchars = NETWIB_TRUE;
      /*no break*/
    case NETWOX_SMBSERGLOB_RE_STATE_SUP :
      if (state == NETWOX_SMBSERGLOB_RE_STATE_QUESTION ||
          state == NETWOX_SMBSERGLOB_RE_STATE_SUP) {
        statequestioncount++;
      } else {
        *dataout++ = '.';
        if (nextstate == NETWOX_SMBSERGLOB_RE_STATE_QUESTION) {
          state = NETWOX_SMBSERGLOB_RE_STATE_QUESTION;
        } else {
          state = NETWOX_SMBSERGLOB_RE_STATE_SUP;
        }
        statequestioncount = 0;
      }
      break;
    case NETWOX_SMBSERGLOB_RE_STATE_STAR :
      seenotherchars = NETWIB_TRUE;
      if (state != NETWOX_SMBSERGLOB_RE_STATE_STAR) {
        *dataout++ = '.';
        *dataout++ = '*';
        state = NETWOX_SMBSERGLOB_RE_STATE_STAR;
      }
      break;
    case NETWOX_SMBSERGLOB_RE_STATE_BEFOREDOT :
      *dataout++ = '[';
      *dataout++ = '.';
      *dataout++ = ']';
      state = NETWOX_SMBSERGLOB_RE_STATE_AFTERDOT;
      seenotherchars = NETWIB_FALSE;
      break;
    default:
      return(NETWOX_ERR_INTERNALERROR);
    }
  }

  /* special case of "file.*" and "file." which are "file[.].*" instead
     of "file[.]?.*" */
  if (dataout - dataoutori >= 5) {
    pc = dataout - 5;
    if (!netwib_c_memcmp(pc, (netwib_constdata)"[.].*", 5)) {
      dataout = pc + 3; /* will only set "?.*" because start is ok */
      *dataout++ = '?';
      *dataout++ = '.';
      *dataout++ = '*';
    }
  }

  /* special case of "*.*" which is ".*[.]?.*" instead of ".*" */
  if (dataout - dataoutori >= 8) {
    pc = dataout - 8;
    if (!netwib_c_memcmp(pc, (netwib_constdata)".*[.]?.*", 8)) {
      dataout = pc + 2; /* truncate before the [ */
    }
  }

  pre->endoffset += dataout - dataoutori;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbserglob_re_test2(netwib_conststring glob,
                                             netwib_conststring re)
{
  netwib_buf bufglob, bufre;

  netwib_er(netwib_buf_init_mallocdefault(&bufre));

  netwib_er(netwib_fmt_display("=> %s\n", glob));
  netwib_er(netwib_buf_init_ext_string(glob, &bufglob));

  netwib_er(netwox_smbserglob_re(&bufglob, &bufre));
  netwib_er(netwox_buf_check_strtxt(&bufre, re));

  netwib_er(netwib_buf_close(&bufre));

  return(NETWIB_ERR_OK);
}
netwib_err netwox_smbserglob_re_test(void)
{

  /* This behavior was tested with msdos dir c:\*.*, etc.
     This is sometimes different from the documented way. */
  /*  these 5 means to list all files */
  netwib_er(netwox_smbserglob_re_test2("", ".*")); /* from doc */
  netwib_er(netwox_smbserglob_re_test2("*", ".*"));
  netwib_er(netwox_smbserglob_re_test2("*.*", ".*"));
  netwib_er(netwox_smbserglob_re_test2(">>>>>>>>", ".*"));
  netwib_er(netwox_smbserglob_re_test2(">>>>>>>>\">>>", ".*"));
  /*  list files with a requested extension */
  netwib_er(netwox_smbserglob_re_test2("*.txt",
                                       ".*[.][tT][xX][tT]"));
  netwib_er(netwox_smbserglob_re_test2("<.c",
                                       ".*[.][cC]"));
  netwib_er(netwox_smbserglob_re_test2(".ext",
                                       ".*[.][eE][xX][tT]"));
  netwib_er(netwox_smbserglob_re_test2(">>>>>>>>\"TXT",
                                       ".*[.][tT][xX][tT]"));
  /*  list files with an optional extension */
  netwib_er(netwox_smbserglob_re_test2("file\"*",
                                       "[fF][iI][lL][eE][.]?.*"));
  netwib_er(netwox_smbserglob_re_test2("file.",
                                       "[fF][iI][lL][eE][.]?.*"));
  netwib_er(netwox_smbserglob_re_test2("file\">>>",
                                       "[fF][iI][lL][eE][.]?.*"));
  netwib_er(netwox_smbserglob_re_test2("file",
                                       "[fF][iI][lL][eE][.]?.*"));
  /* This behavior was not tested, but is supposed to be correct.
     This is the documented way. */
  netwib_er(netwox_smbserglob_re_test2("file.ext",
                                       "[fF][iI][lL][eE][.][eE][xX][tT]"));
  netwib_er(netwox_smbserglob_re_test2("A>>>>>>>>",
                                       "[aA]..{0,7}[.]?.*"));
  netwib_er(netwox_smbserglob_re_test2("A>>>>>>>>.B>>>",
                                       "[aA]..{0,7}[.][bB]..{0,2}"));
  netwib_er(netwox_smbserglob_re_test2(">>>>>>>>A.>>>B",
                                       "........[aA][.]...[bB]"));
  netwib_er(netwox_smbserglob_re_test2("????????",
                                       "..{0,7}[.]?.*"));
  netwib_er(netwox_smbserglob_re_test2("????????\"???",
                                       "..{0,7}[.]..{0,2}"));
  netwib_er(netwox_smbserglob_re_test2("????????\"TXT",
                                       "..{0,7}[.][tT][xX][tT]"));
  netwib_er(netwox_smbserglob_re_test2("file*",
                                       "[fF][iI][lL][eE].*"));
  netwib_er(netwox_smbserglob_re_test2("file.???",
                                       "[fF][iI][lL][eE][.]..{0,2}"));
  netwib_er(netwox_smbserglob_re_test2("?*.TXT",
                                       "..*[.][tT][xX][tT]"));
  netwib_er(netwox_smbserglob_re_test2(">\"txt",
                                       ".[.][tT][xX][tT]"));
  netwib_er(netwox_smbserglob_re_test2("?.TXT",
                                       ".[.][tT][xX][tT]"));
  netwib_er(netwox_smbserglob_re_test2("?A.*A",
                                       ".[aA][.].*[aA]"));
  netwib_er(netwox_smbserglob_re_test2("??A.**A",
                                       "..[aA][.].*[aA]"));
  netwib_er(netwox_smbserglob_re_test2("A?.A*",
                                       "[aA].[.][aA].*"));
  netwib_er(netwox_smbserglob_re_test2("A??.A**",
                                       "[aA]..{0,1}[.][aA].*"));
  netwib_er(netwox_smbserglob_re_test2(">A\"<A",
                                       ".[aA][.].*[aA]"));
  netwib_er(netwox_smbserglob_re_test2(">>A\"<<A",
                                       "..[aA][.].*[aA]"));
  netwib_er(netwox_smbserglob_re_test2("A>\"A<",
                                       "[aA].[.][aA].*"));
  netwib_er(netwox_smbserglob_re_test2("A>>\"A<<",
                                       "[aA]..{0,1}[.][aA].*"));
  /* this one is invalid for 8.3, but it should be ok */
  netwib_er(netwox_smbserglob_re_test2("??A.*A.*A",
                                       "..[aA][.].*[aA][.].*[aA]"));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserglob_item_init(netwib_bufpool *pbufpool,
                                       netwox_smbserglob_item *pitem)
{
  pitem->pbufpool = pbufpool;
  pitem->searchattributes = 0;
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pitem->prealdirname));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pitem->pfilere));
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pitem->psaved));
  pitem->volumegiven = NETWIB_FALSE;
  pitem->pdir = NULL;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserglob_item_set(netwox_smbserglob_item *pitem,
                                      netwox_smbcmdcmn_fileattr32 searchattributes,
                                      netwib_constbuf *prealdirname,
                                      netwib_constbuf *pfileglob)
{
  pitem->searchattributes = searchattributes;

  netwib_er(netwib_buf_append_buf(prealdirname, pitem->prealdirname));

  netwib_er(netwib_buf_append_byte('^', pitem->pfilere));
  netwib_er(netwox_smbserglob_re(pfileglob, pitem->pfilere));
  netwib_er(netwib_buf_append_byte('$', pitem->pfilere));

  netwib_er(netwib_dir_init(prealdirname, &pitem->pdir));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserglob_item_close(netwox_smbserglob_item *pitem)
{
  if (pitem->pdir != NULL) {
    netwib_er(netwib_dir_close(&pitem->pdir));
  }
  netwib_er(netwib_bufpool_buf_close(pitem->pbufpool, &pitem->psaved));
  netwib_er(netwib_bufpool_buf_close(pitem->pbufpool, &pitem->pfilere));
  netwib_er(netwib_bufpool_buf_close(pitem->pbufpool, &pitem->prealdirname));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserglob_item_next(netwox_smbser83 *psmbser83,
                                       netwox_smbserglob_item *pitem,
                                       netwib_buf *prealpathname)
{
  netwib_buf *ptmp, *ptmp2, *pfile, buffound;
  netwib_pathstat pathstat;
  netwib_err ret;

  /* check if there is a saved value */
  if (netwib__buf_ref_data_size(pitem->psaved)) {
    netwib_er(netwib_buf_append_buf(pitem->psaved, prealpathname));
    netwib__buf_reinit(pitem->psaved);
    return(NETWIB_ERR_OK);
  }

  /* special case of volume id */
  if (pitem->searchattributes & NETWOX_SMBCMDCMN_FILEATTR32_VOLUME) {
    if (pitem->volumegiven) {
      ret = NETWIB_ERR_DATAEND;
    } else {
      netwib_er(netwib_buf_append_string(NETWOX_SMBSER_FAKE_FS_LABEL,
                                       prealpathname));
      pitem->volumegiven = NETWIB_TRUE;
      ret = NETWIB_ERR_OK;
    }
    return(ret);
  }

  netwib_er(netwib_bufpool_buf_init(pitem->pbufpool, &pfile));
  netwib_er(netwib_bufpool_buf_init(pitem->pbufpool, &ptmp));
  netwib_er(netwib_bufpool_buf_init(pitem->pbufpool, &ptmp2));

  ret = NETWIB_ERR_OK;
  while(NETWIB_TRUE) {
    /* find next entry */
    netwib__buf_reinit(pfile);
    ret = netwib_dir_next(pitem->pdir, pfile);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    /* construct the candidate */
    netwib__buf_reinit(ptmp);
    netwib_eg(netwib_path_init_concat(pitem->prealdirname, pfile, ptmp));
    /* check if matches our type criteria */
    netwib_eg(netwib_pathstat_init(ptmp, &pathstat));
    if (pathstat.type != NETWIB_PATHSTAT_TYPE_DIR &&
        pathstat.type != NETWIB_PATHSTAT_TYPE_REG) {
      /* don't add links, sockets, etc. */
      continue;
    }
    if (pathstat.type == NETWIB_PATHSTAT_TYPE_DIR &&
        !(pitem->searchattributes & NETWOX_SMBCMDCMN_FILEATTR32_DIR)) {
      continue;
    }
    /* check if matches our name criteria. We use both 8.3 and long. This
       return too many entries, but we have to be coherent. For example,
       a "DIR file*" has to return the same files as "MOVE file* d".
       Msdos under Windows 98 does this, so I think returning
       too many entries is not a bad thing. */
    ret = netwib_buf_search_re(pfile, pitem->pfilere, NETWIB_FALSE, &buffound);
    if (ret != NETWIB_ERR_OK) {
      netwib__buf_reinit(ptmp2);
      ret = netwox_smbser83_longto83(psmbser83, pathstat.type, pfile, ptmp2);
      if (ret == NETWIB_ERR_OK) {
        ret = netwib_buf_search_re(ptmp2, pitem->pfilere, NETWIB_FALSE,
                                   &buffound);
      }
    }
    if (ret != NETWIB_ERR_OK) {
      continue;
    }
    /* if we are here, the file is a good one */
    netwib_eg(netwib_buf_append_buf(ptmp, prealpathname));
    break;
  }

 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(pitem->pbufpool, &ptmp2));
  netwib_er(netwib_bufpool_buf_close(pitem->pbufpool, &ptmp));
  netwib_er(netwib_bufpool_buf_close(pitem->pbufpool, &pfile));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserglob_item_save(netwox_smbserglob_item *pitem,
                                       netwib_constbuf *prealpathname)
{

  if (netwib__buf_ref_data_size(pitem->psaved)) {
    return(NETWOX_ERR_INTERNALERROR);
  }

  netwib_er(netwib_buf_append_buf(prealpathname, pitem->psaved));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserglob_itemptr_erase(netwib_ptr ptr)
{
  netwib_er(netwox_smbserglob_item_close((netwox_smbserglob_item *)ptr));
  netwib_er(netwib_ptr_free(&ptr));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserglob_itemptr_duplicate(netwib_constptr ptr,
                                              netwib_ptr *pptrdup)
{
  const netwox_smbserglob_item *pitem = (const netwox_smbserglob_item *)ptr;
  netwox_smbserglob_item *pitemdup;
  netwib_ptr ptrdup;

  netwib_er(netwox_smbserglob_itemptr_malloc(&ptrdup));
  pitemdup = (netwox_smbserglob_item *)ptrdup;
  *pptrdup = ptrdup;

  netwib_er(netwox_smbserglob_item_init(pitem->pbufpool, pitemdup));
  pitemdup->pbufpool = pitem->pbufpool;
  pitemdup->searchattributes = pitem->searchattributes;
  netwib_er(netwib_buf_append_buf(pitem->prealdirname, pitemdup->prealdirname));
  netwib_er(netwib_buf_append_buf(pitem->pfilere, pitemdup->pfilere));
  /* no dir dup */

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
netwib_err netwox_smbserglob_init(netwib_bufpool *pbufpool,
                                  netwox_smbserglob *psmbserglob)
{
  psmbserglob->pbufpool = pbufpool;
  psmbserglob->nextnumber = 1;
  netwib_er(netwox_smbserglob_item_hash_init(&psmbserglob->phash));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserglob_close(netwox_smbserglob *psmbserglob)
{
  netwib_er(netwox_smbserglob_item_hash_close(&psmbserglob->phash));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserglob_new(netwox_smbserglob *psmbserglob,
                                 netwib_uint32 *pid,
                                 netwox_smbserglob_item **ppitem)
{
  netwib_byte keyarray[NETWOX_BUFSTORE_HASH_KEY_SIZE];
  netwox_smbserglob_item *pitem;
  netwib_ptr ptr;
  netwib_buf key;

  netwib_er(netwox_bufstore_hash_key(keyarray, psmbserglob->nextnumber, &key));

  netwib_er(netwox_smbserglob_itemptr_malloc(&ptr));
  pitem = (netwox_smbserglob_item *)ptr;

  netwib_er(netwox_smbserglob_item_init(psmbserglob->pbufpool, pitem));

  netwib_er(netwib_hash_add(psmbserglob->phash, &key, ptr, NETWIB_TRUE));

  *pid = psmbserglob->nextnumber;
  *ppitem = (netwox_smbserglob_item *)ptr;
  psmbserglob->nextnumber++;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserglob_search(netwox_smbserglob *psmbserglob,
                                    netwib_uint32 id,
                                    netwox_smbserglob_item **ppitem)
{
  netwib_byte keyarray[NETWOX_BUFSTORE_HASH_KEY_SIZE];
  netwib_buf key;
  netwib_ptr ptr;

  netwib_er(netwox_bufstore_hash_key(keyarray, id, &key));
  netwib_er(netwib_hash_value(psmbserglob->phash, &key, &ptr));
  *ppitem = (netwox_smbserglob_item *)ptr;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserglob_del(netwox_smbserglob *psmbserglob,
                                 netwib_uint32 id)
{
  netwib_byte keyarray[NETWOX_BUFSTORE_HASH_KEY_SIZE];
  netwib_buf key;

  netwib_er(netwox_bufstore_hash_key(keyarray, id, &key));
  netwib_er(netwib_hash_del(psmbserglob->phash, &key, NETWIB_TRUE));

  return(NETWIB_ERR_OK);
}
