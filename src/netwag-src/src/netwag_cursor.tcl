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
global netwag_cursor_default_cursor;
set netwag_cursor_default_cursor [. cget -cursor]

#################################################################
# PRIVATE FUNCTIONS
#################################################################
global netwag_priv_cursor_w
set netwag_priv_cursor_w {. .m .fc.text .fm.text .fn.body.pf1.bot.right.text.left.text}


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Set waiting cursor
proc netwag_cursor_wait { } {
  # We should change cursor for each window using something like:
  #   catch {. configure -cursor watch}
  #   set list [winfo children .]
  #   while {[llength $list]} {
  #     set next {}
  #     foreach w $list {
  #       catch {$w configure -cursor watch}
  #       set next [concat $next [winfo children $w]]
  #     }
  #     set list $next
  #   }
  #   update idletasks
  # However, this is long to run. When displaying watch, it's because
  # we have a long task to run. We do not want it to become longer.
  # So only main windows will have the clock:
  global netwag_priv_cursor_w
  foreach w $netwag_priv_cursor_w {
    $w configure -cursor watch
  }
  netwag_update
}

## Set default cursor
proc netwag_cursor_default { } {
  global netwag_cursor_default_cursor;
  global netwag_priv_cursor_w
  foreach w $netwag_priv_cursor_w {
    $w configure -cursor $netwag_cursor_default_cursor
  }
  netwag_update
}
