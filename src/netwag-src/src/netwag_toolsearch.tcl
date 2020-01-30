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
