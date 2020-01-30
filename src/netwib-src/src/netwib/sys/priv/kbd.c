/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_kbd_init_fd(int fd,
                                   netwib_priv_kbd *pkbd)
#if defined NETWIBDEF_SYSNAME_Unix
{
  struct termios thetermios;

  pkbd->fd = fd;

  /* check if it's a tty */
  pkbd->type = NETWIB_PRIV_KBDTYPE_UNKNOWN;
  pkbd->consoleecho = NETWIB_FALSE;
  pkbd->consolereadbyline = NETWIB_FALSE;
  if (isatty(fd)) {
    pkbd->type = NETWIB_PRIV_KBDTYPE_CONSOLE;
    /* check if characters are echoed */
    if (tcgetattr(fd, &thetermios)) {
      return(NETWIB_ERR_FUTCGETATTR);
    }
    if (thetermios.c_lflag & ECHO) {
      pkbd->consoleecho = NETWIB_TRUE;
    }
    if (thetermios.c_lflag & ICANON) {
      pkbd->consolereadbyline = NETWIB_TRUE;
    }
  }

  pkbd->originalconsoleecho = pkbd->consoleecho;
  pkbd->originalconsolereadbyline = pkbd->consolereadbyline;

  pkbd->availdata = NETWIB_FALSE;

  return(NETWIB_ERR_OK);
}
#elif defined NETWIBDEF_SYSNAME_Windows
{
  pkbd = pkbd; /* for compiler warning */
  fd = fd; /* for compiler warning */
  return(NETWIB_ERR_LONOTIMPLEMENTED);
}
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif
/*-------------------------------------------------------------*/
netwib_err netwib_priv_kbd_init_handle(NETWIBHANDLE h,
                                       netwib_priv_kbd *pkbd)
