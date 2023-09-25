#ifndef PKEY_KEY_AGREMEENT_STRUCT_H_INCLUDED
#define PKEY_KEY_AGREMEENT_STRUCT_H_INCLUDED

#include <openssl/evp.h>

#define ERROR_KEY_AGREMEENT_STRUCT_ADDRESS ERROR_FUNCTION_ADDRESS(28)

struct dhData
{
    int secret_len; // 4
    EVP_PKEY * key_pair; // 16
    EVP_PKEY * peer_public_key; // 24
    unsigned char * secret; // 32
};

struct dhData * setupDh();

void freeDh(struct dhData * dhParameters);

int provideDhKeys(EVP_PKEY * key_pair, EVP_PKEY * peer_public_key, struct dhData * dhParameters);

#endif // PKEY_KEY_AGREMEENT_STRUCT_H_INCLUDED
