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
static netwib_err netwox_priv_cmdline_init(netwib_buf *pbufcommand,
                                           netwib_string *pitem)
{
  netwib_uint32 datasize;
  netwib_buf buf;
  netwib_err ret;

  datasize = netwib__buf_ref_data_size(pbufcommand) + 1;
  netwib_er(netwib_ptr_malloc(datasize, (netwib_ptr*)pitem));
  netwib_er(netwib_buf_init_ext_arrayempty(*pitem, datasize, &buf));

  ret = netwib_buf_decode_quoted(pbufcommand, &buf);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free((netwib_ptr*)pitem));
    return(ret);
  }
  netwib_er(netwib_buf_append_byte('\0', &buf));

  datasize = netwib__buf_ref_data_size(&buf);
  netwib_er(netwib_ptr_realloc(datasize, (netwib_ptr*)pitem));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_cmdline_init(netwib_constbuf *pbufcommand,
                               netwib_string *pfilename,
                               int *pargc,
                               netwib_string *pargv[])
{
  netwib_buf bufcommand;
  netwib_string filename, pcfind1, pcfind2;
  netwib_string *argv;
  netwib_uint32 argvmaxsize, i;
  netwib_err ret;

  bufcommand = *pbufcommand;

  /* obtain filename */
  netwib_er(netwox_priv_cmdline_init(&bufcommand, &filename));

  /* allocate argv array */
  argvmaxsize = 10;
  netwib_er(netwib_ptr_malloc(argvmaxsize*sizeof(netwib_string*),
                              (netwib_ptr*)&argv));

  /* set argv[0] */
  netwib_er(netwib_ptr_malloc(netwib_c_strlen(filename)+1,
                              (netwib_ptr*)&(argv[0])));
  pcfind1 = netwib_c_strrchr(filename, '/');
  pcfind2 = netwib_c_strrchr(filename, '\\');
  if (pcfind1 == NULL && pcfind2 == NULL) {
    netwib_c_strcpy(argv[0], filename);
  } else if (pcfind1 == NULL) {
    netwib_c_strcpy(argv[0], pcfind2+1);
  } else if (pcfind2 == NULL) {
    netwib_c_strcpy(argv[0], pcfind1+1);
  } else {
    if (pcfind1 <= pcfind2) {
      netwib_c_strcpy(argv[0], pcfind2+1);
    } else {
      netwib_c_strcpy(argv[0], pcfind1+1);
    }
  }
  netwib_er(netwib_ptr_realloc(netwib_c_strlen(argv[0])+1,
                               (netwib_ptr*)&(argv[0])));

  /* set next args */
  i = 1;
  while (NETWIB_TRUE) {
    /* might need to enlarge argv */
    if (i == argvmaxsize-1) {
      argvmaxsize += 10;
      netwib_er(netwib_ptr_realloc(argvmaxsize*sizeof(netwib_string*),
                                   (netwib_ptr*)&argv));
    }
    /* store item */
    ret = netwox_priv_cmdline_init(&bufcommand, &(argv[i]));
    if (ret != NETWIB_ERR_OK) {
      if (ret == NETWIB_ERR_DATAEND) {
        break;
      }
      argv[i] = NULL; /* to indicate stop in netwib_priv_cmdline_close */
      netwib_er(netwox_cmdline_close(&filename, &argv));
      return(ret);
    }
    i++;
  }

  /* set end */
  argv[i] = NULL;

  *pfilename = filename;
  if (pargc != NULL) *pargc = i;
  *pargv = argv;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_cmdline_close(netwib_string *pfilename,
                                netwib_string *pargv[])
{
  int i;

  netwib_er(netwib_ptr_free((netwib_ptr*)pfilename));

  i = 0;
  while ((*pargv)[i] != NULL) {
    netwib_er(netwib_ptr_free((netwib_ptr*)&((*pargv)[i])));
    i++;
  }
  netwib_er(netwib_ptr_free((netwib_ptr*)pargv));

  return(NETWIB_ERR_OK);
}
