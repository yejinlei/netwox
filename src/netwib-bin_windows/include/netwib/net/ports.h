
/*-------------------------------------------------------------*/
/***************************************************************
 * An port range is of the form :                              *
 *                                   portinf     portsup       *
 *   123                      :        123         123         *
 *   123-124                  :        123         124         *
 *                                                             *
 * An netwib_ports is of the form :                            *
 *   port,port,portrange,portrange                             *
 *   all,!port,!portrange                                      *
 *                                                             *
 * Complete examples :                                         *
 *   123                                                       *
 *   123-125                                                   *
 *   1234,5678                                                 *
 *   1234,12356-12358                                          *
 *   all,!1234,!1244-1246                                      *
 ***************************************************************/

/*-------------------------------------------------------------*/
/***************************************************************
 * Those functions ignores following error cases :             *
 *  - if we try to add a value already in the list             *
 *  - if we try to remove a value not in the list              *
 ***************************************************************/

/*-------------------------------------------------------------*/
typedef struct netwib_ports netwib_ports;
typedef const netwib_ports netwib_constports;

/*-------------------------------------------------------------*/
/* Name : netwib_ports_init
   Description :
     Initialize a netwib_ports used to store port list.
   Input parameter(s) :
     inittype : if future added items will be sorted and/or unique
   Input/output parameter(s) :
   Output parameter(s) :
     **ppports : netwib_ports allocated and initialized
   Normal return values :
     NETWIB_ERR_OK : ok
*/
typedef enum {
  NETWIB_PORTS_INITTYPE_SORTUNIQ = 1, /* sorted and unique */
  NETWIB_PORTS_INITTYPE_NOTSORTUNIQ = 2, /* not sorted and unique */
  NETWIB_PORTS_INITTYPE_NOTSORTNOTUNIQ = 3 /* not sorted and not
                                              unique (duplicates
                                              are not removed) */
} netwib_ports_inittype;
netwib_err netwib_ports_init(netwib_ports_inittype inittype,
                             netwib_ports **ppports);
#define netwib_ports_initdefault(ppports) netwib_ports_init(NETWIB_PORTS_INITTYPE_SORTUNIQ,ppports)

/*-------------------------------------------------------------*/
/* Name : netwib_ports_close
   Description :
     Close a netwib_ports.
   Input parameter(s) :
   Input/output parameter(s) :
     **ppports : netwib_ports closed
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ports_close(netwib_ports **ppports);

/*-------------------------------------------------------------*/
/* Name : netwib_ports_add_port
   Description :
     Add a port to the netwib_ports.
   Input parameter(s) :
     port : address to add
   Input/output parameter(s) :
     *pports : netwib_ports where to work
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ports_add_port(netwib_ports *pports,
                                 netwib_port port);

/*-------------------------------------------------------------*/
/* Name : netwib_ports_add_portrange
   Description :
     Add a range of ports to the netwib_ports.
   Input parameter(s) :
     infport : inferior port
     support : superior port
   Input/output parameter(s) :
     *pports : netwib_ports where to work
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ports_add_portrange(netwib_ports *pports,
                                      netwib_port infport,
                                      netwib_port support);

/*-------------------------------------------------------------*/
/* Name : netwib_ports_add_ports
   Description :
     Add a list of port to the netwib_ports.
   Input parameter(s) :
     *pportstoadd : netwib_ports to add
   Input/output parameter(s) :
     *pports : netwib_ports where to work
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ports_add_ports(netwib_ports *pports,
                                  netwib_constports *pportstoadd);

/*-------------------------------------------------------------*/
/* Name : netwib_ports_add_buf
   Description :
     Update a netwib_ports with a string like "1234-1235".
   Input parameter(s) :
     *pbuf : buffer containing string
   Input/output parameter(s) :
     *pports : netwib_ports updated
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
   Note :
     If an error occurs during insertion, result will only
     contain partial data. It's developer's job to use a
     temporary netwib_ports to deal with such errors.
*/
netwib_err netwib_ports_add_buf(netwib_ports *pports,
                                netwib_constbuf *pbuf);

/*-------------------------------------------------------------*/
/* Name : netwib_ports_add_kbd
   Description :
     Update a netwib_ports with data entered through keyboard.
   Input parameter(s) :
     message : message to print before
     defaultlist : default list to use if user enters nothing
                   if NULL, there is no default
   Input/output parameter(s) :
     *pports : netwib_ports updated
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ports_add_kbd(netwib_ports *pports,
                                netwib_constbuf *pmessage,
                                netwib_constbuf *pdefaultlist);

/*-------------------------------------------------------------*/
/* Name : netwib_ports_del_port
   Description :
     Del a port from the netwib_ports.
   Input parameter(s) :
     port : address to delete
   Input/output parameter(s) :
     *pports : netwib_ports where to work
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ports_del_port(netwib_ports *pports,
                                 netwib_port port);

/*-------------------------------------------------------------*/
/* Name : netwib_ports_del_portrange
   Description :
     Del a range of ports to the netwib_ports.
   Input parameter(s) :
     infport : inferior port
     support : superior port
   Input/output parameter(s) :
     *pports : netwib_ports where to work
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ports_del_portrange(netwib_ports *pports,
                                      netwib_port infport,
                                      netwib_port support);

/*-------------------------------------------------------------*/
/* Name : netwib_ports_del_ports
   Description :
     Remove a list of port to the netwib_ports.
   Input parameter(s) :
     *pportstodel : netwib_ports to remove
   Input/output parameter(s) :
     *pports : netwib_ports where to work
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ports_del_ports(netwib_ports *pports,
                                  netwib_constports *pportstodel);

/*-------------------------------------------------------------*/
/* Name : netwib_ports_contains_xyz
   Description :
     Check if a netwib_port is in the list.
   Input parameter(s) :
     port : netwib_port to find
   Input/output parameter(s) :
     *pports : netwib_ports containing the list of addresses
   Output parameter(s) :
     *pyes : true if netwib_port is found
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_ports_contains_port(netwib_constports *pports,
                                      netwib_port port,
                                      netwib_bool *pyes);
netwib_err netwib_ports_contains_portrange(netwib_constports *pports,
                                           netwib_port infport,
                                           netwib_port support,
                                           netwib_bool *pyes);

/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_ports
   Description :
     Append a string representing a netwib_ports.
   Input parameter(s) :
     *pports : netwib_ports to append
   Input/output parameter(s) :
     pbuf : buffer updated
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_buf_append_ports(netwib_constports *pports,
                                   netwib_buf *pbuf);
