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



