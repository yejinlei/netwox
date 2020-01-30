#################################################################
#                             NETWAG                            #
#                        Network toolbox                        #
#           Copyright(c) 1999-2012 Laurent CONSTANTIN           #
#                 http://ntwag.sourceforge.net/                 #
#                   laurentconstantin@free.fr                   #
#                             -----                             #
#  This file is part of Netwag.                                 #
#  Netwag is free software: you can redistribute it and/or      #
#  modify it under the terms of the GNU General Public License  #
#  version 3 as published by the Free Software Foundation.      #
#  Netwag is distributed in the hope that it will be useful,    #
#  but WITHOUT ANY WARRANTY; without even the implied warranty  #
#  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See #
#  the GNU General Public License for more details              #
#  (http://www.gnu.org/).                                       #
#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#

#################################################################
# GLOBAL VARIABLES
#################################################################
global netwag_sys_platform;
global netwag_sys_os;

global netwag_sys_platform_unknown; set netwag_sys_platform_unknown 1
global netwag_sys_platform_unix; set netwag_sys_platform_unix 2
global netwag_sys_platform_windows; set netwag_sys_platform_windows 3
global netwag_sys_platform_macintosh; set netwag_sys_platform_macintosh 4

global netwag_sys_os_unknown; set netwag_sys_os_unknown 1
global netwag_sys_os_linux; set netwag_sys_os_linux 2
global netwag_sys_os_freebsd; set netwag_sys_os_freebsd 3
global netwag_sys_os_openbsd; set netwag_sys_os_openbsd 4
global netwag_sys_os_solaris; set netwag_sys_os_solaris 5
global netwag_sys_os_macos; set netwag_sys_os_macos 6
global netwag_sys_os_windows95; set netwag_sys_os_windows95 100
global netwag_sys_os_windows98; set netwag_sys_os_windows98 101
global netwag_sys_os_windowsme; set netwag_sys_os_windowsme 102
global netwag_sys_os_windowsnt4; set netwag_sys_os_windowsnt4 110
global netwag_sys_os_windows2000; set netwag_sys_os_windows2000 111
global netwag_sys_os_windowsxp; set netwag_sys_os_windowsxp 112
global netwag_sys_os_windows2003; set netwag_sys_os_windows2003 113


#################################################################
# PRIVATE FUNCTIONS
#################################################################


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Initialize global variables containing system
proc netwag_sys_init { } {
  global tcl_platform
  global netwag_sys_platform
  global netwag_sys_os
  global netwag_sys_platform_unknown
  global netwag_sys_platform_unix
  global netwag_sys_platform_windows
  global netwag_sys_platform_macintosh
  global netwag_sys_os_unknown
  global netwag_sys_os_linux
  global netwag_sys_os_freebsd
  global netwag_sys_os_openbsd
  global netwag_sys_os_solaris
  global netwag_sys_os_macos
  global netwag_sys_os_windows95
  global netwag_sys_os_windows98
  global netwag_sys_os_windowsme
  global netwag_sys_os_windowsnt4
  global netwag_sys_os_windows2000
  global netwag_sys_os_windowsxp
  global netwag_sys_os_windows2003

  if { $tcl_platform(platform) == "unix" } {
    set netwag_sys_platform $netwag_sys_platform_unix
  } elseif { $tcl_platform(platform) == "windows" } {
    set netwag_sys_platform $netwag_sys_platform_windows
  } elseif { $tcl_platform(platform) == "macintosh" } {
    set netwag_sys_platform $netwag_sys_platform_macintosh
  } else {
    set netwag_sys_platform $netwag_sys_platform_unknown
  }

  set osv $tcl_platform(osVersion)
  set netwag_sys_os $netwag_sys_os_unknown
  if { $tcl_platform(os) == "Linux" } {
    set netwag_sys_os $netwag_sys_os_linux
  } elseif { $tcl_platform(os) == "FreeBSD" } {
    set netwag_sys_os $netwag_sys_os_freebsd
  } elseif { $tcl_platform(os) == "OpenBSD" } {
    set netwag_sys_os $netwag_sys_os_openbsd
  } elseif { $tcl_platform(os) == "Solaris" } {
    set netwag_sys_os $netwag_sys_os_solaris
  } elseif { $tcl_platform(os) == "SunOS" } {
    set netwag_sys_os $netwag_sys_os_solaris
  } elseif { $tcl_platform(os) == "Darwin" } {
    set netwag_sys_os $netwag_sys_os_macos
  } elseif { $tcl_platform(os) == "Windows 95" } {
    set netwag_sys_os $netwag_sys_os_windows95
    if { $osv == "4.0" } {
      set netwag_sys_os $netwag_sys_os_windows95
    } elseif { $osv == "4.10" } {
      set netwag_sys_os $netwag_sys_os_windows98
    } elseif { $osv == "4.90" } {
      set netwag_sys_os $netwag_sys_os_windowsme
    }
  } elseif { $tcl_platform(os) == "Windows NT" } {
    set netwag_sys_os $netwag_sys_os_windows2000
    if { $osv == "4" } {
      set netwag_sys_os $netwag_sys_os_windowsnt4
    } elseif { $osv == "5.0" } {
      set netwag_sys_os $netwag_sys_os_windows2000
    } elseif { $osv == "5.1" } {
      set netwag_sys_os $netwag_sys_os_windowsxp
    } elseif { $osv == "5.2" } {
      set netwag_sys_os $netwag_sys_os_windows2003
    }
  }

}


#################################################################
# INITIALIZE SYSTEM VARIABLES
#################################################################
netwag_sys_init


#################################################################
# CHECK TCL/TK VERSION
#################################################################
proc netwag_glo_check_version { versiontype theversion wantedmajor } {
  # theversion is w.x.y.z...
  set pospoint [string first "." $theversion]
  set versionmajor [string range $theversion 0 [expr $pospoint - 1]]
  if { $versionmajor < $wantedmajor } {
    set errms "Version of $versiontype is too old ($versionmajor<$wantedmajor)"
    puts $errms
    label .l -text $errms
    pack .l
    update idletasks
    after 10000
    exit
  }
}
netwag_glo_check_version "Tk" $tk_version 8
netwag_glo_check_version "Tcl" $tcl_version 8
