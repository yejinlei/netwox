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
static netwib_err netwib_priv_path_info_smb(netwib_constdata data,
                                            netwib_uint32 datasize,
                                            netwib_buf *pbufbegin,
                                            netwib_bufext *pbufthen)
{
  netwib_constdata dataori;
  netwib_buf buf;

  /* data should contain "[\/][\/]server[\/]share[\/]*.*" */

  /* leading // or \\ was detected before calling this function. */
  data += 2;
  datasize -= 2;

  /* search the server */
  dataori = data;
  while (NETWIB_TRUE) {
    if (datasize == 0) {
      return(NETWIB_ERR_PAPATHNOTCANON);
    }
    if (*data == '/' || *data == '\\') {
      break;
    }
    data++;
    datasize--;
  }
  if (data - dataori == 0) {
    return(NETWIB_ERR_PAPATHNOTCANON);
  }
  if (pbufbegin != NULL) {
    netwib_er(netwib_buf_append_byte('/', pbufbegin));
    netwib_er(netwib_buf_append_byte('/', pbufbegin));
    netwib_er(netwib_buf_init_ext_arrayfilled(dataori, data - dataori, &buf));
    netwib_er(netwib_buf_encode(&buf, NETWIB_ENCODETYPE_LOWERCASE, pbufbegin));
    netwib_er(netwib_buf_append_byte('/', pbufbegin));
  }
  data++;
  datasize--;

  /* now search the share */
  dataori = data;
  while (NETWIB_TRUE) {
    if (datasize == 0 || *data == '/' || *data == '\\') {
      break;
    }
    data++;
    datasize--;
  }
  if (data - dataori == 0) {
    /* path is invalid because there is no share name */
    return(NETWIB_ERR_PAPATHNOTCANON);
  }
  if (pbufbegin != NULL) {
    netwib_er(netwib_buf_init_ext_arrayfilled(dataori, data - dataori, &buf));
    netwib_er(netwib_buf_encode(&buf, NETWIB_ENCODETYPE_LOWERCASE, pbufbegin));
    netwib_er(netwib_buf_append_byte('/', pbufbegin));
  }

  if (datasize == 0) {
    netwib_er(netwib_buf_init_ext_string("/", pbufthen));/* MT safe */
  } else {
    netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, pbufthen));
  }

  return(NETWIB_ERR_OK);
}
/* warning: output parameters are not consistent :
     - pbufbegin : data is canonized and appended
     - pbufthen : data is external pointing in ppathname (so not canonized)
                  pbufthen is similar to core, but not canonized
 */
