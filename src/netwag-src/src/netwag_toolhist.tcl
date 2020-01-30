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
global netwag_priv_toolhist_lb

#################################################################
# PRIVATE FUNCTIONS
#################################################################

## Obtain the current selected command
proc netwag_priv_toolhist_cmd_init_cursel {} {
  global netwag_priv_toolhist_lb

  set sellist [$netwag_priv_toolhist_lb curselection]
  if { ! [llength $sellist] } {
    return ""
  }

  set selnum [lindex $sellist 0]
  set cmd [netwag_priv_toolhist_cmd_init_selnum $selnum]

  return $cmd
}

## Obtain the command at a given position
proc netwag_priv_toolhist_cmd_init_selnum { selnum } {
  global netwag_priv_toolhist_lb

  set seltext [$netwag_priv_toolhist_lb get $selnum]

  return $seltext
}

## Display information about current tool
proc netwag_priv_toolhist_info { } {
  global netwag_priv_toolhist_lb
  global netwag_priv_toolsearch_tools_sort_lbline

  set cmd [netwag_priv_toolhist_cmd_init_cursel]
  if {$cmd == ""} {
    return
  }

  set cmdlist [split $cmd]
  set toolnum [lindex $cmdlist 0]

  if {![info exists netwag_priv_toolsearch_tools_sort_lbline($toolnum)]} {
    return
  }

  set msg "Tool "
  append msg $netwag_priv_toolsearch_tools_sort_lbline($toolnum)
  netwag_msg_add_info $msg 0
}


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Called when the "Copy" button is changed
proc netwag_toolhist_button_copy { } {
  set cmd [netwag_priv_toolhist_cmd_init_cursel]
  if { $cmd == "" } {
    netwag_msg_add_warn "No history line is selected"
    return
  }
  netwag_toolcmd_init $cmd
}

## Called when the "Del" button is changed
proc netwag_toolhist_button_del { } {
  global netwag_priv_toolhist_lb

  set sellist [$netwag_priv_toolhist_lb curselection]
  if { ! [llength $sellist] } {
    netwag_msg_add_warn "No history line is selected"
    return
  }

  $netwag_priv_toolhist_lb del $sellist

  set selsize [$netwag_priv_toolhist_lb size]
  if { $selsize } {
    set toselect [lindex $sellist 0]
    if { $toselect < $selsize } {
      $netwag_priv_toolhist_lb selection set $toselect
    } else {
      $netwag_priv_toolhist_lb selection set end
    }
  }
}

## To add a command in the history
proc netwag_toolhist_add { cmd } {
  global netwag_priv_toolhist_lb

  # add at end
  $netwag_priv_toolhist_lb insert end "$cmd"
  $netwag_priv_toolhist_lb see end

  # now eventually remove it from previous history
  set arraysize [$netwag_priv_toolhist_lb size]
  for {set i 0} {$i < [expr $arraysize - 1]} {incr i} {
    set cmdi [$netwag_priv_toolhist_lb get $i]
    if {$cmdi == $cmd} {
      # must be done now, because after the del, it will be unset
      set sellist [$netwag_priv_toolhist_lb curselection]
      # remove the old entry
      $netwag_priv_toolhist_lb del $i
      # eventually reselect the item
      if { [llength $sellist] } {
        set cursel [lindex $sellist 0]
        if {$i == $cursel} {
          $netwag_priv_toolhist_lb selection set end
        }
      }
      # exit now because it should only be once
      return
      # note : if we decide to not return, do not forget "set i [expr $i - 1]"
    }
  }

}

## Called when the "Add" button is changed
proc netwag_toolhist_button_add { } {

  if { [netwag_toolcmd_check cmd toolnum] } {
    return
  }
  netwag_toolhist_add "$cmd"
}

## Initialize the hist function
proc netwag_toolhist_init { } {
  global netwag_priv_toolhist_lb

  # associate events
  bind $netwag_priv_toolhist_lb <Button-1> {
    focus $netwag_priv_toolhist_lb
  }
  bind $netwag_priv_toolhist_lb <ButtonRelease-1> {
    # this after is to be sure item is selected
    after 1 netwag_priv_toolhist_info
  }
  bind $netwag_priv_toolhist_lb <Key-Up> {
    after 1 netwag_priv_toolhist_info
  }
  bind $netwag_priv_toolhist_lb <Key-Down> {
    after 1 netwag_priv_toolhist_info
  }
  bind $netwag_priv_toolhist_lb <Key-Return> {
    netwag_toolhist_button_copy
  }
  bind $netwag_priv_toolhist_lb <Double-Button-1> {
    netwag_toolhist_button_copy
  }
  bind $netwag_priv_toolhist_lb <Key-Delete> {
    netwag_toolhist_button_del
  }
}
