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
global netwag_formhist_window_name; set netwag_formhist_window_name ".formhist_window"
global netwag_priv_formhist_current_enttype
global netwag_priv_formhist_current_formhist

#################################################################
# PRIVATE FUNCTIONS
#################################################################
## Guess type name of a value. Returns "uint", "ip4", "ip6", "eth",
## "host" or "?". Guess is not perfect, but sufficient.
proc netwag_priv_formhist_guess_type { pvalue } {
  upvar $pvalue value

  set value [string trim $value]

  if { [regexp {^[0-9]+$} $value] } {
    return "uint"
  }

  set sureip 0
  if { [string length $value] } {
    if { [string range $value 0 0] == "@" } {
      set value [string range $value 1 end]
      set sureip 1
    }
  }

  # lists are too complicated to recognize between ip4/ip6/eth/host
  if { [regexp {[=,-]} $value] } {
    return "list"
  }

  if { [regexp {^[0-9]+[.][0-9]+[.][0-9]+[.][0-9]+$} $value] } {
    return "ip4"
  }

  if { [regexp {^[A-Fa-f0-9][A-Fa-f0-9]?:[A-Fa-f0-9][A-Fa-f0-9]?:[A-Fa-f0-9][A-Fa-f0-9]?:[A-Fa-f0-9][A-Fa-f0-9]?:[A-Fa-f0-9][A-Fa-f0-9]?:[A-Fa-f0-9][A-Fa-f0-9]?$} $value] } {
    if {$sureip} {
      return "?"
    } else {
      return "eth"
    }
  }

  if { [regexp {:} $value] } {
    if { ! [regexp {[^A-Fa-f0-9:]} $value] } {
      return "ip6"
    }
    if { [regexp {^[A-Fa-f0-9:]+:[0-9]+[.][0-9]+[.][0-9]+[.][0-9]+$} $value]} {
      return "ip6"
    }
    return "?"
  }

  return "host"
}

## Add in a list
proc netwag_priv_formhist_add { enttype value } {
  global netwag_session_formhist

  # check if list eventually already contains it
  set idx [lsearch -exact $netwag_session_formhist($enttype) $value]
  if {$idx == 0} {
    # at beginning : no need to change
    return
  }
  if {$idx > 0} {
    # remove old
    set netwag_session_formhist($enttype) [lreplace $netwag_session_formhist($enttype) $idx $idx]
  }

  # add new at beginning
  set netwag_session_formhist($enttype) [linsert $netwag_session_formhist($enttype) 0 "$value"]

}

## Add an IP address
proc netwag_priv_formhist_add_ip { value } {
  netwag_priv_formhist_add "ip" "$value"
  netwag_priv_formhist_add "ips" "$value"
  if { $value == "0.0.0.0" || $value == "127.0.0.1" ||
       $value == "255.255.255.255" || $value == "::" || $value == "::1" } {
    return
  }
  netwag_priv_formhist_add "eth" "@$value"
  netwag_priv_formhist_add "eths" "@$value"
}

#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Display lists
proc netwag_formhist_display { } {
  global netwag_session_formhist
  global netwag_session_formhist_list

  foreach enttype $netwag_session_formhist_list {
    set line "$enttype:"
    foreach ad $netwag_session_formhist($enttype) {
      append line " $ad"
    }
    puts $line
  }
}

