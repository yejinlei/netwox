
/*-------------------------------------------------------------*/
/* the FILE type is necessary to write to a file */
#if defined NETWIB_EXT_FILE
 /* user included stdio.h (which defines FILE type) before
    netwib.h */
 #define NETWIBFILE FILE
#else
 /* [default] user perhaps forgot to include stdio.h before
    netwib.h, but netwib has to work without bothering user */
 #define NETWIBFILE void
#endif

/*-------------------------------------------------------------*/
/* the HANDLE type is used everywhere under Windows */
#if defined NETWIB_EXT_HANDLE
 /* user included windows.h (which defines FILE type) before
    netwib.h */
 #define NETWIBHANDLE HANDLE
#else
 /* [default] user perhaps forgot to include windows.h before
    netwib.h or is under Unix, but netwib has to work without
    bothering user */
 #define NETWIBHANDLE void*
#endif
