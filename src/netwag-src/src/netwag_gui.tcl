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

