/*
                         NETWIB - NETWOX - NETWAG
                           Installation program
                 Copyright(c) 2004-2012 Laurent CONSTANTIN
                      http://ntwox.sourceforge.net/
                        laurentconstantin@free.fr
                                  -----

  This file is part of Netwib/ox/ag.

  Netwib/ox/ag is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  version 2 as published by the Free Software Foundation.

  Netwib/ox/ag is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details (http://www.gnu.org/).

------------------------------------------------------------------------
*/

/* IMPORTANT :
   In order to compile this code with Dev-C++, go to:
     Project
     Project Options
     Parameters
     Linker
     Add in the text area : -lole32 -luuid
*/

/*-------------------------------------------------------------*/
/* This program permits to install netwib, netwox and netwag
   under Windows. */

/*-------------------------------------------------------------*/
/***************************************************************
 * First, generic functions, not specific to this installation *
 ***************************************************************/

/*-------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <objbase.h>
#include <shlobj.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>

/*-------------------------------------------------------------*/
/* generic typedef */
typedef unsigned int inst_uint;
typedef void* inst_ptr;
typedef unsigned char* inst_data;
typedef char* inst_string;
typedef const unsigned char* inst_constdata;
typedef const char* inst_conststring;

/*-------------------------------------------------------------*/
/* booleans */
typedef unsigned int inst_bool;
#define INST_FALSE 0
#define INST_TRUE 1

/*-------------------------------------------------------------*/
/* errors */
typedef enum {
  INST_ERR_OK = 0,
  INST_ERR_NOTFOUND,
  INST_ERR_PAINVALIDTYPE,
  INST_ERR_PATCH,
  INST_ERR_FUGENERATECONSOLECTRLEVENT,
  INST_ERR_FUGETSHORTPATHNAME,
  INST_ERR_FUMALLOC,
  INST_ERR_FUMKDIR,
  INST_ERR_FUREALLOC,
  INST_ERR_FUSHFILEOPERATION,
  INST_ERR_FUSHORTCUT
} inst_err;
#define inst_er(e) {inst_err localerr = e; if (localerr != INST_ERR_OK) return(localerr);}

/*-------------------------------------------------------------*/
/* simplified buffer implementation */
typedef struct {
  inst_data ptr;
  inst_uint totalsize;
  inst_uint endoffset;
} inst_buf;
static inst_err inst_buf_init(inst_buf *pbuf)
{
  pbuf->totalsize = 1024;
  pbuf->ptr = (inst_data)malloc(pbuf->totalsize);
  if (pbuf->ptr == NULL) {
    return(INST_ERR_FUMALLOC);
  }
  pbuf->endoffset = 0;

  return(INST_ERR_OK);
}
static inst_err inst_buf_close(inst_buf *pbuf)
{
  free(pbuf->ptr);
  pbuf->ptr = NULL;
  return(INST_ERR_OK);
}
static inst_err inst_buf_wantspace(inst_buf *pbuf,
                                   inst_uint wantedspace,
                                   inst_data *pdata)
{

  if (pbuf->endoffset + wantedspace > pbuf->totalsize) {
    pbuf->totalsize = pbuf->endoffset + wantedspace + 1024;
    pbuf->ptr = (inst_data)realloc(pbuf->ptr, pbuf->totalsize);
    if (pbuf->ptr == NULL) {
      return(INST_ERR_FUREALLOC);
    }
  }

  if (pdata != NULL) *pdata = pbuf->ptr + pbuf->endoffset;

  return(INST_ERR_OK);
}
static inst_err inst_buf_append_data(inst_constdata data,
                                     inst_uint datasize,
                                     inst_buf *pbuf)
{
  inst_data dataend;

  inst_er(inst_buf_wantspace(pbuf, datasize, &dataend));

  memcpy(dataend, data, datasize);
  pbuf->endoffset += datasize;

  return(INST_ERR_OK);
}
#define inst_buf_append_string(str,pbuf) inst_buf_append_data((inst_data)(str),strlen(str),pbuf)
#define inst_buf_append_buf(pbuftoapp,pbuf) inst_buf_append_data((pbuftoapp)->ptr,(pbuftoapp)->endoffset,pbuf)
static inst_err inst_buf_ref_string(inst_buf *pbuf,
                                    inst_string *pstr)
{
  inst_data data;
  inst_er(inst_buf_wantspace(pbuf, 1, &data));
  *data = '\0'; /* might end with several '\0', but it's not important */
  if (pstr != NULL) *pstr = (inst_string)pbuf->ptr;
  return(INST_ERR_OK);
}
static inst_err inst_buf_ref_string2(inst_buf *pbuf,
                                     inst_string *pstr)
{
  inst_data data;
  inst_er(inst_buf_wantspace(pbuf, 2, &data));
  *data++ = '\0';
  *data = '\0';
  if (pstr != NULL) *pstr = (inst_string)pbuf->ptr;
  return(INST_ERR_OK);
}
static inst_err inst_buf_display(inst_buf *pbuf)
{
  inst_string str;
  inst_er(inst_buf_ref_string(pbuf, &str));
  printf("%s", str);
  return(INST_ERR_OK);
}
#define inst__buf_reinit(pbuf) {(pbuf)->endoffset = 0;}


