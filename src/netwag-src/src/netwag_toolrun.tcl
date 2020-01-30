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
global netwag_priv_toolrun_nb
global netwag_priv_toolrun_nb_top
global netwag_priv_toolrun_nb_bot

# arrays of widgets for each running/terminated tool
#  netwag_priv_toolrun_widget($ctx,top)
#  netwag_priv_toolrun_widget($ctx,bot)
#  netwag_priv_toolrun_widget($ctx,buttons)
#  netwag_priv_toolrun_widget($ctx,send)
#  netwag_priv_toolrun_widget($ctx,output)
#  netwag_priv_toolrun_widget($ctx,copy)
#  netwag_priv_toolrun_widget($ctx,buttons_pause)
#  netwag_priv_toolrun_widget($ctx,send_text)
#  netwag_priv_toolrun_widget($ctx,send_echo)
#  netwag_priv_toolrun_widget($ctx,output_text)
#  netwag_priv_toolrun_widget($ctx,realcmd)
#  netwag_priv_toolrun_widget($ctx,usercmd)
global netwag_priv_toolrun_widget

# list of visible contexts
#  ie netwag_priv_toolrun_ctx_visible == {1 4 7 8 9}
global netwag_priv_toolrun_ctx_visible
# current context
global netwag_priv_toolrun_ctx_current ; set netwag_priv_toolrun_ctx_current 0
# next available context number
global netwag_priv_toolrun_ctx_next ; set netwag_priv_toolrun_ctx_next 0


#################################################################
# PRIVATE FUNCTIONS
#################################################################

