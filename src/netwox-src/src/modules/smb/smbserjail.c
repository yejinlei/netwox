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
static netwib_err netwox_smbserjail_case(netwox_smbser *psmbser,
                                         netwib_pathstat_type pathtype,
                                         netwib_buf *prealpathname)
{

#if defined NETWIBDEF_SYSNAME_Windows
  /* under Windows, this is not needed */
  return(NETWIB_ERR_OK);
#endif

  /* Windows 98 does not set this flag, but uses caseless pathnames, so
     we can't use this check
  if (!(psmbmsgq->hdr.flags & NETWOX_SMBMSG_FLAGS_CASELESSPATHNAMES)) [
    return(NETWIB_ERR_OK);
  ]
  */
  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Caseless:  %{buf}\n", prealpathname));
  }

  netwib_er(netwox_smbsercase_update(psmbser->pcommon->pbufpool, pathtype,
                                     prealpathname));

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("With case: %{buf}\n", prealpathname));
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwox_smbserjail_83(netwox_smbser *psmbser,
                                       netwib_constbuf *p83pathname,
                                       netwib_buf *plongpathname)
{
  /* Windows 98 does not set this flag, but uses long pathnames, so
     we can't use this check
  if (!(psmbmsgq->hdr.flags2 & NETWOX_SMBMSG_FLAGS2_USESLONGPATHNAME)) [
    netwib_er(netwib_buf_append_buf(p83pathname, plongpathname));
    return(NETWIB_ERR_OK);
  ]
  */
  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Pathname 8.3:  %{buf}\n", p83pathname));
  }

  netwib_er(netwox_smbser83_83tolong_path(&psmbser->smbser83, p83pathname,
                                          plongpathname));

  if (psmbser->pcommon->verbose) {
    netwib_er(netwib_fmt_display("Pathname long: %{buf}\n", plongpathname));
  }
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserjail_E(netwox_smbser *psmbser,
                               netwib_constbuf *prootdir,
                               netwib_pathstat_type pathtype,
                               netwib_constbuf *ppathname,
                               netwib_buf *prealpathname)
{
  netwib_buf b, *plongpathname;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool,
                                    &plongpathname));

  ret = NETWIB_ERR_OK;
  if (netwib__buf_ref_data_size(ppathname) != 0) {
    netwib_eg(netwox_smbserjail_83(psmbser, ppathname, plongpathname));
    netwib_eg(netwib_path_init_jail(prootdir, plongpathname, prealpathname));
    netwib_eg(netwox_smbserjail_case(psmbser, pathtype, prealpathname));
  } else {
    /* Windows use an empty filename to mean root directory */
    netwib_eg(netwib_buf_init_ext_string("/", &b));
    netwib_eg(netwib_path_init_jail(prootdir, &b, prealpathname));
  }

 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool,
                                     &plongpathname));
  if (ret != NETWIB_ERR_OK) {
    psmbser->smberrtosend = NETWOX_SMBERR_BADNAME;
  }
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserjail_rootfileid_E(netwox_smbser *psmbser,
                                          netwib_constbuf *prootdir,
                                          netwib_uint32 rootfileid,
                                          netwib_pathstat_type pathtype,
                                          netwib_constbuf *ppathname,
                                          netwib_buf *prealpathname)
{
  netwox_smbserfid_item *pitem;
  netwib_buf *prootfile, *ppathname2;
  netwib_err ret;

  /* simple case */
  if (rootfileid == 0) {
    return(netwox_smbserjail_E(psmbser, prootdir, pathtype, ppathname,
                               prealpathname));
  }

  /* init */
  ret = NETWIB_ERR_OK;
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &prootfile));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool, &ppathname2));

  /* concatenate */
  netwib_eg(netwox_smbserfid_search(&psmbser->smbserfid, (netwib_uint16)rootfileid, &pitem));
  netwib_eg(netwox_smbserfid_item_svcname(prootdir, pitem, prootfile));
  netwib_eg(netwib_path_init_concat(prootfile, ppathname, ppathname2));
  netwib_eg(netwox_smbserjail_E(psmbser, prootdir, pathtype, ppathname2,
                                prealpathname));

 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &ppathname2));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool, &prootfile));
  if (ret != NETWIB_ERR_OK) {
    psmbser->smberrtosend = NETWOX_SMBERR_BADNAME;
  }
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwox_smbserjailglob_E(netwox_smbser *psmbser,
                                   netwib_constbuf *prootdir,
                                   netwox_smbcmdcmn_fileattr32 searchattributes,
                                   netwib_constbuf *psearchpattern,
                                   netwox_smbserglob_item *pitem)
{
  netwib_buf *plongpathname, *prealpathname, *pparentdir, *pchildglob;
  netwib_err ret;

  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool,
                                    &plongpathname));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool,
                                    &prealpathname));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool,
                                    &pparentdir));
  netwib_er(netwib_bufpool_buf_init(psmbser->pcommon->pbufpool,
                                    &pchildglob));

  ret = NETWIB_ERR_OK;

  netwib_eg(netwox_smbserjail_83(psmbser, psearchpattern, plongpathname));
  netwib_eg(netwib_path_init_jail(prootdir, plongpathname, prealpathname));
  netwib_eg(netwib_path_decode_parent(prealpathname, pparentdir));
  if (psmbser->pcommon->verbose) {
    netwib_eg(netwib_fmt_display("Work on parent directory: %{buf}\n",
                                 pparentdir));
  }
  netwib_eg(netwox_smbserjail_case(psmbser, NETWIB_PATHSTAT_TYPE_DIR,
                                   pparentdir));
  netwib_eg(netwib_path_decode_child(prealpathname, pchildglob));
  netwib_eg(netwox_smbserglob_item_set(pitem, searchattributes, pparentdir,
                                       pchildglob));

 netwib_gotolabel:
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool,
                                     &pchildglob));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool,
                                     &pparentdir));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool,
                                     &prealpathname));
  netwib_er(netwib_bufpool_buf_close(psmbser->pcommon->pbufpool,
                                     &plongpathname));
  if (ret != NETWIB_ERR_OK) {
    psmbser->smberrtosend = NETWOX_SMBERR_BADNAME;
  }
  return(ret);
}
