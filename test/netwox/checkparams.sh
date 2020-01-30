:

i=0
while [ $i -lt 300 ] ; do
  #echo $i
  a=`./netwox $i --help2 | grep -- '^ --' | sed 's/^.*--//' | sed 's/ .*$//' | sort | wc -l`
  b=`./netwox $i --help2 | grep -- '^ --' | sed 's/^.*--//' | sed 's/ .*$//' | sort -u | wc -l`
  if [ $a -ne $b ] ; then
    echo Erreur long $i
  fi

  a=`./netwox $i --help2 | grep -- '|--' | grep -v "kbd-name" | sed 's/|.*$//' | sed 's/^.*-//' | sort | wc -l`
  b=`./netwox $i --help2 | grep -- '|--' | grep -v "kbd-name" | sed 's/|.*$//' | sed 's/^.*-//' | sort -u | wc -l`
  if [ $a -ne $b ] ; then
    echo Erreur court $i
  fi

  i=`expr $i + 1`
done

echo "Check if new tools correctly display in netwag"
echo "Check if all tools using stdin are defined for netwag"
echo "Check if all tools using backspace are defined for netwag"

