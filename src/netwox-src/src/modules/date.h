
/*-------------------------------------------------------------*/
netwib_err netwox_date_rfc822(netwib_consttime *pt,
                              netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* like rfc822 but without GMT */
netwib_err netwox_date_rfc822simple(netwib_consttime *pt,
                                    netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* can be used to set local clock:
     date --set "thisvalue"
*/
netwib_err netwox_date_unixdate(netwib_consttime *pt,
                                netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* an human readable value with nanoseconds */
netwib_err netwox_date_human(netwib_consttime *pt,
                             netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* second */
netwib_err netwox_date_sec(netwib_consttime *pt,
                           netwib_buf *pbuf);

/*-------------------------------------------------------------*/
/* second:nanosecond */
netwib_err netwox_date_secnsec(netwib_consttime *pt,
                               netwib_buf *pbuf);
