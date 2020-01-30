/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_rand_seed(netwib_uint64 *pseed)
{

#if defined NETWIBDEF_SYSNAME_Unix
  {
    int fd, reti;
    char arr[8];
    netwib_err ret;

    /* try with "/dev/random" */
    fd = open("/dev/random", O_RDONLY);
    if (fd != -1) {
      ret = netwib_priv_fd_block_set(fd, NETWIB_FALSE);
      if (ret == NETWIB_ERR_OK) {
        reti = read(fd, arr, 8);
        if (reti == 8) {
          netwib__uint64_init_8(arr[0], arr[1], arr[2], arr[3],
                                arr[4], arr[5], arr[6], arr[7], *pseed);
          close(fd);
          errno = 0;
          return(NETWIB_ERR_OK);
        }
      }
      close(fd);
    }

    /* try with "/dev/urandom" */
    fd = open("/dev/urandom", O_RDONLY);
    if (fd != -1) {
      ret = netwib_priv_fd_block_set(fd, NETWIB_FALSE);
      if (ret == NETWIB_ERR_OK) {
        reti = read(fd, arr, 8);
        if (reti == 8) {
          netwib__uint64_init_8(arr[0], arr[1], arr[2], arr[3],
                                arr[4], arr[5], arr[6], arr[7], *pseed);
          close(fd);
          errno = 0;
          return(NETWIB_ERR_OK);
        }
      }
      close(fd);
    }

    /* use time and pid */
    netwib__uint64_init_32(time(0), (getpid() << 16) | getppid(), *pseed);
  }
#elif defined NETWIBDEF_SYSNAME_Windows
  {
    struct _timeb tstruct;

    /* use localtime */
    _ftime(&tstruct);
    netwib__uint64_init_32(tstruct.time, tstruct.millitm, *pseed);
  }
#else
#error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  errno = 0;
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_rand_gene(netwib_uint32 *prand32,
                                 netwib_data randarray)
{
  netwib_priv_glovars_t *ptr = &netwib_priv_glovars;
  netwib_uint64 a, b;
  netwib_uint32 high, low;

  netwib_er(netwib_priv_glovars_wrlock());

  netwib__uint64_init_32(0x5851F42D, 0x4C957F2D, a);
  netwib__uint64_mul(a, ptr->rand_seed, &b);
  netwib__uint64_inc(b);
  ptr->rand_seed = b;

  if (prand32 != NULL) {
    *prand32 = netwib_c2_uint64_32high(ptr->rand_seed);
  }

  if (randarray != NULL) {
    high = netwib_c2_uint64_32high(ptr->rand_seed);
    low = netwib_c2_uint64_32low(ptr->rand_seed);
    randarray[0] = netwib_c2_uint32_0(high);
    randarray[1] = netwib_c2_uint32_1(high);
    randarray[2] = netwib_c2_uint32_2(high);
    randarray[3] = netwib_c2_uint32_3(high);
    randarray[4] = netwib_c2_uint32_0(low);
    randarray[5] = netwib_c2_uint32_1(low);
  }

  netwib_er(netwib_priv_glovars_wrunlock());

  return(NETWIB_ERR_OK);
}
