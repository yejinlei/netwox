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
global netwag_priv_toolform_cv
global netwag_priv_toolform_cvs

global netwag_priv_toolform_numobj ; set netwag_priv_toolform_numobj 0
global netwag_priv_toolform_info

global netwag_priv_toolform_numradio ; set netwag_priv_toolform_numradio 3

# a bit tricky but necessary to hide passwords
global netwag_priv_toolform_hidden
global netwag_priv_toolform_hidden_next
set netwag_priv_toolform_hidden_next 0
global netwag_priv_toolform_hidden_pattern
set netwag_priv_toolform_hidden_pattern(0) "--Hidden-Password-Number-"
set netwag_priv_toolform_hidden_pattern(1) "--"

#################################################################
# COMMON FUNCTIONS
#################################################################

## Left "[X] -k"
proc netwag_common_toolform_add_left_key { numobj key } {
  global netwag_priv_toolform_cvf
  global netwag_priv_toolform_info
  global netwag_glo_gui_bgcolor_unused

  set f1 $netwag_priv_toolform_cvf.key($numobj)
  checkbutton $f1 -text "" -variable netwag_priv_toolform_info($numobj,isset) -font myfont -bg $netwag_glo_gui_bgcolor_unused -highlightthickness 0 -justify left -takefocus 0
  netwag_mw_form $f1
  netwag_dynhelp_init $f1 "Indicates if this line has to be generated."
  grid $f1 -row $numobj -column 1 -sticky ns
}
proc netwag_common_toolform_del_left_key { numobj } {
  global netwag_priv_toolform_cvf

  grid forget $netwag_priv_toolform_cvf.key($numobj)
  destroy $netwag_priv_toolform_cvf.key($numobj)
}

## Right "title"
proc netwag_common_toolform_add_right_title { numobj title } {
  global netwag_priv_toolform_cvf
  global netwag_priv_toolform_info
  global netwag_glo_gui_bgcolor_unused

  set f3 $netwag_priv_toolform_cvf.title($numobj)
  label $f3 -text $title -font myfont -highlightthickness 0 -justify left -bg $netwag_glo_gui_bgcolor_unused -wraplen 0
  netwag_mw_form $f3
  netwag_dynhelp_init $f3 "Description of field on left."
  grid $f3 -row $numobj -column 3 -sticky w
}
proc netwag_common_toolform_del_right_title { numobj } {
  global netwag_priv_toolform_cvf

  grid forget $netwag_priv_toolform_cvf.title($numobj)
  destroy $netwag_priv_toolform_cvf.title($numobj)
}
proc netwag_common_toolform_resize_right_title { numobj } {
  global netwag_priv_toolform_cv
  global netwag_priv_toolform_cvf

  set leftsize [lindex [grid bbox $netwag_priv_toolform_cvf 2 $numobj] 2]
  set totalsize [winfo width $netwag_priv_toolform_cv]
  set freespace [expr $totalsize - $leftsize - 40]

  $netwag_priv_toolform_cvf.title($numobj) configure -wraplen $freespace
}

## Entry
proc netwag_common_toolform_entry_tab { c } {
  if {[regexp "Tab" $c]} {
    return 0
  }
  if {[regexp "Shift" $c]} {
    return 0
  }
  return 1
}


#################################################################
# PRIVATE FUNCTIONS
#################################################################

########################################################################
## Generate a button
proc netwag_toolform_add_button { isreq key name title defaultval } {
  global netwag_priv_toolform_cvf
  global netwag_priv_toolform_info
  global netwag_glo_gui_bgcolor_unused

  global netwag_priv_toolform_numobj ; set numobj $netwag_priv_toolform_numobj

  # create "-key"
  netwag_common_toolform_add_left_key $numobj $key

  # create "[X]"
  set f2 $netwag_priv_toolform_cvf.button($numobj)
  checkbutton $f2 -text "" -variable netwag_priv_toolform_info($numobj,var) -font myfont -bg $netwag_glo_gui_bgcolor_unused -wraplen 0 -justify left -command "set netwag_priv_toolform_info($numobj,isset) 1"
  netwag_mw_form $f2
  netwag_dynhelp_init $f2 "Check or uncheck this button to set or unset variable described on right."
  grid $f2 -row $numobj -column 2 -sticky wens

  # create "title"
  netwag_common_toolform_add_right_title $numobj $title

  # save info
  set netwag_priv_toolform_info($numobj,type) "button"
  set netwag_priv_toolform_info($numobj,isset) $isreq
  set netwag_priv_toolform_info($numobj,var) $defaultval
  set netwag_priv_toolform_info($numobj,isreq) $isreq
  set netwag_priv_toolform_info($numobj,key) $key
  set netwag_priv_toolform_info($numobj,name) $name
  set netwag_priv_toolform_info($numobj,defaultval) $defaultval

  # ready for next
  incr netwag_priv_toolform_numobj
}
proc netwag_toolform_gene_button { numobj } {
  global netwag_priv_toolform_info
  if {!$netwag_priv_toolform_info($numobj,isset)} {
    return ""
  }
  set name $netwag_priv_toolform_info($numobj,name)
  if {$netwag_priv_toolform_info($numobj,var)} {
    return " --$name"
  }
  return " --no-$name"
}
proc netwag_toolform_rst_button { numobj } {
  global netwag_priv_toolform_info
  set netwag_priv_toolform_info($numobj,isset) $netwag_priv_toolform_info($numobj,isreq)
  set netwag_priv_toolform_info($numobj,var) $netwag_priv_toolform_info($numobj,defaultval)
}
proc netwag_toolform_update_button { numobj value } {
  global netwag_priv_toolform_info
  set netwag_priv_toolform_info($numobj,isset) 1
  set netwag_priv_toolform_info($numobj,var) $value
}
proc netwag_toolform_del_button { numobj } {
  global netwag_priv_toolform_cvf
  netwag_common_toolform_del_left_key $numobj
  grid forget $netwag_priv_toolform_cvf.button($numobj)
  destroy $netwag_priv_toolform_cvf.button($numobj)
  netwag_common_toolform_del_right_title $numobj
}
proc netwag_toolform_resize_button { numobj } {
  netwag_common_toolform_resize_right_title $numobj
}

