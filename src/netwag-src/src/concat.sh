:

set -u

genval=${1-notset}
wishpath=${2-/usr/bin/wish}

if [ "$genval" = "unix" ] ; then
  gentype=1
else
  gentype=0
fi

if [ $gentype -eq 1 ] ; then
  echo "#!$wishpath"
fi
cat netwag_vars1c.tcl
if [ $gentype -eq 1 ] ; then
  cat netwag_vars1v_unix.tcl
else
  cat netwag_vars1v_win.tcl
fi
echo ""
echo ""
echo "#CHANGE NOTHING BELOW unless you know what you do"
echo "#CHANGE NOTHING BELOW unless you know what you do"
echo ""
if [ $gentype -eq 1 ] ; then
  cat netwag_vars2c.tcl
  cat netwag_vars2v_unix.tcl
fi
cat netwag_varsver.tcl
cat netwag_varsglo.tcl
# generic
cat netwag_sys.tcl
cat netwag_misc.tcl
cat netwag_update.tcl
cat netwag_notebook.tcl
cat netwag_dynhelp.tcl
cat netwag_tmpfile.tcl
cat netwag_varfile.tcl
# generic but with some small specific parts
cat netwag_cursor.tcl
cat netwag_mw.tcl
# execution
cat netwag_popen.tcl
cat netwag_exec.tcl
cat netwag_runcmd.tcl
cat netwag_runnw.tcl
cat netwag_runtext.tcl
# specific, but non graphical
cat netwag_glo.tcl
cat netwag_session.tcl
cat netwag_bitmap.tcl
# popup windows
cat netwag_txtfind.tcl
cat netwag_font.tcl
cat netwag_formhist.tcl
cat netwag_help.tcl
# main interface
cat netwag_gui.tcl
cat netwag_menu.tcl
cat netwag_toolcmd.tcl
cat netwag_toolsearch.tcl
cat netwag_toolhf.tcl
cat netwag_toolhelp.tcl
cat netwag_toolform.tcl
cat netwag_toolrun.tcl
cat netwag_toolhist.tcl
cat netwag_infol.tcl
cat netwag_infor.tcl
cat netwag_clip.tcl
cat netwag_smallclip.tcl
cat netwag_msg.tcl
cat netwag_main.tcl
