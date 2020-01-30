
#ifndef NETWIB_PKT_TYPES_INCLUDED
#define NETWIB_PKT_TYPES_INCLUDED 1

/*-------------------------------------------------------------*/
/* Protocol values */
typedef enum {
  NETWIB_IPPROTO_IP = 0,        /* Dummy protocol for TCP */
  NETWIB_IPPROTO_HOPOPTS = 0,   /* IPv6 Hop-by-Hop options */
  NETWIB_IPPROTO_ICMP4 = 1,     /* Internet Control Message Protocol */
  NETWIB_IPPROTO_IGMP = 2,      /* Internet Group Management Protocol */
  NETWIB_IPPROTO_IPIP = 4,      /* IPIP tunnels */
  NETWIB_IPPROTO_TCP = 6,       /* Transmission Control Protocol */
  NETWIB_IPPROTO_EGP = 8,       /* Exterior Gateway Protocol */
  NETWIB_IPPROTO_PUP = 12,      /* PUP protocol */
  NETWIB_IPPROTO_UDP = 17,      /* User Datagram Protocol */
  NETWIB_IPPROTO_IDP = 22,      /* XNS IDP protocol */
  NETWIB_IPPROTO_TP = 29,       /* SO Transport Protocol Class 4 */
  NETWIB_IPPROTO_IP6 = 41,      /* IPv6 header */
  NETWIB_IPPROTO_ROUTING = 43,  /* IPv6 routing header */
  NETWIB_IPPROTO_FRAGMENT = 44, /* IPv6 fragmentation header */
  NETWIB_IPPROTO_RSVP = 46,     /* Reservation Protocol */
  NETWIB_IPPROTO_GRE = 47,      /* General Routing Encapsulation */
  NETWIB_IPPROTO_ESP = 50,      /* encapsulating security payload */
  NETWIB_IPPROTO_AH = 51,       /* authentication header */
  NETWIB_IPPROTO_ICMP6 = 58,    /* ICMPv6 */
  NETWIB_IPPROTO_NONE = 59,     /* IPv6 no next header */
  NETWIB_IPPROTO_DSTOPTS = 60,  /* IPv6 destination options */
  NETWIB_IPPROTO_MTP = 92,      /* Multicast Transport Protocol */
  NETWIB_IPPROTO_ENCAP = 98,    /* Encapsulation Header */
  NETWIB_IPPROTO_PIM = 103,     /* Protocol Independent Multicast */
  NETWIB_IPPROTO_COMP = 108,    /* Compression Header Protocol */
  NETWIB_IPPROTO_RAW = 255      /* Raw IP packets */
} netwib_ipproto;

/*-------------------------------------------------------------*/
#define netwib__data_append_uint8(data,u) *(data)++=(netwib_byte)(u)
#define netwib__data_append_uint16(data,u) *(data)++=netwib_c2_uint16_0(u);*(data)++=netwib_c2_uint16_1(u)
#define netwib__data_append_uint32(data,u) *(data)++=netwib_c2_uint32_0(u);*(data)++=netwib_c2_uint32_1(u);*(data)++=netwib_c2_uint32_2(u);*(data)++=netwib_c2_uint32_3(u)
#define netwib__data_append_uint64(data,u) *(data)++=netwib_c2_uint64_0(u);*(data)++=netwib_c2_uint64_1(u);*(data)++=netwib_c2_uint64_2(u);*(data)++=netwib_c2_uint64_3(u);*(data)++=netwib_c2_uint64_4(u);*(data)++=netwib_c2_uint64_5(u);*(data)++=netwib_c2_uint64_6(u);*(data)++=netwib_c2_uint64_7(u)
/* for structures created with little endian processors */
#define netwib__data_append_uint8_le(data,u) netwib__data_append_uint8(data,u)
#define netwib__data_append_uint16_le(data,u) *(data)++=netwib_c2_uint16_1(u);*(data)++=netwib_c2_uint16_0(u)
#define netwib__data_append_uint32_le(data,u) *(data)++=netwib_c2_uint32_3(u);*(data)++=netwib_c2_uint32_2(u);*(data)++=netwib_c2_uint32_1(u);*(data)++=netwib_c2_uint32_0(u)
#define netwib__data_append_uint64_le(data,u) *(data)++=netwib_c2_uint64_7(u);*(data)++=netwib_c2_uint64_6(u);*(data)++=netwib_c2_uint64_5(u);*(data)++=netwib_c2_uint64_4(u);*(data)++=netwib_c2_uint64_3(u);*(data)++=netwib_c2_uint64_2(u);*(data)++=netwib_c2_uint64_1(u);*(data)++=netwib_c2_uint64_0(u)

