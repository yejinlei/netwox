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
