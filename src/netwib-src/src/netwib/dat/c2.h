/* non standard functions */

/*-------------------------------------------------------------*/
/* CHARACTER FUNCTIONS */

/*-------------------------------------------------------------*/
/* warning : param is called several times. Use those functions with care.
   For example, they are not suitable for "netwib_c2_isalnum(c++)". */
#define netwib_c2_isalnum(c) ( netwib_c2_isalpha(c) || netwib_c2_isdigit(c) )
#define netwib_c2_isalpha(c) ( netwib_c2_islower(c) || netwib_c2_isupper(c) )
#define netwib_c2_isdigit(c) ( ((c)>='0') && ((c)<='9') )
#define netwib_c2_islower(c) ( ((c)>='a') && ((c)<='z') )
#define netwib_c2_isupper(c) ( ((c)>='A') && ((c)<='Z') )
#define netwib_c2_isxdigit(c) ( netwib_c2_isdigit(c) || ( ((c)>='a') && ((c)<='f') ) || ( ((c)>='A') && ((c)<='F') ) )
#define netwib_c2_isprint(c) ( (c)>=32 && (c)<=126 )
#define netwib_c2_isprint2(c) ( netwib_c2_isprint(c) || (c)=='\t' || (c)=='\r' || (c)=='\n' )
#define netwib_c2_isspace(c) ( (c)==' ' || (c)=='\t' || (c)=='\n' || (c)=='\r' || (c)=='\f' || (c)=='\v')

/*-------------------------------------------------------------*/
/* Warning : first 2 functions change the parameter */
#define netwib_c2_lower(c) if (netwib_c2_isupper(c)) { c += 'a' - 'A'; }
#define netwib_c2_upper(c) if (netwib_c2_islower(c)) { c -= 'a' - 'A'; }
/* Warning : param is called several times */
#define netwib_c2_lc(c) ((netwib_char)(netwib_c2_isupper(c)?((c)+'a'-'A'):(c)))
#define netwib_c2_uc(c) ((netwib_char)(netwib_c2_islower(c)?((c)-'a'+'A'):(c)))

/*-------------------------------------------------------------*/
/* 0->16 to '0'->'F' without error checking */
#define netwib_c2_16toc(x) (char)(((x)<=9)?('0'+(x)):('a'+(x)-10))
#define netwib_c2_16toC(x) (char)(((x)<=9)?('0'+(x)):('A'+(x)-10))
/* '0'->'F' to 0->16 without error checking */
#define netwib_c2_cto16(x) (((x)>='0'&&(x)<='9')?((x)-'0'):(((x)>='a'&&(x)<='f')?(10+(x)-'a'):(10+(x)-'A')))
/* 0->9 to '0'->'9' without error checking */
#define netwib_c2_9toc(x) (char)('0'+(x))
/* '0'->'9' to 0->9 without error checking */
#define netwib_c2_cto9(x) ((x)-'0')

/*-------------------------------------------------------------*/
/* '0'->'F' to 0->16 with error checking */
#define netwib_c2_cto16_if(c,quartet) if ((c) >= '0' && (c) <= '9') { quartet = (c) - '0'; } else if ((c) >= 'a' && (c) <= 'f') { quartet = 10 + (c) - 'a'; } else if ((c) >= 'A' && (c) <= 'F') { quartet = 10 + (c) - 'A'; }
/* '0'->'9' to 0->9 with error checking */
#define netwib_c2_cto9_if(c,digit) if ((c) >= '0' && (c) <= '9') { digit = (c) - '0'; }

/*-------------------------------------------------------------*/
/* INTEGER FUNCTIONS */