########################################################################
## Generate a radio button
proc netwag_toolform_add_radio { numradio isreq key name title defaultval } {
  global netwag_priv_toolform_cvf
  global netwag_priv_toolform_info
  global netwag_glo_gui_bgcolor_unused

  global netwag_priv_toolform_numobj ; set numobj $netwag_priv_toolform_numobj

  # create "[X]"
  set f2 $netwag_priv_toolform_cvf.button($numobj)
  radiobutton $f2 -text "" -variable netwag_priv_toolform_info(radio,$numradio) -font myfont -bg $netwag_glo_gui_bgcolor_unused -wraplen 0 -justify left -command "set netwag_priv_toolform_info(radio,$numradio,set) 1" -value $name -activebackground $netwag_glo_gui_bgcolor_unused
  netwag_mw_form $f2
  netwag_dynhelp_init $f2 "Check or uncheck this button to set variable described on right."
  grid $f2 -row $numobj -column 2 -sticky wens
  if {$defaultval == 1} {
    $f2 select
  }

  # create "title"
  netwag_common_toolform_add_right_title $numobj $title

  # save info
  set netwag_priv_toolform_info($numobj,type) "radio"
  set netwag_priv_toolform_info($numobj,numradio) $numradio
  set netwag_priv_toolform_info($numobj,isset) 0
  set netwag_priv_toolform_info($numobj,isreq) $isreq
  set netwag_priv_toolform_info($numobj,key) $key
  set netwag_priv_toolform_info($numobj,name) $name
  set netwag_priv_toolform_info($numobj,defaultval) $defaultval

  # ready for next
  incr netwag_priv_toolform_numobj
}
proc netwag_toolform_gene_radio { numradio } {
  global netwag_priv_toolform_info
  if {!$netwag_priv_toolform_info(radio,$numradio,set)} {
    return ""
  }
  set name $netwag_priv_toolform_info(radio,$numradio)
  return " --$name"
}
proc netwag_toolform_rst_radio { numobj } {
  global netwag_priv_toolform_cvf
  global netwag_priv_toolform_info
  if {$netwag_priv_toolform_info($numobj,defaultval) == 1} {
    $netwag_priv_toolform_cvf.button($numobj) select
  }
}
proc netwag_toolform_update_radio { numobj value } {
  global netwag_priv_toolform_cvf
  global netwag_priv_toolform_info
  if {$value == 1} {
    set r $netwag_priv_toolform_info($numobj,numradio)
    set netwag_priv_toolform_info(radio,$r,set) 1
    $netwag_priv_toolform_cvf.button($numobj) select
  }
}
proc netwag_toolform_del_radio { numobj } {
  global netwag_priv_toolform_cvf
  grid forget $netwag_priv_toolform_cvf.button($numobj)
  destroy $netwag_priv_toolform_cvf.button($numobj)
  netwag_common_toolform_del_right_title $numobj
}
proc netwag_toolform_resize_radio { numobj } {
  netwag_common_toolform_resize_right_title $numobj
}

