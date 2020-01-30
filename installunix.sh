:

#cd ../..

nokbd=0
configdat=0
if [ "Z$1" = "Znokbd" ] ; then
  # automatic installation
  nokbd=1
fi
if [ "Z$1" = "Zconfigdat" ] ; then
  # edit config.dat
  configdat=1
fi

cd src/netwib-src/src/
if [ $configdat -eq 1 ] ; then
  echo "*** EDITING config.dat file"
  editor=vi
  echo "Now, config.dat eventually needs to be edited."
  echo "It contains variables about your system."
  echo "Press Return key to enter $editor editor, or Enter an editor name (emacs,"
  echo "textedit, kedit, etc.) followed by Return key to use another editor."
  read foobar
  if [ "Z$foobar" != "Z" ] ; then
    editor=$foobar
    echo ""
  fi
  $editor config.dat
  if [ "Z$?" != "Z0" ] ; then
    echo "Editor $editor returned an error"
    exit 1
  fi
fi

echo "*** RETRIEVING documentation directory"
. ./config.dat
instdoc=${NETWIBDEF_INSTDESTDIR}${NETWIBDEF_INSTPREFIX}/doc
instdoc2=$instdoc/netw539
if [ -d $instdoc2 ] ; then
  echo "Found $instdoc2"
else
  echo "Will create $instdoc2"
fi
echo ""

instag=1
if [ $nokbd -eq 0 ] ; then
  echo "*** NETWAG installation ?"
  # I think printf is more widely supported than echo -n
  printf "Do you want to install netwag ? [n/Y] "
  read userchoice
  if [ "Z$userchoice" = "Zn" -o "Z$userchoice" = "ZN" ] ; then
    instag=0
  fi
  echo ""
fi

if [ $nokbd -eq 0 ] ; then
  echo "*** INSTALLATION"
  echo "Press Return key to start installation."
  read foobar
fi

echo "*** CREATING documentation directory"
rm -rf $instdoc2
mkdir -p $instdoc2

echo "*** INSTALLING netwib"
./genemake
err=$?
if [ $err -ne 0 ] ; then
  echo "A fatal error occurred"
  exit $err
fi
make
make install
cd ../../
cp -R netwib-doc_html $instdoc2

echo "*** INSTALLING netwox"
cd netwox-src/src/
./genemake
err=$?
if [ $err -ne 0 ] ; then
  echo "A fatal error occurred"
  exit $err
fi
make
make install
cd ../../
cp -R netwox-doc_html $instdoc2

if [ $instag -eq 1 ] ; then
  echo "*** INSTALLING netwag"
  cd netwag-src/src/
  ./genemake
  err=$?
  if [ $err -ne 0 ] ; then
    echo "A fatal error occurred"
    exit $err
  fi
  make
  make install
  cd ../../
  cp -R netwag-doc_html $instdoc2
fi

echo "*** CHANGING documentation rights"
chmod -R 755 $instdoc2

echo ""
echo ""
echo "*** END OK"
echo " You can now:"
echo "  - read $instdoc2/netwox-doc_html/html/examples.html"
echo "  - run netwox"
if [ $instag -eq 1 ] ; then
  echo "  - read $instdoc2/netwag-doc_html/html/lessons.html"
  echo "  - run netwag"
fi
if [ $nokbd -eq 0 ] ; then
  printf "\a"
fi

