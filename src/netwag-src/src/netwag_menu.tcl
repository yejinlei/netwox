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

