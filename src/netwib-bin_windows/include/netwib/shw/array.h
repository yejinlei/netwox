
/*-------------------------------------------------------------*/
/***************************************************************
 * An array permits to show a packet. Generally, one line      *
 * corresponds to 32bits/4bytes, as seen in RFCs               *
 ***************************************************************/
/*
For example :
._______________________________________________________________.
|0|1|1|0|0|1|1|0|0|1|1|0|0|1|1|0|0|1|1|0|0|1|1|0|0|1|1|0|0|1|1|0|
|_3_|_3_|_3_|_3_|_3_|_3_|_3_|_3_|_3_|_3_|_3_|_3_|_3_|_3_|_3_|_3_|
|0xF=15_|__0xF__|__15___|0xF=15_|__0xF__|__15___|0xF=15_|__0xF__|
|_0x80=128='c'__|_0x80=128='c'__|_0x80=128='c'__|_0x80=128='c'__|
| data: 0123456789012345678901234567890123456789012345678901234 |
|       0123456789012345678901234567890123456789012345678901234 |
|_______________________________________________________________|

Ethernet________________________________________________________.
| 00:00:01:02:03:04->00:00:05:06:97:08 type:0x0800              |
|_______________________________________________________________|
IP______________________________________________________________.
|version|  ihl  |      tos      |            totlen             |
|___4___|___5___|____0x00=0_____|___________0x0054=84___________|
|              id               |r|D|M|       offsetfrag        |
|___________0x0008=8____________|0|0|0|________0x0000=0_________|
|      ttl      |   protocol    |           checksum            |
|___0xFF=255____|____0x01=1_____|____________0x2551_____________|
|                            source                             |
|_________________________192.168.1.1___________________________|
|                          destination                          |
|_________________________192.168.1.2___________________________|
ICMP4_echo reply________________________________________________.
|     type      |     code      |           checksum            |
|____0x00=0_____|____0x00=0_____|_________0xC6A9=50857__________|

*/

/*-------------------------------------------------------------*/
/* "._________________." or "Title_____________." */
netwib_err netwib_show_array_head(netwib_conststring title,
                                  netwib_buf *pbuf);
/* "|_________________|" */
netwib_err netwib_show_array_tail(netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* "|" */
netwib_err netwib_show_array_line_begin(netwib_buf *pbuf);
/* "\n" */
netwib_err netwib_show_array_line_end(netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* centered text surrounded by spaces and ending with "|" */
netwib_err netwib_show_array_text(netwib_uint32 size,
                                  netwib_conststring txt,
                                  netwib_buf *pbuf);
/* num32 is special : line_begin and line_end are internally called */
#define netwib_show_array_text32(text,pbuf) netwib_show_array_text(32,text,pbuf)
#define netwib_show_array_text16(text,pbuf) netwib_show_array_text(16,text,pbuf)
#define netwib_show_array_text8(text,pbuf) netwib_show_array_text(8,text,pbuf)
#define netwib_show_array_text4(text,pbuf) netwib_show_array_text(4,text,pbuf)
#define netwib_show_array_text1(text,pbuf) netwib_show_array_text(1,text,pbuf)

/*-------------------------------------------------------------*/
/* centered number surrounded by "_" and ending with "|" */
typedef enum {
  NETWIB_SHOW_ARRAY_NUMTYPE_BIN = 1,
  NETWIB_SHOW_ARRAY_NUMTYPE_DEC,
  NETWIB_SHOW_ARRAY_NUMTYPE_HEXA,
  NETWIB_SHOW_ARRAY_NUMTYPE_CHAR,
  NETWIB_SHOW_ARRAY_NUMTYPE_DECCHAR,
  NETWIB_SHOW_ARRAY_NUMTYPE_HEXADEC,
  NETWIB_SHOW_ARRAY_NUMTYPE_HEXACHAR,
  NETWIB_SHOW_ARRAY_NUMTYPE_HEXADECCHAR
} netwib_show_array_numtype;
netwib_err netwib_show_array_num(netwib_uint32 size,
                                 netwib_uint32 ui,
                                 netwib_show_array_numtype type,
                                 netwib_buf *pbuf);
/* num32 is special : line_begin and line_end are internally called */
#define netwib_show_array_num32(ui,type,pbuf) netwib_show_array_num(32,ui,type,pbuf)
#define netwib_show_array_num16(ui,type,pbuf) netwib_show_array_num(16,ui,type,pbuf)
#define netwib_show_array_num8(ui,type,pbuf) netwib_show_array_num(8,ui,type,pbuf)
#define netwib_show_array_num4(ui,type,pbuf) netwib_show_array_num(4,ui,type,pbuf)
#define netwib_show_array_num1(ui,pbuf) netwib_show_array_num(1,ui,NETWIB_SHOW_ARRAY_NUMTYPE_DEC,pbuf)

/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_SHOW_ARRAY_ALIGN_LEFT,
  NETWIB_SHOW_ARRAY_ALIGN_CENTER,
  NETWIB_SHOW_ARRAY_ALIGN_RIGHT
} netwib_show_array_align;
/* if size==32, line_begin and line_end are internally called */
netwib_err netwib_show_array_fmt(netwib_uint32 size,
                                 netwib_show_array_align align,
                                 netwib_char fillchar, /* ie ' ' or '_' */
                                 netwib_buf *pbuf,
                                 netwib_conststring fmt,
                                 ...);
/* size==32, align==NETWIB_SHOW_ARRAY_ALIGN_LEFT and fillchar==' ' */
netwib_err netwib_show_array_fmt32(netwib_buf *pbuf,
                                   netwib_conststring fmt,
                                   ...);

/*-------------------------------------------------------------*/
/*
 data wrapped to fit in the array. For example :
    |title: 0123456789012345678901234567890123456789012 |
    |       3456789012345678901234567801234567890123456 |
    |_______567890123456789012345678____________________|
*/
netwib_err netwib_show_array_data(netwib_conststring title,
                                  netwib_constbuf *pdata,
                                  netwib_encodetype encodetype,
                                  netwib_char fillchar, /* ie ' ' or '_' */
                                  netwib_buf *pbuf);
