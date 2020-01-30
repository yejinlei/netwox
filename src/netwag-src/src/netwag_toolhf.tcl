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
# used for help and form
global netwag_priv_toolhf_curtoolnum ; set netwag_priv_toolhf_curtoolnum -1
global netwag_priv_toolhf_wanttoolnum ; set netwag_priv_toolhf_wanttoolnum -2


#################################################################
# PRIVATE FUNCTIONS
#################################################################

proc netwag_priv_toolhf_init { toolnum } {
  global netwag_priv_toolhelp_help
  global netwag_priv_toolhelp_example
  global netwag_priv_toolhelp_usage
  global netwag_glo_bin_netwox

  if {![info exist netwag_priv_toolhelp_help($toolnum)]} {
    set ret [netwag_exec "$netwag_glo_bin_netwox 0 -h -u $toolnum" outmsg retval errmsg]
    netwag_cursor_default
    if { $ret } {
      netwag_msg_add_err "Cannot obtain the help for $toolnum ($errmsg)"
      return $ret
    }
    if { $retval } {
      netwag_msg_add_err "Help for $toolnum might be incomplete (retgets=$retval)"
    }
    eval $outmsg
  }
}


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Fake function for empty message
proc netwag_toolform_display_func0 { } {
  netwag_toolform_add_text "No tool is selected. You have two choices to obtain help :\n - select a tool in Search notebook\n - enter a command (in front of Run button) and press Update button\n"
}

## Display description of a tool
proc netwag_toolhf_display { } {
  global netwag_priv_toolhelp_text
  global netwag_priv_toolhf_curtoolnum
  global netwag_priv_toolhf_wanttoolnum

  if {$netwag_priv_toolhf_wanttoolnum == $netwag_priv_toolhf_curtoolnum} {
    return
  }
  if {$netwag_priv_toolhf_wanttoolnum < 0} {
    # message
    set msg "No tool is selected. You have two choices to obtain help :\n - select a tool in Search notebook\n - enter a command (in front of Run button) and press Update button\n"
    $netwag_priv_toolhelp_text configure -state normal
    $netwag_priv_toolhelp_text delete 1.0 end
    $netwag_priv_toolhelp_text insert end $msg
    $netwag_priv_toolhelp_text configure -state disabled
    netwag_toolform_display 0
    return
  }

  # reinit help and form
  $netwag_priv_toolhelp_text configure -state normal
  $netwag_priv_toolhelp_text delete 1.0 end
  $netwag_priv_toolhelp_text configure -state disabled
  netwag_toolform_reinit

  # query and display (use a temporary variable because
  # netwag_priv_toolhf_wanttoolnum is global and can be changed between
  # the toolhf_init and _display)
  set wanttoolnum $netwag_priv_toolhf_wanttoolnum
  netwag_priv_toolhf_init $wanttoolnum
  netwag_toolhelp_display $wanttoolnum
  netwag_toolform_display $wanttoolnum
  set netwag_priv_toolhf_curtoolnum $wanttoolnum
}

## Called when Update tool is pressed
proc netwag_toolhf_button_update {} {

  if { [netwag_toolcmd_check usercmd toolnum] } {
    return
  }
  set realcmd [netwag_toolcmd_withpassword $usercmd]

  global netwag_priv_toolhf_wanttoolnum
  set netwag_priv_toolhf_wanttoolnum $toolnum

  netwag_toolhf_display

  # update contents
  global netwag_glo_bin_netwox
  netwag_runcmd_file_init $realcmd tmpfilename
  set ret [netwag_exec "$netwag_glo_bin_netwox 0 -f -b $tmpfilename -u $toolnum" outmsg retval errmsg]
  if { $ret } {
    netwag_msg_add_warn "Cannot update command line parameters for tool $toolnum ($errmsg)"
    netwag_runcmd_file_del $tmpfilename
    return
  }
  if { $retval } {
    netwag_msg_add_warn "Cannot update tool $toolnum: $outmsg"
    return
  }
  netwag_toolform_uncheck
  eval $outmsg
}

