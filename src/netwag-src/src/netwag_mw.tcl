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

