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
typedef struct {
  netwib_io *pnextio;
} netwox_priv_io_conffile;

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_io_conffile_init(netwib_constbuf *pfilename,
                                               netwib_bool *preadinitialized,
                                               netwib_bool *pwriteinitialized,
                                               netwox_priv_io_conffile *ptr)
{
  netwib_io *piofile;

  netwib_er(netwib_io_init_file_textread(pfilename, &piofile));

  netwib_er(netwib_io_init_data_line(&ptr->pnextio));
  netwib_er(netwib_io_plug_read(ptr->pnextio, piofile));

  *preadinitialized = NETWIB_TRUE;
  *pwriteinitialized = NETWIB_TRUE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_io_conffile_close(netwox_priv_io_conffile *ptr)
{

  netwib_er(netwib_io_close(&ptr->pnextio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_io_conffile_read(netwib_io *pio,
                                               netwib_buf *pbuf)
{
  netwox_priv_io_conffile *ptr = (netwox_priv_io_conffile *)pio->pcommon;
  netwib_string pc;
  netwib_uint32 oldsize;

  oldsize = netwib__buf_ref_data_size(pbuf);
  while (NETWIB_TRUE) {
    pbuf->endoffset = pbuf->beginoffset + oldsize;
    netwib_er(netwib_io_read(ptr->pnextio, pbuf));
    netwib_er(netwib_buf_ref_string(pbuf, &pc));
    /* check empty lines and comments */
    pc += oldsize;
    while(*pc != '\0') {
      if (*pc != ' ' && *pc != '\t') {
        break;
      }
      pc++;
    }
    if (*pc == '#' || *pc == '\0') {
      /* skip this line */
      continue;
    }
    break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_io_conffile_fclose(netwib_io *pio)
{
  netwox_priv_io_conffile *ptr = (netwox_priv_io_conffile *)pio->pcommon;
  netwib_er(netwox_priv_io_conffile_close(ptr));
  netwib_er(netwib_ptr_free(&pio->pcommon));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_io_init_conffile_read(netwib_constbuf *pfilename,
                                        netwib_io **ppio)
{
  netwib_bool rdsup, wrsup;
  netwib_ptr pcommon;
  netwib_err ret;

  netwib_er(netwib_ptr_malloc(sizeof(netwox_priv_io_conffile), &pcommon));
  ret = netwox_priv_io_conffile_init(pfilename, &rdsup, &wrsup,
                                     (netwox_priv_io_conffile *)pcommon);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_ptr_free(&pcommon));
    return(ret);
  }

  netwib_er(netwib_io_init(rdsup, wrsup,
                           pcommon,
                           &netwox_priv_io_conffile_read,
                           NULL, NULL, NULL, NULL, NULL,
                           &netwox_priv_io_conffile_fclose,
                           ppio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_conffile_read_var(netwib_bufpool *pbufpool,
                                    netwib_io *pio,
                                    netwib_buf *pvar,
                                    netwib_buf *pvalbuf,
                                    netwib_bool *pvaluintdecoded,
                                    netwib_uint32 *pvaluint)
{
  netwib_data data, pc;
  netwib_uint32 datasize, varsize, ui;
  netwib_buf *pbuf, var, val, *pval;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));

  /* read */
  ret = netwib_io_read(pio, pbuf);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));
    return(ret);
  }

  /* pbuf should contain "var = val ...[ignored]..." */
  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);
  pc = netwib_c_memchr(data, '=', datasize);
  if (pc == NULL) {
    netwib_er(netwib_fmt_display("Line does not contain '=' :\n  %{buf}",
                                 pbuf));
    netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));
    return(NETWIB_ERR_NOTCONVERTED);
  }
  varsize = pc - data;
  netwib_er(netwib_buf_init_ext_arrayfilled(data, varsize, &var));
  netwib_er(netwib_buf_init_ext_arrayfilled(data + varsize + 1,
                                            datasize - varsize - 1, &val));

  /* decode var */
  ret = netwib_buf_decode_quoted(&var, pvar);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_fmt_display("Variable name is incorrect :\n  %{buf}",
                                 pbuf));
    netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));
    return(ret);
  }
  if (netwib__buf_ref_data_size(&var)) {
    netwib_er(netwib_fmt_display("There is forbidden data between variable name and '=' :\n  %{buf}", pbuf));
    netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));
    return(NETWIB_ERR_NOTCONVERTED);
  }

  /* decode val */
  netwib_er(netwib_bufpool_buf_init(pbufpool, &pval));
  ret = netwib_buf_decode_quoted(&val, pval);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_fmt_display("Variable value is incorrect :\n  %{buf}",
                                 pbuf));
    netwib_er(netwib_bufpool_buf_close(pbufpool, &pval));
    netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));
    return(ret);
  }
  netwib_er(netwib_buf_append_buf(pval, pvalbuf));

  /* try to decode integers */
  if (pvaluintdecoded != NULL) *pvaluintdecoded = NETWIB_FALSE;
  ret = netwib_buf_decode_fmt(pval, "%{uint32}%$", &ui);
  if (ret == NETWIB_ERR_OK) {
    if (pvaluint != NULL) *pvaluint = ui;
    if (pvaluintdecoded != NULL) *pvaluintdecoded = NETWIB_TRUE;
  }

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pval));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_conffile_read_concat(netwib_constbuf *pfilename,
                                       netwib_buf *pbuf)
{
  netwib_io *pio;
  netwib_err ret=NETWIB_ERR_OK;

  netwib_er(netwox_io_init_conffile_read(pfilename, &pio));
  while(NETWIB_TRUE) {
    ret = netwib_io_read(pio, pbuf);
    if (ret == NETWIB_ERR_OK) {
      netwib_er(netwib_buf_append_byte(' ', pbuf));
    } else if (ret != NETWIB_ERR_DATANOTAVAIL) {
      if (ret == NETWIB_ERR_DATAEND) ret = NETWIB_ERR_OK;
      break;
    }
    ret = NETWIB_ERR_OK;
  }
  netwib_er(netwib_io_close(&pio));

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_conffile_write_help(netwib_bufpool *pbufpool,
                                      netwib_io *pio)
{
  netwib_buf *pbuf;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));

  netwib_er(netwib_buf_append_string("#The format of this file is :\n", pbuf));
  netwib_er(netwib_buf_append_string("# # comment\n", pbuf));
  netwib_er(netwib_buf_append_string("# variablename=value\n", pbuf));
  netwib_er(netwib_buf_append_string("# variablename=\"value\"\n", pbuf));
  netwib_er(netwib_buf_append_string("# variablename=\" \\\\ \\\"\"\n", pbuf));
  netwib_er(netwib_buf_append_string("\n", pbuf));

  netwib_er(netwib_io_write(pio, pbuf));

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_conffile_write_cmt(netwib_bufpool *pbufpool,
                                     netwib_io *pio,
                                     netwib_constbuf *pcmt)
{
  netwib_buf *pbuf;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));

  netwib_er(netwib_buf_append_string("# ", pbuf));
  netwib_er(netwib_buf_append_buf(pcmt, pbuf));
  netwib_er(netwib_buf_append_string("\n", pbuf));

  netwib_er(netwib_io_write(pio, pbuf));

  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_conffile_write__var(netwib_bufpool *pbufpool,
                                      netwib_io *pio,
                                      netwib_bool cmt,
                                      netwib_bool indented,
                                      netwib_constbuf *pvar,
                                      netwib_constbuf *pval)
{
  netwib_buf *pbuf;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));

  if (cmt) {
    netwib_er(netwib_buf_append_string("#", pbuf));
  }
  if (indented) {
    netwib_er(netwib_buf_append_string("  ", pbuf));
  }
  netwib_er(netwib_buf_append_buf(pvar, pbuf));
  netwib_er(netwib_buf_append_string(" = ", pbuf));
  netwib_er(netwib_buf_encode(pval, NETWIB_ENCODETYPE_QUOTED, pbuf));
  netwib_er(netwib_buf_append_string("\n", pbuf));

  netwib_er(netwib_io_write(pio, pbuf));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_conffile_write__var_uint32(netwib_bufpool *pbufpool,
                                             netwib_io *pio,
                                             netwib_bool cmt,
                                             netwib_bool indented,
                                             netwib_constbuf *pvar,
                                             netwib_uint32 val)
{
  netwib_buf *pbuf;

  netwib_er(netwib_bufpool_buf_init(pbufpool, &pbuf));
  if (cmt) {
    netwib_er(netwib_buf_append_string("#", pbuf));
  }
  if (indented) {
    netwib_er(netwib_buf_append_string("  ", pbuf));
  }
  netwib_er(netwib_buf_append_fmt(pbuf, "%{buf} = %{uint32}\n", pvar, val));
  netwib_er(netwib_io_write(pio, pbuf));
  netwib_er(netwib_bufpool_buf_close(pbufpool, &pbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_conffile_write_empty(netwib_io *pio)
{
  netwib_buf buf;

  netwib_er(netwib_buf_init_ext_string("\n", &buf));
  netwib_er(netwib_io_write(pio, &buf));

  return(NETWIB_ERR_OK);
}
