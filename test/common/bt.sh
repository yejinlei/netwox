:

set -u

if [ $# -ne 3 ] ; then
  echo "Usage : $0 program display[0_1] ad,ad,ad,ad"
  exit 1
fi

program=$1
display_notused=$2
addresses=$3

# should use mktemp, but does not work under Solaris
tmpconf="./tmp_bt_1"

echo $addresses | tr ',' '\012' | while read aline
do
  #echo "set listsize 5" > $tmpconf
  echo "l *$aline" > $tmpconf
  echo "quit" >> $tmpconf
  (echo "quit" | gdb -q -x $tmpconf $program ) 2>/dev/null | grep "is in"
done

rm $tmpconf

