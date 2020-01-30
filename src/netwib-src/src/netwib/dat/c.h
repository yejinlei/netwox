
/*-------------------------------------------------------------*/
/* do not forget to add "#include <string.h>" at the beginning of
   the file to use those defines */
/* copies n bytes from memory area src to memory area dest (no overlap) */
#define netwib_c_memcpy(dest,src,n) memcpy(dest,src,n)
/* copies n bytes from memory area src to memory area dest */
#define netwib_c_memmove(dest,src,n) memmove(dest,src,n)
/* fills the first n bytes of the memory area pointed to by s with
   the constant byte c */
#define netwib_c_memset(d,c,n) memset(d,c,n)
/* compares the first n bytes of the memory areas s1 and s2 */
#define netwib_c_memcmp(s1,s2,n) memcmp(s1,s2,n)
/* scans the first n bytes of the memory area pointed to by s for
   the character c */
#define netwib_c_memchr(s,c,n) memchr(s,c,n)

/*-------------------------------------------------------------*/
/* do not forget to add "#include <string.h>" at the beginning of
   the file to use those defines */
/* compares the two strings s1 and s2 */
#define netwib_c_strcmp(s1,s2) strcmp(s1,s2)
/* compares the first n characters of strings s1 and s2 */
#define netwib_c_strncmp(s1,s2,n) strncmp(s1,s2,n)
/* returns a pointer to the first occurrence of the character c in
   the string s */
#define netwib_c_strchr(s,c) strchr(s,c)
/* returns a pointer to the last occurrence of the character c in
   the string s */
#define netwib_c_strrchr(s,c) strrchr(s,c)
/* finds the first occurrence of the needle in the string haystack */
#define netwib_c_strstr(haystack,needle) strstr(haystack,needle)
/* calculates the length of the initial segment of s which consists
   entirely of characters in accept */
#define netwib_c_strspn(s,accept) strcspn(s,accept)
/* calculates the length of the initial segment of s which consists
   entirely of characters not in reject */
#define netwib_c_strcspn(s,reject) strcspn(s,reject)
/* locates the first occurrence in the string s of any of the
   characters in the string accept */
#define netwib_c_strpbrk(s,accept) strpbrk(s,accept)
/* calculates the length of the string s, not including the
   terminating `\0' character */
#define netwib_c_strlen(s) strlen(s)
/* copies the string pointed to be src to the array pointed
   to by dest (you need to take care about overflows) */
#define netwib_c_strcpy(dest,src) strcpy(dest,src)
/* append the string pointed to be src to the array pointed
   to by dest (you need to take care about overflows) */
#define netwib_c_strcat(dest,src) strcat(dest,src)

/*-------------------------------------------------------------*/
/* we create those functions because they might not exists on
   every system */
/* compares the two strings s1 and s2, ignoring the case of the
   characters */
int netwib_c_strcasecmp(netwib_conststring s1,
                        netwib_conststring s2);
/* compares the first n characters of strings s1 and s2, ignoring
   the case of the characters */
int netwib_c_strncasecmp(netwib_conststring s1,
                         netwib_conststring s2,
                         netwib_uint32 n);
/* compares the first n bytes of the memory areas s1 and s2,
   ignoring the case of the characters */
int netwib_c_memcasecmp(netwib_constdata s1,
                        netwib_constdata s2,
                        netwib_uint32 n);
/* finds the first occurrence of the needle in the string haystack,
   ignoring the case of the characters */
netwib_string netwib_c_strcasestr(netwib_conststring haystack,
                                  netwib_conststring needle);
/* finds the start of the first occurrence of the substring needle
   of length needlelen in the memory area haystack of length haystacklen */
netwib_data netwib_c_memmem(netwib_constdata haystack,
                            netwib_uint32 haystacklen,
                            netwib_constdata needle,
                            netwib_uint32 needlelen);
/* idem, with case */
netwib_data netwib_c_memcasemem(netwib_constdata haystack,
                                netwib_uint32 haystacklen,
                                netwib_constdata needle,
                                netwib_uint32 needlelen);
