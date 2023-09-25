#include <openssl/pem.h>
#include <string.h>

#include "pkey_priv_loadwrite_provider.h"
#include "bio_errorhandle.h"

#define writeErrorCode_full(type, code) writeErrorCode(ERROR_FULL_ADDRESS(ERROR_PRIV_LOADWRITE_PROVIDER_ADDRESS, type, code))

struct privPassword * setupProvider()
{
    struct privPassword * passwordProvider;

    passwordProvider = (struct privPassword*)OPENSSL_malloc(sizeof(struct privPassword));
    if(passwordProvider == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 0);
        return NULL;
    }

    passwordProvider->algorithm = NULL;
    passwordProvider->phrase = NULL;
    passwordProvider->pass_provider = NULL;

    return passwordProvider;
};

void freeProvider(struct privPassword * passwordProvider)
{
    if(passwordProvider == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 0);
        return;
    }

    passwordProvider->algorithm = NULL;
    passwordProvider->pass_provider = NULL;
    if(passwordProvider->phrase != NULL)
        OPENSSL_clear_free(passwordProvider->phrase, strlen(passwordProvider->phrase));
    passwordProvider->phrase = NULL;
}

int provideProviderPassword(struct privPassword * passwordProvider, const char * password, int (*pass_provider)(char *, int, int, void*))
{
    int iResult;

    if(passwordProvider == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 1);
        goto error;
    }

    if(password == NULL && pass_provider == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 2);
        goto error;
    }

    if(password != NULL)
    {
        passwordProvider->phrase = (char*)OPENSSL_malloc(sizeof(char)*(strlen(password)+1));
        if(passwordProvider->phrase == NULL)
        {
            writeErrorCode_full(ERROR_TYPE_MEMORY_ALLOCATION, 1);
            goto error;
        }
        strcpy(passwordProvider->phrase, password);
    }

    if(pass_provider != NULL)
        passwordProvider->pass_provider = pass_provider;

    iResult = 0;
    goto provideProviderPassword_end;

error:
    iResult = -1;

provideProviderPassword_end:
    return iResult;
}

int provideProviderAlgorithm(struct privPassword * passwordProvider, const EVP_CIPHER * algorithm)
{
    int iResult;

    if(passwordProvider == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 3);
        goto error;
    }

    if(algorithm == NULL)
    {
        writeErrorCode_full(ERROR_TYPE_INPUT_NULL, 4);
        goto error;
    }

    passwordProvider->algorithm = algorithm;

    iResult = 0;
    goto provideProviderAlgorithm_end;

error:
    iResult = -1;

provideProviderAlgorithm_end:
    return iResult;
}
