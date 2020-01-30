
/*-------------------------------------------------------------*/
/* error in smbmsg header
    - DOS error is errorclass(8).reserved(8).errorcode(16)
    - NT error is level(2bits).reserved(14).errorcode(16)
*/

/*-------------------------------------------------------------*/
/* DOS errors */
/*  defines */
#define netwox_smberr_uint16_swap(x) ((netwib_c2_uint16_1(x)<<8)|netwib_c2_uint16_0(x))
#define netwox_smberr_dos_classcode(errorclass,errorcode) (((errorclass)<<24)|netwox_smberr_uint16_swap(errorcode))
#define netwox_smberr_dos_class(error) ((netwib_uint8)((error)>>24))
#define netwox_smberr_dos_code(error) ((netwib_uint16)netwox_smberr_uint16_swap((error)&0xFFFF))
/*  errorclass */
#define NETWOX_SMBERR_DOS_CLASS_OK 0
#define NETWOX_SMBERR_DOS_CLASS_DOS 1 /* dos error */
#define NETWOX_SMBERR_DOS_CLASS_SRV 2 /* server error */
#define NETWOX_SMBERR_DOS_CLASS_HARD 3 /* hardware error */
#define NETWOX_SMBERR_DOS_CLASS_FMT 0xFF /* SMB packet format error */
/*  errorcode : there are a lot of error codes, but only following
    are defined. More will be added when necessary. */
