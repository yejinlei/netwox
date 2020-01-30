
/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_fmt
   Description :
     Add data to a buf.
   Input parameter(s) :
     fmt : format as explained below
   Input/output parameter(s) :
     *pbuf : netwib_buf updated
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
   This function supports NETWIB_BUF_FLAGS_SENSITIVE.
*/
/***************************************************************
 * Netwib provides a kind of printf format. It is not          *
 * compatible with standard printf, but it corresponds to      *
 * netwib usage.                                               *
 *   %% : %                                                    *
 *   %c : char                                                 *
 *   %s : string                                               *
 *   %p : pointer                                              *
 *   %{type} : special type                                    *
 *   %{generalformat;type:specificformat} : special type with  *
 *                                          a format           *
 *   %{generalformat;type} : special type with a format        *
 *   %{type:specificformat} : special type with a format       *
 * generalformat :                                             *
 *   - expressed as a regexp : "[lcr].[1-9][0-9]{0,1}"         *
 *       'lcr' : left, center, right                           *
 *       '.'   : padding character                             *
 *       '0-9' : minimum size                                  *
 *   - default value : "l 0"                                   *
 * type :                                                      *
 *   - uint32, uint16, uint8, uintmax, uintptr, uint64, byte   *
 *      - specificformat expressed as                          *
 *        "[#]{0,1}[0-9]{0,2}[bouxX]{0,1}"                     *
 *         '#' : add 'b' before binary numbers                 *
 *               add '0' before octal numbers                  *
 *               add '0x' before hexadecimal numbers           *
 *         '0-9' : total size (minimum value)                  *
 *         'bouxX' : binary, octal, decimal, hexadecimal       *
 *      - default value for specificformat : "u"               *
 *   - int32, int16, int8, intmax, intptr, int64               *
 *      - specificformat expressed as "[+]{0,1}[0-9]{0,2}"     *
 *         '+' : add a '+' before positive numbers             *
 *         '0-9' : total size (minimum value)                  *
 *      - default value for specificformat : ""                *
 *   - bool                                                    *
 *      - specificformat expressed as "[0tTyYsS]{0,1}"         *
 *         '0' : 0/1                                           *
 *         't' : true/false                                    *
 *         'T' : TRUE/FALSE                                    *
 *         'y' : yes/no                                        *
 *         'Y' : YES/NO                                        *
 *         's' : set/unset                                     *
 *         'S' : SET/UNSET                                     *
 *      - default value for specificformat : "t"               *
 *   - cmp                                                     *
 *      - specificformat expressed as "[=0e]{0,1}"             *
 *         '=' : <  =  >                                       *
 *         '0' : -  0  +                                       *
 *         'e' : lt eq gt                                      *
 *      - default value for specificformat : "="               *
 *   - c(char), s(char*), p(void*)                             *
 *      no specificformat                                      *
 *   - buf(netwib_buf*)                                        *
 *      no specificformat                                      *
 *   - eth(netwib_eth*), ip(netwib_ip*), port(netwib_port)     *
 *      no specificformat                                      *
 *   - eths(netwib_eths*), ips(netwib_ips*),                   *
 *     ports(netwib_ports*)                                    *
 *      no specificformat                                      *
 * examples :                                                  *
 *     %{uint32} : decimal                                     *
 *     %{int32} : decimal                                      *
 *     %{uint32:b} : binary                                    *
 *     %{uint32:o} : octal                                     *
 *     %{uint32:x} : hexadecimal                               *
 *     %{uint32:08X} : hexadecimal                             *
 *     %{c_20;uint32} : decimal centered in a 20 bytes block   *
 *                      padded with _                          *
 ***************************************************************/
netwib_err netwib_buf_append_fmt(netwib_buf *pbuf,
                                 netwib_conststring fmt,
                                 ...);

/*-------------------------------------------------------------*/
/* Name : netwib_buf_decode_fmt
   Description :
     Decode data from a buf.
   Input parameter(s) :
     *pbuf : netwib_buf
     fmt : format as explained above
   Input/output parameter(s) :
   Output parameter(s) :
     *pnumset : number of items set (decoded)
   Normal return values :
     NETWIB_ERR_OK : ok
     NETWIB_ERR_NOTCONVERTED : not decoded
   This function supports NETWIB_BUF_FLAGS_SENSITIVE.
*/
/***************************************************************
 * Netwib provides a kind of sscanf format. It is not          *
 * compatible with standard sscanf, but it corresponds to      *
 * netwib usage.                                               *
 *   %% : %                                                    *
 *   %c : char                                                 *
 *   %s : string                                               *
 *   %p : pointer                                              *
 *   %$ : end of string (if present, must be last)             *
 *   %{type} : special type                                    *
 *   %{generalformat;type:specificformat} : special type with  *
 *                                          a format           *
 *   %{generalformat;type} : special type with a format        *
 *   %{type:specificformat} : special type with a format       *
 * generalformat :                                             *
 *   - expressed as a regexp : "[*]{0,1}[lcr].[1-9][0-9]{0,1}" *
 *     or "[*]{0,1}"                                           *
 *       '*'   : suppresses assignment (skip field)            *
 *       'lcr' : left, center, right                           *
 *       '.'   : padding character                             *
 *       '0-9' : minimum size                                  *
 *   - default value : "l 0"                                   *
 * type :                                                      *
 *   They have the same meaning as for netwib_buf_append_fmt,  *
 *   but are always pointers :                                 *
 *   - uint32 : netwib_uint32*                                 *
 *   - uint16 : netwib_uint16*                                 *
 *   - uint8  : netwib_uint8*                                  *
 *   - uintmax: netwib_uintmax*                                *
 *   - uintptr: netwib_uintptr*                                *
 *   - uint64 : netwib_uint64*                                 *
 *   - byte   : netwib_byte*                                   *
 *   - bool   : netwib_bool*                                   *
 *   - int32  : netwib_int32*                                  *
 *   - int16  : netwib_int16*                                  *
 *   - int8   : netwib_int8*                                   *
 *   - cmp    : netwib_cmp*                                    *
 *   - c      : char*                                          *
 *   - s      : char[] (must be big enough : use buf instead)  *
 *   - p      : netwib_ptr* (or void**)                        *
 *   - buf    : netwib_buf*                                    *
 *   - eth    : netwib_eth*                                    *
 *   - ip     : netwib_ip*                                     *
 *   - port   : netwib_port*                                   *
 *   - eths   : netwib_eths*                                   *
 *   - ips    : netwib_ips*                                    *
 *   - ports  : netwib_ports*                                  *
 * specificformat :                                            *
 *   For "s" and "buf", the default behavior is to stop when a *
 *   space is encountered. By using "glob" as specificformat,  *
 *   every char is stored in the buffer (including leading and *
 *   ending spaces).                                           *
 ***************************************************************/
netwib_err netwib_buf_decode_fmt(netwib_constbuf *pbuf,
                                 netwib_conststring fmt,
                                 ...);