static netwib_err netwib_priv_path_info(netwib_constbuf *ppathname,
                                        netwib_pathtype *ppathtype,
                                        netwib_buf *pbufbegin,
                                        netwib_bufext *pbufthen)
{
  netwib_data data;
  netwib_uint32 datasize;
  netwib_char c1, c2, c3;
  netwib_err ret;

  datasize = netwib__buf_ref_data_size(ppathname);
  if (datasize == 0) {
    return(NETWIB_ERR_PAPATHNOTCANON);
  }
  data = netwib__buf_ref_data_ptr(ppathname);

  c1 = data[0];
  c2 = (netwib_char)((datasize > 1)?data[1]:'\0');
  c3 = (netwib_char)((datasize > 2)?data[2]:'\0');
  if (ppathtype != NULL) {
    *ppathtype = 0;
  }
  if (c1 == '.') {
    if (ppathtype != NULL) {
      *ppathtype = NETWIB_PATHTYPE_UNIX;
    }
    if (pbufbegin != NULL) {
      netwib_er(netwib_buf_append_byte('.', pbufbegin));
    }
    if (pbufthen != NULL) {
      netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, pbufthen));
    }
  } else if (c1 == '/' || c1 == '\\') {
    netwib_bool issmb;
    issmb = NETWIB_FALSE;
    if (c2 == '/' || c2 == '\\') {
      netwib_uint32 bufbeginsize;
      netwib_buf bufthen;
      bufbeginsize = netwib__buf_ref_data_sizenull(pbufbegin);
      if (pbufthen == NULL) {
        pbufthen = &bufthen;
      }
      ret = netwib_priv_path_info_smb(data, datasize, pbufbegin, pbufthen);
      if (ret == NETWIB_ERR_OK) {
        if (ppathtype != NULL) {
          *ppathtype = NETWIB_PATHTYPE_WINSHARE | NETWIB_PATHTYPE_ABSOLUTE;
          data = netwib__buf_ref_data_ptr(pbufthen);
          datasize = netwib__buf_ref_data_size(pbufthen);
          if (datasize == 1 && (data[0] == '/' || data[0] == '\\')) {
            *ppathtype |= NETWIB_PATHTYPE_ROOT;
          }
        }
        issmb = NETWIB_TRUE;
      } else {
        /* error, so suppose it's absolute (skip leading /) */
        if (pbufbegin != NULL) {
          pbufbegin->endoffset = pbufbegin->beginoffset + bufbeginsize;
        }
        data++;
        datasize--;
      }
    }
    if (!issmb) {
      /* absolute path */
      if (ppathtype != NULL) {
        *ppathtype = NETWIB_PATHTYPE_UNIX | NETWIB_PATHTYPE_ABSOLUTE;
        if (datasize == 1) *ppathtype |= NETWIB_PATHTYPE_ROOT;
      }
      if (pbufbegin != NULL) {
        netwib_er(netwib_buf_append_byte('/', pbufbegin));
      }
      if (pbufthen != NULL) {
        netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, pbufthen));
      }
    }
  } else if (netwib_c2_isalpha(c1) && c2 == ':') {
    if (ppathtype != NULL) {
      *ppathtype = NETWIB_PATHTYPE_WINDRIVE;
    }
    if (pbufbegin != NULL) {
      netwib_er(netwib_buf_append_byte((netwib_char)(netwib_c2_lc(c1)), pbufbegin));
      netwib_er(netwib_buf_append_byte(':', pbufbegin));
    }
    if (c3 == '/' || c3 == '\\') {
      if (ppathtype != NULL) {
        *ppathtype |= NETWIB_PATHTYPE_ABSOLUTE;
        if (datasize == 3) *ppathtype |= NETWIB_PATHTYPE_ROOT;
      }
      if (pbufbegin != NULL) {
        netwib_er(netwib_buf_append_byte('/', pbufbegin));
      }
      if (pbufthen != NULL) {
        netwib_er(netwib_buf_init_ext_arrayfilled(data+2, datasize-2,
                                                  pbufthen));
      }
    } else if (c3 == '\0') {
      if (pbufthen != NULL) {
        netwib_er(netwib_buf_init_ext_string(".", pbufthen));/* MT safe */
      }
    } else {
      if (pbufthen != NULL) {
        netwib_er(netwib_buf_init_ext_arrayfilled(data+2, datasize-2,
                                                  pbufthen));
      }
    }
  } else {
    /* suppose it's a relative path */
    if (ppathtype != NULL) {
      *ppathtype = NETWIB_PATHTYPE_UNIX;
    }
    if (pbufbegin != NULL) {
      netwib_er(netwib_buf_append_byte('.', pbufbegin));
    }
    if (pbufthen != NULL) {
      netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, pbufthen));
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#define NETWIB_PRIV_PATH_CANON_LASTWAS_PATH 0
#define NETWIB_PRIV_PATH_CANON_LASTWAS_SLASH 1
#define NETWIB_PRIV_PATH_CANON_LASTWAS_DOT 2
static netwib_err netwib_priv_path_canon_goup(netwib_buf *pout,
                                              netwib_uint32 outinitsize,
                                              netwib_uint32 *plastwas)
{
  netwib_data data, pc;
  netwib_uint32 datasize;

  /* convert :
     /       to NETWIB_ERR_PAPATHROOTDOTDOT [/..]
     .       to ..      [./..]
     ..      to ../..   [../..]
     x/..    to x/../.. [x/../..]
     x/aa/bb to x/aa    [x/aa/bb/..]
     x/aa    to x       [x/aa/..]
     x       to .       [x/..]
     /x      to /       [/x/..]
  */

  data = netwib__buf_ref_data_ptr(pout) + outinitsize;
  datasize = netwib__buf_ref_data_size(pout) - outinitsize;

  if (datasize == 1) {
    if (data[0] == '/' || data[0] == '\\') {
      return(NETWIB_ERR_PAPATHROOTDOTDOT);
    }
    if (data[0] == '.') {
      /* "." to ".." */
      netwib_er(netwib_buf_append_byte('.', pout));
      *plastwas = NETWIB_PRIV_PATH_CANON_LASTWAS_PATH;
      return(NETWIB_ERR_OK);
    }
  }

  if (datasize == 2) {
    if (data[0] == '.' && data[1] == '.') {
      /* ".." to "../.." */
      netwib_er(netwib_buf_append_string("/..", pout));
      *plastwas = NETWIB_PRIV_PATH_CANON_LASTWAS_PATH;
      return(NETWIB_ERR_OK);
    }
  }

  if (datasize >= 3) {
    pc = data + datasize - 3;
    if (*pc == '/' || *pc == '\\') {
      pc++;
      if (*pc++ == '.') {
        if (*pc == '.') {
          /* "x/.." to "x/../.." */
          netwib_er(netwib_buf_append_string("/..", pout));
          *plastwas = NETWIB_PRIV_PATH_CANON_LASTWAS_PATH;
          return(NETWIB_ERR_OK);
        }
      }
    }
  }

  pc = data + datasize - 1;
  while (NETWIB_TRUE) {
    if (*pc == '/' || *pc == '\\') {
      break;
    }
    pc--;
    if (pc < data) {
      /* no '/' found, so : "x" to "." */
      pout->endoffset = pout->beginoffset + outinitsize;
      netwib_er(netwib_buf_append_string(".", pout));
      *plastwas = NETWIB_PRIV_PATH_CANON_LASTWAS_DOT;
      return(NETWIB_ERR_OK);
    }
  }
  if (pc == data) {
    /* "/x" to "/" */
    pout->endoffset = pout->beginoffset + outinitsize + 1;
    *plastwas = NETWIB_PRIV_PATH_CANON_LASTWAS_SLASH;
  } else {
    /* "x/aa/bb" to "x/aa" */
    pout->endoffset = pout->beginoffset + outinitsize + pc - data;
    *plastwas = NETWIB_PRIV_PATH_CANON_LASTWAS_PATH;
  }

  return(NETWIB_ERR_OK);
}
/* works only on a NETWIB_PATHTYPE_UNIX path */
static netwib_err netwib_priv_path_canon(netwib_constbuf *ppathname,
                                         netwib_buf *pout)
{
  netwib_data data, dataori;
  netwib_uint32 datasize, lastwas, outinitsize;

  datasize = netwib__buf_ref_data_size(ppathname);
  if (datasize == 0) {
    /* forbids empty filenames */
    return(NETWIB_ERR_PAPATHNOTCANON);
  }
  data = netwib__buf_ref_data_ptr(ppathname);
  outinitsize = netwib__buf_ref_data_size(pout);

  /* set root */
  lastwas = NETWIB_PRIV_PATH_CANON_LASTWAS_PATH;
  if (data[0] == '/' || data[0] == '\\') {
    netwib_er(netwib_buf_append_byte('/', pout));
    lastwas = NETWIB_PRIV_PATH_CANON_LASTWAS_SLASH;
  } else {
    netwib_er(netwib_buf_append_byte('.', pout));
    lastwas = NETWIB_PRIV_PATH_CANON_LASTWAS_DOT;
  }

  /* main loop treating each directory */
  while (NETWIB_TRUE) {
    /* ensures data points to the begin of directory */
    while (NETWIB_TRUE) {
      if (datasize == 0) {
        /* normal end */
        return(NETWIB_ERR_OK);
      }
      if (*data != '/' && *data != '\\') {
        break;
      }
      data++;
      datasize--;
    }
    /* obtain the directory end */
    dataori = data;
    while (NETWIB_TRUE) {
      if (datasize == 0 || *data == '/' || *data == '\\') {
        break;
      }
      data++;
      datasize--;
    }
    /* now, the directory name is between dataori and data */
    if ((data-dataori == 1) && (dataori[0] == '.')) {
      /* add nothing */
    } else if ((data-dataori == 2) && (dataori[0] == '.')
               && (dataori[1] == '.')) {
      /* go up */
      netwib_er(netwib_priv_path_canon_goup(pout, outinitsize, &lastwas));
    } else {
      /* add directory */
      if (lastwas == NETWIB_PRIV_PATH_CANON_LASTWAS_DOT) {
        pout->endoffset--; /* remove dot */
      } else if (lastwas != NETWIB_PRIV_PATH_CANON_LASTWAS_SLASH) {
        netwib_er(netwib_buf_append_byte('/', pout));
      }
      netwib_er(netwib_buf_append_data(dataori, data-dataori, pout));
      lastwas = NETWIB_PRIV_PATH_CANON_LASTWAS_PATH;
    }
  }

  return(NETWIB_ERR_LOINTERNALERROR);
}

