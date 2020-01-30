
/*-------------------------------------------------------------*/
/***************************************************************
 * Do a "man regexp" if you don't know what is a "regular      *
 * expression").                                               *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* Name : netwib_buf_search_re
   Description :
     Search a regular expression in a string.
   Input parameter(s) :
     *pbuf : buffer containing input data
     regularexpression : searched regular expression.
                         For example : "a.", "f[A-Z]", etc.
     casesensitive : NETWIB_TRUE if the search has to be case
                     sensitive
   Input/output parameter(s) :
   Output parameter(s) :
     *pfound : buffer set with found data
   Normal return values :
     NETWIB_ERR_OK : the searched element was found
     NETWIB_ERR_NOTFOUND : the searched element wasn't found
   This function supports NETWIB_BUF_FLAGS_SENSITIVE.
*/
netwib_err netwib_buf_search_re(netwib_constbuf *pbuf,
                                netwib_constbuf *pregularexpression,
                                netwib_bool casesensitive,
                                netwib_bufext *pfound);

/*-------------------------------------------------------------*/
/***************************************************************
 * A netwib_regexp contains an array of found data during a    *
 * a regular expression search.                                *
 * The element at index 0 defines the location of the          *
 * global expression.                                          *
 * Elements at index 1, 2, etc. define the found string        *
 * between '(' and ')'.                                        *
 *                                                             *
 * For example, if the data is "houses" and the regular        *
 * expression is "o(.(s))", then :                             *
 *   re.numset = 3                                             *
 *   re.array[0] = "ous"                                       *
 *   re.array[1] = "us"                                        *
 *   re.array[2] = "s"                                         *
 *   re.array[3..63] not initialized                           *
 ***************************************************************/

/*-------------------------------------------------------------*/
/* We cannot have more than 64 parenthesis. This should be
   sufficient in all cases, but you may want to increase it,
   and recompile netwib.
 */
#define NETWIB_REGEXP_MAXLEN 64
typedef struct {
  netwib_uint32 numset; /* number of buffers set */
  netwib_bufext array[NETWIB_REGEXP_MAXLEN];
} netwib_regexp;

/*-------------------------------------------------------------*/
/* Name : netwib_buf_search_regexp
   Description :
     Search a regular expression in a string.
   Input parameter(s) :
     *pbuf : buffer containing input data
     regularexpression : searched regular expression.
                         For example : "a.", "f[A-Z]", etc.
     casesensitive : NETWIB_TRUE if the search has to be case
                     sensitive
   Input/output parameter(s) :
   Output parameter(s) :
     *pfound : netwib_regexp containing the found data
   Normal return values :
     NETWIB_ERR_OK : the searched regular expression was found
     NETWIB_ERR_NOTFOUND : the searched regexp wasn't found
   This function supports NETWIB_BUF_FLAGS_SENSITIVE.
*/
netwib_err netwib_buf_search_regexp(netwib_constbuf *pbuf,
                                    netwib_constbuf *pregularexpression,
                                    netwib_bool casesensitive,
                                    netwib_regexp *pfound);

