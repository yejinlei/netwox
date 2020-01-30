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

## Complete exec of a command
# Example :
#   set ret [netwag_exec "/bin/ls" outmsg retval errmsg]
#   if { $ret } {
#     puts "$errmsg"
#     exit
#   } elseif { $retval } {
#     puts "process returned : $retval"
#     exit
#   }
#   puts $outmsg
proc netwag_exec { cmd poutmsg pretval perrmsg } {
  upvar $poutmsg outmsg
  upvar $pretval retval
  upvar $perrmsg errmsg

  set outmsg ""
  netwag_cursor_wait
  set ret [netwag_popen_init $cmd "r" fd errmsg]
  if { $ret } {
    netwag_cursor_default
    return $ret
  }

  while {1} {
    set data [read $fd 16383]
    if { [string length $data] == 0 && [eof $fd] } {
      break
    }
    append outmsg "$data"
  }

  set ret [netwag_popen_close $fd retval errmsg]
  netwag_cursor_default
  return $ret
}
