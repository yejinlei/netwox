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
