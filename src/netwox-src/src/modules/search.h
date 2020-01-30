
/*-------------------------------------------------------------*/
/* exact string search */
netwib_err netwox_search_exact(netwib_constbuf *ptext,
                               netwib_constbuf *ppattern, /* to found */
                               netwib_bool *pfound);

/*-------------------------------------------------------------*/
/* root string search:
    50% of "spooff" searches "spo"
    40% of "spoof" searches "sp"
*/
netwib_err netwox_search_root(netwib_constbuf *ptext,
                              netwib_constbuf *ppattern, /* to found */
                              netwib_uint32 percentage,
                              netwib_bool *pfound);

/*-------------------------------------------------------------*/
/*
  This algorithm is a basic Approximate String Matching algorithm:
    The text T has a size of t characters.
    The pattern P has a size of p characters.
    We create an array A of t+1 columns and p+1 rows.
    The formula to compute fields of this array is :
      for i in 0..t+1 and for j in 0..p+1 [
        A[i, 0] = 0 (or i, for exact matching of P and T,
                     but not used here)
        A[0, j] = i
        if (T[i] == P[j]) [
          A[i, j] = A[i-1, j-1]
        ] else [
          A[i, j] = 1 + min(A[i-1,j], A[i,j-1], A[i-1,j-1])
        ]
      ]
      The number of errors is min(A[i, p]).

  For example, T="With_the_hat_on" and P="that".
        W i t h _ t h e _ h a t _ o n
      0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    t 1 1 1 0 1 1 0 1 1 1 1 1 0 1 1 1
    h 2 2 2 1 0 1 1 0 2 2 1 2 1 1 2 2
    a 3 3 3 2 1 1 2 1 1 2 2 1 2 2 2 3
    t 4 4 4 3 2 2 1 2 2 2 3 2 1 3 3 3
  So, the best matches (only one error) are:
            t h _ t     _ h a t

  We can note that there is no need to parse the full line: we
  can go from left to right (using only 2 columns) and stop when
  the last row is <= expected error number.

  Parameter percentage is the maximum percentage of expected error
  number over length of pattern. If 0, an exact match is done.
  A good value seems to be 25.
*/
/*
  This algorithm can give strange results: searching "ping"
  returns "havING", "comING", etc.
*/
netwib_err netwox_search_approx(netwib_constbuf *ptext,
                                netwib_constbuf *ppattern, /* to found */
                                netwib_uint32 percentage,
                                netwib_bool *pfound);
