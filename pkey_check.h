#ifndef EVP_CHECK_H_INCLUDED
#define EVP_CHECK_H_INCLUDED

#include <openssl/evp.h>

#define ERROR_CHECK_ADDRESS ERROR_FUNCTION_ADDRESS(21)

int checkRsaKeyPair(EVP_PKEY * key_pair);

int checkDhPubKey(EVP_PKEY * params, EVP_PKEY * pub_key);

int checkDhParams(EVP_PKEY * params);

int checkEccKeyPair(EVP_PKEY * key_pair);

int checkKey(EVP_PKEY * pairKey, ENGINE * engine);

int checkPubKey(EVP_PKEY * pubKey, ENGINE * engine);

int checkParams(EVP_PKEY * params, ENGINE * engine);

#endif // EVP_CHECK_H_INCLUDED
