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
global netwag_priv_txtfind_config_w ; set netwag_priv_txtfind_config_w "."
global netwag_priv_txtfind_config_txt ; set netwag_priv_txtfind_config_txt ""
global netwag_priv_txtfind_config_nocase ; set netwag_priv_txtfind_config_nocase 0
global netwag_priv_txtfind_config_regexp ; set netwag_priv_txtfind_config_regexp 0
global netwag_priv_txtfind_config_back ; set netwag_priv_txtfind_config_back 0

global netwag_txtfind_windows_name; set netwag_txtfind_windows_name ".txtfind_window"

#################################################################
# PRIVATE FUNCTIONS
#################################################################

## Find a text
proc netwag_priv_txtfind_find { } {
  global netwag_priv_txtfind_config_w
  global netwag_priv_txtfind_config_txt
  global netwag_priv_txtfind_config_nocase
  global netwag_priv_txtfind_config_regexp
  global netwag_priv_txtfind_config_back

  set w $netwag_priv_txtfind_config_w
  set txt $netwag_priv_txtfind_config_txt

  # check searched text
  if {$txt == ""} {
    netwag_msg_add_warn "Enter text to search in Find window"
    return 0
  }

  # check if w is a text widget
  if {[catch {$w search " " 1.0}]} {
    netwag_msg_add_warn "Click in a text zone, then press Ctrl-F/Find"
    return 0
  }

  # obtain current position
  set curpos [$w index insert]
  set curline [lindex [split $curpos "."] 0]
  set curcol [lindex [split $curpos "."] 1]

  # search
  set searchopt ""
  set endpos "end"
  set startoff "1"
  if {$netwag_priv_txtfind_config_nocase} {
    append searchopt " -nocase"
  }
  if {$netwag_priv_txtfind_config_regexp} {
    append searchopt " -regexp"
  }
  if {$netwag_priv_txtfind_config_back} {
    append searchopt " -backwards"
    set endpos "1.0"
    set startoff "-1"
  }
  set startsearchat $curline.[expr $curcol + $startoff]
  set foundpos [eval $w search $searchopt -count foundlen -- {$txt} $startsearchat $endpos]

  # not found
  if {$foundpos == ""} {
    if {$netwag_priv_txtfind_config_regexp} {
      netwag_msg_add_info "Regexp \"$txt\" not found"
    } else {
      netwag_msg_add_info "String \"$txt\" not found"
    }
    return 0
  }

  # determine found position
  set foundline [lindex [split $foundpos "."] 0]
  set foundcol [lindex [split $foundpos "."] 1]
  set foundcolend [expr $foundcol + $foundlen]
  set foundposend $foundline.$foundcolend

  # select the found text
  $w tag remove sel 1.0 end
  $w tag add sel $foundpos $foundposend
  $w see $foundpos
  $w mark set insert $foundpos
  focus -force $w

}

## Popup the window
proc netwag_priv_txtfind_popup { } {
  global netwag_txtfind_windows_name
  global netwag_glo_gui_bgcolor_button
  global netwag_glo_gui_bgcolor_unused
  global netwag_glo_gui_bgcolor_text_edit
  global netwag_priv_txtfind_config_w
  global netwag_priv_txtfind_config_txt
  global netwag_priv_txtfind_config_nocase
  global netwag_priv_txtfind_config_regexp
  global netwag_priv_txtfind_config_back

  if { [winfo exists $netwag_txtfind_windows_name] } {
    raise $netwag_txtfind_windows_name
    return 0
  }

  # create window
  set fw "$netwag_txtfind_windows_name"
  set fwt "$fw.top"
  set fwb "$fw.bottom"
  set fwbl "$fwb.left"
  set fwbr "$fwb.right"
  set fwte "$fwt.entry"
  set fwblf1 "$fwbl.f1"
  set fwblf2 "$fwbl.f2"
  set fwblf3 "$fwbl.f3"
  set fwbln "$fwblf1.nocase"
  set fwblr "$fwblf2.regexp"
  set fwblb "$fwblf3.back"
  set fwbrf "$fwbr.find"
  set fwbrc "$fwbr.close"

  toplevel $fw
  wm title $fw "Find"
  bind $fw <Control-f> netwag_txtfind_find

  frame $fwt
  frame $fwb -bg $netwag_glo_gui_bgcolor_unused
  pack $fwt $fwb -fill x -expand 1
  frame $fwbl
  frame $fwbr
  pack $fwbl $fwbr -side left -fill x -expand 1

  entry $fwte -textvariable netwag_priv_txtfind_config_txt -font myfont -highlightthickness 0 -width 20 -bg $netwag_glo_gui_bgcolor_text_edit
  pack $fwte -fill x -expand 1
  netwag_dynhelp_init $fwte "Enter text to search"
  bind $fwte <Return> netwag_txtfind_find

  frame $fwblf1 -bg $netwag_glo_gui_bgcolor_unused
  frame $fwblf2 -bg $netwag_glo_gui_bgcolor_unused
  frame $fwblf3 -bg $netwag_glo_gui_bgcolor_unused
  pack $fwblf1 $fwblf2 $fwblf3 -fill x -expand 1 -anchor w
  checkbutton $fwbln -text "nocase" -variable netwag_priv_txtfind_config_nocase -font myfont -bg $netwag_glo_gui_bgcolor_unused -highlightthickness 0
  checkbutton $fwblr -text "regexp" -variable netwag_priv_txtfind_config_regexp -font myfont -bg $netwag_glo_gui_bgcolor_unused -highlightthickness 0
  checkbutton $fwblb -text "back" -variable netwag_priv_txtfind_config_back -font myfont -bg $netwag_glo_gui_bgcolor_unused -highlightthickness 0
  pack $fwbln $fwblr $fwblb -side left
  netwag_dynhelp_init $fwbln "Search ignoring case"
  netwag_dynhelp_init $fwblr "Search using a regular expression"
  netwag_dynhelp_init $fwblb "Search backwards"

  eval "button $fwbrf -text Find -font myfont -command { netwag_txtfind_find } -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0"
  eval "button $fwbrc -text Close -font myfont -command { destroy $netwag_txtfind_windows_name } -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0"
  pack $fwbrf $fwbrc -fill x -expand 1

  return 1
}


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Find a text
proc netwag_txtfind_find { } {
  global netwag_priv_txtfind_config_w
  global netwag_priv_txtfind_config_txt
  global netwag_priv_txtfind_config_nocase
  global netwag_priv_txtfind_config_regexp
  global netwag_priv_txtfind_config_back

  # eventually open the popup
  if {[netwag_priv_txtfind_popup]} {
    return
  }

  # search the text
  netwag_priv_txtfind_find

}

## Update current widget
proc netwag_txtfind_current { w } {
  global netwag_priv_txtfind_config_w

  set netwag_priv_txtfind_config_w $w
}