## Add address
proc netwag_formhist_add { enttype value } {
  global netwag_session_formhist

  if {$enttype == "filerd" || $enttype == "filewr"} {
    set enttype "file"
  }
  if {$enttype == "dirrd" || $enttype == "dirwr"} {
    set enttype "dir"
  }

  switch -exact -- $enttype {
    "ip" {
      set guessedtype [netwag_priv_formhist_guess_type value]
      switch -exact -- $guessedtype {
        "ip4" {
          netwag_priv_formhist_add_ip "$value"
          netwag_priv_formhist_add "ip4" "$value"
        }
        "ip6" {
          netwag_priv_formhist_add_ip "$value"
          netwag_priv_formhist_add "ip6" "$value"
        }
        "host" {
          netwag_priv_formhist_add_ip "$value"
        }
      }
    }
    "ip4" {
      set guessedtype [netwag_priv_formhist_guess_type value]
      switch -exact -- $guessedtype {
        "ip4" {
          netwag_priv_formhist_add_ip "$value"
          netwag_priv_formhist_add "ip4" "$value"
        }
        "host" {
          netwag_priv_formhist_add_ip "$value"
          netwag_priv_formhist_add "ip4" "$value"
        }
      }
    }
    "ip6" {
      set guessedtype [netwag_priv_formhist_guess_type value]
      switch -exact -- $guessedtype {
        "ip6" {
          netwag_priv_formhist_add_ip "$value"
          netwag_priv_formhist_add "ip6" "$value"
        }
        "host" {
          netwag_priv_formhist_add_ip "$value"
          netwag_priv_formhist_add "ip6" "$value"
        }
      }
    }
    "eth" {
      set guessedtype [netwag_priv_formhist_guess_type value]
      switch -exact -- $guessedtype {
        "eth" {
          netwag_priv_formhist_add "eth" "$value"
          netwag_priv_formhist_add "eths" "$value"
        }
      }
    }
    "port" {
      set guessedtype [netwag_priv_formhist_guess_type value]
      switch -exact -- $guessedtype {
        "uint" {
          netwag_priv_formhist_add "port" "$value"
          netwag_priv_formhist_add "ports" "$value"
        }
      }
    }
    "ips" {
      set guessedtype [netwag_priv_formhist_guess_type value]
      switch -exact -- $guessedtype {
        "ip4" {
          netwag_priv_formhist_add_ip "$value"
          netwag_priv_formhist_add "ip4" "$value"
        }
        "ip6" {
          netwag_priv_formhist_add_ip "$value"
          netwag_priv_formhist_add "ip6" "$value"
        }
        "host" {
          netwag_priv_formhist_add_ip "$value"
        }
        "list" {
          netwag_priv_formhist_add "ips" "$value"
        }
      }
    }
    "eths" {
      set guessedtype [netwag_priv_formhist_guess_type value]
      switch -exact -- $guessedtype {
        "eth" {
          netwag_priv_formhist_add "eths" "$value"
          netwag_priv_formhist_add "eth" "$value"
        }
        "list" {
          netwag_priv_formhist_add "eths" "$value"
        }
      }
    }
    "ports" {
      set guessedtype [netwag_priv_formhist_guess_type value]
      switch -exact -- $guessedtype {
        "uint" {
          netwag_priv_formhist_add "ports" "$value"
          netwag_priv_formhist_add "port" "$value"
        }
        "list" {
          netwag_priv_formhist_add "ports" "$value"
        }
      }
    }
    "uint32" {
      set guessedtype [netwag_priv_formhist_guess_type value]
      switch -exact -- $guessedtype {
        "uint" {
          netwag_priv_formhist_add "uint32" "$value"
        }
      }
    }
    "uint64" {
      set guessedtype [netwag_priv_formhist_guess_type value]
      switch -exact -- $guessedtype {
        "uint" {
          netwag_priv_formhist_add "uint64" "$value"
        }
      }
    }
    default {
      netwag_priv_formhist_add "$enttype" "$value"
    }
  }

}

