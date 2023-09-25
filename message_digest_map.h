#ifndef MESSAGE_DIGEST_MAP_H_INCLUDED
#define MESSAGE_DIGEST_MAP_H_INCLUDED

#include <openssl/evp.h>

#define MESSAGE_DIGEST_ALGORITHMS 3

#define EVP_SHA384 0
#define EVP_SHA512_224 1
#define EVP_SHA512_256 2

const EVP_MD * get_digest_algorithm(int num);

#endif // MESSAGE_DIGEST_MAP_H_INCLUDED
