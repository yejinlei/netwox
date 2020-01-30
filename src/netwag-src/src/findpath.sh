:

set -u

searchbin=$1

IFS=":"

for direc in $PATH
do
  test -z "$direc" && dir=.
  if test -x "$direc/$searchbin" ; then
    echo "$direc/$searchbin"
    exit
  fi
done
echo ""