#if defined NETWIBDEF_SYSNAME_Unix
{
  pkbd = pkbd; /* for compiler warning */
  h = h; /* for compiler warning */
  return(NETWIB_ERR_LONOTIMPLEMENTED);
}
#elif defined NETWIBDEF_SYSNAME_Windows
{
  DWORD ftype, conmode;
  BOOL err;

  pkbd->h = h;

  /* check if it's a tty */
  pkbd->type = NETWIB_PRIV_KBDTYPE_UNKNOWN;
  ftype = GetFileType(h);
  if (ftype == FILE_TYPE_CHAR) {
    pkbd->type = NETWIB_PRIV_KBDTYPE_CONSOLE;
  } else if (ftype == FILE_TYPE_DISK) {
    pkbd->type = NETWIB_PRIV_KBDTYPE_FILE;
  } else if (ftype == FILE_TYPE_PIPE) {
    pkbd->type = NETWIB_PRIV_KBDTYPE_PIPE;
  }

  pkbd->availdata = NETWIB_FALSE;

  /* check if characters are echoed */
  pkbd->consoleecho = NETWIB_TRUE;
  if (pkbd->type == NETWIB_PRIV_KBDTYPE_CONSOLE) {
    err = GetConsoleMode(h, &conmode);
    if (!err) return(NETWIB_ERR_FUGETCONSOLEMODE);
    if (!(conmode & ENABLE_ECHO_INPUT)) {
      pkbd->consoleecho = NETWIB_FALSE;
    }
  }

  /* default behavior is to read line by line */
  pkbd->consolereadbyline = NETWIB_TRUE;

  pkbd->originalconsoleecho = pkbd->consoleecho;
  pkbd->originalconsolereadbyline = pkbd->consolereadbyline;

  return(NETWIB_ERR_OK);
}
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_kbd_initdefault(netwib_priv_kbd *pkbd)
{
#if defined NETWIBDEF_SYSNAME_Unix
  return(netwib_priv_kbd_init_fd(0, pkbd));
#elif defined NETWIBDEF_SYSNAME_Windows
  return(netwib_priv_kbd_init_handle(GetStdHandle(STD_INPUT_HANDLE), pkbd));
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_kbd_close(netwib_priv_kbd *pkbd)
{
  netwib_er(netwib_priv_kbd_ctl_set_echoline(pkbd,
                                             pkbd->originalconsoleecho,
                                             pkbd->originalconsolereadbyline));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_kbd_ctl_set_purge(netwib_priv_kbd *pkbd)
{
  if (pkbd->type == NETWIB_PRIV_KBDTYPE_CONSOLE) {
#if defined NETWIBDEF_SYSNAME_Unix
    struct termios old_termios;
    if (tcgetattr(pkbd->fd, &old_termios)) {
      return(NETWIB_ERR_FUTCGETATTR);
    }
    /* we use TCSAFLUSH which discards all input */
    if (tcsetattr(pkbd->fd, TCSAFLUSH, &old_termios)) {
      return(NETWIB_ERR_FUTCSETATTR);
    }
#if 0
    /* this works under Linux, Solaris, but not FreeBSD or OpenBSD (it
       perturbs next calls) */
    /* Note for next implementation : we can also use tcflush() */
    struct termios old_termios, new_termios;
    int c;

    /* ensure we read char by char (instead of line by line) */
    if (tcgetattr(pkbd->fd, &old_termios)) {
      return(NETWIB_ERR_FUTCGETATTR);
    }
    new_termios = old_termios;
    new_termios.c_lflag &= (~ICANON);
    new_termios.c_cc[VMIN] = 0;
    new_termios.c_cc[VTIME] = 0;
    if (tcsetattr(pkbd->fd, TCSANOW, &new_termios)) {
      return(NETWIB_ERR_FUTCSETATTR);
    }

    do {
      c = getchar();
    } while (c != EOF);

    /* reset */
    if (tcsetattr(pkbd->fd, TCSANOW, &old_termios)) {
      return(NETWIB_ERR_FUTCSETATTR);
    }
#endif
#elif defined NETWIBDEF_SYSNAME_Windows
    while (_kbhit()) {
      _getch();
    }
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif
  }

  pkbd->availdata = NETWIB_FALSE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_kbd_ctl_set_echoline(netwib_priv_kbd *pkbd,
                                            netwib_bool consoleecho,
                                            netwib_bool consolereadbyline)
#if defined NETWIBDEF_SYSNAME_Unix
{
  struct termios thetermios;

  /* change mode */
  if (pkbd->type == NETWIB_PRIV_KBDTYPE_CONSOLE) {
    if (tcgetattr(pkbd->fd, &thetermios)) {
      return(NETWIB_ERR_FUTCGETATTR);
    }
    if (consoleecho) {
      thetermios.c_lflag |= ECHO;
    } else {
      thetermios.c_lflag &= ~ECHO;
    }
    if (consolereadbyline) {
      thetermios.c_lflag |= ICANON;
    } else {
      thetermios.c_lflag &= (~ICANON);
      thetermios.c_cc[VMIN] = 1;   /* need one char */
      thetermios.c_cc[VTIME] = 0;  /* wait forever */
    }
    if (tcsetattr(pkbd->fd, TCSANOW, &thetermios)) {
      return(NETWIB_ERR_FUTCSETATTR);
    }
  }

  /* store them */
  pkbd->consoleecho = consoleecho;
  pkbd->consolereadbyline = consolereadbyline;

  return(NETWIB_ERR_OK);
}
#elif defined NETWIBDEF_SYSNAME_Windows
{
  /* we cannot change. We'll then use getch/getche */

  /* store them */
  pkbd->consoleecho = consoleecho;
  pkbd->consolereadbyline = consolereadbyline;

  return(NETWIB_ERR_OK);
}
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_kbd_press(netwib_priv_kbd *pkbd,
                                 netwib_char *pc)
#if defined NETWIBDEF_SYSNAME_Unix
{
  netwib_bool consoleecho, consolereadbyline;

  netwib_er(netwib_priv_kbd_ctl_set_purge(pkbd));
  consoleecho = pkbd->consoleecho;
  consolereadbyline = pkbd->consolereadbyline;
  netwib_er(netwib_priv_kbd_ctl_set_echoline(pkbd, NETWIB_FALSE,
                                             NETWIB_FALSE));
  netwib_er(netwib_priv_kbd_read_key(pkbd, pc));
  netwib_er(netwib_priv_kbd_ctl_set_echoline(pkbd, consoleecho,
                                             consolereadbyline));

  return(NETWIB_ERR_OK);
}
#elif defined NETWIBDEF_SYSNAME_Windows
{
  netwib_byte c;
  netwib_err ret;
  BOOL err;

  netwib_er(netwib_priv_kbd_ctl_set_purge(pkbd));

  /* cases which cannot be treated here */
  if (pkbd->type != NETWIB_PRIV_KBDTYPE_CONSOLE) {
    ret = netwib_priv_kbd_read_key(pkbd, pc);
    return(ret);
  }

  c = (netwib_byte) _getch();
  if (c == 0x00) {
    /* on Function-i, get the second part */
    _getch();
  } else if (c == 0xE0) {
    /* arrow key, but get the second part */
    _getch();
  } else if (c == 0x03) {
    /* on control-c, generate the event (then the program should exit) */
    err = GenerateConsoleCtrlEvent(CTRL_C_EVENT, 0);
    if (!err) {
      return(NETWIB_ERR_FUGENERATECONSOLECTRLEVENT);
    }
  }
  if (pc != NULL) *pc = c;

  return(NETWIB_ERR_OK);
}
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_kbd_read_key_pipe(netwib_priv_kbd *pkbd,
                                                netwib_char *pc)
{
#if defined NETWIBDEF_SYSNAME_Unix
  return(netwib_priv_fd_read_byte(pkbd->fd, (netwib_byte*)pc));
#elif defined NETWIBDEF_SYSNAME_Windows
  return(netwib_priv_handle_read_byte(pkbd->h, (netwib_byte*)pc));
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_kbd_read_key(netwib_priv_kbd *pkbd,
                                    netwib_char *pc)
#if defined NETWIBDEF_SYSNAME_Unix
{
  netwib_char c;

  /* cases which cannot be treated here */
  if (pkbd->type != NETWIB_PRIV_KBDTYPE_CONSOLE || pkbd->fd != 0) {
    return(netwib_priv_kbd_read_key_pipe(pkbd, pc));
  }

  /* read */
  c = (netwib_char)fgetc(stdin);

  if (pc != NULL) *pc = c;
  return(NETWIB_ERR_OK);
}
#elif defined NETWIBDEF_SYSNAME_Windows
{
  netwib_byte c;
  BOOL err;

  /* cases which cannot be treated here */
  if (pkbd->type != NETWIB_PRIV_KBDTYPE_CONSOLE) {
    return(netwib_priv_kbd_read_key_pipe(pkbd, pc));
  }

  c = 0;
  while (NETWIB_TRUE) {
    c = (netwib_byte)_getch();
    if (c == 0x08) {
      /* ignore backspace */
    } else if (c == 0x00) {
      /* ignore Function-i, but get the second part */
      _getch();
    } else if (c == 0xE0) {
      /* ignore arrow, but get the second part */
      _getch();
    } else if (c == 0x03) {
      /* on control-c, generate the event (then the program should exit) */
      err = GenerateConsoleCtrlEvent(CTRL_C_EVENT, 0);
      if (!err) {
        return(NETWIB_ERR_FUGENERATECONSOLECTRLEVENT);
      }
    } else {
      /* ok */
      break;
    }
  }

  if (pkbd->consoleecho) {
    /* display pressed key */
    if (c == 0x0D) {
      /* because the newline is suppressed */
      putchar(0x0D);
      putchar(0x0A);
    } else {
      putchar(c);
    }
  }

  if (pc != NULL) *pc = c;
  return(NETWIB_ERR_OK);
}
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_kbd_read_line_pipe(netwib_priv_kbd *pkbd,
                                                 netwib_buf *pbuf)
{
  netwib_char c;

  while(NETWIB_TRUE) {
    netwib_er(netwib_priv_kbd_read_key(pkbd, &c));
    if (c == '\n') break;
    netwib_er(netwib_buf_append_byte(c, pbuf));
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_kbd_read_line(netwib_priv_kbd *pkbd,
                                     netwib_buf *pbuf)
#if defined NETWIBDEF_SYSNAME_Unix
{
  int c;
  netwib_err ret;

  /* cases which cannot be treated here */
  if (pkbd->type != NETWIB_PRIV_KBDTYPE_CONSOLE || pkbd->fd != 0) {
    return(netwib_priv_kbd_read_line_pipe(pkbd, pbuf));
  }

  ret = NETWIB_ERR_OK;
  while (NETWIB_TRUE) {
    c = getc(stdin);
    if (c == 0x0D || c == 0x0A || c == EOF) {
      break;
    }
    ret = netwib_buf_append_byte(c, pbuf);
    if (ret != NETWIB_ERR_OK) {
      break;
    }
  }
  if (!pkbd->consoleecho) {
    puts("");
  }

  return(NETWIB_ERR_OK);
}
#elif defined NETWIBDEF_SYSNAME_Windows
{
  netwib_uint32 userstringsize;
  int c;
  BOOL err;

  /* cases which cannot be treated here */
  if (pkbd->type != NETWIB_PRIV_KBDTYPE_CONSOLE) {
    return(netwib_priv_kbd_read_line_pipe(pkbd, pbuf));
  }

  userstringsize = 0;
  while (NETWIB_TRUE) {
    /* cannot use getche because backspace can erase data
       printed before calling the function */
    c = _getch();
    if (c == 0x0D || c == 0x0A) {
      if (pkbd->consoleecho) {
        putchar(0x0D);
        putchar(0x0A);
      }
      break;
    } else if (c == 0x08) {
      /* backspace */
      if (userstringsize) {
        userstringsize--;
        pbuf->endoffset--;
        if (pkbd->consoleecho) {
          putchar(0x08);
          putchar(' ');
          putchar(0x08);
        }
      }
    } else if (c == 0x00) {
      /* Function-i ignore, but get the second part */
      _getch();
    } else if (c == 0xE0) {
      /* ignore arrow, but get the second part */
      _getch();
    } else if (c == 0x03) {
      /* on control-c, generate the event (then the program should exit) */
      err = GenerateConsoleCtrlEvent(CTRL_C_EVENT, 0);
      if (!err) {
        return(NETWIB_ERR_FUGENERATECONSOLECTRLEVENT);
      }
    } else {
      netwib_er(netwib_buf_append_byte((netwib_char)c, pbuf));
      userstringsize++;
      if (pkbd->consoleecho) {
        putchar(c);
      }
    }
  }
  if (!pkbd->consoleecho) {
    puts("");
  }

  return(NETWIB_ERR_OK);
}
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_kbd_wait(netwib_priv_kbd *pkbd,
                                netwib_consttime *pabstime,
                                netwib_bool *pevent)
#if defined NETWIBDEF_SYSNAME_Unix
{
  netwib_bool event;

  /* unfortunately, Linux does not detect a line twice, so we have
     to keep state. This state is reset when we purge or read. */
  if (pkbd->availdata) {
    if (pevent != NULL) *pevent = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  /* wait */
  netwib_er(netwib_priv_fd_wait(pkbd->fd, NETWIB_IO_WAYTYPE_READ,
                                pabstime, &event));

  /* save state */
  if (event) {
    pkbd->availdata = NETWIB_TRUE;
  }

  if (pevent != NULL) *pevent = event;
  return(NETWIB_ERR_OK);
}
#elif defined NETWIBDEF_SYSNAME_Windows
{
  netwib_handletype handletype;
  netwib_bool event;

  switch(pkbd->type) {
    case NETWIB_PRIV_KBDTYPE_CONSOLE : handletype = NETWIB_HANDLETYPE_CONSOLE; break;
    case NETWIB_PRIV_KBDTYPE_FILE : handletype = NETWIB_HANDLETYPE_FILE; break;
    case NETWIB_PRIV_KBDTYPE_PIPE : handletype = NETWIB_HANDLETYPE_PIPE; break;
    default : handletype = NETWIB_HANDLETYPE_UNKNOWN; break;
  }

  netwib_er(netwib_priv_handle_wait(pkbd->h, handletype,
                                    NETWIB_IO_WAYTYPE_READ,
                                    pabstime, &event));
  /* waiting on a console handle does not ensure we have an event
     because we use PeekConsoleInput to recheck */
  if (!event) {
    return(NETWIB_ERR_PLEASELOOPTIME);
  }

  if (pevent != NULL) *pevent = NETWIB_TRUE;
  return(NETWIB_ERR_OK);
}
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_kbd_read(netwib_priv_kbd *pkbd,
                                netwib_buf *pbuf)
#if defined NETWIBDEF_SYSNAME_Unix
{
  netwib_char c;

  /* reset state */
  pkbd->availdata = NETWIB_FALSE;

  if (pkbd->consolereadbyline) {
    netwib_er(netwib_priv_kbd_read_line(pkbd, pbuf));
  } else {
    netwib_er(netwib_priv_kbd_read_key(pkbd, &c));
    netwib_er(netwib_buf_append_byte(c, pbuf));
  }

  return(NETWIB_ERR_OK);
}
#elif defined NETWIBDEF_SYSNAME_Windows
{
  netwib_char c;

  if (pkbd->consolereadbyline) {
    netwib_er(netwib_priv_kbd_read_line(pkbd, pbuf));
  } else {
    netwib_er(netwib_priv_kbd_read_key(pkbd, &c));
    netwib_er(netwib_buf_append_byte(c, pbuf));
  }

  return(NETWIB_ERR_OK);
}
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_kbd_buf_append(netwib_constbuf *pmessage,
                                      netwib_constbuf *pdefaulttext,
                                      netwib_bool echokeys,
                                      netwib_char prompt,
                                      netwib_bool askifempty,
                                      netwib_buf *pbuf)
{
  netwib_buf buf, cmsg, call;
  netwib_string bufref;
  netwib_priv_kbd kbd;
  netwib_bool havemessage, havedefaulttext;
  netwib_char c;
  netwib_err ret;

  /* obtain strings */
  havemessage = NETWIB_FALSE;
  if (netwib__buf_ref_data_sizenull(pmessage) != 0) {
    havemessage = NETWIB_TRUE;
  }
  havedefaulttext = NETWIB_FALSE;
  if (netwib__buf_ref_data_sizenull(pdefaulttext) != 0) {
    havedefaulttext = NETWIB_TRUE;
  }

  /* display message */
  if (havemessage) {
    if (echokeys && havedefaulttext) {
      netwib_er(netwib_fmt_display("%{buf} [%{buf}]%c ", pmessage,
                                   pdefaulttext, prompt));
    } else {
      netwib_er(netwib_fmt_display("%{buf}%c ", pmessage, prompt));
    }
  }

  /* get user string */
  netwib_er(netwib_buf_init_mallocdefault(&buf));
  netwib_er(netwib_priv_kbd_initdefault(&kbd));
  netwib_er(netwib_priv_kbd_ctl_set_echoline(&kbd, echokeys, NETWIB_TRUE));
  netwib_er(netwib_priv_kbd_read_line(&kbd, &buf));
  netwib_er(netwib_priv_kbd_close(&kbd));

  /* user entered something */
  netwib_er(netwib_buf_ref_string(&buf, &bufref));
  if (bufref[0] != '\0') {
    ret = netwib_buf_append_buf(&buf, pbuf);
    netwib_er(netwib_buf_close(&buf));
    return(ret);
  }
  netwib_er(netwib_buf_close(&buf));

  /* user entered nothing and there is no default : nothing to append */
  if (!havedefaulttext) {
    return(NETWIB_ERR_OK);
  }

  /* ask user */
  if (askifempty) {
    netwib_er(netwib_buf_init_ext_string("Do you want an Empty string or the Default string ?", &cmsg));
    netwib_er(netwib_buf_init_ext_string("eEdD", &call));
    netwib_er(netwib_char_init_kbd(&cmsg, &call, 'd', &c));
    if (c == 'd' || c == 'D') {
      netwib_er(netwib_buf_append_buf(pdefaulttext, pbuf));
    }
  }

  return(NETWIB_ERR_OK);
}
