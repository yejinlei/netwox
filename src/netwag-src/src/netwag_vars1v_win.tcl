set netwag_glo_dir_tmp "$env(TEMP)"
if {[info exists env(APPDATA)]} {
  set netwag_glo_file_session "$env(APPDATA)/netwag.ses"
} else {
  set netwag_glo_file_session "$env(TEMP)/netwag.ses"
}
set netwag_glo_bin_netwox "netwox539"