/*-------------------------------------------------------------*/
#define netwib__data_decode_uint8t(data,u,type) u=(type)(*(data)++)
#define netwib__data_decode_uint8t_le(data,u,type) netwib__data_decode_uint8t(data,u,type)
#define netwib__data_decode_uint16t(data,u,type) u=(type)((*(data)++)<<8);u=(type)((u)|*(data)++)
#define netwib__data_decode_uint16t_le(data,u,type) u=(type)(*(data)++);u=(type)(((*(data)++)<<8)|(u))
#define netwib__data_decode_uint32t(data,u,type) u=(type)((*(data)++)<<24);u=(type)((u)|((*(data)++)<<16));u=(type)((u)|((*(data)++)<<8));u=(type)((u)|*(data)++)
#define netwib__data_decode_uint32t_le(data,u,type) u=(type)(*(data)++);u=(type)(((*(data)++)<<8)|(u));u=(type)(((*(data)++)<<16)|(u));u=(type)(((*(data)++)<<24)|(u))
#if NETWIB_INT64_FAKE == 0
 #define netwib__data_decode_uint64t(data,u,type) u=(type)((netwib_uint64)(*(data)++)<<56);u=(type)((u)|((netwib_uint64)(*(data)++)<<48));u=(type)((u)|((netwib_uint64)(*(data)++)<<40));u=(type)((u)|((netwib_uint64)(*(data)++)<<32));u=(type)((u)|((netwib_uint64)(*(data)++)<<24));u=(type)((u)|((netwib_uint64)(*(data)++)<<16));u=(type)((u)|((netwib_uint64)(*(data)++)<<8));u=(type)((u)|(netwib_uint64)(*(data)++))
 #define netwib__data_decode_uint64t_le(data,u,type) u=(type)(netwib_uint64)(*(data)++);u=(type)(((netwib_uint64)(*(data)++)<<8)|(u));u=(type)(((netwib_uint64)(*(data)++)<<16)|(u));u=(type)(((netwib_uint64)(*(data)++)<<24)|(u));u=(type)(((netwib_uint64)(*(data)++)<<32)|(u));u=(type)(((netwib_uint64)(*(data)++)<<40)|(u));u=(type)(((netwib_uint64)(*(data)++)<<48)|(u));u=(type)(((netwib_uint64)(*(data)++)<<56)|(u))
#else
 #define netwib__data_decode_uint64t(data,u,type) netwib__data_decode_uint32(data,(u).high);netwib__data_decode_uint32(data,(u).low)
 #define netwib__data_decode_uint64t_le(data,u,type) netwib__data_decode_uint32_le(data,(u).low);netwib__data_decode_uint32_le(data,(u).high)
#endif
#define netwib__data_decode_uint8(data,u) netwib__data_decode_uint8t(data,u,netwib_uint8)
#define netwib__data_decode_uint8_le(data,u) netwib__data_decode_uint8(data,u)
#define netwib__data_decode_uint16(data,u) netwib__data_decode_uint16t(data,u,netwib_uint16)
#define netwib__data_decode_uint16_le(data,u) netwib__data_decode_uint16t_le(data,u,netwib_uint16)
#define netwib__data_decode_uint32(data,u) netwib__data_decode_uint32t(data,u,netwib_uint32)
#define netwib__data_decode_uint32_le(data,u) netwib__data_decode_uint32t_le(data,u,netwib_uint32)
#define netwib__data_decode_uint64(data,u) netwib__data_decode_uint64t(data,u,netwib_uint64)
#define netwib__data_decode_uint64_le(data,u) netwib__data_decode_uint64t_le(data,u,netwib_uint64)

#endif
