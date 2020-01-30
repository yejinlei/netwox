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
global netwag_varfile_error


#################################################################
# PRIVATE FUNCTIONS
#################################################################

## Set error messages
proc netwag_varfile_error_set { msg } {
  global netwag_varfile_error
  set netwag_varfile_error $msg
}

## Write variable value
proc netwag_priv_varfile_write_val { fd val } {
  set strlenval [string length $val]

  puts $fd $strlenval

  set chunksize 60
  set startpos 0
  while { $startpos < $strlenval } {
    set endpos [expr $startpos + $chunksize - 1]
    if { $endpos < $strlenval } {
      set trueline [string range $val $startpos $endpos]
    } else {
      set trueline [string range $val $startpos end]
    }
    set line [netwag_misc_string_replace $trueline "\\" "\\\\"]
    set line [netwag_misc_string_replace $line "\0" "\\0"]
    set line [netwag_misc_string_replace $line "\r" "\\r"]
    set line [netwag_misc_string_replace $line "\n" "\\n"]
    puts $fd $line
    set startpos [expr $endpos + 1]
  }

}

## Read variable value
proc netwag_priv_varfile_read_val { filename valname fd pval } {
  upvar $pval val

  # first, there is the size of the variable
  set ret [gets $fd line]
  if { $ret == -1 } {
    netwag_varfile_error_set "Could not read variable size for $valname from $filename"
    return -1
  }
  set varsize $line

  # special case of empty line
  if { $varsize == 0 } {
    # no data to read
    set val ""
    return 0
  }

  # now, read the value
  set val ""
  while {![eof $fd]} {
    set ret [gets $fd line]
    if { $ret == -1 } {
      netwag_varfile_error_set "Error when trying to read value for $valname from $filename"
      return -1
    }
    set line [netwag_misc_string_map $line {"\\\\" "\\" "\\0" "\0" "\\r" "\r" "\\n" "\n"}]
    append val $line
    set readsize [string length $val]
    if { $readsize == $varsize } {
      return 0
    } elseif { $readsize > $varsize } {
      netwag_varfile_error_set "Value contain too much data ($readsize>$varsize) for $valname from $filename"
      return -1
    }
  }
  return 0
}

## Read the varfile
proc netwag_priv_varfile_read_variable { filename fd pvarfilestruct } {
  upvar $pvarfilestruct varfilestruct

  # first, there is the name of the variable
  set ret [gets $fd line]
  if { $ret == -1 } {
    netwag_varfile_error_set "Could not read variable name from $filename"
    return -1
  }
  set varname $line

  # now, read the value
  set ret [netwag_priv_varfile_read_val $filename $varname $fd varval]
  if { $ret } {
    return -1
  }

  set varfilestruct($varname) $varval
  return 0
}

## Read the varfile
proc netwag_priv_varfile_read_array { filename fd pvarfilestruct } {
  upvar $pvarfilestruct varfilestruct

  # first, there is the name of the array
  set ret [gets $fd line]
  if { $ret == -1 } {
    netwag_varfile_error_set "Could not read array name from $filename"
    return -1
  }
  set arrayname $line

  # then, read the array size
  set ret [gets $fd line]
  if { $ret == -1 } {
    netwag_varfile_error_set "Could not read array size for $arrayname from $filename"
    return -1
  }
  set arraysize $line

  # now, read the values
  for {set i 0} {$i < $arraysize} {incr i} {
    set varname "$arrayname\($i\)"
    set ret [netwag_priv_varfile_read_val $filename $varname $fd varval]
    if { $ret } {
      return -1
    }
    set varfilestruct($arrayname,$i) $varval
  }
  return 0
}

## Read the varfile
proc netwag_priv_varfile_read_list { filename fd pvarfilestruct } {
  upvar $pvarfilestruct varfilestruct

  # first, there is the name of the list
  set ret [gets $fd line]
  if { $ret == -1 } {
    netwag_varfile_error_set "Could not read list name from $filename"
    return -1
  }
  set listname $line

  # then, read the list size
  set ret [gets $fd line]
  if { $ret == -1 } {
    netwag_varfile_error_set "Could not read list size for $listname from $filename"
    return -1
  }
  set listsize $line

  # now, read the values
  for {set i 0} {$i < $listsize} {incr i} {
    set varname "$listname at index $i"
    set ret [netwag_priv_varfile_read_val $filename $varname $fd varval]
    if { $ret } {
      return -1
    }
    lappend varfilestruct($listname) $varval
  }
  return 0
}


#################################################################
# PUBLIC WRITING FUNCTIONS
#################################################################

