
/*-------------------------------------------------------------*/
typedef struct ADAPTER ADAPTER;
typedef ADAPTER *LPADAPTER;

typedef union {
  struct {
    UINT LinkType;
    UINT LinkSpeed;
  } v30;
  struct {
    UINT LinkType;
    ULONGLONG LinkSpeed;
  } v31;
} netwib_priv_NetType;

struct _PACKET_OID_DATA {
  ULONG Oid;
  ULONG Length;
  UCHAR Data[1];
};
typedef struct _PACKET_OID_DATA PACKET_OID_DATA, *PPACKET_OID_DATA;

typedef struct _PACKET PACKET;
typedef struct PACKET *LPPACKET;

/*-------------------------------------------------------------*/
typedef LPPACKET (*pPacketAllocatePacket) (void);
typedef VOID (*pPacketCloseAdapter) (LPADAPTER lpAdapter);
typedef VOID (*pPacketFreePacket) (LPPACKET lpPacket);
typedef BOOLEAN (*pPacketGetNetType) (LPADAPTER AdapterObject, netwib_priv_NetType *type);
typedef PCHAR (*pPacketGetVersion) (void);
typedef VOID (*pPacketInitPacket) (LPPACKET lpPacket,PVOID  Buffer,UINT  Length);
typedef LPADAPTER (*pPacketOpenAdapter) (LPTSTR AdapterName);
typedef BOOLEAN (*pPacketRequest) (LPADAPTER  AdapterObject,BOOLEAN Set,PPACKET_OID_DATA  OidData);
typedef BOOLEAN (*pPacketSendPacket) (LPADAPTER AdapterObject,LPPACKET pPacket,BOOLEAN Sync);
typedef BOOLEAN (*pPacketSetBuff) (LPADAPTER AdapterObject,int dim);

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bool isloaded;
  HINSTANCE hinstance;
  netwib_uint32 versionmajor;
  netwib_uint32 versionminor;
  pPacketAllocatePacket PacketAllocatePacket;
  pPacketCloseAdapter PacketCloseAdapter;
  pPacketFreePacket PacketFreePacket;
  pPacketGetNetType PacketGetNetType;
  pPacketGetVersion PacketGetVersion;
  pPacketInitPacket PacketInitPacket;
  pPacketOpenAdapter PacketOpenAdapter;
  pPacketRequest PacketRequest;
  pPacketSendPacket PacketSendPacket;
  pPacketSetBuff PacketSetBuff;
} netwib_priv_dll_packet;
extern netwib_priv_dll_packet netwib_global_dll_packet;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_packet_init(netwib_priv_dll_packet *pdll_packet);
netwib_err netwib_priv_dll_packet_close(netwib_priv_dll_packet *pdll_packet);
