
/*-------------------------------------------------------------*/
/* Unicode Consortium and ISO worked on universal characters:
    - Unicode Consortium defined Unicode (up to 0x10FFFF chars)
    - ISO defined ISO 10646 (up to 0x7FFFFFFF chars originally, but
      they lately defined an upper limit of 0x10FFFF, like Unicode)

   They are equivalent (characters have the same numbering).
   The 127 lower characters of Unicode are ASCII(7bits).
   The 256 lower characters of Unicode are ISO-8859-1(Latin-1).
   A range from U-D800 to U-DFFF is reserved (surrogate area).

   They defined their encoding (UTF for Unicode and UCS for ISO):
    - UTF-7 (rfc2152) encodes using common characters (deprecated)
    - UTF-8 (rfc3629):
       U-00000000 -> U-0000007F : 0xxxxxxx
       U-00000080 -> U-000007FF : 110xxxxx 10xxxxxx
       U-00000800 -> U-0000FFFF : 1110xxxx 10xxxxxx 10xxxxxx
       U-00010000 -> U-001FFFFF : 11110xxx followed_by_3_bytes
       When upper limit was 0x7FFFFFFF, we also had:
       U-00200000 -> U-03FFFFFF : 111110xx followed_by_4_bytes
       U-04000000 -> U-7FFFFFFF : 1111110x followed_by_5_bytes
      This encoding is common because it preserves 7bits ASCII.
    - UTF-16 (rfc2781):
       U-00000000 -> U-0000FFFF : use 16 bits
       U-00010000 -> U-0010FFFF : cut in two 10 bits chunks:
                                  - the 1st is between D800 and DBFF
                                  - the 2nd is between DC00 and DFFF
                                  they are both in the surrogate area
       As the number is under 16 bit, it can be in BigEndian
       or LittleEndian. So, it can be named UTF-16BE or UTF-16LE.
       To automate this, the special character U-FEFF (Byte Order
       Mark) can be at the beginning to indicate endianess.
       Windows 2000, 2003 and XP uses UTF-16LE.
    - UCS-2 (UCS-2BE, UCS-2LE):
       U-00000000 -> U-0000FFFF : use 16 bits
       U-00010000 -> U-0010FFFF : cannot be encoded
       Windows 95, 98, Me and NT uses UCS-2LE.
    - UTF-32 (UTF-32BE, UTF-32LE):
       U-00000000 -> U-0010FFFF : use 32 bits
    - UCS-4 (UCS-4BE, UCS-4LE):
       U-00000000 -> U-0010FFFF : use 32 bits
       It is equivalent to UTF-32.

   Netwox does not support Unicode.
*/

/*-------------------------------------------------------------*/
/* Convert a 8bit to UCS-2LE:
     0x00 -> 0x7F : U-0000 -> U-007F
     0x80 -> 0xFF : U-0080 -> U-00FF (only ok for ISO-8859-1,
                                      for other, it's incorrect)
*/
netwib_err netwox_buf_decode_ucs2le(netwib_constbuf *pbuf8bit,
                                    netwib_buf *pbufucs2le);
/* converted to uppercase before */
netwib_err netwox_buf_decode_ucs2le_uc(netwib_constbuf *pbuf8bit,
                                       netwib_buf *pbufucs2le);
/* Convert a UCS-2LE to 8bit:
     U-0000 -> U-007F : 0x00 -> 0x7F
     U-0080 -> U-00FF : 0x80 -> 0xFF (only ok for ISO-8859-1,
                                      for other, it's incorrect)
     U-0100 -> U-FFFF : not converted
*/
netwib_err netwox_buf_append_ucs2le(netwib_constbuf *pbufucs2le,
                                    netwib_buf *pbuf8bit);




