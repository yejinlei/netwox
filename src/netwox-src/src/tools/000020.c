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
netwib_conststring t000020_description[] = {
  NETWOX_DESC_record,
  "",
  "This tool searches a pattern in packets of a record, and saves",
  "matching packets in another record. The pattern can be a string, a",
  "mixed ('hello' 09 'bob'), or a regular expression.",
  "",
  NETWOX_DESC_record_srcfile,
  NETWOX_DESC_record_dstfile,
  NETWOX_DESC_record_encode,
  NETWOX_DESC_toolpriv_none,
  NULL
};
netwox_toolarg t000020_args[] = {
  NETWOX_TOOLARG_REQ_BUF_FILE_RD('f', NULL, "input record file", NULL),
  NETWOX_TOOLARG_REQ_BUF_FILE_WR('F', NULL, "output record file", NULL),
  NETWOX_TOOLARG_OPT_RECORDENCODE('r', "recordencode",
                                  "encoding type for output record", NULL),
  NETWOX_TOOLARG_REQ_BUF('p', "pattern", "searched pattern", NULL),
  NETWOX_TOOLARG_OPT_BOOL('c', "case", "case sensitive", NULL),
  NETWOX_TOOLARG_RADIO1_SET('S', "string", "search string"),
  NETWOX_TOOLARG_RADIO1('M', "mixed", "search mixed string"),
  NETWOX_TOOLARG_RADIO1('R', "regexp", "search regular expression"),
  NETWOX_TOOLARG_END
};
netwox_tooltreenodetype t000020_nodes[] = {
  NETWOX_TOOLTREENODETYPE_RECORD,
  NETWOX_TOOLTREENODETYPE_END
};
netwox_tool_info t000020_info = {
  "Search for strings in packets from a record",
  t000020_description,
  "capture, find, match, ngrep, payload, text",
  t000020_args,
  t000020_nodes,
};

/*-------------------------------------------------------------*/
typedef enum {
  T000020_SEARCHTYPE_STRING,
  T000020_SEARCHTYPE_MIXED,
  T000020_SEARCHTYPE_REGEXP
} t000020_searchtype;
typedef struct {
  t000020_searchtype searchtype;
  netwib_bool casesensitive;
  netwib_buf buf;
} t000020_search;

/*-------------------------------------------------------------*/
static netwib_err t000020_match(netwib_constbuf *ppkt,
                                t000020_search *psearch,
                                netwib_buf *ppkt2,
                                netwib_bool *pyes)
{
  netwib_data pc;
  netwib_uint32 pktsize;
  netwib_err ret;

  *pyes = NETWIB_FALSE;
  switch(psearch->searchtype) {
  case T000020_SEARCHTYPE_STRING :
  case T000020_SEARCHTYPE_MIXED :
    if (psearch->casesensitive) {
      pc = netwib_c_memmem(netwib__buf_ref_data_ptr(ppkt),
                           netwib__buf_ref_data_size(ppkt),
                           netwib__buf_ref_data_ptr(&psearch->buf),
                           netwib__buf_ref_data_size(&psearch->buf));
    } else {
      pc = netwib_c_memcasemem(netwib__buf_ref_data_ptr(ppkt),
                               netwib__buf_ref_data_size(ppkt),
                               netwib__buf_ref_data_ptr(&psearch->buf),
                               netwib__buf_ref_data_size(&psearch->buf));
    }
    if (pc != NULL) *pyes = NETWIB_TRUE;
    break;
  case T000020_SEARCHTYPE_REGEXP :
    netwib__buf_reinit(ppkt2);
    netwib_er(netwib_buf_append_buf(ppkt, ppkt2));
    /* we have to replace '\0' by ' ' */
    pc = netwib__buf_ref_data_ptr(ppkt2);
    pktsize = netwib__buf_ref_data_size(ppkt2);
    while (pktsize) {
      if (*pc == '\0') {
        *pc = ' ';
      }
      pc++;
      pktsize--;
    }
    ret = netwib_buf_search_re(ppkt2, &psearch->buf, psearch->casesensitive,
                               NULL);
    if (ret == NETWIB_ERR_OK) *pyes = NETWIB_TRUE;
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err t000020_loop(netwib_constbuf *pfilenamein,
                               netwib_constbuf *pfilenameout,
                               netwib_record_encodetype encodetype,
                               t000020_search *psearch)
{
  netwib_buf pkt, pkt2;
  netwib_io *pioin, *pioout;
  netwib_bool yes;
  netwib_err ret;

  /* initialize records */
  netwib_er(netwib_io_init_record_read(pfilenamein, &pioin));
  ret = netwib_io_init_record_write(pfilenameout, encodetype, &pioout);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_io_close(&pioin));
    return(ret);
  }

  /* main loop */
  netwib_er(netwib_buf_init_mallocdefault(&pkt));
  netwib_er(netwib_buf_init_mallocdefault(&pkt2));
  while (NETWIB_TRUE) {
    /* read packet */
    netwib__buf_reinit(&pkt);
    ret = netwib_io_read(pioin, &pkt);
    if (ret == NETWIB_ERR_DATANOTAVAIL) {
      ret = NETWIB_ERR_OK;
      continue;
    } else if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    /* write packet */
    netwib_er(t000020_match(&pkt, psearch, &pkt2, &yes));
    if (yes) {
      netwib_er(netwib_io_write(pioout, &pkt));
    }
  }

  /* close */
  netwib_er(netwib_buf_close(&pkt2));
  netwib_er(netwib_buf_close(&pkt));
  netwib_er(netwib_io_close(&pioin));
  netwib_er(netwib_io_close(&pioout));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err t000020_core(int argc, char *argv[])
{
  netwox_arg *parg;
  netwib_buf filenamein, filenameout, buf;
  netwib_record_encodetype encodetype;
  t000020_search search;
  netwib_char c;

  /* obtain parameters */
  netwib_er(netwox_arg_init(argc, argv, &t000020_info, &parg));
  netwib_er(netwox_arg_buf(parg, 'f', &filenamein));
  netwib_er(netwox_arg_buf(parg, 'F', &filenameout));
  netwib_er(netwox_arg_recordencode(parg, 'r', &encodetype));
  netwib_er(netwox_arg_buf(parg, 'p', &buf));
  netwib_er(netwox_arg_bool(parg, 'c', &search.casesensitive));
  netwib_er(netwox_arg_radio1(parg, &c));
  switch(c) {
    case 'S' :
      search.searchtype = T000020_SEARCHTYPE_STRING;
      search.buf = buf;
      break;
    case 'M' :
      search.searchtype = T000020_SEARCHTYPE_MIXED;
      netwib_er(netwib_buf_init_mallocdefault(&search.buf));
      netwib_er(netwib_buf_decode(&buf, NETWIB_DECODETYPE_MIXED, &search.buf));
      break;
    case 'R' :
      search.searchtype = T000020_SEARCHTYPE_REGEXP;
      search.buf = buf;
      break;
  }

  /* main loop */
  netwib_er(t000020_loop(&filenamein, &filenameout, encodetype, &search));

  /* close */
  if (search.searchtype == T000020_SEARCHTYPE_MIXED) {
    netwib_er(netwib_buf_close(&search.buf));
  }
  netwib_er(netwox_arg_close(&parg));

  return(NETWIB_ERR_OK);
}
