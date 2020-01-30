#include "netwibtest.h"

/*-------------------------------------------------------------*/
#define NETWIB_SOCK_PORT 65231

/*-------------------------------------------------------------*/
static netwib_err sock_ser(netwib_io *pio)
{
  netwib_buf buf;
  netwib_ip localip, remoteip;
  netwib_port localport, remoteport;
  netwib_err ret;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_fmt_display("S : before read1\n"));
  ret = netwib_io_read(pio, &buf);
  if (ret == NETWIB_ERR_DATANOTAVAIL) {
    /* TCP server */
    netwib_er(netwib_io_read(pio, &buf));
  } else if (ret != NETWIB_ERR_OK) {
    return(ret);
  }
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));

  netwib_er(netwib_sock_ctl_get_local(pio, &localip, &localport));
  netwib_er(netwib_sock_ctl_get_remote(pio, &remoteip, &remoteport));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_fmt_display("S : before write1\n"));
  netwib_er(netwib_buf_append_fmt(&buf, "S : Welcome on %{ip}:%{port} : you are %{ip}:%{port}\n", &localip, localport, &remoteip, remoteport));
  netwib_er(netwib_io_write(pio, &buf));

  netwib_er(netwib_fmt_display("S : before read2\n"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_io_read(pio, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "C : msg\n"));

  netwib_er(netwib_fmt_display("S : before write2\n"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_string("S : msg\n", &buf));
  netwib_er(netwib_io_write(pio, &buf));

  netwib_er(netwib_fmt_display("S : before read3\n"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_io_read(pio, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "C : Bye\n"));

  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err sock_cli(netwib_io *pio)
{
  netwib_buf buf;
  netwib_ip localip, remoteip;
  netwib_port localport, remoteport;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_sock_ctl_get_local(pio, &localip, &localport));
  netwib_er(netwib_sock_ctl_get_remote(pio, &remoteip, &remoteport));

  netwib_er(netwib_fmt_display("C : before write1\n"));
  netwib_er(netwib_buf_append_fmt(&buf, "C : I am %{ip}:%{port} : you are %{ip}:%{port}\n", &localip, localport, &remoteip, remoteport));
  netwib_er(netwib_io_write(pio, &buf));

  netwib_er(netwib_fmt_display("C : before read1\n"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_io_read(pio, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));

  netwib_er(netwib_fmt_display("C : before write2\n"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_string("C : msg\n", &buf));
  netwib_er(netwib_io_write(pio, &buf));

  netwib_er(netwib_fmt_display("C : before read2\n"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_io_read(pio, &buf));
  netwib_er(netwibtest_buf_content2(&buf, "S : msg\n"));

  netwib_er(netwib_fmt_display("C : before write3\n"));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_string("C : Bye\n", &buf));
  netwib_er(netwib_io_write(pio, &buf));

  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
typedef struct {
  netwib_uint32 type;
  netwib_thread_cond *pthcond;
} sock_thread_ser_info;
static netwib_err sock_thread_ser(netwib_ptr infosin,
                                  netwib_ptr *pinfosout)
{
  netwib_io *pio=NULL;
  sock_thread_ser_info *pinfo = (sock_thread_ser_info *)infosin;

  switch(pinfo->type) {
  case 1:
    netwib_er(netwib_io_init_sock_udp_ser_easy(NETWIB_IPTYPE_IP4,
                                               NETWIB_SOCK_PORT, &pio));
    netwib_er(netwib_thread_cond_broadcast(pinfo->pthcond, 0));
    break;
  case 2:
    netwib_er(netwib_thread_cond_broadcast(pinfo->pthcond, 100));
    netwib_er(netwib_io_init_sock_tcp_ser_easy(NETWIB_IPTYPE_IP4,
                                               NETWIB_SOCK_PORT, &pio));
    break;
  case 3:
    netwib_er(netwib_io_init_sock_udp_mulser_easy(NETWIB_IPTYPE_IP4,
                                                  NETWIB_SOCK_PORT, &pio));
    netwib_er(netwib_thread_cond_broadcast(pinfo->pthcond, 0));
    break;
  }

  netwib_er(sock_ser(pio));
  netwib_er(netwib_io_close(&pio));
  infosin = infosin; /* for compiler warning */
  pinfosout = pinfosout; /* for compiler warning */
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err sock_thread_cli(netwib_uint32 type)
{
  sock_thread_ser_info info;
  netwib_thread *pthread;
  netwib_uint32 sleeptime;
  netwib_io *pio=NULL;
  netwib_ip ipa;
  netwib_err ret;

  info.type = type;
  netwib_er(netwib_thread_cond_init(&info.pthcond));
  netwib_er(netwib_thread_init(&sock_thread_ser, &info, &pthread));
  netwib_er(netwib_thread_cond_wait(info.pthcond, NETWIB_TIME_INFINITE,
                                    NULL, &sleeptime));
  netwib_er(netwib_thread_cond_close(&info.pthcond));
  if (sleeptime) {
    netwib_er(netwib_time_sleep_msec(sleeptime));
  }

  netwib_er(netwib_ip_init_ip4(0x7F000001, &ipa));
  switch(type) {
  case 1:
    netwib_er(netwib_io_init_sock_udp_cli_easy(&ipa, NETWIB_SOCK_PORT, &pio));
    break;
  case 2:
    netwib_er(netwib_io_init_sock_tcp_cli_easy(&ipa, NETWIB_SOCK_PORT, &pio));
    break;
  case 3:
    netwib_er(netwib_io_init_sock_udp_cli_easy(&ipa, NETWIB_SOCK_PORT, &pio));
    break;
  }

  netwib_er(sock_cli(pio));
  netwib_er(netwib_io_close(&pio));

  netwib_er(netwib_thread_wait(pthread, NETWIB_TIME_INFINITE, NULL, &ret, NULL));
  netwib_er(ret);
  netwib_er(netwib_thread_close(&pthread));

  return(NETWIB_ERR_OK);
}


/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_sock_udp_cli_full_test(void)
{
  puts("++++----\nTest of netwib_io_init_sock_udp_cli_full");
  puts("Redundant with netwib_io_init_sock_udp_ser_full");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_sock_tcp_cli_full_test(void)
{
  puts("++++----\nTest of netwib_io_init_sock_tcp_cli_full");
  puts("Redundant with netwib_io_init_sock_tcp_ser_full");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_sock_udp_ser_full_test(void)
{
  puts("++++----\nTest of netwib_io_init_sock_udp_ser_full");
  netwib_er(sock_thread_cli(1));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_sock_udp_ser_easy_test(void)
{
  puts("++++----\nTest of netwib_io_init_sock_udp_ser_easy");
  puts("Redundant with netwib_io_init_sock_udp_ser");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_sock_tcp_ser_full_test(void)
{
  puts("++++----\nTest of netwib_io_init_sock_tcp_ser_full");
  netwib_er(sock_thread_cli(2));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_sock_tcp_ser_easy_test(void)
{
  puts("++++----\nTest of netwib_io_init_sock_tcp_ser_easy");
  puts("Redundant with netwib_io_init_sock_tcp_ser");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_sock_udp_mulser_full_test(void)
{
  puts("++++----\nTest of netwib_io_init_sock_udp_mulser_full");
  netwib_er(sock_thread_cli(3));
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_io_init_sock_udp_mulser_easy_test(void)
{
  puts("++++----\nTest of netwib_io_init_sock_udp_mulser_easy");
  puts("Redundant with netwib_io_init_sock_udp_mulser");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err sock_tms(netwib_io *pio,
                           netwib_ptr pinfos)
{
  netwib_buf buf;
  netwib_ip localip, remoteip;
  netwib_port localport, remoteport;

  netwib_er(netwib_buf_init_mallocdefault(&buf));

  netwib_er(netwib_sock_ctl_get_local(pio, &localip, &localport));
  netwib_er(netwib_sock_ctl_get_remote(pio, &remoteip, &remoteport));
  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_fmt(&buf, "Welcome on %{ip}:%{port} (you are %{ip}:%{port}). Please enter a line\n", &localip, localport, &remoteip, remoteport));
  netwib_er(netwib_io_write(pio, &buf));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_io_read(pio, &buf));
  netwib_er(netwib_buf_display(&buf, NETWIB_ENCODETYPE_DATA));

  netwib__buf_reinit(&buf);
  netwib_er(netwib_buf_append_string("Bye\n", &buf));
  netwib_er(netwib_io_write(pio, &buf));

  netwib_er(netwib_buf_close(&buf));
  return(NETWIB_ERR_OK);
  pinfos = pinfos;
}
static netwib_err netwib_sock_tcp_mulser_full_test(void)
{
  puts("++++----\nTest of netwib_sock_tcp_mulser_full");
  puts("Not tested : local interaction is needed. You can enable it by hand");
  return(NETWIB_ERR_OK);

  netwib_er(netwib_sock_tcp_mulser_easy(NETWIB_IPTYPE_IP4,
                                        NETWIB_SOCK_PORT, &sock_tms, NULL));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_sock_tcp_mulser_easy_test(void)
{
  puts("++++----\nTest of netwib_sock_tcp_mulser_easy");
  puts("Redundant with netwib_sock_tcp_mulser");
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/
netwib_err netwibtest_net_sock(void)
{
  puts("###########################################\nTest of net/sock");

  netwib_er(netwib_io_init_sock_udp_cli_full_test());
  netwib_er(netwib_io_init_sock_tcp_cli_full_test());

  netwib_er(netwib_io_init_sock_tcp_ser_full_test());
  netwib_er(netwib_io_init_sock_tcp_ser_easy_test());

  netwib_er(netwib_io_init_sock_udp_ser_full_test());
  netwib_er(netwib_io_init_sock_udp_ser_easy_test());

  netwib_er(netwib_io_init_sock_udp_mulser_full_test());
  netwib_er(netwib_io_init_sock_udp_mulser_easy_test());

  netwib_er(netwib_sock_tcp_mulser_full_test());
  netwib_er(netwib_sock_tcp_mulser_easy_test());

  return(NETWIB_ERR_OK);
}
