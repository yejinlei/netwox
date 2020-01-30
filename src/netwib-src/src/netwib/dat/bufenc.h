
/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_ENCODETYPE_DATA = 1,        /* exact data */
  NETWIB_ENCODETYPE_HEXA0,           /* hexadecimal */
  NETWIB_ENCODETYPE_HEXA1,           /* hexadecimal */
  NETWIB_ENCODETYPE_HEXA2,           /* hexadecimal */
  NETWIB_ENCODETYPE_HEXA4,           /* hexadecimal */
  NETWIB_ENCODETYPE_MIXED0,          /* mixed */
  NETWIB_ENCODETYPE_MIXED1,          /* mixed */
  NETWIB_ENCODETYPE_TEXT,            /* printable text */
  NETWIB_ENCODETYPE_BASE64,          /* base64 */
  NETWIB_ENCODETYPE_QUOTED,          /* quoted */
  NETWIB_ENCODETYPE_NOTHING = 100,   /* print nothing */
  NETWIB_ENCODETYPE_SYNTH,           /* print a synthetic form */
  /* wrap to 80 columns (or less) */
  NETWIB_ENCODETYPE_DATA_WRAP = 300, /* data */
  NETWIB_ENCODETYPE_HEXA0_WRAP,      /* hexa (32 bytes per line) */
  NETWIB_ENCODETYPE_HEXA1_WRAP,      /* hexa (16 bytes per line) */
  NETWIB_ENCODETYPE_HEXA2_WRAP,      /* hexa (32 bytes per line) */
  NETWIB_ENCODETYPE_HEXA4_WRAP,      /* hexa (32 bytes per line) */
  NETWIB_ENCODETYPE_MIXED0_WRAP,     /* mixed (16 bytes per line) */
  NETWIB_ENCODETYPE_MIXED1_WRAP,     /* mixed (16 bytes per line) */
  NETWIB_ENCODETYPE_TEXT_WRAP,       /* printable text */
  NETWIB_ENCODETYPE_BASE64_WRAP,     /* base64 */
  NETWIB_ENCODETYPE_ARRAY1 = 400,    /* array (4 bytes per line) */
  NETWIB_ENCODETYPE_ARRAY4,          /* array (4 bytes per line) */
  NETWIB_ENCODETYPE_ARRAY8,          /* array (4 bytes per line) */
  NETWIB_ENCODETYPE_ARRAY16,         /* array (4 bytes per line) */
  NETWIB_ENCODETYPE_ARRAY32,         /* array (4 bytes per line) */
  NETWIB_ENCODETYPE_DUMP,            /* dump (16 bytes per line) */
  NETWIB_ENCODETYPE_MIXED0H_WRAP,    /* mixed (8 bytes per line) */
  NETWIB_ENCODETYPE_MIXED1H_WRAP,    /* mixed (8 bytes per line) */
  NETWIB_ENCODETYPE_LOWERCASE,       /* lower case */
  NETWIB_ENCODETYPE_UPPERCASE,       /* upper case */
  /* aliases */
  NETWIB_ENCODETYPE_HEXA = NETWIB_ENCODETYPE_HEXA1,
  NETWIB_ENCODETYPE_MIXED = NETWIB_ENCODETYPE_MIXED1,
  NETWIB_ENCODETYPE_HEXA_WRAP = NETWIB_ENCODETYPE_HEXA1_WRAP,
  NETWIB_ENCODETYPE_MIXED_WRAP = NETWIB_ENCODETYPE_MIXED1_WRAP,
  NETWIB_ENCODETYPE_ARRAY = NETWIB_ENCODETYPE_ARRAY8,
  NETWIB_ENCODETYPE_MIXEDH_WRAP = NETWIB_ENCODETYPE_MIXED1H_WRAP,
  /* for transition */
  NETWIB_ENCODETYPE_TRANSITION_INIT = 500,
  NETWIB_ENCODETYPE_TRANSITION_END
} netwib_encodetype;

