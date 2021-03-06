
/*-------------------------------------------------------------*/
typedef enum {
  /* positive preliminary replies */
  NETWOX_FTPREPLY_125_DATABEGIN = 125,
  NETWOX_FTPREPLY_150_WILLOPENDATA = 150,
  /* positive completion replies */
  NETWOX_FTPREPLY_200_CMDOK = 200,
  NETWOX_FTPREPLY_202_CMDSUPERFLUOUS = 202,
  NETWOX_FTPREPLY_213_FILESTATUS = 213,
  NETWOX_FTPREPLY_215_SYST = 215,
  NETWOX_FTPREPLY_220_SVCREADY = 220,
  NETWOX_FTPREPLY_221_SVCCLOSING = 221,
  NETWOX_FTPREPLY_226_DATAEND = 226,
  NETWOX_FTPREPLY_227_ENTERINGPASSIVEMODE = 227,
  NETWOX_FTPREPLY_229_ENTERINGPASSIVEMODEEXT = 229,
  NETWOX_FTPREPLY_230_AUTHOK = 230,
  NETWOX_FTPREPLY_250_FILEACTIONDONE = 250,
  NETWOX_FTPREPLY_257_FILECREATED = 257,
  /* positive intermediate replies */
  NETWOX_FTPREPLY_331_AUTHUSEROK = 331,
  NETWOX_FTPREPLY_332_NEEDACCOUNT = 332,
  NETWOX_FTPREPLY_350_FILEACTIONPENDING = 350,
  /* transient/temporary negative completion replies */
  NETWOX_FTPREPLY_421_SVCNOTAVAILABLE = 421,
  NETWOX_FTPREPLY_425_CANTOPENDATA = 425,
  NETWOX_FTPREPLY_426_TRANSFERABORTED = 426,
  NETWOX_FTPREPLY_450_FILEACTIONNOTTAKEN = 450, /* file busy */
  NETWOX_FTPREPLY_451_ACTIONABORTED = 451,
  NETWOX_FTPREPLY_452_ACTIONNOTTAKEN = 452,
  /* permanent negative completion replies */
  NETWOX_FTPREPLY_500_SYNTAXERROR = 500,
  NETWOX_FTPREPLY_501_PARAMETERERROR = 501,
  NETWOX_FTPREPLY_502_CMDNOTIMPLEMENTED = 502,
  NETWOX_FTPREPLY_503_CMDBADSEQUENCE = 503,
  NETWOX_FTPREPLY_504_CMDOKBADPARAMETER = 504,
  NETWOX_FTPREPLY_521_DIREXISTS = 521,
  NETWOX_FTPREPLY_530_AUTHBAD = 530,
  NETWOX_FTPREPLY_532_FILECANTSTORE = 532, /* need a privileged account */
  NETWOX_FTPREPLY_550_ACTIONNOTTAKEN = 550, /* file not found, no access */
  NETWOX_FTPREPLY_551_ACTIONABORTED = 551,
  NETWOX_FTPREPLY_552_FILEACTIONABORTED = 552,
  NETWOX_FTPREPLY_553_ACTIONNOTTAKEN = 553 /* forbidden filename */
} netwox_ftpreply;
