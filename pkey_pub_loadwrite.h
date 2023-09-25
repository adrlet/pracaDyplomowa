#ifndef PKEY_PUB_LOADWRITE_H_INCLUDED
#define PKEY_PUB_LOADWRITE_H_INCLUDED

#include <openssl/evp.h>
#include <openssl/bio.h>

#define ERROR_PUB_LOADWRITE_ADDRESS ERROR_FUNCTION_ADDRESS(34)

int extractRsaPublicKey(EVP_PKEY * pub_key, BIO * output);

EVP_PKEY * importRsaPublicKey(BIO * input, EVP_PKEY ** pub_key);

#endif // PKEY_PUB_LOADWRITE_H_INCLUDED
