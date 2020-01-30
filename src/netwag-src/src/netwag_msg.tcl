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
global netwag_priv_msg_textframe
global netwag_priv_msg_nummsg

global netwag_priv_msg_repeatnum ; set netwag_priv_msg_repeatnum 1
global netwag_priv_msg_repeatlast ; set netwag_priv_msg_repeatlast ""

#################################################################
# PRIVATE FUNCTIONS
#################################################################
## Add a message and a newline
proc netwag_priv_msg_add { msg {errorlevel 0} {repeatit 1} } {
  global netwag_priv_msg_textframe
  global netwag_glo_msg_text_height
  global netwag_priv_msg_nummsg
  global netwag_priv_msg_repeatnum
  global netwag_priv_msg_repeatlast

  # suppress optional lines
  set msg [netwag_misc_string_map $msg {"\r" " " "\n" " "}]
  # suppress optional spaces
  set msg [string trim $msg]

  if {$msg == $netwag_priv_msg_repeatlast} {
    if {!$repeatit} {
      return
    }
    incr netwag_priv_msg_repeatnum
    set msg "$msg (repeated $netwag_priv_msg_repeatnum)"
  } else {
    set netwag_priv_msg_repeatnum 1
    set netwag_priv_msg_repeatlast $msg
  }

  $netwag_priv_msg_textframe configure -state normal
  if { $errorlevel == 0 } {
    $netwag_priv_msg_textframe insert end "$msg\n" netwag_priv_msg_info
  } elseif { $errorlevel == 1 } {
    $netwag_priv_msg_textframe insert end "$msg\n" netwag_priv_msg_warn
  } else {
    $netwag_priv_msg_textframe insert end "$msg\n" netwag_priv_msg_err
  }
  $netwag_priv_msg_textframe configure -state disabled

  incr netwag_priv_msg_nummsg
  if { $netwag_priv_msg_nummsg <= $netwag_glo_msg_text_height } {
    $netwag_priv_msg_textframe see 1.0
  } else {
    $netwag_priv_msg_textframe see "end -2c"
  }
}


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Set the global variable indicating where is the message zone
proc netwag_msg_init { textframe } {
  global netwag_priv_msg_textframe
  global netwag_glo_msg_color_info
  global netwag_glo_msg_color_warn
  global netwag_glo_msg_color_err
  global netwag_priv_msg_nummsg

  set netwag_priv_msg_nummsg 0

  set netwag_priv_msg_textframe $textframe
  $netwag_priv_msg_textframe configure -state disabled

  $netwag_priv_msg_textframe tag configure netwag_priv_msg_info -foreground $netwag_glo_msg_color_info
  $netwag_priv_msg_textframe tag configure netwag_priv_msg_warn -foreground $netwag_glo_msg_color_warn
  $netwag_priv_msg_textframe tag configure netwag_priv_msg_err -foreground $netwag_glo_msg_color_err
}

## Empty the message box
proc netwag_msg_purge { } {
  global netwag_priv_msg_textframe
  global netwag_priv_msg_nummsg

  $netwag_priv_msg_textframe configure -state normal
  $netwag_priv_msg_textframe delete 1.0 end
  $netwag_priv_msg_textframe configure -state disabled

  set netwag_priv_msg_nummsg 0
}

## Add a message and a newline
proc netwag_msg_add_info { msg {repeatit 1} } {
  netwag_priv_msg_add $msg 0 $repeatit
}
proc netwag_msg_add_warn { msg {repeatit 1} } {
  netwag_priv_msg_add $msg 1 $repeatit
}
proc netwag_msg_add_err { msg {repeatit 1} } {
  netwag_priv_msg_add $msg 2 $repeatit
}