########################################################################
## Generate an entry
proc netwag_toolform_add_entry_show { numobj } {
  global netwag_priv_toolform_cvf
  global netwag_priv_toolform_info

  set f2 $netwag_priv_toolform_cvf.entry($numobj)
  if {$netwag_priv_toolform_info($numobj,show)} {
    set netwag_priv_toolform_info($numobj,show) 0
    $f2.e configure -show "*"
  } else {
    set netwag_priv_toolform_info($numobj,show) 1
    $f2.e configure -show ""
  }
}
proc netwag_toolform_add_entry_inc { numobj step } {
  global netwag_priv_toolform_info

  if {[catch {expr $netwag_priv_toolform_info($numobj,var) + $step} v]} {
    set v 0
  }
  if {$v < 0} {
    set v 0
  }

  set netwag_priv_toolform_info($numobj,isset) 1
  set netwag_priv_toolform_info($numobj,var) $v
}
proc netwag_toolform_add_entry_file { numobj } {
  global netwag_priv_toolform_info

  set initialdir {}
  set initialfile {}

  set oldvalue $netwag_priv_toolform_info($numobj,var)
  if {![file isdir $oldvalue]} {
    regsub "^.*/" "$oldvalue" {} initialfile
    set num 0
    while {![file isdir $oldvalue]} {
      set oldvalue [file dirname $oldvalue]
      incr num
      if {$num > 10} {
        # prevent infinite loop with "r:" under Windows
        set oldvalue {}
        break
      }
    }
  }
  set initialdir $oldvalue

  set optparadir {}
  set optparafile {}
  if {$initialdir != {}} {
    append optparadir " -initialdir \"$initialdir\""
    append optparafile " -initialdir \"$initialdir\""
  }
  if {$initialfile != {}} {
    append optparafile " -initialfile \"$initialfile\""
  }

  set enttype $netwag_priv_toolform_info($numobj,enttype)
  if {$enttype == "filerd"} {
    eval "set filename \[tk_getOpenFile $optparafile\]"
  } elseif {$enttype == "filewr"} {
    eval "set filename \[tk_getSaveFile $optparafile\]"
  } elseif {$enttype == "dirrd"} {
    eval "set filename \[tk_chooseDirectory -mustexist 1 $optparadir\]"
  } elseif {$enttype == "dirwr"} {
    eval "set filename \[tk_chooseDirectory -mustexist 0 $optparadir\]"
  } else {
    set filename "Internal error"
  }

  set netwag_priv_toolform_info($numobj,isset) 1
  set netwag_priv_toolform_info($numobj,var) $filename
}
proc netwag_toolform_add_entry_hist { numobj } {
  global netwag_priv_toolform_info

  set v [netwag_formhist_select $netwag_priv_toolform_info($numobj,enttype)]
  if {$v == ""} {
    return
  }

  set netwag_priv_toolform_info($numobj,isset) 1
  set netwag_priv_toolform_info($numobj,var) $v
}
proc netwag_toolform_add_entry { enttype isreq key name title defaultval } {
  global netwag_priv_toolform_cvf
  global netwag_priv_toolform_info
  global netwag_glo_gui_bgcolor_unused
  global netwag_glo_gui_bgcolor_text_edit
  global netwag_glo_gui_bgcolor_button

  global netwag_priv_toolform_numobj ; set numobj $netwag_priv_toolform_numobj

  # create "-key"
  netwag_common_toolform_add_left_key $numobj $key

  # create "entry [*][-][+][f/d][h]"
  set f2 $netwag_priv_toolform_cvf.entry($numobj)
  frame $f2 -bg $netwag_glo_gui_bgcolor_unused
  grid $f2 -row $numobj -column 2 -sticky we
  entry $f2.e -textvariable netwag_priv_toolform_info($numobj,var) -font myfont -highlightthickness 0 -bg $netwag_glo_gui_bgcolor_text_edit
  bind $f2.e <Button-1> "set netwag_priv_toolform_info($numobj,isset) 1"
  bind $f2.e <Key> "if {\[netwag_common_toolform_entry_tab %K\]} { set netwag_priv_toolform_info($numobj,isset) 1 }"
  netwag_mw_form $f2.e
  pack $f2.e -side left -expand 1 -fill x
  netwag_dynhelp_init $f2.e "Enter the value you want in this field."
  # *
  label $f2.show -relief raised -text "*" -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  eval "bind $f2.show <Button-1> {netwag_toolform_add_entry_show $numobj}"
  netwag_dynhelp_init $f2.show "Press to show or hide password."
  netwag_mw_form $f2.show
  # -
  label $f2.less -relief raised -text "-" -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  eval "bind $f2.less <Button-1> {netwag_toolform_add_entry_inc $numobj -1}"
  eval "bind $f2.less <Control-Button-1> {netwag_toolform_add_entry_inc $numobj -10}"
  netwag_dynhelp_init $f2.less "Decrease number by 1. If Control key is pressed, decrease by 10."
  netwag_mw_form $f2.less
  # +
  label $f2.more -relief raised -text "+" -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  eval "bind $f2.more <Button-1> {netwag_toolform_add_entry_inc $numobj +1}"
  eval "bind $f2.more <Control-Button-1> {netwag_toolform_add_entry_inc $numobj +10}"
  netwag_dynhelp_init $f2.more "Increase number by 1. If Control key is pressed, increase by 10."
  netwag_mw_form $f2.more
  # f/d
  label $f2.file -relief raised -text "f" -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  eval "bind $f2.file <Button-1> {netwag_toolform_add_entry_file $numobj}"
  netwag_dynhelp_init $f2.file "Press to select a File."
  netwag_mw_form $f2.file
  # h
  label $f2.hist -relief raised -text "h" -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  eval "bind $f2.hist <Button-1> {netwag_toolform_add_entry_hist $numobj}"
  netwag_dynhelp_init $f2.hist "Press to see History of previous values. They were set in other fields or in other tools."
  netwag_mw_form $f2.hist

  # special entry type configuration
  set netwag_priv_toolform_info($numobj,show) 1
  $f2.e configure -show ""
  if {$enttype == "password"} {
    $f2.e configure -width 18
    pack $f2.show -side left
    pack forget $f2.less
    pack forget $f2.more
    pack forget $f2.file
    pack forget $f2.hist
    netwag_toolform_add_entry_show $numobj
  } elseif {$enttype == "port" || $enttype == "uint32" || $enttype == "int32" || $enttype == "uint64" || $enttype == "int64"} {
    $f2.e configure -width 15
    pack forget $f2.show
    pack $f2.less -side left
    pack $f2.more -side left
    pack forget $f2.file
    pack $f2.hist -side left
  } elseif {$enttype == "filerd" || $enttype == "filewr" || $enttype == "dirrd" || $enttype == "dirwr"} {
    $f2.e configure -width 17
    pack forget $f2.show
    pack forget $f2.less
    pack forget $f2.more
    pack $f2.file -side left
    pack $f2.hist -side left
    if {$enttype == "filerd" || $enttype == "filewr"} {
      $f2.file configure -text "f"
      netwag_dynhelp_change $f2.file "Press to select a File."
    } else {
      $f2.file configure -text "d"
      netwag_dynhelp_change $f2.file "Press to select a Directory."
    }
  } else {
    $f2.e configure -width 20
    pack forget $f2.show
    pack forget $f2.less
    pack forget $f2.more
    pack forget $f2.file
    pack $f2.hist -side left
  }

  # create "title"
  netwag_common_toolform_add_right_title $numobj $title

  # save info
  set netwag_priv_toolform_info($numobj,type) "entry"
  set netwag_priv_toolform_info($numobj,enttype) $enttype
  set netwag_priv_toolform_info($numobj,isset) $isreq
  set netwag_priv_toolform_info($numobj,var) $defaultval
  set netwag_priv_toolform_info($numobj,isreq) $isreq
  set netwag_priv_toolform_info($numobj,key) $key
  set netwag_priv_toolform_info($numobj,name) $name
  set netwag_priv_toolform_info($numobj,defaultval) $defaultval

  # ready for next
  incr netwag_priv_toolform_numobj
}
proc netwag_toolform_gene_entry { numobj } {
  global netwag_priv_toolform_info
  global netwag_priv_toolform_hidden
  global netwag_priv_toolform_hidden_next
  global netwag_priv_toolform_hidden_pattern
  if {!$netwag_priv_toolform_info($numobj,isset)} {
    return ""
  }
  set enttype $netwag_priv_toolform_info($numobj,enttype)
  set name $netwag_priv_toolform_info($numobj,name)
  set val $netwag_priv_toolform_info($numobj,var)

  netwag_formhist_add $enttype "$val"
  if {$enttype == "port" || $enttype == "uint32" || $enttype == "int32" || $enttype == "uint64" || $enttype == "int64" || $enttype == "ip" || $enttype == "ip4" || $enttype == "ip6" || $enttype == "eth" || $enttype == "port"} {
    return " --$name $val"
  }

  set val [netwag_misc_string_replace $val "\\" "\\\\"]
  set val [netwag_misc_string_replace $val "\"" "\\\""]
  set val "\"$val\""

  if {$enttype == "password"} {
    set hid $netwag_priv_toolform_hidden_pattern(0)
    append hid $netwag_priv_toolform_hidden_next
    append hid $netwag_priv_toolform_hidden_pattern(1)
    set netwag_priv_toolform_hidden($netwag_priv_toolform_hidden_next) $val
    incr netwag_priv_toolform_hidden_next
    return " --$name$hid"
  }

  return " --$name $val"
}
proc netwag_toolform_rst_entry { numobj } {
  global netwag_priv_toolform_info
  set netwag_priv_toolform_info($numobj,isset) $netwag_priv_toolform_info($numobj,isreq)
  set netwag_priv_toolform_info($numobj,var) $netwag_priv_toolform_info($numobj,defaultval)
  set enttype $netwag_priv_toolform_info($numobj,enttype)
  if {$enttype == "password"} {
    set netwag_priv_toolform_info($numobj,show) 1
    netwag_toolform_add_entry_show $numobj
  }
}
proc netwag_toolform_update_entry { numobj value } {
  global netwag_priv_toolform_info
  set netwag_priv_toolform_info($numobj,isset) 1
  set netwag_priv_toolform_info($numobj,var) $value
}
proc netwag_toolform_del_entry { numobj } {
  global netwag_priv_toolform_cvf
  netwag_common_toolform_del_left_key $numobj
  grid forget $netwag_priv_toolform_cvf.entry($numobj)
  destroy $netwag_priv_toolform_cvf.entry($numobj)
  netwag_common_toolform_del_right_title $numobj
}
proc netwag_toolform_resize_entry { numobj } {
  netwag_common_toolform_resize_right_title $numobj
}