/*-------------------------------------------------------------*/
/* byte extract from uint16, uint32 or uint64 */
#define netwib_c2_uint16_0(x) (netwib_byte)(((x)>>8)&0xFF)
#define netwib_c2_uint16_1(x) (netwib_byte)((x)&0xFF)
#define netwib_c2_uint32_0(x) (netwib_byte)(((x)>>24)&0xFF)
#define netwib_c2_uint32_1(x) (netwib_byte)(((x)>>16)&0xFF)
#define netwib_c2_uint32_2(x) (netwib_byte)(((x)>>8)&0xFF)
#define netwib_c2_uint32_3(x) (netwib_byte)((x)&0xFF)
#if NETWIB_INT64_FAKE == 0
 #define netwib_c2_uint64_0(x) (netwib_byte)(((x)>>56)&0xFF)
 #define netwib_c2_uint64_1(x) (netwib_byte)(((x)>>48)&0xFF)
 #define netwib_c2_uint64_2(x) (netwib_byte)(((x)>>40)&0xFF)
 #define netwib_c2_uint64_3(x) (netwib_byte)(((x)>>32)&0xFF)
 #define netwib_c2_uint64_4(x) (netwib_byte)(((x)>>24)&0xFF)
 #define netwib_c2_uint64_5(x) (netwib_byte)(((x)>>16)&0xFF)
 #define netwib_c2_uint64_6(x) (netwib_byte)(((x)>>8)&0xFF)
 #define netwib_c2_uint64_7(x) (netwib_byte)((x)&0xFF)
 #define netwib_c2_uint64_32high(x) (netwib_uint32)((x)>>32)
 #define netwib_c2_uint64_32low(x) (netwib_uint32)((x)&0xFFFFFFFFu)
#else
 #define netwib_c2_uint64_0(x) netwib_c2_uint32_0((x).high)
 #define netwib_c2_uint64_1(x) netwib_c2_uint32_1((x).high)
 #define netwib_c2_uint64_2(x) netwib_c2_uint32_2((x).high)
 #define netwib_c2_uint64_3(x) netwib_c2_uint32_3((x).high)
 #define netwib_c2_uint64_4(x) netwib_c2_uint32_0((x).low)
 #define netwib_c2_uint64_5(x) netwib_c2_uint32_1((x).low)
 #define netwib_c2_uint64_6(x) netwib_c2_uint32_2((x).low)
 #define netwib_c2_uint64_7(x) netwib_c2_uint32_3((x).low)
 #define netwib_c2_uint64_32high(x) ((x).high)
 #define netwib_c2_uint64_32low(x) ((x).low)
#endif

/*-------------------------------------------------------------*/
/* recomposition of uint16 or uint32 */
#define netwib_c2_uint16_2(a,b) (netwib_uint16)((((netwib_byte)(a))<<8)|((netwib_byte)(b)))
#define netwib_c2_uint32_4(a,b,c,d) (netwib_uint32)((((netwib_byte)(a))<<24)|(((netwib_byte)(b))<<16)|(((netwib_byte)(c))<<8)|((netwib_byte)(d)))
#if NETWIB_INT64_FAKE == 0
 #define netwib_c2_uint64_8(a,b,c,d,e,f,g,h) (netwib_uint64)((((netwib_uint64)((netwib_byte)(a)))<<56)|(((netwib_uint64)((netwib_byte)(b)))<<48)|(((netwib_uint64)((netwib_byte)(c)))<<40)|(((netwib_uint64)((netwib_byte)(d)))<<32)|(((netwib_uint64)((netwib_byte)(e)))<<24)|(((netwib_uint64)((netwib_byte)(f)))<<16)|(((netwib_uint64)((netwib_byte)(g)))<<8)|((netwib_uint64)((netwib_byte)(h))))
#endif
/* other 64 bit version defines are in uint64.h */

/*-------------------------------------------------------------*/
/* left and right rotation */
#define netwib_c2_uint8_ror(x,n) ((netwib_uint8)( (netwib_uint8)((x)>>(n)) | (netwib_uint8)((x)<<(8-(n))) ))
#define netwib_c2_uint8_rol(x,n) ((netwib_uint8)( (netwib_uint8)((x)<<(n)) | (netwib_uint8)((x)>>(8-(n))) ))
#define netwib_c2_uint16_ror(x,n) ((netwib_uint16)( (netwib_uint16)((x)>>(n)) | (netwib_uint16)((x)<<(16-(n))) ))
#define netwib_c2_uint16_rol(x,n) ((netwib_uint16)( (netwib_uint16)((x)<<(n)) | (netwib_uint16)((x)>>(16-(n))) ))
#define netwib_c2_uint32_ror(x,n) ((netwib_uint32)( (netwib_uint32)((x)>>(n)) | (netwib_uint32)((x)<<(32-(n))) ))
#define netwib_c2_uint32_rol(x,n) ((netwib_uint32)( (netwib_uint32)((x)<<(n)) | (netwib_uint32)((x)>>(32-(n))) ))
/* 64 bit version defines are in uint64.h */
