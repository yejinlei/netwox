/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_priv_dll_snmp netwib_global_dll_snmp;

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_dll_snmpoid_alloc(netwib_int32 idLength,
                                                AsnObjectIdentifier *ObjId)
{
  ObjId->idLength = idLength;
  /* we cannot use netwib_ptr_malloc because netwib_priv_dll_snmpoid_free
     cannot use netwib_ptr_free */
  ObjId->ids = (UINT *) GlobalAlloc(GMEM_ZEROINIT, idLength *sizeof(UINT));
  if (ObjId->ids == NULL)
    return(NETWIB_ERR_FUGLOBALALLOC);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_dll_snmpoid_free(AsnObjectIdentifier *ObjId)
{
  void *pret;

  /* we cannot use netwib_ptr_free because this function is sometimes used
     to free items which are allocated by SNMP DLL with a GlobalAlloc */
  pret = GlobalFree(ObjId->ids);
  if (pret != NULL)
    return(NETWIB_ERR_FUGLOBALFREE);
  ObjId->ids = NULL;
  ObjId->idLength = 0;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_dll_snmpoid_cpy(AsnObjectIdentifier *DstObjId,
                                              AsnObjectIdentifier *SrcObjId)
{
  netwib_er(netwib_priv_dll_snmpoid_alloc(SrcObjId->idLength, DstObjId));
  netwib_c_memcpy(DstObjId->ids, SrcObjId->ids,
                 SrcObjId->idLength *sizeof(UINT));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_snmpoid_init(AsnObjectIdentifier *DstObjId,
                                        ...)
{
  int count, i;
  va_list marker;

  /* count the number of arguments */
  va_start(marker, DstObjId);
  count = 0;
  i = va_arg(marker, int);

  while (i != -1) {
    count++;
    i = va_arg(marker, int);
  }
  va_end(marker);

  /* initialize memory */
  netwib_er(netwib_priv_dll_snmpoid_alloc(count, DstObjId));

  /* set values */
  va_start(marker, DstObjId);
  for (i = 0; i < count; i++) {
    DstObjId->ids[i] = va_arg(marker, int);
  }
  va_end(marker);

  return NETWIB_ERR_OK;
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_snmpoid_close(AsnObjectIdentifier *ObjId)
{
  netwib_er(netwib_priv_dll_snmpoid_free(ObjId));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_snmpoid_display(AsnObjectIdentifier *ObjId)
{
  netwib_uint32 i;

  for (i = 0; i < ObjId->idLength - 1; i++) {
    netwib_er(netwib_fmt_display("%{uint32}.", (netwib_uint32)ObjId->ids[i]));
  }
  netwib_er(netwib_fmt_display("%{uint32}", (netwib_uint32)ObjId->ids[ObjId->idLength - 1]));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
static netwib_err netwib_SnmpUtilVarBindFree(IN OUT SnmpVarBind *VarBind)
{
  BYTE asnType;
  void *pret;

  /* free object name */
  netwib_er(netwib_priv_dll_snmpoid_free(&VarBind->name));

  asnType = VarBind->value.asnType;
  if (asnType == ASN_OBJECTIDENTIFIER) {
    netwib_er(netwib_priv_dll_snmpoid_free(&VarBind->value.asnValue.object));
  } else if ((asnType == ASN_OCTETSTRING) ||
             (asnType == ASN_IPADDRESS) ||
             (asnType == ASN_OPAQUE) || (asnType == ASN_SEQUENCE)) {
    if (VarBind->value.asnValue.string.dynamic) {
      /* this variable is not allocated by netwib, so we cannot use
         netwib_ptr_free */
      pret = GlobalFree(VarBind->value.asnValue.string.stream);
      if (pret != NULL)
        return(NETWIB_ERR_FUGLOBALFREE);
    }
  }
  VarBind->value.asnType = ASN_NULL;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
typedef BOOL(WINAPI *pSnmpExtensionInit) (IN DWORD dwTimeZeroReference,
                                          OUT HANDLE *hPollForTrapEvent,
                                          OUT AsnObjectIdentifier *supportedView);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_snmp_init(netwib_priv_dll_snmp *pdll_snmp)
{
  pSnmpExtensionInit snmpe_init;
  HANDLE PollForTrapEvent;
  AsnObjectIdentifier SupportedView;
  int reti;

  pdll_snmp->isloaded = NETWIB_FALSE;

  /* load the DLL */
  netwib_er(netwib_priv_dll_loadlibrary("inetmib1.dll",
                                        &pdll_snmp->hinstance));

  /* init query function */
  netwib__priv_dll_f(pdll_snmp, snmpqueryfunction,
                     pSnmpExtensionQuery, "SnmpExtensionQuery");

  /* initialize the extension agent */
  snmpe_init =
    (pSnmpExtensionInit) GetProcAddress(pdll_snmp->hinstance,
                                        "SnmpExtensionInit");
  if (snmpe_init == NULL) {
    FreeLibrary(pdll_snmp->hinstance);
    return(NETWIB_ERR_FUGETPROCADDRESS);
  }
  reti = snmpe_init(GetTickCount(), &PollForTrapEvent, &SupportedView);
  if (!reti) {
    FreeLibrary(pdll_snmp->hinstance);
    return(NETWIB_ERR_FUSNMPEXTENSIONINIT);
  }
  /*netwib_priv_AsnOid_display(&SupportedView); */
  /* PollForTrapEvent and SupportedView are automatically closed by the dll */

  pdll_snmp->isloaded = NETWIB_TRUE;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_snmp_close(netwib_priv_dll_snmp *pdll_snmp)
{
  int reti;

  if (!pdll_snmp->isloaded)
    return(NETWIB_ERR_OK);

  /* unload the DLL */
  reti = FreeLibrary(pdll_snmp->hinstance);
  if (!reti)
    return(NETWIB_ERR_FUFREELIBRARY);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_snmpreq_init(netwib_priv_dll_snmpreq *pdll_snmpreq,
                                        AsnObjectIdentifier *poid1,
                                        AsnObjectIdentifier *poid2,
                                        AsnObjectIdentifier *poid3,
                                        AsnObjectIdentifier *poid4,
                                        AsnObjectIdentifier *poid5)
{

  /* varBind will be used as the array */
  pdll_snmpreq->varBindList.list = pdll_snmpreq->varBind;

  /* if there is no oid */
  if (poid1 == NULL) {
    pdll_snmpreq->varBindList.len = 0;
    return(NETWIB_ERR_OK);
  }

  /* first oid is set */
  pdll_snmpreq->varBindList.len = 1;
  netwib_er(netwib_priv_dll_snmpoid_cpy
           (&(pdll_snmpreq->varBindList.list[0].name), poid1));
  pdll_snmpreq->varBindList.list[0].value.asnType = ASN_NULL;

  /* second oid is set */
  if (poid2 != NULL) {
    pdll_snmpreq->varBindList.len = 2;
    pdll_snmpreq->varBindList.list[1].value.asnType = ASN_NULL;
    netwib_er(netwib_priv_dll_snmpoid_cpy
             (&(pdll_snmpreq->varBindList.list[1].name), poid2));
  }

  /* third oid is set */
  if (poid3 != NULL) {
    pdll_snmpreq->varBindList.len = 3;
    pdll_snmpreq->varBindList.list[2].value.asnType = ASN_NULL;
    netwib_er(netwib_priv_dll_snmpoid_cpy
             (&(pdll_snmpreq->varBindList.list[2].name), poid3));
  }

  /* fourth oid is set */
  if (poid4 != NULL) {
    pdll_snmpreq->varBindList.len = 4;
    pdll_snmpreq->varBindList.list[3].value.asnType = ASN_NULL;
    netwib_er(netwib_priv_dll_snmpoid_cpy
             (&(pdll_snmpreq->varBindList.list[3].name), poid4));
  }

  /* fifth oid is set */
  if (poid5 != NULL) {
    pdll_snmpreq->varBindList.len = 5;
    pdll_snmpreq->varBindList.list[4].value.asnType = ASN_NULL;
    netwib_er(netwib_priv_dll_snmpoid_cpy
             (&(pdll_snmpreq->varBindList.list[4].name), poid5));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_snmpreq_close(netwib_priv_dll_snmpreq *pdll_snmpreq)
{
  unsigned int i;

  for (i = 0; i < pdll_snmpreq->varBindList.len; i++) {
    netwib_er(netwib_SnmpUtilVarBindFree(&(pdll_snmpreq->varBindList.list[i])));
  }
  pdll_snmpreq->varBindList.len = 0;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_snmpreq_display(netwib_priv_dll_snmpreq *pdll_snmpreq)
{
  netwib_buf buf;
  netwib_uint32 i, j;

  netwib_er(netwib_fmt_display("varBindList : %{uint32}\n",
                                pdll_snmpreq->varBindList.len));
  for (i = 0; i < pdll_snmpreq->varBindList.len; i++) {
    /* display name */
    netwib_er(netwib_priv_dll_snmpoid_display(&(pdll_snmpreq->varBindList.list[i].name)));
    /* display value type */
    netwib_er(netwib_fmt_display(" Type %{uint32}(",
                                  pdll_snmpreq->varBindList.list[i].value.asnType));
    switch (pdll_snmpreq->varBindList.list[i].value.asnType) {
      case ASN_INTEGER:
        netwib_er(netwib_fmt_display("ASN_INTEGER"));
        break;
      case ASN_OCTETSTRING:
        netwib_er(netwib_fmt_display("ASN_OCTETSTRING"));
        break;
      case ASN_NULL:
        netwib_er(netwib_fmt_display("ASN_NULL"));
        break;
      case ASN_OBJECTIDENTIFIER:
        netwib_er(netwib_fmt_display("ASN_OBJECTIDENTIFIER"));
        break;
      case ASN_SEQUENCE:
        netwib_er(netwib_fmt_display("ASN_SEQUENCE"));
        break;
      case ASN_IPADDRESS:
        netwib_er(netwib_fmt_display("ASN_IPADDRESS"));
        break;
      case ASN_COUNTER32:
        netwib_er(netwib_fmt_display("ASN_COUNTER32"));
        break;
      case ASN_GAUGE32:
        netwib_er(netwib_fmt_display("ASN_GAUGE32"));
        break;
      case ASN_TIMETICKS:
        netwib_er(netwib_fmt_display("ASN_TIMETICKS"));
        break;
      case ASN_OPAQUE:
        netwib_er(netwib_fmt_display("ASN_OPAQUE"));
        break;
      default:
        netwib_er(netwib_fmt_display("???"));
        break;
    }
    netwib_er(netwib_fmt_display(")\n"));
    /* display the value */
    switch (pdll_snmpreq->varBindList.list[i].value.asnType) {
      case ASN_INTEGER:
        netwib_er(netwib_fmt_display(" %{uint32}\n",
                                      (netwib_uint32)pdll_snmpreq->varBindList.list[i].value.asnValue.number));
        break;
      case ASN_OCTETSTRING:
        netwib_er(netwib_fmt_display("len=%{uint32}\n",
                                      (netwib_uint32)pdll_snmpreq->varBindList.list[i].value.asnValue.string.length));
        netwib_er(netwib_buf_init_ext_arrayfilled((netwib_data)pdll_snmpreq->varBindList.list[i].value.asnValue.string.stream,
                  (netwib_uint32)pdll_snmpreq->varBindList.list[i].value.asnValue.string.length,
                  &buf));
        netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DUMP));
        break;
      case ASN_NULL:
        /* nothing to print */
        break;
      case ASN_COUNTER32:
        netwib_er(netwib_fmt_display(" %{uint32}\n",
               pdll_snmpreq->varBindList.list[i].value.asnValue.counter));
        break;
      case ASN_GAUGE32:
        netwib_er(netwib_fmt_display(" %{uint32}\n",
               pdll_snmpreq->varBindList.list[i].value.asnValue.gauge));
        break;
      case ASN_TIMETICKS:
        netwib_er(netwib_fmt_display(" %{uint32}\n",
               pdll_snmpreq->varBindList.list[i].value.asnValue.ticks));
        break;
      case ASN_OBJECTIDENTIFIER:
        /* display name */
        netwib_er(netwib_fmt_display(" "));
        netwib_er(netwib_priv_dll_snmpoid_display(&(pdll_snmpreq->varBindList.list[i].value.asnValue.object)));
        netwib_er(netwib_fmt_display("\n"));
        break;
      case ASN_IPADDRESS:
        netwib_er(netwib_fmt_display(" "));
        for (j = 0;
             j < pdll_snmpreq->varBindList.list[i].value.asnValue.address.length;
             j++) {
          netwib_er(netwib_fmt_display("%{uint32}.",
                 pdll_snmpreq->varBindList.list[i].value.asnValue.address.stream[j]));
        }
        netwib_er(netwib_fmt_display("\n"));
        break;
      case ASN_SEQUENCE:
      case ASN_OPAQUE:
      default:
        return(NETWIB_ERR_LONOTIMPLEMENTED);
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_snmpreq__uint32(netwib_priv_dll_snmpreq *pdll_snmpreq,
                                           netwib_uint32 index,
                                           netwib_uint32 *pnumber)
{
  netwib_uint32 number;

  if (index >= pdll_snmpreq->varBindList.len)
    return(NETWIB_ERR_PATOOHIGH);

  switch (pdll_snmpreq->varBindList.list[index].value.asnType) {
    case ASN_INTEGER:
      number = pdll_snmpreq->varBindList.list[index].value.asnValue.number;
      break;
    case ASN_COUNTER32:
      number = pdll_snmpreq->varBindList.list[index].value.asnValue.counter;
      break;
    case ASN_GAUGE32:
      number = pdll_snmpreq->varBindList.list[index].value.asnValue.gauge;
      break;
    case ASN_TIMETICKS:
      number = pdll_snmpreq->varBindList.list[index].value.asnValue.ticks;
      break;
    case ASN_IPADDRESS:
      if (pdll_snmpreq->varBindList.list[index].value.asnValue.address.length != 4) {
        number = 0;
      } else {
        number = (pdll_snmpreq->varBindList.list[index].value.asnValue.address.stream[0] << 24) |
                 (pdll_snmpreq->varBindList.list[index].value.asnValue.address.stream[1] << 16) |
                 (pdll_snmpreq->varBindList.list[index].value.asnValue.address.stream[2] << 8) |
                 (pdll_snmpreq->varBindList.list[index].value.asnValue.address.stream[3]);
      }
      break;
    case ASN_OCTETSTRING:
    case ASN_NULL:
    case ASN_OBJECTIDENTIFIER:
    case ASN_SEQUENCE:
    case ASN_OPAQUE:
    default:
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  if (pnumber != NULL)
    *pnumber = number;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_snmpreq__buf(netwib_priv_dll_snmpreq *pdll_snmpreq,
                                        netwib_uint32 index,
                                        netwib_buf *pbuf)
{
  if (index >= pdll_snmpreq->varBindList.len)
    return(NETWIB_ERR_PATOOHIGH);

  switch (pdll_snmpreq->varBindList.list[index].value.asnType) {
    case ASN_OCTETSTRING:
    case ASN_IPADDRESS:
      netwib_er(netwib_buf_append_data(
         (netwib_data)pdll_snmpreq->varBindList.list[index].value.asnValue.string.stream,
                pdll_snmpreq->varBindList.list[index].value.asnValue.string.length,
                pbuf));
      break;
    case ASN_NULL:
    case ASN_INTEGER:
    case ASN_SEQUENCE:
    case ASN_OPAQUE:
    case ASN_COUNTER32:
    case ASN_GAUGE32:
    case ASN_TIMETICKS:
    default:
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_snmpreq__eth(netwib_priv_dll_snmpreq *pdll_snmpreq,
                                        netwib_uint32 index,
                                        netwib_eth *peth)
{
  netwib_uint32 i;

  if (index >= pdll_snmpreq->varBindList.len)
    return(NETWIB_ERR_PATOOHIGH);

  switch (pdll_snmpreq->varBindList.list[index].value.asnType) {
    case ASN_OCTETSTRING:
      if (pdll_snmpreq->varBindList.list[index].value.asnValue.string.length == (netwib_uint32)NETWIB_ETH_LEN) {
        for (i = 0; i < (netwib_uint32)NETWIB_ETH_LEN; i++) {
          peth->b[i] = pdll_snmpreq->varBindList.list[index].value.asnValue.string.stream[i];
        }
      } else {
        for (i = 0; i < (netwib_uint32)NETWIB_ETH_LEN; i++) {
          peth->b[i] = 0;
        }
      }
      break;
    case ASN_IPADDRESS:
    case ASN_NULL:
    case ASN_INTEGER:
    case ASN_SEQUENCE:
    case ASN_OPAQUE:
    case ASN_COUNTER32:
    case ASN_GAUGE32:
    case ASN_TIMETICKS:
    default:
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_snmpreq__ip(netwib_priv_dll_snmpreq *pdll_snmpreq,
                                       netwib_uint32 index,
                                       netwib_ip *pip)
{
  AsnIPAddress *pasnip;

  if (index >= pdll_snmpreq->varBindList.len)
    return(NETWIB_ERR_PATOOHIGH);

  switch (pdll_snmpreq->varBindList.list[index].value.asnType) {
    case ASN_IPADDRESS:
      if (pdll_snmpreq->varBindList.list[index].value.asnValue.address.length != 4) {
        netwib_er(netwib_ip_init_ip4(0, pip));
      } else {
        pasnip = &pdll_snmpreq->varBindList.list[index].value.asnValue.address;
        netwib_er(netwib_ip_init_ip4_fields(pasnip->stream[0], pasnip->stream[1],
                                            pasnip->stream[2], pasnip->stream[3], pip));
      }
      break;
    default:
      return(NETWIB_ERR_PAINVALIDTYPE);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_snmpreq_getnext(netwib_priv_dll_snmpreq *pdll_snmpreq)
{
  AsnInteger32 errorStatus;
  AsnInteger32 errorIndex;
  int reti;

  reti = (netwib_global_dll_snmp.snmpqueryfunction) (SNMP_PDU_GETNEXT,
                                               &(pdll_snmpreq->varBindList),
                                               &errorStatus, &errorIndex);
  if (!reti)
    return(NETWIB_ERR_FUSNMPEXTENSIONQUERY);
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_snmp_display()
{
  netwib_priv_dll_snmpreq dll_snmpreq;
  AsnObjectIdentifier oid0;

  netwib_er(netwib_priv_dll_snmpoid_init(&oid0, 1, 3, 6, 1, 2, 1, -1));
  netwib_er(netwib_priv_dll_snmpreq_init(&dll_snmpreq, &oid0, NULL, NULL, NULL, NULL));
  netwib_er(netwib_priv_dll_snmpoid_close(&oid0));
  while (dll_snmpreq.varBindList.list[0].name.idLength < 7
         || dll_snmpreq.varBindList.list[0].name.ids[6] < 10) {
    netwib_er(netwib_priv_dll_snmpreq_getnext(&dll_snmpreq));
    netwib_er(netwib_priv_dll_snmpreq_display(&dll_snmpreq));
  }
  netwib_er(netwib_priv_dll_snmpreq_close(&dll_snmpreq));

  return(NETWIB_ERR_OK);
}