########################################################################
## Generate a listbox
proc netwag_toolform_add_listbox { isreq key name title listval defaultval } {
  global netwag_priv_toolform_cvf
  global netwag_priv_toolform_info
  global netwag_glo_gui_bgcolor_unused
  global netwag_glo_gui_bgcolor_listbox

  global netwag_priv_toolform_numobj ; set numobj $netwag_priv_toolform_numobj

  # create "-key"
  netwag_common_toolform_add_left_key $numobj $key

  # obtain listbox height/width
  set listboxwidth [string length $defaultval]
  set listboxheight 0
  set listboxscrol 0
  set defaultfound 0
  foreach val $listval {
    set len [string length $val]
    if {$len > $listboxwidth} { set listboxwidth $len }
    if {$val == $defaultval} { set defaultfound 1 }
    incr listboxheight
  }
  if {!$defaultfound && $defaultval != ""} { incr listboxheight }
  if {$listboxheight > 6} { set listboxscrol 1 ; set listboxheight 6 }

  # create listbox
  set f2 $netwag_priv_toolform_cvf.listbox($numobj)
  frame $f2 -bg $netwag_glo_gui_bgcolor_unused
  listbox $f2.l -yscrollcommand "$f2.s set" -font myfont -bg $netwag_glo_gui_bgcolor_listbox -highlightthickness 0 -height $listboxheight -exportselection false -width $listboxwidth
  scrollbar $f2.s -command "$f2.l yview" -troughcolor $netwag_glo_gui_bgcolor_listbox -highlightthickness 0 -takefocus 0
  pack $f2.l -side left -expand 1 -fill y
  bind $f2.l <Button-1> "focus $f2.l ; set netwag_priv_toolform_info($numobj,isset) 1"
  bind $f2.l <Key-Up> "set netwag_priv_toolform_info($numobj,isset) 1"
  bind $f2.l <Key-Down> "set netwag_priv_toolform_info($numobj,isset) 1"
  pack $f2.l -side left -expand 1 -fill x
  netwag_dynhelp_init $f2.l "Select value you want in this field."
  netwag_dynhelp_init $f2.s "Select value you want in this field."
  if {$listboxscrol} {
    pack $f2.s -side left -fill y
  } else {
    pack forget $f2.s
    netwag_mw_form $f2.l
  }
  grid $f2 -row $numobj -column 2 -sticky we

  # create "title"
  netwag_common_toolform_add_right_title $numobj $title

  # save info
  set netwag_priv_toolform_info($numobj,type) "listbox"
  set netwag_priv_toolform_info($numobj,lb) $f2.l
  set netwag_priv_toolform_info($numobj,isset) $isreq
  set netwag_priv_toolform_info($numobj,isreq) $isreq
  set netwag_priv_toolform_info($numobj,key) $key
  set netwag_priv_toolform_info($numobj,name) $name
  set netwag_priv_toolform_info($numobj,listval) $listval
  set netwag_priv_toolform_info($numobj,defaultval) $defaultval

  # display entries
  netwag_toolform_rst_listbox $numobj

  # ready for next
  incr netwag_priv_toolform_numobj
}
proc netwag_toolform_gene_listbox { numobj } {
  global netwag_priv_toolform_info
  if {!$netwag_priv_toolform_info($numobj,isset)} {
    return ""
  }
  set name $netwag_priv_toolform_info($numobj,name)
  set i [lindex [$netwag_priv_toolform_info($numobj,lb) curselection] 0]
  set val [$netwag_priv_toolform_info($numobj,lb) get $i]
  set val [netwag_misc_string_replace $val "\\" "\\\\"]
  set val [netwag_misc_string_replace $val "\"" "\\\""]
  return " --$name \"$val\""
}
proc netwag_toolform_rst_listbox { numobj } {
  global netwag_priv_toolform_info

  set netwag_priv_toolform_info($numobj,isset) $netwag_priv_toolform_info($numobj,isreq)

  $netwag_priv_toolform_info($numobj,lb) delete 0 end
  set defaultfound 0
  if {$netwag_priv_toolform_info($numobj,defaultval) == ""} {
    set defaultfound 1
  }
  foreach val $netwag_priv_toolform_info($numobj,listval) {
    $netwag_priv_toolform_info($numobj,lb) insert end $val
    if {!$defaultfound && $val==$netwag_priv_toolform_info($numobj,defaultval)} {
      $netwag_priv_toolform_info($numobj,lb) activate end
      set defaultfound 1
    }
  }
  if {!$defaultfound} {
    $netwag_priv_toolform_info($numobj,lb) insert end $netwag_priv_toolform_info($numobj,defaultval)
    $netwag_priv_toolform_info($numobj,lb) activate end
  }
  $netwag_priv_toolform_info($numobj,lb) selection set active
  $netwag_priv_toolform_info($numobj,lb) see active
}
proc netwag_toolform_update_listbox { numobj value } {
  global netwag_priv_toolform_info

  set isreq netwag_priv_toolform_info($numobj,isreq)
  set defaultval $netwag_priv_toolform_info($numobj,defaultval)
  set netwag_priv_toolform_info($numobj,isreq) 1
  set netwag_priv_toolform_info($numobj,defaultval) $value
  netwag_toolform_rst_listbox $numobj
  set netwag_priv_toolform_info($numobj,isreq) $isreq
  set netwag_priv_toolform_info($numobj,defaultval) $defaultval
}
proc netwag_toolform_del_listbox { numobj } {
  global netwag_priv_toolform_cvf
  netwag_common_toolform_del_left_key $numobj
  grid forget $netwag_priv_toolform_cvf.listbox($numobj)
  destroy $netwag_priv_toolform_cvf.listbox($numobj)
  netwag_common_toolform_del_right_title $numobj
}
proc netwag_toolform_resize_listbox { numobj } {
  netwag_common_toolform_resize_right_title $numobj
}

