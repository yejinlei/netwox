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
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
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


