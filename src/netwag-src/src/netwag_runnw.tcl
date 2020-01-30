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


#################################################################
# PRIVATE FUNCTIONS
#################################################################


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Run a netwox tool in a new window
proc netwag_runnw_run { cmd } {
  global netwag_glo_bin_netwox
  global netwag_sys_platform
  global netwag_sys_platform_unix
  global netwag_sys_platform_windows
  global netwag_glo_bin_term
  global netwag_glo_bin_term_param
  global env
  global netwag_sys_os;
  global netwag_sys_os_windows95
  global netwag_sys_os_windows98
  global netwag_sys_os_windowsme

  netwag_runcmd_file_init $cmd tmpfilename

  if {$netwag_sys_platform == $netwag_sys_platform_unix} {
    # we use "sh -c" in case netwox is not in the path
    exec $netwag_glo_bin_term $netwag_glo_bin_term_param /bin/sh -c "$netwag_glo_bin_netwox 0 -R -b $tmpfilename" &
  } elseif {$netwag_sys_platform == $netwag_sys_platform_windows} {
    # we use "$env(COMSPEC) /c" in case netwox is not in the path
    if { $netwag_sys_os==$netwag_sys_os_windows95 || $netwag_sys_os==$netwag_sys_os_windows98 || $netwag_sys_os==$netwag_sys_os_windowsme } {
      exec start $env(COMSPEC) /c $netwag_glo_bin_netwox 0 -R -b $tmpfilename &
    } else {
      # because start is not an external command
      exec $env(COMSPEC) /c start $env(COMSPEC) /c $netwag_glo_bin_netwox 0 -R -b $tmpfilename &
    }
  } else {
    netwag_msg_add_err "I don't know how to run a command on this platform"
  }

  # eventually remove command filename
  after 5000 "netwag_runcmd_file_del $tmpfilename"

  return 0
}
