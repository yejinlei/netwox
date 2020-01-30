
/*-------------------------------------------------------------*/
/* Unfortunately (Linux) :
     sizeof(struct sockaddr)==16 < sizeof(struct sockaddr_in6)==24
   So, a sockaddr cannot be used to store an IPv6 address.
   We define netwib_priv_sockaddr_... as an opaque structure sufficiently
   big to store every kind of address.
   Note : RFC2553 proposes struct sockaddr_storage, but it does
   not exists everywhere.
*/
/*
   Normally, the sockaddr starts with a 16 bits integer, which
   should be aligned.
   However, when retrieving configuration (BSD), the structure
   may start unaligned. So, our functions have to:
    - accept unaligned structures on input
    - create aligned structure on output (for example, this is
      needed for send() sockaddr parameter)
   Note: to prove this, compile under Solaris + Sparc ("cast increases
   required alignment of target type" gcc error).
*/
typedef struct {
  /* we have to be tolerant for what we receive, because it can
     be unaligned, so this is similar to this structure */
  netwib_byte donotuse[32];
} netwib_priv_sockaddr_unalign;
typedef struct {
  /* we have to be strict for what we give, because it has to be
     aligned, so this is similar to this structure */
  netwib_uintptr donotuse[8];
} netwib_priv_sockaddr_align;
/* for conversions, we have to use:
     unalign to align: netwib_c_memcpy()
     align to unalign: cast (netwib_c_memcpy() is not needed)
 */
/* which one to use ?:
     - netwib storage: netwib_priv_sockaddr_align
     - pointer to data coming from outside: netwib_priv_sockaddr_unalign
*/

/*-------------------------------------------------------------*/
/* for socklen, we have to use netwib_priv_sockaddr_len. Using an uint32
   leads to problems because socklen_t is defined as 64 bit on Tru64 Unix. */

/*-------------------------------------------------------------*/
netwib_err netwib_priv_sa_ipport_init_sali(const netwib_priv_sockaddr_unalign *psa,
                                           netwib_priv_sockaddr_len supposedsalen,
                                           netwib_iptype supposediptype,
                                           netwib_ip *pip,
                                           netwib_port *pport);
#define netwib_priv_sa_ipport_init_sal(psa,supposedsalen,pip,pport) netwib_priv_sa_ipport_init_sali(psa,supposedsalen,NETWIB_IPTYPE_UNKNOWN,pip,pport)
#define netwib_priv_sa_ipport_init_sai(psa,supposediptype,pip,pport) netwib_priv_sa_ipport_init_sali(psa,0xFFFFFFFFu,supposediptype,pip,pport)
#define netwib_priv_sa_ipport_init_sa(psa,pip,pport) netwib_priv_sa_ipport_init_sali(psa,0xFFFFFFFFu,NETWIB_IPTYPE_UNKNOWN,pip,pport)
#define netwib_priv_sa_ip_init_sali(psa,supposedsalen,supposediptype,pip) netwib_priv_sa_ipport_init_sali(psa,supposedsalen,supposediptype,pip,NULL)
#define netwib_priv_sa_ip_init_sal(psa,supposedsalen,pip) netwib_priv_sa_ipport_init_sal(psa,supposedsalen,pip,NULL)
#define netwib_priv_sa_ip_init_sai(psa,supposediptype,pip) netwib_priv_sa_ipport_init_sai(psa,supposediptype,pip,NULL)
#define netwib_priv_sa_ip_init_sa(psa,pip) netwib_priv_sa_ipport_init_sa(psa,pip,NULL)
/* We could also define a function working on netwib_priv_sockaddr_align.
   However, it would only support _align, but unsupport _unalign,
   so a simple mistake/mix would lead to a crash.
   Having one function supporting both structures is safer (in case
   of mix, we only have a gcc warning). The drawback, is that we have
   to use a cast to convert a _align to a _unalign, to suppress this warning.
   Note about the mix: normally, a mix is detected immediately, however
   functions in this module are included in a lot of #if #endif, so to
   test all cases, we have to install netwib on a lot of computers.
*/

/*-------------------------------------------------------------*/
/* note: if pip!=NULL, pip->iptype overwrites iptype */
netwib_err netwib_priv_sa_sal_init_iptport(netwib_constip *pip,
                                           netwib_iptype iptype,
                                           netwib_port port,
                                           netwib_priv_sockaddr_align *psa,
                                           netwib_priv_sockaddr_len *psalen);
/* use IPv4 by default to overwrite if pip is NULL */
#define netwib_priv_sa_sal_init_ipport(pip,port,psa,psalen) netwib_priv_sa_sal_init_iptport(pip,NETWIB_IPTYPE_IP4,port,psa,psalen)
#define netwib_priv_sa_sa_init_ip(pip,psa) netwib_priv_sa_sal_init_ipport(pip,0,psa,NULL)

/*-------------------------------------------------------------*/
netwib_err netwib_priv_sa_send(int fd,
                               netwib_constbuf *pbuf);
netwib_err netwib_priv_sa_sendto(int fd,
                                 netwib_constbuf *pbuf,
                                 const netwib_priv_sockaddr_unalign *psa,
                                 netwib_priv_sockaddr_len salen);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_sa_recv(int fd,
                               netwib_buf *pbuf);
netwib_err netwib_priv_sa_recvfrom(int fd,
                                   netwib_buf *pbuf,
                                   netwib_priv_sockaddr_align *psa,
                                   netwib_priv_sockaddr_len *psalen);
