
/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_init(void);
netwib_err netwib_priv_dll_close(void);

/*-------------------------------------------------------------*/
netwib_err netwib_priv_dll_loadlibrary(netwib_conststring name,
                                       HINSTANCE *ph);

/*-------------------------------------------------------------*/
#define netwib__priv_dll_f(d,f,t,fs) d->f = (t)GetProcAddress(d->hinstance, fs); if (d->f == NULL) { FreeLibrary(d->hinstance); return(NETWIB_ERR_FUGETPROCADDRESS); }

