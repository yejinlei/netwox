
/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_PRIV_NOTIFYTYPE_EMERG = 1,   /* internal error */
  NETWIB_PRIV_NOTIFYTYPE_ALERT,       /* user must change its program */
  NETWIB_PRIV_NOTIFYTYPE_WARNING,     /* warning */
  NETWIB_PRIV_NOTIFYTYPE_DEBUGBT,     /* debug with backtrace */
  NETWIB_PRIV_NOTIFYTYPE_DEBUG        /* debug */
} netwib_priv_notifytype;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_notify_string(netwib_priv_notifytype type,
                                     netwib_conststring msg);
netwib_err netwib_priv_notify_fmt(netwib_priv_notifytype type,
                                  netwib_conststring fmt,
                                  ...);
netwib_err netwib_priv_notify_err(netwib_priv_notifytype type,
                                  netwib_err error);
