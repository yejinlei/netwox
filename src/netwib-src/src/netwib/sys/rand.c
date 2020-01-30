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
netwib_err netwib_uint32_init_rand(netwib_uint32 min,
                                   netwib_uint32 max,
                                   netwib_uint32 *prand)
{
  if (min > max) {
    return(NETWIB_ERR_PAINVALIDRANGE);
  }

  if (prand != NULL) {
    netwib_er(netwib_priv_rand_gene(prand, NULL));
    if (min != 0 || max != 0xFFFFFFFFU) {
      *prand = min + *prand % (max - min + 1);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_uint64_init_rand(netwib_uint64 min,
                                   netwib_uint64 max,
                                   netwib_uint64 *prand)
{
  netwib_uint32 r, r2;
  netwib_uint64 zero, ff, tmp, q;

  if (netwib__uint64_cmp_gt(min, max)) {
    return(NETWIB_ERR_PAINVALIDRANGE);
  }

  if (prand != NULL) {
    /* generate */
    netwib_er(netwib_priv_rand_gene(&r, NULL));
    netwib_er(netwib_priv_rand_gene(&r2, NULL));
    netwib__uint64_init_32(r, r2, *prand);
    /* round */
    netwib__uint64_init_32(0, 0, zero);
    netwib__uint64_init_32(0xFFFFFFFFU, 0xFFFFFFFFU, ff);
    if (netwib__uint64_cmp_ne(min, zero) || netwib__uint64_cmp_ne(min, ff)) {
      netwib__uint64_sub(max, min, tmp);
      netwib__uint64_inc(tmp);
      netwib__uint64_div(*prand, tmp, &q, &tmp);
      netwib__uint64_add(min, tmp, *prand);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_buf_append_rand(netwib_uint32 size,
                                  netwib_byte min,
                                  netwib_byte max,
                                  netwib_buf *pbuf)
{
  netwib_data data;
  netwib_uint32 s, range;
  netwib_byte b, randarray[NETWIB_PRIV_RAND_ARRAY_LEN];

  if (min > max) {
    return(NETWIB_ERR_PAINVALIDRANGE);
  }

  netwib_er(netwib_buf_wantspace(pbuf, size, &data));

  /* chunks of 6/8 */
  s = size; /* we need to keep size for endoffset */
  while (s > NETWIB_PRIV_RAND_ARRAY_LEN - 1) {
    netwib_er(netwib_priv_rand_gene(NULL, data));
    data += NETWIB_PRIV_RAND_ARRAY_LEN;
    s -= NETWIB_PRIV_RAND_ARRAY_LEN;
  }

  /* last bytes */
  if (s) {
    netwib_er(netwib_priv_rand_gene(NULL, randarray));
    netwib_c_memcpy(data, randarray, s);
    data += s;
  }

  /* eventually recompute bytes if a smaller range is expected */
  if (min != 0 || max != 255) {
    range = max - min + 1;
    s = size;
    data -= size;
    while (s--) {
      b = *data;
      *data++ = (netwib_byte)(min + ((b*range) / 256));
    }
  }

  pbuf->endoffset += size;

  /* suppose this buffer is sensitive */
  pbuf->flags |= NETWIB_BUF_FLAGS_SENSITIVE;

  return(NETWIB_ERR_OK);
}