/*-------------------------------------------------------------*/
typedef enum {
  INST_WINDOWSTYPE_UNKNOWN = 0, /* Unknown system */
  INST_WINDOWSTYPE_31,          /* Windows 3.1 */
  INST_WINDOWSTYPE_95,          /* Windows 95 */
  INST_WINDOWSTYPE_98,          /* Windows 98 */
  INST_WINDOWSTYPE_ME,          /* Windows Me */
  INST_WINDOWSTYPE_NT350,       /* Windows NT 3.5.0 */
  INST_WINDOWSTYPE_NT351,       /* Windows NT 3.5.1 */
  INST_WINDOWSTYPE_NT4,         /* Windows NT 4 */
  INST_WINDOWSTYPE_2000,        /* Windows 2000 */
  INST_WINDOWSTYPE_XP,          /* Windows XP */
  INST_WINDOWSTYPE_2003,        /* Windows 2003 */
  INST_WINDOWSTYPE_2008,        /* Windows 2008 */
  INST_WINDOWSTYPE_7            /* Windows 7 */
} inst_windowstype;
inst_err inst_windowstype_init(inst_windowstype *ptype)
{
  OSVERSIONINFO verInfo;
  inst_windowstype type;

  /* obtain the version */
  verInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  if (!GetVersionEx(&verInfo)) {
    if (ptype != NULL)
      *ptype = INST_WINDOWSTYPE_UNKNOWN;
    return(INST_ERR_OK);
  }

  /* set version */
  type = INST_WINDOWSTYPE_UNKNOWN;

  /* update these fields */
  switch (verInfo.dwPlatformId) {
    case VER_PLATFORM_WIN32s:
      type = INST_WINDOWSTYPE_31;
      break;

    case VER_PLATFORM_WIN32_WINDOWS:
      if (verInfo.dwMajorVersion == 4 && verInfo.dwMinorVersion == 0) {
        type = INST_WINDOWSTYPE_95;
      } else if (verInfo.dwMajorVersion == 4 && verInfo.dwMinorVersion == 10) {
        type = INST_WINDOWSTYPE_98;
      } else if (verInfo.dwMajorVersion == 4 && verInfo.dwMinorVersion == 90) {
        type = INST_WINDOWSTYPE_ME;
      }
      break;

    case VER_PLATFORM_WIN32_NT:
      if (verInfo.dwMajorVersion == 3 && verInfo.dwMinorVersion == 50) {
        type = INST_WINDOWSTYPE_NT350;
      } else if (verInfo.dwMajorVersion == 3 && verInfo.dwMinorVersion == 51) {
        type = INST_WINDOWSTYPE_NT351;
      } else if (verInfo.dwMajorVersion == 4) {
        type = INST_WINDOWSTYPE_NT4;
      } else if (verInfo.dwMajorVersion == 5 && verInfo.dwMinorVersion == 0) {
        type = INST_WINDOWSTYPE_2000;
      } else if (verInfo.dwMajorVersion == 5 && verInfo.dwMinorVersion == 1) {
        type = INST_WINDOWSTYPE_XP;
      } else if (verInfo.dwMajorVersion == 5 && verInfo.dwMinorVersion == 2) {
        type = INST_WINDOWSTYPE_2003;
      } else if (verInfo.dwMajorVersion == 6 && verInfo.dwMinorVersion == 0) {
        type = INST_WINDOWSTYPE_2008;
      } else if (verInfo.dwMajorVersion == 6 && verInfo.dwMinorVersion == 1) {
        type = INST_WINDOWSTYPE_7;
      }
      break;
  }

  if (ptype != NULL)
    *ptype = type;

  return(INST_ERR_OK);
}

/*-------------------------------------------------------------*/
/* wait a key press */
static inst_err inst_kbd_pause(inst_string msg)
{
  int c;
  BOOL err;

  printf("%s", msg);

  c = _getch();
  if (c == 0x00 || c == 0xE0) {
    _getch();
  } else if (c == 0x03) {
    err = GenerateConsoleCtrlEvent(CTRL_C_EVENT, 0);
    if (!err) {
      return(INST_ERR_FUGENERATECONSOLECTRLEVENT);
    }
  }

  printf("\n");

  return(INST_ERR_OK);
}
#define inst_pause_default() inst_pause("Press any key to continue")

/*-------------------------------------------------------------*/
/* ask user a choice */
static inst_err inst_kbd_yes(inst_string question,
                             inst_bool *pyes)
{
  int c;
  BOOL err;

  while (1) {
    printf("%s ? [y/n] ", question);
    c = _getch();
    if (c == 'y' || c == 'Y') {
      if (pyes != NULL) *pyes = INST_TRUE;
      printf("%c\n", c);
      break;
    } else if (c == 'n' || c == 'N') {
      if (pyes != NULL) *pyes = INST_FALSE;
      printf("%c\n", c);
      break;
    } else {
      if (c == 0x03) {
        err = GenerateConsoleCtrlEvent(CTRL_C_EVENT, 0);
        if (!err) {
          return(INST_ERR_FUGENERATECONSOLECTRLEVENT);
        }
      }
      if (c == 0x00 || c == 0xE0) {
        _getche();
      }
      if (isprint(c)) {
        printf("%c\nCharacter '%c' is not valid. ", c, c);
      } else {
        printf("\n");
      }
      printf("Please press 'y' or 'n'.\n");
    }
  }

  return(INST_ERR_OK);
}

/*-------------------------------------------------------------*/
/* ask user a string */
static inst_err inst_kbd_buf(inst_string question,
                             inst_buf *pbuf)
{
  inst_data data;
  char *pc;

  printf("%s: ", question);
  inst_er(inst_buf_wantspace(pbuf, 512, &data));
  pc = fgets((inst_string)data, 512, stdin);
  if (pc != NULL) {
    pbuf->endoffset += strlen((inst_conststring)data);
    while (pbuf->endoffset) {
      if (pbuf->ptr[pbuf->endoffset - 1] == 0x0D ||
          pbuf->ptr[pbuf->endoffset - 1] == 0x0A) {
        pbuf->endoffset--;
      } else {
        break;
      }
    }
  }

  return(INST_ERR_OK);
}