########################################################################
## Generate a more
proc netwag_toolform_add_more { title defaultval } {
  global netwag_priv_toolform_cvf
  global netwag_priv_toolform_info
  global netwag_glo_gui_bgcolor_unused
  global netwag_glo_gui_bgcolor_text_edit

  global netwag_priv_toolform_numobj ; set numobj $netwag_priv_toolform_numobj

  # create "- "
  netwag_common_toolform_add_left_key $numobj " "

  # create "entry"
  set f2 $netwag_priv_toolform_cvf.more($numobj)
  entry $f2 -textvariable netwag_priv_toolform_info($numobj,var) -font myfont -highlightthickness 0 -bg $netwag_glo_gui_bgcolor_text_edit -width 20
  bind $f2 <Button-1> "set netwag_priv_toolform_info($numobj,isset) 1"
  bind $f2 <Key> "if {\[netwag_common_toolform_entry_tab %K\]} { set netwag_priv_toolform_info($numobj,isset) 1 }"
  netwag_mw_form $f2
  netwag_dynhelp_init $f2 "Enter value you want in this field."
  grid $f2 -row $numobj -column 2 -sticky we

  # create "title"
  netwag_common_toolform_add_right_title $numobj $title

  # save info
  set netwag_priv_toolform_info($numobj,type) "more"
  set netwag_priv_toolform_info($numobj,isset) 0
  set netwag_priv_toolform_info($numobj,var) $defaultval
  # fake key to allow update
  set netwag_priv_toolform_info($numobj,key) "-"
  set netwag_priv_toolform_info($numobj,isreq) 0
  set netwag_priv_toolform_info($numobj,defaultval) $defaultval

  # ready for next
  incr netwag_priv_toolform_numobj
}
proc netwag_toolform_gene_more { numobj } {
  global netwag_priv_toolform_info
  if {!$netwag_priv_toolform_info($numobj,isset)} {
    return ""
  }
  set val $netwag_priv_toolform_info($numobj,var)
  if {$val == ""} {
    return ""
  }
  set val [netwag_misc_string_replace $val "\\" "\\\\"]
  return " $val"
}
proc netwag_toolform_rst_more { numobj } {
  global netwag_priv_toolform_info
  set netwag_priv_toolform_info($numobj,isset) $netwag_priv_toolform_info($numobj,isreq)
  set netwag_priv_toolform_info($numobj,var) $netwag_priv_toolform_info($numobj,defaultval)
}
proc netwag_toolform_update_more { numobj value } {
  global netwag_priv_toolform_info
  set netwag_priv_toolform_info($numobj,isset) 1
  set netwag_priv_toolform_info($numobj,var) $value
}
proc netwag_toolform_del_more { numobj } {
  global netwag_priv_toolform_cvf
  netwag_common_toolform_del_left_key $numobj
  grid forget $netwag_priv_toolform_cvf.more($numobj)
  destroy $netwag_priv_toolform_cvf.more($numobj)
  netwag_common_toolform_del_right_title $numobj
}
proc netwag_toolform_resize_more { numobj } {
  netwag_common_toolform_resize_right_title $numobj
}

