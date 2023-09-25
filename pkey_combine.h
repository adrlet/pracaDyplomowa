#ifndef PKEY_COMBINE_H_INCLUDED
#define PKEY_COMBINE_H_INCLUDED

#include <openssl/evp.h>

#define ERROR_KEY_COMBINE_ADDRESS ERROR_FUNCTION_ADDRESS(22)

int combineKeys(EVP_PKEY * public_key, EVP_PKEY * private_key, EVP_PKEY ** key_pair);

#endif // PKEY_COMBINE_H_INCLUDED