/*-------------------------------------------------------------*/
/* obtain some special directories */
typedef enum {
  INST_DIR_SPECIALTYPE_WINDOWS = 1, /* works everywhere */
  INST_DIR_SPECIALTYPE_SYSTEM, /* works everywhere */
  INST_DIR_SPECIALTYPE_CURDIR, /* works everywhere */
  INST_DIR_SPECIALTYPE_PROGRAMFILES_FUNC, /* IE 5 needed */
  INST_DIR_SPECIALTYPE_PROGRAMFILES_REG, /* works everywhere ? */
  INST_DIR_SPECIALTYPE_PROGRAMFILES,
  INST_DIR_SPECIALTYPE_DESKTOP, /* works everywhere */
  INST_DIR_SPECIALTYPE_STARTPROGRAMS, /* works everywhere */
  INST_DIR_SPECIALTYPE_APPDATA /* IE 4 needed */
} inst_dir_specialtype;
static inst_err inst_dir_special_idl(int type,
                                     inst_buf *pbuf)
{
  inst_data data;
  HRESULT herr;
  BOOL berr;
  LPITEMIDLIST pidl;
  IMalloc *pim;

  herr = SHGetSpecialFolderLocation(NULL, type, &pidl);
  if (herr != NOERROR) {
    return(INST_ERR_NOTFOUND);
  }

  inst_er(inst_buf_wantspace(pbuf, MAX_PATH, &data));
  berr = SHGetPathFromIDList(pidl, (inst_string)data);

  SHGetMalloc(&pim);
  if (pim) {
    pim->Free(pidl);
    pim->Release();
  }
  
  if (berr != TRUE) {
    return(INST_ERR_NOTFOUND);
  }

  pbuf->endoffset += strlen((inst_conststring)data);
  return(INST_ERR_OK);
}
static inst_err inst_dir_special_reg(HKEY root,
                                     inst_conststring subkey,
                                     inst_conststring valuename,
                                     inst_buf *pbuf)
{
  inst_data data;
  HKEY hk;
  LONG err;
  DWORD type, size;

  err = RegOpenKeyEx(root, subkey, 0, KEY_READ, &hk);
  if (err != ERROR_SUCCESS) {
    return(INST_ERR_NOTFOUND);
  }

  err = RegQueryValueEx(hk, valuename, NULL, &type, NULL, &size);
  if (err != ERROR_SUCCESS) {
    RegCloseKey(hk);
    return(INST_ERR_NOTFOUND);
  }
  if (type != REG_SZ) {
    RegCloseKey(hk);
    return(INST_ERR_NOTFOUND);
  }

  inst_er(inst_buf_wantspace(pbuf, size, &data));
  err = RegQueryValueEx(hk, valuename, NULL, &type, data, &size);
  if (err != ERROR_SUCCESS) {
    RegCloseKey(hk);
    return(INST_ERR_NOTFOUND);
  }
  pbuf->endoffset += size - 1;

  RegCloseKey(hk);

  return(INST_ERR_OK);
}
static inst_err inst_dir_special(inst_dir_specialtype type,
                                 inst_buf *pbuf)
{
  inst_data data;
  inst_err ret;
  UINT err;

  switch(type) {
  case INST_DIR_SPECIALTYPE_SYSTEM :
    err = GetSystemDirectory(NULL, 0);
    if (err == 0) {
      return(INST_ERR_NOTFOUND);
    }
    inst_er(inst_buf_wantspace(pbuf, err, &data));
    err = GetSystemDirectory((inst_string)data, err);
    if (err == 0) {
      return(INST_ERR_NOTFOUND);
    }
    pbuf->endoffset += err;
    break;
  case INST_DIR_SPECIALTYPE_CURDIR :
    err = GetCurrentDirectory(0, NULL);
    if (err == 0) {
      return(INST_ERR_NOTFOUND);
    }
    inst_er(inst_buf_wantspace(pbuf, err, &data));
    err = GetCurrentDirectory(err, (inst_string)data);
    if (err == 0) {
      return(INST_ERR_NOTFOUND);
    }
    pbuf->endoffset += err;
    break;
  case INST_DIR_SPECIALTYPE_WINDOWS :
    err = GetWindowsDirectory(NULL, 0);
    if (err == 0) {
      return(INST_ERR_NOTFOUND);
    }
    inst_er(inst_buf_wantspace(pbuf, err, &data));
    err = GetWindowsDirectory((inst_string)data, err);
    if (err == 0) {
      return(INST_ERR_NOTFOUND);
    }
    pbuf->endoffset += err;
    break;
  case INST_DIR_SPECIALTYPE_PROGRAMFILES_FUNC :
    #ifndef CSIDL_PROGRAM_FILES
     #define CSIDL_PROGRAM_FILES 0x26
    #endif
    inst_er(inst_dir_special_idl(CSIDL_PROGRAM_FILES, pbuf));
    break;
  case INST_DIR_SPECIALTYPE_PROGRAMFILES_REG :
    inst_er(inst_dir_special_reg(HKEY_LOCAL_MACHINE,
                              "Software\\Microsoft\\Windows\\CurrentVersion",
                              "ProgramFilesDir", pbuf));
    break;
  case INST_DIR_SPECIALTYPE_PROGRAMFILES :
    ret = inst_dir_special(INST_DIR_SPECIALTYPE_PROGRAMFILES_FUNC, pbuf);
    if (ret != INST_ERR_OK) {
      inst_er(inst_dir_special(INST_DIR_SPECIALTYPE_PROGRAMFILES_REG, pbuf));
    }
    break;
  case INST_DIR_SPECIALTYPE_DESKTOP :
    ret = inst_dir_special_idl(CSIDL_COMMON_DESKTOPDIRECTORY, pbuf);
    if (ret != INST_ERR_OK) {
      inst_er(inst_dir_special_idl(CSIDL_DESKTOP, pbuf));
    }
    break;
  case INST_DIR_SPECIALTYPE_STARTPROGRAMS :
    ret = inst_dir_special_idl(CSIDL_COMMON_PROGRAMS, pbuf);
    if (ret != INST_ERR_OK) {
      inst_er(inst_dir_special_idl(CSIDL_PROGRAMS, pbuf));
    }
    break;
  case INST_DIR_SPECIALTYPE_APPDATA :
    #ifndef CSIDL_COMMON_APPDATA
     #define CSIDL_COMMON_APPDATA 0x23
    #endif
    inst_er(inst_dir_special_idl(CSIDL_COMMON_APPDATA, pbuf));
    break;
  default :
    return(INST_ERR_PAINVALIDTYPE);
  }  

  return(INST_ERR_OK);
}

