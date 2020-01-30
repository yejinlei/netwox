#!/bin/bash

. ./launcher.sh

# Note : This is just a small example. We should redirect tool's outputs
#        to different files otherwise lines are mixed.

foobar () {
  ipad=$1

  echo "$ipad in progress..."
  netwox 6 --ip $ipad
  # to simulate a slow command, sleep 2 seconds
  sleep 2
  echo "$ipad done"
}

launch_max 5
launch foobar 192.168.1.6
launch foobar 192.168.1.9
launch foobar 192.168.1.13
launch foobar 192.168.1.60
launch foobar 192.168.1.61
launch foobar 192.168.1.62
launch foobar 192.168.1.63
launch foobar 192.168.1.89
launch foobar 192.168.1.90
launch_waitall