## Create a new context
proc netwag_priv_toolrun_ctx_new { ctx buttontitle } {
  global netwag_glo_gui_bgcolor_text_select
  global netwag_glo_gui_bgcolor_text_edit
  global netwag_glo_gui_bgcolor_unused
  global netwag_glo_gui_bgcolor_button
  global netwag_priv_toolrun_nb_top
  global netwag_priv_toolrun_nb_topb
  global netwag_priv_toolrun_nb_bot
  global netwag_priv_toolrun_widget

  set netwag_priv_toolrun_widget($ctx,topb) $netwag_priv_toolrun_nb_topb.ctx$ctx
  set netwag_priv_toolrun_widget($ctx,bot) $netwag_priv_toolrun_nb_bot.ctx$ctx
  set netwag_priv_toolrun_widget($ctx,buttons) $netwag_priv_toolrun_widget($ctx,bot).buttons
  set netwag_priv_toolrun_widget($ctx,buttons_pause) $netwag_priv_toolrun_widget($ctx,buttons).pause
  set netwag_priv_toolrun_widget($ctx,send) $netwag_priv_toolrun_widget($ctx,bot).send
  set netwag_priv_toolrun_widget($ctx,send_text) $netwag_priv_toolrun_widget($ctx,send).text
  set netwag_priv_toolrun_widget($ctx,send_echo) $netwag_priv_toolrun_widget($ctx,send).echo
  set netwag_priv_toolrun_widget($ctx,output) $netwag_priv_toolrun_widget($ctx,bot).output
  set netwag_priv_toolrun_widget($ctx,output_text) $netwag_priv_toolrun_widget($ctx,output).left.text
  set txt $netwag_priv_toolrun_widget($ctx,output_text)
  set netwag_priv_toolrun_widget($ctx,copy) $netwag_priv_toolrun_widget($ctx,bot).copy

  button $netwag_priv_toolrun_widget($ctx,topb) -text $buttontitle -command "netwag_priv_toolrun_ctx_change $ctx" -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  netwag_dynhelp_init $netwag_priv_toolrun_widget($ctx,topb) "Press on this button to see running command. If foreground is black command is running, if it is white command has terminated."

  frame $netwag_priv_toolrun_widget($ctx,bot) -bg $netwag_glo_gui_bgcolor_unused
  frame $netwag_priv_toolrun_widget($ctx,buttons) -bg $netwag_glo_gui_bgcolor_unused
  frame $netwag_priv_toolrun_widget($ctx,send) -bg $netwag_glo_gui_bgcolor_unused
  frame $netwag_priv_toolrun_widget($ctx,output) -bg $netwag_glo_gui_bgcolor_unused
  frame $netwag_priv_toolrun_widget($ctx,copy) -bg $netwag_glo_gui_bgcolor_unused
  pack $netwag_priv_toolrun_widget($ctx,buttons) -fill x
  pack $netwag_priv_toolrun_widget($ctx,output) -expand 1 -fill both

  # buttons
  button $netwag_priv_toolrun_widget($ctx,buttons).copycmd -text "Copy command" -command "netwag_toolrun_button_copycmd $ctx" -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  button $netwag_priv_toolrun_widget($ctx,buttons).int -text "Interrupt" -command "netwag_toolrun_button_int $ctx" -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  button $netwag_priv_toolrun_widget($ctx,buttons_pause) -text "Pause" -command "netwag_toolrun_button_pause $ctx" -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  checkbutton $netwag_priv_toolrun_widget($ctx,buttons).autoscroll -text "scroll" -variable netwag_runtext_autoscroll($txt) -font myfont -bg $netwag_glo_gui_bgcolor_unused -highlightthickness 0 -command "netwag_runtext_autoscroll_show $txt"
  checkbutton $netwag_priv_toolrun_widget($ctx,buttons).autocrush -text "crush" -variable netwag_runtext_autocrush($txt) -font myfont -bg $netwag_glo_gui_bgcolor_unused -highlightthickness 0
  pack $netwag_priv_toolrun_widget($ctx,buttons).copycmd $netwag_priv_toolrun_widget($ctx,buttons).int $netwag_priv_toolrun_widget($ctx,buttons_pause) $netwag_priv_toolrun_widget($ctx,buttons).autoscroll $netwag_priv_toolrun_widget($ctx,buttons).autocrush -side left
  netwag_dynhelp_init $netwag_priv_toolrun_widget($ctx,buttons).copycmd "To copy command which launched this tool."
  netwag_dynhelp_init $netwag_priv_toolrun_widget($ctx,buttons).int "To interrupt a running tool."
  netwag_dynhelp_init $netwag_priv_toolrun_widget($ctx,buttons_pause) "To pause (or continue) the display of a tool."
  netwag_dynhelp_init $netwag_priv_toolrun_widget($ctx,buttons).autoscroll "To automatically scroll window."
  global netwag_glo_gui_toolrun_crush
  netwag_dynhelp_init $netwag_priv_toolrun_widget($ctx,buttons).autocrush "To automatically crush old data (greater than $netwag_glo_gui_toolrun_crush lines)."

  # send
  entry $netwag_priv_toolrun_widget($ctx,send).entry -textvariable netwag_priv_toolrun_widget($ctx,send_text) -width 10 -font myfont -bg $netwag_glo_gui_bgcolor_text_edit -highlightthickness 0
  bind $netwag_priv_toolrun_widget($ctx,send).entry <Return> "netwag_toolrun_button_send $ctx"
  button $netwag_priv_toolrun_widget($ctx,send).send -text "Send" -command "netwag_toolrun_button_send $ctx" -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  checkbutton $netwag_priv_toolrun_widget($ctx,send).echobut -text "echo" -variable netwag_priv_toolrun_widget($ctx,send_echo) -font myfont -bg $netwag_glo_gui_bgcolor_unused -highlightthickness 0 -command "global netwag_priv_toolrun_widget; if {\$netwag_priv_toolrun_widget($ctx,send_echo)} {$netwag_priv_toolrun_widget($ctx,send).entry configure -show {}} else {$netwag_priv_toolrun_widget($ctx,send).entry configure -show {*}}"
  pack $netwag_priv_toolrun_widget($ctx,send).entry -side left -expand 1 -fill x -padx 2
  pack $netwag_priv_toolrun_widget($ctx,send).send $netwag_priv_toolrun_widget($ctx,send).echobut -side left
  netwag_dynhelp_init $netwag_priv_toolrun_widget($ctx,send).entry "Text to send to tool. Note : \"\\n\" indicates a new line."
  netwag_dynhelp_init $netwag_priv_toolrun_widget($ctx,send).send "Send the text (at the left) to tool."
  netwag_dynhelp_init $netwag_priv_toolrun_widget($ctx,send).echobut "Send text is/isn't echoed."
  set netwag_priv_toolrun_widget($ctx,send_text) "\\n"
  set netwag_priv_toolrun_widget($ctx,send_echo) 1

  # output
  set f $netwag_priv_toolrun_widget($ctx,output)
  set ft $netwag_priv_toolrun_widget($ctx,output_text)
  frame $f.left
  scrollbar $f.yscro -command "$ft yview" -troughcolor $netwag_glo_gui_bgcolor_text_select -highlightthickness 0
  pack $f.left -side left -expand 1 -fill both
  pack $f.yscro -side left -fill y
  text $ft -yscrollcommand "$f.yscro set" -xscrollcommand "$f.left.xscro set" -width 2 -height 2 -font myfont -bg $netwag_glo_gui_bgcolor_text_select -wrap none -highlightthickness 0
  scrollbar $f.left.xscro -command "$ft xview" -orient horizontal -troughcolor $netwag_glo_gui_bgcolor_text_select -highlightthickness 0
  pack $ft -side top -expand 1 -fill both
  pack $f.left.xscro -side top -fill x
  eval "bind $ft <Button-1> { focus $ft }"
  pack $f -expand 1 -fill both
  netwag_dynhelp_init $ft "Output (stdout) of a tool, if the checkbox NW was not checked."

  # copy buttons
  netwag_bitmap_down $netwag_priv_toolrun_widget($ctx,copy).genbm
  button $netwag_priv_toolrun_widget($ctx,copy).copycmd -text "Copy command" -command "netwag_toolrun_button_copycmd $ctx" -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
    button $netwag_priv_toolrun_widget($ctx,copy).runit -text "Run it again" -command "netwag_toolrun_button_copycmd $ctx ; netwag_priv_toolcmd_run" -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  netwag_bitmap_down $netwag_priv_toolrun_widget($ctx,copy).genbm2
  pack $netwag_priv_toolrun_widget($ctx,copy).genbm $netwag_priv_toolrun_widget($ctx,copy).copycmd $netwag_priv_toolrun_widget($ctx,copy).runit $netwag_priv_toolrun_widget($ctx,copy).genbm2 -side left
  netwag_dynhelp_init $netwag_priv_toolrun_widget($ctx,copy).copycmd "Copy command which launched this tool."
  netwag_dynhelp_init $netwag_priv_toolrun_widget($ctx,copy).runit "Copy command and run it again in another context."

  global netwag_priv_toolrun_ctx_visible
  lappend netwag_priv_toolrun_ctx_visible $ctx
}

