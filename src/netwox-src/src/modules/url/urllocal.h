
/*-------------------------------------------------------------*/
/* Convert an url to a filename and an array of extensions:
    - http://server/dir/file          => file []            use_ct
    - http://server/dir/FILE.jPg      => file [jpg]         use_ct
    - http://server/dir/file.tar.gz   => file [tar.gz,gz]   use_ct
    - http://server/dir/f.php?v=a     => f_php_v_a []       use_ct
    - http://server/dir/              => index [html]       !use_ct
    - http://server/dir/?v=a          => v_a []             use_ct
   All characters are converted to lowercase.
   Characters not in [A-Za-z0-9-] are represented by _.
*/
netwib_err netwox_urllocal_encode_part1normal(netwib_constbuf *purl,
                                              netwib_buf *pfilename,
                                              netwib_array *pextarray,
                                              netwib_bool *pusemimetypes);
/*
   Idem, but, on error:
    - http://server/dir/file          => file [html]        !use_ct
    - http://server/dir/file.html     => file [html]        !use_ct
    - http://server/dir/file.r.htm    => file_r [htm]       !use_ct
    - http://server/dir/file.jpg      => file_jpg [html]    !use_ct
    - http://server/dir/file.tar.gz   => file_tar_gz [html] !use_ct
    - http://server/dir/f.php?v=a     => f_php_v_a [html]   !use_ct
    - http://server/dir/              => index [html]       !use_ct
    - http://server/dir/?v=a          => v_a [html]         !use_ct
*/
netwib_err netwox_urllocal_encode_part1(netwib_constbuf *purl,
                                        netwib_bool iserror,
                                        netwib_buf *pfilename,
                                        netwib_array *pextarray,
                                        netwib_bool *pusemimetypes);

/*-------------------------------------------------------------*/
/*
Configuration file contains a list of MIME types with their allowed
extensions. For example:
      type      allowed-extensions
    text/html     html,htm
    image/gif     gif
    image/jpeg    jpeg,jpg
    none          dat,zip,tar.gz

Each end of path is split to a filename and an extension, according to
the following rules:
 - if the MIME type is known:
     + if there is no extension, use the first entry in the allowed
       extensions for the MIME type
     + if the extension is in the allowed extensions, use this extension
     + else, convert .ext to _ext (except if ext is in deletedextensions) and
       use the first entry in the allowed extensions for the MIME type,
       and display a warning inviting user to:
         - add "mime-type extension" in the list if he wants to keep
           the .ext
         - add extension in deletedextensions if it is ok to not use _ext
 - if the MIME type is unknown:
     + if there is no extension, use html, and display a
       warning inviting user to add the MIME type in the list
     + else, use this extension, and display a warning inviting user to
       add "mime-type extension" in the list
 - if there is no MIME type:
     + if there is no extension, use html
     + if the extension is in one of the allowed extensions, use this extension
     + else, convert .ext to _ext (except if ext is in deletedextensions) and
       use html, and display a warning inviting user to:
         - add "none extension" in the list if he wants to keep the .ext
         - add extension in deletedextensions if it is ok to not use _ext

Example:
 - file [jpg] (jpg extension is in one of the list of allowed extensions)
   mime-type=image/jpeg => file        jpg
   mime-type=image/gif  => file_jpg    gif [+warning1]
                           file        gif (if jpg in deletedextensions)
   mime-type=image/foo  => file        jpg [+warning]
   no mime-type         => file        jpg
 - file [bar] (bar extension is not in any list of allowed extensions)
   mime-type=image/jpeg => file_bar    jpeg [+warning1]
                           file        jpeg (if bar in deletedextensions)
   mime-type=image/foo  => file        bar [+warning]
   no mime-type         => file_bar    html [+warning2]
                           file        html (if bar in deletedextensions)
 - file [] (no extension)
   mime-type=image/jpeg => file        jpeg
   mime-type=image/foo  => file        html [+warning]
   no mime-type         => file        html
*/
netwib_err netwox_urllocal_encode_part2(netwib_constbuf *purl,
                                        netwib_constbuf *pfilename,
                                        netwib_constarray *pextarray,
                                        netwib_bool usemimetypes,
                                        netwox_mimetypes *pmimetypes,
                                        netwib_bool mimetypeswarn,
                                        netwib_constarray *pdeletedextensions,
                                        netwib_constbuf *pcontenttype,
                                        netwib_buf *pfilename2,
                                        netwib_buf *pextension2);

/*-------------------------------------------------------------*/
/* Depending on file number, construct the directory name:
    rootdir/files        f0
    rootdir/files        f1
    rootdir/files        f63
    rootdir/files/01     f64 .. f127
    rootdir/files/3f     f4032 .. f4095
    rootdir/files/01/00  f4096 .. f4159
    rootdir/files/01/01  f4160 ..
*/
netwib_err netwox_urllocal_encode_part3(netwib_constbuf *prootdir,
                                        netwib_uint64 filenumber,
                                        netwib_buf *plocaldir);

/*-------------------------------------------------------------*/
/* When the filename is too long, or already exists in the destination
   directory, a random end is added:
    filerandom.ext
    filefilefilrandom.ext
   We also ignore filenames using two hexadecimal characters
   because it might correspond to a future directory name.
   We also ignore trans.tbl because it might be added by mkisofs.
*/
netwib_err netwox_urllocal_encode_part4(netwib_constbuf *plocaldir,
                                        netwib_constbuf *pfilename2,
                                        netwib_constbuf *pextension2,
                                        netwib_buf *plocalfilename);

/*-------------------------------------------------------------*/
/* try to guess content of a file */
netwib_err netwox_urllocal_encode_part5(netwib_constbuf *pextension2,
                                        netwox_mimetypes *pmimetypes,
                                        netwib_bool *pcontainshtml);

/*-------------------------------------------------------------*/
/* all operations */
netwib_err netwox_urllocal_encode(netwib_constbuf *prootdir,
                                  netwib_constbuf *purl,
                                  netwib_bool iserror,
                                  netwox_mimetypes *pmimetypes,
                                  netwib_bool mimetypeswarn,
                                  netwib_constarray *pdeletedextensions,
                                  netwib_constbuf *pcontenttype,
                                  netwib_uint64 filenumber,
                                  netwib_buf *plocalfilename,
                                  netwib_bool *pcontainshtml);

/*-------------------------------------------------------------*/
netwib_err netwox_urllocal_test(void);
