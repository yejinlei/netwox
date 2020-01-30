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
# PUBLIC FUNCTIONS
#################################################################

## Time profiling
proc netwag_prof_init {} {
  global netwag_prof_init_lastime
  global netwag_prof_init_msg
  set netwag_prof_init_lastime [clock clicks]
  set netwag_prof_init_msg {}
}
proc netwag_prof_diff { msg } {
  global netwag_prof_init_lastime
  global netwag_prof_init_msg
  append netwag_prof_init_msg [expr [clock clicks] - $netwag_prof_init_lastime]
  append netwag_prof_init_msg "\t($msg)\n"
  set netwag_prof_init_lastime [clock clicks]
}
proc netwag_prof_report {} {
  global netwag_prof_init_lastime
  global netwag_prof_init_msg
  append netwag_prof_init_msg [expr [clock clicks] - $netwag_prof_init_lastime]
  append netwag_prof_init_msg "\t(end)"
  puts $netwag_prof_init_msg
}

## Create a fake widget using a font. Indeed, font computation is
## very slow when there is not yet any window
proc netwag_main_fake_widget {} {
  label .fake_widget -font myfont
}

## Check netwox
proc netwag_main_checknetwox { } {
  global netwag_glo_versionmajor
  global netwag_glo_versionminor
  global netwag_glo_bin_netwox

  set errmsg1 "We cannot run $netwag_glo_bin_netwox"
  set errmsg2 "Edit variable netwag_glo_bin_netwox (current value $netwag_glo_bin_netwox) to contain a valid path to netwox version $netwag_glo_versionmajor.$netwag_glo_versionminor.x."
  set ret [netwag_exec "$netwag_glo_bin_netwox 0 -v" outmsg retval errmsg]
  if { $ret } {
    netwag_msg_add_err "$errmsg1 ($errmsg)"
    netwag_priv_glo_errorbadval "$errmsg1 ($errmsg).\n$errmsg2"
    return -1
  } elseif { $retval } {
    netwag_msg_add_err "$errmsg1 (retval=$retval)"
    netwag_priv_glo_errorbadval "$errmsg1 (retval=$retval).\n$errmsg2"
    return -1
  }
  set netwag_netwox_versionmajor 0
  set netwag_netwox_versionminor 0
  set netwag_netwox_versionmicro 0
  if {[catch {eval $outmsg}]} {
    set errmsg3 "Program $netwag_glo_bin_netwox returned a bad value"
    netwag_msg_add_err "$errmsg3."
    netwag_priv_glo_errorbadval "$errmsg3.\n$errmsg2"
    return -1
  }
  if {$netwag_netwox_versionmajor == 0 && $netwag_netwox_versionminor == 0} {
    set errmsg3 "Program $netwag_glo_bin_netwox is an old version of netwox"
    netwag_msg_add_err "$errmsg3."
    netwag_priv_glo_errorbadval "$errmsg3.\n$errmsg2"
    return -1
  }
  if {$netwag_netwox_versionmajor != $netwag_glo_versionmajor || $netwag_netwox_versionminor != $netwag_glo_versionminor} {
    set errmsg3 "Version of $netwag_glo_bin_netwox is $netwag_netwox_versionmajor.$netwag_netwox_versionminor (wanted $netwag_glo_versionmajor.$netwag_glo_versionminor)"
    netwag_msg_add_err "$errmsg3."
    netwag_priv_glo_errorbadval "$errmsg3.\n$errmsg2"
    return -1
  }

  netwag_msg_add_info "Linked with netwox version $netwag_netwox_versionmajor.$netwag_netwox_versionminor.$netwag_netwox_versionmicro"

  return 0
}

## Main
proc netwag_main {} {
  global frame_notebook
  global frame_clipboard
  global frame_message
  global netwag_glo_versionmajor
  global netwag_glo_versionminor
  global netwag_glo_versionmicro

  # check global variables
  netwag_glo_check
  #netwag_prof_diff "after glo_check"

  # load the last session
  netwag_session_load_vars
  #netwag_prof_diff "after load_vars"

  # initialize font
  netwag_font_init
  #netwag_prof_diff "after font_init"

  # create a fake widget to speed startup
  #netwag_main_fake_widget
  #netwag_prof_diff "after fake_widget"

  # initialize the clipboard
  netwag_clip_init
  #netwag_prof_diff "after clip_init"

  # initialize widgets
  netwag_gui_skeleton frame_notebook frame_clipboard frame_message
  netwag_gui_message $frame_message
  netwag_gui_clipboard $frame_clipboard
  netwag_gui_menu
  netwag_update
  #netwag_prof_diff "after gui"

  # display first message
  netwag_msg_add_info "Netwag version $netwag_glo_versionmajor.$netwag_glo_versionminor.$netwag_glo_versionmicro"

  # check netwox
  set ret [netwag_main_checknetwox]
  if { $ret == -1 } {
    global netwag_menu_fileexit_try
    # to ensure session is not overwritten
    set netwag_menu_fileexit_try 1
    return $ret
  }
  #netwag_prof_diff "after run"

  # initialize search functions
  netwag_toolsearch_init
  #netwag_prof_diff "after search"

  # initialize history
  netwag_toolhist_init
  #netwag_prof_diff "after runhist_init"

  # initialize information
  netwag_infol_init
  netwag_infor_init
  #netwag_prof_diff "after info_init"

  # apply the last session
  netwag_session_load_apply
  #netwag_prof_diff "after load_apply"

  # initialize the clipboard
  netwag_smallclip_init
  #netwag_prof_diff "after smallclip_init"

  # press on Search button
  global netwag_priv_tool_nb
  netwag_notebook_button_press $netwag_priv_tool_nb 0
  #netwag_prof_diff "after click"

  return 0
}


#################################################################
# MAIN
#################################################################
#netwag_prof_init
netwag_main

# used with "time netwag" to obtain startup time
#tkwait visibility $netwag_priv_toolsearch_lb
#netwag_prof_diff "after visibility"
#netwag_prof_report
#exit

# activate to display debug messages
# catch {console show}

if {$netwag_sys_platform == $netwag_sys_platform_windows} {
  # when netwag popups, it does not have focus. Force it.
  focus -force .
  # need at least one item focus before been able to scroll
  focus $netwag_priv_msg_textframe
}

# If activated before end of load, display is strange under Windows,
# Solaris 9, but not under Linux. Better wait for all system.
tkwait visibility $netwag_priv_toolsearch_lb
netwag_update_allow

