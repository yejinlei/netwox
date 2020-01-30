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
global netwag_font_windows_name; set netwag_font_windows_name ".font_window"
global netwag_font_isfixed; set netwag_font_isfixed 1


#################################################################
# PRIVATE FUNCTIONS
#################################################################


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Create the skeleton
proc netwag_font_init { } {
  global netwag_session_font_family
  global netwag_session_font_size
  global netwag_font_isfixed

  font create myfont -family $netwag_session_font_family -size $netwag_session_font_size
  set netwag_font_isfixed [font metric myfont -fixed]
}

## Change the font
proc netwag_font_change { fam size } {
  global netwag_session_font_family
  global netwag_session_font_size
  global netwag_font_isfixed

  if {$fam != ""} {
    set netwag_session_font_family $fam
    font configure myfont -family $fam
  }
  if {$size != ""} {
    set netwag_session_font_size $size
    font configure myfont -size $size
  }

  set netwag_font_isfixed [font metric myfont -fixed]

  netwag_toolform_resize
}

## Change the font opening a popup
proc netwag_font_changepopup { } {
  global netwag_session_font_family
  global netwag_session_font_size
  global netwag_font_windows_name
  global netwag_glo_gui_bgcolor_button
  global netwag_glo_gui_bgcolor_listbox
  global netwag_glo_gui_bgcolor_unused
  global netwag_font_isfixed

  # obtain the old configuration
  set oldfam $netwag_session_font_family
  set oldsize $netwag_session_font_size

  # create window
  set fw "$netwag_font_windows_name"
  set fwc "$fw.checklists"
  set fwb "$fw.buttons"
  set fwcf "$fwc.font"
  set fwcs "$fwc.size"
  set fwbo "$fwb.ok"
  set fwbi "$fwb.ignore"

  toplevel $fw
  wm title $fw "Choose font"
  frame $fwc
  frame $fwb -bg $netwag_glo_gui_bgcolor_unused
  pack $fwc -fill both -expand 1
  pack $fwb -fill x
  frame $fwcf
  frame $fwcs
  pack $fwcf $fwcs -side left -fill both -expand 1

  # create listbox containing the families
  listbox $fwcf.box -yscrollcommand "$fwcf.scro set" -exportselection false -bg $netwag_glo_gui_bgcolor_listbox -highlightthickness 0
  scrollbar $fwcf.scro -command "$fwcf.box yview" -troughcolor $netwag_glo_gui_bgcolor_listbox -highlightthickness 0
  pack $fwcf.box -side left -fill both -expand 1
  pack $fwcf.scro -side left -fill y
  set i 0
  set isel -1
  foreach fam [lsort [font families]] {
    $fwcf.box insert end $fam
    if { $fam == $oldfam } { set isel $i }
    incr i
  }
  if { $isel != -1 } {
    $fwcf.box selection set $isel
    $fwcf.box yview $isel
  }
  bind $fwcf.box <Button-1> {
    set fam [%W get [%W nearest %y]]
    netwag_font_change $fam ""
  }
  netwag_dynhelp_init $fwcf.box "Choose the font to be used."

  # create listbox containing the sizes
  listbox $fwcs.box  -yscrollcommand "$fwcs.scro set" -exportselection false -bg $netwag_glo_gui_bgcolor_listbox -highlightthickness 0
  scrollbar $fwcs.scro -command "$fwcs.box yview" -troughcolor $netwag_glo_gui_bgcolor_listbox -highlightthickness 0
  pack $fwcs.box -side left -fill both -expand 1
  pack $fwcs.scro -side left -fill y
  set imax [expr 2 * $oldsize]
  if { $imax < 30 } { set imax 30 }
  set isel -1
  for {set i 1} {$i < $imax} {incr i} {
    $fwcs.box insert end $i
    if { $i == $oldsize } { set isel $i }
  }
  if { $isel != -1 } {
    $fwcs.box selection set $isel
    $fwcs.box yview $isel
  }
  bind $fwcs.box <Button-1> {
    set size [%W get [%W nearest %y]]
    netwag_font_change "" $size
  }
  netwag_dynhelp_init $fwcs.box "Choose the size for the font."

  # create buttons
  eval "button $fwb.bi -text Ignore -command { netwag_font_change \"$oldfam\" $oldsize ; destroy $fw } -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0"
  eval "button $fwb.bo -text OK -command { destroy $fw } -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0"
  pack $fwb.bi $fwb.bo -side left -expand 1
  netwag_dynhelp_init $fwb.bi "Revert to the old font."
  netwag_dynhelp_init $fwb.bo "Choose the current font."

}

