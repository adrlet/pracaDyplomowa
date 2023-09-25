#ifndef PKEY_SIGN_STRUCT_H_INCLUDED
#define PKEY_SIGN_STRUCT_H_INCLUDED

#include <openssl/evp.h>

#define ERROR_SIGN_STRUCT_ADDRESS ERROR_FUNCTION_ADDRESS(36)

struct signData
{
    int digest_size; // 4
    EVP_PKEY * key_pair; // 16
    EVP_MD * algorithm; // 24
};

struct signData * setupSign();

void freeSign(struct signData * signParameters);

int provideSignDigest(EVP_MD * algorithm, struct signData * signParameters);

int provideSignKeys(EVP_PKEY * pair, struct signData * signParameters);



#endif // PKEY_SIGN_STRUCT_H_INCLUDED
