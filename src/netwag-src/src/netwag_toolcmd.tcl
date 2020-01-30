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
global netwag_priv_toolcmd_frame
global netwag_priv_toolcmd_text
global netwag_priv_toolcmd_nw

#################################################################
# PRIVATE FUNCTIONS
#################################################################

## Check if the command is empty
proc netwag_priv_toolcmd_empty { } {
  global netwag_priv_toolcmd_text

  set cmd [$netwag_priv_toolcmd_text get 1.0 end]
  set cmd [netwag_misc_string_map $cmd {"\r" " " "\n" " "}]
  set cmd [string trim $cmd]

  if { $cmd == "" } {
    return -1
  }

  return 0
}

## Called when Run button is pressed
proc netwag_priv_toolcmd_run { } {
  global netwag_priv_toolcmd_nw
  global netwag_priv_tool_nb

  if { [netwag_priv_toolcmd_empty] } {
    netwag_msg_add_warn "Command is empty. Will try to automatically press on first button."
    set butnum [netwag_notebook_butnum_get $netwag_priv_tool_nb]
    if {$butnum == 1} {
      netwag_toolhelp_button_copyex
    } elseif {$butnum == 2} {
      netwag_toolform_button_gen
    } elseif {$butnum == 4} {
      netwag_toolhist_button_copy
    } else {
      netwag_msg_add_err "Butnum $butnum is not implemented."
      return
    }
    if { ! [netwag_priv_toolcmd_empty] } {
      netwag_msg_add_info "If you agree with this command, press Run again."
    }
    return
  }

  # obtain user command (passwords are hidden)
  if { [netwag_toolcmd_check usercmd toolnum] } {
    return
  }

  # obtain real command containing passwords
  set realcmd [netwag_toolcmd_withpassword $usercmd]

  # run
  netwag_toolhist_add "$usercmd"
  if { $netwag_priv_toolcmd_nw } {
    netwag_msg_add_info "Running \"$usercmd\""
    netwag_msg_add_info "We cannot check its return value because tool is run in a New Window"
    netwag_runnw_run $realcmd
  } else {
    netwag_toolrun_run $toolnum $realcmd $usercmd
  }
}


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Initialize the command
proc netwag_toolcmd_init { cmd } {
  global netwag_priv_toolcmd_text
  $netwag_priv_toolcmd_text delete 1.0 end
  $netwag_priv_toolcmd_text insert end $cmd
}

## Hide/show run command
proc netwag_toolcmd_hide { } {
  global netwag_priv_toolcmd_frame

  pack forget $netwag_priv_toolcmd_frame
}
proc netwag_toolcmd_show { } {
  global netwag_priv_toolcmd_frame

  pack $netwag_priv_toolcmd_frame -side left -expand 1 -fill x
}

## Check command
proc netwag_toolcmd_check { pcmd ptoolnum } {
  global netwag_priv_toolcmd_text

  upvar $pcmd cmd
  upvar $ptoolnum toolnum

  set cmd [$netwag_priv_toolcmd_text get 1.0 end]
  set cmd [netwag_misc_string_map $cmd {"\r" " " "\n" " "}]
  set cmd [string trim $cmd]

  set ret [netwag_runcmd_check cmd toolnum]
}

## Obtain stored password
proc netwag_toolcmd_withpassword { usercmd } {
  global netwag_priv_toolform_hidden
  global netwag_priv_toolform_hidden_pattern

  set re $netwag_priv_toolform_hidden_pattern(0)
  append re "(\[0-9\]+)"
  append re $netwag_priv_toolform_hidden_pattern(1)
  set realcmd $usercmd
  while {1} {
    if { ! [regexp -- $re $realcmd foobar n] } {
      break
    }
    set r " "
    if { [info exists netwag_priv_toolform_hidden($n)] } {
      set val $netwag_priv_toolform_hidden($n)
      set val [netwag_misc_string_replace $val "\\" "\\\\"]
      append r $val
    } else {
      append r "password_"
      append r $n
      append r "_not_available"
    }
    regsub -- $re $realcmd $r realcmd
  }
  return $realcmd
}