#define NETWOX_SMBERR_DOS_CODE_OK_OK 0
#define NETWOX_SMBERR_DOS_CODE_DOS_NOTIMPLEMENTED 0x0001
#define NETWOX_SMBERR_DOS_CODE_DOS_FILENOTFOUND 0x0002
#define NETWOX_SMBERR_DOS_CODE_DOS_PATHNOTFOUND 0x0003
#define NETWOX_SMBERR_DOS_CODE_DOS_TOOMANYOPENFILES 0x0004
#define NETWOX_SMBERR_DOS_CODE_DOS_ACCESSDENIED 0x0005
#define NETWOX_SMBERR_DOS_CODE_DOS_BADFILEID 0x0006
#define NETWOX_SMBERR_DOS_CODE_DOS_NOTENOUGHMEMORY 0x0008
#define NETWOX_SMBERR_DOS_CODE_DOS_BADDATA 0x000d
#define NETWOX_SMBERR_DOS_CODE_DOS_DIRNOTEMPTY 0x0010
#define NETWOX_SMBERR_DOS_CODE_DOS_CROSSDEVICE 0x0011
#define NETWOX_SMBERR_DOS_CODE_DOS_NOMOREFILES 0x0012
#define NETWOX_SMBERR_DOS_CODE_DOS_FILEEXISTS 0x0050
#define NETWOX_SMBERR_DOS_CODE_DOS_INVALIDPARAMETER 0x0057
#define NETWOX_SMBERR_DOS_CODE_DOS_INVALIDNAME 0x007b
#define NETWOX_SMBERR_DOS_CODE_DOS_QUOTAEXCEEDED 0x0200
#define NETWOX_SMBERR_DOS_CODE_SRV_UNKNOWN 0x0001
#define NETWOX_SMBERR_DOS_CODE_SRV_BADPASSWORD 0x0002
#define NETWOX_SMBERR_DOS_CODE_SRV_NOPERM 0x0004
#define NETWOX_SMBERR_DOS_CODE_SRV_BADSHARENAME 0x0006
#define NETWOX_SMBERR_DOS_CODE_SRV_BADUSERID 0x005b
#define NETWOX_SMBERR_DOS_CODE_SRV_NOTSUPPORTED 0xffff
#define NETWOX_SMBERR_DOS_CODE_HARD_DEVICEWRITEPROTECTED 0x0013
#define NETWOX_SMBERR_DOS_CODE_HARD_DEVICENOTREADY 0x0015
netwib_err netwox_smberr_dos_show(netwib_uint8 cl,
                                       netwib_uint16 code,
                                       netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* NT errors */
/*  defines */
#define netwox_smberr_nt_levelcode(errorlevel,errorcode) ((netwib_uint32)(((errorlevel<<6)|(netwox_smberr_uint16_swap(errorcode)<<16))))
#define netwox_smberr_nt_level(error) ((netwib_uint8)(((error)&0xFF)>>6))
#define netwox_smberr_nt_code(error) ((netwib_uint16)netwox_smberr_uint16_swap((error)>>16))
/*  errorlevel */
#define NETWOX_SMBERR_NT_LEVEL_OK 0
#define NETWOX_SMBERR_NT_LEVEL_INFO 1
#define NETWOX_SMBERR_NT_LEVEL_WARNING 2
#define NETWOX_SMBERR_NT_LEVEL_ERROR 3
/*  errorcode : there are a lot of error codes, but only following
    are defined. More will be added when necessary. */
#define NETWOX_SMBERR_NT_CODE_OK_OK 0
#define NETWOX_SMBERR_NT_CODE_OK_WAIT2 0x0002
#define NETWOX_SMBERR_NT_CODE_OK_NOTIFYENUMDIR 0x010c
#define NETWOX_SMBERR_NT_CODE_WARNING_NOMOREFILES 0x0006
#define NETWOX_SMBERR_NT_CODE_ERROR_NOTIMPLEMENTED 0x0002
#define NETWOX_SMBERR_NT_CODE_ERROR_BADFILEID 0x0008
#define NETWOX_SMBERR_NT_CODE_ERROR_NOSUCHDEVICE 0x000e
#define NETWOX_SMBERR_NT_CODE_ERROR_INVALIDPARAMETER 0x000d
#define NETWOX_SMBERR_NT_CODE_ERROR_FILENOTFOUND 0x000f
#define NETWOX_SMBERR_NT_CODE_ERROR_MOREPROCESSINGREQUIRED 0x0016
#define NETWOX_SMBERR_NT_CODE_ERROR_ACCESSDENIED 0x0022
#define NETWOX_SMBERR_NT_CODE_ERROR_BUFFERTOOSMALL 0x0023
#define NETWOX_SMBERR_NT_CODE_ERROR_NAMENOTFOUND 0x0034
#define NETWOX_SMBERR_NT_CODE_ERROR_FILEEXISTS 0x0035
#define NETWOX_SMBERR_NT_CODE_ERROR_INVALIDNAME 0x0033
#define NETWOX_SMBERR_NT_CODE_ERROR_PATHNOTFOUND 0x003a
#define NETWOX_SMBERR_NT_CODE_ERROR_BADPATHSYNTAX 0x003b
#define NETWOX_SMBERR_NT_CODE_ERROR_BADDATA 0x003e
#define NETWOX_SMBERR_NT_CODE_ERROR_SHARINGVIOLATION 0x0043
#define NETWOX_SMBERR_NT_CODE_ERROR_QUOTAEXCEEDED 0x0044
#define NETWOX_SMBERR_NT_CODE_ERROR_PATHISADIR 0x00ba
#define NETWOX_SMBERR_NT_CODE_ERROR_CROSSDEVICE 0x00d4
#define NETWOX_SMBERR_NT_CODE_ERROR_BADPASSWORD 0x006a
#define NETWOX_SMBERR_NT_CODE_ERROR_LOGONFAILURE 0x006d
#define NETWOX_SMBERR_NT_CODE_ERROR_NOTENOUGHMEMORY 0x009a
#define NETWOX_SMBERR_NT_CODE_ERROR_DEVICEWRITEPROTECTED 0x00a2
#define NETWOX_SMBERR_NT_CODE_ERROR_DEVICENOTREADY 0x00a3
#define NETWOX_SMBERR_NT_CODE_ERROR_NOTSUPPORTED 0x00bb
#define NETWOX_SMBERR_NT_CODE_ERROR_BADSHARENAME 0x00cc
#define NETWOX_SMBERR_NT_CODE_ERROR_DIRNOTEMPTY 0x0101
#define NETWOX_SMBERR_NT_CODE_ERROR_NOTADIR 0x0103
#define NETWOX_SMBERR_NT_CODE_ERROR_CANCELED 0x0120
#define NETWOX_SMBERR_NT_CODE_ERROR_INVALIDLEVEL 0x0148
#define NETWOX_SMBERR_NT_CODE_ERROR_TOOMANYOPENFILES 0x011f
#define NETWOX_SMBERR_NT_CODE_ERROR_NOTAREPARSEPOINT 0x0275
netwib_err netwox_smberr_nt_show(netwib_uint8 level,
                                      netwib_uint16 code,
                                      netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* This enum is used to construct an error. It can't be used
   for decoding an error, because dos and nt errors are not
   equivalent.
*/
typedef enum {
  NETWOX_SMBERR_OK = 1,
  NETWOX_SMBERR_INTERNALERROR, /* severe error */
  NETWOX_SMBERR_NOTIMPLEMENTED, /* a feature not implemented */
  NETWOX_SMBERR_NOTSUPPORTED, /* a feature not supported by protocol */
  NETWOX_SMBERR_BADSHARENAME,
  NETWOX_SMBERR_BADPASSWORD,
  NETWOX_SMBERR_BADPARAMETER, /* bad integer */
  NETWOX_SMBERR_BADNAME, /* bad string */
  NETWOX_SMBERR_BADFILEID,
  NETWOX_SMBERR_FILENOTFOUND,
  NETWOX_SMBERR_FILEEXISTS,
  NETWOX_SMBERR_NOTADIR,
  NETWOX_SMBERR_NOTAFILE,
  NETWOX_SMBERR_QUOTAEXCEEDED, /* use if cannot write */
  NETWOX_SMBERR_DEVICEWRITEPROTECTED ,/* use if write not allowed */
  NETWOX_SMBERR_NOTAREPARSEPOINT,
  NETWOX_SMBERR_NOMOREFILES
} netwox_smberr;
netwib_err netwox_smberr_init(netwox_smberr smberr,
                              netwib_bool isnt,
                              netwib_uint32 *perr);
