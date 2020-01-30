
/*-------------------------------------------------------------*/
typedef enum {
  NETWIB_ERR_ENCODETYPE_TEXT = 1,    /* append the error text */
  NETWIB_ERR_ENCODETYPE_NUMTEXT,     /* append "Error n : text" */
  NETWIB_ERR_ENCODETYPE_FULL         /* full (error, errno, h_errno,
                                        GetLastError, errstr) :
                                        "Error n : text\n `--> message\n"
                                     */
} netwib_err_encodetype;

/*-------------------------------------------------------------*/
/* Name : netwib_buf_append_err
   Description :
     Append a string representing an error.
   Input parameter(s) :
     error : error to print
     encodetype : netwib_err_encodetype to use
   Input/output parameter(s) :
   Output parameter(s) :
     *pbuf : netwib_buf receiving data
   Normal return values :
     NETWIB_ERR_OK : ok
   Notes :
     - Error code might have been set by a previous error. By
       example, error in open() sets errno, and then an error
       is gethostbyname() sets h_errno, and both will be
       displayed because the first one is not purged.
       This is normal, because if user directly used
       gethostbyname(), he knows he doesn't have to look at
        errno.
     - If an error occurred in function open() and sets errno.
       Then, memory might need to be freed (so errno will be
       unset). Then error is returned to user, but errno is
       zero.
    As a conclusion, user might not_get/get_incorrect errno,
    h_errno or GetLastError.
*/
netwib_err netwib_buf_append_err(netwib_err error,
                                 netwib_err_encodetype encodetype,
                                 netwib_buf *buf);

/*-------------------------------------------------------------*/
/* Name : netwib_err_display
   Description :
     Print the error associated to a netwib_err.
   Input parameter(s) :
     error : error to print
     encodetype : netwib_err_encodetype to use
   Input/output parameter(s) :
   Output parameter(s) :
   Normal return values :
     NETWIB_ERR_OK : ok
*/
netwib_err netwib_err_display(netwib_err error,
                              netwib_err_encodetype encodetype);

/*-------------------------------------------------------------*/
/***************************************************************
 * Every netwib function return an netwib_err corresponding to *
 * an error code. Developer have to test this value to ensure  *
 * everything went fine. There is 3 ways to do it :            *
 *                                                             *
 *  1 - doing the test by hand :                               *
 *    ret = function(...);                                     *
 *    if (ret != NETWIB_ERR_OK) {                              *
 *      return(ret);                                           *
 *    }                                                        *
 *                                                             *
 *  2 - use netwib_er :                                        *
 *    netwib_er(function(...));                                *
 *                                                             *
 *  3 - use netwib_eg :                                        *
 *    Note : this define uses a "goto". Developers tend to     *
 *           hate using goto. I also do, but there is one case *
 *           where goto are very useful. This case is for      *
 *           error handling because it creates an error flow   *
 *           similar to exceptions in Java or C++.             *
 *           I'll try to explain it : a program has two flows  *
 *           inside : the normal flow (the real job done) and  *
 *           the error flow (what to do when an error occurs). *
 *           With most algorithms, both flow use the same path,*
 *           so there is no need to use goto. But, when flow   *
 *           differs, we have to complicate the algorithm to   *
 *           deal with both normal and errors conditions.      *
 *           Without goto, it quickly becomes hard to read     *
 *           code, to free (only allocated) resources, to close*
 *           (only opened) descriptors, etc.                   *
 *           With goto, we have something like :               *
 *    {
 *      netwib_io *pio1, *pio2, *pio3;
 *      netwib_bool pio1set, pio2set, pio3set;
 *      netwib_err ret;
 *
 *      pio1set = pio2set = pio3set = NETWIB_FALSE;
 *      ret = NETWIB_ERR_OK;
 *
 *      netwib_eg(netwib_io_init...(&pio1));
 *      pio1set = NETWIB_TRUE;
 *      netwib_eg(netwib_io_init...(&pio2));
 *      pio2set = NETWIB_TRUE;
 *      here_complicated_code_which_can_use_"netwib_eg()"
 *      netwib_eg(netwib_io_close(&pio2));
 *      pio2set = NETWIB_FALSE;
 *      here_complicated_code_which_can_use_"netwib_eg()"
 *      netwib_eg(netwib_io_init...(&pio3));
 *      pio3set = NETWIB_TRUE;
 *
 *     netwib_gotolabel :
 *      if (pio1set) { netwib_er(netwib_io_close(&pio1)); }
 *      if (pio2set) { netwib_er(netwib_io_close(&pio2)); }
 *      if (pio3set) { netwib_er(netwib_io_close(&pio3)); }
 *      return(ret);
 *    }
 *           As seen in this simple example, program flow and  *
 *           error flow are separated.                         *
 ***************************************************************/

/* if (r != NETWIB_ERR_OK) return(r) */
#define netwib_er(r) {netwib_err netwib_coderr=r;if(netwib_coderr!=NETWIB_ERR_OK)return(netwib_coderr);}

/* the label */
#define netwib_gotolabel netwibleavefunction

/* goto label and return r (note : this uses a variable named "ret") */
#define netwib_goto(r) {ret = r; goto netwibleavefunction;}

/* if (r != NETWIB_ERR_OK) { goto label and return r } */
#define netwib_eg(r) {netwib_err netwib_coderr=r;if(netwib_coderr!=NETWIB_ERR_OK)netwib_goto(netwib_coderr);}



