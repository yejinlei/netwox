/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dir_create(netwib_constbuf *pdirname)
{
  netwib_string dirnamestr;
  netwib_bool errwin240;
  int reti;

  netwib__constbuf_ref_string(pdirname, dirnamestr, bufstorage,
                              netwib_priv_dir_create(&bufstorage));

  errwin240 = NETWIB_FALSE;
#if defined NETWIBDEF_SYSNAME_Unix
  reti = mkdir(dirnamestr, 0700);
#elif defined NETWIBDEF_SYSNAME_Windows
  reti = _mkdir(dirnamestr);
  if (reti == -1) {
    if (errno == ENOENT) {
      errwin240 = NETWIB_TRUE;
    }
  }
#else
#error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  if (reti == -1) {
    netwib_er(netwib_priv_errmsg_string("cannot create this dir: "));
    netwib_er(netwib_priv_errmsg_append_buf(pdirname));
#if defined NETWIBDEF_SYSNAME_Unix
    errwin240 = errwin240;
#elif defined NETWIBDEF_SYSNAME_Windows
    if (errwin240) {
      netwib_er(netwib_priv_errmsg_append_string(" (origin of this problem is perhaps Windows'limitation of 240 chars for total pathname length)"));
    }
#else
#error "Unknown value for NETWIBDEF_SYSNAME"
#endif
    return(NETWIB_ERR_FUMKDIR);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_dir_create_recur(netwib_constbuf *pdirname,
                                               netwib_uint32 numcall)
{
  netwib_byte array[512];
  netwib_buf parent;
  netwib_bool yes;
  netwib_err ret;

  numcall++;
  if (numcall > 100) {
    /* we suppose it is not legitimate to create too many directories */
    return(NETWIB_ERR_PATOOHIGH);
  }

  netwib_er(netwib_buf_init_ext_storagearraysizeof(array, &parent));
  ret = netwib_path_decode_parent(pdirname, &parent);
  if (ret == NETWIB_ERR_PAPATHCANTINIT) {
    /* can't go up: we will directly create directory */
    ret = NETWIB_ERR_OK;
  } else if (ret == NETWIB_ERR_OK) {
    /* eventually create parent */
    netwib_er(netwib_dirname_exists(&parent, &yes));
    if ( !yes ) {
      ret = netwib_priv_dir_create_recur(&parent, numcall);
    }
  }
  netwib_er(netwib_buf_close(&parent));

  if (ret == NETWIB_ERR_OK) {
    ret = netwib_priv_dir_create(pdirname);
  }

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dir_create_parents(netwib_constbuf *ppathname)
{
  netwib_byte parentarray[512], startarray[128];
  netwib_buf parent, start;
  netwib_bool yes;
  netwib_err ret;

  /* obtain parent */
  netwib_er(netwib_buf_init_ext_storagearraysizeof(parentarray, &parent));
  ret = netwib_path_decode_parent(ppathname, &parent);
  if (ret == NETWIB_ERR_PAPATHCANTINIT) {
    /* can't go up */
    netwib_er(netwib_buf_close(&parent));
    return(NETWIB_ERR_OK);
  } else if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_buf_close(&parent));
    return(ret);
  }

  /* if parent exists, do not need to continue */
  netwib_er(netwib_dirname_exists(&parent, &yes));
  if ( yes ) {
    netwib_er(netwib_buf_close(&parent));
    return(NETWIB_ERR_OK);
  }

  /* check if begin directory exists */
  netwib_er(netwib_buf_init_ext_storagearraysizeof(startarray, &start));
  netwib_er(netwib_path_decode_begin(&parent, &start));
  netwib_er(netwib_dirname_exists(&start, &yes));
  if ( !yes ) {
    netwib_er(netwib_priv_errmsg_string("cannot create this dir: "));
    netwib_er(netwib_priv_errmsg_append_buf(&start));
    netwib_er(netwib_buf_close(&start));
    netwib_er(netwib_buf_close(&parent));
    return(NETWIB_ERR_NOTFOUND);
  }
  netwib_er(netwib_buf_close(&start));

  /* create parent */
  ret = netwib_priv_dir_create_recur(&parent, 0);
  if (ret != NETWIB_ERR_OK) {
    netwib_er(netwib_priv_errmsg_string("cannot create parent dir: "));
    netwib_er(netwib_priv_errmsg_append_buf(&parent));
  }
  netwib_er(netwib_buf_close(&parent));

  return(ret);
}
