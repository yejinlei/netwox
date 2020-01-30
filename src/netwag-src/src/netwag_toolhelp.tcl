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
