#!/bin/sh

set -u

param=${1-notset}

case $param in
  1)
    ;;
  2)
    exit 123
    ;;
  3)
    echo "Hello"
    ;;
  4)
    read varname
    ;;
  5)
    read varname
    echo "Read $varname"
    ;;
  6)
    echo "Hello"
    read varname
    echo "Read $varname"
    ;;
  7)
    sleep 2
    echo "Hello"
    sleep 2
    read varname
    sleep 2
    echo "Read $varname"
    sleep 2
    ;;
  8)
    while true
    do
      read varname
      if [ "Z$varname" = "Z" ] ; then
        exit 0
      fi
      echo "Read $varname"
    done
    ;;
  9)
    echo "BUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUG"
    echo "BUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUG"
    echo "BUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUG"
    echo "BUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUG"
    echo "BUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUG"
    echo "BUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUG" 1>&2
    echo "BUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUG" 1>&2
    echo "BUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUG" 1>&2
    echo "BUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUG" 1>&2
    echo "BUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUGBUG" 1>&2
    ;;
  *)
    echo "Unrecognized value : $param"
    exit 255
esac