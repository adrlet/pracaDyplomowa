#include "message_digest_map.h"

#include <openssl/evp.h>

// https://en.wikipedia.org/wiki/Template:Comparison_of_SHA_functions
const EVP_MD * get_digest_algorithm(int num)
{
    switch(num)
    {
    case EVP_SHA384:
        return EVP_sha384();
    case EVP_SHA512_224:
        return EVP_sha512_224();
    case EVP_SHA512_256:
        return EVP_sha512_256();
    default:
        return NULL;
    }
}
