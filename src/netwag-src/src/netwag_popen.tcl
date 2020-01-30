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
global netwag_priv_popen_fdcmd


#################################################################
# PRIVATE FUNCTIONS
#################################################################


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Initialize a popen
proc netwag_popen_init { cmd mode pfd perrmsg } {
  global netwag_priv_popen_fdcmd

  upvar $pfd fd
  upvar $perrmsg errmsg

  if { $mode == "rw" || $mode == "r+" } {
    set acc "r+"
  } elseif { $mode == "r" } {
    set acc "r"
  } elseif { $mode == "w" } {
    set acc "w"
  } else {
    set errmsg "Unknown mode: $mode"
    return -1
  }

  if { [catch { set fd [open "| $cmd" $acc] }] } {
    set errmsg "Cannot run $cmd"
    return -1
  }

  if { $fd == -1 } {
    set errmsg "Cannot run $cmd"
    return -1
  }

  set netwag_priv_popen_fdcmd($fd) $cmd
  return 0
}

## Configure popen to be unblocking
proc netwag_popen_conf_block { fd block } {

  if { $block } {
    fconfigure $fd -blocking 1 -buffering line
  } else  {
    fconfigure $fd -blocking 0 -buffering none
  }

  return 0
}

## Get the next line
proc netwag_popen_read_line { fd pline } {
  global netwag_priv_popen_fdcmd

  upvar $pline line

  set ret [gets $fd line]
  if { $ret == -1 } {
    return -1
  }

  return 0
}

## Get the next data
proc netwag_popen_read_data { fd pdata } {
  global netwag_priv_popen_fdcmd

  upvar $pdata data

  if [eof $fd] {
    return -1
  }

  set data [read $fd 16383]

  if { [string length $data] == 0 && [eof $fd] } {
    return -1
  }

  return 0
}

## Write a line
proc netwag_popen_write_line { fd line } {
  puts $fd $line
}

## Write data
proc netwag_popen_write_data { fd data } {
  puts -nonewline $fd $data
}

## Close
proc netwag_popen_close { fd pretval perrmsg } {
  global errorCode
  global netwag_priv_popen_fdcmd

  upvar $pretval retval
  upvar $perrmsg errmsg

  if { [catch { close $fd }] } {
    set errlist [split $errorCode]
    set errtype [lindex $errlist 0]
    if { $errtype == "CHILDSTATUS" } {
      set retval [lindex $errlist 2]
      return 0
    } elseif { $errtype == "NONE" } {
      # occurs under Solaris 9, to obtain remote information
      set retval 0
      return 0
    } else {
      set cmd $netwag_priv_popen_fdcmd($fd)
      set errmsg "Command $cmd exited with error $errlist"
      return -1
    }
  }
  set retval 0
  return 0
}

## Interrupt
proc netwag_popen_interrupt { fd perrmsg } {
  global errorCode
  global netwag_priv_popen_fdcmd

  upvar $perrmsg errmsg

  if { [catch { close $fd }] } {
    set errlist [split $errorCode]
    set errtype [lindex $errlist 0]
    if { $errtype == "CHILDKILLED" } {
      return 0
    } elseif { $errtype == "CHILDSTATUS" } {
      return 0
    } else {
      set cmd $netwag_priv_popen_fdcmd($fd)
      set errmsg "Command $cmd exited with error $errlist"
      return -1
    }
  }

  return 0
}
