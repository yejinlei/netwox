
/*-------------------------------------------------------------*/
/*
  Random generation uses a Linear Congruential Generator, with
  values proposed by Knuth (The Art of Computer Programming book).

    X[n+1] = (6364136223846793005 *X[n] + 1) % (2^64)
           = (0x5851F42D4C957F2D  *X[n] + 1) % (2^64)

  Note: we don't bother about overflow because modulo is 2^64,
        which is the size of integer

  However, low byte (8 bits) is not very random (drawback of LCGs).
  So, we only extract 4 top bytes (uint32), or 6 top bytes (randarray).
*/

/*-------------------------------------------------------------*/
/* called only on start */
netwib_err netwib_priv_rand_seed(netwib_uint64 *pseed);

/*-------------------------------------------------------------*/
/* generate random fields */
#define NETWIB_PRIV_RAND_ARRAY_LEN 6
netwib_err netwib_priv_rand_gene(netwib_uint32 *prand32,
                                 netwib_data randarray);

