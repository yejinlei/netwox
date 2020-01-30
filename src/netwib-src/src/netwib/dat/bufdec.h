
/*-------------------------------------------------------------*/
/***************************************************************
 * An hexadecimal string is for example:                       *
 *  010A 0b00Ff                                                *
 ***************************************************************/

/*-------------------------------------------------------------*/
/***************************************************************
 * A mixed string permits to represent data as a clear form    *
 * using hexadecimal and text.                                 *
 * Hexadecimal is used without "0x" or "0h"                    *
 * Text is included between apostrophes "'"                    *
 * The character ' is ''                                       *
 * For example :                                               *
 *   'hello' : data "hello"                                    *
 *   'a' 'b' : data "ab"                                       *
 *   41 'b'  : data "Ab" (because 'A'==0x41)                   *
 *   'man'00 : data "man" ending with 0x00                     *
 *   'a''b'  : data "a'b"                                      *
 * Real examples :                                             *
 *  'hello' 0D 0A 'this is after a newline'                    *
 *  'one' 09 'two' 0D 0A                                       *
 ***************************************************************/

/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_DECODETYPE_DATA = 1,        /* exact data */
  NETWIB_DECODETYPE_HEXA,            /* hexadecimal */
  NETWIB_DECODETYPE_MIXED,           /* mixed */
  NETWIB_DECODETYPE_BASE64           /* base64 */
} netwib_decodetype;

/*-------------------------------------------------------------*/
/* Name : netwib_buf_decode
   Description :
     Append a decoded buffer.
   Input parameter(s) :
     *pbuftodecode : buffer to decode
     decodetype : decoding type
   Input/output parameter(s) :
     *pbuf : netwib_buf updated
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
   This function supports NETWIB_BUF_FLAGS_SENSITIVE.
*/
netwib_err netwib_buf_decode(netwib_constbuf *pbuftodecode,
                             netwib_decodetype decodetype,
                             netwib_buf *pbuf);
#define netwib_buf_decode_data(pbuftodecode,pbuf) netwib_buf_decode(pbuftodecode,NETWIB_DECODETYPE_DATA,pbuf)
#define netwib_buf_decode_hexa(pbuftodecode,pbuf) netwib_buf_decode(pbuftodecode,NETWIB_DECODETYPE_HEXA,pbuf)
#define netwib_buf_decode_mixed(pbuftodecode,pbuf) netwib_buf_decode(pbuftodecode,NETWIB_DECODETYPE_MIXED,pbuf)
#define netwib_buf_decode_base64(pbuftodecode,pbuf) netwib_buf_decode(pbuftodecode,NETWIB_DECODETYPE_BASE64,pbuf)

/*-------------------------------------------------------------*/
/***************************************************************
 * A quoted string is for example:                             *
 *   hello "hello" "hel\nlo" "abc d \\ \n \" \x00 z"           *
 * It is particularly adapted to decode program's parameters,  *
 * which can be enclosed, or not, between '"' character.       *
 * It is special :                                             *
 *  - decoding stops after first valid parameter               *
 *  - pbuftodecode is updated                                  *
 * Specifications:                                             *
 *   General :                                                 *
 *     Leading spaces or tabulations are ignored.              *
 *     Ending spaces, tabulations, NL or LF are ignored.       *
 *     Buffer pbuftodecode has beginoffset updated to point    *
 *     past the decoded string (after optional ending spaces). *
 *     When end is reached, without reading data, error        *
 *     NETWIB_ERR_DATAEND is returned.                         *
 *   Quoted strings (ex : "aa", "a\nb") :                      *
 *     Char '\' can be used before any char. It means the      *
 *     char following, except for sequences \a(alarm bell)     *
 *     \b(backspace) \t(tab) \n(NL) \r(LF) and                 *
 *     \x(hexa code of a character).                           *
 *     All chars, including NL and LF, are read till finding   *
 *     the last ". If it is not found, error                   *
 *     NETWIB_ERR_DATANOTAVAIL is returned (nothing is saved   *
 *     in pbuf).                                               *
 *   Unquoted strings (ex : aa, a\b)                           *
 *     Char '\' only means char '\'.                           *
 *     Read is stopped when a space, tab, NL or LF is found,   *
 *     or when end of pbuftodecode is reached.                 *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* Name : netwib_buf_decode_quoted
   Description :
     Append a decoded buffer.
   Input parameter(s) :
     *pbuftodecode : buffer to decode
     decodetype : decoding type
   Input/output parameter(s) :
     *pbuf : netwib_buf updated
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
   This function supports NETWIB_BUF_FLAGS_SENSITIVE.
*/
netwib_err netwib_buf_decode_quoted(netwib_buf *pbuftodecode,
                                    netwib_buf *pbuf);

