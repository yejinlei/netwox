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

## Display an error
proc netwag_priv_glo_errorunset { pname } {

  set msg "Variable $pname does not exist.\nSet its value, or if unsure reinstall netwag."
  set ans [tk_messageBox -message $msg -type okcancel -title "Warning" -icon warning]
  if { $ans == "cancel" } {
    exit
  }
}
proc netwag_priv_glo_errorbadval { msg } {

  set msg "$msg\nModify its value, or if unsure reinstall netwag."
  set ans [tk_messageBox -message $msg -type okcancel -title "Warning" -icon warning]
  if { $ans == "cancel" } {
    exit
  }
}

## Special check for the session file
proc netwag_priv_glo_check_file_session { pname } {
  upvar $pname name
  global $pname

  if { ! [info exists $pname] } {
    netwag_priv_glo_errorunset $pname
    set name ""
    return
  }

  set name [netwag_misc_string_replace $name "\\" "/"]

  # we do no more checks (exists, readable, writable) because this state
  # can change during execution. So, it's better to deal with
  # it in corresponding functions
}

## Special check for netwox path
proc netwag_priv_glo_check_bin_netwox { pname defaultvalue } {
  upvar $pname name
  global $pname

  if { ! [info exists $pname] }  {
    netwag_priv_glo_errorunset $pname
    set name $defaultvalue
    return
  }

}

## Check for a directory
proc netwag_priv_glo_check_directory { pname defaultvalue } {
  upvar $pname name
  global $pname

  if { ! [info exists $pname] }  {
    netwag_priv_glo_errorunset $pname
    set name $defaultvalue
    return
  }

  if { ! [file isdirectory $name] } {
    netwag_priv_glo_errorbadval "Variable $pname contains \"$name\" which is not a directory."
    set name $defaultvalue
  }
}

## Check if the file is writable
proc netwag_priv_glo_check_writable { pname defaultvalue } {
  upvar $pname name
  global $pname

  if { ! [info exists $pname] }  {
    netwag_priv_glo_errorunset $pname
    set name $defaultvalue
    return
  }

  if { ! [file readable $name] } {
    netwag_priv_glo_errorbadval "Variable $pname contains \"$name\" which is not a writable file."
    set name $defaultvalue
  }
}

## Concert backslashes
proc netwag_priv_glo_convslash { pname defaultvalue } {
  upvar $pname name
  global $pname

  if { ! [info exists $pname] }  {
    netwag_priv_glo_errorunset $pname
    set name $defaultvalue
    return
  }

  set name [netwag_misc_string_replace $name "\\" "/"]
}


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Check main global variables
proc netwag_glo_check { } {
  global env
  global netwag_glo_dir_tmp
  global netwag_glo_file_session
  global netwag_glo_bin_netwox

  # netwag_glo_dir_tmp must be a writable directory
  netwag_priv_glo_convslash netwag_glo_dir_tmp ""
  netwag_priv_glo_check_directory netwag_glo_dir_tmp ""
  if { $netwag_glo_dir_tmp != "" } {
    netwag_priv_glo_check_writable netwag_glo_dir_tmp ""
  }
  if { $netwag_glo_dir_tmp == "" } {
    if { $netwag_sys_platform == $netwag_sys_platform_windows } {
      if { [catch { set netwag_glo_dir_tmp $env(TEMP) }] } {
        if { [catch { set netwag_glo_dir_tmp $env(TMP) }] } {
          set netwag_glo_dir_tmp "."
        }
      }
    } elseif { $netwag_sys_platform == $netwag_sys_platform_unix } {
      set netwag_glo_dir_tmp "/tmp"
    } else {
      set netwag_glo_dir_tmp "."
    }
  }

  # netwag_glo_file_session should be a readable/writable file
  netwag_priv_glo_check_file_session netwag_glo_file_session

  # netwag_glo_bin_netwox
  netwag_priv_glo_convslash netwag_glo_bin_netwox "netwox"
  netwag_priv_glo_check_bin_netwox netwag_glo_bin_netwox "netwox"

}
