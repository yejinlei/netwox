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
global netwag_priv_dynhelp_afterid
global netwag_priv_dynhelp_msg
global netwag_priv_dynhelp_name ; set netwag_priv_dynhelp_name ".netwag_priv_dynhelp_w"


#################################################################
# PRIVATE FUNCTIONS
#################################################################

## Event handlers
bind netwag_priv_dynhelp_evt <Enter> {
  global netwag_priv_dynhelp_afterid
  global netwag_glo_dynhelp_timeout
  global netwag_session_dynhelp_active
  if { $netwag_session_dynhelp_active == 0 } {continue}
  set netwag_priv_dynhelp_afterid [after $netwag_glo_dynhelp_timeout {netwag_priv_dynhelp_display %W %X %Y}]
}
bind netwag_priv_dynhelp_evt <Motion> {
  global netwag_priv_dynhelp_afterid
  global netwag_glo_dynhelp_timeout
  global netwag_session_dynhelp_active
  if { $netwag_session_dynhelp_active == 0 } {continue}
  after cancel $netwag_priv_dynhelp_afterid
  if {[winfo exists .netwag_priv_dynhelp] != 1} {
    set netwag_priv_dynhelp_afterid [after $netwag_glo_dynhelp_timeout {netwag_priv_dynhelp_display %W %X %Y}]
  }
}
bind netwag_priv_dynhelp_evt <Leave> {
  global netwag_session_dynhelp_active
  if { $netwag_session_dynhelp_active == 0 } {continue}
  netwag_priv_dynhelp_destroy
}
bind netwag_priv_dynhelp_evt <Button> {
  global netwag_session_dynhelp_active
  if { $netwag_session_dynhelp_active == 0 } {continue}
  netwag_priv_dynhelp_destroy
}

## Display a small window
proc netwag_priv_dynhelp_display {w x y} {
  global netwag_priv_dynhelp_msg
  global netwag_priv_dynhelp_name
  global netwag_glo_dynhelp_bgcolor
  global netwag_glo_dynhelp_wraplength
  global netwag_session_dynhelp_active

  if { $netwag_session_dynhelp_active == 0 } {return}
  if [catch {toplevel $netwag_priv_dynhelp_name} res] {
    # simply ignore the error, and do not create a new window
    return
  }
  wm overrideredirect $netwag_priv_dynhelp_name 1
  set x [expr $x + 0]
  set y [expr $y + 15]
  wm geometry $netwag_priv_dynhelp_name +$x+$y

  frame $netwag_priv_dynhelp_name.f -relief flat -bd 1 -bg black
  label $netwag_priv_dynhelp_name.f.l -anchor e -text $netwag_priv_dynhelp_msg($w) -bg $netwag_glo_dynhelp_bgcolor -wraplength $netwag_glo_dynhelp_wraplength -font myfont -justify left
  pack $netwag_priv_dynhelp_name.f.l -anchor nw
  pack $netwag_priv_dynhelp_name.f
}

## Destroy the window
proc netwag_priv_dynhelp_destroy {} {
  global netwag_priv_dynhelp_afterid
  global netwag_priv_dynhelp_name
  global netwag_session_dynhelp_active

  if { $netwag_session_dynhelp_active == 0 } {return}
  after cancel $netwag_priv_dynhelp_afterid
  if {[winfo exists $netwag_priv_dynhelp_name] == 1} {
    destroy $netwag_priv_dynhelp_name
  }
}


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Initialize the message for an object
proc netwag_dynhelp_init {w message} {
  global netwag_priv_dynhelp_msg

  set netwag_priv_dynhelp_msg($w) $message
  bindtags $w "[bindtags $w] netwag_priv_dynhelp_evt"
}

## Change the message of an object
proc netwag_dynhelp_change {w message} {
  global netwag_priv_dynhelp_msg

  set netwag_priv_dynhelp_msg($w) $message
}

## Remove the message for an object
proc netwag_dynhelp_close {w} {
  set tmptags [bindtags $w]
  set posdebut [string first "netwag_priv_dynhelp_evt" $tmptags]
  set posfin [expr $posdebut + [string length "netwag_priv_dynhelp_evt"]]
  set posdebut [expr $posdebut - 1]
  set newtags [string range $tmptags 1 $posdebut]
  append newtags [string range $tmptags $posfin end]
  bindtags $w $newtags
}

## Activate/deactivate the dynhelp
proc netwag_dynhelp_activate { activate } {
  global netwag_priv_dynhelp_afterid
  global netwag_priv_dynhelp_name
  global netwag_session_dynhelp_active
  set netwag_session_dynhelp_active $activate
  if { !$activate } {
    catch {after cancel $netwag_priv_dynhelp_afterid}
    if {[winfo exists $netwag_priv_dynhelp_name] == 1} {
      destroy $netwag_priv_dynhelp_name
    }
  }
}