## Destroy context
proc netwag_priv_toolrun_ctx_del { ctx } {
  global netwag_priv_toolrun_widget
  global netwag_priv_toolrun_ctx_visible

  destroy $netwag_priv_toolrun_widget($ctx,topb)
  destroy $netwag_priv_toolrun_widget($ctx,bot)

  set idx [lsearch $netwag_priv_toolrun_ctx_visible $ctx]
  if {$idx == -1} {
    return [lindex $netwag_priv_toolrun_ctx_visible 0]
  }

  set netwag_priv_toolrun_ctx_visible [lreplace $netwag_priv_toolrun_ctx_visible $idx $idx]

  if {$idx >= [llength $netwag_priv_toolrun_ctx_visible]} {
    incr idx -1
  }

  return [lindex $netwag_priv_toolrun_ctx_visible $idx]
}

## Context change
proc netwag_priv_toolrun_ctx_change { ctx } {
  global netwag_priv_toolrun_widget
  global netwag_priv_toolrun_ctx_visible
  global netwag_glo_notebook_button_visible_color
  global netwag_glo_notebook_button_hidden_color

  foreach c $netwag_priv_toolrun_ctx_visible {
    pack $netwag_priv_toolrun_widget($c,topb) -side left -padx 1
    $netwag_priv_toolrun_widget($c,topb) configure -bg $netwag_glo_notebook_button_hidden_color
    pack forget $netwag_priv_toolrun_widget($c,bot)
  }

  $netwag_priv_toolrun_widget($ctx,topb) configure -bg $netwag_glo_notebook_button_visible_color
  pack $netwag_priv_toolrun_widget($ctx,bot) -expand 1 -fill both

  global netwag_priv_toolrun_nb_top
  pack forget $netwag_priv_toolrun_nb_top
  if {[llength $netwag_priv_toolrun_ctx_visible] != 1} {
    pack $netwag_priv_toolrun_nb_top -fill x
  }

  global netwag_priv_toolrun_nb_bot
  pack forget $netwag_priv_toolrun_nb_bot.defaultmsg
  pack forget $netwag_priv_toolrun_nb_bot
  pack $netwag_priv_toolrun_nb_bot -expand 1 -fill both

  global netwag_priv_toolrun_ctx_current
  set netwag_priv_toolrun_ctx_current $ctx

  netwag_update_idletasks

  return 0
}


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Press Run button
proc netwag_toolrun_cb_start { txt realcmd usercmd } {
  set msg "Command $usercmd"
  if {[string length $msg] > 60} {
    set msg [string range $msg 0 59]
    append msg "..."
  }
  append msg " :\n"
  netwag_priv_runtext_append_info $txt $msg
}
proc netwag_toolrun_cb_end { txt realcmd usercmd } {
  global netwag_priv_toolrun_widget

  if { [regexp -- "\[.\]ctx(\[0-9\]+)\[.\]" $txt foobar ctx] } {
    pack forget $netwag_priv_toolrun_widget($ctx,buttons)
    pack forget $netwag_priv_toolrun_widget($ctx,send)
    pack $netwag_priv_toolrun_widget($ctx,copy) -fill x
    $netwag_priv_toolrun_widget($ctx,topb) configure -foreground white
  }
  netwag_priv_runtext_append_info $txt "__END_OF_PROGRAM__\n"
}
proc netwag_toolrun_run { toolnum realcmd usercmd } {
  global netwag_priv_toolrun_widget
  global netwag_priv_tools_stdin
  global netwag_priv_tools_backspace
  global netwag_priv_toolrun_ctx_next

  set ctx $netwag_priv_toolrun_ctx_next
  incr netwag_priv_toolrun_ctx_next

  netwag_priv_toolrun_ctx_new $ctx $toolnum
  set txt $netwag_priv_toolrun_widget($ctx,output_text)

  if {[info exists netwag_priv_tools_stdin($toolnum)]} {
    pack configure $netwag_priv_toolrun_widget($ctx,send) -before $netwag_priv_toolrun_widget($ctx,output) -fill x
  }

  if {[info exists netwag_priv_tools_backspace($toolnum)]} {
    global netwag_runtext_backspace
    set netwag_runtext_backspace($txt) 1
  }

  global netwag_priv_tool_nb
  netwag_notebook_button_press $netwag_priv_tool_nb 3

  netwag_runtext_init $txt "netwag_toolrun_cb_start" "netwag_toolrun_cb_end"
  netwag_runtext_run $txt $realcmd $usercmd

  set netwag_priv_toolrun_widget($ctx,realcmd) $realcmd
  set netwag_priv_toolrun_widget($ctx,usercmd) $usercmd

  netwag_priv_toolrun_ctx_change $ctx
}