/*-------------------------------------------------------------*/
/* pcanonizedpathname must contain "begin canonized" */
static netwib_err netwib_priv_path_append_then(netwib_pathtype beginpathtype,
                                               netwib_constbuf *pthen,
                                               netwib_buf *pcanonizedpathname)
{
  netwib_data data;
  netwib_uint32 size;

  if (beginpathtype & NETWIB_PATHTYPE_UNIX) {
    pcanonizedpathname->endoffset--; /* suppress last / or . of begin */
    netwib_er(netwib_priv_path_canon(pthen, pcanonizedpathname));
  } else if (beginpathtype & NETWIB_PATHTYPE_WINDRIVE) {
    if (beginpathtype & NETWIB_PATHTYPE_ABSOLUTE) {
      pcanonizedpathname->endoffset--; /* suppress last / of begin */
      netwib_er(netwib_priv_path_canon(pthen, pcanonizedpathname));
    } else {
      size = netwib__buf_ref_data_size(pcanonizedpathname);
      netwib_er(netwib_priv_path_canon(pthen, pcanonizedpathname));
      /* suppress last '.' in 'c:.' */
      if (netwib__buf_ref_data_size(pcanonizedpathname) - size == 1) {
        data = netwib__buf_ref_data_ptr(pcanonizedpathname);
        if (data[size] == '.') {
          pcanonizedpathname->endoffset--; /* suppress . of then */
        }
      }
    }
  } else if (beginpathtype & NETWIB_PATHTYPE_WINSHARE) {
    pcanonizedpathname->endoffset--; /* suppress last / of begin */
    netwib_er(netwib_priv_path_canon(pthen, pcanonizedpathname));
  } else {
    return(NETWIB_ERR_LOINTERNALERROR);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_path_canon(netwib_constbuf *ppathname,
                             netwib_buf *pcanonizedpathname)
{
  netwib_buf then;
  netwib_pathtype pathtype;

  /* directly store begin in pcanonizedpathname */
  netwib_er(netwib_priv_path_info(ppathname, &pathtype, pcanonizedpathname,
                                  &then));

  /* append then */
  netwib_er(netwib_priv_path_append_then(pathtype, &then,
                                         pcanonizedpathname));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_pathtype_init(netwib_constbuf *ppathname,
                                netwib_pathtype *ppathtype)
{
  netwib_er(netwib_priv_path_info(ppathname, ppathtype, NULL, NULL));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_path_decode(netwib_constbuf *ppathname,
                              netwib_path_decodetype type,
                              netwib_buf *pout)
{
  netwib_pathtype pathtype;
  netwib_byte buftmparray[512];
  netwib_buf buftmp, then;
  netwib_data data;
  netwib_uint32 datasize;
  netwib_err ret;

  if (netwib__buf_ref_data_sizenull(ppathname) == 0) {
    return(NETWIB_ERR_PAPATHNOTCANON);
  }

  switch(type) {
  case NETWIB_PATH_DECODETYPE_BEGIN :
    netwib_er(netwib_priv_path_info(ppathname, NULL, pout, NULL));
    break;
  case NETWIB_PATH_DECODETYPE_CORE :
    netwib_er(netwib_priv_path_info(ppathname, NULL, NULL, &then));
    netwib_er(netwib_priv_path_canon(&then, pout));
    break;
  case NETWIB_PATH_DECODETYPE_PARENT :
    /* We could simply convert "a/b" to "a", but there are cases such
       as "a/.." to "a/../.." which are not so easy. So, use buftmp. */
    netwib_er(netwib_priv_path_info(ppathname, &pathtype, pout, &then));
    netwib_er(netwib_buf_init_ext_storagearraysizeof(buftmparray, &buftmp));
    netwib_er(netwib_buf_append_buf(&then, &buftmp));
    netwib_er(netwib_buf_append_string("/..", &buftmp));
    ret = netwib_priv_path_append_then(pathtype, &buftmp, pout);
    netwib_er(netwib_buf_close(&buftmp));
    netwib_er(ret);
    break;
  case NETWIB_PATH_DECODETYPE_CHILD :
    /* We could use last part, but there are cases such as "a/" or "a/."
       which are not so easy. So, use buftmp, and canonize it. */
    netwib_er(netwib_priv_path_info(ppathname, NULL, NULL, &then));
    netwib_er(netwib_buf_init_ext_storagearraysizeof(buftmparray, &buftmp));
    ret = netwib_priv_path_canon(&then, &buftmp);
    if (ret == NETWIB_ERR_OK) {
      datasize = netwib__buf_ref_data_size(&buftmp);
      if (datasize == 0) {
        return(NETWIB_ERR_LOINTERNALERROR);
      }
      data = netwib__buf_ref_data_ptr(&buftmp);
      if (datasize == 1 && data[0] == '/') {
        netwib_er(netwib_buf_append_byte('/', pout));
      } else {
        data += datasize;
        while (datasize--) {
          data--;
          if (*data == '/') {
            data++;
            break;
          }
        }
        netwib_er(netwib_buf_append_data(data, netwib__buf_ref_data_size(&buftmp) - datasize - 1, pout));
      }
    }
    netwib_er(netwib_buf_close(&buftmp));
    netwib_er(ret);
    break;
  case NETWIB_PATH_DECODETYPE_EXTENSION :
    datasize = netwib__buf_ref_data_size(ppathname);
    data = netwib__buf_ref_data_ptr(ppathname);
    data += datasize;
    while (NETWIB_TRUE) {
      if (datasize == 0) { /* no extension */
        netwib_er(netwib_buf_init_ext_empty(pout));
        return(NETWIB_ERR_OK);
      }
      datasize--;
      data--;
      if (*data == '.') {
        data++;
        break;
      }
      if (*data == '/' || *data == '\\') { /* no extension */
        netwib_er(netwib_buf_init_ext_empty(pout));
        return(NETWIB_ERR_OK);
      }
    }
    netwib_er(netwib_buf_init_ext_arrayfilled(data, netwib__buf_ref_data_size(ppathname) - datasize - 1, pout));
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_path_begin_cmp(netwib_constbuf *pbegin1,
                                             netwib_pathtype pathtype1,
                                             netwib_constbuf *pbegin2,
                                             netwib_pathtype pathtype2,
                                             netwib_bool ignoreroot)
{
  netwib_buf begin1, begin2;
  netwib_cmp cmp;

  if (pathtype1 & NETWIB_PATHTYPE_UNIX) {
    if (!(pathtype2 & NETWIB_PATHTYPE_UNIX)) {
      return(NETWIB_ERR_PAPATHCANTINIT);
    }
    if (!ignoreroot) {
      netwib_er(netwib_buf_cmp(pbegin1, pbegin2, &cmp));
      if (cmp != NETWIB_CMP_EQ) {
        return(NETWIB_ERR_PAPATHCANTINIT);
      }
    }
  } else if (pathtype1 & NETWIB_PATHTYPE_WINSHARE) {
    if (!(pathtype2 & NETWIB_PATHTYPE_WINSHARE)) {
      return(NETWIB_ERR_PAPATHCANTINIT);
    }
    netwib_er(netwib_buf_cmp(pbegin1, pbegin2, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      return(NETWIB_ERR_PAPATHCANTINIT);
    }
  } else if (pathtype1 & NETWIB_PATHTYPE_WINDRIVE) {
    if (!(pathtype2 & NETWIB_PATHTYPE_WINDRIVE)) {
      return(NETWIB_ERR_PAPATHCANTINIT);
    }
    begin1 = *pbegin1;
    begin2 = *pbegin2;
    if (ignoreroot) {
      if (pathtype1 & NETWIB_PATHTYPE_ABSOLUTE) {
        begin1.endoffset--; /* ignore "c:'/'" for comparing */
      }
      if (pathtype2 & NETWIB_PATHTYPE_ABSOLUTE) {
        begin2.endoffset--; /* ignore "c:'/'" for comparing */
      }
    }
    netwib_er(netwib_buf_cmp(&begin1, &begin2, &cmp));
    if (cmp != NETWIB_CMP_EQ) {
      return(NETWIB_ERR_PAPATHCANTINIT);
    }
  } else {
    return(NETWIB_ERR_LOINTERNALERROR);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_path_init_concat(netwib_constbuf *pdirname1,
                                               netwib_constbuf *ppathname2,
                                               netwib_buf *pout)
{
  netwib_byte buftmp1array[512], buftmp2array[512];
  netwib_buf buftmp1, buftmp2, then1, then2;
  netwib_pathtype pathtype1, pathtype2;
  netwib_err ret=NETWIB_ERR_OK;

  /* initialize buffers */
  netwib_er(netwib_buf_init_ext_storagearraysizeof(buftmp1array, &buftmp1));
  netwib_er(netwib_buf_init_ext_storagearraysizeof(buftmp2array, &buftmp2));

  /* obtain information */
  netwib_eg(netwib_priv_path_info(pdirname1, &pathtype1, &buftmp1, &then1));
  netwib_eg(netwib_priv_path_info(ppathname2, &pathtype2, &buftmp2, &then2));

  /* check parameters */
  if (pathtype2 & NETWIB_PATHTYPE_ABSOLUTE) {
    if (!(pathtype1 & NETWIB_PATHTYPE_ROOT)) {
      /* if ppathname2 is absolute, pdirname1 must be a root */
      netwib_eg(NETWIB_ERR_PAPATHCANTINIT);
    }
    /* if ppathname2 is absolute, pdirname1 must be the same root */
    if (!(pathtype2 & NETWIB_PATHTYPE_UNIX)) {
      netwib_eg(netwib_priv_path_begin_cmp(&buftmp1, pathtype1, &buftmp2,
                                           pathtype2, NETWIB_FALSE));
    }
  } else {
    /* the root must be the same */
    if (!(pathtype2 & NETWIB_PATHTYPE_UNIX)) {
      netwib_eg(netwib_priv_path_begin_cmp(&buftmp1, pathtype1, &buftmp2,
                                           pathtype2, NETWIB_TRUE));
    }
  }

  /* create path */
  netwib__buf_reinit(&buftmp2);
  netwib_eg(netwib_buf_append_buf(&then1, &buftmp2));
  netwib_eg(netwib_buf_append_byte('/', &buftmp2));
  netwib_eg(netwib_buf_append_buf(&then2, &buftmp2));
  netwib_eg(netwib_priv_path_append_then(pathtype1, &buftmp2, &buftmp1));
  netwib_eg(netwib_buf_append_buf(&buftmp1, pout));

  /* close */
 netwib_gotolabel:
  netwib_er(netwib_buf_close(&buftmp2));
  netwib_er(netwib_buf_close(&buftmp1));
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_path_init_jail(netwib_constbuf *pdirname1,
                                             netwib_constbuf *ppathname2,
                                             netwib_buf *pout)
{
  netwib_byte buftmp2array[512], buftmp3array[512];
  netwib_buf buftmp2, buftmp3, then1, then2;
  netwib_pathtype pathtype1, pathtype2;
  netwib_err ret=NETWIB_ERR_OK;

  /* initialize buffers */
  netwib_er(netwib_buf_init_ext_storagearraysizeof(buftmp2array, &buftmp2));
  netwib_er(netwib_buf_init_ext_storagearraysizeof(buftmp3array, &buftmp3));

  /* obtain information */
  netwib_eg(netwib_priv_path_info(pdirname1, &pathtype1, pout, &then1));
  netwib_eg(netwib_priv_path_info(ppathname2, &pathtype2, NULL, &then2));

  /* check parameters */
  if (pathtype2 & NETWIB_PATHTYPE_WINDRIVE) {
    /* for security reasons, this should not be allowed */
    netwib_goto(NETWIB_ERR_PAPATHCANTINIT);
  }
  if (pathtype2 & NETWIB_PATHTYPE_WINSHARE) {
    /* for security reasons, this should not be allowed */
    netwib_goto(NETWIB_ERR_PAPATHCANTINIT);
  }

  /* create path */
  netwib_eg(netwib_buf_append_buf(&then1, &buftmp2));
  netwib_eg(netwib_buf_append_byte('/', &buftmp3));
  netwib_eg(netwib_buf_append_buf(&then2, &buftmp3));
  netwib_eg(netwib_priv_path_canon(&buftmp3, &buftmp2));
  netwib_eg(netwib_priv_path_append_then(pathtype1, &buftmp2, pout));

  /* close */
 netwib_gotolabel:
  netwib_er(netwib_buf_close(&buftmp3));
  netwib_er(netwib_buf_close(&buftmp2));
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_path_init_abs(netwib_constbuf *pdirname1,
                                            netwib_constbuf *ppathname2,
                                            netwib_buf *pout)
{
  netwib_byte buftmp1array[512], buftmp2array[512];
  netwib_buf buftmp1, buftmp2, then1, then2;
  netwib_pathtype pathtype1, pathtype2;
  netwib_err ret=NETWIB_ERR_OK;

  /* initialize buffers */
  netwib_er(netwib_buf_init_ext_storagearraysizeof(buftmp1array, &buftmp1));
  netwib_er(netwib_buf_init_ext_storagearraysizeof(buftmp2array, &buftmp2));

  /* check parameters */
  netwib_eg(netwib_priv_path_info(ppathname2, &pathtype2, &buftmp2, &then2));
  if (pathtype2 & NETWIB_PATHTYPE_ABSOLUTE) {
    netwib_eg(netwib_buf_append_buf(ppathname2, pout));
    netwib_goto(NETWIB_ERR_OK);
  }
  netwib_eg(netwib_priv_path_info(pdirname1, &pathtype1, &buftmp1, &then1));
  if (!(pathtype1 & NETWIB_PATHTYPE_ABSOLUTE)) {
    netwib_eg(NETWIB_ERR_PAPATHCANTINIT);
  }

  /* check parameters */
  if (!(pathtype2 & NETWIB_PATHTYPE_UNIX)) {
    netwib_eg(netwib_priv_path_begin_cmp(&buftmp1, pathtype1, &buftmp2,
                                         pathtype2, NETWIB_TRUE));
  }

  /* create path */
  netwib__buf_reinit(&buftmp2);
  netwib_eg(netwib_buf_append_buf(&then1, &buftmp2));
  netwib_eg(netwib_buf_append_byte('/', &buftmp2));
  netwib_eg(netwib_buf_append_buf(&then2, &buftmp2));
  netwib_eg(netwib_priv_path_append_then(pathtype1, &buftmp2, &buftmp1));
  netwib_eg(netwib_buf_append_buf(&buftmp1, pout));

  /* close */
 netwib_gotolabel:
  netwib_er(netwib_buf_close(&buftmp2));
  netwib_er(netwib_buf_close(&buftmp1));
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_path_init_rel(netwib_constbuf *ppathname1,
                                            netwib_constbuf *ppathname2,
                                            netwib_buf *pout)
{
  netwib_byte buftmp1array[512], buftmp2array[512], buftmp3array[512];
  netwib_buf buftmp1, buftmp2, buftmp3;
  netwib_string pc1, pc2, pitem1, pitem2;
  netwib_err ret=NETWIB_ERR_OK;

  /* initialize buffers */
  netwib_er(netwib_buf_init_ext_storagearraysizeof(buftmp1array, &buftmp1));
  netwib_er(netwib_buf_init_ext_storagearraysizeof(buftmp2array, &buftmp2));
  netwib_er(netwib_buf_init_ext_storagearraysizeof(buftmp3array, &buftmp3));

  /* first, canonize */
  netwib_eg(netwib_priv_path_canon(ppathname1, &buftmp1));
  netwib_eg(netwib_priv_path_canon(ppathname2, &buftmp2));
  netwib_eg(netwib_buf_ref_string(&buftmp1, &pc1));
  netwib_eg(netwib_buf_ref_string(&buftmp2, &pc2));

  /* find the common part */
  pitem1 = pc1;
  pitem2 = pc2;
  while (NETWIB_TRUE) {
    if (*pc1 != *pc2) {
      if (*pc1 == '\0' && *pc2 == '/') {
        pitem1 = NULL; /* means end */
        pitem2 = pc2+1; /* char after '/'*/
      }
      if (*pc1 == '/' && *pc2 == '\0') {
        pitem1 = pc1+1; /* char after '/'*/
        pitem2 = NULL; /* means end */
      }
      break;
    }
    if (*pc1 == '\0' /* comparing pc2 is not needed because *pc1 == *pc2 */) {
      pitem1 = NULL; /* means end */
      pitem2 = NULL; /* means end */
      break;
    }
    if (*pc1 == '/' /* comparing pc2 is not needed because *pc1 == *pc2 */) {
      pitem1 = pc1+1; /* char after '/'*/
      pitem2 = pc2+1; /* char after '/'*/
    }
    pc1++;
    pc2++;
  }
  /* now, cases "pci+1" (to go char after '/') have to be converted to NULL */
  if (pitem1 != NULL && *pitem1 == '\0') pitem1 = NULL;
  if (pitem2 != NULL && *pitem2 == '\0') pitem2 = NULL;

  if (pitem1 != NULL) {
    /* if pitem1 points to "../xyz", we cannot determine */
    if (pitem1[0] == '.' && pitem1[1] == '.' && pitem1[2] == '/') {
      netwib_eg(NETWIB_ERR_PAPATHCANTINIT);
    }
    /* loop till the end of pc1 to create "../../../" */
    pc1 = pitem1;
    while (*pc1 != '\0') {
      if (*pc1 == '/') {
        netwib_eg(netwib_buf_append_string("../", pout));
      }
      pc1++;
    }
    netwib_eg(netwib_buf_append_string("..", pout));
    if (pitem2 != NULL) {
      netwib_eg(netwib_buf_append_string("/", pout));
      /* pitem2 is already canonized, so use it */
      netwib_eg(netwib_buf_append_string(pitem2, pout));
    }
  } else {
    /* if pitem1 is NULL, there is no "../../../" to create */
    if (pitem2 != NULL) {
      /* pitem2 is already canonized, so use it */
      netwib_eg(netwib_buf_append_string(pitem2, pout));
    } else {
      netwib_eg(netwib_buf_append_string(".", pout));
    }
  }

  /* close */
 netwib_gotolabel:
  netwib_er(netwib_buf_close(&buftmp3));
  netwib_er(netwib_buf_close(&buftmp2));
  netwib_er(netwib_buf_close(&buftmp1));
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_path_init_rela(netwib_constbuf *pdirname1,
                                             netwib_constbuf *ppathname2,
                                             netwib_buf *pout)
{
  netwib_byte buftmp1array[512], buftmp2array[512];
  netwib_buf buftmp1, buftmp2, then1, then2;
  netwib_pathtype pathtype1, pathtype2;
  netwib_err ret=NETWIB_ERR_OK;

  /* initialize buffers */
  netwib_er(netwib_buf_init_ext_storagearraysizeof(buftmp1array, &buftmp1));
  netwib_er(netwib_buf_init_ext_storagearraysizeof(buftmp2array, &buftmp2));

  /* obtain information */
  netwib_eg(netwib_priv_path_info(pdirname1, &pathtype1, &buftmp1, &then1));
  netwib_eg(netwib_priv_path_info(ppathname2, &pathtype2, &buftmp2, &then2));

  /* check parameters */
  if (pathtype2 & NETWIB_PATHTYPE_ABSOLUTE) {
    if (!(pathtype1 & NETWIB_PATHTYPE_ABSOLUTE)) {
      netwib_eg(NETWIB_ERR_PAPATHCANTINIT);
    }
    if (!(pathtype2 & NETWIB_PATHTYPE_UNIX)) {
      netwib_eg(netwib_priv_path_begin_cmp(&buftmp1, pathtype1, &buftmp2,
                                           pathtype2, NETWIB_TRUE));
    }
    netwib_eg(netwib_priv_path_init_rel(&then1, &then2, pout));
  } else {
    /* ensure root is the same */
    if (!(pathtype2 & NETWIB_PATHTYPE_UNIX)) {
      netwib_eg(netwib_priv_path_begin_cmp(&buftmp1, pathtype1, &buftmp2,
                                           pathtype2, NETWIB_TRUE));
    }
    /* if ppathname2 is relative, use it */
    netwib_eg(netwib_priv_path_canon(ppathname2, pout));
  }

  /* close */
 netwib_gotolabel:
  netwib_er(netwib_buf_close(&buftmp2));
  netwib_er(netwib_buf_close(&buftmp1));
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_path_init_relb(netwib_constbuf *pdirname1,
                                             netwib_constbuf *ppathname2,
                                             netwib_buf *pout)
{
  netwib_byte buftmp1array[512], buftmp2array[512];
  netwib_buf buftmp1, buftmp2, then1, then2;
  netwib_pathtype pathtype1, pathtype2;
  netwib_err ret=NETWIB_ERR_OK;

  /* initialize buffers */
  netwib_er(netwib_buf_init_ext_storagearraysizeof(buftmp1array, &buftmp1));
  netwib_er(netwib_buf_init_ext_storagearraysizeof(buftmp2array, &buftmp2));

  /* obtain information */
  netwib_eg(netwib_priv_path_info(pdirname1, &pathtype1, &buftmp1, &then1));
  netwib_eg(netwib_priv_path_info(ppathname2, &pathtype2, &buftmp2, &then2));

  /* check parameters */
  netwib_eg(netwib_priv_path_begin_cmp(&buftmp1, pathtype1, &buftmp2,
                                       pathtype2, NETWIB_FALSE));

  /* create path */
  netwib_eg(netwib_priv_path_init_rel(&then1, &then2, pout));

  /* close */
 netwib_gotolabel:
  netwib_er(netwib_buf_close(&buftmp2));
  netwib_er(netwib_buf_close(&buftmp1));
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_path_init(netwib_constbuf *pdirname1,
                            netwib_constbuf *ppathname2,
                            netwib_path_inittype type,
                            netwib_buf *pout)
{
  netwib_err ret=NETWIB_ERR_OK;

  if (netwib__buf_ref_data_sizenull(pdirname1) == 0 ||
      netwib__buf_ref_data_sizenull(ppathname2) == 0) {
    return(NETWIB_ERR_PAPATHNOTCANON);
  }

  switch(type) {
    case NETWIB_PATH_INITTYPE_CONCAT :
      ret = netwib_priv_path_init_concat(pdirname1, ppathname2, pout);
      break;
    case NETWIB_PATH_INITTYPE_JAIL :
      ret = netwib_priv_path_init_jail(pdirname1, ppathname2, pout);
      break;
    case NETWIB_PATH_INITTYPE_ABS :
      ret = netwib_priv_path_init_abs(pdirname1, ppathname2, pout);
      break;
    case NETWIB_PATH_INITTYPE_RELA :
      ret = netwib_priv_path_init_rela(pdirname1, ppathname2, pout);
      break;
    case NETWIB_PATH_INITTYPE_RELB :
      ret = netwib_priv_path_init_relb(pdirname1, ppathname2, pout);
      break;
  }

  if (ret == NETWIB_ERR_PAPATHROOTDOTDOT || ret == NETWIB_ERR_PAPATHNOTCANON) {
    ret = NETWIB_ERR_PAPATHCANTINIT;
  }

  return(ret);
}
