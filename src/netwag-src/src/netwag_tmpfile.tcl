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
global netwag_tmpfile_root ; set netwag_tmpfile_root ""


#################################################################
# PRIVATE FUNCTIONS
#################################################################


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Create a temporary file
# usage :
#set f [netwag_tmpfile_open filename]
#puts "$filename open"
#puts $f "blabla"
#close $f
proc netwag_tmpfile_open { {pfilename ""} } {
  global netwag_glo_dir_tmp
  global netwag_tmpfile_root

  if { $netwag_tmpfile_root == "" } {
    set netwag_tmpfile_root $netwag_glo_dir_tmp
    append netwag_tmpfile_root "/netwag."
  }

  set fn $netwag_tmpfile_root

  if {"$pfilename" != ""} {
    upvar $pfilename filename
  }

  while {1} {
    set randnum [netwag_misc_bigrand]
    set filename $fn$randnum
    set ret [catch {open $filename {WRONLY EXCL CREAT} 0600} fd]
    if { ! $ret } {
      return $fd
    }
  }
  return -1
}

## Delete the temporary file
proc netwag_tmpfile_del { filename {warnifabsent 1} } {
  global netwag_tmpfile_root

  if { $netwag_tmpfile_root == "" } {
    netwag_msg_add_err "netwag_tmpfile_del called to delete $filename, but netwag_tmpfile_root is unset"
    return
  }

  if { ! [regexp "^$netwag_tmpfile_root\[0-9\]*$" $filename] } {
    netwag_msg_add_err "netwag_tmpfile_del called to delete $filename, which was not created by netwag_tmpfile_open"
    return
  }

  file delete -- $filename
}

