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
