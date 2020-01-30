
/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_PRIV_KBDTYPE_UNKNOWN = 0,
  NETWIB_PRIV_KBDTYPE_CONSOLE,
  NETWIB_PRIV_KBDTYPE_FILE,
  NETWIB_PRIV_KBDTYPE_PIPE
} netwib_priv_kbdtype;

/*-------------------------------------------------------------*/
typedef struct {
  int fd;
  NETWIBHANDLE h;
  netwib_priv_kbdtype type;
  netwib_bool consoleecho;
  netwib_bool consolereadbyline;
  netwib_bool availdata;
  netwib_bool originalconsoleecho;
  netwib_bool originalconsolereadbyline;
} netwib_priv_kbd;

/*-------------------------------------------------------------*/
netwib_err netwib_priv_kbd_init_fd(int fd,
                                   netwib_priv_kbd *pkbd);
netwib_err netwib_priv_kbd_init_handle(NETWIBHANDLE h,
                                       netwib_priv_kbd *pkbd);
netwib_err netwib_priv_kbd_initdefault(netwib_priv_kbd *pkbd);
netwib_err netwib_priv_kbd_close(netwib_priv_kbd *pkbd);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_kbd_ctl_set_purge(netwib_priv_kbd *pkbd);
netwib_err netwib_priv_kbd_ctl_set_echoline(netwib_priv_kbd *pkbd,
                                            netwib_bool consoleecho,
                                            netwib_bool consolereadbyline);
/* no need for ctl_get_echoline, because they are stored in pkbd */

/*-------------------------------------------------------------*/
/* will internally change and restore echo/line */
netwib_err netwib_priv_kbd_press(netwib_priv_kbd *pkbd,
                                 netwib_char *pc);

/*-------------------------------------------------------------*/
/* ctl_set_echoline(x,false) has to be called before (once) */
netwib_err netwib_priv_kbd_read_key(netwib_priv_kbd *pkbd,
                                    netwib_char *pc);

/*-------------------------------------------------------------*/
/* ctl_set_echoline(x,true) has to be called before (once) */
netwib_err netwib_priv_kbd_read_line(netwib_priv_kbd *pkbd,
                                     netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_kbd_wait(netwib_priv_kbd *pkbd,
                                netwib_consttime *pabstime,
                                netwib_bool *pevent);
netwib_err netwib_priv_kbd_read(netwib_priv_kbd *pkbd,
                                netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_kbd_buf_append(netwib_constbuf *pmessage,
                                      netwib_constbuf *pdefaulttext,
                                      netwib_bool echokeys,
                                      netwib_char prompt,
                                      netwib_bool askifempty,
                                      netwib_buf *pbuf);
