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
netwib_err netwox_smbser83_init(netwib_bufpool *pbufpool,
                                netwox_smbser83 *psmbser83)
{
  psmbser83->pbufpool = pbufpool;
  psmbser83->count = 0;
  netwib_er(netwox_bufstore_hash_init(&psmbser83->p83tolong));
  netwib_er(netwox_bufstore_hash_init(&psmbser83->plongto83));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbser83_close(netwox_smbser83 *psmbser83)
{
  netwib_er(netwox_bufstore_hash_close(&psmbser83->plongto83));
  netwib_er(netwox_bufstore_hash_close(&psmbser83->p83tolong));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwox_buf_findlist netwox_smbser83_findlist_dot = {
  {"", 1},
  {".", 1},
  {"..", 1},
  NETWOX_BUF_FINDLIST_END
};
netwox_buf_findlist netwox_smbser83_findlist_reserved = {
  {"aux", 1},
  {"con", 1},
  {"com1", 1},
  {"com2", 1},
  {"com3", 1},
  {"com4", 1},
  {"lpt1", 1},
  {"lpt2", 1},
  {"lpt3", 1},
  {"lpt4", 1},
  {"nul", 1},
  {"prn", 1},
  NETWOX_BUF_FINDLIST_END
};
static netwib_err netwox_smbser83_longto832(netwox_smbser83 *psmbser83,
                                            netwib_pathstat_type pathtype,
                                            netwib_constbuf *plongpathname,
                                            netwib_buf *p83pathname,
                                            netwib_bool *pnamecreated)
{
  netwib_buf *pupper8, *pupper3;
  netwib_data data;
  netwib_uint32 ui32, datasize, u8size=0;
  netwib_bool addext;
  netwib_char c;
  netwib_err ret;

  *pnamecreated = NETWIB_FALSE;

  /* special case of "", . and .. */
  ret = netwox_buf_find(netwox_smbser83_findlist_dot, NETWIB_TRUE, NETWIB_TRUE,
                        plongpathname, &ui32);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwib_buf_append_buf(plongpathname, p83pathname));
    return(NETWIB_ERR_OK);
  }

  /* init memory */
  netwib_er(netwib_bufpool_buf_init(psmbser83->pbufpool, &pupper8));
  netwib_er(netwib_bufpool_buf_init(psmbser83->pbufpool, &pupper3));

  /* separate name and extension */
  data = netwib__buf_ref_data_ptr(plongpathname);
  datasize = netwib__buf_ref_data_size(plongpathname);
  addext = NETWIB_FALSE;
  while(datasize--) {
    c = *data++;
    if (netwib_c2_isalnum(c) || c == '-' || c == '_') {
      /* add upper character */
      netwib_c2_upper(c);
      if (addext) {
        netwib_er(netwib_buf_append_byte(c, pupper3));
      } else {
        netwib_er(netwib_buf_append_byte(c, pupper8));
      }
    } else if (c == '.') {
      /* manage extension */
      if (addext) {
        netwib_er(netwib_buf_append_buf(pupper3, pupper8));
        netwib__buf_reinit(pupper3);
        *pnamecreated = NETWIB_TRUE; /* two or more dots */
      } else {
        addext = NETWIB_TRUE;
      }
    } else {
      /* if c is special, don't add it */
      *pnamecreated = NETWIB_TRUE;
    }
  }

  /* directories must not have extensions */
  if (pathtype == NETWIB_PATHSTAT_TYPE_DIR) {
    if (addext) {
      netwib_er(netwib_buf_append_buf(pupper3, pupper8));
      addext = NETWIB_FALSE;
      *pnamecreated = NETWIB_TRUE;
    }
  }

  /* check lengths */
  if (netwib__buf_ref_data_size(pupper8) == 0) {
    if (addext) {
      netwib_er(netwib_buf_append_buf(pupper3, pupper8));
      addext = NETWIB_FALSE;
    }
    *pnamecreated = NETWIB_TRUE;
  }
  if (netwib__buf_ref_data_size(pupper8) > 8) {
    pupper8->endoffset = pupper8->beginoffset + 8;
    *pnamecreated = NETWIB_TRUE;
  }
  if (addext) {
    if (netwib__buf_ref_data_size(pupper3) == 0) {
      addext = NETWIB_FALSE;
      *pnamecreated = NETWIB_TRUE;
    }
    if (netwib__buf_ref_data_size(pupper3) > 3) {
      pupper3->endoffset = pupper3->beginoffset + 3;
      *pnamecreated = NETWIB_TRUE;
    }
  }

  /* check this name is not reserved */
  ret = netwox_buf_find(netwox_smbser83_findlist_reserved, NETWIB_TRUE,
                        NETWIB_FALSE, pupper8, &ui32);
  if (ret == NETWIB_ERR_OK) {
    *pnamecreated = NETWIB_TRUE;
  }

  /* compose 8.3 name */
  if (*pnamecreated) {
    if      (psmbser83->count < 10)       { u8size = 6; }
    else if (psmbser83->count < 100)      { u8size = 5; }
    else if (psmbser83->count < 1000)     { u8size = 4; }
    else if (psmbser83->count < 10000)    { u8size = 3; }
    else if (psmbser83->count < 100000)   { u8size = 2; }
    else if (psmbser83->count < 1000000)  { u8size = 1; }
    else if (psmbser83->count < 10000000) { u8size = 0; }
    else { /* restart at 0 */ psmbser83->count = 0; u8size = 6; }
    if (netwib__buf_ref_data_size(pupper8) > u8size) {
      pupper8->endoffset = pupper8->beginoffset + u8size;
    }
    netwib_er(netwib_buf_append_buf(pupper8, p83pathname));
    netwib_er(netwib_buf_append_fmt(p83pathname, "~%{uint32}",
                                    psmbser83->count++));
  } else {
    netwib_er(netwib_buf_append_buf(pupper8, p83pathname));
  }
  if (addext) {
    netwib_er(netwib_buf_append_byte('.', p83pathname));
    netwib_er(netwib_buf_append_buf(pupper3, p83pathname));
  }

  /* close memory */
  netwib_er(netwib_bufpool_buf_close(psmbser83->pbufpool, &pupper3));
  netwib_er(netwib_bufpool_buf_close(psmbser83->pbufpool, &pupper8));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbser83_longto83(netwox_smbser83 *psmbser83,
                                    netwib_pathstat_type pathtype,
                                    netwib_constbuf *plongpathname,
                                    netwib_buf *p83pathname)
{
  netwib_buf *p83nameext, buf, *plongpathnametype;
  netwib_ptr pitem;
  netwib_bool namecreated;
  netwib_err ret;

  /* search in hash table (key is longpathname + 1_byte_for_type) */
  netwib_er(netwib_bufpool_buf_init(psmbser83->pbufpool, &plongpathnametype));
  netwib_er(netwib_buf_append_buf(plongpathname, plongpathnametype));
  netwib_er(netwib_buf_append_byte((netwib_byte)pathtype, plongpathnametype));
  ret = netwib_hash_value(psmbser83->plongto83, plongpathnametype, &pitem);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwox_bufstore_decode(pitem, &buf));
    netwib_er(netwib_buf_append_buf(&buf, p83pathname));
    netwib_er(netwib_bufpool_buf_close(psmbser83->pbufpool,
                                       &plongpathnametype));
    return(NETWIB_ERR_OK);
  }

  /* init memory */
  netwib_er(netwib_bufpool_buf_init(psmbser83->pbufpool, &p83nameext));

  /* create the short name */
  netwib_er(netwox_smbser83_longto832(psmbser83, pathtype, plongpathname,
                                      p83nameext, &namecreated));

  /* set output and add in hash tables */
  netwib_er(netwib_buf_append_buf(p83nameext, p83pathname));
  if (namecreated) {
    netwib_er(netwox_bufstore_create(plongpathname, &pitem));
    netwib_er(netwib_hash_add(psmbser83->p83tolong, p83nameext, pitem,
                              NETWIB_TRUE));
    netwib_er(netwox_bufstore_create(p83nameext, &pitem));
    netwib_er(netwib_hash_add(psmbser83->plongto83, plongpathnametype, pitem,
                              NETWIB_TRUE));
  }

  /* close memory */
  netwib_er(netwib_bufpool_buf_close(psmbser83->pbufpool, &p83nameext));
  netwib_er(netwib_bufpool_buf_close(psmbser83->pbufpool, &plongpathnametype));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbser83_longto83_path(netwox_smbser83 *psmbser83,
                                         netwib_pathstat_type pathtype,
                                         netwib_constbuf *plongpathname,
                                         netwib_buf *p83pathname)
{
  netwib_buf buf;
  netwib_data data, datastart;
  netwib_uint32 datasize;
  netwib_char c;

  datasize = netwib__buf_ref_data_size(plongpathname);
  if (datasize == 0) {
    return(NETWIB_ERR_OK);
  }
  data = netwib__buf_ref_data_ptr(plongpathname);

  datastart = data;
  while(datasize--) {
    c = *data;
    if (c == '/' || c == '\\') {
      if (data - datastart) {
        netwib_er(netwib_buf_init_ext_arrayfilled(datastart, data - datastart,
                                                  &buf));
        netwib_er(netwox_smbser83_longto83(psmbser83, NETWIB_PATHSTAT_TYPE_DIR,
                                           &buf, p83pathname));
      }
      netwib_er(netwib_buf_append_byte(c, p83pathname));
      datastart = data + 1;
    }
    data++;
  }
  if (data - datastart) {
    netwib_er(netwib_buf_init_ext_arrayfilled(datastart, data - datastart,
                                              &buf));
    netwib_er(netwox_smbser83_longto83(psmbser83, pathtype, &buf,
                                       p83pathname));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbser83_83tolong(netwox_smbser83 *psmbser83,
                                    netwib_constbuf *p83pathname,
                                    netwib_buf *plongpathname)
{
  netwib_buf buf, *pupper;
  netwib_ptr pitem;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(psmbser83->pbufpool, &pupper));

  /* convert name to uppercase */
  netwib_er(netwib_buf_encode(p83pathname, NETWIB_ENCODETYPE_UPPERCASE,
                              pupper));

  /* search in hash table */
  ret = netwib_hash_value(psmbser83->p83tolong, pupper, &pitem);
  if (ret == NETWIB_ERR_OK) {
    netwib_er(netwox_bufstore_decode(pitem, &buf));
    netwib_er(netwib_buf_append_buf(&buf, plongpathname));
  } else {
    /* not found, so use the original name */
    netwib_er(netwib_buf_append_buf(p83pathname, plongpathname));
  }

  netwib_er(netwib_bufpool_buf_close(psmbser83->pbufpool, &pupper));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbser83_83tolong_path(netwox_smbser83 *psmbser83,
                                         netwib_constbuf *p83pathname,
                                         netwib_buf *plongpathname)
{
  netwib_buf buf;
  netwib_data data, datastart;
  netwib_uint32 datasize;
  netwib_char c;

  datasize = netwib__buf_ref_data_size(p83pathname);
  if (datasize == 0) {
    return(NETWIB_ERR_OK);
  }
  data = netwib__buf_ref_data_ptr(p83pathname);

  datastart = data;
  while(datasize--) {
    c = *data;
    if (c == '/' || c == '\\') {
      if (data - datastart) {
        netwib_er(netwib_buf_init_ext_arrayfilled(datastart, data - datastart,
                                                  &buf));
        netwib_er(netwox_smbser83_83tolong(psmbser83, &buf, plongpathname));
      }
      netwib_er(netwib_buf_append_byte(c, plongpathname));
      datastart = data + 1;
    }
    data++;
  }
  if (data - datastart) {
    netwib_er(netwib_buf_init_ext_arrayfilled(datastart, data - datastart,
                                              &buf));
    netwib_er(netwox_smbser83_83tolong(psmbser83, &buf, plongpathname));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbser83_test_longto83(netwox_smbser83 *psmbser83,
                                             netwib_conststring longpathname,
                                             netwib_conststring wantedmixedreg,
                                             netwib_conststring wantedmixeddir)
{
  netwib_buf *pbuf1, *pbuf2, *pbuf3;
  netwib_cmp cmp;

  netwib_er(netwib_bufpool_buf_init(psmbser83->pbufpool, &pbuf1));
  netwib_er(netwib_bufpool_buf_init(psmbser83->pbufpool, &pbuf2));
  netwib_er(netwib_bufpool_buf_init(psmbser83->pbufpool, &pbuf3));

  netwib_er(netwox_buf_append_strmixed(longpathname, pbuf1));
  netwib_er(netwib_fmt_display(" %{buf}\n", pbuf1));

  netwib_er(netwox_smbser83_longto83_path(psmbser83, NETWIB_PATHSTAT_TYPE_REG,
                                          pbuf1, pbuf2));
  netwib_er(netwox_buf_check_strmixed(pbuf2, wantedmixedreg));
  netwib_er(netwox_smbser83_83tolong_path(psmbser83, pbuf2, pbuf3));
  netwib_er(netwib_buf_casecmp(pbuf3, pbuf1, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    netwib_er(netwox_buf_check_strmixed(pbuf3, longpathname));
  } else {
    netwib_er(netwib_fmt_display(" => %{buf}\n", pbuf3));
  }

  netwib__buf_reinit(pbuf2);
  netwib__buf_reinit(pbuf3);
  netwib_er(netwox_smbser83_longto83_path(psmbser83, NETWIB_PATHSTAT_TYPE_DIR,
                                          pbuf1, pbuf2));
  netwib_er(netwox_buf_check_strmixed(pbuf2, wantedmixeddir));
  netwib_er(netwox_smbser83_83tolong_path(psmbser83, pbuf2, pbuf3));
  netwib_er(netwib_buf_casecmp(pbuf3, pbuf1, &cmp));
  if (cmp != NETWIB_CMP_EQ) {
    netwib_er(netwox_buf_check_strmixed(pbuf3, longpathname));
  } else {
    netwib_er(netwib_fmt_display(" => %{buf}\n", pbuf3));
  }

  netwib_er(netwib_bufpool_buf_close(psmbser83->pbufpool, &pbuf3));
  netwib_er(netwib_bufpool_buf_close(psmbser83->pbufpool, &pbuf2));
  netwib_er(netwib_bufpool_buf_close(psmbser83->pbufpool, &pbuf1));
  return(NETWIB_ERR_OK);
}
netwib_err netwox_smbser83_test(void)
{
  netwib_bufpool *pbufpool;
  netwox_smbser83 smbser83;
  netwib_uint32 i;

  netwib_er(netwib_bufpool_initdefault(&pbufpool));
  netwib_er(netwox_smbser83_init(pbufpool, &smbser83));

  i = 2;
  while(i--) {
    netwib_er(netwox_smbser83_test_longto83(&smbser83, "''",
                                            "''", "''"));
    netwib_er(netwox_smbser83_test_longto83(&smbser83, "'Abcd.efgh'",
                                            "'ABCD~0.EFG'", "'ABCDEF~1'"));
    netwib_er(netwox_smbser83_test_longto83(&smbser83, "'.'",
                                            "'.'", "'.'"));
    netwib_er(netwox_smbser83_test_longto83(&smbser83, "'a'",
                                            "'A'", "'A'"));
    netwib_er(netwox_smbser83_test_longto83(&smbser83, "'..'",
                                            "'..'", "'..'"));
    netwib_er(netwox_smbser83_test_longto83(&smbser83, "'abc'",
                                            "'ABC'", "'ABC'"));
    netwib_er(netwox_smbser83_test_longto83(&smbser83, "'abcd'",
                                            "'ABCD'", "'ABCD'"));
    netwib_er(netwox_smbser83_test_longto83(&smbser83, "'abcd.ef'",
                                            "'ABCD.EF'", "'ABCDEF~2'"));
    netwib_er(netwox_smbser83_test_longto83(&smbser83, "'abcdabcd.efg'",
                                            "'ABCDABCD.EFG'", "'ABCDAB~3'"));
    netwib_er(netwox_smbser83_test_longto83(&smbser83, "'abcd.efgh'",
                                            "'ABCD~4.EFG'", "'ABCDEF~5'"));
    netwib_er(netwox_smbser83_test_longto83(&smbser83, "'abcdabcdabcd.efgh'",
                                            "'ABCDAB~6.EFG'", "'ABCDAB~7'"));
    netwib_er(netwox_smbser83_test_longto83(&smbser83, "'abcdabcdabcd.e'",
                                            "'ABCDAB~8.E'", "'ABCDAB~9'"));
    netwib_er(netwox_smbser83_test_longto83(&smbser83, "'abcdabcdabcd'",
                                            "'ABCDA~10'", "'ABCDA~11'"));
    netwib_er(netwox_smbser83_test_longto83(&smbser83, "'abc_dc dabcd'",
                                            "'ABC_D~12'", "'ABC_D~13'"));
    netwib_er(netwox_smbser83_test_longto83(&smbser83, "'ab.cd.ef'",
                                            "'ABCD~14.EF'", "'ABCDE~15'"));
    netwib_er(netwox_smbser83_test_longto83(&smbser83, "'.cd'",
                                            "'CD~16'", "'CD~17'"));
    netwib_er(netwox_smbser83_test_longto83(&smbser83, "'.cd.ef'",
                                            "'CD~18.EF'", "'CDEF~19'"));
    netwib_er(netwox_smbser83_test_longto83(&smbser83, "'cd.'",
                                            "'CD~20'", "'CD~21'"));
    netwib_er(netwox_smbser83_test_longto83(&smbser83, "'cd.ef.'",
                                            "'CDEF~22'", "'CDEF~23'"));
    netwib_er(netwox_smbser83_test_longto83(&smbser83, "' ab cd'",
                                            "'ABCD~24'", "'ABCD~25'"));
    netwib_er(netwox_smbser83_test_longto83(&smbser83, "'ab cd'",
                                            "'ABCD~26'", "'ABCD~27'"));
    netwib_er(netwox_smbser83_test_longto83(&smbser83, "'/\\a/b /cd/\\'",
                                            "'/\\A/B~28/CD/\\'", "'/\\A/B~28/CD/\\'"));
    netwib_er(netwox_smbser83_test_longto83(&smbser83, "'a/b /cd/\\'",
                                            "'A/B~28/CD/\\'", "'A/B~28/CD/\\'"));
    netwib_er(netwox_smbser83_test_longto83(&smbser83, "'a/b /cd.ef.'",
                                            "'A/B~28/CDEF~22'", "'A/B~28/CDEF~23'"));
  }

  netwib_er(netwox_smbser83_close(&smbser83));
  netwib_er(netwib_bufpool_close(&pbufpool));
  return(NETWIB_ERR_OK);
}

