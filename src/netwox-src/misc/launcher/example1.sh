#!/bin/bash

. ./launcher.sh

# Note : This is just a small example. We should redirect tool's outputs
#        to different files otherwise lines are mixed.

launch_max 5
launch_loop_init
netwox 213 192.168.1.0%24 | while read ipaddress
do
  launch netwox 6 --ip $ipaddress
  launch_loop_core
done
launch_loop_end
launch_waitall

