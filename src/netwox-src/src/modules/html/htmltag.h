
/*-------------------------------------------------------------*/
/*
   This file is specialized for analyzing HTML tags containing
   an URI. Other tags cannot be analyzed by those functions.
   The recognized elements are from :
    - HTML 3.2
    - HTML 4.01
    - http://msdn.microsoft.com/workshop/author/html/reference/elements.asp
    - http://developer.netscape.com/docs/manuals/htmlguid/contents.htm

   An HTML tag consists of :
    <elementname attributename=attvalue  attname attname = attvalue >
    <elementname attname=attvalue attname="attvalue" attname='attvalue' >

   An element name is supposed to not be encoded using "&...;".
   An attribute name is supposed to not be encoded using "&...;".
   An attribute value can be encoded using "&...;".
   A "space" can be a space, tabulation, linefeed or newline.
*/

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_HTMLTAG_ELMT_UNKNOWN = 1,
  NETWOX_HTMLTAG_ELMT_A,
  NETWOX_HTMLTAG_ELMT_APPLET,
  NETWOX_HTMLTAG_ELMT_AREA,
  NETWOX_HTMLTAG_ELMT_BASE,
  NETWOX_HTMLTAG_ELMT_BGSOUND,
  NETWOX_HTMLTAG_ELMT_BLOCKQUOTE,
  NETWOX_HTMLTAG_ELMT_BODY,
  NETWOX_HTMLTAG_ELMT_DEL,
  NETWOX_HTMLTAG_ELMT_EMBED,
  NETWOX_HTMLTAG_ELMT_FIG,
  NETWOX_HTMLTAG_ELMT_FORM,
  NETWOX_HTMLTAG_ELMT_FRAME,
  NETWOX_HTMLTAG_ELMT_HEAD,
  NETWOX_HTMLTAG_ELMT_IFRAME,
  NETWOX_HTMLTAG_ELMT_ILAYER,
  NETWOX_HTMLTAG_ELMT_IMG,
  NETWOX_HTMLTAG_ELMT_INPUT,
  NETWOX_HTMLTAG_ELMT_INS,
  NETWOX_HTMLTAG_ELMT_LAYER,
  NETWOX_HTMLTAG_ELMT_LINK,
  NETWOX_HTMLTAG_ELMT_META,
  NETWOX_HTMLTAG_ELMT_OBJECT,
  NETWOX_HTMLTAG_ELMT_OVERLAY,
  NETWOX_HTMLTAG_ELMT_Q,
  NETWOX_HTMLTAG_ELMT_SCRIPT,
  NETWOX_HTMLTAG_ELMT_SELECT,
  NETWOX_HTMLTAG_ELMT_TABLE,
  NETWOX_HTMLTAG_ELMT_TD,
  NETWOX_HTMLTAG_ELMT_TH,
  NETWOX_HTMLTAG_ELMT_TR,
  NETWOX_HTMLTAG_ELMT_XML
} netwox_htmltag_elmt;
#define NETWOX_HTMLTAG_ELMTNAME_A "a"
#define NETWOX_HTMLTAG_ELMTNAME_APPLET "applet"
#define NETWOX_HTMLTAG_ELMTNAME_AREA "area"
#define NETWOX_HTMLTAG_ELMTNAME_BASE "base"
#define NETWOX_HTMLTAG_ELMTNAME_BGSOUND "bgsound"
#define NETWOX_HTMLTAG_ELMTNAME_BLOCKQUOTE "blockquote"
#define NETWOX_HTMLTAG_ELMTNAME_BODY "body"
#define NETWOX_HTMLTAG_ELMTNAME_DEL "del"
#define NETWOX_HTMLTAG_ELMTNAME_EMBED "embed"
#define NETWOX_HTMLTAG_ELMTNAME_FIG "fig"
#define NETWOX_HTMLTAG_ELMTNAME_FORM "form"
#define NETWOX_HTMLTAG_ELMTNAME_FRAME "frame"
#define NETWOX_HTMLTAG_ELMTNAME_HEAD "head"
#define NETWOX_HTMLTAG_ELMTNAME_IFRAME "iframe"
#define NETWOX_HTMLTAG_ELMTNAME_ILAYER "ilayer"
#define NETWOX_HTMLTAG_ELMTNAME_IMG "img"
#define NETWOX_HTMLTAG_ELMTNAME_INPUT "input"
#define NETWOX_HTMLTAG_ELMTNAME_INS "ins"
#define NETWOX_HTMLTAG_ELMTNAME_LAYER "layer"
#define NETWOX_HTMLTAG_ELMTNAME_LINK "link"
#define NETWOX_HTMLTAG_ELMTNAME_META "meta"
#define NETWOX_HTMLTAG_ELMTNAME_OBJECT "object"
#define NETWOX_HTMLTAG_ELMTNAME_OVERLAY "overlay"
#define NETWOX_HTMLTAG_ELMTNAME_Q "q"
#define NETWOX_HTMLTAG_ELMTNAME_SCRIPT "script"
#define NETWOX_HTMLTAG_ELMTNAME_SELECT "select"
#define NETWOX_HTMLTAG_ELMTNAME_TABLE "table"
#define NETWOX_HTMLTAG_ELMTNAME_TD "td"
#define NETWOX_HTMLTAG_ELMTNAME_TH "th"
#define NETWOX_HTMLTAG_ELMTNAME_TR "tr"
#define NETWOX_HTMLTAG_ELMTNAME_XML "xml"

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_HTMLTAG_ATTR_UNKNOWN = 1,
  NETWOX_HTMLTAG_ATTR_ACTION,
  NETWOX_HTMLTAG_ATTR_ARCHIVE,
  NETWOX_HTMLTAG_ATTR_BACKGROUND,
  NETWOX_HTMLTAG_ATTR_CITE,
  NETWOX_HTMLTAG_ATTR_CLASSID,
  NETWOX_HTMLTAG_ATTR_CODE,
  NETWOX_HTMLTAG_ATTR_CODEBASE,
  NETWOX_HTMLTAG_ATTR_CONTENT,
  NETWOX_HTMLTAG_ATTR_DATA,
  NETWOX_HTMLTAG_ATTR_DYNSRC,
  NETWOX_HTMLTAG_ATTR_FOR,
  NETWOX_HTMLTAG_ATTR_HREF,
  NETWOX_HTMLTAG_ATTR_HTTPEQUIV,
  NETWOX_HTMLTAG_ATTR_IMAGEMAP,
  NETWOX_HTMLTAG_ATTR_LONGDESC,
  NETWOX_HTMLTAG_ATTR_PLUGINSPAGE,
  NETWOX_HTMLTAG_ATTR_PROFILE,
  NETWOX_HTMLTAG_ATTR_SRC,
  NETWOX_HTMLTAG_ATTR_USEMAP
} netwox_htmltag_attr;
#define NETWOX_HTMLTAG_ATTRNAME_ACTION "action"
#define NETWOX_HTMLTAG_ATTRNAME_ARCHIVE "archive"
#define NETWOX_HTMLTAG_ATTRNAME_BACKGROUND "background"
#define NETWOX_HTMLTAG_ATTRNAME_CITE "cite"
#define NETWOX_HTMLTAG_ATTRNAME_CLASSID "classid"
#define NETWOX_HTMLTAG_ATTRNAME_CODE "code"
#define NETWOX_HTMLTAG_ATTRNAME_CODEBASE "codebase"
#define NETWOX_HTMLTAG_ATTRNAME_CONTENT "content"
#define NETWOX_HTMLTAG_ATTRNAME_DATA "data"
#define NETWOX_HTMLTAG_ATTRNAME_DYNSRC "dynsrc"
#define NETWOX_HTMLTAG_ATTRNAME_FOR "for"
#define NETWOX_HTMLTAG_ATTRNAME_HREF "href"
#define NETWOX_HTMLTAG_ATTRNAME_HTTPEQUIV "http-equiv"
#define NETWOX_HTMLTAG_ATTRNAME_IMAGEMAP "imagemap"
#define NETWOX_HTMLTAG_ATTRNAME_LONGDESC "longdesc"
#define NETWOX_HTMLTAG_ATTRNAME_PLUGINSPAGE "pluginspage"
#define NETWOX_HTMLTAG_ATTRNAME_PROFILE "profile"
#define NETWOX_HTMLTAG_ATTRNAME_SRC "src"
#define NETWOX_HTMLTAG_ATTRNAME_USEMAP "usemap"

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_HTMLTAG_ELMTATTR_UNKNOWN = 1,
  NETWOX_HTMLTAG_ELMTATTR_A_HREF,
  NETWOX_HTMLTAG_ELMTATTR_APPLET_ARCHIVE,
  NETWOX_HTMLTAG_ELMTATTR_APPLET_CODE,
  NETWOX_HTMLTAG_ELMTATTR_APPLET_CODEBASE,
  NETWOX_HTMLTAG_ELMTATTR_AREA_HREF,
  NETWOX_HTMLTAG_ELMTATTR_BASE_HREF,
  NETWOX_HTMLTAG_ELMTATTR_BGSOUND_SRC,
  NETWOX_HTMLTAG_ELMTATTR_BLOCKQUOTE_CITE,
  NETWOX_HTMLTAG_ELMTATTR_BODY_BACKGROUND,
  NETWOX_HTMLTAG_ELMTATTR_DEL_CITE,
  NETWOX_HTMLTAG_ELMTATTR_EMBED_PLUGINSPAGE,
  NETWOX_HTMLTAG_ELMTATTR_EMBED_SRC,
  NETWOX_HTMLTAG_ELMTATTR_EMBED_HREF,
  NETWOX_HTMLTAG_ELMTATTR_FIG_IMAGEMAP,
  NETWOX_HTMLTAG_ELMTATTR_FIG_SRC,
  NETWOX_HTMLTAG_ELMTATTR_FORM_ACTION,
  NETWOX_HTMLTAG_ELMTATTR_FRAME_LONGDESC,
  NETWOX_HTMLTAG_ELMTATTR_FRAME_SRC,
  NETWOX_HTMLTAG_ELMTATTR_HEAD_PROFILE,
  NETWOX_HTMLTAG_ELMTATTR_IFRAME_LONGDESC,
  NETWOX_HTMLTAG_ELMTATTR_IFRAME_SRC,
  NETWOX_HTMLTAG_ELMTATTR_ILAYER_BACKGROUND,
  NETWOX_HTMLTAG_ELMTATTR_ILAYER_SRC,
  NETWOX_HTMLTAG_ELMTATTR_IMG_DYNSRC,
  NETWOX_HTMLTAG_ELMTATTR_IMG_LONGDESC,
  NETWOX_HTMLTAG_ELMTATTR_IMG_SRC,
  NETWOX_HTMLTAG_ELMTATTR_IMG_USEMAP,
  NETWOX_HTMLTAG_ELMTATTR_INPUT_SRC,
  NETWOX_HTMLTAG_ELMTATTR_INPUT_USEMAP,
  NETWOX_HTMLTAG_ELMTATTR_INS_CITE,
  NETWOX_HTMLTAG_ELMTATTR_LAYER_BACKGROUND,
  NETWOX_HTMLTAG_ELMTATTR_LAYER_SRC,
  NETWOX_HTMLTAG_ELMTATTR_LINK_HREF,
  NETWOX_HTMLTAG_ELMTATTR_META_HTTPEQUIV,
  NETWOX_HTMLTAG_ELMTATTR_META_REFRESHCONTENT, /* <meta http-equiv="refresh"
                                                  content="0;url=http://s/"> */
  NETWOX_HTMLTAG_ELMTATTR_OBJECT_ARCHIVE,
  NETWOX_HTMLTAG_ELMTATTR_OBJECT_CLASSID,
  NETWOX_HTMLTAG_ELMTATTR_OBJECT_CODEBASE,
  NETWOX_HTMLTAG_ELMTATTR_OBJECT_DATA,
  NETWOX_HTMLTAG_ELMTATTR_OBJECT_USEMAP,
  NETWOX_HTMLTAG_ELMTATTR_OVERLAY_IMAGEMAP,
  NETWOX_HTMLTAG_ELMTATTR_OVERLAY_SRC,
  NETWOX_HTMLTAG_ELMTATTR_Q_CITE,
  NETWOX_HTMLTAG_ELMTATTR_SCRIPT_FOR,
  NETWOX_HTMLTAG_ELMTATTR_SCRIPT_SRC,
  NETWOX_HTMLTAG_ELMTATTR_SELECT_SRC,
  NETWOX_HTMLTAG_ELMTATTR_TABLE_BACKGROUND,
  NETWOX_HTMLTAG_ELMTATTR_TD_BACKGROUND,
  NETWOX_HTMLTAG_ELMTATTR_TH_BACKGROUND,
  NETWOX_HTMLTAG_ELMTATTR_TR_BACKGROUND,
  NETWOX_HTMLTAG_ELMTATTR_XML_SRC
} netwox_htmltag_elmtattr;

