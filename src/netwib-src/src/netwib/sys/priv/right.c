/* This file is part of Netwib. Read and honor its license.
*/

/*-------------------------------------------------------------*/
/* those defines are here because I have a script checking if
   netwib contains MT-unsafe functions. Like this, they are ignored. */
#define netwib_setpwent setpwent
#define netwib_getpwent getpwent
#define netwib_endpwent endpwent
#define netwib_getpwnam getpwnam
#define netwib_getgrnam getgrnam
#define netwib_getgrgid getgrgid

/*-------------------------------------------------------------*/
netwib_err netwib_priv_right_user_current(netwib_uint32 *puid)
{
  if (puid != NULL) *puid = getuid();
  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_right_unam2uid_mtunsafe(netwib_conststring name,
                                                      netwib_bool *pfound,
                                                      netwib_uint32 *puid)
{
  struct passwd *presult;

  presult = netwib_getpwnam(name);
  if (presult == NULL) {
    *pfound = NETWIB_FALSE;
  } else {
    *pfound = NETWIB_TRUE;
    *puid = (netwib_uint32)presult->pw_uid;
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_right_unam2uid(netwib_conststring name,
                                             netwib_bool *pfound,
                                             netwib_uint32 *puid)
#if NETWIBDEF_HAVEFUNC_GETPWNAM_R == 1
{
  netwib_uint32 mallocsize;
  netwib_ptr ptr;
  struct passwd entry, *presult;
  int reti;

 #if NETWIBDEF_HAVEFUNC_SYSCONF==1
  #if NETWIBDEF_HAVEVAR_SC_GETPW_R_SIZE_MAX == 1
    mallocsize = sysconf(_SC_GETPW_R_SIZE_MAX);
  #else
   mallocsize = 4096;
  #endif
 #else
  mallocsize = 4096;
 #endif

  netwib_er(netwib_ptr_malloc(mallocsize, &ptr));

 #if NETWIBDEF_LIBPTHREADFROMDCE == 1
  presult = &entry;
  reti = getpwnam_r(name, &entry, (netwib_string)ptr, mallocsize);
  if (reti) {
    presult = NULL;
  }
 #else
  reti = getpwnam_r(name, &entry, (netwib_string)ptr, mallocsize, &presult);
  if (reti) {
    if (errno == ENOENT) {
      /* because Mandrake 9 set errno instead of presult */
      presult = NULL;
    } else {
      netwib_er(netwib_ptr_free(&ptr));
      return(NETWIB_ERR_FUGETPWNAMR);
    }
  }
 #endif

  if (presult == NULL) {
    *pfound = NETWIB_FALSE;
  } else {
    *pfound = NETWIB_TRUE;
    *puid = (netwib_uint32)entry.pw_uid;
  }

  netwib_er(netwib_ptr_free(&ptr));
  return(NETWIB_ERR_OK);
}
#else
{
  netwib_err ret;

  netwib_er(netwib_priv_glovars_other_wrlock());
  ret = netwib_priv_right_unam2uid_mtunsafe(name, pfound, puid);
  netwib_er(netwib_priv_glovars_other_wrunlock());

  return(ret);
}
#endif

/*-------------------------------------------------------------*/
#if NETWIBDEF_HAVEFUNC_GETGRNAM_R == 0
static netwib_err netwib_priv_right_gnam2gid_mtunsafe(netwib_conststring name,
                                                      netwib_bool *pfound,
                                                      netwib_uint32 *pgid)
{
  struct group *presult;

  presult = netwib_getgrnam(name);
  if (presult == NULL) {
    *pfound = NETWIB_FALSE;
  } else {
    *pfound = NETWIB_TRUE;
    *pgid = (netwib_uint32)presult->gr_gid;
  }

  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_right_gnam2gid(netwib_conststring name,
                                             netwib_bool *pfound,
                                             netwib_uint32 *pgid)
#if NETWIBDEF_HAVEFUNC_GETGRNAM_R == 1
{
  netwib_uint32 mallocsize;
  netwib_ptr ptr;
  struct group entry, *presult;
  int reti;

 #if NETWIBDEF_HAVEFUNC_SYSCONF==1
  #if NETWIBDEF_HAVEVAR_SC_GETGR_R_SIZE_MAX == 1
   mallocsize = sysconf(_SC_GETGR_R_SIZE_MAX);
  #else
   mallocsize = 4096;
  #endif
 #else
  mallocsize = 4096;
 #endif

  netwib_er(netwib_ptr_malloc(mallocsize, &ptr));

 #if NETWIBDEF_LIBPTHREADFROMDCE == 1
  presult = &entry;
  reti = getgrnam_r(name, &entry, (netwib_string)ptr, mallocsize);
  if (reti) {
    presult = NULL;
  }
 #else
  reti = getgrnam_r(name, &entry, (netwib_string)ptr, mallocsize, &presult);
  if (reti) {
    if (errno == ENOENT) {
      /* because Mandrake 9 set errno instead of presult */
      presult = NULL;
    } else {
      netwib_er(netwib_ptr_free(&ptr));
      return(NETWIB_ERR_FUGETGRNAMR);
    }
  }
 #endif

  if (presult == NULL) {
    *pfound = NETWIB_FALSE;
  } else {
    *pfound = NETWIB_TRUE;
    *pgid = (netwib_uint32)entry.gr_gid;
  }

  netwib_er(netwib_ptr_free(&ptr));
  return(NETWIB_ERR_OK);
}
#else
{
  netwib_err ret;

  netwib_er(netwib_priv_glovars_other_wrlock());
  ret = netwib_priv_right_gnam2gid_mtunsafe(name, pfound, pgid);
  netwib_er(netwib_priv_glovars_other_wrunlock());

  return(ret);
}
#endif

/*-------------------------------------------------------------*/
netwib_err netwib_priv_right_user_trust(netwib_uint32 uid,
                                        netwib_bool *pyes)
{
  netwib_uint32 currentuid, hisuid;
  netwib_bool found;

  if (uid == 0) {
    if (pyes != NULL) *pyes = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_priv_right_user_current(&currentuid));
  if (uid == currentuid) {
    if (pyes != NULL) *pyes = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_priv_right_unam2uid("sys", &found, &hisuid));
  if (found && hisuid == uid) {
    if (pyes != NULL) *pyes = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_priv_right_unam2uid("bin", &found, &hisuid));
  if (found && hisuid == uid) {
    if (pyes != NULL) *pyes = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  if (pyes != NULL) *pyes = NETWIB_FALSE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_right_user_trust_mtunsafe(netwib_uint32 uid,
                                                        netwib_bool *pyes)
{
  netwib_uint32 currentuid, hisuid;
  netwib_bool found;

  if (uid == 0) {
    if (pyes != NULL) *pyes = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_priv_right_user_current(&currentuid));
  if (uid == currentuid) {
    if (pyes != NULL) *pyes = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_priv_right_unam2uid_mtunsafe("sys", &found, &hisuid));
  if (found && hisuid == uid) {
    if (pyes != NULL) *pyes = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  netwib_er(netwib_priv_right_unam2uid_mtunsafe("bin", &found, &hisuid));
  if (found && hisuid == uid) {
    if (pyes != NULL) *pyes = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  if (pyes != NULL) *pyes = NETWIB_FALSE;

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
#if NETWIBDEF_HAVEFUNC_GETGRGID_R == 0
static netwib_err netwib_priv_right_gid_alltrust_mtunsafe(netwib_uint32 gid,
                                                          netwib_bool *pyes)
{
  netwib_bool yes, found;
  netwib_string user;
  netwib_uint32 i, hisuid;
  struct group *presult;

  presult = netwib_getgrgid(gid);
  if (presult == NULL) {
    if (pyes != NULL) *pyes = NETWIB_FALSE;
    return(NETWIB_ERR_OK);
  }
  i = 0;
  while (NETWIB_TRUE) {
    user = presult->gr_mem[i];
    if (user == NULL) {
      break;
    }
    netwib_er(netwib_priv_right_unam2uid_mtunsafe(user, &found, &hisuid));
    if (!found) {
      if (pyes != NULL) *pyes = NETWIB_FALSE;
      return(NETWIB_ERR_OK);
    }
    netwib_er(netwib_priv_right_user_trust_mtunsafe(hisuid, &yes));
    if (!yes) {
      if (pyes != NULL) *pyes = NETWIB_FALSE;
      return(NETWIB_ERR_OK);
    }
    i++;
  }

  return(NETWIB_ERR_OK);
}
#endif

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_right_gid_alltrust(netwib_uint32 gid,
                                                 netwib_bool *pyes)
#if NETWIBDEF_HAVEFUNC_GETGRGID_R == 1
{
  netwib_ptr ptr;
  netwib_bool yes, found;
  netwib_string usr;
  netwib_uint32 i, hisuid, mallocsize;
  struct group entry, *presult;
  int reti;

 #if NETWIBDEF_HAVEFUNC_SYSCONF==1
  #if NETWIBDEF_HAVEVAR_SC_GETGR_R_SIZE_MAX == 1
   mallocsize = sysconf(_SC_GETGR_R_SIZE_MAX);
  #else
   mallocsize = 4096;
  #endif
 #else
  mallocsize = 4096;
 #endif
  netwib_er(netwib_ptr_malloc(mallocsize, &ptr));

 #if NETWIBDEF_LIBPTHREADFROMDCE == 1
  presult = &entry;
  reti = getgrgid_r(gid, &entry, (netwib_string)ptr, mallocsize);
  if (reti) {
    presult = NULL;
  }
 #else
  reti = getgrgid_r(gid, &entry, (netwib_string)ptr, mallocsize, &presult);
  if (reti) {
    if (errno == ENOENT) {
      /* because Mandrake 9 set errno instead of presult */
      presult = NULL;
    } else {
      netwib_er(netwib_ptr_free(&ptr));
      return(NETWIB_ERR_FUGETGRGIDR);
    }
  }
 #endif

  if (presult == NULL) {
    if (pyes != NULL) *pyes = NETWIB_FALSE;
    netwib_er(netwib_ptr_free(&ptr));
    return(NETWIB_ERR_OK);
  }

  i = 0;
  while (NETWIB_TRUE) {
    usr = entry.gr_mem[i];
    if (usr == NULL) {
      break;
    }
    netwib_er(netwib_priv_right_unam2uid(usr, &found, &hisuid));
    if (!found) {
      if (pyes != NULL) *pyes = NETWIB_FALSE;
      netwib_er(netwib_ptr_free(&ptr));
      return(NETWIB_ERR_OK);
    }
    netwib_er(netwib_priv_right_user_trust(hisuid, &yes));
    if (!yes) {
      if (pyes != NULL) *pyes = NETWIB_FALSE;
      netwib_er(netwib_ptr_free(&ptr));
      return(NETWIB_ERR_OK);
    }
    i++;
  }
  netwib_er(netwib_ptr_free(&ptr));

  return(NETWIB_ERR_OK);
}
#else
{
  netwib_err ret;

  netwib_er(netwib_priv_glovars_other_wrlock());
  ret = netwib_priv_right_gid_alltrust_mtunsafe(gid, pyes);
  netwib_er(netwib_priv_glovars_other_wrunlock());

  return(ret);
}
#endif

/*-------------------------------------------------------------*/
static netwib_err netwib_priv_right_gid_onlytrust_mtunsafe(netwib_uint32 gid,
                                                           netwib_bool *pyes)
{
  netwib_bool yes;
  struct passwd *ppw;

  netwib_setpwent();

  if (pyes != NULL) *pyes = NETWIB_TRUE;
  while (NETWIB_TRUE) {
    ppw = netwib_getpwent();
    if (ppw == NULL) {
      break;
    }
    if ((netwib_uint32)ppw->pw_gid == gid) {
      netwib_er(netwib_priv_right_user_trust_mtunsafe(ppw->pw_uid, &yes));
      if (!yes) {
        if (pyes != NULL) *pyes = NETWIB_FALSE;
        break;
      }
    }
  }

  netwib_endpwent();

  return(NETWIB_ERR_OK);
}
static netwib_err netwib_priv_right_gid_onlytrust(netwib_uint32 gid,
                                                  netwib_bool *pyes)
{
  netwib_err ret;

  netwib_er(netwib_priv_glovars_other_wrlock());
  ret = netwib_priv_right_gid_onlytrust_mtunsafe(gid, pyes);
  netwib_er(netwib_priv_glovars_other_wrunlock());

  return(ret);
}

/*-------------------------------------------------------------*/
netwib_err netwib_priv_right_group_trust(netwib_uint32 gid,
                                         netwib_bool *pyes)
{
  netwib_bool found;
  netwib_uint32 hisgid;

  /* check if it corresponds to a secure group */
  if (gid == 0) {
    if (pyes != NULL) *pyes = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }
  netwib_er(netwib_priv_right_gnam2gid("sys", &found, &hisgid));
  if (found && hisgid == gid) {
    if (pyes != NULL) *pyes = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }
  netwib_er(netwib_priv_right_gnam2gid("bin", &found, &hisgid));
  if (found && hisgid == gid) {
    if (pyes != NULL) *pyes = NETWIB_TRUE;
    return(NETWIB_ERR_OK);
  }

  /* check if all users in this group are trusted */
  netwib_er(netwib_priv_right_gid_alltrust(gid, pyes));

  /* check if only trusted users have this group */
  netwib_er(netwib_priv_right_gid_onlytrust(gid, pyes));

  return(NETWIB_ERR_OK);
}
