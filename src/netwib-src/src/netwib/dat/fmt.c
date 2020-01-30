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

/*
                             IMPORTANT
Function netwib_priv_buf_append_vfmt2 must be independent, and
work even if netwib is not initialized : it is used in netwib_init
to display error. Of course, this is only valid if the buffer
is an external array (ie not allocated).
*/

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_buf_append_vfmt_error(netwib_conststring msg)
{
  netwib_bool canuseglo;

  netwib_er(netwib_priv_glovars_canuse(&canuseglo));
  if (canuseglo) {
    netwib_er(netwib_priv_errmsg_string(msg));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_buf_append_vfmt_error2(netwib_conststring msg,
                                                     netwib_conststring badfmt)
{
  netwib_buf buf;
  netwib_bool canuseglo;

  netwib_er(netwib_priv_glovars_canuse(&canuseglo));
  if (canuseglo) {
    netwib_er(netwib_buf_init_mallocdefault(&buf));
    netwib_er(netwib_buf_append_string(msg, &buf));
    netwib_er(netwib_buf_append_string(" in \"...%", &buf));
    netwib_er(netwib_buf_append_string(badfmt, &buf));
    netwib_er(netwib_buf_append_string("\"", &buf));
    netwib_er(netwib_priv_errmsg_buf(&buf));
    netwib_er(netwib_buf_close(&buf));
  }

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
#define NETWIB_PRIV_FMT_FIELD_MAXSIZE 20

typedef enum {
  NETWIB_PRIV_FMT_ALIGNTYPE_LEFT = 1,
  NETWIB_PRIV_FMT_ALIGNTYPE_CENTER,
  NETWIB_PRIV_FMT_ALIGNTYPE_RIGHT
} netwib_priv_fmt_aligntype;

typedef struct {
  netwib_bool altformat;
  netwib_uint32 minsize;
  netwib_uint32 base;
} netwib_priv_fmt_spec_uint;

typedef struct {
  netwib_bool sign;
  netwib_uint32 minsize;
} netwib_priv_fmt_spec_int;

typedef struct {
  netwib_char type;
} netwib_priv_fmt_spec_bool;

typedef struct {
  netwib_char type;
} netwib_priv_fmt_spec_cmp;

typedef struct {
  netwib_bool useglob;
} netwib_priv_fmt_spec_buf;

typedef struct {
  netwib_bool useglob;
} netwib_priv_fmt_spec_s;

typedef enum {
  NETWIB_PRIV_FMT_FIELDTYPE_PERCENT = 1,
  NETWIB_PRIV_FMT_FIELDTYPE_C,
  NETWIB_PRIV_FMT_FIELDTYPE_S,
  NETWIB_PRIV_FMT_FIELDTYPE_P,
  NETWIB_PRIV_FMT_FIELDTYPE_END,
  NETWIB_PRIV_FMT_FIELDTYPE_UINT64,
  NETWIB_PRIV_FMT_FIELDTYPE_UINT32,
  NETWIB_PRIV_FMT_FIELDTYPE_UINT16,
  NETWIB_PRIV_FMT_FIELDTYPE_UINT8,
  NETWIB_PRIV_FMT_FIELDTYPE_UINTMAX,
  NETWIB_PRIV_FMT_FIELDTYPE_UINTPTR,
  NETWIB_PRIV_FMT_FIELDTYPE_BYTE,
  NETWIB_PRIV_FMT_FIELDTYPE_INT64,
  NETWIB_PRIV_FMT_FIELDTYPE_INT32,
  NETWIB_PRIV_FMT_FIELDTYPE_INT16,
  NETWIB_PRIV_FMT_FIELDTYPE_INT8,
  NETWIB_PRIV_FMT_FIELDTYPE_INTMAX,
  NETWIB_PRIV_FMT_FIELDTYPE_INTPTR,
  NETWIB_PRIV_FMT_FIELDTYPE_BOOL,
  NETWIB_PRIV_FMT_FIELDTYPE_CMP,
  NETWIB_PRIV_FMT_FIELDTYPE_BUF,
  NETWIB_PRIV_FMT_FIELDTYPE_ETH,
  NETWIB_PRIV_FMT_FIELDTYPE_IP,
  NETWIB_PRIV_FMT_FIELDTYPE_PORT,
  NETWIB_PRIV_FMT_FIELDTYPE_ETHS,
  NETWIB_PRIV_FMT_FIELDTYPE_IPS,
  NETWIB_PRIV_FMT_FIELDTYPE_PORTS
} netwib_priv_fmt_fieldtype;

typedef struct {
  netwib_uint32 skipsize;
  netwib_priv_fmt_fieldtype fieldtype;
  /* below are not always set */
  netwib_bool geneset;
  struct {
    netwib_priv_fmt_aligntype align;
    netwib_char padchar;
    netwib_uint32 minsize;
    netwib_bool skipfield;
  } gene;
  netwib_bool specset;
  union {
    netwib_priv_fmt_spec_uint specuint;
    netwib_priv_fmt_spec_int specint;
    netwib_priv_fmt_spec_bool specbool;
    netwib_priv_fmt_spec_cmp speccmp;
    netwib_priv_fmt_spec_buf specbuf;
    netwib_priv_fmt_spec_s specs;
  } spec;
} netwib_priv_fmtinfos;

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_fmt_analyze_type(netwib_conststring fmt,
                                               netwib_conststring type,
                                               netwib_priv_fmtinfos *pinfos)
{

  netwib_uint32 datasize;
  netwib_char firstchar;

  datasize = netwib_c_strlen(type);
  if (datasize == 0) {
    return(NETWIB_ERR_PAFMT);
  }

#define netwib__priv_fmt_analyze_type_cmp(txt,val) if (!netwib_c_memcmp(type, txt, datasize)) { pinfos->fieldtype = val; return(NETWIB_ERR_OK); }
  firstchar = type[0];
  switch(firstchar) {
  case 'b' :
    switch (datasize) {
    case 3 :
      netwib__priv_fmt_analyze_type_cmp("buf",
                                        NETWIB_PRIV_FMT_FIELDTYPE_BUF);
      break;
    case 4 :
      netwib__priv_fmt_analyze_type_cmp("bool",
                                        NETWIB_PRIV_FMT_FIELDTYPE_BOOL);
      netwib__priv_fmt_analyze_type_cmp("byte",
                                        NETWIB_PRIV_FMT_FIELDTYPE_BYTE);
      break;
    }
    break;
  case 'c' :
    switch (datasize) {
    case 1 :
      pinfos->fieldtype = NETWIB_PRIV_FMT_FIELDTYPE_C;
      return(NETWIB_ERR_OK);
      break;
    case 3 :
      netwib__priv_fmt_analyze_type_cmp("cmp",
                                        NETWIB_PRIV_FMT_FIELDTYPE_CMP);
      break;
    }
    break;
  case 'e' :
    switch (datasize) {
    case 3 :
      netwib__priv_fmt_analyze_type_cmp("eth",
                                        NETWIB_PRIV_FMT_FIELDTYPE_ETH);
      break;
    case 4 :
      netwib__priv_fmt_analyze_type_cmp("eths",
                                        NETWIB_PRIV_FMT_FIELDTYPE_ETHS);
      break;
    }
    break;
  case 'i' :
    switch (datasize) {
    case 2 :
      netwib__priv_fmt_analyze_type_cmp("ip",
                                        NETWIB_PRIV_FMT_FIELDTYPE_IP);
      break;
    case 3 :
      netwib__priv_fmt_analyze_type_cmp("ips",
                                        NETWIB_PRIV_FMT_FIELDTYPE_IPS);
      break;
    case 4 :
      netwib__priv_fmt_analyze_type_cmp("int8",
                                        NETWIB_PRIV_FMT_FIELDTYPE_INT8);
      break;
    case 5 :
      netwib__priv_fmt_analyze_type_cmp("int32",
                                        NETWIB_PRIV_FMT_FIELDTYPE_INT32);
      netwib__priv_fmt_analyze_type_cmp("int16",
                                        NETWIB_PRIV_FMT_FIELDTYPE_INT16);
      netwib__priv_fmt_analyze_type_cmp("int64",
                                        NETWIB_PRIV_FMT_FIELDTYPE_INT64);
      break;
    case 6 :
      netwib__priv_fmt_analyze_type_cmp("intmax",
                                        NETWIB_PRIV_FMT_FIELDTYPE_INTMAX);
      netwib__priv_fmt_analyze_type_cmp("intptr",
                                        NETWIB_PRIV_FMT_FIELDTYPE_INTPTR);
      break;
    }
    break;
  case 'p' :
    switch (datasize) {
    case 1 :
      pinfos->fieldtype = NETWIB_PRIV_FMT_FIELDTYPE_P;
      return(NETWIB_ERR_OK);
      break;
    case 4 :
      netwib__priv_fmt_analyze_type_cmp("port",
                                        NETWIB_PRIV_FMT_FIELDTYPE_PORT);
      break;
    case 5 :
      netwib__priv_fmt_analyze_type_cmp("ports",
                                        NETWIB_PRIV_FMT_FIELDTYPE_PORTS);
      break;
    }
    break;
  case 's' :
    switch (datasize) {
    case 1 :
      pinfos->fieldtype = NETWIB_PRIV_FMT_FIELDTYPE_S;
      return(NETWIB_ERR_OK);
      break;
    }
    break;
  case 'u' :
    switch (datasize) {
    case 5 :
      netwib__priv_fmt_analyze_type_cmp("uint8",
                                        NETWIB_PRIV_FMT_FIELDTYPE_UINT8);
      break;
    case 6 :
      netwib__priv_fmt_analyze_type_cmp("uint32",
                                        NETWIB_PRIV_FMT_FIELDTYPE_UINT32);
      netwib__priv_fmt_analyze_type_cmp("uint16",
                                        NETWIB_PRIV_FMT_FIELDTYPE_UINT16);
      netwib__priv_fmt_analyze_type_cmp("uint64",
                                        NETWIB_PRIV_FMT_FIELDTYPE_UINT64);
      break;
    case 7 :
      netwib__priv_fmt_analyze_type_cmp("uintmax",
                                        NETWIB_PRIV_FMT_FIELDTYPE_UINTMAX);
      netwib__priv_fmt_analyze_type_cmp("uintptr",
                                        NETWIB_PRIV_FMT_FIELDTYPE_UINTPTR);
      break;
    }
    break;
  }

  netwib_er(netwib_priv_buf_append_vfmt_error2("type not recognized", fmt));
  return(NETWIB_ERR_PAFMT);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_fmt_analyze_gene(netwib_conststring fmt,
                                               netwib_conststring gene,
                                               netwib_priv_fmtinfos *pinfos)
{
  netwib_conststring pgene;

  /* set default values */
  pinfos->geneset = NETWIB_TRUE;
  pinfos->gene.align = NETWIB_PRIV_FMT_ALIGNTYPE_LEFT;
  pinfos->gene.padchar = ' ';
  pinfos->gene.minsize = 0;
  pinfos->gene.skipfield = NETWIB_FALSE;

  /* decode pgene expressed as a regexp : "[*]{0,1}[lcr].[1-9][0-9]{0,1}"
     or simply "[*]{0,1}" */
  pgene = gene;

  if (*pgene == '*') {
    pinfos->gene.skipfield = NETWIB_TRUE;
    pgene++;
    if (*pgene == '\0') {
      /* stop here */
      return(NETWIB_ERR_OK);
    }
  }

  switch(*pgene) {
    case 'l' :
      pinfos->gene.align = NETWIB_PRIV_FMT_ALIGNTYPE_LEFT;
      break;
    case 'c' :
      pinfos->gene.align = NETWIB_PRIV_FMT_ALIGNTYPE_CENTER;
      break;
    case 'r' :
      pinfos->gene.align = NETWIB_PRIV_FMT_ALIGNTYPE_RIGHT;
      break;
    default :
      netwib_er(netwib_priv_buf_append_vfmt_error2("generic should start with l, c or r",
                                                 fmt));
      return(NETWIB_ERR_PAFMT);
  }
  pgene++;

  /* obtain padding */
  if (*pgene == '\0') {
    netwib_er(netwib_priv_buf_append_vfmt_error2("the padding char is missing",
                                               fmt));
    return(NETWIB_ERR_PAFMT);
  }
  pinfos->gene.padchar = *pgene++;

  /* obtain minsize */
  if (!netwib_c2_isdigit(*pgene)) {
    netwib_er(netwib_priv_buf_append_vfmt_error2("the size is missing", fmt));
    return(NETWIB_ERR_PAFMT);
  }
  pinfos->gene.minsize = netwib_c2_cto9(*pgene);
  pgene++;
  if (netwib_c2_isdigit(*pgene)) {
    pinfos->gene.minsize *= 10;
    pinfos->gene.minsize += netwib_c2_cto9(*pgene);
    pgene++;
  }

  if (*pgene != '\0') {
    netwib_er(netwib_priv_buf_append_vfmt_error2("specific parameter is too long",
                                               fmt));
    return(NETWIB_ERR_PAFMT);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_fmt_analyze_spec_s(netwib_conststring fmt,
                                                 netwib_conststring spec,
                                                 netwib_priv_fmt_spec_s *ps)
{

  /* set default values */
  ps->useglob = NETWIB_FALSE;

  if (spec == NULL) {
    return(NETWIB_ERR_OK);
  }

  if (!netwib_c_strcmp(spec, "glob")) {
    ps->useglob = NETWIB_TRUE;
  } else {
    netwib_er(netwib_priv_buf_append_vfmt_error2("specific is not recognized",
                                               fmt));
    return(NETWIB_ERR_PAFMT);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_fmt_analyze_spec_uint(netwib_conststring fmt,
                                                    netwib_conststring spec,
                                                    netwib_priv_fmt_spec_uint *ps)
{
  netwib_conststring pspec;

  /* set default values */
  ps->altformat = NETWIB_FALSE;
  ps->minsize = 0;
  ps->base = 10;

  if (spec == NULL) {
    return(NETWIB_ERR_OK);
  }
  pspec = spec;

  /* analyze pspec which should contain "[0]{0,1}[0-9]{0,2}[bouxX]" */

  /* set alternative format */
  if (*pspec == '#') {
    ps->altformat = NETWIB_TRUE;
    pspec++;
  }

  /* set minsize */
  if (netwib_c2_isdigit(*pspec)) {
      ps->minsize = netwib_c2_cto9(*pspec);
      pspec++;
  }
  if (netwib_c2_isdigit(*pspec)) {
    ps->minsize *= 10;
    ps->minsize += netwib_c2_cto9(*pspec);
    pspec++;
  }

  /* set base */
  switch (*pspec) {
    case 'b' :
      ps->base = 2;
      pspec++;
      break;
    case 'o' :
      ps->base = 8;
      pspec++;
      break;
    case 'u' :
      ps->base = 10;
      pspec++;
      break;
    case 'x' :
      ps->base = 16;
      pspec++;
      break;
    case 'X' :
      ps->base = 17;
      pspec++;
      break;
    case '\0' :
      break;
    default :
      netwib_er(netwib_priv_buf_append_vfmt_error2("specific base is not b, o, u, x or X", fmt));
      return(NETWIB_ERR_PAFMT);
  }

  /* now, should be empty */
  if (*pspec != '\0') {
    netwib_er(netwib_priv_buf_append_vfmt_error2("specific base is not recognized", fmt));
    return(NETWIB_ERR_PAFMT);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_fmt_analyze_spec_int(netwib_conststring fmt,
                                                   netwib_conststring spec,
                                                   netwib_priv_fmt_spec_int *ps)
{
  netwib_conststring pspec;

  /* set default values */
  ps->sign = NETWIB_FALSE;
  ps->minsize = 0;

  if (spec == NULL) {
    return(NETWIB_ERR_OK);
  }
  pspec = spec;

  /* analyze pspec which should contain "[+]{0,1}[0-9]{0,2}" */

  /* set sign */
  if (*pspec == '+') {
    ps->sign = NETWIB_TRUE;
    pspec++;
  }

  /* set minsize */
  if (netwib_c2_isdigit(*pspec)) {
    ps->minsize = netwib_c2_cto9(*pspec);
    pspec++;
  }
  if (netwib_c2_isdigit(*pspec)) {
    ps->minsize *= 10;
    ps->minsize += netwib_c2_cto9(*pspec);
    pspec++;
  }

  /* now, should be empty */
  if (*pspec != '\0') {
    netwib_er(netwib_priv_buf_append_vfmt_error2("specific base is not recognized", fmt));
    return(NETWIB_ERR_PAFMT);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_fmt_analyze_spec_bool(netwib_conststring fmt,
                                                    netwib_conststring spec,
                                                    netwib_priv_fmt_spec_bool *ps)
{
  netwib_conststring pspec;

  /* set default value */
  ps->type = 't';

  if (spec == NULL) {
    return(NETWIB_ERR_OK);
  }
  pspec = spec;

  /* analyze pspec which should contain "[0tTyYsS]{0,1}" */
  switch(*pspec) {
    case '0' :
    case 't' :
    case 'T' :
    case 'y' :
    case 'Y' :
    case 's' :
    case 'S' :
      ps->type = *pspec;
      break;
    default :
      netwib_er(netwib_priv_buf_append_vfmt_error2("specific is not 0, t, T, y, Y, s or S", fmt));
      return(NETWIB_ERR_PAFMT);
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_fmt_analyze_spec_cmp(netwib_conststring fmt,
                                                   netwib_conststring spec,
                                                   netwib_priv_fmt_spec_cmp *ps)
{
  netwib_conststring pspec;

  /* set default value */
  ps->type = 't';

  if (spec == NULL) {
    return(NETWIB_ERR_OK);
  }
  pspec = spec;

  /* analyze pspec which should contain "[=0e]{0,1}" */
  switch(*pspec) {
    case '=' :
    case '0' :
    case 'e' :
      ps->type = *pspec;
      break;
    default :
      netwib_er(netwib_priv_buf_append_vfmt_error2("specific is not =, 0 or e",
                                                 fmt));
      return(NETWIB_ERR_PAFMT);
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_fmt_analyze_spec_buf(netwib_conststring fmt,
                                                   netwib_conststring spec,
                                                   netwib_priv_fmt_spec_buf *ps)
{

  /* set default values */
  ps->useglob = NETWIB_FALSE;

  if (spec == NULL) {
    return(NETWIB_ERR_OK);
  }

  if (!netwib_c_strcmp(spec, "glob")) {
    ps->useglob = NETWIB_TRUE;
  } else {
    netwib_er(netwib_priv_buf_append_vfmt_error2("specific is not recognized",
                                               fmt));
    return(NETWIB_ERR_PAFMT);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_fmt_analyze2(netwib_conststring fmt,
                                           netwib_priv_fmtinfos *pinfos)
{
  netwib_char array[3][NETWIB_PRIV_FMT_FIELD_MAXSIZE+1];
  netwib_conststring pfmt;
  netwib_string pgene, ptype, pspec;
  netwib_char c;
  netwib_uint32 setsize, storeinarray;
  netwib_bool continuetoloop;

  /*
    Decompose generalformat;type:specificformat :
      %{type}
      %{generalformat;type:specificformat}
      %{generalformat;type}
      %{type:specificformat}
  */

  pfmt = fmt + 1;
  setsize = 0;
  storeinarray = 0;
  pgene = NULL;
  ptype = NULL;
  pspec = NULL;
  continuetoloop = NETWIB_TRUE;
  while (continuetoloop) {
    c = *pfmt++;
    switch (c) {
      case '\0' :
        netwib_er(netwib_priv_buf_append_vfmt_error2("character '}' not found",
                                                     fmt));
        return(NETWIB_ERR_PAFMT);
      case '}' :
        array[storeinarray][setsize] = '\0';
        if (storeinarray == 0) {
          ptype = (netwib_string)&(array[storeinarray]);
        } else if (storeinarray == 1) {
          if (ptype == NULL) {
            ptype = (netwib_string)&(array[storeinarray]);
          } else {
            pspec = (netwib_string)&(array[storeinarray]);
          }
        } else {
          pspec = (netwib_string)&(array[storeinarray]);
        }
        continuetoloop = NETWIB_FALSE;
        break;
      case ';' :
        array[storeinarray][setsize] = '\0';
        if (storeinarray == 0) {
          pgene = (netwib_string)&(array[storeinarray]);
          storeinarray++;
          setsize = 0;
        } else {
          netwib_er(netwib_priv_buf_append_vfmt_error2("char ';' found too late", fmt));
          return(NETWIB_ERR_PAFMT);
        }
        break;
      case ':' :
        array[storeinarray][setsize] = '\0';
        if (storeinarray == 2) {
          netwib_er(netwib_priv_buf_append_vfmt_error2("char ':' found too late", fmt));
          return(NETWIB_ERR_PAFMT);
        } else {
          ptype = (netwib_string)&(array[storeinarray]);
          storeinarray++;
          setsize = 0;
        }
        break;
      default :
        array[storeinarray][setsize++] = c;
    }
    if (setsize > NETWIB_PRIV_FMT_FIELD_MAXSIZE) {
      netwib_er(netwib_priv_buf_append_vfmt_error2("format is too long", fmt));
      return(NETWIB_ERR_PAFMT);
    }
  }

  /* reset pointers if string are empty */
  if (pgene != NULL && pgene[0] == '\0') pgene = NULL;
  if (ptype != NULL && ptype[0] == '\0') ptype = NULL;
  if (pspec != NULL && pspec[0] == '\0') pspec = NULL;

  /* set info values */
  pinfos->skipsize = pfmt - fmt;

  if (ptype == NULL) {
    netwib_er(netwib_priv_buf_append_vfmt_error2("format type must be present",
                                                 fmt));
    return(NETWIB_ERR_PAFMT);
  }
  netwib_er(netwib_priv_fmt_analyze_type(fmt, ptype, pinfos));

  if (pgene == NULL) {
    pinfos->geneset = NETWIB_FALSE;
  } else {
    netwib_er(netwib_priv_fmt_analyze_gene(fmt, pgene, pinfos));
  }

  switch(pinfos->fieldtype) {
    case NETWIB_PRIV_FMT_FIELDTYPE_UINT32 :
    case NETWIB_PRIV_FMT_FIELDTYPE_UINT16 :
    case NETWIB_PRIV_FMT_FIELDTYPE_UINT8 :
    case NETWIB_PRIV_FMT_FIELDTYPE_UINT64 :
    case NETWIB_PRIV_FMT_FIELDTYPE_UINTMAX :
    case NETWIB_PRIV_FMT_FIELDTYPE_UINTPTR :
    case NETWIB_PRIV_FMT_FIELDTYPE_BYTE :
      netwib_er(netwib_priv_fmt_analyze_spec_uint(fmt, pspec,
                                                  &pinfos->spec.specuint));
      pinfos->specset = NETWIB_TRUE;
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_INT32 :
    case NETWIB_PRIV_FMT_FIELDTYPE_INT16 :
    case NETWIB_PRIV_FMT_FIELDTYPE_INT8 :
    case NETWIB_PRIV_FMT_FIELDTYPE_INT64 :
    case NETWIB_PRIV_FMT_FIELDTYPE_INTMAX :
    case NETWIB_PRIV_FMT_FIELDTYPE_INTPTR :
      netwib_er(netwib_priv_fmt_analyze_spec_int(fmt, pspec,
                                                 &pinfos->spec.specint));
      pinfos->specset = NETWIB_TRUE;
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_BOOL :
      if (pspec == NULL) {
        pinfos->specset = NETWIB_FALSE;
      } else {
        netwib_er(netwib_priv_fmt_analyze_spec_bool(fmt, pspec,
                                                    &pinfos->spec.specbool));
        pinfos->specset = NETWIB_TRUE;
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_CMP :
      if (pspec == NULL) {
        pinfos->specset = NETWIB_FALSE;
      } else {
        netwib_er(netwib_priv_fmt_analyze_spec_cmp(fmt, pspec,
                                                 &pinfos->spec.speccmp));
        pinfos->specset = NETWIB_TRUE;
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_S :
      netwib_er(netwib_priv_fmt_analyze_spec_s(fmt, pspec,
                                               &pinfos->spec.specs));
      pinfos->specset = NETWIB_TRUE;
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_BUF :
      netwib_er(netwib_priv_fmt_analyze_spec_buf(fmt, pspec,
                                               &pinfos->spec.specbuf));
      pinfos->specset = NETWIB_TRUE;
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_C :
    case NETWIB_PRIV_FMT_FIELDTYPE_P :
    case NETWIB_PRIV_FMT_FIELDTYPE_ETH :
    case NETWIB_PRIV_FMT_FIELDTYPE_IP :
    case NETWIB_PRIV_FMT_FIELDTYPE_PORT :
    case NETWIB_PRIV_FMT_FIELDTYPE_ETHS :
    case NETWIB_PRIV_FMT_FIELDTYPE_IPS :
    case NETWIB_PRIV_FMT_FIELDTYPE_PORTS :
      if (pspec == NULL) {
        pinfos->specset = NETWIB_FALSE;
      } else {
        netwib_er(netwib_priv_buf_append_vfmt_error2("there should be no specific parameter", fmt));
        return(NETWIB_ERR_PAFMT);
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_PERCENT :
    case NETWIB_PRIV_FMT_FIELDTYPE_END :
      return(NETWIB_ERR_LOINTERNALERROR);
      break;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_fmt_analyze(netwib_conststring fmt,
                                          netwib_priv_fmtinfos *pinfos)
{
  char fmtfirstchar;

  fmtfirstchar = *fmt;

  pinfos->geneset = NETWIB_FALSE;
  pinfos->specset = NETWIB_FALSE;

  /* fmt ends */
  if (fmtfirstchar == '\0') {
    netwib_er(netwib_priv_buf_append_vfmt_error("format ends with '%'"));
    return(NETWIB_ERR_PAFMT);
  }

  /* character '%' */
  if (fmtfirstchar == '%') {
    pinfos->fieldtype = NETWIB_PRIV_FMT_FIELDTYPE_PERCENT;
    pinfos->skipsize = 1;
    return(NETWIB_ERR_OK);
  }

  /* char */
  if (fmtfirstchar == 'c') {
    pinfos->fieldtype = NETWIB_PRIV_FMT_FIELDTYPE_C;
    pinfos->skipsize = 1;
    return(NETWIB_ERR_OK);
  }

  /* string */
  if (fmtfirstchar == 's') {
    pinfos->fieldtype = NETWIB_PRIV_FMT_FIELDTYPE_S;
    pinfos->skipsize = 1;
    return(NETWIB_ERR_OK);
  }

  /* pointer */
  if (fmtfirstchar == 'p') {
    pinfos->fieldtype = NETWIB_PRIV_FMT_FIELDTYPE_P;
    pinfos->skipsize = 1;
    return(NETWIB_ERR_OK);
  }

  /* end */
  if (fmtfirstchar == '$') {
    pinfos->fieldtype = NETWIB_PRIV_FMT_FIELDTYPE_END;
    pinfos->skipsize = 1;
    return(NETWIB_ERR_OK);
  }

  /* specific type */
  if (fmtfirstchar == '{') {
    netwib_er(netwib_priv_fmt_analyze2(fmt, pinfos));
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_priv_buf_append_vfmt_error2("format is not recognized",
                                               fmt));
  return(NETWIB_ERR_PAFMT);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
/* 99 + '\0' = 100 characters. Do not define NETWIB_PRIV_FMT_ARR_SIZE
   to less than 100 (not checked). */
#define NETWIB_PRIV_FMT_ARR_SIZE 100

/*-------------------------------------------------------------*/
static netwib_uint64 netwib_priv_fmt_ui64_unused;
#define netwib_priv_fmt_append_uintmax(uimax,ps,array) netwib_priv_fmt_append_uint(NETWIB_TRUE,uimax,netwib_priv_fmt_ui64_unused,ps,array)
#if NETWIB_INT64_FAKE == 0
 #define netwib_priv_fmt_append_uint64(ui64,ps,array) netwib_priv_fmt_append_uint(NETWIB_TRUE,ui64,netwib_priv_fmt_ui64_unused,ps,array)
#else
 #define netwib_priv_fmt_append_uint64(ui64,ps,array) netwib_priv_fmt_append_uint(NETWIB_FALSE,0,ui64,ps,array)
#endif
static netwib_err netwib_priv_fmt_append_uint(netwib_bool usemax,
                                              netwib_uintmax uimax,
                                              netwib_uint64 ui64,
                                              netwib_priv_fmt_spec_uint *ps,
                                              netwib_string array)
{
  netwib_char localarray[NETWIB_PRIV_FMT_ARR_SIZE], *ploc, c;
  netwib_string pout;
  netwib_uint32 nbdigit, nbzeroes, c32, base32;
  netwib_uint64 zero;

  netwib__uint64_init_uint32(0, zero);

  nbdigit = 0;
  base32 = ps->base;
  if (ps->base == 17) base32 = 16;
  if (usemax && uimax != 0) {
    netwib_uintmax uidivbase, uidivbaseperbase;
    do {
      uidivbase = uimax / base32;
      uidivbaseperbase = uidivbase * base32;
      c32 = (netwib_uint32)(uimax - uidivbaseperbase);
      if (ps->base == 17) {
        c = netwib_c2_16toC(c32);
      } else {
        c = netwib_c2_16toc(c32);
      }
      localarray[nbdigit++] = c;
      uimax = uidivbase;
    } while (uimax != 0);
  } else if (!usemax && netwib__uint64_cmp_ne(ui64, zero)) {
    netwib_uint64 base64, uidivbase, uidivbaseperbase, c64, remainder;
    netwib__uint64_init_uint32(base32, base64);
    do {
      netwib__uint64_div(ui64, base64, &uidivbase, &remainder);
      netwib__uint64_mul(uidivbase, base64, &uidivbaseperbase);
      netwib__uint64_sub(ui64, uidivbaseperbase, c64);
      netwib__uint32_init_uint64(c64, c32);
      if (ps->base == 17) {
        c = netwib_c2_16toC(c32);
      } else {
        c = netwib_c2_16toc(c32);
      }
      localarray[nbdigit++] = c;
      ui64 = uidivbase;
    } while (netwib__uint64_cmp_ne(ui64, zero));
  } else {
    localarray[nbdigit++] = '0';
  }
  localarray[nbdigit] = '\0';

  if (ps->minsize == 0) {
    nbzeroes = 0;
  } else {
    if (nbdigit >= ps->minsize) {
      nbzeroes = 0;
    } else {
      nbzeroes = ps->minsize - nbdigit;
    }
  }

  ploc = localarray + nbdigit;
  pout = array;
  switch(ps->base) {
  case 2 :
    if (ps->altformat) {
      *pout++ = 'b';
    }
    while (nbzeroes) {
      *pout++ = '0';
      nbzeroes--;
    }
    break;
  case 8 :
    if (ps->altformat) {
      *pout++ = '0';
    }
    while (nbzeroes) {
      *pout++ = '0';
      nbzeroes--;
    }
    break;
  case 10 :
    while (nbzeroes) {
      *pout++ = ' ';
      nbzeroes--;
    }
    break;
  case 16 :
  case 17 :
    if (ps->altformat) {
      *pout++ = '0';
      *pout++ = 'x';
    }
    while (nbzeroes) {
      *pout++ = '0';
      nbzeroes--;
    }
    break;
  default :
    return(NETWIB_ERR_LOINTERNALERROR);
  }
  while (nbdigit) {
    *pout++ = *--ploc;
    nbdigit--;
  }
  *pout++ = '\0';

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#define netwib_priv_fmt_append_intmax(imax,ps,array) netwib_priv_fmt_append_int(NETWIB_TRUE,imax,netwib_priv_fmt_ui64_unused,ps,array)
#if NETWIB_INT64_FAKE == 0
 #define netwib_priv_fmt_append_int64(i64,ps,array) netwib_priv_fmt_append_int(NETWIB_TRUE,i64,netwib_priv_fmt_ui64_unused,ps,array)
#else
 #define netwib_priv_fmt_append_int64(i64,ps,array) netwib_priv_fmt_append_int(NETWIB_FALSE,0,i64,ps,array)
#endif
static netwib_err netwib_priv_fmt_append_int(netwib_bool usemax,
                                             netwib_intmax imax,
                                             netwib_int64 i64,
                                             netwib_priv_fmt_spec_int *ps,
                                             netwib_string array)
{
  netwib_priv_fmt_spec_uint su;
  netwib_uint32 startat;
  netwib_uintmax uimax=0;
  netwib_uint64 ui64, zero;
  netwib_cmp cmp;

  netwib__uint64_init_uint32(0, ui64);

  cmp = NETWIB_CMP_LT;
  if (usemax) {
    if (imax == 0) {
      cmp = NETWIB_CMP_EQ;
    } else if (imax > 0) {
      cmp = NETWIB_CMP_GT;
    }
  } else {
    netwib__uint64_init_uint32(0, zero);
    if (netwib__int64_cmp_eq(i64, zero)) {
      cmp = NETWIB_CMP_EQ;
    } else if (netwib__int64_cmp_gt(i64, zero)) {
      cmp = NETWIB_CMP_GT;
    }
  }

  /* eventually put a sign */
  startat = 0;
  if (cmp == NETWIB_CMP_LT) {
    if (usemax) {
      uimax = (netwib_uintmax)-imax;
    } else {
      netwib__int64_neg(i64, ui64);
    }
    /* put a '-' */
    array[0] = '-';
    startat = 1;
  } else {
    if (usemax) {
      uimax = (netwib_uintmax)imax;
    } else {
      ui64 = (netwib_uint64)i64;
    }
    /* eventually put a '+' (nothing for zero) */
    if (ps->sign && cmp == NETWIB_CMP_GT) {
      array[0] = '+';
      startat = 1;
    }
  }
  if (startat) {
    /* shrink minsize */
    if (ps->minsize) {
      ps->minsize--;
    }
  }

  su.altformat = NETWIB_FALSE;
  su.minsize = ps->minsize;
  su.base = 10;
  netwib_er(netwib_priv_fmt_append_uint(usemax, uimax, ui64,
                                        &su, array+startat));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_fmt_append_eth(netwib_consteth *peth,
                                             netwib_string array)
{
  netwib_string parray;
  netwib_uint32 ethbyte, ethquartet;
  netwib_uint32 i;

  parray = array;
  for (i = 0; i < 6; i++) {
    ethbyte = peth->b[i];
    ethquartet = (ethbyte>>4) & 0x0F;
    *parray++ = netwib_c2_16toC(ethquartet);
    ethquartet = ethbyte & 0x0F;
    *parray++ = netwib_c2_16toC(ethquartet);
    if (i != 5) {
      *parray++ = ':';
    }
  }
  *parray ='\0';

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_fmt_append_gene(netwib_conststring str,
                                              netwib_priv_fmtinfos *pinfos,
                                              netwib_buf *pbuf)
{
  netwib_char localarray[NETWIB_PRIV_FMT_ARR_SIZE], *ploc;
  netwib_conststring pstr;
  netwib_uint32 strlenstr, nbpad, nbpadl, nbpadr;
  netwib_err ret;

  if (pinfos->gene.minsize > NETWIB_PRIV_FMT_ARR_SIZE-1) {
    return(NETWIB_ERR_LOINTERNALERROR);
  }

  /* simple case */
  strlenstr = netwib_c_strlen(str);
  if (pinfos->gene.minsize <= strlenstr) {
    netwib_er(netwib_buf_append_string(str, pbuf));
    return(NETWIB_ERR_OK);
  }

  /* compute padding */
  nbpadl = 0; /* for compiler warning */
  nbpadr = 0; /* for compiler warning */
  nbpad = pinfos->gene.minsize - strlenstr;
  switch (pinfos->gene.align) {
    case NETWIB_PRIV_FMT_ALIGNTYPE_LEFT :
      nbpadl = 0;
      nbpadr = nbpad;
      break;
    case NETWIB_PRIV_FMT_ALIGNTYPE_CENTER :
      nbpadl = nbpad >> 1;
      nbpadr = nbpadl;
      if (nbpad&1) nbpadr++; /* if odd, one more at right */
      break;
    case NETWIB_PRIV_FMT_ALIGNTYPE_RIGHT :
      nbpadl = nbpad;
      nbpadr = 0;
      break;
  }

  /* generate string */
  ploc = localarray;
  pstr = str;
  while (nbpadl) {
    *ploc++ = pinfos->gene.padchar;
    nbpadl--;
  }
  while (strlenstr) {
    *ploc++ = *pstr++;
    strlenstr--;
  }
  while (nbpadr) {
    *ploc++ = pinfos->gene.padchar;
    nbpadr--;
  }
  *ploc = '\0';

  /* add it */
  ret = netwib_buf_append_string(localarray, pbuf);
  netwib__localarray_ifbuf_wipe(pbuf, localarray);
  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_fmt_append(netwib_buf *pbuf,
                                         netwib_conststring fmt,
                                         va_list *pap,
                                         netwib_priv_fmtinfos *pinfos)
{
  netwib_char array[NETWIB_PRIV_FMT_ARR_SIZE];
  netwib_buf localbuf;
  netwib_string localstring;
  netwib_err ret;

  /* check if everything is fine for append */
  if (pinfos->geneset && pinfos->gene.skipfield) {
    netwib_er(netwib_priv_buf_append_vfmt_error2("an '*' is forbidden for append", fmt));
    return(NETWIB_ERR_PAFMT);
  }

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &localbuf));

  /* append depending on type */
  switch(pinfos->fieldtype) {
    case NETWIB_PRIV_FMT_FIELDTYPE_END :
      netwib_er(netwib_priv_buf_append_vfmt_error2("a %$ is forbidden for append", fmt));
      return(NETWIB_ERR_PAFMT);
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_PERCENT :
      netwib_er(netwib_buf_append_byte('%', pbuf));
      /* leave here */
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_C :
      {
        char c;
        /* Note : on some systems, there is a "cast increases required
           alignment size" warning here. It can be ignored */
        c = (char)va_arg(*pap, int);
        if (pinfos->geneset) {
          array[0] = c;
          array[1] = '\0';
          netwib_er(netwib_priv_fmt_append_gene(array, pinfos, pbuf));
        } else {
          netwib_er(netwib_buf_append_byte(c, pbuf));
        }
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_BOOL :
      {
        netwib_bool b;
        netwib_conststring pstr;
        b = va_arg(*pap, netwib_bool);
        pstr = b?"true":"false";
        if (pinfos->specset) {
          switch(pinfos->spec.specbool.type) {
            case '0' : pstr = b?"1":"0"; break;
            case 'T' : pstr = b?"TRUE":"FALSE"; break;
            case 'y' : pstr = b?"yes":"no"; break;
            case 'Y' : pstr = b?"YES":"NO"; break;
            case 's' : pstr = b?"set":"unset"; break;
            case 'S' : pstr = b?"SET":"UNSET"; break;
          }
        }
        if (pinfos->geneset) {
          netwib_er(netwib_priv_fmt_append_gene(pstr, pinfos, pbuf));
        } else {
          netwib_er(netwib_buf_append_string(pstr, pbuf));
        }
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_CMP :
      {
        netwib_cmp cmp;
        netwib_conststring pstr;
        cmp = va_arg(*pap, netwib_cmp);
        pstr = (cmp==NETWIB_CMP_LT)?"<":((cmp==NETWIB_CMP_EQ)?"=":">");
        if (pinfos->specset) {
          switch(pinfos->spec.specbool.type) {
            case '0' :
              pstr = (cmp==NETWIB_CMP_LT)?"-":((cmp==NETWIB_CMP_EQ)?"0":"+");
              break;
            case 'e' :
              pstr = (cmp==NETWIB_CMP_LT)?"lt":((cmp==NETWIB_CMP_EQ)?"eq":"gt");
              break;
          }
        }
        if (pinfos->geneset) {
          netwib_er(netwib_priv_fmt_append_gene(pstr, pinfos, pbuf));
        } else {
          netwib_er(netwib_buf_append_string(pstr, pbuf));
        }
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_S :
      {
        netwib_char *pc;
        if (pinfos->specset && pinfos->spec.specs.useglob) {
          netwib_er(netwib_priv_buf_append_vfmt_error2("glob is forbidden for append", fmt));
          return(NETWIB_ERR_PAFMT);
        }
        pc = va_arg(*pap, netwib_char*);
        if (pinfos->geneset) {
          netwib_er(netwib_priv_fmt_append_gene(pc, pinfos, pbuf));
        } else {
          netwib_er(netwib_buf_append_string(pc, pbuf));
        }
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_P :
      {
        netwib_priv_fmt_spec_uint su;
        void *p;
        p = va_arg(*pap, void*);
        array[0] = '0';
        array[1] = 'x';
        su.altformat = NETWIB_FALSE;
        su.minsize = 2*sizeof(void*);
        su.base = 16;
        netwib_er(netwib_priv_fmt_append_uintmax((netwib_uintptr)p, &su,
                                                 array+2));
        if (pinfos->geneset) {
          netwib_er(netwib_priv_fmt_append_gene(array, pinfos, pbuf));
        } else {
          netwib_er(netwib_buf_append_string(array, pbuf));
        }
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_UINT32 :
    case NETWIB_PRIV_FMT_FIELDTYPE_UINT16 :
    case NETWIB_PRIV_FMT_FIELDTYPE_UINT8 :
    case NETWIB_PRIV_FMT_FIELDTYPE_BYTE :
    case NETWIB_PRIV_FMT_FIELDTYPE_UINTMAX :
    case NETWIB_PRIV_FMT_FIELDTYPE_UINTPTR :
      {
        netwib_uintmax ui=0;
        switch(pinfos->fieldtype) {
        case NETWIB_PRIV_FMT_FIELDTYPE_UINTMAX :
          ui = va_arg(*pap, netwib_uintmax);
          break;
        case NETWIB_PRIV_FMT_FIELDTYPE_UINTPTR :
          ui = va_arg(*pap, netwib_uintptr);
          break;
        default :
          ui = va_arg(*pap, netwib_uint32);
          break;
        }
        netwib_er(netwib_priv_fmt_append_uintmax(ui, &pinfos->spec.specuint,
                                                 array));
        if (pinfos->geneset) {
          netwib_er(netwib_priv_fmt_append_gene(array, pinfos, pbuf));
        } else {
          netwib_er(netwib_buf_append_string(array, pbuf));
        }
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_UINT64 :
      {
        netwib_uint64 ui;
        ui = va_arg(*pap, netwib_uint64);
        netwib_er(netwib_priv_fmt_append_uint64(ui, &pinfos->spec.specuint,
                                                array));
        if (pinfos->geneset) {
          netwib_er(netwib_priv_fmt_append_gene(array, pinfos, pbuf));
        } else {
          netwib_er(netwib_buf_append_string(array, pbuf));
        }
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_INT32 :
    case NETWIB_PRIV_FMT_FIELDTYPE_INT16 :
    case NETWIB_PRIV_FMT_FIELDTYPE_INT8 :
    case NETWIB_PRIV_FMT_FIELDTYPE_INTMAX :
    case NETWIB_PRIV_FMT_FIELDTYPE_INTPTR :
      {
        netwib_intmax i=0;
        switch(pinfos->fieldtype) {
        case NETWIB_PRIV_FMT_FIELDTYPE_INTMAX :
          i = va_arg(*pap, netwib_intmax);
          break;
        case NETWIB_PRIV_FMT_FIELDTYPE_INTPTR :
          i = va_arg(*pap, netwib_intptr);
          break;
        default :
          i = va_arg(*pap, netwib_int32);
          break;
        }
        netwib_er(netwib_priv_fmt_append_intmax(i, &pinfos->spec.specint,
                                                array));
        if (pinfos->geneset) {
          netwib_er(netwib_priv_fmt_append_gene(array, pinfos, pbuf));
        } else {
          netwib_er(netwib_buf_append_string(array, pbuf));
        }
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_INT64 :
      {
        netwib_int64 i;
        i = va_arg(*pap, netwib_int64);
        netwib_er(netwib_priv_fmt_append_int64(i, &pinfos->spec.specint,
                                               array));
        if (pinfos->geneset) {
          netwib_er(netwib_priv_fmt_append_gene(array, pinfos, pbuf));
        } else {
          netwib_er(netwib_buf_append_string(array, pbuf));
        }
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_BUF :
      {
        netwib_buf *pfmtbuf;
        if (pinfos->specset && pinfos->spec.specbuf.useglob) {
          netwib_er(netwib_priv_buf_append_vfmt_error2("glob is forbidden for append", fmt));
          return(NETWIB_ERR_PAFMT);
        }
        pfmtbuf = va_arg(*pap, netwib_buf*);
        /* do the transfer now because needed in netwib_priv_fmt_append_gene */
        netwib__buf_transfersensitive(pfmtbuf, pbuf);
        if (pinfos->geneset) {
          ret = netwib_constbuf_ref_string(pfmtbuf, &localstring);
          if (ret != NETWIB_ERR_OK) {
            netwib_er(netwib_buf_append_buf(pfmtbuf, &localbuf));
            netwib_er(netwib_buf_ref_string(&localbuf, &localstring));
          }
          netwib_er(netwib_priv_fmt_append_gene(localstring, pinfos, pbuf));
        } else {
          netwib_er(netwib_buf_append_buf(pfmtbuf, pbuf));
        }
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_ETH :
      {
        netwib_eth *peth;
        peth = va_arg(*pap, netwib_eth*);
        netwib_er(netwib_priv_fmt_append_eth(peth, array));
        if (pinfos->geneset) {
          netwib_er(netwib_priv_fmt_append_gene(array, pinfos, pbuf));
        } else {
          netwib_er(netwib_buf_append_string(array, pbuf));
        }
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_IP :
      {
        netwib_ip *pip;
        pip = va_arg(*pap, netwib_ip*);
        if (pinfos->geneset) {
          netwib_er(netwib_buf_append_ip(pip, NETWIB_IP_ENCODETYPE_IP,
                                         &localbuf));
          netwib_er(netwib_buf_ref_string(&localbuf, &localstring));
          netwib_er(netwib_priv_fmt_append_gene(localstring, pinfos, pbuf));
        } else {
          netwib_er(netwib_buf_append_ip(pip, NETWIB_IP_ENCODETYPE_IP,
                                         pbuf));
        }
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_PORT :
      {
        netwib_priv_fmt_spec_uint su;
        netwib_port ui;
        ui = va_arg(*pap, netwib_port);
        su.altformat = NETWIB_FALSE;
        su.minsize = 0;
        su.base = 10;
        netwib_er(netwib_priv_fmt_append_uintmax(ui, &su, array));
        if (pinfos->geneset) {
          netwib_er(netwib_priv_fmt_append_gene(array, pinfos, pbuf));
        } else {
          netwib_er(netwib_buf_append_string(array, pbuf));
        }
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_ETHS :
      {
        netwib_eths *peths;
        peths = va_arg(*pap, netwib_eths*);
        if (pinfos->geneset) {
          netwib_er(netwib_buf_append_eths(peths, &localbuf));
          netwib_er(netwib_buf_ref_string(&localbuf, &localstring));
          netwib_er(netwib_priv_fmt_append_gene(localstring, pinfos, pbuf));
        } else {
          netwib_er(netwib_buf_append_eths(peths, pbuf));
        }
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_IPS :
      {
        netwib_ips *pips;
        pips = va_arg(*pap, netwib_ips*);
        if (pinfos->geneset) {
          netwib_er(netwib_buf_append_ips(pips, NETWIB_IPS_ENCODETYPE_BEST,
                                          &localbuf));
          netwib_er(netwib_buf_ref_string(&localbuf, &localstring));
          netwib_er(netwib_priv_fmt_append_gene(localstring, pinfos, pbuf));
        } else {
          netwib_er(netwib_buf_append_ips(pips, NETWIB_IPS_ENCODETYPE_BEST,
                                          pbuf));
        }
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_PORTS :
      {
        netwib_ports *pports;
        pports = va_arg(*pap, netwib_ports*);
        if (pinfos->geneset) {
          netwib_er(netwib_buf_append_ports(pports, &localbuf));
          netwib_er(netwib_buf_ref_string(&localbuf, &localstring));
          netwib_er(netwib_priv_fmt_append_gene(localstring, pinfos, pbuf));
        } else {
          netwib_er(netwib_buf_append_ports(pports, pbuf));
        }
      }
      break;
  }

  netwib_er(netwib_buf_close(&localbuf));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_buf_append_vfmt(netwib_buf *pbuf,
                                       netwib_conststring fmt,
                                       va_list *pap)
{
  netwib_conststring pfmt, ppercent;
  netwib_priv_fmtinfos infos;
  netwib_uint32 savedsize=0;
  netwib_err ret;

  /* save position in case of error */
  if (pbuf != NULL) {
    savedsize = netwib__buf_ref_data_size(pbuf);
  }

  /* main loop */
  pfmt = fmt;
  ret = NETWIB_ERR_LOINTERNALERROR;
  while (NETWIB_TRUE) {
    /* search '%' */
    ppercent = netwib_c_strchr(pfmt, '%');
    if (ppercent == NULL) {
      /* not found, so copy the end of format */
      ret = netwib_buf_append_string(pfmt, pbuf);
      break;
    } else {
      /* first append data before '%' */
      ret = netwib_buf_append_data((netwib_constdata)pfmt, ppercent-pfmt,
                                   pbuf);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
      /* analyze format and complete */
      pfmt = ppercent + 1;
      ret = netwib_priv_fmt_analyze(pfmt, &infos);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
      ret = netwib_priv_fmt_append(pbuf, pfmt, pap, &infos);
      if (ret != NETWIB_ERR_OK) {
        break;
      }
      pfmt += infos.skipsize;
    }
  }

  /* on error, restore first pos */
  if (ret != NETWIB_ERR_OK) {
    if (pbuf != NULL) {
      pbuf->endoffset = pbuf->beginoffset + savedsize;
    }
  }

  /* leave */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_fmt(netwib_buf *pbuf,
                                 netwib_conststring fmt,
                                 ...)
{
  va_list ap;
  netwib_err ret;

  va_start(ap, fmt);
  ret = netwib_priv_buf_append_vfmt(pbuf, fmt, &ap);
  va_end(ap);
  return(ret);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
typedef struct {
  netwib_priv_fmtinfos fmtinfos;
  netwib_ptr ptr;
} netwib_priv_fmt_scanitem;

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_fmt_regexp(netwib_buf *pbuf,
                                         netwib_priv_fmtinfos *pinfos)
{
  netwib_bool skipfield;

  if (pinfos->geneset) {
    if (pinfos->gene.minsize) {
      if (pinfos->gene.align != NETWIB_PRIV_FMT_ALIGNTYPE_LEFT) {
        netwib_byte array[4];
        array[0] = '[';
        array[1] = pinfos->gene.padchar;
        array[2] = ']';
        array[3] = '*';
        netwib_er(netwib_buf_append_data(array, 4, pbuf));
      }
    }
  }

  skipfield = NETWIB_FALSE;
  if (pinfos->geneset) {
    if (pinfos->gene.skipfield) {
      skipfield = NETWIB_TRUE;
    }
  }

  if (!skipfield) {
    /* those cases can easily be encapsulated by parenthesis */
    switch(pinfos->fieldtype) {
      case NETWIB_PRIV_FMT_FIELDTYPE_C :
      case NETWIB_PRIV_FMT_FIELDTYPE_P :
      case NETWIB_PRIV_FMT_FIELDTYPE_INT32 :
      case NETWIB_PRIV_FMT_FIELDTYPE_INT16 :
      case NETWIB_PRIV_FMT_FIELDTYPE_INT8 :
      case NETWIB_PRIV_FMT_FIELDTYPE_INT64 :
      case NETWIB_PRIV_FMT_FIELDTYPE_INTMAX :
      case NETWIB_PRIV_FMT_FIELDTYPE_INTPTR :
      case NETWIB_PRIV_FMT_FIELDTYPE_BOOL :
      case NETWIB_PRIV_FMT_FIELDTYPE_CMP :
      case NETWIB_PRIV_FMT_FIELDTYPE_ETH :
      case NETWIB_PRIV_FMT_FIELDTYPE_IP :
      case NETWIB_PRIV_FMT_FIELDTYPE_PORT :
      case NETWIB_PRIV_FMT_FIELDTYPE_ETHS :
      case NETWIB_PRIV_FMT_FIELDTYPE_IPS :
      case NETWIB_PRIV_FMT_FIELDTYPE_PORTS :
        netwib_er(netwib_buf_append_byte('(', pbuf));
        break;
      default :
        break;
    }
  }

  switch(pinfos->fieldtype) {
    case NETWIB_PRIV_FMT_FIELDTYPE_PERCENT :
      netwib_er(netwib_buf_append_string("[%]", pbuf));
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_END :
      netwib_er(netwib_buf_append_byte('$', pbuf));
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_C :
      netwib_er(netwib_buf_append_byte('.', pbuf));
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_S :
      if (pinfos->specset && pinfos->spec.specs.useglob) {
        if (skipfield) {
          netwib_er(netwib_buf_append_string(".+", pbuf));
        } else {
          netwib_er(netwib_buf_append_string("(.+)", pbuf));
        }
      } else {
        if (skipfield) {
          netwib_er(netwib_buf_append_string("[ \t]*[^ \t]+[ \t]*", pbuf));
        } else {
          netwib_er(netwib_buf_append_string("[ \t]*([^ \t]+)[ \t]*", pbuf));
        }
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_BUF :
      if (pinfos->specset && pinfos->spec.specbuf.useglob) {
        if (skipfield) {
          netwib_er(netwib_buf_append_string(".+", pbuf));
        } else {
          netwib_er(netwib_buf_append_string("(.+)", pbuf));
        }
      } else {
        if (skipfield) {
          netwib_er(netwib_buf_append_string("[ \t]*[^ \t]+[ \t]*", pbuf));
        } else {
          netwib_er(netwib_buf_append_string("[ \t]*([^ \t]+)[ \t]*", pbuf));
        }
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_P :
      netwib_er(netwib_buf_append_string("[0-9a-fA-Fx]+", pbuf));
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_UINT32 :
    case NETWIB_PRIV_FMT_FIELDTYPE_UINT16 :
    case NETWIB_PRIV_FMT_FIELDTYPE_UINT8 :
    case NETWIB_PRIV_FMT_FIELDTYPE_UINT64 :
    case NETWIB_PRIV_FMT_FIELDTYPE_BYTE :
    case NETWIB_PRIV_FMT_FIELDTYPE_UINTMAX :
    case NETWIB_PRIV_FMT_FIELDTYPE_UINTPTR :
      if (pinfos->specset) {
        if (!skipfield) {
          netwib_er(netwib_buf_append_byte('(', pbuf));
        }
        switch(pinfos->spec.specuint.base) {
          case 2 :
            netwib_er(netwib_buf_append_string("b?[0-1]+", pbuf));
            break;
          case 8 :
            netwib_er(netwib_buf_append_string("[0-7+]+", pbuf));
            break;
          case 10 :
            netwib_er(netwib_buf_append_string("[0-9+]+", pbuf));
            break;
          case 16 :
          case 17 :
            netwib_er(netwib_buf_append_string("[0-9a-fA-Fx+]+", pbuf));
            break;
        }
        if (!skipfield) {
          netwib_er(netwib_buf_append_byte(')', pbuf));
        }
      } else {
        if (skipfield) {
          netwib_er(netwib_buf_append_string("[0-9a-fA-Fx+]+", pbuf));
        } else {
          netwib_er(netwib_buf_append_string("([0-9a-fA-Fx+]+)", pbuf));
        }
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_INT32 :
    case NETWIB_PRIV_FMT_FIELDTYPE_INT16 :
    case NETWIB_PRIV_FMT_FIELDTYPE_INT8 :
    case NETWIB_PRIV_FMT_FIELDTYPE_INT64 :
    case NETWIB_PRIV_FMT_FIELDTYPE_INTMAX :
    case NETWIB_PRIV_FMT_FIELDTYPE_INTPTR :
      netwib_er(netwib_buf_append_string("[0-9+-]+", pbuf));
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_BOOL :
      if (pinfos->specset) {
        switch (pinfos->spec.specbool.type) {
          case '0' :
            netwib_er(netwib_buf_append_string("[01]", pbuf));
            break;
          case 't' :
            netwib_er(netwib_buf_append_string("[truefals]+", pbuf));
            break;
          case 'T' :
            netwib_er(netwib_buf_append_string("[TRUEFALS]+", pbuf));
            break;
          case 'y' :
            netwib_er(netwib_buf_append_string("[yesno]+", pbuf));
            break;
          case 'Y' :
            netwib_er(netwib_buf_append_string("[YESNO]+", pbuf));
            break;
          case 's' :
            netwib_er(netwib_buf_append_string("[unset]+", pbuf));
            break;
          case 'S' :
            netwib_er(netwib_buf_append_string("[UNSET]+", pbuf));
            break;
        }
      } else {
        netwib_er(netwib_buf_append_string("[01enorstuyENORSTUY]+", pbuf));
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_CMP :
      if (pinfos->specset) {
        switch (pinfos->spec.speccmp.type) {
          case '=' :
            netwib_er(netwib_buf_append_string("[<=>]", pbuf));
            break;
          case '0' :
            netwib_er(netwib_buf_append_string("[+0-]", pbuf));
            break;
          case 'e' :
            netwib_er(netwib_buf_append_string("[egl][qt]", pbuf));
            break;
        }
      } else {
        netwib_er(netwib_buf_append_string("[<=>eglqt+0-]+", pbuf));
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_ETH :
      netwib_er(netwib_buf_append_string("[0-9a-fA-F][0-9a-fA-F]?:[0-9a-fA-F][0-9a-fA-F]?:[0-9a-fA-F][0-9a-fA-F]?:[0-9a-fA-F][0-9a-fA-F]?:[0-9a-fA-F][0-9a-fA-F]?:[0-9a-fA-F][0-9a-fA-F]?", pbuf));
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_IP :
      netwib_er(netwib_buf_append_string("[0-9a-fA-F.:]+", pbuf));
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_PORT :
      netwib_er(netwib_buf_append_string("[0-9]+", pbuf));
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_ETHS :
      netwib_er(netwib_buf_append_string("[0-9a-fA-F:,-]+", pbuf));
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_IPS :
      netwib_er(netwib_buf_append_string("[0-9.:,-]+", pbuf));
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_PORTS :
      netwib_er(netwib_buf_append_string("[0-9,-]+", pbuf));
      break;
  }

  if (!skipfield) {
    switch(pinfos->fieldtype) {
      case NETWIB_PRIV_FMT_FIELDTYPE_C :
      case NETWIB_PRIV_FMT_FIELDTYPE_P :
      case NETWIB_PRIV_FMT_FIELDTYPE_INT32 :
      case NETWIB_PRIV_FMT_FIELDTYPE_INT16 :
      case NETWIB_PRIV_FMT_FIELDTYPE_INT8 :
      case NETWIB_PRIV_FMT_FIELDTYPE_INT64 :
      case NETWIB_PRIV_FMT_FIELDTYPE_INTMAX :
      case NETWIB_PRIV_FMT_FIELDTYPE_INTPTR :
      case NETWIB_PRIV_FMT_FIELDTYPE_BOOL :
      case NETWIB_PRIV_FMT_FIELDTYPE_CMP :
      case NETWIB_PRIV_FMT_FIELDTYPE_ETH :
      case NETWIB_PRIV_FMT_FIELDTYPE_IP :
      case NETWIB_PRIV_FMT_FIELDTYPE_PORT :
      case NETWIB_PRIV_FMT_FIELDTYPE_ETHS :
      case NETWIB_PRIV_FMT_FIELDTYPE_IPS :
      case NETWIB_PRIV_FMT_FIELDTYPE_PORTS :
        netwib_er(netwib_buf_append_byte(')', pbuf));
        break;
      default :
        break;
    }
  }

  if (pinfos->geneset) {
    if (pinfos->gene.minsize) {
      if (pinfos->gene.align != NETWIB_PRIV_FMT_ALIGNTYPE_RIGHT) {
        netwib_byte array[4];
        array[0] = '[';
        array[1] = pinfos->gene.padchar;
        array[2] = ']';
        array[3] = '*';
        netwib_er(netwib_buf_append_data(array, 4, pbuf));
      }
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_fmt_decode_uint32(netwib_constbuf *pbuf,
                                                netwib_uint32 base,
                                                netwib_uint32 *pnumber)
{
  netwib_string str;
  netwib_uintmax number;
  char *p;

  /* obtain a reference to the string */
  netwib__constbuf_ref_string(pbuf, str, bufstorage,
                              netwib_priv_fmt_decode_uint32(&bufstorage, base, pnumber));

  /* special case */
  if (*str == '\0') {
    return(NETWIB_ERR_PAINT);
  }

  /* base 2 starting with 'b' is not supported by strtoul */
  if (base == 2 && *str == 'b') {
    str++;
  }

  /* find number */
  number = strtoul(str, &p, base);
  if (*p != '\0') {
    return(NETWIB_ERR_PAINT);
  }

  /* number is too big */
  if (number == ULONG_MAX && errno == ERANGE) {
    errno = 0;
    return(NETWIB_ERR_PATOOHIGH);
  }
#if NETWIB_INTMAX_BITS == 64
  if (number > 0xFFFFFFFFu) {
    return(NETWIB_ERR_PATOOHIGH);
  }
#endif

  if (pnumber != NULL) *pnumber = (netwib_uint32)number;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_fmt_decode_int32(netwib_constbuf *pbuf,
                                               netwib_int32 *pnumber)
{
  netwib_string str;
  netwib_intmax number;
  char *p;

  /* obtain a reference to the string */
  netwib__constbuf_ref_string(pbuf, str, bufstorage,
                              netwib_priv_fmt_decode_int32(&bufstorage, pnumber));

  /* special case */
  if (*str == '\0') {
    return(NETWIB_ERR_PAINT);
  }

  /* find number */
  number = strtol(str, &p, 10);
  if (*p != '\0') {
    return(NETWIB_ERR_PAINT);
  }

  /* number is too big */
  if (errno == ERANGE) {
    errno = 0;
    if (number == LONG_MAX)
      return(NETWIB_ERR_PATOOHIGH);
    else if (number == LONG_MIN)
      return(NETWIB_ERR_PATOOLOW);
  }
#if NETWIB_INTMAX_BITS == 64
  if (number > 0 && number > 0x7FFFFFFF) {
    return(NETWIB_ERR_PATOOHIGH);
  }
  if (number < 0 && number < -0x7FFFFFFF-1) {
    return(NETWIB_ERR_PATOOLOW);
  }
#endif

  if (pnumber != NULL) *pnumber = (netwib_int32)number;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_fmt_decode_uint64(netwib_constbuf *pbuf,
                                                netwib_uint32 base,
                                                netwib_uint64 *pnumber)
{
  netwib_uint64 base64, number, number2, c64;
  netwib_uint32 c32;
  netwib_string pc;
  netwib_char c;

  /* obtain a reference to the string */
  netwib__constbuf_ref_string(pbuf, pc, bufstorage,
                              netwib_priv_fmt_decode_uint64(&bufstorage, base, pnumber));

  /* skip useless characters*/
  if (*pc == '+') {
    pc++;
  }
  switch(base) {
  case 2 :
    if (*pc == 'b') {
      pc++;
    }
    break;
  case 16 :
    if (*pc == '0' && *(pc+1) == 'x') {
      pc += 2;
    }
    break;
  case 0 : /* need to guess ; by default 10 (note: no way to detect binary) */
    base = 10;
    if (*pc == '0') {
      if (*(pc+1) == 'x') {
        pc += 2;
        base = 16;
      } else {
        /* do not increment pc to leave the '0' */
        base = 8;
      }
    }
    break;
  }
  if (*pc == '\0') {
    return(NETWIB_ERR_PAINT);
  }

  /* loop and store in number */
  netwib__uint64_init_uint32(0, number);
  netwib__uint64_init_uint32(base, base64);
  while(NETWIB_TRUE) {
    c = *pc++;
    if (c == '\0') {
      break;
    }
    netwib_c2_cto16_if(c, c32) else {
      return(NETWIB_ERR_PAINT);
    }
    if (c32 >= base) {
      return(NETWIB_ERR_PAINT);
    }
    netwib__uint64_init_uint32(c32, c64);
    netwib__uint64_mul(number, base64, &number2);
    netwib__uint64_add(number2, c64, number2);
    if (netwib__uint64_cmp_lt(number2, number)) {
      /* an overflow occurred */
      return(NETWIB_ERR_PAINT);
    }
    number = number2;
  }

  if (pnumber != NULL) *pnumber = number;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_fmt_decode_int64(netwib_constbuf *pbuf,
                                               netwib_int64 *pnumber)
{
  netwib_buf buf;
  netwib_data data;
  netwib_uint32 datasize;
  netwib_uint64 ui64, maxint64;
  netwib_bool ispos;

  datasize = netwib__buf_ref_data_size(pbuf);
  if (datasize == 0) {
    return(NETWIB_ERR_PAINT);
  }
  data = netwib__buf_ref_data_ptr(pbuf);

  /* check if a sign is present */
  ispos = NETWIB_TRUE;
  switch(*data) {
  case '-' :
    ispos = NETWIB_FALSE;
    data++;
    datasize--;
    break;
  case '+' :
    data++;
    datasize--;
    break;
  }

  /* decode an unsigned */
  netwib_er(netwib_buf_init_ext_arrayfilled(data, datasize, &buf));
  netwib_er(netwib_priv_fmt_decode_uint64(&buf, 10, &ui64));
  netwib__uint64_init_32(0x80000000u, 0, maxint64);
  if (ispos) {
    if (netwib__uint64_cmp_gt(ui64, maxint64)) {
      return(NETWIB_ERR_PATOOHIGH);
    }
    *pnumber = ui64;
  } else {
    if (netwib__uint64_cmp_gt(ui64, maxint64)) {
      return(NETWIB_ERR_PATOOLOW);
    }
    netwib__int64_neg(ui64, *pnumber);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_fmt_decode_bool(netwib_constbuf *pbuf,
                                              netwib_bool *pb)
{
  netwib_data data;
  netwib_uint32 datasize;
  netwib_char firstchar;
  netwib_bool b;

  datasize = netwib__buf_ref_data_size(pbuf);
  if (datasize == 0) {
    return(NETWIB_ERR_PAINT);
  }
  data = netwib__buf_ref_data_ptr(pbuf);

#define netwib__priv_fmt_decode_bool_cmp(txt,val) if (!netwib_c_memcasecmp(data, (netwib_constdata)txt, datasize)) { b = val; break; }
  firstchar = data[0];
  switch(firstchar) {
  case '0' :
    if (datasize == 1) {
      b = NETWIB_FALSE;
      break;
    }
    return(NETWIB_ERR_PAINT);
    break;
  case '1' :
    if (datasize == 1) {
      b = NETWIB_TRUE;
      break;
    }
    return(NETWIB_ERR_PAINT);
    break;
  case 'f' :
  case 'F' :
    if (datasize == 5) {
      netwib__priv_fmt_decode_bool_cmp("false", NETWIB_FALSE);
    }
    return(NETWIB_ERR_PAINT);
    break;
  case 'n' :
  case 'N' :
    if (datasize == 2) {
      netwib__priv_fmt_decode_bool_cmp("no", NETWIB_FALSE);
    }
    return(NETWIB_ERR_PAINT);
    break;
  case 's' :
  case 'S' :
    if (datasize == 3) {
      netwib__priv_fmt_decode_bool_cmp("set", NETWIB_TRUE);
    }
    return(NETWIB_ERR_PAINT);
    break;
  case 't' :
  case 'T' :
    if (datasize == 4) {
      netwib__priv_fmt_decode_bool_cmp("true", NETWIB_TRUE);
    }
    return(NETWIB_ERR_PAINT);
    break;
  case 'u' :
  case 'U' :
    if (datasize == 5) {
      netwib__priv_fmt_decode_bool_cmp("unset", NETWIB_FALSE);
    }
    return(NETWIB_ERR_PAINT);
    break;
  case 'y' :
  case 'Y' :
    if (datasize == 3) {
      netwib__priv_fmt_decode_bool_cmp("yes", NETWIB_TRUE);
    }
    return(NETWIB_ERR_PAINT);
    break;
  default :
    return(NETWIB_ERR_PAINT);
  }

  if (pb != NULL) *pb = b;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_fmt_decode_cmp(netwib_constbuf *pbuf,
                                             netwib_cmp *pcmp)
{
  netwib_data data;
  netwib_uint32 datasize;
  netwib_char firstchar;
  netwib_cmp cmp;

  datasize = netwib__buf_ref_data_size(pbuf);
  if (datasize == 0) {
    return(NETWIB_ERR_PAINT);
  }
  data = netwib__buf_ref_data_ptr(pbuf);

  firstchar = data[0];
  switch(firstchar) {
  case '<' :
    if (datasize == 1) {
      cmp = NETWIB_CMP_LT;
      break;
    }
    return(NETWIB_ERR_PAINT);
    break;
  case '=' :
    if (datasize == 1) {
      cmp = NETWIB_CMP_EQ;
      break;
    }
    return(NETWIB_ERR_PAINT);
    break;
  case '>' :
    if (datasize == 1) {
      cmp = NETWIB_CMP_GT;
      break;
    }
    return(NETWIB_ERR_PAINT);
    break;
  case '-' :
    if (datasize == 1) {
      cmp = NETWIB_CMP_LT;
      break;
    }
    return(NETWIB_ERR_PAINT);
    break;
  case '0' :
    if (datasize == 1) {
      cmp = NETWIB_CMP_EQ;
      break;
    }
    return(NETWIB_ERR_PAINT);
    break;
  case '+' :
    if (datasize == 1) {
      cmp = NETWIB_CMP_GT;
      break;
    }
    return(NETWIB_ERR_PAINT);
    break;
  case 'e' :
    if (datasize == 2 && data[1] == 'q') {
      cmp = NETWIB_CMP_EQ;
      break;
    }
    return(NETWIB_ERR_PAINT);
    break;
  case 'g' :
    if (datasize == 2 && data[1] == 't') {
      cmp = NETWIB_CMP_GT;
      break;
    }
    return(NETWIB_ERR_PAINT);
    break;
  case 'l' :
    if (datasize == 2 && data[1] == 't') {
      cmp = NETWIB_CMP_LT;
      break;
    }
    return(NETWIB_ERR_PAINT);
    break;
  default :
    return(NETWIB_ERR_PAINT);
  }

  if (pcmp != NULL) *pcmp = cmp;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_fmt_decode(netwib_constbuf *pbuf,
                                         const netwib_priv_fmt_scanitem *pscanitem)
{
  netwib_uint64 ui64;
  netwib_uint32 ui32=0;
  netwib_int64 i64;
  netwib_int32 i32=0;
  netwib_uintptr uiptr=0;
  netwib_err ret;

  if (pscanitem->ptr == NULL) {
    return(NETWIB_ERR_OK);
  }

  switch(pscanitem->fmtinfos.fieldtype) {
    case NETWIB_PRIV_FMT_FIELDTYPE_PERCENT :
      return(NETWIB_ERR_LOINTERNALERROR);
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_C :
      {
        netwib_char *pc = (netwib_char *)pscanitem->ptr;
        *pc = *netwib__buf_ref_data_ptr(pbuf);
      }
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_S :
      {
        netwib_char *pc = (netwib_char *)pscanitem->ptr;
        netwib_c_memcpy(pc, netwib__buf_ref_data_ptr(pbuf),
                        netwib__buf_ref_data_size(pbuf));
        pc[netwib__buf_ref_data_size(pbuf)] = '\0';
      }
      return(NETWIB_ERR_OK);
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_BUF :
      {
        netwib_buf *p = (netwib_buf *)pscanitem->ptr;
        netwib_er(netwib_buf_append_buf(pbuf, p));
      }
      return(NETWIB_ERR_OK);
      break;
    default :
      break;
  }

  /* initialize for integer conversions */
  ret = NETWIB_ERR_OK;
  netwib__uint64_init_uint32(0, ui64);
  netwib__int64_init_int32(0, i64);
  switch(pscanitem->fmtinfos.fieldtype) {
    case NETWIB_PRIV_FMT_FIELDTYPE_UINT32 :
    case NETWIB_PRIV_FMT_FIELDTYPE_UINT16 :
    case NETWIB_PRIV_FMT_FIELDTYPE_UINT8 :
    case NETWIB_PRIV_FMT_FIELDTYPE_BYTE :
      {
        netwib_uint32 base;
        base = 0;
        if (pscanitem->fmtinfos.specset) {
          base = pscanitem->fmtinfos.spec.specuint.base;
          if (base == 17) base = 16;
        }
        ret = netwib_priv_fmt_decode_uint32(pbuf, base, &ui32);
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_UINT64 :
    case NETWIB_PRIV_FMT_FIELDTYPE_UINTMAX :
    case NETWIB_PRIV_FMT_FIELDTYPE_UINTPTR :
      {
        netwib_uint32 base;
        base = 0;
        if (pscanitem->fmtinfos.specset) {
          base = pscanitem->fmtinfos.spec.specuint.base;
          if (base == 17) base = 16;
        }
        ret = netwib_priv_fmt_decode_uint64(pbuf, base, &ui64);
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_P :
#if NETWIB_INTPTR_BITS == 64
      ret = netwib_priv_fmt_decode_uint64(pbuf, 16, &ui64);
      if (ret == NETWIB_ERR_OK) {
        netwib__uintptr_init_uint64(ui64, uiptr);
      }
#else
      ret = netwib_priv_fmt_decode_uint32(pbuf, 16,
                                          (netwib_uint32*)&uiptr);
#endif
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_INT32 :
    case NETWIB_PRIV_FMT_FIELDTYPE_INT16 :
    case NETWIB_PRIV_FMT_FIELDTYPE_INT8 :
      ret = netwib_priv_fmt_decode_int32(pbuf, &i32);
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_INT64 :
    case NETWIB_PRIV_FMT_FIELDTYPE_INTMAX :
    case NETWIB_PRIV_FMT_FIELDTYPE_INTPTR :
      ret = netwib_priv_fmt_decode_int64(pbuf, &i64);
      break;
    default :
      /* ui does not need to be initialized */
      break;
  }

  if (ret == NETWIB_ERR_OK) {
    switch(pscanitem->fmtinfos.fieldtype) {
    case NETWIB_PRIV_FMT_FIELDTYPE_P :
      {
        netwib_ptr *p = (netwib_ptr *)pscanitem->ptr;
        *p = (netwib_ptr)uiptr;
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_UINT32 :
      {
        netwib_uint32 *p = (netwib_uint32 *)pscanitem->ptr;
        *p = ui32;
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_UINT16 :
      {
        netwib_uint16 *p = (netwib_uint16 *)pscanitem->ptr;
        if (ui32 > 0xFFFF) {
          ret = NETWIB_ERR_PATOOHIGH;
        } else {
          *p = (netwib_uint16)ui32;
        }
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_UINT8 :
      {
        netwib_uint8 *p = (netwib_uint8 *)pscanitem->ptr;
        if (ui32 > 0xFF) {
          ret = NETWIB_ERR_PATOOHIGH;
        } else {
          *p = (netwib_uint8)ui32;
        }
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_BYTE :
      {
        netwib_byte *p = (netwib_byte *)pscanitem->ptr;
        if (ui32 > 0xFF) {
          ret = NETWIB_ERR_PATOOHIGH;
        } else {
          *p = (netwib_byte)ui32;
        }
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_UINT64 :
      {
        netwib_uint64 *p = (netwib_uint64 *)pscanitem->ptr;
        *p = ui64;
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_UINTMAX :
      netwib__uintmax_init_uint64(ui64, *(netwib_uintmax*)pscanitem->ptr);
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_UINTPTR :
      netwib__uintptr_init_uint64(ui64, *(netwib_uintptr*)pscanitem->ptr);
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_INT32 :
      {
        netwib_int32 *p = (netwib_int32 *)pscanitem->ptr;
        *p = i32;
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_INT16 :
      {
        netwib_int16 *p = (netwib_int16 *)pscanitem->ptr;
        if (i32 > 0 && i32 > 0xFFFF) {
          ret = NETWIB_ERR_PATOOHIGH;
        } else if (i32 < 0 && i32 < -0x8000) {
          ret = NETWIB_ERR_PATOOLOW;
        } else {
          *p = (netwib_int16)i32;
        }
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_INT8 :
      {
        netwib_int8 *p = (netwib_int8 *)pscanitem->ptr;
        if (i32 > 0 && i32 > 0xFF) {
          ret = NETWIB_ERR_PATOOHIGH;
        } else if (i32 < 0 && i32 < -0x80) {
          ret = NETWIB_ERR_PATOOLOW;
        } else {
          *p = (netwib_int8)i32;
        }
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_INT64 :
      {
        netwib_int64 *p = (netwib_int64 *)pscanitem->ptr;
        *p = i64;
      }
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_INTMAX :
      netwib__intmax_init_int64(i64, *(netwib_intmax*)pscanitem->ptr);
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_INTPTR :
      netwib__intptr_init_int64(ui64, *(netwib_intptr*)pscanitem->ptr);
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_BOOL :
      ret = netwib_priv_fmt_decode_bool(pbuf,
                                        (netwib_bool*)pscanitem->ptr);
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_CMP :
      ret = netwib_priv_fmt_decode_cmp(pbuf, (netwib_cmp*)pscanitem->ptr);
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_ETH :
      ret = netwib_eth_init_buf(pbuf, (netwib_eth*)pscanitem->ptr);
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_IP :
      ret = netwib_ip_init_buf(pbuf, NETWIB_IP_DECODETYPE_IP,
                               (netwib_ip*)pscanitem->ptr);
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_PORT :
      ret = netwib_port_init_buf(pbuf, (netwib_port*)pscanitem->ptr);
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_ETHS :
      ret = netwib_eths_add_buf((netwib_eths*)pscanitem->ptr, pbuf);
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_IPS :
      ret = netwib_ips_add_buf((netwib_ips*)pscanitem->ptr, pbuf);
      break;
    case NETWIB_PRIV_FMT_FIELDTYPE_PORTS :
      ret = netwib_ports_add_buf((netwib_ports*)pscanitem->ptr, pbuf);
      break;
    default :
      return(NETWIB_ERR_LOINTERNALERROR);
      break;
    }
  }

  return(ret);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_buf_decode_vfmt(netwib_constbuf *pbuf,
                                              netwib_conststring fmt,
                                              va_list *pap)
{
  netwib_priv_fmt_scanitem scanitem[NETWIB_REGEXP_MAXLEN];
  netwib_conststring pfmt;
  netwib_priv_fmtinfos infos;
  netwib_regexp arrayfound;
  netwib_buf regexpbuf;
  netwib_uint32 numitems, i;
  netwib_char c;
  netwib_err ret;

  netwib_er(netwib_buf_init_mallocdefault(&regexpbuf));
  netwib_er(netwib_buf_append_byte('^', &regexpbuf));

  /* analyze fmt, set regexp and array of addresses */
  pfmt = fmt;
  ret = NETWIB_ERR_OK;
  numitems = 0;
  while (NETWIB_TRUE) {
    c = *pfmt++;
    if (c == '\0') {
      break;
    } else if (c == '%') {
      ret = netwib_priv_fmt_analyze(pfmt, &infos);
      if (ret == NETWIB_ERR_OK) {
        pfmt += infos.skipsize;
        if (infos.fieldtype != NETWIB_PRIV_FMT_FIELDTYPE_PERCENT &&
            infos.fieldtype != NETWIB_PRIV_FMT_FIELDTYPE_END) {
          if (!infos.geneset || !infos.gene.skipfield) {
            if (numitems >= NETWIB_REGEXP_MAXLEN) {
              netwib_er(netwib_priv_buf_append_vfmt_error("There are too many items"));
              ret = NETWIB_ERR_PATOOHIGH;
              break;
            }
            scanitem[numitems].fmtinfos = infos;
            scanitem[numitems].ptr = va_arg(*pap, void*);
            numitems++;
          }
        }
        ret = netwib_priv_fmt_regexp(&regexpbuf, &infos);
        if (infos.fieldtype == NETWIB_PRIV_FMT_FIELDTYPE_END) {
          if (*pfmt != '\0') {
            netwib_er(netwib_priv_buf_append_vfmt_error("%$ must be the last one"));
            ret = NETWIB_ERR_PAFMT;
            break;
          }
        }
      }
    } else if (netwib_c2_isalnum(c)) {
      ret = netwib_buf_append_byte(c, &regexpbuf);
    } else if (c == ' ') {
      ret = netwib_buf_append_string("[ \t]+", &regexpbuf);
    } else {
      netwib_byte array[4];
      array[0] = '[';
      array[1] = c;
      array[2] = ']';
      ret = netwib_buf_append_data(array, 3, &regexpbuf);
    }
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_buf_close(&regexpbuf));
    return(ret);
  }

  /* search regexp */
  ret = netwib_buf_search_regexp(pbuf, &regexpbuf, NETWIB_TRUE, &arrayfound);
  if (ret == NETWIB_ERR_NOTFOUND) {
    ret = NETWIB_ERR_NOTCONVERTED;
  } else if (ret == NETWIB_ERR_OK) {
    for (i = 0; i < numitems; i++) {
      ret = netwib_priv_fmt_decode(&arrayfound.array[i+1], &scanitem[i]);
      if (ret != NETWIB_ERR_OK) {
        if (ret == NETWIB_ERR_PAINT) ret = NETWIB_ERR_NOTCONVERTED;
        if (ret == NETWIB_ERR_PATOOHIGH) ret = NETWIB_ERR_NOTCONVERTED;
        if (ret == NETWIB_ERR_PATOOLOW) ret = NETWIB_ERR_NOTCONVERTED;
        break;
      }
    }
  }

  netwib_er(netwib_buf_close(&regexpbuf));

  /* leave */
  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_decode_fmt(netwib_constbuf *pbuf,
                                 netwib_conststring fmt,
                                 ...)
{
  va_list ap;
  netwib_err ret;

  va_start(ap, fmt);
  ret = netwib_priv_buf_decode_vfmt(pbuf, fmt, &ap);
  va_end(ap);
  return(ret);
}