## Press Copy_command button
proc netwag_toolrun_button_copycmd { ctx } {
  global netwag_priv_toolrun_widget

  set cmd $netwag_priv_toolrun_widget($ctx,usercmd)
  netwag_toolcmd_init $cmd

  return 0
}

## Press Interrupt button
proc netwag_toolrun_button_int { ctx } {
  global netwag_priv_toolrun_widget
  global netwag_runtext_state
  global netwag_runtext_state_paused
  global netwag_runtext_state_norun

  set txt $netwag_priv_toolrun_widget($ctx,output_text)

  if { $netwag_runtext_state($txt) == $netwag_runtext_state_paused } {
    set b $netwag_priv_toolrun_widget($ctx,buttons_pause)
    $b configure -text "Pause"
    netwag_dynhelp_change $b "To pause the display of a tool."
  }

  netwag_runtext_int $txt

  return 0
}

## Press Pause button
proc netwag_toolrun_button_pause { ctx } {
  global netwag_priv_toolrun_widget
  global netwag_runtext_state
  global netwag_runtext_state_norun
  global netwag_runtext_state_running
  global netwag_runtext_state_paused

  set txt $netwag_priv_toolrun_widget($ctx,output_text)

  netwag_runtext_pause $txt

  set b $netwag_priv_toolrun_widget($ctx,buttons_pause)
  if { $netwag_runtext_state($txt) == $netwag_runtext_state_paused } {
    netwag_dynhelp_change $b "To continue the display of a tool."
    $b configure -text "Continue"
  } else {
    netwag_dynhelp_change $b "To pause the display of a tool."
    $b configure -text "Pause"
  }
  netwag_update_idletasks

  return 0
}

## Press Send button
proc netwag_toolrun_button_send { ctx } {
  global netwag_priv_toolrun_widget

  set txt $netwag_priv_toolrun_widget($ctx,output_text)

  set t $netwag_priv_toolrun_widget($ctx,send_text)
  set e $netwag_priv_toolrun_widget($ctx,send_echo)

  set sendtext [netwag_misc_string_map $t {"\\\\" "\\" "\\0" "\0" "\\r" "\r" "\\n" "\n"}]

  netwag_runtext_send $txt $sendtext $e

  return 0
}

## Close current context
proc netwag_toolrun_current_close { } {
  global netwag_priv_toolrun_widget
  global netwag_priv_toolrun_ctx_current

  set ctx $netwag_priv_toolrun_ctx_current

  set txt $netwag_priv_toolrun_widget($ctx,output_text)
  netwag_runtext_int $txt
  netwag_runtext_close $txt

  set ctx [netwag_priv_toolrun_ctx_del $ctx]

  netwag_priv_toolrun_ctx_change $ctx
}

