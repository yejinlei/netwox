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
global netwag_runtext_state
global netwag_runtext_state_norun ; set netwag_runtext_state_norun 0
global netwag_runtext_state_running ; set netwag_runtext_state_running 1
global netwag_runtext_state_paused ; set netwag_runtext_state_paused 2

global netwag_runtext_realcmd
global netwag_runtext_usercmd

global netwag_runtext_autoscroll
global netwag_runtext_backspace
global netwag_runtext_autocrush
global netwag_runtext_fd
global netwag_runtext_pauseddata

global netwag_runtext_cb_begin
global netwag_runtext_cb_end


#################################################################
# PRIVATE FUNCTIONS
#################################################################

## Append stdout data
proc netwag_priv_runtext_append_stdout { txt data } {
  global netwag_runtext_state
  global netwag_runtext_state_paused
  global netwag_runtext_state_running
  global netwag_runtext_pauseddata
  global netwag_runtext_backspace
  global netwag_runtext_autocrush

  if {$netwag_runtext_state($txt) == $netwag_runtext_state_running} {
    $txt configure -state normal
    if {$netwag_runtext_backspace($txt)} {
      # deal with backspaces
      while {1} {
        while {[string range $data 0 0] == ""} {
          $txt delete "end -2c"
          set data [string range $data 1 end]
        }
        if { ! [regsub -- "." "$data" "" data] } {
          break
        }
      }
    }
    $txt insert end "$data"
    if {$netwag_runtext_autocrush($txt)} {
      set linecol [$txt index end]
      set dotpos [expr [string first "." $linecol] - 1]
      set numlines [string range $linecol 0 $dotpos]
      global netwag_glo_gui_toolrun_crush
      if {$numlines > $netwag_glo_gui_toolrun_crush} {
        $txt delete 1.0 "[expr $netwag_glo_gui_toolrun_crush / 5].0"
        $txt insert 1.0 "Lines before were crushed (limit to $netwag_glo_gui_toolrun_crush lines).\n" netwag_runtext_color_info
        netwag_update_idletasks
      }
    }
    $txt configure -state disabled
    netwag_runtext_autoscroll_show $txt
  } else {
    append netwag_runtext_pauseddata($txt) "$data"
  }

}

## Append stdin data
proc netwag_priv_runtext_append_stdin { txt data } {

  $txt configure -state normal
  $txt insert end "$data" netwag_runtext_color_stdin
  $txt configure -state disabled

  netwag_runtext_autoscroll_show $txt
}

## Append end
proc netwag_priv_runtext_append_info { txt data } {

  $txt configure -state normal
  $txt insert end "$data" netwag_runtext_color_info
  $txt configure -state disabled

  netwag_runtext_autoscroll_show $txt
}

## Called when data is available
proc netwag_priv_runtext_event { txt } {
  global netwag_runtext_state
  global netwag_runtext_state_running
  global netwag_runtext_state_norun
  global netwag_runtext_fd

  set fd $netwag_runtext_fd($txt)
  set ret [netwag_popen_read_data $fd data]

  if { $ret == 0 } {
    # suppress \r
    set data [netwag_misc_string_replace $data "\r" ""]
    # ok
    netwag_priv_runtext_append_stdout $txt "$data"
    return 0
  }

  # if we are here, program has terminated
  set netwag_runtext_state($txt) $netwag_runtext_state_norun

  # reconfigure blocking, otherwise, variable errorCode is never set
  netwag_popen_conf_block $fd 1

  set ret [netwag_popen_close $fd retval errmsg]
  if { $ret } {
    netwag_msg_add_err "Error when closing popen"
    return $ret
  }

  if { $retval } {
    netwag_msg_add_err "Tool returned an error"
  } else {
    netwag_msg_add_info "Tool finished its job"
  }

  global netwag_runtext_cb_end
  if {$netwag_runtext_cb_end($txt) != ""} {
    global netwag_runtext_realcmd
    global netwag_runtext_usercmd
    $netwag_runtext_cb_end($txt) $txt $netwag_runtext_realcmd($txt) $netwag_runtext_usercmd($txt)
  }

  return 0
}


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Initialize
# cb_start $txt $realcmd $usercmd : callback called when tool starts
# cb_end $txt $realcmd $usercmd : callback called when tool finishes
proc netwag_runtext_init { txt {cb_start ""} {cb_end ""} } {
  global netwag_runtext_state
  global netwag_runtext_state_norun

  set netwag_runtext_state($txt) $netwag_runtext_state_norun

  global netwag_runtext_cb_start
  set netwag_runtext_cb_start($txt) $cb_start

  global netwag_runtext_cb_end
  set netwag_runtext_cb_end($txt) $cb_end

  global netwag_runtext_autoscroll
  if {![info exists netwag_runtext_autoscroll($txt)]} {
    set netwag_runtext_autoscroll($txt) 0
  }

  global netwag_runtext_backspace
  if {![info exists netwag_runtext_backspace($txt)]} {
    set netwag_runtext_backspace($txt) 0
  }

  global netwag_runtext_autocrush
  if {![info exists netwag_runtext_autocrush($txt)]} {
    set netwag_runtext_autocrush($txt) 0
  }

  $txt tag configure netwag_runtext_color_stdin -foreground "#0020FF"
  $txt tag configure netwag_runtext_color_info -foreground "#66CC66"
  $txt configure -state disabled

  return 0
}

