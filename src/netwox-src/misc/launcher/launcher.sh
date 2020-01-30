#!/bin/bash

# This script was developed for bash shell only.

# It permits to launch several commands in parallel. Indeed, sometimes
# netwox tools are slow because they wait for a timeout to expire. This
# script permits to launch several simultaneous commands, in order to
# optimize bandwidth.

# Global variables
declare -i launch_maxchilds=5
declare -a launch_inuse=(0)

# Change the max simultaneous commands
launch_max () {
  launch_maxchilds=$1
}

# Wait until a new command can be launched
launch_waitone () {
  local i=0

  # wait for ready slot
  while [ $i -lt ${launch_maxchilds} ]  ; do
    if [ ${launch_inuse[$i]-0} -eq 0 ] ; then
      return $i
    fi
    let i+=1
  done

  # wait for end of one command
  i=0
  while [ 1 ] ; do
    ps p ${launch_inuse[$i]} > /dev/null 2> /dev/null
    if [ $? -ne 0 ] ; then
      # process has terminated, so its slot is free
      launch_inuse[$i]=0
      return $i
    fi
    let i+=1
    if [ $i -ge ${launch_maxchilds} ] ; then
      # here we could sleep
      i=0
    fi
  done
}

# Wait for all launched commands. It has to be called at end.
launch_waitall () {
  local i=0
  local end=1

  # wait for end of all commands
  while [ 1 ] ; do
    if [ ${launch_inuse[$i]-0} -ne 0 ] ; then
      ps p ${launch_inuse[$i]} > /dev/null 2> /dev/null
      if [ $? -ne 0 ] ; then
        # process has terminated, so its slot is free
        launch_inuse[$i]=0
      else
        # process is still running, so we cannot end
        end=0
      fi
    fi
    let i+=1
    if [ $i -ge ${launch_maxchilds} ] ; then
      if [ $end -eq 1 ] ; then
        return 0
      fi
      i=0
      end=1
    fi
  done
}

# Launch a command
launch () {
  local cmd=$*
  local i=0

  # wait for a free slot
  launch_waitone
  i=$?

  # run command
  $* &
  launch_inuse[$i]=$!
}

# Those 3 functions are needed to pass back variables to parent process.
# They have to be used for loops. There are not needed if there is
# no child process.
declare launch_loop_tmpfile
launch_loop_init () {
  launch_loop_tmpfile=`mktemp /tmp/netwox_launcher_XXXXXXXXX`
  chmod 600 $launch_loop_tmpfile
}
launch_loop_core () {
  declare | egrep "^launch_inuse=" > $launch_loop_tmpfile
}
launch_loop_end () {
  . $launch_loop_tmpfile
  rm $launch_loop_tmpfile
}