########################################################################
## Generate a text
proc netwag_toolform_add_text { txt } {
  global netwag_priv_toolform_cvf
  global netwag_priv_toolform_info
  global netwag_glo_gui_bgcolor_unused
  global netwag_glo_gui_bgcolor_text_edit

  global netwag_priv_toolform_numobj ; set numobj $netwag_priv_toolform_numobj

  # create "text" on 3 columns
  set f1 $netwag_priv_toolform_cvf.text($numobj)
  label $f1 -text $txt -font myfont -highlightthickness 0 -wraplen 0 -justify left -bg $netwag_glo_gui_bgcolor_unused
  netwag_mw_form $f1
  grid $f1 -row $numobj -column 1 -columnspan 3 -sticky w

  # save info
  set netwag_priv_toolform_info($numobj,type) "text"
  set netwag_priv_toolform_info($numobj,key) "fake"
  set netwag_priv_toolform_info($numobj,isset) 0
  set netwag_priv_toolform_info($numobj,var) $txt
  # set netwag_priv_toolform_info($numobj,isreq) not used
  # set netwag_priv_toolform_info($numobj,defaultval) not used

  # ready for next
  incr netwag_priv_toolform_numobj
}
proc netwag_toolform_gene_text { numobj } {
  return ""
}
proc netwag_toolform_rst_text { numobj } {
}
proc netwag_toolform_del_text { numobj } {
  global netwag_priv_toolform_cvf
  grid forget $netwag_priv_toolform_cvf.text($numobj)
  destroy $netwag_priv_toolform_cvf.text($numobj)
}
proc netwag_toolform_resize_text { numobj } {
  global netwag_priv_toolform_cv
  global netwag_priv_toolform_cvf

  set totalsize [winfo width $netwag_priv_toolform_cv]
  set freespace [expr $totalsize - 40]

  $netwag_priv_toolform_cvf.text($numobj) configure -wraplen $freespace
}


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Uncheck all left buttons
proc netwag_toolform_uncheck {} {
  global netwag_priv_toolform_numobj
  global netwag_priv_toolform_info
  global netwag_priv_toolform_numradio

  for {set i 0} {$i < $netwag_priv_toolform_numobj} {incr i} {
    set netwag_priv_toolform_info($i,isset) 0
    if {$netwag_priv_toolform_info($i,type) == "radio"} {
      # also reset radio
      netwag_toolform_rst_radio $i
    }
  }

  for {set i 0} {$i < $netwag_priv_toolform_numradio} {incr i} {
    set netwag_priv_toolform_info(radio,$i,set) 0
  }
}

