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
global netwag_bitmap_down_init ; set netwag_bitmap_down_init 0
global netwag_bitmap_up_init ; set netwag_bitmap_up_init 0
global netwag_bitmap_top_init ; set netwag_bitmap_top_init 0
global netwag_bitmap_plus_init ; set netwag_bitmap_plus_init 0
global netwag_bitmap_close_init ; set netwag_bitmap_close_init 0


#################################################################
# PRIVATE FUNCTIONS
#################################################################


#################################################################
# PUBLIC FUNCTIONS
#################################################################

proc netwag_bitmap_down { bm } {
  global netwag_glo_gui_bgcolor_button
  global netwag_bitmap_down_init

  if {! $netwag_bitmap_down_init} {
    # to create this, use Unix "bitmap", save file, and paste contents
    set bm_down {
      #define i_width 15
      #define i_height 15
      static unsigned char i_bits[] = {
        0xc0, 0x01, 0xe0, 0x03, 0xf0, 0x07, 0xf0, 0x07, 0xe0, 0x03, 0xc0, 0x01,
        0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0xf8, 0x0f, 0xf0, 0x07,
        0xe0, 0x03, 0xc0, 0x01, 0x80, 0x00, 0x00, 0x00};
    }
    image create bitmap netwag_bitmap_down_bm -data $bm_down
    set netwag_bitmap_down_init 1
  }

  label $bm -image netwag_bitmap_down_bm -bd 0 -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  netwag_dynhelp_init $bm "This icon indicates that, when button is pressed, data goes from top window to bottom window."
}

proc netwag_bitmap_up { bm } {
  global netwag_glo_gui_bgcolor_button
  global netwag_bitmap_up_init

  if {! $netwag_bitmap_up_init} {
    set bm_up {
      #define i_width 15
      #define i_height 15
      static unsigned char i_bits[] = {
        0x80, 0x00, 0xc0, 0x01, 0xe0, 0x03, 0xf0, 0x07, 0xf8, 0x0f, 0x80, 0x00,
        0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0xc0, 0x01, 0xe0, 0x03, 0xf0, 0x07,
        0xf0, 0x07, 0xe0, 0x03, 0xc0, 0x01, 0x00, 0x00};
    }
    image create bitmap netwag_bitmap_up_bm -data $bm_up
    set netwag_bitmap_up_init 1
  }

  label $bm -image netwag_bitmap_up_bm -bd 0 -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  netwag_dynhelp_init $bm "This icon indicates that, when button is pressed, data goes from bottom window to top window."
}

proc netwag_bitmap_top { bm } {
  global netwag_glo_gui_bgcolor_button
  global netwag_bitmap_top_init

  if {! $netwag_bitmap_top_init} {
    set bm_top {
      #define i_width 15
      #define i_height 15
      static unsigned char i_bits[] = {
        0xc0, 0x01, 0xe0, 0x03, 0xf0, 0x07, 0xf0, 0x07, 0xe0, 0x03, 0xc0, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    }
    image create bitmap netwag_bitmap_top_bm -data $bm_top
    set netwag_bitmap_top_init 1
  }

  label $bm -image netwag_bitmap_top_bm -bd 0 -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
  netwag_dynhelp_init $bm "This icon indicates button work on above window."
}

proc netwag_bitmap_plus { bm } {
  global netwag_glo_gui_bgcolor_button
  global netwag_bitmap_plus_init

  if {! $netwag_bitmap_plus_init} {
    set bm_plus {
      #define tt_width 15
      #define tt_height 5
      static unsigned char i_bits[] = {
        0x04, 0x02, 0x0e, 0x07, 0x9f, 0x0f, 0x04, 0x02, 0x04, 0x02};
    }
    image create bitmap netwag_bitmap_plus_bm -data $bm_plus
    set netwag_bitmap_plus_init 1
  }

  label $bm -image netwag_bitmap_plus_bm -bd 0 -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
}

proc netwag_bitmap_close { bm } {
  global netwag_glo_gui_bgcolor_button
  global netwag_bitmap_close_init

  if {! $netwag_bitmap_close_init} {
    set bm_close {
      #define tt_width 16
      #define tt_height 16
      static unsigned char t_bits[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x1c, 0x0f, 0x1e, 0x1f, 0x1f,
        0xbe, 0x0f, 0xfc, 0x07, 0xf8, 0x03, 0xf0, 0x01, 0xf8, 0x03, 0xfc, 0x07,
        0xbe, 0x0f, 0x1f, 0x1f, 0x0f, 0x1e, 0x07, 0x1c};
    }
    image create bitmap netwag_bitmap_close_bm -data $bm_close
    set netwag_bitmap_close_init 1
  }

  label $bm -image netwag_bitmap_close_bm -bd 0 -bg $netwag_glo_gui_bgcolor_button -highlightthickness 0
}

