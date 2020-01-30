/*
                                  NETWOX
                             Network toolbox
                Copyright(c) 1999-2012 Laurent CONSTANTIN
                      http://ntwox.sourceforge.net/
                        laurentconstantin@free.fr
                                  -----

  This file is part of Netwox.

  Netwox is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License version 3
  as published by the Free Software Foundation.

  Netwox is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details (http://www.gnu.org/).

------------------------------------------------------------------------
*/

/*-------------------------------------------------------------*/
#include "../../netwox.h"

/*-------------------------------------------------------------*/
netwib_err netwox_hmac_keys_init(netwox_cryptohash_type hashtype,
                                 netwib_constbuf *pkey,
                                 netwox_hmac_keys *pkeys)
{
  netwib_byte keyblockarray[NETWOX_CRYPTOHASH_BLOCK_MAXLEN];
  netwib_data data, data2;
  netwib_uint32 datasize, blocksize, hashsize;

  pkeys->hashtype = hashtype;

  netwib_er(netwox_cryptohash_size(hashtype, &blocksize, &hashsize));
  if (blocksize > NETWOX_CRYPTOHASH_BLOCK_MAXLEN) {
    /* because ki and ko have this size */
    return(NETWOX_ERR_INTERNALERROR);
  }
  pkeys->blocksize = blocksize;

  datasize = netwib__buf_ref_data_size(pkey);
  if (datasize <= blocksize) {
    netwib_c_memcpy(keyblockarray, netwib__buf_ref_data_ptr(pkey), datasize);
    netwib_c_memset(keyblockarray+datasize, 0, blocksize-datasize);
  } else {
    netwib_buf keyblock;
    netwib_er(netwib_buf_init_ext_arrayempty(keyblockarray, blocksize,
                                             &keyblock));
    netwib_er(netwox_cryptohash_compute(hashtype, pkey, &keyblock));
    netwib_c_memset(keyblockarray+hashsize, 0, blocksize-hashsize);
  }

  data = pkeys->ki;
  data2 = keyblockarray;
  datasize = blocksize;
  while(datasize--) {
    *data++ = (netwib_byte)((*data2++) ^ 0x36);
  }

  data = pkeys->ko;
  data2 = keyblockarray;
  datasize = blocksize;
  while(datasize--) {
    *data++ = (netwib_byte)((*data2++) ^ 0x5C);
  }

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
netwib_err netwox_hmac_compute(netwox_hmac_keys *pkeys,
                               netwib_constbuf *pbuf,
                               netwib_buf *phmac)
{
  netwib_byte array[NETWOX_CRYPTOHASH_HASH_MAXLEN];
  netwib_buf buf, key;
  netwox_cryptohash_context ctx;

  netwib_er(netwox_cryptohash_init(pkeys->hashtype, &ctx));
  netwib_er(netwib_buf_init_ext_arrayfilled(pkeys->ki, pkeys->blocksize,
                                            &key));
  netwib_er(netwox_cryptohash_update(&ctx, &key));
  netwib_er(netwox_cryptohash_update(&ctx, pbuf));
  netwib_er(netwib_buf_init_ext_arraysizeofempty(array, &buf));
  netwib_er(netwox_cryptohash_final(&ctx, &buf));

  netwib_er(netwox_cryptohash_init(pkeys->hashtype, &ctx));
  netwib_er(netwib_buf_init_ext_arrayfilled(pkeys->ko, pkeys->blocksize,
                                            &key));
  netwib_er(netwox_cryptohash_update(&ctx, &key));
  netwib_er(netwox_cryptohash_update(&ctx, &buf));
  netwib_er(netwox_cryptohash_final(&ctx, phmac));

  return(NETWIB_ERR_OK);
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

/*-------------------------------------------------------------*/
static netwib_err netwox_priv_hmac_test(netwox_cryptohash_type hashtype,
                                        netwib_conststring mixedkey,
                                        netwib_conststring mixeddata,
                                        netwib_conststring hexawanted)
{
  netwib_buf key, data, hmac;
  netwox_hmac_keys keys;

  netwib_er(netwib_buf_init_mallocdefault(&key));
  netwib_er(netwox_buf_append_strmixed(mixedkey, &key));
  netwib_er(netwox_hmac_keys_init(hashtype, &key, &keys));

  netwib_er(netwib_buf_init_mallocdefault(&data));
  netwib_er(netwox_buf_append_strmixed(mixeddata, &data));

  netwib_er(netwib_buf_init_mallocdefault(&hmac));
  netwib_er(netwox_hmac_compute(&keys, &data, &hmac));
  netwib_er(netwox_buf_check_strmixed(&hmac, hexawanted));

  netwib_er(netwib_buf_close(&hmac));
  netwib_er(netwib_buf_close(&data));
  netwib_er(netwib_buf_close(&key));

  return(NETWIB_ERR_OK);
}
#define netwox_priv_hmac_test_md4(key,str,strwanted) netwox_priv_hmac_test(NETWOX_CRYPTOHASH_TYPE_MD4,key,str,strwanted)
#define netwox_priv_hmac_test_md5(key,str,strwanted) netwox_priv_hmac_test(NETWOX_CRYPTOHASH_TYPE_MD5,key,str,strwanted)
#define netwox_priv_hmac_test_ripemd128(key,str,strwanted) netwox_priv_hmac_test(NETWOX_CRYPTOHASH_TYPE_RIPEMD128,key,str,strwanted)
#define netwox_priv_hmac_test_ripemd160(key,str,strwanted) netwox_priv_hmac_test(NETWOX_CRYPTOHASH_TYPE_RIPEMD160,key,str,strwanted)
#define netwox_priv_hmac_test_sha1(key,str,strwanted) netwox_priv_hmac_test(NETWOX_CRYPTOHASH_TYPE_SHA1,key,str,strwanted)
#define netwox_priv_hmac_test_sha256(key,str,strwanted) netwox_priv_hmac_test(NETWOX_CRYPTOHASH_TYPE_SHA256,key,str,strwanted)

/*-------------------------------------------------------------*/
netwib_err netwox_hmac_test(void)
{

  /* rfc 2104 test suite */
  netwib_er(netwox_priv_hmac_test_md5("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b", "'Hi There'", "9294727a3638bb1c13f48ef8158bfc9d"));
  netwib_er(netwox_priv_hmac_test_md5("'Jefe'", "'what do ya want for nothing?'", "750c783e6ab0b503eaa86e310a5db738"));
  netwib_er(netwox_priv_hmac_test_md5("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD", "56be34521d144c88dbb8c733f0e8b3f6"));

  /* rfc 2202 test suite */
  netwib_er(netwox_priv_hmac_test_md5("0102030405060708090a0b0c0d0e0f10111213141516171819", "cdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcd", "697eaf0aca3a3aea3a75164746ffaa79"));
  netwib_er(netwox_priv_hmac_test_md5("0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c", "'Test With Truncation'", "56461ef2342edc00f9bab995690efd4c"));
  netwib_er(netwox_priv_hmac_test_md5("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "'Test Using Larger Than Block-Size Key - Hash Key First'", "6b1ab7fe4bd7bf8f0b62e6ce61b9d0cd"));
  netwib_er(netwox_priv_hmac_test_md5("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "'Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data'", "6f630fad67cda0ee1fb1f562db3aa53e"));
  netwib_er(netwox_priv_hmac_test_sha1("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b", "'Hi There'", "b617318655057264e28bc0b6fb378c8ef146be00"));
  netwib_er(netwox_priv_hmac_test_sha1("'Jefe'", "'what do ya want for nothing?'", "effcdf6ae5eb2fa2d27416d5f184df9c259a7c79"));
  netwib_er(netwox_priv_hmac_test_sha1("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD", "125d7342b9ac11cd91a39af48aa17b4f63f175d3"));
  netwib_er(netwox_priv_hmac_test_sha1("0102030405060708090a0b0c0d0e0f10111213141516171819", "cdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcd", "4c9007f4026250c6bc8414f9bf50c86c2d7235da"));
  netwib_er(netwox_priv_hmac_test_sha1("0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c", "'Test With Truncation'", "4c1a03424b55e07fe7f27be1d58bb9324a9a5a04"));
  netwib_er(netwox_priv_hmac_test_sha1("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "'Test Using Larger Than Block-Size Key - Hash Key First'", "aa4ae5e15272d00e95705637ce8a3b55ed402112"));
  netwib_er(netwox_priv_hmac_test_sha1("aaaaaaaaaaaaaaaaaaaa aaaaaaaaaaaaaaaaaaaa aaaaaaaaaaaaaaaaaaaa aaaaaaaaaaaaaaaaaaaa aaaaaaaaaaaaaaaaaaaa aaaaaaaaaaaaaaaaaaaa aaaaaaaaaaaaaaaaaaaa aaaaaaaaaaaaaaaaaaaa", "'Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data'", "e8e99d0f45237d786d6bbaa7965c7808bbff1a91"));
  netwib_er(netwox_priv_hmac_test_sha1("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "'Test Using Larger Than Block-Size Key - Hash Key First'", "aa4ae5e15272d00e95705637ce8a3b55ed402112"));
  netwib_er(netwox_priv_hmac_test_sha1("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "'Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data'", "e8e99d0f45237d786d6bbaa7965c7808bbff1a91"));

  /* draft-ietf-ipsec-ciph-sha-256-01.txt test suite */
  netwib_er(netwox_priv_hmac_test_sha256("0102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f2000", "'abc'", "a21b1f5d4cf4f73a4dd939750f7a066a7f98cc131cb16a6692759021cfab8181"));
  netwib_er(netwox_priv_hmac_test_sha256("0102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f2000", "'abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq'", "104fdc1257328f08184ba73131c53caee698e36119421149ea8c712456697d30"));
  netwib_er(netwox_priv_hmac_test_sha256("'Jefe'", "'what do ya want for nothing?'", "5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec3843"));
  netwib_er(netwox_priv_hmac_test_sha256("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "'Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data'", "6355ac22e890d0a3c8481a5ca4825bc884d3e7a1ff98a2fc2ac7d8e064c3b2e6"));

  /* test suite from http://www.esat.kuleuven.ac.be/~bosselae/ripemd160.html */
  netwib_er(netwox_priv_hmac_test_ripemd128("00112233445566778899aabbccddeeff", "''", "ad9db2c1e22af9ab5ca9dbe5a86f67dc"));
  netwib_er(netwox_priv_hmac_test_ripemd128("00112233445566778899aabbccddeeff", "'a'", "3bf448c762de00bcfa0310b11c0bde4c"));
  netwib_er(netwox_priv_hmac_test_ripemd128("00112233445566778899aabbccddeeff", "'abc'", "f34ec0945f02b70b8603f89e1ce4c78c"));
  netwib_er(netwox_priv_hmac_test_ripemd128("00112233445566778899aabbccddeeff", "'message digest'", "e8503a8aec2289d82aa0d8d445a06bdd"));
  netwib_er(netwox_priv_hmac_test_ripemd128("00112233445566778899aabbccddeeff", "'abcdefghijklmnopqrstuvwxyz'", "ee880b735ce3126065de1699cc136199"));
  netwib_er(netwox_priv_hmac_test_ripemd128("00112233445566778899aabbccddeeff", "'abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq'", "794daf2e3bdeea2538638a5ced154434"));
  netwib_er(netwox_priv_hmac_test_ripemd128("00112233445566778899aabbccddeeff", "'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789'", "3a06eef165b23625247800be23e232b6"));
  netwib_er(netwox_priv_hmac_test_ripemd128("00112233445566778899aabbccddeeff", "'12345678901234567890123456789012345678901234567890123456789012345678901234567890'", "9a4f0159c0952da43a8d466d46b0af58"));
  /**/
  netwib_er(netwox_priv_hmac_test_ripemd160("00112233445566778899aabbccddeeff01234567", "''", "cf387677bfda8483e63b57e06c3b5ecd8b7fc055"));
  netwib_er(netwox_priv_hmac_test_ripemd160("00112233445566778899aabbccddeeff01234567", "'a'", "0d351d71b78e36dbb7391c810a0d2b6240ddbafc"));
  netwib_er(netwox_priv_hmac_test_ripemd160("00112233445566778899aabbccddeeff01234567", "'abc'", "f7ef288cb1bbcc6160d76507e0a3bbf712fb67d6"));
  netwib_er(netwox_priv_hmac_test_ripemd160("00112233445566778899aabbccddeeff01234567", "'message digest'", "f83662cc8d339c227e600fcd636c57d2571b1c34"));
  netwib_er(netwox_priv_hmac_test_ripemd160("00112233445566778899aabbccddeeff01234567", "'abcdefghijklmnopqrstuvwxyz'", "843d1c4eb880ac8ac0c9c95696507957d0155ddb"));
  netwib_er(netwox_priv_hmac_test_ripemd160("00112233445566778899aabbccddeeff01234567", "'abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq'", "60f5ef198a2dd5745545c1f0c47aa3fb5776f881"));
  netwib_er(netwox_priv_hmac_test_ripemd160("00112233445566778899aabbccddeeff01234567", "'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789'", "e49c136a9e5627e0681b808a3b97e6a6e661ae79"));
  netwib_er(netwox_priv_hmac_test_ripemd160("00112233445566778899aabbccddeeff01234567", "'12345678901234567890123456789012345678901234567890123456789012345678901234567890'", "31be3cc98cee37b79b0619e3e1c2be4f1aa56e6c"));
  /**/
  netwib_er(netwox_priv_hmac_test_ripemd128("0123456789abcdeffedcba9876543210", "''", "8931eeee56a6b257fd1ab5418183d826"));
  netwib_er(netwox_priv_hmac_test_ripemd128("0123456789abcdeffedcba9876543210", "'a'", "dbbcf169ea7419d5ba7bd8eb3673ff2d"));
  netwib_er(netwox_priv_hmac_test_ripemd128("0123456789abcdeffedcba9876543210", "'abc'", "2c4cd07d3162d6a0e338004d6b6fbc9a"));
  netwib_er(netwox_priv_hmac_test_ripemd128("0123456789abcdeffedcba9876543210", "'message digest'", "75bfb25888f4bb77c77ae83ad0817447"));
  netwib_er(netwox_priv_hmac_test_ripemd128("0123456789abcdeffedcba9876543210", "'abcdefghijklmnopqrstuvwxyz'", "b1b5dc0fcb7258758855dd1840fcdce4"));
  netwib_er(netwox_priv_hmac_test_ripemd128("0123456789abcdeffedcba9876543210", "'abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq'", "670d0f7a697b18f1a8ab7d2a2a00dbc1"));
  netwib_er(netwox_priv_hmac_test_ripemd128("0123456789abcdeffedcba9876543210", "'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789'", "54e315fdb34a61c0475392e5c7852998"));
  netwib_er(netwox_priv_hmac_test_ripemd128("0123456789abcdeffedcba9876543210", "'12345678901234567890123456789012345678901234567890123456789012345678901234567890'", "ad04354d8aa2a623e72e3594ee3535c0"));
  /**/
  netwib_er(netwox_priv_hmac_test_ripemd160("0123456789abcdeffedcba987654321000112233", "''", "fe69a66c7423eea9c8fa2eff8d9dafb4f17a62f5"));
  netwib_er(netwox_priv_hmac_test_ripemd160("0123456789abcdeffedcba987654321000112233", "'a'", "85743e899bc82dbfa36faaa7a25b7cfd372432cd"));
  netwib_er(netwox_priv_hmac_test_ripemd160("0123456789abcdeffedcba987654321000112233", "'abc'", "6e4afd501fa6b4a1823ca3b10bd9aa0ba97ba182"));
  netwib_er(netwox_priv_hmac_test_ripemd160("0123456789abcdeffedcba987654321000112233", "'message digest'", "2e066e624badb76a184c8f90fba053330e650e92"));
  netwib_er(netwox_priv_hmac_test_ripemd160("0123456789abcdeffedcba987654321000112233", "'abcdefghijklmnopqrstuvwxyz'", "07e942aa4e3cd7c04dedc1d46e2e8cc4c741b3d9"));
  netwib_er(netwox_priv_hmac_test_ripemd160("0123456789abcdeffedcba987654321000112233", "'abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq'", "b6582318ddcfb67a53a67d676b8ad869aded629a"));
  netwib_er(netwox_priv_hmac_test_ripemd160("0123456789abcdeffedcba987654321000112233", "'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789'", "f1be3ee877703140d34f97ea1ab3a07c141333e2"));
  netwib_er(netwox_priv_hmac_test_ripemd160("0123456789abcdeffedcba987654321000112233", "'12345678901234567890123456789012345678901234567890123456789012345678901234567890'", "85f164703e61a63131be7e45958e0794123904f9"));

  /* these test cases were defined by me */
  netwib_er(netwox_priv_hmac_test_md4("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b", "'Hi There'", "90a79458f58f437e21f169cdba283da6"));
  netwib_er(netwox_priv_hmac_test_md4("'Jefe'", "'what do ya want for nothing?'", "be192c588a8e914d8a59b474a828128f"));
  netwib_er(netwox_priv_hmac_test_md4("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD", "75e5fb6e71ca6dcdd9fca269a9a3cd9c"));

  return(NETWIB_ERR_OK);
}
