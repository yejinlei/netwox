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
netwib_err netwib_filename_exists(netwib_constbuf *pfilename,
                                  netwib_bool *pyes)
{
  netwib_pathstat pathstat;
  netwib_bool exists;
  netwib_err ret;

  exists = NETWIB_FALSE;
  ret = netwib_priv_stat_init_pathname(pfilename, &pathstat);
  if (ret == NETWIB_ERR_OK) {
    if (pathstat.type == NETWIB_PATHSTAT_TYPE_REG ||
        pathstat.type == NETWIB_PATHSTAT_TYPE_LINK) {
      exists = NETWIB_TRUE;
    } else {
      exists = NETWIB_FALSE;
    }
  } else {
    exists = NETWIB_FALSE;
  }

  if (pyes != NULL) {
    *pyes = exists;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_filename_size(netwib_constbuf *pfilename,
                                netwib_uint32 *psize)
{
  netwib_pathstat pathstat;

  netwib_er(netwib_priv_stat_init_pathname(pfilename, &pathstat));
  if (pathstat.type != NETWIB_PATHSTAT_TYPE_REG) {
    return(NETWIB_ERR_PAFILENOTREG);
  }
  if (pathstat.size == NETWIB_PATHSTAT_SIZE_GT2G) {
    return(NETWIB_ERR_PAFILE2G);
  }
  if (psize != NULL) *psize = pathstat.size;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_filename_create(netwib_constbuf *pfilename)
{ netwib_io *pio;

  netwib_er(netwib_priv_dir_create_parents(pfilename));
  netwib_er(netwib_io_init_file_write(pfilename, &pio));
  netwib_er(netwib_io_close(&pio));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_filename_remove(netwib_constbuf *pfilename)
{
  netwib_string filename;
  int reti, errnocopy;

  netwib__constbuf_ref_string(pfilename, filename, bufstorage,
                              netwib_filename_remove(&bufstorage));

  reti = netwib_c_unlink(filename);
  if (reti == -1) {
    errnocopy = errno;
    netwib_er(netwib_priv_errmsg_string("cannot remove this file: "));
    netwib_er(netwib_priv_errmsg_append_buf(pfilename));
    if (errnocopy == ENOENT) {
      return(NETWIB_ERR_NOTFOUND);
    }
    return(NETWIB_ERR_FUUNLINK);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_filename_rename(netwib_constbuf *poldfilename,
                                  netwib_constbuf *pnewfilename)
{
  netwib_string oldfilename, newfilename;
  int reti, errnocopy;

  netwib__constbuf_ref_string(poldfilename, oldfilename, bufstorage,
                              netwib_filename_rename(&bufstorage, pnewfilename));
  netwib__constbuf_ref_string(pnewfilename, newfilename, bufstorage,
                              netwib_filename_rename(poldfilename, &bufstorage));

  netwib_er(netwib_priv_dir_create_parents(pnewfilename));
  reti = rename(oldfilename, newfilename);
  if (reti == -1) {
    errnocopy = errno;
#if defined NETWIBDEF_SYSNAME_Windows
    if (errnocopy == EACCES || errnocopy == EEXIST) {
      /* means newfilename exists : try to delete it */
      reti = netwib_c_unlink(newfilename);
      if (reti == 0) {
        reti = rename(oldfilename, newfilename);
        if (reti == 0) {
          return(NETWIB_ERR_OK);
        }
      }
    }
#endif
    netwib_er(netwib_priv_errmsg_string("cannot rename this file: "));
    netwib_er(netwib_priv_errmsg_append_buf(poldfilename));
    if (errnocopy == ENOENT) {
      return(NETWIB_ERR_NOTFOUND);
    }
    return(NETWIB_ERR_FURENAME);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_filename_copy(netwib_constbuf *poldfilename,
                                netwib_constbuf *pnewfilename)
{
  netwib_io *prdio, *pwrio;
  netwib_err ret;
  netwib_buf bufdata;

  netwib_er(netwib_io_init_file_read(poldfilename, &prdio));
  ret = netwib_priv_dir_create_parents(pnewfilename);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_io_close(&prdio));
    return(ret);
  }
  ret = netwib_io_init_file_write(pnewfilename, &pwrio);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_io_close(&prdio));
    return(ret);
  }

  netwib_er(netwib_buf_init_mallocdefault(&bufdata));
  while (NETWIB_TRUE) {
    ret = netwib_io_read(prdio, &bufdata);
    if (ret == NETWIB_ERR_DATAEND) {
      ret = NETWIB_ERR_OK;
      break;
    } else if (ret != NETWIB_ERR_OK) {
      break;
    }
    ret = netwib_io_write(pwrio, &bufdata);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
    netwib__buf_reinit(&bufdata);
  }

  netwib_er(netwib_buf_close(&bufdata));
  netwib_er(netwib_io_close(&pwrio));
  netwib_er(netwib_io_close(&prdio));

  return(ret);
}
