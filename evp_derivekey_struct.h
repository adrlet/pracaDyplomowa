#ifndef EVP_DERIVEKEY_STRUCT_H_INCLUDED
#define EVP_DERIVEKEY_STRUCT_H_INCLUDED

#include <openssl/evp.h>

#define SAFE_ITERATIONS 1024
#define SALT_LEN 8

#define ERROR_DERIVE_KEY_STRUCT_ADDRESS ERROR_FUNCTION_ADDRESS(2)

struct deriveData
{
    int key_len; // 4
    int iv_len; // 8
    int salt_len; // 12
    int iterations; // 16
    const EVP_MD * algorithm; // 24
    unsigned char * salt; // 32
};

struct deriveData * setupDerive();

void freeDerive(struct deriveData * deriveParamters);

int provideDeriveAlgorithm(struct deriveData * deriveParamters, const EVP_MD * algorithm);

int provideDeriveCipherInfo(struct deriveData * deriveParameters, int key_len, int iv_len);

int provideDeriveIterations(struct deriveData * deriveParameters, int iterations);

int generateDeriveSalt(struct deriveData * deriveParameters, int salt_len);

#endif // EVP_DERIVEKEY_STRUCT_H_INCLUDED
