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

## Create the temporary file containing the command
proc netwag_runcmd_file_init { cmd ptmpfilename } {
  upvar $ptmpfilename filename

  set fd [netwag_tmpfile_open filename]
  puts $fd "$cmd\n"
  close $fd
}

## Close the temporary file
proc netwag_runcmd_file_del { tmpfilename } {
  netwag_tmpfile_del $tmpfilename 0
}

## Check the command
proc netwag_runcmd_check { pcmd ptoolnum } {
  upvar $pcmd cmd
  upvar $ptoolnum toolnum

  if { $cmd == "" } {
    netwag_msg_add_warn "Command \"$cmd\" too short"
    return -1
  }
  set cmdlist [split $cmd]

  set toolnum [lindex $cmdlist 0]
  if { [regexp {[^0-9]} $toolnum] } {
    netwag_msg_add_warn "The first parameter (\"$toolnum\") must be a number"
    return -1
  }

  return 0
}
