This directory contains a patch for the FreeBSD kernel.

It is used to spoof ethernet source addresses.

This patch has to be used on 3.x kernels.

How to use :
 - cp diff.if_ethersubr.c /sys/net/
 - cd /sys/net/
 - cp if_ethersubr.c if_ethersubr.c.ori
 - patch < diff.if_ethersubr.c
 - compile the kernel (read the handbook)
 - add in a starting script :
    sysctl -w net.link.ether.inet.specify_src_hwaddr=1
 - reboot
