/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
netwib_err netwib_priv_stat_init_pathname(netwib_constbuf *pathname,
                                          netwib_pathstat *ppathstat)
{
  netwib_string str;

  netwib__constbuf_ref_string(pathname, str, bufstorage,
                              netwib_priv_stat_init_pathname(&bufstorage,
                                                             ppathstat));

  return(netwib_priv_stat_init_pathname2(str, ppathstat));
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_stat_init_pathname2(netwib_conststring pathname,
                                           netwib_pathstat *ppathstat)
{
#if defined NETWIBDEF_SYSNAME_Unix
  struct stat st;
#elif defined NETWIBDEF_SYSNAME_Windows
  struct _stati64 st;
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif
  netwib_uintmax size;
  int reti;

#if defined NETWIBDEF_SYSNAME_Unix
  reti = lstat(pathname, &st);
#elif defined NETWIBDEF_SYSNAME_Windows
  reti = _stati64(pathname, &st);
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif
  if (reti == -1) {
    if (errno == ENOENT || errno == ENOTDIR) {
      netwib_er(netwib_priv_errmsg_string("file not found: "));
      netwib_er(netwib_priv_errmsg_append_string(pathname));
      return(NETWIB_ERR_NOTFOUND);
    }
    return(NETWIB_ERR_FULSTAT);
  }

  if (ppathstat == NULL) {
    return(NETWIB_ERR_OK);
  }

#if defined NETWIBDEF_SYSNAME_Unix
  if (S_ISREG(st.st_mode))
    ppathstat->type = NETWIB_PATHSTAT_TYPE_REG;
  else if (S_ISDIR(st.st_mode))
    ppathstat->type = NETWIB_PATHSTAT_TYPE_DIR;
  else if (S_ISLNK(st.st_mode))
    ppathstat->type = NETWIB_PATHSTAT_TYPE_LINK;
  else if (S_ISSOCK(st.st_mode))
    ppathstat->type = NETWIB_PATHSTAT_TYPE_SOCK;
  else if (S_ISBLK(st.st_mode))
    ppathstat->type = NETWIB_PATHSTAT_TYPE_BLOCK;
  else if (S_ISCHR(st.st_mode))
    ppathstat->type = NETWIB_PATHSTAT_TYPE_CHAR;
  else if (S_ISFIFO(st.st_mode))
    ppathstat->type = NETWIB_PATHSTAT_TYPE_FIFO;
  else
    ppathstat->type = NETWIB_PATHSTAT_TYPE_UNKNOWN;
#elif defined NETWIBDEF_SYSNAME_Windows
  if (st.st_mode & _S_IFREG)
    ppathstat->type = NETWIB_PATHSTAT_TYPE_REG;
  else if (st.st_mode & _S_IFDIR)
    ppathstat->type = NETWIB_PATHSTAT_TYPE_DIR;
  else
    ppathstat->type = NETWIB_PATHSTAT_TYPE_UNKNOWN;
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

  /* Under BSD/MacOS, st.st_size has a 64 bit size.
     Under Linux, Solaris and HP-UX, defines _LARGEFILE_SOURCE and
     _FILE_OFFSET_BITS have to be set to transparently replace "struct stat"
     by "struct stat64".
  */
  size = (netwib_uintmax)st.st_size;
  if (size > 0x7FFFFFFFu) {
    ppathstat->size = NETWIB_PATHSTAT_SIZE_GT2G;
  } else {
    ppathstat->size = (netwib_uint32)size;
  }
  netwib__uint64_init_uintmax(size, ppathstat->size64);

  /* FreeBSD has st.st_mtimensec, but it is not available here because
     of defines in maininc.h.
  */
  ppathstat->mtime.sec = st.st_mtime;
  ppathstat->mtime.nsec = 0;

  return(NETWIB_ERR_OK);
}
