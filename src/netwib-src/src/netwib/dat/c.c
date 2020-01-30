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
int netwib_c_strcasecmp(netwib_conststring s1,
                        netwib_conststring s2)
{
  netwib_conststring ps1, ps2;
  char c1, c2;

  ps1 = s1;
  ps2 = s2;

  while (NETWIB_TRUE) {
    c1 = *ps1++;
    netwib_c2_lower(c1);
    c2 = *ps2++;
    if (c2 == '\0')
      return(c1);
    netwib_c2_lower(c2);
    if (c1 != c2)
      return(c1 - c2);
  }

  /* should never be reached */
  return(0);
}

/*-------------------------------------------------------------*/
int netwib_c_strncasecmp(netwib_conststring s1,
                         netwib_conststring s2,
                         netwib_uint32 n)
{
  netwib_conststring ps1, ps2;
  char c1, c2;

  ps1 = s1;
  ps2 = s2;

  while (n--) {
    c1 = *ps1++;
    netwib_c2_lower(c1);
    c2 = *ps2++;
    if (c2 == '\0')
      return(c1);
    netwib_c2_lower(c2);
    if (c1 != c2)
      return(c1 - c2);
  }

  return(0);
}

/*-------------------------------------------------------------*/
int netwib_c_memcasecmp(netwib_constdata s1,
                        netwib_constdata s2,
                        netwib_uint32 n)
{
  netwib_constdata ps1, ps2;
  char c1, c2;

  ps1 = s1;
  ps2 = s2;

  while (n--) {
    c1 = *ps1++;
    netwib_c2_lower(c1);
    c2 = *ps2++;
    netwib_c2_lower(c2);
    if (c1 != c2)
      return(c1 - c2);
  }

  return(0);
}

/*-------------------------------------------------------------*/
/* algorithm Knuth-Morris-Pratt
b a b c b a b c a b c a a b c a b c a b c a c a b c
a b c a b c a c a b
^
  a b c a b c a c a b
        ^
          a b c a b c a c a b
                        ^
                a b c a b c a c a b
                        ^
                        a b c a b c a c a b
                                      ^
                              a b c a b c a c a b
                                                  OK
*/
netwib_string netwib_c_strcasestr(netwib_conststring haystack,
                                  netwib_conststring needle)
{
  char firstcharofneedle, ch, cn;
  netwib_conststring phaystack, pneedle, phaystacksave;

  /* we will search this value in haystack */
  firstcharofneedle = *needle;

  /* special case of empty needle */
  if (firstcharofneedle == '\0') {
    return(netwib_priv_caststring(haystack));
  }
  netwib_c2_lower(firstcharofneedle);

  /* main loop */
  phaystack = haystack;
  while (NETWIB_TRUE) {
    /* search position of firstcharofneedle in haystack */
    while (NETWIB_TRUE) {
      ch = *phaystack++;
      netwib_c2_lower(ch);
      if (ch == firstcharofneedle) {
        /* found it, so phaystack points to the char after */
        break;
      }
      if (ch == '\0') {
        /* not found, so end here */
        return(NULL);
      }
    }
    /* save the position because we need it later */
    phaystacksave = phaystack;
    /* now, continue to search other characters */
    pneedle = needle + 1;
    while (NETWIB_TRUE) {
      cn = *pneedle++;
      if (cn == '\0') {
        /* found it */
        phaystacksave--;
        return(netwib_priv_caststring(phaystacksave));
      }
      netwib_c2_lower(cn);
      ch = *phaystack++;
      netwib_c2_lower(ch);
      if (ch != cn) {
        /* not found, so restart */
        phaystack = phaystacksave;
        break;
      }
    }
  }

  /* should never be reached */
  return(NULL);
}

/*-------------------------------------------------------------*/
netwib_data netwib_c_memmem(netwib_constdata haystack,
                            netwib_uint32 haystacklen,
                            netwib_constdata needle,
                            netwib_uint32 needlelen)
{
  netwib_uint8 firstcharofneedle;
  netwib_constdata phaystack, pneedle, phaystacksave;
  netwib_uint32 seencharsinhaystack, maxlookcharsinhaystack, seencharsinneedle;

  /* special cases */
  if (needlelen == 0) {
    return(netwib_priv_castdata(haystack));
  }
  if (haystacklen < needlelen) {
    return(NULL);
  }

  /* we will search this value in haystack */
  firstcharofneedle = *needle;

  /* main loop */
  phaystack = haystack;
  seencharsinhaystack = 0;
  maxlookcharsinhaystack = haystacklen - needlelen + 1;
  while (NETWIB_TRUE) {
    /* search position of firstcharofneedle in haystack */
    while (NETWIB_TRUE) {
      seencharsinhaystack++;
      if (*phaystack++ == firstcharofneedle) {
        /* found it, so phaystack points to the char after */
        break;
      }
      if (seencharsinhaystack >= maxlookcharsinhaystack) {
        /* not found, so end here */
        return(NULL);
      }
    }
    /* save the position because we need it later */
    phaystacksave = phaystack;
    /* now, continue to search other characters */
    pneedle = needle + 1;
    seencharsinneedle = 1;
    while (NETWIB_TRUE) {
      if (seencharsinneedle == needlelen) {
        /* found it */
        phaystacksave--;
        return(netwib_priv_castdata(phaystacksave));
      }
      seencharsinneedle++;
      if (*phaystack++ != *pneedle++) {
        /* not found, so restart */
        phaystack = phaystacksave;
        break;
      }
    }
  }

  /* should never be reached */
  return(NULL);
}

/*-------------------------------------------------------------*/
netwib_data netwib_c_memcasemem(netwib_constdata haystack,
                                netwib_uint32 haystacklen,
                                netwib_constdata needle,
                                netwib_uint32 needlelen)
{
  netwib_uint8 firstcharofneedle;
  netwib_constdata phaystack, pneedle, phaystacksave;
  netwib_uint32 seencharsinhaystack, maxlookcharsinhaystack, seencharsinneedle;
  netwib_byte c, c2;

  /* special cases */
  if (needlelen == 0) {
    return(netwib_priv_castdata(haystack));
  }
  if (haystacklen < needlelen) {
    return(NULL);
  }

  /* we will search this value in haystack */
  firstcharofneedle = (netwib_uint8)netwib_c2_lc(*needle);

  /* main loop */
  phaystack = haystack;
  seencharsinhaystack = 0;
  maxlookcharsinhaystack = haystacklen - needlelen + 1;
  while (NETWIB_TRUE) {
    /* search position of firstcharofneedle in haystack */
    while (NETWIB_TRUE) {
      seencharsinhaystack++;
      c = *phaystack++;
      if (netwib_c2_lc(c) == firstcharofneedle) {
        /* found it, so phaystack points to the char after */
        break;
      }
      if (seencharsinhaystack >= maxlookcharsinhaystack) {
        /* not found, so end here */
        return(NULL);
      }
    }
    /* save the position because we need it later */
    phaystacksave = phaystack;
    /* now, continue to search other characters */
    pneedle = needle + 1;
    seencharsinneedle = 1;
    while (NETWIB_TRUE) {
      if (seencharsinneedle == needlelen) {
        /* found it */
        phaystacksave--;
        return(netwib_priv_castdata(phaystacksave));
      }
      seencharsinneedle++;
      c = *phaystack++;
      c2 = *pneedle++;
      if (netwib_c2_lc(c) != netwib_c2_lc(c2)) {
        /* not found, so restart */
        phaystack = phaystacksave;
        break;
      }
    }
  }

  /* should never be reached */
  return(NULL);
}

