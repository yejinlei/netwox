
/*-------------------------------------------------------------*/
/***************************************************************
 * An Ethernet range is of the form :                          *
 *                                    ethinf       ethsup      *
 *   a:b:c:d:e:f                  : a:b:c:d:e:f  a:b:c:d:e:f   *
 *   a:b:c:d:e:f-a:b:c:d:f:f      : a:b:c:d:e:f  a:b:c:d:f:f   *
 *   a:b:c:d:e:0/40               : a:b:c:d:e:0  a:b:c:d:e:f   *
 *   a:b:c:d:e:0%40               : a:b:c:d:e:1  a:b:c:d:e:e   *
 *   a:b:c:d:e:0/ff:ff:ff:ff:ff:0 : a:b:c:d:e:0  a:b:c:d:e:f   *
 * Notes :                                                     *
 *  - '%' has the same meaning as '/', except that the         *
 *    broadcast addresses are excluded from range.             *
 *                                                             *
 * A netwib_eths is of the form :                              *
 *   eth,eth,ethrange,ethrange                                 *
 *   all,!eth,!ethrange                                        *
 *                                                             *
 * Complete examples :                                         *
 *   a:b:c:d:e:f                                               *
 *   a:b:c:d:e:f-a:b:c:d:f:f                                   *
 *   a:b:c:d:e:f,a:b:c:d:f:f                                   *
 *   1:2:3:4:5:6,a:b:c:d:e:f-a:b:c:d:f:f                       *
 *   all,!1:2:3:4:5:6,!a:b:c:d:e:f-a:b:c:d:f:f                 *
 ***************************************************************/

/*-------------------------------------------------------------*/
/***************************************************************
 * Those functions ignores following error cases :             *
 *  - if we try to add a value already in the list             *
 *  - if we try to remove a value not in the list              *
 ***************************************************************/

/*-------------------------------------------------------------*/
typedef struct netwib_eths netwib_eths;
typedef const netwib_eths netwib_consteths;

/*-------------------------------------------------------------*/
/* Name : netwib_eths_init
   Description :
     Initialize a netwib_eths used to store several Ethernet
     addresses.
   Input parameter(s) :
     inittype : if future added items will be sorted and/or unique
   Input/output parameter(s) :
   Output parameter(s) :
     **ppeths : netwib_eths allocated and initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
typedef enum {
  NETWIB_ETHS_INITTYPE_SORTUNIQ = 1, /* sorted and unique */
  NETWIB_ETHS_INITTYPE_NOTSORTUNIQ = 2, /* not sorted and unique */
  NETWIB_ETHS_INITTYPE_NOTSORTNOTUNIQ = 3 /* not sorted and not
                                             unique (duplicates
                                             are not removed) */
} netwib_eths_inittype;
netwib_err netwib_eths_init(netwib_eths_inittype inittype,
                            netwib_eths **ppeths);
#define netwib_eths_initdefault(ppeths) netwib_eths_init(NETWIB_ETHS_INITTYPE_SORTUNIQ,ppeths)

/*-------------------------------------------------------------*/
/* Name : netwib_eths_close
   Description :
     Close a netwib_eths.
   Input parameter(s) :
   Input/output parameter(s) :
     **ppeths : netwib_eths closed
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_eths_close(netwib_eths **ppeths);

/*-------------------------------------------------------------*/
/* Name : netwib_eths_add_eth
   Description :
     Add an address to the netwib_eths.
   Input parameter(s) :
     eth : address to add
   Input/output parameter(s) :
     *peths : netwib_eths where to work
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_eths_add_eth(netwib_eths *peths,
                               netwib_consteth *peth);

/*-------------------------------------------------------------*/
/* Name : netwib_eths_add_ethrange
   Description :
     Add a range of addresses to the netwib_eths.
   Input parameter(s) :
     infeth : inferior eth
     supeth : superior eth
   Input/output parameter(s) :
     *peths : netwib_eths where to work
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_eths_add_ethrange(netwib_eths *peths,
                                    netwib_consteth *pinfeth,
                                    netwib_consteth *psupeth);

/*-------------------------------------------------------------*/
/* Name : netwib_eths_add_eths
   Description :
     Add a list of addresses to the netwib_eths.
   Input parameter(s) :
     *pethstoadd : netwib_eths to add
   Input/output parameter(s) :
     *peths : netwib_eths where to work
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
   Note :
     If an error occurs during insertion, result will only
     contain partial data. It's developer's job to use a
     temporary netwib_eths to deal with such errors.
*/
netwib_err netwib_eths_add_eths(netwib_eths *peths,
                                netwib_consteths *pethstoadd);