## Called when Generate is pressed
proc netwag_toolform_button_gen {} {
  global netwag_priv_toolform_numobj
  global netwag_priv_toolform_info
  global netwag_priv_toolhf_curtoolnum
  global netwag_priv_toolform_numradio

  set cmd $netwag_priv_toolhf_curtoolnum

  for {set i 0} {$i < $netwag_priv_toolform_numobj} {incr i} {
    if {$netwag_priv_toolform_info($i,type) == "button"} {
      append cmd [netwag_toolform_gene_button $i]
    } elseif {$netwag_priv_toolform_info($i,type) == "radio"} {
      # generated below
    } elseif {$netwag_priv_toolform_info($i,type) == "entry"} {
      append cmd [netwag_toolform_gene_entry $i]
    } elseif {$netwag_priv_toolform_info($i,type) == "listbox"} {
      append cmd [netwag_toolform_gene_listbox $i]
    } elseif {$netwag_priv_toolform_info($i,type) == "more"} {
      append cmd [netwag_toolform_gene_more $i]
    } elseif {$netwag_priv_toolform_info($i,type) == "text"} {
      append cmd [netwag_toolform_gene_text $i]
    } else {
      set type $netwag_priv_toolform_info($i,type)
      netwag_msg_add_warn "Cannot generate this type($type $i) of form"
    }
  }

  for {set i 0} {$i < $netwag_priv_toolform_numradio} {incr i} {
    append cmd [netwag_toolform_gene_radio $i]
  }

  netwag_toolcmd_init $cmd
}

## Called when Reset is pressed
proc netwag_toolform_button_rst {} {
  global netwag_priv_toolform_numobj
  global netwag_priv_toolform_info
  global netwag_priv_toolform_numradio

  for {set i 0} {$i < $netwag_priv_toolform_numobj} {incr i} {
    if {$netwag_priv_toolform_info($i,type) == "button"} {
      netwag_toolform_rst_button $i
    } elseif {$netwag_priv_toolform_info($i,type) == "radio"} {
      netwag_toolform_rst_radio $i
    } elseif {$netwag_priv_toolform_info($i,type) == "entry"} {
      netwag_toolform_rst_entry $i
    } elseif {$netwag_priv_toolform_info($i,type) == "listbox"} {
      netwag_toolform_rst_listbox $i
    } elseif {$netwag_priv_toolform_info($i,type) == "more"} {
      netwag_toolform_rst_more $i
    } elseif {$netwag_priv_toolform_info($i,type) == "text"} {
      netwag_toolform_rst_text $i
    } else {
      set type $netwag_priv_toolform_info($i,type)
      netwag_msg_add_warn "Cannot reset this type($type $i) of form"
    }
  }

  for {set i 0} {$i < $netwag_priv_toolform_numradio} {incr i} {
    set netwag_priv_toolform_info(radio,$i,set) 0
  }
}

