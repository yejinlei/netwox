#include "netwibtest.h"

/*-------------------------------------------------------------*/
static netwib_err test_data_append_decode(void)
{
  netwib_data data, data2;
  netwib_uint32 u32;
  netwib_uint64 u64;
  netwib_buf buf;

  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_buf_wantspace(&buf, 80, &data));
  data2 = data;

  netwib__uint64_init_32(0x12345678, 0x90abcdefu, u64);

  netwib__data_append_uint8(data, 0x12);
  netwib__data_append_uint16(data, 0x3456);
  netwib__data_append_uint32(data, 0x7890abcdu);
  netwib__data_append_uint64(data, u64);
  netwib__data_append_uint16_le(data, 0x3456);
  netwib__data_append_uint32_le(data, 0x7890abcdu);
  netwib__data_append_uint64_le(data, u64);
  netwib__data_append_uint8(data, 0x38);
  buf.endoffset += (netwib_uint32)(data-data2);
  netwib_er(netwibtest_buf_content3(&buf, "12 3456 7890abcd 1234567890abcdef 5634 CDAB9078 EFCDAB9078563412 38"));

  data = data2;
  netwib__data_decode_uint8(data, u32);
  if (u32 != 0x12) return(NETWIBTEST_ERR_1);
  netwib__data_decode_uint16(data, u32);
  if (u32 != 0x3456) return(NETWIBTEST_ERR_1);
  netwib__data_decode_uint32(data, u32);
  if (u32 != 0x7890abcdu) return(NETWIBTEST_ERR_1);
  netwib__data_decode_uint64(data, u64);
  netwib_er(netwibtest_uint64_content(u64, "1234567890ABCDEF"));
  netwib__data_decode_uint16_le(data, u32);
  if (u32 != 0x3456) return(NETWIBTEST_ERR_1);
  netwib__data_decode_uint32_le(data, u32);
  if (u32 != 0x7890abcdu) return(NETWIBTEST_ERR_1);
  netwib__data_decode_uint64_le(data, u64);
  netwib_er(netwibtest_uint64_content(u64, "1234567890ABCDEF"));
  netwib__data_decode_uint8(data, u32);
  if (u32 != 0x38) return(NETWIBTEST_ERR_1);

  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_pkt_priv(void)
{
  puts("###########################################\nTest of pkt/priv");
  netwib_er(test_data_append_decode());

  return(NETWIB_ERR_OK);
}
