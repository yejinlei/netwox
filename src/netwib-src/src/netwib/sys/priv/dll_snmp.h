
/*-------------------------------------------------------------*/
typedef BOOL(WINAPI *pSnmpExtensionQuery) (IN BYTE requestType, IN OUT SnmpVarBindList *variableBindings, OUT AsnInteger32 *errorStatus, OUT AsnInteger32 *errorIndex);

/*-------------------------------------------------------------*/
typedef struct {
  netwib_bool isloaded;
  HINSTANCE hinstance;
  pSnmpExtensionQuery snmpqueryfunction;
} netwib_priv_dll_snmp;
extern netwib_priv_dll_snmp netwib_global_dll_snmp;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_snmp_init(netwib_priv_dll_snmp *pdll_snmp);
netwib_err netwib_priv_dll_snmp_close(netwib_priv_dll_snmp *pdll_snmp);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_snmpoid_init(AsnObjectIdentifier *DstObjId, ...);
netwib_err netwib_priv_dll_snmpoid_close(AsnObjectIdentifier *ObjId);
netwib_err netwib_priv_dll_snmpoid_display(AsnObjectIdentifier *ObjId);

/*-------------------------------------------------------------*/
typedef struct {
  SnmpVarBindList varBindList;
  SnmpVarBind varBind[5];
} netwib_priv_dll_snmpreq;

netwib_err netwib_priv_dll_snmpreq_init(netwib_priv_dll_snmpreq *pdll_snmpreq,
                                        AsnObjectIdentifier *poid1,
                                        AsnObjectIdentifier *poid2,
                                        AsnObjectIdentifier *poid3,
                                        AsnObjectIdentifier *poid4,
                                        AsnObjectIdentifier *poid5);
netwib_err netwib_priv_dll_snmpreq_close(netwib_priv_dll_snmpreq *pdll_snmpreq);
netwib_err netwib_priv_dll_snmpreq_getnext(netwib_priv_dll_snmpreq *pdll_snmpreq);
netwib_err netwib_priv_dll_snmpreq__uint32(netwib_priv_dll_snmpreq *pdll_snmpreq,
                                           netwib_uint32 index,
                                           netwib_uint32 *pnumber);
netwib_err netwib_priv_dll_snmpreq__buf(netwib_priv_dll_snmpreq *pdll_snmpreq,
                                        netwib_uint32 index,
                                        netwib_buf *pbuf);
netwib_err netwib_priv_dll_snmpreq__eth(netwib_priv_dll_snmpreq *pdll_snmpreq,
                                        netwib_uint32 index,
                                        netwib_eth *peth);
netwib_err netwib_priv_dll_snmpreq__ip(netwib_priv_dll_snmpreq *pdll_snmpreq,
                                       netwib_uint32 index,
                                       netwib_ip *pip);
netwib_err netwib_priv_dll_snmpreq_display(netwib_priv_dll_snmpreq *pdll_snmpreq);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_snmp_display(void);