## Update values
proc netwag_toolform_update { key value } {
  global netwag_priv_toolform_numobj
  global netwag_priv_toolform_info

  for {set i 0} {$i < $netwag_priv_toolform_numobj} {incr i} {
    if {$netwag_priv_toolform_info($i,key) != $key} {
      continue
    }
    if {$netwag_priv_toolform_info($i,type) == "button"} {
      netwag_toolform_update_button $i $value
    } elseif {$netwag_priv_toolform_info($i,type) == "radio"} {
      netwag_toolform_update_radio $i $value
    } elseif {$netwag_priv_toolform_info($i,type) == "entry"} {
      netwag_toolform_update_entry $i $value
    } elseif {$netwag_priv_toolform_info($i,type) == "listbox"} {
      netwag_toolform_update_listbox $i $value
    } elseif {$netwag_priv_toolform_info($i,type) == "more"} {
      netwag_toolform_update_more $i $value
    } else {
      set type $netwag_priv_toolform_info($i,type)
      netwag_msg_add_warn "Cannot update this type($type $i) of form"
    }
    break
  }
}

## To remove all fields
proc netwag_toolform_reinit {} {
  global netwag_priv_toolform_numobj
  global netwag_priv_toolform_info
  global netwag_priv_toolform_numradio

  for {set i 0} {$i < $netwag_priv_toolform_numobj} {incr i} {
    if {$netwag_priv_toolform_info($i,type) == "button"} {
      netwag_toolform_del_button $i
    } elseif {$netwag_priv_toolform_info($i,type) == "radio"} {
      netwag_toolform_del_radio $i
    } elseif {$netwag_priv_toolform_info($i,type) == "entry"} {
      netwag_toolform_del_entry $i
    } elseif {$netwag_priv_toolform_info($i,type) == "listbox"} {
      netwag_toolform_del_listbox $i
    } elseif {$netwag_priv_toolform_info($i,type) == "more"} {
      netwag_toolform_del_more $i
    } elseif {$netwag_priv_toolform_info($i,type) == "text"} {
      netwag_toolform_del_text $i
    } else {
      set type $netwag_priv_toolform_info($i,type)
      netwag_msg_add_warn "Cannot delete this type($type $i) of form"
    }
  }
  set netwag_priv_toolform_numobj 0

  for {set i 0} {$i < $netwag_priv_toolform_numradio} {incr i} {
    set netwag_priv_toolform_info(radio,$i,set) 0
  }
}

## To resize a displayed form
proc netwag_toolform_resize {} {
  global netwag_priv_toolform_cv
  global netwag_priv_toolform_cvf
  global netwag_priv_toolform_cvs
  global netwag_priv_toolform_numobj
  global netwag_priv_toolform_info

  # resize titles
  netwag_update_idletasks
  for {set i 0} {$i < $netwag_priv_toolform_numobj} {incr i} {
    if {$netwag_priv_toolform_info($i,type) == "button"} {
      netwag_toolform_resize_button $i
    } elseif {$netwag_priv_toolform_info($i,type) == "radio"} {
      netwag_toolform_resize_radio $i
    } elseif {$netwag_priv_toolform_info($i,type) == "entry"} {
      netwag_toolform_resize_entry $i
    } elseif {$netwag_priv_toolform_info($i,type) == "listbox"} {
      netwag_toolform_resize_listbox $i
    } elseif {$netwag_priv_toolform_info($i,type) == "more"} {
      netwag_toolform_resize_more $i
    } elseif {$netwag_priv_toolform_info($i,type) == "text"} {
      netwag_toolform_resize_text $i
    } else {
      set type $netwag_priv_toolform_info($i,type)
      netwag_msg_add_warn "Cannot resize this type($type $i) of form"
    }
  }
  netwag_update_idletasks

  # resize scroll region
  set sh [winfo height $netwag_priv_toolform_cvs]
  set ah [lindex [grid bbox $netwag_priv_toolform_cvf] 3]

  if { $ah < $sh } {
    # otherwise, scrolls strangely
    set y $sh
  } else {
    # +10 to see a little space before end
    set y [expr $ah + 10]
  }
  eval "$netwag_priv_toolform_cv configure -scrollregion {0 0 1 $y}"
}

## Display the description of a tool
proc netwag_toolform_display { toolnum } {
  global netwag_priv_toolform_numobj

  # remove old
  netwag_toolform_reinit

  # create new ones
  eval "netwag_toolform_display_func$toolnum"

  if {$netwag_priv_toolform_numobj == 1 && $toolnum != 0} {
    netwag_toolform_add_text "This tool does not need parameters"
  }

  # resize correctly
  netwag_toolform_resize
}