/*-------------------------------------------------------------*/
/* Name : netwib_eths_add_buf
   Description :
     Add a list represented as a string like
     "aa:bb:cc:dd:ee:ff-aa:bb:cc:dd:ff:ff".
   Input parameter(s) :
     *pbuf : an Ethernet string
   Input/output parameter(s) :
     *peths : netwib_eths updated
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_eths_add_buf(netwib_eths *peths,
                               netwib_constbuf *pbuf);

/*-------------------------------------------------------------*/
/* Name : netwib_eths_add_kbd
   Description :
     Update a list with data entered through keyboard.
   Input parameter(s) :
     message : message to print before
    defaultlist : default list to use if user enters nothing
                  if NULL, there is no default
   Input/output parameter(s) :
   Output parameter(s) :
     *peths : netwib_eths updated
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_eths_add_kbd(netwib_eths *peths,
                               netwib_constbuf *pmessage,
                               netwib_constbuf *pdefaultlist);

/*-------------------------------------------------------------*/
/* Name : netwib_eths_del_eth
   Description :
     Del an address to the netwib_eths.
   Input parameter(s) :
     eth : address to delete
   Input/output parameter(s) :
     *peths : netwib_eths where to work
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_eths_del_eth(netwib_eths *peths,
                               netwib_consteth *peth);

/*-------------------------------------------------------------*/
/* Name : netwib_eths_del_ethrange
   Description :
     Del a range of addresses to the netwib_eths.
   Input parameter(s) :
     infeth : inferior eth
     supeth : superior eth
   Input/output parameter(s) :
     *peths : netwib_eths where to work
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_eths_del_ethrange(netwib_eths *peths,
                                    netwib_consteth *pinfeth,
                                    netwib_consteth *psupeth);

/*-------------------------------------------------------------*/
/* Name : netwib_eths_del_eths
   Description :
     Remove a list of addresses to the netwib_eths.
   Input parameter(s) :
     *pethstoadd : netwib_eths to remove
   Input/output parameter(s) :
     *peths : netwib_eths where to work
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_eths_del_eths(netwib_eths *peths,
                                netwib_consteths *pethstodel);

/*-------------------------------------------------------------*/
/* Name : netwib_eths_contains_xyz
   Description :
     Check if a netwib_eth is in the list.
   Input parameter(s) :
     eth : netwib_eth to find
   Input/output parameter(s) :
     *peths : netwib_eths containing the list of addresses
   Output parameter(s) :
     *pyes : true if netwib_eth is found
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_eths_contains_eth(netwib_consteths *peths,
                                    netwib_consteth *peth,
                                    netwib_bool *pyes);
netwib_err netwib_eths_contains_ethrange(netwib_consteths *peths,
                                         netwib_consteth *pinfeth,
                                         netwib_consteth *psupeth,
                                         netwib_bool *pyes);

/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_eths
   Description :
     Append a string representing a netwib_eths.
   Input parameter(s) :
     *peths : netwib_eths to append
   Input/output parameter(s) :
     pbuf : buffer updated
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_append_eths(netwib_consteths *peths,
                                  netwib_buf *pbuf);
