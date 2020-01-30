
/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
netwib_err netwib_priv_right_user_current(netwib_uint32 *puid);
#elif defined NETWIBDEF_SYSNAME_Windows
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif

/*-------------------------------------------------------------*/
#if defined NETWIBDEF_SYSNAME_Unix
netwib_err netwib_priv_right_user_trust(netwib_uint32 uid,
                                        netwib_bool *pyes);
netwib_err netwib_priv_right_group_trust(netwib_uint32 gid,
                                         netwib_bool *pyes);
#elif defined NETWIBDEF_SYSNAME_Windows
#else
 #error "Unknown value for NETWIBDEF_SYSNAME"
#endif
