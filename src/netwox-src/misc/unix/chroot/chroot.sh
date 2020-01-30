:

# This script creates the chroot environment to run netwox or netwag in a jail.
# It was only tested under Linux. It is not guaranteed to work.

# root directory
rootdir=/securityjail
# adding /proc might cause security problems. Ensure you understand.
addproc=0
# also add netwag
addnetwag=1

# path
netwox=/usr/local/bin/netwox
netwag=/usr/local/bin/netwag
tcltkdir=/usr/lib

# END of user variables

if [ $addnetwag -eq 1 ] ; then
  # libtcl will contain "/usr/lib/tcl8.x"
  libtcl=$tcltkdir/`ls $tcltkdir | grep -e "^tcl"`
  # libtk will contain "/usr/lib/tk8.x"
  libtk=$tcltkdir/`ls $tcltkdir | grep -e "^tk"`
  # netwox5 will contain "/usr/local/bin/netwox52x"
  netwox5=`grep "set netwag_glo_bin_netwox" $netwag`
  netwox5=`echo $netwox5 | sed 's/"$//' | sed 's/^.*"//'`
  # wish will contain "/usr/bin/wish8.x"
  wish=`head -1 $netwag | sed 's/^#!//'`
fi

# END of guessed variables

#--
# add a file to the jail root
addfilereg () {
  fname=$1
  dname=$2

  cp $fname $rootdir/$dname

  # shared libraries
  ldd $fname 2>&1 | grep " => " | sed 's/^.* => //' |\
    sed 's/ .*$//' | while read f
  do
      addfile $f
  done
}
addfilemknod () {
  fname=$1
  type=$2

  maj=`ls -l $fname | awk '{print $5}' | sed 's/,//'`
  min=`ls -l $fname | awk '{print $6}'`

  mknod $rootdir/$fname $type $maj $min
}
addfilesocket () {
  fname=$1
  dname=$2

  # here, I use ln, because I don't know how to do it in a portable way
  ln $fname $rootdir/$dname
}
addfile () {
  fname=$1
  dname=`dirname $fname`

  # no need to put it twice
  if [ -e $rootdir/$fname ] ; then
    return
  fi

  echo "addfile $1"
  # create parent directory
  mkdir -p $rootdir/$dname

  if [ -f $fname ] ; then
    addfilereg $fname $dname
  elif [ -c $fname ] ; then
    addfilemknod $fname c
  elif [ -b $fname ] ; then
    addfilemknod $fname b
  elif [ -d $fname ] ; then
    mkdir $rootdir/$fname
  elif [ -S $fname ] ; then
    addfilesocket $fname $dname
  elif [ -e $fname ] ; then
    echo "**Not implemented. You have to add $fname"
  fi
}

#--
# run a program, and add what it needs
addrun () {

  strace $@ 2>&1 | grep "^open[(]" |\
    grep -v "= -1" | sed 's/^open[(]"//' | sed 's/".*$//' | while read f
  do
    addfile $f
  done

  strace $@ 2>&1 | grep "^connect[(].*AF_UNIX" |\
    grep -v "= -1" | sed 's/^.*path="//' | sed 's/".*$//' | while read f
  do
    addfile $f
  done
}

#--
#--
# MAIN

# must be root to run this program
if [ `id -u` -ne 0 ] ; then
  echo "Must be root to run this program."
  exit 1
fi

# remove old
if [ -e $rootdir/proc/sys ] ; then
  umount $rootdir/proc
fi
rm -rf $rootdir

# create root directory
mkdir -p $rootdir

# add /proc
if [ $addproc -eq 1 ] ; then
  mkdir -p $rootdir/proc
  # note: Linux kernel > 2.4 have mount --bind, which could be more precise
  mount -t proc /proc $rootdir/proc
else
  # Having a copy of /proc/net seems to be sufficient for most usages.
  # This could be improved by having a background script periodically copying
  # file in the jail.
  echo "/proc is not mounted"
fi

# we can need these (not only Linux)
addfile /bin/ip
addfile /bin/netstat
addfile /dev/null
addfile /dev/random
addfile /dev/urandom
addfile /etc/gateways
addfile /etc/host.conf
addfile /etc/hostname
addfile /etc/hosts
addfile /etc/ld.so.cache
addfile /etc/nsswitch.conf
addfile /etc/protocols
addfile /etc/resolv.conf
addfile /etc/services
addfile /sbin/ip
addfile /usr/bin/netstat
addfile /usr/sbin/arp
addfile /usr/sbin/ndp
addfile /usr/sbin/lanscan
if [ $addproc -ne 1 ] ; then
  addfile /proc/net/arp
  addfile /proc/net/if_inet6
  addfile /proc/net/ipv6_route
  addfile /proc/net/rarp
  addfile /proc/net/route
fi

# add main programs
addfile /bin/sh
addfile $netwox
#addfile /bin/ls
#addfile /usr/bin/less
#addfile /usr/bin/strace

# add run time dependencies
addrun $netwox 0
addrun $netwox 1
addrun $netwox 2
addrun $netwox 3 1.1.1.1
addrun $netwox 3 fec0::1
addrun $netwox 3 www.example.com
addrun $netwox 4 00:1:2:3:4:5
addrun $netwox 187
# others (sniff, spoof, client, server) cannot be easily automated

# add wish and netwag
if [ $addnetwag -eq 1 ] ; then
  mkdir -p $rootdir/$libtcl ; cp -R $libtcl/* $rootdir/$libtcl
  mkdir -p $rootdir/$libtk ; cp -R $libtk/* $rootdir/$libtk
  # netwag.ses is not copied by default because it might contain sensitive info
  #addfile /root/.netwag.ses
  addfile $wish
  addfile $netwox5
  addfile $netwag
  addrun $wish exit
fi

# create root
echo ""
echo "Now, run 'netwox' or 'netwag'. To finish, enter 'exit'."
echo "Other commands such as 'ls' are not available because of the jail."
echo "In ANOTHER shell, you can transfer file from/to $rootdir."
chroot $rootdir /bin/sh

# remove
if [ $addproc -eq 1 ] ; then
  umount $rootdir/proc
fi
rm -rf $rootdir
