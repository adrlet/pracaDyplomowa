#ifndef PKEY_PRIV_LOADWRITE_H_INCLUDED
#define PKEY_PRIV_LOADWRITE_H_INCLUDED

#include <openssl/evp.h>
#include <openssl/bio.h>

#include "pkey_priv_loadwrite_provider.h"

#define ERROR_PRIV_LOADWRITE_ADDRESS ERROR_FUNCTION_ADDRESS(32)

int extractRsaPrivateKey(EVP_PKEY * priv_key, BIO * output, struct privPassword * provider);

EVP_PKEY * importRsaPrivateKey(BIO * input, EVP_PKEY ** priv_key, struct privPassword * provider);

#endif // PKEY_PRIV_LOADWRITE_H_INCLUDED