/*-------------------------------------------------------------*/
/* eventually create parent directories (this function is a bit
   simplified, but it should work) */
static inst_err inst_dir_parents_(inst_buf *ppath,
                                  inst_uint count)
{
  inst_buf buf;
  inst_string str, pc;
  struct _stat st;
  int err;

  inst_er(inst_buf_init(&buf));
  inst_er(inst_buf_append_buf(ppath, &buf));

  inst_er(inst_buf_ref_string(&buf, &str));
  pc = strrchr(str, '\\');
  if (pc == NULL) {
    inst_er(inst_buf_close(&buf));
    return(INST_ERR_OK);
  }
  *pc = '\0';

  err = _stat(str, &st);
  if (err) {
    inst_er(inst_dir_parents_(&buf, count+1));
  }
  inst_er(inst_buf_close(&buf));

  if (count) {
    inst_er(inst_buf_ref_string(ppath, &str));
    err = _mkdir(str);
    if (err) {
      return(INST_ERR_FUMKDIR);
    }
  }

  return(INST_ERR_OK);
}
#define inst_dir_parents(pbuffile) inst_dir_parents_(pbuffile,0)

/*-------------------------------------------------------------*/
/* remove a file or a directory */
static inst_err inst_path_del(inst_buf *ppath)
{
  inst_string str;
  SHFILEOPSTRUCT fileop;

  fileop.hwnd = NULL;
  fileop.wFunc = FO_DELETE;
  fileop.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;

  inst_er(inst_buf_ref_string2(ppath, &str));
  fileop.pFrom = str;
  fileop.pTo = NULL;

  /*ignore*/SHFileOperation(&fileop);

  return(INST_ERR_OK);
}

/*-------------------------------------------------------------*/
/* copy a file or a directory */
static inst_err inst_path_copy(inst_buf *pfrom,
                               inst_buf *pto)
{
  inst_string str;
  int err;
  SHFILEOPSTRUCT fileop;

  /* eventually create parents directories */
  inst_er(inst_dir_parents(pto));

  /* to permit double installation */
  inst_er(inst_path_del(pto));

  fileop.hwnd = NULL;
  fileop.wFunc = FO_COPY;
  fileop.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI | FOF_SILENT;

  inst_er(inst_buf_ref_string2(pfrom, &str));
  fileop.pFrom = str;
  inst_er(inst_buf_ref_string2(pto, &str));
  fileop.pTo = str;

  err = SHFileOperation(&fileop);
  if (err) {
    return(INST_ERR_FUSHFILEOPERATION);
  }

  return(INST_ERR_OK);
}

/*-------------------------------------------------------------*/
/* convert a path using 8.3 filenames, and replace \ by / */
static inst_err inst_path_83sl(inst_buf *pfrom,
                               inst_buf *pto)
{
  inst_buf buf;
  inst_string str;
  inst_data data, pc;
  DWORD err;

  inst_er(inst_buf_ref_string2(pfrom, &str));
  inst_er(inst_buf_init(&buf));
  inst_er(inst_buf_wantspace(&buf, strlen(str) + 1, &data));
  err = GetShortPathName(str, (inst_string)data, strlen(str) + 1);
  if (err == 0) {
    inst_er(inst_buf_close(&buf));
    return(INST_ERR_FUGETSHORTPATHNAME);
  }

  pc = data;
  while(*pc != '\0') {
    if (*pc == '\\') {
      *pc = '/';
    }
    pc++;
  }

  inst_er(inst_buf_append_string((inst_string)data, pto));

  inst_er(inst_buf_close(&buf));
  return(INST_ERR_OK);
}

