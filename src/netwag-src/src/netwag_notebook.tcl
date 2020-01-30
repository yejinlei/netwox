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
global netwag_priv_notebook_tabs
global netwag_priv_notebook_numtab
global netwag_priv_notebook_butnum_pressed
global netwag_priv_notebook_resizecb

#################################################################
# PRIVATE FUNCTIONS
#################################################################
## Resize the frame
proc netwag_priv_notebook_resize { mainframe } {
  global netwag_priv_notebook_numtab
  set numtab $netwag_priv_notebook_numtab($mainframe)

  set w [winfo width $mainframe.body]
  set h [winfo height $mainframe.body]
  set h [expr $h - 1]
  # puts "res $w $h"
  for {set i 0} {$i < $numtab } {incr i} {
    $mainframe.body.pf$i.top.c configure -width $w
    $mainframe.body.pf$i.bot.left.c configure -height $h
  }

  # call callback function
  global netwag_priv_notebook_resizecb
  if {$netwag_priv_notebook_resizecb($mainframe) != ""} {
    $netwag_priv_notebook_resizecb($mainframe)
  }
}


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Create the notebook
proc netwag_notebook_init { mainframe tabs presscb {resizecb ""}} {
  global netwag_priv_notebook_tabs
  global netwag_priv_notebook_numtab
  global netwag_glo_gui_bgcolor_unused
  global netwag_priv_notebook_presscb
  global netwag_priv_notebook_resizecb

  # set all callbacks
  set numtab 0
  foreach cb $presscb {
    set netwag_priv_notebook_presscb($mainframe,$numtab) $cb
    incr numtab
  }
  # currently, only one, but we could use a list
  set netwag_priv_notebook_resizecb($mainframe) $resizecb

  frame $mainframe
  frame $mainframe.head -bg $netwag_glo_gui_bgcolor_unused
  frame $mainframe.body -bg $netwag_glo_gui_bgcolor_unused
  set numtab 0
  foreach ong $tabs {
    set netwag_priv_notebook_tabs($mainframe,$numtab) $ong
    button $mainframe.head.b$numtab -text "$ong" -font myfont -highlightthickness 0
    pack $mainframe.head.b$numtab -side left -padx 1
    incr numtab
  }
  set netwag_priv_notebook_numtab($mainframe) $numtab
  pack $mainframe.head -anchor w -fill x
  pack $mainframe.body -anchor w -expand 1 -fill both

  for {set i 0} {$i < $numtab} {incr i} {
    # create the place frame
    frame $mainframe.body.pf$i
    place $mainframe.body.pf$i -x 0 -y 0
    # create the frames inside
    frame $mainframe.body.pf$i.top
    frame $mainframe.body.pf$i.bot
    frame $mainframe.body.pf$i.bot.left
    frame $mainframe.body.pf$i.bot.right
    pack $mainframe.body.pf$i.top
    pack $mainframe.body.pf$i.bot -expand 1 -fill both
    pack $mainframe.body.pf$i.bot.left -side left
    pack $mainframe.body.pf$i.bot.right -expand 1 -fill both -side left
    # create the canvas used to force respect of sizes
    canvas $mainframe.body.pf$i.top.c -width 1 -height 1 -bg white -highlightthickness 0
    pack $mainframe.body.pf$i.top.c
    canvas $mainframe.body.pf$i.bot.left.c -width 1 -height 1 -bg $netwag_glo_gui_bgcolor_unused -highlightthickness 0
    pack $mainframe.body.pf$i.bot.left.c
    # bind the buttons
    set presscb "netwag_notebook_button_press $mainframe $i"
    if {[ info exist netwag_priv_notebook_presscb($mainframe,$i)]} {
      append presscb " ; "
      append presscb $netwag_priv_notebook_presscb($mainframe,$i)
    }
    eval "bind $mainframe.head.b$i <ButtonRelease-1> { $presscb }"
    eval "bind $mainframe.head.b$i <Key-space> { $presscb }"
    eval "bind $mainframe.head.b$i <Key-Return> { $presscb }"
  }

  eval "bind $mainframe.body <Configure> {netwag_priv_notebook_resize $mainframe }"

  # always see first (without callbacks)
  netwag_notebook_button_press $mainframe 0 0

  return $mainframe
}

## Simulate the press of a button
proc netwag_notebook_button_press { mainframe butnum {callcb 1} } {
  global netwag_priv_notebook_numtab
  global netwag_glo_notebook_button_visible_color
  global netwag_glo_notebook_button_hidden_color
  global netwag_priv_notebook_butnum_pressed
  global netwag_priv_notebook_presscb

  set numtab $netwag_priv_notebook_numtab($mainframe)
  if { $butnum >= $numtab } {
    error "netwag_notebook_button_press: butnum > numtab"
  }
  set netwag_priv_notebook_butnum_pressed($mainframe) $butnum

  raise $mainframe.body.pf$butnum

  for {set i 0} {$i < $numtab} {incr i} {
    if {$i == $butnum} {
      $mainframe.head.b$i configure -bg $netwag_glo_notebook_button_visible_color
    } else {
      $mainframe.head.b$i configure -bg $netwag_glo_notebook_button_hidden_color
    }
  }

  netwag_update

  if {$callcb} {
    if {[ info exist netwag_priv_notebook_presscb($mainframe,$butnum)]} {
      set presscb $netwag_priv_notebook_presscb($mainframe,$butnum)
      eval "$presscb"
    }
  }

}

## Get the frame associated to a button number
proc netwag_notebook_frame_get { mainframe butnum } {
  global netwag_priv_notebook_numtab
  set numtab $netwag_priv_notebook_numtab($mainframe)
  if { $butnum >= $numtab } {
    error "netwag_notebook_frame_get: butnum > numtab"
  }
  return $mainframe.body.pf$butnum.bot.right
}

## Get the button associated to a button number
proc netwag_notebook_button_get { mainframe butnum } {
  global netwag_priv_notebook_numtab
  set numtab $netwag_priv_notebook_numtab($mainframe)
  if { $butnum >= $numtab } {
    error "netwag_notebook_frame_get: butnum > numtab"
  }
  return $mainframe.head.b$butnum
}

## Get the button number currently pressed
proc netwag_notebook_butnum_get { mainframe } {
  global netwag_priv_notebook_butnum_pressed
  return $netwag_priv_notebook_butnum_pressed($mainframe)
}