## Select address
proc netwag_priv_formhist_window_create { } {
  global netwag_formhist_window_name
  global netwag_glo_gui_bgcolor_unused
  global netwag_glo_gui_bgcolor_button
  global netwag_glo_gui_bgcolor_listbox

  # names
  set aw "$netwag_formhist_window_name"
  set awb "$aw.border"
  set awbc "$awb.checklist"
  set awbcl "$awbc.left"
  set awbcll "$awbcl.list"
  set awbcls "$awbcl.xscro"
  set awbcs "$awbc.yscro"
  set awbb "$awb.buttons"
  set awbbd "$awbb.del"
  set awbbc "$awbb.cancel"
  set awbbu "$awbb.use"

  # create window
  toplevel $aw
  wm overrideredirect $aw 1
  wm transient $aw .
  wm group $aw .

  frame $awb -bg $netwag_glo_gui_bgcolor_unused -borderwidth 4 -relief ridge
  pack $awb
  frame $awbc
  frame $awbcl
  scrollbar $awbcs -command "$awbcll yview" -troughcolor $netwag_glo_gui_bgcolor_listbox -highlightthickness 0
  pack $awbcl -side left -fill both -expand 1
  listbox $awbcll -yscrollcommand "$awbcs set" -xscrollcommand "$awbcls set" -exportselection false -bg $netwag_glo_gui_bgcolor_listbox -highlightthickness 0 -font myfont
  netwag_dynhelp_init $awbcll ""
  bind $awbcll <Double-Button-1> "netwag_priv_formhist_window_use"
  scrollbar $awbcls -command "$awbcll xview" -troughcolor $netwag_glo_gui_bgcolor_listbox -highlightthickness 0 -orient horizontal
  pack $awbcll -side top -fill both -expand 1
  pack $awbc -fill x
  frame $awbb -bg $netwag_glo_gui_bgcolor_unused
  button $awbbd -text "Del" -command "netwag_priv_formhist_window_del" -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0 -font myfont
  netwag_dynhelp_init $awbbd ""
  button $awbbc -text "Cancel" -command "netwag_priv_formhist_window_cancel" -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0 -font myfont
  netwag_dynhelp_init $awbbc "Close this window."
  button $awbbu -text "Use" -command "netwag_priv_formhist_window_use" -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0 -font myfont
  netwag_dynhelp_init $awbbu ""
  pack $awbbd $awbbc $awbbu -side left -fill both -expand 1
  pack $awbb -fill x
}
proc netwag_priv_formhist_window_set { enttype } {
  global netwag_formhist_window_name
  global netwag_priv_formhist_current_enttype

  set netwag_priv_formhist_current_enttype $enttype

  # names
  set aw "$netwag_formhist_window_name"
  set awb "$aw.border"
  set awbc "$awb.checklist"
  set awbcl "$awbc.left"
  set awbcll "$awbcl.list"
  set awbcls "$awbcl.xscro"
  set awbcs "$awbc.yscro"
  set awbb "$awb.buttons"
  set awbbd "$awbb.del"
  set awbbc "$awbb.cancel"
  set awbbu "$awbb.use"

  # decide which kind of selection
  if {$enttype == "ips" || $enttype == "eths" || $enttype == "ports"} {
    $awbcll configure -selectmode extended
    set plural "es"
  } else {
    $awbcll configure -selectmode browse
    set plural ""
  }

  # create items
  netwag_dynhelp_change $awbbd "Delete selected value$plural."
  netwag_dynhelp_change $awbbu "Use selected value$plural."
  netwag_dynhelp_change $awbcll "Select value$plural to use."

  # fill listbox
  global netwag_session_formhist
  $awbcll delete 0 end
  set witdh 0
  set height 0
  foreach a $netwag_session_formhist($enttype) {
    $awbcll insert end $a
    set s [string length $a]
    if {$s > $witdh} {
      set witdh $s
    }
    incr height
  }
  if {$witdh > 40} {
    set witdh 40
    pack $awbcls -side top -fill x
    set hlimit 4
  } elseif {$witdh == 40} {
    pack forget $awbcls
    set hlimit 6
  } else {
    incr witdh
    pack forget $awbcls
    set hlimit 6
  }
  if {$height > $hlimit} {
    set height $hlimit
    pack $awbcs -side left -fill y
  } elseif {$height == $hlimit} {
    pack forget $awbcs
  } else {
    incr height
    pack forget $awbcs
  }
  $awbcll configure -width $witdh -height $height
  $awbcll selection set 0
  $awbcll yview 0

  # position window
  set x [winfo pointerx .]
  set y [winfo pointery .]
  set x [expr $x - [font measure myfont "123456789012345678901"]]
  set y [expr $y + 5]
  wm geometry $aw +$x+$y

}
proc netwag_priv_formhist_window_use { } {
  global netwag_formhist_window_name
  global netwag_priv_formhist_current_enttype

  # names
  set aw "$netwag_formhist_window_name"
  set awbcll "$aw.border.checklist.left.list"

  # no selection
  if {[llength [$awbcll curselection]] == 0} {
    return
  }

  # construct list of addresses
  set ad ""
  set addcomma 0
  foreach a [$awbcll curselection] {
    if {$addcomma} {
      append ad ","
    }
    append ad [$awbcll get $a]
    set addcomma 1
  }

  # add in list
  netwag_formhist_add $netwag_priv_formhist_current_enttype $ad

  # window is not needed anymore
  destroy $aw

  # inform
  global netwag_priv_formhist_current_formhist
  set netwag_priv_formhist_current_formhist $ad
}
proc netwag_priv_formhist_window_del { } {
  global netwag_formhist_window_name
  global netwag_session_formhist
  global netwag_priv_formhist_current_enttype

  # names
  set aw "$netwag_formhist_window_name"
  set awbcll "$aw.border.checklist.left.list"

  # no selection
  set sellist [$awbcll curselection]
  if { ! [llength $sellist] } {
    return
  }

  # del
  set enttype $netwag_priv_formhist_current_enttype
  foreach idx $sellist {
    set netwag_session_formhist($enttype) [lreplace $netwag_session_formhist($enttype) $idx $idx]
  }
  $awbcll del $sellist

  # select best
  set selsize [$awbcll size]
  if { $selsize } {
    set toselect [lindex $sellist 0]
    if { $toselect < $selsize } {
      $awbcll selection set $toselect
    } else {
      $awbcll selection set end
    }
  }

}
proc netwag_priv_formhist_window_cancel { } {
  global netwag_formhist_window_name

  # window is not needed anymore
  destroy $netwag_formhist_window_name

  # inform
  global netwag_priv_formhist_current_formhist
  set netwag_priv_formhist_current_formhist ""
}


proc netwag_formhist_select { enttype } {
  global netwag_formhist_window_name
  global netwag_glo_gui_bgcolor_unused
  global netwag_glo_gui_bgcolor_button
  global netwag_glo_gui_bgcolor_listbox

  if {$enttype == "filerd" || $enttype == "filewr"} {
    set enttype "file"
  }
  if {$enttype == "dirrd" || $enttype == "dirwr"} {
    set enttype "dir"
  }

  if { ! [winfo exists $netwag_formhist_window_name] } {
    netwag_priv_formhist_window_create
  }
  # We raise the window in all cases, because Windows 7 opens it
  # behind the current window.
  raise $netwag_formhist_window_name
  netwag_priv_formhist_window_set $enttype

  global netwag_priv_formhist_current_formhist
  vwait netwag_priv_formhist_current_formhist
  # We refocus, because Windows 7 lose the focus, and does not
  # allow the text to be selected after (the -force is required).
  focus -force .
  return $netwag_priv_formhist_current_formhist
}

