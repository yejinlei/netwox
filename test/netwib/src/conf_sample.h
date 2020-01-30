/* Those values have to be adapted to match your computer */

/*-------------------------------------------------------------*/
/* This define indicates if netwibtest will be run as administrator.
   If NETWIB_TRUE : all functions will be tested
   If NETWIB_FALSE : functions needing privileges (sniff, spoof)
                    will not be tested
*/
#define NETWIBTESTDEF_RUNASROOT NETWIB_FALSE

/*-------------------------------------------------------------*/
/* Those defines are needed in several path functions.
   You can enter up to 10 directories.
     NETWIBTESTDEF_DIR_NAME_x :
        directory name
     NETWIBTESTDEF_DIR_EXISTS_x :
        NETWIB_TRUE or NETWIB_FALSE depending if name exists
*/
#if defined NETWIBDEF_SYSNAME_Unix
 #define NETWIBTESTDEF_DIR_NAME_1 "/usr"
 #define NETWIBTESTDEF_DIR_EXISTS_1 NETWIB_TRUE
 #define NETWIBTESTDEF_DIR_NAME_2 "."
 #define NETWIBTESTDEF_DIR_EXISTS_2 NETWIB_TRUE
 #define NETWIBTESTDEF_DIR_NAME_3 "/sdfsdf"
 #define NETWIBTESTDEF_DIR_EXISTS_3 NETWIB_FALSE
#elif defined NETWIBDEF_SYSNAME_Windows
 #define NETWIBTESTDEF_DIR_NAME_1 "c:/"
 #define NETWIBTESTDEF_DIR_EXISTS_1 NETWIB_TRUE
 #define NETWIBTESTDEF_DIR_NAME_2 "."
 #define NETWIBTESTDEF_DIR_EXISTS_2 NETWIB_TRUE
 #define NETWIBTESTDEF_DIR_NAME_3 "d:\\sdfsdf"
 #define NETWIBTESTDEF_DIR_EXISTS_3 NETWIB_FALSE
 #define NETWIBTESTDEF_DIR_NAME_4 "\\\\sdf\\sdfsdf\\"
 #define NETWIBTESTDEF_DIR_EXISTS_4 NETWIB_FALSE
#endif

/*-------------------------------------------------------------*/
/* Those defines are needed in several path functions.
   You can enter up to 10 regular files.
     NETWIBTESTDEF_FILE_NAME_x :
        filename
     NETWIBTESTDEF_FILE_EXISTS_x :
        NETWIB_TRUE or NETWIB_FALSE depending if name exists
*/
#if defined NETWIBDEF_SYSNAME_Unix
 #define NETWIBTESTDEF_FILE_NAME_1 "/etc/passwd"
 #define NETWIBTESTDEF_FILE_EXISTS_1 NETWIB_TRUE
 #define NETWIBTESTDEF_FILE_NAME_2 "/etc/sdfsdf"
 #define NETWIBTESTDEF_FILE_EXISTS_2 NETWIB_FALSE
#elif defined NETWIBDEF_SYSNAME_Windows
 #define NETWIBTESTDEF_FILE_NAME_1 "./netwibtest.exe"
 #define NETWIBTESTDEF_FILE_EXISTS_1 NETWIB_TRUE
 #define NETWIBTESTDEF_FILE_NAME_2 "c:\\sdfsdf"
 #define NETWIBTESTDEF_FILE_EXISTS_2 NETWIB_FALSE
 #define NETWIBTESTDEF_FILE_NAME_3 "\\\\sdf\\sdfsdf\\sdf"
 #define NETWIBTESTDEF_FILE_EXISTS_3 NETWIB_FALSE
 #define NETWIBTESTDEF_FILE_NAME_4 "m:./sdf"
 #define NETWIBTESTDEF_FILE_EXISTS_4 NETWIB_FALSE
#endif

/*-------------------------------------------------------------*/
/* Those defines are needed to test netwib_dirname_secure.
   You can enter up to 10 entries.
     NETWIBTESTDEF_DIRSECURE_NAME_x :
        directory name
     NETWIBTESTDEF_DIRSECURE_SECURE_x :
        NETWIB_TRUE or NETWIB_FALSE depending if name is secure
*/
#if defined NETWIBDEF_SYSNAME_Unix
 #define NETWIBTESTDEF_DIRSECURE_NAME_1 "/"
 #define NETWIBTESTDEF_DIRSECURE_SECURE_1 NETWIB_TRUE
 #define NETWIBTESTDEF_DIRSECURE_NAME_2 "/usr"
 #define NETWIBTESTDEF_DIRSECURE_SECURE_2 NETWIB_TRUE
 #if ! defined NETWIBDEF_SYSNAME_Tru64UNIX
  /* under HP Testdrive for Tru64 UNIX, /tmp is a link */
  #define NETWIBTESTDEF_DIRSECURE_NAME_3 "/tmp"
  #define NETWIBTESTDEF_DIRSECURE_SECURE_3 NETWIB_FALSE
 #endif
#elif defined NETWIBDEF_SYSNAME_Windows
 #define NETWIBTESTDEF_DIRSECURE_NAME_1 "c:."
 #define NETWIBTESTDEF_DIRSECURE_SECURE_1 NETWIB_TRUE
#endif

/*-------------------------------------------------------------*/
/* Those defines are needed to test several conversion functions.
   You can enter up to 10 entries.
     NETWIBTESTDEF_HOST_NAME_x :
        hostname (or undefined if none)
     NETWIBTESTDEF_IP_x :
        IPv4 or IPv6 address
     NETWIBTESTDEF_ETH_x :
        Ethernet address (or undefined if none)
     NETWIBTESTDEF_HOST_NAMETOIP_x :
        If we can convert from NAME to IP
     NETWIBTESTDEF_HOST_IPTONAME_x :
        If we can convert from IP to NAME
     NETWIBTESTDEF_HOST_ETHTOIP_x :
        If we can convert from ETH to IP
     NETWIBTESTDEF_HOST_IPTOETH_x :
        If we can convert from IP to ETH
*/
#if 0
 #define NETWIBTESTDEF_HOST_NAME_1 "localhost"
 #define NETWIBTESTDEF_HOST_IP_1 "127.0.0.1"
 /*#define NETWIBTESTDEF_HOST_IP_1 "::1" */
 /*#define NETWIBTESTDEF_HOST_ETH_1 "00:01:02:03:04:05"*/
 #define NETWIBTESTDEF_HOST_NAMETOIP_1 NETWIB_TRUE
 #define NETWIBTESTDEF_HOST_IPTONAME_1 NETWIB_TRUE
 #define NETWIBTESTDEF_HOST_ETHTOIP_1 NETWIB_FALSE
 #define NETWIBTESTDEF_HOST_IPTOETH_1 NETWIB_FALSE
#endif
#if 0
 #define NETWIBTESTDEF_HOST_NAME_2 "hostname"
 #define NETWIBTESTDEF_HOST_IP_2 "192.168.1.1"
 /*#define NETWIBTESTDEF_HOST_IP_2 "fec0:0:0:1::1"*/
 #define NETWIBTESTDEF_HOST_ETH_2 "00:11:22:33:44:55"
 #define NETWIBTESTDEF_HOST_NAMETOIP_2 NETWIB_TRUE
 #define NETWIBTESTDEF_HOST_IPTONAME_2 NETWIB_TRUE
 #define NETWIBTESTDEF_HOST_ETHTOIP_2 NETWIB_TRUE
 #define NETWIBTESTDEF_HOST_IPTOETH_2 NETWIB_TRUE
#endif

