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
proc netwag_misc_string_replace_test { } {
  set ret [netwag_misc_string_replace "abcde" "c" "Z"]
  if { $ret != "abZde" } {
    puts "Bad output : $ret"
    exit
  }
  set ret [netwag_misc_string_replace "" "c" "Z"]
  if { $ret != "" } {
    puts "Bad output : $ret"
    exit
  }
  set ret [netwag_misc_string_replace "accde" "c" "ZY"]
  if { $ret != "aZYZYde" } {
    puts "Bad output : $ret"
    exit
  }
  set ret [netwag_misc_string_replace "ccdddc" "c" "ZY"]
  if { $ret != "ZYZYdddZY" } {
    puts "Bad output : $ret"
    exit
  }
  set ret [netwag_misc_string_replace "ccdddc" "cd" "ZY"]
  if { $ret != "cZYddc" } {
    puts "Bad output : $ret"
    exit
  }

  exit
}

proc netwag_misc_string_map_test { } {
  set ret [netwag_misc_string_map "abcde" {"c" "Z"}]
  if { $ret != "abZde" } {
    puts "Bad output : $ret"
    exit
  } else {puts "ok"}
  set ret [netwag_misc_string_map "" {"c" "Z"}]
  if { $ret != "" } {
    puts "Bad output : $ret"
    exit
  } else {puts "ok"}
  set ret [netwag_misc_string_map "accde" {"c" "ZY"}]
  if { $ret != "aZYZYde" } {
    puts "Bad output : $ret"
    exit
  } else {puts "ok"}
  set ret [netwag_misc_string_map "ccdddc" {"c" "ZY"}]
  if { $ret != "ZYZYdddZY" } {
    puts "Bad output : $ret"
    exit
  } else {puts "ok"}
  set ret [netwag_misc_string_map "ccdddc" {"cd" "ZY"}]
  if { $ret != "cZYddc" } {
    puts "Bad output : $ret"
    exit
  } else {puts "ok"}
  set ret [netwag_misc_string_map "abcdef" {"ab" "A" "de" "D"}]
  if { $ret != "AcDf" } {
    puts "Bad output : $ret"
    exit
  } else {puts "ok"}
  set ret [netwag_misc_string_map "1abcaababcabababc" {abc 1 ab 2 a 3 1 0}]
  if { $ret != "01321221" } {
    puts "Bad output : $ret"
    exit
  } else {puts "ok"}

}


#################################################################
# PUBLIC FUNCTIONS
#################################################################

## Replace data in a string (needed because "string map" does not
## exists in version 8.0)
proc netwag_misc_string_replace { str strtoreplace replaceby } {

  if {![catch {string map [list "$strtoreplace" "$replaceby"] $str} newstr]} {
    return $newstr
  }

  # note : we cannot use "string first" with 5 parameters because
  # it is not supported in version 8.0
  set newstr ""
  set searchin $str
  set lengthstrtoreplace [string length $strtoreplace]
  while {1} {
    set pos [string first $strtoreplace $searchin]
    if { $pos == -1 } {
      # append end
      append newstr $searchin
      return $newstr
    }
    append newstr [string range $searchin 0 [expr $pos - 1]]
    append newstr $replaceby
    set startat [expr $pos + $lengthstrtoreplace]
    set searchin [string range $searchin $startat end]
  }

}

## Replace data in a string (needed because "string map" does not
## exists in version 8.0)
proc netwag_misc_string_map { str listab } {

  if {![catch {string map $listab $str} newstr]} {
    return $newstr
  }

  set listlength [llength $listab]
  set j 0
  for {set i 0} {$i < $listlength} {incr i} {
    set strfrom($j) [lindex $listab $i]
    incr i
    if {$i ==  $listlength} {
      # ignore error
      break;
    }
    set strto($j) [lindex $listab $i]
    incr j
  }

  set newstr ""
  set searchin $str
  set arraysize [array size strfrom]
  while {1} {

    set found 0
    for {set i 0} {$i < $arraysize} {incr i} {
      set pos [string first $strfrom($i) $searchin]
      if {$pos != -1} {
        if {!$found} {
          set found 1
          set minpos $pos
          set maxlength [string length $strfrom($i)]
          set useitem $i
        } else {
          if {$pos < $minpos} {
            set minpos $pos
            set maxlength [string length $strfrom($i)]
            set useitem $i
          } elseif {$pos == $minpos} {
            if {[string length $strfrom($i)] > $maxlength} {
              set maxlength [string length $strfrom($i)]
              set useitem $i
            }
          }
        }
      }
    }

    if { ! $found } {
      # append end
      append newstr $searchin
      return $newstr
    }
    append newstr [string range $searchin 0 [expr $minpos - 1]]
    append newstr $strto($useitem)
    set startat [expr $minpos + $maxlength]
    set searchin [string range $searchin $startat end]
  }

}

## Generate a random number used for filenames
proc netwag_misc_bigrand { } {
  set randnum [expr int(rand()*1000000000)]
  set randnum [expr $randnum - 10000000 * int($randnum/10000000)]
  return $randnum
}

## Convert data to hexa printable
proc netwag_misc_debug_hexa { data } {
  set result ""
  set resulth ""
  set resultc ""
  set strlendata [string length $data]
  for {set i 0} {$i < $strlendata} {} {
    set c [string index $data $i]
    # create hexa string
    scan $c "%c" cb
    append resulth [format "%02X " $cb]
    # create char string
    if {$cb >= 32 && $cb <= 127} {
      append resultc $c
    } else {
      append resultc "."
    }
    # concatenate
    incr i
    if {!($i % 16)} {
      append result "$resulth    $resultc\n"
      set resulth ""
      set resultc ""
    }
  }
  set leftsize [expr $strlendata % 16]
  if {$leftsize} {
    append result "$resulth    "
    set leftsize [expr 16 - $leftsize]
    for {set i 0} {$i < $leftsize} {incr i} {
      append result "   "
    }
    append result "$resultc\n"
  }
  return $result
}

## Align text. For example (var="ab..n", size=10, leading="0000") :
# "0000abcdef\n"
# "0000ghijkl\n"
# "0000mn\n"
proc netwag_misc_align { var size {leading ""} } {

  set leadingsize [string length $leading]
  if {$leadingsize >= $size} {
    return "Error in alignment"
  }

  set var [string trim $var]

  set i 0
  set res $leading
  set ressize $leadingsize
  while { 1 } {
    set c [string index $var $i]
    if {$c == "\r"} {
      # ignore
    } elseif {$c == "\n"} {
      append res "\n$leading"
      set ressize $leadingsize
    } elseif {$c == ""} {
      break
    } else {
      if {$ressize == $size} {
        append res "\n$leading$c"
        set ressize [expr $leadingsize + 1]
      } else {
        append res $c
        incr ressize
      }
    }
    incr i
  }
  if {$ressize > $leadingsize} {
    append res "\n"
  }

  return "$res"
}