## Free memory when a Running notebook is closed
proc netwag_runtext_close { txt } {
  $txt configure -state normal
  $txt delete 1.0 end
  $txt configure -state disabled
  return 0
}

## Run a command and put result in a text
proc netwag_runtext_run { txt realcmd usercmd } {
  global netwag_glo_bin_netwox
  global netwag_runtext_state
  global netwag_runtext_state_running
  global netwag_runtext_state_norun
  global netwag_runtext_fd
  global netwag_runtext_pauseddata

  if { $netwag_runtext_state($txt) != $netwag_runtext_state_norun } {
    netwag_msg_add_warn "There is a tool currently running"
    return 0
  }

  netwag_runcmd_file_init $realcmd tmpfilename

  set ret [netwag_popen_init "$netwag_glo_bin_netwox 0 -r -b $tmpfilename" "rw" fd errmsg]
  if { $ret } {
    netwag_msg_add_err "Cannot run \"$usercmd\""
    netwag_runcmd_file_del $tmpfilename
    return $ret
  } else {
    netwag_msg_add_info "Running \"$usercmd\""
  }

  set netwag_runtext_state($txt) $netwag_runtext_state_running
  set netwag_runtext_fd($txt) $fd
  set netwag_runtext_pauseddata($txt) ""

  global netwag_runtext_realcmd
  global netwag_runtext_usercmd
  set netwag_runtext_realcmd($txt) $realcmd
  set netwag_runtext_usercmd($txt) $usercmd

  netwag_popen_conf_block $fd 0
  fconfigure $fd -translation {binary binary}

  $txt configure -state normal
  $txt delete 1.0 end
  $txt configure -state disabled

  global netwag_runtext_cb_start
  if {$netwag_runtext_cb_start($txt) != ""} {
    $netwag_runtext_cb_start($txt) $txt $realcmd $usercmd
  }

  fileevent $fd readable "netwag_priv_runtext_event $txt"

  return 0
}

## Eventually autoscroll
proc netwag_runtext_autoscroll_show { txt } {
  global netwag_runtext_autoscroll
  if {$netwag_runtext_autoscroll($txt)} {
    $txt see "end"
    netwag_update_idletasks
  }
}

## Called on interrupt request
proc netwag_runtext_int { txt } {
  global netwag_glo_bin_netwox
  global netwag_runtext_state
  global netwag_runtext_state_paused
  global netwag_runtext_state_norun
  global netwag_runtext_fd

  if { $netwag_runtext_state($txt) == $netwag_runtext_state_norun } {
    # might have just finished, so do not display error
    return 0
  }

  # we kill the child in 500 ms if it doesn't close itself
  set thepid [pid $netwag_runtext_fd($txt)]
  exec $netwag_glo_bin_netwox 0 -k -u $thepid -b 500 &

  set ret [netwag_popen_interrupt $netwag_runtext_fd($txt) errmsg]
  if { $ret == 0 } {
    netwag_msg_add_info "Tool successfully interrupted"
  }
  set netwag_runtext_state($txt) $netwag_runtext_state_norun

  global netwag_runtext_cb_end
  if {$netwag_runtext_cb_end($txt) != ""} {
    global netwag_runtext_realcmd
    global netwag_runtext_usercmd
    $netwag_runtext_cb_end($txt) $txt $netwag_runtext_realcmd($txt) $netwag_runtext_usercmd($txt)
  }

  return $ret
}

## Called to interrupt all running programs
proc netwag_runtext_int_all { } {
  global netwag_runtext_state
  global netwag_runtext_state_norun

  foreach txt [array names netwag_runtext_state] {
    if { $netwag_runtext_state($txt) != $netwag_runtext_state_norun } {
      netwag_runtext_int $txt
    }
  }
}

## Called on send request
proc netwag_runtext_send { txt data echo } {
  global netwag_runtext_state
  global netwag_runtext_state_norun
  global netwag_runtext_fd

  if { $netwag_runtext_state($txt) == $netwag_runtext_state_norun } {
    netwag_msg_add_warn "There is no tool currently running"
    return 0
  }

  if {$echo} {
    netwag_priv_runtext_append_stdin $txt $data
  }
  netwag_popen_write_data $netwag_runtext_fd($txt) $data

  return 0
}

## Called on pause/continue request
proc netwag_runtext_pause { txt } {
  global netwag_runtext_state
  global netwag_runtext_state_norun
  global netwag_runtext_state_running
  global netwag_runtext_state_paused
  global netwag_runtext_pauseddata

  if { $netwag_runtext_state($txt) == $netwag_runtext_state_norun } {
    netwag_msg_add_warn "There is no tool currently running"
    return 0
  }

  if { $netwag_runtext_state($txt) == $netwag_runtext_state_running } {
    set netwag_runtext_state($txt) $netwag_runtext_state_paused
  } else {
    set netwag_runtext_state($txt) $netwag_runtext_state_running
    netwag_priv_runtext_append_stdout $txt $netwag_runtext_pauseddata($txt)
  }
  set netwag_runtext_pauseddata($txt) ""

  return 0
}

## Store a message in the text zone
proc netwag_runtext_append_msg { txt data } {
  $txt configure -state normal
  $txt insert end "$data"
  $txt configure -state disabled

  netwag_runtext_autoscroll_show $txt
}