/*-------------------------------------------------------------*/
/* Examples :
NETWIB_ENCODETYPE_HEXA0  : 01020304050607080910...
NETWIB_ENCODETYPE_HEXA1  : 01 02 03 04 05 06 07 08 09 10 ...
NETWIB_ENCODETYPE_HEXA2  : 0102 0304 0506 0708 0910 ...
NETWIB_ENCODETYPE_HEXA4  : 01020304 05060708 0910...
NETWIB_ENCODETYPE_MIXED0 : 'abc' 112233445566
NETWIB_ENCODETYPE_MIXED1 : 'abc' 11 22 33 44 55 66
NETWIB_ENCODETYPE_HEXA0_WRAP :
0102030405060708091011121314151617181920212223242526272829303132
NETWIB_ENCODETYPE_HEXA1_WRAP :
01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16
NETWIB_ENCODETYPE_HEXA2_WRAP :
0102 0304 0506 0708 0910 1112 1314 1516 1718 1920 2122 2324 2526 2728 2930 3132
NETWIB_ENCODETYPE_HEXA4_WRAP :
01020304 05060708 09101112 13141516 17181920 21222324 25262728 29303132
NETWIB_ENCODETYPE_MIXED0_WRAP :
'a' 02 'a' 02 'a' 02 'a' 02 'a' 02 'a' 02 'a' 02 'a' 02
'a' 026102 'a' 02 'a' 02 'a' 02 'a' 02 'a' 02 'a' 02
NETWIB_ENCODETYPE_MIXED1_WRAP :
'a' 02 'a' 02 'a' 02 'a' 02 'a' 02 'a' 02 'a' 02 'a' 02
'a' 02 61 02 'a' 02 'a' 02 'a' 02 'a' 02 'a' 02 'a' 02
NETWIB_ENCODETYPE_ARRAY1 :
|1|0|0|0|0|1|1|0|0|1|0|0|0|1|1|0|1|1|0|0|0|1|1|0|0|0|1|0|0|1|1|0|
NETWIB_ENCODETYPE_ARRAY4 :
|__0x1__|__0x6__|_______________|_______________|_______________|
NETWIB_ENCODETYPE_ARRAY8 :
|____0x80=128___|____0x11=17____|____0x80=128___|____0x11=17____|
NETWIB_ENCODETYPE_ARRAY16 :
|_________0x6162=24930__________|_________0x6364=25444__________|
NETWIB_ENCODETYPE_ARRAY32 :
|_____________________0x61626364=1633837924_____________________|
NETWIB_ENCODETYPE_DUMP :
61 62 63 64  65 66 67 68  69 6a 6b 6c  6d 6e 6f 70  # abcdefghijklmnop
NETWIB_ENCODETYPE_MIXED0H_WRAP :
'a' 62 'c' 64 'e' 66 'f' 68             # 61 62 63 64 65 66 67 68
'a' 626364 'e' 66 'f' 68                # 61 62 63 64 65 66 67 68
NETWIB_ENCODETYPE_MIXED1H_WRAP :
'a' 62 'c' 64 'e' 66 'f' 68             # 61 62 63 64 65 66 67 68
'a' 62 63 64 'e' 66 'f' 68              # 61 62 63 64 65 66 67 68
NETWIB_ENCODETYPE_QUOTED :
"abc d \\ \t \n \r \" \x00 z"
*/

/*-------------------------------------------------------------*/
/* Name : netwib_buf_encode
   Description :
     Append a encoded buffer.
   Input parameter(s) :
     *pbuftoencode : buffer to encode
     encodetype : decoding type
   Input/output parameter(s) :
     *pbuf : netwib_buf updated
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
   This function supports NETWIB_BUF_FLAGS_SENSITIVE.
*/
netwib_err netwib_buf_encode(netwib_constbuf *pbuftoencode,
                             netwib_encodetype encodetype,
                             netwib_buf *pbuf);
/* only the main ones are defined */
#define netwib_buf_encode_data(pbuftoencode,pbuf) netwib_buf_encode(pbuftoencode,NETWIB_ENCODETYPE_DATA,pbuf)
#define netwib_buf_encode_hexa(pbuftoencode,pbuf) netwib_buf_encode(pbuftoencode,NETWIB_ENCODETYPE_HEXA,pbuf)
#define netwib_buf_encode_mixed(pbuftoencode,pbuf) netwib_buf_encode(pbuftoencode,NETWIB_ENCODETYPE_MIXED,pbuf)
#define netwib_buf_encode_text(pbuftoencode,pbuf) netwib_buf_encode(pbuftoencode,NETWIB_ENCODETYPE_TEXT,pbuf)
#define netwib_buf_encode_base64(pbuftoencode,pbuf) netwib_buf_encode(pbuftoencode,NETWIB_ENCODETYPE_BASE64,pbuf)
#define netwib_buf_encode_quoted(pbuftoencode,pbuf) netwib_buf_encode(pbuftoencode,NETWIB_ENCODETYPE_QUOTED,pbuf)
#define netwib_buf_encode_dump(pbuftoencode,pbuf) netwib_buf_encode(pbuftoencode,NETWIB_ENCODETYPE_DUMP,pbuf)
#define netwib_buf_encode_lowercase(pbuftoencode,pbuf) netwib_buf_encode(pbuftoencode,NETWIB_ENCODETYPE_LOWERCASE,pbuf)
#define netwib_buf_encode_uppercase(pbuftoencode,pbuf) netwib_buf_encode(pbuftoencode,NETWIB_ENCODETYPE_UPPERCASE,pbuf)

/*-------------------------------------------------------------*/
typedef struct {
  netwib_encodetype last;
  netwib_bool containnl;
} netwib_encodetype_context;
/* Name : netwib_buf_encode_transition
   Description :
     Append a transition between two data.
     First call has to be done with NETWIB_ENCODETYPE_TRANSITION_INIT.
   Input parameter(s) :
     encodetype : next encoding type
   Input/output parameter(s) :
     *pctx : context
     *pbuf : netwib_buf updated
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_encode_transition(netwib_encodetype_context *pctx,
                                        netwib_encodetype encodetype,
                                        netwib_buf *pbuf);
#define netwib_buf_encode_transition_init(pctx) netwib_buf_encode_transition(pctx,NETWIB_ENCODETYPE_TRANSITION_INIT,NULL)
#define netwib_buf_encode_transition_end(pctx,pbuf) netwib_buf_encode_transition(pctx,NETWIB_ENCODETYPE_TRANSITION_END,pbuf)
