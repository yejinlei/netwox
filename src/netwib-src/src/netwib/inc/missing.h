
/*-------------------------------------------------------------*/
#if 0
/* Some systems have the function in their library, but
   prototypes are missing. Those one can be used instead. */
 int pthread_yield(void);
 int getpagesize(void);
 int usleep(unsigned int useconds);
 int getpwnam_r(const char *name,
                struct passwd *resultbuf,
                char *buffer, size_t buflen,
                struct passwd **result);
 int getgrnam_r(const char *name,
                struct group *resultbuf,
                char *buffer, size_t buflen,
                struct group **result);
 int getgrgid_r(int gid,
                struct group *resultbuf,
                char *buffer, size_t buflen,
                struct group **result);
 int ether_ntohost(char *hostname,
                   struct ether_addr *e);
 int ether_hostton(char *hostname,
                   struct ether_addr *e);
 int clock_gettime(clockid_t i,
                   struct timespec *p);
 int nanosleep(const struct timespec *p,
               struct timespec *p2);
 int gethostbyaddr_r(const char *addr,
                     size_t len,
                     int type,
                     struct hostent *result_buf,
                     char *buf,
                     size_t buflen,
                     struct hostent **result,
                     int *h_errnop);
 /* needed for a bug in FreeBSD 5.4 declarations in pthread.h */
 int pthread_rwlock_timedwrlock(pthread_rwlock_t *,
                                const struct timespec *);
#endif