## Initialize the varfile
proc netwag_varfile_write_init { filename pfd } {
  upvar $pfd fd

  if {[file exists $filename]} {
    if  {[catch {file delete -- $filename}]} {
      netwag_varfile_error_set "Could not modify file $filename"
      return -1
    }
  }

  if  {[catch {open $filename {WRONLY EXCL CREAT} 0600} fd]} {
    netwag_varfile_error_set "Could not open $filename"
    return -1
  }
  # 2 (add support for LIST) is the current version of varfile syntax
  puts $fd "2"
  puts $fd "# This is a session file for netwag. You should not edit it."
  puts $fd ""
  return 0
}

## Close the varfile
proc netwag_varfile_write_close { fd } {
  close $fd
  return 0
}

## Write variable
proc netwag_varfile_write_variable { fd var val } {
  puts $fd "VARIABLE"
  puts $fd "$var"
  netwag_priv_varfile_write_val $fd $val
  puts $fd ""
  return 0
}

## Write array
proc netwag_varfile_write_array { fd var parray } {
  upvar $parray thearray
  puts $fd "ARRAY"
  puts $fd "$var"
  set arraysize [array size thearray]
  puts $fd $arraysize
  for {set i 0} {$i<$arraysize} {incr i} {
    netwag_priv_varfile_write_val $fd $thearray($i)
  }
  puts $fd ""
  return 0
}

## Write list
proc netwag_varfile_write_list { fd var plist } {
  upvar $plist thelist
  puts $fd "LIST"
  puts $fd "$var"

  set listsize [llength $thelist]
  puts $fd $listsize
    foreach v $thelist {
    netwag_priv_varfile_write_val $fd $v
  }
  puts $fd ""
  return 0
}

## Just for test
proc netwag_varfile_write_test {} {
  global netwag_varfile_error
  for {set i 0} {$i<30} {incr i} {
    set thearr($i) "aa $i bb"
  }
  set ret [netwag_varfile_write_init "/tmp/ses" fd]
  if { $ret } {
    puts "Error $netwag_varfile_error"
  }

  netwag_varfile_write_variable $fd "toto1" "blabla"
  netwag_varfile_write_variable $fd "toto2" "bl\nabla"
  netwag_varfile_write_variable $fd "toto3" "bl\0abla"
  netwag_varfile_write_variable $fd "toto4" 1
  netwag_varfile_write_variable $fd "toto5" ""
  netwag_varfile_write_array $fd "thearr" thearr
  netwag_varfile_write_close $fd
}


#################################################################
# PUBLIC READING FUNCTIONS
#################################################################

## Read the varfile
proc netwag_varfile_read { filename pvarfilestruct } {

  if  {[catch {set fd [open $filename "r"]}]} {
    netwag_varfile_error_set "Could not read $filename"
    return -1
  }

  # read the version
  set ret [gets $fd line]
  if { $ret == -1 } {
    netwag_varfile_error_set "Could not read version from $filename"
    close $fd
    return -1
  }
  set theversion $line
  if { $theversion != 1 && $theversion != 2 } {
    netwag_varfile_error_set "This program is too old to recognize sessions format version \"$theversion\", or the file is not a session file (from $filename)"
    close $fd
    return -1
  }

  upvar $pvarfilestruct varfilestruct
  while {![eof $fd]} {
    # get a line
    set ret [gets $fd line]
    if { $ret == -1 } {
      if {[eof $fd]} { break; }
      netwag_varfile_error_set "Could not read line from $filename"
      close $fd
      return -1
    }
    # ignore empty lines
    if { [regexp "^\s*$" $line] } { continue; }
    # ignore commented lines
    if { [regexp "^\s*#" $line] } { continue; }
    # check known parameters
    if { $line == "VARIABLE" } {
      set ret [netwag_priv_varfile_read_variable $filename $fd varfilestruct]
    } elseif { $line == "ARRAY" } {
      set ret [netwag_priv_varfile_read_array $filename $fd varfilestruct]
    } elseif { $line == "LIST" } {
      set ret [netwag_priv_varfile_read_list $filename $fd varfilestruct]
    } else {
      netwag_varfile_error_set "Parameter $line unrecognized in file $filename"
      set ret -1
    }
    if { $ret } {
      if {[eof $fd]} {
        netwag_varfile_error_set "Premature end of file $filename"
      }
      close $fd
      return -1
    }
  }

  if { [catch { close $fd }] } {
    netwag_varfile_error_set "Could not close $filename"
    return -1
  }
  return 0
}

## Just for test
proc netwag_varfile_read_test {} {
  global netwag_varfile_error
  set ret [netwag_varfile_read "/tmp/ses" titi]
  if { $ret } {
     puts "err $netwag_varfile_error"
     return
  }
  puts $titi(toto1)
  puts $titi(toto2)
  puts $titi(toto3)
  puts $titi(toto4)
  puts $titi(toto5)
  puts $titi(thearr,12)

}