/*-------------------------------------------------------------*/
netwib_err netwox_htmltag_elmt_init_buf(netwib_constbuf *pbuf,
                                        netwox_htmltag_elmt *pelmt);
netwib_err netwox_htmltag_elmt_init_elmtattr(netwox_htmltag_elmtattr elmtattr,
                                             netwox_htmltag_elmt *pelmt);
netwib_err netwox_htmltag_buf_append_elmt(netwox_htmltag_elmt elmt,
                                          netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_htmltag_attr_init_buf(netwib_constbuf *pbuf,
                                        netwox_htmltag_attr *pattr);
netwib_err netwox_htmltag_attr_init_elmtattr(netwox_htmltag_elmtattr elmtattr,
                                             netwox_htmltag_attr *pattr);
netwib_err netwox_htmltag_buf_append_attr(netwox_htmltag_attr attr,
                                          netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_htmltag_elmtattr_init_buf(netwox_htmltag_elmt elmt,
                                            netwib_constbuf *pbufattr,
                                           netwox_htmltag_elmtattr *pelmtattr);
netwib_err netwox_htmltag_elmtattr_init_elmt_attr(netwox_htmltag_elmt elmt,
                                                  netwox_htmltag_attr attr,
                                      netwox_htmltag_elmtattr *pelmtattr);

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_HTMLTAG_ELMTATTR_URICONTAIN_UNKNOWN = 1,
  NETWOX_HTMLTAG_ELMTATTR_URICONTAIN_HTML,
  NETWOX_HTMLTAG_ELMTATTR_URICONTAIN_IMAGE,
  NETWOX_HTMLTAG_ELMTATTR_URICONTAIN_MULTIMEDIA,
  NETWOX_HTMLTAG_ELMTATTR_URICONTAIN_CODE,
  NETWOX_HTMLTAG_ELMTATTR_URICONTAIN_URIREF,
  NETWOX_HTMLTAG_ELMTATTR_URICONTAIN_DATA
} netwox_htmltag_elmtattr_uricontain;

/*-------------------------------------------------------------*/
netwib_err netwox_htmltag_uricontain_init_elmtattr(netwox_htmltag_elmtattr elmtattr,
                             netwox_htmltag_elmtattr_uricontain *puricontain);

/*-------------------------------------------------------------*/
typedef enum {
  NETWOX_HTMLTAG_ELMTATTR_URIFORMAT_UNKNOWN = 1,
  NETWOX_HTMLTAG_ELMTATTR_URIFORMAT_ONE,
  NETWOX_HTMLTAG_ELMTATTR_URIFORMAT_LISTSPACED,
  NETWOX_HTMLTAG_ELMTATTR_URIFORMAT_LISTCOMMA,
  NETWOX_HTMLTAG_ELMTATTR_URIFORMAT_DATA,
  NETWOX_HTMLTAG_ELMTATTR_URIFORMAT_REFRESH
} netwox_htmltag_elmtattr_uriformat;

/*-------------------------------------------------------------*/
netwib_err netwox_htmltag_uriformat_init_elmtattr(netwox_htmltag_elmtattr elmtattr,
                             netwox_htmltag_elmtattr_uriformat *puriformat);
