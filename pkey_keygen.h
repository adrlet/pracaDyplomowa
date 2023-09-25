#ifndef PKEY_KEYGEN_H_INCLUDED
#define PKEY_KEYGEN_H_INCLUDED

#include <openssl/evp.h>

#define ERROR_KEY_GEN_ADDRESS ERROR_FUNCTION_ADDRESS(15)

int generateRsaKeys(int count, int bits, EVP_PKEY ** keys, ENGINE * engine);

int generateKeysOnParams(int count, EVP_PKEY * parameters, EVP_PKEY ** keys, ENGINE * engine);

int generateHmac(EVP_PKEY ** pkeys, int count, EVP_MD * algorithm, ENGINE * engine);

#endif // PKEY_KEYGEN_H_INCLUDED
