
/*-------------------------------------------------------------*/
typedef const netwib_char* netwox_passwordgeneword;

/*-------------------------------------------------------------*/
netwib_err netwox_passwordgeneword_count(netwox_passwordgeneword words[],
                                         netwib_uint32 *pcount);

/*-------------------------------------------------------------*/
netwib_err netwox_passwordgeneword_choose(netwox_passwordgeneword words[],
                                          netwib_uint32 count,
                                          netwib_buf *pbuf);

/*-------------------------------------------------------------*/
netwib_err netwox_passwordgeneword_findsize(netwox_passwordgeneword words[],
                                            netwib_data data,
                                            netwib_uint32 datasize,
                                            netwib_uint32 *pfoundsize);

/*-------------------------------------------------------------*/
netwib_err netwox_passwordgene(netwib_uint32 numpassword,
                               netwib_uint32 numimages,
                               netwib_bool pronounceable,
                               netwib_uint32 maxsyllables,
                               netwox_passwordgeneword words[],
                               netwib_constbuf *pallowedcharvar,
                               netwox_passwordgeneword vowel[],
                               netwox_passwordgeneword consonantbegin[],
                               netwox_passwordgeneword consonantend[]);
