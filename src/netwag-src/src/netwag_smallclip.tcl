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

# draw the clipboard
proc netwag_priv_smallclip_draw { } {
  global frame_clipboard
  global frame_message
  global netwag_glo_clipboard_text_height

  pack forget $frame_clipboard.scro.s
  pack forget $frame_clipboard.scro.less
  pack forget $frame_clipboard.scro.more
  pack forget $frame_clipboard.scro.plus
  pack forget $frame_clipboard.scro.c
  pack forget $frame_clipboard.text

  if { $netwag_glo_clipboard_text_height == 0 } {
    set netwag_glo_clipboard_text_height 1
  }
  pack $frame_clipboard.scro -side right -fill y
  $frame_clipboard.text configure -height $netwag_glo_clipboard_text_height
  $frame_clipboard.scro.c configure -width [winfo reqwidth $frame_message.scro]

  if { $netwag_glo_clipboard_text_height == 1 } {
    pack $frame_clipboard.scro.plus
  } elseif { $netwag_glo_clipboard_text_height <= 5 } {
    pack $frame_clipboard.scro.c -fill both -expand 1
    pack $frame_clipboard.scro.less -fill x
    pack $frame_clipboard.scro.more -fill x
    pack $frame_clipboard.text -expand yes -fill both
  } else {
    pack $frame_clipboard.scro.s -expand yes -fill y
    pack $frame_clipboard.scro.less -fill x
    pack $frame_clipboard.scro.more -fill x
    pack $frame_clipboard.text -expand yes -fill both
  }

}


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Initialize the clipboard
proc netwag_smallclip_init {} {

  # set size
  netwag_priv_smallclip_draw
}

## Buttons pressed to resize the height
proc netwag_smallclip_height_callback_less { } {
  global netwag_glo_clipboard_text_height

  incr netwag_glo_clipboard_text_height -1

  netwag_priv_smallclip_draw
}
proc netwag_smallclip_height_callback_more { } {
  global netwag_glo_clipboard_text_height

  incr netwag_glo_clipboard_text_height +1

  netwag_priv_smallclip_draw
}