/*-------------------------------------------------------------*/
/* create a shortcut */
inst_err inst_path_shortcut(inst_buf *pshortcut,
                            inst_buf *ptarget,
                            inst_buf *parg,
                            inst_buf *pworkdir,
                            inst_buf *picon,
                            inst_conststring desc)
{
  inst_string str;
  inst_buf unicod;
  HRESULT hres;
  IShellLink *pisl;
  IPersistFile *pipf;

  inst_er(inst_dir_parents(pshortcut));
  inst_er(inst_path_del(pshortcut));

  hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, 
                          IID_IShellLink, (LPVOID*)&pisl); 
  if (SUCCEEDED(hres)) {
    inst_er(inst_buf_ref_string(ptarget, &str));
    pisl->SetPath(str);
    if (parg != NULL) {
      inst_er(inst_buf_ref_string(parg, &str));
      pisl->SetArguments(str);
    }
    if (pworkdir != NULL) {
      inst_er(inst_buf_ref_string(pworkdir, &str));
      pisl->SetWorkingDirectory(str);
    }
    if (picon != NULL) {
      inst_er(inst_buf_ref_string(picon, &str));
      pisl->SetIconLocation(str, 0);
    }
    if (desc != NULL) {
      pisl->SetDescription(desc);
    }
    hres = pisl->QueryInterface(IID_IPersistFile, (LPVOID*)&pipf);
    if (SUCCEEDED(hres)) {
      inst_er(inst_buf_ref_string(pshortcut, &str));
      inst_er(inst_buf_init(&unicod));
      inst_er(inst_buf_wantspace(&unicod, 2*strlen(str)+2, NULL));
      MultiByteToWideChar(CP_ACP, 0, str, -1, (WCHAR*)unicod.ptr, unicod.totalsize);
      hres = pipf->Save((WCHAR*)unicod.ptr, TRUE);
      inst_er(inst_buf_close(&unicod));
      pipf->Release();
    } 
    pisl->Release(); 
  }

  if (!SUCCEEDED(hres)) {
    return(INST_ERR_FUSHORTCUT);
  }

  return(INST_ERR_OK);
}


/*-------------------------------------------------------------*/
/***************************************************************
 * Now, specific functions                                     *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* global buffer containing version (ie 518 for 5.18.x) */
