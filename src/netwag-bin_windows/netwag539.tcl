## You can modify following values :
# netwag_glo_dir_tmp :
#   description : temporary directory
#   examples : "/tmp", "$env(TEMP)", "C:/TEMP", "$env(HOME)/tmp"
# netwag_glo_file_session :
#   description : default filename containing the last session
#                 this file will be created the first time
#   examples : "/tmp/netwag.ses", "$env(HOME)/.netwag.ses",
#              "$netwag_glo_dir_tmp/netwag.ses"
# netwag_glo_bin_netwox :
#   description : path to netwox
#   examples : "netwox", "/usr/bin/netwox", "c:/netwox.exe"
# Note: Those paths cannot contain spaces. For example replace
#       "C:\\Documents and Settings" with "C:\\Docume~1"
set netwag_glo_dir_tmp "$env(TEMP)"
if {[info exists env(APPDATA)]} {
  set netwag_glo_file_session "$env(APPDATA)/netwag.ses"
} else {
  set netwag_glo_file_session "$env(TEMP)/netwag.ses"
}
set netwag_glo_bin_netwox "netwox539"


#CHANGE NOTHING BELOW unless you know what you do
#CHANGE NOTHING BELOW unless you know what you do

set netwag_glo_versionmajor 5
set netwag_glo_versionminor 39
set netwag_glo_versionmicro 0

# color of buttons for invisible frames of notebook
set netwag_glo_notebook_button_hidden_color grey55

# color of button of the top frame of notebook
set netwag_glo_notebook_button_visible_color grey

# timeout for dynamic help in milliseconds
set netwag_glo_dynhelp_timeout 500

# background color for dynamic help
set netwag_glo_dynhelp_bgcolor "#FFFFBB"

# wrap size for dynamic help
set netwag_glo_dynhelp_wraplength 200

# size of the message listbox
set netwag_glo_msg_text_height 3

# size of the clipboard listbox
set netwag_glo_clipboard_text_height 0

# size of the run command
set netwag_glo_gui_toolcmd_height 3

# number of max crush line (limit to around 50k*80 = 4Mbytes)
set netwag_glo_gui_toolrun_crush 50000

# background colors for main interface
#  listboxes
set netwag_glo_gui_bgcolor_listbox "#D0FFD0"
#  buttons
set netwag_glo_gui_bgcolor_button grey
#  background for unused zones
set netwag_glo_gui_bgcolor_unused grey
#  text which cannot be modified or selected
set netwag_glo_gui_bgcolor_text_display "#FFD0FF"
#  text which cannot be modified, but can be selected
set netwag_glo_gui_bgcolor_text_select "#D0FFFF"
#  text which can be modified and selected
set netwag_glo_gui_bgcolor_text_edit white

# messages colors
set netwag_glo_msg_color_info "#000000"
set netwag_glo_msg_color_warn "#FF00FF"
set netwag_glo_msg_color_err "#FF0000"

## CHANGE NOTHING BELOW
## CHANGE NOTHING BELOW
#################################################################
#################################################################
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
global netwag_sys_platform;
global netwag_sys_os;

global netwag_sys_platform_unknown; set netwag_sys_platform_unknown 1
global netwag_sys_platform_unix; set netwag_sys_platform_unix 2
global netwag_sys_platform_windows; set netwag_sys_platform_windows 3
global netwag_sys_platform_macintosh; set netwag_sys_platform_macintosh 4

global netwag_sys_os_unknown; set netwag_sys_os_unknown 1
global netwag_sys_os_linux; set netwag_sys_os_linux 2
global netwag_sys_os_freebsd; set netwag_sys_os_freebsd 3
global netwag_sys_os_openbsd; set netwag_sys_os_openbsd 4
global netwag_sys_os_solaris; set netwag_sys_os_solaris 5
global netwag_sys_os_macos; set netwag_sys_os_macos 6
global netwag_sys_os_windows95; set netwag_sys_os_windows95 100
global netwag_sys_os_windows98; set netwag_sys_os_windows98 101
global netwag_sys_os_windowsme; set netwag_sys_os_windowsme 102
global netwag_sys_os_windowsnt4; set netwag_sys_os_windowsnt4 110
global netwag_sys_os_windows2000; set netwag_sys_os_windows2000 111
global netwag_sys_os_windowsxp; set netwag_sys_os_windowsxp 112
global netwag_sys_os_windows2003; set netwag_sys_os_windows2003 113


#################################################################
# PRIVATE FUNCTIONS
#################################################################


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Initialize global variables containing system
proc netwag_sys_init { } {
  global tcl_platform
  global netwag_sys_platform
  global netwag_sys_os
  global netwag_sys_platform_unknown
  global netwag_sys_platform_unix
  global netwag_sys_platform_windows
  global netwag_sys_platform_macintosh
  global netwag_sys_os_unknown
  global netwag_sys_os_linux
  global netwag_sys_os_freebsd
  global netwag_sys_os_openbsd
  global netwag_sys_os_solaris
  global netwag_sys_os_macos
  global netwag_sys_os_windows95
  global netwag_sys_os_windows98
  global netwag_sys_os_windowsme
  global netwag_sys_os_windowsnt4
  global netwag_sys_os_windows2000
  global netwag_sys_os_windowsxp
  global netwag_sys_os_windows2003

  if { $tcl_platform(platform) == "unix" } {
    set netwag_sys_platform $netwag_sys_platform_unix
  } elseif { $tcl_platform(platform) == "windows" } {
    set netwag_sys_platform $netwag_sys_platform_windows
  } elseif { $tcl_platform(platform) == "macintosh" } {
    set netwag_sys_platform $netwag_sys_platform_macintosh
  } else {
    set netwag_sys_platform $netwag_sys_platform_unknown
  }

  set osv $tcl_platform(osVersion)
  set netwag_sys_os $netwag_sys_os_unknown
  if { $tcl_platform(os) == "Linux" } {
    set netwag_sys_os $netwag_sys_os_linux
  } elseif { $tcl_platform(os) == "FreeBSD" } {
    set netwag_sys_os $netwag_sys_os_freebsd
  } elseif { $tcl_platform(os) == "OpenBSD" } {
    set netwag_sys_os $netwag_sys_os_openbsd
  } elseif { $tcl_platform(os) == "Solaris" } {
    set netwag_sys_os $netwag_sys_os_solaris
  } elseif { $tcl_platform(os) == "SunOS" } {
    set netwag_sys_os $netwag_sys_os_solaris
  } elseif { $tcl_platform(os) == "Darwin" } {
    set netwag_sys_os $netwag_sys_os_macos
  } elseif { $tcl_platform(os) == "Windows 95" } {
    set netwag_sys_os $netwag_sys_os_windows95
    if { $osv == "4.0" } {
      set netwag_sys_os $netwag_sys_os_windows95
    } elseif { $osv == "4.10" } {
      set netwag_sys_os $netwag_sys_os_windows98
    } elseif { $osv == "4.90" } {
      set netwag_sys_os $netwag_sys_os_windowsme
    }
  } elseif { $tcl_platform(os) == "Windows NT" } {
    set netwag_sys_os $netwag_sys_os_windows2000
    if { $osv == "4" } {
      set netwag_sys_os $netwag_sys_os_windowsnt4
    } elseif { $osv == "5.0" } {
      set netwag_sys_os $netwag_sys_os_windows2000
    } elseif { $osv == "5.1" } {
      set netwag_sys_os $netwag_sys_os_windowsxp
    } elseif { $osv == "5.2" } {
      set netwag_sys_os $netwag_sys_os_windows2003
    }
  }

}


#################################################################
# INITIALIZE SYSTEM VARIABLES
#################################################################
netwag_sys_init


#################################################################
# CHECK TCL/TK VERSION
#################################################################
proc netwag_glo_check_version { versiontype theversion wantedmajor } {
  # theversion is w.x.y.z...
  set pospoint [string first "." $theversion]
  set versionmajor [string range $theversion 0 [expr $pospoint - 1]]
  if { $versionmajor < $wantedmajor } {
    set errms "Version of $versiontype is too old ($versionmajor<$wantedmajor)"
    puts $errms
    label .l -text $errms
    pack .l
    update idletasks
    after 10000
    exit
  }
}
netwag_glo_check_version "Tk" $tk_version 8
netwag_glo_check_version "Tcl" $tcl_version 8
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


#################################################################
# PRIVATE FUNCTIONS
#################################################################
proc netwag_misc_string_replace_test { } {
  set ret [netwag_misc_string_replace "abcde" "c" "Z"]
  if { $ret != "abZde" } {
    puts "Bad output : $ret"
    exit
  }
  set ret [netwag_misc_string_replace "" "c" "Z"]
  if { $ret != "" } {
    puts "Bad output : $ret"
    exit
  }
  set ret [netwag_misc_string_replace "accde" "c" "ZY"]
  if { $ret != "aZYZYde" } {
    puts "Bad output : $ret"
    exit
  }
  set ret [netwag_misc_string_replace "ccdddc" "c" "ZY"]
  if { $ret != "ZYZYdddZY" } {
    puts "Bad output : $ret"
    exit
  }
  set ret [netwag_misc_string_replace "ccdddc" "cd" "ZY"]
  if { $ret != "cZYddc" } {
    puts "Bad output : $ret"
    exit
  }

  exit
}

