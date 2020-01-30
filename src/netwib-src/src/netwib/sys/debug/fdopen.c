/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
#ifdef NETWIBDEF_SYSNAME_Unix
 #define NETWIB_PRIV_FDOPEN_FD_FILENAME "/dev/null"
#elif defined NETWIBDEF_SYSNAME_Windows
 #define NETWIB_PRIV_FDOPEN_FD_FILENAME "c:\\winzip.log"
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
int *netwib_debug_fdopen_array;
int netwib_debug_fdopen_max;

/*-------------------------------------------------------------*/
static netwib_err netwib_debug_fdopen_max_obtain(void)
{

#if defined NETWIBDEF_SYSNAME_FreeBSD
  /* we should use getrlimit but the returned value cannot really be reached */
  netwib_debug_fdopen_max = OPEN_MAX;
#elif defined NETWIBDEF_SYSNAME_Windows
  netwib_debug_fdopen_max = 2048;
#else
 #if NETWIBDEF_HAVEFUNC_GETRLIMIT==1
  {
    struct rlimit lim;
    int reti;
    reti = getrlimit(RLIMIT_NOFILE, &lim);
    if (reti == -1) {
      return(NETWIB_ERR_FUGETRLIMIT);
    }
    netwib_debug_fdopen_max = lim.rlim_cur;
  }
 #elif NETWIBDEF_HAVEFUNC_SYSCONF==1
  netwib_debug_fdopen_max = sysconf(_SC_OPEN_MAX);
 #else
  netwib_debug_fdopen_max = OPEN_MAX;
 #endif
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_debug_fdopen_max_check(void)
{
#if defined NETWIBDEF_SYSNAME_FreeBSD
  /* we used a fake max, so we cannot check */
#else
  int fd, reti;
  fd = netwib_c_open(NETWIB_PRIV_FDOPEN_FD_FILENAME, O_RDONLY);
  if (fd != -1) {
    netwib_er(netwib_debug_disp_fmt("Warning : the maximum fd could "));
    netwib_er(netwib_debug_disp_fmt("be %{uint32} instead of %{uint32}\n",
                                  (netwib_uint32)(fd + 1),
                                  (netwib_uint32)netwib_debug_fdopen_max));
    reti = netwib_c_close(fd);
    if (reti == -1) {
      netwib_er(netwib_debug_disp_fmt("Warning : error in closing "));
      netwib_er(netwib_debug_disp_fmt("fd %{uint32}\n", (netwib_uint32)fd));
    }
  }
#endif

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_debug_fdopen_array_init(int **parray)
{
  int *array;
  netwib_uint32 arrayindex;
  int fd, previousfd, i, reti;

  /* obtain limit */
  netwib_er(netwib_debug_fdopen_max_obtain());

  /* allocate array which will contain the list of previously
     opened descriptors */
  array = (int*)malloc((netwib_debug_fdopen_max+1)*sizeof(int));
  if (array == NULL) return(NETWIB_ERR_FUMALLOC);
  arrayindex = 0;

  /* obtain list */
  fd = -1;
  previousfd = fd;
  while (fd < netwib_debug_fdopen_max-1) {
    fd = netwib_c_open(NETWIB_PRIV_FDOPEN_FD_FILENAME, O_RDONLY);
    if (fd == -1) {
      /* last reached */
      if (errno != EMFILE && errno != ENFILE) {
        netwib_er(netwib_debug_disp_fmt("Warning: errno=%{uint32} ",
                                        (netwib_uint32)errno));
        netwib_er(netwib_debug_disp_fmt("after fd=%{uint32}\n",
                                        (netwib_uint32)previousfd));
      }
      break;
    }
    /* store skiped file descriptors */
    for (i = previousfd+1; i < fd; i++) {
      array[arrayindex++] = i;
    }
    previousfd = fd;
  }
  /* set last part */
  for (i = previousfd+1; i < netwib_debug_fdopen_max; i++) {
    array[arrayindex++] = i;
  }
  array[arrayindex++] = -1; /* end marker */
  /* realloc as needed */
  *parray = (int*)realloc(array, arrayindex*sizeof(int));

  /* check if the max is the good value */
  netwib_er(netwib_debug_fdopen_max_check());

  /* close descriptors we opened (ie which are not in the array) */
  arrayindex = 0;
  for (fd = 0; fd < netwib_debug_fdopen_max; fd++) {
    if (array[arrayindex] == fd) {
      arrayindex++;
    } else {
      reti = netwib_c_close(fd);
      if (reti == -1) {
        netwib_er(netwib_debug_disp_fmt("Warning : error in closing "));
        netwib_er(netwib_debug_disp_fmt("fd %{uint32}\n", (netwib_uint32)fd));
      }
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_debug_fdopen_array_close(int **parray)
{
  /* free array */
  free(*parray);

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#if 0
static netwib_err netwib_debug_fdopen_array_out_screen(int *array)
{
  netwib_uint32 arrayindex;

  arrayindex = 0;
  while (array[arrayindex] != -1) {
    arrayindex++;
    netwib_er(netwib_debug_disp_fmt("%{r 2;int32} ",
                                    (netwib_int32)array[arrayindex]));
  }
  netwib_er(netwib_debug_disp_string("\n"));

  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
static void *netwib_debug_fdopen_init_func(void *p)
{ return(p);
}
#endif
netwib_err netwib_debug_fdopen_init(void)
{
  int fd, reti;

  /* check if reference file exists */
  fd = netwib_c_open(NETWIB_PRIV_FDOPEN_FD_FILENAME, O_RDONLY);
  if (fd == -1) {
    netwib_er(netwib_debug_disp_fmt("debug_fdopen: error : file %s ",
                                    NETWIB_PRIV_FDOPEN_FD_FILENAME));
    if (errno == ENOENT) {
      netwib_er(netwib_debug_disp_string("does not exist\n"));
    } else {
      netwib_er(netwib_debug_disp_fmt("generated errno==%{uint32}\n",
                                      (netwib_uint32)errno));
    }
    return(NETWIB_ERR_LOINTERNALERROR);
  }
  reti = netwib_c_close(fd);
  if (reti) return(NETWIB_ERR_FUCLOSE);

  /* to count thread manager descriptors */
#if defined NETWIBDEF_SYSNAME_Unix
  {
    pthread_t th;
    /* because thread manager opens a pipe */
#if NETWIBDEF_LIBPTHREADFROMDCE == 1
    pthread_create(&th, pthread_attr_default, &netwib_debug_fdopen_init_func,
                   NULL);
#else
    pthread_create(&th, NULL, &netwib_debug_fdopen_init_func, NULL);
#endif
    pthread_join(th, NULL);
  }
#endif

  /* obtain list of opened descriptors */
  netwib_er(netwib_debug_fdopen_array_init(&netwib_debug_fdopen_array));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_debug_fdopen_check3(int *array1,
                                             int *array2)
{
  netwib_uint32 arrayindex1, arrayindex2;
  int fd, fd1, fd2;

  arrayindex1 = 0;
  arrayindex2 = 0;
  for (fd = 0; fd < netwib_debug_fdopen_max; fd++) {
    fd1 = array1[arrayindex1];
    fd2 = array2[arrayindex2];
    if (fd1 == -1 && fd2 == -1) {
      /* only valid exit */
      break;
    } else if (fd1 == fd && fd2 == fd) {
      arrayindex1++;
      arrayindex2++;
    } else if (fd1 == fd) {
      arrayindex1++;
      netwib_er(netwib_debug_disp_fmt("Warning : fd "));
      netwib_er(netwib_debug_disp_fmt("%{uint32} is closed\n", fd));
    } else if (fd2 == fd) {
      arrayindex2++;
      netwib_er(netwib_debug_disp_fmt("Warning : fd "));
      netwib_er(netwib_debug_disp_fmt("%{uint32} is open\n", fd));
    } else {
      /* no move */
    }
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_debug_fdopen_check(void)
{
  int *array;

  /* obtain list of opened descriptors */
  netwib_er(netwib_debug_fdopen_array_init(&array));

  /* compare netwib_debug_fdopen_array and array */
  /*netwib_er(netwib_debug_fdopen_array_out_screen(netwib_debug_fdopen_array));
    netwib_er(netwib_debug_fdopen_array_out_screen(array));*/
  netwib_er(netwib_debug_fdopen_check3(netwib_debug_fdopen_array, array));

  /* free memory */
  netwib_er(netwib_debug_fdopen_array_close(&netwib_debug_fdopen_array));
  netwib_er(netwib_debug_fdopen_array_close(&array));

  return(NETWIB_ERR_OK);
}