inst_buf gloversion;
/* global buffer containing install directory */
inst_buf gloinstalldir;
/* global buffer containing netwox.exe, and netwag.tcl */
inst_buf glonetwox;
inst_buf glonetwag;
/* initialize global variables */
inst_err inst_path_global_init(void)
{
  inst_buf path, msg;
  inst_string str;
  inst_data data;
  inst_bool yes;
  inst_err ret;
  DWORD readsize;
  HANDLE hFile;
  BOOL err;

  puts("Setting global variables.");

  /* init */
  inst_er(inst_buf_init(&gloversion));
  inst_er(inst_buf_init(&gloinstalldir));
  inst_er(inst_buf_init(&glonetwox));
  inst_er(inst_buf_init(&glonetwag));

  /* reading version (not very nice, but it works) */
  puts(" Version...");
  inst_er(inst_buf_init(&path));
  inst_er(inst_dir_special(INST_DIR_SPECIALTYPE_CURDIR, &path));
  inst_er(inst_buf_append_string("\\installwindows_src\\installwindows.conf",
                                 &path));
  inst_er(inst_buf_ref_string(&path, &str));
  hFile = CreateFile(str, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  inst_er(inst_buf_close(&path));
  if (hFile == INVALID_HANDLE_VALUE) {
    puts("Cannot find installwindows.conf");
    return(INST_ERR_NOTFOUND);
  }
  inst_er(inst_buf_wantspace(&gloversion, 10, &data));
  err = ReadFile(hFile, data, 10, &readsize, NULL);
  CloseHandle(hFile);
  if (err == FALSE || readsize < 3) {
    puts("File installwindows.conf is badly formatted");
    return(INST_ERR_NOTFOUND);
  }
  gloversion.endoffset = 3;
  inst_er(inst_buf_init(&msg));
  inst_er(inst_buf_append_string(" Version=", &msg));
  inst_er(inst_buf_append_buf(&gloversion, &msg));
  inst_er(inst_buf_append_string("\n", &msg));
  inst_er(inst_buf_display(&msg));
  inst_er(inst_buf_close(&msg));

  /* setting installation root directory */
  puts(" InstallDir...");
  ret = inst_dir_special(INST_DIR_SPECIALTYPE_PROGRAMFILES, &gloinstalldir);
  if (ret != INST_ERR_OK) {
    inst_er(inst_buf_append_string("C:\\Program Files", &gloinstalldir));
  }
  inst_er(inst_buf_append_string("\\netw\\netw", &gloinstalldir));
  inst_er(inst_buf_append_buf(&gloversion, &gloinstalldir));
  inst_er(inst_buf_init(&msg));
  inst_er(inst_buf_append_string(" InstallDir=", &msg));
  inst_er(inst_buf_append_buf(&gloinstalldir, &msg));
  inst_er(inst_buf_append_string("\n", &msg));
  inst_er(inst_buf_display(&msg));
  inst_er(inst_buf_close(&msg));

  /* eventually change installation directory */
  inst_er(inst_kbd_yes("Do you agree to use this installation directory", &yes));
  if (yes == INST_FALSE) {
    gloinstalldir.endoffset = 0;
    inst_er(inst_kbd_buf("Enter directory", &gloinstalldir));
    /* no error check: will create it if it does not exists */
  }

  /* setting other variables */
  inst_er(inst_buf_append_buf(&gloinstalldir, &glonetwox));
  inst_er(inst_buf_append_string("\\netwox", &glonetwox));
  inst_er(inst_buf_append_buf(&gloversion, &glonetwox));
  inst_er(inst_buf_append_string(".exe", &glonetwox));
  inst_er(inst_buf_append_buf(&gloinstalldir, &glonetwag));
  inst_er(inst_buf_append_string("\\netwag", &glonetwag));
  inst_er(inst_buf_append_buf(&gloversion, &glonetwag));
  inst_er(inst_buf_append_string(".tcl", &glonetwag));

  return(INST_ERR_OK);
}

/*-------------------------------------------------------------*/
/* patch (currently, only netwag is supported) */
static inst_err inst_path_patch_var(FILE *pfout)
{
  inst_buf buf;
  inst_err ret;

  inst_er(inst_buf_init(&buf));

  /* temporary directory */
  inst_er(inst_buf_append_string("set netwag_glo_dir_tmp ", &buf));
  inst_er(inst_buf_append_string("\"$env(TEMP)\"\r\n", &buf));

  /* session file. We use %TEMP% and %APPDATA% because :
     CSIDL_COMMON_APPDATA returns :
      C:\Documents and Settings\All Users\Application Data
      => shared between all users => privacy/file_right problem
     CSIDL_APPDATA returns :
      C:\Documents and Settings\[Installing_user]\Application Data
      => other users cannot use it
     So, shell variables automatically adapts, whereas others are fixed. */
  inst_er(inst_buf_append_string("if {[info exists env(APPDATA)]} {\r\n", &buf));
  inst_er(inst_buf_append_string("  set netwag_glo_file_session ", &buf));
  inst_er(inst_buf_append_string("\"$env(APPDATA)/netwag.ses\"\r\n", &buf));
  inst_er(inst_buf_append_string("} else {\r\n", &buf));
  inst_er(inst_buf_append_string("  set netwag_glo_file_session ", &buf));
  inst_er(inst_buf_append_string("\"$env(TEMP)/netwag.ses\"\r\n", &buf));
  inst_er(inst_buf_append_string("}\r\n", &buf));

  /* netwox binary */
  inst_er(inst_buf_append_string("set netwag_glo_bin_netwox \"", &buf));
  ret = inst_path_83sl(&gloinstalldir, &buf);
  if (ret == INST_ERR_OK) {
    inst_er(inst_buf_append_string("/", &buf));
  }
  inst_er(inst_buf_append_string("netwox", &buf));
  inst_er(inst_buf_append_buf(&gloversion, &buf));
  inst_er(inst_buf_append_string(".exe\"\r\n", &buf));

  /* write */
  if (fwrite(buf.ptr, buf.endoffset, 1, pfout) < 0) {
    return(INST_ERR_PATCH);
  }

  inst_er(inst_buf_close(&buf));
  return(INST_ERR_OK);
}
static inst_err inst_path_patch(inst_buf *pfrom,
                                inst_buf *pto)
{
  inst_string from, to;
  inst_data data;
  inst_buf buf;
  inst_uint patchlevel;
  FILE *pfin, *pfout;
  char *pc;

  inst_er(inst_buf_ref_string2(pfrom, &from));
  inst_er(inst_buf_ref_string2(pto, &to));

  pfin = fopen(from, "r");
  if (pfin == NULL) {
    return(INST_ERR_PATCH);
  }
  pfout = fopen(to, "w");
  if (pfout == NULL) {
    return(INST_ERR_PATCH);
  }

  inst_er(inst_buf_init(&buf));
  inst_er(inst_buf_wantspace(&buf, 1024, &data));
  patchlevel = 0;
  while(1) {
    pc = fgets((inst_string)data, 1024, pfin);
    if (pc == NULL) {
      break;
    }
    if (patchlevel == 0 && data[0] != '#') {
      patchlevel = 1;
      inst_er(inst_path_patch_var(pfout));
    } else if (patchlevel == 1 && data[0] == '#') {
      patchlevel = 2;
    }
    if (patchlevel != 1) {
      if (fputs((inst_string)data, pfout) < 0) {
        return(INST_ERR_PATCH);
      }
    }
  }
  inst_er(inst_buf_close(&buf));

  fclose(pfin);
  fclose(pfout);
  return(INST_ERR_OK);
}

/*-------------------------------------------------------------*/
/* copy from current_directory to program_file */
inst_err inst_path_copy_cd_pf(inst_buf *pfrom,
                              inst_buf *pto,
                              inst_bool patch)
{
  inst_buf from, to, msg;
  inst_err ret=INST_ERR_OK;

  inst_er(inst_buf_init(&msg));
  inst_er(inst_buf_append_string(" ", &msg));
  inst_er(inst_buf_append_buf(pfrom, &msg));
  inst_er(inst_buf_append_string("\n", &msg));
  inst_er(inst_buf_display(&msg));
  inst_er(inst_buf_close(&msg));

  inst_er(inst_buf_init(&from));
  inst_er(inst_dir_special(INST_DIR_SPECIALTYPE_CURDIR, &from));
  inst_er(inst_buf_append_string("\\", &from));
  inst_er(inst_buf_append_buf(pfrom, &from));

  inst_er(inst_buf_init(&to));
  inst_er(inst_buf_append_buf(&gloinstalldir, &to));
  inst_er(inst_buf_append_string("\\", &to));
  inst_er(inst_buf_append_buf(pto, &to));

  if (patch) {
    ret = inst_path_patch(&from, &to);
  } else {
    ret = inst_path_copy(&from, &to);
  }
  if (ret != INST_ERR_OK) {
    puts("This copy failed");
    return(ret);
  }

  inst_er(inst_buf_close(&to));
  inst_er(inst_buf_close(&from));

  return(INST_ERR_OK);
}

/*-------------------------------------------------------------*/
/* copy from current_directory to program_file */
inst_err inst_path_copy_cd_pf_all(void)
{
  inst_buf msg, from, to;
  inst_windowstype windowstype;

  inst_er(inst_buf_init(&msg));
  inst_er(inst_buf_append_string("Copying files under ", &msg));
  inst_er(inst_buf_append_buf(&gloinstalldir, &msg));
  inst_er(inst_buf_append_string("\n", &msg));
  inst_er(inst_buf_display(&msg));
  inst_er(inst_buf_close(&msg));

  inst_er(inst_buf_init(&from));
  inst_er(inst_buf_init(&to));

  /* netwib5xy.dll */
  inst__buf_reinit(&from);
  inst_er(inst_buf_append_string("src\\netwox-bin_windows\\netwib", &from));
  inst_er(inst_buf_append_buf(&gloversion, &from));
  inst_er(inst_buf_append_string(".dll", &from));
  inst__buf_reinit(&to);
  inst_er(inst_buf_append_string("netwib", &to));
  inst_er(inst_buf_append_buf(&gloversion, &to));
  inst_er(inst_buf_append_string(".dll", &to));
  inst_er(inst_path_copy_cd_pf(&from, &to, INST_FALSE));

  /* netwox5xy.exe */
  inst__buf_reinit(&from);
  inst_er(inst_buf_append_string("src\\netwox-bin_windows\\netwox", &from));
  inst_er(inst_buf_append_buf(&gloversion, &from));
  inst_er(inst_buf_append_string(".exe", &from));
  inst__buf_reinit(&to);
  inst_er(inst_buf_append_string("netwox", &to));
  inst_er(inst_buf_append_buf(&gloversion, &to));
  inst_er(inst_buf_append_string(".exe", &to));
  inst_er(inst_path_copy_cd_pf(&from, &to, INST_FALSE));

  /* netwag5xy.tcl */
  inst__buf_reinit(&from);
  inst_er(inst_buf_append_string("src\\netwag-bin_windows\\netwag", &from));
  inst_er(inst_buf_append_buf(&gloversion, &from));
  inst_er(inst_buf_append_string(".tcl", &from));
  inst__buf_reinit(&to);
  inst_er(inst_buf_append_string("netwag", &to));
  inst_er(inst_buf_append_buf(&gloversion, &to));
  inst_er(inst_buf_append_string(".tcl", &to));
  inst_er(inst_path_copy_cd_pf(&from, &to, INST_TRUE));

  /* netwib compil */
  inst__buf_reinit(&from);
  inst_er(inst_buf_append_string("src\\netwib-bin_windows\\dll", &from));
  inst__buf_reinit(&to);
  /* Note : Windows NT 4.0 needs "netwib\\" instead of "netwib\\dll".
     Windows 7 needs "netwib\\dll". I'm unsure for other systems. */
  inst_er(inst_windowstype_init(&windowstype));
  switch(windowstype) {
  case INST_WINDOWSTYPE_NT4 :
    inst_er(inst_buf_append_string("netwib\\", &to));
    break;
  default :
    inst_er(inst_buf_append_string("netwib\\dll", &to));
  }
  inst_er(inst_path_copy_cd_pf(&from, &to, INST_FALSE));
  inst__buf_reinit(&from);
  inst_er(inst_buf_append_string("src\\netwib-bin_windows\\include", &from));
  inst__buf_reinit(&to);
  switch(windowstype) {
  case INST_WINDOWSTYPE_NT4 :
    inst_er(inst_buf_append_string("netwib\\", &to));
    break;
  default :
    inst_er(inst_buf_append_string("netwib\\include", &to));
  }
  inst_er(inst_path_copy_cd_pf(&from, &to, INST_FALSE));
  inst__buf_reinit(&from);
  inst_er(inst_buf_append_string("src\\netwib-bin_windows\\lib", &from));
  inst__buf_reinit(&to);
  switch(windowstype) {
  case INST_WINDOWSTYPE_NT4 :
    inst_er(inst_buf_append_string("netwib\\", &to));
    break;
  default :
    inst_er(inst_buf_append_string("netwib\\lib", &to));
  }
  inst_er(inst_path_copy_cd_pf(&from, &to, INST_FALSE));

  /* doc */
  inst__buf_reinit(&from);
  inst_er(inst_buf_append_string("src\\netwib-doc_html\\*", &from));
  inst__buf_reinit(&to);
  inst_er(inst_buf_append_string("doc\\netwib\\", &to));
  inst_er(inst_path_copy_cd_pf(&from, &to, INST_FALSE));
  inst__buf_reinit(&from);
  inst_er(inst_buf_append_string("src\\netwox-doc_html\\*", &from));
  inst__buf_reinit(&to);
  inst_er(inst_buf_append_string("doc\\netwox\\", &to));
  inst_er(inst_path_copy_cd_pf(&from, &to, INST_FALSE));
  inst__buf_reinit(&from);
  inst_er(inst_buf_append_string("src\\netwag-doc_html\\*", &from));
  inst__buf_reinit(&to);
  inst_er(inst_buf_append_string("doc\\netwag\\", &to));
  inst_er(inst_path_copy_cd_pf(&from, &to, INST_FALSE));

  inst_er(inst_buf_close(&from));
  inst_er(inst_buf_close(&to));

  return(INST_ERR_OK);
}

/*-------------------------------------------------------------*/
/* create shortcuts on desktop */
inst_err inst_shortcuts(inst_bool desktop,
                        inst_bool start)
{
  inst_buf shortcut, shortcutstart, target, workdir, arg, icon;
  inst_windowstype windowstype;

  inst_er(inst_buf_init(&shortcut));
  inst_er(inst_buf_init(&shortcutstart));
  inst_er(inst_buf_init(&target));
  inst_er(inst_buf_init(&workdir));
  inst_er(inst_buf_init(&arg));
  inst_er(inst_buf_init(&icon));

  inst_er(inst_dir_special(INST_DIR_SPECIALTYPE_STARTPROGRAMS, &shortcutstart));
  inst_er(inst_buf_append_string("\\Netwox Netwag", &shortcutstart));

  inst_er(inst_windowstype_init(&windowstype));
  switch(windowstype) {
  case INST_WINDOWSTYPE_NT4 :
  case INST_WINDOWSTYPE_2000 :
  case INST_WINDOWSTYPE_XP :
  case INST_WINDOWSTYPE_2003 :
  case INST_WINDOWSTYPE_2008 :
  case INST_WINDOWSTYPE_7 :
    /* We use cmd so when the user exits, he is inside the shell,
       so he can easilly restart netwox. */
    inst_er(inst_buf_append_string("cmd", &target));
    inst_er(inst_buf_append_string("/k \"", &arg));
    inst_er(inst_buf_append_buf(&glonetwox, &arg));
    inst_er(inst_buf_append_string("\"", &arg));
    inst_er(inst_buf_append_buf(&gloinstalldir, &workdir));
    break;
  default :
    inst_er(inst_buf_append_buf(&glonetwox, &target));
    inst_er(inst_buf_append_string("C:/", &workdir));
    break;
  }
  if (desktop) {
    inst__buf_reinit(&shortcut);
    inst_er(inst_dir_special(INST_DIR_SPECIALTYPE_DESKTOP, &shortcut));
    inst_er(inst_buf_append_string("\\Netwox", &shortcut));
    inst_er(inst_buf_append_buf(&gloversion, &shortcut));
    inst_er(inst_buf_append_string(".lnk", &shortcut));
    inst_er(inst_path_shortcut(&shortcut, &target, &arg, &workdir,
                               &glonetwox, "Network toolbox"));
  }
  if (start) {
    inst__buf_reinit(&shortcut);
    inst_er(inst_buf_append_buf(&shortcutstart, &shortcut));
    inst_er(inst_buf_append_string("\\Netwox", &shortcut));
    inst_er(inst_buf_append_buf(&gloversion, &shortcut));
    inst_er(inst_buf_append_string(".lnk", &shortcut));
    inst_er(inst_path_shortcut(&shortcut, &target, &arg, &workdir,
                               &glonetwox, "Network toolbox"));
  }

  inst__buf_reinit(&workdir);
  inst_er(inst_buf_append_string("C:/", &workdir));
  if (desktop) {
    inst__buf_reinit(&shortcut);
    inst_er(inst_dir_special(INST_DIR_SPECIALTYPE_DESKTOP, &shortcut));
    inst_er(inst_buf_append_string("\\Netwag", &shortcut));
    inst_er(inst_buf_append_buf(&gloversion, &shortcut));
    inst_er(inst_buf_append_string(".lnk", &shortcut));
    inst_er(inst_path_shortcut(&shortcut, &glonetwag, NULL, &workdir,
                               &glonetwox, "Network toolbox"));
  }
  if (start) {
    inst__buf_reinit(&shortcut);
    inst_er(inst_buf_append_buf(&shortcutstart, &shortcut));
    inst_er(inst_buf_append_string("\\Netwag", &shortcut));
    inst_er(inst_buf_append_buf(&gloversion, &shortcut));
    inst_er(inst_buf_append_string(".lnk", &shortcut));
    inst_er(inst_path_shortcut(&shortcut, &glonetwag, NULL, &workdir,
                               &glonetwox, "Network toolbox"));
  }

  if (!start) {
    return(INST_ERR_OK);
  }

  inst__buf_reinit(&shortcut);
  inst__buf_reinit(&target);
  inst_er(inst_buf_append_buf(&shortcutstart, &shortcut));
  inst_er(inst_buf_append_string("\\Netwox documentation.lnk", &shortcut));
  inst_er(inst_buf_append_buf(&gloinstalldir, &target));
  inst_er(inst_buf_append_string("\\doc\\netwox\\index.html", &target));
  inst_er(inst_path_shortcut(&shortcut, &target, NULL, NULL, NULL,
                             "Netwox HTML documentation"));

  inst__buf_reinit(&shortcut);
  inst__buf_reinit(&target);
  inst_er(inst_buf_append_buf(&shortcutstart, &shortcut));
  inst_er(inst_buf_append_string("\\Netwag documentation.lnk", &shortcut));
  inst_er(inst_buf_append_buf(&gloinstalldir, &target));
  inst_er(inst_buf_append_string("\\doc\\netwag\\index.html", &target));
  inst_er(inst_path_shortcut(&shortcut, &target, NULL, NULL, NULL,
                             "Netwag HTML documentation"));

  return(INST_ERR_OK);
}

/*-------------------------------------------------------------*/
/***************************************************************
 * Main program                                                *
 ***************************************************************/

/*-------------------------------------------------------------*/
static inst_err main2(void)
{
  inst_bool yes, desktop, start;

  puts("This program will install netwib, netwox and netwag on your system.");
  inst_er(inst_kbd_yes("Do you agree", &yes));
  if (yes == INST_FALSE) {
    return(INST_ERR_OK);
  }

  inst_er(inst_path_global_init());

  inst_er(inst_path_copy_cd_pf_all());

  inst_er(inst_kbd_yes("Do you agree to place shortcuts on desktop", &desktop));
  inst_er(inst_kbd_yes("Do you agree to place shortcuts in start menu", &start));
  if (desktop || start) {
    inst_er(inst_shortcuts(desktop, start));
  }

  return(INST_ERR_OK);
}

/*-------------------------------------------------------------*/
int main(void)
{
  inst_err ret;

  CoInitialize(NULL);

  ret = main2();
  if (ret != INST_ERR_OK) {
    printf("Error number %d occurred\n", ret);
  }

  inst_er(inst_kbd_pause("Press any key to terminate"));
  CoUninitialize();

  return(ret); 
}