proc netwag_misc_string_map_test { } {
  set ret [netwag_misc_string_map "abcde" {"c" "Z"}]
  if { $ret != "abZde" } {
    puts "Bad output : $ret"
    exit
  } else {puts "ok"}
  set ret [netwag_misc_string_map "" {"c" "Z"}]
  if { $ret != "" } {
    puts "Bad output : $ret"
    exit
  } else {puts "ok"}
  set ret [netwag_misc_string_map "accde" {"c" "ZY"}]
  if { $ret != "aZYZYde" } {
    puts "Bad output : $ret"
    exit
  } else {puts "ok"}
  set ret [netwag_misc_string_map "ccdddc" {"c" "ZY"}]
  if { $ret != "ZYZYdddZY" } {
    puts "Bad output : $ret"
    exit
  } else {puts "ok"}
  set ret [netwag_misc_string_map "ccdddc" {"cd" "ZY"}]
  if { $ret != "cZYddc" } {
    puts "Bad output : $ret"
    exit
  } else {puts "ok"}
  set ret [netwag_misc_string_map "abcdef" {"ab" "A" "de" "D"}]
  if { $ret != "AcDf" } {
    puts "Bad output : $ret"
    exit
  } else {puts "ok"}
  set ret [netwag_misc_string_map "1abcaababcabababc" {abc 1 ab 2 a 3 1 0}]
  if { $ret != "01321221" } {
    puts "Bad output : $ret"
    exit
  } else {puts "ok"}

}


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Replace data in a string (needed because "string map" does not
## exists in version 8.0)
proc netwag_misc_string_replace { str strtoreplace replaceby } {

  if {![catch {string map [list "$strtoreplace" "$replaceby"] $str} newstr]} {
    return $newstr
  }

  # note : we cannot use "string first" with 5 parameters because
  # it is not supported in version 8.0
  set newstr ""
  set searchin $str
  set lengthstrtoreplace [string length $strtoreplace]
  while {1} {
    set pos [string first $strtoreplace $searchin]
    if { $pos == -1 } {
      # append end
      append newstr $searchin
      return $newstr
    }
    append newstr [string range $searchin 0 [expr $pos - 1]]
    append newstr $replaceby
    set startat [expr $pos + $lengthstrtoreplace]
    set searchin [string range $searchin $startat end]
  }

}

## Replace data in a string (needed because "string map" does not
## exists in version 8.0)
proc netwag_misc_string_map { str listab } {

  if {![catch {string map $listab $str} newstr]} {
    return $newstr
  }

  set listlength [llength $listab]
  set j 0
  for {set i 0} {$i < $listlength} {incr i} {
    set strfrom($j) [lindex $listab $i]
    incr i
    if {$i ==  $listlength} {
      # ignore error
      break;
    }
    set strto($j) [lindex $listab $i]
    incr j
  }

  set newstr ""
  set searchin $str
  set arraysize [array size strfrom]
  while {1} {

    set found 0
    for {set i 0} {$i < $arraysize} {incr i} {
      set pos [string first $strfrom($i) $searchin]
      if {$pos != -1} {
        if {!$found} {
          set found 1
          set minpos $pos
          set maxlength [string length $strfrom($i)]
          set useitem $i
        } else {
          if {$pos < $minpos} {
            set minpos $pos
            set maxlength [string length $strfrom($i)]
            set useitem $i
          } elseif {$pos == $minpos} {
            if {[string length $strfrom($i)] > $maxlength} {
              set maxlength [string length $strfrom($i)]
              set useitem $i
            }
          }
        }
      }
    }

    if { ! $found } {
      # append end
      append newstr $searchin
      return $newstr
    }
    append newstr [string range $searchin 0 [expr $minpos - 1]]
    append newstr $strto($useitem)
    set startat [expr $minpos + $maxlength]
    set searchin [string range $searchin $startat end]
  }

}

## Generate a random number used for filenames
proc netwag_misc_bigrand { } {
  set randnum [expr int(rand()*1000000000)]
  set randnum [expr $randnum - 10000000 * int($randnum/10000000)]
  return $randnum
}

## Convert data to hexa printable
proc netwag_misc_debug_hexa { data } {
  set result ""
  set resulth ""
  set resultc ""
  set strlendata [string length $data]
  for {set i 0} {$i < $strlendata} {} {
    set c [string index $data $i]
    # create hexa string
    scan $c "%c" cb
    append resulth [format "%02X " $cb]
    # create char string
    if {$cb >= 32 && $cb <= 127} {
      append resultc $c
    } else {
      append resultc "."
    }
    # concatenate
    incr i
    if {!($i % 16)} {
      append result "$resulth    $resultc\n"
      set resulth ""
      set resultc ""
    }
  }
  set leftsize [expr $strlendata % 16]
  if {$leftsize} {
    append result "$resulth    "
    set leftsize [expr 16 - $leftsize]
    for {set i 0} {$i < $leftsize} {incr i} {
      append result "   "
    }
    append result "$resultc\n"
  }
  return $result
}

## Align text. For example (var="ab..n", size=10, leading="0000") :
# "0000abcdef\n"
# "0000ghijkl\n"
# "0000mn\n"
proc netwag_misc_align { var size {leading ""} } {

  set leadingsize [string length $leading]
  if {$leadingsize >= $size} {
    return "Error in alignment"
  }

  set var [string trim $var]

  set i 0
  set res $leading
  set ressize $leadingsize
  while { 1 } {
    set c [string index $var $i]
    if {$c == "\r"} {
      # ignore
    } elseif {$c == "\n"} {
      append res "\n$leading"
      set ressize $leadingsize
    } elseif {$c == ""} {
      break
    } else {
      if {$ressize == $size} {
        append res "\n$leading$c"
        set ressize [expr $leadingsize + 1]
      } else {
        append res $c
        incr ressize
      }
    }
    incr i
  }
  if {$ressize > $leadingsize} {
    append res "\n"
  }

  return "$res"
}
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
global netwag_update_can ; set netwag_update_can 0


#################################################################
# PRIVATE FUNCTIONS
#################################################################


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Allow update
proc netwag_update_allow { } {
  global netwag_update_can
  set netwag_update_can 1
}

## Call update
proc netwag_update { } {
  global netwag_update_can
  if {$netwag_update_can} {
    update
  }
}
proc netwag_update_idletasks { } {
  global netwag_update_can
  if {$netwag_update_can} {
    update idletasks
  }
}

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
global netwag_tmpfile_root ; set netwag_tmpfile_root ""


#################################################################
# PRIVATE FUNCTIONS
#################################################################


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Create a temporary file
# usage :
#set f [netwag_tmpfile_open filename]
#puts "$filename open"
#puts $f "blabla"
#close $f
proc netwag_tmpfile_open { {pfilename ""} } {
  global netwag_glo_dir_tmp
  global netwag_tmpfile_root

  if { $netwag_tmpfile_root == "" } {
    set netwag_tmpfile_root $netwag_glo_dir_tmp
    append netwag_tmpfile_root "/netwag."
  }

  set fn $netwag_tmpfile_root

  if {"$pfilename" != ""} {
    upvar $pfilename filename
  }

  while {1} {
    set randnum [netwag_misc_bigrand]
    set filename $fn$randnum
    set ret [catch {open $filename {WRONLY EXCL CREAT} 0600} fd]
    if { ! $ret } {
      return $fd
    }
  }
  return -1
}

## Delete the temporary file
proc netwag_tmpfile_del { filename {warnifabsent 1} } {
  global netwag_tmpfile_root

  if { $netwag_tmpfile_root == "" } {
    netwag_msg_add_err "netwag_tmpfile_del called to delete $filename, but netwag_tmpfile_root is unset"
    return
  }

  if { ! [regexp "^$netwag_tmpfile_root\[0-9\]*$" $filename] } {
    netwag_msg_add_err "netwag_tmpfile_del called to delete $filename, which was not created by netwag_tmpfile_open"
    return
  }

  file delete -- $filename
}

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
global netwag_varfile_error


#################################################################
# PRIVATE FUNCTIONS
#################################################################

## Set error messages
proc netwag_varfile_error_set { msg } {
  global netwag_varfile_error
  set netwag_varfile_error $msg
}

## Write variable value
proc netwag_priv_varfile_write_val { fd val } {
  set strlenval [string length $val]

  puts $fd $strlenval

  set chunksize 60
  set startpos 0
  while { $startpos < $strlenval } {
    set endpos [expr $startpos + $chunksize - 1]
    if { $endpos < $strlenval } {
      set trueline [string range $val $startpos $endpos]
    } else {
      set trueline [string range $val $startpos end]
    }
    set line [netwag_misc_string_replace $trueline "\\" "\\\\"]
    set line [netwag_misc_string_replace $line "\0" "\\0"]
    set line [netwag_misc_string_replace $line "\r" "\\r"]
    set line [netwag_misc_string_replace $line "\n" "\\n"]
    puts $fd $line
    set startpos [expr $endpos + 1]
  }

}

## Read variable value
proc netwag_priv_varfile_read_val { filename valname fd pval } {
  upvar $pval val

  # first, there is the size of the variable
  set ret [gets $fd line]
  if { $ret == -1 } {
    netwag_varfile_error_set "Could not read variable size for $valname from $filename"
    return -1
  }
  set varsize $line

  # special case of empty line
  if { $varsize == 0 } {
    # no data to read
    set val ""
    return 0
  }

  # now, read the value
  set val ""
  while {![eof $fd]} {
    set ret [gets $fd line]
    if { $ret == -1 } {
      netwag_varfile_error_set "Error when trying to read value for $valname from $filename"
      return -1
    }
    set line [netwag_misc_string_map $line {"\\\\" "\\" "\\0" "\0" "\\r" "\r" "\\n" "\n"}]
    append val $line
    set readsize [string length $val]
    if { $readsize == $varsize } {
      return 0
    } elseif { $readsize > $varsize } {
      netwag_varfile_error_set "Value contain too much data ($readsize>$varsize) for $valname from $filename"
      return -1
    }
  }
  return 0
}

## Read the varfile
proc netwag_priv_varfile_read_variable { filename fd pvarfilestruct } {
  upvar $pvarfilestruct varfilestruct

  # first, there is the name of the variable
  set ret [gets $fd line]
  if { $ret == -1 } {
    netwag_varfile_error_set "Could not read variable name from $filename"
    return -1
  }
  set varname $line

  # now, read the value
  set ret [netwag_priv_varfile_read_val $filename $varname $fd varval]
  if { $ret } {
    return -1
  }

  set varfilestruct($varname) $varval
  return 0
}

## Read the varfile
proc netwag_priv_varfile_read_array { filename fd pvarfilestruct } {
  upvar $pvarfilestruct varfilestruct

  # first, there is the name of the array
  set ret [gets $fd line]
  if { $ret == -1 } {
    netwag_varfile_error_set "Could not read array name from $filename"
    return -1
  }
  set arrayname $line

  # then, read the array size
  set ret [gets $fd line]
  if { $ret == -1 } {
    netwag_varfile_error_set "Could not read array size for $arrayname from $filename"
    return -1
  }
  set arraysize $line

  # now, read the values
  for {set i 0} {$i < $arraysize} {incr i} {
    set varname "$arrayname\($i\)"
    set ret [netwag_priv_varfile_read_val $filename $varname $fd varval]
    if { $ret } {
      return -1
    }
    set varfilestruct($arrayname,$i) $varval
  }
  return 0
}

## Read the varfile
proc netwag_priv_varfile_read_list { filename fd pvarfilestruct } {
  upvar $pvarfilestruct varfilestruct

  # first, there is the name of the list
  set ret [gets $fd line]
  if { $ret == -1 } {
    netwag_varfile_error_set "Could not read list name from $filename"
    return -1
  }
  set listname $line

  # then, read the list size
  set ret [gets $fd line]
  if { $ret == -1 } {
    netwag_varfile_error_set "Could not read list size for $listname from $filename"
    return -1
  }
  set listsize $line

  # now, read the values
  for {set i 0} {$i < $listsize} {incr i} {
    set varname "$listname at index $i"
    set ret [netwag_priv_varfile_read_val $filename $varname $fd varval]
    if { $ret } {
      return -1
    }
    lappend varfilestruct($listname) $varval
  }
  return 0
}


#################################################################
# PUBLIC WRITING FUNCTIONS
#################################################################

## Initialize the varfile
proc netwag_varfile_write_init { filename pfd } {
  upvar $pfd fd

  if {[file exists $filename]} {
    if  {[catch {file delete -- $filename}]} {
      netwag_varfile_error_set "Could not modify file $filename"
      return -1
    }
  }

  if  {[catch {open $filename {WRONLY EXCL CREAT} 0600} fd]} {
    netwag_varfile_error_set "Could not open $filename"
    return -1
  }
  # 2 (add support for LIST) is the current version of varfile syntax
  puts $fd "2"
  puts $fd "# This is a session file for netwag. You should not edit it."
  puts $fd ""
  return 0
}

## Close the varfile
proc netwag_varfile_write_close { fd } {
  close $fd
  return 0
}

## Write variable
proc netwag_varfile_write_variable { fd var val } {
  puts $fd "VARIABLE"
  puts $fd "$var"
  netwag_priv_varfile_write_val $fd $val
  puts $fd ""
  return 0
}

## Write array
proc netwag_varfile_write_array { fd var parray } {
  upvar $parray thearray
  puts $fd "ARRAY"
  puts $fd "$var"
  set arraysize [array size thearray]
  puts $fd $arraysize
  for {set i 0} {$i<$arraysize} {incr i} {
    netwag_priv_varfile_write_val $fd $thearray($i)
  }
  puts $fd ""
  return 0
}

## Write list
proc netwag_varfile_write_list { fd var plist } {
  upvar $plist thelist
  puts $fd "LIST"
  puts $fd "$var"

  set listsize [llength $thelist]
  puts $fd $listsize
    foreach v $thelist {
    netwag_priv_varfile_write_val $fd $v
  }
  puts $fd ""
  return 0
}

## Just for test
proc netwag_varfile_write_test {} {
  global netwag_varfile_error
  for {set i 0} {$i<30} {incr i} {
    set thearr($i) "aa $i bb"
  }
  set ret [netwag_varfile_write_init "/tmp/ses" fd]
  if { $ret } {
    puts "Error $netwag_varfile_error"
  }

  netwag_varfile_write_variable $fd "toto1" "blabla"
  netwag_varfile_write_variable $fd "toto2" "bl\nabla"
  netwag_varfile_write_variable $fd "toto3" "bl\0abla"
  netwag_varfile_write_variable $fd "toto4" 1
  netwag_varfile_write_variable $fd "toto5" ""
  netwag_varfile_write_array $fd "thearr" thearr
  netwag_varfile_write_close $fd
}


#################################################################
# PUBLIC READING FUNCTIONS
#################################################################

## Read the varfile
proc netwag_varfile_read { filename pvarfilestruct } {

  if  {[catch {set fd [open $filename "r"]}]} {
    netwag_varfile_error_set "Could not read $filename"
    return -1
  }

  # read the version
  set ret [gets $fd line]
  if { $ret == -1 } {
    netwag_varfile_error_set "Could not read version from $filename"
    close $fd
    return -1
  }
  set theversion $line
  if { $theversion != 1 && $theversion != 2 } {
    netwag_varfile_error_set "This program is too old to recognize sessions format version \"$theversion\", or the file is not a session file (from $filename)"
    close $fd
    return -1
  }

  upvar $pvarfilestruct varfilestruct
  while {![eof $fd]} {
    # get a line
    set ret [gets $fd line]
    if { $ret == -1 } {
      if {[eof $fd]} { break; }
      netwag_varfile_error_set "Could not read line from $filename"
      close $fd
      return -1
    }
    # ignore empty lines
    if { [regexp "^\s*$" $line] } { continue; }
    # ignore commented lines
    if { [regexp "^\s*#" $line] } { continue; }
    # check known parameters
    if { $line == "VARIABLE" } {
      set ret [netwag_priv_varfile_read_variable $filename $fd varfilestruct]
    } elseif { $line == "ARRAY" } {
      set ret [netwag_priv_varfile_read_array $filename $fd varfilestruct]
    } elseif { $line == "LIST" } {
      set ret [netwag_priv_varfile_read_list $filename $fd varfilestruct]
    } else {
      netwag_varfile_error_set "Parameter $line unrecognized in file $filename"
      set ret -1
    }
    if { $ret } {
      if {[eof $fd]} {
        netwag_varfile_error_set "Premature end of file $filename"
      }
      close $fd
      return -1
    }
  }

  if { [catch { close $fd }] } {
    netwag_varfile_error_set "Could not close $filename"
    return -1
  }
  return 0
}

## Just for test
proc netwag_varfile_read_test {} {
  global netwag_varfile_error
  set ret [netwag_varfile_read "/tmp/ses" titi]
  if { $ret } {
     puts "err $netwag_varfile_error"
     return
  }
  puts $titi(toto1)
  puts $titi(toto2)
  puts $titi(toto3)
  puts $titi(toto4)
  puts $titi(toto5)
  puts $titi(thearr,12)

}
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
global netwag_cursor_default_cursor;
set netwag_cursor_default_cursor [. cget -cursor]

#################################################################
# PRIVATE FUNCTIONS
#################################################################
global netwag_priv_cursor_w
set netwag_priv_cursor_w {. .m .fc.text .fm.text .fn.body.pf1.bot.right.text.left.text}


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Set waiting cursor
proc netwag_cursor_wait { } {
  # We should change cursor for each window using something like:
  #   catch {. configure -cursor watch}
  #   set list [winfo children .]
  #   while {[llength $list]} {
  #     set next {}
  #     foreach w $list {
  #       catch {$w configure -cursor watch}
  #       set next [concat $next [winfo children $w]]
  #     }
  #     set list $next
  #   }
  #   update idletasks
  # However, this is long to run. When displaying watch, it's because
  # we have a long task to run. We do not want it to become longer.
  # So only main windows will have the clock:
  global netwag_priv_cursor_w
  foreach w $netwag_priv_cursor_w {
    $w configure -cursor watch
  }
  netwag_update
}

## Set default cursor
proc netwag_cursor_default { } {
  global netwag_cursor_default_cursor;
  global netwag_priv_cursor_w
  foreach w $netwag_priv_cursor_w {
    $w configure -cursor $netwag_cursor_default_cursor
  }
  netwag_update
}
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

#################################################################
# PRIVATE FUNCTIONS
#################################################################

## compute scrolling step for each click
proc netwag_priv_mw_step {w D} {
  global netwag_priv_toolform_cv
  global netwag_priv_toolform_cvs
  global netwag_sys_platform
  global netwag_sys_platform_windows

  if {$w == $netwag_priv_toolform_cv || $w == $netwag_priv_toolform_cvs } {
    # Will scroll one by one under Windows, but 2 by 2 under unix
    # We don't go as fast as for other objects (*4) because user is
    # quickly lost
    if { $netwag_sys_platform == $netwag_sys_platform_windows } {
      set d [expr {- $D}]
    } else {
      set d [expr {- $D * 2}]
    }
  } else {
    set d [expr {- $D * 4}]
  }
  return $d
}

## scroll generic object
proc netwag_priv_mw_scroll_object {w D} {
  $w yview scroll [netwag_priv_mw_step $w $D] units
}

## scroll scrollbar (inspired from tk function tkScrollByUnits)
proc netwag_priv_mw_scroll_scrollbar {w D} {
  set cmd [$w cget -command]
  if {$cmd == ""} {
    return
  }
  set d [netwag_priv_mw_step $w $D]
  set info [$w get]
  if {[llength $info] == 2} {
    uplevel #0 $cmd scroll $d units
  } else {
    uplevel #0 $cmd [expr {[lindex $info 2] + $d}]
  }
}

## scroll runform canvas
proc netwag_priv_mw_scroll_toolform {D} {
  global netwag_priv_toolform_cv
  global netwag_sys_platform
  global netwag_sys_platform_windows

  set d [netwag_priv_mw_step $netwag_priv_toolform_cv $D]
  $netwag_priv_toolform_cv yview scroll $d units
}

## handle a scroll event
proc netwag_priv_mw_event {X Y D} {
  global netwag_priv_toolform_cv

  set w [winfo containing $X $Y]
  if {![winfo exists $w]} {
    catch {set w [focus]}
  }
  if {![winfo exists $w]} {
    return
  }
  set cla [winfo class $w]
  if {$cla == "Scrollbar"} {
    # scrollbar is special
    catch {netwag_priv_mw_scroll_scrollbar $w $D}
  } elseif {$cla == "Text" || $cla == "Listbox" || $cla == "Canvas"} {
    catch {netwag_priv_mw_scroll_object $w $D}
  } else {
    global netwag_sys_platform
    global netwag_sys_platform_windows
    if { $netwag_sys_platform == $netwag_sys_platform_windows } {
      # we are perhaps inside the runform canvas
      set cvlen [string length $netwag_priv_toolform_cv]
      set w [string range $w 0 [expr $cvlen - 1]]
      if {$w == $netwag_priv_toolform_cv} {
        netwag_priv_mw_scroll_toolform $D
      }
    }
  }
}


#################################################################
# PUBLIC FUNCTIONS
#################################################################

# Set generic MouseWheel events
proc netwag_mw {what} {
  global netwag_sys_platform
  global netwag_sys_platform_unix

  # 120 is the min step in Win95/98 (explained in Tk doc)
  catch {bind $what <MouseWheel> {netwag_priv_mw_event %X %Y [expr %D / 120]}}
  if { $netwag_sys_platform == $netwag_sys_platform_unix } {
    bind $what <Button-4> {netwag_priv_mw_event %X %Y +1}
    bind $what <Button-5> {netwag_priv_mw_event %X %Y -1}
  }
}

# Set Form canvas MouseWheel events
proc netwag_mw_form {what} {
  global netwag_sys_platform
  global netwag_sys_platform_unix

  if { $netwag_sys_platform == $netwag_sys_platform_unix } {
    bind $what <Button-4> {netwag_priv_mw_scroll_toolform +1}
    bind $what <Button-5> {netwag_priv_mw_scroll_toolform -1}
  } else {
    catch {bind $what <MouseWheel> {netwag_priv_mw_scroll_toolform [expr %D / 120]}}
  }
}

# Set default
netwag_mw Text
netwag_mw Listbox
netwag_mw Scrollbar
netwag_mw Canvas

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
global netwag_priv_popen_fdcmd


#################################################################
# PRIVATE FUNCTIONS
#################################################################


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Initialize a popen
proc netwag_popen_init { cmd mode pfd perrmsg } {
  global netwag_priv_popen_fdcmd

  upvar $pfd fd
  upvar $perrmsg errmsg

  if { $mode == "rw" || $mode == "r+" } {
    set acc "r+"
  } elseif { $mode == "r" } {
    set acc "r"
  } elseif { $mode == "w" } {
    set acc "w"
  } else {
    set errmsg "Unknown mode: $mode"
    return -1
  }

  if { [catch { set fd [open "| $cmd" $acc] }] } {
    set errmsg "Cannot run $cmd"
    return -1
  }

  if { $fd == -1 } {
    set errmsg "Cannot run $cmd"
    return -1
  }

  set netwag_priv_popen_fdcmd($fd) $cmd
  return 0
}

## Configure popen to be unblocking
proc netwag_popen_conf_block { fd block } {

  if { $block } {
    fconfigure $fd -blocking 1 -buffering line
  } else  {
    fconfigure $fd -blocking 0 -buffering none
  }

  return 0
}

## Get the next line
proc netwag_popen_read_line { fd pline } {
  global netwag_priv_popen_fdcmd

  upvar $pline line

  set ret [gets $fd line]
  if { $ret == -1 } {
    return -1
  }

  return 0
}

## Get the next data
proc netwag_popen_read_data { fd pdata } {
  global netwag_priv_popen_fdcmd

  upvar $pdata data

  if [eof $fd] {
    return -1
  }

  set data [read $fd 16383]

  if { [string length $data] == 0 && [eof $fd] } {
    return -1
  }

  return 0
}

## Write a line
proc netwag_popen_write_line { fd line } {
  puts $fd $line
}

## Write data
proc netwag_popen_write_data { fd data } {
  puts -nonewline $fd $data
}

## Close
proc netwag_popen_close { fd pretval perrmsg } {
  global errorCode
  global netwag_priv_popen_fdcmd

  upvar $pretval retval
  upvar $perrmsg errmsg

  if { [catch { close $fd }] } {
    set errlist [split $errorCode]
    set errtype [lindex $errlist 0]
    if { $errtype == "CHILDSTATUS" } {
      set retval [lindex $errlist 2]
      return 0
    } elseif { $errtype == "NONE" } {
      # occurs under Solaris 9, to obtain remote information
      set retval 0
      return 0
    } else {
      set cmd $netwag_priv_popen_fdcmd($fd)
      set errmsg "Command $cmd exited with error $errlist"
      return -1
    }
  }
  set retval 0
  return 0
}

## Interrupt
proc netwag_popen_interrupt { fd perrmsg } {
  global errorCode
  global netwag_priv_popen_fdcmd

  upvar $perrmsg errmsg

  if { [catch { close $fd }] } {
    set errlist [split $errorCode]
    set errtype [lindex $errlist 0]
    if { $errtype == "CHILDKILLED" } {
      return 0
    } elseif { $errtype == "CHILDSTATUS" } {
      return 0
    } else {
      set cmd $netwag_priv_popen_fdcmd($fd)
      set errmsg "Command $cmd exited with error $errlist"
      return -1
    }
  }

  return 0
}
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


#################################################################
# PRIVATE FUNCTIONS
#################################################################


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Complete exec of a command
# Example :
#   set ret [netwag_exec "/bin/ls" outmsg retval errmsg]
#   if { $ret } {
#     puts "$errmsg"
#     exit
#   } elseif { $retval } {
#     puts "process returned : $retval"
#     exit
#   }
#   puts $outmsg
proc netwag_exec { cmd poutmsg pretval perrmsg } {
  upvar $poutmsg outmsg
  upvar $pretval retval
  upvar $perrmsg errmsg

  set outmsg ""
  netwag_cursor_wait
  set ret [netwag_popen_init $cmd "r" fd errmsg]
  if { $ret } {
    netwag_cursor_default
    return $ret
  }

  while {1} {
    set data [read $fd 16383]
    if { [string length $data] == 0 && [eof $fd] } {
      break
    }
    append outmsg "$data"
  }

  set ret [netwag_popen_close $fd retval errmsg]
  netwag_cursor_default
  return $ret
}
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


#################################################################
# PRIVATE FUNCTIONS
#################################################################


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Create the temporary file containing the command
proc netwag_runcmd_file_init { cmd ptmpfilename } {
  upvar $ptmpfilename filename

  set fd [netwag_tmpfile_open filename]
  puts $fd "$cmd\n"
  close $fd
}

## Close the temporary file
proc netwag_runcmd_file_del { tmpfilename } {
  netwag_tmpfile_del $tmpfilename 0
}

## Check the command
proc netwag_runcmd_check { pcmd ptoolnum } {
  upvar $pcmd cmd
  upvar $ptoolnum toolnum

  if { $cmd == "" } {
    netwag_msg_add_warn "Command \"$cmd\" too short"
    return -1
  }
  set cmdlist [split $cmd]

  set toolnum [lindex $cmdlist 0]
  if { [regexp {[^0-9]} $toolnum] } {
    netwag_msg_add_warn "The first parameter (\"$toolnum\") must be a number"
    return -1
  }

  return 0
}
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


#################################################################
# PRIVATE FUNCTIONS
#################################################################


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Run a netwox tool in a new window
proc netwag_runnw_run { cmd } {
  global netwag_glo_bin_netwox
  global netwag_sys_platform
  global netwag_sys_platform_unix
  global netwag_sys_platform_windows
  global netwag_glo_bin_term
  global netwag_glo_bin_term_param
  global env
  global netwag_sys_os;
  global netwag_sys_os_windows95
  global netwag_sys_os_windows98
  global netwag_sys_os_windowsme

  netwag_runcmd_file_init $cmd tmpfilename

  if {$netwag_sys_platform == $netwag_sys_platform_unix} {
    # we use "sh -c" in case netwox is not in the path
    exec $netwag_glo_bin_term $netwag_glo_bin_term_param /bin/sh -c "$netwag_glo_bin_netwox 0 -R -b $tmpfilename" &
  } elseif {$netwag_sys_platform == $netwag_sys_platform_windows} {
    # we use "$env(COMSPEC) /c" in case netwox is not in the path
    if { $netwag_sys_os==$netwag_sys_os_windows95 || $netwag_sys_os==$netwag_sys_os_windows98 || $netwag_sys_os==$netwag_sys_os_windowsme } {
      exec start $env(COMSPEC) /c $netwag_glo_bin_netwox 0 -R -b $tmpfilename &
    } else {
      # because start is not an external command
      exec $env(COMSPEC) /c start $env(COMSPEC) /c $netwag_glo_bin_netwox 0 -R -b $tmpfilename &
    }
  } else {
    netwag_msg_add_err "I don't know how to run a command on this platform"
  }

  # eventually remove command filename
  after 5000 "netwag_runcmd_file_del $tmpfilename"

  return 0
}
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
global netwag_runtext_state
global netwag_runtext_state_norun ; set netwag_runtext_state_norun 0
global netwag_runtext_state_running ; set netwag_runtext_state_running 1
global netwag_runtext_state_paused ; set netwag_runtext_state_paused 2

global netwag_runtext_realcmd
global netwag_runtext_usercmd

global netwag_runtext_autoscroll
global netwag_runtext_backspace
global netwag_runtext_autocrush
global netwag_runtext_fd
global netwag_runtext_pauseddata

global netwag_runtext_cb_begin
global netwag_runtext_cb_end


#################################################################
# PRIVATE FUNCTIONS
#################################################################

## Append stdout data
proc netwag_priv_runtext_append_stdout { txt data } {
  global netwag_runtext_state
  global netwag_runtext_state_paused
  global netwag_runtext_state_running
  global netwag_runtext_pauseddata
  global netwag_runtext_backspace
  global netwag_runtext_autocrush

  if {$netwag_runtext_state($txt) == $netwag_runtext_state_running} {
    $txt configure -state normal
    if {$netwag_runtext_backspace($txt)} {
      # deal with backspaces
      while {1} {
        while {[string range $data 0 0] == ""} {
          $txt delete "end -2c"
          set data [string range $data 1 end]
        }
        if { ! [regsub -- "." "$data" "" data] } {
          break
        }
      }
    }
    $txt insert end "$data"
    if {$netwag_runtext_autocrush($txt)} {
      set linecol [$txt index end]
      set dotpos [expr [string first "." $linecol] - 1]
      set numlines [string range $linecol 0 $dotpos]
      global netwag_glo_gui_toolrun_crush
      if {$numlines > $netwag_glo_gui_toolrun_crush} {
        $txt delete 1.0 "[expr $netwag_glo_gui_toolrun_crush / 5].0"
        $txt insert 1.0 "Lines before were crushed (limit to $netwag_glo_gui_toolrun_crush lines).\n" netwag_runtext_color_info
        netwag_update_idletasks
      }
    }
    $txt configure -state disabled
    netwag_runtext_autoscroll_show $txt
  } else {
    append netwag_runtext_pauseddata($txt) "$data"
  }

}

## Append stdin data
proc netwag_priv_runtext_append_stdin { txt data } {

  $txt configure -state normal
  $txt insert end "$data" netwag_runtext_color_stdin
  $txt configure -state disabled

  netwag_runtext_autoscroll_show $txt
}

## Append end
proc netwag_priv_runtext_append_info { txt data } {

  $txt configure -state normal
  $txt insert end "$data" netwag_runtext_color_info
  $txt configure -state disabled

  netwag_runtext_autoscroll_show $txt
}

## Called when data is available
proc netwag_priv_runtext_event { txt } {
  global netwag_runtext_state
  global netwag_runtext_state_running
  global netwag_runtext_state_norun
  global netwag_runtext_fd

  set fd $netwag_runtext_fd($txt)
  set ret [netwag_popen_read_data $fd data]

  if { $ret == 0 } {
    # suppress \r
    set data [netwag_misc_string_replace $data "\r" ""]
    # ok
    netwag_priv_runtext_append_stdout $txt "$data"
    return 0
  }

  # if we are here, program has terminated
  set netwag_runtext_state($txt) $netwag_runtext_state_norun

  # reconfigure blocking, otherwise, variable errorCode is never set
  netwag_popen_conf_block $fd 1

  set ret [netwag_popen_close $fd retval errmsg]
  if { $ret } {
    netwag_msg_add_err "Error when closing popen"
    return $ret
  }

  if { $retval } {
    netwag_msg_add_err "Tool returned an error"
  } else {
    netwag_msg_add_info "Tool finished its job"
  }

  global netwag_runtext_cb_end
  if {$netwag_runtext_cb_end($txt) != ""} {
    global netwag_runtext_realcmd
    global netwag_runtext_usercmd
    $netwag_runtext_cb_end($txt) $txt $netwag_runtext_realcmd($txt) $netwag_runtext_usercmd($txt)
  }

  return 0
}


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Initialize
# cb_start $txt $realcmd $usercmd : callback called when tool starts
# cb_end $txt $realcmd $usercmd : callback called when tool finishes
proc netwag_runtext_init { txt {cb_start ""} {cb_end ""} } {
  global netwag_runtext_state
  global netwag_runtext_state_norun

  set netwag_runtext_state($txt) $netwag_runtext_state_norun

  global netwag_runtext_cb_start
  set netwag_runtext_cb_start($txt) $cb_start

  global netwag_runtext_cb_end
  set netwag_runtext_cb_end($txt) $cb_end

  global netwag_runtext_autoscroll
  if {![info exists netwag_runtext_autoscroll($txt)]} {
    set netwag_runtext_autoscroll($txt) 0
  }

  global netwag_runtext_backspace
  if {![info exists netwag_runtext_backspace($txt)]} {
    set netwag_runtext_backspace($txt) 0
  }

  global netwag_runtext_autocrush
  if {![info exists netwag_runtext_autocrush($txt)]} {
    set netwag_runtext_autocrush($txt) 0
  }

  $txt tag configure netwag_runtext_color_stdin -foreground "#0020FF"
  $txt tag configure netwag_runtext_color_info -foreground "#66CC66"
  $txt configure -state disabled

  return 0
}

## Free memory when a Running notebook is closed
proc netwag_runtext_close { txt } {
  $txt configure -state normal
  $txt delete 1.0 end
  $txt configure -state disabled
  return 0
}

## Run a command and put result in a text
proc netwag_runtext_run { txt realcmd usercmd } {
  global netwag_glo_bin_netwox
  global netwag_runtext_state
  global netwag_runtext_state_running
  global netwag_runtext_state_norun
  global netwag_runtext_fd
  global netwag_runtext_pauseddata

  if { $netwag_runtext_state($txt) != $netwag_runtext_state_norun } {
    netwag_msg_add_warn "There is a tool currently running"
    return 0
  }

  netwag_runcmd_file_init $realcmd tmpfilename

  set ret [netwag_popen_init "$netwag_glo_bin_netwox 0 -r -b $tmpfilename" "rw" fd errmsg]
  if { $ret } {
    netwag_msg_add_err "Cannot run \"$usercmd\""
    netwag_runcmd_file_del $tmpfilename
    return $ret
  } else {
    netwag_msg_add_info "Running \"$usercmd\""
  }

  set netwag_runtext_state($txt) $netwag_runtext_state_running
  set netwag_runtext_fd($txt) $fd
  set netwag_runtext_pauseddata($txt) ""

  global netwag_runtext_realcmd
  global netwag_runtext_usercmd
  set netwag_runtext_realcmd($txt) $realcmd
  set netwag_runtext_usercmd($txt) $usercmd

  netwag_popen_conf_block $fd 0
  fconfigure $fd -translation {binary binary}

  $txt configure -state normal
  $txt delete 1.0 end
  $txt configure -state disabled

  global netwag_runtext_cb_start
  if {$netwag_runtext_cb_start($txt) != ""} {
    $netwag_runtext_cb_start($txt) $txt $realcmd $usercmd
  }

  fileevent $fd readable "netwag_priv_runtext_event $txt"

  return 0
}

## Eventually autoscroll
proc netwag_runtext_autoscroll_show { txt } {
  global netwag_runtext_autoscroll
  if {$netwag_runtext_autoscroll($txt)} {
    $txt see "end"
    netwag_update_idletasks
  }
}

## Called on interrupt request
proc netwag_runtext_int { txt } {
  global netwag_glo_bin_netwox
  global netwag_runtext_state
  global netwag_runtext_state_paused
  global netwag_runtext_state_norun
  global netwag_runtext_fd

  if { $netwag_runtext_state($txt) == $netwag_runtext_state_norun } {
    # might have just finished, so do not display error
    return 0
  }

  # we kill the child in 500 ms if it doesn't close itself
  set thepid [pid $netwag_runtext_fd($txt)]
  exec $netwag_glo_bin_netwox 0 -k -u $thepid -b 500 &

  set ret [netwag_popen_interrupt $netwag_runtext_fd($txt) errmsg]
  if { $ret == 0 } {
    netwag_msg_add_info "Tool successfully interrupted"
  }
  set netwag_runtext_state($txt) $netwag_runtext_state_norun

  global netwag_runtext_cb_end
  if {$netwag_runtext_cb_end($txt) != ""} {
    global netwag_runtext_realcmd
    global netwag_runtext_usercmd
    $netwag_runtext_cb_end($txt) $txt $netwag_runtext_realcmd($txt) $netwag_runtext_usercmd($txt)
  }

  return $ret
}

## Called to interrupt all running programs
proc netwag_runtext_int_all { } {
  global netwag_runtext_state
  global netwag_runtext_state_norun

  foreach txt [array names netwag_runtext_state] {
    if { $netwag_runtext_state($txt) != $netwag_runtext_state_norun } {
      netwag_runtext_int $txt
    }
  }
}

## Called on send request
proc netwag_runtext_send { txt data echo } {
  global netwag_runtext_state
  global netwag_runtext_state_norun
  global netwag_runtext_fd

  if { $netwag_runtext_state($txt) == $netwag_runtext_state_norun } {
    netwag_msg_add_warn "There is no tool currently running"
    return 0
  }

  if {$echo} {
    netwag_priv_runtext_append_stdin $txt $data
  }
  netwag_popen_write_data $netwag_runtext_fd($txt) $data

  return 0
}

## Called on pause/continue request
proc netwag_runtext_pause { txt } {
  global netwag_runtext_state
  global netwag_runtext_state_norun
  global netwag_runtext_state_running
  global netwag_runtext_state_paused
  global netwag_runtext_pauseddata

  if { $netwag_runtext_state($txt) == $netwag_runtext_state_norun } {
    netwag_msg_add_warn "There is no tool currently running"
    return 0
  }

  if { $netwag_runtext_state($txt) == $netwag_runtext_state_running } {
    set netwag_runtext_state($txt) $netwag_runtext_state_paused
  } else {
    set netwag_runtext_state($txt) $netwag_runtext_state_running
    netwag_priv_runtext_append_stdout $txt $netwag_runtext_pauseddata($txt)
  }
  set netwag_runtext_pauseddata($txt) ""

  return 0
}

## Store a message in the text zone
proc netwag_runtext_append_msg { txt data } {
  $txt configure -state normal
  $txt insert end "$data"
  $txt configure -state disabled

  netwag_runtext_autoscroll_show $txt
}
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


#################################################################
# PRIVATE FUNCTIONS
#################################################################

## Display an error
proc netwag_priv_glo_errorunset { pname } {

  set msg "Variable $pname does not exist.\nSet its value, or if unsure reinstall netwag."
  set ans [tk_messageBox -message $msg -type okcancel -title "Warning" -icon warning]
  if { $ans == "cancel" } {
    exit
  }
}
proc netwag_priv_glo_errorbadval { msg } {

  set msg "$msg\nModify its value, or if unsure reinstall netwag."
  set ans [tk_messageBox -message $msg -type okcancel -title "Warning" -icon warning]
  if { $ans == "cancel" } {
    exit
  }
}

## Special check for the session file
proc netwag_priv_glo_check_file_session { pname } {
  upvar $pname name
  global $pname

  if { ! [info exists $pname] } {
    netwag_priv_glo_errorunset $pname
    set name ""
    return
  }

  set name [netwag_misc_string_replace $name "\\" "/"]

  # we do no more checks (exists, readable, writable) because this state
  # can change during execution. So, it's better to deal with
  # it in corresponding functions
}

## Special check for netwox path
proc netwag_priv_glo_check_bin_netwox { pname defaultvalue } {
  upvar $pname name
  global $pname

  if { ! [info exists $pname] }  {
    netwag_priv_glo_errorunset $pname
    set name $defaultvalue
    return
  }

}

## Check for a directory
proc netwag_priv_glo_check_directory { pname defaultvalue } {
  upvar $pname name
  global $pname

  if { ! [info exists $pname] }  {
    netwag_priv_glo_errorunset $pname
    set name $defaultvalue
    return
  }

  if { ! [file isdirectory $name] } {
    netwag_priv_glo_errorbadval "Variable $pname contains \"$name\" which is not a directory."
    set name $defaultvalue
  }
}

## Check if the file is writable
proc netwag_priv_glo_check_writable { pname defaultvalue } {
  upvar $pname name
  global $pname

  if { ! [info exists $pname] }  {
    netwag_priv_glo_errorunset $pname
    set name $defaultvalue
    return
  }

  if { ! [file readable $name] } {
    netwag_priv_glo_errorbadval "Variable $pname contains \"$name\" which is not a writable file."
    set name $defaultvalue
  }
}

## Concert backslashes
proc netwag_priv_glo_convslash { pname defaultvalue } {
  upvar $pname name
  global $pname

  if { ! [info exists $pname] }  {
    netwag_priv_glo_errorunset $pname
    set name $defaultvalue
    return
  }

  set name [netwag_misc_string_replace $name "\\" "/"]
}


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Check main global variables
proc netwag_glo_check { } {
  global env
  global netwag_glo_dir_tmp
  global netwag_glo_file_session
  global netwag_glo_bin_netwox

  # netwag_glo_dir_tmp must be a writable directory
  netwag_priv_glo_convslash netwag_glo_dir_tmp ""
  netwag_priv_glo_check_directory netwag_glo_dir_tmp ""
  if { $netwag_glo_dir_tmp != "" } {
    netwag_priv_glo_check_writable netwag_glo_dir_tmp ""
  }
  if { $netwag_glo_dir_tmp == "" } {
    if { $netwag_sys_platform == $netwag_sys_platform_windows } {
      if { [catch { set netwag_glo_dir_tmp $env(TEMP) }] } {
        if { [catch { set netwag_glo_dir_tmp $env(TMP) }] } {
          set netwag_glo_dir_tmp "."
        }
      }
    } elseif { $netwag_sys_platform == $netwag_sys_platform_unix } {
      set netwag_glo_dir_tmp "/tmp"
    } else {
      set netwag_glo_dir_tmp "."
    }
  }

  # netwag_glo_file_session should be a readable/writable file
  netwag_priv_glo_check_file_session netwag_glo_file_session

  # netwag_glo_bin_netwox
  netwag_priv_glo_convslash netwag_glo_bin_netwox "netwox"
  netwag_priv_glo_check_bin_netwox netwag_glo_bin_netwox "netwox"

}
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

# by saving a session, we save :
#  - font (family and size)
#  - state of dynamic help (small yellow help displayed when inactive)
#  - width and height of the window
#  - the two clipboards
#  - the history of run commands
#  - field history for Form notebook

#################################################################
# GLOBAL VARIABLES
#################################################################
global netwag_session_font_family
global netwag_session_font_size
global netwag_session_dynhelp_active
global netwag_session_window_width
global netwag_session_window_height
global netwag_session_toolsearch_type
global netwag_session_clipboard_small
global netwag_session_clipboard_big1
global netwag_session_clipboard_big2
global netwag_session_clipboard_big3
global netwag_session_clipboard_big4
global netwag_session_run_history
global netwag_session_clipboard_text_height
global netwag_session_couldload

global netwag_session_formhist
global netwag_session_formhist_list
set netwag_session_formhist_list {"ip" "ip4" "ip6" "eth" "port" "ips" "eths" "ports" "uint32" "int32" "uint64" "int64" "buf" "login" "password" "file" "dir" "filter" "oid" "mixed" "ip4opts" "ip6exts" "tcpopts" "email" "uri" "newsgroup" "hostname" "md5" "useragent" "netbiosname" "smbshare" }

global netwag_sessionvars_error


#################################################################
# PRIVATE FUNCTIONS
#################################################################

## Initialize default values for session variables
proc netwag_priv_sessionvars_initdefault { } {
  global netwag_session_font_family
  global netwag_session_font_size
  global netwag_session_dynhelp_active
  global netwag_session_window_width
  global netwag_session_window_height
  global netwag_session_toolsearch_type
  global netwag_session_clipboard_small
  global netwag_session_clipboard_big1
  global netwag_session_clipboard_big2
  global netwag_session_clipboard_big3
  global netwag_session_clipboard_big4
  global netwag_session_run_history
  global netwag_session_clipboard_text_height
  global netwag_session_formhist
  global netwag_sys_platform
  global netwag_sys_platform_windows

  set netwag_session_dynhelp_active 1
  if {$netwag_sys_platform == $netwag_sys_platform_windows} {
    set netwag_session_font_family Courier
    set netwag_session_font_size 8
    set netwag_session_window_width 490
    set netwag_session_window_height 405
  } else {
    set netwag_session_font_family fixed
    set netwag_session_font_size 10
    set netwag_session_window_width 490
    set netwag_session_window_height 405
  }
  global netwag_priv_toolsearch_type_sort
  set netwag_session_toolsearch_type $netwag_priv_toolsearch_type_sort
  set netwag_session_clipboard_small {you can put your data here}
  set netwag_session_clipboard_big1 {you can store what you want here}
  set netwag_session_clipboard_big2 {you can also store what you want here}
  set netwag_session_clipboard_big3 {you can also store what you need here}
  set netwag_session_clipboard_big4 {you can also store your data here}
  catch {unset netwag_session_run_history}
  set netwag_session_run_history(0) {1}
  set netwag_session_run_history(1) {2}
  set netwag_session_run_history(2) {22 "hello"}
  set netwag_session_run_history(3) {23}

  set netwag_session_clipboard_text_height 0

  set netwag_session_formhist(ip) {127.0.0.1 255.255.255.255 :: fec0:1:2:3::5 "www.example.com"}
  set netwag_session_formhist(ip4) {127.0.0.1 255.255.255.255}
  set netwag_session_formhist(ip6) {:: fec0:1:2:3::5}
  set netwag_session_formhist(eth) {0:0:0:0:0:0 ff:ff:ff:ff:ff:ff}
  set netwag_session_formhist(port) {80 1234}
  set netwag_session_formhist(ips) {all}
  set netwag_session_formhist(eths) {all}
  set netwag_session_formhist(ports) {all}
  set netwag_session_formhist(uint32) {0 1}
  set netwag_session_formhist(int32) {0 1}
  set netwag_session_formhist(uint64) {0 1}
  set netwag_session_formhist(int64) {0 1}
  set netwag_session_formhist(buf) {"hello" "some data"}
  set netwag_session_formhist(login) {"bob"}
  set netwag_session_formhist(password) {"mypasswd"}
  set netwag_session_formhist(file) {"myfile.txt"}
  set netwag_session_formhist(dir) {"mydir"}
  set netwag_session_formhist(filter) {"host 1.2.3.4" "host 1.2.3.4 and tcp port 80"}
  set netwag_session_formhist(oid) {".1.3.6.1.2.1.1.4.0"}
  set netwag_session_formhist(mixed) {"'some' 0D0A 'data' '0D0A'"}
  set netwag_session_formhist(ip4opts) {"01 01 01 00" "01010101 010101 00"}
  set netwag_session_formhist(ip6exts) {"2C 0000 4001 00000007"}
  set netwag_session_formhist(tcpopts) {"010101 00" "0204400c" "0402 01 00" "030302 00"}
  set netwag_session_formhist(email) {"user@example.com"}
  set netwag_session_formhist(uri) {"http://www.example.com/"}
  set netwag_session_formhist(newsgroup) {"comp.lang.c" "comp.os.linux.announce"}
  set netwag_session_formhist(hostname) {"www.example.com"}
  set netwag_session_formhist(md5) {"c2c8fd6dbba2fdbd0a12386cb85850e6"}
  set netwag_session_formhist(useragent) {"Mozilla/5.0"}
  set netwag_session_formhist(netbiosname) {"*SMBSERVER"}
  set netwag_session_formhist(smbshare) {"C\$" "D\$"}

  return 0
}

## Initialize current values for session variables
proc netwag_priv_sessionvars_initcurrent { } {
  global netwag_session_font_family
  global netwag_session_font_size
  global netwag_session_dynhelp_active
  global netwag_session_window_width
  global netwag_session_window_height
  global netwag_session_toolsearch_type
  global netwag_session_clipboard_small
  global netwag_session_clipboard_big1
  global netwag_session_clipboard_big2
  global netwag_session_clipboard_big3
  global netwag_session_clipboard_big4
  global netwag_session_run_history
  global netwag_session_clipboard_text_height

  # font_family and font_size
  # used directly

  # netwag_session_dynhelp_active
  # used directly

  # window_width and window_height
  set netwag_session_window_width [winfo width .]
  set netwag_session_window_height [winfo height .]

  # netwag_session_toolsearch_type
  # used directly

  # clipboard_small and big
  global netwag_gui_text_small_clipboard
  global netwag_gui_text_big_clipboard1
  global netwag_gui_text_big_clipboard2
  global netwag_gui_text_big_clipboard3
  global netwag_gui_text_big_clipboard4
  set v [$netwag_gui_text_small_clipboard get 1.0 end]
  set netwag_session_clipboard_small [string trim $v]
  set v [$netwag_gui_text_big_clipboard1 get 1.0 end]
  set netwag_session_clipboard_big1 [string trim $v]
  set v [$netwag_gui_text_big_clipboard2 get 1.0 end]
  set netwag_session_clipboard_big2 [string trim $v]
  set v [$netwag_gui_text_big_clipboard3 get 1.0 end]
  set netwag_session_clipboard_big3 [string trim $v]
  set v [$netwag_gui_text_big_clipboard4 get 1.0 end]
  set netwag_session_clipboard_big4 [string trim $v]

  # run history
  global netwag_priv_toolhist_lb
  set arraysize [$netwag_priv_toolhist_lb size]
  catch {unset netwag_session_run_history}
  for {set i 0} {$i < $arraysize} {incr i} {
    set netwag_session_run_history($i) [$netwag_priv_toolhist_lb get $i]
  }

  # netwag_glo_clipboard_text_height
  global netwag_glo_clipboard_text_height
  set netwag_session_clipboard_text_height $netwag_glo_clipboard_text_height

  # netwag_session_formhist
  # used directly

  return 0
}

## Applies values for the session
proc netwag_priv_sessionvars_apply { } {
  global netwag_session_font_family
  global netwag_session_font_size
  global netwag_session_dynhelp_active
  global netwag_session_window_width
  global netwag_session_window_height
  global netwag_session_toolsearch_type
  global netwag_session_clipboard_small
  global netwag_session_clipboard_big1
  global netwag_session_clipboard_big2
  global netwag_session_clipboard_big3
  global netwag_session_clipboard_big4
  global netwag_session_run_history
  global netwag_session_clipboard_text_height

  # font_family and font_size
  netwag_font_change $netwag_session_font_family $netwag_session_font_size

  # dynhelp_active : nothing to do

  # window_width and window_height
  set newgeometry "$netwag_session_window_width"
  append newgeometry "x"
  append newgeometry "$netwag_session_window_height"
  set rx [winfo rootx .]
  set ry [winfo rooty .]
  if { $rx!=0 && $ry!=0 } {
    # we add them otherwise, if [wm geometry .] ends with +0+0, it
    # will be located in the corner.
    append newgeometry "+$rx+$ry"
  }
  wm geometry . "$newgeometry"

  # toolsearch_type : nothing to do

  # small clipboard
  global netwag_gui_text_small_clipboard
  $netwag_gui_text_small_clipboard delete 1.0 end
  $netwag_gui_text_small_clipboard insert end "$netwag_session_clipboard_small\n"

  # big clipboards
  global netwag_gui_text_big_clipboard1
  $netwag_gui_text_big_clipboard1 delete 1.0 end
  $netwag_gui_text_big_clipboard1 insert end "$netwag_session_clipboard_big1\n"
  global netwag_gui_text_big_clipboard2
  $netwag_gui_text_big_clipboard2 delete 1.0 end
  $netwag_gui_text_big_clipboard2 insert end "$netwag_session_clipboard_big2\n"
  global netwag_gui_text_big_clipboard3
  $netwag_gui_text_big_clipboard3 delete 1.0 end
  $netwag_gui_text_big_clipboard3 insert end "$netwag_session_clipboard_big3\n"
  global netwag_gui_text_big_clipboard4
  $netwag_gui_text_big_clipboard4 delete 1.0 end
  $netwag_gui_text_big_clipboard4 insert end "$netwag_session_clipboard_big4\n"

  # run history
  global netwag_priv_toolhist_lb
  $netwag_priv_toolhist_lb delete 0 end
  set arraysize [array size netwag_session_run_history]
  for {set i 0} {$i < $arraysize} {incr i} {
    $netwag_priv_toolhist_lb insert end $netwag_session_run_history($i)
  }

  # netwag_session_clipboard_text_height
  global frame_clipboard
  global netwag_glo_clipboard_text_height
  set netwag_glo_clipboard_text_height $netwag_session_clipboard_text_height
  $frame_clipboard.text configure -height $netwag_glo_clipboard_text_height

  # netwag_session_formhist : nothing to do

  return 0
}

## Write session variables to a filename
proc netwag_priv_sessionvars_write { filename } {
  global netwag_varfile_error
  global netwag_sessionvars_error
  global netwag_session_font_family
  global netwag_session_font_size
  global netwag_session_dynhelp_active
  global netwag_session_window_width
  global netwag_session_window_height
  global netwag_session_toolsearch_type
  global netwag_session_clipboard_small
  global netwag_session_clipboard_big1
  global netwag_session_clipboard_big2
  global netwag_session_clipboard_big3
  global netwag_session_clipboard_big4
  global netwag_session_run_history
  global netwag_session_clipboard_text_height
  global netwag_session_formhist
  global netwag_session_formhist_list

  set ret [netwag_varfile_write_init $filename fd]
  if { $ret } {
    set netwag_sessionvars_error "$netwag_varfile_error"
    return -1
  }

  # simple variables
  set ret [netwag_varfile_write_variable $fd "font_family" $netwag_session_font_family]
  if { $ret } {
    set netwag_sessionvars_error "$netwag_varfile_error"
    return -1
  }
  set ret [netwag_varfile_write_variable $fd "font_size" $netwag_session_font_size]
  if { $ret } {
    set netwag_sessionvars_error "$netwag_varfile_error"
    return -1
  }
  set ret [netwag_varfile_write_variable $fd "dynhelp_active" $netwag_session_dynhelp_active]
  if { $ret } {
    set netwag_sessionvars_error "$netwag_varfile_error"
    return -1
  }
  set ret [netwag_varfile_write_variable $fd "window_width" $netwag_session_window_width]
  if { $ret } {
    set netwag_sessionvars_error "$netwag_varfile_error"
    return -1
  }
  set ret [netwag_varfile_write_variable $fd "window_height" $netwag_session_window_height]
  if { $ret } {
    set netwag_sessionvars_error "$netwag_varfile_error"
    return -1
  }
  set ret [netwag_varfile_write_variable $fd "toolsearch_type" $netwag_session_toolsearch_type]
  if { $ret } {
    set netwag_sessionvars_error "$netwag_varfile_error"
    return -1
  }
  set ret [netwag_varfile_write_variable $fd "clipboard_small" $netwag_session_clipboard_small]
  if { $ret } {
    set netwag_sessionvars_error "$netwag_varfile_error"
    return -1
  }
  set ret [netwag_varfile_write_variable $fd "clipboard_big" $netwag_session_clipboard_big1]
  if { $ret } {
    set netwag_sessionvars_error "$netwag_varfile_error"
    return -1
  }
  set ret [netwag_varfile_write_variable $fd "clipboard_big2" $netwag_session_clipboard_big2]
  if { $ret } {
    set netwag_sessionvars_error "$netwag_varfile_error"
    return -1
  }
  set ret [netwag_varfile_write_variable $fd "clipboard_big3" $netwag_session_clipboard_big3]
  if { $ret } {
    set netwag_sessionvars_error "$netwag_varfile_error"
    return -1
  }
  set ret [netwag_varfile_write_variable $fd "clipboard_big4" $netwag_session_clipboard_big4]
  if { $ret } {
    set netwag_sessionvars_error "$netwag_varfile_error"
    return -1
  }
  set ret [netwag_varfile_write_variable $fd "clipboard_text_height" $netwag_session_clipboard_text_height]
  if { $ret } {
    set netwag_sessionvars_error "$netwag_varfile_error"
    return -1
  }

  # run history
  set ret [netwag_varfile_write_array $fd "run_history" netwag_session_run_history]
  if { $ret } {
    set netwag_sessionvars_error "$netwag_varfile_error"
    return -1
  }

  # form history
  foreach v $netwag_session_formhist_list {
    set ret [netwag_varfile_write_list $fd "formhist_$v" netwag_session_formhist($v)]
    if { $ret } {
      set netwag_sessionvars_error "$netwag_varfile_error"
      return -1
    }
  }

  # end
  set ret [netwag_varfile_write_close $fd]
  if { $ret } {
    set netwag_sessionvars_error "$netwag_varfile_error"
    return -1
  }

  return 0
}

## Read session variables from a filename
proc netwag_priv_sessionvars_read { filename } {
  global netwag_varfile_error
  global netwag_sessionvars_error
  global netwag_session_font_family
  global netwag_session_font_size
  global netwag_session_dynhelp_active
  global netwag_session_window_width
  global netwag_session_window_height
  global netwag_session_toolsearch_type
  global netwag_session_clipboard_small
  global netwag_session_clipboard_big1
  global netwag_session_clipboard_big2
  global netwag_session_clipboard_big3
  global netwag_session_clipboard_big4
  global netwag_session_run_history
  global netwag_session_clipboard_text_height
  global netwag_session_formhist

  set ret [netwag_varfile_read $filename bigvar]
  if { $ret } {
    set netwag_sessionvars_error "$netwag_varfile_error"
    return -1
  }

  # reassign variables
  catch {set netwag_session_font_family $bigvar(font_family)}
  catch {set netwag_session_font_size $bigvar(font_size)}
  catch {set netwag_session_dynhelp_active $bigvar(dynhelp_active)}
  catch {set netwag_session_window_width $bigvar(window_width)}
  catch {set netwag_session_window_height $bigvar(window_height)}
  catch {set netwag_session_toolsearch_type $bigvar(toolsearch_type)}
  catch {set netwag_session_clipboard_small $bigvar(clipboard_small)}
  # before version 5.15.0, there were only 1 big clipboard, named
  # "clipboard_big". So we do not create "clipboard_big1".
  catch {set netwag_session_clipboard_big1 $bigvar(clipboard_big)}
  catch {set netwag_session_clipboard_big2 $bigvar(clipboard_big2)}
  catch {set netwag_session_clipboard_big3 $bigvar(clipboard_big3)}
  catch {set netwag_session_clipboard_big4 $bigvar(clipboard_big4)}
  catch {set netwag_session_clipboard_text_height $bigvar(clipboard_text_height)}
  catch {unset netwag_session_run_history}
  set i 0
  while {[info exists bigvar(run_history,$i)]} {
    set netwag_session_run_history($i) $bigvar(run_history,$i)
    incr i
  }

  global netwag_session_formhist_list
  foreach v $netwag_session_formhist_list {
    eval "catch {set netwag_session_formhist($v) \$bigvar(formhist_$v)}"
  }

  return 0
}


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Load the last session
proc netwag_session_load_vars { } {
  global netwag_glo_file_session
  global netwag_session_couldload

  netwag_priv_sessionvars_initdefault
  set ret [netwag_priv_sessionvars_read $netwag_glo_file_session]
  if { $ret } {
    set netwag_session_couldload 0
  } else {
    set netwag_session_couldload 1
  }
  return 0
}
proc netwag_session_load_apply { } {
  global netwag_glo_file_session
  global netwag_session_couldload
  global netwag_sessionvars_error

  if {!$netwag_session_couldload} {
    netwag_priv_sessionvars_initdefault
    netwag_priv_sessionvars_apply
    netwag_msg_add_err "$netwag_sessionvars_error"
    netwag_msg_add_warn "This error is normal if this is the first run of netwag"
    return -1
  }

  netwag_priv_sessionvars_apply
  return 0
}

## Create a new session
proc netwag_session_new { } {
  netwag_priv_sessionvars_initdefault
  netwag_priv_sessionvars_apply
  netwag_msg_add_info "Default session loaded"
}

## Open a session
proc netwag_session_open { } {
  global netwag_sessionvars_error
  global netwag_glo_file_session
  # note : apparently, ".ses" does not match hidden files ".netwag.ses"
  set filetypes {
    {{Session Files} {.ses} }
    {{All Files} * }
  }
  set dn [file dirname $netwag_glo_file_session]
  set filename [tk_getOpenFile -filetypes $filetypes -initialdir $dn]
  if { $filename == "" } {
    return 0
  }
  netwag_priv_sessionvars_initdefault
  set ret [netwag_priv_sessionvars_read $filename]
  if { $ret } {
    netwag_msg_add_err "$netwag_sessionvars_error"
    netwag_msg_add_warn "Could not load session from $filename"
    return -1
  }
  netwag_priv_sessionvars_apply
  netwag_msg_add_info "Session in $filename loaded"
  return 0
}

## Save the session
proc netwag_session_save { } {
  global netwag_sessionvars_error
  global netwag_glo_file_session
  netwag_priv_sessionvars_initcurrent
  set ret [netwag_priv_sessionvars_write $netwag_glo_file_session]
  if { $ret } {
    netwag_msg_add_err "$netwag_sessionvars_error"
    netwag_msg_add_warn "Could not save session to $netwag_glo_file_session"
    return -1
  }
  netwag_msg_add_info "Session saved in $netwag_glo_file_session"
  return 0
}

## Save a session
proc netwag_session_saveas { } {
  global netwag_sessionvars_error
  global netwag_glo_file_session
  set filetypes {
    {{Session Files} {.ses} }
    {{All Files} * }
  }
  set dn [file dirname $netwag_glo_file_session]
  set filename [tk_getSaveFile -filetypes $filetypes -initialdir $dn -defaultextension ".ses"]
  if { $filename == "" } {
    return
  }
  netwag_priv_sessionvars_initcurrent
  set ret [netwag_priv_sessionvars_write $filename]
  if { $ret } {
    netwag_msg_add_err "$netwag_sessionvars_error"
    netwag_msg_add_warn "Could not save session to $filename"
    return -1
  }
  netwag_msg_add_info "Session saved in $filename"
  return 0
}



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
global netwag_bitmap_down_init ; set netwag_bitmap_down_init 0
global netwag_bitmap_up_init ; set netwag_bitmap_up_init 0
global netwag_bitmap_top_init ; set netwag_bitmap_top_init 0
global netwag_bitmap_plus_init ; set netwag_bitmap_plus_init 0
global netwag_bitmap_close_init ; set netwag_bitmap_close_init 0


#################################################################
# PRIVATE FUNCTIONS
#################################################################


#################################################################
# PUBLIC FUNCTIONS
#################################################################

proc netwag_bitmap_down { bm } {
  global netwag_glo_gui_bgcolor_button
  global netwag_bitmap_down_init

  if {! $netwag_bitmap_down_init} {
    # to create this, use Unix "bitmap", save file, and paste contents
    set bm_down {
      #define i_width 15
      #define i_height 15
      static unsigned char i_bits[] = {
        0xc0, 0x01, 0xe0, 0x03, 0xf0, 0x07, 0xf0, 0x07, 0xe0, 0x03, 0xc0, 0x01,
        0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0xf8, 0x0f, 0xf0, 0x07,
        0xe0, 0x03, 0xc0, 0x01, 0x80, 0x00, 0x00, 0x00};
    }
    image create bitmap netwag_bitmap_down_bm -data $bm_down
    set netwag_bitmap_down_init 1
  }

  label $bm -image netwag_bitmap_down_bm -bd 0 -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  netwag_dynhelp_init $bm "This icon indicates that, when button is pressed, data goes from top window to bottom window."
}

proc netwag_bitmap_up { bm } {
  global netwag_glo_gui_bgcolor_button
  global netwag_bitmap_up_init

  if {! $netwag_bitmap_up_init} {
    set bm_up {
      #define i_width 15
      #define i_height 15
      static unsigned char i_bits[] = {
        0x80, 0x00, 0xc0, 0x01, 0xe0, 0x03, 0xf0, 0x07, 0xf8, 0x0f, 0x80, 0x00,
        0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0xc0, 0x01, 0xe0, 0x03, 0xf0, 0x07,
        0xf0, 0x07, 0xe0, 0x03, 0xc0, 0x01, 0x00, 0x00};
    }
    image create bitmap netwag_bitmap_up_bm -data $bm_up
    set netwag_bitmap_up_init 1
  }

  label $bm -image netwag_bitmap_up_bm -bd 0 -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  netwag_dynhelp_init $bm "This icon indicates that, when button is pressed, data goes from bottom window to top window."
}

proc netwag_bitmap_top { bm } {
  global netwag_glo_gui_bgcolor_button
  global netwag_bitmap_top_init

  if {! $netwag_bitmap_top_init} {
    set bm_top {
      #define i_width 15
      #define i_height 15
      static unsigned char i_bits[] = {
        0xc0, 0x01, 0xe0, 0x03, 0xf0, 0x07, 0xf0, 0x07, 0xe0, 0x03, 0xc0, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    }
    image create bitmap netwag_bitmap_top_bm -data $bm_top
    set netwag_bitmap_top_init 1
  }

  label $bm -image netwag_bitmap_top_bm -bd 0 -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  netwag_dynhelp_init $bm "This icon indicates button work on above window."
}

proc netwag_bitmap_plus { bm } {
  global netwag_glo_gui_bgcolor_button
  global netwag_bitmap_plus_init

  if {! $netwag_bitmap_plus_init} {
    set bm_plus {
      #define tt_width 15
      #define tt_height 5
      static unsigned char i_bits[] = {
        0x04, 0x02, 0x0e, 0x07, 0x9f, 0x0f, 0x04, 0x02, 0x04, 0x02};
    }
    image create bitmap netwag_bitmap_plus_bm -data $bm_plus
    set netwag_bitmap_plus_init 1
  }

  label $bm -image netwag_bitmap_plus_bm -bd 0 -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
}

proc netwag_bitmap_close { bm } {
  global netwag_glo_gui_bgcolor_button
  global netwag_bitmap_close_init

  if {! $netwag_bitmap_close_init} {
    set bm_close {
      #define tt_width 16
      #define tt_height 16
      static unsigned char t_bits[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x1c, 0x0f, 0x1e, 0x1f, 0x1f,
        0xbe, 0x0f, 0xfc, 0x07, 0xf8, 0x03, 0xf0, 0x01, 0xf8, 0x03, 0xfc, 0x07,
        0xbe, 0x0f, 0x1f, 0x1f, 0x0f, 0x1e, 0x07, 0x1c};
    }
    image create bitmap netwag_bitmap_close_bm -data $bm_close
    set netwag_bitmap_close_init 1
  }

  label $bm -image netwag_bitmap_close_bm -bd 0 -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
}

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
global netwag_help_about_winname ; set netwag_help_about_winname ".helpabout_window"
global netwag_help_doc_winname ; set netwag_help_doc_winname ".helpdoc_window"


#################################################################
# PRIVATE FUNCTIONS
#################################################################


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## About
proc netwag_help_about_popup { } {
  global netwag_help_about_winname
  global netwag_glo_gui_bgcolor_unused
  global netwag_glo_gui_bgcolor_button
  global netwag_glo_versionmajor
  global netwag_glo_versionminor
  global netwag_glo_versionmicro

  # create window
  set hw $netwag_help_about_winname
  toplevel $hw
  wm title $hw "About netwag"

  # set content
  set msg "netwag $netwag_glo_versionmajor.$netwag_glo_versionminor.$netwag_glo_versionmicro\n"
  append msg "Copyright Laurent CONSTANTIN\n"
  label $hw.l1 -font myfont -bg $netwag_glo_gui_bgcolor_unused -text $msg
  pack $hw.l1 -fill both

  image create photo .logonetwimg -format GIF -data {
R0lGODlhPAA8AOcAAAcCAjNRHTSAX16pjXe1oooOJn7GrqYYM6ZPSlKCbpvexldZ8ykE1oZO
Ptzc2DECBLwlRrWEheTl4eGos1NqyIpASO6yuj8CvWuE4kMDDExju25MUoo5RY+CX3mtmeAq
TWtFTOLq8WyC1Eo+Guvt89LHr6Q0TY4SkoK7ppKk8JJcVi53TqhibVIGFo0xRLuol749Vefy
/ouNjtUnSGJKmumOoqW3+P7c5frR24ae4lVPJ9Zieai1vKknl0U56Y4Uhk4yFockNToCqun+
/ZBkV1IEmI6enkUCi3FXUsYzTcfS/JuZpqgtSNWNme7+/l9lYZil3D56UqGChpYOJqy46KA8
oIYMeq2K0lYCDp2TcaFxc5ZyllVt08wuinpsRHt12K+liGJQMILGssYlROfGznI6soFwSo4e
M3BlZjoChug6c9PBt/X+/sVUZT4y/oGCgVVeVqbezPPd3v76/XQcLapDUzRoK6aKlNYzUkOU
dXZ2usUuTr7GyoeX5vTk3VI+HtDc/shjdKqOeOI2YYWX1ZZaspoyjtHT53IKfp4egmF62VNa
VZIYMN/u8XpqSq42fH4+gswygKAqfJyho8C3nGZCqmENt4QVKpltZ/ipuEeJcMh0eu7y8bE1
TqZWjrQjP9anqt7Sw0hNSUJuOvr+/v7r+IYCdrhPYod6WKSWfs4nSI+XmWI6IpSGcW5ctqOr
qday6tZ3ib/14X4VKra7vNYuTOYyWY4iesoqepk+TGZw5CkV46YgOcUqSK2qpm9tZbKTgmaG
asuFj889WFlTSTeJapYydpI6bNXk/mZqamN2xZJSXNP778CPmI4YLU4ervjy5k1UT3waLRgD
BO/ByYsprsfGyFNiqYV2UGZva8o0cMS8rLBufLMtSF121rxkdJiXmLCSmv31+rzm2KdXZrws
R/7+/rjJ+mowMdDPyqSipujUz6Y8UXk9QZcaMOgrTv3AzXB2cq51f64eZt/79KTSwqGVm0JK
Ro6e0naR6pMkOLa+9tY2gp6s4mVRTliTeiwAAAAAPAA8AAAI/gDNCRxIsKDBgwgTmpszR6HD
hxAd+llHpmHEixgRQlv3IkKzihlDRoTmx0EoSmCIYFJBRJw5Tg4ciJwpkGSok2BSdcCGzQwR
FazEzXHAg8dACTQfknRQYhuYLB1QmZFqxgvLZehKiZu0SubLV0iTFrRZAiXUqDy9qPVChAgC
BOyC4JCwahUgUuYaGUmXkSFDgXOgSQhVNieqw9hQeam6toMgTAjqdDPBZIIcI0Z4hAjBZxWt
i3P8WMjUBJ6cdU1fpGq1k6eZ12u9oGoFhhKOJp16jSnXjdy6VZgn8TDyKsTFG5kCndKXSx6w
DtBR9WSstiprMNskQIN2Y1OvGWN6/nU75gsz8FWT+ICGZ6JbuU/6XKjgmXatGZ6tXviSENYc
NHgw9KLKbs2BA5xwtNCCV0TiZMLEGGN8wssl7RDBGGzYZAEGGKH0J5A4N+xQDoR71CGPPQaq
Yw1/GYlTQzcQRugMHYtVhU0HWfiSjkwLDjQHDrGMOEY3dXBzxxL2SIHJDRYdRAJCLsIIYTkz
zocjJSV4WNAcN8SS2xgQsMNNOEje4QkHm0zgUkGcpDPJK7448KSP7I14wBi8zELEC9s4AE2P
BnEXi3u7iWnPoVKcwkQ3THwDz5oDOQDOItEk88Yq6gwkTikmjDEDeN3MwoIcEM2RiQkQhNcJ
C+GAY88d/uR8CUFvTQqUTjyUihJNNKLMuSk7uo2hCpUcQPNQYINCUE4drNoziRQsdCIkmPrc
QBAptLxB6a68zjlHKad85ykEvNBBTa0GcTkBO7x0w843rS5xBwtM6AYemKdYSxAt8fAqiq69
AlZKG+V8Ct6Eyxh7EHcTkBPEGfqwQw6Zr7LATqqfqkLuKZCaQ8or2eh6jyj3RKPpwN/d28uM
ZKR7Ayjk5HLGGc64MPES8nLDTjn2DsiLNMKgu0o2IwNsMmA3wJCEwTPU0s0ly1gU2A0Nk0Mz
I0G4sIw9ON/BjQnTasyLMxycSxAbMhCtK7ehpOPHDTjAUMsMH2Rcjjt0wANN/mgTyGP1Ge5g
rfUdk0xyhzw7CxgeuQXQ0U7HHg+9NqU6pALGCxbUMMwgTINXziXk4DBBE+Too487qJ/hQgNH
TmIPN5IJO+AnBzhzCR3NQG7OKvHouggc0SzSiiCpbNLG3AbXPcYevJxBehAPM4J1fMsciaQ8
dZST6oAQHHDJLLNwAFJBMvRLKfCLPBfBJksnT3fGB7jjQi65uAC9CxUsI4U6rmtxTDcDgtAn
GFEA8M3icQYhxRuyETxuIQEYL2jC8Zj2gQrSrRZjOAAvEMACcnRQC1JoxRImoY4tVEAf0/rE
FAp4OzpwQ3ekyAYDd6WrZISiCYFon8HeYcF3vGMG/p9whwlYEAFBgEEdS+DfsyoQBHekqhxj
K6AB22E2NsmwgYsQhSOgYYFwJUEVO/RhBXkIRHdwQAVkGuErSEiEIDjjAJ/o3hQMCD7HCeUg
6ojHE3YFPFRAQxid6EQFKsCO9pHRh2QMIge4IQgpyAAcJFxGE3lBLmdIEXzSwB26BMIGB/AO
DqCMBhigsYlBhkEHrOBAwcTYtFrY4h21gGIulqGS/YXjGLzQHi8uaUBpqEBfB/mYHuHwhGyE
QhxSOOUo7BCAEVSgE+/DwyWCwIth2AKDvIhPBTARjmaY4BNBtCQmZ5HJClQxmOkIRjxA+Qb/
hAIOo1jBCpj5hwrM7QBn/njAA7BQgGF8AIO1s5k8lrEod/DyEpnkACj+khBOyGAVoPTFQILx
jzwIYJ4BYEUSalGAB0wDANN4gAvKUYsP9GIKzmiDZHjBiO/REaEVAIXuDhIMI8AhHpzwGD08
MIBizJOZQIABHoLwAAAYdRoZmMUg/uk9fXRibHQ0IB04kLuITGKBMiAFG+ihgAEEwwthOKUO
0NGJWnTCo0YFaQaGwcMxFMAd+iBgL8kpDQ7IY6YI4cMrXuEAUgwhDgPIQwIc0YDCNiAX1hxG
AbDg0WkgNQlkRKkzoorQWbgQmBEhgVaHMA4DFOMfPNjAIOuAgGG4EoN7MKg0WpCBA/hwEO5w
/uk4EUoHFZwTI06QhQHykAdZmCMM7WgHIZWGB1t8YBCDUEMSusGLWvjQFgZ9KTnpQI7bXsSv
CshDMerBBnO8AQloAAEI2sEEGEQCF5HgR3LVoAZbvPIDl5TtVM+1SYiQghk9/Qcz2DCHSSSj
A08gBgjQYYxHmMIKJ+hBJNjbXluoQoouTehCZ6LVcVjUA2wgBTRoIYM3oCEaGyBwNYpwgRJb
oho9wIUt1OBWA14Cps1gaEi0GodiCGAcbIBJh9HgD/GiwwWGuAAD3LALBlygCKbogi3cKtu6
yqO+F2GDE1Bg4yEMBRxvOJxwXcAESVThCD5YgA+KzIAiSGIY3nux/jScfEeakIIeBKiyBLD8
BjnAg5BM4E0VyuAKXSyAzGkwhDYOUEBpWFYeTBKLX+OsCSdY4w0ysMa3viGtPTyiEF/4wgLc
wIBOH+EWnZiCmumwjPElhQ1DYHQMsvUZc4hDGO55hCeu8AVXVOIZQhCCJRDRg24U4HvUJQNe
M4JqRjuBB2/wyhzIEAh2zPoKroCEMSTxgx8kYh7lmMev5yvjU6daAJqIgS+S7SMLNOEKKfgC
DYzRjW7sYR64UEUvdjnVhUI5JGzAbzE00Yhx84Ugc9gHPr6wBWMkAQLxVoW881SBqorlbMz4
xwo8gIxDgCNTZwMEIfQAixrsIDd7+FQv/hhh1zY/nJOyyMMKxtEIB2xjFdY4GzIIQQE2iMPc
gehEEnqRBBMguiCAEslWFVCMBDDDHIdwAM76Q4pxpEAESmAIiDLhcRNwg0lziAEJkKGEc+wD
EHcROj0GsIJ/dJcEDrDGxQdCinOcIx99aIjUb2CBCZyLBEqwQQ76kA8ucEEEKbDBOcIekQrn
QRPcJcUcSOAmGYSFDcg4RwowsA9SLGgO4hAH3lOgCG94gwsUAH3oKaAIfNgg6AnZKgoEkIAh
EMQatADHEgSiVRvYIB/5UAIySKB1rj/dGxSggAaGrwhFBP/4yugH6g+SW7KjoLtHscYq2ikQ
JwBi73APvA1StTD5z2vAGxjYez/cvg8q5AP5/YB+QrBbdGYE3QHlyykn+4GB+mOg+J0XvjL6
gI9DaHYhHkMC50AFnyd8ypd6QyALAyAA/+AEQScH5TMnHhMD52ADfeB3XDB8+9d/y0d75+B9
13CAB5FvnqUJgKB+R7EKv+AVtMcGjTCAhIAPMtgPh6B4DkEK+6AIoKcBIng2jZBd2+WABhEK
bxAPhxBMbBACgBACGdaBBsEGNqCDGnANlVcQAQEAOw==
}
  label $hw.l2 -image .logonetwimg -bg $netwag_glo_gui_bgcolor_unused
  pack $hw.l2 -fill x

  set msg "\nhttp://ntwag.sourceforge.net/\n"
  append msg "laurentconstantin@free.fr\n"
  label $hw.l3 -font myfont -bg $netwag_glo_gui_bgcolor_unused -text $msg -justify left
  pack $hw.l3 -fill both -expand 1

  # set button
  frame $hw.f -bg $netwag_glo_gui_bgcolor_unused
  pack $hw.f -fill x
  eval "button $hw.f.b -text OK -command { destroy $hw } -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0 -font myfont"
  pack $hw.f.b

}

## Documentation
proc netwag_help_doc_popup { } {
  global netwag_help_doc_winname
  global netwag_glo_gui_bgcolor_unused
  global netwag_glo_gui_bgcolor_button
  global netwag_glo_dynhelp_timeout
  global netwag_glo_versionmajor
  global netwag_glo_versionminor
  global netwag_glo_versionmicro

  # create window
  set hw $netwag_help_doc_winname
  toplevel $hw
  set msg "netwag 's documentation"
  wm title $hw $msg

  # set content
  frame $hw.f1 -bg $netwag_glo_gui_bgcolor_unused
  pack $hw.f1 -fill both
  set msg "There are two ways to obtain help:\n"
  append msg "\n"
  append msg "Using dynamic popups :\n"
  append msg " - activate \"Dynamic Help\" (in menu Help)\n"
  append msg " - put the mouse on top of an item\n"
  append msg " - do not move during $netwag_glo_dynhelp_timeout milliseconds\n"
  append msg " - then, a small yellow help window displays\n"
  append msg "\n"
  set v "$netwag_glo_versionmajor.$netwag_glo_versionminor.0"
  append msg "Reading lessons in netwag-$v-doc_html.tgz\n"
  label $hw.f1.l1 -font myfont -bg $netwag_glo_gui_bgcolor_unused -text $msg -justify left
  pack $hw.f1.l1 -anchor w

  # set button
  frame $hw.f6 -bg $netwag_glo_gui_bgcolor_unused
  pack $hw.f6 -fill x
  eval "button $hw.f6.b -text OK -command { destroy $hw } -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0 -font myfont"
  pack $hw.f6.b

}


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
global netwag_gui_text_big_clipboard
global netwag_gui_text_small_clipboard

#################################################################
# PRIVATE FUNCTIONS
#################################################################


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Create the skeleton
proc netwag_gui_skeleton { pframe_notebook pframe_clipboard pframe_message } {
  upvar $pframe_notebook frame_notebook
  upvar $pframe_clipboard frame_clipboard
  upvar $pframe_message frame_message
  global netwag_session_window_width
  global netwag_session_window_height

  wm title . "netwag"
  set newgeometry "$netwag_session_window_width"
  append newgeometry "x"
  append newgeometry "$netwag_session_window_height"
  wm geometry . "$newgeometry"
  wm protocol . WM_DELETE_WINDOW { netwag_menu_fileexit }

  set frame_notebook ".fn"
  set frame_clipboard ".fc"
  set frame_message ".fm"

  netwag_notebook_init $frame_notebook {"Tool" "Local info" "Remote info" "Clipboard"} { {} {netwag_infol_eventuallyinit} }
  set cf [netwag_notebook_frame_get $frame_notebook 0]
  set cb [netwag_notebook_button_get $frame_notebook 0]
  netwag_gui_notebook_tool $cf $cb
  set cf [netwag_notebook_frame_get $frame_notebook 1]
  set cb [netwag_notebook_button_get $frame_notebook 1]
  netwag_gui_notebook_infol $cf $cb
  set cf [netwag_notebook_frame_get $frame_notebook 2]
  set cb [netwag_notebook_button_get $frame_notebook 2]
  netwag_gui_notebook_infor $cf $cb
  set cf [netwag_notebook_frame_get $frame_notebook 3]
  set cb [netwag_notebook_button_get $frame_notebook 3]
  netwag_gui_notebook_clip $cf $cb

  frame $frame_clipboard

  frame $frame_message

  pack $frame_notebook -expand yes -fill both
  pack $frame_clipboard -fill x
  pack $frame_message -fill x

}

## Create the search notebook
proc netwag_gui_notebook_tool_search { cf cb } {
  global netwag_glo_gui_bgcolor_unused
  global netwag_glo_gui_bgcolor_listbox
  global netwag_glo_gui_bgcolor_button
  global netwag_glo_gui_bgcolor_text_select
  global netwag_glo_gui_bgcolor_text_edit

  # create the radios, search, infos and buttons frames
  set cfr "$cf.radios"
  frame $cfr -bg $netwag_glo_gui_bgcolor_unused
  set cfs "$cf.search"
  frame $cfs -bg $netwag_glo_gui_bgcolor_unused
  pack $cfr -fill x
  pack $cfs -expand 1 -fill both

  # create the radios frame
  global netwag_priv_toolsearch_type_sort
  global netwag_priv_toolsearch_type_tree
  frame $cfr.a
  radiobutton $cfr.a.sort -text "sort" -variable netwag_session_toolsearch_type -command netwag_priv_toolsearch_radiobutton_callback -value $netwag_priv_toolsearch_type_sort -font myfont -bg $netwag_glo_gui_bgcolor_unused -highlightthickness 0
  radiobutton $cfr.a.tree -text "tree" -variable netwag_session_toolsearch_type -command netwag_priv_toolsearch_radiobutton_callback -value $netwag_priv_toolsearch_type_tree -font myfont -bg $netwag_glo_gui_bgcolor_unused -highlightthickness 0
  pack $cfr.a.sort $cfr.a.tree -side left
  pack $cfr.a -side left
  netwag_dynhelp_init $cfr.a.sort "Show tools sorted by number."
  netwag_dynhelp_init $cfr.a.tree "Show tools in a tree."

  frame $cfr.c -bg $netwag_glo_gui_bgcolor_unused
  radiobutton $cfr.c.all -text "show all" -variable netwag_priv_toolsearch_showsearch -command netwag_priv_toolsearch_radiobutton_callback -value "1" -font myfont -bg $netwag_glo_gui_bgcolor_unused -highlightthickness 0
  radiobutton $cfr.c.search -text "search" -variable netwag_priv_toolsearch_showsearch -command netwag_priv_toolsearch_radiobutton_callback -value "2" -font myfont -bg $netwag_glo_gui_bgcolor_unused -highlightthickness 0
  set cfrbe $cfr.c.entry
  global netwag_priv_toolsearch_searchentry ; set netwag_priv_toolsearch_searchentry $cfrbe
  entry $cfrbe -textvariable netwag_priv_toolsearch_searchvar -width 15 -font myfont -bg $netwag_glo_gui_bgcolor_text_edit -highlightthickness 0
  pack $cfr.c.all $cfr.c.search -side left
  pack $cfrbe -side left -padx 2
  pack $cfr.c -side right
  $cfr.c.all select
  netwag_dynhelp_init $cfr.c.all "Show all tools in the listbox below."
  netwag_dynhelp_init $cfr.c.search "Search among the tools in the listbox below."
  netwag_dynhelp_init $cfr.c.entry "This is the searched text."

  frame $cfr.d
  button $cfr.d.close -text "Close branches" -command { netwag_priv_toolsearch_branches 0 } -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  button $cfr.d.expand -text "Expand branches" -command { netwag_priv_toolsearch_branches 1 } -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  pack $cfr.d.close $cfr.d.expand -side left
  netwag_dynhelp_init $cfr.d.close "Close all branches in the tree below."
  netwag_dynhelp_init $cfr.d.expand "Expand all branches in the tree below."

  # create the search frame containing the listbox
  set cfsb "$cfs.left.box"
  global netwag_priv_toolsearch_lb ; set netwag_priv_toolsearch_lb $cfsb
  frame $cfs.left
  scrollbar $cfs.yscro -command "$cfsb yview" -troughcolor $netwag_glo_gui_bgcolor_listbox -highlightthickness 0
  pack $cfs.left -side left -expand 1 -fill both
  pack $cfs.yscro -side left -fill y
  listbox $cfsb -yscrollcommand "$cfs.yscro set" -xscrollcommand "$cfs.left.xscro set" -font myfont -bg $netwag_glo_gui_bgcolor_listbox -highlightthickness 0 -height 1
  scrollbar $cfs.left.xscro -command "$cfsb xview" -orient horizontal -troughcolor $netwag_glo_gui_bgcolor_listbox -highlightthickness 0
  pack $cfsb -side top -expand 1 -fill both
  pack $cfs.left.xscro -side top -fill x
  netwag_dynhelp_init $cfsb "This listbox contains the title of tools. If \"Show all\" is checked this list contains every tool. If \"Search\" is checked, this list contains the result of the search."

  netwag_dynhelp_init $cb "Search a tool."
}

## Create the help notebook
proc netwag_gui_notebook_tool_help { cf cb } {
  global netwag_glo_gui_bgcolor_button
  global netwag_glo_gui_bgcolor_unused
  global netwag_glo_gui_bgcolor_text_select

  frame $cf.top
  frame $cf.bot -bg $netwag_glo_gui_bgcolor_unused
  pack $cf.top -side top -expand 1 -fill both
  pack $cf.bot -side top -fill x
  set cft "$cf.top.text"
  global netwag_priv_toolhelp_text ; set netwag_priv_toolhelp_text $cft
  text $cft -height 1 -width 1 -font myfont -bg $netwag_glo_gui_bgcolor_text_select -highlightthickness 0 -yscrollcommand "$cf.top.scro set"
  scrollbar $cf.top.scro -command "$cft yview" -troughcolor $netwag_glo_gui_bgcolor_text_select -highlightthickness 0
  pack $cft -side left -expand 1 -fill both
  pack $cf.top.scro -side left -fill y
  eval "bind $cft <Button-1> { focus $cft }"
  frame $cf.bot.l -bg $netwag_glo_gui_bgcolor_unused
  frame $cf.bot.r -bg $netwag_glo_gui_bgcolor_unused
  pack $cf.bot.l $cf.bot.r -side left -expand 1 -fill x
  netwag_bitmap_down $cf.bot.l.copybm
  button $cf.bot.l.copyex -text "Example" -command netwag_toolhelp_button_copyex -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  button $cf.bot.l.copyus -text "Usage" -command netwag_toolhelp_button_copyus -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  button $cf.bot.l.runit -text "Run it" -command {netwag_toolhelp_button_copyex ; netwag_priv_toolcmd_run} -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  netwag_bitmap_down $cf.bot.l.copybm2
  pack $cf.bot.l.copybm $cf.bot.l.copyex $cf.bot.l.copyus $cf.bot.l.runit $cf.bot.l.copybm2 -side left
  netwag_bitmap_up $cf.bot.r.chbm
  button $cf.bot.r.ch -text "Update" -command netwag_toolhf_button_update -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  netwag_bitmap_up $cf.bot.r.chbm2
  pack $cf.bot.r.chbm $cf.bot.r.ch $cf.bot.r.chbm2 -side right
  netwag_dynhelp_init $cft "This zone contains help of tool selected in Search notebook."
  netwag_dynhelp_init $cf.bot.l.copyex "Copy above example below."
  netwag_dynhelp_init $cf.bot.l.copyus "Copy above usage below."
  netwag_dynhelp_init $cf.bot.l.runit "Copy above example below, and run it."
  netwag_dynhelp_init $cf.bot.r.ch "Obtain help about current number in the run command below."
  netwag_dynhelp_init $cb "Help of selected tool."
}

## Create the form notebook
proc netwag_gui_notebook_tool_form { cf cb } {
  global netwag_glo_gui_bgcolor_button
  global netwag_glo_gui_bgcolor_unused
  global netwag_glo_gui_bgcolor_text_edit

  frame $cf.top
  canvas $cf.line -width 1 -height 1 -bg grey50 -highlightthickness 0
  frame $cf.bot -bg $netwag_glo_gui_bgcolor_unused
  pack $cf.top -side top -expand 1 -fill both
  pack $cf.line -side top -fill x
  pack $cf.bot -side top -fill x
  set cfc "$cf.top.c"
  global netwag_priv_toolform_cv ; set netwag_priv_toolform_cv $cfc
  canvas $cfc -yscrollcommand "$cf.top.scro set" -width 1 -height 1 -bg $netwag_glo_gui_bgcolor_unused -highlightthickness 0
  global netwag_priv_toolform_cvf ; set netwag_priv_toolform_cvf $cfc.f
  frame $netwag_priv_toolform_cvf -bg $netwag_glo_gui_bgcolor_unused
  $netwag_priv_toolform_cv create window 0 0 -window $netwag_priv_toolform_cvf -anchor nw -tags item
  grid columnconfigure $netwag_priv_toolform_cvf 3 -weight 1
  netwag_mw_form $netwag_priv_toolform_cvf
  set cfcs "$cf.top.scro"
  global netwag_priv_toolform_cvs ; set netwag_priv_toolform_cvs $cfcs
  scrollbar $cfcs -command "$cfc yview" -troughcolor $netwag_glo_gui_bgcolor_text_edit -highlightthickness 0
  pack $cfc -side left -expand 1 -fill both
  pack $cfcs -side left -fill y
  frame $cf.bot.l -bg $netwag_glo_gui_bgcolor_unused
  frame $cf.bot.c -bg $netwag_glo_gui_bgcolor_unused
  frame $cf.bot.c.c -bg $netwag_glo_gui_bgcolor_unused
  frame $cf.bot.r -bg $netwag_glo_gui_bgcolor_unused
  pack $cf.bot.l $cf.bot.c $cf.bot.r -side left -expand 1 -fill x
  netwag_bitmap_down $cf.bot.l.genbm
  button $cf.bot.l.gen -text "Generate" -command netwag_toolform_button_gen -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  button $cf.bot.l.runit -text "Run it" -command {netwag_toolform_button_gen ; netwag_priv_toolcmd_run} -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  netwag_bitmap_down $cf.bot.l.genbm2
  pack $cf.bot.l.genbm $cf.bot.l.gen $cf.bot.l.runit $cf.bot.l.genbm2 -side left
  netwag_bitmap_top $cf.bot.c.c.rstbm
  button $cf.bot.c.c.rst -text "Reset" -command netwag_toolform_button_rst -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  netwag_bitmap_top $cf.bot.c.c.rstbm2
  pack $cf.bot.c.c -side top
  pack $cf.bot.c.c.rstbm $cf.bot.c.c.rst $cf.bot.c.c.rstbm2 -side left
  netwag_bitmap_up $cf.bot.r.chbm
  button $cf.bot.r.ch -text "Update" -command netwag_toolhf_button_update -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  netwag_bitmap_up $cf.bot.r.chbm2
  pack $cf.bot.r.chbm $cf.bot.r.ch $cf.bot.r.chbm2 -side right
  netwag_dynhelp_init $cf.bot.l.gen "Generate command line. The checked button on the left indicates if the corresponding option has to be generated."
  netwag_dynhelp_init $cf.bot.l.runit "Generate command line, and run it."
  netwag_dynhelp_init $cf.bot.c.c.rst "Reset form."
  netwag_dynhelp_init $cf.bot.r.ch "Obtain form about current number in the run command below."
  netwag_dynhelp_init $cb "Easy selection of tool parameters."
}

## Create the run output notebook
proc netwag_gui_notebook_tool_run { cf cb } {
  global netwag_glo_gui_bgcolor_unused

  global netwag_priv_toolrun_nb
  set netwag_priv_toolrun_nb $cf

  global netwag_priv_toolrun_nb_top
  set netwag_priv_toolrun_nb_top "$cf.top"
  global netwag_priv_toolrun_nb_topb
  set netwag_priv_toolrun_nb_topb "$cf.top.b.f"
  frame $cf.top -bg $netwag_glo_gui_bgcolor_unused
  frame $cf.top.b -bg $netwag_glo_gui_bgcolor_unused
  canvas $cf.top.c -width 1 -height 1 -bg white -highlightthickness 0
  pack $cf.top.b -anchor w -expand 1 -fill x
  pack $cf.top.c -expand 1 -fill x
  netwag_bitmap_close $cf.top.b.cl1
  bind $cf.top.b.cl1 <Button-1> netwag_toolrun_current_close
  netwag_dynhelp_init $cf.top.b.cl1 "Close current tool."
  frame $cf.top.b.f -bg $netwag_glo_gui_bgcolor_unused
  netwag_bitmap_close $cf.top.b.cl2
  bind $cf.top.b.cl2 <Button-1> netwag_toolrun_current_close
  netwag_dynhelp_init $cf.top.b.cl2 "Close current tool."
  pack $cf.top.b.cl1 -side left
  pack $cf.top.b.f -side left -expand 1 -fill x
  pack $cf.top.b.cl2 -side right -anchor e

  global netwag_priv_toolrun_nb_bot
  set netwag_priv_toolrun_nb_bot "$cf.bot"
  frame $netwag_priv_toolrun_nb_bot -bg $netwag_glo_gui_bgcolor_unused
  set msg "When you'll run a tool, its output will be stored here."
  label $netwag_priv_toolrun_nb_bot.defaultmsg -text $msg -font myfont -bg $netwag_glo_gui_bgcolor_unused -highlightthickness 0
  pack $netwag_priv_toolrun_nb_bot.defaultmsg -expand 1 -anchor nw
  pack $netwag_priv_toolrun_nb_bot -expand 1 -fill both

  netwag_dynhelp_init $cb "Running (or terminated) tool, if the checkbox NW was not checked. Otherwise, the tool is run in a New Window."
}

## Create the history notebook
proc netwag_gui_notebook_tool_history { cf cb } {
  global netwag_glo_gui_bgcolor_listbox
  global netwag_glo_gui_bgcolor_button
  global netwag_glo_gui_bgcolor_unused

  frame $cf.top
  frame $cf.bot -bg $netwag_glo_gui_bgcolor_unused
  pack $cf.top -side top -expand 1 -fill both
  pack $cf.bot -side top -fill x
  set cfb "$cf.top.left.box"
  global netwag_priv_toolhist_lb ; set netwag_priv_toolhist_lb $cfb
  frame $cf.top.left
  scrollbar $cf.top.yscro -command "$cfb yview" -troughcolor $netwag_glo_gui_bgcolor_listbox -highlightthickness 0
  pack $cf.top.left -side left -expand 1 -fill both
  pack $cf.top.yscro -side left -fill y
  listbox $cfb -yscrollcommand "$cf.top.yscro set" -xscrollcommand "$cf.top.left.xscro set" -font myfont -bg $netwag_glo_gui_bgcolor_listbox -highlightthickness 0 -height 1
  scrollbar $cf.top.left.xscro -command "$cfb xview" -orient horizontal -troughcolor $netwag_glo_gui_bgcolor_listbox -highlightthickness 0
  pack $cfb -side top -expand 1 -fill both
  pack $cf.top.left.xscro -side top -fill x
  frame $cf.bot.l -bg $netwag_glo_gui_bgcolor_unused
  frame $cf.bot.c -bg $netwag_glo_gui_bgcolor_unused
  frame $cf.bot.c.c -bg $netwag_glo_gui_bgcolor_unused
  frame $cf.bot.r -bg $netwag_glo_gui_bgcolor_unused
  pack $cf.bot.l $cf.bot.c $cf.bot.r -side left -expand 1 -fill x
  netwag_bitmap_down $cf.bot.l.copybm
  button $cf.bot.l.copy -text "Copy line" -command netwag_toolhist_button_copy -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  button $cf.bot.l.runit -text "Run it" -command {netwag_toolhist_button_copy ; netwag_priv_toolcmd_run} -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  netwag_bitmap_down $cf.bot.l.copybm2
  pack $cf.bot.l.copybm $cf.bot.l.copy $cf.bot.l.runit $cf.bot.l.copybm2 -side left
  netwag_bitmap_top $cf.bot.c.c.delbm
  button $cf.bot.c.c.del -text "Delete" -command netwag_toolhist_button_del -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  netwag_bitmap_top $cf.bot.c.c.delbm2
  pack $cf.bot.c.c -side top
  pack $cf.bot.c.c.delbm $cf.bot.c.c.del $cf.bot.c.c.delbm2 -side left
  netwag_bitmap_up $cf.bot.r.addbm
  button $cf.bot.r.add -text "Add" -command netwag_toolhist_button_add -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  netwag_bitmap_up $cf.bot.r.addbm2
  pack $cf.bot.r.addbm $cf.bot.r.add $cf.bot.r.addbm2 -side right
  netwag_dynhelp_init $cfb "History of tools. When a tool is run, it is automatically added here."
  netwag_dynhelp_init $cf.bot.l.copy "Copy selected tool below."
  netwag_dynhelp_init $cf.bot.l.runit "Copy selected tool below, and run it."
  netwag_dynhelp_init $cf.bot.c.c.del "Delete the selected tool from history."
  netwag_dynhelp_init $cf.bot.r.add "Add the current command line in the history, without running it."
  netwag_dynhelp_init $cb "History of run tools."
}

## Create the tool notebook
proc netwag_gui_notebook_tool { cf cb } {
  global netwag_glo_gui_toolcmd_height
  global netwag_glo_gui_bgcolor_button
  global netwag_glo_gui_bgcolor_unused
  global netwag_glo_gui_bgcolor_text_edit

  # create top frame
  set cfn "$cf.nb"
  global netwag_priv_tool_nb ; set netwag_priv_tool_nb $cfn

  # create bottom frame
  set cfr "$cf.run"
  global netwag_priv_toolcmd_frame ; set netwag_priv_toolcmd_frame $cfr
  frame $cfr

  # create notebooks
  netwag_notebook_init $cfn {"Search" "Help" "Form" "Running" "History"} { {netwag_toolcmd_hide;netwag_toolsearch_eventuallyinit} {netwag_toolcmd_show;netwag_toolhf_display} {netwag_toolcmd_show;netwag_toolhf_display} {netwag_toolcmd_show} {netwag_toolcmd_show;} } {netwag_toolform_resize}
  set cf2 [netwag_notebook_frame_get $cfn 0]
  set cb2 [netwag_notebook_button_get $cfn 0]
  netwag_gui_notebook_tool_search $cf2 $cb2
  set cf2 [netwag_notebook_frame_get $cfn 1]
  set cb2 [netwag_notebook_button_get $cfn 1]
  netwag_gui_notebook_tool_help $cf2 $cb2
  set cf2 [netwag_notebook_frame_get $cfn 2]
  set cb2 [netwag_notebook_button_get $cfn 2]
  netwag_gui_notebook_tool_form $cf2 $cb2
  set cf2 [netwag_notebook_frame_get $cfn 3]
  set cb2 [netwag_notebook_button_get $cfn 3]
  netwag_gui_notebook_tool_run $cf2 $cb2
  set cf2 [netwag_notebook_frame_get $cfn 4]
  set cb2 [netwag_notebook_button_get $cfn 4]
  netwag_gui_notebook_tool_history $cf2 $cb2

  # pack frames ($cfr is hidden by default (== netwag_toolcmd_hide))
  pack $cfn -expand 1 -fill both

  # create the run frame containing the command parameters
  frame $cfr.left
  frame $cfr.right -bg $netwag_glo_gui_bgcolor_unused
  pack $cfr.left -side left -expand 1 -fill both
  pack $cfr.right -side left -fill y
  set cfrt "$cfr.left.text"
  global netwag_priv_toolcmd_text ; set netwag_priv_toolcmd_text $cfrt
  text $cfrt -yscrollcommand "$cfr.left.scro set" -width 2 -height $netwag_glo_gui_toolcmd_height -font myfont -bg $netwag_glo_gui_bgcolor_text_edit -highlightthickness 0
  scrollbar $cfr.left.scro -command "$cfrt yview" -troughcolor $netwag_glo_gui_bgcolor_text_edit -highlightthickness 0
  pack $cfrt -side left -expand 1 -fill both
  pack $cfr.left.scro -side left -fill y
  button $cfr.right.run -text "Run" -command netwag_priv_toolcmd_run -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  checkbutton $cfr.right.runnw -text "NW" -variable netwag_priv_toolcmd_nw -font myfont -bg $netwag_glo_gui_bgcolor_unused -highlightthickness 0
  pack $cfr.right.run $cfr.right.runnw -fill x
  netwag_dynhelp_init $cfrt "Tool to run. For example :\n1\n3 1.2.3.4\nThose are equivalent to \"netwox 1\" and \"netwox 3 1.2.3.4\""
  netwag_dynhelp_init $cfr.right.run "Run the current command and add it in the history."
  netwag_dynhelp_init $cfr.right.runnw "If checked, run the tool in a New Window.\nElse, display its output in the \"Running\" notebook."

  netwag_dynhelp_init $cb "Search and run a tool."
}

## Create the local information notebook
proc netwag_gui_notebook_infol { cf cb } {
  global netwag_glo_gui_bgcolor_text_select
  global netwag_glo_gui_bgcolor_text_edit
  global netwag_glo_gui_bgcolor_unused
  global netwag_glo_gui_bgcolor_button

  frame $cf.top -bg $netwag_glo_gui_bgcolor_unused
  frame $cf.text
  pack $cf.top -fill x
  pack $cf.text -expand 1 -fill both

  global netwag_glo_notebook_button_hidden_color
  button $cf.top.butupdate -text "Update" -command netwag_infol_button_update -font myfont -bg $netwag_glo_notebook_button_hidden_color -highlightthickness 0
  global netwag_priv_infol_buttons ; set netwag_priv_infol_buttons $cf.top
  button $cf.top.butdevices -text "Devices" -command {netwag_infol_button_press 1} -font myfont -bg $netwag_glo_notebook_button_hidden_color -highlightthickness 0
  button $cf.top.butip -text "Ip" -command {netwag_infol_button_press 2} -font myfont -bg $netwag_glo_notebook_button_hidden_color -highlightthickness 0
  button $cf.top.butarp -text "Arp cache" -command {netwag_infol_button_press 3} -font myfont -bg $netwag_glo_notebook_button_hidden_color -highlightthickness 0
  button $cf.top.butroutes -text "Routes" -command {netwag_infol_button_press 4} -font myfont -bg $netwag_glo_notebook_button_hidden_color -highlightthickness 0
  pack $cf.top.butupdate $cf.top.butdevices $cf.top.butip -side left -padx 1
  pack $cf.top.butarp $cf.top.butroutes -side left -padx 1
  netwag_dynhelp_init $cf.top.butupdate "Update configuration."
  netwag_dynhelp_init $cf.top.butdevices "Display devices."
  netwag_dynhelp_init $cf.top.butip "Display ip."
  netwag_dynhelp_init $cf.top.butarp "Display arp cache."
  netwag_dynhelp_init $cf.top.butroutes "Display routes."

  set cftt "$cf.text.left.text"
  global netwag_priv_infol_text_output ; set netwag_priv_infol_text_output $cftt
  frame $cf.text.left
  scrollbar $cf.text.yscro -command "$cftt yview" -troughcolor $netwag_glo_gui_bgcolor_text_select -highlightthickness 0
  pack $cf.text.left -side left -expand 1 -fill both
  pack $cf.text.yscro -side left -fill y
  text $cftt -yscrollcommand "$cf.text.yscro set" -xscrollcommand "$cf.text.left.xscro set" -width 2 -height 2 -font myfont -bg $netwag_glo_gui_bgcolor_text_select -wrap none -highlightthickness 0
  scrollbar $cf.text.left.xscro -command "$cftt xview" -orient horizontal -troughcolor $netwag_glo_gui_bgcolor_text_select -highlightthickness 0
  pack $cftt -side top -expand 1 -fill both
  pack $cf.text.left.xscro -side top -fill x
  eval "bind $cftt <Button-1> { focus $cftt }"
  netwag_dynhelp_init $cftt "Local information depending on pressed button."

  netwag_dynhelp_init $cb "Display network configuration of the local computer."
}

## Create the remote information notebook
proc netwag_gui_notebook_infor { cf cb } {
  global netwag_glo_gui_bgcolor_text_select
  global netwag_glo_gui_bgcolor_text_edit
  global netwag_glo_gui_bgcolor_unused
  global netwag_glo_gui_bgcolor_button

  frame $cf.top -bg $netwag_glo_gui_bgcolor_unused
  frame $cf.text
  pack $cf.top -fill x
  pack $cf.text -expand 1 -fill both

  entry $cf.top.addr -textvariable netwag_priv_infor_text_input -width 10 -font myfont -bg $netwag_glo_gui_bgcolor_text_edit -highlightthickness 0
  bind $cf.top.addr <Return> netwag_infor_button_query
  button $cf.top.butquery -text "Query" -command netwag_infor_button_query -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  global netwag_priv_infor_button_int
  set netwag_priv_infor_button_int $cf.top.butint
  button $netwag_priv_infor_button_int -text "Interrupt" -command netwag_infor_button_int -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  pack $cf.top.addr -side left -expand 1 -fill x -padx 2
  pack $cf.top.butquery -side left
  netwag_dynhelp_init $cf.top.addr "Contain the hostname, IP address or Ethernet address to query."
  netwag_dynhelp_init $cf.top.butquery "Query configuration and display the results."
  netwag_dynhelp_init $cf.top.butint "Interrupt current request."

  set cftt "$cf.text.left.text"
  global netwag_priv_infor_text_output ; set netwag_priv_infor_text_output $cftt
  frame $cf.text.left
  scrollbar $cf.text.yscro -command "$cftt yview" -troughcolor $netwag_glo_gui_bgcolor_text_select -highlightthickness 0
  pack $cf.text.left -side left -expand 1 -fill both
  pack $cf.text.yscro -side right -fill y
  text $cftt -yscrollcommand "$cf.text.yscro set" -xscrollcommand "$cf.text.left.xscro set" -width 2 -height 2 -font myfont -bg $netwag_glo_gui_bgcolor_text_select -wrap none -highlightthickness 0
  scrollbar $cf.text.left.xscro -command "$cftt xview" -orient horizontal -troughcolor $netwag_glo_gui_bgcolor_text_select -highlightthickness 0
  pack $cftt -side top -expand 1 -fill both
  pack $cf.text.left.xscro -side top -fill x
  eval "bind $cftt <Button-1> { focus $cftt }"
  netwag_dynhelp_init $cftt "Information obtained about the hostname, IP address or Ethernet address."

  netwag_dynhelp_init $cb "Display network configuration of a remote computer."
}

## Create the clipboard notebook
proc netwag_gui_notebook_clip_n { cfn num } {
  global netwag_glo_gui_bgcolor_text_edit

  set cf [netwag_notebook_frame_get $cfn $num]
  set cb [netwag_notebook_button_get $cfn $num]

  set n [expr $num + 1]
  eval "global netwag_gui_text_big_clipboard$n"
  eval "set netwag_gui_text_big_clipboard$n $cf.text.left.text"

  frame $cf.text
  pack $cf.text -expand 1 -fill both

  frame $cf.text.left
  scrollbar $cf.text.yscro -command "$cf.text.left.text yview" -troughcolor $netwag_glo_gui_bgcolor_text_edit -highlightthickness 0
  pack $cf.text.left -side left -expand 1 -fill both
  pack $cf.text.yscro -side left -fill y
  text $cf.text.left.text -yscrollcommand "$cf.text.yscro set" -xscrollcommand "$cf.text.left.xscro set" -width 2 -height 2 -font myfont -bg $netwag_glo_gui_bgcolor_text_edit -wrap none -highlightthickness 0
  scrollbar $cf.text.left.xscro -command "$cf.top.text xview" -orient horizontal -troughcolor $netwag_glo_gui_bgcolor_text_edit -highlightthickness 0
  pack $cf.text.left.text -expand 1 -fill both
  pack $cf.text.left.xscro -fill x

  netwag_dynhelp_init $cf.text.left.text "Clipboard number $n to store data you want."

  netwag_dynhelp_init $cb "Clipboard number $n to store data you want."
}
proc netwag_gui_notebook_clip { cf cb } {

  netwag_dynhelp_init $cb "Those big clipboards are useful to store the data you want."

  # create top frame
  set cfn "$cf.nb"

  # create notebooks
  netwag_notebook_init $cfn {"Clipboard 1" "Clip. 2" "Clip. 3" "Clip. 4"} { {} {} {} {} }
  netwag_gui_notebook_clip_n $cfn 0
  netwag_gui_notebook_clip_n $cfn 1
  netwag_gui_notebook_clip_n $cfn 2
  netwag_gui_notebook_clip_n $cfn 3
  # pack frame
  pack $cfn -expand 1 -fill both
}

## Create the clipboard
proc netwag_gui_clipboard { frame_clipboard } {
  global netwag_glo_gui_bgcolor_text_edit
  global netwag_gui_text_small_clipboard
  global netwag_glo_gui_bgcolor_button
  global netwag_glo_gui_bgcolor_unused
  global netwag_glo_clipboard_text_height

  $frame_clipboard configure -bg $netwag_glo_gui_bgcolor_unused
  set netwag_gui_text_small_clipboard $frame_clipboard.text
  text $frame_clipboard.text -height $netwag_glo_clipboard_text_height -yscrollcommand "$frame_clipboard.scro.s set" -font myfont -bg $netwag_glo_gui_bgcolor_text_edit -highlightthickness 0
  frame $frame_clipboard.scro
  scrollbar $frame_clipboard.scro.s -command "$frame_clipboard.text yview" -troughcolor $netwag_glo_gui_bgcolor_text_edit -highlightthickness 0
  # we do not use buttons because they are too big under Windows
  label $frame_clipboard.scro.less -relief raised -text "-" -pady 0 -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  bind $frame_clipboard.scro.less <Button-1> netwag_smallclip_height_callback_less
  label $frame_clipboard.scro.more -relief raised -text "+" -pady 0 -font myfont -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  bind $frame_clipboard.scro.more <Button-1> netwag_smallclip_height_callback_more
  netwag_bitmap_plus $frame_clipboard.scro.plus
  bind $frame_clipboard.scro.plus <Button-1> netwag_smallclip_height_callback_more
  canvas $frame_clipboard.scro.c -width 1 -height 1 -bg $netwag_glo_gui_bgcolor_unused -highlightthickness 0

  netwag_priv_smallclip_draw

  netwag_dynhelp_init $frame_clipboard.text "This clipboard is shared between the four top notebooks. You can store your data here."
  netwag_dynhelp_init $frame_clipboard.scro.less "Shrink the clipboard."
  netwag_dynhelp_init $frame_clipboard.scro.more "Enlarge the clipboard."
  netwag_dynhelp_init $frame_clipboard.scro.plus "Enlarge the clipboard."
}

## Create the message
proc netwag_gui_message { frame_message } {
  global netwag_glo_msg_text_height
  global netwag_glo_gui_bgcolor_text_select

  text $frame_message.text -height $netwag_glo_msg_text_height -yscrollcommand "$frame_message.scro set" -font myfont -bg $netwag_glo_gui_bgcolor_text_select -highlightthickness 0
  scrollbar $frame_message.scro -command "$frame_message.text yview" -troughcolor $netwag_glo_gui_bgcolor_text_select -highlightthickness 0
  pack $frame_message.scro -side right -fill y
  pack $frame_message.text -expand yes -fill both
  eval "bind $frame_message.text <Button-1> { focus $frame_message.text }"

  netwag_msg_init $frame_message.text

  netwag_dynhelp_init $frame_message.text "This zone contains info, warning and error messages."
}

## Create the menu
proc netwag_gui_menu { } {
  global netwag_session_helpdyn_active

  set topmenu ".m"
  set topmenufile "$topmenu.file"
  set topmenuedit "$topmenu.edit"
  set topmenusess "$topmenu.sess"
  set topmenuopt  "$topmenu.opt"
  set topmenuhelp "$topmenu.help"

  menu $topmenu -font myfont
  $topmenu add cascade -label "File" -menu $topmenufile -underline 0
  $topmenu add cascade -label "Edit" -menu $topmenuedit -underline 0
  $topmenu add cascade -label "Session" -menu $topmenusess -underline 0
  $topmenu add cascade -label "Options" -menu $topmenuopt -underline 0
  $topmenu add cascade -label "Help" -menu $topmenuhelp -underline 0
  . configure -menu $topmenu

  menu $topmenufile -tearoff 0 -font myfont
  $topmenufile add command -label "Exit" -command netwag_menu_fileexit -underline 1 -accelerator "Ctrl-Q"
  bind . <Control-q> netwag_menu_fileexit

  menu $topmenuedit -tearoff 0 -font myfont
  $topmenuedit add command -label "Cut" -command netwag_menu_editcut -underline 2 -accelerator "Ctrl-X"
  $topmenuedit add command -label "Copy" -command netwag_menu_editcopy -underline 0 -accelerator "Ctrl-C"
  $topmenuedit add command -label "Paste" -command netwag_menu_editpaste -underline 0 -accelerator "Ctrl-V"
  $topmenuedit add command -label "Select" -command netwag_menu_editselect -underline 0 -accelerator "Ctrl-A"
  $topmenuedit add command -label "Find" -command netwag_menu_editfind -underline 0 -accelerator "Ctrl-F"

  menu $topmenusess -tearoff 0 -font myfont
  $topmenusess add command -label "New" -command netwag_menu_sessnew -underline 0 -accelerator "Ctrl-N"
  bind . <Control-n> netwag_menu_sessnew
  $topmenusess add separator
  $topmenusess add command -label "Open" -command netwag_menu_sessopen -underline 0 -accelerator "Ctrl-O"
  bind . <Control-o> netwag_menu_sessopen
  $topmenusess add separator
  $topmenusess add command -label "Save" -command netwag_menu_sesssave -underline 0 -accelerator "Ctrl-S"
  bind . <Control-s> netwag_menu_sesssave
  $topmenusess add command -label "Save As..." -command netwag_menu_sesssaveas -underline 1

  menu $topmenuopt -tearoff 0 -font myfont
  $topmenuopt add command -label "Font..." -command netwag_menu_optfont -underline 0 -accelerator "Ctrl-T"
  bind . <Control-t> netwag_menu_optfont
  $topmenuopt add checkbutton -label "Dynamic help" -variable netwag_session_dynhelp_active -underline 0

  menu $topmenuhelp -tearoff 0 -font myfont
  $topmenuhelp add checkbutton -label "Dynamic help" -variable netwag_session_dynhelp_active -underline 0
  $topmenuhelp add command -label "Documentation" -command netwag_menu_helpdoc -underline 1
  $topmenuhelp add command -label "About netwag..." -command netwag_menu_helpabout -underline 0

}

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
global netwag_menu_fileexit_try ; set netwag_menu_fileexit_try 0

#################################################################
# PRIVATE FUNCTIONS
#################################################################


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Menu File - Exit
proc netwag_menu_fileexit { } {
  global netwag_menu_fileexit_try

  if {$netwag_menu_fileexit_try} {
    # direct exit, otherwise on error could not exit
    exit
  }
  set netwag_menu_fileexit_try 1

  netwag_runtext_int_all

  netwag_session_save
  exit
}

## Menu Edit - Cut
proc netwag_menu_editcut { } {
  event generate [focus] <Control-Key-x>
}

## Menu Edit - Copy
proc netwag_menu_editcopy { } {
  event generate [focus] <Control-Key-c>
}

## Menu Edit - Paste
proc netwag_menu_editpaste { } {
  event generate [focus] <Control-Key-v>
}

## Menu Edit - Select
proc netwag_menu_editselect { } {
  event generate [focus] <Control-Key-a>
}

## Menu Edit - Find
proc netwag_menu_editfind { } {
  netwag_txtfind_find
}

## Menu Session - New
proc netwag_menu_sessnew { } {
  netwag_session_new
}

## Menu Session - Open
proc netwag_menu_sessopen { } {
  netwag_session_open
}

## Menu Session - Save
proc netwag_menu_sesssave { } {
  netwag_session_save
}

## Menu Session - SaveAs
proc netwag_menu_sesssaveas { } {
  netwag_session_saveas
}

## Menu Option - Font
proc netwag_menu_optfont { } {
  global netwag_font_windows_name
  if { [winfo exists $netwag_font_windows_name] } {
    raise $netwag_font_windows_name
  } else {
    netwag_font_changepopup
  }
}

## Menu Help - Documentation
proc netwag_menu_helpdoc { } {
  global netwag_help_doc_winname
  if { [winfo exists $netwag_help_doc_winname] } {
    raise $netwag_help_doc_winname
  } else {
    netwag_help_doc_popup
  }
}

## Menu Help - About
proc netwag_menu_helpabout { } {
  global netwag_help_about_winname
  if { [winfo exists $netwag_help_about_winname] } {
    raise $netwag_help_about_winname
  } else {
    netwag_help_about_popup
  }
}

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
global netwag_priv_toolcmd_frame
global netwag_priv_toolcmd_text
global netwag_priv_toolcmd_nw

#################################################################
# PRIVATE FUNCTIONS
#################################################################

## Check if the command is empty
proc netwag_priv_toolcmd_empty { } {
  global netwag_priv_toolcmd_text

  set cmd [$netwag_priv_toolcmd_text get 1.0 end]
  set cmd [netwag_misc_string_map $cmd {"\r" " " "\n" " "}]
  set cmd [string trim $cmd]

  if { $cmd == "" } {
    return -1
  }

  return 0
}

## Called when Run button is pressed
proc netwag_priv_toolcmd_run { } {
  global netwag_priv_toolcmd_nw
  global netwag_priv_tool_nb

  if { [netwag_priv_toolcmd_empty] } {
    netwag_msg_add_warn "Command is empty. Will try to automatically press on first button."
    set butnum [netwag_notebook_butnum_get $netwag_priv_tool_nb]
    if {$butnum == 1} {
      netwag_toolhelp_button_copyex
    } elseif {$butnum == 2} {
      netwag_toolform_button_gen
    } elseif {$butnum == 4} {
      netwag_toolhist_button_copy
    } else {
      netwag_msg_add_err "Butnum $butnum is not implemented."
      return
    }
    if { ! [netwag_priv_toolcmd_empty] } {
      netwag_msg_add_info "If you agree with this command, press Run again."
    }
    return
  }

  # obtain user command (passwords are hidden)
  if { [netwag_toolcmd_check usercmd toolnum] } {
    return
  }

  # obtain real command containing passwords
  set realcmd [netwag_toolcmd_withpassword $usercmd]

  # run
  netwag_toolhist_add "$usercmd"
  if { $netwag_priv_toolcmd_nw } {
    netwag_msg_add_info "Running \"$usercmd\""
    netwag_msg_add_info "We cannot check its return value because tool is run in a New Window"
    netwag_runnw_run $realcmd
  } else {
    netwag_toolrun_run $toolnum $realcmd $usercmd
  }
}


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Initialize the command
proc netwag_toolcmd_init { cmd } {
  global netwag_priv_toolcmd_text
  $netwag_priv_toolcmd_text delete 1.0 end
  $netwag_priv_toolcmd_text insert end $cmd
}

## Hide/show run command
proc netwag_toolcmd_hide { } {
  global netwag_priv_toolcmd_frame

  pack forget $netwag_priv_toolcmd_frame
}
proc netwag_toolcmd_show { } {
  global netwag_priv_toolcmd_frame

  pack $netwag_priv_toolcmd_frame -side left -expand 1 -fill x
}

## Check command
proc netwag_toolcmd_check { pcmd ptoolnum } {
  global netwag_priv_toolcmd_text

  upvar $pcmd cmd
  upvar $ptoolnum toolnum

  set cmd [$netwag_priv_toolcmd_text get 1.0 end]
  set cmd [netwag_misc_string_map $cmd {"\r" " " "\n" " "}]
  set cmd [string trim $cmd]

  set ret [netwag_runcmd_check cmd toolnum]
}

## Obtain stored password
proc netwag_toolcmd_withpassword { usercmd } {
  global netwag_priv_toolform_hidden
  global netwag_priv_toolform_hidden_pattern

  set re $netwag_priv_toolform_hidden_pattern(0)
  append re "(\[0-9\]+)"
  append re $netwag_priv_toolform_hidden_pattern(1)
  set realcmd $usercmd
  while {1} {
    if { ! [regexp -- $re $realcmd foobar n] } {
      break
    }
    set r " "
    if { [info exists netwag_priv_toolform_hidden($n)] } {
      set val $netwag_priv_toolform_hidden($n)
      set val [netwag_misc_string_replace $val "\\" "\\\\"]
      append r $val
    } else {
      append r "password_"
      append r $n
      append r "_not_available"
    }
    regsub -- $re $realcmd $r realcmd
  }
  return $realcmd
}
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
global netwag_priv_toolsearch_type_sort ; set netwag_priv_toolsearch_type_sort 1
global netwag_priv_toolsearch_type_tree; set netwag_priv_toolsearch_type_tree 2

global netwag_priv_toolsearch_lb
# this variable indicates current type of listbox
global netwag_priv_toolsearch_lb_type

# this variable indicates current displayed listbox
# It is different from netwag_priv_toolsearch_lb_type, and needed to
# be able to select sort, select line of tool 7, press tree, and see
# the tree automatically open on tool 7. Both variables are not redundant.
global netwag_session_toolsearch_type

global netwag_priv_toolsearch_showsearch
global netwag_priv_toolsearch_searchentry
global netwag_priv_toolsearch_searchvar

global netwag_priv_toolsearch_tools_max

global netwag_priv_toolsearch_tools_sort_max
global netwag_priv_toolsearch_tools_sort_lbline

global netwag_priv_toolsearch_tools_tree_expand
global netwag_priv_toolsearch_tools_tree_nodes
global netwag_priv_toolsearch_tools_tree_text
global netwag_priv_toolsearch_tools_tree_toolstart
global netwag_priv_toolsearch_tools_tree_branch
global netwag_priv_toolsearch_tools_tree_max
global netwag_priv_toolsearch_tools_tree_lbline

global netwag_priv_toolsearch_tools_synonyms


#################################################################
# PRIVATE FUNCTIONS
#################################################################

## Obtain current selected line number
proc netwag_priv_toolsearch_selnum_init_cursel {} {
  global netwag_priv_toolsearch_lb

  set sellist [$netwag_priv_toolsearch_lb curselection]
  if { ! [llength $sellist] } {
    return -1
  }

  return [lindex $sellist 0]
}

## Select last line
proc netwag_priv_toolsearch_lb_select_end { } {
  global netwag_priv_toolsearch_lb
  $netwag_priv_toolsearch_lb activate end
  $netwag_priv_toolsearch_lb selection set end
}

## See current entry
proc netwag_priv_toolsearch_lb_see_active { } {
  global netwag_priv_toolsearch_lb
  $netwag_priv_toolsearch_lb see active
}

## Display all tools in the listbox
proc netwag_priv_toolsearch_lb_all_sort { } {
  global netwag_priv_toolsearch_lb
  global netwag_priv_toolsearch_lb_type
  global netwag_priv_toolsearch_type_sort
  global netwag_priv_toolsearch_tools_sort_max
  global netwag_priv_toolsearch_tools_sort_lbline

  set entrytoselect [netwag_toolsearch_toolnum_init_cursel]
  $netwag_priv_toolsearch_lb delete 0 end
  for {set i 1} {$i<=$netwag_priv_toolsearch_tools_sort_max} {incr i} {
    $netwag_priv_toolsearch_lb insert end $netwag_priv_toolsearch_tools_sort_lbline($i)
    if { $i == $entrytoselect } {
      netwag_priv_toolsearch_lb_select_end
    }
  }
  set netwag_priv_toolsearch_lb_type $netwag_priv_toolsearch_type_sort
}

## Display searched tools in the listbox (sorted view)
proc netwag_priv_toolsearch_lb_toolsearch_sort { } {
  global netwag_priv_toolsearch_searchvar
  global netwag_priv_toolsearch_lb
  global netwag_priv_toolsearch_lb_type
  global netwag_priv_toolsearch_type_sort
  global netwag_priv_toolsearch_tools_sort_max
  global netwag_priv_toolsearch_tools_sort_lbline
  global netwag_priv_toolsearch_tools_synonyms

  # we use "string tolower" instead of "string match -nocase"
  # because it does not exists in version 8.0
  set entrytoselect [netwag_toolsearch_toolnum_init_cursel]
  $netwag_priv_toolsearch_lb delete 0 end
  set searchedstr [string tolower $netwag_priv_toolsearch_searchvar]
  for {set i 1} {$i<=$netwag_priv_toolsearch_tools_sort_max} {incr i} {
    if { [string match "*$searchedstr*" [string tolower $netwag_priv_toolsearch_tools_sort_lbline($i)]] || [string match "*$searchedstr*" $netwag_priv_toolsearch_tools_synonyms($i)] } {
      $netwag_priv_toolsearch_lb insert end $netwag_priv_toolsearch_tools_sort_lbline($i)
      if { $i == $entrytoselect } {
        netwag_priv_toolsearch_lb_select_end
      }
    }
  }
  set netwag_priv_toolsearch_lb_type $netwag_priv_toolsearch_type_sort
}

## Display searched tools in the listbox (tree view)
proc netwag_priv_toolsearch_lb_tree_rec_insert { subnode selnode repplus } {
  global netwag_priv_toolsearch_lb
  global netwag_priv_toolsearch_tools_tree_max
  global netwag_priv_toolsearch_tools_tree_lbline
  global netwag_priv_toolsearch_tools_tree_text

  set netwag_priv_toolsearch_tools_tree_lbline($netwag_priv_toolsearch_tools_tree_max) $subnode
  incr netwag_priv_toolsearch_tools_tree_max
  set lbline $netwag_priv_toolsearch_tools_tree_text($subnode)
  if {$repplus} {
    set lbline [netwag_misc_string_replace $lbline "+" "-"]
  }
  $netwag_priv_toolsearch_lb insert end $lbline
  if {$subnode == $selnode} {
    netwag_priv_toolsearch_lb_select_end
  }
}
proc netwag_priv_toolsearch_lb_tree_rec { curnode selnode } {
  global netwag_priv_toolsearch_tools_tree_nodes
  global netwag_priv_toolsearch_tools_tree_text
  global netwag_priv_toolsearch_tools_tree_expand
  global netwag_priv_toolsearch_tools_tree_toolstart

  foreach subnode $netwag_priv_toolsearch_tools_tree_nodes($curnode) {
    if {$subnode < $netwag_priv_toolsearch_tools_tree_toolstart} {
      if {$netwag_priv_toolsearch_tools_tree_expand($subnode)} {
        netwag_priv_toolsearch_lb_tree_rec_insert $subnode $selnode 1
        netwag_priv_toolsearch_lb_tree_rec $subnode $selnode
      } else {
        netwag_priv_toolsearch_lb_tree_rec_insert $subnode $selnode 0
      }
    } else {
      netwag_priv_toolsearch_lb_tree_rec_insert $subnode $selnode 0
    }
  }
}
proc netwag_priv_toolsearch_lb_tree { } {
  global netwag_priv_toolsearch_lb
  global netwag_priv_toolsearch_lb_type
  global netwag_priv_toolsearch_type_tree
  global netwag_priv_toolsearch_tools_tree_branch
  global netwag_priv_toolsearch_tools_tree_max
  global netwag_priv_toolsearch_tools_tree_lbline
  global netwag_priv_toolsearch_tools_tree_toolstart
  global netwag_priv_toolsearch_tools_tree_expand

  # selected entry
  set selnode [netwag_toolsearch_node_init_cursel]
  if {$netwag_priv_toolsearch_lb_type != $netwag_priv_toolsearch_type_tree} {
    # ensure tool is visible
    if {$selnode > $netwag_priv_toolsearch_tools_tree_toolstart} {
      foreach expandnode $netwag_priv_toolsearch_tools_tree_branch($selnode) {
        set netwag_priv_toolsearch_tools_tree_expand($expandnode) 1
      }
    }
  }

  # clean
  set netwag_priv_toolsearch_tools_tree_max 0
  catch { unset netwag_priv_toolsearch_tools_tree_lbline }
  $netwag_priv_toolsearch_lb delete 0 end

  # main
  netwag_priv_toolsearch_lb_tree_rec 1 $selnode
  set netwag_priv_toolsearch_lb_type $netwag_priv_toolsearch_type_tree
}

## Called when the radio button is changed (or to update lb data)
proc netwag_priv_toolsearch_radiobutton_callback { } {
  global netwag_session_toolsearch_type
  global netwag_priv_toolsearch_type_sort
  global netwag_priv_toolsearch_type_tree
  global netwag_priv_toolsearch_showsearch
  global netwag_priv_toolsearch_searchvar

  set framesort ".fn.body.pf0.bot.right.nb.body.pf0.bot.right.radios"

  if {$netwag_session_toolsearch_type == $netwag_priv_toolsearch_type_sort} {
    pack forget $framesort.d
    pack $framesort.c -side right
    if { $netwag_priv_toolsearch_showsearch == 1 } {
      netwag_priv_toolsearch_lb_all_sort
    } elseif { $netwag_priv_toolsearch_showsearch == 2 } {
      netwag_priv_toolsearch_lb_toolsearch_sort
    }
  } elseif {$netwag_session_toolsearch_type == $netwag_priv_toolsearch_type_tree} {
    pack forget $framesort.c
    pack $framesort.d -side right
    netwag_priv_toolsearch_lb_tree
  }

  netwag_priv_toolsearch_lb_see_active
}

## Called to toggle tree
proc netwag_priv_toolsearch_tree_toggle {} {
  global netwag_session_toolsearch_type
  global netwag_priv_toolsearch_type_tree
  global netwag_priv_toolsearch_tools_tree_toolstart
  global netwag_priv_toolsearch_tools_tree_expand

  if {$netwag_session_toolsearch_type != $netwag_priv_toolsearch_type_tree} {
    return 0
  }

  set selnode [netwag_toolsearch_node_init_cursel]
  if {$selnode == -1 || $selnode >= $netwag_priv_toolsearch_tools_tree_toolstart} {
    return 0
  }

  if {$netwag_priv_toolsearch_tools_tree_expand($selnode)} {
    set netwag_priv_toolsearch_tools_tree_expand($selnode) 0
  } else {
    set netwag_priv_toolsearch_tools_tree_expand($selnode) 1
  }

  netwag_priv_toolsearch_radiobutton_callback
}

## Called to expand or close branches of tree
proc netwag_priv_toolsearch_branches_rec { curnode expand } {
  global netwag_priv_toolsearch_tools_tree_expand
  global netwag_priv_toolsearch_tools_tree_nodes
  global netwag_priv_toolsearch_tools_tree_toolstart

  set netwag_priv_toolsearch_tools_tree_expand($curnode) $expand
  foreach subnode $netwag_priv_toolsearch_tools_tree_nodes($curnode) {
    if {$subnode < $netwag_priv_toolsearch_tools_tree_toolstart} {
      netwag_priv_toolsearch_branches_rec $subnode $expand
    }
  }
}
proc netwag_priv_toolsearch_branches { expand } {
  netwag_priv_toolsearch_branches_rec 1 $expand
  netwag_priv_toolsearch_radiobutton_callback
}

#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Obtain the current selected tool number
proc netwag_toolsearch_toolnum_init_cursel {} {
  global netwag_priv_toolsearch_lb
  global netwag_priv_toolsearch_tools_tree_toolstart
  global netwag_priv_toolsearch_tools_tree_lbline
  global netwag_priv_toolsearch_tools_tree_text
  global netwag_priv_toolsearch_lb_type
  global netwag_priv_toolsearch_type_sort

  set selnum [netwag_priv_toolsearch_selnum_init_cursel]
  if { $selnum == -1 } {
    return -1
  }

  if {$netwag_priv_toolsearch_lb_type == $netwag_priv_toolsearch_type_sort} {
    set seltext [$netwag_priv_toolsearch_lb get $selnum]
    if { $seltext == {} } {
      return -1
    }
    set poscolon [expr [string first ":" $seltext] - 1]
    set toolnumtext [string range $seltext 0 $poscolon]
    set toolnum [expr $toolnumtext + 0]
  } else {
    set selnode $netwag_priv_toolsearch_tools_tree_lbline($selnum)
    if {$selnode < $netwag_priv_toolsearch_tools_tree_toolstart} {
      return -1
    } else {
      set seltext $netwag_priv_toolsearch_tools_tree_text($selnode)
      set poscolon [expr [string first ":" $seltext] - 1]
      set toolnumtext [string range $seltext 0 $poscolon]
      set toolnum [expr $toolnumtext + 0]
    }
  }

  return $toolnum
}

## Obtain the current selected node
proc netwag_toolsearch_node_init_cursel {} {
  global netwag_priv_toolsearch_lb_type
  global netwag_priv_toolsearch_type_tree
  global netwag_priv_toolsearch_tools_tree_lbline
  global netwag_priv_toolsearch_tools_tree_toolstart

  set selnode -1
  if {$netwag_priv_toolsearch_lb_type == $netwag_priv_toolsearch_type_tree} {
    set selnum [netwag_priv_toolsearch_selnum_init_cursel]
    if { $selnum != -1 } {
      set selnode $netwag_priv_toolsearch_tools_tree_lbline($selnum)
    }
  } else {
    set seltoolnum [netwag_toolsearch_toolnum_init_cursel]
    if { $seltoolnum != -1 } {
      set selnode [expr $netwag_priv_toolsearch_tools_tree_toolstart + $seltoolnum]
    }
  }

  return $selnode
}

## Initialize the search function
proc netwag_toolsearch_init { } {
  global netwag_priv_toolsearch_lb
  global netwag_priv_toolsearch_searchentry
  global netwag_priv_toolsearch_searchvar
  global netwag_priv_toolsearch_showsearch
  global netwag_priv_toolhf_wanttoolnum
  global netwag_priv_tool_nb

  # associate events
  bind $netwag_priv_toolsearch_searchentry <KeyRelease> {
    netwag_priv_toolsearch_radiobutton_callback
  }
  bind $netwag_priv_toolsearch_searchentry <Button-1> {
    set netwag_priv_toolsearch_showsearch 2
    netwag_priv_toolsearch_radiobutton_callback
  }
  bind $netwag_priv_toolsearch_lb <Button-1> {
    focus $netwag_priv_toolsearch_lb
  }
  bind $netwag_priv_toolsearch_lb <ButtonRelease-1> {
    # Note : this is a bit tricky here...
    # We have "search_lb <ButtonRelease-1> netwag_priv_toolsearch_tree_toggle".
    # On the system there is (/usr/lib/tk/listbox.tcl) "Listbox <ButRelease-1>
    # %W activate @%x,%y".
    # And bindtag is "search_lb Listbox . all"
    # So, in tree mode, when we release button, we do :
    #   search_lb activate end (this is correct)
    #   Listbox activate @%x,%y (this activates the current location
    # of the mouse, which might be incorrect because we've redrawed
    # the listbox)
    # So we use a small delay to ensure "Listbox <ButRelease-1>" is finished
    # BEFORE netwag_priv_toolsearch_tree_toggle
    after 1 netwag_priv_toolsearch_tree_toggle
    # set wanted num for help and form
    set wanttoolnum [netwag_toolsearch_toolnum_init_cursel]
    if {$wanttoolnum != -1} {
      set netwag_priv_toolhf_wanttoolnum $wanttoolnum
    }
  }
  bind $netwag_priv_toolsearch_lb <Double-Button-1> {
    set wanttoolnum [netwag_toolsearch_toolnum_init_cursel]
    if {$wanttoolnum != -1} {
      set netwag_priv_toolhf_wanttoolnum $wanttoolnum
      netwag_notebook_button_press $netwag_priv_tool_nb 2
    }
  }
  bind $netwag_priv_toolsearch_lb <Key-Return> {
    netwag_priv_toolsearch_tree_toggle
    set wanttoolnum [netwag_toolsearch_toolnum_init_cursel]
    if {$wanttoolnum != -1} {
      set netwag_priv_toolhf_wanttoolnum $wanttoolnum
      netwag_notebook_button_press $netwag_priv_tool_nb 2
    }
  }

  # initialize tool information
  global netwag_priv_toolsearch_tools_max
  global netwag_priv_toolsearch_tools_sort_max
  global netwag_priv_toolsearch_tools_tree_max
  set netwag_priv_toolsearch_tools_max 0
  set netwag_priv_toolsearch_tools_sort_max 0
  set netwag_priv_toolsearch_tools_tree_max 0

  # by default, netwag_gui shows the listbox of type sort
  global netwag_priv_toolsearch_lb_type
  global netwag_priv_toolsearch_type_sort
  set netwag_priv_toolsearch_lb_type $netwag_priv_toolsearch_type_sort
}

## Load tool definitions
global netwag_toolsearch_eventuallyinit_done
set netwag_toolsearch_eventuallyinit_done 0
proc netwag_toolsearch_eventuallyinit { } {
  global netwag_toolsearch_eventuallyinit_done
  if {$netwag_toolsearch_eventuallyinit_done} {
    return 0
  }

  # obtain list
  global netwag_glo_bin_netwox
  set ret [netwag_exec "$netwag_glo_bin_netwox 0 --tools" outmsg retval errmsg]
  if { $ret } {
    netwag_msg_add_err "Cannot obtain the list of tools ($errmsg)"
    return $ret
  }
  if { $retval } {
    netwag_msg_add_err "Tool list might be incomplete (retval=$retval)"
  }

  # set
  global netwag_priv_toolsearch_tools_max
  global netwag_priv_toolsearch_tools_sort_max
  global netwag_priv_toolsearch_tools_sort_lbline
  global netwag_priv_toolsearch_tools_tree_expand
  global netwag_priv_toolsearch_tools_tree_nodes
  global netwag_priv_toolsearch_tools_tree_branch
  global netwag_priv_toolsearch_tools_tree_text
  global netwag_priv_toolsearch_tools_tree_toolstart
  global netwag_priv_toolsearch_tools_synonyms
  netwag_cursor_wait
  catch {eval $outmsg}

  # init display
  netwag_priv_toolsearch_radiobutton_callback
  netwag_cursor_default

  # just indicate the number of tools
  global netwag_priv_toolsearch_tools_max
  netwag_msg_add_info "This version contains $netwag_priv_toolsearch_tools_max tools"

  set netwag_toolsearch_eventuallyinit_done 1
  return 0
}
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
# used for help and form
global netwag_priv_toolhf_curtoolnum ; set netwag_priv_toolhf_curtoolnum -1
global netwag_priv_toolhf_wanttoolnum ; set netwag_priv_toolhf_wanttoolnum -2


#################################################################
# PRIVATE FUNCTIONS
#################################################################

proc netwag_priv_toolhf_init { toolnum } {
  global netwag_priv_toolhelp_help
  global netwag_priv_toolhelp_example
  global netwag_priv_toolhelp_usage
  global netwag_glo_bin_netwox

  if {![info exist netwag_priv_toolhelp_help($toolnum)]} {
    set ret [netwag_exec "$netwag_glo_bin_netwox 0 -h -u $toolnum" outmsg retval errmsg]
    netwag_cursor_default
    if { $ret } {
      netwag_msg_add_err "Cannot obtain the help for $toolnum ($errmsg)"
      return $ret
    }
    if { $retval } {
      netwag_msg_add_err "Help for $toolnum might be incomplete (retgets=$retval)"
    }
    eval $outmsg
  }
}


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Fake function for empty message
proc netwag_toolform_display_func0 { } {
  netwag_toolform_add_text "No tool is selected. You have two choices to obtain help :\n - select a tool in Search notebook\n - enter a command (in front of Run button) and press Update button\n"
}

## Display description of a tool
proc netwag_toolhf_display { } {
  global netwag_priv_toolhelp_text
  global netwag_priv_toolhf_curtoolnum
  global netwag_priv_toolhf_wanttoolnum

  if {$netwag_priv_toolhf_wanttoolnum == $netwag_priv_toolhf_curtoolnum} {
    return
  }
  if {$netwag_priv_toolhf_wanttoolnum < 0} {
    # message
    set msg "No tool is selected. You have two choices to obtain help :\n - select a tool in Search notebook\n - enter a command (in front of Run button) and press Update button\n"
    $netwag_priv_toolhelp_text configure -state normal
    $netwag_priv_toolhelp_text delete 1.0 end
    $netwag_priv_toolhelp_text insert end $msg
    $netwag_priv_toolhelp_text configure -state disabled
    netwag_toolform_display 0
    return
  }

  # reinit help and form
  $netwag_priv_toolhelp_text configure -state normal
  $netwag_priv_toolhelp_text delete 1.0 end
  $netwag_priv_toolhelp_text configure -state disabled
  netwag_toolform_reinit

  # query and display (use a temporary variable because
  # netwag_priv_toolhf_wanttoolnum is global and can be changed between
  # the toolhf_init and _display)
  set wanttoolnum $netwag_priv_toolhf_wanttoolnum
  netwag_priv_toolhf_init $wanttoolnum
  netwag_toolhelp_display $wanttoolnum
  netwag_toolform_display $wanttoolnum
  set netwag_priv_toolhf_curtoolnum $wanttoolnum
}

## Called when Update tool is pressed
proc netwag_toolhf_button_update {} {

  if { [netwag_toolcmd_check usercmd toolnum] } {
    return
  }
  set realcmd [netwag_toolcmd_withpassword $usercmd]

  global netwag_priv_toolhf_wanttoolnum
  set netwag_priv_toolhf_wanttoolnum $toolnum

  netwag_toolhf_display

  # update contents
  global netwag_glo_bin_netwox
  netwag_runcmd_file_init $realcmd tmpfilename
  set ret [netwag_exec "$netwag_glo_bin_netwox 0 -f -b $tmpfilename -u $toolnum" outmsg retval errmsg]
  if { $ret } {
    netwag_msg_add_warn "Cannot update command line parameters for tool $toolnum ($errmsg)"
    netwag_runcmd_file_del $tmpfilename
    return
  }
  if { $retval } {
    netwag_msg_add_warn "Cannot update tool $toolnum: $outmsg"
    return
  }
  netwag_toolform_uncheck
  eval $outmsg
}

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
global netwag_priv_toolhelp_text

global netwag_priv_toolhelp_help
global netwag_priv_toolhelp_example
global netwag_priv_toolhelp_usage


#################################################################
# PRIVATE FUNCTIONS
#################################################################


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Display the description of a tool
proc netwag_toolhelp_display { toolnum } {
  global netwag_priv_toolhelp_text
  global netwag_priv_toolhelp_help

  $netwag_priv_toolhelp_text configure -state normal
  $netwag_priv_toolhelp_text delete 1.0 end
  $netwag_priv_toolhelp_text insert end $netwag_priv_toolhelp_help($toolnum)
  $netwag_priv_toolhelp_text configure -state disabled
}

## Called when Copy example is pressed
proc netwag_toolhelp_button_copyex {} {
  global netwag_priv_toolhf_curtoolnum
  global netwag_priv_toolhelp_example

  if {$netwag_priv_toolhf_curtoolnum == -1} {
    netwag_msg_add_err "No tool is selected"
    return
  }

  netwag_toolcmd_init $netwag_priv_toolhelp_example($netwag_priv_toolhf_curtoolnum)
}

## Called when Copy usage is pressed
proc netwag_toolhelp_button_copyus {} {
  global netwag_priv_toolhf_curtoolnum
  global netwag_priv_toolhelp_usage

  if {$netwag_priv_toolhf_curtoolnum == -1} {
    netwag_msg_add_err "No tool is selected"
    return
  }

  netwag_toolcmd_init $netwag_priv_toolhelp_usage($netwag_priv_toolhf_curtoolnum)
}
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
global netwag_priv_infol_text_output
global netwag_priv_infol_out_devices
global netwag_priv_infol_out_ip
global netwag_priv_infol_out_arp
global netwag_priv_infol_out_routes
global netwag_priv_infol_out_current


#################################################################
# PRIVATE FUNCTIONS
#################################################################

## Append data in the text
proc netwag_priv_infol_text_output_set { thedata } {
  global netwag_priv_infol_text_output

  $netwag_priv_infol_text_output configure -state normal
  $netwag_priv_infol_text_output delete 1.0 end
  $netwag_priv_infol_text_output insert end "$thedata"
  $netwag_priv_infol_text_output configure -state disabled
}

## Get configuration
proc netwag_priv_infol_update { } {
  global netwag_glo_bin_netwox
  global netwag_priv_infol_out_devices
  global netwag_priv_infol_out_ip
  global netwag_priv_infol_out_arp
  global netwag_priv_infol_out_routes

  netwag_priv_infol_text_output_set "Update in progress..."

  set msg ""
  set ret [netwag_exec "$netwag_glo_bin_netwox 0 -c" outmsg retval errmsg]
  if { $ret } {
    set msg "could not obtain the configuration ($errmsg)"
  } elseif { $retval } {
    set msg "could not obtain the configuration (retval=$retval)"
  } else {
    if {[catch {eval $outmsg}]} {
      set msg "error in tcl commands returned"
    }
  }
  if {$msg != ""} {
    set netwag_priv_infol_out_devices $msg
    set netwag_priv_infol_out_ip $msg
    set netwag_priv_infol_out_arp $msg
    set netwag_priv_infol_out_routes $msg
  }

  return 0
}


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Initialize local information
proc netwag_infol_init { } {
  global netwag_priv_infol_out_current
  set netwag_priv_infol_out_current 1

  return 0
}

## Called when a button is pressed
proc netwag_infol_button_press { butnum } {
  global netwag_priv_infol_out_devices
  global netwag_priv_infol_out_ip
  global netwag_priv_infol_out_arp
  global netwag_priv_infol_out_routes
  global netwag_priv_infol_out_current
  global netwag_glo_notebook_button_hidden_color
  global netwag_glo_notebook_button_visible_color
  global netwag_priv_infol_buttons

  if {$butnum == 1} {
    netwag_priv_infol_text_output_set $netwag_priv_infol_out_devices
    $netwag_priv_infol_buttons.butdevices configure -bg $netwag_glo_notebook_button_visible_color
  } else {
    $netwag_priv_infol_buttons.butdevices configure -bg $netwag_glo_notebook_button_hidden_color
  }

  if {$butnum == 2} {
    netwag_priv_infol_text_output_set $netwag_priv_infol_out_ip
    $netwag_priv_infol_buttons.butip configure -bg $netwag_glo_notebook_button_visible_color
  } else {
    $netwag_priv_infol_buttons.butip configure -bg $netwag_glo_notebook_button_hidden_color
  }

  if {$butnum == 3} {
    netwag_priv_infol_text_output_set $netwag_priv_infol_out_arp
    $netwag_priv_infol_buttons.butarp configure -bg $netwag_glo_notebook_button_visible_color
  } else {
    $netwag_priv_infol_buttons.butarp configure -bg $netwag_glo_notebook_button_hidden_color
  }

  if {$butnum == 4} {
    netwag_priv_infol_text_output_set $netwag_priv_infol_out_routes
    $netwag_priv_infol_buttons.butroutes configure -bg $netwag_glo_notebook_button_visible_color
  } else {
    $netwag_priv_infol_buttons.butroutes configure -bg $netwag_glo_notebook_button_hidden_color
  }

  set netwag_priv_infol_out_current $butnum
}

## Called when Update button is pressed
proc netwag_infol_button_update { } {

  netwag_cursor_wait
  netwag_priv_infol_update
  netwag_cursor_default

  global netwag_priv_infol_out_devices
  netwag_priv_infol_text_output_set $netwag_priv_infol_out_devices

  global netwag_priv_infol_out_current
  netwag_infol_button_press $netwag_priv_infol_out_current
  return 0
}

## To initialize
global netwag_infol_eventuallyinit_done
set netwag_infol_eventuallyinit_done 0
proc netwag_infol_eventuallyinit { } {
  global netwag_infol_eventuallyinit_done
  if {$netwag_infol_eventuallyinit_done} {
    return 0
  }
  netwag_infol_button_update
  set netwag_infol_eventuallyinit_done 1
}
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
global netwag_priv_infor_text_output
global netwag_priv_infor_text_input


#################################################################
# PRIVATE FUNCTIONS
#################################################################


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Initialize local information
proc netwag_infor_cb_end { txt realcmd usercmd } {
  global netwag_priv_infor_button_int
  pack forget $netwag_priv_infor_button_int
}
proc netwag_infor_init { } {
  global netwag_priv_infor_text_output

  netwag_runtext_init $netwag_priv_infor_text_output "" "netwag_infor_cb_end"

  set msg "Enter an IP address or a Hostname in field above and press on Query button."
  netwag_runtext_append_msg $netwag_priv_infor_text_output $msg

  return 0
}

## Called when Query button is pressed
proc netwag_infor_button_query { } {
  global netwag_priv_infor_text_input
  global netwag_priv_infor_text_output

  set q $netwag_priv_infor_text_input

  # do not use trim, because we might have spaces in the middle "ha, hb"
  set q [netwag_misc_string_replace $q " " ""]

  if { $q == "" } {
    netwag_msg_add_warn "Query \"\" too short"
    return -1
  }

  global netwag_priv_infor_button_int
  pack $netwag_priv_infor_button_int -side left
  netwag_update

  set ret [netwag_runtext_run $netwag_priv_infor_text_output "3 -a \"$q\"" "query for $q"]
  if { $ret } {
    pack forget $netwag_priv_infor_button_int
    return ret
  }

  return 0
}

## Called when Interrupt button is pressed
proc netwag_infor_button_int { } {
  global netwag_priv_infor_text_output

  netwag_runtext_int $netwag_priv_infor_text_output

  return 0
}
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


#################################################################
# PRIVATE FUNCTIONS
#################################################################
# Most of this code is taken from the Tk library.

## Used for Entry
proc netwag_priv_clip_tkEntryGetSelection {w} {
  set posstart [$w index sel.first]
  set posend [expr [$w index sel.last] - 1]
  set entryString [string range [$w get] $posstart $posend]
  if {[$w cget -show] != ""} {
    # a password
    regsub -all . $entryString [string index [$w cget -show] 0] entryString
  }
  return $entryString
}
proc netwag_priv_clip_tkEntrySeeInsert w {
  set c [$w index insert]
  set left [$w index @0]
  if {$left > $c} {
    $w xview $c
    return
  }
  set x [winfo width $w]
  if {$c > [$w index @[winfo width $w]]} {
    $w xview insert
  }
}

# on Text widget, cut
proc netwag_priv_clip_text_cut {w} {
  #puts mytextcut
  if {![catch {set data [$w get sel.first sel.last]}]} {
    clipboard clear -displayof $w
    clipboard append -displayof $w $data
    $w delete sel.first sel.last
  }
}

# on Entry widget, cut
proc netwag_priv_clip_entry_cut {w} {
  #puts myentrycut
  if {![catch {netwag_priv_clip_tkEntryGetSelection $w} data]} {
    clipboard clear -displayof $w
    clipboard append -displayof $w $data
    $w delete sel.first sel.last
  }
}

# on Text widget, copy
proc netwag_priv_clip_text_copy {w} {
  #puts mytextcopy
  if {![catch {set data [$w get sel.first sel.last]}]} {
    clipboard clear -displayof $w
    clipboard append -displayof $w $data
  }
}

# on Entry widget, copy
proc netwag_priv_clip_entry_copy {w} {
  #puts myentrycopy
  if {![catch {netwag_priv_clip_tkEntryGetSelection $w} data]} {
    clipboard clear -displayof $w
    clipboard append -displayof $w $data
  }
}

# on Text widget, paste
proc netwag_priv_clip_text_paste {w} {
  #puts mytextpaste
  catch {$w delete sel.first sel.last}
  if {[catch {selection get -displayof $w -selection CLIPBOARD} inclip]} {
    # ignore error
    return
  }
  $w insert insert $inclip
}

# on Entry widget, paste
proc netwag_priv_clip_entry_paste {w} {
  #puts myentrypaste
  catch {$w delete sel.first sel.last}
  if {[catch {selection get -displayof $w -selection CLIPBOARD} inclip]} {
    # ignore error
    return
  }
  $w insert insert $inclip
  netwag_priv_clip_tkEntrySeeInsert $w
}

# on Text widget, select
proc netwag_priv_clip_text_select {w} {
  catch {$w tag add sel 1.0 end}
}

# on Entry widget, select
proc netwag_priv_clip_entry_select {w} {
  catch {$w selection range 0 end}
}


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Initialize the clipboard
proc netwag_clip_init {} {
  global netwag_sys_platform
  global netwag_sys_platform_unix

  # add events to Text
  bind Text <Control-Key-x> { netwag_priv_clip_text_cut %W }
  bind Text <Control-Key-c> { netwag_priv_clip_text_copy %W }
  bind Text <Control-Key-v> { netwag_priv_clip_text_paste %W }
  bind Text <Shift-Key-Delete> { netwag_priv_clip_text_cut %W }
  bind Text <Control-Key-Insert> { netwag_priv_clip_text_copy %W }
  bind Text <Shift-Key-Insert> { netwag_priv_clip_text_paste %W }
  if {$netwag_sys_platform == $netwag_sys_platform_unix} {
    bind Text <Key-F20> { netwag_priv_clip_text_cut %W }
    bind Text <Key-F16> { netwag_priv_clip_text_copy %W }
    bind Text <Key-F18> { netwag_priv_clip_text_paste %W }
  }
  bind Text <Control-Key-a> { netwag_priv_clip_text_select %W }
  bind Text <Control-Key-f> netwag_menu_editfind
  bind Text <Button-1> {+ netwag_txtfind_current %W }

  # add events to Entry
  bind Entry <Control-Key-x> { netwag_priv_clip_entry_cut %W }
  bind Entry <Control-Key-c> { netwag_priv_clip_entry_copy %W }
  bind Entry <Control-Key-v> { netwag_priv_clip_entry_paste %W }
  bind Entry <Shift-Key-Delete> { netwag_priv_clip_entry_cut %W }
  bind Entry <Control-Key-Insert> { netwag_priv_clip_entry_copy %W }
  bind Entry <Shift-Key-Insert> { netwag_priv_clip_entry_paste %W }
  if {$netwag_sys_platform == $netwag_sys_platform_unix} {
    bind Entry <Key-F20> { netwag_priv_clip_entry_cut %W }
    bind Entry <Key-F16> { netwag_priv_clip_entry_copy %W }
    bind Entry <Key-F18> { netwag_priv_clip_entry_paste %W }
  }
  bind Entry <Control-Key-a> { netwag_priv_clip_entry_select %W }

}
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


#################################################################
# PRIVATE FUNCTIONS
#################################################################

# draw the clipboard
proc netwag_priv_smallclip_draw { } {
  global frame_clipboard
  global frame_message
  global netwag_glo_clipboard_text_height

  pack forget $frame_clipboard.scro.s
  pack forget $frame_clipboard.scro.less
  pack forget $frame_clipboard.scro.more
  pack forget $frame_clipboard.scro.plus
  pack forget $frame_clipboard.scro.c
  pack forget $frame_clipboard.text

  if { $netwag_glo_clipboard_text_height == 0 } {
    set netwag_glo_clipboard_text_height 1
  }
  pack $frame_clipboard.scro -side right -fill y
  $frame_clipboard.text configure -height $netwag_glo_clipboard_text_height
  $frame_clipboard.scro.c configure -width [winfo reqwidth $frame_message.scro]

  if { $netwag_glo_clipboard_text_height == 1 } {
    pack $frame_clipboard.scro.plus
  } elseif { $netwag_glo_clipboard_text_height <= 5 } {
    pack $frame_clipboard.scro.c -fill both -expand 1
    pack $frame_clipboard.scro.less -fill x
    pack $frame_clipboard.scro.more -fill x
    pack $frame_clipboard.text -expand yes -fill both
  } else {
    pack $frame_clipboard.scro.s -expand yes -fill y
    pack $frame_clipboard.scro.less -fill x
    pack $frame_clipboard.scro.more -fill x
    pack $frame_clipboard.text -expand yes -fill both
  }

}


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Initialize the clipboard
proc netwag_smallclip_init {} {

  # set size
  netwag_priv_smallclip_draw
}

## Buttons pressed to resize the height
proc netwag_smallclip_height_callback_less { } {
  global netwag_glo_clipboard_text_height

  incr netwag_glo_clipboard_text_height -1

  netwag_priv_smallclip_draw
}
proc netwag_smallclip_height_callback_more { } {
  global netwag_glo_clipboard_text_height

  incr netwag_glo_clipboard_text_height +1

  netwag_priv_smallclip_draw
}
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

