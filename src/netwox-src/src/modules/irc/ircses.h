
/*-------------------------------------------------------------*/
typedef struct {
  netwib_io *pio;
} netwox_ircses;

/*-------------------------------------------------------------*/
netwib_err netwox_ircses_init(netwib_io *pio,
                              netwib_constbuf *ppassword,
                              netwib_constbuf *pnickname,
                              netwib_constbuf *pusername,
                              netwib_constbuf *prealname,
                              netwox_ircses *pircses);

/*-------------------------------------------------------------*/
netwib_err netwox_ircses_close(netwox_ircses *pircses);

/*-------------------------------------------------------------*/
netwib_err netwox_ircses_write(netwox_ircses *pircses,
                               netwib_constbuf *pbuf);

/*-------------------------------------------------------------*/
/* ":prefix command parameter :trailing" */
netwib_err netwox_ircses_write_cmd(netwox_ircses *pircses,
                                   netwib_constbuf *pprefix,
                                   netwib_conststring command,
                                   netwib_constbuf *pparameter,
                                   netwib_constbuf *ptrailing);
#define netwox_ircses_write_cmd_cpt(pio,command,pparameter,ptrailing) netwox_ircses_write_cmd(pio,NULL,command,pparameter,ptrailing)
#define netwox_ircses_write_cmd_cp(pio,command,pparameter) netwox_ircses_write_cmd_cpt(pio,command,pparameter,NULL)
#define netwox_ircses_write_cmd_ct(pio,command,ptrailing) netwox_ircses_write_cmd(pio,NULL,command,NULL,ptrailing)
#define netwox_ircses_write_cmd_c(pio,command) netwox_ircses_write_cmd_cp(pio,command,NULL)

/*-------------------------------------------------------------*/
#define netwox_ircses_write_privmsg(pio,preceiver,ptext) netwox_ircses_write_cmd_cpt(pio,"PRIVMSG",preceiver,ptext)

/*-------------------------------------------------------------*/
#define netwox_ircses_write_pass(pio,ppassword) netwox_ircses_write_cmd_cp(pio,"PASS",ppassword)
#define netwox_ircses_write_nick(pio,pnickname) netwox_ircses_write_cmd_cp(pio,"NICK",pnickname)
#define netwox_ircses_write_pong(pio,pserver) netwox_ircses_write_cmd_cp(pio,"PONG",pserver)
#define netwox_ircses_write_quit(pio,pmsg) netwox_ircses_write_cmd_cp(pio,"QUIT",pmsg)
#define netwox_ircses_write_join(pio,pchannel) netwox_ircses_write_cmd_cp(pio,"JOIN",pchannel)
#define netwox_ircses_write_names(pio,pchannel) netwox_ircses_write_cmd_cp(pio,"NAMES",pchannel)

/*-------------------------------------------------------------*/
#define netwox_ircses_write_away(pio,pmsg) netwox_ircses_write_cmd_ct(pio,"AWAY",pmsg)

/*-------------------------------------------------------------*/
#define netwox_ircses_write_list(pio) netwox_ircses_write_cmd_c(pio,"LIST")
#define netwox_ircses_write_version(pio) netwox_ircses_write_cmd_c(pio,"VERSION")
#define netwox_ircses_write_info(pio) netwox_ircses_write_cmd_c(pio,"INFO")
#define netwox_ircses_write_users(pio) netwox_ircses_write_cmd_c(pio,"USERS")

/*-------------------------------------------------------------*/
netwib_err netwox_ircses_write_user(netwox_ircses *pircses,
                                    netwib_constbuf *pprefix,
                                    netwib_constbuf *pusername,
                                    netwib_constbuf *prealname);

/*-------------------------------------------------------------*/
typedef netwib_err (*netwox_ircses_loop_pf)(netwox_ircses *pircses,
                                            netwib_ptr infos,
                                            netwib_constbuf *pline,
                                            netwib_constbuf *pprefix,
                                            netwox_irc_cmd cmd,
                                            netwib_constbuf *pparameters,
                                            netwib_bool *pstoploop);
netwib_err netwox_ircses_loop(netwox_ircses_loop_pf pfunc,
                              netwox_ircses *pircses,
                              netwib_ptr infos);

/*-------------------------------------------------------------*/
netwib_err netwox_ircses_waitready(netwox_ircses *pircses);
