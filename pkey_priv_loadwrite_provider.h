#ifndef PKEY_PRIV_LOADWRITE_PROVIDER_H_INCLUDED
#define PKEY_PRIV_LOADWRITE_PROVIDER_H_INCLUDED

#include <openssl/evp.h>

#define ERROR_PRIV_LOADWRITE_PROVIDER_ADDRESS ERROR_FUNCTION_ADDRESS(33)

struct privPassword
{
    const EVP_CIPHER * algorithm; // 8
    char * phrase; // 16
    int (*pass_provider)(char *, int, int, void*); // 24
};

struct privPassword * setupProvider();

void freeProvider(struct privPassword * passwordProvider);

int provideProviderPassword(struct privPassword * passwordProvider, const char * password, int (*pass_provider)(char *, int, int, void*));

int provideProviderAlgorithm(struct privPassword * passwordProvider, const EVP_CIPHER * algorithm);

#endif // PKEY_PRIV_LOADWRITE_PROVIDER_H_INCLUDED
