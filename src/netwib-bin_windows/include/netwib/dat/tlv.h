
/*-------------------------------------------------------------*/
/* We need a forward declaration for netwib_eth, netwib_ip4 and
   netwib_port.
*/
#include "../net/types.h"

/*-------------------------------------------------------------*/
/***************************************************************
 * This module permits to store TLV in a netwib_buf. A TLV is  *
 * a data block containing :                                   *
 *  - Type : netwib_tlvtype                                    *
 *  - Length : length of value                                 *
 *  - Value : value of type 'type'                             *
 ***************************************************************/


/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_TLVTYPE_BUF = 1,     /* data */
  NETWIB_TLVTYPE_UINT,        /* netwib_uint32 16 or 8 */
  NETWIB_TLVTYPE_ETH,         /* netwib_eth */
  NETWIB_TLVTYPE_IP,          /* netwib_ip */
  NETWIB_TLVTYPE_END = 100,   /* end */
  /* start of free numbers for user */
  NETWIB_TLVTYPE_USER_BEGIN = NETWIB_ENUM_USER_BEGIN
} netwib_tlvtype;

/*-------------------------------------------------------------*/
/* Name : netwib_tlv_append_xyz
   Description :
     Add a TLV to a buf.
   Input parameter(s) :
     ...
   Input/output parameter(s) :
     *pbuf : netwib_buf updated
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
   The netwib_tlv_append_buf function supports NETWIB_BUF_FLAGS_SENSITIVE.
*/
netwib_err netwib_tlv_append_buf(netwib_constbuf *pbuf,
                                 netwib_buf *ptlv);
netwib_err netwib_tlv_append_uint32(netwib_uint32 ui,
                                    netwib_buf *ptlv);
netwib_err netwib_tlv_append_uint64(netwib_uint64 ui,
                                    netwib_buf *ptlv);
netwib_err netwib_tlv_append_eth(netwib_consteth *peth,
                                 netwib_buf *ptlv);
netwib_err netwib_tlv_append_ip(netwib_constip *pip,
                                netwib_buf *ptlv);

/*-------------------------------------------------------------*/
/* Name : netwib_tlv_append_end
   Description :
     Indicates the end of data. It is for example used after
     several netwib_tlv_append_data to indicates the end of data.
   Input parameter(s) :
   Input/output parameter(s) :
     *pbuf : netwib_buf updated
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_tlv_append_end(netwib_buf *ptlv);

/*-------------------------------------------------------------*/
/* Name : netwib_tlv_append_newtype
   Description :
     Add a user defined TLV.
   Input parameter(s) :
     type : type
     *pvalue : buffer containing the value
   Input/output parameter(s) :
     *pbuf : netwib_buf updated
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_tlv_append_newtype(netwib_tlvtype type,
                                     netwib_constbuf *pvalue,
                                     netwib_buf *ptlv);

/*-------------------------------------------------------------*/
/* Name : netwib_tlv_xyzpend_tlv
   Description :
     Add a predefined TLV.
   Input parameter(s) :
     *pnewtlv : buffer containing the preformed tlv
   Input/output parameter(s) :
     *pbuf : netwib_buf updated
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
   Both functions supports NETWIB_BUF_FLAGS_SENSITIVE.
*/
netwib_err netwib_tlv_append_tlv(netwib_constbuf *pnewtlv,
                                 netwib_buf *ptlv);
netwib_err netwib_tlv_prepend_tlv(netwib_constbuf *pnewtlv,
                                  netwib_buf *ptlv);

/*-------------------------------------------------------------*/
/* Name : netwib_tlv_entry_typelen
   Description :
     Obtain type and length of current TLV.
   Input parameter(s) :
     *pbuf : netwib_buf containing the TLV
   Input/output parameter(s) :
   Output parameter(s) :
     *ptype : type
     *plength : length
     *pskipsize : size to skip this entry
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_tlv_entry_typelen(netwib_constbuf *ptlv,
                                    netwib_tlvtype *ptype,
                                    netwib_uint32 *plength,
                                    netwib_uint32 *pskipsize);

/*-------------------------------------------------------------*/
/* Name : netwib_tlv_decode_xyz
   Description :
     Decode a TLV from a buf.
   Input parameter(s) :
     *pbuf : netwib_buf containing the TLV
   Input/output parameter(s) :
     ...
   Output parameter(s) :
     *pskipsize : size to skip this entry
   Normal return values :
     NETWIB_ERR_OK : ok
   The netwib_tlv_decode_buf function supports NETWIB_BUF_FLAGS_SENSITIVE.
*/
netwib_err netwib_tlv_decode_buf(netwib_constbuf *ptlv,
                                 netwib_bufext *pbuf,
                                 netwib_uint32 *pskipsize);
netwib_err netwib_tlv_decode_uint32(netwib_constbuf *ptlv,
                                    netwib_uint32 *pui,
                                    netwib_uint32 *pskipsize);
netwib_err netwib_tlv_decode_uint64(netwib_constbuf *ptlv,
                                    netwib_uint64 *pui,
                                    netwib_uint32 *pskipsize);
netwib_err netwib_tlv_decode_eth(netwib_constbuf *ptlv,
                                 netwib_eth *peth,
                                 netwib_uint32 *pskipsize);
netwib_err netwib_tlv_decode_ip(netwib_constbuf *ptlv,
                                netwib_ip *pip,
                                netwib_uint32 *pskipsize);

/*-------------------------------------------------------------*/
/* Name : netwib_tlv_decode_newtype
   Description :
     Decode a user defined TLV from a buf.
   Input parameter(s) :
     *pbuf : netwib_buf containing the TLV
   Input/output parameter(s) :
   Output parameter(s) :
     *ptype : type
     *plength : length
     *pvalue : buffer containing the value
     *pskipsize : size to skip this entry
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_tlv_decode_newtype(netwib_constbuf *ptlv,
                                     netwib_tlvtype *ptype,
                                     netwib_uint32 *plength,
                                     netwib_bufext *pvalue,
                                     netwib_uint32 *pskipsize);

/*-------------------------------------------------------------*/
/* Name : netwib_tlv_decode_tlv
   Description :
     Decode a TLV from a buf.
   Input parameter(s) :
     *pbuf : netwib_buf containing the TLV
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf containing the first entry
     *pskipsize : size to skip this entry
   Normal return values :
     NETWIB_ERR_OK : ok
   This function supports NETWIB_BUF_FLAGS_SENSITIVE.
*/
netwib_err netwib_tlv_decode_tlv(netwib_constbuf *ptlv,
                                 netwib_bufext *pfirsttlv,
                                 netwib_uint32 *pskipsize);
