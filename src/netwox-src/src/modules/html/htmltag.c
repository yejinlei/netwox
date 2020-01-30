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
netwib_err netwox_htmltag_elmt_init_buf(netwib_constbuf *pbuf,
                                        netwox_htmltag_elmt *pelmt)
{
  netwib_data data;
  netwib_uint32 datasize;
  netwib_byte firstchar;

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);

  if (datasize == 0) {
    if (pelmt != NULL) *pelmt = NETWOX_HTMLTAG_ELMT_UNKNOWN;
    return(NETWIB_ERR_OK);
  }

  firstchar = data[0];
#define netwox_htmltag_elmt_init_buf_text(item,itemsize,v) if (datasize == itemsize) { if (!netwib_c_memcasecmp(data, (netwib_constdata)item, itemsize)) { if (pelmt != NULL) *pelmt = v; return(NETWIB_ERR_OK);}}
  switch(firstchar) {
  case 'a' :
  case 'A' :
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_A, 1,
                                      NETWOX_HTMLTAG_ELMT_A);
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_APPLET, 6,
                                      NETWOX_HTMLTAG_ELMT_APPLET);
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_AREA, 4,
                                      NETWOX_HTMLTAG_ELMT_AREA);
    break;
  case 'b' :
  case 'B' :
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_BASE, 4,
                                      NETWOX_HTMLTAG_ELMT_BASE);
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_BGSOUND, 7,
                                      NETWOX_HTMLTAG_ELMT_BGSOUND);
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_BLOCKQUOTE, 10,
                                      NETWOX_HTMLTAG_ELMT_BLOCKQUOTE);
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_BODY, 4,
                                      NETWOX_HTMLTAG_ELMT_BODY);
    break;
  case 'd' :
  case 'D' :
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_DEL, 3,
                                      NETWOX_HTMLTAG_ELMT_DEL);
    break;
  case 'e' :
  case 'E' :
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_EMBED, 5,
                                      NETWOX_HTMLTAG_ELMT_EMBED);
    break;
  case 'f' :
  case 'F' :
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_FIG, 3,
                                      NETWOX_HTMLTAG_ELMT_FIG);
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_FORM, 4,
                                      NETWOX_HTMLTAG_ELMT_FORM);
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_FRAME, 5,
                                      NETWOX_HTMLTAG_ELMT_FRAME);
    break;
  case 'h' :
  case 'H' :
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_HEAD, 4,
                                      NETWOX_HTMLTAG_ELMT_HEAD);
    break;
  case 'i' :
  case 'I' :
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_IFRAME, 6,
                                      NETWOX_HTMLTAG_ELMT_IFRAME);
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_ILAYER, 6,
                                      NETWOX_HTMLTAG_ELMT_ILAYER);
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_IMG, 3,
                                      NETWOX_HTMLTAG_ELMT_IMG);
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_INPUT, 5,
                                      NETWOX_HTMLTAG_ELMT_INPUT);
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_INS, 3,
                                      NETWOX_HTMLTAG_ELMT_INS);
    break;
  case 'l' :
  case 'L' :
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_LAYER, 5,
                                      NETWOX_HTMLTAG_ELMT_LAYER);
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_LINK, 4,
                                      NETWOX_HTMLTAG_ELMT_LINK);
    break;
  case 'm' :
  case 'M' :
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_META, 4,
                                      NETWOX_HTMLTAG_ELMT_META);
    break;
  case 'o' :
  case 'O' :
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_OBJECT, 6,
                                      NETWOX_HTMLTAG_ELMT_OBJECT);
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_OVERLAY, 7,
                                      NETWOX_HTMLTAG_ELMT_OVERLAY);
    break;
  case 'q' :
  case 'Q' :
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_Q, 1,
                                      NETWOX_HTMLTAG_ELMT_Q);
    break;
  case 's' :
  case 'S' :
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_SCRIPT, 6,
                                      NETWOX_HTMLTAG_ELMT_SCRIPT);
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_SELECT, 6,
                                      NETWOX_HTMLTAG_ELMT_SELECT);
    break;
  case 't' :
  case 'T' :
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_TABLE, 5,
                                      NETWOX_HTMLTAG_ELMT_TABLE);
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_TD, 2,
                                      NETWOX_HTMLTAG_ELMT_TD);
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_TH, 2,
                                      NETWOX_HTMLTAG_ELMT_TH);
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_TR, 2,
                                      NETWOX_HTMLTAG_ELMT_TR);
    break;
  case 'x' :
  case 'X' :
    netwox_htmltag_elmt_init_buf_text(NETWOX_HTMLTAG_ELMTNAME_XML, 3,
                                      NETWOX_HTMLTAG_ELMT_XML);
    break;
  }

  if (pelmt != NULL) *pelmt = NETWOX_HTMLTAG_ELMT_UNKNOWN;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_htmltag_elmt_init_elmtattr(netwox_htmltag_elmtattr elmtattr,
                                             netwox_htmltag_elmt *pelmt)
{
  netwox_htmltag_elmt elmt;

  switch(elmtattr) {
  case NETWOX_HTMLTAG_ELMTATTR_A_HREF :
    elmt = NETWOX_HTMLTAG_ELMT_A;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_APPLET_ARCHIVE :
  case NETWOX_HTMLTAG_ELMTATTR_APPLET_CODE :
  case NETWOX_HTMLTAG_ELMTATTR_APPLET_CODEBASE :
    elmt = NETWOX_HTMLTAG_ELMT_APPLET;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_AREA_HREF :
    elmt = NETWOX_HTMLTAG_ELMT_AREA;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_BASE_HREF :
    elmt = NETWOX_HTMLTAG_ELMT_BASE;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_BGSOUND_SRC :
    elmt = NETWOX_HTMLTAG_ELMT_BGSOUND;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_BLOCKQUOTE_CITE :
    elmt = NETWOX_HTMLTAG_ELMT_BLOCKQUOTE;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_BODY_BACKGROUND :
    elmt = NETWOX_HTMLTAG_ELMT_BODY;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_DEL_CITE :
    elmt = NETWOX_HTMLTAG_ELMT_DEL;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_EMBED_PLUGINSPAGE :
  case NETWOX_HTMLTAG_ELMTATTR_EMBED_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_EMBED_HREF :
    elmt = NETWOX_HTMLTAG_ELMT_EMBED;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_FIG_IMAGEMAP :
  case NETWOX_HTMLTAG_ELMTATTR_FIG_SRC :
    elmt = NETWOX_HTMLTAG_ELMT_FIG;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_FORM_ACTION :
    elmt = NETWOX_HTMLTAG_ELMT_FORM;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_FRAME_LONGDESC :
  case NETWOX_HTMLTAG_ELMTATTR_FRAME_SRC :
    elmt = NETWOX_HTMLTAG_ELMT_FRAME;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_HEAD_PROFILE :
    elmt = NETWOX_HTMLTAG_ELMT_HEAD;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_IFRAME_LONGDESC :
  case NETWOX_HTMLTAG_ELMTATTR_IFRAME_SRC :
    elmt = NETWOX_HTMLTAG_ELMT_IFRAME;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_ILAYER_BACKGROUND :
  case NETWOX_HTMLTAG_ELMTATTR_ILAYER_SRC :
    elmt = NETWOX_HTMLTAG_ELMT_ILAYER;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_IMG_DYNSRC :
  case NETWOX_HTMLTAG_ELMTATTR_IMG_LONGDESC :
  case NETWOX_HTMLTAG_ELMTATTR_IMG_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_IMG_USEMAP :
    elmt = NETWOX_HTMLTAG_ELMT_IMG;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_INPUT_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_INPUT_USEMAP :
    elmt = NETWOX_HTMLTAG_ELMT_INPUT;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_INS_CITE :
    elmt = NETWOX_HTMLTAG_ELMT_INS;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_LAYER_BACKGROUND :
  case NETWOX_HTMLTAG_ELMTATTR_LAYER_SRC :
    elmt = NETWOX_HTMLTAG_ELMT_LAYER;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_LINK_HREF :
    elmt = NETWOX_HTMLTAG_ELMT_LINK;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_META_HTTPEQUIV :
  case NETWOX_HTMLTAG_ELMTATTR_META_REFRESHCONTENT :
    elmt = NETWOX_HTMLTAG_ELMT_META;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_OBJECT_ARCHIVE :
  case NETWOX_HTMLTAG_ELMTATTR_OBJECT_CLASSID :
  case NETWOX_HTMLTAG_ELMTATTR_OBJECT_CODEBASE :
  case NETWOX_HTMLTAG_ELMTATTR_OBJECT_DATA :
  case NETWOX_HTMLTAG_ELMTATTR_OBJECT_USEMAP :
    elmt = NETWOX_HTMLTAG_ELMT_OBJECT;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_OVERLAY_IMAGEMAP :
  case NETWOX_HTMLTAG_ELMTATTR_OVERLAY_SRC :
    elmt = NETWOX_HTMLTAG_ELMT_OVERLAY;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_Q_CITE :
    elmt = NETWOX_HTMLTAG_ELMT_Q;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_SCRIPT_FOR :
  case NETWOX_HTMLTAG_ELMTATTR_SCRIPT_SRC :
    elmt = NETWOX_HTMLTAG_ELMT_SCRIPT;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_SELECT_SRC :
    elmt = NETWOX_HTMLTAG_ELMT_SELECT;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_TABLE_BACKGROUND :
    elmt = NETWOX_HTMLTAG_ELMT_TABLE;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_TD_BACKGROUND :
    elmt = NETWOX_HTMLTAG_ELMT_TD;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_TH_BACKGROUND :
    elmt = NETWOX_HTMLTAG_ELMT_TH;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_TR_BACKGROUND :
    elmt = NETWOX_HTMLTAG_ELMT_TR;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_XML_SRC :
    elmt = NETWOX_HTMLTAG_ELMT_XML;
    break;
  default :
    elmt = NETWOX_HTMLTAG_ELMT_UNKNOWN;
    break;
  }

  if (pelmt != NULL) *pelmt = elmt;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_htmltag_buf_append_elmt(netwox_htmltag_elmt elmt,
                                          netwib_buf *pbuf)
{
  netwib_conststring pc = NULL;

  switch(elmt) {
  case NETWOX_HTMLTAG_ELMT_A :
    pc = NETWOX_HTMLTAG_ELMTNAME_A;
    break;
  case NETWOX_HTMLTAG_ELMT_APPLET :
    pc = NETWOX_HTMLTAG_ELMTNAME_APPLET;
    break;
  case NETWOX_HTMLTAG_ELMT_AREA :
    pc = NETWOX_HTMLTAG_ELMTNAME_AREA;
    break;
  case NETWOX_HTMLTAG_ELMT_BASE :
    pc = NETWOX_HTMLTAG_ELMTNAME_BASE;
    break;
  case NETWOX_HTMLTAG_ELMT_BGSOUND :
    pc = NETWOX_HTMLTAG_ELMTNAME_BGSOUND;
    break;
  case NETWOX_HTMLTAG_ELMT_BLOCKQUOTE :
    pc = NETWOX_HTMLTAG_ELMTNAME_BLOCKQUOTE;
    break;
  case NETWOX_HTMLTAG_ELMT_BODY :
    pc = NETWOX_HTMLTAG_ELMTNAME_BODY;
    break;
  case NETWOX_HTMLTAG_ELMT_DEL :
    pc = NETWOX_HTMLTAG_ELMTNAME_DEL;
    break;
  case NETWOX_HTMLTAG_ELMT_EMBED :
    pc = NETWOX_HTMLTAG_ELMTNAME_EMBED;
    break;
  case NETWOX_HTMLTAG_ELMT_FIG :
    pc = NETWOX_HTMLTAG_ELMTNAME_FIG;
    break;
  case NETWOX_HTMLTAG_ELMT_FORM :
    pc = NETWOX_HTMLTAG_ELMTNAME_FORM;
    break;
  case NETWOX_HTMLTAG_ELMT_FRAME :
    pc = NETWOX_HTMLTAG_ELMTNAME_FRAME;
    break;
  case NETWOX_HTMLTAG_ELMT_HEAD :
    pc = NETWOX_HTMLTAG_ELMTNAME_HEAD;
    break;
  case NETWOX_HTMLTAG_ELMT_IFRAME :
    pc = NETWOX_HTMLTAG_ELMTNAME_IFRAME;
    break;
  case NETWOX_HTMLTAG_ELMT_ILAYER :
    pc = NETWOX_HTMLTAG_ELMTNAME_ILAYER;
    break;
  case NETWOX_HTMLTAG_ELMT_IMG :
    pc = NETWOX_HTMLTAG_ELMTNAME_IMG;
    break;
  case NETWOX_HTMLTAG_ELMT_INPUT :
    pc = NETWOX_HTMLTAG_ELMTNAME_INPUT;
    break;
  case NETWOX_HTMLTAG_ELMT_INS :
    pc = NETWOX_HTMLTAG_ELMTNAME_INS;
    break;
  case NETWOX_HTMLTAG_ELMT_LAYER :
    pc = NETWOX_HTMLTAG_ELMTNAME_LAYER;
    break;
  case NETWOX_HTMLTAG_ELMT_LINK :
    pc = NETWOX_HTMLTAG_ELMTNAME_LINK;
    break;
  case NETWOX_HTMLTAG_ELMT_META :
    pc = NETWOX_HTMLTAG_ELMTNAME_META;
    break;
  case NETWOX_HTMLTAG_ELMT_OBJECT :
    pc = NETWOX_HTMLTAG_ELMTNAME_OBJECT;
    break;
  case NETWOX_HTMLTAG_ELMT_OVERLAY :
    pc = NETWOX_HTMLTAG_ELMTNAME_OVERLAY;
    break;
  case NETWOX_HTMLTAG_ELMT_Q :
    pc = NETWOX_HTMLTAG_ELMTNAME_Q;
    break;
  case NETWOX_HTMLTAG_ELMT_SCRIPT :
    pc = NETWOX_HTMLTAG_ELMTNAME_SCRIPT;
    break;
  case NETWOX_HTMLTAG_ELMT_SELECT :
    pc = NETWOX_HTMLTAG_ELMTNAME_SELECT;
    break;
  case NETWOX_HTMLTAG_ELMT_TABLE :
    pc = NETWOX_HTMLTAG_ELMTNAME_TABLE;
    break;
  case NETWOX_HTMLTAG_ELMT_TD :
    pc = NETWOX_HTMLTAG_ELMTNAME_TD;
    break;
  case NETWOX_HTMLTAG_ELMT_TH :
    pc = NETWOX_HTMLTAG_ELMTNAME_TH;
    break;
  case NETWOX_HTMLTAG_ELMT_TR :
    pc = NETWOX_HTMLTAG_ELMTNAME_TR;
    break;
  case NETWOX_HTMLTAG_ELMT_XML :
    pc = NETWOX_HTMLTAG_ELMTNAME_XML;
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  netwib_er(netwib_buf_append_string(pc, pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_htmltag_attr_init_buf(netwib_constbuf *pbuf,
                                        netwox_htmltag_attr *pattr)
{
  netwib_data data;
  netwib_uint32 datasize;
  netwib_byte firstchar;

  data = netwib__buf_ref_data_ptr(pbuf);
  datasize = netwib__buf_ref_data_size(pbuf);

  if (datasize == 0) {
    if (pattr != NULL) *pattr = NETWOX_HTMLTAG_ATTR_UNKNOWN;
    return(NETWIB_ERR_OK);
  }

  firstchar = data[0];
#define netwox_htmltag_attr_init_buf_text(item,itemsize,v) if (datasize == itemsize) { if (!netwib_c_memcasecmp(data, (netwib_constdata)item, itemsize)) { if (pattr != NULL) *pattr = v; return(NETWIB_ERR_OK);}}
  switch(firstchar) {
  case 'a' :
  case 'A' :
    netwox_htmltag_attr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_ACTION, 6,
                                      NETWOX_HTMLTAG_ATTR_ACTION);
    netwox_htmltag_attr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_ARCHIVE, 7,
                                      NETWOX_HTMLTAG_ATTR_ARCHIVE);
    break;
  case 'b' :
  case 'B' :
    netwox_htmltag_attr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_BACKGROUND, 10,
                                      NETWOX_HTMLTAG_ATTR_BACKGROUND);
    break;
  case 'c' :
  case 'C' :
    netwox_htmltag_attr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_CITE, 4,
                                      NETWOX_HTMLTAG_ATTR_CITE);
    netwox_htmltag_attr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_CLASSID, 7,
                                      NETWOX_HTMLTAG_ATTR_CLASSID);
    netwox_htmltag_attr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_CODE, 4,
                                      NETWOX_HTMLTAG_ATTR_CODE);
    netwox_htmltag_attr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_CODEBASE, 8,
                                      NETWOX_HTMLTAG_ATTR_CODEBASE);
    netwox_htmltag_attr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_CONTENT, 7,
                                      NETWOX_HTMLTAG_ATTR_CONTENT);
    break;
  case 'd' :
  case 'D' :
    netwox_htmltag_attr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_DATA, 4,
                                      NETWOX_HTMLTAG_ATTR_DATA);
    netwox_htmltag_attr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_DYNSRC, 6,
                                      NETWOX_HTMLTAG_ATTR_DYNSRC);
    break;
  case 'f' :
  case 'F' :
    netwox_htmltag_attr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_FOR, 3,
                                      NETWOX_HTMLTAG_ATTR_FOR);
    break;
  case 'h' :
  case 'H' :
    netwox_htmltag_attr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_HREF, 4,
                                      NETWOX_HTMLTAG_ATTR_HREF);
    netwox_htmltag_attr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_HTTPEQUIV, 10,
                                      NETWOX_HTMLTAG_ATTR_HTTPEQUIV);
    break;
  case 'i' :
  case 'I' :
    netwox_htmltag_attr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_IMAGEMAP, 8,
                                      NETWOX_HTMLTAG_ATTR_IMAGEMAP);
    break;
  case 'l' :
  case 'L' :
    netwox_htmltag_attr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_LONGDESC, 8,
                                      NETWOX_HTMLTAG_ATTR_LONGDESC);
    break;
  case 'p' :
  case 'P' :
    netwox_htmltag_attr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_PLUGINSPAGE, 11,
                                      NETWOX_HTMLTAG_ATTR_PLUGINSPAGE);
    netwox_htmltag_attr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_PROFILE, 7,
                                      NETWOX_HTMLTAG_ATTR_PROFILE);
    break;
  case 's' :
  case 'S' :
    netwox_htmltag_attr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_SRC, 3,
                                      NETWOX_HTMLTAG_ATTR_SRC);
    break;
  case 'u' :
  case 'U' :
    netwox_htmltag_attr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_USEMAP, 6,
                                      NETWOX_HTMLTAG_ATTR_USEMAP);
    break;
  }

  if (pattr != NULL) *pattr = NETWOX_HTMLTAG_ATTR_UNKNOWN;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_htmltag_attr_init_elmtattr(netwox_htmltag_elmtattr elmtattr,
                                             netwox_htmltag_attr *pattr)
{
  netwox_htmltag_attr attr;

  switch(elmtattr) {
  case NETWOX_HTMLTAG_ELMTATTR_FORM_ACTION :
    attr = NETWOX_HTMLTAG_ATTR_ACTION;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_APPLET_ARCHIVE :
  case NETWOX_HTMLTAG_ELMTATTR_OBJECT_ARCHIVE :
    attr = NETWOX_HTMLTAG_ATTR_ARCHIVE;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_BODY_BACKGROUND :
  case NETWOX_HTMLTAG_ELMTATTR_ILAYER_BACKGROUND :
  case NETWOX_HTMLTAG_ELMTATTR_LAYER_BACKGROUND :
  case NETWOX_HTMLTAG_ELMTATTR_TABLE_BACKGROUND :
  case NETWOX_HTMLTAG_ELMTATTR_TD_BACKGROUND :
  case NETWOX_HTMLTAG_ELMTATTR_TH_BACKGROUND :
  case NETWOX_HTMLTAG_ELMTATTR_TR_BACKGROUND :
    attr = NETWOX_HTMLTAG_ATTR_BACKGROUND;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_BLOCKQUOTE_CITE :
  case NETWOX_HTMLTAG_ELMTATTR_DEL_CITE :
  case NETWOX_HTMLTAG_ELMTATTR_INS_CITE :
  case NETWOX_HTMLTAG_ELMTATTR_Q_CITE :
    attr = NETWOX_HTMLTAG_ATTR_CITE;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_OBJECT_CLASSID :
    attr = NETWOX_HTMLTAG_ATTR_CLASSID;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_APPLET_CODE :
    attr = NETWOX_HTMLTAG_ATTR_CODE;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_APPLET_CODEBASE :
  case NETWOX_HTMLTAG_ELMTATTR_OBJECT_CODEBASE :
    attr = NETWOX_HTMLTAG_ATTR_CODEBASE;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_META_REFRESHCONTENT :
    attr = NETWOX_HTMLTAG_ATTR_CONTENT;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_OBJECT_DATA :
    attr = NETWOX_HTMLTAG_ATTR_DATA;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_IMG_DYNSRC :
    attr = NETWOX_HTMLTAG_ATTR_DYNSRC;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_SCRIPT_FOR :
    attr = NETWOX_HTMLTAG_ATTR_FOR;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_A_HREF :
  case NETWOX_HTMLTAG_ELMTATTR_AREA_HREF :
  case NETWOX_HTMLTAG_ELMTATTR_BASE_HREF :
  case NETWOX_HTMLTAG_ELMTATTR_EMBED_HREF :
  case NETWOX_HTMLTAG_ELMTATTR_LINK_HREF :
    attr = NETWOX_HTMLTAG_ATTR_HREF;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_META_HTTPEQUIV :
    attr = NETWOX_HTMLTAG_ATTR_HTTPEQUIV;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_FIG_IMAGEMAP :
  case NETWOX_HTMLTAG_ELMTATTR_OVERLAY_IMAGEMAP :
    attr = NETWOX_HTMLTAG_ATTR_IMAGEMAP;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_FRAME_LONGDESC :
  case NETWOX_HTMLTAG_ELMTATTR_IFRAME_LONGDESC :
  case NETWOX_HTMLTAG_ELMTATTR_IMG_LONGDESC :
    attr = NETWOX_HTMLTAG_ATTR_LONGDESC;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_EMBED_PLUGINSPAGE :
    attr = NETWOX_HTMLTAG_ATTR_PLUGINSPAGE;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_HEAD_PROFILE :
    attr = NETWOX_HTMLTAG_ATTR_PROFILE;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_BGSOUND_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_EMBED_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_FIG_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_FRAME_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_IFRAME_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_ILAYER_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_IMG_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_INPUT_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_LAYER_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_OVERLAY_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_SCRIPT_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_SELECT_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_XML_SRC :
    attr = NETWOX_HTMLTAG_ATTR_SRC;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_IMG_USEMAP :
  case NETWOX_HTMLTAG_ELMTATTR_INPUT_USEMAP :
  case NETWOX_HTMLTAG_ELMTATTR_OBJECT_USEMAP :
    attr = NETWOX_HTMLTAG_ATTR_USEMAP;
    break;
  default :
    attr = NETWOX_HTMLTAG_ATTR_UNKNOWN;
    break;
  }

  if (pattr != NULL) *pattr = attr;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_htmltag_buf_append_attr(netwox_htmltag_attr attr,
                                          netwib_buf *pbuf)
{
  netwib_conststring pc = NULL;

  switch(attr) {
  case NETWOX_HTMLTAG_ATTR_ACTION :
    pc = NETWOX_HTMLTAG_ATTRNAME_ACTION;
    break;
  case NETWOX_HTMLTAG_ATTR_ARCHIVE :
    pc = NETWOX_HTMLTAG_ATTRNAME_ARCHIVE;
    break;
  case NETWOX_HTMLTAG_ATTR_BACKGROUND :
    pc = NETWOX_HTMLTAG_ATTRNAME_BACKGROUND;
    break;
  case NETWOX_HTMLTAG_ATTR_CITE :
    pc = NETWOX_HTMLTAG_ATTRNAME_CITE;
    break;
  case NETWOX_HTMLTAG_ATTR_CLASSID :
    pc = NETWOX_HTMLTAG_ATTRNAME_CLASSID;
    break;
  case NETWOX_HTMLTAG_ATTR_CODE :
    pc = NETWOX_HTMLTAG_ATTRNAME_CODE;
    break;
  case NETWOX_HTMLTAG_ATTR_CODEBASE :
    pc = NETWOX_HTMLTAG_ATTRNAME_CODEBASE;
    break;
  case NETWOX_HTMLTAG_ATTR_CONTENT :
    pc = NETWOX_HTMLTAG_ATTRNAME_CONTENT;
    break;
  case NETWOX_HTMLTAG_ATTR_DATA :
    pc = NETWOX_HTMLTAG_ATTRNAME_DATA;
    break;
  case NETWOX_HTMLTAG_ATTR_DYNSRC :
    pc = NETWOX_HTMLTAG_ATTRNAME_DYNSRC;
    break;
  case NETWOX_HTMLTAG_ATTR_FOR :
    pc = NETWOX_HTMLTAG_ATTRNAME_FOR;
    break;
  case NETWOX_HTMLTAG_ATTR_HREF :
    pc = NETWOX_HTMLTAG_ATTRNAME_HREF;
    break;
  case NETWOX_HTMLTAG_ATTR_HTTPEQUIV :
    pc = NETWOX_HTMLTAG_ATTRNAME_HTTPEQUIV;
    break;
  case NETWOX_HTMLTAG_ATTR_IMAGEMAP :
    pc = NETWOX_HTMLTAG_ATTRNAME_IMAGEMAP;
    break;
  case NETWOX_HTMLTAG_ATTR_LONGDESC :
    pc = NETWOX_HTMLTAG_ATTRNAME_LONGDESC;
    break;
  case NETWOX_HTMLTAG_ATTR_PLUGINSPAGE :
    pc = NETWOX_HTMLTAG_ATTRNAME_PLUGINSPAGE;
    break;
  case NETWOX_HTMLTAG_ATTR_PROFILE :
    pc = NETWOX_HTMLTAG_ATTRNAME_PROFILE;
    break;
  case NETWOX_HTMLTAG_ATTR_SRC :
    pc = NETWOX_HTMLTAG_ATTRNAME_SRC;
    break;
  case NETWOX_HTMLTAG_ATTR_USEMAP :
    pc = NETWOX_HTMLTAG_ATTRNAME_USEMAP;
    break;
  default :
    return(NETWIB_ERR_PAINVALIDTYPE);
  }

  netwib_er(netwib_buf_append_string(pc, pbuf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_htmltag_elmtattr_init_buf(netwox_htmltag_elmt elmt,
                                            netwib_constbuf *pbufattr,
                                           netwox_htmltag_elmtattr *pelmtattr)
{
  netwib_data data;
  netwib_uint32 datasize;

  data = netwib__buf_ref_data_ptr(pbufattr);
  datasize = netwib__buf_ref_data_size(pbufattr);

#define netwox_htmltag_elmtattr_init_buf_text(item,itemsize,v) if (datasize == itemsize) { if (!netwib_c_memcasecmp(data, (netwib_constdata)item, itemsize)) { if (pelmtattr != NULL) *pelmtattr = v; return(NETWIB_ERR_OK);}}

  switch(elmt) {
  case NETWOX_HTMLTAG_ELMT_A :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_HREF, 4, NETWOX_HTMLTAG_ELMTATTR_A_HREF);
    break;
  case NETWOX_HTMLTAG_ELMT_APPLET :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_ARCHIVE, 7, NETWOX_HTMLTAG_ELMTATTR_APPLET_ARCHIVE);
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_CODE, 4, NETWOX_HTMLTAG_ELMTATTR_APPLET_CODE);
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_CODEBASE, 8, NETWOX_HTMLTAG_ELMTATTR_APPLET_CODEBASE);
    break;
  case NETWOX_HTMLTAG_ELMT_AREA :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_HREF, 4, NETWOX_HTMLTAG_ELMTATTR_AREA_HREF);
    break;
  case NETWOX_HTMLTAG_ELMT_BASE :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_HREF, 4, NETWOX_HTMLTAG_ELMTATTR_BASE_HREF);
    break;
  case NETWOX_HTMLTAG_ELMT_BGSOUND :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_SRC, 3, NETWOX_HTMLTAG_ELMTATTR_BGSOUND_SRC);
    break;
  case NETWOX_HTMLTAG_ELMT_BLOCKQUOTE :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_CITE, 4, NETWOX_HTMLTAG_ELMTATTR_BLOCKQUOTE_CITE);
    break;
  case NETWOX_HTMLTAG_ELMT_BODY :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_BACKGROUND, 10, NETWOX_HTMLTAG_ELMTATTR_BODY_BACKGROUND);
    break;
  case NETWOX_HTMLTAG_ELMT_DEL :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_CITE, 4, NETWOX_HTMLTAG_ELMTATTR_DEL_CITE);
    break;
  case NETWOX_HTMLTAG_ELMT_EMBED :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_PLUGINSPAGE, 11, NETWOX_HTMLTAG_ELMTATTR_EMBED_PLUGINSPAGE);
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_SRC, 3, NETWOX_HTMLTAG_ELMTATTR_EMBED_SRC);
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_HREF, 4, NETWOX_HTMLTAG_ELMTATTR_EMBED_HREF);
    break;
  case NETWOX_HTMLTAG_ELMT_FIG :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_IMAGEMAP, 8, NETWOX_HTMLTAG_ELMTATTR_FIG_IMAGEMAP);
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_SRC, 3, NETWOX_HTMLTAG_ELMTATTR_FIG_SRC);
    break;
  case NETWOX_HTMLTAG_ELMT_FORM :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_ACTION, 6, NETWOX_HTMLTAG_ELMTATTR_FORM_ACTION);
    break;
  case NETWOX_HTMLTAG_ELMT_FRAME :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_LONGDESC, 8, NETWOX_HTMLTAG_ELMTATTR_FRAME_LONGDESC);
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_SRC, 3, NETWOX_HTMLTAG_ELMTATTR_FRAME_SRC);
    break;
  case NETWOX_HTMLTAG_ELMT_HEAD :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_PROFILE, 7, NETWOX_HTMLTAG_ELMTATTR_HEAD_PROFILE);
    break;
  case NETWOX_HTMLTAG_ELMT_IFRAME :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_LONGDESC, 8, NETWOX_HTMLTAG_ELMTATTR_IFRAME_LONGDESC);
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_SRC, 3, NETWOX_HTMLTAG_ELMTATTR_IFRAME_SRC);
    break;
  case NETWOX_HTMLTAG_ELMT_ILAYER :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_BACKGROUND, 10, NETWOX_HTMLTAG_ELMTATTR_ILAYER_BACKGROUND);
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_SRC, 3, NETWOX_HTMLTAG_ELMTATTR_ILAYER_SRC);
    break;
  case NETWOX_HTMLTAG_ELMT_IMG :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_DYNSRC, 6, NETWOX_HTMLTAG_ELMTATTR_IMG_DYNSRC);
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_LONGDESC, 8, NETWOX_HTMLTAG_ELMTATTR_IMG_LONGDESC);
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_SRC, 3, NETWOX_HTMLTAG_ELMTATTR_IMG_SRC);
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_USEMAP, 6, NETWOX_HTMLTAG_ELMTATTR_IMG_USEMAP);
    break;
  case NETWOX_HTMLTAG_ELMT_INPUT :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_SRC, 3, NETWOX_HTMLTAG_ELMTATTR_INPUT_SRC);
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_USEMAP, 6, NETWOX_HTMLTAG_ELMTATTR_INPUT_USEMAP);
    break;
  case NETWOX_HTMLTAG_ELMT_INS :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_CITE, 4, NETWOX_HTMLTAG_ELMTATTR_INS_CITE);
    break;
  case NETWOX_HTMLTAG_ELMT_LAYER :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_BACKGROUND, 10, NETWOX_HTMLTAG_ELMTATTR_LAYER_BACKGROUND);
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_SRC, 3, NETWOX_HTMLTAG_ELMTATTR_LAYER_SRC);
    break;
  case NETWOX_HTMLTAG_ELMT_LINK :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_HREF, 4, NETWOX_HTMLTAG_ELMTATTR_LINK_HREF);
    break;
  case NETWOX_HTMLTAG_ELMT_META :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_CONTENT, 7, NETWOX_HTMLTAG_ELMTATTR_META_REFRESHCONTENT);
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_HTTPEQUIV, 10, NETWOX_HTMLTAG_ELMTATTR_META_HTTPEQUIV);
    break;
  case NETWOX_HTMLTAG_ELMT_OBJECT :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_ARCHIVE, 7, NETWOX_HTMLTAG_ELMTATTR_OBJECT_ARCHIVE);
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_CLASSID, 7, NETWOX_HTMLTAG_ELMTATTR_OBJECT_CLASSID);
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_CODEBASE, 8, NETWOX_HTMLTAG_ELMTATTR_OBJECT_CODEBASE);
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_DATA, 4, NETWOX_HTMLTAG_ELMTATTR_OBJECT_DATA);
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_USEMAP, 6, NETWOX_HTMLTAG_ELMTATTR_OBJECT_USEMAP);
    break;
  case NETWOX_HTMLTAG_ELMT_OVERLAY :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_IMAGEMAP, 8, NETWOX_HTMLTAG_ELMTATTR_OVERLAY_IMAGEMAP);
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_SRC, 3, NETWOX_HTMLTAG_ELMTATTR_OVERLAY_SRC);
    break;
  case NETWOX_HTMLTAG_ELMT_Q :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_CITE, 4, NETWOX_HTMLTAG_ELMTATTR_Q_CITE);
    break;
  case NETWOX_HTMLTAG_ELMT_SCRIPT :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_FOR, 3, NETWOX_HTMLTAG_ELMTATTR_SCRIPT_FOR);
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_SRC, 3, NETWOX_HTMLTAG_ELMTATTR_SCRIPT_SRC);
    break;
  case NETWOX_HTMLTAG_ELMT_SELECT :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_SRC, 3, NETWOX_HTMLTAG_ELMTATTR_SELECT_SRC);
    break;
  case NETWOX_HTMLTAG_ELMT_TABLE :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_BACKGROUND, 10, NETWOX_HTMLTAG_ELMTATTR_TABLE_BACKGROUND);
    break;
  case NETWOX_HTMLTAG_ELMT_TD :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_BACKGROUND, 10, NETWOX_HTMLTAG_ELMTATTR_TD_BACKGROUND);
    break;
  case NETWOX_HTMLTAG_ELMT_TH :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_BACKGROUND, 10, NETWOX_HTMLTAG_ELMTATTR_TH_BACKGROUND);
    break;
  case NETWOX_HTMLTAG_ELMT_TR :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_BACKGROUND, 10, NETWOX_HTMLTAG_ELMTATTR_TR_BACKGROUND);
    break;
  case NETWOX_HTMLTAG_ELMT_XML :
    netwox_htmltag_elmtattr_init_buf_text(NETWOX_HTMLTAG_ATTRNAME_SRC, 3, NETWOX_HTMLTAG_ELMTATTR_XML_SRC);
    break;
  default : break;
  }

  if (pelmtattr != NULL) *pelmtattr = NETWOX_HTMLTAG_ELMTATTR_UNKNOWN;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_htmltag_elmtattr_init_elmt_attr(netwox_htmltag_elmt elmt,
                                                  netwox_htmltag_attr attr,
                                                  netwox_htmltag_elmtattr *pelmtattr)
{
  netwox_htmltag_elmtattr elmtattr;

  elmtattr = NETWOX_HTMLTAG_ELMTATTR_UNKNOWN;

  switch(elmt) {
  case NETWOX_HTMLTAG_ELMT_A :
    if (attr == NETWOX_HTMLTAG_ATTR_HREF) {
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_A_HREF;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_APPLET :
    switch(attr) {
    case NETWOX_HTMLTAG_ATTR_ARCHIVE :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_APPLET_ARCHIVE;
      break;
    case NETWOX_HTMLTAG_ATTR_CODE :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_APPLET_CODE;
      break;
    case NETWOX_HTMLTAG_ATTR_CODEBASE :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_APPLET_CODEBASE;
      break;
    default : break;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_AREA :
    if (attr == NETWOX_HTMLTAG_ATTR_HREF) {
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_AREA_HREF;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_BASE :
    if (attr == NETWOX_HTMLTAG_ATTR_HREF) {
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_BASE_HREF;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_BGSOUND :
    if (attr == NETWOX_HTMLTAG_ATTR_SRC) {
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_BGSOUND_SRC;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_BLOCKQUOTE :
    if (attr == NETWOX_HTMLTAG_ATTR_CITE) {
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_BLOCKQUOTE_CITE;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_BODY :
    if (attr == NETWOX_HTMLTAG_ATTR_BACKGROUND) {
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_BODY_BACKGROUND;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_DEL :
    if (attr == NETWOX_HTMLTAG_ATTR_CITE) {
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_DEL_CITE;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_EMBED :
    switch(attr) {
    case NETWOX_HTMLTAG_ATTR_PLUGINSPAGE :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_EMBED_PLUGINSPAGE;
      break;
    case NETWOX_HTMLTAG_ATTR_SRC :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_EMBED_SRC;
      break;
    case NETWOX_HTMLTAG_ATTR_HREF :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_EMBED_HREF;
      break;
    default : break;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_FIG :
    switch(attr) {
    case NETWOX_HTMLTAG_ATTR_IMAGEMAP :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_FIG_IMAGEMAP;
      break;
    case NETWOX_HTMLTAG_ATTR_SRC :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_FIG_SRC;
      break;
    default : break;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_FORM :
    if (attr == NETWOX_HTMLTAG_ATTR_ACTION) {
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_FORM_ACTION;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_FRAME :
    switch(attr) {
    case NETWOX_HTMLTAG_ATTR_LONGDESC :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_FRAME_LONGDESC;
      break;
    case NETWOX_HTMLTAG_ATTR_SRC :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_FRAME_SRC;
      break;
    default : break;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_HEAD :
    if (attr == NETWOX_HTMLTAG_ATTR_PROFILE) {
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_HEAD_PROFILE;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_IFRAME :
    switch(attr) {
    case NETWOX_HTMLTAG_ATTR_LONGDESC :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_IFRAME_LONGDESC;
      break;
    case NETWOX_HTMLTAG_ATTR_SRC :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_IFRAME_SRC;
      break;
    default : break;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_ILAYER :
    switch(attr) {
    case NETWOX_HTMLTAG_ATTR_BACKGROUND :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_ILAYER_BACKGROUND;
      break;
    case NETWOX_HTMLTAG_ATTR_SRC :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_ILAYER_SRC;
      break;
    default : break;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_IMG :
    switch(attr) {
    case NETWOX_HTMLTAG_ATTR_DYNSRC :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_IMG_DYNSRC;
      break;
    case NETWOX_HTMLTAG_ATTR_LONGDESC :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_IMG_LONGDESC;
      break;
    case NETWOX_HTMLTAG_ATTR_SRC :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_IMG_SRC;
      break;
    case NETWOX_HTMLTAG_ATTR_USEMAP :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_IMG_USEMAP;
      break;
    default : break;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_INPUT :
    switch(attr) {
    case NETWOX_HTMLTAG_ATTR_SRC :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_INPUT_SRC;
      break;
    case NETWOX_HTMLTAG_ATTR_USEMAP :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_INPUT_USEMAP;
      break;
    default : break;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_INS :
    if (attr == NETWOX_HTMLTAG_ATTR_CITE) {
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_INS_CITE;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_LAYER :
    switch(attr) {
    case NETWOX_HTMLTAG_ATTR_BACKGROUND :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_LAYER_BACKGROUND;
      break;
    case NETWOX_HTMLTAG_ATTR_SRC :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_LAYER_SRC;
      break;
    default : break;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_LINK :
    if (attr == NETWOX_HTMLTAG_ATTR_HREF) {
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_LINK_HREF;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_META :
    switch(attr) {
    case NETWOX_HTMLTAG_ATTR_CONTENT :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_META_REFRESHCONTENT;
      break;
    case NETWOX_HTMLTAG_ATTR_HTTPEQUIV :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_META_HTTPEQUIV;
      break;
    default : break;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_OBJECT :
    switch(attr) {
    case NETWOX_HTMLTAG_ATTR_ARCHIVE :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_OBJECT_ARCHIVE;
      break;
    case NETWOX_HTMLTAG_ATTR_CLASSID :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_OBJECT_CLASSID;
      break;
    case NETWOX_HTMLTAG_ATTR_CODEBASE :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_OBJECT_CODEBASE;
      break;
    case NETWOX_HTMLTAG_ATTR_DATA :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_OBJECT_DATA;
      break;
    case NETWOX_HTMLTAG_ATTR_USEMAP :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_OBJECT_USEMAP;
      break;
    default : break;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_OVERLAY :
    switch(attr) {
    case NETWOX_HTMLTAG_ATTR_IMAGEMAP :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_OVERLAY_IMAGEMAP;
      break;
    case NETWOX_HTMLTAG_ATTR_SRC :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_OVERLAY_SRC;
      break;
    default : break;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_Q :
    if (attr == NETWOX_HTMLTAG_ATTR_CITE) {
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_Q_CITE;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_SCRIPT :
    switch(attr) {
    case NETWOX_HTMLTAG_ATTR_FOR :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_SCRIPT_FOR;
      break;
    case NETWOX_HTMLTAG_ATTR_SRC :
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_SCRIPT_SRC;
      break;
    default : break;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_SELECT :
    if (attr == NETWOX_HTMLTAG_ATTR_SRC) {
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_SELECT_SRC;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_TABLE :
    if (attr == NETWOX_HTMLTAG_ATTR_BACKGROUND) {
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_TABLE_BACKGROUND;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_TD :
    if (attr == NETWOX_HTMLTAG_ATTR_BACKGROUND) {
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_TD_BACKGROUND;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_TH :
    if (attr == NETWOX_HTMLTAG_ATTR_BACKGROUND) {
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_TH_BACKGROUND;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_TR :
    if (attr == NETWOX_HTMLTAG_ATTR_BACKGROUND) {
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_TR_BACKGROUND;
    }
    break;
  case NETWOX_HTMLTAG_ELMT_XML :
    if (attr == NETWOX_HTMLTAG_ATTR_SRC) {
      elmtattr = NETWOX_HTMLTAG_ELMTATTR_XML_SRC;
    }
    break;
  default : break;
  }

  if (pelmtattr != NULL) *pelmtattr = elmtattr;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_htmltag_uricontain_init_elmtattr(netwox_htmltag_elmtattr elmtattr,
                             netwox_htmltag_elmtattr_uricontain *puricontain)
{
  netwox_htmltag_elmtattr_uricontain uricontain;

  switch(elmtattr) {
  case NETWOX_HTMLTAG_ELMTATTR_A_HREF :
  case NETWOX_HTMLTAG_ELMTATTR_AREA_HREF :
  case NETWOX_HTMLTAG_ELMTATTR_BLOCKQUOTE_CITE :
  case NETWOX_HTMLTAG_ELMTATTR_DEL_CITE :
  case NETWOX_HTMLTAG_ELMTATTR_EMBED_PLUGINSPAGE :
  case NETWOX_HTMLTAG_ELMTATTR_EMBED_HREF :
  case NETWOX_HTMLTAG_ELMTATTR_FIG_IMAGEMAP :
  case NETWOX_HTMLTAG_ELMTATTR_FORM_ACTION :
  case NETWOX_HTMLTAG_ELMTATTR_FRAME_LONGDESC :
  case NETWOX_HTMLTAG_ELMTATTR_FRAME_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_HEAD_PROFILE :
  case NETWOX_HTMLTAG_ELMTATTR_IFRAME_LONGDESC :
  case NETWOX_HTMLTAG_ELMTATTR_IFRAME_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_ILAYER_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_IMG_LONGDESC :
  case NETWOX_HTMLTAG_ELMTATTR_IMG_USEMAP :
  case NETWOX_HTMLTAG_ELMTATTR_INPUT_USEMAP :
  case NETWOX_HTMLTAG_ELMTATTR_INS_CITE :
  case NETWOX_HTMLTAG_ELMTATTR_LAYER_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_LINK_HREF :
  case NETWOX_HTMLTAG_ELMTATTR_META_REFRESHCONTENT :
  case NETWOX_HTMLTAG_ELMTATTR_OBJECT_USEMAP :
  case NETWOX_HTMLTAG_ELMTATTR_OVERLAY_IMAGEMAP :
  case NETWOX_HTMLTAG_ELMTATTR_Q_CITE :
  case NETWOX_HTMLTAG_ELMTATTR_XML_SRC :
    uricontain = NETWOX_HTMLTAG_ELMTATTR_URICONTAIN_HTML;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_BODY_BACKGROUND :
  case NETWOX_HTMLTAG_ELMTATTR_FIG_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_ILAYER_BACKGROUND :
  case NETWOX_HTMLTAG_ELMTATTR_IMG_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_INPUT_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_LAYER_BACKGROUND :
  case NETWOX_HTMLTAG_ELMTATTR_OVERLAY_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_SELECT_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_TABLE_BACKGROUND :
  case NETWOX_HTMLTAG_ELMTATTR_TD_BACKGROUND :
  case NETWOX_HTMLTAG_ELMTATTR_TH_BACKGROUND :
  case NETWOX_HTMLTAG_ELMTATTR_TR_BACKGROUND :
    uricontain = NETWOX_HTMLTAG_ELMTATTR_URICONTAIN_IMAGE;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_BGSOUND_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_EMBED_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_IMG_DYNSRC :
    uricontain = NETWOX_HTMLTAG_ELMTATTR_URICONTAIN_MULTIMEDIA;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_APPLET_ARCHIVE :
  case NETWOX_HTMLTAG_ELMTATTR_OBJECT_ARCHIVE :
  case NETWOX_HTMLTAG_ELMTATTR_SCRIPT_SRC :
    uricontain = NETWOX_HTMLTAG_ELMTATTR_URICONTAIN_CODE;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_APPLET_CODEBASE :
  case NETWOX_HTMLTAG_ELMTATTR_BASE_HREF :
  case NETWOX_HTMLTAG_ELMTATTR_OBJECT_CODEBASE :
    uricontain = NETWOX_HTMLTAG_ELMTATTR_URICONTAIN_URIREF;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_APPLET_CODE :
  case NETWOX_HTMLTAG_ELMTATTR_OBJECT_CLASSID :
  case NETWOX_HTMLTAG_ELMTATTR_OBJECT_DATA :
  case NETWOX_HTMLTAG_ELMTATTR_SCRIPT_FOR :
    uricontain = NETWOX_HTMLTAG_ELMTATTR_URICONTAIN_DATA;
    break;
  default :
    uricontain = NETWOX_HTMLTAG_ELMTATTR_URICONTAIN_UNKNOWN;
    break;
  }

  if (puricontain != NULL) *puricontain = uricontain;
  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
netwib_err netwox_htmltag_uriformat_init_elmtattr(netwox_htmltag_elmtattr elmtattr,
                             netwox_htmltag_elmtattr_uriformat *puriformat)
{
  netwox_htmltag_elmtattr_uriformat uriformat;

  switch(elmtattr) {
  case NETWOX_HTMLTAG_ELMTATTR_A_HREF :
  case NETWOX_HTMLTAG_ELMTATTR_APPLET_CODEBASE :
  case NETWOX_HTMLTAG_ELMTATTR_AREA_HREF :
  case NETWOX_HTMLTAG_ELMTATTR_BASE_HREF :
  case NETWOX_HTMLTAG_ELMTATTR_BGSOUND_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_BLOCKQUOTE_CITE :
  case NETWOX_HTMLTAG_ELMTATTR_BODY_BACKGROUND :
  case NETWOX_HTMLTAG_ELMTATTR_DEL_CITE :
  case NETWOX_HTMLTAG_ELMTATTR_EMBED_PLUGINSPAGE :
  case NETWOX_HTMLTAG_ELMTATTR_EMBED_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_EMBED_HREF :
  case NETWOX_HTMLTAG_ELMTATTR_FIG_IMAGEMAP :
  case NETWOX_HTMLTAG_ELMTATTR_FIG_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_FORM_ACTION :
  case NETWOX_HTMLTAG_ELMTATTR_FRAME_LONGDESC :
  case NETWOX_HTMLTAG_ELMTATTR_FRAME_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_IFRAME_LONGDESC :
  case NETWOX_HTMLTAG_ELMTATTR_IFRAME_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_ILAYER_BACKGROUND :
  case NETWOX_HTMLTAG_ELMTATTR_ILAYER_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_IMG_DYNSRC :
  case NETWOX_HTMLTAG_ELMTATTR_IMG_LONGDESC :
  case NETWOX_HTMLTAG_ELMTATTR_IMG_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_IMG_USEMAP :
  case NETWOX_HTMLTAG_ELMTATTR_INPUT_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_INPUT_USEMAP :
  case NETWOX_HTMLTAG_ELMTATTR_INS_CITE :
  case NETWOX_HTMLTAG_ELMTATTR_LAYER_BACKGROUND :
  case NETWOX_HTMLTAG_ELMTATTR_LAYER_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_LINK_HREF :
  case NETWOX_HTMLTAG_ELMTATTR_OBJECT_CODEBASE :
  case NETWOX_HTMLTAG_ELMTATTR_OBJECT_USEMAP :
  case NETWOX_HTMLTAG_ELMTATTR_OVERLAY_IMAGEMAP :
  case NETWOX_HTMLTAG_ELMTATTR_OVERLAY_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_Q_CITE :
  case NETWOX_HTMLTAG_ELMTATTR_SCRIPT_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_SELECT_SRC :
  case NETWOX_HTMLTAG_ELMTATTR_TABLE_BACKGROUND :
  case NETWOX_HTMLTAG_ELMTATTR_TD_BACKGROUND :
  case NETWOX_HTMLTAG_ELMTATTR_TH_BACKGROUND :
  case NETWOX_HTMLTAG_ELMTATTR_TR_BACKGROUND :
  case NETWOX_HTMLTAG_ELMTATTR_XML_SRC :
    uriformat = NETWOX_HTMLTAG_ELMTATTR_URIFORMAT_ONE;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_HEAD_PROFILE :
  case NETWOX_HTMLTAG_ELMTATTR_OBJECT_ARCHIVE :
    uriformat = NETWOX_HTMLTAG_ELMTATTR_URIFORMAT_LISTSPACED;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_APPLET_ARCHIVE :
    uriformat = NETWOX_HTMLTAG_ELMTATTR_URIFORMAT_LISTCOMMA;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_APPLET_CODE :
  case NETWOX_HTMLTAG_ELMTATTR_OBJECT_CLASSID :
  case NETWOX_HTMLTAG_ELMTATTR_OBJECT_DATA :
  case NETWOX_HTMLTAG_ELMTATTR_SCRIPT_FOR :
    uriformat = NETWOX_HTMLTAG_ELMTATTR_URIFORMAT_DATA;
    break;
  case NETWOX_HTMLTAG_ELMTATTR_META_REFRESHCONTENT :
    uriformat = NETWOX_HTMLTAG_ELMTATTR_URIFORMAT_REFRESH;
    break;
  default :
    uriformat = NETWOX_HTMLTAG_ELMTATTR_URIFORMAT_UNKNOWN;
    break;
  }

  if (puriformat != NULL) *puriformat = uriformat;
  return(NETWIB_ERR_OK);
}
